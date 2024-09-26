#include "user_type.h"
#include<toolkit/plugins/csv/csv_reader.h>
typedef ypc::plugins::typed_csv_reader<reader_item_t> reader_reader_t;
impl_csv_reader(reader_reader_t)