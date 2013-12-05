#ifndef _CONFIGDATA_H_
  #define _CONFIGDATA_H_

//�����
extern const int FmRadioRecorderType;              //��� ���������

extern const TIME FmTIME;                     //��������� TIME ������� ������

extern const unsigned int FmSize;             //������ ������

extern const int RecordFmToFile;              //������� ������ � ���� ����
extern const wchar_t FmFolder[200];           //���� � �����
extern const wchar_t FmName[200];             //��� �����

extern const int FmStyle;                     //����� ���������

extern const wchar_t _FmTitle[40];           //���������

extern const int Hook;                        //������� ���������� ������ ����
extern const wchar_t _ItemText[40];           //��� ������ ����

//������� ��������
extern const int UserSoundRecorderType;            //��� ���������

extern const TIME UserTIME;                       //��������� TIME ������� ������

extern const unsigned int UserSize;               //������ ������

extern const int RecordUserToFile;                //������� ������ � ���� ����
extern const wchar_t UserFolder[200];             //���� � �����
extern const wchar_t UserName[200];               //��� �����

extern const int UserStyle;                       //����� ���������

extern const wchar_t _UserTitle[40];             //���������

extern const int KEY1;                             //������� ������ �����
extern const int KEYMODE1;
extern const int KEY2;                             //������� ���������
extern const int KEYMODE2;
extern const int KEY3;                             //������� ������ ���������
extern const int KEYMODE3;

extern const int LimitRadioType;                             //��� �����������
extern const int LimitSoundType;                             //��� �����������

#endif
