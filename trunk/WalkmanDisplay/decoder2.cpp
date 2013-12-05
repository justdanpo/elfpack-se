#include "..\deleaker\mem2.h"
#include "..\include\Lib_Clara.h" 
#include "..\include\Dir.h" 
#include "header\wdisplay.h"

// Модуль получения инфы из мп3
// Оригинальный код Robert Wlodarczyk'а (http://www.devhood.com/tutorials/tutorial_details.aspx?tutorial_id=79)
// Адаптация под SE от UltraShot'а

typedef unsigned long int ulong;
typedef char byte;

#define INDEX(x) x/8
#define SHIFT(x) x%8
#define GETBIT(v,x) ((v[INDEX(x)] << SHIFT(x)) & 0x80)>>7

int synchsafeToNormal(char tagSize[4])
{
  int synchsafe, sizeloc, size, power, x;
  
  size=sizeloc=0;
  for(synchsafe=31;synchsafe>=0;synchsafe--)
  {
    if(GETBIT(tagSize, synchsafe))
    {
      power=1;
      for(x=0;x<sizeloc;x++) power*=2;
      size+=power;
    }
    if(synchsafe%8) sizeloc++;
  }
  return size;
};

long lngFileSize;

ulong bithdr;
bool boolVBitRate;
int intVFrames;

int getFrequency();
int getMode();
void LoadMP3Header(byte *c);
bool LoadVBRHeader(byte *inputheader);
int getLengthInSeconds();
bool IsValidHeader();
int getVersionIndex();
int getBitrate();
int getModeIndex();
    
HEADER_INFO ReadMP3Information(wchar_t *path, wchar_t *name)
{
  HEADER_INFO hdr;
  FSTAT stat;
  fstat(path,name,&stat);
  lngFileSize = stat.fsize;
  int file;
  file=_fopen(path,name,FSX_O_RDONLY,FSX_S_IREAD|FSX_S_IWRITE,0);
  
  char BUF[100+1];
  fread(file,BUF,100);
  int size=0;
  if (BUF[0]=='I' && BUF[1]=='D' && BUF[2]=='3') // проверяем, есть ли тэги v2.x
  {
    // Есть, значит надо вычесть из размера файла размер тэгов
    char tagSize[4];
    tagSize[0]=BUF[6];
    tagSize[1]=BUF[7];
    tagSize[2]=BUF[8];
    tagSize[3]=BUF[9];
    size=synchsafeToNormal(tagSize);
    lngFileSize=lngFileSize-size;
  }
  int intPos = 0;
  intPos=size;
  hdr.lngFileSize=lngFileSize;
  hdr.id3size=size;
  byte *bytHeader = new byte[4];
  byte *bytVBitRate = new byte[12];
  // Keep reading 4 bytes from the header until we know for sure that in 
  // fact it's an MP3
  do
  {
    lseek(file,intPos,0);
    fread(file,bytHeader,4);
    intPos++;
    LoadMP3Header(bytHeader);
  }
  while(!IsValidHeader() && (intPos!=stat.fsize));
  
  // If the current file stream position is equal to the length, 
  // that means that we've read the entire file and it's not a valid MP3 file
  if(intPos != stat.fsize)
  {
    intPos += 3;
    
    if(getVersionIndex() == 3)    // MPEG Version 1
    {
      if(getModeIndex() == 3)    // Single Channel
      {
        intPos += 17;
      }
      else
      {
        intPos += 32;
      }
    }
    else                        // MPEG Version 2.0 or 2.5
    {
      if(getModeIndex() == 3)    // Single Channel
      {
        intPos += 9;
      }
      else
      {
        intPos += 17;
      }
    }
    
    // Check to see if the MP3 has a variable bitrate
    lseek(file,intPos,0);
    fread(file,bytVBitRate,12);
    boolVBitRate = LoadVBRHeader(bytVBitRate);
    hdr.VBR=boolVBitRate;
    // Once the file's read in, then assign the properties of the file to the public variables
    hdr.bitrate = getBitrate();
    hdr.frequency = getFrequency();
    hdr.mode=getMode();
    hdr.LengthInSeconds = getLengthInSeconds();
    delete(bytVBitRate);
    delete(bytHeader);
    fclose(file);
    return hdr;
  }
  delete(bytVBitRate);
  delete(bytHeader);
  fclose(file);
  return hdr;
};

void LoadMP3Header(byte *c)
{
  // this thing is quite interesting, it works like the following
  // c[0] = 00000011
  // c[1] = 00001100
  // c[2] = 00110000
  // c[3] = 11000000
  // the operator << means that we'll move the bits in that direction
  // 00000011 << 24 = 00000011000000000000000000000000
  // 00001100 << 16 =         000011000000000000000000
  // 00110000 << 24 =                 0011000000000000
  // 11000000       =                         11000000
  //                +_________________________________
  //                  00000011000011000011000011000000
  bithdr = (ulong)(((c[0] & 255) << 24) | ((c[1] & 255) << 16) | ((c[2] & 255) <<  8) | ((c[3] & 255))); 
};

bool LoadVBRHeader(byte *inputheader)
{
  // If it's a variable bitrate MP3, the first 4 bytes will read 'Xing'
  // since they're the ones who added variable bitrate-edness to MP3s
  if(inputheader[0] == 88 && inputheader[1] == 105 && 
     inputheader[2] == 110 && inputheader[3] == 103)
  {
    int flags = (int)(((inputheader[4] & 255) << 24) | ((inputheader[5] & 255) << 16) | ((inputheader[6] & 255) <<  8) | ((inputheader[7] & 255)));
    if((flags & 0x0001) == 1)
    {
      intVFrames = (int)(((inputheader[8] & 255) << 24) | ((inputheader[9] & 255) << 16) | ((inputheader[10] & 255) <<  8) | ((inputheader[11] & 255)));
      return true;
    }
    else
    {
      intVFrames = -1;
      return true;
    }
  }
  return false;
};

int getFrameSync()     
{
  return (int)((bithdr>>21) & 2047); 
};

int getVersionIndex()  
{ 
  return (int)((bithdr>>19) & 3);  
};

int getLayerIndex()    
{ 
  return (int)((bithdr>>17) & 3);  
};

int getProtectionBit() 
{ 
  return (int)((bithdr>>16) & 1);  
};

int getBitrateIndex()  
{ 
  return (int)((bithdr>>12) & 15); 
};

int getFrequencyIndex()
{ 
  return (int)((bithdr>>10) & 3);  
};

int getPaddingBit()    
{ 
  return (int)((bithdr>>9) & 1);  
};

int getPrivateBit()    
{ 
  return (int)((bithdr>>8) & 1);  
};

int getModeIndex()     
{ 
  return (int)((bithdr>>6) & 3);  
};

int getModeExtIndex()  
{ 
  return (int)((bithdr>>4) & 3);  
};

int getCoprightBit()   
{ 
  return (int)((bithdr>>3) & 1);  
};

int getOrginalBit()    
{ 
  return (int)((bithdr>>2) & 1);  
};

int getEmphasisIndex() 
{ 
  return (int)(bithdr & 3);  
};

double getVersion() 
{
  double table[4] = {2.5, 0.0, 2.0, 1.0};
  return table[getVersionIndex()];
};

int getLayer() 
{
  return (int)(4 - getLayerIndex());
};

bool IsValidHeader() 
{
  return (((getFrameSync()      & 2047)==2047) &&
          ((getVersionIndex()   &    3)!=   1) &&
            ((getLayerIndex()     &    3)!=   0) && 
              ((getBitrateIndex()   &   15)!=   0) &&
                ((getBitrateIndex()   &   15)!=  15) &&
                  ((getFrequencyIndex() &    3)!=   3) &&
                    ((getEmphasisIndex()  &    3)!=   2)    );
};


int getFrequency() 
{
  int table[5][5] =    {    
    {32000, 16000,  8000}, // MPEG 2.5
    {    0,     0,     0}, // reserved
    {22050, 24000, 16000}, // MPEG 2
    {44100, 48000, 32000}  // MPEG 1
  };
  
  return table[getVersionIndex()][getFrequencyIndex()];
};


int getNumberOfFrames() 
{
  // Again, the number of MPEG frames is dependant on whether it's a variable bitrate MP3 or not
  if (!boolVBitRate) 
  {
    double medFrameSize = (double)(((getLayerIndex()==3) ? 12 : 144) *((1000.0 * (float)getBitrate())/(float)getFrequency()));
    return (int)(lngFileSize/medFrameSize);
  }
  else 
    return intVFrames;
};

int getBitrate() 
{
  // If the file has a variable bitrate, then we return an integer average bitrate,
  // otherwise, we use a lookup table to return the bitrate
  if(boolVBitRate)
  {
    double medFrameSize = (double)lngFileSize / (double)getNumberOfFrames();
    return (int)((medFrameSize * (double)getFrequency()) / (1000.0 * ((getLayerIndex()==3) ? 12.0 : 144.0)));
  }
  else
  {
    int table[2][3][16] =        {
      { // MPEG 2 & 2.5
        {0,  8, 16, 24, 32, 40, 48, 56, 64, 80, 96,112,128,144,160,0}, // Layer III
        {0,  8, 16, 24, 32, 40, 48, 56, 64, 80, 96,112,128,144,160,0}, // Layer II
        {0, 32, 48, 56, 64, 80, 96,112,128,144,160,176,192,224,256,0}  // Layer I
      },
      { // MPEG 1
        {0, 32, 40, 48, 56, 64, 80, 96,112,128,160,192,224,256,320,0}, // Layer III
        {0, 32, 48, 56, 64, 80, 96,112,128,160,192,224,256,320,384,0}, // Layer II
        {0, 32, 64, 96,128,160,192,224,256,288,320,352,384,416,448,0}  // Layer I
      }
    };
    
    return table[getVersionIndex() & 1][getLayerIndex()-1][getBitrateIndex()];
  }
};

int getMode() 
{
  switch(getModeIndex()) 
  {
  default:
    return 0;//"Stereo"
  case 1:
    return 1;//"Joint Stereo"
  case 2:
    return 2;//"Dual Channel"
  case 3:
    return 3;//"Single Channel"
  }
};

int getLengthInSeconds() 
{
  int intKiloBitFileSize = (int)((8 * lngFileSize) / 1000);
  return (int)(intKiloBitFileSize/getBitrate());
};

