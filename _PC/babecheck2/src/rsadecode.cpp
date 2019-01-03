#include "stdafx.h"
#include <memory.h>
#include <stdint.h>

struct BIGNUM
{
	short allocated_in_dwords;
	short logicalsize_indwords;
	uint32_t* valueptr;
};

class CBigNum
{
public:
	short allocated_in_dwords;
	short logicalsize_indwords;
	uint32_t* valueptr;

	CBigNum(int size)
	{
		allocated_in_dwords = size;
		logicalsize_indwords = 1;
		valueptr = new uint32_t [size];
	}

	CBigNum(void* data,int size)
	{
		allocated_in_dwords = size;
		logicalsize_indwords = size;
		valueptr = new uint32_t [size];
		memcpy(valueptr, data, size*sizeof(uint32_t));
	}

	~CBigNum()
	{
		delete[] valueptr;
	}

	uint32_t big_mod_n()
	{
		if(logicalsize_indwords>=0x22)return 0;
		if( (valueptr[0] & 1) == 0)return 0;

		uint32_t R3_mul = 0xFFFFFFFF;
		uint32_t currentbit = 2;
		uint32_t R2_mod = valueptr[0];

		for(int i=2; i<= this->logicalsize_indwords ;i++, currentbit<<=1)
		{
			uint32_t R5= ( 0xFFFFFFFF >> (0x20-i) ) & R2_mod;
			if(currentbit<=R5)
			{
				R3_mul -= currentbit;
				R2_mod += currentbit * valueptr[0];
			}
		}
		return R3_mul;
	}

};

void __big2__(CBigNum* pbig_indata, CBigNum* pbig_indata2, CBigNum* pbig_keyN_0, uint32_t modn, CBigNum* pbig_retvalue)
{
	if( (pbig_indata->logicalsize_indwords==1 && *pbig_indata->valueptr==0)
		|| (pbig_indata2->logicalsize_indwords==1 && *pbig_indata2->valueptr==0) )
	{
		pbig_retvalue->valueptr[0]=0;
		pbig_retvalue->logicalsize_indwords = 1;
		return;
	}

	uint32_t* pbig_raw_tempdata = new uint32_t [pbig_keyN_0->logicalsize_indwords+2];

	memset(pbig_raw_tempdata,0, (pbig_keyN_0->logicalsize_indwords+2)*sizeof(uint32_t));
	memset(&pbig_indata2->valueptr[pbig_indata2->logicalsize_indwords], 0,(pbig_keyN_0->logicalsize_indwords-pbig_indata2->logicalsize_indwords)*sizeof(uint32_t));

	uint32_t carry2,carry1;
	for(int i=0;i< pbig_keyN_0->logicalsize_indwords; i++)
	{
		if(i<pbig_indata->logicalsize_indwords)
		{
			uint64_t tmp1= (uint64_t)pbig_indata->valueptr[i] * pbig_indata2->valueptr[0]+ pbig_raw_tempdata[0];
			carry1 = tmp1>>32;
			pbig_raw_tempdata[0] = (uint32_t)tmp1;

			uint32_t somevalue= (uint32_t)tmp1*modn;

			uint64_t tmp2= (uint64_t)pbig_keyN_0->valueptr[0] * somevalue+ (uint32_t)tmp1;
			carry2 = tmp2>>32;

			for(int j=1; j< pbig_keyN_0->logicalsize_indwords; j++)
			{
				tmp1 = (uint64_t)pbig_indata->valueptr[i] * pbig_indata2->valueptr[j] + pbig_raw_tempdata[j] + carry1;
				carry1 = tmp1 >>32;

				tmp2 = (uint64_t)pbig_keyN_0->valueptr[j] * somevalue + (uint32_t)tmp1 + carry2;
				carry2 = tmp2>>32;

				pbig_raw_tempdata[j-1]=(uint32_t)tmp2;
			}
			int z=0;//266C
		}else
		{
			//2670
			carry1=0;

			uint32_t somevalue=modn * pbig_raw_tempdata[0];

			uint64_t tmp4 = (uint64_t)somevalue * (uint64_t)pbig_keyN_0->valueptr[0] + pbig_raw_tempdata[0];
			carry2 = tmp4>>32;

			for(int j=1;j<pbig_keyN_0->logicalsize_indwords;j++)
			{
				tmp4 =                (uint64_t)somevalue * (uint64_t)pbig_keyN_0->valueptr[j] + pbig_raw_tempdata[j] + carry2;
				carry2 = tmp4>>32;
				pbig_raw_tempdata[j-1]=(uint32_t)tmp4;
			}
		}

		//26D8
		*(uint64_t*)(&pbig_raw_tempdata[ pbig_keyN_0->logicalsize_indwords-1 ]) =
			*(uint64_t*)(&pbig_raw_tempdata[ pbig_keyN_0->logicalsize_indwords ]) +carry2 +carry1;

		pbig_raw_tempdata[ pbig_keyN_0->logicalsize_indwords+1 ]=0;

	}

	{
		//2728
		//retvalue = raw_tempdata - keyN
		uint32_t carry=0;
		for(int i=0; i< pbig_keyN_0->logicalsize_indwords;i++)
		{
			int64_t tmp=(uint64_t)pbig_raw_tempdata[i] - (uint64_t)pbig_keyN_0->valueptr[i] - carry;
			pbig_retvalue->valueptr[i] = tmp;
			carry = tmp<0;
		}
		//277C
		pbig_retvalue->valueptr[pbig_keyN_0->logicalsize_indwords] = pbig_raw_tempdata[pbig_keyN_0->logicalsize_indwords] - carry;
	}

	if(pbig_retvalue->valueptr[ pbig_keyN_0->logicalsize_indwords ] > pbig_raw_tempdata[ pbig_keyN_0->logicalsize_indwords ])
	{
		memcpy(pbig_retvalue->valueptr, pbig_raw_tempdata, pbig_keyN_0->logicalsize_indwords*sizeof(uint32_t));
	}

	for(pbig_retvalue->logicalsize_indwords = pbig_keyN_0->logicalsize_indwords;
		pbig_retvalue->valueptr[pbig_retvalue->logicalsize_indwords-1] ==0;
		pbig_retvalue->logicalsize_indwords--);

	delete[] pbig_raw_tempdata;
}

int getmaxbit(CBigNum* num)
{
	for(int i=num->logicalsize_indwords-1;i>=0;i--)
	{
		if(num->valueptr[i]!=0)
		{
			int retvalue=31;
			for(long l=0x80000000;l!=0;l>>=1, retvalue--)
				if(num->valueptr[i] & l)
					return retvalue+i*32;

			break;
		}
	}
	return 0;
}

void __big4_2__(CBigNum* big22_1, CBigNum* key_E, CBigNum* key_N_0, uint32_t modn, CBigNum* big22_2)
{
	uint32_t currentbit=(1 << getmaxbit(key_E))&0x1F;
	for(int i=key_E->logicalsize_indwords-1 ; i>=0 ; i--)
	{
		for(;currentbit;currentbit>>=1)
		{
			__big2__(big22_2, big22_2, key_N_0, modn, big22_2);
			if(key_E->valueptr[i] & currentbit)
			{
				__big2__(big22_2,big22_1,key_N_0,modn,big22_2);
			}
		}
		currentbit=0x80000000;
	}

	CBigNum big1(key_N_0->allocated_in_dwords+2);
	big1.valueptr[0]=1;

	__big2__(big22_2, &big1, key_N_0, modn, big22_2);
}


void RSADecode(CBigNum* indata, CBigNum* key_E, CBigNum* key_N, CBigNum* cert80, CBigNum* rezult)
{
	uint32_t mod_n = key_N->big_mod_n();

	CBigNum big_first( key_N->logicalsize_indwords+2 );

	__big2__(indata, cert80, key_N, mod_n, &big_first);

	CBigNum big_second( key_N->logicalsize_indwords+2 );

	CBigNum big1(key_N->allocated_in_dwords+2);
	big1.valueptr[0]=1;

	__big2__(&big1, cert80, key_N, mod_n, &big_second);

	__big4_2__(&big_first, key_E, key_N, mod_n, &big_second);

	rezult->logicalsize_indwords = big_second.logicalsize_indwords;
	memcpy(rezult->valueptr, big_second.valueptr, big_second.logicalsize_indwords*sizeof(uint32_t));

}

int decodeblock( unsigned char * dest, int size, unsigned char * src, unsigned char * key )
{
	CBigNum big_rezult(0x20);

	CBigNum big_indata( src, 0x20 );
	CBigNum big_key_N( &key[0x0], 0x20 );
	CBigNum big_cert80( &key[0x80], 0x20 );
	CBigNum big_key_E( &key[0x100], 1 );

	RSADecode( &big_indata, &big_key_E, &big_key_N, &big_cert80, &big_rezult);
	if(big_rezult.logicalsize_indwords<0x20)
		memset(big_rezult.valueptr+big_rezult.logicalsize_indwords*4,0,(0x20-big_rezult.logicalsize_indwords)*sizeof(uint32_t));
	memcpy(dest,big_rezult.valueptr,size);

	return 0;
}
