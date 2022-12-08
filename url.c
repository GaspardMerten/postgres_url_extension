#include "postgres.h"
#include "fmgr.h"
#include "src/models.c"

#define PG_RETURN_URL(x)    PG_RETURN_POINTER(x)

PG_MODULE_MAGIC;

PG_FUNCTION_INFO_V1(url_in);

int32 getComparisonResult(const struct FunctionCallInfoBaseData *fcinfo) {
    URL const *url1 = (URL *) PG_GETARG_POINTER(0);
    URL const *url2 = (URL *) PG_GETARG_POINTER(1);

    return strcmp(url1->url, url2->url);
}


Datum url_in(PG_FUNCTION_ARGS) {
    char const *s = PG_GETARG_CSTRING(0);
    URL *url = urlFromString(s);
    PG_RETURN_URL(url);
}

PG_FUNCTION_INFO_V1(url_out);

Datum url_out(PG_FUNCTION_ARGS) {
    URL const *url = (URL *) PG_DETOAST_DATUM(PG_GETARG_POINTER(0));
    PG_RETURN_CSTRING(url->url);
}

PG_FUNCTION_INFO_V1(getauthority);
Datum
getauthority(PG_FUNCTION_ARGS) {
    URL const *url = (URL *) PG_GETARG_POINTER(0);
    return getAuthorityFromUrl(url);
}

PG_FUNCTION_INFO_V1(getfile);
Datum
getfile(PG_FUNCTION_ARGS) {
URL const *url = (URL *) PG_GETARG_POINTER(0);
return getFileFromUrl(url);
}

PG_FUNCTION_INFO_V1(getuserinfo);

Datum
getuserinfo(PG_FUNCTION_ARGS) {
    URL const *url = (URL *) PG_GETARG_POINTER(0);
    return getUsernameFromUrl(url);

}

PG_FUNCTION_INFO_V1(getscheme);

Datum
getscheme(PG_FUNCTION_ARGS) {
    URL const *url = (URL *) PG_GETARG_POINTER(0);
    return getSchemeFromUrl(url);
}

PG_FUNCTION_INFO_V1(getquery);

Datum
getquery(PG_FUNCTION_ARGS) {
    URL const *url = (URL *) PG_GETARG_POINTER(0);
    return getQueryFromUrl(url);
}

PG_FUNCTION_INFO_V1(getref);

Datum
getref(PG_FUNCTION_ARGS) {
    URL const *url = (URL *) PG_GETARG_POINTER(0);
    return getRefFromUrl(url);
}

PG_FUNCTION_INFO_V1(getport);
Datum
getport(PG_FUNCTION_ARGS) {
    URL const *url = (URL *) PG_GETARG_POINTER(0);
    return getPortFromUrl(url);
}

PG_FUNCTION_INFO_V1(getdefaultport);
Datum
getdefaultport(PG_FUNCTION_ARGS) {
    URL const *url = (URL *) PG_GETARG_POINTER(0);
    // copy url->url array into string
    char *protocol = url->url;
    protocol[url->scheme - 3] = '\0';


    if (strcmp(protocol, "http") == 0) {
        PG_RETURN_INT32(80);
    } else if (strcmp(protocol, "https") == 0) {
        PG_RETURN_INT32(443);
    } else if (strcmp(protocol, "ftp") == 0) {
        PG_RETURN_INT32(21);
    }
    else if (strcmp(protocol, "ssh") == 0) {
        PG_RETURN_INT32(22);
    } else if (strcmp(protocol, "dns") == 0) {
        PG_RETURN_INT32(53);
    }

}
PG_FUNCTION_INFO_V1(getprotocol);
Datum
getprotocol(PG_FUNCTION_ARGS) {
    URL const *url = (URL *) PG_GETARG_POINTER(0);
    return getProtocolFromUrl(url);
}

PG_FUNCTION_INFO_V1(getpath);

Datum
getpath(PG_FUNCTION_ARGS) {
    URL const *url = (URL *) PG_GETARG_POINTER(0);
    return getPathFromUrl(url);
}

PG_FUNCTION_INFO_V1(equals);

Datum
equals(PG_FUNCTION_ARGS) {
    PG_RETURN_BOOL(getComparisonResult(fcinfo) == 0);
}

PG_FUNCTION_INFO_V1(urlne);

Datum
urlne(PG_FUNCTION_ARGS) {
    PG_RETURN_BOOL(getComparisonResult(fcinfo) != 0);
}

PG_FUNCTION_INFO_V1(urllt);

Datum
urllt(PG_FUNCTION_ARGS) {
    PG_RETURN_BOOL(getComparisonResult(fcinfo) < 0);

}

PG_FUNCTION_INFO_V1(urlle);

Datum
urlle(PG_FUNCTION_ARGS) {
    PG_RETURN_BOOL(getComparisonResult(fcinfo) <= 0);

}

PG_FUNCTION_INFO_V1(urlgt);

Datum
urlgt(PG_FUNCTION_ARGS) {
    PG_RETURN_BOOL(getComparisonResult(fcinfo) > 0);
}

PG_FUNCTION_INFO_V1(urlge);

Datum
urlge(PG_FUNCTION_ARGS) {
    PG_RETURN_BOOL(getComparisonResult(fcinfo) >= 0);
}

PG_FUNCTION_INFO_V1(cmpurls);

Datum
cmpurls(PG_FUNCTION_ARGS) {
    PG_RETURN_INT32(getComparisonResult(fcinfo));

}

PG_FUNCTION_INFO_V1(hashFromUrl);
Datum
hashFromUrl(PG_FUNCTION_ARGS) {
    URL const *value = (URL*) PG_GETARG_DATUM(0);

    int32 hash = 0;

    char const *url = getUrl(value);

    while (*url++) {
        hash += 31 * hash + *url;
    }

    PG_RETURN_INT32(hash);
}