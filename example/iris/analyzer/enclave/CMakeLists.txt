set(T_SRCS eparser.cpp)

add_enclave_library(iris_parser SRCS ${T_SRCS}
  EDL ${YPC_INSTALL_DIR}/include/ypc/edl/eparser.edl
  EDL_SEARCH_PATHS ${YPC_INSTALL_DIR}/include/ypc/edl/:${YPC_INSTALL_DIR}/include/ypc/stbox/
  )

target_link_libraries(iris_parser
  stbox_common_t stbox_channel_t analyzer_t
  gmssl_t stdeth_t stbox_crypto_t stdeth_ecc_t
  stbox_keccak_t gmssl_ecc_t
  stbox_channel_t
  stbox_gmssl_t
  stbox_secp256k1_t
  common_t
  sgx_trts
  sgx_tcxx
  sgx_tcrypto
  sgx_tservice
  sgx_tstdc
  )

if(SGX_MODE STREQUAL "Debug")
  enclave_sign(iris_parser KEY enclave_private.pem
    CONFIG enclave.config.debug.xml)
else()
  enclave_sign(iris_parser KEY enclave_private.pem
    CONFIG enclave.config.xml)
endif()
install(FILES ${PROJECT_SOURCE_DIR}/lib/iris_parser.signed.so DESTINATION lib COMPONENT c_example_iris OPTIONAL)
