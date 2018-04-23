#include <sys/stat.h>
#include <sys/types.h>
#include <io.h>

typedef int mode_t;

/// @Note If STRICT_UGO_PERMISSIONS is not defined, then setting Read for any
///       of User, Group, or Other will set Read for User and setting Write
///       will set Write for User.  Otherwise, Read and Write for Group and
///       Other are ignored.
///
/// @Note For the POSIX modes that do not have a Windows equivalent, the modes
///       defined here use the POSIX values left shifted 16 bits.

static const mode_t S_ISUID = 0x08000000;           ///< does nothing
static const mode_t S_ISGID = 0x04000000;           ///< does nothing
static const mode_t S_ISVTX = 0x02000000;           ///< does nothing
static const mode_t S_IRUSR = mode_t(_S_IREAD);     ///< read by user
static const mode_t S_IWUSR = mode_t(_S_IWRITE);    ///< write by user
static const mode_t S_IXUSR = 0x00400000;           ///< does nothing

#ifndef STRICT_UGO_PERMISSIONS
static const mode_t S_IRGRP = mode_t(_S_IREAD);     ///< read by *USER*
static const mode_t S_IWGRP = mode_t(_S_IWRITE);    ///< write by *USER*
static const mode_t S_IXGRP = 0x00080000;           ///< does nothing
static const mode_t S_IROTH = mode_t(_S_IREAD);     ///< read by *USER*
static const mode_t S_IWOTH = mode_t(_S_IWRITE);    ///< write by *USER*
static const mode_t S_IXOTH = 0x00010000;           ///< does nothing
#else
static const mode_t S_IRGRP = 0x00200000;           ///< does nothing
static const mode_t S_IWGRP = 0x00100000;           ///< does nothing
static const mode_t S_IXGRP = 0x00080000;           ///< does nothing
static const mode_t S_IROTH = 0x00040000;           ///< does nothing
static const mode_t S_IWOTH = 0x00020000;           ///< does nothing
static const mode_t S_IXOTH = 0x00010000;           ///< does nothing
#endif

static const mode_t MS_MODE_MASK = 0x0000ffff;           ///< low word
