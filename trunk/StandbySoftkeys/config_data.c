#include "..\\include\cfg_items.h"
#include "inc\config_data.h"

#pragma diag_suppress=Pe177

__root const CFG_HDR cfghdr0 = {CFG_LEVEL, "Positions", 1, 0};

    __root const CFG_HDR cfghdr1 = {CFG_UINT, "Left Indent", 0, 100};
    __root const unsigned int LEFT_INDENT = 3;

    __root const CFG_HDR cfghdr2 = {CFG_UINT, "Right Indent", 0, 100};
    __root const unsigned int RIGHT_INDENT = 3;

    __root const CFG_HDR cfghdr3 = {CFG_UINT, "Y Indent", 0, 30};
    __root const unsigned int Y_INDENT = 3;

    __root const CFG_HDR cfghdr4 = {CFG_LEVEL, "Center Softkey", 1, 0};
        
        __root const CFG_HDR cfghdr5 = {CFG_CBOX, "Indent", 0, 3};
        __root const int CENTER_INDENT = 0;
        __root const CFG_CBOX_ITEM cfgcbox0[3] = {"None", "Right", "Left"};
        
        __root const CFG_HDR cfghdr6 = {CFG_UINT, "Indent Value", 0, 30};
        __root const unsigned int CENTER_INDENT_VALUE = 0;
    
        __root const CFG_HDR cfghdr7 = {CFG_LEVEL, "", 0, 0};
    
    __root const CFG_HDR cfghdr8 = {CFG_LEVEL, "", 0, 0};

__root const CFG_HDR cfghdr9 = {CFG_LEVEL, "Images Names", 1, 0};

    __root const CFG_HDR cfghdr10_1 = {CFG_UTF16_STRING, "Left Soft", 0, 127};
    __root const wchar_t LEFT_SOFT[128] = L"left.png";

    __root const CFG_HDR cfghdr10_2 = {CFG_UTF16_STRING, "Center", 0, 127};
    __root const wchar_t CENTER[128] = L"center.png";

    __root const CFG_HDR cfghdr10_3 = {CFG_UTF16_STRING, "Right Soft", 0, 127};
    __root const wchar_t RIGHT_SOFT[128] = L"right.png";

    __root const CFG_HDR cfghdr11_1 = {CFG_UTF16_STRING, "Left Soft (Pressed)", 0, 127};
    __root const wchar_t LEFT_SOFT_PRESSED[128] = L"left_pressed.png";

    __root const CFG_HDR cfghdr11_2 = {CFG_UTF16_STRING, "Center (Pressed)", 0, 127};
    __root const wchar_t CENTER_PRESSED[128] = L"center_pressed.png";

    __root const CFG_HDR cfghdr11_3 = {CFG_UTF16_STRING, "Right Soft (Pressed)", 0, 127};
    __root const wchar_t RIGHT_SOFT_PRESSED[128] = L"right_pressed.png";

    __root const CFG_HDR cfghdr12 = {CFG_LEVEL, "", 0, 0};
    
__root const CFG_HDR cfghdr13 = {CFG_CHECKBOX, "Hide When Music Is Playing", 0, 0};
__root const int HIDE_WHEN_MUSIC_PLAYING = 0;
