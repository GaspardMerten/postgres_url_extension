cmake_minimum_required(VERSION 3.16.3)
project(dbms_extension C)

set(CMAKE_C_STANDARD 99)

add_executable(dbms_extension url.c src/models.c)

INCLUDE_DIRECTORIES(
        "/usr/local/pgsql/include/server "
)