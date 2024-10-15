#pragma once
#include <ff/util/ntobject.h>
#include <string>

define_nt(id, std::string);
define_nt(company_id, std::string);
typedef ff::util::ntobject<id, company_id> a_item_t;

define_nt(income, std::string);
define_nt(tax, std::string);
define_nt(month, std::string);
typedef ff::util::ntobject<id, company_id, income, tax, month> b_item_t;
