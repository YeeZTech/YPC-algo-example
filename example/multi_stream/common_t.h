#pragma once
#include <ff/net/middleware/ntpackage.h>
#include <ff/util/ntobject.h>

define_nt(imei, std::string);
define_nt(tel, std::string);
define_nt(label1, std::string);
define_nt(label2, std::string);
define_nt(label3, std::string);

typedef ff::util::ntobject<imei, tel> id_mapping_t;
typedef ff::util::ntobject<imei, label1, label2, label3> sdk_t;

typedef ff::net::ntpackage<0, ::tel> input_buf_t;
