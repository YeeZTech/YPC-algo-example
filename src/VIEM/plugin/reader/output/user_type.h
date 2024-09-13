#pragma once
#include <ff/util/ntobject.h>
#include <string>
define_nt(name, std::string);
define_nt(age, int);
typedef ff::util::ntobject<name,age> reader_item_t;
