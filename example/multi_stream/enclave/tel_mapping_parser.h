#include "../common_t.h"
#include "ypc/core_t/analyzer/data_source.h"
#include "ypc/corecommon/data_source.h"
#include "ypc/corecommon/package.h"
#include "ypc/corecommon/to_type.h"
#include "ypc/stbox/ebyte.h"
#include "ypc/stbox/stx_common.h"
#include "ypc/stbox/tsgx/log.h"
#include <hpda/extractor/raw_data.h>
#include <hpda/output/memory_output.h>
#include <hpda/processor/query/filter.h>
#include <hpda/processor/set/intersection.h>
#include <hpda/processor/transform/concat.h>

typedef ff::util::ntobject<::imei, ::tel, ::label1, ::label2, ::label3>
    output_obj_t;

class tel_mapping_parser {
public:
  tel_mapping_parser() {}
  tel_mapping_parser(
      std::vector<std::shared_ptr<ypc::data_source_with_dhash>> &source)
      : m_datasources(source){};

  inline stbox::bytes do_parse(const stbox::bytes &param) {
    LOG(INFO) << "do parse";
    auto pkg = ypc::make_package<input_buf_t>::from_bytes(param);
    if (m_datasources.size() != 2) {
      return stbox::bytes("Should include two data sources");
    }

    // set intersection
    hpda::processor::internal::ordered_intersection_impl<output_obj_t,
                                                         std::string>
        oi;
    ypc::to_type<stbox::bytes, id_mapping_t> conv_im(m_datasources[0].get());
    oi.add_upper_stream<::imei>(&conv_im);
    ypc::to_type<stbox::bytes, sdk_t> conv_sdk(m_datasources[1].get());
    oi.add_upper_stream<::imei>(&conv_sdk);

    // find by `tel`
    hpda::processor::internal::filter_impl<output_obj_t> match(
        &oi, [&](const output_obj_t &v) {
          if (pkg.get<::tel>() == v.get<::tel>()) {
            return true;
          }
          return false;
        });

    hpda::output::internal::memory_output_impl<output_obj_t> mo(&match);
    mo.get_engine()->run();
    LOG(INFO) << "do parse done";

    stbox::bytes result;
    if (mo.values().empty()) {
      result = stbox::bytes("not found!");
    }
    for (auto &it : mo.values()) {
      result += it.get<::tel>();
      result += ",";
      result += it.get<::imei>();
      result += ",";
      result += it.get<::label1>();
      result += ",";
      result += it.get<::label2>();
      result += ",";
      result += it.get<::label3>();
    }
    return result;
  }

protected:
  std::vector<std::shared_ptr<ypc::data_source_with_dhash>> m_datasources;
};
