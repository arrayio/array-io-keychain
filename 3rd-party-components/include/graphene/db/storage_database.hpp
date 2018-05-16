/*
 * storagedatabase.hpp
 *
 *  Created on: Sep 1, 2017
 *      (c) 2017 Array.io
 */

#pragma once

#include <array>
#include <deque>
#include <stack>
//#include <memory>
#include <unordered_map>

#include <fc/filesystem.hpp>

#include <graphene/db/lmdb_lite.hpp>
#include <graphene/db/object_id.hpp>

namespace graphene { namespace db {

class undo_database;

// handles persistent storage of contracts' data
class storage_database
{
public:
    storage_database(undo_database& udb);
    ~storage_database();

static constexpr unsigned KEY_SIZE      = 32;
static constexpr unsigned FULL_KEY_SIZE = KEY_SIZE + sizeof(uint64_t);
static constexpr unsigned VAL_SIZE      = 32;
    typedef std::array<unsigned char, FULL_KEY_SIZE>    TStgFullKey;
    typedef std::array<unsigned char, KEY_SIZE>         TStgKey;
    typedef std::array<unsigned char, VAL_SIZE>         TStgVal;

    /// \returns true - OK; false - key not found, val_out filled with zeroes
    bool stg_get(object_id_type plane, const TStgKey& key, TStgVal& val_out);
    uint32_t stg_get(object_id_type plane, const TStgKey& key, uint32_t num_of_keys, std::vector<storage_database::TStgVal>& data);
    bool stg_set(object_id_type plane, const TStgKey& key, const TStgVal& val);
    uint32_t stg_set(object_id_type plane, const storage_database::TStgKey& key, const std::vector<storage_database::TStgVal>& vals);
    uint32_t stg_del(object_id_type plane, const storage_database::TStgKey& key, uint32_t num_of_keys);

    struct nvs_attr
    {
        typedef fc::time_point_sec ts_t;
        ts_t exp_ts;
        uint32_t len;
        std::vector<object_id_type> owners; // sorted if > 4

        void bld_owners();
        bool is_owner(object_id_type actor) const;
        bool expired(ts_t now) const;
        unsigned buf_size() const;
    static unsigned attr_size(const void *buf, size_t buf_sz);
        unsigned to_buf(void *buf, size_t buf_sz) const;
        unsigned from_buf(const void *buf, size_t buf_sz);
    };
    void nvs_create(const std::vector<char>& name, fc::time_point_sec now, const nvs_attr& attr, const std::vector<char> *value);
    void nvs_update(const std::vector<char>& name, object_id_type actor, fc::time_point_sec now, const nvs_attr& attr, const std::vector<char> *value);
    void nvs_set   (const std::vector<char>& name, object_id_type actor, fc::time_point_sec now, const std::vector<char>& value);
    void nvs_delete(const std::vector<char>& name, object_id_type actor, fc::time_point_sec now);
    bool nvs_get   (const std::vector<char>& name, fc::time_point_sec now, std::vector<char> *value, nvs_attr *attr) const;

    void perform_maintenance(fc::time_point_sec now) { nvs_perform_maintenance(now); }

    void open(const fc::path& data_dir );
    void flush();
    void wipe(const fc::path& data_dir); // remove from disk
    void close();

    // { undo
    void        undo_disable();
    void        undo_enable();
    bool        undo_enabled();
    void        undo_start_undo_session();
    void        undo_pop_commit();
    std::size_t undo_size() const;
    std::size_t undo_session_count() const;
    void        undo_pop_front();

    void        undo_undo();
    void        undo_merge();
    void        undo_commit();
    // } undo

private:
    struct undo_rec
    {
        TStgVal     val;
        enum EHint { EH_None = 0, EH_Created = 1, EH_NowRemoved = 2, EH_Zero = 4 };
        unsigned char hint = EH_None;

        undo_rec(const TStgVal& val, unsigned char hint) : val(val), hint(hint)
        {}
    };
    struct undo_rec_nvs
    {
        std::vector<char>   val;
        enum EHint { EH_None = 0, EH_UpdNoTime, EH_Updated, EH_Created, EH_Deleted, EH_LAST };
        unsigned            hint;
        nvs_attr::ts_t      exp_ts;

        undo_rec_nvs(EHint hint, nvs_attr::ts_t exp_ts, const void *val, size_t val_sz)
            : val((const char *)val, ((const char *)val) + val_sz), hint(EH_None), exp_ts(0)
        { touch(hint, exp_ts); }
        void touch(EHint hint, nvs_attr::ts_t exp_ts);
    };
    struct undo_state
    {
        struct khash { size_t operator()(const TStgFullKey& key) const; };
        typedef std::unordered_map<TStgFullKey, undo_rec, struct khash> TTouchedMap;
        TTouchedMap undoes;
        struct vhash { size_t operator()(const std::vector<char>& key) const; };
        typedef std::unordered_map<std::vector<char>, undo_rec_nvs, struct vhash> TTouchedNVSMap;
        TTouchedNVSMap undoes_nvs;
    };
    struct undo_session
    {
        MDB_txn_ptr txn;
    };

    MDB_txn *txn(); // top session or m_txn
    undo_state& head();
    bool stg_set_undo(const TStgFullKey& full_key, const TStgVal& val);
    bool stg_set_int(const TStgFullKey& full_key, const TStgVal& val);
    bool stg_del_undo(const TStgFullKey& full_key);
    bool stg_del_int(const TStgFullKey& full_key);

    MDB_cursor_ptr cursor_open(MDB_dbi dbi);
    void nvs_undo_commit(undo_state::TTouchedNVSMap undoes_nvs);
    struct nvs_accessor;
    bool nvs_expired(uint32_t ts);
    bool nvs_check_int(const std::vector<char>& name, storage_database::nvs_accessor& acr);
    bool nvs_check(const std::vector<char>& name, object_id_type actor, fc::time_point_sec now, storage_database::nvs_accessor& acr);
    void nvs_update_int(nvs_accessor& acr, const std::vector<char>& name, const nvs_attr& attr, const std::vector<char> *value);
    void nvs_perform_maintenance(fc::time_point_sec now);

    undo_database&              m_udb;
    MDB_env_ptr                 m_env;
    MDB_txn_ptr                 m_txn;
    MDB_dbi                     m_stg_dbi;
    MDB_dbi                     m_nvs_dbi;
    MDB_dbi                     m_nvs_ts_dbi;
    std::deque<undo_state>      m_undo_states;
    std::stack<undo_session>    m_undo_sessions;
};

} } /* namespace db */ /* namespace graphene */

FC_REFLECT( graphene::db::storage_database::nvs_attr, (exp_ts)(len)(owners) )

