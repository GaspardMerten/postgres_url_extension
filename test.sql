DROP table if exists table_with_url;
DROP extension if exists url;

CREATE extension url;
CREATE TABLE table_with_url
(
    my_url url
);

-- CREATE INDEX imdex1 ON table_with_url (my_url) WHERE geturl(my_url) = 'https://www.google.be';
-- CREATE INDEX imdex2 ON table_with_url (my_url) WHERE getpath(my_url) ='/doc/';
-- CREATE INDEX index3 ON table_with_url (my_url) WHERE equals(my_url, url_in('https://www.google.be/%27'));
-- CREATE INDEX imdex4 ON table_with_url (my_url);
CREATE INDEX url_index ON table_with_url(my_url);
SET enable_seqscan TO off;

INSERT INTO table_with_url
VALUES (url_in('pp','www.norse.be.super',2,'/doc/')),
       (url_in('http','www.norse.be.super','/doc/')),
       (url_in(url_in('https://a@www.google.be:2/duc/'),'doc/#3')),
       ('https://gaspardmertenpremierdunometavecuntreslongnom@www.norse.be.super:4242/lul/?x#4'),
       ('http://www.norse.be.super:4242/lal/?x#4'),
       ('https://gaspardmertenpremierdunometavecuntreslongnom@www.norse.be.super:4242/doc/#4'),
       ('https://w@x.be/a/?x#4'),
       ('httpsx://www.norse.be.super:4242/doc/?x#4');
SELECT *
from table_with_url;
SELECT getscheme(my_url),
       getUserinfo(my_url),
       getauthority(my_url),
       getport(my_url),
       getpath(my_url),
       getquery(my_url),
       getref(my_url)
FROM table_with_url;
VACUUM ANALYSE ;
EXPLAIN ANALYSE SELECT my_url FROM table_with_url WHERE sameFile(my_url, 'https://google.com/ULB/SYSTDATA/path%27');
EXPLAIN ANALYSE SELECT my_url FROM table_with_url WHERE sameHost(my_url, 'https://google.com/ULB/SYSTDATA/path%27');
EXPLAIN ANALYSE SELECT my_url FROM table_with_url WHERE equals(my_url, 'https://google.com/ULB/SYSTDATA/path%27');

-- EXPLAIN ANALYSE SELECT equals(r.my_url, t.my_url)
--         FROM table_with_url r
--                  LEFT OUTER JOIN  table_with_url t on true;
-- EXPLAIN ANALYSE SELECT r.my_url
--         from table_with_url r
--         where geturl(my_url) = 'https://gaspardmertenpremierdunometavecuntreslongnom@www.norse.be.super:4242/doc/#4';
-- EXPLAIN ANALYSE SELECT r.my_url
--         from table_with_url r
--         where getpath(my_url) = '/doc/';
-- SELECT getdefaultport(my_url) FROM table_with_url;
-- SELECT getport(my_url) FROM table_with_url;



SET enable_seqscan TO on;
DROP INDEX IF EXISTS url_index;