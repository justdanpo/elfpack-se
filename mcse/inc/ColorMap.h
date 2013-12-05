#ifndef _COLORMAP_H
#define _COLORMAP_H

//Colors cash
#define clBD 0
#define clBG 1
#define clListBG 2
#define clListBD 3
#define clSelItemBG 4
#define clSelItemBD 5

#define clFileNormal 6
#define clSelFileNormal 7
#define clFileHidden 8
#define clSelFileHidden 9
#define clFileReadOnly 10
#define clSelFileReadOnly 11
#define clInfoTxt 12

#define clIndexTxt 13
#define clIndexTxtBD 14

#define clTabTxt 15
#define clTabTxtBD 16
#define clSlTabBG 17
#define clSlTabBD 18
#define clUSTabBG 19
#define clUSTabBD 20

#define clProgrTxt 21
#define clProgrTxtBD 22
#define clProgrBoxBG 23
#define clProgrBoxBD 24
#define clProgrBG 25
#define clProgrBD 26
#define clCurProgrBG 27

#define clScBar 28
#define clScBarBG 29

#define clMAX clScBarBG

#define MCCS_hdr mccfg_hdr

#define mccs_sig (*(int*)"mccs")
#define mccs_ver 1
#define mccs_size ((clMAX+1)*4)
#define mccs_offset sizeof(MCCS_hdr)
#define mccs_minsize (sizeof(MCCS_hdr)+mccs_size)

extern int Colors[];

extern const wchar_t mccs_deffile[];
extern const wchar_t mccs_ext[];

void InitCS();
void SaveCS(wchar_t* filename);
int LoadCS(wchar_t* filename);

#define COLOR_RGBA(r,g,b,a) (((unsigned int)r<<16)|((unsigned int)g<<8)|((unsigned int)b)|((unsigned int)a<<24))


#endif
