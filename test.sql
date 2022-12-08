DROP table if exists urlnew;
DROP EXTENSION IF EXISTS url CASCADE;
CREATE extension url;


CREATE TABLE urlnew(my_url url);
CREATE INDEX am ON urlnew (my_url) WHERE equals(my_url, url_in('https://www.google.be/%27'));
CREATE INDEX in2 ON urlnew (my_url);

INSERT INTO urlnew VALUES ('https://olloooo@google.be:900/doc/?x#4'),
       ('ftp://www.norse.be.super:4242/doc/?x#4%27'),
       ('http://www.norse.be.super:4242/doc/?x#4%27'),
       ('https://www.norse.be.super:4242/doc/#4%27'),
       ('https://x.be/doc/?x#4%27'),
       ('sambaaa://www.norse.be.super:4242/doc/?x#4');

SELECT getdefaultport(my_url) FROM urlnew;
SELECT getport(my_url) FROM urlnew;




