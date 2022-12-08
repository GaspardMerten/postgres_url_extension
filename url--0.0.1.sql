\echo Use "CREATE EXTENSION url" to load this file. \quit


CREATE FUNCTION url_in(cstring)
    RETURNS url
AS
'$libdir/url', 'url_in_1_arg'
    LANGUAGE C IMMUTABLE
               STRICT;

CREATE FUNCTION url_in(url, cstring)
    RETURNS url
AS
'$libdir/url', 'url_in_2_arg'
    LANGUAGE C IMMUTABLE
               STRICT;

CREATE FUNCTION url_in(cstring, cstring, cstring)
    RETURNS url
AS
'$libdir/url', 'url_in_3_arg'
    LANGUAGE C IMMUTABLE
               STRICT;

CREATE FUNCTION url_in(cstring, cstring, int, cstring)
    RETURNS url
AS
'$libdir/url', 'url_in_4_arg'
    LANGUAGE C IMMUTABLE
               STRICT;

CREATE FUNCTION url_out(url)
    RETURNS cstring
AS
'$libdir/url'
    LANGUAGE C IMMUTABLE
               STRICT;

CREATE TYPE url
(
    INPUT = url_in,
    OUTPUT = url_out,
    INTERNALLENGTH = VARIABLE
);

CREATE FUNCTION geturl(url) RETURNS text
    IMMUTABLE
    STRICT
    LANGUAGE C
AS
'$libdir/url';

CREATE FUNCTION getAuthority(url) RETURNS text
    IMMUTABLE
    STRICT
    LANGUAGE C
AS
'$libdir/url';

CREATE FUNCTION getUsername(url) RETURNS text
    IMMUTABLE
    STRICT
    LANGUAGE C
AS
'$libdir/url';

CREATE FUNCTION getPath(url) RETURNS text
    IMMUTABLE
    STRICT
    LANGUAGE C
AS
'$libdir/url';

CREATE FUNCTION getRef(url) RETURNS text
    IMMUTABLE
    STRICT
    LANGUAGE C
AS
'$libdir/url';

CREATE FUNCTION getPort(url) RETURNS text
    IMMUTABLE
    STRICT
    LANGUAGE C
AS
'$libdir/url';

CREATE FUNCTION getQuery(url) RETURNS text
    IMMUTABLE
    STRICT
    LANGUAGE C
AS
'$libdir/url';

CREATE FUNCTION getScheme(url) RETURNS text
    IMMUTABLE
    STRICT
    LANGUAGE C
AS
'$libdir/url';

CREATE FUNCTION equals(url, url) RETURNS bool
    IMMUTABLE
    STRICT
    LANGUAGE C
AS
'$libdir/url';


CREATE FUNCTION urlne(url, url) RETURNS bool
    IMMUTABLE
    STRICT
    LANGUAGE C
AS
'$libdir/url';

CREATE FUNCTION urlgt(url, url) RETURNS bool
    IMMUTABLE
    STRICT
    LANGUAGE C
AS
'$libdir/url';

CREATE FUNCTION urlge(url, url) RETURNS bool
    IMMUTABLE
    STRICT
    LANGUAGE C
AS
'$libdir/url';

CREATE FUNCTION urlle(url, url) RETURNS bool
    IMMUTABLE
    STRICT
    LANGUAGE C
AS
'$libdir/url';

CREATE FUNCTION cmpUrls(url, url) RETURNS int
    IMMUTABLE
    STRICT
    LANGUAGE C
AS
'$libdir/url';

CREATE FUNCTION urllt(url, url) RETURNS bool
    IMMUTABLE
    STRICT
    LANGUAGE C
AS
'$libdir/url';



CREATE OPERATOR = (
    LEFTARG = url,
    RIGHTARG = url,
    PROCEDURE = equals,
    COMMUTATOR = =,
    NEGATOR = <>,
    RESTRICT = eqsel,
    JOIN = eqjoinsel,
    HASHES, MERGES
    );

CREATE OPERATOR <> (
    LEFTARG = url,
    RIGHTARG = url,
    PROCEDURE = urlne,
    COMMUTATOR = <>,
    NEGATOR = =,
    RESTRICT = neqsel,
    JOIN = neqjoinsel
    );

CREATE OPERATOR < (
    LEFTARG = url,
    RIGHTARG = url,
    PROCEDURE = urlLT,
    COMMUTATOR = >,
    NEGATOR = >= ,
    RESTRICT = scalarltsel,
    JOIN = scalarltjoinsel
    );

CREATE OPERATOR <= (
    LEFTARG = url,
    RIGHTARG = url,
    PROCEDURE = urlLE,
    COMMUTATOR = >=,
    NEGATOR = >,
    RESTRICT = scalarltsel,
    JOIN = scalarltjoinsel
    );

CREATE OPERATOR > (
    LEFTARG = url,
    RIGHTARG = url,
    PROCEDURE = urlGT,
    COMMUTATOR = < ,
    NEGATOR = <= ,
    RESTRICT = scalargtsel,
    JOIN = scalargtjoinsel
    );

CREATE OPERATOR >= (
    LEFTARG = url,
    RIGHTARG = url,
    PROCEDURE = urlGE,
    COMMUTATOR = <= ,
    NEGATOR = < ,
    RESTRICT = scalargtsel,
    JOIN = scalargtjoinsel
    );

CREATE OPERATOR CLASS url_ops
    DEFAULT FOR TYPE url USING btree AS
    OPERATOR 1 < ,
    OPERATOR 2 <= ,
    OPERATOR 3 = ,
    OPERATOR 4 >= ,
    OPERATOR 5 > ,
    FUNCTION 1 cmpUrls(url, url);