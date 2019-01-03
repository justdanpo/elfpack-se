#pragma pack(push,1)
struct BABEHDR
{
	uint16_t sig;//0xBEBA
	uint8_t unk;
	int8_t          ver;// 03/04
	uint32_t color;//00 - red, 0x60 - brown
	uint32_t platform;
	uint32_t z1;//used by bootrom      (&F0==0) has certname           &0F==root cert cid
	uint32_t cid;
	uint32_t clr;//???????? = 0xC1
	uint32_t f0[9];//ffffffff
	uint8_t certplace[488];//
	uint32_t prologuestart;
	uint32_t prologuesize1;
	uint32_t prologuesize2;
	uint32_t unk1[4];//0,1,1,0xFFFFFFFF
	uint8_t hash1[128];
	uint32_t flags;//0x200 - main/fs/sfa/cert [2C0]
	uint32_t unk2[4];
	uint32_t clr2;//???????? = 0xC1
	uint32_t f1[3];//ffffffff
	uint32_t payloadstart;
	uint32_t payloadsize1;//numblocks
	uint32_t payloadsize2;
	uint32_t flags2;//10 - sfa, 1 - main/fs/cert
	uint32_t unk4[3];//1,1,0xFFFFFFFF
	uint8_t hash2[128];
};
#pragma pack(pop)

enum
{
	DB2000   =0x00010000
	,DB2001   =0x00030000
	,DB2010   =0x00100000
	,DB2012   =0x00300000

	,PNX5230  =0x01000000
	,DB2020   =0x10000000
};

enum
{
	COLOR_RED=0
	,COLOR_BROWN	=0x60
	,COLOR_BLUE		=0xFFFFFFEF
	,COLOR_BLACK	=0x4B434C42 //'BLCK'
	,COLOR_UNKN		=0x4E4B4E55 //'UNKN'
};
