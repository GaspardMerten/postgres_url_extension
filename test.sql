DROP table if exists table_with_url;
DROP extension if exists url;



CREATE extension url;
CREATE TABLE table_with_url
(
    my_url url
);

CREATE INDEX am ON table_with_url (my_url) WHERE equals(my_url, url_in('https://www.google.be'));

INSERT INTO table_with_url
VALUES ('https://gaspardmertenpremierdunometavecuntreslongnom@www.norse.be.super:4242/doc/?x#4'),
       ('http://www.norse.be.super:4242/doc/?x#4'),
       ('https://gaspardmertenpremierdunometavecuntreslongnom@www.norse.be.super:4242/doc/#4'),
       ('https://w@x.be/doc/?x#4'),
       ('httpsx://www.norse.be.super:4242/doc/?x#4');
SELECT *
from table_with_url;
SELECT getscheme(my_url),
       getusername(my_url),
       getauthority(my_url),
       getport(my_url),
       getpath(my_url),
       getquery(my_url),
       getref(my_url)
FROM table_with_url;
VACUUM ANALYSE ;
EXPLAIN ANALYSE SELECT equals(r.my_url, t.my_url)
FROM table_with_url r
         LEFT OUTER JOIN  table_with_url t on true;


EXPLAIN ANALYSE SELECT r.my_url
from table_with_url r;