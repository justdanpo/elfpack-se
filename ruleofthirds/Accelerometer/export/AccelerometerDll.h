////////////////////////////////////////////////////////////////////////////////
//  AccelerometerDll.h
// 
//  Accelerometer dll interface for elfpack
//   (c) nkls
//  
//  Use at your own risk, in any way you want. For questions to me, the threads 
//  at se-nse.net and mobilefree.ru are open.
//
////////////////////////////////////////////////////////////////////////////////


#ifndef __ACCELEROMETER_DLL_H__
#define __ACCELEROMETER_DLL_H__


////////////////////////////////////////////////////////////////////////////////
// Declare the classes so that we're not dependant on accelerometer_interface.h
class IAccelerometerControl;
class IAccelerometerData;


////////////////////////////////////////////////////////////////////////////////
// DLL Version number
#define ACCELEROMETER_DLL_VERSION_NUMBER       0x02
#define ACCELEROMETER_DLL_VERSION_NUMBER_MINOR 0x00


////////////////////////////////////////////////////////////////////////////////
// Calibration data structure
//  Please note that this might not be the optimal way to calibrate the
//  accelerometer data sensor, it's just how I would do it. The calibration is
//  my own implementation, and not something I've reverse-engineered from OSE.
//  In short you would get the most accurate calibrated data like this:
//    if((x_raw - zero_x) > 0)
//      x_cal = (x_raw - x_zero) / (x_1g - x_zero);
//    else 
//      x_cal = (x_raw - x_zero) / (x_n1g - x_zero);
//  However, that high accuracy won't be possible du to the "noise" in the
//  accelerometer readings. For most applications (x_raw - x_zero) should be 
//  accurate enough.


typedef struct {
  int zero[3]; // zero_x = zero[0], zero_y = zero[1], zero_z = zero[2] ...
  int g[3];
  int ng[3];
} CALIBRATION_DATA;


////////////////////////////////////////////////////////////////////////////////
// Accelerometer dll data structure

typedef struct ACCELEROMETER_DLL_DATA _ACCELEROMETER_DLL_DATA;

typedef struct ACCELEROMETER_DLL_DATA
{
  // Mandatory member. Needed by UnLoadDLL.
  wchar_t * dll_name;
  
  // Dll version number (major). If this is lower than
  // ACCELEROMETER_DLL_VERSION_NUMBER you should not use this
  // structure and instead tell the user to go upgrade their
  // accelerometer.dll.
  u16 dll_ver_major;
  
  // Dll version number (minor). Not important for
  // applications, increases with bugfixes and extended
  // support.
  u16 dll_ver_minor;
  
  // int is_phone_supported()
  //   Returns zero if the phone is not supported. This is either because
  //   CoCreateInstance is missing from the library or because
  //   IAccelerometerManager could not be created.
  int  (*is_phone_supported)(ACCELEROMETER_DLL_DATA * data);
  
  // int open()
  //   Gives your DLL instance access to the accelerometer. Call before you want
  //   to read data through get_coords(), and call close() when you are done.
  //   Returns zero if it fails.
  int  (*open)(ACCELEROMETER_DLL_DATA * data);

  // void close()
  //   Close the access to the accelerometer.
  void (*close)(ACCELEROMETER_DLL_DATA * data);

  // void is_open()
  //   Tells you whether the access to the accelerometer of your dll instance
  //   is open or closed.
  int  (*is_open)(ACCELEROMETER_DLL_DATA * data);

  // int get_coords(int * x, int * y, int * z, int use_global_calibration)
  //   Read the accelerometer data. If the function returns a negative number it
  //   has failed, either because you haven't called open() before or because 
  //   the accelerometer data could not be read for some other reason.
  //   If calibrated_zero != 0 the function will calibrate the components
  //   before the function returns.
  int  (*get_coords)(ACCELEROMETER_DLL_DATA * data, int * x,  int * y, int * z, int use_global_calibration);

  // get_global_calibration
  //   Writes the current global calibration to cal.
  void (*get_global_calibration)(ACCELEROMETER_DLL_DATA * data, CALIBRATION_DATA * cal);

  // set_global_calibration
  //   Sets cal as the current global calibration.
  //   DON'T mix this up with get_global_calibration as you will overwrite the
  //   old global calibration data with this!
  //   Only use this function if you are making a calibrator.
  void (*set_global_calibration)(ACCELEROMETER_DLL_DATA * data, CALIBRATION_DATA * cal);

  // get_accelerometer_control & get_accelerometer_data
  //   Used for advanced accelerometer readings and settings.
  //   Don't "->Release()" these or use PowerOnChip() or PowerOffChip(), as 
  //   that will be handled by the DLL.
  IAccelerometerControl * (*get_accelerometer_control)(ACCELEROMETER_DLL_DATA * data);
  IAccelerometerData * (*get_accelerometer_data)(ACCELEROMETER_DLL_DATA * data);
  
#ifdef DLL_PRIVATE_AREA
#include "AccelerometerDLL_private.h"
#endif

}_ACCELEROMETER_DLL_DATA;

////////////////////////////////////////////////////////////////////////////////
// Definitions

#define ACCELEROMETER_Version(__this__)           __this__->dll_ver_major
#define ACCELEROMETER_IsPhoneSupported(__this__)  __this__->is_phone_supported(__this__)
#define ACCELEROMETER_Open(__this__)              __this__->open(__this__)
#define ACCELEROMETER_Close(__this__)             __this__->close(__this__)
#define ACCELEROMETER_IsOpen(__this__)            __this__->is_open(__this__)
#define ACCELEROMETER_GetCoords(__this__, ...)    __this__->get_coords(__this__,__VA_ARGS__)
#define ACCELEROMETER_GetGlobalCal(__this__, ...) __this__->get_global_calibration(__this__,__VA_ARGS__)
#define ACCELEROMETER_SetGlobalCal(__this__, ...) __this__->set_global_calibration(__this__,__VA_ARGS__)
#define ACCELEROMETER_GetAccCtrl(__this__)        __this__->get_accelerometer_control(__this__)
#define ACCELEROMETER_GetAccData(__this__)        __this__->get_accelerometer_data(__this__)

#endif
