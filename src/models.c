#include <string.h>
#include <stdio.h>
#include <postgres.h>


typedef struct URL {
    char *scheme;
    char *host;
    char *path;
    char *query;
    char *fragment;
} URL;


URL fromString(const char *source) {
    elog(DEBUG1, source);
    char scheme[10] = "";
    char host[1024] = "";
    char path[2048] = "";
    char query[2048] = "";
    char fragment[256] = "";

    char *pointers[] = {scheme, host, path, query, fragment};

    int currentPointer = 0;

    char lastChar = '0';
    char cToStr[2];
    cToStr[1] = '\0';

    while ( *source ) {
        if (currentPointer == 1 && *source == '/') {
            currentPointer = 2;
        }

        if ((currentPointer == 1 || currentPointer == 2) && *source == '?') {
            currentPointer = 3;
        }

        if ((currentPointer == 1 || currentPointer == 2 || currentPointer == 3) && *source == '#') {
            currentPointer = 4;
        }

        cToStr[0] = *source;
        strcat(pointers[currentPointer], cToStr);
        elog(DEBUG1, cToStr);

        if (currentPointer == 0 && lastChar == '/' && *source == '/') {
            currentPointer = 1;
        }

        lastChar = *source;

        source++;
    }

    URL url = {scheme, host, path, query, fragment};

    elog(DEBUG1, scheme);
    elog(DEBUG1, host);
    elog(DEBUG1, path);
    elog(DEBUG1, query);
    elog(DEBUG1, fragment);

    return url;
}

URL fromStringWithContext(URL context, const char *source) {
    elog(DEBUG1, source);
    char scheme[10] = "";
    char host[1024] = "";
    char path[2048] = "";
    char query[2048] = "";
    char fragment[256] = "";
    char new_scheme[10] = "";
    char new_host[1024] = "";
    char new_path[2048] = "";
    char new_query[2048] = "";
    char new_fragment[256] = "";

    char *pointers[] = {scheme, host, path, query, fragment};

    int currentPointer = 0;

    char lastChar = '0';
    char cToStr[2];
    cToStr[1] = '\0';

    while ( *source ) {
        if (currentPointer == 1 && *source == '/') {
            currentPointer = 2;
        }

        if ((currentPointer == 1 || currentPointer == 2) && *source == '?') {
            currentPointer = 3;
        }

        if ((currentPointer == 1 || currentPointer == 2 || currentPointer == 3) && *source == '#') {
            currentPointer = 4;
        }

        cToStr[0] = *source;
        strcat(pointers[currentPointer], cToStr);
        elog(DEBUG1, cToStr);

        if (currentPointer == 0 && lastChar == '/' && *source == '/') {
            currentPointer = 1;
        }

        lastChar = *source;

        source++;
    }

    // ref to current doc
    if ( strcmp(path,"") != 0 &&  strcmp(scheme,"") != 0
    && strcmp(host,"") != 0 && strcmp(query,"") != 0){
        strcpy(new_scheme, context.scheme) ;
        strcpy(new_host, context.host);
        strcpy(new_path, context.path);
        strcpy(new_query, context.query);
        strcpy(new_fragment, context.fragment);
    }  //Otherwise, the fragment and query parts present in the spec are used in the new URL
    else if ( strcmp(path,"") == 0 &&  strcmp(scheme,"") == 0
    && strcmp(host,"") == 0 && strcmp(query,"") == 0) {
        strcpy(new_scheme, context.scheme);
        strcpy(new_host, context.host);
        strcpy(new_path, context.path);
        strcpy(new_query, query);
        strcpy(new_fragment, fragment);
    } // the new URL is created as an absolute URL based on the spec alon
    else if (strcmp(scheme,context.scheme) != 0) {
        strcpy(new_scheme, scheme) ;
        strcpy(new_host, host);
        strcpy(new_path, path);
        strcpy(new_query, query);
        strcpy(new_fragment, fragment);
    } //scheme component is inherited from the context URL
    else if (!strcmp(scheme,context.scheme)) {
        strcpy(new_scheme, context.scheme) ;
    } //  the spec is treated as absolute and the spec host and path will replace the context host and path
    else if (!strcmp(host,"")) {
        strcpy(new_scheme, context.scheme) ;
        strcpy(new_host, host);
        strcpy(new_path, path);
    } // host component is absent in the spec then the host of the new URL will be inherited from the context
    else if (strcmp(host,"") != 0) {
        strcpy(new_host, context.host);
    } // path is treated as absolute and the spec path replaces the context path
    else if (path[0] == '/') {
        strcpy(new_path, path);
    } // path is treated as a relative path and is appended to the context path
    else if (path[0] != '/') {
        strcpy(new_path, context.path);
        strcat(new_path  , path);
    }

    URL url = {new_scheme, new_host, new_path, new_query, new_fragment};

    elog(DEBUG1, new_scheme);
    elog(DEBUG1, new_host);
    elog(DEBUG1, new_path);
    elog(DEBUG1, new_query);
    elog(DEBUG1, new_fragment);

    return url;
}
URL fromProtocolHostPortFile(const char *protocol,const char *host, const int *port, const char *file) {
    char new_scheme[10] = "";
    char new_host[1024] = "";
    char new_path[2048] = "";

    strcpy(new_scheme, protocol);
    if (*port == -1 ){
        strcpy(new_host,host);
    }
    else{sprintf(new_host, "%s:%d",host ,*port);}

    strcpy(new_path,file);

    URL url= {new_scheme, new_host, new_path, "", ""};

    return url;
}

URL fromProtocolHostFile(const char *protocol,const char *host,const char *file) {
    int *port;
    *port = -1;
    URL url= fromProtocolHostPortFile(protocol, host, port, file);

    return url;
}

