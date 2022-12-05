-- complain if script is sourced in psql, rather than via CREATE EXTENSION
\echo Use "CREATE EXTENSION url" to load this file. \quit


CREATE FUNCTION url_in(cstring)
    RETURNS url
AS '$libdir/url'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION url_out(url)
    RETURNS cstring
AS '$libdir/url'
LANGUAGE C IMMUTABLE STRICT;

CREATE TYPE url (
  INPUT          = url_in,
  OUTPUT         = url_out,
  INTERNALLENGTH = -1
);

CREATE FUNCTION getAuthority(url) RETURNS text
    IMMUTABLE
    STRICT
    LANGUAGE C
    AS '$libdir/url';

CREATE FUNCTION getDefaultPort(url) RETURNS text
    IMMUTABLE
    STRICT
    LANGUAGE C
AS '$libdir/url';

CREATE FUNCTION getFile(url) RETURNS text
    IMMUTABLE
    STRICT
    LANGUAGE C
AS '$libdir/url';

CREATE FUNCTION getHost(url) RETURNS text
    IMMUTABLE
    STRICT
    LANGUAGE C
AS '$libdir/url';

CREATE FUNCTION getPath(url) RETURNS text
    IMMUTABLE
    STRICT
    LANGUAGE C
AS '$libdir/url';

CREATE FUNCTION getPort(url) RETURNS text
    IMMUTABLE
    STRICT
    LANGUAGE C
AS '$libdir/url';

CREATE FUNCTION getProtocol(url) RETURNS text
    IMMUTABLE
    STRICT
    LANGUAGE C
AS '$libdir/url';

CREATE FUNCTION getQuery(url) RETURNS text
    IMMUTABLE
    STRICT
    LANGUAGE C
AS '$libdir/url';