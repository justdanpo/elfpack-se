#include "stdafx.h"

#include <memory>


#include "openssl/bio.h"
#include "openssl/pem.h"
#include "openssl/rsa.h"
#include "openssl/sha.h"

#include <stdint.h>
#include "babeheaders\babe_a2.h"

static TCHAR* getfiletypename(BABEHDR_A2* hdr)
{
        switch( hdr->payload_type )
        {
        case A2_PAYLOADTYPE_SOFTWARE_IMAGE:
                return _T("SSW");
        case A2_PAYLOADTYPE_SOFTWARE_LOADER:
                return _T("LOADER");
        case A2_PAYLOADTYPE_ARCHIVE:
                return _T("SFA?");
        case A2_PAYLOADTYPE_GENERIC:
                return _T("GENERIC");
        }
        return _T("unknown");
}

static TCHAR* getplatformname(int platform)
{
        struct PN
        {
                int platform;
                TCHAR* name;
        } static const pn[]={
                DB3150   ,_T("DB3150")
                ,DB3200   ,_T("DB3200")
                ,DB3210   ,_T("DB3210")
                ,DB3350   ,_T("DB3350")
        };

        for(int i=0;i<sizeof(pn)/sizeof(pn[0]);i++)
                if(pn[i].platform & platform)
                        return pn[i].name;
        return _T("unknown");
}

static TCHAR* getcolorname(int keyusage)
{
        static TCHAR colorname[256];

        colorname[0]=0;

        if(keyusage & A2_KEYUSAGE_BLUE)
                _tcscat_s(colorname,_T(" BLUE"));
        if(keyusage & A2_KEYUSAGE_BROWN)
                _tcscat_s(colorname,_T(" BROWN"));
        if(keyusage & A2_KEYUSAGE_RED)
                _tcscat_s(colorname,_T(" RED"));
        if(keyusage & A2_KEYUSAGE_SERVICE)
                _tcscat_s(colorname,_T(" SERVICE"));
        if(keyusage & A2_KEYUSAGE_TA)
                _tcscat_s(colorname,_T(" TA"));
        if(!colorname)
                _tcscat_s(colorname,_T("UNKNOWN"));

        return colorname;
}


int isbabe_a2(TCHAR* infilename)
{
        int retvalue=0;
        BABEHDR_A2 hdr;

        FILE* f;
        if(_tfopen_s(&f,infilename,_T("rb")) == 0)
        {
                if(sizeof(hdr)==fread(&hdr,1,sizeof(hdr),f))
                {
                        if(hdr.sig == 0xBEBA)
                        {
                                if(hdr.verhi>=2 && hdr.verhi<=2)
                                {
                                        switch(hdr.targetchipclass)
                                        {
                                        case DB3150:
                                        case DB3210:
                                        case DB3200:
                                        case DB3350:
                                                retvalue = 1;

                                        }
                                }
                        }
                }
                fclose(f);
        }else
        {
                retvalue = -1;
        }
        return retvalue;
}

int checkcommon(BABEHDR_A2* hdr,FILE* f, unsigned char* destsig)
{
        std::auto_ptr<unsigned char> certs( new unsigned char[ hdr->certslength * hdr->numcerts ] );
        std::auto_ptr<unsigned char> signature( new unsigned char[ 128 ] );
        std::auto_ptr<unsigned char> hashlist( new unsigned char[ 20 * hdr->hashlist_numberofblocks_values ] );

        std::auto_ptr<unsigned char> decodedsig( new unsigned char[128] );

        if(hdr->numcerts != fread( certs.get(), hdr->certslength, hdr->numcerts, f))
        {
                _tprintf(_T("can't read certificate chain\n"));
                return -1;
        }
        if(1 != fread( signature.get(), 128, 1, f ))
        {
                _tprintf(_T("can't read signature\n"));
                return -1;
        }
        if(hdr->hashlist_numberofblocks_values != fread( hashlist.get(), 20, hdr->hashlist_numberofblocks_values, f ))
        {
                _tprintf(_T("can't read hash list\n"));
                return -1;
        }

        int retvalue = -1;

        BIO *bio = BIO_new_mem_buf(certs.get(), hdr->certslength);
        if(bio)
        {
                X509* x509 = (X509*)d2i_X509_bio(bio, NULL);//PEM_read_bio_X509(bio, NULL, NULL, NULL);

                if(x509)
                {
                        EVP_PKEY *pkey = X509_get_pubkey(x509);
                        if(pkey)
                        {
                                RSA* rsa = EVP_PKEY_get1_RSA(pkey);
                                if(rsa)
                                {
                                        int decodedlen=RSA_public_decrypt(128, signature.get(), decodedsig.get(), rsa, RSA_PKCS1_PADDING);
                                        if(decodedlen>0)
                                        {
                                                memcpy(destsig, decodedsig.get(), decodedlen);
                                                retvalue = decodedlen;
                                        }
                                        RSA_free( rsa);
                                }
                                EVP_PKEY_free( pkey );
                        }
                        X509_free( x509 );
                }
                BIO_free(bio);
        }

        if( retvalue >= 0 )
        {
                SHA_CTX shactx;
                unsigned char hash[20];

                SHA1_Init(&shactx);
                SHA1_Update(&shactx, hdr, sizeof(*hdr));
                SHA1_Update(&shactx, certs.get(), hdr->certslength * hdr->numcerts);
                SHA1_Final(hash, &shactx);

                if(memcmp(decodedsig.get(), hash, 20))
                {
                        printf("bad header (hash1)\n");
                        return -1;
                }

                SHA1_Init(&shactx);
                SHA1_Update(&shactx, hashlist.get(), 20 * hdr->hashlist_numberofblocks_values);
                SHA1_Final(hash, &shactx);

                if(retvalue>=60)
                {
                        if(memcmp(decodedsig.get()+40, hash, 20))
                        {
                                printf("bad hashlist (hash3)\n");
                                return -1;
                        }
                }
        }

        return 0;
}

int checka2ssw(BABEHDR_A2* hdr,FILE* f)
{
        if(hdr->hashlist_type != 0)
        {
                _tprintf(_T("unknown hash list type %d\n"), hdr->hashlist_type);
                return -1;
        }

        unsigned char signature[128];

        int retvalue = checkcommon(hdr, f, signature);
        if( retvalue<0 )
                return retvalue;

        fseek(f, (hdr->hdrlen +7) &~ 7, SEEK_SET);

        SHA_CTX shactx;
        unsigned char hash[20];

        SHA1_Init(&shactx);
        for(int curblock = 1; curblock <= hdr->hashlist_numberofblocks_values; curblock++ )
        {
				uint32_t offset;
                uint32_t addrsize[2];
                if(2!= fread( addrsize, sizeof(unsigned long), 2, f))
                {
                        _tprintf(_T("error! block %d out of file!\n"),curblock);
                        retvalue = -1;
                }
				if(curblock>2)
				{
					if(addrsize[0]!=offset)
						_tprintf(_T("warning! block %d addr %08X size %08X, expected address: %08X\n")
						,curblock, addrsize[0], addrsize[1], offset);
				}
                //HOLE!!!
                //SHA1_Update(&shactx, addrsize, sizeof(addrsize));
                std::auto_ptr<unsigned char> datablock( new unsigned char[addrsize[1]] );
                if(addrsize[1] != fread( datablock.get(), 1, addrsize[1], f))
                {
                        _tprintf(_T("error! block %d out of file!\n"),curblock);
                        retvalue = -1;
                }
                SHA1_Update(&shactx, datablock.get(), addrsize[1]);
				offset = (addrsize[0] + addrsize[1] + 3) &~3;
        }
        SHA1_Final(hash, &shactx);

        if(memcmp(&signature[20], hash, 20))
        {
                printf("bad software (hash2)\n");
                return -1;
        }

        return retvalue;
}

int checka2loader(BABEHDR_A2* hdr,FILE* f)
{
        unsigned char signature[128];

        int retvalue = checkcommon(hdr, f, signature);
        if( retvalue<0 )
                return retvalue;

        fseek(f, (hdr->hdrlen +7) &~ 7, SEEK_SET);

        SHA_CTX shactx;
        unsigned char hash[20];

        SHA1_Init(&shactx);

        std::auto_ptr<unsigned char> datablock( new unsigned char[hdr->payload_length] );
        if(hdr->payload_length != fread( datablock.get(), 1, hdr->payload_length, f))
        {
                _tprintf(_T("error! payload out of file!\n"));
                retvalue = -1;
        }
        SHA1_Update(&shactx, datablock.get(), hdr->payload_length);
        SHA1_Final(hash, &shactx);

        if(memcmp(&signature[20], hash, 20))
        {
                printf("bad software (hash2)\n");
                return -1;
        }

        return retvalue;
}

int checkbabe_a2(TCHAR* infilename)
{
        int retvalue=0;
        FILE* f;
        if(_tfopen_s(&f,infilename,_T("rb")) == 0)
        {
                BABEHDR_A2 hdr;
                if(sizeof(hdr)==fread(&hdr,1,sizeof(hdr),f))
                {
                        _tprintf(_T("platform %s, cid %d, domains:%s, filetype %s\n")
                                ,getplatformname(hdr.targetchipclass),hdr.cid, getcolorname(hdr.keyusage),getfiletypename(&hdr));

                        if(hdr.protectiontype!=1)
                        {
                                _tprintf(_T("unknown protection type %d\n"), hdr.protectiontype);
                                retvalue = -1;
                        }else
                        {

                                switch( hdr.payload_type )
                                {
                                case A2_PAYLOADTYPE_SOFTWARE_IMAGE:
                                        retvalue = checka2ssw(&hdr,f);
                                        break;
                                case A2_PAYLOADTYPE_SOFTWARE_LOADER:
                                        retvalue = checka2loader(&hdr,f);
                                        break;
                                case A2_PAYLOADTYPE_ARCHIVE:
                                        //retvalue = checka2sfa(&hdr,f);
                                        //break;
                                case A2_PAYLOADTYPE_GENERIC:
                                        _tprintf(_T("unknown payload type %d\n"), hdr.payload_type );
                                        retvalue = -1;
                                        break;
                                }

                        }
                }
                fclose(f);
        }else
        {
                retvalue = -1;
        }

        return retvalue;
}

