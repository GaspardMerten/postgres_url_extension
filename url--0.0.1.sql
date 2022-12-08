-- complain if script is sourced in psql, rather than via CREATE EXTENSION
\echo Use "CREATE EXTENSION url" to load this file. \quit


CREATE FUNCTION url_in(cstring)
    RETURNS url
AS
'$libdir/url'
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

CREATE FUNCTION getFile(url) RETURNS text
    IMMUTABLE
    STRICT
    LANGUAGE C
AS
'$libdir/url';

CREATE FUNCTION getUrl(url) RETURNS text
    IMMUTABLE
    STRICT
    LANGUAGE C
AS
'$libdir/url';


CREATE FUNCTION equals(url1 url, url2 url) RETURNS bool
    IMMUTABLE
    STRICT
    LANGUAGE sql
as 'SELECT geturl(url1) = geturl(url2)';

CREATE FUNCTION sameFile(url1 url, url2 url) RETURNS bool
    IMMUTABLE
    STRICT
    LANGUAGE sql
as 'SELECT getfile(url1) = getfile(url2)';


CREATE FUNCTION sameHost(url1 url, url2 url) RETURNS bool
    IMMUTABLE
    STRICT
    LANGUAGE sql
    as 'SELECT getauthority(url1) = getauthority(url2)';


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
    FUNCTION 1 cmpUrls(url, url)
;


