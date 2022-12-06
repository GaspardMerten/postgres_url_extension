#include "postgres.h"
#include "fmgr.h"
#include "utils/builtins.h"
#include "src/models.c"

#define PG_RETURN_URL(x)    PG_RETURN_POINTER(x)

PG_MODULE_MAGIC;

PG_FUNCTION_INFO_V1(url_in);

Datum url_in(PG_FUNCTION_ARGS) {
    char const *s = PG_GETARG_CSTRING(0);
    URL *url = urlFromString(s);
    PG_RETURN_URL(url);
}

PG_FUNCTION_INFO_V1(url_out);

Datum url_out(PG_FUNCTION_ARGS) {
    URL *url = (URL *) PG_GETARG_POINTER(0);
    elog(DEBUG1, url);
    PG_RETURN_CSTRING(url->url);
}


PG_FUNCTION_INFO_V1(getauthority);
Datum
getauthority(PG_FUNCTION_ARGS) {
    URL const *url = (URL *) PG_GETARG_POINTER(0);
    char const *string = getHostFromUrl(url);
    elog(DEBUG1, string);
    PG_RETURN_CSTRING(cstring_to_text(string));
}

PG_FUNCTION_INFO_V1(getusername);
Datum
getusername(PG_FUNCTION_ARGS) {
    URL const *url = (URL *) PG_GETARG_POINTER(0);
    char const *string = getUsernameFromUrl(url);
    PG_RETURN_CSTRING(cstring_to_text(string));
}
PG_FUNCTION_INFO_V1(getscheme);
Datum
getscheme(PG_FUNCTION_ARGS) {
    URL const *url = (URL *) PG_GETARG_POINTER(0);
    char const *string = getSchemeFromUrl(url);
    PG_RETURN_TEXT_P(cstring_to_text(string));
}

PG_FUNCTION_INFO_V1(getquery);
Datum
getquery(PG_FUNCTION_ARGS) {
    URL const *url = (URL *) PG_GETARG_POINTER(0);
    char const *string = getQueryFromUrl(url);
    PG_RETURN_TEXT_P(cstring_to_text(string));
}

PG_FUNCTION_INFO_V1(getref);
Datum
getref(PG_FUNCTION_ARGS) {
    URL const *url = (URL *) PG_GETARG_POINTER(0);
    char const *string = getRefFromUrl(url);
    PG_RETURN_TEXT_P(cstring_to_text(string));
}

PG_FUNCTION_INFO_V1(getport);
Datum
getport(PG_FUNCTION_ARGS) {
    URL const *url = (URL *) PG_GETARG_POINTER(0);
    char const *string = getPortFromUrl(url);
    PG_RETURN_TEXT_P(cstring_to_text(string));
}

PG_FUNCTION_INFO_V1(getpath);
Datum
getpath(PG_FUNCTION_ARGS) {
    URL const *url = (URL *) PG_GETARG_POINTER(0);
    char const *string = getPathFromUrl(url);
    PG_RETURN_TEXT_P(cstring_to_text(string));
}