/*
* This file realize all config file's load function besides of some files in COMM.
* Created by Shuo.Yan on 2017/11/23
*/

#include "avm_load_parameter.h"

/*
* Function Name:				Load_Single_View_Crop_Param
* Function Function:			load single view crop param
* Input:
*     p_single_view_crop_param: the structure save the loaded param
*     p_file:					the file pointer
* Note:
* Revision:
*     Create by Shuo.Yan on 2017/11/30.
*/
AVM_Error_Code_E Load_Single_View_Crop_Param(
	OUT P_Single_View_Crop_Param_S p_single_view_crop_param, IN FILE* p_file)
{
	AVM_Error_Code_E ret = ERROR_NONE;

	char buf[MAX_ARRAY_SIZE];

	fgets(buf, MAX_ARRAY_SIZE, p_file);
	fscanf(p_file, "crop_rect: %d %d %d %d",
		&p_single_view_crop_param->crop_rect.x,
		&p_single_view_crop_param->crop_rect.y,
		&p_single_view_crop_param->crop_rect.width,
		&p_single_view_crop_param->crop_rect.height);
	fgets(buf, MAX_ARRAY_SIZE, p_file);
	fscanf(p_file, "rect_roll_angle: %lf",
		&p_single_view_crop_param->roll_angle);
	fgets(buf, MAX_ARRAY_SIZE, p_file);
	fscanf(p_file, "\n", buf);

	return ret;
}

/*
* Function Name:				Load_Single_View_Front_Rear_Plane_Expand_Param
* Function Function:			Load_Single_View_Front_Rear_Plane_Expand_Param
* Input:
*  p_single_view_fr_plane_param:the structure save the loaded param
*  p_file:						the file pointer
* Note:
* Revision:
*     Create by Shuo.Yan on 2017/11/30.
*/
AVM_Error_Code_E Load_Single_View_Front_Rear_Plane_Expand_Param(
	OUT P_Single_View_Front_Rear_Plane_Expand_Param_S p_single_view_fr_plane_param,
	IN FILE* p_file)
{
	AVM_Error_Code_E ret = ERROR_NONE;

	char buf[MAX_ARRAY_SIZE];

	fgets(buf, MAX_ARRAY_SIZE, p_file);
	fscanf(p_file, "plane_expand_trans_x: %d",
		&p_single_view_fr_plane_param->trans_x);
	fgets(buf, MAX_ARRAY_SIZE, p_file);

	fscanf(p_file, "plane_expand_trans_y: %d",
		&p_single_view_fr_plane_param->trans_y);
	fgets(buf, MAX_ARRAY_SIZE, p_file);

	fscanf(p_file, "plane_expand_fov_h: %lf",
		&p_single_view_fr_plane_param->fov_h);
	fgets(buf, MAX_ARRAY_SIZE, p_file);

	fscanf(p_file, "plane_expand_fov_v: %lf",
		&p_single_view_fr_plane_param->fov_v);
	fgets(buf, MAX_ARRAY_SIZE, p_file);

	fscanf(p_file, "\n", buf);

	return ret;
}

/*
* Function Name:				Load_Single_View_Front_Rear_Plane_Expand_Param
* Function Function:			Load_Single_View_Front_Rear_Plane_Expand_Param
* Input:
*   p_single_view_fr_box_param:	the structure save the loaded param
*   p_file:						the file pointer
* Note:
* Revision:
*     Create by Shuo.Yan on 2017/11/30.
*/
AVM_Error_Code_E Load_Single_View_Front_Rear_Box_Expand_Param(
	OUT P_Single_View_Front_Rear_Box_Expand_Param_S p_single_view_fr_box_param,
	IN FILE* p_file)
{
	AVM_Error_Code_E ret = ERROR_NONE;

	char buf[MAX_ARRAY_SIZE];

	//fscanf(p_file, "%s\n", buf);
	fgets(buf, MAX_ARRAY_SIZE, p_file);
	fscanf(p_file, "box_center_width: %d",
		&p_single_view_fr_box_param->center_width);
	fgets(buf, MAX_ARRAY_SIZE, p_file);

	fscanf(p_file, "box_expand_trans_y: %d",
		&p_single_view_fr_box_param->trans_y);
	fgets(buf, MAX_ARRAY_SIZE, p_file);


	fscanf(p_file, "box_expand_fov_center_h: %lf",
		&p_single_view_fr_box_param->fov_h_center);
	fgets(buf, MAX_ARRAY_SIZE, p_file);


	fscanf(p_file, "box_expand_fov_side_h: %lf",
		&p_single_view_fr_box_param->fov_h_side);
	fgets(buf, MAX_ARRAY_SIZE, p_file);


	fscanf(p_file, "box_expand_fov_v: %lf",
		&p_single_view_fr_box_param->fov_v);
	fgets(buf, MAX_ARRAY_SIZE, p_file);

	fscanf(p_file, "\n", buf);

	return ret;
}

/*
* Function Name:				Load_Single_View_Left_Right_Plane_Expand_Param
* Function Function:			Load_Single_View_Left_Right_Plane_Expand_Param
* Input:
*  p_single_view_lr_plane_param:the structure save the loaded param
*  p_file:						the file pointer
* Note:
* Revision:
*     Create by Shuo.Yan on 2017/11/30.
*/
AVM_Error_Code_E Load_Single_View_Left_Right_Plane_Expand_Param(
	OUT P_Single_View_Left_Right_Plane_Expand_Param_S p_single_view_lr_plane_param,
	IN FILE* p_file)
{
	AVM_Error_Code_E ret = ERROR_NONE;

	char buf[MAX_ARRAY_SIZE];

	//fscanf(p_file, "%s\n", buf);
	fgets(buf, MAX_ARRAY_SIZE, p_file);
	fscanf(p_file, "trans_y: %lf",
		&p_single_view_lr_plane_param->trans_y);
	fgets(buf, MAX_ARRAY_SIZE, p_file);

	fscanf(p_file, "world_width: %lf",
		&p_single_view_lr_plane_param->world_width);
	fgets(buf, MAX_ARRAY_SIZE, p_file);

	fscanf(p_file, "world_height: %lf",
		&p_single_view_lr_plane_param->world_height);
	fgets(buf, MAX_ARRAY_SIZE, p_file);


	fscanf(p_file, "car_body_width: %d",
		&p_single_view_lr_plane_param->car_body_width);
	fgets(buf, MAX_ARRAY_SIZE, p_file);


	fscanf(p_file, "\n", buf);

	return ret;
}


/*
* Function Name:				Load_Single_View_Left_Right_Combine_Expand_Param
* Function Function:			Load_Single_View_Left_Right_Combine_Expand_Param
* Input:
*  p_single_view_lr_plane_param:the structure save the loaded param
*  p_file:						the file pointer
* Note:
* Revision:
*     Create by Shuo.Yan on 2017/11/30.
*/
AVM_Error_Code_E Load_Single_View_Left_Right_Combine_Expand_Param(
	OUT P_Single_View_Left_Right_Combine_Expand_Param_S p_single_view_lr_combine_param,
	IN FILE* p_file)
{
	AVM_Error_Code_E ret = ERROR_NONE;

	char buf[MAX_ARRAY_SIZE];

	//fscanf(p_file, "%s\n", buf);

	fgets(buf, MAX_ARRAY_SIZE, p_file);
	fscanf(p_file, "trans_y: %lf",
		&p_single_view_lr_combine_param->trans_y);
	fgets(buf, MAX_ARRAY_SIZE, p_file);

	fscanf(p_file, "car_body_width: %d",
		&p_single_view_lr_combine_param->car_body_width);
	fgets(buf, MAX_ARRAY_SIZE, p_file);

	fscanf(p_file, "world_height: %lf",
		&p_single_view_lr_combine_param->world_height);
	fgets(buf, MAX_ARRAY_SIZE, p_file);

	fscanf(p_file, "plane_width: %lf",
		&p_single_view_lr_combine_param->plane_width);
	fgets(buf, MAX_ARRAY_SIZE, p_file);

	fscanf(p_file, "plane_ratio: %lf",
		&p_single_view_lr_combine_param->plane_ratio);
	fgets(buf, MAX_ARRAY_SIZE, p_file);

	fscanf(p_file, "valid_angle_range: %lf",
		&p_single_view_lr_combine_param->valid_angle_range);
	fgets(buf, MAX_ARRAY_SIZE, p_file);

	fscanf(p_file, "\n", buf);

	return ret;
}


/*
* Function Name:				Load_Junc_View_4_Common_Param
* Function Function:			load junc 4 pass view common config param
* Input:
*   p_junc_view_4_config_common:the structure save the loaded param
*   p_file:						the file pointer
* Note:
* Revision:
*     Create by Shuo.Yan on 2017/11/30.
*/
AVM_Error_Code_E Load_Junc_View_4_Common_Param(
	OUT P_Junc_View_4_Config_Common_S p_junc_view_4_config_common, IN FILE* p_file)
{
	AVM_Error_Code_E ret = ERROR_NONE;

	char buf[MAX_ARRAY_SIZE];

	fgets(buf, MAX_ARRAY_SIZE,p_file);
	fscanf(p_file, "front_view_length: %lf",
		&p_junc_view_4_config_common->front_view_length);
	fgets(buf, MAX_ARRAY_SIZE, p_file);

	fscanf(p_file, "rear_view_length: %lf",
		&p_junc_view_4_config_common->rear_view_length);
	fgets(buf, MAX_ARRAY_SIZE, p_file);

	fscanf(p_file, "left_view_length: %lf",
		&p_junc_view_4_config_common->left_view_length);
	fgets(buf, MAX_ARRAY_SIZE, p_file);


	fscanf(p_file, "right_view_length: %lf",
		&p_junc_view_4_config_common->right_view_length);
	fgets(buf, MAX_ARRAY_SIZE, p_file);

	fscanf(p_file, "front_blind_length: %lf",
		&p_junc_view_4_config_common->front_blind_length);
	fgets(buf, MAX_ARRAY_SIZE, p_file);

	fscanf(p_file, "rear_blind_length: %lf",
		&p_junc_view_4_config_common->rear_blind_length);
	fgets(buf, MAX_ARRAY_SIZE, p_file);

	fscanf(p_file, "fl_fusion_angle: %lf",
		&p_junc_view_4_config_common->fl_fusion_angle);
	fgets(buf, MAX_ARRAY_SIZE, p_file);

	fscanf(p_file, "fr_fusion_angle: %lf",
		&p_junc_view_4_config_common->fr_fusion_angle);
	fgets(buf, MAX_ARRAY_SIZE, p_file);

	fscanf(p_file, "rl_fusion_angle: %lf",
		&p_junc_view_4_config_common->rl_fusion_angle);
	fgets(buf, MAX_ARRAY_SIZE, p_file);

	fscanf(p_file, "rr_fusion_angle: %lf",
		&p_junc_view_4_config_common->rr_fusion_angle);
	fgets(buf, MAX_ARRAY_SIZE, p_file);

	fscanf(p_file, "fl_fusion_range: %lf",
		&p_junc_view_4_config_common->fl_fusion_range);
	fgets(buf, MAX_ARRAY_SIZE, p_file);


	fscanf(p_file, "fr_fusion_range: %lf",
		&p_junc_view_4_config_common->fr_fusion_range);
	fgets(buf, MAX_ARRAY_SIZE, p_file);

	fscanf(p_file, "rl_fusion_range: %lf",
		&p_junc_view_4_config_common->rl_fusion_range);
	fgets(buf, MAX_ARRAY_SIZE, p_file);

	fscanf(p_file, "rr_fusion_range: %lf",
		&p_junc_view_4_config_common->rr_fusion_range);
	fgets(buf, MAX_ARRAY_SIZE, p_file);

	fscanf(p_file, "parallel_range: %lf",
		&p_junc_view_4_config_common->parallel_range);
	fgets(buf, MAX_ARRAY_SIZE, p_file);

	fscanf(p_file, "curve_range: %lf",
		&p_junc_view_4_config_common->curve_range);
	fgets(buf, MAX_ARRAY_SIZE, p_file);
	fscanf(p_file, "\n");

	return ret;
}

/*
* Function Name:				Load_Junc_View_4_Common_Param
* Function Function:			load junc 3 front view common config param
* Input:
*  p_junc_view_3_front_config_common:the structure save the loaded param
*  p_file:						the file pointer
* Note:
* Revision:
*     Create by Shuo.Yan on 2017/11/30.
*/
AVM_Error_Code_E Load_Junc_View_3_Front_Common_Param(
	OUT P_Junc_View_3_Front_Config_Common_S p_junc_view_3_front_config_common, 
	IN FILE* p_file)
{
	AVM_Error_Code_E ret = ERROR_NONE;

	char buf[MAX_ARRAY_SIZE];

	fgets(buf, MAX_ARRAY_SIZE, p_file);
	fscanf(p_file, "front_view_length: %lf",
		&p_junc_view_3_front_config_common->front_view_length);
	fgets(buf, MAX_ARRAY_SIZE, p_file);

	fscanf(p_file, "left_view_length: %lf",
		&p_junc_view_3_front_config_common->left_view_length);
	fgets(buf, MAX_ARRAY_SIZE, p_file);

	fscanf(p_file, "right_view_length: %lf",
		&p_junc_view_3_front_config_common->right_view_length);
	fgets(buf, MAX_ARRAY_SIZE, p_file);

	fscanf(p_file, "car_head_length: %lf",
		&p_junc_view_3_front_config_common->car_head_length);
	fgets(buf, MAX_ARRAY_SIZE, p_file);

	fscanf(p_file, "front_blind_length: %lf",
		&p_junc_view_3_front_config_common->front_blind_length);
	fgets(buf, MAX_ARRAY_SIZE, p_file);

	fscanf(p_file, "fl_fusion_angle: %lf",
		&p_junc_view_3_front_config_common->fl_fusion_angle);
	fgets(buf, MAX_ARRAY_SIZE, p_file);

	fscanf(p_file, "fr_fusion_angle: %lf",
		&p_junc_view_3_front_config_common->fr_fusion_angle);
	fgets(buf, MAX_ARRAY_SIZE, p_file);

	fscanf(p_file, "fl_fusion_range: %lf",
		&p_junc_view_3_front_config_common->fl_fusion_range);
	fgets(buf, MAX_ARRAY_SIZE, p_file);

	fscanf(p_file, "fr_fusion_range: %lf",
		&p_junc_view_3_front_config_common->fr_fusion_range);
	fgets(buf, MAX_ARRAY_SIZE, p_file);

	fscanf(p_file, "\n");

	return ret;
}




















//
//
///*
//* Function Name:				Load_Result_Size_Param
//* Function Function:			load junc 3 pass rear view result config param
//* Input:
//*     p_result_size_param:		the structure save the loaded param
//*     p_file:					the file pointer
//* Note:
//* Revision:
//*     Create by Shuo.Yan on 2017/11/30.
//*/
//AVM_Error_Code_E Load_Result_Size_Param(
//	OUT P_Result_Size_Param_S p_result_size_param, IN FILE* p_file)
//{
//	AVM_Error_Code_E ret = ERROR_NONE;
//
//	char buf[MAX_ARRAY_SIZE];
//
//	// load result_param
//	fgets(buf, MAX_ARRAY_SIZE, p_file);
//	fscanf(p_file, "result_image_height: %d",
//		&p_result_size_param->result_image_height);
//	fgets(buf, MAX_ARRAY_SIZE, p_file);
//
//	fscanf(p_file, "result_image_width: %d",
//		&p_result_size_param->result_image_width);
//	fgets(buf, MAX_ARRAY_SIZE, p_file);
//
//
//	fscanf(p_file, "car_logo_rect_init_x: %d",
//		&p_result_size_param->car_logo_rect.x);
//	fgets(buf, MAX_ARRAY_SIZE, p_file);
//
//
//	fscanf(p_file, "car_logo_rect_init_y: %d",
//		&p_result_size_param->car_logo_rect.y);
//	fgets(buf, MAX_ARRAY_SIZE, p_file);
//
//
//	fscanf(p_file, "car_logo_rect_width: %d",
//		&p_result_size_param->car_logo_rect.width);
//	fgets(buf, MAX_ARRAY_SIZE, p_file);
//
//	fscanf(p_file, "car_logo_rect_height: %d",
//		&p_result_size_param->car_logo_rect.height);
//	fgets(buf, MAX_ARRAY_SIZE, p_file);
//
//	fscanf(p_file, "\n");
//	return ret;
//}





/*
* Function Name:				Load_Junc_View_3_Rear_Common_Param
* Function Function:			load junc 3 pass rear view common config param
* Input:
*  p_junc_view_3_front_config_common:the structure save the loaded param
*  p_file:						the file pointer
* Note:
* Revision:
*     Create by Shuo.Yan on 2017/11/30.
*/
AVM_Error_Code_E Load_Junc_View_3_Rear_Common_Param(
	OUT P_Junc_View_3_Rear_Config_Common_S p_junc_view_3_rear_config_common,
	IN FILE* p_file)
{
	AVM_Error_Code_E ret = ERROR_NONE;

	char buf[MAX_ARRAY_SIZE];

	//fscanf(p_file, "%s\n", buf);
	//fgets(buf, MAX_ARRAY_SIZE, p_file);
	fscanf(p_file, "rear_view_length: %lf",
		&p_junc_view_3_rear_config_common->rear_view_length);
	fgets(buf, MAX_ARRAY_SIZE, p_file);

	fscanf(p_file, "left_view_length: %lf",
		&p_junc_view_3_rear_config_common->left_view_length);
	fgets(buf, MAX_ARRAY_SIZE, p_file);

	fscanf(p_file, "right_view_length: %lf",
		&p_junc_view_3_rear_config_common->right_view_length);
	fgets(buf, MAX_ARRAY_SIZE, p_file);


	fscanf(p_file, "car_rear_length: %lf",
		&p_junc_view_3_rear_config_common->car_rear_length);
	fgets(buf, MAX_ARRAY_SIZE, p_file);

	fscanf(p_file, "rear_blind_length: %lf",
		&p_junc_view_3_rear_config_common->rear_blind_length);
	fgets(buf, MAX_ARRAY_SIZE, p_file);

	fscanf(p_file, "rl_fusion_angle: %lf",
		&p_junc_view_3_rear_config_common->rl_fusion_angle);
	fgets(buf, MAX_ARRAY_SIZE, p_file);

	fscanf(p_file, "rr_fusion_angle: %lf",
		&p_junc_view_3_rear_config_common->rr_fusion_angle);
	fgets(buf, MAX_ARRAY_SIZE, p_file);

	fscanf(p_file, "rl_fusion_range: %lf",
		&p_junc_view_3_rear_config_common->rl_fusion_range);
	fgets(buf, MAX_ARRAY_SIZE, p_file);

	fscanf(p_file, "rr_fusion_range: %lf",
		&p_junc_view_3_rear_config_common->rr_fusion_range);
	fgets(buf, MAX_ARRAY_SIZE, p_file);


	fscanf(p_file, "\n");

	return ret;
}
/*
* Function Name:				Load_Result_Size_Param
* Function Function:			load junc 4 pass view result config param
* Input:
*     p_result_size_param:		the structure save the loaded param
*     p_file:					the file pointer
* Note:
* Revision:11
*     Create by Shuo.Yan on 2017/11/30.
*/
AVM_Error_Code_E Load_Result_Size_Param(
	OUT P_Result_Size_Param_S p_result_size_param, IN FILE* p_file)
{
	AVM_Error_Code_E ret = ERROR_NONE;

	char buf[MAX_ARRAY_SIZE];

	// load result_param
	fgets(buf, MAX_ARRAY_SIZE, p_file);
	fscanf(p_file, "result_image_height: %d",
		&p_result_size_param->result_image_height);
	fgets(buf, MAX_ARRAY_SIZE, p_file);

	fscanf(p_file, "result_image_width: %d",
		&p_result_size_param->result_image_width);
	fgets(buf, MAX_ARRAY_SIZE, p_file);


	fscanf(p_file, "car_logo_rect_init_x: %d",
		&p_result_size_param->car_logo_rect.x);
	fgets(buf, MAX_ARRAY_SIZE, p_file);


	fscanf(p_file, "car_logo_rect_init_y: %d",
		&p_result_size_param->car_logo_rect.y);
	fgets(buf, MAX_ARRAY_SIZE, p_file);


	fscanf(p_file, "car_logo_rect_width: %d",
		&p_result_size_param->car_logo_rect.width);
	fgets(buf, MAX_ARRAY_SIZE, p_file);

	fscanf(p_file, "car_logo_rect_height: %d",
		&p_result_size_param->car_logo_rect.height);
	fgets(buf, MAX_ARRAY_SIZE, p_file);

	fscanf(p_file, "\n");
	return ret;
}

/*
* Function Name:				Load_Dis_Line_Param
* Function Function:			load distance line config param
* Input:
*     p_result_size_param:		the structure save the loaded param
*     p_file:					the file pointer
* Note:
* Revision:
*     Create by Shuo.Yan on 2017/11/30.
*/
AVM_Error_Code_E Load_Dis_Line_Param(
	OUT P_Dis_Line_Param_S p_dis_line_param, IN FILE* p_file)
{
	AVM_Error_Code_E ret = ERROR_NONE;

	char buf[MAX_ARRAY_SIZE];

	// load static line param
	fgets(buf, MAX_ARRAY_SIZE, p_file);
	fscanf(p_file, "is_static_line_exist: %d",
		&p_dis_line_param->static_line_param.is_static_line_exist);
	fgets(buf, MAX_ARRAY_SIZE, p_file);

	fscanf(p_file, "static_line_dis_to_car[4]: %d %d %d %d",
		&p_dis_line_param->static_line_param.static_line_dis_to_car[0],
		&p_dis_line_param->static_line_param.static_line_dis_to_car[1],
		&p_dis_line_param->static_line_param.static_line_dis_to_car[2],
		&p_dis_line_param->static_line_param.static_line_dis_to_car[3]);
	fgets(buf, MAX_ARRAY_SIZE, p_file);

	fscanf(p_file, "static_line_length: %d",
		&p_dis_line_param->static_line_param.static_line_length);
	fgets(buf, MAX_ARRAY_SIZE, p_file);

	fscanf(p_file, "\n");
	// load dynamic line param
	//fscanf(p_file, "%s\n", buf);
	fgets(buf, MAX_ARRAY_SIZE, p_file);

	fscanf(p_file, "is_dynamic_line_exist: %d",
		&p_dis_line_param->dynamic_line_param.is_dynamic_line_exist);
	fgets(buf, MAX_ARRAY_SIZE, p_file);

	fscanf(p_file, "dynamic_line_count: %d",
		&p_dis_line_param->dynamic_line_param.dynamic_line_count);
	fgets(buf, MAX_ARRAY_SIZE, p_file);

	fscanf(p_file, "dynamic_line_length: %d",
		&p_dis_line_param->dynamic_line_param.dynamic_line_length);
	fgets(buf, MAX_ARRAY_SIZE, p_file);

	fscanf(p_file, "\n");
	return ret;
}

/*
 * Function Name:				Load_Junc_View_4_Param
 * Function Function:			load junc 4 pass view config param
 * Input:
 *     p_junc_view_4_param:		the structure save the loaded param
 *     file_name:				param name
 * Note:
 *     all params stored on disk can only be loaded through this function
 * Revision:
 *     Create by Shuo.Yan on 2017/11/23.
 */
AVM_Error_Code_E Load_Junc_View_4_Param(
	OUT P_Junc_View_4_Param_S p_junc_view_4_param, IN const char* file_name, IN P_Vehicle_Param_S p_vehicle_param)
{
	AVM_Error_Code_E ret = ERROR_NONE;

	P_Dis_Line_Param_S p_dis_line_param_small =
		&p_junc_view_4_param->small_view_dis_line_param;

	P_Dis_Line_Param_S p_dis_line_param_big =
		&p_junc_view_4_param->big_view_dis_line_param;

	FILE* p_file;	

	if (!(p_file = fopen(file_name, "r")))
	{
		printf("%s cannot be opened\n", file_name);
		ret = ERROR_OPEN_FILE_FIAL;
		return ret;
	}	

	// load common_config
	ret = Load_Junc_View_4_Common_Param(
		&p_junc_view_4_param->common_config, p_file);

	// load small view result parameters
	ret = Load_Result_Size_Param(
		&p_junc_view_4_param->result_small_param, p_file);

	// load big view result parameters
	ret = Load_Result_Size_Param(&p_junc_view_4_param->result_big_param, p_file);

	// load small view distance line config parameters
	ret = Load_Dis_Line_Param(p_dis_line_param_small, p_file);

	// load big view distance line config parameters
	ret = Load_Dis_Line_Param(p_dis_line_param_big, p_file);

	fclose(p_file);

#if CAR_DYNAMIC
	Junc_View_4_Param_S  junc_view_4_param = *p_junc_view_4_param;
	Float64_t pixel_row_dis = (p_vehicle_param->vehicle_length + junc_view_4_param.common_config.front_view_length + junc_view_4_param.common_config.rear_view_length)
		/ (Float64_t)(junc_view_4_param.result_big_param.result_image_height);//pixel distance
	Float64_t pixel_col_dis = (p_vehicle_param->vehicle_width + junc_view_4_param.common_config.left_view_length + junc_view_4_param.common_config.right_view_length)
		/ (Float64_t)(junc_view_4_param.result_big_param.result_image_width);//pixel distance

	Float64_t pixel_row_dis_small = (p_vehicle_param->vehicle_length + junc_view_4_param.common_config.front_view_length + junc_view_4_param.common_config.rear_view_length)
		/ (Float64_t)(junc_view_4_param.result_small_param.result_image_height);//pixel distance
	Float64_t pixel_col_dis_small = (p_vehicle_param->vehicle_width + junc_view_4_param.common_config.left_view_length + junc_view_4_param.common_config.right_view_length)
		/ (Float64_t)(junc_view_4_param.result_small_param.result_image_width);//pixel distance

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


	// calc generated small 4-pass lut file size by the loaded parameters
	P_Result_Size_Param_S p_result_small_size_param = 
		&p_junc_view_4_param->result_small_param;
	p_result_small_size_param->lut_size[0] = 
		p_result_small_size_param->result_image_width 
		* p_result_small_size_param->car_logo_rect.y;
	p_result_small_size_param->lut_size[1] = 
		p_result_small_size_param->result_image_width
		* (p_result_small_size_param->result_image_height- 
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
		* (p_result_big_size_param->result_image_width - (p_result_big_size_param->car_logo_rect.x + p_result_big_size_param->car_logo_rect.width));

	// set small distance line lut size to 0
	p_dis_line_param_small->static_line_param.static_line_pixel_count = 0;

	P_Dynamic_Line_Param_S p_small_view_dynamic_line_param = 
		&p_dis_line_param_small->dynamic_line_param;
	for (Int32_t line_id = 0; line_id < MAX_DYNAMIC_LINE_COUNT; line_id++)
	{
		p_small_view_dynamic_line_param->dynamic_line_pixel_count[line_id] = 0;
	}

	// set small distance line lut size to 0
	p_dis_line_param_big->static_line_param.static_line_pixel_count = 0;

	P_Dynamic_Line_Param_S p_big_view_dynamic_line_param =
		&p_dis_line_param_big->dynamic_line_param;
	for (Int32_t line_id = 0; line_id < MAX_DYNAMIC_LINE_COUNT; line_id++)
	{
		p_big_view_dynamic_line_param->dynamic_line_pixel_count[line_id] = 0;
	}

	return ret;
}

/*
* Function Name:				Load_Junc_View_3_Front_Param
* Function Function:			load junc 3 pass front view config param
* Input:
*     p_junc_view_4_param:		the structure save the loaded param
*     file_name:				param name
* Note:
*     all params stored on disk can only be loaded through this function
* Revision:
*     Create by Shuo.Yan on 2017/11/27.
*/
AVM_Error_Code_E Load_Junc_View_3_Front_Param(
	OUT P_Junc_View_3_Front_Param_S p_junc_view_3_front_param, 
	IN const char* file_name, IN P_Vehicle_Param_S p_vehicle_param)
{
	AVM_Error_Code_E ret = ERROR_NONE;

	P_Dis_Line_Param_S p_dis_line_param_3_front =
		&p_junc_view_3_front_param->junc_3_front_dis_line_param;
	FILE* p_file;	

	if (!(p_file = fopen(file_name, "r")))
	{
		ret = ERROR_OPEN_FILE_FIAL;
		CHECK_ERROR(ret);
	}

	ret = Load_Junc_View_3_Front_Common_Param(
		&p_junc_view_3_front_param->common_config, p_file);

	// load junction 3-pass front view result parameters
	ret = Load_Result_Size_Param(
		&p_junc_view_3_front_param->result_3_front_param, p_file);

	// load junction 3-pass front view distance line config parameters
	ret = Load_Dis_Line_Param(
		&p_junc_view_3_front_param->junc_3_front_dis_line_param, p_file);
	fclose(p_file);
#if CAR_DYNAMIC
	/**********************calculate front rect****************************/
	Float64_t pixel_row_dis = (p_junc_view_3_front_param->common_config.car_head_length + p_junc_view_3_front_param->common_config.front_view_length)
		/ (Float64_t)(p_junc_view_3_front_param->result_3_front_param.result_image_height);//pixel distance
	Float64_t pixel_col_dis = (p_vehicle_param->vehicle_width + p_junc_view_3_front_param->common_config.left_view_length + p_junc_view_3_front_param->common_config.right_view_length)
		/ (Float64_t)(p_junc_view_3_front_param->result_3_front_param.result_image_width);//pixel distance

	p_junc_view_3_front_param->result_3_front_param.car_logo_rect.width = (int)(p_vehicle_param->vehicle_width / pixel_col_dis);
	p_junc_view_3_front_param->result_3_front_param.car_logo_rect.height = (int)(p_junc_view_3_front_param->common_config.car_head_length) / pixel_row_dis;
	p_junc_view_3_front_param->result_3_front_param.car_logo_rect.x = (int)(p_junc_view_3_front_param->common_config.left_view_length / pixel_col_dis);
	p_junc_view_3_front_param->result_3_front_param.car_logo_rect.y = (int)(p_junc_view_3_front_param->common_config.front_view_length / pixel_row_dis);
#endif
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
	// set distance line lut size to 0
	p_dis_line_param_3_front->static_line_param.static_line_pixel_count = 0;
	P_Dynamic_Line_Param_S p_junc_3_front_dynamic_dis_line_param =
		&p_dis_line_param_3_front->dynamic_line_param;
	for (Int32_t line_id = 0; line_id < MAX_DYNAMIC_LINE_COUNT; line_id++)
	{
		p_junc_3_front_dynamic_dis_line_param->dynamic_line_pixel_count[line_id] = 0;
	}

	return ret;
}

/*
* Function Name:				Load_Junc_View_3_Rear_Param
* Function Function:			load junc 3 pass rear view config param
* Input:
*     p_junc_view_4_param:		the structure save the loaded param
*     file_name:				param name
* Note:
*     all params stored on disk can only be loaded through this function
* Revision:
*     Create by Shuo.Yan on 2017/11/27.
*/
AVM_Error_Code_E Load_Junc_View_3_Rear_Param(
	OUT P_Junc_View_3_Rear_Param_S p_junc_view_3_rear_param, 
	IN const char* file_name, IN P_Vehicle_Param_S p_vehicle_param)
{
	AVM_Error_Code_E ret = ERROR_NONE;

	P_Dis_Line_Param_S p_dis_line_param_3_rear = 
		&p_junc_view_3_rear_param->junc_3_rear_dis_line_param;

	FILE* p_file;

	if (!(p_file = fopen(file_name, "r")))
	{
		printf("%s cannot be opened\n", file_name);
		ret = ERROR_OPEN_FILE_FIAL;
		return ret;
	}

	// load 3 pass rear view common config param
	ret = Load_Junc_View_3_Rear_Common_Param(
		&(p_junc_view_3_rear_param->common_config), p_file);

	// load 3 pass rear view result size param
	ret = Load_Result_Size_Param(
		&(p_junc_view_3_rear_param->result_3_rear_param), p_file);

	// load 3 pass rear view_dis_line_param
	ret = Load_Dis_Line_Param(p_dis_line_param_3_rear, p_file);

	fclose(p_file);

	P_Result_Size_Param_S p_result_3_rear_param =
		&p_junc_view_3_rear_param->result_3_rear_param;

#if CAR_DYNAMIC
	/**********************calculate rear rect****************************/
	Float64_t pixel_row_dis = (p_junc_view_3_rear_param->common_config.car_rear_length + p_junc_view_3_rear_param->common_config.rear_view_length)
		/ (Float64_t)(p_junc_view_3_rear_param->result_3_rear_param.result_image_height);//pixel distance
	Float64_t pixel_col_dis = (p_vehicle_param->vehicle_width + p_junc_view_3_rear_param->common_config.left_view_length + p_junc_view_3_rear_param->common_config.right_view_length)
		/ (Float64_t)(p_junc_view_3_rear_param->result_3_rear_param.result_image_width);//pixel distance

	p_junc_view_3_rear_param->result_3_rear_param.car_logo_rect.width = (int)(p_vehicle_param->vehicle_width / pixel_col_dis);
	p_junc_view_3_rear_param->result_3_rear_param.car_logo_rect.height = (int)(p_junc_view_3_rear_param->common_config.car_rear_length / pixel_row_dis);
	p_junc_view_3_rear_param->result_3_rear_param.car_logo_rect.x = (int)(p_junc_view_3_rear_param->common_config.left_view_length / pixel_col_dis);
	//p_junc_view_3_rear_param->result_3_rear_param.car_logo_rect.y = (int)(p_junc_view_3_rear_param->common_config.rear_view_length / pixel_row_dis);
	p_junc_view_3_rear_param->result_3_rear_param.car_logo_rect.y = 0;
#endif
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

	// set distance line lut size to 0
	p_dis_line_param_3_rear->static_line_param.static_line_pixel_count = 0;
	P_Dynamic_Line_Param_S p_junc_3_rear_dynamic_dis_line_param =
		&p_dis_line_param_3_rear->dynamic_line_param;
	for (Int32_t line_id = 0; line_id < MAX_DYNAMIC_LINE_COUNT; line_id++)
	{
		p_junc_3_rear_dynamic_dis_line_param->dynamic_line_pixel_count[line_id] = 0;
	}

	return ret;
}

/*
 * Function Name:				Load_Awb_View_Param
 * Function Function:			load awb view config param
 * Input:
 *     p_junc_view_4_param:		the structure save the loaded param
 *     file_name:				param name
 * Note:
 *     all params stored on disk can only be loaded through this function
 * Revision:
 *     Create by Shuo.Yan on 2017/11/25.
 */
AVM_Error_Code_E Load_Awb_View_Param(
	OUT P_Awb_View_Param_S p_awb_view_param, IN const char* file_name
	, IN P_Vehicle_Param_S p_vehicle_param)
{
	AVM_Error_Code_E ret = ERROR_NONE;

	FILE* f;
	char buf[MAX_ARRAY_SIZE];

	if (!(f = fopen(file_name, "r")))
	{
		printf("%s cannot be opened\n", file_name);
		ret = ERROR_OPEN_FILE_FIAL;
		return ret;
	}

	fscanf(f, "front_view_length: %lf", 
		&p_awb_view_param->common_config.front_view_length);
	fgets(buf, MAX_ARRAY_SIZE, f);


	fscanf(f, "rear_view_length: %lf", 
		&p_awb_view_param->common_config.rear_view_length);
	fgets(buf, MAX_ARRAY_SIZE, f);


	fscanf(f, "left_view_length: %lf ", 
		&p_awb_view_param->common_config.left_view_length);
	fgets(buf, MAX_ARRAY_SIZE, f);

	fscanf(f, "right_view_length: %lf", 
		&p_awb_view_param->common_config.right_view_length);
	fgets(buf, MAX_ARRAY_SIZE, f);

	fscanf(f, "front_blind_length: %lf", 
		&p_awb_view_param->common_config.front_blind_length);
	fgets(buf, MAX_ARRAY_SIZE, f);

	fscanf(f, "rear_blind_length: %lf", 
		&p_awb_view_param->common_config.rear_blind_length);
	fgets(buf, MAX_ARRAY_SIZE, f);

	fscanf(f, "fl_fusion_angle: %lf", 
		&p_awb_view_param->common_config.fl_fusion_angle);
	fgets(buf, MAX_ARRAY_SIZE, f);

	fscanf(f, "fr_fusion_angle: %lf", 
		&p_awb_view_param->common_config.fr_fusion_angle);
	fgets(buf, MAX_ARRAY_SIZE, f);

	fscanf(f, "rl_fusion_angle: %lf", 
		&p_awb_view_param->common_config.rl_fusion_angle);
	fgets(buf, MAX_ARRAY_SIZE, f);

	fscanf(f, "rr_fusion_angle: %lf", 
		&p_awb_view_param->common_config.rr_fusion_angle);
	fgets(buf, MAX_ARRAY_SIZE, f);


	fscanf(f, "fl_fusion_range: %lf", 
		&p_awb_view_param->common_config.fl_fusion_range);
	fgets(buf, MAX_ARRAY_SIZE, f);

	fscanf(f, "fr_fusion_range: %lf", 
		&p_awb_view_param->common_config.fr_fusion_range);
	fgets(buf, MAX_ARRAY_SIZE, f);

	fscanf(f, "rl_fusion_range: %lf", 
		&p_awb_view_param->common_config.rl_fusion_range);
	fgets(buf, MAX_ARRAY_SIZE, f);

	fscanf(f, "rr_fusion_range: %lf", 
		&p_awb_view_param->common_config.rr_fusion_range);
	fgets(buf, MAX_ARRAY_SIZE, f);


	fscanf(f, "\n");

	fscanf(f, "result_image_height_awb: %d", 
		&p_awb_view_param->result_awb_param.result_image_height);
	fgets(buf, MAX_ARRAY_SIZE, f);

	fscanf(f, "result_image_width_awb: %d", 
		&p_awb_view_param->result_awb_param.result_image_width);
	fgets(buf, MAX_ARRAY_SIZE, f);

	fscanf(f, "car_logo_rect_awb_init_x: %d", 
		&p_awb_view_param->result_awb_param.car_logo_rect.x);
	fgets(buf, MAX_ARRAY_SIZE, f);

	fscanf(f, "car_logo_rect_awb_init_y: %d", 
		&p_awb_view_param->result_awb_param.car_logo_rect.y);
	fgets(buf, MAX_ARRAY_SIZE, f);

	fscanf(f, "car_logo_rect_awb_width: %d", 
		&p_awb_view_param->result_awb_param.car_logo_rect.width);
	fgets(buf, MAX_ARRAY_SIZE, f);

	fscanf(f, "car_logo_rect_awb_height: %d", 
		&p_awb_view_param->result_awb_param.car_logo_rect.height);
	fgets(buf, MAX_ARRAY_SIZE, f);

	fclose(f);





	Float64_t pixel_row_dis = (p_vehicle_param->vehicle_length + p_awb_view_param->common_config.front_view_length + p_awb_view_param->common_config.rear_view_length)
		/ (Float64_t)(p_awb_view_param->result_awb_param.result_image_height);//pixel distance
	Float64_t pixel_col_dis= (p_vehicle_param->vehicle_width + p_awb_view_param->common_config.left_view_length + p_awb_view_param->common_config.right_view_length)
		/ (Float64_t)(p_awb_view_param->result_awb_param.result_image_width);//pixel distance





	/***********calculate rect x,y width height**************/
	p_awb_view_param->result_awb_param.car_logo_rect.width = (int)(p_vehicle_param->vehicle_width / pixel_col_dis);
	p_awb_view_param->result_awb_param.car_logo_rect.height = (int)(p_vehicle_param->vehicle_length / pixel_row_dis);
	p_awb_view_param->result_awb_param.car_logo_rect.x = (int)(p_awb_view_param->common_config.left_view_length / pixel_col_dis);
	p_awb_view_param->result_awb_param.car_logo_rect.y = (int)(p_awb_view_param->common_config.front_view_length / pixel_row_dis);


	// calc generated small 4-pass lut file size by the loaded parameters
	P_Result_Size_Param_S p_result_awb_param = &p_awb_view_param->result_awb_param;
	p_result_awb_param->lut_size[0] = p_result_awb_param->result_image_width 
		* p_result_awb_param->car_logo_rect.y;
	p_result_awb_param->lut_size[1] = p_result_awb_param->result_image_width 
		* (p_result_awb_param->result_image_height 
		   - (p_result_awb_param->car_logo_rect.y 
		      + p_result_awb_param->car_logo_rect.height)
		   );
	p_result_awb_param->lut_size[2] = p_result_awb_param->result_image_height 
		* p_result_awb_param->car_logo_rect.x;
	p_result_awb_param->lut_size[3] = p_result_awb_param->result_image_height
		* (p_result_awb_param->result_image_width 
		   - (p_result_awb_param->car_logo_rect.x 
		      + p_result_awb_param->car_logo_rect.width)
		  );
	return ret;
}

/*
* Function Name:				Load_Front_Rear_Single_View_Param
* Function Function:			load front or right view config param
* Input:
*     p_fr_single_view_param:	the structure save the loaded param
*     file_name:				param name
* Note:
*     all params stored on disk can only be loaded through this function
* Revision:
*     Create by Shuo.Yan on 2017/11/27.
*/
AVM_Error_Code_E Load_Front_Rear_Single_View_Param(
	OUT P_Front_Rear_Single_View_Param_S p_fr_single_view_param, 
	IN const char* file_name)
{
	AVM_Error_Code_E ret = ERROR_NONE;
	P_Dis_Line_Param_S p_dis_line_param_front = &p_fr_single_view_param->dis_line_param;

	FILE* p_file;
	char buf[MAX_ARRAY_SIZE];

	if (!(p_file = fopen(file_name, "r")))
	{		
		ret = ERROR_OPEN_FILE_FIAL;
		CHECK_ERROR(ret);
	}

	// load result size and expand type
	fgets(buf, MAX_ARRAY_SIZE, p_file);
	fscanf(p_file, "result_image_height: %d", 
		&p_fr_single_view_param->result_image_height);
	fgets(buf, MAX_ARRAY_SIZE, p_file);



	fscanf(p_file, "result_image_width: %d",
		&p_fr_single_view_param->result_image_width);
	fgets(buf, MAX_ARRAY_SIZE, p_file);


	fscanf(p_file, "single_view_expand_type: %d",
		&p_fr_single_view_param->expand_type, buf);
	fgets(buf, MAX_ARRAY_SIZE, p_file);


	fscanf(p_file, "\n");

	Load_Single_View_Crop_Param(&p_fr_single_view_param->crop_param, p_file);

	Load_Single_View_Front_Rear_Plane_Expand_Param(
		&p_fr_single_view_param->plane_expand_param, p_file);

	Load_Single_View_Front_Rear_Box_Expand_Param(
		&p_fr_single_view_param->box_expand_param, p_file);

	Load_Dis_Line_Param(&p_fr_single_view_param->dis_line_param, p_file);

	fclose(p_file);

	// set distance line lut size to 0
	p_dis_line_param_front->static_line_param.static_line_pixel_count = 0;
	P_Dynamic_Line_Param_S p_dynamic_dis_line_param_front = 
		&p_dis_line_param_front->dynamic_line_param;
	for (Int32_t line_id = 0; line_id < MAX_DYNAMIC_LINE_COUNT; line_id++)
	{
		p_dynamic_dis_line_param_front->dynamic_line_pixel_count[line_id] = 0;
	}

	return ret;
}

/*
* Function Name:				Load_Left_Right_Single_View_Param
* Function Function:			load left or right view config param
* Input:
*     p_lr_single_view_param:	the structure save the loaded param
*     file_name:				param name
* Note:
*     all params stored on disk can only be loaded through this function
* Revision:
*     Create by Shuo.Yan on 2017/11/30.
*/
AVM_Error_Code_E Load_Left_Right_Single_View_Param(
	OUT P_Left_Right_Single_View_Param_S p_lr_single_view_param,
	IN const char* file_name)
{
	AVM_Error_Code_E ret = ERROR_NONE;
	P_Dis_Line_Param_S p_dis_line_param_left = &p_lr_single_view_param->dis_line_param;

	FILE* p_file;
	char buf[MAX_ARRAY_SIZE];

	if (!(p_file = fopen(file_name, "r")))
	{
		ret = ERROR_OPEN_FILE_FIAL;
		CHECK_ERROR(ret);
	}

	// load result size and expand type
	//fscanf(p_file, "%s\n", buf);
	fgets(buf, MAX_ARRAY_SIZE, p_file);
	fscanf(p_file, "result_image_height: %d",
		&p_lr_single_view_param->result_image_height);
	fgets(buf, MAX_ARRAY_SIZE, p_file);


	fscanf(p_file, "result_image_width: %d",
		&p_lr_single_view_param->result_image_width);
	fgets(buf, MAX_ARRAY_SIZE, p_file);



	fscanf(p_file, "single_view_expand_type: %d",
		&p_lr_single_view_param->expand_type);
	fgets(buf, MAX_ARRAY_SIZE, p_file);
	fscanf(p_file, "\n");

	Load_Single_View_Crop_Param(&p_lr_single_view_param->crop_param, p_file);

	Load_Single_View_Left_Right_Plane_Expand_Param(
		&p_lr_single_view_param->plane_expand_param, p_file);

	Load_Single_View_Left_Right_Combine_Expand_Param(
		&p_lr_single_view_param->combine_expand_param, p_file);

	Load_Dis_Line_Param(&p_lr_single_view_param->dis_line_param, p_file);

	fclose(p_file);

	// set distance line lut size to 0
	p_dis_line_param_left->static_line_param.static_line_pixel_count = 0;
	P_Dynamic_Line_Param_S p_dynamic_dis_line_param_left =
		&p_dis_line_param_left->dynamic_line_param;
	for (Int32_t line_id = 0; line_id < MAX_DYNAMIC_LINE_COUNT; line_id++)
	{
		p_dynamic_dis_line_param_left->dynamic_line_pixel_count[line_id] = 0;
	}

	return ret;
}

/*
* Function Name:				Load_Left_Right_Together_Single_View_Param
* Function Function:			load left and right together view config param
* Input:
*     p_lr_together_view_param:	the structure save the loaded param
*     file_name_left:			param name left
*     file_name_right:			param name right
* Note:
*     all params stored on disk can only be loaded through this function
* Revision:
*     Create by Shuo.Yan on 2017/11/30.
*/
AVM_Error_Code_E Load_Left_Right_Together_View_Param(
	OUT P_Left_Right_View_Param_S p_lr_together_view_param,
	IN const char* file_name_left, 
	IN const char* file_name_right)
{
	AVM_Error_Code_E ret = ERROR_NONE;
	P_Dis_Line_Param_S p_dis_line_param_left = &p_lr_together_view_param->dis_line_param_left;
	P_Dis_Line_Param_S p_dis_line_param_right = &p_lr_together_view_param->dis_line_param_right;

	FILE* p_file_left, *p_file_right;
	char buf[MAX_ARRAY_SIZE];

	// open and load left view config param
	if (!(p_file_left = fopen(file_name_left, "r")))
	{
		ret = ERROR_OPEN_FILE_FIAL;
		CHECK_ERROR(ret);
	}

	// load result size and expand type
	//fscanf(p_file_left, "%s\n", buf);
	fgets(buf, MAX_ARRAY_SIZE, p_file_left);
	fscanf(p_file_left, "result_image_height: %d",
		&p_lr_together_view_param->result_image_height);
	fgets(buf, MAX_ARRAY_SIZE, p_file_left);

	fscanf(p_file_left, "result_image_width: %d",
		&p_lr_together_view_param->result_image_width);
	fgets(buf, MAX_ARRAY_SIZE, p_file_left);


	fscanf(p_file_left, "single_view_expand_type: %d",
		&p_lr_together_view_param->expand_type);
	fgets(buf, MAX_ARRAY_SIZE, p_file_left);

	fscanf(p_file_left, "\n");

	Load_Single_View_Crop_Param(&p_lr_together_view_param->crop_param_left, p_file_left);

	Load_Single_View_Left_Right_Plane_Expand_Param(
		&p_lr_together_view_param->plane_expand_param_left, p_file_left);

	Load_Single_View_Left_Right_Combine_Expand_Param(
		&p_lr_together_view_param->combine_expand_param_left, p_file_left);

	Load_Dis_Line_Param(p_dis_line_param_left, p_file_left);

	fclose(p_file_left);


	// open and load right view config param
	if (!(p_file_right = fopen(file_name_right, "r")))
	{
		ret = ERROR_OPEN_FILE_FIAL;
		CHECK_ERROR(ret);
	}
	fgets(buf, MAX_ARRAY_SIZE, p_file_right);
	fgets(buf, MAX_ARRAY_SIZE, p_file_right);
	fgets(buf, MAX_ARRAY_SIZE, p_file_right);
	fgets(buf, MAX_ARRAY_SIZE, p_file_right);
	fgets(buf, MAX_ARRAY_SIZE, p_file_right);
	Load_Single_View_Crop_Param(&p_lr_together_view_param->crop_param_right, p_file_right);
	/******xiazai youyou youyo you ******/
	Load_Single_View_Left_Right_Plane_Expand_Param(
		&p_lr_together_view_param->plane_expand_param_right, p_file_right);

	Load_Single_View_Left_Right_Combine_Expand_Param(
		&p_lr_together_view_param->combine_expand_param_right, p_file_right);

	Load_Dis_Line_Param(p_dis_line_param_right, p_file_right);

	fclose(p_file_right);


	// set left side distance line lut size to 0
	p_dis_line_param_left->static_line_param.static_line_pixel_count = 0;
	P_Dynamic_Line_Param_S p_dynamic_dis_line_param_left =
		&p_dis_line_param_left->dynamic_line_param;
	for (Int32_t line_id = 0; line_id < MAX_DYNAMIC_LINE_COUNT; line_id++)
	{
		p_dynamic_dis_line_param_left->dynamic_line_pixel_count[line_id] = 0;
	}

	// set right distance line lut size to 0
	p_dis_line_param_right->static_line_param.static_line_pixel_count = 0;
	P_Dynamic_Line_Param_S p_dynamic_dis_line_param_right =
		&p_dis_line_param_right->dynamic_line_param;
	for (Int32_t line_id = 0; line_id < MAX_DYNAMIC_LINE_COUNT; line_id++)
	{
		p_dynamic_dis_line_param_right->dynamic_line_pixel_count[line_id] = 0;
	}

	return ret;
}
