/*
 * This file provides a AVM module call manner for embedded platform
 * Create By Shuo.Yan on 2017/11/21
 */

#include "stdio.h"
#include "camera_model.h"
#include "opencv/opencv.h"
#include "avm_data_type.h"
#include "avm_interface.h"

IplImage* temp_img_copy, *norm_img;

int main()
{
	AVM_Error_Code_E error = ERROR_NONE;
	Handle p_avm_handle = NULL;
	
	error = AVM_Init(&p_avm_handle);
	CHECK_ERROR(error);

	//avm_param;
	error = AVM_Process(p_avm_handle);
	CHECK_ERROR(error);
	

	return (Int32_t)error;
}