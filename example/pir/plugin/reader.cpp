#include "user_type.h"
#include<toolkit/plugins/csv/csv_reader.h>
typedef ypc::plugins::typed_csv_reader<business_item_t> business_reader_t;
impl_csv_reader(business_reader_t)
