#ifndef _KEYS_H
#define _KEYS_H

typedef struct{
	const int *key;
	char* name;
} KEYNM_MAP;

KEY_PROC GetKeyprocByKey(char key);
KEY_PROC GetKeyprocLongByKey(char key);
void DoKey(int isLongPress, int key);
void LoadKeys();

extern const wchar_t keys_file[];
#endif
