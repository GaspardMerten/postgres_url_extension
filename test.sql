DROP table if exists table_with_url;
DROP extension if exists url;

CREATE extension url;
CREATE TABLE table_with_url
(
    my_url url
);

INSERT INTO table_with_url
VALUES ('https://gaspardmertenpremierdunometavecuntreslongnom@www.norse.be.super:4242/doc/?x#4'),
       ('http://www.norse.be.super:4242/doc/?x#4'),
       ('https://gaspardmertenpremierdunometavecuntreslongnom@www.norse.be.super:4242/doc/#4'),
       ('https://w@x.be/doc/?x#4'),
       ('httpsx://www.norse.be.super:4242/doc/?x#4');
SELECT getscheme(my_url),
       getusername(my_url),
       getauthority(my_url),
       getport(my_url),
       getpath(my_url),
       getquery(my_url),
       getref(my_url)
FROM table_with_url;
