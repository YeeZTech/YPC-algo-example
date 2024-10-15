#pragma once
#include <ff/util/ntobject.h>
#include <string>
define_nt(Series_reference, std::string);
define_nt(Period, std::string);
define_nt(Data_value, std::string);
define_nt(Suppressed, std::string);
define_nt(STATUS, std::string);
define_nt(UNITS, std::string);
define_nt(Magnitude, std::string);
define_nt(Subject, std::string);
define_nt(Group, std::string);
define_nt(Series_title_1, std::string);
define_nt(Series_title_2, std::string);
define_nt(Series_title_3, std::string);
define_nt(Series_title_4, std::string);
define_nt(Series_title_5, std::string);
typedef ff::util::ntobject<Series_reference,Period,Data_value,Suppressed,STATUS,UNITS,Magnitude,Subject,Group,Series_title_1,Series_title_2,Series_title_3,Series_title_4,Series_title_5> business_item_t;
