#include <string.h>
#include <postgres.h>


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

char *mallocAndMakeSlice(const char *start, int length) {
    char *out = malloc(length + 1);
    if (length == 0) {
        *out = '\0';
    } else {
        strncpy(out, start, length);
        *(out + length) = '\0';
    }
    return out;
}

char *getHostFromUrl(const URL *url) {
    return mallocAndMakeSlice(url->url + url->scheme + url->user, url->host);
}

char *getSchemeFromUrl(const URL *url) {
    return mallocAndMakeSlice(url->url, url->scheme);
}

char *getPortFromUrl(const URL *url) {
    int delta = 0;
    if (url->port > 0) {
        delta = 1;
    }
    return mallocAndMakeSlice(url->url + url->scheme + url->user + url->host+delta, url->port-delta); // +delta (and thus -delta for length) removes the ":"
}

char *getRefFromUrl(const URL *url) {
    return mallocAndMakeSlice(url->url + url->scheme + url->user + url->host + url->port + url->path + url->query,
                              url->fragment);
}

char *getQueryFromUrl(const URL *url) {
    return mallocAndMakeSlice(url->url + url->scheme + url->user + url->host + url->port + url->path, url->query);
}

char *getPathFromUrl(const URL *url) {
    return mallocAndMakeSlice(url->url + url->scheme + url->user + url->host + url->port, url->path);
}

char *getUsernameFromUrl(const URL *url) {
    elog(DEBUG1, url->url);
    char int_str[20];

    sprintf(int_str, "%d", url->user);

    elog(DEBUG1, "Hello world");
    elog(DEBUG1,  int_str);
    sprintf(int_str, "%d", url->scheme);
    elog(DEBUG1,  int_str);


    int delta = 0;
    if (url->user > 0) {
        delta = url->user - 1; // prevent inclusion of @
    }
    sprintf(int_str, "%d", delta);
    elog(DEBUG1,  int_str);
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