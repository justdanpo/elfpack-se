#ifndef _READ_CSV_H_
  #define _READ_CSV_H_

typedef struct {
  unsigned short ci;
  unsigned short lac;
  float point_x;
  float point_y;
} BS_POINT;

int ReadCsvFile(TOWN *town);
TOWN *FindTownByCiLac(MAIN_INI *ini, int ci, int lac);
void DestroyCsvList(TOWN *town);
int FindTownById(void *what, void *where);
int FindBSByCiLac(void *what, void *where);

#endif
