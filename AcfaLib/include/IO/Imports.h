#pragma once
#include <cell/cell_fs.h>

typedef CellFsErrno (*cellFsOpen_t)(const char* path, int flags, int* fd, const void* arg, uint64_t size);
typedef CellFsErrno (*cellFsRead_t)(int fd, void* buf, uint64_t nbytes, uint64_t* nread);
typedef CellFsErrno (*cellFsWrite_t)(int fd, const void* buf, uint64_t nbytes, uint64_t* nwrite);
typedef CellFsErrno (*cellFsClose_t)(int fd);
typedef CellFsErrno (*cellFsOpendir_t)(const char* path, int* fd);
typedef CellFsErrno (*cellFsReaddir_t)(int fd, CellFsDirent* dir, uint64_t* nread);
typedef CellFsErrno (*cellFsClosedir_t)(int fd);
typedef CellFsErrno (*cellFsStat_t)(const char* path, CellFsStat* sb);
typedef CellFsErrno (*cellFsFstat_t)(int fd, CellFsStat* sb);
typedef CellFsErrno (*cellFsMkdir_t)(const char* path, CellFsMode mode);
typedef CellFsErrno (*cellFsRename_t)(const char* from, const char* to);
typedef CellFsErrno (*cellFsRmdir_t)(const char* path);
typedef CellFsErrno (*cellFsUnlink_t)(const char* path);
typedef CellFsErrno (*cellFsLseek_t)(int fd, int64_t offset, int whence, uint64_t* pos);
typedef CellFsErrno (*cellFsFsync_t)(int fd);
typedef CellFsErrno (*cellFsFGetBlockSize_t)(int fd, uint64_t* sector_size, uint64_t* block_size);
typedef CellFsErrno (*cellFsGetBlockSize_t)(const char* path, uint64_t* sector_size, uint64_t* block_size);
typedef CellFsErrno (*cellFsGetFreeSize_t)(const char* directory_path, uint32_t* fs_block_size, uint64_t* free_block_count);
typedef CellFsErrno (*cellFsTruncate_t)(const char* path, uint64_t size);
typedef CellFsErrno (*cellFsFtruncate_t)(int fd, uint64_t size);
typedef CellFsErrno (*cellFsUtime_t)(const char* path, const CellFsUtimbuf* timep);
typedef CellFsErrno (*cellFsChmod_t)(const char* path, CellFsMode mode);
typedef CellFsErrno (*cellFsGetDirectoryEntries_t)(int fd, CellFsDirectoryEntry* entries, uint32_t entries_size, uint32_t* data_count);
typedef CellFsErrno (*cellFsAllocateFileAreaWithoutZeroFill_t)(const char* path, const uint64_t size);
typedef CellFsErrno (*cellFsSetIoBuffer_t)(int fd, size_t buffer_size_limit, int page_type, sys_memory_container_t container);
typedef CellFsErrno (*cellFsSetDefaultContainer_t)(sys_memory_container_t container, size_t total_limit);
typedef CellFsErrno (*cellFsSetIoBufferFromDefaultContainer_t)(int fd, size_t buffer_size_limit, int page_type);
typedef CellFsErrno (*cellFsReadWithOffset_t)(int fd, uint64_t offset, void* buffer, uint64_t buffer_size, uint64_t* read_bytes);
typedef CellFsErrno (*cellFsWriteWithOffset_t)(int fd, uint64_t offset, const void* data, uint64_t data_size, uint64_t* written_bytes);
typedef CellFsErrno (*cellFsSdataOpen_t)(const char* path, int flags, int* fd, const void* arg, uint64_t size);
typedef CellFsErrno (*cellFsSdataOpenByFd_t)(int mself_fd, int flags, int* sdata_fd, uint64_t offset, const void* arg, uint64_t size);
typedef CellFsErrno (*cellFsSetDiscReadRetrySetting_t)(const CellFsDiscReadRetryType retry_type);
typedef CellFsErrno (*cellFsStReadInit_t)(int fd, const CellFsRingBuffer* ringbuf);
typedef CellFsErrno (*cellFsStReadFinish_t)(int fd);
typedef CellFsErrno (*cellFsStReadGetRingBuf_t)(int fd, CellFsRingBuffer* ringbuf);
typedef CellFsErrno (*cellFsStReadGetStatus_t)(int fd, uint64_t* status);
typedef CellFsErrno (*cellFsStReadGetRegid_t)(int fd, uint64_t* regid);
typedef CellFsErrno (*cellFsStReadStart_t)(int fd, uint64_t offset, uint64_t size);
typedef CellFsErrno (*cellFsStReadStop_t)(int fd);
typedef CellFsErrno (*cellFsStRead_t)(int fd, char* buf, uint64_t size, uint64_t* rsize);
typedef CellFsErrno (*cellFsStReadGetCurrentAddr_t)(int fd, char** addr, uint64_t* size);
typedef CellFsErrno (*cellFsStReadPutCurrentAddr_t)(int fd, char* addr, uint64_t size);
typedef CellFsErrno (*cellFsStReadWait_t)(int fd, uint64_t size);
typedef CellFsErrno (*cellFsStReadWaitCallback_t)(int fd, uint64_t size, void(*func)(int xfd, uint64_t xsize));
typedef CellFsErrno (*cellFsAioInit_t)(const char* mount_point);
typedef CellFsErrno (*cellFsAioFinish_t)(const char* mount_point);
typedef CellFsErrno (*cellFsAioRead_t)(CellFsAio* aio, int* id, void(*func)(CellFsAio* aio, CellFsErrno err, int id, uint64_t size));
typedef CellFsErrno (*cellFsAioCancel_t)(int id);
typedef CellFsErrno (*cellFsAioWrite_t)(CellFsAio* aio, int* id, void(*func)(CellFsAio* aio, CellFsErrno err, int id, uint64_t size));

#pragma pack(push, 1) // Ensures no compiler alignment padding messes up the offsets
struct CellFsFunctionTable
{
    cellFsFtruncate_t cellFsFtruncate;
    cellFsRmdir_t cellFsRmdir;
    cellFsClose_t cellFsClose;
    cellFsOpendir_t cellFsOpendir;
    cellFsAioWrite_t cellFsAioWrite;
    cellFsRead_t cellFsRead;
    cellFsReaddir_t cellFsReaddir;
    cellFsOpen_t cellFsOpen;
    cellFsStat_t cellFsStat;
    cellFsUnlink_t cellFsUnlink;
    cellFsLseek_t cellFsLseek;
    cellFsMkdir_t cellFsMkdir;
    cellFsAioRead_t cellFsAioRead;
    cellFsFGetBlockSize_t cellFsFGetBlockSize;
    cellFsAioInit_t cellFsAioInit;
    cellFsWrite_t cellFsWrite;
    cellFsFstat_t cellFsFstat;
    cellFsRename_t cellFsRename;
    cellFsClosedir_t cellFsClosedir;
};
#pragma pack(pop)