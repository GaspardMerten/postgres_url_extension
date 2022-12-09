DROP table if exists table_with_url;
DROP extension if exists url;

CREATE extension url;
CREATE TABLE table_with_url
(
    my_url url
);


INSERT INTO table_with_url
VALUES (url_in('pp', 'www.norse.be.super', 2, '/doc/')),
       (url_in('http', 'www.google.be', '/doc/')),
        (url_in(url_in('https://a@www.google.be:2/duc/'), '/doc/#3')),
       ('https://gaspardmertenpremierdunometavecuntreslongnom@www.norse.be.super:4242/lul/?x#4'),
       ('http://www.norse.be.super:4242/lal/?x#4'),
       ('https://gaspardmertenpremierdunometavecuntreslongnom@www.norse.be.super:4242/doc/#4'),
       ('https://w@x.be/a/?x#4'),
       ('httpsx://www.norse.be.super:4242/duc/?x#4');


CREATE INDEX spec_index ON table_with_url (my_url) WHERE samehost(my_url, 'https://www.google.be:2/duc/');
-- CREATE INDEX path_index ON table_with_url (my_url) WHERE samepath(my_url,'https://a@www.google.be:2/doc/');
CREATE INDEX index3 ON table_with_url (my_url) WHERE samefile(my_url, 'httpsx://www.norse.be.super:4242/duc/');
CREATE INDEX path_index ON table_with_url (getpath(my_url));
CREATE INDEX authority_index ON table_with_url (getauthority(my_url));
CREATE INDEX url_index ON table_with_url (my_url);
SET enable_seqscan TO off;

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
VACUUM ANALYSE;
EXPLAIN ANALYSE
SELECT my_url
FROM table_with_url
WHERE sameFile(my_url, 'httpsx://www.norse.be.super:4242/duc/');
EXPLAIN     ANALYSE
SELECT my_url
FROM table_with_url
WHERE sameHost(my_url, 'https://google.com/ULB/SYSTDATA/path%27');
-- EXPLAIN ANALYSE SELECT my_url FROM table_with_url WHERE equals(my_url, 'https://google.com/ULB/SYSTDATA/path%27');
EXPLAIN ANALYSE
SELECT my_url
from table_with_url
where samePath(my_url, 'httpsx://www.norse.be.super:4242/duc/');
EXPLAIN ANALYSE
SELECT my_url
from table_with_url
where samehost(my_url, 'httpsx://gaspardmertenpremierdunometavecuntreslongnom:4242/duc/');
EXPLAIN ANALYSE
SELECT my_url
from table_with_url
where equals(my_url, 'httpsx://www.norse.be.super:4242/duc/');

SET enable_seqscan TO on;
