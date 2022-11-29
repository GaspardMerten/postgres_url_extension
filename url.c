#include "postgres.h"
#include "fmgr.h"
#include "utils/builtins.h"
#include "src/models.c"


#define PG_RETURN_url(x)    PG_RETURN_POINTER(x)

PG_MODULE_MAGIC;

PG_FUNCTION_INFO_V1(url_in);

Datum url_in(PG_FUNCTION_ARGS) {
    elog(DEBUG1, "XXX world");

    char const *s = PG_GETARG_CSTRING(0);
    fromString(s);
    PG_RETURN_url(cstring_to_text(s));
}

PG_FUNCTION_INFO_V1(url_out);

Datum
url_out(PG_FUNCTION_ARGS) {
    Datum arg = PG_GETARG_DATUM(0);
    PG_RETURN_CSTRING(TextDatumGetCString(arg));
}


PG_FUNCTION_INFO_V1(getauthority);
Datum
getauthority(PG_FUNCTION_ARGS)
{
    Datum arg = PG_GETARG_DATUM(0);
    char const *s = TextDatumGetCString(arg);

    URL url = fromString(s);

    PG_RETURN_CSTRING(cstring_to_text(url.host));

}