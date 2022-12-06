#include <string.h>
#include <postgres.h>
#include <fmgr.h>
#include <utils/builtins.h>
#include <libpq-fe.h>


typedef struct URL {
#pragma clang diagnostic push
#pragma ide diagnostic ignored "OCUnusedGlobalDeclarationInspection"
    int32_t length; /// DO NOT REMOVE, used internally
#pragma clang diagnostic pop

    u_int8_t scheme;
    u_int8_t host;
    u_int8_t path;
    u_int8_t query;
    u_int8_t user;
    u_int8_t port;
    u_int8_t fragment;
    char url[FLEXIBLE_ARRAY_MEMBER];
} URL;

Datum mallocAndMakeSlice(const char *start, int length) {
    PG_RETURN_TEXT_P(cstring_to_text_with_len(start, length));
}

Datum getHostFromUrl(const URL *url) {
    return mallocAndMakeSlice(url->url + url->scheme + url->user, url->host);
}

Datum getSchemeFromUrl(const URL *url) {
    return mallocAndMakeSlice(url->url, url->scheme);
}

Datum getPortFromUrl(const URL *url) {
    int delta = 0;
    if (url->port > 0) {
        delta = 1;
    }
    return mallocAndMakeSlice(url->url + url->scheme + url->user + url->host + delta,
                              url->port - delta); // +delta (and thus -delta for length) removes the ":"
}

Datum getRefFromUrl(const URL *url) {
    return mallocAndMakeSlice(url->url + url->scheme + url->user + url->host + url->port + url->path + url->query,
                              url->fragment);
}

Datum getQueryFromUrl(const URL *url) {
    return mallocAndMakeSlice(url->url + url->scheme + url->user + url->host + url->port + url->path, url->query);
}

Datum getPathFromUrl(const URL *url) {
    return mallocAndMakeSlice(url->url + url->scheme + url->user + url->host + url->port, url->path);
}

Datum getUsernameFromUrl(const URL *url) {
    char int_str[20];
    int delta = 0;
    if (url->user > 0) {
        delta = url->user - 1; // prevent inclusion of @
    }
    return mallocAndMakeSlice(url->url + url->scheme, delta);
}

URL *urlFromString(const char *source) {
    u_int8_t pointerSize[] = {0, 0, 0, 0, 0, 0, 0};

    int currentPointer = 0;

    char lastChar = '\0';

    int charInSource = 0;

    const char *sourceStart = source;

    while (*source) {

        if (currentPointer == 2 && *source == ':') {
            currentPointer = 3;
        }

        if ((currentPointer == 2 || currentPointer == 3) && *source == '/') {
            currentPointer = 4;
        }
        if ((currentPointer == 2 || currentPointer == 3 || currentPointer == 4) && *source == '?') {
            currentPointer = 5;
        }
        if ((currentPointer == 2 || currentPointer == 3 || currentPointer == 4 || currentPointer == 5) &&
            *source == '#') {
            currentPointer = 6;
        }

        pointerSize[currentPointer]++;

        if (*source == '@') {
            pointerSize[1] = pointerSize[2];
            pointerSize[2] = 0;
            currentPointer = 2;
        }

        if (currentPointer == 0 && lastChar == '/' && *source == '/') {
            currentPointer = 2;
        }

        lastChar = *source;
        charInSource++;
        source++;
    }


    if (pointerSize[2] == 0) { // no host part -> invalid url
        ereport(ERROR,
                (
                        errmsg("Invalid url format."),
                                errdetail("The '%s' string does not appear to follow the following pattern => scheme://[username@]hostname[:port][path][?query][#fragment] pattern. ([value] means the value is optional)", sourceStart),
                                errhint("Make sure you are entering a valid url. (not very helpful, we know...)")
                )
        );
    }

    int32 schemeStructSize = VARHDRSZ + charInSource + sizeof(u_int8_t) * lengthof(pointerSize);
    URL *url = (URL *) palloc(schemeStructSize);
    SET_VARSIZE(url, schemeStructSize);
    memcpy(url->url, sourceStart, charInSource);
    url->scheme = pointerSize[0];
    url->user = pointerSize[1];
    url->host = pointerSize[2];
    url->port = pointerSize[3];
    url->path = pointerSize[4];
    url->query = pointerSize[5];
    url->fragment = pointerSize[6];

    return url;
}






URL *fromStringWithContext(const URL *context, const char *source) {
    URL *spec = urlFromString(source);
    char scheme[10] = "";
    char host[1024] = "";
    char path[2048] = "";
    char query[2048] = "";
    char fragment[256] = "";
    char context_scheme[10] = "";
    char context_host[1024] = "";
    char context_path[2048] = "";
    char context_query[2048] = "";
    char context_fragment[256] = "";
    char new_scheme[10] = "";
    char new_host[1024] = "";
    char new_path[2048] = "";
    char new_query[2048] = "";
    char new_fragment[256] = "";

    // Initialize url spec
    strcpy(scheme, (const char *) getSchemeFromUrl(spec));
    strcpy(host, (const char *) getHostFromUrl(spec));
    strcpy(path, (const char *) getPathFromUrl(spec));
    strcpy(query, (const char *) getQueryFromUrl(spec));
    strcpy(fragment, (const char *) getRefFromUrl(spec));
    // Initialize context
    strcpy(context_scheme, (const char *) getSchemeFromUrl(context)) ;
    strcpy(context_host, (const char *) getHostFromUrl(context));
    strcpy(context_path, (const char *) getPathFromUrl(context));
    strcpy(context_query, (const char *) getQueryFromUrl(context));
    strcpy(context_fragment, (const char *) getRefFromUrl(context));
    // Initialize default new_url
    strcpy(new_scheme, (const char *) getSchemeFromUrl(context)) ;
    strcpy(new_host, (const char *) getHostFromUrl(context));
    strcpy(new_path, (const char *) getPathFromUrl(context));
    strcpy(new_query, (const char *) getQueryFromUrl(context));
    strcpy(new_fragment, (const char *) getRefFromUrl(context));

    // ref to current doc
    if ( strcmp(path,"") != 0 &&  strcmp(scheme,"") != 0
         && strcmp(host,"") != 0 && strcmp(query,"") != 0){
        strcpy(new_scheme, context_scheme) ;
        strcpy(new_host, context_host);
        strcpy(new_path, context_path);
        strcpy(new_query, context_query);
        strcpy(new_fragment, context_fragment);
    }  //Otherwise, the fragment and query parts present in the spec are used in the new URL
    else if ( strcmp(path,"") == 0 &&  strcmp(scheme,"") == 0
              && strcmp(host,"") == 0 && strcmp(query,"") == 0) {
        strcpy(new_scheme, context_scheme) ;
        strcpy(new_host, context_host);
        strcpy(new_path, context_path);
        strcpy(new_query, query);
        strcpy(new_fragment, fragment);
    } // the new URL is created as an absolute URL based on the spec alon
    else{
        if (strcmp(scheme, context_scheme) != 0) {
            strcpy(new_scheme, scheme) ;
            strcpy(new_host, host);
            strcpy(new_path, path);
            strcpy(new_query, query);
            strcpy(new_fragment, fragment);
        } //scheme component is inherited from the context URL
        if (!strcmp(scheme,context_scheme)) {
            strcpy(new_scheme, context_scheme) ;
        } //  the spec is treated as absolute and the spec host and path will replace the context host and path
        if (strcmp(host,"") != 0) {
            strcpy(new_host, host);
            strcpy(new_path, path);
        } // host component is absent in the spec then the host of the new URL will be inherited from the context
        if (strcmp(host,"") == 0) {
            strcpy(new_host, context_host);
            strcpy(new_path, context_path);
        } // path is treated as absolute and the spec path replaces the context path
        if (path[0] == '/') {
            strcpy(new_path, path);
        } // path is treated as a relative path and is appended to the context path
        if (path[0] != '/') {
            strcpy(new_path, context_scheme);
            strcat(new_path  , path);
        }
    }
    int32 schemeStructSize =lengthof(new_scheme) + lengthof(new_host) +lengthof(new_path) +lengthof(new_query) +lengthof(new_fragment)  ;
    URL *url = (URL *) palloc(schemeStructSize);
    SET_VARSIZE(url, schemeStructSize);

    url->scheme =  *new_scheme;
    url->host = *new_path;
    url->path = *new_path;
    url->query = *new_query;
    url->fragment = *new_fragment;


    return url;
}


URL *fromProtocolHostPortFile(const char *protocol,const char *host, const int *port, const char *file) {
    char new_scheme[10] = "";
    char new_host[1024] = "";
    char new_path[2048] = "";
//    PGconn *conn;
//    PGresult        *res;
//    conn = PQconnectdb("dbname=ljdata host=localhost user=dataman password=supersecret");
//
//    char request[100];
//    sprintf(request,"SELECT COUNT(*) FROM [Table] WHERE (scheme = %s)", protocol);
//    res= PQexec(conn, request);
//
//    if(PQntuples(res) > 0){
//        strcpy(new_scheme, protocol);
//        if (*port == -1 ){
//            strcpy(new_host,host);
//        }
//        else{sprintf(new_host, "%s:%d",host ,*port);}
//
//        strcpy(new_path,file);
//    }
//    else
//    {
//        //Username doesn't exist.
//    }
    strcpy(new_scheme, protocol);
    if (*port == -1 ){
        strcpy(new_host,host);
    }
    else{sprintf(new_host, "%s:%d",host ,*port);}

    strcpy(new_path,file);

    int32 schemeStructSize =lengthof(new_scheme) + lengthof(new_host) +lengthof(new_path)  ;
    URL *url = (URL *) palloc(schemeStructSize);
    SET_VARSIZE(url, schemeStructSize);

    url->scheme =  *new_scheme;
    url->host = *new_path;
    url->path = *new_path;

    return url;
}

URL *fromProtocolHostFile(const char *protocol,const char *host,const char *file) {
    int *port = NULL;
    *port = -1;

    URL *url= fromProtocolHostPortFile(protocol, host, port, file);

    return url;
}