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