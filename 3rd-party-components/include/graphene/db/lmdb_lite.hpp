/*
 * lmdb_lite.hpp
 *
 *  Created on: Aug 29, 2017
 *      (c) 2017 Annihilat.io
 */
#pragma once

/** \file lmdb_lite.hpp
 *
 *  Lightweight wrappers around lmdb, not to spread knowledge about it
 *  all over the project.
 */

#include <memory>

extern "C"
{
typedef struct MDB_env MDB_env;
typedef struct MDB_txn MDB_txn;
typedef unsigned int   MDB_dbi;
typedef struct MDB_cursor MDB_cursor;
} // extern "C"

namespace graphene { namespace db {

struct mdb_env_close_s { void operator()(MDB_env *penv) const; };
typedef std::unique_ptr<MDB_env, mdb_env_close_s> MDB_env_ptr;


struct mdb_txn_abort_s { void operator()(MDB_txn *ptxn) const; };
typedef std::unique_ptr<MDB_txn, mdb_txn_abort_s> MDB_txn_ptr;

struct mdb_cursor_close_s { void operator()(MDB_cursor *pcursor) const; };
typedef std::unique_ptr<MDB_cursor, mdb_cursor_close_s> MDB_cursor_ptr;


} } // namespace graphene  namespace db
