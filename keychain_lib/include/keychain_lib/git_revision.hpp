//
// Created by roman on 10/12/18.
//

#ifndef KEYCHAINAPP_GIT_REVISION_HPP
#define KEYCHAINAPP_GIT_REVISION_HPP

#include <cstdint>

namespace keychain_app { namespace version_info {
extern const char* const git_revision_sha;
extern const uint32_t git_revision_unix_timestamp;
extern const char* const git_revision_description;
} }

#endif //KEYCHAINAPP_GIT_REVISION_HPP
