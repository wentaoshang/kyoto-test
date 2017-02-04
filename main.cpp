#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include <kcpolydb.h>

#include <ndn-cxx/name.hpp>

namespace kc = kyotocabinet;

int main() {
  kc::PolyDB db;

  if (!db.open("test.kct", kc::PolyDB::OWRITER | kc::PolyDB::OCREATE)) {
    std::cerr << "open error: " << db.error().name() << std::endl;
  }

  ndn::Name n1("/a/b1/1");
  ndn::Name n2("/a/b1/2");
  ndn::Name n3("/a/b2/0");

  auto insert_name = [&db](const ndn::Name& n) {
    const auto& block = n.wireEncode();
    std::string uri = n.toUri();
    db.add(reinterpret_cast<const char*>(block.value()), block.value_size(),
           uri.data(), uri.size());
  };

  insert_name(n1);
  insert_name(n2);
  insert_name(n3);

  {
    const auto& block = n2.wireEncode();
    std::unique_ptr<char[]> value;
    size_t value_size;

    value.reset(db.get(reinterpret_cast<const char*>(block.value()),
                       block.value_size(), &value_size));

    if (value != nullptr) {
      std::cout << std::string(value.get(), value_size) << std::endl;
    } else {
      std::cerr << "get error: " << db.error().name() << std::endl;
    }
  }

  {
    std::unique_ptr<kc::DB::Cursor> cur(db.cursor());
    cur->jump();
    std::string ckey, cvalue;
    while (cur->get(&ckey, &cvalue, true)) {
      std::cout << cvalue << std::endl;
    }
  }

  {
    ndn::Name prefix("/a/b1");
    const auto& block = prefix.wireEncode();
    std::vector<std::string> results;
    auto r = db.match_prefix(
        {reinterpret_cast<const char*>(block.value()), block.value_size()},
        &results);
    std::cout << r << std::endl;
    // for (const auto& k : results) {
    //   ndn::Name temp;
    //   temp.wireDecode({k.data(), k.size()});
    //   std::cout << temp.toUri() << std::endl;
    // }
  }

  {
    ndn::Name prefix("/a/b");
    const auto& block = prefix.wireEncode();
    std::vector<std::string> results;
    auto r = db.match_prefix(
        {reinterpret_cast<const char*>(block.value()), block.value_size()},
        &results);
    std::cout << r << std::endl;
    // for (const auto& k : results) {
    //   ndn::Name temp;
    //   temp.wireDecode({k.data(), k.size()});
    //   std::cout << temp.toUri() << std::endl;
    // }
  }

  if (!db.close()) {
    std::cerr << "close error: " << db.error().name() << std::endl;
  }

  return 0;
}
