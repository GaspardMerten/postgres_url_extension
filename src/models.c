#include <string.h>
#include <postgres.h>
#include <fmgr.h>
#include <utils/builtins.h>


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

Datum getHostFromUrl(const URL *url) {
    return mallocAndMakeSlice(url->url + url->scheme + url->user, url->host);
}


Datum getSchemeFromUrl(const URL *url) {
    return mallocAndMakeSlice(url->url, url->scheme);
}
Datum getFileFromUrl(const URL *url) {
    int totalLengthWithoutFragment = url->scheme + url->user + url->host + url->port + url->path + url->query;

    return mallocAndMakeSlice(url->url, totalLengthWithoutFragment);
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
    return mallocAndMakeSlice(url->url + url->scheme + url->user + url->host + delta,
                              url->port - delta); // +delta (and thus -delta for length) removes the ":"
}

char *getUrl(const URL *url) {
    int totalLength = url->scheme + url->user + url->host + url->port + url->path + url->query + url->fragment;

    char *urlString = malloc((totalLength + 1) * sizeof(char));
    strncpy(urlString, url->url, totalLength);
    *(urlString + totalLength) = '\0';

    return urlString;
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
    int delta = 0;
    if (url->user > 0) {
        delta = url->user - 1; // prevent inclusion of @
    }
    return mallocAndMakeSlice(url->url + url->scheme, delta);
}

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
    memcpy(url->url, sourceStart, charInSource+1);
    char const* endSymbol = "\0";
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