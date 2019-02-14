//
// Created by roman on 5/1/19.
//

#pragma once

#include <boost/multi_index_container.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index/sequenced_index.hpp>
#include <boost/multi_index/random_access_index.hpp>
#include <boost/multi_index/member.hpp>
#include <boost/multi_index/mem_fun.hpp>
#include <boost/multi_index/tag.hpp>
#include <boost/multi_index/identity.hpp>

#include <fc_light/exception/exception.hpp>

#include "keyfile_parser.hpp"

namespace keychain_app {

namespace keyfiles_map {

struct prim_pubkey_tag {};
struct second_keyname_tag {};
struct third_date_tag {};
struct log_date_tag {};
struct key_random_access_tag {};
struct log_random_access_tag {};


using keyfile_map_t = boost::multi_index::multi_index_container<
  keychain_app::keyfile_format::keyfile_t,
  boost::multi_index::indexed_by<
    boost::multi_index::ordered_unique<
      boost::multi_index::tag <prim_pubkey_tag>,
      boost::multi_index::const_mem_fun<keyfile_format::keyfile_t, const dev::Public&, &keyfile_format::keyfile_t::public_key>
    >,
    boost::multi_index::ordered_non_unique<
      boost::multi_index::tag <second_keyname_tag>,
      boost::multi_index::member<keyfile_format::keyfile_t, std::string, &keyfile_format::keyfile_t::keyname>
    >,
    boost::multi_index::ordered_non_unique<
      boost::multi_index::tag <third_date_tag>,
      boost::multi_index::const_mem_fun<keyfile_format::keyfile_t, const fc_light::time_point&, &keyfile_format::keyfile_t::last_date>
    >,
    boost::multi_index::random_access<
      boost::multi_index::tag<key_random_access_tag>
    >
  >
>;

using log_records_t = boost::multi_index::multi_index_container<
  keyfile_format::log_record,
  boost::multi_index::indexed_by<
    boost::multi_index::ordered_non_unique<
      boost::multi_index::tag<log_date_tag>,
      boost::multi_index::member<keyfile_format::log_record, fc_light::time_point, &keyfile_format::log_record::sign_time>
    >,
    boost::multi_index::random_access<
      boost::multi_index::tag<log_random_access_tag>
    >
  >
>;

using signlog_map_t = std::map<dev::Public, log_records_t>;

}

using keyfiles_map::keyfile_map_t;
using keyfiles_map::signlog_map_t;
using keyfiles_map::log_records_t;

class keyfile_singleton
{
  keyfile_singleton();
  ~keyfile_singleton();
  
  using value_t = keyfile_map_t::value_type;
  keyfile_map_t m_keydata_map;
  signlog_map_t m_signlog_map;
  
  using prim_index_type = keyfile_map_t::index<keyfiles_map::prim_pubkey_tag>::type;
  using second_index_type = keyfile_map_t::index<keyfiles_map::second_keyname_tag>::type;
  using third_index_type = keyfile_map_t::index<keyfiles_map::third_date_tag>::type;
  using log_random_access_index_type = log_records_t::index<keyfiles_map::log_random_access_tag>::type;
  using log_date_index_type = log_records_t::index<keyfiles_map::log_date_tag>::type;
  using key_random_access_index_type = keyfile_map_t::index<keyfiles_map::key_random_access_tag>::type;
  
  using prim_key_type = keyfile_map_t::key_type;
  using second_key_type = second_index_type::key_type;
  using third_key_type = third_index_type::key_type;
  
  void flush_keyfile_impl(const value_t& keyfile_data) const;
  void flush_logrecords_impl(const prim_key_type& key, const log_records_t& log_records) const;
  
  void keydata_initialize();
  void signlog_initialize();
  
  void print_exception (const boost::filesystem::path& filename, fc_light::exception &er);
public:
  static keyfile_singleton& instance();
  
  using iterator = keyfile_map_t::iterator; //primary_index
  using const_iterator = keyfile_map_t::const_iterator; //primary_index
  
  const log_random_access_index_type& get_logs(const dev::Public& pkey);
  const log_date_index_type& get_logs_date_ordered(const dev::Public& pkey);
  void add_log_record(const dev::Public& pkey, const keyfile_format::log_record& record);
  
  const_iterator begin() const; //primary_index
  const_iterator end() const; //primary_index
  
  // NOTE: You can get all indexes
  // If you want to implement search in STL style you can get any index and call find function for every selected index
  // example code:
  // auto it = keyfile_map.second_index().find(keyname);
  // if (it == keyfile_map.second_index().end())
  //   return <- element not found result ->
  
  // For sorting record by key use begin/end function STL like way
  // auto it = keyfile_map.third_index().begin();
  // for (; it != keyfile_map.third_index().end(); ++it)
  // {
  //   const keyfile_t& key_file = *it;
  //   some_print_function(keyfile);
  // }
  const prim_index_type& prim_index() const;
  const second_index_type& second_index() const;
  const third_index_type& third_index() const;
  const key_random_access_index_type& random_access_index() const;
  
  const keyfile_format::keyfile_t& operator[](const prim_key_type& key); //NOTE: use for searching by public key
  const keyfile_format::keyfile_t& operator[](const second_key_type& key); //NOTE: use for searchin by keyname
  const keyfile_format::keyfile_t& operator[](size_t index); //NOTE: random_access_index for key table
  void insert(keyfile_format::keyfile_t&& keyfile_data); //NOTE: keyfile_t (fc_light::variant) is not support move semantic
  
  using create_f = std::function<keyfile_format::keyfile_t()>;
  bool create(create_f&& creator);
  
  using unlock_f = std::function<bool(const keyfile_format::keyfile_t&)>;
  bool remove(const dev::Public& pkey, unlock_f&& unlocker);
  
  template <typename modifier_f>
  void update(const prim_key_type& key, modifier_f&& f)
  {
    auto it = m_keydata_map.find(key);
    if (it == m_keydata_map.end())
      FC_LIGHT_THROW_EXCEPTION(fc_light::key_not_found_exception, "Private key is not exist, public = ${key_}", ("key_", key));
    auto keyfile = *it;
    f(keyfile);
    //NOTE: I don't want to use boost multi_index modify function because it is unsafe
    //If the rearrangement (reindexing) fails, the element is erased.
    //For example, modifier functor may modyfy prim unique key so that element will become non unique
    //I want to exclude a sequence where a user's private key can be deleted as a result of an internal keychain error
    auto res = m_keydata_map.replace(it, std::move(keyfile));
    if (!res)
      FC_LIGHT_THROW_EXCEPTION(fc_light::internal_error_exception, "Cannot replace keyfile in map, public = ${key_}", ("key_", key));
    flush_keyfile(key);
  };
  
  template <typename modifier_f>
  void update(const second_key_type& key, modifier_f&& f)
  {
    auto& ind = m_keydata_map.get<keyfiles_map::second_keyname_tag>();
    auto it = ind.find(key);
    if (it == ind.end())
      FC_LIGHT_THROW_EXCEPTION(fc_light::key_not_found_exception, "Private key is not exist, keyname = ${key_}", ("key_", key));
    auto keyfile = *it;
    f(keyfile);
    //NOTE: I don't want to use boost multi_index modify function because it is unsafe
    //If the rearrangement (reindexing) fails, the element is erased.
    //For example, modifier functor may modyfy prim unique key so that element will become non unique
    //I want to exclude a sequence where a user's private key can be deleted as a result of an internal keychain error
    auto res = ind.replace(it, std::move(keyfile));
    if (!res)
      FC_LIGHT_THROW_EXCEPTION(fc_light::internal_error_exception, "Cannot replace keyfile in map, keyname = ${key_}", ("key_", key));
    flush_keyfile(key);
  };
  
  void update(keyfile_format::keyfile_t&& keyfile);
  
  bool is_exist(const prim_key_type& key) const;
  void flush_keyfile(const prim_key_type& key) const;
  void flush_keyfile(const second_key_type& key) const;
  void flush_logrecords(const prim_key_type& key) const;
  void flush_all() const;
};

using get_password_create_f = std::function<byte_seq_t(const std::string&)>; //NOTE: may incapsulate call to sec module or just return password string

keyfile_format::keyfile_t create_new_keyfile(
  const std::string& keyname,
  const std::string& description,
  bool encrypted,
  keyfile_format::cipher_etype cipher,
  keyfile_format::curve_etype curve,
  get_password_create_f&& get_passwd);

using get_password_f = std::function<std::pair<byte_seq_t, bool>(const std::string&, bool no_password)>; //NOTE: may incapsulate call to sec module or just return password string

bool remove_unlock(const keyfile_format::keyfile_t& keyfile, get_password_f&& get_passwd);

}