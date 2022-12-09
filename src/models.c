#include <string.h>
#include <postgres.h>
#include <fmgr.h>
#include <utils/builtins.h>

// Structure of an url
typedef struct URL {
#pragma clang diagnostic push
#pragma ide diagnostic ignored "OCUnusedGlobalDeclarationInspection"
    uint32 length; /// DO NOT REMOVE, used internally
#pragma clang diagnostic pop
    int8 scheme;
    int8 host;
    int8 path;
    int8 query;
    int8 user;
    int8 port;
    int8 fragment;
    char url[];
} URL;


Datum mallocAndMakeSlice(const char *start, int length) {
    PG_RETURN_TEXT_P(cstring_to_text_with_len(start, length));
}
// Get information of the url as Datum for postgresql uses.
Datum getHostFromUrl(const URL *url) {
    return mallocAndMakeSlice(url->url + url->scheme + url->user, url->host);
}

Datum getSchemeFromUrl(const URL *url) {
    return mallocAndMakeSlice(url->url, url->scheme);
}

Datum getFileFromUrl(const URL *url) {
    return mallocAndMakeSlice(url->url + url->scheme + url->user + url->host + url->port, url->path + url->query);
}

Datum getRawUrlFromUrl(const URL *url) {
    int totalLength = url->scheme + url->user + url->host + url->port + url->path + url->query + url->fragment;
    return mallocAndMakeSlice(url->url, totalLength);
}

Datum getPortFromUrl(const URL *url) {
    int delta = 0;
    if (url->port > 0) {
        delta = 1;
    }
    char port[7];
    char const *portStart = url->url + url->scheme + url->user + url->host + delta;

    for (int i = 0; i < url->port - delta; ++i) {
        port[i] = *(portStart + i);
    }
    port[url->port - delta] = '\0';

    PG_RETURN_INT16(pg_strtoint16(port));
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

Datum getProtocolFromUrl(const URL *url) {
    return mallocAndMakeSlice(url->url, url->scheme - 3); // -3 to delete the ://
}

Datum getUsernameFromUrl(const URL *url) {
    int delta = 0;
    if (url->user > 0) {
        delta = url->user - 1; // prevent inclusion of @
    }
    return mallocAndMakeSlice(url->url + url->scheme, delta);
}

// Get the string of the different part of an url.

char *getUrl(const URL *url) {
    int totalLength = url->scheme + url->user + url->host + url->port + url->path + url->query + url->fragment;
    char *urlString = malloc((totalLength + 1) * sizeof(char));
    strncpy(urlString, url->url, totalLength);
    *(urlString + totalLength) = '\0';
    return urlString;
}

char *getSchemeString(const URL *url) {
    return text_to_cstring(cstring_to_text_with_len(url->url, +url->scheme));
}
// Get the user, host, port information.
char *getHostString(const URL *url) {
    return text_to_cstring(cstring_to_text_with_len(url->url + url->scheme, url->user + url->host + url->port));
}

char *getPathString(const URL *url) {
    return text_to_cstring(cstring_to_text_with_len(url->url + url->scheme + url->user
                                                    + url->host + url->port, url->path));
}

char *getQueryString(const URL *url) {
    return text_to_cstring(cstring_to_text_with_len(url->url + url->scheme + url->user
                                                    + url->host + url->port + url->path, url->query));
}

char *getFragmentString(const URL *url) {
    return text_to_cstring(cstring_to_text_with_len(url->url + url->scheme + url->user
                                                    + url->host + url->port + url->path + url->query, url->fragment));
}

// Creates a URL object from the String representation
URL *urlFromString(const char *source) {
    int8 pointerSize[] = {0, 0, 0, 0, 0, 0, 0};
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
                                errdetail(
                                        "The '%s' string does not appear to follow the following pattern => scheme://[username@]hostname[:port][path][?query][#fragment] pattern. ([value] means the value is optional)",
                                        sourceStart),
                                errhint("Make sure you are entering a valid url. (not very helpful, we know...)")
                )
        );
    }
    int32 schemeStructSize = sizeof(URL) + sizeof(char) * charInSource;
    URL *url = (URL *) palloc(schemeStructSize);
    url->length = ((uint32) schemeStructSize << 2);
    memcpy(url->url, sourceStart, charInSource + 1);
    char const *endSymbol = "\0";
    strcat(url->url, endSymbol);
    url->scheme = pointerSize[0];
    url->user = pointerSize[1];
    url->host = pointerSize[2];
    url->port = pointerSize[3];
    url->path = pointerSize[4];
    url->query = pointerSize[5];
    url->fragment = pointerSize[6];

    return url;
}
// Creates a URL object from the String representation.
// Start the parsing at the end to detect uncompleted url
URL *urlFromStringReverse(const char *source) {
    int8 pointerSize[] = {0, 0, 0, 0, 0, 0, 0};
    int8 pos = 0;
    int currentPointer = 6;
    int previousPointer;
    char lastChar = '\0';
    int charInSource = 0;
    const char *sourceStart = source;

    while (*source) {
        charInSource++;
        source++;
    }

    source--;
    charInSource--;

    for (int i = 0; i <= charInSource; i++) {
        pos++;
        if (currentPointer == 6 && *source == '#') {
            currentPointer = 5;
            previousPointer = 6;
            pointerSize[previousPointer] = pos;
            pos = 0;
        }
        if ((currentPointer == 6 || currentPointer == 5) && *source == '?') {
            currentPointer = 2;
            previousPointer = 5;
            pointerSize[previousPointer] = pos;
            pos = 0;
        }
        if ((currentPointer == 6 || currentPointer == 5 || currentPointer == 2)
            && *source == '/' && lastChar == '/') {
            currentPointer = 0;
            previousPointer = 2;
            pos -= 2;
            pointerSize[previousPointer] = pos;
            pos = 2;
        }
        if (i == charInSource) {
            if (currentPointer > 0) { pointerSize[2] = pos; }
            else { pointerSize[currentPointer] = pos; }
        } else {
            lastChar = *source;
            source--;
        }
    }
    // if there is no scheme there is no host, only a path
    if (pointerSize[0] == 0) {
        pointerSize[4] = pointerSize[2];
        pointerSize[2] = 0;
    }
    // If we found a host check if there is a user or a port or path
    if (pointerSize[2] != 0) {
        for (int i = 0; i < pointerSize[0]; i++) {
            source++;
        }
        currentPointer = 2;
        int8 totalHostSize = pointerSize[2];
        pointerSize[2] = 0;
        for (int i = 0; i < totalHostSize; i++) {
            if (currentPointer == 2 && *source == ':') {
                currentPointer = 3;
            }
            if ((currentPointer == 2 || currentPointer == 3) && *source == '/') {
                currentPointer = 4;
            }
            pointerSize[currentPointer]++;
            if (*source == '@') {
                pointerSize[1] = pointerSize[2];
                pointerSize[2] = 0;
                currentPointer = 2;
            }
            source++;
        }
    }

    int32 schemeStructSize = sizeof(URL) + sizeof(char) * charInSource;
    URL *url = (URL *) palloc(schemeStructSize);
    url->length = ((uint32) schemeStructSize << 2);
    memcpy(url->url, sourceStart, charInSource + 1);
    char const *endSymbol = "\0";
    strcat(url->url, endSymbol);
    url->scheme = pointerSize[0];
    url->user = pointerSize[1];
    url->host = pointerSize[2];
    url->port = pointerSize[3];
    url->path = pointerSize[4];
    url->query = pointerSize[5];
    url->fragment = pointerSize[6];
    return url;
}
// Creates a URL by parsing the given spec within a specified context.
URL *urlFromStringWithContext(const URL *context, const char *source) {
    URL const *spec = urlFromStringReverse(source);
    char new_scheme[10] = "";
    char new_host[2048] = "";
    char new_path[2048] = "";
    char new_query[256] = "";
    char new_fragment[256] = "";

    // Initialize default new_url
    strcpy(new_scheme, getSchemeString(context));
    strcpy(new_host, getHostString(context));
    strcpy(new_path, getPathString(context));
    strcpy(new_query, getQueryString(context));
    strcpy(new_fragment, getFragmentString(context));

    // ref to current doc
    if (strcmp(getPathString(spec),"") != 0 && strcmp(getSchemeString(spec), "") != 0
        && strcmp(getHostString(spec), "") != 0 && strcmp(getQueryString(spec), "") != 0) {
        strcpy(new_scheme, getSchemeString(context));
        strcpy(new_host, getHostString(context));
        strcpy(new_path, getPathString(context));
        strcpy(new_query, getQueryString(context));
        strcpy(new_fragment, getFragmentString(context));
    }  //Otherwise, the fragment and query parts present in the spec are used in the new URL
    else if (strcmp(getPathString(spec), "") == 0 && strcmp(getSchemeString(spec), "") == 0
             && strcmp(getHostString(spec), "") == 0 && strcmp(getQueryString(spec), "") == 0) {
        strcpy(new_scheme, getSchemeString(context));
        strcpy(new_host, getHostString(context));
        strcpy(new_path, getPathString(context));
        strcpy(new_query, getQueryString(spec));
        strcpy(new_fragment, getFragmentString(spec));
    } // the new URL is created as an absolute URL based on the spec alon
    else {
        if (strcmp(getSchemeString(spec), getSchemeString(context)) != 0 && strcmp(getSchemeString(spec), "") != 0) {
            strcpy(new_scheme, getSchemeString(spec));
            strcpy(new_host, getHostString(spec));
            strcpy(new_path, getPathString(spec));
            strcpy(new_query, getQueryString(spec));
            strcpy(new_fragment, getFragmentString(spec));
        } //scheme component is inherited from the context URL
        if (strcmp(getSchemeString(spec), getSchemeString(context)) == 0) {
            strcpy(new_scheme, getSchemeString(context));
        } //  the spec is treated as absolute and the spec host and path will replace the context host and path
        if (strcmp(getHostString(spec), "") != 0) {
            strcpy(new_host, getHostString(spec));
            strcpy(new_path, getPathString(spec));
        } // host component is absent in the spec then the host of the new URL will be inherited from the context
        if (strcmp(getHostString(spec), "") == 0) {
            strcpy(new_host, getHostString(context));
            strcpy(new_path, getPathString(context));
        } // path is treated as absolute and the spec path replaces the context path
        if (getPathString(spec)[0] == '/') {
            strcpy(new_path, getPathString(spec));
        } // path is treated as a relative path and is appended to the context path
        if (getPathString(spec)[0] != '/') {
            strcpy(new_path, getPathString(context));
            strcat(new_path, getPathString(spec));
        }
        if (strcmp(getQueryString(spec), "") != 0) {
            strcpy(new_query, getQueryString(spec));
        }
        if (strcmp(getFragmentString(spec), "") != 0) {
            strcpy(new_fragment, getFragmentString(spec));
        }
    }

    char *finalUrlString = malloc(sizeof(new_scheme) + 3 + sizeof(new_host)
                                  + sizeof(new_path) + sizeof(new_query) + sizeof(new_fragment));
    sprintf(finalUrlString, "%s%s%s%s%s", new_scheme, new_host, new_path, new_query, new_fragment);

    URL *url = urlFromString(finalUrlString);

    return url;
}

// Creates a URL object from the specified protocol, host, port number, and file.
URL *urlFromProtocolHostPortFile(const char *protocol, const char *host, int port, const char *file) {
    char *finalUrlString;
    if (port < 0) {
        finalUrlString = malloc(sizeof(protocol) + 3 + sizeof(host) + sizeof(file));
        sprintf(finalUrlString, "%s://%s%s", protocol, host, file);
    } else {
        finalUrlString = malloc(sizeof(protocol) + 3 + sizeof(host) + 1 + sizeof(port) + sizeof(file));
        sprintf(finalUrlString, "%s://%s:%d%s", protocol, host, port, file);
    }
    URL *url = urlFromString(finalUrlString);

    return url;
}
//Creates a URL from the specified protocol name, host name, and file name.
URL *urlFromProtocolHostFile(const char *protocol, const char *host, const char *file) {
    int port = -1;
    URL *url = urlFromProtocolHostPortFile(protocol, host, port, file);

    return url;
}