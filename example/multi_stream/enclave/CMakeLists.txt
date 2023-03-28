set(T_SRCS eparser.cpp)

add_ypc_applet(tel_mapping
  CRYPTO stdeth
  SRCS ${T_SRCS})

enclave_sign(tel_mapping KEY enclave_private.pem
  CONFIG enclave.config.xml)
