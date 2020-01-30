#include "fread_bin.h"


void read_bin(IN  P_Avm_Param_S p_avm_param,IN int view_flag)
{
	CvSize size0;
	UInt32_t *p_lut_static_line = NULL;
	UInt32_t *p_lut_dynamic_line = NULL;
	Left_Right_Single_View_Param_S left_right_single_view_param;
	int result_image_height;
	int result_image_width;
	CvRect car_logo_rect;
	UInt32_t * p_lut = NULL;
	int camera_size[4];
	FILE *fid;
	if (view_flag == _4_VIEW_BIG)//大图
	{
		p_lut = p_avm_param->p_avm_lut->p_lut_junc_view_4_big;
		result_image_height = p_avm_param->p_avm_config_param->avm_2d_param.junc_view_4_param.result_big_param.result_image_height;
		result_image_width = p_avm_param->p_avm_config_param->avm_2d_param.junc_view_4_param.result_big_param.result_image_width;
		size0 = cvSize(result_image_width, result_image_height);//
		car_logo_rect = p_avm_param->p_avm_config_param->avm_2d_param.junc_view_4_param.result_big_param.car_logo_rect;
		for (Int32_t cam_id = 0; cam_id < CAMERA_NUM; cam_id++)
		{
			camera_size[cam_id] = p_avm_param->p_avm_config_param->avm_2d_param.junc_view_4_param.result_big_param.lut_size[cam_id] * 2;
		}
		char vehicle_param_name[MAX_ARRAY_SIZE];
		/*******存储大图的二进制表********/
		sprintf(vehicle_param_name, "%s/4_view_big.bin", lut_file_path);
		fid = fopen(vehicle_param_name, "rb");
		int  size_all = camera_size[0] + camera_size[1] + camera_size[2] + camera_size[3];
		//p_lut = (UInt32_t*)malloc(sizeof(UInt32_t)* size_all);
		//memset(p_lut_p, 0, sizeof(UInt32_t)*  size_all);

		fread(p_lut, sizeof(UInt32_t), size_all, fid);
		fclose(fid);
	}
	if (view_flag == _4_VIEW_SMALL)//小图
	{
		result_image_height = p_avm_param->p_avm_config_param->avm_2d_param.junc_view_4_param.result_small_param.result_image_height;
		result_image_width = p_avm_param->p_avm_config_param->avm_2d_param.junc_view_4_param.result_small_param.result_image_width;
		size0 = cvSize(result_image_width, result_image_height);//
		car_logo_rect = p_avm_param->p_avm_config_param->avm_2d_param.junc_view_4_param.result_small_param.car_logo_rect;
		for (Int32_t cam_id = 0; cam_id < CAMERA_NUM; cam_id++)
		{
			camera_size[cam_id] = p_avm_param->p_avm_config_param->avm_2d_param.junc_view_4_param.result_small_param.lut_size[cam_id] * 2;
		}
		char vehicle_param_name[MAX_ARRAY_SIZE];
		/*******存储大图的二进制表********/
		sprintf(vehicle_param_name, "%s/4_view_small.bin", lut_file_path);
		fid = fopen(vehicle_param_name, "rb");
		int  size_all = camera_size[0] + camera_size[1] + camera_size[2] + camera_size[3];
		//p_lut = (UInt32_t*)malloc(sizeof(UInt32_t)* size_all);
		//memset(p_lut_p, 0, sizeof(UInt32_t)*  size_all);

		fread(p_lut, sizeof(UInt32_t), size_all, fid);
		fclose(fid);
	}

	if (view_flag == _3_VIEW_FRONT)//
	{
		Junc_View_3_Front_Param_S	junc_view_3_front_param = p_avm_param->p_avm_config_param->avm_2d_param.junc_view_3_front_param;
		CvRect car_logo_rect = junc_view_3_front_param.result_3_front_param.car_logo_rect;
		Vehicle_Param_S	 vehicle_param = p_avm_param->p_avm_config_param->vehicle_param;
		Float32_t pixel_row_dis = (vehicle_param.vehicle_length + junc_view_3_front_param.common_config.front_view_length)
			/ (Float32_t)(junc_view_3_front_param.result_3_front_param.result_image_height);//pixel distance
		Float32_t pixel_col_dis = (vehicle_param.vehicle_width + junc_view_3_front_param.common_config.left_view_length
			+ junc_view_3_front_param.common_config.right_view_length)
			/ (Float32_t)(junc_view_3_front_param.result_3_front_param.result_image_height);//pixel distance
		Float32_t weight_Float32_t;
		CvSize size0 = cvSize(junc_view_3_front_param.result_3_front_param.result_image_width,
			junc_view_3_front_param.result_3_front_param.result_image_height);//
		for (Int32_t cam_id = 0; cam_id < CAMERA_NUM; cam_id++)
		{
			camera_size[cam_id] = p_avm_param->p_avm_config_param->avm_2d_param.junc_view_3_front_param.result_3_front_param.lut_size[cam_id] * 2;
		}
		char vehicle_param_name[MAX_ARRAY_SIZE];
		/*******存储的二进制表********/
		sprintf(vehicle_param_name, "%s/3_view_front.bin", lut_file_path);
		fid = fopen(vehicle_param_name, "rb");
		int  size_all = camera_size[0] + camera_size[1] + camera_size[2] + camera_size[3];
		//p_lut = (UInt32_t*)malloc(sizeof(UInt32_t)* size_all);
		//memset(p_lut_p, 0, sizeof(UInt32_t)*  size_all);

		fread(p_avm_param->p_avm_lut->p_lut_junc_view_3_front, sizeof(UInt32_t), size_all, fid);
		fclose(fid);

	}


	if (view_flag == _3_VIEW_REAR)//
	{
		Junc_View_3_Rear_Param_S	junc_view_3_rear_param = p_avm_param->p_avm_config_param
			->avm_2d_param.junc_view_3_rear_param;
		CvRect car_logo_rect = junc_view_3_rear_param.result_3_rear_param.car_logo_rect;
		Vehicle_Param_S	 vehicle_param = p_avm_param->p_avm_config_param->vehicle_param;
		Float32_t pixel_row_dis = (vehicle_param.vehicle_length + junc_view_3_rear_param.common_config.rear_view_length)
			/ (Float32_t)(junc_view_3_rear_param.result_3_rear_param.result_image_height);//pixel distance
		Float32_t pixel_col_dis = (vehicle_param.vehicle_width + junc_view_3_rear_param.common_config.left_view_length
			+ junc_view_3_rear_param.common_config.right_view_length)
			/ (Float32_t)(junc_view_3_rear_param.result_3_rear_param.result_image_height);//pixel distance
		Float32_t weight_Float32_t;

		UInt32_t *p_lut_static_line = p_avm_param->p_avm_dis_line->p_static_dis_line_3_rear;
		UInt32_t *p_lut_dynamic_line = p_avm_param->p_avm_dis_line->p_dynamic_dis_line_3_rear;


		CvSize size0 = cvSize(junc_view_3_rear_param.result_3_rear_param.result_image_width,
			junc_view_3_rear_param.result_3_rear_param.result_image_height);//
		for (Int32_t cam_id = 0; cam_id < CAMERA_NUM; cam_id++)
		{
			camera_size[cam_id] = p_avm_param->p_avm_config_param->avm_2d_param.junc_view_3_rear_param.result_3_rear_param.lut_size[cam_id] * 2;
		}
		char vehicle_param_name[MAX_ARRAY_SIZE];
		/*******存储的二进制表********/
		sprintf(vehicle_param_name, "%s/3_view_rear.bin", lut_file_path);
		fid = fopen(vehicle_param_name, "rb");
		int  size_all = camera_size[0] + camera_size[1] + camera_size[2] + camera_size[3];
		//p_lut = (UInt32_t*)malloc(sizeof(UInt32_t)* size_all);
		//memset(p_lut_p, 0, sizeof(UInt32_t)*  size_all);

		fread(p_avm_param->p_avm_lut->p_lut_junc_view_3_rear, sizeof(UInt32_t), size_all, fid);
		fclose(fid);
	}
	Front_Rear_Single_View_Param_S single_view_param_front_rear;
	if (view_flag == _1_VIEW_FRONT)
	{
		single_view_param_front_rear = p_avm_param->p_avm_config_param->avm_single_view_param.front_single_view_param;
		size0 = cvSize(single_view_param_front_rear.result_image_width, single_view_param_front_rear.result_image_height);//
		p_lut = p_avm_param->p_avm_lut->p_lut_single_view_front;
		p_lut_static_line = p_avm_param->p_avm_dis_line->p_static_dis_line_front;
		p_lut_dynamic_line = p_avm_param->p_avm_dis_line->p_dynamic_dis_line_front;
		result_image_height = single_view_param_front_rear.result_image_height;
		result_image_width = single_view_param_front_rear.result_image_width;

		char vehicle_param_name[MAX_ARRAY_SIZE];
		/*******存储的二进制表********/
		sprintf(vehicle_param_name, "%s/single_view4.bin", lut_file_path);
		fid = fopen(vehicle_param_name, "rb");
		int  size_all = result_image_height*result_image_width*2;
		//p_lut = (UInt32_t*)malloc(sizeof(UInt32_t)* size_all);
		//memset(p_lut_p, 0, sizeof(UInt32_t)*  size_all);

		fread(p_avm_param->p_avm_lut->p_lut_single_view_front, sizeof(UInt32_t), size_all, fid);
		fclose(fid);



	}
	if (view_flag == _1_VIEW_REAR)
	{
		single_view_param_front_rear = p_avm_param->p_avm_config_param->avm_single_view_param.rear_single_view_param;
		size0 = cvSize(single_view_param_front_rear.result_image_width, single_view_param_front_rear.result_image_height);//
		p_lut = p_avm_param->p_avm_lut->p_lut_single_view_rear;
		p_lut_static_line = p_avm_param->p_avm_dis_line->p_static_dis_line_rear;
		p_lut_dynamic_line = p_avm_param->p_avm_dis_line->p_dynamic_dis_line_rear;
		result_image_height = single_view_param_front_rear.result_image_height;
		result_image_width = single_view_param_front_rear.result_image_width;

		char vehicle_param_name[MAX_ARRAY_SIZE];
		/*******存储的二进制表********/
		sprintf(vehicle_param_name, "%s/single_view5.bin", lut_file_path);
		fid = fopen(vehicle_param_name, "rb");
		int  size_all = result_image_height*result_image_width * 2;
		//p_lut = (UInt32_t*)malloc(sizeof(UInt32_t)* size_all);
		//memset(p_lut_p, 0, sizeof(UInt32_t)*  size_all);

		fread(p_avm_param->p_avm_lut->p_lut_single_view_rear, sizeof(UInt32_t), size_all, fid);
		fclose(fid);
	}

	if (view_flag == _1_VIEW_LEFT)
	{
		left_right_single_view_param = p_avm_param->p_avm_config_param->avm_single_view_param.left_single_view_param;
		size0 = cvSize(left_right_single_view_param.result_image_width, left_right_single_view_param.result_image_height);//
		p_lut = p_avm_param->p_avm_lut->p_lut_single_view_left;
		p_lut_static_line = p_avm_param->p_avm_dis_line->p_static_dis_line_rear;
		p_lut_dynamic_line = p_avm_param->p_avm_dis_line->p_dynamic_dis_line_rear;
		result_image_height = left_right_single_view_param.result_image_height;
		result_image_width = left_right_single_view_param.result_image_width;
		char vehicle_param_name[MAX_ARRAY_SIZE];
		/*******存储的二进制表********/
		sprintf(vehicle_param_name, "%s/single_view6.bin", lut_file_path);
		fid = fopen(vehicle_param_name, "rb");
		int  size_all = result_image_height*result_image_width * 2;
		//p_lut = (UInt32_t*)malloc(sizeof(UInt32_t)* size_all);
		//memset(p_lut_p, 0, sizeof(UInt32_t)*  size_all);

		fread(p_avm_param->p_avm_lut->p_lut_single_view_left, sizeof(UInt32_t), size_all, fid);
		fclose(fid);

	}
	if (view_flag == _1_VIEW_RIGHT)
	{
		left_right_single_view_param = p_avm_param->p_avm_config_param->avm_single_view_param.right_single_view_param;
		size0 = cvSize(left_right_single_view_param.result_image_width, left_right_single_view_param.result_image_height);//
		p_lut = p_avm_param->p_avm_lut->p_lut_single_view_right;
		p_lut_static_line = p_avm_param->p_avm_dis_line->p_static_dis_line_right;
		p_lut_dynamic_line = p_avm_param->p_avm_dis_line->p_dynamic_dis_line_right;
		result_image_height = left_right_single_view_param.result_image_height;
		result_image_width = left_right_single_view_param.result_image_width;
		char vehicle_param_name[MAX_ARRAY_SIZE];
		/*******存储的二进制表********/
		sprintf(vehicle_param_name, "%s/single_view7.bin", lut_file_path);
		fid = fopen(vehicle_param_name, "rb");
		int  size_all = result_image_height*result_image_width * 2;
		//p_lut = (UInt32_t*)malloc(sizeof(UInt32_t)* size_all);
		//memset(p_lut_p, 0, sizeof(UInt32_t)*  size_all);

		fread(p_avm_param->p_avm_lut->p_lut_single_view_right, sizeof(UInt32_t), size_all, fid);
		fclose(fid);

	}
	if (view_flag == _1_VIEW_LEFT_RIGHT)
	{
		result_image_height = p_avm_param->p_avm_config_param->avm_single_view_param.left_right_view_param.result_image_height;
		result_image_width = p_avm_param->p_avm_config_param->avm_single_view_param.left_right_view_param.result_image_width;

		size0 = cvSize(result_image_width, result_image_height);//
		p_lut = p_avm_param->p_avm_lut->p_lut_single_view_left_right;
		p_lut_static_line = p_avm_param->p_avm_dis_line->p_static_dis_line_left_right;
		p_lut_dynamic_line = p_avm_param->p_avm_dis_line->p_dynamic_dis_line_left_right;

		char vehicle_param_name[MAX_ARRAY_SIZE];
		/*******存储的二进制表********/
		sprintf(vehicle_param_name, "%s/single_view8.bin", lut_file_path);
		fid = fopen(vehicle_param_name, "rb");
		int  size_all = result_image_height*result_image_width * 2;
		//p_lut = (UInt32_t*)malloc(sizeof(UInt32_t)* size_all);
		//memset(p_lut_p, 0, sizeof(UInt32_t)*  size_all);

		fread(p_avm_param->p_avm_lut->p_lut_single_view_left_right, sizeof(UInt32_t), size_all, fid);
		fclose(fid);
	}


}


#if 0

#endif

