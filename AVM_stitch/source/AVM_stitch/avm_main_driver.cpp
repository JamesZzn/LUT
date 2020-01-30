/*
 * This file provides a AVM module call manner for embedded platform
 * Create By Shuo.Yan on 2017/11/21
 */

#include "stdio.h"
#include "camera_model.h"
#include "opencv/opencv.h"
#include "avm_data_type.h"
#include "avm_interface.h"
#include "generate_amba_table.h"
#include"SMC.h"
#include "avm_load_parameter.h"
IplImage* temp_img_copy, *norm_img;


int main()
{
	AVM_Error_Code_E error = ERROR_NONE;
	Handle p_avm_handle = NULL;
	P_Avm_Param_S p_avm_param;
#if 1
	/*********S32V项目********/
#if RELEASE==1
	config_param_path = "stitch/config/S32V_config/2D";
	//source_image_path = "image/source/S32V";
	source_image_path = "COMM/source_images";
	result_image_path = "stitch/image/result/S32V/2D";
	lut_file_path = "COMM/embed";




#else
	config_param_path = "../../config/S32V_config/2D";
	source_image_path = "../../image/source/S32V";
	result_image_path = "../../image/result/S32V/2D";
	lut_file_path = "../../lut_table/S32V/2D";
#endif

	error = AVM_Init(&p_avm_handle);
	CHECK_ERROR(error);
	p_avm_param = (P_Avm_Param_S)p_avm_handle;
	p_avm_param->project_flag = TEST;//支持项目的标志位
    
	error = S32V(p_avm_param);//自己测试的代码
	CHECK_ERROR(error);
#if RELEASE==1
	FILE* p_file_release;

	if (!(p_file_release = fopen("COMM/embed/user_config.txt", "rt+")))
	{
		printf("cannot be opened\n");

	}
	Load_Junc_View_4_Common_Param_write(
		p_avm_param, p_file_release);


	
#endif


	AVM_Deinit(p_avm_handle);
#endif




return (Int32_t)error;
}

