#ifndef _CONFIGDATA_H_
  #define _CONFIGDATA_H_

//Радио
extern const int FmRadioRecorderType;              //Тип диктофона

extern const TIME FmTIME;                     //Структура TIME времени записи

extern const unsigned int FmSize;             //Размер записи

extern const int RecordFmToFile;              //Чекбокс записи в свой файл
extern const wchar_t FmFolder[200];           //Путь к файлу
extern const wchar_t FmName[200];             //Имя файла

extern const int FmStyle;                     //Стиль диктофона

extern const wchar_t _FmTitle[40];           //Заголовок

extern const int Hook;                        //Чекбокс добавления пункта меню
extern const wchar_t _ItemText[40];           //Имя пункта меню

//Обычный диктофон
extern const int UserSoundRecorderType;            //Тип диктофона

extern const TIME UserTIME;                       //Структура TIME времени записи

extern const unsigned int UserSize;               //Размер записи

extern const int RecordUserToFile;                //Чекбокс записи в свой файл
extern const wchar_t UserFolder[200];             //Путь к файлу
extern const wchar_t UserName[200];               //Имя файла

extern const int UserStyle;                       //Стиль диктофона

extern const wchar_t _UserTitle[40];             //Заголовок

extern const int KEY1;                             //Клавиша записи радио
extern const int KEYMODE1;
extern const int KEY2;                             //Клавиша диктофона
extern const int KEYMODE2;
extern const int KEY3;                             //Клавиша записи разговора
extern const int KEYMODE3;

extern const int LimitRadioType;                             //тип ограничения
extern const int LimitSoundType;                             //тип ограничения

#endif
