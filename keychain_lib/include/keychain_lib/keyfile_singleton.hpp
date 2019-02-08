//
// Created by roman on 5/1/19.
//

#pragma once

#include <boost/multi_index_container.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index/sequenced_index.hpp>
#include <boost/multi_index/member.hpp>
#include <boost/multi_index/mem_fun.hpp>
#include <boost/multi_index/tag.hpp>
#include <boost/multi_index/identity.hpp>

#include <fc_light/exception/exception.hpp>

#include <iterator>

#include "keyfile_parser.hpp"

struct sqlite3;

namespace keychain_app {

namespace keyfiles_map {

enum struct index_te {
  null = 0,
  public_key,
  keyname,
  date
};
/*
using keyfile_map_t = boost::multi_index::multi_index_container<
  keyfile_format::keyfile_t,
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
    >
  >
>;
*/
/*
struct db_iterator
{
  db_iterator(index_te itype);
  virtual ~db_iterator();
  using self_traits = std::iterator_traits<keyfile_format::keyfile_t*>;
  using iterator_category = self_traits::iterator_category;
  using value_type = self_traits::value_type;
  using difference_type = self_traits::difference_type;
  using pointer = self_traits::pointer;
  using reference = self_traits::reference;
  const reference operator*() const;
  const pointer operator->() const;
  db_iterator& operator++();
  db_iterator& operator--();
  db_iterator operator++(int);
  db_iterator operator--(int);
  db_iterator operator+();
  db_iterator operator-();
  db_iterator& operator+=();
  db_iterator& operator-=();
  reference operator[](difference_type n) const;
};

template <index_te itype>
bool operator==(const db_iterator<itype>& x, const db_iterator<itype>& y);

template <index_te itype>
bool operator<(const db_iterator<itype>& x, const db_iterator<itype>& y);

template <index_te itype>
bool operator!=(const db_iterator<itype>& x, const db_iterator<itype>& y);

template <index_te itype>
bool operator>(const db_iterator<itype>& x, const db_iterator<itype>& y);

template <index_te itype>
bool operator>=(const db_iterator<itype>& x, const db_iterator<itype>& y);

template <index_te itype>
bool operator<=(const db_iterator<itype>& x, const db_iterator<itype>& y);

template <index_te itype>
db_iterator<itype> operator-(const db_iterator<itype>& x, const db_iterator<itype>& y);

template <index_te itype>
db_iterator<itype> operator+(const db_iterator<itype>& x, const db_iterator<itype>& y);
*/

#define KEYFILES_DEFAULT_DB "keyfiles.db"

}

//NOTE: This database connection is not threadsafe, because it is not necessary
class database_connection
{
  database_connection(const char* db_name);
  virtual ~database_connection();
private:
  enum table_check_result: uint8_t {
    CHECK_NULL = 0,
    CHECK_KEYFILES_TABLE = 0x01,
    CHECK_SIGNLOGS_TABLE = 0x10,
    CHECK_FULL = 0x11
  };
  static constexpr const char* check_keyfiles_tables =
    "SELECT name AS keyfiles_table_name FROM sqlite_master WHERE type='table' AND name='{keyfiles}';\
     SELECT name AS signlogs_table_name FROM sqlite_master WHERE type='table' AND name='{sign_logs}'";
  static constexpr const char* create_keyfiles_tables =
    "CREATE ...;\
     CREATE ...";//TODO:
  static int callback(void *checker_value, int columns_num, char **row, char **column_name);
  sqlite3 *m_db;
};

/*
template <index_te itype>
struct index
{

};
*/

}

//using keyfiles_map::keyfile_map_t;

class keyfile_singleton
{
  keyfile_singleton();
  ~keyfile_singleton();
  
  using value_t = keyfile_map_t::value_type;
  keyfile_map_t m_keydata_map;
  
  using prim_index_type = keyfile_map_t::index<keyfiles_map::prim_pubkey_tag>::type;
  using second_index_type = keyfile_map_t::index<keyfiles_map::second_keyname_tag>::type;
  using third_index_type = keyfile_map_t::index<keyfiles_map::third_date_tag>::type;
  
  using prim_key_type = keyfile_map_t::key_type;
  using second_key_type = second_index_type::key_type;
  using third_key_type = third_index_type::key_type;
  
  void flush_keyfile_impl(const value_t& keyfile_data) const;
public:
  static keyfile_singleton& instance();
  
  using iterator = keyfile_map_t::iterator; //primary_index
  using const_iterator = keyfile_map_t::const_iterator; //primary_index
  
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
  
  const keyfile_format::keyfile_t& operator[](const prim_key_type& key) const; //NOTE: use for searching by public key
  const keyfile_format::keyfile_t& operator[](const second_key_type& key) const; //NOTE: use for searchin by keyname
  void insert(keyfile_format::keyfile_t&& keyfile_data); //NOTE: keyfile_t (fc_light::variant) is not support move semantic
  
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
  void flush_all() const;
};

}