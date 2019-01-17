//
// Created by roman on 9/1/19.
//

#pragma once

#include <boost/multi_index_container.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index/sequenced_index.hpp>
#include <boost/multi_index/mem_fun.hpp>
#include <boost/multi_index/identity.hpp>

#include <chrono>

#include <eth-crypto/crypto/Common.h>

namespace keychain_app {

struct private_key_item
{
  private_key_item(const dev::Secret& secret, int unlock_duration);
  dev::Public public_key() const;
  dev::Secret secret;
  std::chrono::system_clock::time_point unlock_time_point;
  int unlock_duration;
};

using private_key_map_t = boost::multi_index::multi_index_container<
  private_key_item,
  boost::multi_index::indexed_by<
    boost::multi_index::ordered_unique<boost::multi_index::const_mem_fun<private_key_item,dev::Public,&private_key_item::public_key> >,
    boost::multi_index::sequenced<>
  >
>;

}
