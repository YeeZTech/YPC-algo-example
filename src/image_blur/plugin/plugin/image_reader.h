#pragma once
#define STB_IMAGE_IMPLEMENTATION  // 在使用stb_image时需要定义这个宏
#include "../../include/stb_image.h"
#include "ypc/common/limits.h"
#include "ypc/corecommon/package.h"
#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/ptree.hpp>
#include <ff/util/ntobject.h>
#include <fstream>
#include <iostream>
#include <memory>
#include <stdint.h>


#ifdef __cplusplus
extern "C" {
#endif

void *create_item_reader(const char *extra_param, int len);

int reset_for_read(void *handle);
int read_item_data(void *handle, char *buf, int *len);
int close_item_reader(void *handle);
uint64_t get_item_number(void *handle);

#ifdef __cplusplus
}
#endif

namespace plugin {

class reader_plugin {
public:
  virtual int reset_for_read() = 0;
  virtual int read_item_data(char *buf, int *len) = 0;
  virtual int close_item_reader() = 0;
  virtual int get_item_number() = 0;
};

// template <typename NT> struct ntobject_size { const static size_t size = 0; };
// template <typename T1> struct ntobject_size<::ff::util::ntobject<T1>> {
//   const static size_t size = 1;
// };
// template <typename T1, typename... ARGS>
// struct ntobject_size<::ff::util::ntobject<T1, ARGS...>> {
//   const static size_t size =
//       1 + ntobject_size<::ff::util::ntobject<ARGS...>>::size;
// };

define_nt(data_batch, std::string);
constexpr static uint32_t max_item_size = 64 * 1024;

struct StbImageDeleter {
    void operator()(unsigned char* ptr) const {
        stbi_image_free(ptr);
    }
};

class image_reader_plugin : public reader_plugin {
public:

  image_reader_plugin(const std::string &extra_param)
      : m_extra_param(extra_param), m_file_path(extra_param), 
      m_current_pos(0), m_max_batch_size(max_item_size - 12), m_stream_size(0), width(0), height(0), channels(0),
      m_stream(nullptr), is_open(false) {

    // 读取图像数据到buffer中，返回值是一个char*，表示图像数据的内存地址
    m_stream.reset(stbi_load(m_file_path.c_str(), &width, &height, &channels, 0));
    if (!m_stream) {
        std::cerr << "Error: Failed to load image." << std::endl;
    }
    m_stream_size = width * height * channels;
    is_open = true;
  }

  virtual int reset_for_read() {
    if(m_stream != nullptr) {
      stbi_image_free(m_stream.get());
    }
    m_stream.reset(stbi_load(m_file_path.c_str(), &width, &height, &channels, 0));
    m_current_pos = 0;
    m_stream_size = width * height * channels;
    if (!m_stream) {
      return -1;
    }
    is_open = true;
    return 0;
  }

  virtual int read_item_data(char *buf, int *len) {
    if(!is_open) {
      std::cerr << "Error: image_reader is not open" << std::endl;
      return 1;
    }
    // read batch
    if (m_current_pos >= m_stream_size) {
      std::cerr << "All data has been copied." << std::endl;
      return 1;
    }

    size_t bytes_to_copy = std::min(m_max_batch_size, m_stream_size - m_current_pos);
    bytes_to_copy = std::min(static_cast<size_t>(width * channels), bytes_to_copy);
    
    std::string s(bytes_to_copy, '0');
    s.assign(reinterpret_cast<char*>(m_stream.get() + m_current_pos), bytes_to_copy);
    m_current_pos += bytes_to_copy;

    // serialize
    typedef ff::net::ntpackage<0, plugin::data_batch> package_t;
    package_t v;
    v.set<plugin::data_batch>(s);
    if (len) {
      ff::net::marshaler lm(ff::net::marshaler::length_retriver);
      v.arch(lm);
      *len = static_cast<int>(lm.get_length());
    }
    if (buf) {
      ff::net::marshaler sm(buf, *len, ff::net::marshaler::serializer);
      v.arch(sm);
    }
    return 0;
  }

  virtual int close_item_reader() {
    is_open = false;
    return 0;
  }

  virtual int get_item_number() {
    if(!is_open) {
      std::cerr << "Error: image_reader is not open" << std::endl;
      return 0;
    }
    size_t batch_size = std::min(m_max_batch_size, static_cast<size_t>(width * channels));
    size_t n = m_stream_size / batch_size;
    if (m_stream_size % batch_size) {
      n++;
    }
    return n;
  }

protected:
  bool is_open;
  int width, height, channels;
  size_t m_current_pos; 
  size_t m_max_batch_size;
  size_t m_stream_size;
  const std::string m_extra_param;
  std::string m_file_path;
  std::unique_ptr<unsigned char, StbImageDeleter> m_stream;
};
} // namespace plugin

#define impl_reader_plugin(type)                                                    \
  void *create_item_reader(const char *extra_param, int len) {                      \
    try {                                                                           \
      plugin::reader_plugin *reader = new type(std::string(extra_param, len));      \
      return reader;                                                                \
    } catch (const std::exception &e) {                                             \
      std::cout << "create_item_reader got exception: " << e.what()                 \
                << std::endl;                                                       \
      return nullptr;                                                               \
    }                                                                               \
  }                                                                                 \
  int reset_for_read(void *handle) {                                                \
    plugin::reader_plugin *reader = (plugin::reader_plugin *)handle;                \
    return reader->reset_for_read();                                                \
  }                                                                                 \
  int read_item_data(void *handle, char *buf, int *len) {                           \
    plugin::reader_plugin *reader = (plugin::reader_plugin *)handle;                \
    return reader->read_item_data(buf, len);                                        \
  }                                                                                 \
  int close_item_reader(void *handle) {                                             \
    plugin::reader_plugin *reader = (plugin::reader_plugin *)handle;                \
    reader->close_item_reader();                                                    \
    delete reader;                                                                  \
    return 0;                                                                       \
  }                                                                                 \
                                                                                    \
  uint64_t get_item_number(void *handle) {                                          \
    plugin::reader_plugin *reader = (plugin::reader_plugin *)handle;                \
    return reader->get_item_number();                                               \
  }                                                                                 
                                                                                  
