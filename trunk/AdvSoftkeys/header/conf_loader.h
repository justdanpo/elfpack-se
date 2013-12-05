/*
Загрузчик конфигурации
(c) Rst7, Kibab
 Осуществляет загрузку конфига и предоставляет функции инициализации
*/
#ifndef __HEADER_CONFLOADER__
  #define __HEADER_CONFLOADER__

// Вызвать обязательно перед обращением к переменным конфигурации
void InitConfig(void);

extern const wchar_t *successed_config_path;
extern const wchar_t *successed_config_name;
#endif
