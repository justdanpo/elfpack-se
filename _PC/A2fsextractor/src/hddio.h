#include <windows.h>
#include <vector>
#include <tchar.h>
using namespace std;

#pragma pack(push,1)

struct bootrec32part
{
	DWORD BPB_FATSz32;
	WORD BPB_ExtFlags;// (Bits 0-4 Indicate Active FAT Copy) (Bit 7 Indicates whether FAT Mirroring is Enabled or Disabled <Clear is Enabled>) (If FAT Mirroring is Disabled, the FAT Information is only written to the copy indicated by bits 0-4)
	WORD BPB_FSVer;// (High Byte = Major Version, Low Byte = Minor Version)
	DWORD BPB_RootClus;
	WORD BPB_FSInfo;//SectorNumberoftheFileSystemInformationSector;// (See Structure Below) (Referenced from the Start of the Partition)
	WORD BPB_BkBootSec;// (Referenced from the Start of the Partition)
	char BPB_Reserved[12];
	BYTE BS_DrvNum;
	BYTE BS_Reserved1;//(Could be High Byte of Previous Entry)
	BYTE BS_BootSig;// (29h)
	DWORD BS_VolID;
	char BS_VolLab[11];
	char BS_FilSysType[8];// (FAT32)
};
struct bootrec16part
{
	BYTE BS_DrvNum;
	BYTE BS_Reserved1;
	BYTE BS_BootSig;
	DWORD BS_VolID;
	char BS_VolLab[11];
	char BS_FilSysType[8];
};

struct bootrecord
{
	char BS_jmpBoot[3];
	char BS_OEMName[8];
	WORD BPB_BytsPerSec;
	BYTE BPB_SecPerClus;
	WORD BPB_RsvdSecCnt;
	BYTE BPB_NumFATs;
	WORD BPB_RootEntCnt;//(N/A for FAT32)
	WORD BPB_TotSec16;// (N/A for FAT32)
	BYTE BPB_Media;//(F8h for Hard Disks)
	WORD BPB_FATSz16;// (N/A for FAT32)
	WORD BPB_SecPerTrk;
	WORD BPB_NumHeads;
	DWORD BPB_HiddSec;
	DWORD BPB_TotSec32;//

	union
	{
		bootrec32part b32part_;
		bootrec16part b16part;
	};

	char ExecutableCode[420];
	WORD BootRecordSignature;// (55h AAh)
};

struct DIRS
{
	char DirName[11];
	BYTE DIR_Attr;
	BYTE DIR_NTRes;
	BYTE DIR_CrtTimeTenth;
	WORD DIR_CrtTime;
	WORD DIR_CrtDate;
	WORD DIR_LstAccDate;
	WORD DIR_FstClusHI;
	WORD DIR_WrtTime;
	WORD DIR_WrtDate;
	WORD DIR_FstClusLO;
	DWORD DIR_FileSize;
};
struct DIRL
{
	BYTE LDIR_Ord;//The order of this entry in the sequence of long dir entries associated with the short dir entry at the end of the long dir set.
	//If masked with 0x40 (LAST_LONG_ENTRY), this indicates the entry is the last long dir entry in a set of long dir entries. All valid sets of long dir entries must begin with an entry having this mask.
	WORD LDIR_Name1[5];//Characters 1-5 of the long-name sub-component in this dir entry.
	BYTE LDIR_Attr;//Attributes - must be ATTR_LONG_NAME
	BYTE LDIR_Type;//If zero, indicates a directory entry that is a sub-component of a long name.  NOTE: Other values reserved for future extensions. Non-zero implies other dirent types.
	BYTE LDIR_Chksum;//Checksum of name in the short dir entry at the end of the long dir set.
	WCHAR LDIR_Name2[6];//Characters 6-11 of the long-name sub-component in this dir entry.
	WORD LDIR_FstClusLO;//Must be ZERO. This is an artifact of the FAT "first cluster" and must be zero for compatibility with existing disk utilities.  It's meaningless in the context of a long dir entry.
	WORD LDIR_Name3[2];//Characters 12-13 of the long-name sub-component in this dir entry.
};

union DIR
{
	DIRL l;
	DIRS s;
};

#pragma pack(pop)


class mydirlistitem
{
public:
	WIN32_FIND_DATA fd;
	int firstcluster;

	mydirlistitem* parent;

	mydirlistitem(){ memset(this,0,sizeof this); }
	//    mydirlistitem(const mydirlistitem &x){}
};


class HDDIo
{
private:
	HANDLE h;
	unsigned char *fat;
	vector<long> packedfatitems;
	bool unpackitems;
	unsigned long fatsize;

	unsigned long partitionstart;
	char fattype;
	unsigned long FATSz;
	unsigned long FirstDataSector;
	unsigned long RootDirSectors;
	unsigned long TotSec;

	bool gotocluster(long cluster);
	bool gotosector(long long sector);
	unsigned long getfatitem(long cluster);
	BOOL initpartition();

	TCHAR* detectedpartitionname;

public:
	bootrecord boot;

	HDDIo();        
	~HDDIo();
	BOOL opendrive(TCHAR driveletter);
	BOOL openfile(TCHAR* filename);

	vector<mydirlistitem> readdir(long cluster);
	unsigned char* readfile(unsigned int cluster,int size);

	BOOL trynextpartition();
	TCHAR* getpartition();
};

