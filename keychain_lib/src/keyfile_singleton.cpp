//
// Created by roman on 5/1/19.
//

#include "keychain_commands.hpp"
#include "keyfile_singleton.hpp"
#include <cryptopp/osrng.h>
#include <cryptopp/hex.h>

using namespace keychain_app;

keyfile_singleton& keyfile_singleton::instance()
{
  static keyfile_singleton keyfile_map;
  return keyfile_map;
}

keyfile_singleton::const_iterator keyfile_singleton::begin() const
{
  return m_keydata_map.begin();
}

keyfile_singleton::const_iterator keyfile_singleton::end() const
{
  return m_keydata_map.end();
}

const keyfile_singleton::prim_index_type& keyfile_singleton::prim_index() const
{
  return m_keydata_map.get<keyfiles_map::prim_pubkey_tag>();
}

const keyfile_singleton::second_index_type& keyfile_singleton::second_index() const
{
  return m_keydata_map.get<keyfiles_map::second_keyname_tag>();
}

const keyfile_singleton::third_index_type& keyfile_singleton::third_index() const
{
  return m_keydata_map.get<keyfiles_map::third_date_tag>();
}

const keyfile_singleton::key_random_access_index_type& keyfile_singleton::random_access_index() const
{
  return m_keydata_map.get<keyfiles_map::key_random_access_tag>();
}

void keyfile_singleton::keydata_load()
{
#if defined(macintosh) || defined(__APPLE__) || defined(__APPLE_CC__)
  auto key_dir = bfs::path(getenv("HOME"));
  key_dir += bfs::path("/" KEY_DEFAULT_PATH_);
#else
  bfs::path key_dir(KEY_DEFAULT_PATH_);
#endif

  if(!bfs::exists(key_dir))
  {
      auto res = bfs::create_directories(key_dir);
      if(res == false)
          FC_LIGHT_THROW_EXCEPTION(fc_light::internal_error_exception,
                                     "Can not create key directory, path = ${directory}", ("directory", key_dir.string()));
  }

  auto first = bfs::directory_iterator(key_dir);
  std::for_each(first, bfs::directory_iterator(), [this](const auto& unit) {
    try {
      fc_light::variant j_keyfile = open_keyfile(unit.path().c_str());
      auto keyfile = j_keyfile.as<keyfile_format::keyfile_t>();
      auto& ind = prim_index();
      auto it = ind.find(keyfile.public_key());
      if(it == ind.end())
        m_keydata_map.insert(keyfile);
      else
        m_keydata_map.replace(it, keyfile);
    }
    catch (fc_light::parse_error_exception& er) {
      return print_exception(unit.path(), er);
    }
    catch (fc_light::bad_cast_exception& er) {
      return print_exception(unit.path(), er);
    }
    FC_LIGHT_RETHROW_EXCEPTIONS(log_level::error, "Cannot parse key file \"${KEYFILE}\"", ("KEYFILE", unit.path().c_str()))
  });
}

/*
void keyfile_singleton::signlog_load()
{
#if defined(macintosh) || defined(__APPLE__) || defined(__APPLE_CC__)
  auto dir = bfs::path(getenv("HOME"));
  dir += bfs::path("/" SIGN_LOGS_DEFAULT_PATH_);
#else
  bfs::path dir(SIGN_LOGS_DEFAULT_PATH_);
#endif

  if(!bfs::exists(dir))
  {
        auto res = bfs::create_directories(dir);
        if(res == false)
            FC_LIGHT_THROW_EXCEPTION(fc_light::internal_error_exception,
                                     "Can not create sign_logs directory, path = ${directory}", ("directory", dir.string()));
  }

  auto first = bfs::directory_iterator(dir);
  std::for_each(first, bfs::directory_iterator(), [this](const auto& unit) {
    try {
      fc_light::variant j_keyfile = open_keyfile(unit.path().c_str());
      auto file = j_keyfile.as<keyfile_format::signlog_file_t>();
      auto found_it = m_signlog_map.find(file.public_key);
      auto it = m_signlog_map.end();
      if(found_it == m_signlog_map.end())
      {
        auto res = m_signlog_map.insert(signlog_map_t::value_type(file.public_key,log_records_t()));
        FC_LIGHT_ASSERT(res.second);
        it = res.first;
      } else
      {
        found_it->second.clear();
        it = found_it;
      }
      auto& logmap = it->second;
      std::copy(file.sign_events.begin(), file.sign_events.end(), std::inserter(logmap, logmap.begin()));
    }
    catch (fc_light::parse_error_exception& er) {
      return print_exception(unit.path(), er);
    }
    catch (fc_light::bad_cast_exception& er) {
      return print_exception(unit.path(), er);
    }
    FC_LIGHT_RETHROW_EXCEPTIONS(log_level::error, "Cannot parse key file \"${KEYFILE}\"", ("KEYFILE", unit.path().c_str()))
  });
}
*/

void keyfile_singleton::print_exception (const boost::filesystem::path& filename, fc_light::exception &er)
{
  auto& log = logger_singleton::instance();
  BOOST_LOG_SEV(log.lg, warning) << "Cannot read key file \"" << filename.c_str() << "\"" << er.to_detail_string();
};

keyfile_singleton::keyfile_singleton()
{
  
  try
  {
    keydata_load();
//    signlog_load();
  }
  FC_LIGHT_RETHROW_EXCEPTIONS(log_level::error, "Cannot create keyfiles map")
}

keyfile_singleton::~keyfile_singleton()
{
  flush_all();
}

bool keyfile_singleton::create(create_f&& creator)
{
  auto& log = logger_singleton::instance();
  try {
    insert(std::move(creator()));
    return true;
  }
  catch (fc_light::exception& exc)
  {
    BOOST_LOG_SEV(log.lg, error) << "keyfile_singleton::create(), error = " << exc.to_detail_string();
  }
  catch (std::exception& exc)
  {
    BOOST_LOG_SEV(log.lg, error) << "keyfile_singleton::create(), error = " << exc.what();
  }
  return false;
}

bool keyfile_singleton::remove(const dev::Public& pkey, unlock_f&& unlock)
{
  auto& log = logger_singleton::instance();
  try {
    auto it = m_keydata_map.find(pkey);
    if (it == m_keydata_map.end())
      return false;
    if(!unlock(*it))
      return false;
    m_keydata_map.erase(it);
    return true;
  }
  catch (fc_light::exception& exc)
  {
    BOOST_LOG_SEV(log.lg, error) << "keyfile_singleton::create(), error = " << exc.to_detail_string();
  }
  catch (std::exception& exc)
  {
    BOOST_LOG_SEV(log.lg, error) << "keyfile_singleton::create(), error = " << exc.what();
  }
  return false;
}

const keyfile_format::keyfile_t& keyfile_singleton::operator[](size_t index)
{
  bool stop = false;
  do{
    auto& ra_ind = random_access_index();
    if( index >= ra_ind.size() )
    {
      if(stop == true)
        FC_LIGHT_THROW_EXCEPTION(fc_light::out_of_range_exception, "index = ${ind_}", ("ind_", index));
      keydata_load();
      bool stop = true;
    }
    return ra_ind[index];
  } while (true);
}

const keyfile_format::keyfile_t& keyfile_singleton::operator[](const keyfile_singleton::prim_key_type& key)
{
  bool stop = false;
  do{
    auto it = m_keydata_map.find(key);
    if (it == m_keydata_map.end())
    {
      if(stop == true)
        FC_LIGHT_THROW_EXCEPTION(fc_light::key_not_found_exception, "Private key is not exist public = ${key_}", ("key_", key));
      keydata_load();
      stop = true;
    }
    return *it;
  } while (true);
}

const keyfile_format::keyfile_t& keyfile_singleton::operator[](const keyfile_singleton::second_key_type& key)
{
  bool stop = false;
  do {
    auto &second_index = m_keydata_map.get<1>();
    auto it = second_index.find(key);
    if (it == second_index.end())
    {
      if(stop == true)
        FC_LIGHT_THROW_EXCEPTION(fc_light::key_not_found_exception, "Private key is not found by keyname = ${name_}",
                                 ("name_", key));
      keydata_load();
      stop = true;
    }
    return *it;
  } while(true);
}

void keyfile_singleton::update(keyfile_format::keyfile_t&& keyfile)
{
  auto it = m_keydata_map.find(keyfile.keyinfo.public_key);
  if (it == m_keydata_map.end())
    FC_LIGHT_THROW_EXCEPTION(fc_light::key_not_found_exception, "Private key is not exist, public = ${key_}", ("key_", keyfile.keyinfo.public_key));
  auto res = m_keydata_map.replace(it, keyfile);
  if (!res)
    FC_LIGHT_THROW_EXCEPTION(fc_light::internal_error_exception, "Cannot replace keyfile in map, public = ${key_}", ("key_", keyfile.keyinfo.public_key));
}

void keyfile_singleton::insert(keyfile_format::keyfile_t&& keyfile_data)
{
  auto res = m_keydata_map.insert(keyfile_data);
  if (res.second == false)
    FC_LIGHT_THROW_EXCEPTION(fc_light::internal_error_exception, "Keyfile with name \"${key_}\" is already exist", ("key_", keyfile_data.keyinfo.public_key));
  flush_keyfile(keyfile_data.keyinfo.public_key);
}

bool keyfile_singleton::is_exist(const keyfile_singleton::prim_key_type& key) const
{
  auto it = m_keydata_map.find(key);
  return it != m_keydata_map.end();
}

void keyfile_singleton::flush_keyfile(const keyfile_singleton::prim_key_type& key) const
{
  auto it = m_keydata_map.find(key);
  if (it == m_keydata_map.end())
    FC_LIGHT_THROW_EXCEPTION(fc_light::key_not_found_exception, "Private key is not exist public = ${key_}", ("key_", key));
  flush_keyfile_impl(*it);
}

void keyfile_singleton::flush_keyfile_impl(const value_t& keyfile_data) const
{
#if defined(macintosh) || defined(__APPLE__) || defined(__APPLE_CC__)
  auto filepath = bfs::path(getenv("HOME"));
  filepath += bfs::path("/" KEY_DEFAULT_PATH_"/");
#else
  bfs::path filepath(KEY_DEFAULT_PATH_"/");
#endif
  auto hash = dev::openssl::sha3(keyfile_data.keyinfo.public_key);
  auto filename = hash.hex().substr(0, 16);
  filename += ".json";
  filepath += bfs::path(std::string(filename));
  auto fout = std::ofstream(filepath.c_str());
  if (!fout.is_open())
    FC_LIGHT_THROW_EXCEPTION(fc_light::internal_error_exception, "Cannot open keyfile (${filename})", ("filename", filename));
  fout << fc_light::json::to_pretty_string(keyfile_data) << std::endl;
}

/*
void keyfile_singleton::flush_logrecords(const prim_key_type& key) const
{
  auto it = m_signlog_map.find(key);
  if (it == m_signlog_map.end())
    return;
  flush_logrecords_impl(key, it->second);
}
*/

/*
void keyfile_singleton::flush_logrecords_impl(const prim_key_type& key, const log_records_t& log_records) const
{
#if defined(macintosh) || defined(__APPLE__) || defined(__APPLE_CC__)
  auto filepath = bfs::path(getenv("HOME"));
  filepath += bfs::path("/" SIGN_LOGS_DEFAULT_PATH_"/");
#else
  bfs::path filepath(SIGN_LOGS_DEFAULT_PATH_"/");
#endif
  keyfile_format::signlog_file_t logfile;
  logfile.public_key = key;
  std::copy(log_records.begin(), log_records.end(), std::back_inserter(logfile.sign_events));
  auto hash = dev::openssl::sha3(key);
  auto filename = hash.hex().substr(0, 16);
  filename += "_signlog.json";
  filepath += std::string(filename);
  auto fout = std::ofstream(filepath.c_str());
  if (!fout.is_open())
    FC_LIGHT_THROW_EXCEPTION(fc_light::internal_error_exception, "Cannot open keyfile (${filename})", ("filename", filename));
  fout << fc_light::json::to_pretty_string(logfile) << std::endl;
}
*/

void keyfile_singleton::flush_all() const
{
#if defined(macintosh) || defined(__APPLE__) || defined(__APPLE_CC__)
  auto keyfile_dir = bfs::path(getenv("HOME"));
  keyfile_dir += bfs::path("/" KEY_DEFAULT_PATH_);
#else
  bfs::path keyfile_dir(KEY_DEFAULT_PATH_);
#endif
  auto first = bfs::directory_iterator(keyfile_dir);
  std::for_each(m_keydata_map.begin(), m_keydata_map.end(), [first, &keyfile_dir](const auto& keyfile_data) {
    auto hash = dev::openssl::sha3(keyfile_data.keyinfo.public_key);
    auto filename = hash.hex().substr(0, 16);
    filename += ".json";
    bfs::path filepath = keyfile_dir.c_str() + std::string(filename);
    auto fout = std::ofstream(filepath.c_str());
    if (!fout.is_open())
      FC_LIGHT_THROW_EXCEPTION(fc_light::internal_error_exception, "Cannot open keyfile (${filename})", ("filename", filename));
    fout << fc_light::json::to_pretty_string(keyfile_data) << std::endl;
  });
/*  std::for_each(m_signlog_map.begin(), m_signlog_map.end(), [this](const auto& val){
    flush_logrecords(val.first);
  });*/
}

const keyfile_singleton::log_random_access_index_type& keyfile_singleton::get_logs(const dev::Public& pkey)
{
    auto& sql = sql_singleton::instance();
    auto records =  sql.select_log(pkey);

    if (records.size() == 0)
        FC_LIGHT_THROW_EXCEPTION(fc_light::file_not_found_exception, "Public_key: ${PKEY}", ("PKEY", pkey));

    m_signlog_map.clear();
    m_signlog_map.insert(signlog_map_t::value_type(pkey, log_records_t()));
    auto it = m_signlog_map.begin();
    auto& logmap = it->second;
    std::copy(records.begin(), records.end(), std::inserter(logmap, logmap.begin()));
    return logmap.get<keyfiles_map::log_random_access_tag>();
}

/*
const keyfile_singleton::log_date_index_type& keyfile_singleton::get_logs_date_ordered(const dev::Public& pkey)
{
  signlog_load();//NOTE: it may be slowly, using sqlite and triggers is more preferable
  auto it = m_signlog_map.find(pkey);
  if (it == m_signlog_map.end())
    FC_LIGHT_THROW_EXCEPTION(fc_light::file_not_found_exception, "Public_key: ${PKEY}", ("PKEY", pkey));
  auto& records = it->second;
  return records.get<keyfiles_map::log_date_tag>();
}
*/

void keyfile_singleton::add_log_record(const dev::Public& pkey, const keyfile_format::log_record& record)
{
    auto& sql = sql_singleton::instance();
    sql.insert_log(pkey, record);
  /*
  auto it = m_signlog_map.find(pkey);
  if (it == m_signlog_map.end())
  {
    it = m_signlog_map.insert(m_signlog_map.begin(), std::pair<dev::Public, log_records_t>(pkey,log_records_t()));
    if (it == m_signlog_map.end())
      FC_LIGHT_THROW_EXCEPTION(fc_light::internal_error_exception, "Cannot insert log record, public_key: ${PKEY}", ("PKEY", pkey));
  }
  it->second.insert(record);
  flush_logrecords(pkey);
   */
}

keyfile_format::keyfile_t keychain_app::create_new_keyfile(
  const std::string& keyname,
  const std::string& description,
  bool encrypted,
  keyfile_format::cipher_etype cipher,
  keyfile_format::curve_etype curve,
  get_password_create_f&& get_passwd)
{
  keyfile_format::keyfile_t keyfile;
  dev::Secret priv_key;
  dev::Public pb_hex;
  dev::h256 hash;
  std::string filename;
  switch (curve)
  {
    case keyfile_format::curve_etype::secp256k1:
    {
      auto keys = dev::KeyPair::create();
      pb_hex = keys.pub();
      hash = dev::ethash::sha3_ethash(keys.pub());
      priv_key = keys.secret();
      filename    = hash.hex().substr(0,16);
      filename += ".json";
    }
      break;
    default:
    {
      FC_LIGHT_THROW_EXCEPTION(fc_light::invalid_arg_exception,
                               "Unsupported curve format, curve = ${type}", ("type", curve));
    }
  }
  
  if (encrypted)
  {
    auto passwd = get_passwd(keyname);//operation canceled exception need to be thrown into get_password functor
    
    if (passwd.empty())
      FC_LIGHT_THROW_EXCEPTION(fc_light::password_input_exception, "");
    auto& encryptor = encryptor_singleton::instance();
    auto enc_data = encryptor.encrypt_private_key(cipher, passwd, priv_key);
    keyfile.keyinfo.priv_key_data = fc_light::variant(enc_data);
    keyfile.keyinfo.encrypted = true;
  }
  else{
    keyfile.keyinfo.priv_key_data = fc_light::variant(priv_key);
    keyfile.keyinfo.encrypted = false;
  }
  
  keyfile.keyinfo.public_key = pb_hex;
  keyfile.keyname = keyname;
  keyfile.description = description;
  keyfile.creation_time = fc_light::time_point::now();
  keyfile.keychain_version = version_info::short_version();
  keyfile.filetype = keyfile_format::TYPE_KEY;
  keyfile.keyinfo.curve_type = curve;
  return keyfile;
}

dev::bytes keyfile_singleton::seed(dev::bytes& user_entropy)
{
    using namespace CryptoPP;
    SecByteBlock key(32);
    std::string k;

    OS_GenerateRandomBlock(false, key, k.size());

    HexEncoder hex(new StringSink(k));
    hex.Put(key, key.size());
    hex.MessageEnd();

    dev::bytes seed(32);
    auto res = from_hex(k, seed.data(), seed.size());
    seed.resize(res);

    return seed;
}


bool keychain_app::remove_unlock(const keyfile_format::keyfile_t& keyfile, get_password_f&& get_passwd)
{
  try {
    if(!keyfile.keyinfo.encrypted)
      return get_passwd(keyfile.keyname, true).second; //Need user approve without password entry
    auto passwd = get_passwd(keyfile.keyname, false).first;//operation canceled exception need to be thrown into get_password functor
    if (passwd.empty())
      FC_LIGHT_THROW_EXCEPTION(fc_light::password_input_exception, "");
    auto encrypted_data = keyfile.keyinfo.priv_key_data.as<keyfile_format::encrypted_data>();
    auto& encryptor = encryptor_singleton::instance();
    encryptor.decrypt_private_key(passwd, encrypted_data); //unlock verifing incapsulated here
    return true;
  }
  catch(fc_light::privkey_invalid_unlock& exc)
  {
    return false;
  }
  
  
}
