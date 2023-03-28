#include "../common_t.h"
#include "ypc/core/byte.h"
#include "ypc/core/filesystem.h"
#include "ypc/corecommon/package.h"
#include <boost/program_options.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/ptree.hpp>
#include <iostream>

boost::program_options::variables_map parse_command_line(int argc,
                                                         char *argv[]) {
  namespace bp = boost::program_options;
  bp::options_description all("Serialize Param");

  // clang-format off
  all.add_options()
    ("help", "help message")
    ("param", bp::value<std::string>(), "input param")
    ("output", bp::value<std::string>(), "output serialized result to file with JSON format");

  // clang-format on
  bp::variables_map vm;
  boost::program_options::store(bp::parse_command_line(argc, argv, all), vm);

  if (vm.count("help") != 0u) {
    std::cout << all << std::endl;
    exit(-1);
  }

  if (vm.count("param") == 0u) {
    std::cerr << "`param` should be specified!" << std::endl;
    exit(-1);
  }

  return vm;
}

int main(int argc, char *argv[]) {
  boost::program_options::variables_map vm;
  try {
    vm = parse_command_line(argc, argv);
  } catch (const std::exception &e) {
    std::cout << e.what() << std::endl;
    std::cout << "invalid cmd line parameters!" << std::endl;
    return -1;
  }

  auto tel = vm["param"].as<std::string>();
  input_buf_t buf;
  buf.set<::tel>(tel);
  auto b = ypc::make_bytes<ypc::bytes>::for_package(buf);

  if (vm.count("output") != 0u) {
    std::string output_path =
        ypc::complete_path(vm["output"].as<std::string>());

    boost::property_tree::ptree pt;
    std::stringstream ss;
    ss << b;
    pt.put("param", ss.str());
    boost::property_tree::json_parser::write_json(output_path, pt);
  } else {
    std::cout << b << std::endl;
  }

  return 0;
}
