#ifndef _DATETIME_H_
#define _DATETIME_H_

#define TI_GET_HOUR(x) ((unsigned int)x&0xFF)
#define TI_GET_MIN(x) (((unsigned int)x>>8)&0xFF)
#define TI_GET_SEC(x) (((unsigned int)x>>16)&0xFF)

#define DI_GET_YEAR(x) ((unsigned int)x&0xFFFF)
#define TI_GET_MONTH(x) (((unsigned int)x>>16)&0xFF)
#define TI_GET_DAY(x) ((unsigned int)x>>24)

extern const PAGE_DESC bk_time_input;
extern const PAGE_DESC bk_date_input;

#define IDN_TIME_INPUT_ICON L"FR_INPUT_GUI_SET_TIME_ICN"
#define IDN_DATE_INPUT_ICON L"FR_INPUT_GUI_SET_DATE_ICN"

#endif
