#include "generate_3_view_lut.h"
#include"resolve_data.h"
#include"generate_4_view_lut.h"
#include<math.h>

/**************calculate world coordinate***********************/
/*
* Function Name:				_3_front_view_calculate_world_coordinate
* Function Function:			_3_front_view calculate_world_coordinate
* Input:
*       i;                      this is picture's row coordinate
*		j;						this is picture's cols coordinate
*       world_coordinate;       this is world coordinate
		p_avm_param;
* Note:
*     the interface function should be called at the beginning of this module
* Revision:
*     Create by Cheng.Li on 2018/1/17.
*/
void  _3_front_view_calculate_world_coordinate(OUT Float32_t *world_coordinate, 
	IN int i, IN int j, IN P_Avm_Param_S p_avm_param)
{


	Junc_View_3_Front_Param_S	junc_view_3_front_param = p_avm_param->p_avm_config_param
		->avm_2d_param.junc_view_3_front_param;
	CvRect car_logo_rect = junc_view_3_front_param.result_3_front_param.car_logo_rect;
	Vehicle_Param_S	 vehicle_param = p_avm_param->p_avm_config_param->vehicle_param;
	
	Float32_t pixel_row_dis = (junc_view_3_front_param.common_config.car_head_length
		+ junc_view_3_front_param.common_config.front_view_length)
		/ (Float32_t)(junc_view_3_front_param.result_3_front_param.result_image_height);//pixel distance
	Float32_t pixel_col_dis = (vehicle_param.vehicle_width + junc_view_3_front_param.common_config.left_view_length
		+ junc_view_3_front_param.common_config.right_view_length)
		/ (Float32_t)(junc_view_3_front_param.result_3_front_param.result_image_width);//pixel distance
	

	world_coordinate[0] = (Float32_t)(pixel_col_dis)*(Float32_t)((-(car_logo_rect.x + (car_logo_rect.width / 2))) + j);
	world_coordinate[1] = (Float32_t)(pixel_row_dis)*(Float32_t)(car_logo_rect.y + car_logo_rect.height - i) 
		+ p_avm_param->p_avm_config_param->vehicle_param.vehicle_length / 2
		- junc_view_3_front_param.common_config.car_head_length;

}

/***************world coordinate to pic coordinate**********************/

/*
* Function Name:				_3_front_view_world_coordinate_to_pic_coordinate
* Function Function:			_3_front_view world_coordinate_to_pic_coordinate
* Input:
*       camera_id;             this is you shuold look for which picture
*		world_coordinate;
*       image_point;           this is return the picture coordinate
		p_avm_param;
* Note:
*
* Revision:
*     Create by Cheng.Li on 2018/1/17.
*/
void _3_front_view_world_coordinate_to_pic_coordinate(IN int camera_id, 
	IN Float32_t * world_coordinate, OUT Float32_t *image_point, IN P_Avm_Param_S p_avm_param)
{
	Float32_t camera_coordinate[3];

	Mat_Mul_Vec_3by3_DB(camera_coordinate, p_avm_param->p_avm_config_param
		->camera_model[camera_id].camera_model_ext.inv_R, world_coordinate);
	for (int i = 3; i < 6; i++)
	{
		camera_coordinate[i - 3] = p_avm_param->p_avm_config_param
			->camera_model[camera_id].camera_model_ext.inv_pose[i] 
								+ camera_coordinate[i - 3];
	}

	Float32_t tmp = 0;
	tmp = camera_coordinate[0];
	camera_coordinate[0] = camera_coordinate[1];
	camera_coordinate[1] = tmp;
	camera_coordinate[2] = -camera_coordinate[2];
	Cam_Ray_To_Image_Point(image_point, camera_coordinate, &(p_avm_param
		->p_avm_config_param->camera_model[camera_id].camera_model_int));

}

/*****************************zip data**************************/
/*
* Function Name:				_3_front_view_zip_data
* Function Function:			_3_front_view_zip_data
* Input:
*       shift_altogether;       this is the position you should storage
*		image_point;			the picture coordinate
*       weight_Float32_t;          the weight of pixel
		p_avm_param
* Note:
*
* Revision:
*     Create by Cheng.Li on 2018/1/12.
*/
AVM_Error_Code_E _3_front_view_zip_data(IN Int32_t shift_altogether, IN Float32_t *image_point
		, IN Float32_t weight_Float32_t, IN P_Avm_Param_S p_avm_param)
{
	AVM_Error_Code_E ret = ERROR_NONE;
	Int32_t u_int = (Int32_t)image_point[0], v_int = (Int32_t)image_point[1];
	Float32_t u_Float32_t = image_point[0] - u_int, v_Float32_t = image_point[1] - v_int;
	UInt32_t u_point_int = (UInt32_t)u_Float32_t * 255, v_point_int = (UInt32_t)v_Float32_t * 255;
	Int32_t temp_shift = 0;
	Int32_t temp_shift0 = 0;
	Int32_t temp_weight;
	Int32_t temp_camera = 0;
	u_int = u_int << 21;
	v_int = v_int << 10;
	temp_weight = (Int32_t)(weight_Float32_t * 255);
	temp_weight = temp_weight << 2;
	temp_shift = temp_shift | u_int;
	temp_shift = temp_shift | v_int;
	temp_shift = temp_shift | temp_weight;
	temp_shift = temp_shift | temp_camera;
	temp_shift0 = (u_point_int << 24) + (v_point_int << 16);
	p_avm_param->p_avm_lut->p_lut_junc_view_3_front[shift_altogether] = temp_shift;
	p_avm_param->p_avm_lut->p_lut_junc_view_3_front[shift_altogether + 1] = temp_shift0;
	return ret;
}




#if 1
//这一版可以完美运行不掺杂MATLAB 的拉伸效果存下来 
/*******init lut********/
/*
* Function Name:				generate_3_front_view_lut
* Function Function:			generate 3 front_view lut
* Input:
*       image[4];
* Note:
*
* Revision:
*     Create by Cheng.Li on 2018/1/17.
*/
AVM_Error_Code_E generate_3_front_view_lut(IN P_Avm_Param_S p_avm_param)
{
	AVM_Error_Code_E ret = ERROR_NONE;
	Junc_View_3_Front_Param_S junc_view_3_front_param = p_avm_param
		->p_avm_config_param->avm_2d_param.junc_view_3_front_param;
	/**********calculate fusion equation  malloc some memory to storage front weight to calculate left and right weight
	, which is assure weight's sum is 1.000 *****
	******/
	p_avm_param->p_avm_config_param->vehicle_param.vehicle_length = p_avm_param
		->p_avm_config_param->vehicle_param.vehicle_length +
		junc_view_3_front_param.common_config.front_blind_length;

	Float32_t *weight_2_dim = (Float32_t *)malloc(sizeof(Float32_t)
		*junc_view_3_front_param.result_3_front_param.result_image_width
		*junc_view_3_front_param.result_3_front_param.result_image_height);
	if (!weight_2_dim)
	{
		ret = ERROR_MEMORY_MALLOC_FAIL;
		return ret;
	}
	memset(weight_2_dim, 0, sizeof(Float32_t)
		*junc_view_3_front_param.result_3_front_param.result_image_width
		*junc_view_3_front_param.result_3_front_param.result_image_height);
	/****malloc look up table ****/
	int camera_size[4];
	for (Int32_t cam_id = 0; cam_id < CAMERA_NUM; cam_id++)
	{
		camera_size[cam_id] = junc_view_3_front_param.result_3_front_param.lut_size[cam_id] * 2;
	}
	int m0_sum = 0;//存放地址偏移的变量
	int m1_sum = 0;
	int m2_sum = 0;
	int m3_sum = 0;
	/*****public use*****/
	Float32_t world_coordinate[3];//world_coordinate x,y,z
	world_coordinate[2] = 0;
	
	CvRect car_logo_rect = junc_view_3_front_param.result_3_front_param.car_logo_rect;
	CvSize size0 = cvSize(junc_view_3_front_param.result_3_front_param.result_image_width
		, junc_view_3_front_param.result_3_front_param.result_image_height);//
#if CAR_DYNAMIC//此处 如果定义了 CAR_DYNAMIC表示小车随着视野范围的配置变化而变化
	Float32_t pixel_row_dis = (junc_view_3_front_param.common_config.car_head_length 
		+ junc_view_3_front_param.common_config.front_view_length)
		/ (Float32_t)(junc_view_3_front_param.result_3_front_param.result_image_height);//pixel distance
	Float32_t pixel_col_dis = (p_avm_param->p_avm_config_param->vehicle_param.vehicle_width 
		+ junc_view_3_front_param.common_config.left_view_length 
		+ junc_view_3_front_param.common_config.right_view_length)
		/ (Float32_t)(junc_view_3_front_param.result_3_front_param.result_image_width);//pixel distance
	Float32_t weight_Float32_t;
#endif

#if CAR_FIXED//支持安霸项目的小车位置是固定的
	Float32_t pixel_row_dis_front = (junc_view_3_front_param.common_config.front_view_length)
		/ (Float32_t)(car_logo_rect.y);//pixel distance
	Float32_t pixel_row_dis_middle = junc_view_3_front_param.common_config.car_head_length 
		/ junc_view_3_front_param.result_3_front_param.car_logo_rect.height;
	Float32_t pixel_col_dis_left = (junc_view_3_front_param.common_config.left_view_length)
		/ (Float32_t)(car_logo_rect.x);//pixel distance
	Float32_t pixel_col_dis_middle = (p_avm_param->p_avm_config_param->vehicle_param.vehicle_width)
		/ (Float32_t)(car_logo_rect.width);//pixel distance
	Float32_t pixel_col_dis_right = (junc_view_3_front_param.common_config.right_view_length)
		/ (Float32_t)(junc_view_3_front_param.result_3_front_param.result_image_width
		- car_logo_rect.width - car_logo_rect.x);//pixel distance
	UInt32_t left_right_flag;
#endif
	/********************extend all over result image pixel****************************/
	for (int i = 0; i < junc_view_3_front_param.result_3_front_param.result_image_height; i++)
	{
		for (int j = 0; j < junc_view_3_front_param.result_3_front_param.result_image_width; j++)
		{
			Float32_t image_point[2];
#if CAR_FIXED
#if 1
			/************front view*******************************/
			if (i <= car_logo_rect.y )//
			{
				if (j <= car_logo_rect.x)//如果是左上角
				{
					/***********calculate world coordinate*********/
					world_coordinate[0] = (Float32_t)(pixel_col_dis_left)
						*(Float32_t)(j - car_logo_rect.x)
						- (p_avm_param->p_avm_config_param->vehicle_param.vehicle_width / 2);
					world_coordinate[1] = (Float32_t)(pixel_row_dis_front)
						*(Float32_t)(car_logo_rect.y - i)
						+ (p_avm_param->p_avm_config_param->vehicle_param.vehicle_length / 2);
				}
				if (j >= car_logo_rect.x + car_logo_rect.width)//
				{
					/***********calculate world coordinate*********/
					world_coordinate[0] = (Float32_t)(pixel_col_dis_right)
						*(Float32_t)(j - car_logo_rect.x - car_logo_rect.width)
						+ (p_avm_param->p_avm_config_param->vehicle_param.vehicle_width / 2);
					world_coordinate[1] = (Float32_t)(pixel_row_dis_front)
						*(Float32_t)(car_logo_rect.y - i)
						+ (p_avm_param->p_avm_config_param->vehicle_param.vehicle_length / 2);

				}
				if (j > car_logo_rect.x&&j < car_logo_rect.x 
					+ car_logo_rect.width)//如果是前部中间的非融合区域
				{
					/***********calculate world coordinate*********/
					world_coordinate[0] = (Float32_t)(pixel_col_dis_middle)
						*(Float32_t)(j - car_logo_rect.x - car_logo_rect.width / 2);
					world_coordinate[1] = (Float32_t)(pixel_row_dis_front)
						*(Float32_t)(car_logo_rect.y - i)
						+ (p_avm_param->p_avm_config_param->vehicle_param.vehicle_length / 2);
				}
				/********calculate weight************/
				Float32_t weight_Float32_t = calculate_4_view_weight_curve(CAM_FRONT, i, j,
					world_coordinate, weight_2_dim, p_avm_param, junc_view_3_front_param.result_3_front_param,1);
				/***********world coordinate to pic coordinate**************/
				_3_front_view_world_coordinate_to_pic_coordinate(CAM_FRONT, 
					world_coordinate, image_point, p_avm_param);//0 is front
				/********zip data and fill lut*****/
				m3_sum = i * junc_view_3_front_param.result_3_front_param.result_image_width * 2 + j * 2;
				//_3_front_view_zip_data(m3_sum, image_point, weight_Float32_t, p_avm_param);
				if (j <  junc_view_3_front_param.result_3_front_param.result_image_width / 2)
				{
					left_right_flag = 1;
				}
				zip_data_new(m3_sum, image_point, weight_Float32_t, 
					p_avm_param->p_avm_lut->p_lut_junc_view_3_front, CAM_FRONT, left_right_flag, p_avm_param);

			}
#endif
			/************left view*******************************/
			if (j < car_logo_rect.x)//左部的数据区
			{

				if (i <= car_logo_rect.y)//左上角
				{
					/***********calculate world coordinate*********/
					world_coordinate[0] = (Float32_t)(pixel_col_dis_left)*(Float32_t)(j - car_logo_rect.x)
						- (p_avm_param->p_avm_config_param->vehicle_param.vehicle_width / 2);
					world_coordinate[1] = (Float32_t)(pixel_row_dis_front)*(Float32_t)(car_logo_rect.y - i)
						+ (p_avm_param->p_avm_config_param->vehicle_param.vehicle_length / 2);
				}
				if (i >= car_logo_rect.y)
				{
					/***********calculate world coordinate*********/
					world_coordinate[0] = (Float32_t)(pixel_col_dis_left)*(Float32_t)(j - car_logo_rect.x)
						- (p_avm_param->p_avm_config_param->vehicle_param.vehicle_width / 2);
					world_coordinate[1] = (p_avm_param->p_avm_config_param->vehicle_param.vehicle_length / 2) 
						- (Float32_t)(pixel_row_dis_middle)*(Float32_t)(i - car_logo_rect.y);
				}

				/*********calculate weight*****/

				Float32_t weight_Float32_t;
				weight_Float32_t = 1.000000 - weight_2_dim[i
					*junc_view_3_front_param.result_3_front_param.result_image_width + j];
				/***********world coordinate to pic coordinate**************/
				world_coordinate_to_pic_coordinate(CAM_LEFT, world_coordinate, image_point, p_avm_param);
				/********************************************zip data***************************************/
				m2_sum = camera_size[0] + camera_size[1] + i * car_logo_rect.x * 2 + j * 2;
				//_3_front_view_zip_data(m2_sum, image_point, weight_Float32_t, p_avm_param);
				zip_data_new(m2_sum, image_point, weight_Float32_t,
					p_avm_param->p_avm_lut->p_lut_junc_view_3_front, CAM_FRONT, left_right_flag, p_avm_param);
			}
			/*************************right view****************/
			if (j >= car_logo_rect.x + car_logo_rect.width)//
			{
				/***********calculate world coordinate*********/
				if (i <= car_logo_rect.y)//
				{
					/***********calculate world coordinate*********/
					world_coordinate[0] = (Float32_t)(pixel_col_dis_right)
						*(Float32_t)(j - car_logo_rect.x - car_logo_rect.width)
						+ (p_avm_param->p_avm_config_param->vehicle_param.vehicle_width / 2);
					world_coordinate[1] = (Float32_t)(pixel_row_dis_front)*(Float32_t)(car_logo_rect.y - i)
						+ (p_avm_param->p_avm_config_param->vehicle_param.vehicle_length / 2);

				}

				if (i >= car_logo_rect.y&&i <= car_logo_rect.y + car_logo_rect.height)
				{
					/***********calculate world coordinate*********/
					world_coordinate[0] = (Float32_t)(pixel_col_dis_right)
						*(Float32_t)(j - car_logo_rect.x - car_logo_rect.width)
						+ (p_avm_param->p_avm_config_param->vehicle_param.vehicle_width / 2);
					world_coordinate[1] = (p_avm_param->p_avm_config_param->vehicle_param.vehicle_length / 2)
						- (Float32_t)(pixel_row_dis_middle)*(Float32_t)(i - car_logo_rect.y);
				}
				/*********calculate weight ************/
				Float32_t weight_Float32_t;
				weight_Float32_t = 1.000000 - weight_2_dim[i
					*junc_view_3_front_param.result_3_front_param.result_image_width + j];
				/***********world coordinate to pic coordinate**************/
				world_coordinate_to_pic_coordinate(CAM_RIGHT,
					world_coordinate, image_point, p_avm_param);
				/*******************zip data*******************/
				/*******************zip data*******************/
				m3_sum = camera_size[0] + camera_size[2]
					+ i * (junc_view_3_front_param.result_3_front_param.result_image_width
					- car_logo_rect.x - car_logo_rect.width) * 2
					+ (j - car_logo_rect.x - car_logo_rect.width) * 2;
				//_3_front_view_zip_data(m3_sum, image_point, weight_Float32_t, p_avm_param);
				zip_data_new(m3_sum, image_point, weight_Float32_t,
					p_avm_param->p_avm_lut->p_lut_junc_view_3_front, CAM_FRONT, left_right_flag, p_avm_param);
			}

#endif
#if CAR_DYNAMIC
			/***********calculate world coordinate*********/
			_3_front_view_calculate_world_coordinate(world_coordinate, i, j, p_avm_param);
			/************front view*******************************/
			if (i < car_logo_rect.y)
			{
				/********calculate weight************/
				Float32_t weight_Float32_t = calculate_4_view_weight_curve(CAM_FRONT, i, j,
					world_coordinate, weight_2_dim, p_avm_param,
					junc_view_3_front_param.result_3_front_param,1);
				/***********world coordinate to pic coordinate**************/
				Float32_t image_point[2];
				_3_front_view_world_coordinate_to_pic_coordinate(0,
					world_coordinate, image_point, p_avm_param);//0 is front
				/******get pixel from source pic***********/
				/********zip data and fill lut*****/
				m0_sum = i * junc_view_3_front_param.result_3_front_param.result_image_width * 2 + j * 2;
				_3_front_view_zip_data(m0_sum, image_point, weight_Float32_t, p_avm_param);
			}
			/*******************************************left view********************************/
			if (j < car_logo_rect.x)
			{
				/*********calculate weight*****/
				Float32_t weight_Float32_t;
				weight_Float32_t = 1.000000 - weight_2_dim[i
					*junc_view_3_front_param.result_3_front_param.result_image_width + j];
				/***********world coordinate to pic coordinate**************/
				Float32_t image_point[2];
				_3_front_view_world_coordinate_to_pic_coordinate(2, 
					world_coordinate, image_point, p_avm_param);
				/*get pixel from source pic*/
				/********************************************zip data***************************************/
				m2_sum = camera_size[0] + camera_size[1] + i * car_logo_rect.x * 2 + j * 2;
				_3_front_view_zip_data(m2_sum, image_point, weight_Float32_t, p_avm_param);

			}
			/*************************right view****************/
			if (j >= (car_logo_rect.x + car_logo_rect.width))
			{
				/*********calculate weight ************/
				Float32_t weight_Float32_t;
				weight_Float32_t = 1.000000 - weight_2_dim[i
					*junc_view_3_front_param.result_3_front_param.result_image_width + j];
				/***********world coordinate to pic coordinate**************/
				Float32_t image_point[2];
				_3_front_view_world_coordinate_to_pic_coordinate(3, 
					world_coordinate, image_point, p_avm_param);

				/*******************zip data*******************/
				m3_sum = camera_size[0] + camera_size[2]
					+ i * (junc_view_3_front_param.result_3_front_param.result_image_width
					- car_logo_rect.x - car_logo_rect.width) * 2
					+ (j - car_logo_rect.x - car_logo_rect.width) * 2;
				_3_front_view_zip_data(m3_sum, image_point, weight_Float32_t, p_avm_param);
			}
#endif
		}
	}
	free(weight_2_dim);
	p_avm_param->p_avm_config_param->vehicle_param.vehicle_length 
		= p_avm_param->p_avm_config_param->vehicle_param.vehicle_length +
		junc_view_3_front_param.common_config.front_blind_length;//前三路的盲区用另一种方式实现
	return ret;
}
#endif
/*****************************zip data**************************/
/*
* Function Name:				_3_rear_view_zip_data
* Function Function:			_3_rear_view_zip_data
* Input:
*       shift_altogether;       this is the position you should storage
*		image_point;			the picture coordinate
*       weight_Float32_t;          the weight of pixel
p_avm_param;
* Note:
*
* Revision:
*     Create by Cheng.Li on 2018/1/12.
*/
AVM_Error_Code_E _3_rear_view_zip_data(IN int shift_altogether, IN Float32_t *image_point,
	IN Float32_t weight_Float32_t, IN P_Avm_Param_S p_avm_param)
{
	AVM_Error_Code_E ret = ERROR_NONE;
	UInt32_t u_int = (UInt32_t)image_point[0], v_int = (UInt32_t)image_point[1];
	Float32_t u_Float32_t = image_point[0] - u_int, v_Float32_t = image_point[1] - v_int;
	UInt32_t u_point_int = (UInt32_t)(u_Float32_t * 255), v_point_int = (UInt32_t)(v_Float32_t * 255);
	UInt32_t temp_shift = 0;
	UInt32_t temp_shift0 = 0;
	UInt32_t temp_weight;
	UInt32_t temp_camera = 0;
	u_int = u_int << 21;
	v_int = v_int << 10;
	temp_weight = (UInt32_t)(weight_Float32_t * 255);
	temp_weight = temp_weight << 2;
	temp_shift = temp_shift | u_int;
	temp_shift = temp_shift | v_int;
	temp_shift = temp_shift | temp_weight;
	temp_shift = temp_shift | temp_camera;
	temp_shift0 = (u_point_int << 24) + (v_point_int << 16);
	p_avm_param->p_avm_lut->p_lut_junc_view_3_rear[shift_altogether] = temp_shift;
	p_avm_param->p_avm_lut->p_lut_junc_view_3_rear[shift_altogether + 1] = temp_shift0;
	return ret;
}



/*
* Function Name:				 _3_rear_view_calculate_front_weight
* Function Function:			 _3_rear view calculate front weight
* Input:
*       i;
*		j;
*       world_coordinate;
*		weight_2_dim;
p_avm_param;
* Note:
*     the interface function should be called at the beginning of this module
* Revision:
*     Create by Cheng.Li on 2018/1/12.
*/
Float32_t _3_rear_view_calculate_front_weight(int i, int j, 
	Float32_t *world_coordinate, Float32_t *weight_2_dim, IN P_Avm_Param_S p_avm_param)
{

	Junc_View_3_Rear_Param_S	junc_view_3_rear_param = p_avm_param
		->p_avm_config_param->avm_2d_param.junc_view_3_rear_param;
	CvRect car_logo_rect = junc_view_3_rear_param.result_3_rear_param.car_logo_rect;
	Vehicle_Param_S	 vehicle_param = p_avm_param->p_avm_config_param->vehicle_param;
	Float32_t pixel_row_dis = (junc_view_3_rear_param.common_config.car_rear_length 
		+ junc_view_3_rear_param.common_config.rear_view_length)
		/ (Float32_t)(junc_view_3_rear_param.result_3_rear_param.result_image_height);//pixel distance
	Float32_t pixel_col_dis = (p_avm_param->p_avm_config_param->vehicle_param.vehicle_width 
		+ junc_view_3_rear_param.common_config.left_view_length
		+ junc_view_3_rear_param.common_config.right_view_length)
		/ (Float32_t)(junc_view_3_rear_param.result_3_rear_param.result_image_height);//pixel distance
	Float32_t weight_Float32_t;

	/***calculate front view weight***/
	Float32_t rl_fusion_angle = junc_view_3_rear_param.common_config.rl_fusion_angle;
	Float32_t rl_fusion_range = junc_view_3_rear_param.common_config.rl_fusion_range;
	Float32_t rl_fusion_0 = rl_fusion_angle - rl_fusion_range / 2;

	Float32_t rr_fusion_angle = junc_view_3_rear_param.common_config.rr_fusion_angle;
	Float32_t rr_fusion_range = junc_view_3_rear_param.common_config.rr_fusion_range;
	Float32_t rr_fusion_0 = rr_fusion_angle - (rr_fusion_range / 2);
	Float32_t world_coordinate_2[2];
	_3_rear_view_calculate_world_coordinate(world_coordinate_2, 
		Int32_t(car_logo_rect.y + car_logo_rect.height+junc_view_3_rear_param.common_config.rear_blind_length / pixel_row_dis), car_logo_rect.x,
		p_avm_param);
	Float32_t world_coordinate_3[2];
	_3_rear_view_calculate_world_coordinate(world_coordinate_3, Int32_t(car_logo_rect.y + car_logo_rect.height + junc_view_3_rear_param.common_config.rear_blind_length / pixel_row_dis),
		car_logo_rect.x + car_logo_rect.width, p_avm_param);

	if (j<car_logo_rect.x || j>(car_logo_rect.x + car_logo_rect.width))
	{

		if (world_coordinate[0] > 0)
		{
			Float32_t temp_angle = atan((world_coordinate[1] - world_coordinate_3[1]) 
				/ (world_coordinate[0] - world_coordinate_3[0]));
			Float32_t temp_point_abgle = (temp_angle / 3.1415) * 180;
			weight_Float32_t = (abs(temp_point_abgle) - rl_fusion_0) / rl_fusion_range;
			if (weight_Float32_t >= 1)
			{
				weight_Float32_t = 1;
			}
			if (weight_Float32_t <= 0)
			{
				weight_Float32_t = 0;
			}
		}
		if (world_coordinate[0] < 0)
		{
			Float32_t temp_angle = (Float32_t)atan((world_coordinate[1] - world_coordinate_2[1])
				/ (world_coordinate[0] - world_coordinate_2[0]));
			Float32_t temp_point_abgle = (temp_angle / 3.1415) * 180;
			weight_Float32_t = (abs(temp_point_abgle) - rr_fusion_0) / rr_fusion_range;
			if (weight_Float32_t >= 1)
			{
				weight_Float32_t = 1;
			}
			if (weight_Float32_t <= 0)
			{
				weight_Float32_t = 0;
			}
		}
	}
	else
	{
		weight_Float32_t = 1.000;
	}
	weight_2_dim[i*junc_view_3_rear_param.result_3_rear_param.result_image_width 
		+ j] = weight_Float32_t;//storage 2 dim weighjt
	return	weight_Float32_t;

}


/***************world coordinate to pic coordinate**********************/

/*
* Function Name:				_3_rear_view_world_coordinate_to_pic_coordinate
* Function Function:			_3_rear_view world coordinate to pic_coordinate
* Input:
*       camera_id;             this is you shuold look for which picture
*		world_coordinate;
*       image_point;           this is return the picture coordinate
p_avm_param;
* Note:
*
* Revision:
*     Create by Cheng.Li on 2018/1/17.
*/
void _3_rear_view_world_coordinate_to_pic_coordinate(IN int camera_id,
	IN Float32_t * world_coordinate, OUT Float32_t *image_point, IN P_Avm_Param_S p_avm_param)
{
	Float32_t camera_coordinate[3];

	Mat_Mul_Vec_3by3_DB(camera_coordinate, p_avm_param->p_avm_config_param
		->camera_model[camera_id].camera_model_ext.inv_R, world_coordinate);
	for (int i = 3; i < 6; i++)
	{
		camera_coordinate[i - 3] = p_avm_param->p_avm_config_param
			->camera_model[camera_id].camera_model_ext.inv_pose[i] 
			+ camera_coordinate[i - 3];
	}

	Float32_t tmp = 0;
	tmp = camera_coordinate[0];
	camera_coordinate[0] = camera_coordinate[1];
	camera_coordinate[1] = tmp;
	camera_coordinate[2] = -camera_coordinate[2];
	Cam_Ray_To_Image_Point(image_point, camera_coordinate, &(p_avm_param
		->p_avm_config_param->camera_model[camera_id].camera_model_int));
}

/**************calculate world coordinate***********************/
/*
* Function Name:				_3_rear_view_calculate_world_coordinate
* Function Function:			_3 rear view calculate_world_coordinate
* Input:
*       i;                      this is picture's row coordinate
*		j;						this is picture's cols coordinate
*       world_coordinate;       this is world coordinate
p_avm_param;
* Note:
*     the interface function should be called at the beginning of this module
* Revision:
*     Create by Cheng.Li on 2018/1/12.
*/
void  _3_rear_view_calculate_world_coordinate(OUT Float32_t *world_coordinate,
	IN int i, IN int j, IN P_Avm_Param_S p_avm_param)
{


	Junc_View_3_Rear_Param_S	junc_view_3_rear_param = p_avm_param
		->p_avm_config_param->avm_2d_param.junc_view_3_rear_param;
	CvRect car_logo_rect = junc_view_3_rear_param.result_3_rear_param.car_logo_rect;
	Vehicle_Param_S	 vehicle_param = p_avm_param->p_avm_config_param->vehicle_param;
	
	Float32_t pixel_row_dis = (junc_view_3_rear_param.common_config.car_rear_length 
		+ junc_view_3_rear_param.common_config.rear_view_length)
		/ (Float32_t)(junc_view_3_rear_param.result_3_rear_param.result_image_height);//pixel distance
	Float32_t pixel_col_dis = (vehicle_param.vehicle_width 
		+ junc_view_3_rear_param.common_config.left_view_length 
		+ junc_view_3_rear_param.common_config.right_view_length)
		/ (Float32_t)(junc_view_3_rear_param.result_3_rear_param.result_image_height);//pixel distance
	

	world_coordinate[0] = (Float32_t)(pixel_col_dis)
		*(Float32_t)((-(car_logo_rect.x + car_logo_rect.width / 2)) + j);
	world_coordinate[1] = -(Float32_t)(pixel_row_dis)
		*(Float32_t)(i) - (p_avm_param->p_avm_config_param->vehicle_param.vehicle_length / 2
		- junc_view_3_rear_param.common_config.car_rear_length);
}



/*********************generate_3_rear_view_lut********/
/*
* Function Name:				generate_3_rear_view_lut
* Function Function:			generate_3_rear_view_lut
* Input:
*       image[4];
p_avm_param;
* Note:
*
* Revision:
*     Create by Cheng.Li on 2018/1/17.
*/
AVM_Error_Code_E  generate_3_rear_view_lut(IN P_Avm_Param_S p_avm_param)
{
	AVM_Error_Code_E ret = ERROR_NONE;
	Junc_View_3_Rear_Param_S junc_view_3_rear_param = p_avm_param
		->p_avm_config_param->avm_2d_param.junc_view_3_rear_param;
	/**********calculate fusion equation  malloc some memory to storage front
	weight to calculate left and right weight, which is assure weight's sum is 1.000 **************/
	Float32_t *weight_2_dim = (Float32_t *)malloc(sizeof(Float32_t)
		*junc_view_3_rear_param.result_3_rear_param.result_image_width
							*junc_view_3_rear_param.result_3_rear_param.result_image_height);
	if (!weight_2_dim)
	{
		ret = ERROR_MEMORY_MALLOC_FAIL;
		CHECK_ERROR(ret);
	}
	memset(weight_2_dim, 0, sizeof(Float32_t)
		*junc_view_3_rear_param.result_3_rear_param.result_image_width
		  *junc_view_3_rear_param.result_3_rear_param.result_image_height);
	/****malloc look up table ****/
	int camera_size[4];
	for (Int32_t cam_id = 0; cam_id < CAMERA_NUM; cam_id++)
	{
		camera_size[cam_id] = junc_view_3_rear_param.result_3_rear_param.lut_size[cam_id] * 2;
	}
	/*****public use*****/
	Float32_t world_coordinate[3];//world_coordinate x,y,z
	world_coordinate[2] = 0;
	
	CvRect car_logo_rect = junc_view_3_rear_param.result_3_rear_param.car_logo_rect;
    CvSize size0 = cvSize(junc_view_3_rear_param.result_3_rear_param.result_image_width,
		junc_view_3_rear_param.result_3_rear_param.result_image_height);//
	int m3_sum = 0;
	int m0_sum = 0;
	int m1_sum = 0;
	int m2_sum = 0;
	Float32_t pixel_row_dis = (junc_view_3_rear_param.common_config.car_rear_length 
		+ junc_view_3_rear_param.common_config.rear_view_length)
		/ (Float32_t)(junc_view_3_rear_param.result_3_rear_param.result_image_height);//pixel distance
	Float32_t pixel_col_dis = (p_avm_param->p_avm_config_param->vehicle_param.vehicle_width 
		+ junc_view_3_rear_param.common_config.left_view_length
		+ junc_view_3_rear_param.common_config.right_view_length)
		/ (Float32_t)(junc_view_3_rear_param.result_3_rear_param.result_image_height);//pixel distance

#if CAR_FIXED
	/*******************/

	Float32_t pixel_row_dis_middle = (junc_view_3_rear_param.common_config.car_rear_length)
		/ (Float32_t)(car_logo_rect.height);//pixel distance
	Float32_t pixel_row_dis_back = (junc_view_3_rear_param.common_config.rear_view_length)
		/ (Float32_t)(junc_view_3_rear_param.result_3_rear_param.result_image_height
		- car_logo_rect.height - car_logo_rect.y);//pixel distance
	Float32_t pixel_col_dis_left = (junc_view_3_rear_param.common_config.left_view_length)
		/ (Float32_t)(car_logo_rect.x);//pixel distance
	Float32_t pixel_col_dis_middle = (p_avm_param->p_avm_config_param->vehicle_param.vehicle_width)
		/ (Float32_t)(car_logo_rect.width);//pixel distance
	Float32_t pixel_col_dis_right = (junc_view_3_rear_param.common_config.right_view_length)
		/ (Float32_t)(junc_view_3_rear_param.result_3_rear_param.result_image_width
		- car_logo_rect.width - car_logo_rect.x);//pixel distance
	//盲区的设置变量
	Int32_t result_image_height = junc_view_3_rear_param.result_3_rear_param.result_image_height;
	Int32_t result_image_width = junc_view_3_rear_param.result_3_rear_param.result_image_width;
	//后盲区的变量
	Int32_t back_blind_pix = (Int32_t)(junc_view_3_rear_param.common_config.rear_blind_length / pixel_row_dis_back);
	Int32_t Back_height = (Int32_t)(result_image_height - (car_logo_rect.y + car_logo_rect.height));
	Int32_t Back_width = result_image_width;
	Int32_t cut_blind_height = Back_height - back_blind_pix;
	Int32_t add_blind_middle = car_logo_rect.height + back_blind_pix;
	Float32_t ratio_back = (Float32_t)cut_blind_height / (Float32_t)Back_height;
	Float32_t ratio_middle = (Float32_t)car_logo_rect.height / (Float32_t)add_blind_middle;
#endif
	
	UInt32_t left_right_flag = 0;
	/********************extend all over result image pixel****************************/
	for (int i = 0; i < junc_view_3_rear_param.result_3_rear_param.result_image_height; i++)
	{
		for (int j = 0; j < junc_view_3_rear_param.result_3_rear_param.result_image_width; j++)
		{
#if CAR_FIXED
			/*************rear view*********************************/
			if (i >= car_logo_rect.y + car_logo_rect.height)// 
			{
				int a0 = i - (car_logo_rect.y + car_logo_rect.height);//专门为了盲区而实现
				Float32_t a1 = a0*ratio_back;
				Float32_t a2 = a1 + back_blind_pix;
				if (j <= car_logo_rect.x)//左下角
				{
					/***********calculate world coordinate*********/
					world_coordinate[0] = (Float32_t)(pixel_col_dis_left)
						*(Float32_t)(j - car_logo_rect.x)//横坐标
						- (p_avm_param->p_avm_config_param->vehicle_param.vehicle_width / 2);
					world_coordinate[1] = (Float32_t)(pixel_row_dis_back)*(Float32_t)(-a2)
						- (p_avm_param->p_avm_config_param->vehicle_param.vehicle_length / 2);
				}
				if (j >= car_logo_rect.x + car_logo_rect.width)//右下角
				{
					/***********calculate world coordinate*********/
					world_coordinate[0] = (Float32_t)(pixel_col_dis_right)
						*(Float32_t)(j - car_logo_rect.x - car_logo_rect.width)
						+ (p_avm_param->p_avm_config_param->vehicle_param.vehicle_width / 2);
					world_coordinate[1] = (Float32_t)(pixel_row_dis_back)*(Float32_t)(-a2)
						- (p_avm_param->p_avm_config_param->vehicle_param.vehicle_length / 2);
				}
				if (j >= car_logo_rect.x&&j < car_logo_rect.x + car_logo_rect.width)
				{
					/***********calculate world coordinate*********/
					world_coordinate[0] = (Float32_t)(pixel_col_dis_middle)
						*(Float32_t)(j - car_logo_rect.x - car_logo_rect.width / 2);
					world_coordinate[1] = (Float32_t)(pixel_row_dis_back)*(Float32_t)(-a2)
						- (p_avm_param->p_avm_config_param->vehicle_param.vehicle_length / 2);
				}

				/********calculate weight************/
				Float32_t weight_Float32_t = calculate_4_view_weight_curve(CAM_REAR, i, j, world_coordinate,
					weight_2_dim, p_avm_param, junc_view_3_rear_param.result_3_rear_param,2);


				/***********world coordinate to pic coordinate**************/
				Float32_t image_point[2];
				world_coordinate_to_pic_coordinate(CAM_REAR, world_coordinate, image_point, p_avm_param);//0 is front

				/********zip data and fill lut*****/
				m0_sum = (i - (car_logo_rect.y + car_logo_rect.height)) 
					* junc_view_3_rear_param.result_3_rear_param.result_image_width * 2 + j * 2;
				if (j <  junc_view_3_rear_param.result_3_rear_param.result_image_width / 2)
				{
					left_right_flag = 1;
				}
				zip_data_new(m0_sum, image_point, weight_Float32_t,
					p_avm_param->p_avm_lut->p_lut_junc_view_3_rear, CAM_FRONT, left_right_flag, p_avm_param);

				//_3_rear_view_zip_data(m0_sum, image_point, weight_Float32_t, p_avm_param);
			}
			/*******************************************left view********************************/
			if (j < car_logo_rect.x)
			{
				if (i >= car_logo_rect.y + car_logo_rect.height)//左后部
				{
					int a0 = i - (car_logo_rect.y + car_logo_rect.height);//专门为了盲区而实现
					Float32_t a1 = a0*ratio_back;
					Float32_t a2 = a1 + back_blind_pix;

					/***********calculate world coordinate*********/
					world_coordinate[0] = (Float32_t)(pixel_col_dis_left)*(Float32_t)(j - car_logo_rect.x)//横坐标
						- (p_avm_param->p_avm_config_param->vehicle_param.vehicle_width / 2);
					world_coordinate[1] = (Float32_t)(pixel_row_dis_back)*(Float32_t)(-a2)
						- (p_avm_param->p_avm_config_param->vehicle_param.vehicle_length / 2);
				}
				if (i >= car_logo_rect.y&&i <= car_logo_rect.y + car_logo_rect.height)//车体中间部
				{
					Float32_t pixel_row_dis_middle_blind_rear = 
						(junc_view_3_rear_param.common_config.car_rear_length 
						+ junc_view_3_rear_param.common_config.rear_blind_length)
						/ (Float32_t)(car_logo_rect.height);//pixel distance;
					/***********calculate world coordinate*********/
					world_coordinate[0] = (Float32_t)(pixel_col_dis_left)*(Float32_t)(j - car_logo_rect.x)
						- (p_avm_param->p_avm_config_param->vehicle_param.vehicle_width / 2);
					world_coordinate[1] = 
						(Float32_t)(pixel_row_dis_middle_blind_rear)*(Float32_t)(car_logo_rect.height - i)
						- (p_avm_param->p_avm_config_param->vehicle_param.vehicle_length / 2
						+ junc_view_3_rear_param.common_config.rear_blind_length);
				}

				/*********calculate weight*****/
				Float32_t weight_Float32_t;
				weight_Float32_t = 1.000000 - weight_2_dim[i
					* junc_view_3_rear_param.result_3_rear_param.result_image_width + j];
				/***********world coordinate to pic coordinate**************/
				Float32_t image_point[2];
				world_coordinate_to_pic_coordinate(CAM_LEFT, world_coordinate, image_point, p_avm_param);
				/********************************************zip data***************************************/
				m1_sum = camera_size[0] + camera_size[1] + i * car_logo_rect.x * 2 + j * 2;
				//_3_rear_view_zip_data(m1_sum, image_point, weight_Float32_t, p_avm_param);
				zip_data_new(m1_sum, image_point, weight_Float32_t,
					p_avm_param->p_avm_lut->p_lut_junc_view_3_rear, CAM_FRONT, left_right_flag, p_avm_param);
			}

			/*************************right view****************/
			if (j >= car_logo_rect.x + car_logo_rect.width)//
			{
				/***********calculate world coordinate*********/
				
				if (i >= car_logo_rect.y + car_logo_rect.height)//右下角
				{
					int a0 = i - (car_logo_rect.y + car_logo_rect.height);//专门为了盲区而实现
					Float32_t a1 = a0*ratio_back;
					Float32_t a2 = a1 + back_blind_pix;

					/***********calculate world coordinate*********/
					world_coordinate[0] = (Float32_t)(pixel_col_dis_right)
						*(Float32_t)(j - car_logo_rect.x - car_logo_rect.width)
						+ (p_avm_param->p_avm_config_param->vehicle_param.vehicle_width / 2);
					world_coordinate[1] = (Float32_t)(pixel_row_dis_back)*(Float32_t)(-a2)
						- (p_avm_param->p_avm_config_param->vehicle_param.vehicle_length / 2);

				}
				if (i >= car_logo_rect.y&&i <= car_logo_rect.y + car_logo_rect.height)//车体的中间部
				{

					Float32_t pixel_row_dis_middle_blind_rear = 
						(junc_view_3_rear_param.common_config.car_rear_length 
						+ junc_view_3_rear_param.common_config.rear_blind_length)
						/ (Float32_t)(car_logo_rect.height);//pixel distance;
					/***********calculate world coordinate*********/
					world_coordinate[0] = (Float32_t)(pixel_col_dis_right)
						*(Float32_t)(j - car_logo_rect.x - car_logo_rect.width)
						+ (p_avm_param->p_avm_config_param->vehicle_param.vehicle_width / 2);
					
					world_coordinate[1] = (Float32_t)(pixel_row_dis_middle_blind_rear)
						*(Float32_t)(car_logo_rect.height - i)
						- (p_avm_param->p_avm_config_param->vehicle_param.vehicle_length / 2 
						+ junc_view_3_rear_param.common_config.rear_blind_length);
				}
				/*********calculate weight ************/
				Float32_t weight_Float32_t;
				Float32_t image_point[2];
				weight_Float32_t = 1.000000 - weight_2_dim[i
					*junc_view_3_rear_param.result_3_rear_param.result_image_width + j];
				/***********world coordinate to pic coordinate**************/
				world_coordinate_to_pic_coordinate(CAM_RIGHT, world_coordinate, image_point, p_avm_param);
				/*******************zip data*******************/
				m3_sum = camera_size[0] + camera_size[1] + camera_size[2]
					+ i * (junc_view_3_rear_param.result_3_rear_param.result_image_width
					- car_logo_rect.x - car_logo_rect.width) * 2
					+ (j - car_logo_rect.x - car_logo_rect.width) * 2;
				//_3_rear_view_zip_data(m3_sum, image_point, weight_Float32_t, p_avm_param);
				zip_data_new(m3_sum, image_point, weight_Float32_t,
					p_avm_param->p_avm_lut->p_lut_junc_view_3_rear, CAM_FRONT, left_right_flag, p_avm_param);
			}
#endif
#if  CAR_DYNAMIC
			/***********calculate world coordinate*********/
			_3_rear_view_calculate_world_coordinate(world_coordinate, i, j, p_avm_param);
			/************rear view*******************************/
			if (i >= car_logo_rect.y + car_logo_rect.height 
				+ junc_view_3_rear_param.common_config.rear_blind_length / pixel_row_dis)
			{
				/********calculate weight************/
				/*Float32_t weight_Float32_t = _3_rear_view_calculate_front_weight(i,
					j, world_coordinate, weight_2_dim, p_avm_param);*/
				Float32_t weight_Float32_t = calculate_4_view_weight_curve(CAM_REAR, i, j, world_coordinate,
					weight_2_dim, p_avm_param, junc_view_3_rear_param.result_3_rear_param,2);

				/***********world coordinate to pic coordinate**************/
				Float32_t image_point[2];
				_3_rear_view_world_coordinate_to_pic_coordinate(1, world_coordinate, image_point, p_avm_param);//0 is front

				/********zip data and fill lut*****/
				m0_sum = (i - (car_logo_rect.y + car_logo_rect.height)) 
					* junc_view_3_rear_param.result_3_rear_param.result_image_width * 2 + j * 2;
				_3_rear_view_zip_data(m0_sum, image_point, weight_Float32_t, p_avm_param);
			}
			/*******************************************left view********************************/
			if (j < car_logo_rect.x)
			{
				/*********calculate weight*****/
				Float32_t weight_Float32_t;
				weight_Float32_t = 1.000000 - weight_2_dim[i
					*junc_view_3_rear_param.result_3_rear_param.result_image_width + j];

				/***********world coordinate to pic coordinate**************/
				Float32_t image_point[2];
				_3_rear_view_world_coordinate_to_pic_coordinate(2, world_coordinate, image_point, p_avm_param);

				/********************************************zip data***************************************/
				m1_sum = camera_size[0] + camera_size[1] + i * car_logo_rect.x * 2 + j * 2;
				_3_rear_view_zip_data(m1_sum, image_point, weight_Float32_t, p_avm_param);
			}
			/*************************right view****************/
			if (j >= (car_logo_rect.x + car_logo_rect.width))
			{
				/*********calculate weight ************/
				Float32_t weight_Float32_t;
				weight_Float32_t = 1.000000 - weight_2_dim[i
					*junc_view_3_rear_param.result_3_rear_param.result_image_width + j];
				/***********world coordinate to pic coordinate**************/
				Float32_t image_point[2];
				_3_rear_view_world_coordinate_to_pic_coordinate(3, 
					world_coordinate, image_point, p_avm_param);

				/*******************zip data*******************/
				m3_sum = camera_size[0] + camera_size[1] + camera_size[2]
					+ i * (junc_view_3_rear_param.result_3_rear_param.result_image_width 
					- car_logo_rect.x - car_logo_rect.width) * 2
					+ (j - car_logo_rect.x - car_logo_rect.width) * 2;
				_3_rear_view_zip_data(m3_sum, image_point, weight_Float32_t, p_avm_param);
			}
#endif
			
		}
	}
	free(weight_2_dim);

	return ret;
}










































































