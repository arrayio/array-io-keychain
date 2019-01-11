//
// Created by roman on 9/1/19.
//

#pragma once

#include <boost/multi_index_container.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index/sequenced_index.hpp>
#include <boost/multi_index/member.hpp>
#include <boost/multi_index/identity.hpp>

#include <chrono>

#include <eth-crypto/crypto/Common.h>

namespace keychain_app {

struct private_key_item
{
  private_key_item(const std::string& keyname, const dev::Secret& secret, int unlock_duration);
  std::string keyname;
  dev::Secret secret;
  std::chrono::system_clock::time_point unlock_time_point;
  int unlock_duration;
};

using private_key_map_t = boost::multi_index::multi_index_container<
  private_key_item,
  boost::multi_index::indexed_by<
    boost::multi_index::ordered_unique<boost::multi_index::member<private_key_item,std::string,&private_key_item::keyname> >,
    boost::multi_index::sequenced<>
  >
>;

}
