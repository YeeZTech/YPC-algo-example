set(SRCS pir_parser.cpp)
add_ypc_applet(pir_parser
  CRYPTO stdeth
  SRCS ${SRCS})
if(SGX_MODE STREQUAL "Debug")
  enclave_sign(pir_parser KEY enclave_private.pem
    CONFIG enclave.config.debug.xml)
else()
  enclave_sign(pir_parser KEY enclave_private.pem
    CONFIG enclave.config.xml)
endif()
