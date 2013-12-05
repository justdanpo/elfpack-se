#ifndef _BOOKMARKS_H
#define _BOOKMARKS_H

extern const wchar_t mcbm_path[];
extern const wchar_t mcbm_ext[];

void SaveBM(wchar_t* name, wchar_t* path);
void UseBM(wchar_t* filename);
void DoBMAdd();

void DoBMList();

#endif
