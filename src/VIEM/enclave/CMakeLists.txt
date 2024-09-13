set(T_SRCS eparser.cpp)

add_enclave_library(viem_parser SRCS ${T_SRCS}
  EDL ${YPC_INCLUDE_DIR}/ypc/edl/eparser.edl
  EDL_SEARCH_PATHS ${YPC_INCLUDE_DIR}/ypc/edl/:${YPC_INCLUDE_DIR}/ypc/edl/util/:${YPC_INCLUDE_DIR}/ypc/stbox/
  )

target_link_libraries(viem_parser PRIVATE
  YPC::stbox_common_t
  YPC::analyzer_t
  YPC::stdeth_t
  ff_net
  )

enclave_sign(viem_parser KEY enclave_private.pem
  CONFIG enclave.config.xml)
