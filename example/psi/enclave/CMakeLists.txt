set(SRCS psi_parser.cpp)
add_ypc_applet(psi_parser
  CRYPTO stdeth
  SRCS ${SRCS})
if(SGX_MODE STREQUAL "Debug")
  enclave_sign(psi_parser KEY enclave_private.pem
    CONFIG enclave.config.debug.xml)
else()
  enclave_sign(psi_parser KEY enclave_private.pem
    CONFIG enclave.config.xml)
endif()
