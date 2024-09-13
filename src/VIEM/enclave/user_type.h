#pragma once
#include <ff/util/ntobject.h>
#include <stdint.h>
#include <string>

define_nt(image_item, std::string);

typedef ff::util::ntobject<image_item>
    image_item_t;
