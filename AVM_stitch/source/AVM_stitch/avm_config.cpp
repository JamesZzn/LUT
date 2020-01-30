/*
 * This file config some default file path and name.
 * Create by Shuo.Yan on 2017/11/22
 */

#include "avm_config.h"

#if 1
const char* config_param_path = "../../config";


const char* source_image_path = "../../image/source";

const char* result_image_path = "../../image/result";

const char* lut_file_path = "../../lut_table";

const char* temp_file_path = "D:/Temp";



#if 1
char* ConfigFileName_c[VIEW_COUNT] =
{
	"modelParam_above_luminance_adjust.txt",
	"modelParam_incline_above.txt",
	"modelParam_incline_left.txt",
	"modelParam_incline_backleft.txt",
	"modelParam_incline_back.txt",
	"modelParam_incline_backright.txt",
	"modelParam_incline_right.txt",
	"modelParam_incline_frontright.txt",
	"modelParam_incline_front.txt",
	"modelParam_incline_frontleft.txt",
	"modelParam_horizontal_left.txt",
	"modelParam_horizontal_backleft.txt",
	"modelParam_horizontal_back.txt",
	"modelParam_horizontal_backright.txt",
	"modelParam_horizontal_right.txt",
	"modelParam_horizontal_frontright.txt",
	"modelParam_horizontal_front.txt",
	"modelParam_horizontal_frontleft.txt"
};

char* ResultLUTName_c[VIEW_COUNT] =
{
	"Lut_lumi_adjust.lut",
	"Lut_3D_30.lut",
	"Lut_3D_31.lut",
	"Lut_3D_32.lut",
	"Lut_3D_33.lut",
	"Lut_3D_34.lut",
	"Lut_3D_35.lut",
	"Lut_3D_36.lut",
	"Lut_3D_37.lut",
	"Lut_3D_38.lut",
	"Lut_3D_41.lut",
	"Lut_3D_42.lut",
	"Lut_3D_43.lut",
	"Lut_3D_44.lut",
	"Lut_3D_45.lut",
	"Lut_3D_46.lut",
	"Lut_3D_47.lut",
	"Lut_3D_48.lut"
};

char* ResultTxtLUTName_c[VIEW_COUNT] =
{
	"Lut_lumi_adjust.txt",
	"Lut_3D_30.txt",
	"Lut_3D_31.txt",
	"Lut_3D_32.txt",
	"Lut_3D_33.txt",
	"Lut_3D_34.txt",
	"Lut_3D_35.txt",
	"Lut_3D_36.txt",
	"Lut_3D_37.txt",
	"Lut_3D_38.txt",
	"Lut_3D_41.txt",
	"Lut_3D_42.txt",
	"Lut_3D_43.txt",
	"Lut_3D_44.txt",
	"Lut_3D_45.txt",
	"Lut_3D_46.txt",
	"Lut_3D_47.txt",
	"Lut_3D_48.txt"
};

char* ResultImageName_c[VIEW_COUNT] =
{
	"Panoram_lumi_adjust.bmp",
	"Panoram_3D_incline_above.bmp",
	"Panoram_3D_incline_left.bmp",
	"Panoram_3D_incline_backleft.bmp",
	"Panoram_3D_incline_back.bmp",
	"Panoram_3D_incline_backright.bmp",
	"Panoram_3D_incline_right.bmp",
	"Panoram_3D_incline_frontright.bmp",
	"Panoram_3D_incline_front.bmp",
	"Panoram_3D_incline_frontleft.bmp",
	"Panoram_3D_horizontal_left.bmp",
	"Panoram_3D_horizontal_backleft.bmp",
	"Panoram_3D_horizontal_back.bmp",
	"Panoram_3D_horizontal_backright.bmp",
	"Panoram_3D_horizontal_right.bmp",
	"Panoram_3D_horizontal_frontright.bmp",
	"Panoram_3D_horizontal_front.bmp",
	"Panoram_3D_horizontal_frontleft.bmp"
};


#endif







#endif

#if 0//发布版的配置文件
const char* config_param_path = "config";

const char* source_image_path = "image/source";

const char* result_image_path = "image/result";

const char* lut_file_path = "lut_table";

const char* temp_file_path = "D:/Temp";
#endif



char* camera_prefix_array[CAMERA_NUM] =
{
	"front",
	"rear",
	"left",
	"right"
};

char* view_name[10] =
{
	"_4_view_big",
	"_4_view_small",
	"_3_view_front",
	"_3_view_back",
	"_1_view_front",
	"_1_view_back",
	"_1_view_left",
	"_1_view_right",
	"_1_view_left_right"
};

