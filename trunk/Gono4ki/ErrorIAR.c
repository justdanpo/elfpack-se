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
    return L"���";
  case 1:
    return L"������� ���";
  case 2:
    return L"�����";
  }
  return L" ";
}
void ShowInfo()
{
  int met[10];
  met[0]=STR("��������: Gono4ki v0.2fix\n");
  met[1]=STR("(c) DoCent aka MoneyMasteR\n\n");
  met[2]=STR("����������:\n\n");
  met[3]=STR("��.������, 6 - ����������� ������� ������\n\n");
  met[4]=STR("��.�����, 4 - ����������� ������� �����\n\n");
  met[5]=STR("��.�����, 5 - ��������� 3�\n\n");  
  met[6]=STR("�������� ����� - �����\n\n");
  met[7]=STR("������ ����� - �����\n\n");
  met[8]=STR("# - ���./����. ����\n\n");
  met[9]=STR("* - ��������\n");
  MessageBox(STR("����"), TextID_Create(met, ENC_TEXTID, 10), NOIMAGE, 2, 0, 0);
}
