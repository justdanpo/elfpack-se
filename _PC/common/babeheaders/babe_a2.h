#include <stdint.h>

#pragma pack(push,1)
struct ACL1
{
        uint8_t debug:1;//0-disabled 1-enabled
        uint8_t targetcpu:1;//0-access 1-application
        uint8_t softwaretype:1;//0-bootcode or software 1-loader
        uint8_t etxrealtime:1;//0-disabled 1-enabled
        uint8_t etxheader:1;//0-disabled 1-enabled
        uint8_t interactiveload:1;//0-static image 01-modified interactively
        uint8_t asf:2;//0=APP Security off 01=APP Security to data loading 10=APP Security PAF dependent 11=APP Security always on
};
struct ACL2
{
        uint8_t removecertificate:1;//0-yes 1-no
        uint8_t accesssecurity:1;//0-disabled 1-enabled
        uint8_t payloadtype:2;//0-linked to physical address space 1-built for NOR 2/3-built for NAND
        uint8_t headersecurity:2;//0-?? 1-stored as is 2-signature should be changed 3-only verified during download
        uint8_t nandaddressformat:1;//0-pages+4bytes dedundand area 1-only pages
};
struct BABEHDR_A2
{
        uint16_t sig;
        uint8_t verhi;
        uint8_t verlo;
        struct ACL1 acl1;
        struct ACL2 acl2;
        int16_t unk_6;
        int32_t targetchipclass;
        int32_t protectiontype;//0-0 1-rsa 1024 sign with sha-1 2-rsa 2048 sign with sha-1 3-hash protection 4-160bit sha-1 mac over whole sw image 5-160bit SHA-1 MAC over part of sw image. init dependant.
        int32_t unk_10;
        int32_t unk_14;
        int32_t unk_18;
        uint32_t hashlist_maxblocklength;//reverse
        uint32_t hashlist_numberofblocks_values;//reverse
        int32_t hashlist_type;//reverse
        int32_t numcerts;
        int32_t certslength; //?? /256
        int32_t cid;
        int32_t keyusage;
        int32_t maccalculationmode;
        uint32_t payload_destaddress;
        uint32_t payload_length;
        int32_t payload_type;//0-software image 1-software loader 2-generic 3-archive
        int32_t softwareversion;
        int32_t hdrdestaddr;
        int32_t hdrlen;
};
#pragma pack(pop)

enum
{
        A2_PAYLOADTYPE_SOFTWARE_IMAGE = 0,
        A2_PAYLOADTYPE_SOFTWARE_LOADER,
        A2_PAYLOADTYPE_GENERIC,
        A2_PAYLOADTYPE_ARCHIVE
};

enum
{
        A2_KEYUSAGE_BLUE = 1,
        A2_KEYUSAGE_BROWN = 2,
        A2_KEYUSAGE_RED = 4,
        A2_KEYUSAGE_SERVICE = 8,
        A2_KEYUSAGE_TA = 0x10
};

enum
{
	DB3350   =0x00000002,
	DB3150   =0x00000200,
	DB3210   =0x00002000,
	DB3200   =0x00020000,
	DB2000   =0x00010000,
	DB2001   =0x00030000,
	DB2010   =0x00100000,
	DB2012   =0x00300000,
	PNX5230  =0x01000000,
	DB2020   =0x10000000,
};
