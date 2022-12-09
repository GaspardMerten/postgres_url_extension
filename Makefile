EXTENSION = url              # the extension name
DATA      = url--0.0.1.sql   # script files to install
MODULES   = url              # our c module file to build

LDFLAGS=-lrt

PG_CONFIG ?= pg_config
PGXS = $(shell $(PG_CONFIG) --pgxs)
include $(PGXS)
