#include "postgres.h"
#include "fmgr.h"
#include "ed25519.h"

PG_MODULE_MAGIC;
PG_FUNCTION_INFO_V1(ed25519_verify);

Datum
ed25519_verify(PG_FUNCTION_ARGS)
{
    bool res;

    bytea *arg_sig;
    bytea *arg_key;
    bytea *arg_msg;

    arg_sig = PG_GETARG_BYTEA_PP(0);
    arg_key = PG_GETARG_BYTEA_PP(1);
    arg_msg = PG_GETARG_BYTEA_PP(2);

    if (VARSIZE_ANY_EXHDR(arg_sig) != 64) {
        ereport(ERROR,
                (errcode(ERRCODE_INVALID_PARAMETER_VALUE),
                 errmsg("ed25519_verify: 64 bytes signature expected, "
                        "got %ld bytes signature",
                        VARSIZE_ANY_EXHDR(arg_sig))));
    }

    if (VARSIZE_ANY_EXHDR(arg_key) != 32) {
        ereport(ERROR,
                (errcode(ERRCODE_INVALID_PARAMETER_VALUE),
                 errmsg("ed25519_verify: 32 bytes public key expected, "
                        "got %ld bytes public key",
                        VARSIZE_ANY_EXHDR(arg_key))));
    }
    res = 0 == ed25519_sign_open(
        (const unsigned char *) VARDATA_ANY(arg_msg),
        VARSIZE_ANY_EXHDR(arg_msg),
        (const unsigned char *) VARDATA_ANY(arg_key),
        (const unsigned char *) VARDATA_ANY(arg_sig)
    );

    PG_FREE_IF_COPY(arg_sig, 0);
    PG_FREE_IF_COPY(arg_key, 1);
    PG_FREE_IF_COPY(arg_msg, 2);

    PG_RETURN_BOOL(res);
}
