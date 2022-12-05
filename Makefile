EXTENSION = url              # the extension name
DATA      = url--0.0.1.sql   # script files to install
REGRESS   = url_test         # our test script file (without extension)
MODULES   = url              # our c module file to build

# Postgres build stuff
PG_CONFIG = pg_config
PGXS := $(shell $(PG_CONFIG) --pgxs)
include $(PGXS)