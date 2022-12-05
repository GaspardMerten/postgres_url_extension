DROP table if exists table_with_url;
DROP extension if exists url;

CREATE extension url;
CREATE TABLE table_with_url(my_url url);

INSERT INTO table_with_url VALUES (url('https://www.google.be/doc/?x#4'));
SELECT getauthority(my_url) FROM table_with_url;
