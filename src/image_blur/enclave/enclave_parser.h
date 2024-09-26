#include "ypc/corecommon/package.h"
#include "ypc/stbox/ebyte.h"
#include "ypc/stbox/stx_common.h"
#ifdef EXAMPLE_FM_NORMAL
#include <glog/logging.h>
typedef ypc::bytes bytes;
#else
#include "ypc/core_t/analyzer/data_source.h"
#include "ypc/stbox/tsgx/log.h"
typedef stbox::bytes bytes;
#endif
#include "ypc/corecommon/data_source.h"
#include "ypc/corecommon/to_type.h"
#include <hpda/extractor/raw_data.h>
#include <hpda/output/memory_output.h>
#include <hpda/processor/query/filter.h>
#include <string.h>

define_nt(image_item, std::string);
define_nt(input_param, std::string);

typedef ff::util::ntobject<image_item> image_item_t;
typedef ff::net::ntpackage<0, input_param> input_param_t;

class enclave_parser {
public:
  enclave_parser() {}

  enclave_parser(ypc::data_source<bytes> *source) : m_source(source){}; // m_source：输入的数据源

  inline bytes do_parse(const bytes &param) {
    LOG(INFO) << "do parse";
    // 1. 将数据源反序列化，并转为定义的格式
    ypc::to_type<bytes, image_item_t> converter(m_source);
    // 2. 收集到整张图片再处理
    hpda::output::internal::memory_output_impl<image_item_t> mo(&converter);
    mo.get_engine()->run();
    
    std::vector<std::vector<double>> img;
    for (auto it : mo.values()) {
      std::vector<double> row;
      std::string data = it.get<image_item>();
      for(int i = 0; i < data.size(); ++i) {
        row.push_back(static_cast<double>(data[i]));
      }
      img.emplace_back(std::move(row));
    }
    // 准备参数
    size_t height = img.size();
    size_t width = img[0].size();
    LOG(INFO) << height << " " << width;
    
    auto raw_param = std::string((const char *)param.data(), param.size());
    LOG(INFO) << raw_param << " " << raw_param.size();
    std::vector<std::string> params = split(raw_param, ',');
    int size = std::stoi(params[0]);
    double sigam = std::stod(params[1]);

    // 用于存储模糊后的图像数据
    std::vector<std::vector<double>> blurred_img(height, std::vector<double>(width * 3, 0));

    // 生成高斯核
    std::vector<std::vector<double>> kernel = generate_gaussian_kernel(size, sigam);

    // 对图像应用高斯模糊
    apply_gaussian_blur(img, blurred_img, width, height, kernel);

    bytes result;
    for(int i = 0; i < img.size(); ++ i) {
      std::string r(img[i].size(), '0');
      for(int j = 0; j < img[i].size(); ++ j) {
        int pix = (static_cast<int>(blurred_img[i][j]) + 256) % 256;
        r[j] = static_cast<char>(pix);
      }
      result += bytes(r);
    }
    // LOG(INFO) << "result size: " << result.size();
    return result;
  }
  
  // 分割参数
  std::vector<std::string> split(const std::string& str, char delimiter) {
    std::vector<std::string> tokens;
    size_t start = 0;
    size_t end = str.find(delimiter);

    while (end != std::string::npos) {
        tokens.push_back(str.substr(start, end - start));  // 提取子串
        start = end + 1;  // 移动起始位置
        end = str.find(delimiter, start);  // 查找下一个分隔符
    }
    tokens.push_back(str.substr(start));  // 最后一个子串
    return tokens;
  }

  // 生成高斯核
  std::vector<std::vector<double>> generate_gaussian_kernel(int size, double sigma) {
      std::vector<std::vector<double>> kernel(size, std::vector<double>(size, 0));
      double sum = 0.0;
      int half_size = size / 2;

      // 计算高斯核
      for (int i = -half_size; i <= half_size; ++i) {
          for (int j = -half_size; j <= half_size; ++j) {
              double exponent = -(i * i + j * j) / (2 * sigma * sigma);
              kernel[i + half_size][j + half_size] = std::exp(exponent);
              sum += kernel[i + half_size][j + half_size];
          }
      }

      // 归一化核
      for (int i = 0; i < size; ++i) {
          for (int j = 0; j < size; ++j) {
              kernel[i][j] /= sum;
          }
      }

      return kernel;
  }

  // 应用高斯卷积核进行模糊
  void apply_gaussian_blur(const std::vector<std::vector<double>>& img, std::vector<std::vector<double>>& blurred_img, int width, int height, const std::vector<std::vector<double>>& kernel) {
      int kernel_size = kernel.size();
      int half_size = kernel_size / 2;

      // 对每个像素进行卷积
      for (int y = 0; y < height; ++y) {
          for (int x = 0; x < width; ++x) {
              double r_sum = 0, g_sum = 0, b_sum = 0;
              for (int ky = -half_size; ky <= half_size; ++ky) {
                  for (int kx = -half_size; kx <= half_size; ++kx) {
                      int px = std::min(std::max(x + kx, 0), width - 1);  // 边界处理
                      int py = std::min(std::max(y + ky, 0), height - 1);

                      double kernel_value = kernel[ky + half_size][kx + half_size];

                      // 对 RGB 进行加权
                      r_sum += img[py][px * 3] * kernel_value;
                      g_sum += img[py][px * 3 + 1] * kernel_value;
                      b_sum += img[py][px * 3 + 2] * kernel_value;
                  }
              }
              // 存储模糊后的值
              blurred_img[y][x * 3] = r_sum;
              blurred_img[y][x * 3 + 1] = g_sum;
              blurred_img[y][x * 3 + 2] = b_sum;
          }
      }
  }

protected:
  ypc::data_source<bytes> *m_source;
};
