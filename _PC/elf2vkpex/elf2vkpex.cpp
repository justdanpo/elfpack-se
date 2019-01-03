// elf2vkpex.cpp : Defines the entry point for the console application.
//

#include <memory.h>
#include "elfhdr.h"
#include "babe_a1.h"

#include "elf2vkpex.hpp"

#define ELF2VKPVERSION "1.03"


int readelf( TCHAR* fname, pteset &out )
{
	out.clear();

	int retcode = 0;
	FILE* f;
	if( _tfopen_s( &f, fname, _T("rb") ) != 0 )
	{
		retcode = -1;
	}else
	{
		ELFHDR elfhdr;

		if( 1 != fread(&elfhdr, sizeof(ELFHDR), 1, f) )
		{
			retcode = -2;
		}else
		{
			if( elfhdr.Signature[0] != 0x7F 
				|| elfhdr.Signature[1] != 'E'
				|| elfhdr.Signature[2] != 'L'
				|| elfhdr.Signature[3] != 'F'
				)
			{
				retcode = -3;
			}else
			{
				for( int psection = 0; psection < elfhdr.Program_header_table_entry_count; psection++)
				{
					PROGTBLENTRY ptentry;
					memset( &ptentry, 0, sizeof(ptentry) );
					long ptentryoffset = elfhdr.Program_header_table + elfhdr.Program_header_table_entry_size * psection;

					if( 0 != fseek( f, ptentryoffset, SEEK_SET ) 
						|| 1 != fread( &ptentry, elfhdr.Program_header_table_entry_size, 1, f )
						)
					{
						retcode = -4;
						break;
					}else
					{
						if( ptentry.Type == PTET_LOAD
							&& ptentry.Size_in_file > 0
							)
						{
							PTE_BUFF p;
							p.addr = ptentry.Virtual_address;
							p.buff.resize( ptentry.Size_in_file );

							if( 0!= fseek( f, ptentry.Physical_offset, SEEK_SET) 
								|| 1 != fread( &p.buff[0], ptentry.Size_in_file, 1, f )
								)
							{
								retcode = -5;
							}else
							{
								out.insert( p );
							}
						}
					}
				}
			}
		}
		fclose( f );
	}
	return retcode;
}

bool trybabe(FILE* f, fwiset &out, ustring fname, int fwnum)
{
	fwiset tmp;

	if( 0 != fseek( f, 0, SEEK_SET ) )
	{
		return false;
	}

	BABEHDR babehdr;

	if( 1 != fread( &babehdr, sizeof(babehdr), 1, f ) )
		return false;

	if( babehdr.sig != 0xBEBA )
		return false;

	if( babehdr.ver < 2 || babehdr.ver > 4 )
		return false;

	if( babehdr.color != COLOR_RED && babehdr.color != COLOR_BROWN && babehdr.color != COLOR_BLUE )
		return false;

	if( babehdr.platform != DB2000
		&&babehdr.platform != DB2001
		&&babehdr.platform != DB2010
		&&babehdr.platform != DB2012
		&&babehdr.platform != PNX5230
		&&babehdr.platform != DB2020
		)
		return false;

	int blocks = babehdr.payloadsize1;
	uint32_t hashtablesize;

	switch( babehdr.ver )
	{
	case 2:
		hashtablesize = 0x100;
		break;
	case 3:
		hashtablesize = 1*blocks;
		break;
	case 4: 
		hashtablesize = 20*blocks;
		break;
	}

	if( 0 != fseek( f, hashtablesize, SEEK_CUR ))
		return false;

	for( int i = 0; i < blocks; i++ )
	{
		FWSECTIONINFO fwsi;
		fwsi.filenum = fwnum;

		uint32_t tmpaddrsize[2];

		if( 2 != fread( tmpaddrsize, sizeof(uint32_t), 2, f ) )
			return false;

		fwsi.addr = tmpaddrsize[0];
		fwsi.size = tmpaddrsize[1];
		fwsi.fileoffset = ftell( f );
		fwsi.filename = fname;

		if( 0 != fseek( f, fwsi.size, SEEK_CUR ) )
			return false;

		tmp.insert( fwsi );
	}

	for( fwiset_i i = tmp.begin(); i!= tmp.end(); i++ )
	{
		out.insert( *i );
	}

	return true;
}

bool trycxc(FILE* f, fwiset &out, ustring fname, int fwnum)
{
	FWSECTIONINFO fwsinfo;
	fwsinfo.filenum = fwnum;

	if( fname.size()>=13 && _tcsicmp( fname.substr(fname.size()-13).c_str(), _T("phone_app.cxc") ) == 0 )
		fwsinfo.patchfile = PFT_PHONE_APP;
	else if( fname.size()>=17 && _tcsicmp( fname.substr(fname.size()-17).c_str(), _T("phone_emp_app.cxc") ) == 0 )
		fwsinfo.patchfile = PFT_PHONE_EMP_APP;
	else if( fname.size()>=13 && _tcsicmp( fname.substr(fname.size()-13).c_str(), _T("phone_acc.cxc") ) == 0 )
		fwsinfo.patchfile = PFT_PHONE_ACC;
	else 
		return false;

	if( 0 != fseek( f, 0, SEEK_SET ) )
	{
		return false;
	}

	struct _stat st;
	if( 0!= _tstat( fname.c_str(), &st ) )
		return false;

	for( uint32_t i = 0; i < sizeof(CXC1000)*2; i += sizeof(CXC1000) )
	{
		CXC1000 cxchdr;

		if( 1 != fread( &cxchdr, sizeof(cxchdr), 1, f ) )
			return false;

		if( cxchdr.hdr.ver1 == 2 
			&& cxchdr.hdr.ver2 == 1
			&& (cxchdr.hdr.baseaddr & 0x000FFFFF) == 0
			&& cxchdr.hdr.hashtable_offset + cxchdr.hdr.hashtable_len == st.st_size 
			)
		{
			fwsinfo.addr = cxchdr.hdr.baseaddr;
			fwsinfo.size = cxchdr.hdr.cxclen;
			fwsinfo.filename = fname;
			fwsinfo.fileoffset = ftell( f );
			out.insert( fwsinfo );
			return true;
		}
	}

	return false;
}

bool tryraw(FILE* f, fwiset &out, ustring fname, int fwnum)
{
	if( 0 != fseek( f, 0, SEEK_SET ) )
	{
		return false;
	}

	FWSECTIONINFO fwinfo;
	fwinfo.filenum = fwnum;

	uint32_t tmp[2];

	if( 2 != fread( tmp, sizeof(uint32_t), 2, f) )
		return false;

	if( (tmp[0] & 0xFFFFF000) == 0xE59FF000)
	{
		if( 0 != fseek( f, (tmp[0] & 0xFFF) + 8, SEEK_SET ) )
			return false;

		if( 1 != fread( tmp, sizeof( uint32_t ), 1, f ) )
			return false;

		fwinfo.addr = tmp[0] & 0xFFFF0000;
		if( fwinfo.addr == 0 )
		{
			if( (tmp[1] & 0xFFFFF000) == 0xE59FF000 )
			{
				if( 0 != fseek( f, (tmp[1] & 0xFFF) + 8 + 4, SEEK_SET ) )
					return false;
				if( 1 != fread( tmp, sizeof( uint32_t ), 1, f ) )
					return false;

				fwinfo.addr = tmp[0] & 0xFFFF0000;
			}
		}
	}

	if( fwinfo.addr != 0 )
	{
		fseek( f, 0, SEEK_END );
		fwinfo.size = ftell( f );
		fwinfo.fileoffset = 0;
		fwinfo.filename = fname;
		out.insert( fwinfo );
		return true;
	}

	return false;
}

int readfw( TCHAR* fname, fwiset &out, uint8_t &filler, int fwnum )
{
	int retcode = 0;

	FILE* f;

	if( _tfopen_s( &f, fname, _T("rb") ) != 0 )
	{
		retcode = -1;
	}else
	{
		if( trybabe( f, out, fname, fwnum ) )
		{
			filler = 0xFF;
		}else if ( trycxc( f, out, fname, fwnum ) )
		{
			filler = 0x00;
		}else if ( tryraw( f, out, fname, fwnum ) )
		{
			filler = 0xFF;
		}else
		{
			retcode = -2;
		}
	}	


	return retcode;
}

int genpatch( const pteset &elf, const fwiset &fw, uint8_t filler, bool showver, int fwnum )
{
	ustring prevfname;
	FILE* prevf = NULL;
	FILE* outf = NULL;
	int outnum = 0;
	bool retcode = 0;

	for( pteset_ci elfi = elf.begin(); (elfi != elf.end()) && (retcode == 0); elfi++ )
	{
		for( size_t i = 0; i < elfi->buff.size(); i += 16 )
		{
			char temp[32];
			sprintf_s( temp, sizeof(temp), "%X: ", elfi->addr + i );
			std::string saddr = temp;
			std::string sold;
			std::string snew;

			for( size_t j = 0; j < 16 && i+j < elfi->buff.size(); j++ )
			{
				uint8_t oldc = filler;

				uint32_t curaddr = elfi->addr + i + j;

				fwiset_ci fwi = fw.begin();
				while( fwi != fw.end() && fwi->addr + fwi->size <= curaddr )
					++fwi;

				if(fwi==fw.end())
				{
					for( fwiset_ci fwi2 = fw.begin(); fwi2 != fw.end(); ++fwi2 )
						if( (fwi2->addr <= curaddr) && (fwi2->addr + fwi2->size + 32*1024*1024 > curaddr) )
							fwi = fwi2;
				}

				if( fwi != fw.end() && fwi->addr <= curaddr )//&& fwi->addr + fwi->size > curaddr )
				{
					if( prevfname != fwi->filename )
					{
						prevfname = fwi->filename;
						if( prevf )
							fclose( prevf );
						if( outf )
						{
							if( !sold.empty() )
							{
								fprintf( outf, "%s%s %s\n", saddr.c_str(), sold.c_str(), snew.c_str() );
								sold = "";
								snew = "";
							}
							if( showver )
								fprintf( outf, ";generated by elf2vkpex v"ELF2VKPVERSION"\n" );
							fclose( outf );
						}

						_tfopen_s( &prevf, fwi->filename.c_str(), _T("rb") );

						TCHAR tmpoutname[MAX_PATH];

						if( fwi->patchfile == PFT_NONE )
						{
							if( fwnum == 1 )
								_stprintf_s( tmpoutname, sizeof(tmpoutname)/sizeof(tmpoutname[0]), _T("out.vkp") );
							else
								_stprintf_s( tmpoutname, sizeof(tmpoutname)/sizeof(tmpoutname[0]), _T("out%d.vkp"), ++outnum );
						}else
						{
							if( fwnum == 1 )
								_stprintf_s( tmpoutname, sizeof(tmpoutname)/sizeof(tmpoutname[0]), _T("out.vkp") );
							else
								_stprintf_s( tmpoutname, sizeof(tmpoutname)/sizeof(tmpoutname[0]), _T("out.%s.vkp"), patchfilenamesshort[fwi->patchfile] );

						}

						_tfopen_s( &outf, tmpoutname, _T("w") );

						if( fwi->patchfile != PFT_NONE )
							fprintf( outf, ";pAtChFiLe=%s\n", patchfilenames[ fwi->patchfile ] );

					}
					if( (fwi->addr <= elfi->addr + i + j ) && (fwi->addr + fwi->size > elfi->addr + i + j) )
					{
						fseek( prevf, elfi->addr + i + j - fwi->addr + fwi->fileoffset, SEEK_SET );
						fread( &oldc, 1, 1, prevf );
					}
				}
				sprintf_s( temp, sizeof(temp), "%02X", oldc );
				sold += temp;
				sprintf_s( temp, sizeof(temp), "%02X", elfi->buff[ i + j ] );
				snew += temp;
			}
			if( ! outf )
			{
				_tprintf(_T("no file to write patchstring to: %s%s %s\n"), toustring(saddr).c_str(), toustring(sold).c_str(), toustring(snew).c_str());
				retcode = 2;
				break;
			}else
			{
				fprintf( outf, "%s%s %s\n", saddr.c_str(), sold.c_str(), snew.c_str() );
			}
		}		
	}

	if( prevf )
		fclose( prevf );
	if( outf )
	{
		if( showver )
			fprintf( outf, ";generated by elf2vkpex v"ELF2VKPVERSION"\n" );
		fclose( outf );
	}
	return retcode;
}

int _tmain(int argc, _TCHAR* argv[])
{
	pteset elf;
	fwiset fw;
	uint8_t filler;
	bool showver = false;
	int fwnum=0;

	_tprintf( _T("elf2vkpex by den_po v") _T(ELF2VKPVERSION) _T(" (%s)\n"), _T(__DATE__) );

	for( int i = 1; i < argc; i++)
	{
		if(argv[i][0]=='-' && _totupper (argv[i][1])=='E')
		{
			if( readelf( &argv[i][2], elf ) )
			{
				_tprintf( _T("unable to read '%s'\n"), &argv[i][2] );
				return 1;
			}
		}else if(argv[i][0]=='-' && _totupper (argv[i][1])=='F')
		{
			if( readfw( &argv[i][2], fw, filler, ++fwnum ) )
			{
				_tprintf( _T("unable to read '%s'\n"), &argv[i][2] );
				return 1;
			}
		}else if(argv[i][0]=='-' && _totupper (argv[i][1])=='V')
		{
			showver = true;
		}else
		{
			_tprintf( _T("bad argument %d: %s\n"), i, argv[i] );
			return 1;
		}
	}

	if( elf.size() == 0 || fw.size() == 0 )
	{
		_tprintf( 
			_T("usage:\n")
			_T("\telf2vkp -eELFNAME.ELF -fFIRMWAREFILE.EXT [-v]\n")
			_T("\n")
			_T("-v - add elf2vkpex version\n")
			_T("\n")
			_T("supported firmware formats:\n")
			_T("- raw\n")
			_T("- mbn/ssw/babe\n")
			_T("- cxc\n") );
		return 1;
	}

	return genpatch( elf, fw, filler, showver, fwnum );
}

