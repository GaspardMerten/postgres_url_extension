DROP table if exists table_with_url;
DROP extension if exists url;



CREATE extension url;
CREATE TABLE table_with_url
(
    my_url url
);

/* Really working ? :) */
CREATE INDEX ra1 ON table_with_url (sameHost(url1 := table_with_url.my_url, url2 := table_with_url.my_url));
CREATE INDEX ra2 ON table_with_url (samefile(url1 := table_with_url.my_url, url2 := table_with_url.my_url));
CREATE INDEX ra3 ON table_with_url (equals(url1 := table_with_url.my_url, url2 := table_with_url.my_url));

INSERT INTO table_with_url
VALUES ('https://gaspardmertenpremierdunometavecuntreslongnom@www.norse.be.super:4242/doc/?x#4'),
       ('http://www.norse.be.super:4242/doc/?x#4'),
       ('https://gaspardmertenpremierdunometavecuntreslongnom@www.norse.be.super:4242/doc/#4'),
       ('https://w@x.be/doc/?x#4'),
       ('httpsx://www.norse.be.super:4242/doc/?x#4'), ('https://facebook.com'),('https://gaspardmertenpremierdunometavecuntreslongnom@www.norse.be.super:4242/doc/?x#4'),
       ('http://www.norse.be.super:4242/doc/?x#4'),
       ('https://gaspardmertenpremierdunometavecuntreslongnom@www.norse.be.super:4242/doc/#4'),
       ('https://w@x.be/doc/?x#4'),
       ('httpsx://www.norse.be.super:4242/doc/?x#4'), ('https://facebook.com'),('https://gaspardmertenpremierdunometavecuntreslongnom@www.norse.be.super:4242/doc/?x#4'),
       ('http://www.norse.be.super:4242/doc/?x#4'),
       ('https://gaspardmertenpremierdunometavecuntreslongnom@www.norse.be.super:4242/doc/#4'),
       ('https://w@x.be/doc/?x#4'),
       ('httpsx://www.norse.be.super:4242/doc/?x#4'), ('https://facebook.com'),('https://gaspardmertenpremierdunometavecuntreslongnom@www.norse.be.super:4242/doc/?x#4'),
       ('http://www.norse.be.super:4242/doc/?x#4'),
       ('https://gaspardmertenpremierdunometavecuntreslongnom@www.norse.be.super:4242/doc/#4'),
       ('https://w@x.be/doc/?x#4'),
       ('httpsx://www.norse.be.super:4242/doc/?x#4'), ('https://facebook.com'),('https://gaspardmertenpremierdunometavecuntreslongnom@www.norse.be.super:4242/doc/?x#4'),
       ('http://www.norse.be.super:4242/doc/?x#4'),
       ('https://gaspardmertenpremierdunometavecuntreslongnom@www.norse.be.super:4242/doc/#4'),
       ('https://w@x.be/doc/?x#4'),
       ('httpsx://www.norse.be.super:4242/doc/?x#4'), ('https://facebook.com'),('https://gaspardmertenpremierdunometavecuntreslongnom@www.norse.be.super:4242/doc/?x#4'),
       ('http://www.norse.be.super:4242/doc/?x#4'),
       ('https://gaspardmertenpremierdunometavecuntreslongnom@www.norse.be.super:4242/doc/#4'),
       ('https://w@x.be/doc/?x#4'),
       ('httpsx://www.norse.be.super:4242/doc/?x#4'), ('https://facebook.com'),('https://gaspardmertenpremierdunometavecuntreslongnom@www.norse.be.super:4242/doc/?x#4'),
       ('http://www.norse.be.super:4242/doc/?x#4'),
       ('https://gaspardmertenpremierdunometavecuntreslongnom@www.norse.be.super:4242/doc/#4'),
       ('https://w@x.be/doc/?x#4'),
       ('httpsx://www.norse.be.super:4242/doc/?x#4'), ('https://facebook.com'),('https://gaspardmertenpremierdunometavecuntreslongnom@www.norse.be.super:4242/doc/?x#4'),
       ('http://www.norse.be.super:4242/doc/?x#4'),
       ('https://gaspardmertenpremierdunometavecuntreslongnom@www.norse.be.super:4242/doc/#4'),
       ('https://w@x.be/doc/?x#4'),
       ('httpsx://www.norse.be.super:4242/doc/?x#4'), ('https://facebook.com'),('https://gaspardmertenpremierdunometavecuntreslongnom@www.norse.be.super:4242/doc/?x#4'),
       ('http://www.norse.be.super:4242/doc/?x#4'),
       ('https://gaspardmertenpremierdunometavecuntreslongnom@www.norse.be.super:4242/doc/#4'),
       ('https://w@x.be/doc/?x#4'),
       ('httpsx://www.norse.be.super:4242/doc/?x#4'), ('https://facebook.com'),('https://gaspardmertenpremierdunometavecuntreslongnom@www.norse.be.super:4242/doc/?x#4'),
       ('http://www.norse.be.super:4242/doc/?x#4'),
       ('https://gaspardmertenpremierdunometavecuntreslongnom@www.norse.be.super:4242/doc/#4'),
       ('https://w@x.be/doc/?x#4'),
       ('httpsx://www.norse.be.super:4242/doc/?x#4'), ('https://facebook.com'),('https://gaspardmertenpremierdunometavecuntreslongnom@www.norse.be.super:4242/doc/?x#4'),
       ('http://www.norse.be.super:4242/doc/?x#4'),
       ('https://gaspardmertenpremierdunometavecuntreslongnom@www.norse.be.super:4242/doc/#4'),
       ('https://w@x.be/doc/?x#4'),
       ('httpsx://www.norse.be.super:4242/doc/?x#4'), ('https://facebook.com'),('https://gaspardmertenpremierdunometavecuntreslongnom@www.norse.be.super:4242/doc/?x#4'),
       ('http://www.norse.be.super:4242/doc/?x#4'),
       ('https://gaspardmertenpremierdunometavecuntreslongnom@www.norse.be.super:4242/doc/#4'),
       ('https://w@x.be/doc/?x#4'),
       ('httpsx://www.norse.be.super:4242/doc/?x#4'), ('https://facebook.com'),('https://gaspardmertenpremierdunometavecuntreslongnom@www.norse.be.super:4242/doc/?x#4'),
       ('http://www.norse.be.super:4242/doc/?x#4'),
       ('https://gaspardmertenpremierdunometavecuntreslongnom@www.norse.be.super:4242/doc/#4'),
       ('https://w@x.be/doc/?x#4'),
       ('httpsx://www.norse.be.super:4242/doc/?x#4'), ('https://facebook.com'),('https://gaspardmertenpremierdunometavecuntreslongnom@www.norse.be.super:4242/doc/?x#4'),
       ('http://www.norse.be.super:4242/doc/?x#4'),
       ('https://gaspardmertenpremierdunometavecuntreslongnom@www.norse.be.super:4242/doc/#4'),
       ('https://w@x.be/doc/?x#4'),
       ('httpsx://www.norse.be.super:4242/doc/?x#4'), ('https://facebook.com'),('https://gaspardmertenpremierdunometavecuntreslongnom@www.norse.be.super:4242/doc/?x#4'),
       ('http://www.norse.be.super:4242/doc/?x#4'),
       ('https://gaspardmertenpremierdunometavecuntreslongnom@www.norse.be.super:4242/doc/#4'),
       ('https://w@x.be/doc/?x#4'),
       ('httpsx://www.norse.be.super:4242/doc/?x#4'), ('https://facebook.com'),('https://gaspardmertenpremierdunometavecuntreslongnom@www.norse.be.super:4242/doc/?x#4'),
       ('http://www.norse.be.super:4242/doc/?x#4'),
       ('https://gaspardmertenpremierdunometavecuntreslongnom@www.norse.be.super:4242/doc/#4'),
       ('https://w@x.be/doc/?x#4'),
       ('httpsx://www.norse.be.super:4242/doc/?x#4'), ('https://facebook.com'),('https://gaspardmertenpremierdunometavecuntreslongnom@www.norse.be.super:4242/doc/?x#4'),
       ('http://www.norse.be.super:4242/doc/?x#4'),
       ('https://gaspardmertenpremierdunometavecuntreslongnom@www.norse.be.super:4242/doc/#4'),
       ('https://w@x.be/doc/?x#4'),
       ('httpsx://www.norse.be.super:4242/doc/?x#4'), ('https://facebook.com'),('https://gaspardmertenpremierdunometavecuntreslongnom@www.norse.be.super:4242/doc/?x#4'),
       ('http://www.norse.be.super:4242/doc/?x#4'),
       ('https://gaspardmertenpremierdunometavecuntreslongnom@www.norse.be.super:4242/doc/#4'),
       ('https://w@x.be/doc/?x#4'),
       ('httpsx://www.norse.be.super:4242/doc/?x#4'), ('https://facebook.com'),('https://gaspardmertenpremierdunometavecuntreslongnom@www.norse.be.super:4242/doc/?x#4'),
       ('http://www.norse.be.super:4242/doc/?x#4'),
       ('https://gaspardmertenpremierdunometavecuntreslongnom@www.norse.be.super:4242/doc/#4'),
       ('https://w@x.be/doc/?x#4'),
       ('httpsx://www.norse.be.super:4242/doc/?x#4'), ('https://facebook.com'),('https://gaspardmertenpremierdunometavecuntreslongnom@www.norse.be.super:4242/doc/?x#4'),
       ('http://www.norse.be.super:4242/doc/?x#4'),
       ('https://gaspardmertenpremierdunometavecuntreslongnom@www.norse.be.super:4242/doc/#4'),
       ('https://w@x.be/doc/?x#4'),
       ('httpsx://www.norse.be.super:4242/doc/?x#4'), ('https://facebook.com'),('https://gaspardmertenpremierdunometavecuntreslongnom@www.norse.be.super:4242/doc/?x#4'),
       ('http://www.norse.be.super:4242/doc/?x#4'),
       ('https://gaspardmertenpremierdunometavecuntreslongnom@www.norse.be.super:4242/doc/#4'),
       ('https://w@x.be/doc/?x#4'),
       ('httpsx://www.norse.be.super:4242/doc/?x#4'), ('https://facebook.com');
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

EXPLAIN ANALYSE SELECT samehost(my_url, (Select my_url from table_with_url fetch first row only )) from table_with_url;
EXPLAIN ANALYSE SELECT samefile(my_url, (Select my_url from table_with_url fetch first row only )) from table_with_url;
EXPLAIN ANALYSE SELECT t1.my_url, twu.my_url from table_with_url t1 INNER JOIN table_with_url twu on t1.my_url = twu.my_url WHERE equals(t1.my_url,twu.my_url  );

EXPLAIN ANALYSE SELECT r.my_url
from table_with_url r;