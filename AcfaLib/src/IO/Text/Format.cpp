#include <IO/Text/Text.h>
#include "sys_imports.h"

namespace AcfaLib
{
namespace IO
{
namespace Text
{
    const char* ToStringInline(const CellFsMode mode, char* buffer, size_t size)
    {
        int len = 0;
        buffer[0] = '\0';

        int fileType = mode & CELL_FS_S_IFMT;
        switch (fileType)
        {
            case CELL_FS_S_IFREG:
                len += _sys_snprintf(buffer + len, size - len, "CELL_FS_S_IFREG");
                break;
            case CELL_FS_S_IFDIR:
                len += _sys_snprintf(buffer + len, size - len, "CELL_FS_S_IFDIR");
                break;
            case CELL_FS_S_IFLNK:
                len += _sys_snprintf(buffer + len, size - len, "CELL_FS_S_IFLNK");
                break;
            case CELL_FS_S_IFWHT:
                len += _sys_snprintf(buffer + len, size - len, "CELL_FS_S_IFWHT");
                break;
            default:
                len += _sys_snprintf(buffer + len, size - len, "UNDEFINED");
                break;
        }

        int perms = mode & 0777;
        if (len > 0 && perms != 0)
        {
            len += _sys_snprintf(buffer + len, size - len, "|");
        }

        switch (perms)
        {
            case 0:
                break;
            case CELL_FS_DEFAULT_CREATE_MODE_1:
                len += _sys_snprintf(buffer + len, size - len, "CELL_FS_DEFAULT_CREATE_MODE_1");
                break;
            case CELL_FS_DEFAULT_CREATE_MODE_2:
                len += _sys_snprintf(buffer + len, size - len, "CELL_FS_DEFAULT_CREATE_MODE_2");
                break;
            case CELL_FS_DEFAULT_CREATE_MODE_3:
                len += _sys_snprintf(buffer + len, size - len, "CELL_FS_DEFAULT_CREATE_MODE_3");
                break;
            case CELL_FS_DEFAULT_CREATE_MODE_4:
                len += _sys_snprintf(buffer + len, size - len, "CELL_FS_DEFAULT_CREATE_MODE_4");
                break;
            case CELL_FS_DEFAULT_CREATE_MODE_5:
                len += _sys_snprintf(buffer + len, size - len, "CELL_FS_DEFAULT_CREATE_MODE_5");
                break;
            case CELL_FS_DEFAULT_CREATE_MODE_6:
                len += _sys_snprintf(buffer + len, size - len, "CELL_FS_DEFAULT_CREATE_MODE_6");
                break;
            case CELL_FS_DEFAULT_CREATE_MODE_7:
                len += _sys_snprintf(buffer + len, size - len, "CELL_FS_DEFAULT_CREATE_MODE_7");
                break;
            case CELL_FS_DEFAULT_CREATE_MODE_8:
                len += _sys_snprintf(buffer + len, size - len, "CELL_FS_DEFAULT_CREATE_MODE_8");
                break;
            case CELL_FS_OTH_CREATE_MODE_RW:
                len += _sys_snprintf(buffer + len, size - len, "CELL_FS_OTH_CREATE_MODE_RW");
                break;
            case CELL_FS_OTH_CREATE_MODE_RX:
                len += _sys_snprintf(buffer + len, size - len, "CELL_FS_OTH_CREATE_MODE_RX");
                break;
            default:
                int initial_len = len;

#define APPEND_PERM_FLAG(flag) \
                if ((perms & (flag)) == (flag)) \
                { \
                    len += _sys_snprintf(buffer + len, size - len, "%s%s", (len > initial_len ? "|" : ""), #flag); \
                }

                // Owner
                APPEND_PERM_FLAG(CELL_FS_S_IRUSR);
                APPEND_PERM_FLAG(CELL_FS_S_IWUSR);
                APPEND_PERM_FLAG(CELL_FS_S_IXUSR);

                // Group
                APPEND_PERM_FLAG(CELL_FS_S_IRGRP);
                APPEND_PERM_FLAG(CELL_FS_S_IWGRP);
                APPEND_PERM_FLAG(CELL_FS_S_IXGRP);

                // Other
                APPEND_PERM_FLAG(CELL_FS_S_IROTH);
                APPEND_PERM_FLAG(CELL_FS_S_IWOTH);
                APPEND_PERM_FLAG(CELL_FS_S_IXOTH);

#undef APPEND_BIT
                break;
        }

        return buffer;
    }

    const char* ToStringInline(const CellFsStat* stat, char* buffer, size_t size)
    {
        if (stat == NULL)
        {
            return "NULL";
        }

        char modeBuffer[256];
        _sys_snprintf(buffer, size,
            "{ st_mode: %s, st_uid: %d, st_gid: %d, st_atime: %ld, st_mtime: %ld, st_ctime: %ld, st_size: %llu, st_blksize: %llu }",
            ToStringInline(stat->st_mode, modeBuffer, sizeof(modeBuffer)),
            stat->st_uid,
            stat->st_gid,
            (long)stat->st_atime,
            (long)stat->st_mtime,
            (long)stat->st_ctime,
            (unsigned long long)stat->st_size,
            (unsigned long long)stat->st_blksize
        );

        return buffer;
    }

    const char* ToStringInline(const CellFsDirent* dir, char* buffer, size_t size)
    {
        if (dir == NULL)
        {
            return "NULL";
        }

        const char* typeStr;
        switch (dir->d_type)
        {
            case CELL_FS_TYPE_UNKNOWN:
                typeStr = "CELL_FS_TYPE_UNKNOWN";
                break;
            case CELL_FS_TYPE_DIRECTORY:
                typeStr = "CELL_FS_TYPE_DIRECTORY";
                break;
            case CELL_FS_TYPE_REGULAR:
                typeStr = "CELL_FS_TYPE_REGULAR";
                break; 
            case CELL_FS_TYPE_SYMLINK:
                typeStr = "CELL_FS_TYPE_SYMLINK";
                break;
            default:
                typeStr = "UNDEFINED";
                break;
        }

        _sys_snprintf(buffer, size,
            "{ d_type: %s, d_namlen: %d, d_name: \"%.*s\" }",
            typeStr,
            (int)dir->d_namlen,
            (int)dir->d_namlen,
            dir->d_name
        );

        return buffer;
    }

    const char* ToStringInline(const CellFsAio* aio, char* buffer, size_t size)
    {
        if (aio == NULL)
        {
            return "NULL";
        }

        _sys_snprintf(buffer, size,
            "{ fd: %d, offset: %lld, buf: %p, size: %llu, user_data: 0x%llX }",
            aio->fd,
            (long long)aio->offset,
            aio->buf,
            (unsigned long long)aio->size,
            (unsigned long long)aio->user_data
        );

        return buffer;
    }

    const char* OpenFlagsToString(const int flags, char* buffer, size_t size)
    {
        int len = 0;
        buffer[0] = '\0';

        // Access Mode
        int accessMode = flags & CELL_FS_O_ACCMODE;
        switch (accessMode)
        {
            case CELL_FS_O_RDONLY:
                len += _sys_snprintf(buffer + len, size - len, "CELL_FS_O_RDONLY");
                break;
            case CELL_FS_O_WRONLY:
                len += _sys_snprintf(buffer + len, size - len, "CELL_FS_O_WRONLY");
                break;
            case CELL_FS_O_RDWR:
                len += _sys_snprintf(buffer + len, size - len, "CELL_FS_O_RDWR");
                break;
            default:
                len += _sys_snprintf(buffer + len, size - len, "UNDEFINED");
                break;
        }

#define APPEND_OPEN_FLAG(flag) \
            if ((flags & (flag)) == (flag)) { \
                len += _sys_snprintf(buffer + len, size - len, "%s%s", (len > 0 ? " | " : ""), #flag); \
            }

        // Other flags
        APPEND_OPEN_FLAG(CELL_FS_O_CREAT);
        APPEND_OPEN_FLAG(CELL_FS_O_EXCL);
        APPEND_OPEN_FLAG(CELL_FS_O_TRUNC);
        APPEND_OPEN_FLAG(CELL_FS_O_APPEND);
        APPEND_OPEN_FLAG(CELL_FS_O_MSELF);

#undef APPEND_OPEN_FLAG

        return buffer;
    }

    const char* ToString(const CellFsErrno code)
    {
        switch (code)
        {
        case CELL_FS_SUCCEEDED:
            return "CELL_FS_SUCCEEDED";
        case CELL_FS_EDOM:
            return "CELL_FS_EDOM";
        case CELL_FS_EFAULT:
            return "CELL_FS_EFAULT";
        case CELL_FS_EFBIG:
            return "CELL_FS_EFBIG";
        case CELL_FS_EFPOS:
            return "CELL_FS_EFPOS";
        case CELL_FS_EMLINK:
            return "CELL_FS_EMLINK";
        case CELL_FS_ENFILE:
            return "CELL_FS_ENFILE";
        case CELL_FS_ENOENT:
            return "CELL_FS_ENOENT";
        case CELL_FS_ENOSPC:
            return "CELL_FS_ENOSPC";
        case CELL_FS_ENOTTY:
            return "CELL_FS_ENOTTY";
        case CELL_FS_EPIPE:
            return "CELL_FS_EPIPE";
        case CELL_FS_ERANGE:
            return "CELL_FS_ERANGE";
        case CELL_FS_EROFS:
            return "CELL_FS_EROFS";
        case CELL_FS_ESPIPE:
            return "CELL_FS_ESPIPE";
        case CELL_FS_E2BIG:
            return "CELL_FS_E2BIG";
        case CELL_FS_EACCES:
            return "CELL_FS_EACCES";
        case CELL_FS_EAGAIN:
            return "CELL_FS_EAGAIN";
        case CELL_FS_EBADF:
            return "CELL_FS_EBADF";
        case CELL_FS_EBUSY:
            return "CELL_FS_EBUSY";
        case CELL_FS_EEXIST:
            return "CELL_FS_EEXIST";
        case CELL_FS_EINTR:
            return "CELL_FS_EINTR";
        case CELL_FS_EINVAL:
            return "CELL_FS_EINVAL";
        case CELL_FS_EIO:
            return "CELL_FS_EIO";
        case CELL_FS_EISDIR:
            return "CELL_FS_EISDIR";
        case CELL_FS_EMFILE:
            return "CELL_FS_EMFILE";
        case CELL_FS_ENODEV:
            return "CELL_FS_ENODEV";
        case CELL_FS_ENOEXEC:
            return "CELL_FS_ENOEXEC";
        case CELL_FS_ENOMEM:
            return "CELL_FS_ENOMEM";
        case CELL_FS_ENOTDIR:
            return "CELL_FS_ENOTDIR";
        case CELL_FS_ENXIO:
            return "CELL_FS_ENXIO";
        case CELL_FS_EPERM:
            return "CELL_FS_EPERM";
        case CELL_FS_ESRCH:
            return "CELL_FS_ESRCH";
        case CELL_FS_EXDEV:
            return "CELL_FS_EXDEV";
        case CELL_FS_EBADMSG:
            return "CELL_FS_EBADMSG";
        case CELL_FS_ECANCELED:
            return "CELL_FS_ECANCELED";
        case CELL_FS_EDEADLK:
            return "CELL_FS_EDEADLK";
        case CELL_FS_EILSEQ:
            return "CELL_FS_EILSEQ";
        case CELL_FS_EINPROGRESS:
            return "CELL_FS_EINPROGRESS";
        case CELL_FS_EMSGSIZE:
            return "CELL_FS_EMSGSIZE";
        case CELL_FS_ENAMETOOLONG:
            return "CELL_FS_ENAMETOOLONG";
        case CELL_FS_ENOLCK:
            return "CELL_FS_ENOLCK";
        case CELL_FS_ENOSYS:
            return "CELL_FS_ENOSYS";
        case CELL_FS_ENOTEMPTY:
            return "CELL_FS_ENOTEMPTY";
        case CELL_FS_ENOTSUP:
            return "CELL_FS_ENOTSUP";
        case CELL_FS_ETIMEDOUT:
            return "CELL_FS_ETIMEDOUT";
        case CELL_FS_EFSSPECIFIC:
            return "CELL_FS_EFSSPECIFIC";
        case CELL_FS_EOVERFLOW:
            return "CELL_FS_EOVERFLOW";
        case CELL_FS_ENOTMOUNTED:
            return "CELL_FS_ENOTMOUNTED";
        case CELL_FS_ENOTMSELF:
            return "CELL_FS_ENOTMSELF";
        case CELL_FS_ENOTSDATA:
            return "CELL_FS_ENOTSDATA";
        case CELL_FS_EAUTHFATAL:
            return "CELL_FS_EAUTHFATAL";
        default:
            return "UNDEFINED";
        }
    }
}
}
}