#include "user_type.h"
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

#include <sgx_tcrypto.h>
#include <sgx_trts.h>

define_nt(params_pir, std::string);
typedef ff::net::ntpackage<0, ::params_pir> input_buf_t;

std::vector<std::string> split(const std::string &str,
                               const std::string &delim) {
  std::vector<std::string> res;
  if ("" == str) {
    return res;
  }
  std::string::size_type s = 0;
  std::string::size_type e = str.find(delim, s);
  while (e != str.npos) {
    auto ts = std::string(&str[s], e - s);
    res.push_back(ts);
    s = e + 1;
    e = str.find(delim, s);
  }
  res.push_back(std::string(&str[s], str.size() - s));
  return res;
}

class pir_parser {
public:
  pir_parser() {}
  pir_parser(
      std::vector<std::shared_ptr<ypc::data_source_with_dhash>> &source)
      : m_datasources(source) {
  };

  inline stbox::bytes do_parse(const stbox::bytes &param) {
    LOG(INFO) << "do parse";
    LOG(INFO) << "param: " << param;
    std::string pir_params(param.size(), '0');
    memcpy(&pir_params[0], param.data(), param.size());
    std::vector<std::string> items = split(pir_params, ",");
    if (items.size() != 2) {
      return stbox::bytes("invalid params!");
    }
    std::string series_reference = items[0];
    std::string period = items[1];

    LOG(INFO) << "datasources size: " << m_datasources.size();
    typedef ypc::nt<stbox::bytes> ntt;
    hpda::processor::concat<ntt::data> concator(m_datasources[0].get());
    for (size_t i = 1; i < m_datasources.size(); i++) {
      concator.add_upper_stream(m_datasources[i].get());
    }
    LOG(INFO) << "add all upper streams";
    ypc::to_type<stbox::bytes, pir_item_t> converter(&concator);
    hpda::processor::internal::filter_impl<pir_item_t> match(
        &converter, [&](const pir_item_t &v) {
          auto sr = v.get<::Series_reference>();
          auto p = v.get<::Period>();
          if (sr == series_reference && p == period) {
            return true;
          }
          return false;
        });
    hpda::output::internal::memory_output_impl<pir_item_t> mo(&match);
    LOG(INFO) << "start to run";
    mo.get_engine()->run();
    LOG(INFO) << "do parse done";

    std::string result;
    bool flag = false;
    for (auto &it : mo.values()) {
      flag = true;
      result += (it.get<::Series_reference>() + ",");
      result += (it.get<::Period>() + ",");
      result += (it.get<::Data_value>() + ",");
      result += (it.get<::Suppressed>() + ",");
      result += (it.get<::STATUS>() + ",");
      result += (it.get<::UNITS>() + ",");
      result += (it.get<::Magnitude>() + ",");
      result += (it.get<::Subject>() + ",");
      result += (it.get<::Group>() + ",");
      result += (it.get<::Series_title_1>() + ",");
      result += (it.get<::Series_title_2>() + ",");
      result += (it.get<::Series_title_3>() + ",");
      result += (it.get<::Series_title_4>() + ",");
      result += (it.get<::Series_title_5>() + "\n");
    }
    if (!flag) {
      return stbox::bytes("not found!");
    }
    return stbox::bytes(result);
  }

protected:
  std::vector<std::shared_ptr<ypc::data_source_with_dhash>> m_datasources;
};
