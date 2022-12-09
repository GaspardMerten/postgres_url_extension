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

CREATE FUNCTION getUrl(url) RETURNS text
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
CREATE FUNCTION getDefaultPort(url) RETURNS int8
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

CREATE FUNCTION getUserinfo(url) RETURNS text
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

CREATE FUNCTION getProtocol(url) RETURNS text
    IMMUTABLE
    STRICT
    LANGUAGE C
AS
'$libdir/url';

CREATE FUNCTION getPort(url) RETURNS int8
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

CREATE FUNCTION equals(url1 url, url2 url) RETURNS bool
    IMMUTABLE
    STRICT
    LANGUAGE sql
as 'SELECT geturl(url1) = geturl(url2)';

CREATE FUNCTION sameProtocol(url1 url, url2 url) RETURNS bool
    IMMUTABLE
    STRICT
    LANGUAGE sql
as 'SELECT getprotocol(url1) = getprotocol(url2)';

CREATE FUNCTION sameUser(url1 url, url2 url) RETURNS bool
    IMMUTABLE
    STRICT
    LANGUAGE sql
as 'SELECT getUserinfo(url1) = getUserinfo(url2)';

CREATE FUNCTION sameHost(url1 url, url2 url) RETURNS bool
    IMMUTABLE
    STRICT
    LANGUAGE sql
as 'SELECT getauthority(url1) = getauthority(url2)';

CREATE FUNCTION samePort(url1 url, url2 url) RETURNS bool
    IMMUTABLE
    STRICT
    LANGUAGE sql
as 'SELECT getPort(url1) = getPort(url2)';

CREATE FUNCTION samePath(url1 url, url2 url) RETURNS bool
    IMMUTABLE
    STRICT
    LANGUAGE sql
as 'SELECT getpath(url1) = getpath(url2)';

CREATE FUNCTION sameFile(url1 url, url2 url) RETURNS bool
    IMMUTABLE
    STRICT
    LANGUAGE sql
as 'SELECT getfile(url1) = getfile(url2)';

CREATE FUNCTION samQuery(url1 url, url2 url) RETURNS bool
    IMMUTABLE
    STRICT
    LANGUAGE sql
as 'SELECT getquery(url1) = getquery(url2)';

CREATE FUNCTION sameRef(url1 url, url2 url) RETURNS bool
    IMMUTABLE
    STRICT
    LANGUAGE sql
as 'SELECT getref(url1) = getref(url2)';

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