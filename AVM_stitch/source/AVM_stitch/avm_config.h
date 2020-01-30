/*
 * This file config some default file path and name.
 * Create by Shuo.Yan on 2017/11/22
 */

#ifndef __AVM_CONFIG_H_
#define __AVM_CONFIG_H_

#include "data_type.h"
#include <string>
using namespace std;
#define CAMERA_NUM 4
#define VIEW_COUNT (18)
extern const char* config_param_path;
extern const char* source_image_path;
extern const char* result_image_path;
extern const char* lut_file_path;
extern const char* temp_file_path;
extern char* camera_prefix_array[CAMERA_NUM];
extern char* view_name[10];
extern char* ConfigFileName_c[VIEW_COUNT];
extern char* ResultLUTName_c[VIEW_COUNT];
extern char* ResultTxtLUTName_c[VIEW_COUNT];
extern char* ResultImageName_c[VIEW_COUNT];

#define VIEW_COUNT_3D							(17)
#define RESULT_IMAGE_FORMAT						IMAGE_TYPE_YUV420

#define DEFAULT_3D_LUT_SIZE_SD					(512 * 1024)						// unit is byte
#define DEFAULT_3D_LUT_SIZE_HD					(2048 * 1024)						// unit	is byte


#if 0//备份
//CPU
#define MAX_DYNAMIC_LINE_COUNT					(63)								// for init memory malloc
#define DEFAULT_STATIC_DIS_LINE_SIZE			(66 * 1024)						// unit is byte
#define DEFAULT_DYNAMIC_DIS_LINE_SIZE			(4*1024)						// unit is byte
#define LINE_WIDTH                           3         //生成的距离线几个像素宽
#endif
#define RELEASE 0

#if 1
//GPU画线关键点
#define DEFAULT_STATIC_DIS_LINE_SIZE_DENSE			(66*1024)						// unit is byte
#define LINE_WIDTH_DENSE                           3  


#define MAX_DYNAMIC_LINE_COUNT					(63)		
#define DEFAULT_STATIC_DIS_LINE_SIZE			(1024)						// unit is byte
#define DEFAULT_DYNAMIC_DIS_LINE_SIZE			(150)						// unit is byte
#define LINE_WIDTH                           1         //生成的距离线几个像素宽
#endif

#define CURR_3D_LUT_SIZE						DEFAULT_3D_LUT_SIZE_SD				// the memory malloc for each lut of 3d views

#define USE_SMC 0     //使用SMC进行配置参数，用户
#define CAR_FIXED   1//小车固定的项目
#define CAR_DYNAMIC 0 //小车随着视野变化的项目
#define CAR_DYNAMIC_SMC 0

#define TEST 0//目前支持三个项目
#define CVBS 1//CVBS项目
#define HDMI 2


#define FULL_SCREEN 1
#define NARROW_SCREEN 0



//#define CAMERA_NUM					(4)

#define MAX_ARRAY_SIZE				(1024)

#define SOURCE_IMAGE_TYPE				IMAGE_TYPE_YUV422
#define RESULT_IMAGE_TYPE				IMAGE_TYPE_YUV422
#define LOGO_IMAGE_TYPE					IMAGE_TYPE_YUV420

#if 1
#define  DEUCE_LONG						2000//6.0
#define LINE_ACROSS_EXTEND				2000//6
#define EQUALITY_ANGLE                  2000// 6    
#endif

#define YUV 0
#define RGB 1

#define TILT_ANGLE_F 2
#define TILT_ANGLE_B 3

#define TILT_ANGLE_F_CVBS 12
#define TILT_ANGLE_B_CVBS 28 

#endif




#if 0


#include "data_type.h"
#include <string>
using namespace std;
#define CAMERA_NUM 4
#define VIEW_COUNT (18)
extern const char* config_param_path;
extern const char* source_image_path;
extern const char* result_image_path;
extern const char* lut_file_path;
extern const char* temp_file_path;
extern char* camera_prefix_array[CAMERA_NUM];
extern char* view_name[10];
extern char* ConfigFileName_c[VIEW_COUNT];
extern char* ResultLUTName_c[VIEW_COUNT];
extern char* ResultTxtLUTName_c[VIEW_COUNT];
extern char* ResultImageName_c[VIEW_COUNT];

#define VIEW_COUNT_3D							(17)
#define RESULT_IMAGE_FORMAT						IMAGE_TYPE_YUV420

#define DEFAULT_3D_LUT_SIZE_SD					(512 * 1024)						// unit is byte
#define DEFAULT_3D_LUT_SIZE_HD					(2048 * 1024)						// unit	is byte


#if 0//备份
//CPU密集点
#define MAX_DYNAMIC_LINE_COUNT					(63)								// for init memory malloc
#define DEFAULT_STATIC_DIS_LINE_SIZE			(256 * 1024)						// unit is byte
#define DEFAULT_DYNAMIC_DIS_LINE_SIZE			(128 * 1024*2)						// unit is byte

#endif



#if 1//备份
//CPU
#define MAX_DYNAMIC_LINE_COUNT					(63)								// for init memory malloc
#define DEFAULT_STATIC_DIS_LINE_SIZE			(66 * 1024)						// unit is byte
#define DEFAULT_DYNAMIC_DIS_LINE_SIZE			(4*1024)						// unit is byte

#endif


#if 0
//GPU画线关键点
#define MAX_DYNAMIC_LINE_COUNT					(63)		
#define DEFAULT_STATIC_DIS_LINE_SIZE			(1024)						// unit is byte
#define DEFAULT_DYNAMIC_DIS_LINE_SIZE			(150)						// unit is byte
#endif

#define CURR_3D_LUT_SIZE						DEFAULT_3D_LUT_SIZE_SD				// the memory malloc for each lut of 3d views
#define LINE_WIDTH                           3         //生成的距离线几个像素宽
#define USE_SMC 0     //使用SMC进行配置参数，用户
#define CAR_FIXED   1//小车固定的项目
#define CAR_DYNAMIC 0 //小车随着视野变化的项目

#define TEST 0//目前支持三个项目
#define CVBS 1//CVBS项目
#define HDMI 2


#define FULL_SCREEN 1
#define NARROW_SCREEN 0



//#define CAMERA_NUM					(4)

#define MAX_ARRAY_SIZE				(1024)

#define SOURCE_IMAGE_TYPE				IMAGE_TYPE_YUV422
#define RESULT_IMAGE_TYPE				IMAGE_TYPE_YUV422
#define LOGO_IMAGE_TYPE					IMAGE_TYPE_YUV420

#if 1
#define  DEUCE_LONG						2000//6.0
#define LINE_ACROSS_EXTEND				2000//6
#define EQUALITY_ANGLE                  2000// 6    
#endif
#if 0
#define  DEUCE_LONG						100//6.0
#define LINE_ACROSS_EXTEND				100//6
#define EQUALITY_ANGLE                  100// 6 
#endif
#if 0
#define  DEUCE_LONG						6.0
#define LINE_ACROSS_EXTEND				6
#define EQUALITY_ANGLE                  6    

#endif
#define YUV 0
#define RGB 1

#define TILT_ANGLE_F 1
#define TILT_ANGLE_B 1

#define TILT_ANGLE_F_CVBS 16
#define TILT_ANGLE_B_CVBS 28 



#endif