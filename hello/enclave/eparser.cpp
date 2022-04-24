#include "corecommon/crypto/stdeth.h"
#include "hello.h"
#include "ypc_t/analyzer/algo_wrapper.h"
#include "ypc_t/analyzer/macro.h"

ypc::algo_wrapper<ypc::crypto::eth_sgx_crypto, ypc::noinput_data_stream, hello,
                  ypc::local_result>
    pw;

YPC_PARSER_IMPL(pw);
