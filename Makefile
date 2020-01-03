MODULES = pg_ed25519
EXTENSION = pg_ed25519
DATA = pg_ed25519--0.0.1.sql
PG_CONFIG = pg_config
PGXS := $(shell $(PG_CONFIG) --pgxs)
include $(PGXS)
