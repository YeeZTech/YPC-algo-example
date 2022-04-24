#include "corecommon/to_type.h"
#include "stbox/ebyte.h"
#include "stbox/stx_common.h"
#include "stbox/tsgx/log.h"

class hello {
public:
  inline stbox::bytes do_parse(const stbox::bytes &param) {
    LOG(INFO) << "hello!";
    return stbox::bytes("hello!");
  }
};
