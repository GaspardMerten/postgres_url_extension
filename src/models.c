#include <string.h>
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

char* defaultPort(URL url){
    char* scheme = url.scheme;

    // converting to lowercase first

    if(strcmp(scheme, "http://") == 0){
        return "80";
    }
    else if(strcmp(scheme, "https://") == 0){
        return "443";
    }
    else if(strcmp(scheme, "smtp://") == 0){
        return "25";
    }
    else if(strcmp(scheme, "ftp://") == 0){
        return "21";
    }
    else if(strcmp(scheme, "ssh://") == 0){
        return "22";
    }
    else if(strcmp(scheme, "telnet://") == 0){
        return "23";
    }
    else if(strcmp(scheme, "dns://") == 0){
        return "53";
    }

    else {
        return "Port not found";
    }


}