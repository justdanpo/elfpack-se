/*
Загрузчик конфигурации
(c) Rst7, Kibab
 Осуществляет загрузку конфига и предоставляет функции инициализации
*/
#ifndef _SKINLOADER_H_
  #define _SKINLOADER_H_

// Вызвать обязательно перед обращением к переменным конфигурации
void skin_InitConfig(wchar_t*, int mode);

extern wchar_t successed_skin_path[512];
extern wchar_t successed_skin_name[512];
#endif
