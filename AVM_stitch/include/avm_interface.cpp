/*
* This file defines all interface of this module.
* Created by Shuo.Yan on 2017/11/23
*/
#include<math.h>
#include "avm_interface.h"
#include "avm_load_parameter.h"
#include "generate_4_view_lut.h"
#include "generate_3_view_lut.h"
#include "single_view.h"
#include "resolve_data.h"
#include"draw_static_dynamic_line.h"
#include"handle_picture_major_process.h"
#include"fwrite_bin.h"
#include"generate_amba_table.h"
#include"SMC.h"
#include<time.h>
#include"fread_bin.h"
#include"3D_licheng.h"
/*
* Function Name:				AVM_Malloc_Avm_Param_Memory
* Function Function:			malloc memory for Avm_Param_S structure
* Input:
*     p_avm_param:		the address of malloced memory init address
* Note:
* Revision:
*     Create by Shuo.Yan on 2018/1/15.
*/
static AVM_Error_Code_E AVM_Malloc_Avm_Param_Memory(
	OUT P_Avm_Param_S* pp_avm_param)
{
	AVM_Error_Code_E ret = ERROR_NONE;
	*pp_avm_param = (P_Avm_Param_S)malloc(sizeof(Avm_Param_S));
	if (!(*pp_avm_param))
	{
		ret = ERROR_MEMORY_MALLOC_FAIL;
		return ret;
	}
	memset((*pp_avm_param), 0, sizeof(Avm_Param_S));
	return ret;
}

/*
 * Function Name:				AVM_Malloc_Config_Param_Memory
 * Function Function:			malloc memory for Avm_Config_Param_T structure
 * Input:
 *     pp_avm_config_param:		the address of malloced memory init address
 * Note:     
 * Revision:
 *     Create by Shuo.Yan on 2017/11/23.
 */
static AVM_Error_Code_E AVM_Malloc_Config_Param_Memory(
	OUT P_Avm_Config_Param_S* pp_avm_config_param)
{
	AVM_Error_Code_E ret = ERROR_NONE;

	P_Avm_Config_Param_S p_avm_config_param = NULL;

	if (!p_avm_config_param)
	{
		p_avm_config_param = (P_Avm_Config_Param_S)
			malloc(sizeof(Avm_Config_Param_S));
		if (!p_avm_config_param)
		{
			ret = ERROR_MEMORY_MALLOC_FAIL;
			return ret;
		}
		memset(p_avm_config_param, 0, sizeof(Avm_Config_Param_S));
	}

	*pp_avm_config_param = p_avm_config_param;
	
	return ret;
}


AVM_Error_Code_E Smc_to_p_avm_config_param(
	OUT IN P_Avm_Param_S p_avm_param, IN  Smc_Cal_S Smc)
{
	AVM_Error_Code_E ret = ERROR_NONE;

	P_Avm_Config_Param_S p_avm_config_param = p_avm_param->p_avm_config_param;
	/*读取车辆的参数*/
	p_avm_config_param->vehicle_param.vehicle_length = Smc.veh_param.veh_length;
	p_avm_config_param->vehicle_param.vehicle_width = Smc.veh_param.veh_width;
	p_avm_config_param->vehicle_param.wheel_base = Smc.veh_param.veh_axis_length;
	p_avm_config_param->vehicle_param.front_overhang_length = Smc.veh_param.veh_fwheel2head;
	p_avm_config_param->vehicle_param.rear_overhang_length = Smc.veh_param.veh_rwheel2tail;
	for (Int32_t cam_id = 0; cam_id < CAMERA_NUM; cam_id++)
	{
		memcpy(&p_avm_config_param->camera_model[cam_id].camera_model_int, &Smc.camera_model[cam_id].cam_int, sizeof(Smc_Camera_Model_Cam_Int_S));
		memcpy(&p_avm_config_param->camera_model[cam_id].camera_model_ext, &Smc.camera_model[cam_id].cam_ext, sizeof(Smc_Camera_Model_Cam_Int_S));

		Sync_Camera_Model_Ext(&(p_avm_config_param->camera_model[cam_id].camera_model_ext));
	}
	/***************4路的配置文件****************/
	p_avm_config_param->avm_2d_param.junc_view_4_param.common_config.front_view_length = Smc.avm_2d_param.junc_view_4_param.junc_view_4_config_common.front_view_length;
	p_avm_config_param->avm_2d_param.junc_view_4_param.common_config.rear_view_length = Smc.avm_2d_param.junc_view_4_param.junc_view_4_config_common.rear_view_length;
	p_avm_config_param->avm_2d_param.junc_view_4_param.common_config.left_view_length = Smc.avm_2d_param.junc_view_4_param.junc_view_4_config_common.left_view_length;
	p_avm_config_param->avm_2d_param.junc_view_4_param.common_config.right_view_length = Smc.avm_2d_param.junc_view_4_param.junc_view_4_config_common.right_view_length;

	p_avm_config_param->avm_2d_param.junc_view_4_param.common_config.front_blind_length = Smc.avm_2d_param.junc_view_4_param.junc_view_4_config_common.front_blind_length;
	p_avm_config_param->avm_2d_param.junc_view_4_param.common_config.rear_blind_length = Smc.avm_2d_param.junc_view_4_param.junc_view_4_config_common.rear_blind_length;

	p_avm_config_param->avm_2d_param.junc_view_4_param.common_config.fl_fusion_angle = Smc.avm_2d_param.junc_view_4_param.junc_view_4_config_common.fl_fusion_angle;
	p_avm_config_param->avm_2d_param.junc_view_4_param.common_config.fr_fusion_angle = Smc.avm_2d_param.junc_view_4_param.junc_view_4_config_common.fr_fusion_angle;
	p_avm_config_param->avm_2d_param.junc_view_4_param.common_config.rl_fusion_angle = Smc.avm_2d_param.junc_view_4_param.junc_view_4_config_common.rl_fusion_angle;
	p_avm_config_param->avm_2d_param.junc_view_4_param.common_config.rr_fusion_angle = Smc.avm_2d_param.junc_view_4_param.junc_view_4_config_common.rr_fusion_angle;
	p_avm_config_param->avm_2d_param.junc_view_4_param.common_config.curve_range = Smc.avm_2d_param.junc_view_4_param.junc_view_4_config_common.curve_range;
	p_avm_config_param->avm_2d_param.junc_view_4_param.common_config.parallel_range = Smc.avm_2d_param.junc_view_4_param.junc_view_4_config_common.parallel_range;

	p_avm_config_param->avm_2d_param.junc_view_4_param.result_small_param.result_image_height = Smc.avm_2d_param.junc_view_4_param.small_view_result_size_param.result_image_height;
	p_avm_config_param->avm_2d_param.junc_view_4_param.result_small_param.result_image_width = Smc.avm_2d_param.junc_view_4_param.small_view_result_size_param.result_image_width;
	p_avm_config_param->avm_2d_param.junc_view_4_param.result_small_param.car_logo_rect.x = Smc.avm_2d_param.junc_view_4_param.small_view_result_size_param.car_logo_rect_x;
	p_avm_config_param->avm_2d_param.junc_view_4_param.result_small_param.car_logo_rect.y = Smc.avm_2d_param.junc_view_4_param.small_view_result_size_param.car_logo_rect_y;
	p_avm_config_param->avm_2d_param.junc_view_4_param.result_small_param.car_logo_rect.height = Smc.avm_2d_param.junc_view_4_param.small_view_result_size_param.car_logo_rect_height;
	p_avm_config_param->avm_2d_param.junc_view_4_param.result_small_param.car_logo_rect.width = Smc.avm_2d_param.junc_view_4_param.small_view_result_size_param.car_logo_rect_width;

	p_avm_config_param->avm_2d_param.junc_view_4_param.result_big_param.result_image_height = Smc.avm_2d_param.junc_view_4_param.big_view_result_size_param.result_image_height;
	p_avm_config_param->avm_2d_param.junc_view_4_param.result_big_param.result_image_width = Smc.avm_2d_param.junc_view_4_param.big_view_result_size_param.result_image_width;
	p_avm_config_param->avm_2d_param.junc_view_4_param.result_big_param.car_logo_rect.x = Smc.avm_2d_param.junc_view_4_param.big_view_result_size_param.car_logo_rect_x;
	p_avm_config_param->avm_2d_param.junc_view_4_param.result_big_param.car_logo_rect.y = Smc.avm_2d_param.junc_view_4_param.big_view_result_size_param.car_logo_rect_y;
	p_avm_config_param->avm_2d_param.junc_view_4_param.result_big_param.car_logo_rect.height = Smc.avm_2d_param.junc_view_4_param.big_view_result_size_param.car_logo_rect_height;
	p_avm_config_param->avm_2d_param.junc_view_4_param.result_big_param.car_logo_rect.width = Smc.avm_2d_param.junc_view_4_param.big_view_result_size_param.car_logo_rect_width;

	memcpy(&p_avm_config_param->avm_2d_param.junc_view_4_param.big_view_dis_line_param, &Smc.avm_2d_param.junc_view_4_param.big_view_dis_line_param, sizeof(Smc_Big_View_Dis_Line_Param_S));
	memcpy(&p_avm_config_param->avm_2d_param.junc_view_4_param.small_view_dis_line_param, &Smc.avm_2d_param.junc_view_4_param.big_view_dis_line_param, sizeof(Smc_Small_View_Dis_Line_Param_S));
#if CAR_DYNAMIC_SMC
	Junc_View_4_Param_S  junc_view_4_param = *p_junc_view_4_param;
	Float32_t pixel_row_dis = (p_vehicle_param->vehicle_length + junc_view_4_param.common_config.front_view_length + junc_view_4_param.common_config.rear_view_length)
		/ (Float32_t)(junc_view_4_param.result_big_param.result_image_height);//pixel distance
	Float32_t pixel_col_dis = (p_vehicle_param->vehicle_width + junc_view_4_param.common_config.left_view_length + junc_view_4_param.common_config.right_view_length)
		/ (Float32_t)(junc_view_4_param.result_big_param.result_image_width);//pixel distance

	Float32_t pixel_row_dis_small = (p_vehicle_param->vehicle_length + junc_view_4_param.common_config.front_view_length + junc_view_4_param.common_config.rear_view_length)
		/ (Float32_t)(junc_view_4_param.result_small_param.result_image_height);//pixel distance
	Float32_t pixel_col_dis_small = (p_vehicle_param->vehicle_width + junc_view_4_param.common_config.left_view_length + junc_view_4_param.common_config.right_view_length)
		/ (Float32_t)(junc_view_4_param.result_small_param.result_image_width);//pixel distance

	/***********calculate rect x,y width height**************/
	p_junc_view_4_param->result_big_param.car_logo_rect.width
		= (int)(p_vehicle_param->vehicle_width / pixel_col_dis);
	p_junc_view_4_param->result_big_param.car_logo_rect.height
		= p_vehicle_param->vehicle_length / pixel_row_dis;
	p_junc_view_4_param->result_big_param.car_logo_rect.x
		= (int)(junc_view_4_param.common_config.left_view_length / pixel_col_dis);
	p_junc_view_4_param->result_big_param.car_logo_rect.y = junc_view_4_param.common_config.front_view_length / pixel_row_dis;

	p_junc_view_4_param->result_small_param.car_logo_rect.width
		= (int)(p_vehicle_param->vehicle_width / pixel_col_dis_small);
	p_junc_view_4_param->result_small_param.car_logo_rect.height
		= (int)(p_vehicle_param->vehicle_length / pixel_row_dis_small);
	p_junc_view_4_param->result_small_param.car_logo_rect.x
		= (int)(junc_view_4_param.common_config.left_view_length / pixel_col_dis_small);
	p_junc_view_4_param->result_small_param.car_logo_rect.y
		= (int)(junc_view_4_param.common_config.front_view_length / pixel_row_dis_small);
#endif
	/****************因为重新下载了参数所以重新计算查找表的大小为申请内存做重新申请内存大小做准备***********************/
	P_Junc_View_4_Param_S p_junc_view_4_param = &p_avm_config_param->avm_2d_param.junc_view_4_param;
	// calc generated small 4-pass lut file size by the loaded parameters
	P_Result_Size_Param_S p_result_small_size_param =
		&p_junc_view_4_param->result_small_param;
	p_result_small_size_param->lut_size[0] =
		p_result_small_size_param->result_image_width
		* p_result_small_size_param->car_logo_rect.y;
	p_result_small_size_param->lut_size[1] =
		p_result_small_size_param->result_image_width
		* (p_result_small_size_param->result_image_height -
		(p_result_small_size_param->car_logo_rect.y +
		p_result_small_size_param->car_logo_rect.height)
		);
	p_result_small_size_param->lut_size[2] =
		p_result_small_size_param->result_image_height
		* p_result_small_size_param->car_logo_rect.x;
	p_result_small_size_param->lut_size[3] =
		p_result_small_size_param->result_image_height
		* (p_result_small_size_param->result_image_width - (p_result_small_size_param->car_logo_rect.x + p_result_small_size_param->car_logo_rect.width));

	// calc generated big 4-pass lut file size by the loaded parameters
	P_Result_Size_Param_S p_result_big_size_param =
		&p_junc_view_4_param->result_big_param;
	p_result_big_size_param->lut_size[0] =
		p_result_big_size_param->result_image_width
		* p_result_big_size_param->car_logo_rect.y;
	p_result_big_size_param->lut_size[1] =
		p_result_big_size_param->result_image_width
		* (p_result_big_size_param->result_image_height
		- (p_result_big_size_param->car_logo_rect.y
		+ p_result_big_size_param->car_logo_rect.height)
		);
	p_result_big_size_param->lut_size[2] =
		p_result_big_size_param->result_image_height
		* p_result_big_size_param->car_logo_rect.x;
	p_result_big_size_param->lut_size[3] =
		p_result_big_size_param->result_image_height
		* (p_result_big_size_param->result_image_width 
		- (p_result_big_size_param->car_logo_rect.x + p_result_big_size_param->car_logo_rect.width));

	
	/**************************三路qian配置文件*/
	p_avm_config_param->avm_2d_param.junc_view_3_front_param.common_config.front_view_length = Smc.avm_2d_param.junc_view_3_front_param.junc_view_3_config_common_front.front_view_length;
	p_avm_config_param->avm_2d_param.junc_view_3_front_param.common_config.left_view_length = Smc.avm_2d_param.junc_view_3_front_param.junc_view_3_config_common_front.left_view_length;
	p_avm_config_param->avm_2d_param.junc_view_3_front_param.common_config.right_view_length = Smc.avm_2d_param.junc_view_3_front_param.junc_view_3_config_common_front.right_view_length;
	p_avm_config_param->avm_2d_param.junc_view_3_front_param.common_config.car_head_length = Smc.avm_2d_param.junc_view_3_front_param.junc_view_3_config_common_front.car_head_length;

	p_avm_config_param->avm_2d_param.junc_view_3_front_param.common_config.front_blind_length = Smc.avm_2d_param.junc_view_3_front_param.junc_view_3_config_common_front.front_blind_length;
	p_avm_config_param->avm_2d_param.junc_view_3_front_param.common_config.fl_fusion_angle = Smc.avm_2d_param.junc_view_3_front_param.junc_view_3_config_common_front.rl_fusion_angle;
	p_avm_config_param->avm_2d_param.junc_view_3_front_param.common_config.fr_fusion_angle = Smc.avm_2d_param.junc_view_3_front_param.junc_view_3_config_common_front.fr_fusion_angle;
	p_avm_config_param->avm_2d_param.junc_view_3_front_param.common_config.curve_range = Smc.avm_2d_param.junc_view_3_front_param.junc_view_3_config_common_front.curve_range;
	p_avm_config_param->avm_2d_param.junc_view_3_front_param.common_config.parallel_range = Smc.avm_2d_param.junc_view_3_front_param.junc_view_3_config_common_front.parallel_range;


	p_avm_config_param->avm_2d_param.junc_view_3_front_param.result_3_front_param.result_image_height = Smc.avm_2d_param.junc_view_3_front_param.result_3_param_front.result_image_height;
	p_avm_config_param->avm_2d_param.junc_view_3_front_param.result_3_front_param.result_image_width = Smc.avm_2d_param.junc_view_3_front_param.result_3_param_front.result_image_width;
	p_avm_config_param->avm_2d_param.junc_view_3_front_param.result_3_front_param.car_logo_rect.x = Smc.avm_2d_param.junc_view_3_front_param.result_3_param_front.car_logo_rect_x;
	p_avm_config_param->avm_2d_param.junc_view_3_front_param.result_3_front_param.car_logo_rect.y = Smc.avm_2d_param.junc_view_3_front_param.result_3_param_front.car_logo_rect_y;
	p_avm_config_param->avm_2d_param.junc_view_3_front_param.result_3_front_param.car_logo_rect.height = Smc.avm_2d_param.junc_view_3_front_param.result_3_param_front.car_logo_rect_height;
	p_avm_config_param->avm_2d_param.junc_view_3_front_param.result_3_front_param.car_logo_rect.width = Smc.avm_2d_param.junc_view_3_front_param.result_3_param_front.car_logo_rect_width;

	memcpy(&p_avm_config_param->avm_2d_param.junc_view_3_front_param.junc_3_front_dis_line_param, &Smc.avm_2d_param.junc_view_3_front_param.junc_3_dis_line_param_front, sizeof(Smc_Junc_3_Dis_Line_Param_Front_S));

#if CAR_DYNAMIC_SMC//如果是动态的自己计算小车的位置
	/**********************calculate front rect****************************/
	Float32_t pixel_row_dis = (p_junc_view_3_front_param->common_config.car_head_length + p_junc_view_3_front_param->common_config.front_view_length)
		/ (Float32_t)(p_junc_view_3_front_param->result_3_front_param.result_image_height);//pixel distance
	Float32_t pixel_col_dis = (p_vehicle_param->vehicle_width + p_junc_view_3_front_param->common_config.left_view_length + p_junc_view_3_front_param->common_config.right_view_length)
		/ (Float32_t)(p_junc_view_3_front_param->result_3_front_param.result_image_width);//pixel distance

	p_junc_view_3_front_param->result_3_front_param.car_logo_rect.width = (int)(p_vehicle_param->vehicle_width / pixel_col_dis);
	p_junc_view_3_front_param->result_3_front_param.car_logo_rect.height = (int)(p_junc_view_3_front_param->common_config.car_head_length) / pixel_row_dis;
	p_junc_view_3_front_param->result_3_front_param.car_logo_rect.x = (int)(p_junc_view_3_front_param->common_config.left_view_length / pixel_col_dis);
	p_junc_view_3_front_param->result_3_front_param.car_logo_rect.y = (int)(p_junc_view_3_front_param->common_config.front_view_length / pixel_row_dis);
#endif
	P_Junc_View_3_Front_Param_S p_junc_view_3_front_param = &p_avm_config_param->avm_2d_param.junc_view_3_front_param;
	// calc generated front 3-pass lut file size by the loaded parameters
	P_Result_Size_Param_S p_result_3_front_param =
		&p_junc_view_3_front_param->result_3_front_param;
	p_result_3_front_param->lut_size[0] =
		p_result_3_front_param->result_image_width
		* p_result_3_front_param->car_logo_rect.y;
	p_result_3_front_param->lut_size[1] = 0;
	p_result_3_front_param->lut_size[2] =
		p_result_3_front_param->result_image_height
		* p_result_3_front_param->car_logo_rect.x;
	p_result_3_front_param->lut_size[3] =
		p_result_3_front_param->result_image_height
		* (p_result_3_front_param->result_image_width
		- (p_result_3_front_param->car_logo_rect.x
		+ p_result_3_front_param->car_logo_rect.width)
		);
	/**************************三路后配置文件*/

	p_avm_config_param->avm_2d_param.junc_view_3_rear_param.common_config.rear_view_length = Smc.avm_2d_param.junc_view_3_rear_param.junc_view_3_config_common_rear.front_view_length;
	p_avm_config_param->avm_2d_param.junc_view_3_rear_param.common_config.left_view_length = Smc.avm_2d_param.junc_view_3_rear_param.junc_view_3_config_common_rear.left_view_length;
	p_avm_config_param->avm_2d_param.junc_view_3_rear_param.common_config.right_view_length = Smc.avm_2d_param.junc_view_3_rear_param.junc_view_3_config_common_rear.right_view_length;
	p_avm_config_param->avm_2d_param.junc_view_3_rear_param.common_config.car_rear_length = Smc.avm_2d_param.junc_view_3_rear_param.junc_view_3_config_common_rear.car_head_length;


	p_avm_config_param->avm_2d_param.junc_view_3_rear_param.common_config.rear_blind_length = Smc.avm_2d_param.junc_view_3_rear_param.junc_view_3_config_common_rear.front_blind_length;
	p_avm_config_param->avm_2d_param.junc_view_3_rear_param.common_config.rl_fusion_angle = Smc.avm_2d_param.junc_view_3_rear_param.junc_view_3_config_common_rear.rl_fusion_angle;
	p_avm_config_param->avm_2d_param.junc_view_3_rear_param.common_config.rr_fusion_angle = Smc.avm_2d_param.junc_view_3_rear_param.junc_view_3_config_common_rear.fr_fusion_angle;
	p_avm_config_param->avm_2d_param.junc_view_3_rear_param.common_config.curve_range = Smc.avm_2d_param.junc_view_3_rear_param.junc_view_3_config_common_rear.curve_range;
	p_avm_config_param->avm_2d_param.junc_view_3_rear_param.common_config.parallel_range = Smc.avm_2d_param.junc_view_3_rear_param.junc_view_3_config_common_rear.parallel_range;


	p_avm_config_param->avm_2d_param.junc_view_3_rear_param.result_3_rear_param.result_image_height = Smc.avm_2d_param.junc_view_3_rear_param.result_3_param_rear.result_image_height;
	p_avm_config_param->avm_2d_param.junc_view_3_rear_param.result_3_rear_param.result_image_width = Smc.avm_2d_param.junc_view_3_rear_param.result_3_param_rear.result_image_width;
	p_avm_config_param->avm_2d_param.junc_view_3_rear_param.result_3_rear_param.car_logo_rect.x = Smc.avm_2d_param.junc_view_3_rear_param.result_3_param_rear.car_logo_rect_x;
	p_avm_config_param->avm_2d_param.junc_view_3_rear_param.result_3_rear_param.car_logo_rect.y = Smc.avm_2d_param.junc_view_3_rear_param.result_3_param_rear.car_logo_rect_y;
	p_avm_config_param->avm_2d_param.junc_view_3_rear_param.result_3_rear_param.car_logo_rect.height = Smc.avm_2d_param.junc_view_3_rear_param.result_3_param_rear.car_logo_rect_height;
	p_avm_config_param->avm_2d_param.junc_view_3_rear_param.result_3_rear_param.car_logo_rect.width = Smc.avm_2d_param.junc_view_3_rear_param.result_3_param_rear.car_logo_rect_width;

	memcpy(&p_avm_config_param->avm_2d_param.junc_view_3_rear_param.junc_3_rear_dis_line_param, &Smc.avm_2d_param.junc_view_3_rear_param.junc_3_dis_line_param_rear, sizeof(Smc_Junc_3_Dis_Line_Param_Rear_S));



#if CAR_DYNAMIC_SMC
	/**********************calculate rear rect****************************/
	Float32_t pixel_row_dis = (p_junc_view_3_rear_param->common_config.car_rear_length + p_junc_view_3_rear_param->common_config.rear_view_length)
		/ (Float32_t)(p_junc_view_3_rear_param->result_3_rear_param.result_image_height);//pixel distance
	Float32_t pixel_col_dis = (p_vehicle_param->vehicle_width + p_junc_view_3_rear_param->common_config.left_view_length + p_junc_view_3_rear_param->common_config.right_view_length)
		/ (Float32_t)(p_junc_view_3_rear_param->result_3_rear_param.result_image_width);//pixel distance

	p_junc_view_3_rear_param->result_3_rear_param.car_logo_rect.width = (int)(p_vehicle_param->vehicle_width / pixel_col_dis);
	p_junc_view_3_rear_param->result_3_rear_param.car_logo_rect.height = (int)(p_junc_view_3_rear_param->common_config.car_rear_length / pixel_row_dis);
	p_junc_view_3_rear_param->result_3_rear_param.car_logo_rect.x = (int)(p_junc_view_3_rear_param->common_config.left_view_length / pixel_col_dis);
	//p_junc_view_3_rear_param->result_3_rear_param.car_logo_rect.y = (int)(p_junc_view_3_rear_param->common_config.rear_view_length / pixel_row_dis);
	p_junc_view_3_rear_param->result_3_rear_param.car_logo_rect.y = 0;
#endif
	//P_Junc_View_3_Rear_Param_S p_result_3_rear_param = &p_avm_config_param->avm_2d_param.junc_view_3_rear_param;
	P_Result_Size_Param_S p_result_3_rear_param =
		&p_avm_config_param->avm_2d_param.junc_view_3_rear_param.result_3_rear_param;

	p_result_3_rear_param->lut_size[0] = 0;
	p_result_3_rear_param->lut_size[1] =
		p_result_3_rear_param->result_image_width
		* (p_result_3_rear_param->result_image_height
		- (p_result_3_rear_param->car_logo_rect.y
		+ p_result_3_rear_param->car_logo_rect.height)
		);
	p_result_3_rear_param->lut_size[2] = p_result_3_rear_param->result_image_height
		* p_result_3_rear_param->car_logo_rect.x;
	p_result_3_rear_param->lut_size[3] = p_result_3_rear_param->result_image_height
		* (p_result_3_rear_param->result_image_width
		- (p_result_3_rear_param->car_logo_rect.x
		+ p_result_3_rear_param->car_logo_rect.width)
		);

	/**************************单视图配置**********/
	/*前*/
	
	memcpy(&p_avm_config_param->avm_single_view_param.front_single_view_param, &Smc.avm_single_view_param.front_single_view_param, sizeof(Smc_Front_Single_View_Param_S));


	/*后*/
	memcpy(&p_avm_config_param->avm_single_view_param.rear_single_view_param, &Smc.avm_single_view_param.rear_single_view_param, sizeof(Smc_Rear_Single_View_Param_S));
	p_avm_config_param->avm_single_view_param.rear_single_view_param.crop_param.crop_rect.height = Smc.avm_single_view_param.rear_single_view_param.single_view_crop_param_rear.car_logo_rect_height;
	p_avm_config_param->avm_single_view_param.rear_single_view_param.crop_param.crop_rect.width = Smc.avm_single_view_param.rear_single_view_param.single_view_crop_param_rear.car_logo_rect_width;
	p_avm_config_param->avm_single_view_param.rear_single_view_param.crop_param.crop_rect.x = Smc.avm_single_view_param.rear_single_view_param.single_view_crop_param_rear.car_logo_rect_x;
	p_avm_config_param->avm_single_view_param.rear_single_view_param.crop_param.crop_rect.y = Smc.avm_single_view_param.rear_single_view_param.single_view_crop_param_rear.car_logo_rect_y;
	/*左*/
	memcpy(&p_avm_config_param->avm_single_view_param.left_single_view_param, &Smc.avm_single_view_param.left_single_view_param, sizeof(Smc_Left_Single_View_Param_S));
	p_avm_config_param->avm_single_view_param.left_single_view_param.plane_expand_param.trans_y = Smc.avm_single_view_param.left_single_view_param.single_view_plane_expand_param_left.trans_y;
	p_avm_config_param->avm_single_view_param.left_single_view_param.plane_expand_param.car_body_width= Smc.avm_single_view_param.left_single_view_param.single_view_plane_expand_param_left.car_body_width;
	p_avm_config_param->avm_single_view_param.left_single_view_param.plane_expand_param.world_width = Smc.avm_single_view_param.left_single_view_param.single_view_plane_expand_param_left.world_width;
	/*右*/
	memcpy(&p_avm_config_param->avm_single_view_param.right_single_view_param, &Smc.avm_single_view_param.right_single_view_param, sizeof(Smc_Right_Single_View_Param_S));
	
	p_avm_config_param->avm_single_view_param.right_single_view_param.plane_expand_param.trans_y = Smc.avm_single_view_param.right_single_view_param.single_view_plane_expand_param_right.trans_y;
	p_avm_config_param->avm_single_view_param.right_single_view_param.plane_expand_param.car_body_width = Smc.avm_single_view_param.right_single_view_param.single_view_plane_expand_param_right.car_body_width;
	p_avm_config_param->avm_single_view_param.right_single_view_param.plane_expand_param.world_width = Smc.avm_single_view_param.right_single_view_param.single_view_plane_expand_param_right.world_width;

	/**左右**/

	memcpy(&p_avm_config_param->avm_single_view_param.left_right_view_param, &Smc.avm_single_view_param.left_right_view_param, sizeof(Smc_Right_Single_View_Param_S));
	p_avm_config_param->avm_single_view_param.left_right_view_param.plane_expand_param_right.trans_y = Smc.avm_single_view_param.left_right_view_param.plane_expand_param_right.trans_y;
	p_avm_config_param->avm_single_view_param.left_right_view_param.plane_expand_param_right.car_body_width = Smc.avm_single_view_param.left_right_view_param.plane_expand_param_right.car_body_width;
	p_avm_config_param->avm_single_view_param.left_right_view_param.plane_expand_param_right.world_width = Smc.avm_single_view_param.left_right_view_param.plane_expand_param_right.world_width;
	p_avm_config_param->avm_single_view_param.left_right_view_param.plane_expand_param_right.world_height = Smc.avm_single_view_param.left_right_view_param.plane_expand_param_right.world_height;



	//重新下载了参数要重新申请内存
	//ret = AVM_Malloc_Lut_Memory(
	//	p_avm_config_param, &p_avm_param->p_avm_lut);
	//CHECK_ERROR(ret);
	return ret;
}


/*
 * Function Name:				AVM_Load_Config_Param
 * Function Function:			load config param for AVM module
 * Input:
 *     p_avm_param:				the pointer point to Avm_Param_T structure
 * Note:
 *     all params stored on disk can only be loaded through this function
 * Revision:
 *     Create by Shuo.Yan on 2017/11/23.
 */
static AVM_Error_Code_E AVM_Load_Config_Param(OUT IN P_Avm_Param_S p_avm_param,
	OUT IN P_Avm_Config_Param_S p_avm_config_param)
{
	AVM_Error_Code_E ret = ERROR_NONE;
#if USE_SMC
	Smc_Cal_S Smc;
	char SMC_path_name[MAX_ARRAY_SIZE];
	sprintf(SMC_path_name, "%s/SMC/small.bin", config_param_path);
	FILE* pf = fopen(SMC_path_name, "rb+");
	if (!pf)
	{
		ret = ERROR_OPEN_FILE_FIAL;
		CHECK_ERROR(ret);
	}

	fread(&Smc, 1, sizeof(Smc_Cal_S), pf);

	Smc_to_p_avm_config_param(
		p_avm_param, Smc);

#else

	//AVM_SMC(NONE_1_0_0_SD_CVBS_2018_5_28).bin;

	// load vehicle config parameters
	char vehicle_param_name[MAX_ARRAY_SIZE];
	sprintf(vehicle_param_name, "%s/vehicle_info.txt", config_param_path);
	ret = (AVM_Error_Code_E)Get_Vehicle_Info(
		&p_avm_config_param->vehicle_param, vehicle_param_name);
	CHECK_ERROR(ret);

#if RELEASE==1
	char*release_config_intrinsic, *release_config_extrinsic;
	release_config_intrinsic = "COMM/camera_parameter";
	release_config_extrinsic = "COMM/camera_parameter";//eol/eol_result/cameradata
	// load intrinsic and extrinsic parameters
	for (Int32_t cam_id = 0; cam_id < CAMERA_NUM; cam_id++)
	{
		if (cam_id != 1)
		{
			// load camera intrinsic parameters
			char intrinsic_param_name[MAX_ARRAY_SIZE];
			sprintf(intrinsic_param_name, "%s/ocam_intrinsic_%s.txt",
				release_config_intrinsic, camera_prefix_array[cam_id]);
			ret = (AVM_Error_Code_E)Load_Camera_Model_Int(
				&(p_avm_config_param->camera_model[cam_id].camera_model_int),
				intrinsic_param_name);
			CHECK_ERROR(ret);
			// load camera extrinsic parameters
			char extrinsic_param_name[MAX_ARRAY_SIZE];
			sprintf(extrinsic_param_name, "%s/camera_model_ext_%s.txt",
				release_config_extrinsic, camera_prefix_array[cam_id]);
			ret = (AVM_Error_Code_E)Load_Camera_Model_Ext(
				&p_avm_config_param->camera_model[cam_id].camera_model_ext,
				extrinsic_param_name);
			CHECK_ERROR(ret);
			Sync_Camera_Model_Ext(&(p_avm_config_param->camera_model[cam_id].camera_model_ext));
		}
		else
		{
			// load camera intrinsic parameters
			char intrinsic_param_name[MAX_ARRAY_SIZE];
			sprintf(intrinsic_param_name, "%s/ocam_intrinsic_back.txt", release_config_intrinsic);
			ret = (AVM_Error_Code_E)Load_Camera_Model_Int(
				&(p_avm_config_param->camera_model[cam_id].camera_model_int),
				intrinsic_param_name);
			CHECK_ERROR(ret);
			// load camera extrinsic parameters
			char extrinsic_param_name[MAX_ARRAY_SIZE];
			sprintf(extrinsic_param_name, "%s/camera_model_ext_back.txt", release_config_extrinsic);
			ret = (AVM_Error_Code_E)Load_Camera_Model_Ext(
				&p_avm_config_param->camera_model[cam_id].camera_model_ext,
				extrinsic_param_name);
			CHECK_ERROR(ret);
			Sync_Camera_Model_Ext(&(p_avm_config_param->camera_model[cam_id].camera_model_ext));
		}

	}

#else

	// load intrinsic and extrinsic parameters
	for (Int32_t cam_id = 0; cam_id < CAMERA_NUM; cam_id++)
	{
		if (cam_id != 1)
		{
			// load camera intrinsic parameters
			char intrinsic_param_name[MAX_ARRAY_SIZE];
			sprintf(intrinsic_param_name, "%s/ocam_intrinsic_%s.txt",
				config_param_path, camera_prefix_array[cam_id]);
			ret = (AVM_Error_Code_E)Load_Camera_Model_Int(
				&(p_avm_config_param->camera_model[cam_id].camera_model_int),
				intrinsic_param_name);
			CHECK_ERROR(ret);
			// load camera extrinsic parameters
			char extrinsic_param_name[MAX_ARRAY_SIZE];
			sprintf(extrinsic_param_name, "%s/camera_model_ext_%s.txt",
				config_param_path, camera_prefix_array[cam_id]);
			ret = (AVM_Error_Code_E)Load_Camera_Model_Ext(
				&p_avm_config_param->camera_model[cam_id].camera_model_ext,
				extrinsic_param_name);
			CHECK_ERROR(ret);
			Sync_Camera_Model_Ext(&(p_avm_config_param->camera_model[cam_id].camera_model_ext));
		}
		else
		{
			// load camera intrinsic parameters
			char intrinsic_param_name[MAX_ARRAY_SIZE];
			sprintf(intrinsic_param_name, "%s/ocam_intrinsic_back.txt", config_param_path);
			ret = (AVM_Error_Code_E)Load_Camera_Model_Int(
				&(p_avm_config_param->camera_model[cam_id].camera_model_int),
				intrinsic_param_name);
			CHECK_ERROR(ret);
			// load camera extrinsic parameters
			char extrinsic_param_name[MAX_ARRAY_SIZE];
			sprintf(extrinsic_param_name, "%s/camera_model_ext_back.txt", config_param_path);
			ret = (AVM_Error_Code_E)Load_Camera_Model_Ext(
				&p_avm_config_param->camera_model[cam_id].camera_model_ext,
				extrinsic_param_name);
			CHECK_ERROR(ret);
			Sync_Camera_Model_Ext(&(p_avm_config_param->camera_model[cam_id].camera_model_ext));
		}

	}


#endif





	// load junc 4-pass view config parameters, 
	// both small and big view are loaded in this function
	char avm_junc_4_param_name[MAX_ARRAY_SIZE];
	sprintf(avm_junc_4_param_name, "%s/avm_2d_junction_4_param.txt", 
		config_param_path);
	ret = Load_Junc_View_4_Param(
		&p_avm_config_param->avm_2d_param.junc_view_4_param, avm_junc_4_param_name, &p_avm_config_param->vehicle_param);
	CHECK_ERROR(ret);
	// load junc 3-pass front view config parameters
	char avm_junc_3_front_param_name[MAX_ARRAY_SIZE];
	sprintf(avm_junc_3_front_param_name, "%s/avm_2d_junction_3_front_param.txt", 
		config_param_path);
	ret = Load_Junc_View_3_Front_Param(
		&(p_avm_config_param->avm_2d_param.junc_view_3_front_param), 
		avm_junc_3_front_param_name, &p_avm_config_param->vehicle_param);
	CHECK_ERROR(ret);
	// load junc 3-pass rear view config parameters
	char avm_junc_3_rear_param_name[MAX_ARRAY_SIZE];
	sprintf(avm_junc_3_rear_param_name, "%s/avm_2d_junction_3_rear_param.txt", 
		config_param_path);
	ret = Load_Junc_View_3_Rear_Param(
		&p_avm_config_param->avm_2d_param.junc_view_3_rear_param, 
		avm_junc_3_rear_param_name, &p_avm_config_param->vehicle_param);
	CHECK_ERROR(ret);
	// load awb config parameters
	char awb_param_name[MAX_ARRAY_SIZE];
	sprintf(awb_param_name, "%s/avm_2d_awb_param.txt", config_param_path);
	ret = Load_Awb_View_Param(&p_avm_config_param->avm_2d_param.awb_view_param,
		awb_param_name,  &p_avm_config_param->vehicle_param);
	CHECK_ERROR(ret);

	// load front view config parameters
	char front_single_view_param_name[MAX_ARRAY_SIZE];
	sprintf(front_single_view_param_name, "%s/avm_single_view_front.txt",
		config_param_path);
	ret = Load_Front_Rear_Single_View_Param(
		&p_avm_config_param->avm_single_view_param.front_single_view_param,
		front_single_view_param_name);
	CHECK_ERROR(ret);

	// load rear view config parameters
	char rear_single_view_param_name[MAX_ARRAY_SIZE];
	sprintf(rear_single_view_param_name, "%s/avm_single_view_rear.txt",
		config_param_path);
	ret = Load_Front_Rear_Single_View_Param(
		&p_avm_config_param->avm_single_view_param.rear_single_view_param,
		rear_single_view_param_name);
	CHECK_ERROR(ret);

	// load left view config parameters
	char left_single_view_param_name[MAX_ARRAY_SIZE];
	sprintf(left_single_view_param_name, "%s/avm_single_view_left.txt",
		config_param_path);
	ret = Load_Left_Right_Single_View_Param(
		&p_avm_config_param->avm_single_view_param.left_single_view_param,
		left_single_view_param_name);
	CHECK_ERROR(ret);

	// load right view config parameters
	char right_single_view_param_name[MAX_ARRAY_SIZE];
	sprintf(right_single_view_param_name, "%s/avm_single_view_right.txt",
		config_param_path);
	ret = Load_Left_Right_Single_View_Param(
		&p_avm_config_param->avm_single_view_param.right_single_view_param,
		right_single_view_param_name);
	CHECK_ERROR(ret);
	char left_right_single_view_param_name[MAX_ARRAY_SIZE];
	sprintf(left_right_single_view_param_name, "%s/avm_single_view_left_right.txt",
		config_param_path);
	// load left and right together view config parameters
	ret = Load_Left_Right_Together_View_Param(
		&p_avm_config_param->avm_single_view_param.left_right_view_param,
		left_right_single_view_param_name, right_single_view_param_name);
	CHECK_ERROR(ret);



#endif
	return ret;
}





/*
 * Function Name:				AVM_Malloc_Lut_Memory
 * Function Function:			malloc memory for Avm_Lut_T structure
 * Input:
 *     p_avm_config_param:		the param is use to calculate the memory size need to be malloced
 *     pp_avm_lut:				the address of malloced memory init address
 * Note:
 * Revision:
 *     Create by Shuo.Yan on 2017/11/24.
 */
AVM_Error_Code_E AVM_Malloc_Lut_Memory(
	IN P_Avm_Config_Param_S p_avm_config_param, 
	OUT P_Avm_Lut_S* pp_avm_lut)
{
	AVM_Error_Code_E ret = ERROR_NONE;

	P_Avm_Lut_S p_avm_lut = NULL;

	// calculate how many byte of memory should be malloc for lut structure
	Int32_t total_size; // all lut table's memory sum
	Int32_t single_view_size_total, junc_view_size, _3d_view_size = 0;
	Int32_t single_view_size[4], single_view_size_left_right;
	Int32_t junc_view_4_size, awb_view_size = 0, junc_view_3_front_size = 0, junc_view_3_rear_size = 0;
	Int32_t junc_view_4_small_size = 0, junc_view_4_big_size = 0, _3d_view_size_single;

	// accumulate single view lut buffer size
	P_Avm_Single_View_Param_S p_single_view_param = &p_avm_config_param->avm_single_view_param;

	single_view_size[0] = sizeof(UInt8_t)
		* p_single_view_param->front_single_view_param.result_image_height
		* p_single_view_param->front_single_view_param.result_image_width * 8;
	single_view_size[1] = sizeof(UInt8_t)
		* p_single_view_param->rear_single_view_param.result_image_height
		* p_single_view_param->rear_single_view_param.result_image_width * 8;
	single_view_size[2] = sizeof(UInt8_t)
		* p_single_view_param->left_single_view_param.result_image_height
		* p_single_view_param->left_single_view_param.result_image_width * 8;
	single_view_size[3] = sizeof(UInt8_t) 
		* p_single_view_param->right_single_view_param.result_image_height
		* p_single_view_param->right_single_view_param.result_image_width * 8;

	single_view_size_left_right = sizeof(UInt8_t) 
		* p_single_view_param->left_right_view_param.result_image_height
		* p_single_view_param->left_right_view_param.result_image_width * 8;

	single_view_size_total = single_view_size[0] + single_view_size[1] + single_view_size[2]
		+ single_view_size[3] + single_view_size_left_right;

	// accumulate junction 4-pass view lut buffer size
	P_Result_Size_Param_S p_junc_view_4_small_param = 
		&p_avm_config_param->avm_2d_param.junc_view_4_param.result_small_param;
	P_Result_Size_Param_S p_junc_view_4_big_param = 
		&p_avm_config_param->avm_2d_param.junc_view_4_param.result_big_param;

	for (Int32_t cam_id = 0; cam_id < CAMERA_NUM; cam_id++)
	{
		junc_view_4_small_size += sizeof(UInt8_t) * p_junc_view_4_small_param->lut_size[cam_id] * 8;
		junc_view_4_big_size += sizeof(UInt8_t) * p_junc_view_4_big_param->lut_size[cam_id] * 8;
	}

	junc_view_4_size = junc_view_4_small_size + junc_view_4_big_size;

	// accumulate awb view lut size
	P_Result_Size_Param_S p_awb_view_param = 
		&p_avm_config_param->avm_2d_param.awb_view_param.result_awb_param;
	for (Int32_t cam_id = 0; cam_id < CAMERA_NUM; cam_id++)
	{
		awb_view_size += sizeof(UInt8_t) * p_awb_view_param->lut_size[cam_id] * 8;
	}

	// accumulate junction 3-pass view lut buffer size
	P_Junc_View_3_Front_Param_S p_junc_view_3_front_param = 
		&p_avm_config_param->avm_2d_param.junc_view_3_front_param;
	P_Junc_View_3_Rear_Param_S p_junc_view_3_rear_param = 
		&p_avm_config_param->avm_2d_param.junc_view_3_rear_param;

	for (Int32_t cam_id = 0; cam_id < 4; cam_id++)
	{
		junc_view_3_front_size = junc_view_3_front_size + sizeof(UInt8_t)
			* p_junc_view_3_front_param->result_3_front_param.lut_size[cam_id] * 8;
		junc_view_3_rear_size += sizeof(UInt8_t)
			* p_junc_view_3_rear_param->result_3_rear_param.lut_size[cam_id] * 8;
	}

	junc_view_size = junc_view_4_size + awb_view_size + junc_view_3_front_size + junc_view_3_rear_size;

	// accumulate 3d view lut buffer size
	for (Int32_t view_id = 0; view_id < VIEW_COUNT_3D; view_id++)
	{
		_3d_view_size_single = sizeof(UInt8_t) * CURR_3D_LUT_SIZE;
		_3d_view_size += _3d_view_size_single;
	}

	total_size = single_view_size_total + junc_view_size + _3d_view_size;

	//// ****************malloc memory for lut by calculated size***************************//
	// malloc memory for each lut pointer
	p_avm_lut = (P_Avm_Lut_S)malloc(sizeof(Avm_Lut_S));
	if (!p_avm_lut)
	{
		ret = ERROR_MEMORY_MALLOC_FAIL;
		CHECK_ERROR(ret);
	}
	memset(p_avm_lut, 0, sizeof(Avm_Lut_S));
	

	// malloc memory for single view lut
	ret = Avm_Memory_Malloc_UInt32(&p_avm_lut->p_lut_single_view_front, single_view_size[0]);
	CHECK_ERROR(ret);
	ret = Avm_Memory_Malloc_UInt32(&p_avm_lut->p_lut_single_view_rear, single_view_size[1]);
	CHECK_ERROR(ret);
	ret = Avm_Memory_Malloc_UInt32(&p_avm_lut->p_lut_single_view_left, single_view_size[2]);
	CHECK_ERROR(ret);
	ret = Avm_Memory_Malloc_UInt32(&p_avm_lut->p_lut_single_view_right, single_view_size[3]);
	CHECK_ERROR(ret);
	ret = Avm_Memory_Malloc_UInt32(&p_avm_lut->p_lut_single_view_left_right, single_view_size_left_right);
	CHECK_ERROR(ret);

	// malloc memory for 4-pass small junction view lut
	ret = Avm_Memory_Malloc_UInt32(&p_avm_lut->p_lut_junc_view_4_small, junc_view_4_small_size);
	CHECK_ERROR(ret);

	// malloc memory for 4-pass big junction view lut
	ret = Avm_Memory_Malloc_UInt32(&p_avm_lut->p_lut_junc_view_4_big, junc_view_4_big_size);
	CHECK_ERROR(ret);

	// malloc memory for 3-pass front junction view lut
	ret = Avm_Memory_Malloc_UInt32(&p_avm_lut->p_lut_junc_view_3_front, junc_view_3_front_size);
	CHECK_ERROR(ret);

	// malloc memory for 3-pass rear junction view lut
	ret = Avm_Memory_Malloc_UInt32(&p_avm_lut->p_lut_junc_view_3_rear, junc_view_3_rear_size);
	CHECK_ERROR(ret);
	
	// malloc memory for awb lut
	ret = Avm_Memory_Malloc_UInt32(&p_avm_lut->p_lut_awb, awb_view_size);
	CHECK_ERROR(ret);
		
	/***********3D查找表没有用了******/
	*pp_avm_lut = p_avm_lut;

	return ret;
}

/*
 * Function Name:				AVM_Malloc_Image_Memory
 * Function Function:			malloc memory for Avm_Image_T structure
 * Input:
 *     p_avm_config_param:		the param is use to calculate the memory size need to be malloced
 *     pp_avm_image:			the address of malloced memory init address
 * Note:
 * Revision:
 *     Create by Shuo.Yan on 2017/11/25.
 */
 AVM_Error_Code_E AVM_Malloc_Image_Memory(
	IN P_Avm_Config_Param_S p_avm_config_param, 
	OUT P_Avm_Image_S* pp_avm_image)
{
	AVM_Error_Code_E ret = ERROR_NONE;

	Int32_t src_image_height, src_image_width;
	Int32_t front_view_height, front_view_width, rear_view_height, rear_view_width;
	Int32_t left_view_height, left_view_width, right_view_height, right_view_width;
	Int32_t left_right_view_height, left_right_view_width;
	Int32_t junc_4_small_view_height, junc_4_small_view_width;
	Int32_t junc_4_big_view_height, junc_4_big_view_width;
	Int32_t junc_3_front_view_height, junc_3_front_view_width;
	Int32_t junc_3_rear_view_height, junc_3_rear_view_width;
	Int32_t awb_view_height, awb_view_width, _3d_view_height, _3d_view_width;
	Int32_t junc_4_view_small_car_logo_height, junc_4_view_small_car_logo_width;
	Int32_t junc_4_view_big_car_logo_height, junc_4_view_big_car_logo_width;
	Int32_t junc_3_front_view_car_logo_height, junc_3_front_view_car_logo_width;
	Int32_t junc_3_rear_view_car_logo_height, junc_3_rear_view_car_logo_width;
	Int32_t _3d_car_logo_height[VIEW_COUNT_3D], _3d_car_logo_width[VIEW_COUNT_3D];

	P_Avm_Image_S p_avm_image = NULL;
	p_avm_image = (P_Avm_Image_S)malloc(sizeof(Avm_Image_S));
	if (!p_avm_image)
	{
		ret = ERROR_MEMORY_MALLOC_FAIL;
		CHECK_ERROR(ret);
	}
	memset(p_avm_image, 0, sizeof(Avm_Image_S));

	// malloc source image memory	
	for (Int32_t cam_id = 0; cam_id < CAMERA_NUM; cam_id++)
	{
		src_image_height = p_avm_config_param->camera_model[cam_id].camera_model_int.height;
		src_image_width  = p_avm_config_param->camera_model[cam_id].camera_model_int.width;
		ret = (AVM_Error_Code_E)Create_Intesight_Image(&p_avm_image->source_image_array[cam_id], 
			SOURCE_IMAGE_TYPE, src_image_height, src_image_width);
		CHECK_ERROR(ret);
	}

	//malloc result image memory
	P_Avm_Result_Image_S p_avm_result = &p_avm_image->result_image;
#if 1
	// malloc front single view image memory
	front_view_height = 
		p_avm_config_param->avm_single_view_param.front_single_view_param.result_image_height;
	front_view_width = 
		p_avm_config_param->avm_single_view_param.front_single_view_param.result_image_width;
	ret = (AVM_Error_Code_E)Create_Intesight_Image(&p_avm_result->result_image_single_view_front, 
		RESULT_IMAGE_TYPE, front_view_height, front_view_width);
	CHECK_ERROR(ret);

	// malloc rear single view image memory
	rear_view_height = 
		p_avm_config_param->avm_single_view_param.rear_single_view_param.result_image_height;
	rear_view_width = 
		p_avm_config_param->avm_single_view_param.rear_single_view_param.result_image_width;
	ret = (AVM_Error_Code_E)Create_Intesight_Image(&p_avm_result->result_image_single_view_rear, 
		RESULT_IMAGE_TYPE, rear_view_height, rear_view_width);
	CHECK_ERROR(ret);

	// malloc left single view image memory
	left_view_height = 
		p_avm_config_param->avm_single_view_param.left_single_view_param.result_image_height;
	left_view_width = 
		p_avm_config_param->avm_single_view_param.left_single_view_param.result_image_width;
	ret = (AVM_Error_Code_E)Create_Intesight_Image(&p_avm_result->result_image_single_view_left, 
		RESULT_IMAGE_TYPE, left_view_height, left_view_width);
	CHECK_ERROR(ret);

	// malloc right single view image memory
	right_view_height = 
		p_avm_config_param->avm_single_view_param.right_single_view_param.result_image_height;
	right_view_width = 
		p_avm_config_param->avm_single_view_param.right_single_view_param.result_image_width;
	ret = (AVM_Error_Code_E)Create_Intesight_Image(&p_avm_result->result_image_single_view_right, 
		RESULT_IMAGE_TYPE, right_view_height, right_view_width);
	CHECK_ERROR(ret);

	// malloc left right together single view image memory
	left_right_view_height = 
		p_avm_config_param->avm_single_view_param.left_right_view_param.result_image_height;
	left_right_view_width  = 
		p_avm_config_param->avm_single_view_param.left_right_view_param.result_image_width;
	ret = (AVM_Error_Code_E)Create_Intesight_Image(&p_avm_result->result_image_single_view_left_right, 
		RESULT_IMAGE_TYPE, left_right_view_height, left_right_view_width);
	CHECK_ERROR(ret);
#if 1
	// malloc junc 4-pass small view memory
	junc_4_small_view_height = 
		p_avm_config_param->avm_2d_param.junc_view_4_param.result_small_param.result_image_height;
	junc_4_small_view_width = 
		p_avm_config_param->avm_2d_param.junc_view_4_param.result_small_param.result_image_width;
	ret = (AVM_Error_Code_E)Create_Intesight_Image(&p_avm_result->result_image_junc_view_4_small, 
		RESULT_IMAGE_TYPE, junc_4_small_view_height, junc_4_small_view_width);
	CHECK_ERROR(ret);

	// malloc junc 4-pass big view memory
	junc_4_big_view_height = 
		p_avm_config_param->avm_2d_param.junc_view_4_param.result_big_param.result_image_height;
	junc_4_big_view_width = 
		p_avm_config_param->avm_2d_param.junc_view_4_param.result_big_param.result_image_width;
	ret = (AVM_Error_Code_E)Create_Intesight_Image(&p_avm_result->result_image_junc_view_4_big, 
 		RESULT_IMAGE_TYPE, junc_4_big_view_height, junc_4_big_view_width);
	CHECK_ERROR(ret);

	// malloc junc 3 front view memory
	junc_3_front_view_height = 
		p_avm_config_param->avm_2d_param.junc_view_3_front_param.result_3_front_param.result_image_height;
	junc_3_front_view_width = 
		p_avm_config_param->avm_2d_param.junc_view_3_front_param.result_3_front_param.result_image_width;
	ret = (AVM_Error_Code_E)Create_Intesight_Image(&p_avm_result->result_image_junc_view_3_front, 
		RESULT_IMAGE_TYPE, junc_3_front_view_height, junc_3_front_view_width);
	CHECK_ERROR(ret);

	// malloc junc 3 rear view memory
	junc_3_rear_view_height = 
		p_avm_config_param->avm_2d_param.junc_view_3_rear_param.result_3_rear_param.result_image_height;
	junc_3_rear_view_width = 
		p_avm_config_param->avm_2d_param.junc_view_3_rear_param.result_3_rear_param.result_image_width;
	ret = (AVM_Error_Code_E)Create_Intesight_Image(&p_avm_result->result_image_junc_view_3_rear, 
		RESULT_IMAGE_TYPE, junc_3_rear_view_height, junc_3_rear_view_width);
	CHECK_ERROR(ret);

	// malloc awb view memory
	awb_view_height = p_avm_config_param->avm_2d_param.awb_view_param.result_awb_param.result_image_height;
	awb_view_width = p_avm_config_param->avm_2d_param.awb_view_param.result_awb_param.result_image_width;
	ret = (AVM_Error_Code_E)Create_Intesight_Image(&p_avm_result->result_image_awb, 
		RESULT_IMAGE_TYPE, awb_view_height, awb_view_width);
	CHECK_ERROR(ret);

	// malloc 3d view memory
	_3d_view_height = p_avm_config_param->avm_3d_param.result_image_height;
	_3d_view_width = p_avm_config_param->avm_3d_param.result_image_width;
	for (Int32_t view_id = 0; view_id < VIEW_COUNT_3D; view_id++)
	{
		ret = (AVM_Error_Code_E)Create_Intesight_Image(&p_avm_result->result_image_3d[view_id], 
			RESULT_IMAGE_TYPE, _3d_view_height, _3d_view_width);
		CHECK_ERROR(ret);
	}

	//// malloc car logo image memory
	P_Avm_Logo_Image_S p_avm_logo = &p_avm_image->logo_image;

	// malloc junc 4-pass small view car logo image memory
	junc_4_view_small_car_logo_height = 
		p_avm_config_param->avm_2d_param.junc_view_4_param.result_small_param.car_logo_rect.height;
	junc_4_view_small_car_logo_width = 
		p_avm_config_param->avm_2d_param.junc_view_4_param.result_small_param.car_logo_rect.width;
	ret = (AVM_Error_Code_E)Create_Intesight_Image(&p_avm_logo->logo_image_junc_view_4_small, 
		LOGO_IMAGE_TYPE, junc_4_view_small_car_logo_height, junc_4_view_small_car_logo_width);
	CHECK_ERROR(ret);
#endif
	// malloc junc 4-pass big view car logo image memory
	junc_4_view_big_car_logo_height = 
		p_avm_config_param->avm_2d_param.junc_view_4_param.result_big_param.car_logo_rect.height;
	junc_4_view_big_car_logo_width = 
		p_avm_config_param->avm_2d_param.junc_view_4_param.result_big_param.car_logo_rect.width;
	ret = (AVM_Error_Code_E)Create_Intesight_Image(&p_avm_logo->logo_image_junc_view_4_big, 
		LOGO_IMAGE_TYPE, junc_4_view_big_car_logo_height, junc_4_view_big_car_logo_width);
	CHECK_ERROR(ret);

	// malloc junc 3-pass front view car logo image memory
	junc_3_front_view_car_logo_height = 
		p_avm_config_param->avm_2d_param.junc_view_3_front_param.result_3_front_param.car_logo_rect.height;
	junc_3_front_view_car_logo_width = 
		p_avm_config_param->avm_2d_param.junc_view_3_front_param.result_3_front_param.car_logo_rect.width;
	ret = (AVM_Error_Code_E)Create_Intesight_Image(&p_avm_logo->logo_image_junc_view_3_front, 
		LOGO_IMAGE_TYPE, junc_3_front_view_car_logo_height, junc_3_front_view_car_logo_width);
	CHECK_ERROR(ret);

	// malloc junc 3-pass rear view car logo image memory
	junc_3_rear_view_car_logo_height = 
		p_avm_config_param->avm_2d_param.junc_view_3_rear_param.result_3_rear_param.car_logo_rect.height;
	junc_3_rear_view_car_logo_width = 
		p_avm_config_param->avm_2d_param.junc_view_3_rear_param.result_3_rear_param.car_logo_rect.width;
	ret = (AVM_Error_Code_E)Create_Intesight_Image(&p_avm_logo->logo_image_junc_view_3_rear, 
		LOGO_IMAGE_TYPE, junc_3_rear_view_car_logo_height, junc_3_rear_view_car_logo_width);
	CHECK_ERROR(ret);
	
	// malloc 3d view car logo image memory
	for (Int32_t view_id = 0; view_id < VIEW_COUNT_3D; view_id++)
	{
		_3d_car_logo_height[view_id] = p_avm_config_param->avm_3d_param.car_logo_rect[view_id].height;
		_3d_car_logo_width[view_id] = p_avm_config_param->avm_3d_param.car_logo_rect[view_id].width;
		ret = (AVM_Error_Code_E)Create_Intesight_Image(&p_avm_logo->logo_image_3d[view_id], 
			RESULT_IMAGE_TYPE, _3d_car_logo_height[view_id], _3d_car_logo_width[view_id]);
		CHECK_ERROR(ret);
	}

#endif
	*pp_avm_image = p_avm_image;

	return ret;
}

/*
 * Function Name:				AVM_Malloc_Single_View_Dis_Line_Memory
 * Function Function:			malloc memory for single view distance line structure
 * Input:
 *     p_avm_single_view_param:	the param is use to calculate the memory size need to be malloced
 *     pp_avm_image:			the address of malloced memory init address
 * Note:
 * Revision:
 *     Create by Shuo.Yan on 2017/11/29.
 */
static AVM_Error_Code_E AVM_Malloc_Single_View_Dis_Line_Memory(
	IN P_Avm_Single_View_Param_S p_avm_single_view_param, 
	OUT P_Avm_Dis_Line_S p_avm_dis_line)
{
	AVM_Error_Code_E ret = ERROR_NONE;

	Int32_t static_line_size, dynamic_line_size;
	static_line_size = sizeof(UInt32_t)* DEFAULT_STATIC_DIS_LINE_SIZE_DENSE;//DEFAULT_STATIC_DIS_LINE_SIZE_DENSE
	dynamic_line_size = sizeof(UInt32_t)* 
		DEFAULT_DYNAMIC_DIS_LINE_SIZE * MAX_DYNAMIC_LINE_COUNT;


	//// malloc front view dis line memory
	P_Dis_Line_Param_S p_front_dis_line_param =
		&p_avm_single_view_param->front_single_view_param.dis_line_param;

	// malloc front view static line memory
	P_Static_Line_Param_S p_front_static_line_param = 
		&p_front_dis_line_param->static_line_param;

	if (p_front_static_line_param->is_static_line_exist)
	{
		ret = Avm_Memory_Malloc_UInt32(
			&p_avm_dis_line->p_static_dis_line_front, static_line_size);
		CHECK_ERROR(ret);
	}
	else
	{
		p_avm_dis_line->p_static_dis_line_front = NULL;
	}

	// malloc front view dynamic line memory
	P_Dynamic_Line_Param_S p_front_dynamic_line_param = 
		&p_front_dis_line_param->dynamic_line_param;
	if (p_front_dynamic_line_param->is_dynamic_line_exist)
	{
		ret = Avm_Memory_Malloc_UInt32(
			&p_avm_dis_line->p_dynamic_dis_line_front, dynamic_line_size);
		CHECK_ERROR(ret);
	}
	else
	{
		p_avm_dis_line->p_dynamic_dis_line_front = NULL;
	}


	//// malloc rear view dis line memory
	P_Dis_Line_Param_S p_rear_dis_line_param =
		&p_avm_single_view_param->rear_single_view_param.dis_line_param;

	// malloc rear view static line memory
	P_Static_Line_Param_S p_rear_static_line_param = 
		&p_rear_dis_line_param->static_line_param;

	if (p_rear_static_line_param->is_static_line_exist)
	{
		ret = Avm_Memory_Malloc_UInt32(
			&p_avm_dis_line->p_static_dis_line_rear, static_line_size);
		CHECK_ERROR(ret);
	}
	else
	{
		p_avm_dis_line->p_static_dis_line_rear = NULL;
	}

	// malloc rear view dynamic line memory
	P_Dynamic_Line_Param_S p_rear_dynamic_line_param = 
		&p_rear_dis_line_param->dynamic_line_param;

	if (p_rear_dynamic_line_param->is_dynamic_line_exist)
	{
		ret = Avm_Memory_Malloc_UInt32(
			&p_avm_dis_line->p_dynamic_dis_line_rear, dynamic_line_size);
		CHECK_ERROR(ret);
	}
	else
	{
		p_avm_dis_line->p_dynamic_dis_line_rear = NULL;
	}


	//// malloc left view dis line memory
	P_Dis_Line_Param_S p_left_dis_line_param =
		&p_avm_single_view_param->left_single_view_param.dis_line_param;

	// malloc left view static line memory
	P_Static_Line_Param_S p_left_static_line_param = 
		&p_left_dis_line_param->static_line_param;

	if (p_left_static_line_param->is_static_line_exist)
	{
		ret = Avm_Memory_Malloc_UInt32(
			&p_avm_dis_line->p_static_dis_line_left, static_line_size);
		CHECK_ERROR(ret);
	}
	else
	{
		p_avm_dis_line->p_static_dis_line_left = NULL;
	}

	// malloc left view dynamic line memory
	P_Dynamic_Line_Param_S p_left_dynamic_line_param = 
		&p_left_dis_line_param->dynamic_line_param;

	if (p_left_dynamic_line_param->is_dynamic_line_exist)
	{
		ret = Avm_Memory_Malloc_UInt32(
			&p_avm_dis_line->p_dynamic_dis_line_left, dynamic_line_size);
		CHECK_ERROR(ret);
	}
	else
	{
		p_avm_dis_line->p_dynamic_dis_line_left = NULL;
	}


	//// malloc right view dis line memory
	P_Dis_Line_Param_S p_right_dis_line_param =
		&p_avm_single_view_param->right_single_view_param.dis_line_param;

	// malloc right view static line memory
	P_Static_Line_Param_S p_right_static_line_param = 
		&p_right_dis_line_param->static_line_param;

	if (p_right_static_line_param->is_static_line_exist)
	{
		ret = Avm_Memory_Malloc_UInt32(
			&p_avm_dis_line->p_static_dis_line_right, static_line_size);
		CHECK_ERROR(ret);
	}
	else
	{
		p_avm_dis_line->p_static_dis_line_right = NULL;
	}

	// malloc right view dynamic line memory
	P_Dynamic_Line_Param_S p_right_dynamic_line_param = 
		&p_right_dis_line_param->dynamic_line_param;

	if (p_right_dynamic_line_param->is_dynamic_line_exist)
	{
		ret = Avm_Memory_Malloc_UInt32(
			&p_avm_dis_line->p_dynamic_dis_line_right, dynamic_line_size);
		CHECK_ERROR(ret);
	}
	else
	{
		p_avm_dis_line->p_dynamic_dis_line_right = NULL;
	}


	//// malloc left right together view dis line memory
	P_Dis_Line_Param_S p_lr_dis_line_param_left =
		&p_avm_single_view_param->left_right_view_param.dis_line_param_left;
	P_Dis_Line_Param_S p_lr_dis_line_param_right =
		&p_avm_single_view_param->left_right_view_param.dis_line_param_right;

	// malloc left right together view static line memory
	P_Static_Line_Param_S p_lr_static_line_param_left = 
		&p_lr_dis_line_param_left->static_line_param;
	P_Static_Line_Param_S p_lr_static_line_param_right = 
		&p_lr_dis_line_param_right->static_line_param;

	if (p_lr_static_line_param_left->is_static_line_exist 
		|| p_lr_static_line_param_right->is_static_line_exist)
	{
		ret = Avm_Memory_Malloc_UInt32(
			&p_avm_dis_line->p_static_dis_line_left_right, static_line_size);
		CHECK_ERROR(ret);
	}
	else
	{
		p_avm_dis_line->p_static_dis_line_left_right = NULL;
	}

	// malloc left right together view dynamic line memory
	P_Dynamic_Line_Param_S p_lr_dynamic_line_param_left = 
		&p_lr_dis_line_param_left->dynamic_line_param;
	P_Dynamic_Line_Param_S p_lr_dynamic_line_param_right = 
		&p_lr_dis_line_param_right->dynamic_line_param;

	if (p_lr_dynamic_line_param_left->is_dynamic_line_exist 
		|| p_lr_dynamic_line_param_right->is_dynamic_line_exist)
	{
		ret = Avm_Memory_Malloc_UInt32(
			&p_avm_dis_line->p_dynamic_dis_line_left_right, dynamic_line_size);
		CHECK_ERROR(ret);
	}
	else
	{
		p_avm_dis_line->p_dynamic_dis_line_left_right = NULL;
	}
	return ret;
}

/*
 * Function Name:				AVM_Malloc_Junc_View_Dis_Line_Memory
 * Function Function:			malloc memory for junction view distance line structure
 * Input:
 *     p_avm_config_param:		the param is use to calculate the memory size need to be malloced
 *     pp_avm_image:			the address of malloced memory init address
 * Note:
 * Revision:
 *     Create by Shuo.Yan on 2017/11/29.
 */
static AVM_Error_Code_E AVM_Malloc_Junc_View_Dis_Line_Memory(
	IN P_Avm_2D_Param_S p_avm_2d_param, 
	OUT P_Avm_Dis_Line_S p_avm_dis_line)
{
	AVM_Error_Code_E ret = ERROR_NONE;

	Int32_t static_line_size, dynamic_line_size;
	static_line_size = sizeof(UInt32_t)* DEFAULT_STATIC_DIS_LINE_SIZE;
	dynamic_line_size = sizeof(UInt32_t)* 
		DEFAULT_DYNAMIC_DIS_LINE_SIZE * MAX_DYNAMIC_LINE_COUNT;

	//// malloc junction 4 pass small view dis line memory
	P_Dis_Line_Param_S p_junc_view_4_small_dis_line_param =
		&p_avm_2d_param->junc_view_4_param.small_view_dis_line_param;

	P_Static_Line_Param_S p_junc_view_4_small_static_dis_line_param = 
		&p_junc_view_4_small_dis_line_param->static_line_param;
	if (p_junc_view_4_small_static_dis_line_param->is_static_line_exist)
	{
		ret = Avm_Memory_Malloc_UInt32(
			&p_avm_dis_line->p_static_dis_line_4_small, static_line_size);
		CHECK_ERROR(ret);
	}
	else
	{
		p_avm_dis_line->p_static_dis_line_4_small = NULL;
	}

	P_Dynamic_Line_Param_S p_junc_view_4_small_dynamic_dis_line_param = 
		&p_junc_view_4_small_dis_line_param->dynamic_line_param;
	if (p_junc_view_4_small_dynamic_dis_line_param->is_dynamic_line_exist)
	{
		ret = Avm_Memory_Malloc_UInt32(
			&p_avm_dis_line->p_dynamic_dis_line_4_small, dynamic_line_size);
		CHECK_ERROR(ret);
	}
	else
	{
		p_avm_dis_line->p_dynamic_dis_line_4_small = NULL;
	}

	//// malloc junction 4 pass big view dis line memory
	P_Dis_Line_Param_S p_junc_view_4_big_dis_line_param =
		&p_avm_2d_param->junc_view_4_param.big_view_dis_line_param;

	P_Static_Line_Param_S p_junc_view_4_big_static_dis_line_param = 
		&p_junc_view_4_big_dis_line_param->static_line_param;
	if (p_junc_view_4_big_static_dis_line_param->is_static_line_exist)
	{
		ret = Avm_Memory_Malloc_UInt32(
			&p_avm_dis_line->p_static_dis_line_4_big, static_line_size);
		CHECK_ERROR(ret);
	}
	else
	{
		p_avm_dis_line->p_static_dis_line_4_big = NULL;
	}
	P_Dynamic_Line_Param_S p_junc_view_4_big_dynamic_dis_line_param = 
		&p_junc_view_4_big_dis_line_param->dynamic_line_param;
	if (p_junc_view_4_big_dynamic_dis_line_param->is_dynamic_line_exist)
	{
		ret = Avm_Memory_Malloc_UInt32(
			&p_avm_dis_line->p_dynamic_dis_line_4_big, dynamic_line_size);
		CHECK_ERROR(ret);
	}
	else
	{
		p_avm_dis_line->p_dynamic_dis_line_4_big = NULL;
	}

	//// malloc junction 3 pass front view dis line memory
	P_Dis_Line_Param_S p_junc_view_3_front_dis_line_param =
		&p_avm_2d_param->junc_view_3_front_param.junc_3_front_dis_line_param;

	P_Static_Line_Param_S p_junc_view_3_front_static_dis_line_param = 
		&p_junc_view_3_front_dis_line_param->static_line_param;
	if (p_junc_view_3_front_static_dis_line_param->is_static_line_exist)
	{
		ret = Avm_Memory_Malloc_UInt32(
			&p_avm_dis_line->p_static_dis_line_3_front, static_line_size);
		CHECK_ERROR(ret);
	}
	else
	{
		p_avm_dis_line->p_static_dis_line_3_front = NULL;
	}

	P_Dynamic_Line_Param_S p_junc_view_3_front_dynamic_dis_line_param = 
		&p_junc_view_3_front_dis_line_param->dynamic_line_param;
	if (p_junc_view_3_front_dynamic_dis_line_param->is_dynamic_line_exist)
	{
		ret = Avm_Memory_Malloc_UInt32(
			&p_avm_dis_line->p_dynamic_dis_line_3_front, dynamic_line_size);
		CHECK_ERROR(ret);
	}
	else
	{
		p_avm_dis_line->p_dynamic_dis_line_3_front = NULL;
	}

	//// malloc junction 3 pass rear view dis line memory
	P_Dis_Line_Param_S p_junc_view_3_rear_dis_line_param =
		&p_avm_2d_param->junc_view_3_rear_param.junc_3_rear_dis_line_param;

	P_Static_Line_Param_S p_junc_view_3_rear_static_dis_line_param = 
		&p_junc_view_3_rear_dis_line_param->static_line_param;
	if (p_junc_view_3_rear_static_dis_line_param->is_static_line_exist)
	{
		ret = Avm_Memory_Malloc_UInt32(
			&p_avm_dis_line->p_static_dis_line_3_rear, static_line_size);
		CHECK_ERROR(ret);
	}
	else
	{
		p_avm_dis_line->p_static_dis_line_3_rear = NULL;
	}

	P_Dynamic_Line_Param_S p_junc_view_3_rear_dynamic_dis_line_param = 
		&p_junc_view_3_rear_dis_line_param->dynamic_line_param;
	if (p_junc_view_3_rear_dynamic_dis_line_param->is_dynamic_line_exist)
	{
		ret = Avm_Memory_Malloc_UInt32(
			&p_avm_dis_line->p_dynamic_dis_line_3_rear, dynamic_line_size);
		CHECK_ERROR(ret);
	}
	else
	{
		p_avm_dis_line->p_dynamic_dis_line_3_rear = NULL;
	}

	return ret;
}

/*
 * Function Name:				AVM_Malloc_Dis_Line_Memory
 * Function Function:			malloc memory for distance line structure
 * Input:
 *     p_avm_config_param:		the param is use to calculate the memory size need to be malloced
 *     pp_avm_image:			the address of malloced memory init address
 * Note:
 * Revision:
 *     Create by Shuo.Yan on 2017/11/29.
 */
static AVM_Error_Code_E AVM_Malloc_Dis_Line_Memory(
	IN P_Avm_Config_Param_S p_avm_config_param, 
	OUT P_Avm_Dis_Line_S* pp_avm_dis_line)
{
	AVM_Error_Code_E ret = ERROR_NONE;
	
	P_Avm_Dis_Line_S p_avm_dis_line = NULL;
	
	p_avm_dis_line = (P_Avm_Dis_Line_S)malloc(sizeof(Avm_Dis_Line_S));
	if (!p_avm_dis_line)
	{
		ret = ERROR_MEMORY_MALLOC_FAIL;
		CHECK_ERROR(ret);
	}
	memset(p_avm_dis_line, 0, sizeof(Avm_Dis_Line_S));
		
	// malloc single view dis line memory
	P_Avm_Single_View_Param_S p_avm_single_view_param = 
		&p_avm_config_param->avm_single_view_param;	

	ret = AVM_Malloc_Single_View_Dis_Line_Memory(
		p_avm_single_view_param, p_avm_dis_line);
	CHECK_ERROR(ret);
	
	// malloc junction view dis line memory
	P_Avm_2D_Param_S p_avm_2d_param = &p_avm_config_param->avm_2d_param;
	ret = AVM_Malloc_Junc_View_Dis_Line_Memory(p_avm_2d_param, p_avm_dis_line);
	CHECK_ERROR(ret);

	*pp_avm_dis_line = p_avm_dis_line;

	return ret;
}

/*
 * Function Name:				AVM_Release_Lut_Memory
 * Function Function:			free lut memory
 * Input:
 *     p_avm_lut:				the address of P_Avm_Lut_S structure
 * Note:
 * Revision:
 *     Create by Shuo.Yan on 2017/11/25.
 */
void AVM_Release_Lut_Memory(OUT IN P_Avm_Lut_S p_avm_lut)
{
	FREE_POINTER(p_avm_lut->p_lut_single_view_front);
	FREE_POINTER(p_avm_lut->p_lut_single_view_rear);
	FREE_POINTER(p_avm_lut->p_lut_single_view_left);
	FREE_POINTER(p_avm_lut->p_lut_single_view_right);
	FREE_POINTER(p_avm_lut->p_lut_single_view_left_right);

	FREE_POINTER(p_avm_lut->p_lut_junc_view_4_small);
	FREE_POINTER(p_avm_lut->p_lut_junc_view_4_big);
	FREE_POINTER(p_avm_lut->p_lut_junc_view_3_front);
	FREE_POINTER(p_avm_lut->p_lut_junc_view_3_rear);

	FREE_POINTER(p_avm_lut->p_lut_awb);

	FREE_POINTER(p_avm_lut);
}

/*
 * Function Name:				AVM_Release_Image_Memory
 * Function Function:			free image memory
 * Input:
 *     p_avm_image:				the address of P_Avm_Image_S structure
 * Note:
 * Revision:
 *     Create by Shuo.Yan on 2017/11/25.
 */
static void AVM_Release_Image_Memory(OUT IN P_Avm_Image_S p_avm_image)
{
	//// release source image memory
	for (Int32_t cam_id = 0; cam_id < CAMERA_NUM; cam_id++)
	{
		Release_Intesight_Image(p_avm_image->source_image_array[cam_id]);
	}

	//// release result image memory
	// release single image memory
	Release_Intesight_Image(p_avm_image->result_image.result_image_single_view_front);
	Release_Intesight_Image(p_avm_image->result_image.result_image_single_view_rear);
	Release_Intesight_Image(p_avm_image->result_image.result_image_single_view_left);
	Release_Intesight_Image(p_avm_image->result_image.result_image_single_view_right);
	Release_Intesight_Image(p_avm_image->result_image.result_image_single_view_left_right);

	// release junc view image memory
	Release_Intesight_Image(p_avm_image->result_image.result_image_junc_view_4_small);
	Release_Intesight_Image(p_avm_image->result_image.result_image_junc_view_4_big);
	Release_Intesight_Image(p_avm_image->result_image.result_image_junc_view_3_front);
	Release_Intesight_Image(p_avm_image->result_image.result_image_junc_view_3_rear);

	// release awb image memory
	Release_Intesight_Image(p_avm_image->result_image.result_image_awb);

	// release 3d image result memory
	for (Int32_t view_id = 0; view_id < VIEW_COUNT_3D; view_id++)
	{
		Release_Intesight_Image(p_avm_image->result_image.result_image_3d[view_id]);
	}

	//// Release car logo image memory
	Release_Intesight_Image(p_avm_image->logo_image.logo_image_junc_view_4_small);
	Release_Intesight_Image(p_avm_image->logo_image.logo_image_junc_view_4_big);
	Release_Intesight_Image(p_avm_image->logo_image.logo_image_junc_view_3_front);
	Release_Intesight_Image(p_avm_image->logo_image.logo_image_junc_view_3_rear);

	for (Int32_t view_id = 0; view_id < VIEW_COUNT_3D; view_id++)
	{
		Release_Intesight_Image(p_avm_image->logo_image.logo_image_3d[view_id]);
	}
}

/*
* Function Name:				AVM_Release_Dis_Line_Memory
* Function Function:			free distance line memory
* Input:
*     p_avm_dis_line:			the address of P_Avm_Dis_Line_S structure
* Note:
* Revision:
*     Create by Shuo.Yan on 2017/11/25.
*/
static void AVM_Release_Dis_Line_Memory(OUT IN P_Avm_Dis_Line_S p_avm_dis_line)
{
	// free single view distance distance line memory
	FREE_POINTER(p_avm_dis_line->p_static_dis_line_front);
	FREE_POINTER(p_avm_dis_line->p_dynamic_dis_line_front);

	FREE_POINTER(p_avm_dis_line->p_static_dis_line_rear);
	FREE_POINTER(p_avm_dis_line->p_dynamic_dis_line_rear);

	FREE_POINTER(p_avm_dis_line->p_static_dis_line_left);
	FREE_POINTER(p_avm_dis_line->p_dynamic_dis_line_left);

	FREE_POINTER(p_avm_dis_line->p_static_dis_line_right);
	FREE_POINTER(p_avm_dis_line->p_dynamic_dis_line_right);

	FREE_POINTER(p_avm_dis_line->p_static_dis_line_left_right);
	FREE_POINTER(p_avm_dis_line->p_dynamic_dis_line_left_right);

	// free junction view distance distance line memory
	FREE_POINTER(p_avm_dis_line->p_static_dis_line_4_small);
	FREE_POINTER(p_avm_dis_line->p_dynamic_dis_line_4_small);

	FREE_POINTER(p_avm_dis_line->p_static_dis_line_4_big);
	FREE_POINTER(p_avm_dis_line->p_dynamic_dis_line_4_big);

	FREE_POINTER(p_avm_dis_line->p_static_dis_line_3_front);
	FREE_POINTER(p_avm_dis_line->p_dynamic_dis_line_3_front);

	FREE_POINTER(p_avm_dis_line->p_static_dis_line_3_rear);
	FREE_POINTER(p_avm_dis_line->p_dynamic_dis_line_3_rear);

	// free P_Avm_Dis_Line_S structure memory
	FREE_POINTER(p_avm_dis_line);
}

/*
 * Function Name:				AVM_Init
 * Function Function:			load config param for AVM module
 * Input:
 *     p_avm_handle:			the structure is hided outside of this module
 * Note:
 *     the interface function should be called at the beginning of this module
 * Revision:
 *     Create by Shuo.Yan on 2017/11/23.
 */
INTESIGHT_API AVM_Error_Code_E AVM_Init(OUT Handle* p_avm_handle)
{
	AVM_Error_Code_E ret = ERROR_NONE;

	P_Avm_Param_S p_avm_param = NULL;
	
	ret = AVM_Malloc_Avm_Param_Memory(&p_avm_param);
	CHECK_ERROR(ret);

	// malloc config param memory
	ret = AVM_Malloc_Config_Param_Memory(&(p_avm_param->p_avm_config_param));
	CHECK_ERROR(ret);
	// load config param to get lut and image memory size
	ret = AVM_Load_Config_Param(p_avm_param,p_avm_param->p_avm_config_param);
	CHECK_ERROR(ret);
	ret = AVM_Malloc_Lut_Memory(
		p_avm_param->p_avm_config_param, &p_avm_param->p_avm_lut);
	CHECK_ERROR(ret);
	p_avm_param->p_avm_dis_line = (P_Avm_Dis_Line_S)malloc(sizeof(Avm_Dis_Line_S));

	ret = AVM_Malloc_Dis_Line_Memory(p_avm_param->p_avm_config_param,
		&p_avm_param->p_avm_dis_line);
	CHECK_ERROR(ret);

	ret = AVM_Malloc_Image_Memory(
		p_avm_param->p_avm_config_param, &p_avm_param->p_avm_image);
	(*p_avm_handle) = (void*)p_avm_param;
	return ret;
}

AVM_Error_Code_E my_test(IN  P_Avm_Param_S p_avm_param)
{
	AVM_Error_Code_E error = ERROR_NONE;

#if 1
	error = handle_3_4_view(p_avm_param);
	CHECK_ERROR(error);
	error = handle_front_rear_single_view(p_avm_param);
	CHECK_ERROR(error);
	error = handle_left_right_single_view(p_avm_param);
	CHECK_ERROR(error);

	///*从表中解析数据存储结果上位机验证*/
	error = resolve_4_view_data_from_LUT(p_avm_param, _4_VIEW_BIG);
	CHECK_ERROR(error);
	error = resolve_4_view_data_from_LUT(p_avm_param, _4_VIEW_SMALL);
	CHECK_ERROR(error);
	error = resolve_3_front_data_from_LUT(p_avm_param,_3_VIEW_FRONT);
	CHECK_ERROR(error);
	error = resolve_3_front_data_from_LUT(p_avm_param, _3_VIEW_REAR);
	CHECK_ERROR(error);

	error = reslove_single_view_from_lut(p_avm_param, _1_VIEW_FRONT);
	CHECK_ERROR(error);
	error = reslove_single_view_from_lut(p_avm_param, _1_VIEW_REAR);
	CHECK_ERROR(error);

	error = reslove_single_view_from_lut(p_avm_param, _1_VIEW_LEFT);
	CHECK_ERROR(error);
	error = reslove_single_view_from_lut(p_avm_param, _1_VIEW_RIGHT);
	CHECK_ERROR(error);
	error = reslove_single_view_from_lut(p_avm_param, _1_VIEW_LEFT_RIGHT);
	CHECK_ERROR(error);

	/////********************画线*********************/
	error = handle_static_dynamic_line(p_avm_param);//
	CHECK_ERROR(error);
	
	////此处为了支持Tiguan项目临时加了大屏幕的配置，为了程序不在重复配置，将小屏与全屏的配置分开，全屏配置是一个独立的TXT文件
	AVM_Deinit_release(p_avm_param);//因为大屏小屏只有查找表和结果图大小不同释放内存
	////此处为了支持Tiguan项目临时加了大屏幕的配置，为了程序不在重复配置，将小屏与全屏的配置分开，全屏配置是一个独立的TXT文件
	load_big_screen_para(p_avm_param);
	CHECK_ERROR(error);

	/*计算生成裕兰的表*/
	error = handle_3_4_view(p_avm_param);
	CHECK_ERROR(error);
	error = handle_front_rear_single_view(p_avm_param);
	CHECK_ERROR(error);
	error = handle_left_right_single_view(p_avm_param);
	CHECK_ERROR(error);


	/*从表中解析数据存储结果上位机验证*/
	error = reslove_single_view_from_lut(p_avm_param, _1_VIEW_FRONT);
	CHECK_ERROR(error);
	error = reslove_single_view_from_lut(p_avm_param, _1_VIEW_REAR);
	CHECK_ERROR(error);
	error = reslove_single_view_from_lut(p_avm_param, _1_VIEW_FRONT);
	CHECK_ERROR(error);
	error = reslove_single_view_from_lut(p_avm_param, _1_VIEW_REAR);
	CHECK_ERROR(error);
	error = reslove_single_view_from_lut(p_avm_param, _1_VIEW_LEFT);
	CHECK_ERROR(error);
	error = reslove_single_view_from_lut(p_avm_param, _1_VIEW_RIGHT);
	CHECK_ERROR(error);
	error = reslove_single_view_from_lut(p_avm_param, _1_VIEW_LEFT_RIGHT);
	CHECK_ERROR(error);

	/******验证保存距离线小屏***/
	///****************************************************/
	error = handle_static_dynamic_line(p_avm_param);//小屏的画线函数
	CHECK_ERROR(error);
	//error = reslove_line_lut(p_avm_param, _1_VIEW_FRONT);
	CHECK_ERROR(error);
	//error = reslove_line_lut(p_avm_param, _1_VIEW_REAR);
	CHECK_ERROR(error);



	///*****************写二进制文件*********************/
	error = write_3_4_view_lut(p_avm_param);
	CHECK_ERROR(error);
	error = write_single_view_lut(p_avm_param, _1_VIEW_FRONT);
	CHECK_ERROR(error);
	error = write_single_view_lut(p_avm_param, _1_VIEW_REAR);
	CHECK_ERROR(error);
	error = write_single_view_lut(p_avm_param, _1_VIEW_LEFT);
	CHECK_ERROR(error);
	error = write_single_view_lut(p_avm_param, _1_VIEW_RIGHT);
	CHECK_ERROR(error);
	error = write_single_view_lut(p_avm_param, _1_VIEW_LEFT_RIGHT);
	CHECK_ERROR(error);
	error = write_line_lut(p_avm_param, _1_VIEW_FRONT);
	CHECK_ERROR(error);
	error = write_line_lut(p_avm_param, _1_VIEW_REAR);
	CHECK_ERROR(error);

#endif

	/***********读BIN文件 ************/
	error = read_bin(p_avm_param, _4_VIEW_BIG);
	CHECK_ERROR(error);
	error = read_bin(p_avm_param, _4_VIEW_SMALL);
	CHECK_ERROR(error);
	error = read_bin(p_avm_param, _3_VIEW_FRONT);
	CHECK_ERROR(error);
	error = read_bin(p_avm_param, _3_VIEW_REAR);
	CHECK_ERROR(error);
	error = read_bin(p_avm_param, _1_VIEW_FRONT);
	CHECK_ERROR(error);
	error = read_bin(p_avm_param, _1_VIEW_REAR);
	CHECK_ERROR(error);
	error = read_bin(p_avm_param, _1_VIEW_LEFT);
	CHECK_ERROR(error);
	error = read_bin(p_avm_param, _1_VIEW_RIGHT);
	CHECK_ERROR(error);
	error = read_bin(p_avm_param, _1_VIEW_LEFT_RIGHT);
	CHECK_ERROR(error);
	error = read_line_bin(p_avm_param, _1_VIEW_FRONT);
	CHECK_ERROR(error);
	error = read_line_bin(p_avm_param, _1_VIEW_REAR);
	CHECK_ERROR(error);

	///*从表中解析数据存储结果上位机验证*/
	error = resolve_4_view_data_from_LUT(p_avm_param, _4_VIEW_BIG);
	CHECK_ERROR(error);
	error = resolve_4_view_data_from_LUT(p_avm_param, _4_VIEW_SMALL);
	CHECK_ERROR(error);
	error = resolve_3_front_data_from_LUT(p_avm_param, _3_VIEW_FRONT);
	CHECK_ERROR(error);
	error = resolve_3_front_data_from_LUT(p_avm_param, _3_VIEW_REAR);
	CHECK_ERROR(error);

	error = reslove_single_view_from_lut(p_avm_param, _1_VIEW_FRONT);
	CHECK_ERROR(error);
	error = reslove_single_view_from_lut(p_avm_param, _1_VIEW_REAR);
	CHECK_ERROR(error);

	error = reslove_single_view_from_lut(p_avm_param, _1_VIEW_LEFT);
	CHECK_ERROR(error);
	error = reslove_single_view_from_lut(p_avm_param, _1_VIEW_RIGHT);
	CHECK_ERROR(error);
	error = reslove_single_view_from_lut(p_avm_param, _1_VIEW_LEFT_RIGHT);
	CHECK_ERROR(error);

	/******验证保存距离线小屏***/
	error = reslove_line_lut(p_avm_param, _1_VIEW_FRONT);
	CHECK_ERROR(error);
	error = reslove_line_lut(p_avm_param, _1_VIEW_REAR);
	CHECK_ERROR(error);

	/*******************3D项目*************************/
	generate_3D(p_avm_param);
	return error;
}


AVM_Error_Code_E S32V(IN  P_Avm_Param_S p_avm_param)
{
	AVM_Error_Code_E error = ERROR_NONE;

	error = handle_3_4_view(p_avm_param);
	CHECK_ERROR(error);
	error = handle_front_rear_single_view(p_avm_param);
	CHECK_ERROR(error);
	error = handle_left_right_single_view(p_avm_param);
	CHECK_ERROR(error);
	printf("内部表生成成功\n");
	///*从表中解析数据存储结果上位机验证*/
	error = resolve_4_view_data_from_LUT(p_avm_param, _4_VIEW_BIG);
	CHECK_ERROR(error);
	error = reslove_single_view_from_lut(p_avm_param, _1_VIEW_FRONT);
	CHECK_ERROR(error);
	error = reslove_single_view_from_lut(p_avm_param, _1_VIEW_REAR);
	CHECK_ERROR(error);
	error = reslove_single_view_from_lut(p_avm_param, _1_VIEW_LEFT);
	CHECK_ERROR(error);
	error = reslove_single_view_from_lut(p_avm_param, _1_VIEW_RIGHT);
	CHECK_ERROR(error);
	printf("上位机解析前后单视图\n");
	/////********************画线*********************/
	error = handle_static_dynamic_line(p_avm_param);//
	CHECK_ERROR(error);
	printf("动态静态线生成功\n");
	///*****************写二进制文件*********************/
	write_gpu_LUT(p_avm_param, _4_VIEW_BIG);//生成四路环视
	error = write_single_view_lut(p_avm_param, _1_VIEW_FRONT);
	CHECK_ERROR(error);
	error = write_single_view_lut(p_avm_param, _1_VIEW_REAR);
	CHECK_ERROR(error);
	error = write_single_view_lut(p_avm_param, _1_VIEW_LEFT);
	CHECK_ERROR(error);
	error = write_single_view_lut(p_avm_param, _1_VIEW_RIGHT);
	CHECK_ERROR(error);
	error = write_line_lut(p_avm_param, _1_VIEW_FRONT);
	CHECK_ERROR(error);
	error = write_line_lut(p_avm_param, _1_VIEW_REAR);
	CHECK_ERROR(error);
	printf("写查找表成功\n");


	return error;
}



/*
 * Function Name:				AVM_Process
 * Function Function:			the core process function of avm module
 * Input:
 *     avm_handle:				the structure used in the module
 * Note:
 *     the core interface function finish lut generation function
 * Revision:
 *     Create by Shuo.Yan on 2017/11/23.
 */
INTESIGHT_API AVM_Error_Code_E AVM_Process(OUT IN  Handle avm_handle)
{
	AVM_Error_Code_E error = ERROR_NONE;
	P_Avm_Param_S p_avm_param = NULL;
	p_avm_param = (P_Avm_Param_S)avm_handle;

	/////**********生成T2的表************/
	//p_avm_param->project_flag = TEST;//支持项目的标志位
	error = my_test(p_avm_param);//自己测试的代码
	//CHECK_ERROR(error);
	///************************安霸项目**********************************/
	/////********CVBS*******/
	p_avm_param->project_flag = CVBS;//支持项目的标志位
	error = Tiguan_CVBS_out_amba_table(p_avm_param);//如果安霸转表则调用这个函数
	CHECK_ERROR(error);
	/////********HDMI******/
	//p_avm_param->project_flag = HDMI;//设置CVBS与HDMI的标志位
	//error = Tiguan_Amba_HDMI_Table(p_avm_param);
	//CHECK_ERROR(error);
	
	
	return error;
}
 
/*
 * Function Name:				AVM_Deinit
 * Function Function:			load config param for AVM module
 * Input:tf7
 *     avm_handle:				the memory need to be freedes
 * Note:
 *     the interface function should be called after the module finish its function
 * Revision:
 *     Create by Shuo.Yan on 2017/11/23.
 */
INTESIGHT_API void AVM_Deinit(IN Handle avm_handle)
{
	P_Avm_Param_S p_avm_param = (P_Avm_Param_S)(avm_handle);

	FREE_POINTER(p_avm_param->p_avm_config_param);

	AVM_Release_Lut_Memory(p_avm_param->p_avm_lut);//释放查找表内存
	AVM_Release_Dis_Line_Memory(p_avm_param->p_avm_dis_line);//释放静态线动态线查找表内存
	AVM_Release_Image_Memory(p_avm_param->p_avm_image);//释放图片内存

	FREE_POINTER(avm_handle);
}

/*
* Function Name:				AVM_Deinit
* Function Function:			load config param for AVM module
* Input:tf7
*     avm_handle:				the memory need to be freedes
* Note:
*     the interface function should be called after the module finish its function
* Revision:
*     Create by Shuo.Yan on 2017/11/23.
*/
INTESIGHT_API void AVM_Deinit_release(IN Handle avm_handle)
{
	P_Avm_Param_S p_avm_param = (P_Avm_Param_S)(avm_handle);


	AVM_Release_Lut_Memory(p_avm_param->p_avm_lut);//释放查找表内存
	//AVM_Release_Dis_Line_Memory(p_avm_param->p_avm_dis_line);//释放静态线动态线查找表内存
	AVM_Release_Image_Memory(p_avm_param->p_avm_image);//释放图片内存


}