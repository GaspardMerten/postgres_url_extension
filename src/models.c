#include <string.h>
#include <postgres.h>
#include <fmgr.h>
#include <utils/builtins.h>


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