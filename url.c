#include "postgres.h"
#include "fmgr.h"
#include "src/models.c"

#define PG_RETURN_URL(x)    PG_RETURN_POINTER(x)

PG_MODULE_MAGIC;



int32 getComparisonResult(const struct FunctionCallInfoBaseData *fcinfo) {
    URL const *url1 = (URL *) PG_GETARG_POINTER(0);
    URL const *url2 = (URL *) PG_GETARG_POINTER(1);

    return strcmp(url1->url, url2->url);
}

PG_FUNCTION_INFO_V1(url_in_1_arg);
Datum url_in_1_arg(PG_FUNCTION_ARGS) {

    char const *first = PG_GETARG_CSTRING(0);
    URL *url = urlFromString(first);

    PG_RETURN_URL(url);
}
PG_FUNCTION_INFO_V1(url_in_2_arg);
Datum url_in_2_arg(PG_FUNCTION_ARGS) {
    URL *first = (URL *) PG_GETARG_POINTER(0);
    char const *second = PG_GETARG_CSTRING(1);
    URL *url = urlFromStringWithContext(first,second);
    return (Datum) url;
}
PG_FUNCTION_INFO_V1(url_in_3_arg);
Datum url_in_3_arg(PG_FUNCTION_ARGS) {
    char const *first = PG_GETARG_CSTRING(0);
    char const *second = PG_GETARG_CSTRING(1);
    char const *third = PG_GETARG_CSTRING(2);
    URL *url = urlFromProtocolHostFile(first,second,third);
//    PG_RETURN_URL(url);
    return (Datum) url;
}
PG_FUNCTION_INFO_V1(url_in_4_arg);
Datum url_in_4_arg(PG_FUNCTION_ARGS) {
    char const *first = PG_GETARG_CSTRING(0);
    char const *second = PG_GETARG_CSTRING(1);
    int third = PG_GETARG_INT64(2);
    char const *fourth = PG_GETARG_CSTRING(3);
    URL *url = urlFromProtocolHostPortFile(first,second,third,fourth);
//    PG_RETURN_URL(url);
    return (Datum) url;
}


PG_FUNCTION_INFO_V1(url_out);

Datum url_out(PG_FUNCTION_ARGS) {

    URL const *url = (URL *) PG_DETOAST_DATUM(PG_GETARG_POINTER(0));
    PG_RETURN_CSTRING(url->url);
}

PG_FUNCTION_INFO_V1(geturl);
Datum
geturl(PG_FUNCTION_ARGS) {
    URL *url = urlFromStringReverse(PG_GETARG_CSTRING(0));
    return (Datum) getUrl(url);
}

PG_FUNCTION_INFO_V1(getauthority);

Datum
getauthority(PG_FUNCTION_ARGS) {
    URL const *url = (URL *) PG_GETARG_POINTER(0);
    return getHostFromUrl(url);
}

PG_FUNCTION_INFO_V1(getusername);

Datum
getusername(PG_FUNCTION_ARGS) {
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