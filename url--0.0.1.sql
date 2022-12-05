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

-- ********************* OPERATOR AND INDEX *************************************
CREATE OPERATOR = (
	LEFTARG = url,
	RIGHTARG = url,
	PROCEDURE = url_eq,
	COMMUTATOR = '=',
	NEGATOR = '<>',
	RESTRICT = eqsel,
	JOIN = eqjoinsel
);
COMMENT ON OPERATOR =(url, url) IS 'equals?';

CREATE OPERATOR <> (
	LEFTARG = url,
	RIGHTARG = url,
	PROCEDURE = url_ne,
	COMMUTATOR = '<>',
	NEGATOR = '=',
	RESTRICT = neqsel,
	JOIN = neqjoinsel
);
COMMENT ON OPERATOR <>(url, url) IS 'not equals?';

CREATE OPERATOR < (
	LEFTARG = url,
	RIGHTARG = url,
	PROCEDURE = url_lt,
	COMMUTATOR = > , 
	NEGATOR = >= ,
   	RESTRICT = scalarltsel, 
	JOIN = scalarltjoinsel
);
COMMENT ON OPERATOR <(url, url) IS 'less-than';

CREATE OPERATOR <= (
	LEFTARG = url,
	RIGHTARG = url,
	PROCEDURE = url_le,
	COMMUTATOR = >= , 
	NEGATOR = > ,
   	RESTRICT = scalarltsel, 
	JOIN = scalarltjoinsel
);
COMMENT ON OPERATOR <=(url, url) IS 'less-than-or-equal';

CREATE OPERATOR > (
	LEFTARG = url,
	RIGHTARG = url,
	PROCEDURE = url_gt,
	COMMUTATOR = < , 
	NEGATOR = <= ,
   	RESTRICT = scalargtsel, 
	JOIN = scalargtjoinsel
);
COMMENT ON OPERATOR >(url, url) IS 'greater-than';

CREATE OPERATOR >= (
	LEFTARG = url,
	RIGHTARG = url,
	PROCEDURE = url_ge,
	COMMUTATOR = <= , 
	NEGATOR = < ,
   	RESTRICT = scalargtsel, 
	JOIN = scalargtjoinsel
);
COMMENT ON OPERATOR >=(url, url) IS 'greater-than-or-equal';

CREATE OPERATOR CLASS btree_url_ops
DEFAULT FOR TYPE url USING btree
AS
        OPERATOR        1       <  ,
        OPERATOR        2       <= ,
        OPERATOR        3       =  ,
        OPERATOR        4       >= ,
        OPERATOR        5       >  ,
        FUNCTION        1       url_cmp(url, url);
