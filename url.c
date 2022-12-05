#include "postgres.h"
#include "fmgr.h"
#include <stdio.h>
#include <string.h>
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

PG_FUNCTION_INFO_V1(getdefaultport);
Datum
getdefaultport(PG_FUNCTION_ARGS)
{
    Datum arg = PG_GETARG_DATUM(0);
    char const *s = TextDatumGetCString(arg);

    URL url = fromString(s);
    char* dPort = defaultPort(url);

    PG_RETURN_CSTRING(cstring_to_text(dPort));

}
PG_FUNCTION_INFO_V1(getfile);
Datum
getfile(PG_FUNCTION_ARGS)
{
    Datum arg = PG_GETARG_DATUM(0);
    char const *s = TextDatumGetCString(arg);

    URL url = fromString(s);

    PG_RETURN_CSTRING(cstring_to_text(url.path));

}

PG_FUNCTION_INFO_V1(gethost);
Datum
gethost(PG_FUNCTION_ARGS)
{
    Datum arg = PG_GETARG_DATUM(0);
    char const *s = TextDatumGetCString(arg);

    URL url = fromString(s);

    PG_RETURN_CSTRING(cstring_to_text(url.host));

}

PG_FUNCTION_INFO_V1(getpath);
Datum
getpath(PG_FUNCTION_ARGS)
{
    Datum arg = PG_GETARG_DATUM(0);
    char const *s = TextDatumGetCString(arg);

    URL url = fromString(s);
    //elog(DEBUG1,"hello111");

    PG_RETURN_CSTRING(cstring_to_text(url.path));
}

PG_FUNCTION_INFO_V1(getport);
Datum
getport(PG_FUNCTION_ARGS)
{
    Datum arg = PG_GETARG_DATUM(0);
    char const *s = TextDatumGetCString(arg);

    URL url = fromString(s);

    char *host = url.host;

    // detecting the first occurence of ":"
    char* ptr;
    int portSign = ':';
    ptr = strchr(host, portSign);
    if(ptr == NULL){
        // no port given, returns default port for host
        PG_RETURN_CSTRING(cstring_to_text("No port in URL"));

    }
    else {
        // deleting the first character to have the ":" removed from the string of char
        PG_RETURN_CSTRING(cstring_to_text(ptr++));


    }

}

PG_FUNCTION_INFO_V1(getprotocol);
Datum
getprotocol(PG_FUNCTION_ARGS)
{
    Datum arg = PG_GETARG_DATUM(0);
    char const *s = TextDatumGetCString(arg);

    URL url = fromString(s);
    char* protocol = url.scheme;

    // deleting the 3 last characters from the array (://)
    protocol[strlen(protocol)-3] = '\0';


    PG_RETURN_CSTRING(cstring_to_text(protocol));
}

PG_FUNCTION_INFO_V1(getquery);
Datum
getquery(PG_FUNCTION_ARGS)
{
    Datum arg = PG_GETARG_DATUM(0);
    char const *s = TextDatumGetCString(arg);

    URL url = fromString(s);


    PG_RETURN_CSTRING(cstring_to_text(url.query));
}




