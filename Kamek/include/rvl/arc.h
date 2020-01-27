#ifndef __ARC_H__
#define __ARC_H__

#ifdef __cplusplus
extern "C" {
#endif

typedef struct
{
    unsigned int    magic;
    int             fstStart;
    int             fstSize;
    int             fileStart;
    int             reserve[4];
    
} ARCHeader;

#define DARCH_MAGIC         0x55aa382d

typedef struct
{
    void*       archiveStartAddr;
    void*       FSTStart;
    void*       fileStart;
    u32         entryNum;
    char*       FSTStringStart;
    u32         FSTLength;
    u32         currDir;
    
} ARCHandle;

typedef struct
{
    ARCHandle*  handle;
    u32         startOffset;
    u32         length;

} ARCFileInfo;

typedef struct
{
    ARCHandle*  handle;
    u32         entryNum;
    u32         location;
    u32         next;
} ARCDir;

typedef struct 
{
    ARCHandle*  handle;
    u32         entryNum;
    BOOL        isDir;
    char*       name;
} ARCDirEntry;


BOOL  ARCInitHandle(void* arcStart, ARCHandle* handle);
BOOL  ARCOpen(ARCHandle* handle, const char* fileName, ARCFileInfo* af);
BOOL  ARCFastOpen(ARCHandle* handle, s32 entrynum, ARCFileInfo* af);
s32   ARCConvertPathToEntrynum(ARCHandle* handle, const char* pathPtr);
void* ARCGetStartAddrInMem(ARCFileInfo* af);
u32   ARCGetStartOffset(ARCFileInfo* af);
u32   ARCGetLength(ARCFileInfo* af);
BOOL  ARCClose(ARCFileInfo* af);

BOOL  ARCChangeDir(ARCHandle* handle, const char* dirName);
BOOL  ARCGetCurrentDir(ARCHandle* handle, char* path, u32 maxlen);

BOOL  ARCOpenDir(ARCHandle* handle, const char* dirName, ARCDir* dir);
BOOL  ARCReadDir(ARCDir* dir, ARCDirEntry* dirent);
BOOL  ARCCloseDir(ARCDir* dir);

/*---------------------------------------------------------------------------*
  Name:         ARCTellDir

  Description:  Returns the current location associated with the directory

  Arguments:    dir         Pre-opened ARCDir* structure

  Returns:      current location
 *---------------------------------------------------------------------------*/
#define ARCTellDir(dir)             ((dir)->location)

/*---------------------------------------------------------------------------*
  Name:         ARCSeekDir

  Description:  Sets the position of the next ARCReadDir on the directory

  Arguments:    dir         Pre-opened ARCDir* structure
                loc         location to set

  Returns:      None
 *---------------------------------------------------------------------------*/
#define ARCSeekDir(dir, loc)        ((dir)->location = loc)

/*---------------------------------------------------------------------------*
  Name:         ARCRewindDir

  Description:  Resets the position of the directory to the beginning

  Arguments:    dir         Pre-opened ARCDir* structure

  Returns:      None
 *---------------------------------------------------------------------------*/
#define ARCRewindDir(dir)           ((dir)->location = (dir)->entryNum + 1)

#define ARCGetDirEntryName(dirent)  ((dirent)->name)
#define ARCDirEntryIsDir(dirent)    ((dirent)->isDir)


#ifdef __cplusplus
}
#endif

#endif  // __ARC_H__
