#include "../common.h"
#include "../common_t.h"
#include "ypc/core/byte.h"
#include "ypc/corecommon/package.h"
#include <algorithm>
#include <ctime>
#include <iostream>
#include <unordered_map>

char gen_random_ch(char ch, int size) { return ch + std::rand() % size; }
char gen_random_digit() { return gen_random_ch('0', 10); }
char gen_random_alphbet() { return gen_random_ch('a', 26); }

std::string gen_random_num(int size) {
  std::string s;
  for (int i = 0; i < size; i++) {
    s += gen_random_digit();
  }
  return s;
}

// why std::swap not work
void swap(id_mapping_t &a, id_mapping_t &b) {
  id_mapping_t tmp;
  tmp = a;
  a = b;
  b = tmp;
}

void sort_by_imei(std::vector<id_mapping_t> &id_mapping, int left, int right) {
  int i = left;
  int j = right;
  if (i == j) {
    return;
  }
  id_mapping_t pivot = id_mapping[left + (right - left) / 2];
  auto pimei = pivot.get<::imei>();
  while (i <= j) {
    while (id_mapping[i].get<::imei>() < pimei) {
      i++;
    }
    while (pimei < id_mapping[j].get<::imei>()) {
      j--;
    }
    if (i <= j) {
      swap(id_mapping[i], id_mapping[j]);
      i++;
      j--;
    }
  }
  if (left < j) {
    sort_by_imei(id_mapping, left, j);
  }
  if (i < right) {
    sort_by_imei(id_mapping, i, right);
  }
}

void gen_id_mapping(uint64_t start_tel, int num,
                    std::vector<id_mapping_t> &id_mapping) {
  for (int i = 0; i < num; i++) {
    std::string tel = std::to_string(start_tel + i);
    id_mapping_t im;
    im.set<::tel>(tel);
    im.set<::imei>(gen_random_num(15));
    id_mapping.push_back(im);
  }
  sort_by_imei(id_mapping, 0, id_mapping.size() - 1);
}

void gen_sdk(const std::vector<id_mapping_t> &id_mapping, float ratio,
             std::vector<sdk_t> &sdk) {
  for (auto &it : id_mapping) {
    sdk_t s;
    if (std::rand() < 0x7fffffff * ratio) {
      s.set<::imei>(it.get<::imei>());
      s.set<::label1>(gen_random_num(2));
      s.set<::label2>(gen_random_num(2));
      s.set<::label3>(gen_random_num(2));
      sdk.push_back(s);
    }
  }
}

void show(const std::vector<id_mapping_t> &id_mapping,
          const std::vector<sdk_t> &sdk) {
  for (auto &it : id_mapping) {
    std::cout << it.get<::imei>() << ',' << it.get<::tel>() << std::endl;
  }
  for (auto &it : sdk) {
    std::cout << it.get<::imei>() << ',' << it.get<::label1>() << ','
              << it.get<::label2>() << ',' << it.get<::label3>() << std::endl;
  }
  std::cout << "id_mapping size: " << id_mapping.size() << std::endl;
  std::cout << "sdk size: " << sdk.size() << std::endl;
}

template <typename NT>
void save_to_file(const std::string &path, const std::vector<NT> &rows) {
  file_t f;
  f.open_for_write(path.c_str());
  for (auto &row : rows) {
    typename ypc::cast_obj_to_package<NT>::type pt = row;
    auto buf = ypc::make_bytes<ypc::bytes>::for_package(pt);
    f.append_item(buf.data(), buf.size());
  }
  f.close();
}

int main() {
  std::srand(std::time(nullptr));

  std::vector<id_mapping_t> id_mapping;
  gen_id_mapping(13000000000, 10000, id_mapping);

  std::vector<sdk_t> sdk;
  gen_sdk(id_mapping, 0.618, sdk);

  save_to_file("id_mapping.sealed", id_mapping);
  save_to_file("sdk.sealed", sdk);

  show(id_mapping, sdk);
  return 0;
}
