#include "..\\include\Lib_Clara.h"
#include "..\\include\Dir.h"
#include "..\\include\var_arg.h"
#include "..\\include\cfg_items.h"
#include "StructBook.h"
#include "Menu.h"
#include "MyFUNC.h"

wchar_t*GetNamesOfIMN(int x, int)
{
  switch(x)
  {
  case 0:
    return L"Фон";
  case 1:
    return L"Игровой фон";
  case 2:
    return L"Кубик";
  }
  return L" ";
}
void ShowInfo()
{
  int met[10];
  met[0]=STR("Название: Gono4ki v0.2fix\n");
  met[1]=STR("(c) DoCent aka MoneyMasteR\n\n");
  met[2]=STR("Управление:\n\n");
  met[3]=STR("Дж.вправо, 6 - перемещение машинки вправо\n\n");
  met[4]=STR("Дж.влево, 4 - перемещение машинки влево\n\n");
  met[5]=STR("Дж.центр, 5 - ускорение 3х\n\n");  
  met[6]=STR("Короткое Назад - пауза\n\n");
  met[7]=STR("Долгое Назад - выход\n\n");
  met[8]=STR("# - вкл./откл. звук\n\n");
  met[9]=STR("* - свернуть\n");
  MessageBox(STR("Инфо"), TextID_Create(met, ENC_TEXTID, 10), NOIMAGE, 2, 0, 0);
}
