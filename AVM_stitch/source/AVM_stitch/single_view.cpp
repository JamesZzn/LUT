
#include"single_view.h"
#include"resolve_data.h"

/**************single_view_zip_data***********************/
/*
* Function Name:				single_view_zip_data
* Function Function:			single_view_zip_data
* Input:
*       shift_altogether;
*		image_point;					 this is picture's flag is front or back
		p_lut;
		camera_flag��
* Note:
*     the interface function should be called at the beginning of this module
* Revision:
*     Create by Cheng.Li on 2018/1/30.
*/

AVM_Error_Code_E single_view_zip_data(IN int shift_altogether, IN Float32_t *image_point, IN UInt32_t*p_lut, IN UInt32_t camera_flag)
{
	AVM_Error_Code_E ret = ERROR_NONE;
	UInt32_t u_int = (UInt32_t)image_point[0], v_int = (UInt32_t)image_point[1];   //get integer'part of position
	Float32_t u_Float32_t = image_point[0] - u_int, v_Float32_t = image_point[1] - v_int; // get decimal'partr of position
	UInt32_t u_point_int0 = (UInt32_t)u_Float32_t * 255, v_point_int0 = (UInt32_t)v_Float32_t * 255;
	UInt32_t u_point_int = (UInt32_t)u_Float32_t * 255, v_point_int = (UInt32_t)v_Float32_t * 255;
	UInt32_t temp_shift = 0;
	UInt32_t temp_shift0 = 0;
	UInt32_t temp_weight = 1;     //one pixel's weight in dst_pic
	temp_shift = (u_int << 21) + (v_int << 10) + (temp_weight << 2) + (camera_flag);
	temp_shift0 = (u_point_int << 24) + (v_point_int << 16);
	p_lut[shift_altogether] = temp_shift;
	p_lut[shift_altogether + 1] = temp_shift0;
	return ret;
}

/**************crop_front_rear_single_view***********************/
/*
* Function Name:				crop_front_rear_single_view
* Function Function:			crop_front_rear_single_view
* Input:
*       p_avm_param;                     
*		cam_flag;					 this is picture's flag is front or back
* Note:
*     the interface function should be called at the beginning of this module
* Revision:
*     Create by Cheng.Li on 2018/1/30.
*/
AVM_Error_Code_E crop_front_rear_single_view(IN P_Avm_Param_S p_avm_param, IN int cam_flag)
{
	AVM_Error_Code_E ret = ERROR_NONE;
	Front_Rear_Single_View_Param_S single_view_param;
	UInt32_t* p_lut = NULL;
	UInt32_t cam_id;
	if (cam_flag == _1_VIEW_FRONT)
	{
		single_view_param = p_avm_param->p_avm_config_param
			->avm_single_view_param.front_single_view_param;
		p_lut = p_avm_param->p_avm_lut->p_lut_single_view_front;
		cam_id = CAM_FRONT;
	}
	if (cam_flag == _1_VIEW_REAR)
	{
		single_view_param = p_avm_param->p_avm_config_param
			->avm_single_view_param.rear_single_view_param;
		p_lut = p_avm_param->p_avm_lut->p_lut_single_view_rear;
		cam_id = CAM_REAR;
	}

	Float32_t scale_coordinate_col = Float32_t(single_view_param.crop_param.crop_rect.width) 
									/ Float32_t(single_view_param.result_image_width);//���ԭͼ��ü��� �Ĵ�С����

	Float32_t scale_coordinate_row = Float32_t(single_view_param.crop_param.crop_rect.height) 
									/ Float32_t(single_view_param.result_image_height);//���ԭͼ��ü��� �Ĵ�С����
	for (int i = 0; i < single_view_param.result_image_height; i++)
	{
		for (int j = 0; j < single_view_param.result_image_width; j++)
		{
			Float32_t image_point[2];
			if (cam_flag == _1_VIEW_REAR || cam_flag == _1_VIEW_FRONT)//ǰ����ͷ
			{
				image_point[0] = single_view_param.crop_param.crop_rect.y 
					+ i*scale_coordinate_row;
				image_point[1] = single_view_param.crop_param.crop_rect.x
					+ j*scale_coordinate_col;
			}

			UInt32_t  shift_altogether = i*single_view_param.result_image_width * 2 + j * 2;//��������ƫ�Ƶ�ַ
			//ret = single_view_zip_data(shift_altogether, image_point, p_lut, cam_id);
			zip_data_new(shift_altogether, image_point,1,
				p_lut, NULL, NULL, p_avm_param);//Ϊ�˴������ã�����ͼ��ʹû���ں�Ȩ��Ҳʹ����·ȫ���洢��ʽ�ò����Ĵ洢0
			CHECK_ERROR(ret);
		}
	}
	return ret;
}










#if 1
/**************plane_front_rear_single_view***********************/
/*
* Function Name:				plane_front_rear_single_view
* Function Function:			plane_front_rear_single_view
* Input:
*       p_avm_param;
*		cam_flag;					 this is picture's flag is front or back
* Note:
*     the interface function should be called at the beginning of this module
* Revision:
*     Create by Cheng.Li on 2018/1/30.
*/
AVM_Error_Code_E  plane_front_rear_single_view(IN P_Avm_Param_S p_avm_param, IN  int cam_flag)
{
	AVM_Error_Code_E ret = ERROR_NONE;
	Front_Rear_Single_View_Param_S single_view_param;
	UInt32_t* p_lut = NULL;
	Int32_t is_front_cam;
	UInt32_t camera_id;
	Float32_t z_up_down,x_left_right_shift;
	if (cam_flag == _1_VIEW_FRONT)
	{
		single_view_param = p_avm_param->p_avm_config_param
			->avm_single_view_param.front_single_view_param;
		p_lut = p_avm_param->p_avm_lut->p_lut_single_view_front;
		is_front_cam = 1;
		camera_id = CAM_FRONT;
		z_up_down = single_view_param.plane_expand_param.trans_y;
		x_left_right_shift = single_view_param.plane_expand_param.trans_x;
	}
	if (cam_flag == _1_VIEW_REAR)
	{
		single_view_param = p_avm_param->p_avm_config_param
			->avm_single_view_param.rear_single_view_param;
		p_lut = p_avm_param->p_avm_lut->p_lut_single_view_rear;
		is_front_cam = -1;
		camera_id = CAM_REAR;
		z_up_down = single_view_param.plane_expand_param.trans_y;
		x_left_right_shift = single_view_param.plane_expand_param.trans_x;
	}
	if (cam_flag == _1_VIEW_RIGHT)//�ҵ�ʹ��ǰ�����ò���
	{
		single_view_param = p_avm_param->p_avm_config_param
			->avm_single_view_param.front_single_view_param;
		p_lut = p_avm_param->p_avm_lut->p_lut_single_view_right;
		is_front_cam = -1;
		camera_id = CAM_RIGHT;
	}
	Float32_t fov_h = (single_view_param.plane_expand_param.fov_h*PI)/180.0;
	Float32_t focal_length = 1000; // unit is mm
	Float32_t fov_h1 = (Float32_t)(single_view_param.plane_expand_param.fov_h*3.14) / 180.0;
	Float32_t fov_v = (single_view_param.plane_expand_param.fov_v*PI) / 180;
	Int32_t result_width = single_view_param.result_image_width;
	Int32_t result_height = single_view_param.result_image_height;
	// ����չ��ͼ��ÿ�������ڸ߿����������ʵ�ʾ���
	Float32_t	mm_per_pixel_h = focal_length * tan(fov_h * 0.5) * 2 / result_width;
	Float32_t  mm_per_pixel_v = focal_length * tan(fov_v * 0.5) * 2 / result_height;
	Float32_t pose_t_0[3];
	Float32_t pose_t[3];
	Float32_t inv_pose[3];
	
	for (int i = 3; i < 6; i++)
	{
		inv_pose[i-3] = p_avm_param->p_avm_config_param
			->camera_model[camera_id].camera_model_ext.inv_pose[i];
	}
	p_avm_param->p_avm_config_param->camera_model[camera_id].camera_model_ext.inv_R;
	Mat_Mul_Vec_3by3(pose_t_0, p_avm_param->p_avm_config_param
		->camera_model[camera_id].camera_model_ext.R, inv_pose);
	pose_t[0] = -pose_t_0[0];
	pose_t[1] = -pose_t_0[1];
	pose_t[2] = -pose_t_0[2];//�������������ϵ��λ��
	Float32_t pitch_angle  ;//�˴�����������¸�����
	if (cam_flag == _1_VIEW_FRONT)
	{
		pitch_angle = (Float32_t)(TILT_ANGLE_F* PI) / 180;//�˴�����������¸�����16 

	}
	if (cam_flag == _1_VIEW_REAR)
	{
		pitch_angle = (Float32_t)(TILT_ANGLE_B * PI) / 180;//�˴�����������¸�����28
	}
	if (cam_flag == _1_VIEW_RIGHT)
	{
		pitch_angle = (0 * PI) / 180;//�˴�����������¸�����
	}
	Float32_t init_world_coord_x = pose_t[0] - is_front_cam 
		* mm_per_pixel_h * result_width * 0.5 + x_left_right_shift;
	Float32_t init_world_coord_y = pose_t[1] + is_front_cam 
		* (focal_length * cos(pitch_angle) 
		+ mm_per_pixel_v * result_height * 0.5 * sin(pitch_angle));
	Float32_t init_world_coord_z = pose_t[2] - focal_length * sin(pitch_angle) 
		+ mm_per_pixel_v * result_height * 0.5 * cos(pitch_angle) + z_up_down;
	Float32_t delta_x = mm_per_pixel_h;
	Float32_t delta_y = mm_per_pixel_v * sin(pitch_angle);
	Float32_t delta_z = mm_per_pixel_v * cos(pitch_angle);
	Float32_t world_coordinate[3];
	Float32_t image_point[2];

	if (cam_flag == _1_VIEW_FRONT)
	{
		for (int i = 0; i < single_view_param.result_image_height; i++)
		{
			for (int j = 0; j < single_view_param.result_image_width; j++)
			{
				world_coordinate[0] = init_world_coord_x + is_front_cam * j * delta_x;
				world_coordinate[1] = init_world_coord_y - is_front_cam * i * delta_y;
				world_coordinate[2] = init_world_coord_z - i * delta_z;
				world_coordinate_to_pic_coordinate(camera_id,
					world_coordinate, image_point, p_avm_param);//0 i
				UInt32_t  shift_altogether
					= i*single_view_param.result_image_width * 2 + j * 2;
				/*ret=single_view_zip_data(shift_altogether,
				image_point, p_lut, camera_id);*/
				zip_data_new(shift_altogether, image_point, 1,
					p_lut, NULL, NULL, p_avm_param);//Ϊ�˴������ã�����ͼ��ʹû���ں�Ȩ��Ҳʹ����·ȫ���洢��ʽ�ò����Ĵ洢0
				CHECK_ERROR(ret);
			}
		}
	}

	if (cam_flag == _1_VIEW_REAR)
	{
		for (int i = 0; i < single_view_param.result_image_height; i++)
		{
			for (int j = 0; j < single_view_param.result_image_width; j++)
			{
				world_coordinate[0] = init_world_coord_x + is_front_cam * (single_view_param.result_image_width-j) * delta_x;
				world_coordinate[1] = init_world_coord_y - is_front_cam * i * delta_y;
				world_coordinate[2] = init_world_coord_z - i * delta_z;
				world_coordinate_to_pic_coordinate(camera_id,
					world_coordinate, image_point, p_avm_param);//0 i
				UInt32_t  shift_altogether
					= i*single_view_param.result_image_width * 2 + j * 2;
				/*ret=single_view_zip_data(shift_altogether,
					image_point, p_lut, camera_id);*/
				zip_data_new(shift_altogether, image_point, 1,
					p_lut, NULL, NULL, p_avm_param);//Ϊ�˴������ã�����ͼ��ʹû���ں�Ȩ��Ҳʹ����·ȫ���洢��ʽ�ò����Ĵ洢0
				CHECK_ERROR(ret);
			}
		}
	}




	return ret;
}
#endif



#if 1
/*************SLAM_3*plane_front_rear_single_view***********************/
/*
* Function Name:				plane_front_rear_single_view
* Function Function:			plane_front_rear_single_view
* Input:
*       p_avm_param;
*		cam_flag;					 this is picture's flag is front or back
* Note:
*     the interface function should be called at the beginning of this module
* Revision:
*     Create by Cheng.Li on 2018/1/30.
*/
AVM_Error_Code_E  SLAM_3_plane_front_rear_single_view(IN P_Avm_Param_S p_avm_param, IN  int cam_flag)
{

	AVM_Error_Code_E ret = ERROR_NONE;
	Front_Rear_Single_View_Param_S single_view_param;
	UInt32_t* p_lut = NULL;
	Int32_t is_front_cam;
	UInt32_t camera_id;
	Float32_t z_up_down, x_left_right_shift;
	if (cam_flag == _1_VIEW_FRONT)
	{
		single_view_param = p_avm_param->p_avm_config_param
			->avm_single_view_param.front_single_view_param;
		p_lut = p_avm_param->p_avm_lut->p_lut_single_view_front;
		is_front_cam = 1;
		camera_id = CAM_FRONT;
		z_up_down = single_view_param.plane_expand_param.trans_y;
		x_left_right_shift = single_view_param.plane_expand_param.trans_x;
	}
	if (cam_flag == _1_VIEW_REAR)
	{
		single_view_param = p_avm_param->p_avm_config_param
			->avm_single_view_param.rear_single_view_param;
		p_lut = p_avm_param->p_avm_lut->p_lut_single_view_rear;
		is_front_cam = -1;
		camera_id = CAM_REAR;
		z_up_down = single_view_param.plane_expand_param.trans_y;
		x_left_right_shift = single_view_param.plane_expand_param.trans_x;
	}
	if (cam_flag == _1_VIEW_RIGHT)//�ҵ�ʹ��ǰ�����ò���
	{
		single_view_param = p_avm_param->p_avm_config_param
			->avm_single_view_param.front_single_view_param;
		p_lut = p_avm_param->p_avm_lut->p_lut_single_view_right;
		is_front_cam = 1;
		camera_id = CAM_RIGHT;
	}
	Float32_t fov_h = (single_view_param.plane_expand_param.fov_h*PI) / 180.0;
	Float32_t focal_length = 1000; // unit is mm
	Float32_t fov_h1 = (Float32_t)(single_view_param.plane_expand_param.fov_h*3.14) / 180.0;
	Float32_t fov_v = (single_view_param.plane_expand_param.fov_v*PI) / 180;
	Int32_t result_width = single_view_param.result_image_width;
	Int32_t result_height = single_view_param.result_image_height;
	// ����չ��ͼ��ÿ�������ڸ߿����������ʵ�ʾ���
	Float32_t	mm_per_pixel_h = focal_length * tan(fov_h * 0.5) * 2 / result_width;
	Float32_t  mm_per_pixel_v = focal_length * tan(fov_v * 0.5) * 2 / result_height;
	Float32_t pose_t_0[3];
	Float32_t pose_t[3];
	Float32_t inv_pose[3];

	pose_t[0] = p_avm_param->p_avm_config_param->camera_model[camera_id].camera_model_ext.pose[3];
	pose_t[1] = p_avm_param->p_avm_config_param->camera_model[camera_id].camera_model_ext.pose[4];
	pose_t[2] = p_avm_param->p_avm_config_param->camera_model[camera_id].camera_model_ext.pose[5];//�������������ϵ��λ��
	Float32_t pitch_angle = 0;//�˴�����������¸�����

	Float32_t init_world_coord_x = pose_t[0] - is_front_cam
		* mm_per_pixel_h * result_width * 0.5 + x_left_right_shift;
	Float32_t init_world_coord_y = pose_t[1] + is_front_cam
		* (focal_length * cos(pitch_angle)
		+ mm_per_pixel_v * result_height * 0.5 * sin(pitch_angle));
	Float32_t init_world_coord_z = pose_t[2] - focal_length * sin(pitch_angle)
		+ mm_per_pixel_v * result_height * 0.5 * cos(pitch_angle) + z_up_down;
	Float32_t delta_x = mm_per_pixel_h;
	Float32_t delta_y = mm_per_pixel_v * sin(pitch_angle);
	Float32_t delta_z = mm_per_pixel_v * cos(pitch_angle);
	Float32_t world_coordinate[3];
	Float32_t image_point[2];


	if (cam_flag == _1_VIEW_FRONT)
	{
		Float32_t all_long = 5000, z_long = 1500,angle_0=65;
		Float32_t angle_pi = (angle_0 * PI) / 180.0;
		init_world_coord_x = p_avm_param->p_avm_config_param->vehicle_param.vehicle_width / 2;
		init_world_coord_y = p_avm_param->p_avm_config_param->vehicle_param.vehicle_length / 2 ;
		init_world_coord_z = z_long;

		float delta = all_long / single_view_param.result_image_width;
		delta_z = z_long / single_view_param.result_image_height;
		for (int i = 0; i < single_view_param.result_image_height; i++)
		{
			for (int j = 0; j < single_view_param.result_image_width; j++)
			{
				world_coordinate[0] = init_world_coord_x + j * delta*cos(angle_pi);
				world_coordinate[1] = init_world_coord_y + j * delta*sin(angle_pi);
				world_coordinate[2] = init_world_coord_z - i * delta_z;
				world_coordinate_to_pic_coordinate(camera_id,
					world_coordinate, image_point, p_avm_param);//0 i
				UInt32_t  shift_altogether
					= i*single_view_param.result_image_width * 2 + j * 2;
				/*ret=single_view_zip_data(shift_altogether,
				image_point, p_lut, camera_id);*/
				zip_data_new(shift_altogether, image_point, 1,
					p_lut, NULL, NULL, p_avm_param);//Ϊ�˴������ã�����ͼ��ʹû���ں�Ȩ��Ҳʹ����·ȫ���洢��ʽ�ò����Ĵ洢0
				CHECK_ERROR(ret);
			}
		}
	}
	printf("wodebababbaoyoumiaozishentudilaobaoyou");
	if (cam_flag == _1_VIEW_RIGHT)//�ҵ�ʹ��ǰ�����ò���
	{
		
		Float32_t all_long = 5000, z_long = 1500, angle_0 = 25;
		Float32_t angle_pi = (angle_0 * PI) / 180.0;
		init_world_coord_x = p_avm_param->p_avm_config_param->vehicle_param.vehicle_width / 2;
		init_world_coord_y = p_avm_param->p_avm_config_param->vehicle_param.vehicle_length / 2;
		init_world_coord_z = z_long;

		float delta = all_long / single_view_param.result_image_width;
		delta_z = z_long / single_view_param.result_image_height;
		for (int i = 0; i < single_view_param.result_image_height; i++)
		{
			for (int j = 0; j < single_view_param.result_image_width; j++)
			{
				world_coordinate[0] = init_world_coord_x + j * delta*cos(angle_pi);
				world_coordinate[1] = init_world_coord_y + j * delta*sin(angle_pi);
				world_coordinate[2] = init_world_coord_z - i * delta_z;
				world_coordinate_to_pic_coordinate(camera_id,
					world_coordinate, image_point, p_avm_param);//0 i
				UInt32_t  shift_altogether
					= i*single_view_param.result_image_width * 2 + j * 2;
				/*ret=single_view_zip_data(shift_altogether,
				image_point, p_lut, camera_id);*/
				zip_data_new(shift_altogether, image_point, 1,
					p_lut, NULL, NULL, p_avm_param);//Ϊ�˴������ã�����ͼ��ʹû���ں�Ȩ��Ҳʹ����·ȫ���洢��ʽ�ò����Ĵ洢0
				CHECK_ERROR(ret);
			}
		}




	}
	if (cam_flag == _1_VIEW_REAR)
	{
		for (int i = 0; i < single_view_param.result_image_height; i++)
		{
			for (int j = 0; j < single_view_param.result_image_width; j++)
			{
				world_coordinate[0] = init_world_coord_x + is_front_cam * (single_view_param.result_image_width - j) * delta_x;
				world_coordinate[1] = init_world_coord_y - is_front_cam * i * delta_y;
				world_coordinate[2] = init_world_coord_z - i * delta_z;
				world_coordinate_to_pic_coordinate(camera_id,
					world_coordinate, image_point, p_avm_param);//0 i
				UInt32_t  shift_altogether
					= i*single_view_param.result_image_width * 2 + j * 2;
				/*ret=single_view_zip_data(shift_altogether,
				image_point, p_lut, camera_id);*/
				zip_data_new(shift_altogether, image_point, 1,
					p_lut, NULL, NULL, p_avm_param);//Ϊ�˴������ã�����ͼ��ʹû���ں�Ȩ��Ҳʹ����·ȫ���洢��ʽ�ò����Ĵ洢0
				CHECK_ERROR(ret);
			}
		}
	}




	return ret;




}
#endif



#if 1
/***********SLAMƽ��չ��***plane_front_rear_single_view***********************/
/*
* Function Name:				plane_front_rear_single_view
* Function Function:			plane_front_rear_single_view
* Input:
*       p_avm_param;
*		cam_flag;					 this is picture's flag is front or back
* Note:
*     the interface function should be called at the beginning of this module
* Revision:
*     Create by Cheng.Li on 2018/1/30.
*/
AVM_Error_Code_E  SLAM_plane_front_rear_single_view(IN P_Avm_Param_S p_avm_param, IN  int cam_flag)
{
	AVM_Error_Code_E ret = ERROR_NONE;
	Front_Rear_Single_View_Param_S single_view_param;
	UInt32_t* p_lut = NULL;
	Int32_t is_front_cam;
	UInt32_t camera_id;
	Float32_t z_up_down, x_left_right_shift;
	if (cam_flag == _1_VIEW_FRONT)
	{
		single_view_param = p_avm_param->p_avm_config_param
			->avm_single_view_param.front_single_view_param;
		p_lut = p_avm_param->p_avm_lut->p_lut_single_view_front;
		is_front_cam = 1;
		camera_id = CAM_FRONT;
		z_up_down = single_view_param.plane_expand_param.trans_y;
		x_left_right_shift = single_view_param.plane_expand_param.trans_x;
	}
	if (cam_flag == _1_VIEW_REAR)
	{
		single_view_param = p_avm_param->p_avm_config_param
			->avm_single_view_param.rear_single_view_param;
		p_lut = p_avm_param->p_avm_lut->p_lut_single_view_rear;
		is_front_cam = -1;
		camera_id = CAM_REAR;
		z_up_down = single_view_param.plane_expand_param.trans_y;
		x_left_right_shift = single_view_param.plane_expand_param.trans_x;
	}
	if (cam_flag == _1_VIEW_RIGHT)//�ҵ�ʹ��ǰ�����ò���
	{
		single_view_param = p_avm_param->p_avm_config_param
			->avm_single_view_param.front_single_view_param;
		p_lut = p_avm_param->p_avm_lut->p_lut_single_view_right;
		is_front_cam = 1;
		camera_id = CAM_RIGHT;
	}
	Float32_t fov_h = (single_view_param.plane_expand_param.fov_h*PI) / 180.0;
	Float32_t focal_length = 1000; // unit is mm
	Float32_t fov_h1 = (Float32_t)(single_view_param.plane_expand_param.fov_h*3.14) / 180.0;
	Float32_t fov_v = (single_view_param.plane_expand_param.fov_v*PI) / 180;
	Int32_t result_width = single_view_param.result_image_width;
	Int32_t result_height = single_view_param.result_image_height;
	// ����չ��ͼ��ÿ�������ڸ߿����������ʵ�ʾ���
	Float32_t	mm_per_pixel_h = focal_length * tan(fov_h * 0.5) * 2 / result_width;
	Float32_t  mm_per_pixel_v = focal_length * tan(fov_v * 0.5) * 2 / result_height;
	Float32_t pose_t_0[3];
	Float32_t pose_t[3];
	Float32_t inv_pose[3];

	pose_t[0] = p_avm_param->p_avm_config_param->camera_model[camera_id].camera_model_ext.pose[3];
	pose_t[1] = p_avm_param->p_avm_config_param->camera_model[camera_id].camera_model_ext.pose[4];
	pose_t[2] = p_avm_param->p_avm_config_param->camera_model[camera_id].camera_model_ext.pose[5];//�������������ϵ��λ��
	Float32_t pitch_angle=0;//�˴�����������¸�����
	
	Float32_t init_world_coord_x = pose_t[0] - is_front_cam
		* mm_per_pixel_h * result_width * 0.5 + x_left_right_shift;
	Float32_t init_world_coord_y = pose_t[1] + is_front_cam
		* (focal_length * cos(pitch_angle)
		+ mm_per_pixel_v * result_height * 0.5 * sin(pitch_angle));
	Float32_t init_world_coord_z = pose_t[2] - focal_length * sin(pitch_angle)
		+ mm_per_pixel_v * result_height * 0.5 * cos(pitch_angle) + z_up_down;
	Float32_t delta_x = mm_per_pixel_h;
	Float32_t delta_y = mm_per_pixel_v * sin(pitch_angle);
	Float32_t delta_z = mm_per_pixel_v * cos(pitch_angle);
	Float32_t world_coordinate[3];
	Float32_t image_point[2];

	
	if (cam_flag == _1_VIEW_FRONT)
	{
		Float32_t y_long = 3000, z_long = 1500;
		init_world_coord_x = p_avm_param->p_avm_config_param->vehicle_param.vehicle_width / 2;
		init_world_coord_y = p_avm_param->p_avm_config_param->vehicle_param.vehicle_length / 2 + y_long;
		init_world_coord_z = z_long;

		delta_y = y_long / single_view_param.result_image_width;
		delta_z = z_long / single_view_param.result_image_height;
		for (int i = 0; i < single_view_param.result_image_height; i++)
		{
			for (int j = 0; j < single_view_param.result_image_width; j++)
			{
				world_coordinate[0] = init_world_coord_x ;
				world_coordinate[1] = init_world_coord_y - is_front_cam * j * delta_y;
				world_coordinate[2] = init_world_coord_z - i * delta_z;
				world_coordinate_to_pic_coordinate(camera_id,
					world_coordinate, image_point, p_avm_param);//0 i
				UInt32_t  shift_altogether
					= i*single_view_param.result_image_width * 2 + j * 2;
				/*ret=single_view_zip_data(shift_altogether,
				image_point, p_lut, camera_id);*/
				zip_data_new(shift_altogether, image_point, 1,
					p_lut, NULL, NULL, p_avm_param);//Ϊ�˴������ã�����ͼ��ʹû���ں�Ȩ��Ҳʹ����·ȫ���洢��ʽ�ò����Ĵ洢0
				CHECK_ERROR(ret);
			}
		}
	}
	if (cam_flag == _1_VIEW_RIGHT)//�ҵ�ʹ��ǰ�����ò���
	{
		Float32_t x_long = 3000, z_long = 2000;
		init_world_coord_x = p_avm_param->p_avm_config_param->vehicle_param.vehicle_width / 2;
		init_world_coord_y = p_avm_param->p_avm_config_param->vehicle_param.vehicle_length / 2 ;
		init_world_coord_z = z_long;

		delta_x = x_long / single_view_param.result_image_width;
		delta_z = z_long / single_view_param.result_image_height;
		for (int i = 0; i < single_view_param.result_image_height; i++)
		{
			for (int j = 0; j < single_view_param.result_image_width; j++)
			{
				world_coordinate[0] = init_world_coord_x + is_front_cam * j * delta_x;
				world_coordinate[1] = init_world_coord_y ;
				world_coordinate[2] = init_world_coord_z - i * delta_z;
				world_coordinate_to_pic_coordinate(camera_id,
					world_coordinate, image_point, p_avm_param);//0 i
				UInt32_t  shift_altogether
					= i*single_view_param.result_image_width * 2 + j * 2;
				/*ret=single_view_zip_data(shift_altogether,
				image_point, p_lut, camera_id);*/
				zip_data_new(shift_altogether, image_point, 1,
					p_lut, NULL, NULL, p_avm_param);//Ϊ�˴������ã�����ͼ��ʹû���ں�Ȩ��Ҳʹ����·ȫ���洢��ʽ�ò����Ĵ洢0
				CHECK_ERROR(ret);
			}
		}





	}
	if (cam_flag == _1_VIEW_REAR)
	{
		for (int i = 0; i < single_view_param.result_image_height; i++)
		{
			for (int j = 0; j < single_view_param.result_image_width; j++)
			{
				world_coordinate[0] = init_world_coord_x + is_front_cam * (single_view_param.result_image_width - j) * delta_x;
				world_coordinate[1] = init_world_coord_y - is_front_cam * i * delta_y;
				world_coordinate[2] = init_world_coord_z - i * delta_z;
				world_coordinate_to_pic_coordinate(camera_id,
					world_coordinate, image_point, p_avm_param);//0 i
				UInt32_t  shift_altogether
					= i*single_view_param.result_image_width * 2 + j * 2;
				/*ret=single_view_zip_data(shift_altogether,
				image_point, p_lut, camera_id);*/
				zip_data_new(shift_altogether, image_point, 1,
					p_lut, NULL, NULL, p_avm_param);//Ϊ�˴������ã�����ͼ��ʹû���ں�Ȩ��Ҳʹ����·ȫ���洢��ʽ�ò����Ĵ洢0
				CHECK_ERROR(ret);
			}
		}
	}




	return ret;
}
#endif


#if 1
/***********SLAMƽ��չ��***plane_front_rear_single_view***********************/
/*
* Function Name:				plane_front_rear_single_view
* Function Function:			plane_front_rear_single_view
* Input:
*       p_avm_param;
*		cam_flag;					 this is picture's flag is front or back
* Note:
*     the interface function should be called at the beginning of this module
* Revision:
*     Create by Cheng.Li on 2018/1/30.
*/
AVM_Error_Code_E  SLAM_2_plane_front_rear_single_view(IN P_Avm_Param_S p_avm_param, IN  int cam_flag)
{
	AVM_Error_Code_E ret = ERROR_NONE;
	Front_Rear_Single_View_Param_S single_view_param;
	UInt32_t* p_lut = NULL;
	Int32_t is_front_cam;
	UInt32_t camera_id;
	Float32_t z_up_down, x_left_right_shift;
	if (cam_flag == _1_VIEW_FRONT)
	{
		single_view_param = p_avm_param->p_avm_config_param
			->avm_single_view_param.front_single_view_param;
		p_lut = p_avm_param->p_avm_lut->p_lut_single_view_front;
		is_front_cam = 1;
		camera_id = CAM_FRONT;
		z_up_down = single_view_param.plane_expand_param.trans_y;
		x_left_right_shift = single_view_param.plane_expand_param.trans_x;
	}
	if (cam_flag == _1_VIEW_REAR)
	{
		single_view_param = p_avm_param->p_avm_config_param
			->avm_single_view_param.rear_single_view_param;
		p_lut = p_avm_param->p_avm_lut->p_lut_single_view_rear;
		is_front_cam = -1;
		camera_id = CAM_REAR;
		z_up_down = single_view_param.plane_expand_param.trans_y;
		x_left_right_shift = single_view_param.plane_expand_param.trans_x;
	}
	if (cam_flag == _1_VIEW_RIGHT)//�ҵ�ʹ��ǰ�����ò���
	{
		single_view_param = p_avm_param->p_avm_config_param
			->avm_single_view_param.front_single_view_param;
		p_lut = p_avm_param->p_avm_lut->p_lut_single_view_right;
		is_front_cam = 1;
		camera_id = CAM_RIGHT;
	}
	Float32_t fov_h = (single_view_param.plane_expand_param.fov_h*PI) / 180.0;
	Float32_t focal_length = 1000; // unit is mm
	Float32_t fov_h1 = (Float32_t)(single_view_param.plane_expand_param.fov_h*3.14) / 180.0;
	Float32_t fov_v = (single_view_param.plane_expand_param.fov_v*PI) / 180;
	Int32_t result_width = single_view_param.result_image_width;
	Int32_t result_height = single_view_param.result_image_height;
	// ����չ��ͼ��ÿ�������ڸ߿����������ʵ�ʾ���
	Float32_t	mm_per_pixel_h = focal_length * tan(fov_h * 0.5) * 2 / result_width;
	Float32_t  mm_per_pixel_v = focal_length * tan(fov_v * 0.5) * 2 / result_height;
	Float32_t pose_t_0[3];
	Float32_t pose_t[3];
	Float32_t inv_pose[3];

	pose_t[0] = p_avm_param->p_avm_config_param->camera_model[camera_id].camera_model_ext.pose[3];
	pose_t[1] = p_avm_param->p_avm_config_param->camera_model[camera_id].camera_model_ext.pose[4];
	pose_t[2] = p_avm_param->p_avm_config_param->camera_model[camera_id].camera_model_ext.pose[5];//�������������ϵ��λ��
	Float32_t pitch_angle = 0;//�˴�����������¸�����

	Float32_t init_world_coord_x = pose_t[0] - is_front_cam
		* mm_per_pixel_h * result_width * 0.5 + x_left_right_shift;
	Float32_t init_world_coord_y = pose_t[1] + is_front_cam
		* (focal_length * cos(pitch_angle)
		+ mm_per_pixel_v * result_height * 0.5 * sin(pitch_angle));
	Float32_t init_world_coord_z = pose_t[2] - focal_length * sin(pitch_angle)
		+ mm_per_pixel_v * result_height * 0.5 * cos(pitch_angle) + z_up_down;
	Float32_t delta_x = mm_per_pixel_h;
	Float32_t delta_y = mm_per_pixel_v * sin(pitch_angle);
	Float32_t delta_z = mm_per_pixel_v * cos(pitch_angle);
	Float32_t world_coordinate[3], world_coordinate_temp[3];
	Float32_t image_point[2];

	Float32_t centre_rotate_coordinate[3];
	Float32_t d_angle = (45*PI) / 180.0;
	if (cam_flag == _1_VIEW_FRONT)
	{
		Float32_t y_long = 3000, z_long = 1500;
		init_world_coord_x = p_avm_param->p_avm_config_param->vehicle_param.vehicle_width / 2;
		init_world_coord_y = p_avm_param->p_avm_config_param->vehicle_param.vehicle_length / 2 + y_long;
		init_world_coord_z = z_long;

		delta_y = y_long / single_view_param.result_image_width;
		delta_z = z_long / single_view_param.result_image_height;
		for (int i = 0; i < single_view_param.result_image_height; i++)
		{
			centre_rotate_coordinate[0] = init_world_coord_x;
			centre_rotate_coordinate[1] = p_avm_param->p_avm_config_param->vehicle_param.vehicle_length / 2;
			for (int j = 0; j < single_view_param.result_image_width; j++)
			{
				world_coordinate_temp[0] = init_world_coord_x;
				world_coordinate_temp[1] = init_world_coord_y - is_front_cam * j * delta_y;
				world_coordinate_temp[2] = init_world_coord_z - i * delta_z;


				if (cam_flag == _1_VIEW_FRONT)
				{
					world_coordinate[0] = (world_coordinate_temp[0] - centre_rotate_coordinate[0])*cos(-j*d_angle) - (world_coordinate_temp[1] - centre_rotate_coordinate[1])*sin(-j*d_angle)
						+ centre_rotate_coordinate[0];///is x
					world_coordinate[1] = (world_coordinate_temp[0] - centre_rotate_coordinate[0])*sin(-j*d_angle)
						+ (world_coordinate_temp[1] - centre_rotate_coordinate[1])*cos(-j*d_angle)
						+ centre_rotate_coordinate[1];//is y
					world_coordinate[2] = init_world_coord_z - i * delta_z;

				}








				world_coordinate_to_pic_coordinate(camera_id,
					world_coordinate, image_point, p_avm_param);//0 i
				UInt32_t  shift_altogether
					= i*single_view_param.result_image_width * 2 + j * 2;
				/*ret=single_view_zip_data(shift_altogether,
				image_point, p_lut, camera_id);*/
				zip_data_new(shift_altogether, image_point, 1,
					p_lut, NULL, NULL, p_avm_param);//Ϊ�˴������ã�����ͼ��ʹû���ں�Ȩ��Ҳʹ����·ȫ���洢��ʽ�ò����Ĵ洢0
				CHECK_ERROR(ret);
			}
		}
	}
	if (cam_flag == _1_VIEW_RIGHT)//�ҵ�ʹ��ǰ�����ò���
	{
		Float32_t x_long = 3000, z_long = 2000;
		init_world_coord_x = p_avm_param->p_avm_config_param->vehicle_param.vehicle_width / 2;
		init_world_coord_y = p_avm_param->p_avm_config_param->vehicle_param.vehicle_length / 2;
		init_world_coord_z = z_long;

		delta_x = x_long / single_view_param.result_image_width;
		delta_z = z_long / single_view_param.result_image_height;
		for (int i = 0; i < single_view_param.result_image_height; i++)
		{
			for (int j = 0; j < single_view_param.result_image_width; j++)
			{
				world_coordinate[0] = init_world_coord_x + is_front_cam * j * delta_x;
				world_coordinate[1] = init_world_coord_y;
				world_coordinate[2] = init_world_coord_z - i * delta_z;
				world_coordinate_to_pic_coordinate(camera_id,
					world_coordinate, image_point, p_avm_param);//0 i
				UInt32_t  shift_altogether
					= i*single_view_param.result_image_width * 2 + j * 2;
				/*ret=single_view_zip_data(shift_altogether,
				image_point, p_lut, camera_id);*/
				zip_data_new(shift_altogether, image_point, 1,
					p_lut, NULL, NULL, p_avm_param);//Ϊ�˴������ã�����ͼ��ʹû���ں�Ȩ��Ҳʹ����·ȫ���洢��ʽ�ò����Ĵ洢0
				CHECK_ERROR(ret);
			}
		}





	}
	if (cam_flag == _1_VIEW_REAR)
	{
		for (int i = 0; i < single_view_param.result_image_height; i++)
		{
			for (int j = 0; j < single_view_param.result_image_width; j++)
			{
				world_coordinate[0] = init_world_coord_x + is_front_cam * (single_view_param.result_image_width - j) * delta_x;
				world_coordinate[1] = init_world_coord_y - is_front_cam * i * delta_y;
				world_coordinate[2] = init_world_coord_z - i * delta_z;
				world_coordinate_to_pic_coordinate(camera_id,
					world_coordinate, image_point, p_avm_param);//0 i
				UInt32_t  shift_altogether
					= i*single_view_param.result_image_width * 2 + j * 2;
				/*ret=single_view_zip_data(shift_altogether,
				image_point, p_lut, camera_id);*/
				zip_data_new(shift_altogether, image_point, 1,
					p_lut, NULL, NULL, p_avm_param);//Ϊ�˴������ã�����ͼ��ʹû���ں�Ȩ��Ҳʹ����·ȫ���洢��ʽ�ò����Ĵ洢0
				CHECK_ERROR(ret);
			}
		}
	}




	return ret;
}
#endif



#if 1
/**************plane_front_rear_single_view****z�������������չ��*******************/
/*
* Function Name:				plane_front_rear_single_view
* Function Function:			plane_front_rear_single_view
* Input:
*       p_avm_param;
*		cam_flag;					 this is picture's flag is front or back
* Note:
*     the interface function should be called at the beginning of this module
* Revision:
*     Create by Cheng.Li on 2018/1/30.
*/
AVM_Error_Code_E  column_world_coord_left_right_single_view(IN P_Avm_Param_S p_avm_param, IN  int cam_flag)
{
	AVM_Error_Code_E ret = ERROR_NONE;
	Front_Rear_Single_View_Param_S single_view_param;
	UInt32_t* p_lut = NULL;
	Int32_t is_front_cam;
	UInt32_t camera_id;
	Float32_t z_up_down;
	if (cam_flag == _1_VIEW_FRONT)
	{
		single_view_param = p_avm_param->p_avm_config_param
			->avm_single_view_param.front_single_view_param;
		p_lut = p_avm_param->p_avm_lut->p_lut_single_view_front;
		is_front_cam = 1;
		camera_id = CAM_FRONT;
		z_up_down = single_view_param.plane_expand_param.trans_y;
		single_view_param.plane_expand_param.fov_h = 160;
		single_view_param.plane_expand_param.fov_v = 100;
	}
	if (cam_flag == _1_VIEW_REAR)
	{
		single_view_param = p_avm_param->p_avm_config_param
			->avm_single_view_param.rear_single_view_param;
		p_lut = p_avm_param->p_avm_lut->p_lut_single_view_rear;
		is_front_cam = -1;
		camera_id = CAM_REAR;
		z_up_down = single_view_param.plane_expand_param.trans_y;
		single_view_param.plane_expand_param.fov_h = 160;
		single_view_param.plane_expand_param.fov_v = 100;
	}
	if (cam_flag == _1_VIEW_RIGHT)//�ҵ�ʹ��ǰ�����ò���
	{
		single_view_param = p_avm_param->p_avm_config_param
			->avm_single_view_param.front_single_view_param;
		p_lut = p_avm_param->p_avm_lut->p_lut_single_view_right;
		is_front_cam = -1;
		z_up_down = -600;
		camera_id = CAM_RIGHT;
		single_view_param.plane_expand_param.fov_h = 176;
		single_view_param.plane_expand_param.fov_v = 100;
	}
	if (cam_flag == _1_VIEW_LEFT)
	{
		single_view_param = p_avm_param->p_avm_config_param
			->avm_single_view_param.front_single_view_param;
		p_lut = p_avm_param->p_avm_lut->p_lut_single_view_left;
		is_front_cam = -1;
		camera_id = CAM_LEFT;
		z_up_down = 600;
	}
	Int32_t *distance = (Int32_t *)malloc(single_view_param.result_image_width*single_view_param.result_image_height*sizeof(Int32_t)* 2);//s32Vǰ�����ұ���Գ�




	
	z_up_down = z_up_down - 320;
	Float32_t fov_h = (single_view_param.plane_expand_param.fov_h*PI) / 180.0;
	Float32_t focal_length = 1000; // unit is mm
	//Float32_t fov_h1 = (Float32_t)(single_view_param.plane_expand_param.fov_h*3.14) / 180.0;
	Float32_t fov_v = (single_view_param.plane_expand_param.fov_v*PI) / 180;
	Int32_t result_width = single_view_param.result_image_width;
	Int32_t result_height = single_view_param.result_image_height;
	// ����չ��ͼ��ÿ�������ڸ߿����������ʵ�ʾ���
	//Float32_t	mm_per_pixel_h = focal_length * tan(fov_h * 0.5) * 2 / result_width;

	Float32_t  mm_per_pixel_v = focal_length * tan(fov_v * 0.5) * 2 / result_height;
	Float32_t pose_t_0[3];
	Float32_t pose_t[3];
	Float32_t inv_pose[3];

	//for (int i = 3; i < 6; i++)
	//{
	//	inv_pose[i - 3] = p_avm_param->p_avm_config_param
	//		->camera_model[camera_id].camera_model_ext.inv_pose[i];
	//}
	//p_avm_param->p_avm_config_param->camera_model[camera_id].camera_model_ext.inv_R;
	//Mat_Mul_Vec_3by3(pose_t_0, p_avm_param->p_avm_config_param
	//	->camera_model[camera_id].camera_model_ext.R, inv_pose);
	//pose_t[0] = -pose_t_0[0];
	//pose_t[1] = -pose_t_0[1];
	//pose_t[2] = -pose_t_0[2];//�������������ϵ��λ��

	pose_t[0] = p_avm_param->p_avm_config_param->camera_model[camera_id].camera_model_ext.pose[3];
	pose_t[1] = p_avm_param->p_avm_config_param->camera_model[camera_id].camera_model_ext.pose[4];
	pose_t[2] = p_avm_param->p_avm_config_param->camera_model[camera_id].camera_model_ext.pose[5];//�������������ϵ��λ��
	Float32_t pitch_angle;//�˴�����������¸�����
	if (cam_flag == _1_VIEW_FRONT)
	{
		pitch_angle = (Float32_t)(0 * PI) / 180;//�˴�����������¸�����16 

	}
	if (cam_flag == _1_VIEW_REAR)
	{
		pitch_angle = (Float32_t)(0 * PI) / 180;//�˴�����������¸�����28
	}
	if (cam_flag == _1_VIEW_LEFT)
	{
		pitch_angle = (Float32_t)(0 * PI) / 180;//�˴�����������¸�����28
	}
	if (cam_flag == _1_VIEW_RIGHT)
	{
		pitch_angle = (0 * PI) / 180;//�˴�����������¸�����
	}


	Float32_t delta_z = mm_per_pixel_v ;
	Float32_t world_coordinate[3];
	Float32_t image_point[2];
	Float32_t centre_x = pose_t[0], centre_y = pose_t[1];// centre_z = pose_t[2],
	Float32_t init_world_coord_x, init_world_coord_y, init_world_coord_z;
	if (cam_flag == _1_VIEW_REAR)
	{
		init_world_coord_x = -focal_length*sin(fov_h * 0.5) - pose_t[0];
		init_world_coord_y = -focal_length*cos(fov_h * 0.5) + pose_t[1];
		init_world_coord_z = pose_t[2] - focal_length * sin(pitch_angle)
			+ mm_per_pixel_v * result_height * 0.5 * cos(pitch_angle) + z_up_down;//
	}
	if (cam_flag == _1_VIEW_FRONT)
	{
		init_world_coord_x = -focal_length*sin(fov_h * 0.5) - pose_t[0];
		init_world_coord_y = focal_length*cos(fov_h * 0.5) + pose_t[1];
		init_world_coord_z = pose_t[2] - focal_length * sin(pitch_angle)
			+ mm_per_pixel_v * result_height * 0.5 * cos(pitch_angle) + z_up_down;//
	}
	if (cam_flag == _1_VIEW_RIGHT)
	{
		init_world_coord_x = focal_length*cos(fov_h * 0.5) + pose_t[0];
		init_world_coord_y = focal_length*sin(fov_h * 0.5) + pose_t[1];
		init_world_coord_z = pose_t[2] + focal_length * tan(fov_v * 0.5) + z_up_down;//
	}


	Float32_t d_angle = fov_h / single_view_param.result_image_width;
	Float32_t centre_rotate_coordinate[3];
	for (int i = 0; i < single_view_param.result_image_height; i++)
	{
		centre_rotate_coordinate[0] = centre_x;
		centre_rotate_coordinate[1] = centre_y;
		for (int j = 0; j < single_view_param.result_image_width; j++)
		{
			if (cam_flag == _1_VIEW_RIGHT)
			{
				world_coordinate[0] = (init_world_coord_x - centre_rotate_coordinate[0])*cos(-j*d_angle) - (init_world_coord_y - centre_rotate_coordinate[1])*sin(-j*d_angle)
					+ centre_rotate_coordinate[0];///is x
				world_coordinate[1] = (init_world_coord_x - centre_rotate_coordinate[0])*sin(-j*d_angle)
					+ (init_world_coord_y - centre_rotate_coordinate[1])*cos(-j*d_angle)
					+ centre_rotate_coordinate[1];//is y
				world_coordinate[2] = init_world_coord_z - i * delta_z;
			}
			if (cam_flag == _1_VIEW_REAR)
			{
				world_coordinate[0] = (init_world_coord_x - centre_rotate_coordinate[0])*cos(j*d_angle) //������ת��ʽ
					- (init_world_coord_y - centre_rotate_coordinate[1])*sin(j*d_angle)
					+ centre_rotate_coordinate[0];///is x
				world_coordinate[1] = (init_world_coord_x - centre_rotate_coordinate[0])*sin(j*d_angle)
					+ (init_world_coord_y - centre_rotate_coordinate[1])*cos(j*d_angle)
					+ centre_rotate_coordinate[1];//is y
				world_coordinate[2] = init_world_coord_z - i * delta_z;


			}
			if (cam_flag == _1_VIEW_FRONT)
			{
				world_coordinate[0] = (init_world_coord_x - centre_rotate_coordinate[0])*cos(-j*d_angle) - (init_world_coord_y - centre_rotate_coordinate[1])*sin(-j*d_angle)
					+ centre_rotate_coordinate[0];///is x
				world_coordinate[1] = (init_world_coord_x - centre_rotate_coordinate[0])*sin(-j*d_angle)
					+ (init_world_coord_y - centre_rotate_coordinate[1])*cos(-j*d_angle)
					+ centre_rotate_coordinate[1];//is y
				world_coordinate[2] = init_world_coord_z - i * delta_z;

			}


			world_coordinate_to_pic_coordinate(camera_id,
				world_coordinate, image_point, p_avm_param);//0 i


			if (cam_flag == _1_VIEW_REAR)
			{
				Float32_t cam_ray[3];
				Float32_t world_ray[3];
				Image_Point_To_Cam_Ray(cam_ray,
					image_point,
					&(p_avm_param->p_avm_config_param
					->camera_model[camera_id].camera_model_int));
				Cam_Ray_To_World_Ray(world_ray,
					cam_ray, &(p_avm_param->p_avm_config_param
					->camera_model[camera_id].camera_model_ext));
				//distance[i*single_view_param.result_image_width * 2 + j * 2] = (Int32_t)world_ray[0];
				//distance[i*single_view_param.result_image_width * 2 + j * 2 + 1] = (Int32_t)world_ray[1];
			}




			UInt32_t  shift_altogether
				= i*single_view_param.result_image_width * 2 + j * 2;
			/*ret=single_view_zip_data(shift_altogether,
			image_point, p_lut, camera_id);*/
			zip_data_new(shift_altogether, image_point, 1,
				p_lut, NULL, NULL, p_avm_param);//Ϊ�˴������ã�����ͼ��ʹû���ں�Ȩ��Ҳʹ����·ȫ���洢��ʽ�ò����Ĵ洢0
			CHECK_ERROR(ret);
		}
	}

	char lut_path[MAX_ARRAY_SIZE];
	sprintf(lut_path, "%s/distance.bin", lut_file_path);
	FILE*gpu = fopen(lut_path, "wb");
	//fwrite(distance, sizeof(Int32_t), single_view_param.result_image_width*single_view_param.result_image_height * 2, gpu);
	fclose(gpu);


	return ret;
}
#endif


#if 1
/**************plane_front_rear_single_view****z�������������չ��*******************/
/*
* Function Name:				plane_front_rear_single_view
* Function Function:			plane_front_rear_single_view
* Input:
*       p_avm_param;
*		cam_flag;					 this is picture's flag is front or back
* Note:
*     the interface function should be called at the beginning of this module
* Revision:
*     Create by Cheng.Li on 2018/1/30.
*/
AVM_Error_Code_E  SLAM_5_column_world_coord_left_right_single_view(IN P_Avm_Param_S p_avm_param, IN  int cam_flag)
{
	AVM_Error_Code_E ret = ERROR_NONE;
	Front_Rear_Single_View_Param_S single_view_param;
	UInt32_t* p_lut = NULL;
	Int32_t is_front_cam;
	UInt32_t camera_id;
	Float32_t z_up_down;
	if (cam_flag == _1_VIEW_FRONT)
	{
		single_view_param = p_avm_param->p_avm_config_param
			->avm_single_view_param.front_single_view_param;
		p_lut = p_avm_param->p_avm_lut->p_lut_single_view_front;
		is_front_cam = 1;
		camera_id = CAM_FRONT;
		z_up_down = single_view_param.plane_expand_param.trans_y;
		single_view_param.plane_expand_param.fov_h = 160;
		single_view_param.plane_expand_param.fov_v = 100;
	}
	if (cam_flag == _1_VIEW_REAR)
	{
		single_view_param = p_avm_param->p_avm_config_param
			->avm_single_view_param.rear_single_view_param;
		p_lut = p_avm_param->p_avm_lut->p_lut_single_view_rear;
		is_front_cam = -1;
		camera_id = CAM_REAR;
		z_up_down = single_view_param.plane_expand_param.trans_y;
		single_view_param.plane_expand_param.fov_h = 160;
		single_view_param.plane_expand_param.fov_v = 100;
	}
	if (cam_flag == _1_VIEW_RIGHT)//�ҵ�ʹ��ǰ�����ò���
	{
		single_view_param = p_avm_param->p_avm_config_param
			->avm_single_view_param.front_single_view_param;
		p_lut = p_avm_param->p_avm_lut->p_lut_single_view_right;
		is_front_cam = -1;
		z_up_down = -600;
		camera_id = CAM_RIGHT;
		single_view_param.plane_expand_param.fov_h = 90;
		single_view_param.plane_expand_param.fov_v = 100;
	}
	if (cam_flag == _1_VIEW_LEFT)
	{
		single_view_param = p_avm_param->p_avm_config_param
			->avm_single_view_param.front_single_view_param;
		p_lut = p_avm_param->p_avm_lut->p_lut_single_view_left;
		is_front_cam = -1;
		camera_id = CAM_LEFT;
		z_up_down = 600;
	}
	Int32_t *distance = (Int32_t *)malloc(single_view_param.result_image_width*single_view_param.result_image_height
		*sizeof(Int32_t)* 2);//s32Vǰ�����ұ���Գ�





	z_up_down = z_up_down - 320;
	Float32_t fov_h = (single_view_param.plane_expand_param.fov_h*PI) / 180.0;
	Float32_t focal_length = 1000; // unit is mm
	//Float32_t fov_h1 = (Float32_t)(single_view_param.plane_expand_param.fov_h*3.14) / 180.0;
	Float32_t fov_v = (single_view_param.plane_expand_param.fov_v*PI) / 180;
	Int32_t result_width = single_view_param.result_image_width;
	Int32_t result_height = single_view_param.result_image_height;
	// ����չ��ͼ��ÿ�������ڸ߿����������ʵ�ʾ���
	//Float32_t	mm_per_pixel_h = focal_length * tan(fov_h * 0.5) * 2 / result_width;

	Float32_t  mm_per_pixel_v = focal_length * tan(fov_v * 0.5) * 2 / result_height;
	Float32_t pose_t_0[3];
	Float32_t pose_t[3];
	Float32_t inv_pose[3];

	//for (int i = 3; i < 6; i++)
	//{
	//	inv_pose[i - 3] = p_avm_param->p_avm_config_param
	//		->camera_model[camera_id].camera_model_ext.inv_pose[i];
	//}
	//p_avm_param->p_avm_config_param->camera_model[camera_id].camera_model_ext.inv_R;
	//Mat_Mul_Vec_3by3(pose_t_0, p_avm_param->p_avm_config_param
	//	->camera_model[camera_id].camera_model_ext.R, inv_pose);
	//pose_t[0] = -pose_t_0[0];
	//pose_t[1] = -pose_t_0[1];
	//pose_t[2] = -pose_t_0[2];//�������������ϵ��λ��

	pose_t[0] = p_avm_param->p_avm_config_param->camera_model[camera_id].camera_model_ext.pose[3];
	pose_t[1] = p_avm_param->p_avm_config_param->camera_model[camera_id].camera_model_ext.pose[4];
	pose_t[2] = p_avm_param->p_avm_config_param->camera_model[camera_id].camera_model_ext.pose[5];//�������������ϵ��λ��
	Float32_t pitch_angle;//�˴�����������¸�����
	if (cam_flag == _1_VIEW_FRONT)
	{
		pitch_angle = (Float32_t)(0 * PI) / 180;//�˴�����������¸�����16 

	}
	if (cam_flag == _1_VIEW_REAR)
	{
		pitch_angle = (Float32_t)(0 * PI) / 180;//�˴�����������¸�����28
	}
	if (cam_flag == _1_VIEW_LEFT)
	{
		pitch_angle = (Float32_t)(0 * PI) / 180;//�˴�����������¸�����28
	}
	if (cam_flag == _1_VIEW_RIGHT)
	{
		pitch_angle = (0 * PI) / 180;//�˴�����������¸�����
	}


	Float32_t delta_z = mm_per_pixel_v ;
	Float32_t world_coordinate[3];
	Float32_t image_point[2];
	Float32_t centre_x = pose_t[0], centre_y = pose_t[1];// centre_z = pose_t[2],
	Float32_t init_world_coord_x, init_world_coord_y, init_world_coord_z;
	if (cam_flag == _1_VIEW_REAR)
	{
		init_world_coord_x = -focal_length*sin(fov_h * 0.5) - pose_t[0];
		init_world_coord_y = -focal_length*cos(fov_h * 0.5) + pose_t[1];
		init_world_coord_z = pose_t[2] - focal_length * sin(pitch_angle)
			+ mm_per_pixel_v * result_height * 0.5 * cos(pitch_angle) + z_up_down;//
	}
	if (cam_flag == _1_VIEW_FRONT)
	{
		init_world_coord_x = p_avm_param->p_avm_config_param->vehicle_param.vehicle_width / 2;
		init_world_coord_y = p_avm_param->p_avm_config_param->vehicle_param.vehicle_length / 2 + focal_length;
		init_world_coord_z = 2000;//
	}
	if (cam_flag == _1_VIEW_RIGHT)
	{
		init_world_coord_x = focal_length*cos(fov_h * 0.5) + pose_t[0];
		init_world_coord_y = focal_length*sin(fov_h * 0.5) + pose_t[1];
		init_world_coord_z = pose_t[2] + focal_length * tan(fov_v * 0.5) + z_up_down;//
	}


	Float32_t d_angle = fov_h / single_view_param.result_image_width;
	Float32_t centre_rotate_coordinate[3];
	for (int i = 0; i < single_view_param.result_image_height; i++)
	{
		centre_rotate_coordinate[0] = p_avm_param->p_avm_config_param->vehicle_param.vehicle_width / 2;
		centre_rotate_coordinate[1] = p_avm_param->p_avm_config_param->vehicle_param.vehicle_length/2;
		for (int j = 0; j < single_view_param.result_image_width; j++)
		{
			if (cam_flag == _1_VIEW_RIGHT)
			{
				world_coordinate[0] = (init_world_coord_x - centre_rotate_coordinate[0])*cos(-j*d_angle) 
					- (init_world_coord_y - centre_rotate_coordinate[1])*sin(-j*d_angle)
					+ centre_rotate_coordinate[0];///is x
				world_coordinate[1] = (init_world_coord_x - centre_rotate_coordinate[0])*sin(-j*d_angle)
					+ (init_world_coord_y - centre_rotate_coordinate[1])*cos(-j*d_angle)
					+ centre_rotate_coordinate[1];//is y
				world_coordinate[2] = init_world_coord_z - i * delta_z;
			}
			if (cam_flag == _1_VIEW_REAR)
			{
				world_coordinate[0] = (init_world_coord_x - centre_rotate_coordinate[0])*cos(j*d_angle) //������ת��ʽ
					- (init_world_coord_y - centre_rotate_coordinate[1])*sin(j*d_angle)
					+ centre_rotate_coordinate[0];///is x
				world_coordinate[1] = (init_world_coord_x - centre_rotate_coordinate[0])*sin(j*d_angle)
					+ (init_world_coord_y - centre_rotate_coordinate[1])*cos(j*d_angle)
					+ centre_rotate_coordinate[1];//is y
				world_coordinate[2] = init_world_coord_z - i * delta_z;


			}
			if (cam_flag == _1_VIEW_FRONT)
			{
				world_coordinate[0] = (init_world_coord_x - centre_rotate_coordinate[0])*cos(-j*d_angle) 
					- (init_world_coord_y - centre_rotate_coordinate[1])*sin(-j*d_angle)
					+ centre_rotate_coordinate[0];///is x
				world_coordinate[1] = (init_world_coord_x - centre_rotate_coordinate[0])*sin(-j*d_angle)
					+ (init_world_coord_y - centre_rotate_coordinate[1])*cos(-j*d_angle)
					+ centre_rotate_coordinate[1];//is y
				world_coordinate[2] = init_world_coord_z - i * delta_z;

			}


			world_coordinate_to_pic_coordinate(camera_id,
				world_coordinate, image_point, p_avm_param);//0 i


			if (cam_flag == _1_VIEW_REAR)
			{
				Float32_t cam_ray[3];
				Float32_t world_ray[3];
				Image_Point_To_Cam_Ray(cam_ray,
					image_point,
					&(p_avm_param->p_avm_config_param
					->camera_model[camera_id].camera_model_int));
				Cam_Ray_To_World_Ray(world_ray,
					cam_ray, &(p_avm_param->p_avm_config_param
					->camera_model[camera_id].camera_model_ext));
				//distance[i*single_view_param.result_image_width * 2 + j * 2] = (Int32_t)world_ray[0];
				//distance[i*single_view_param.result_image_width * 2 + j * 2 + 1] = (Int32_t)world_ray[1];
			}




			UInt32_t  shift_altogether
				= i*single_view_param.result_image_width * 2 + j * 2;
			/*ret=single_view_zip_data(shift_altogether,
			image_point, p_lut, camera_id);*/
			zip_data_new(shift_altogether, image_point, 1,
				p_lut, NULL, NULL, p_avm_param);//Ϊ�˴������ã�����ͼ��ʹû���ں�Ȩ��Ҳʹ����·ȫ���洢��ʽ�ò����Ĵ洢0
			CHECK_ERROR(ret);
		}
	}

	char lut_path[MAX_ARRAY_SIZE];
	sprintf(lut_path, "%s/distance.bin", lut_file_path);
	FILE*gpu = fopen(lut_path, "wb");
	//fwrite(distance, sizeof(Int32_t), single_view_param.result_image_width*single_view_param.result_image_height * 2, gpu);
	fclose(gpu);


	return ret;
}
#endif


#if 0
/**************plane_front_rear_single_view***********************/
/*
* Function Name:				plane_front_rear_single_view
* Function Function:			plane_front_rear_single_view
* Input:
*       p_avm_param;
*		cam_flag;					 this is picture's flag is front or back
* Note:
*     the interface function should be called at the beginning of this module
* Revision:
*     Create by Cheng.Li on 2018/1/30.
*/
AVM_Error_Code_E  column_world_coord_front_rear_single_view(IN P_Avm_Param_S p_avm_param, IN  int cam_flag)
{
	AVM_Error_Code_E ret = ERROR_NONE;
	Front_Rear_Single_View_Param_S single_view_param;
	UInt32_t* p_lut = NULL;
	Int32_t is_front_cam;
	UInt32_t camera_id;
	Float32_t z_up_down;
	if (cam_flag == _1_VIEW_FRONT)
	{
		single_view_param = p_avm_param->p_avm_config_param
			->avm_single_view_param.front_single_view_param;
		p_lut = p_avm_param->p_avm_lut->p_lut_single_view_front;
		is_front_cam = 1;
		camera_id = CAM_FRONT;
		z_up_down = single_view_param.plane_expand_param.trans_y;
	}
	if (cam_flag == _1_VIEW_REAR)
	{
		single_view_param = p_avm_param->p_avm_config_param
			->avm_single_view_param.rear_single_view_param;
		p_lut = p_avm_param->p_avm_lut->p_lut_single_view_rear;
		is_front_cam = -1;
		camera_id = CAM_REAR;
		z_up_down = single_view_param.plane_expand_param.trans_y;
	}
	if (cam_flag == _1_VIEW_RIGHT)//�ҵ�ʹ��ǰ�����ò���
	{
		single_view_param = p_avm_param->p_avm_config_param
			->avm_single_view_param.front_single_view_param;
		p_lut = p_avm_param->p_avm_lut->p_lut_single_view_front;
		is_front_cam = -1;
		camera_id = CAM_RIGHT;
	}

	Int32_t *distance = (Int32_t *)malloc(single_view_param.result_image_width*single_view_param.result_image_height*sizeof(Int32_t)* 2);//s32Vǰ�����ұ���Գ�




	single_view_param.plane_expand_param.fov_h = 160;
	single_view_param.plane_expand_param.fov_v = 139;
	z_up_down = z_up_down - 620;
	Float32_t fov_h = (single_view_param.plane_expand_param.fov_h*PI) / 180.0;
	Float32_t focal_length = 1000; // unit is mm
	//Float32_t fov_h1 = (Float32_t)(single_view_param.plane_expand_param.fov_h*3.14) / 180.0;
	Float32_t fov_v = (single_view_param.plane_expand_param.fov_v*PI) / 180;
	Int32_t result_width = single_view_param.result_image_width;
	Int32_t result_height = single_view_param.result_image_height;
	// ����չ��ͼ��ÿ�������ڸ߿����������ʵ�ʾ���
	//Float32_t	mm_per_pixel_h = focal_length * tan(fov_h * 0.5) * 2 / result_width;

	Float32_t  mm_per_pixel_v = focal_length * tan(fov_v * 0.5) * 2 / result_height;
	Float32_t pose_t_0[3];
	Float32_t pose_t[3];
	Float32_t inv_pose[3];

	for (int i = 3; i < 6; i++)
	{
		inv_pose[i - 3] = p_avm_param->p_avm_config_param
			->camera_model[camera_id].camera_model_ext.inv_pose[i];
	}
	p_avm_param->p_avm_config_param->camera_model[camera_id].camera_model_ext.inv_R;
	Mat_Mul_Vec_3by3(pose_t_0, p_avm_param->p_avm_config_param
		->camera_model[camera_id].camera_model_ext.R, inv_pose);
	pose_t[0] = -pose_t_0[0];
	pose_t[1] = -pose_t_0[1];
	pose_t[2] = -pose_t_0[2];//�������������ϵ��λ��
	Float32_t pitch_angle;//�˴�����������¸�����
	if (cam_flag == _1_VIEW_FRONT)
	{
		pitch_angle = (Float32_t)(0 * PI) / 180;//�˴�����������¸�����16 

	}
	if (cam_flag == _1_VIEW_REAR)
	{
		pitch_angle = (Float32_t)(0 * PI) / 180;//�˴�����������¸�����28
	}
	if (cam_flag == _1_VIEW_RIGHT)
	{
		pitch_angle = (0 * PI) / 180;//�˴�����������¸�����
	}


	Float32_t delta_z = mm_per_pixel_v * cos(pitch_angle);
	Float32_t world_coordinate[3];
	Float32_t image_point[2];
	Float32_t centre_x = pose_t[0], centre_y = pose_t[1], radius = 1000;// centre_z = pose_t[2],
	Float32_t init_world_coord_x, init_world_coord_y, init_world_coord_z;
	if (cam_flag == _1_VIEW_REAR)
	{
		init_world_coord_x = -radius*sin(fov_h * 0.5) - pose_t[0] ;
		init_world_coord_y = -radius*cos(fov_v * 0.5) + pose_t[1];
		init_world_coord_z = pose_t[2] - focal_length * sin(pitch_angle)
			+ mm_per_pixel_v * result_height * 0.5 * cos(pitch_angle) ;//+ z_up_down
	}
	if (cam_flag == _1_VIEW_FRONT)
	{
		init_world_coord_x = -radius*sin(fov_h * 0.5) - pose_t[0];
		init_world_coord_y = radius*cos(fov_v * 0.5) + pose_t[1];
		init_world_coord_z = pose_t[2] - focal_length * sin(pitch_angle)
			+ mm_per_pixel_v * result_height * 0.5 * cos(pitch_angle) ;//+ z_up_down
	}



	Float32_t d_angle = fov_h / single_view_param.result_image_width;
	Float32_t centre_rotate_coordinate[3];
	for (int i = 0; i < single_view_param.result_image_height; i++)
	{
		centre_rotate_coordinate[0] = centre_x;
		centre_rotate_coordinate[1] = centre_y;
		for (int j = 0; j < single_view_param.result_image_width; j++)
		{
			if (cam_flag == _1_VIEW_REAR)
			{
				world_coordinate[0] = (init_world_coord_x - centre_rotate_coordinate[0])*cos(j*d_angle) //������ת��ʽ
					- (init_world_coord_y - centre_rotate_coordinate[1])*sin(j*d_angle)
					+ centre_rotate_coordinate[0];///is x
				world_coordinate[1] = (init_world_coord_x - centre_rotate_coordinate[0])*sin(j*d_angle)
					+ (init_world_coord_y - centre_rotate_coordinate[1])*cos(j*d_angle)
					+ centre_rotate_coordinate[1];//is y
				world_coordinate[2] = init_world_coord_z - i * delta_z;
#if 0
				///***************************/

				//
				//	//ֱ�߲���
				//	Float32_t  m1 = pose_t[0], m2 = pose_t[1], m3 = pose_t[2];//ֱ�߹�һ�����㼴 �������������ϵ������
				//	Float32_t  v1 = pose_t[0] - world_coordinate[0], v2 = pose_t[1]
				//		- world_coordinate[1], v3 = pose_t[2] - world_coordinate[2];//ֱ�ߵķ�������

				//	//ƽ�����
				//	//����ƽ��Ķ��㣬���������¸����򶨵�Ҳ���¸������˴�ѡȡ�Ķ��������+���⽹��
				//	Float32_t n1 = 0, n2=0, n3=0;
				//	

				//	Float32_t	vp1 = 0, vp2 = 0, vp3 = 1;//���� ƽ��ķ�������
				//	Float32_t t = ((n1 - m1)*vp1 + (n2 - m2)*vp2 + (n3 - m3)*vp3)
				//		/ (vp1*v1 + vp2*v2 + vp3*v3);//��ֱ�ߵĲ������̺�ƽ��ĵ㷢�������ֱ����ƽ�������

				//	Float32_t x_intersection = m1 + v1*t;//���ֱ����ƽ���ϵĽ��� 
				//	Float32_t y_intersection = m2 + v2*t;
				//	Float32_t z_intersection = m3 + v3*t;
				//	
				//





				//	distance[i*single_view_param.result_image_width * 2 + j * 2] = (Int32_t)x_intersection;
				//	distance[i*single_view_param.result_image_width * 2 + j * 2 + 1] = (Int32_t)y_intersection;
#endif

			}
			if (cam_flag == _1_VIEW_FRONT)
			{
				world_coordinate[0] = (init_world_coord_x - centre_rotate_coordinate[0])*cos(-j*d_angle) - (init_world_coord_y - centre_rotate_coordinate[1])*sin(-j*d_angle)
	                            + centre_rotate_coordinate[0];///is x
				world_coordinate[1] = (init_world_coord_x - centre_rotate_coordinate[0])*sin(-j*d_angle)
					+ (init_world_coord_y - centre_rotate_coordinate[1])*cos(-j*d_angle)
					+ centre_rotate_coordinate[1];//is y
				world_coordinate[2] = init_world_coord_z - i * delta_z;

			}


				world_coordinate_to_pic_coordinate(camera_id,
					world_coordinate, image_point, p_avm_param);//0 i
			

				if (cam_flag == _1_VIEW_REAR)
				{
							Float32_t cam_ray[3];
						Float32_t world_ray[3];
						Image_Point_To_Cam_Ray(cam_ray,
							image_point,
							&(p_avm_param->p_avm_config_param
							->camera_model[camera_id].camera_model_int));
						Cam_Ray_To_World_Ray(world_ray,
						cam_ray, &(p_avm_param->p_avm_config_param
						->camera_model[camera_id].camera_model_ext));
						distance[i*single_view_param.result_image_width * 2 + j * 2] = (Int32_t)world_ray[0];
						distance[i*single_view_param.result_image_width * 2 + j * 2 + 1] = (Int32_t)world_ray[1];
				}




			UInt32_t  shift_altogether
				= i*single_view_param.result_image_width * 2 + j * 2;
			/*ret=single_view_zip_data(shift_altogether,
			image_point, p_lut, camera_id);*/
			zip_data_new(shift_altogether, image_point, 1,
				p_lut, NULL, NULL, p_avm_param);//Ϊ�˴������ã�����ͼ��ʹû���ں�Ȩ��Ҳʹ����·ȫ���洢��ʽ�ò����Ĵ洢0
			CHECK_ERROR(ret);
		}
	}

	char lut_path[MAX_ARRAY_SIZE];
	sprintf(lut_path, "%s/distance.bin", lut_file_path);
	FILE*gpu = fopen(lut_path, "wb");
	fwrite(distance, sizeof(Int32_t), single_view_param.result_image_width*single_view_param.result_image_height * 2, gpu);
	fclose(gpu);


	return ret;
}
#endif


#if 1
/**************plane_front_rear_single_view***********************/
/*
* Function Name:				plane_front_rear_single_view
* Function Function:			plane_front_rear_single_view
* Input:
*       p_avm_param;
*		cam_flag;					 this is picture's flag is front or back
* Note:
*     the interface function should be called at the beginning of this module
* Revision:
*     Create by Cheng.Li on 2018/1/30.
*/
AVM_Error_Code_E  column_world_coord_front_rear_single_view(IN P_Avm_Param_S p_avm_param, IN  int cam_flag)
{
	AVM_Error_Code_E ret = ERROR_NONE;
	Front_Rear_Single_View_Param_S single_view_param;
	UInt32_t* p_lut = NULL;
	Int32_t is_front_cam;
	UInt32_t camera_id;
	Float32_t z_up_down;
	if (cam_flag == _1_VIEW_FRONT)
	{
		single_view_param = p_avm_param->p_avm_config_param
			->avm_single_view_param.front_single_view_param;
		p_lut = p_avm_param->p_avm_lut->p_lut_single_view_front;
		is_front_cam = 1;
		camera_id = CAM_FRONT;
		z_up_down = single_view_param.plane_expand_param.trans_y;
	}
	if (cam_flag == _1_VIEW_REAR)
	{
		single_view_param = p_avm_param->p_avm_config_param
			->avm_single_view_param.rear_single_view_param;
		p_lut = p_avm_param->p_avm_lut->p_lut_single_view_rear;
		is_front_cam = -1;
		camera_id = CAM_REAR;
		z_up_down = single_view_param.plane_expand_param.trans_y;
	}
	if (cam_flag == _1_VIEW_RIGHT)//�ҵ�ʹ��ǰ�����ò���
	{
		single_view_param = p_avm_param->p_avm_config_param
			->avm_single_view_param.front_single_view_param;
		p_lut = p_avm_param->p_avm_lut->p_lut_single_view_front;
		is_front_cam = -1;
		camera_id = CAM_RIGHT;
	}
	single_view_param.plane_expand_param.fov_h = 160;
	single_view_param.plane_expand_param.fov_v = 120;
	z_up_down = z_up_down - 300;
	Float32_t fov_h = (single_view_param.plane_expand_param.fov_h*PI) / 180.0;
	Float32_t focal_length = 1000; // unit is mm
	Float32_t fov_h1 = (Float32_t)(single_view_param.plane_expand_param.fov_h*3.14) / 180.0;
	Float32_t fov_v = (single_view_param.plane_expand_param.fov_v*PI) / 180;
	Int32_t result_width = single_view_param.result_image_width;
	Int32_t result_height = single_view_param.result_image_height;
	// ����չ��ͼ��ÿ�������ڸ߿����������ʵ�ʾ���
	Float32_t	mm_per_pixel_h = focal_length * tan(fov_h * 0.5) * 2 / result_width;
	Float32_t  mm_per_pixel_v = focal_length * tan(fov_v * 0.5) * 2 / result_height;
	Float32_t pose_t_0[3];
	Float32_t pose_t[3];
	Float32_t inv_pose[3];

	for (int i = 3; i < 6; i++)
	{
		inv_pose[i - 3] = p_avm_param->p_avm_config_param
			->camera_model[camera_id].camera_model_ext.inv_pose[i];
	}
	p_avm_param->p_avm_config_param->camera_model[camera_id].camera_model_ext.inv_R;
	Mat_Mul_Vec_3by3(pose_t_0, p_avm_param->p_avm_config_param
		->camera_model[camera_id].camera_model_ext.R, inv_pose);
	pose_t[0] = -pose_t_0[0];
	pose_t[1] = -pose_t_0[1];
	pose_t[2] = -pose_t_0[2];//�������������ϵ��λ��
	Float32_t pitch_angle;//�˴�����������¸�����
	if (cam_flag == _1_VIEW_FRONT)
	{
		pitch_angle = (Float32_t)(0 * PI) / 180;//�˴�����������¸�����16 

	}
	if (cam_flag == _1_VIEW_REAR)
	{
		pitch_angle = (Float32_t)(0 * PI) / 180;//�˴�����������¸�����28
	}
	if (cam_flag == _1_VIEW_RIGHT)
	{
		pitch_angle = (0 * PI) / 180;//�˴�����������¸�����
	}
	Float32_t init_world_coord_x = pose_t[0] - is_front_cam
		* mm_per_pixel_h * result_width * 0.5;
	Float32_t init_world_coord_y = pose_t[1] + is_front_cam
		* (focal_length * cos(pitch_angle)
		+ mm_per_pixel_v * result_height * 0.5 * sin(pitch_angle));
	Float32_t init_world_coord_z = pose_t[2] - focal_length * sin(pitch_angle)
		+ mm_per_pixel_v * result_height * 0.5 * cos(pitch_angle) + z_up_down;
	Float32_t delta_x = mm_per_pixel_h;
	Float32_t delta_y = mm_per_pixel_v * sin(pitch_angle);
	Float32_t delta_z = mm_per_pixel_v * cos(pitch_angle);
	Float32_t world_coordinate[3];
	Float32_t image_point[2];

	Float32_t centre_x = pose_t[0], centre_y = pose_t[1], centre_z = pose_t[2], radius = 1000;
	init_world_coord_x = -radius*sin(fov_h * 0.5) - pose_t[0] - 1800;
	init_world_coord_y = -radius*cos(fov_v * 0.5) + pose_t[1];

	Float32_t d_angle = fov_h / single_view_param.result_image_width;


	Float32_t centre_rotate_coordinate[3];
	for (int i = 0; i < single_view_param.result_image_height; i++)
	{
		centre_rotate_coordinate[0] = centre_x;
		centre_rotate_coordinate[1] = centre_y;


		for (int j = 0; j < single_view_param.result_image_width; j++)
		{




			world_coordinate[0] = (init_world_coord_x - centre_rotate_coordinate[0])*cos(j*d_angle) //������ת��ʽ
				- (init_world_coord_y - centre_rotate_coordinate[1])*sin(j*d_angle)
				+ centre_rotate_coordinate[0];///is x
			world_coordinate[1] = (init_world_coord_x - centre_rotate_coordinate[0])*sin(j*d_angle)
				+ (init_world_coord_y - centre_rotate_coordinate[1])*cos(j*d_angle)
				+ centre_rotate_coordinate[1];//is y



			world_coordinate[2] = init_world_coord_z - i * delta_z;
			world_coordinate_to_pic_coordinate(camera_id,
				world_coordinate, image_point, p_avm_param);//0 i
			UInt32_t  shift_altogether
				= i*single_view_param.result_image_width * 2 + j * 2;
			/*ret=single_view_zip_data(shift_altogether,
			image_point, p_lut, camera_id);*/
			zip_data_new(shift_altogether, image_point, 1,
				p_lut, NULL, NULL, p_avm_param);//Ϊ�˴������ã�����ͼ��ʹû���ں�Ȩ��Ҳʹ����·ȫ���洢��ʽ�ò����Ĵ洢0
			CHECK_ERROR(ret);
		}
	}
	return ret;
}
#endif


#if 1

///��ʵ���ǲ���������������չ��

/**************plane_front_rear_single_view***********************/
/*
* Function Name:				plane_front_rear_single_view
* Function Function:			plane_front_rear_single_view
* Input:
*       p_avm_param;
*		cam_flag;					 this is picture's flag is front or back
* Note:
*     the interface function should be called at the beginning of this module
* Revision:
*     Create by Cheng.Li on 2018/1/30.
*/
AVM_Error_Code_E column_camera_coord_front_rear_single_view(IN P_Avm_Param_S p_avm_param, IN  int cam_flag)
{
	AVM_Error_Code_E ret = ERROR_NONE;
	Front_Rear_Single_View_Param_S single_view_param;
	CvSize size0;
	UInt32_t* p_lut = NULL;
	if (cam_flag == CAM_FRONT)
	{
		single_view_param = p_avm_param->p_avm_config_param->avm_single_view_param.front_single_view_param;
		size0 = cvSize(single_view_param.result_image_width, single_view_param.result_image_height);//
		p_lut = p_avm_param->p_avm_lut->p_lut_single_view_front;
		//
	}
	if (cam_flag == _1_VIEW_REAR)
	{
		single_view_param = p_avm_param->p_avm_config_param->avm_single_view_param.rear_single_view_param;
		size0 = cvSize(single_view_param.result_image_width, single_view_param.result_image_height);//
		p_lut = p_avm_param->p_avm_lut->p_lut_single_view_rear;
		//
	}


	single_view_param.plane_expand_param.fov_h=120;
	single_view_param.plane_expand_param.fov_v=150;

#if DEBUG_INFO
	IplImage* image[4] = { NULL, NULL, NULL, NULL };
	ret = my_load_image(image);
	CHECK_ERROR(ret);
	IplImage *image_result_single_front_lut = cvCreateImage(size0, IPL_DEPTH_8U, 1);
	memset(image_result_single_front_lut->imageData, 0, single_view_param.result_image_width*single_view_param.result_image_height);
#endif
	Float32_t cam_ray[3];
	Float64_t tan_h = (tan((single_view_param.plane_expand_param.fov_h / 2 / 180)*PI));
	Float64_t tan_v = (tan((single_view_param.plane_expand_param.fov_v / 2 / 180)*PI));
	cam_ray[2] = -8000.00;//�������⽹��
	Float64_t  pixel_hight_dis = -(cam_ray[2] * tan_v) / (single_view_param.result_image_height / 2);//�����ͼ��ֱ������ÿ������ռ���ٺ���
	Float64_t  pixel_width_dis = -(cam_ray[2] * tan_h) / (single_view_param.result_image_width / 2);//�����ͼ��ˮƽ������ÿ������ռ���ٺ���




	Float32_t fov_h = (single_view_param.plane_expand_param.fov_h*PI) / 180.0;
	Float32_t fov_v = (single_view_param.plane_expand_param.fov_v*PI) / 180;
	Float32_t radius = -8000;
	Float32_t init_world_coord_x = radius*tan(fov_v * 0.5)-1800;
	Float32_t init_world_coord_y = radius*sin(fov_h * 0.5)-600;
	Float32_t init_world_coord_z = radius*cos(fov_h * 0.5);
	Float32_t delta_x = init_world_coord_x / (single_view_param.result_image_height / 2);
	Float32_t d_angle = fov_h / single_view_param.result_image_width;
	Float32_t centre_rotate_coordinate[3], world_coordinate[3];


	//Float32_t init_world_coord_x = cam_ray[2] * tan_v;
	//Float32_t init_world_coord_y = cam_ray[2] * tan_h;
	for (int i = 0; i < single_view_param.result_image_height; i++)
	{
		centre_rotate_coordinate[0] = 0;
		centre_rotate_coordinate[1] = 0;


		for (int j = 0; j < single_view_param.result_image_width; j++)
		{
			/*if (cam_flag == CAM_FRONT)
			{*/
			//cam_ray[1] = (Float64_t)(j - single_view_param.result_image_width / 2.00)* pixel_width_dis;//ת��Ϊ�������ϵ
			//cam_ray[0] = (Float64_t)(i - single_view_param.result_image_height / 2.00)* pixel_hight_dis;//������x

			cam_ray[1] = init_world_coord_y + j*pixel_width_dis;//y
			cam_ray[0] = init_world_coord_x + i*pixel_hight_dis;


			world_coordinate[1] = (init_world_coord_x - centre_rotate_coordinate[0])*cos(j*d_angle) //������ת��ʽ
				- (init_world_coord_y - centre_rotate_coordinate[1])*sin(j*d_angle)
				+ centre_rotate_coordinate[0];///is x
			world_coordinate[2] = (init_world_coord_x - centre_rotate_coordinate[0])*sin(j*d_angle)
				+ (init_world_coord_y - centre_rotate_coordinate[1])*cos(j*d_angle)
				+ centre_rotate_coordinate[1];//is y


			world_coordinate[0] = init_world_coord_x - i * delta_x;


			//}
			//if (cam_flag == CAM_REAR)
			//{
			//	cam_ray[1] = (Float64_t)((Float64_t(j) - ((Float64_t)(single_view_param.result_image_width) / 2.00))* pixel_width_dis);//ת��Ϊ�������ϵ
			//	cam_ray[0] = (Float64_t)((Float64_t(i) - ((Float64_t)(single_view_param.result_image_height) / 2.00))* pixel_hight_dis);
			//	//cam_ray[1] = (Float64_t)((Float64_t(single_view_param.result_image_width - j - 1) - ((Float64_t)(single_view_param.result_image_width) / 2.00))* pixel_width_dis);
			//	//cam_ray[0] = (Float64_t)((Float64_t(single_view_param.result_image_height - i - 1) - ((Float64_t)(single_view_param.result_image_height) / 2.00))* pixel_hight_dis);
			//}
			Float32_t image_point[2];
			Cam_Ray_To_Image_Point(image_point, world_coordinate, &(p_avm_param->p_avm_config_param->camera_model[1].camera_model_int));
			UInt32_t  shift_altogether = i*single_view_param.result_image_width * 2 + j * 2;
			//single_view_zip_data(shift_altogether, image_point, p_lut, cam_flag);
			zip_data_new(shift_altogether, image_point, 1,
				p_lut, NULL, NULL, p_avm_param);//Ϊ�˴������ã�����ͼ��ʹû���ں�Ȩ��Ҳʹ����·ȫ���洢��ʽ�ò����Ĵ洢0
#if DEBUG_INFO
			int rgb[3];
			bilinear_interpolation(image[cam_flag], rgb, image_point[1], image_point[0]);
			image_result_single_front_lut->imageData[i *single_view_param.result_image_width + j] = (uchar)(rgb[0]);
#endif

		}
	}
#if DEBUG_INFO
	char result_name[MAX_ARRAY_SIZE];
	sprintf(result_name, "%s/single_plane_expend_front_bear.bmp", result_image_path);
	cvSaveImage(result_name, image_result_single_front_lut);
#endif
	return ret;
}


#endif



/**************box_front_rear_single_view***********************/
/*
* Function Name:				box_front_rear_single_view
* Function Function:			box_front_rear_single_view
* Input:
*       p_avm_param;
*		cam_flag;					 this is picture's flag is front or back
* Note:
*     the interface function should be called at the beginning of this module
* Revision:
*     Create by Cheng.Li on 2018/1/30.
*/
AVM_Error_Code_E box_front_rear_single_view(IN P_Avm_Param_S p_avm_param,IN int cam_flag)
{
	AVM_Error_Code_E ret = ERROR_NONE;
	Front_Rear_Single_View_Param_S single_view_param;
	UInt32_t* p_lut = NULL;
	Float32_t 	z_shift;
	Float32_t 	is_front_cam;
	UInt32_t camera_id;
	if (cam_flag == _1_VIEW_FRONT)
	{
		single_view_param = p_avm_param->p_avm_config_param
			->avm_single_view_param.front_single_view_param;
		if(p_avm_param)
		p_lut = p_avm_param->p_avm_lut->p_lut_single_view_front;
		//z_shift = -396.88666888;
		//z_shift = -1256.88666888;
		z_shift = (Float32_t)single_view_param.box_expand_param.trans_y;
		is_front_cam = 1;
		camera_id = CAM_FRONT;
	}
	if (cam_flag == _1_VIEW_REAR)
	{
		single_view_param = p_avm_param->p_avm_config_param
			->avm_single_view_param.rear_single_view_param;
		p_lut = p_avm_param->p_avm_lut->p_lut_single_view_rear;
		//z_shift = -480.88666888;
		//z_shift = -1282.88666888;
		z_shift =( Float32_t)single_view_param.box_expand_param.trans_y;
		is_front_cam = -1;
		camera_id = CAM_REAR;
		
	}
	Float32_t fov_h_center = single_view_param.box_expand_param.fov_h_center*PI/180;
	Float32_t fov_h_side = single_view_param.box_expand_param.fov_h_side*PI/180;
	Float32_t fov_v = single_view_param.box_expand_param.fov_v*PI/180;

	Int32_t result_width = single_view_param.result_image_width;
	Int32_t result_height = single_view_param.result_image_height;
	Int32_t box_center_width = single_view_param.box_expand_param.center_width;

	Float32_t focal_length = 1000; //unit is mm
	Float32_t box_fold_angle = (fov_h_center + fov_h_side) * 0.5;
	Float32_t box_side_width = (result_width - box_center_width) * 0.5;

	//����չ��ͼ��ÿ�������ڸ߿����������ʵ�ʾ���
	Float32_t 	mm_per_pixel_h_center = focal_length * tan(fov_h_center * 0.5) * 2 / box_center_width;
	Float32_t mm_per_pixel_h_side = focal_length / cos(fov_h_center * 0.5) 
		* sin(fov_h_side * 0.5) * 2 / box_side_width;
	Float32_t mm_per_pixel_v = focal_length * tan(fov_v * 0.5) * 2 / result_height;

	Float32_t pose_t_0[3];
	Float32_t pose_t[3];
	Float32_t inv_pose[3];
	for (int i = 3; i < 6; i++)
	{
		inv_pose[i - 3] = p_avm_param->p_avm_config_param
			->camera_model[camera_id].camera_model_ext.inv_pose[i];
	}
	p_avm_param->p_avm_config_param->camera_model[camera_id].camera_model_ext.inv_R;
	Mat_Mul_Vec_3by3(pose_t_0, p_avm_param->p_avm_config_param
		->camera_model[camera_id].camera_model_ext.R, inv_pose);
	pose_t[0] = -pose_t_0[0];
	pose_t[1] = -pose_t_0[1];
	pose_t[2] = -pose_t_0[2];
	//pose_t[2]=
	//set init point's coordinate of each part
	Float32_t init_world_coord_x_left = pose_t[0] - is_front_cam 
		* (mm_per_pixel_h_center * box_center_width * 0.5 
		+ mm_per_pixel_h_side * box_side_width * cos(box_fold_angle));
	Float32_t init_world_coord_y_left = pose_t[1] + is_front_cam * (focal_length
		- mm_per_pixel_h_side * box_side_width * sin(box_fold_angle));
	Float32_t init_world_coord_z_left = pose_t[2] + mm_per_pixel_v 
		* result_height * 0.5 + z_shift;

	Float32_t init_world_coord_x_center = pose_t[0] - is_front_cam 
		* mm_per_pixel_h_center * box_center_width * 0.5;
	Float32_t init_world_coord_y_center = pose_t[1] + is_front_cam * focal_length;
	Float32_t init_world_coord_z_center = pose_t[2] 
		+ mm_per_pixel_v * result_height * 0.5 + z_shift;

	Float32_t init_world_coord_x_right = pose_t[0] 
		+ is_front_cam * mm_per_pixel_h_center * box_center_width * 0.5;
	Float32_t init_world_coord_y_right = pose_t[1] + is_front_cam * focal_length;
	Float32_t init_world_coord_z_right = pose_t[2] + mm_per_pixel_v * result_height * 0.5 + z_shift;
	for (int i = 0; i < single_view_param.result_image_height; i++)
	{
		for (int j = 0; j < single_view_param.result_image_width; j++)
		{
			Float32_t world_coord[3];
			if (j < box_side_width)
			{
				world_coord[0] = init_world_coord_x_left
					+ is_front_cam * j * mm_per_pixel_h_side * cos(box_fold_angle);
				world_coord[1] = init_world_coord_y_left 
					+ is_front_cam * j * mm_per_pixel_h_side * sin(box_fold_angle);
				world_coord[2] = init_world_coord_z_left 
					- i * mm_per_pixel_v;
			}
			else if(j <= box_side_width + box_center_width)
			{
				world_coord[0] = init_world_coord_x_center 
					+ is_front_cam * (j - box_side_width) * mm_per_pixel_h_center;
				world_coord[1] = init_world_coord_y_center;
				world_coord[2] = init_world_coord_z_center 
					- i * mm_per_pixel_v;
			}
			else
			{
				world_coord[0] = init_world_coord_x_right + is_front_cam 
					* (j - box_side_width - box_center_width)
					* mm_per_pixel_h_side * cos(box_fold_angle);
				world_coord[1] = init_world_coord_y_right - is_front_cam
					* (j- box_side_width - box_center_width)
					* mm_per_pixel_h_side * sin(box_fold_angle);
				world_coord[2] = init_world_coord_z_right -i* mm_per_pixel_v;
			}
			Float32_t image_point[2];
			ret = world_coordinate_to_pic_coordinate(camera_id, world_coord, image_point, p_avm_param);//0 is front
			CHECK_ERROR(ret);
			UInt32_t  shift_altogether = i*single_view_param.result_image_width * 2 + j * 2;
			//ret = single_view_zip_data(shift_altogether, image_point, p_lut, camera_id);
			zip_data_new(shift_altogether, image_point, 1,
				p_lut, NULL, NULL, p_avm_param);//Ϊ�˴������ã�����ͼ��ʹû���ں�Ȩ��Ҳʹ����·ȫ���洢��ʽ�ò����Ĵ洢0
			CHECK_ERROR(ret);
		}
	}
	return ret;

}



/**************left_right_crop_expend***********************/
/*
* Function Name:				left_right_crop_expend
* Function Function:			left_right_crop_expend
* Input:
*       p_avm_param;
*		cam_flag;					 this is picture's flag is front or back
* Note:
*     the interface function should be called at the beginning of this module
* Revision:
*     Create by Cheng.Li on 2018/1/30.
*/
AVM_Error_Code_E left_right_crop_expend(IN P_Avm_Param_S p_avm_param, IN int cam_flag)
{
	AVM_Error_Code_E ret = ERROR_NONE;
	Left_Right_Single_View_Param_S single_view_param;
	CvSize size0;
	UInt32_t* p_lut = NULL;
	if (cam_flag == 2)
	{
		single_view_param = p_avm_param->p_avm_config_param->avm_single_view_param.left_single_view_param;
		size0 = cvSize(single_view_param.result_image_width, single_view_param.result_image_height);//
		p_lut = p_avm_param->p_avm_lut->p_lut_single_view_left;
	}
	if (cam_flag == CAM_RIGHT)
	{
		single_view_param = p_avm_param->p_avm_config_param->avm_single_view_param.right_single_view_param;
		size0 = cvSize(single_view_param.result_image_width, single_view_param.result_image_height);//
		p_lut = p_avm_param->p_avm_lut->p_lut_single_view_right;
	}
	Float32_t scale_coordinate_col = Float32_t(single_view_param.crop_param.crop_rect.width) 
									/ Float32_t(single_view_param.result_image_width);//��ʾ���εĿ�Ӱ�䵽
	Float32_t scale_coordinate_row = Float32_t(single_view_param.crop_param.crop_rect.height) 
									/ Float32_t(single_view_param.result_image_height);
	for (int i = 0; i < single_view_param.result_image_height; i++)
	{
		for (int j = 0; j < single_view_param.result_image_width; j++)
		{
			Float32_t image_point[2];
			Float32_t image_point_0[2];
			/**************************************************/
			image_point[1] = single_view_param.crop_param.crop_rect.y + i*scale_coordinate_row;
			image_point[0] = single_view_param.crop_param.crop_rect.x + j*scale_coordinate_col;
			image_point_0[0] = (image_point[0] - single_view_param.crop_param.crop_rect.x)
				*cos((single_view_param.crop_param.roll_angle / 180)*PI)
							 - (image_point[1] - single_view_param.crop_param.crop_rect.y)
							 *sin((single_view_param.crop_param.roll_angle / 180)*PI) //Ϊ������ת��ʽ�����ϵ���ת�û�����ĽǶ�
							 + single_view_param.crop_param.crop_rect.x;//
			image_point_0[1] = (image_point[0] - single_view_param.crop_param.crop_rect.x)
				*sin((single_view_param.crop_param.roll_angle / 180)*PI)
							 + (image_point[1] - single_view_param.crop_param.crop_rect.y)
							 *cos((single_view_param.crop_param.roll_angle / 180)*PI) 
							 + single_view_param.crop_param.crop_rect.y;//
			UInt32_t  shift_altogether = i*single_view_param.result_image_width * 2 + j * 2;
			//ret=single_view_zip_data(shift_altogether, image_point, p_lut, cam_flag);//������ѹ��lut��
			zip_data_new(shift_altogether, image_point, 1,
				p_lut, NULL, NULL, p_avm_param);//Ϊ�˴������ã�����ͼ��ʹû���ں�Ȩ��Ҳʹ����·ȫ���洢��ʽ�ò����Ĵ洢0
			CHECK_ERROR(ret);
		}
	}

	return ret;

}
/**************calculate world coordinate***********************/
/*
* Function Name:				calculate_world_coordinate
* Function Function:			calculate world coordinate by picture coordinate
* Input:
*       i;                      this is picture's row coordinate
*		j;						this is picture's cols coordinate
*       world_coordinate;       this is world coordinate
* Note:
*     the interface function should be called at the beginning of this module
* Revision:
*     Create by Cheng.Li on 2018/1/12.
*/
void calculate_world_coordinate_left_right(OUT Float32_t *world_coordinate,
	IN int i, IN int j, IN P_Avm_Param_S p_avm_param
	, IN Left_Right_Single_View_Param_S left_right_view_param, int flag)
{
	Float32_t pixel_row_dis = (left_right_view_param.plane_expand_param.world_height)
		/ (Float32_t)(left_right_view_param.result_image_height);//calculate every pixel distance
	Float32_t pixel_col_dis = (left_right_view_param.plane_expand_param.world_width)
		/ (Float32_t)(left_right_view_param.result_image_width);//calculate every pixel distance//calculate every pixel distance
	if (flag == CAM_LEFT)
	{
		world_coordinate[0] = Float32_t(pixel_col_dis)*Float32_t(j) 
			- left_right_view_param.plane_expand_param.world_width
						- p_avm_param->p_avm_config_param->vehicle_param.vehicle_width / 2;
		world_coordinate[1] = left_right_view_param.plane_expand_param.world_height
			/ 2 - Float32_t(pixel_row_dis)*Float32_t(i);
	}
	if (flag == CAM_RIGHT)
	{
		world_coordinate[0] = Float32_t(pixel_col_dis)*Float32_t(j) 
			+ p_avm_param->p_avm_config_param->vehicle_param.vehicle_width / 2;
		world_coordinate[1] = left_right_view_param.plane_expand_param.world_height
			/ 2 - Float32_t(pixel_row_dis)*Float32_t(i);
	}
}


/**************left_right_overlook_expend***********************/
/*
* Function Name:				left_right_overlook_expend
* Function Function:			left_right_overlook_expend
* Input:
*       p_avm_param;
*		cam_flag;					 this is picture's flag is front or back
* Note:
*     the interface function should be called at the beginning of this module
* Revision:
*     Create by Cheng.Li on 2018/1/30.
*/
AVM_Error_Code_E left_right_overlook_expend(IN P_Avm_Param_S p_avm_param,IN int cam_flag)
{
	AVM_Error_Code_E ret = ERROR_NONE;
	Left_Right_Single_View_Param_S single_view_param;
	CvSize size0;
	UInt32_t* p_lut = NULL;
	if (cam_flag == CAM_LEFT)
	{
		single_view_param = p_avm_param->p_avm_config_param
			->avm_single_view_param.left_single_view_param;
		size0 = cvSize(single_view_param.result_image_width, single_view_param.result_image_height);//
		p_lut = p_avm_param->p_avm_lut->p_lut_single_view_left;
	}
	if (cam_flag == CAM_RIGHT)
	{
		single_view_param = p_avm_param->p_avm_config_param
			->avm_single_view_param.right_single_view_param;
		size0 = cvSize(single_view_param.result_image_width,
			single_view_param.result_image_height);//
		p_lut = p_avm_param->p_avm_lut->p_lut_single_view_right;
	}
	Float32_t dx = (single_view_param.plane_expand_param.world_width)
		/ (Float32_t)(single_view_param.result_image_width);
	Float32_t dy = (single_view_param.plane_expand_param.world_height)
		/ (Float32_t)(single_view_param.result_image_height);
	Float32_t world_init_x_left;
	Float32_t world_init_y_left;

	if (cam_flag == CAM_LEFT)
	{
		world_init_x_left = -(single_view_param.plane_expand_param.world_width
			+ (p_avm_param->p_avm_config_param->vehicle_param.vehicle_width * 0.5 
			- single_view_param.plane_expand_param.car_body_width * dx));
		world_init_y_left = single_view_param.plane_expand_param.world_height * 0.5 
			- single_view_param.plane_expand_param.trans_y;
	}
	if (cam_flag == CAM_RIGHT)
	{
		world_init_x_left = p_avm_param->p_avm_config_param->vehicle_param.vehicle_width * 0.5 
			- single_view_param.plane_expand_param.car_body_width * dx;
		world_init_y_left = single_view_param.plane_expand_param.world_height * 0.5 
			- single_view_param.plane_expand_param.trans_y;
	}
	/********************extend all over result image pixel****************************/
	for (int i = 0; i < single_view_param.result_image_height; i++)
	{
		for (int j = 0; j < single_view_param.result_image_width; j++)
		{
			Float32_t world_coord[3];
			world_coord[0] = world_init_x_left + (j) * dx;
			world_coord[1] = world_init_y_left - (i) * dy;
			world_coord[2] = 0;
			Float32_t image_point[2];
			world_coordinate_to_pic_coordinate(cam_flag, world_coord,
				image_point, p_avm_param);
			UInt32_t  shift_altogether = i*single_view_param.result_image_width * 2 
				+ j * 2;
			//ret=single_view_zip_data(shift_altogether, image_point, p_lut, cam_flag);//���lut��
			zip_data_new(shift_altogether, image_point, 1,
				p_lut, NULL, NULL, p_avm_param);//Ϊ�˴������ã�����ͼ��ʹû���ں�Ȩ��Ҳʹ����·ȫ���洢��ʽ�ò����Ĵ洢0
			CHECK_ERROR(ret);
		}
	}

	return ret;
}



/*************calculate_world_coordinate_combine_left_right***********************/
/*
* Function Name:				calculate_world_coordinate
* Function Function:			calculate world coordinate by picture coordinate
* Input:
*       i;                      this is picture's row coordinate
*		j;						this is picture's cols coordinate
*       world_coordinate;       this is world coordinate
* Note:
*     the interface function should be called at the beginning of this module
* Revision:
*     Create by Cheng.Li on 2018/1/12.
*/
void calculate_world_coordinate_combine_left_right(OUT Float32_t *world_coordinate,
	IN int i, IN int j, IN P_Avm_Param_S p_avm_param
	, IN Left_Right_Single_View_Param_S left_right_view_param,int cam_flag)
{
	Float32_t pixel_row_dis = (left_right_view_param.combine_expand_param.world_height)
		/ (Float32_t)(left_right_view_param.result_image_height);//calculate every pixel distance
	Float32_t pixel_col_dis = (left_right_view_param.combine_expand_param.plane_width)
		/ (Float32_t)(left_right_view_param.result_image_width
		*left_right_view_param.combine_expand_param.plane_ratio);//calculate every pixel distance//calculate every pixel distance
	if (cam_flag == CAM_LEFT)
	{
		world_coordinate[0] = -Float32_t(pixel_col_dis)*Float32_t(left_right_view_param.result_image_width - j)
			- p_avm_param->p_avm_config_param->vehicle_param.vehicle_width / 2;
	}
	if (cam_flag == CAM_RIGHT)
	{
		world_coordinate[0] = Float32_t(pixel_col_dis)*Float32_t(j) 
			+ p_avm_param->p_avm_config_param->vehicle_param.vehicle_width / 2;
	}
	world_coordinate[1] = left_right_view_param.combine_expand_param.world_height / 2 
		- Float32_t(pixel_row_dis)*Float32_t(i);
}




/************** left_right_combine_overlook_expend***********************/
/*
* Function Name:				 left_right_combine_overlook_expend
* Function Function:			 left_right_combine_overlook_expend
* Input:
*       p_avm_param;
*		cam_flag;					 this is picture's flag is front or back
* Note:
*     the interface function should be called at the beginning of this module
* Revision:
*     Create by Cheng.Li on 2018/1/30.
*/

AVM_Error_Code_E left_right_combine_overlook_expend(IN P_Avm_Param_S p_avm_param,IN int cam_flag)
{
	Float32_t R = 1000;
	AVM_Error_Code_E ret = ERROR_NONE;
	Left_Right_Single_View_Param_S single_view_param;
	CvSize size0;
	UInt32_t* p_lut = NULL;
	Float32_t x_z[2];
	Float32_t world_coordinate_0[2];
	if (cam_flag == CAM_LEFT)
	{
		single_view_param = p_avm_param->p_avm_config_param
			->avm_single_view_param.left_single_view_param;
		size0 = cvSize(single_view_param.result_image_width, 
			single_view_param.result_image_height);//
		p_lut = p_avm_param->p_avm_lut->p_lut_single_view_left;
		
		world_coordinate_0[0] = -single_view_param.combine_expand_param.plane_width 
								- p_avm_param->p_avm_config_param
								->vehicle_param.vehicle_width / 2;
		world_coordinate_0[1] = 0;
		x_z[0] = -single_view_param.combine_expand_param.plane_width - p_avm_param
					->p_avm_config_param->vehicle_param.vehicle_width / 2;//ƽ�沿�ֵı߽��x��z������
		x_z[1] = R;//��ת��Բ��
	}
	if (cam_flag == CAM_RIGHT)
	{
		single_view_param = p_avm_param->p_avm_config_param
			->avm_single_view_param.right_single_view_param;
		size0 = cvSize(single_view_param.result_image_width, 
			single_view_param.result_image_height);//
		p_lut = p_avm_param->p_avm_lut->p_lut_single_view_right;
		world_coordinate_0[0] = single_view_param.combine_expand_param.plane_width 
							+ p_avm_param->p_avm_config_param->vehicle_param.vehicle_width / 2;
		world_coordinate_0[1] = 0;
		x_z[0] = single_view_param.combine_expand_param.plane_width 
				+ p_avm_param->p_avm_config_param->vehicle_param.vehicle_width / 2;//�ӽ�ƽ�沿�ֵı߽��x��z����ת����Բ��
		x_z[1] = R;
	}
	Float32_t world_coordinate[3] = { 0, 0, 0 };
	/********************extend all over result image pixel****************************/
	for (int i = 0; i < single_view_param.result_image_height; i++)
	{
		for (int j = 0; j < single_view_param.result_image_width; j++)
		{
			/***********calculate world coordinate*********/
			Float32_t image_point[2];
			world_coordinate[2] = 0;
			calculate_world_coordinate_combine_left_right(world_coordinate, 
				i, j, p_avm_param, single_view_param,cam_flag);
			if (cam_flag == CAM_RIGHT)
			{
				/**********right***********/
				if (j<=single_view_param.result_image_width
					*single_view_param.combine_expand_param.plane_ratio)
				{
					world_coordinate[2] = 0;
					world_coordinate_to_pic_coordinate(cam_flag, 
						world_coordinate, image_point, p_avm_param);
					UInt32_t  shift_altogether =
						i*single_view_param.result_image_width * 2 + j * 2;
					//single_view_zip_data(shift_altogether, image_point, p_lut, cam_flag);
					zip_data_new(shift_altogether, image_point, 1,
						p_lut, NULL, NULL, p_avm_param);//Ϊ�˴������ã�����ͼ��ʹû���ں�Ȩ��Ҳʹ����·ȫ���洢��ʽ�ò����Ĵ洢0

				}
				if (j>=single_view_param.result_image_width
					*single_view_param.combine_expand_param.plane_ratio + 1)
				{
					Float32_t m = (abs(world_coordinate[0]) - single_view_param.combine_expand_param.plane_width 
								- p_avm_param->p_avm_config_param->vehicle_param.vehicle_width / 2);
					Float32_t angle_temp = atan(m / R);
					world_coordinate[0] = (world_coordinate_0[0] - x_z[0])*cos(angle_temp) 
										- (world_coordinate_0[1] - x_z[1])*sin(angle_temp) + x_z[0];///is z
					world_coordinate[2] = (world_coordinate_0[0] - x_z[0])*sin(angle_temp)
										+ (world_coordinate_0[1] - x_z[1])*cos(angle_temp) + x_z[1];//is y
					world_coordinate_to_pic_coordinate(cam_flag, world_coordinate, image_point, p_avm_param);
					UInt32_t  shift_altogether = i*single_view_param.result_image_width * 2 + j * 2;
					//single_view_zip_data(shift_altogether, image_point, p_lut, cam_flag);
					zip_data_new(shift_altogether, image_point, 1,
						p_lut, NULL, NULL, p_avm_param);//Ϊ�˴������ã�����ͼ��ʹû���ں�Ȩ��Ҳʹ����·ȫ���洢��ʽ�ò����Ĵ洢0

				}

			}
			if (cam_flag == CAM_LEFT)
			{
				/************left*********/
				if (j >= single_view_param.result_image_width*(1.000 
					- single_view_param.combine_expand_param.plane_ratio))
				{
					world_coordinate[2] = 0;
					world_coordinate_to_pic_coordinate(cam_flag, world_coordinate, 
						image_point, p_avm_param);
					UInt32_t  shift_altogether = i*single_view_param.result_image_width * 2 + j * 2;
					//single_view_zip_data(shift_altogether, image_point, p_lut, cam_flag);
					zip_data_new(shift_altogether, image_point, 1,
						p_lut, NULL, NULL, p_avm_param);//Ϊ�˴������ã�����ͼ��ʹû���ں�Ȩ��Ҳʹ����·ȫ���洢��ʽ�ò����Ĵ洢0
				}
				if (j <= single_view_param.result_image_width*(1.000 
					- single_view_param.combine_expand_param.plane_ratio) - 1)
				{
					Float32_t m = (abs(world_coordinate[0]) - single_view_param.combine_expand_param.plane_width
								- p_avm_param->p_avm_config_param->vehicle_param.vehicle_width / 2);
					Float32_t angle_temp = -atan(m / R);
					world_coordinate[0] = ((world_coordinate_0[0] - x_z[0])*cos(angle_temp) 
										- (world_coordinate_0[1] - x_z[1])*sin(angle_temp) + x_z[0]);///is z
					world_coordinate[2] = (world_coordinate_0[0] - x_z[0])*sin(angle_temp) 
										+ (world_coordinate_0[1] - x_z[1])*cos(angle_temp) + x_z[1];//is y
					world_coordinate_to_pic_coordinate(cam_flag, world_coordinate, image_point, p_avm_param);
					UInt32_t  shift_altogether = i*single_view_param.result_image_width * 2 + j * 2;
					//single_view_zip_data(shift_altogether, image_point, p_lut, cam_flag);
					zip_data_new(shift_altogether, image_point, 1,
						p_lut, NULL, NULL, p_avm_param);//Ϊ�˴������ã�����ͼ��ʹû���ں�Ȩ��Ҳʹ����·ȫ���洢��ʽ�ò����Ĵ洢0

				}
			}

		}
	}

	return ret;
}







/**************calculate world coordinate***********************/
/*
* Function Name:				calculate_world_coordinate
* Function Function:			calculate world coordinate by picture coordinate
* Input:
*       i;                      this is picture's row coordinate
*		j;						this is picture's cols coordinate
*       world_coordinate;       this is world coordinate
* Note:
*     the interface function should be called at the beginning of this module
* Revision:
*     Create by Cheng.Li on 2018/1/12.
*/
void calculate_world_coordinate_comine_left_right(OUT Float32_t *world_coordinate,
	IN int i, IN int j, IN P_Avm_Param_S p_avm_param
	, IN Single_View_Left_Right_Plane_Expand_Param_S left_right_view_param, int flag)
{
	Float32_t pixel_row_dis = (left_right_view_param.world_height)
		/ (Float32_t)(p_avm_param->p_avm_config_param
		->avm_single_view_param.left_right_view_param.result_image_height);//calculate every pixel distance
	Float32_t pixel_col_dis = (left_right_view_param.world_width)
		/ ((Float32_t)p_avm_param->p_avm_config_param
		->avm_single_view_param.left_right_view_param.result_image_width/2.000);//calculate every pixel distance//calculate every pixel distance
	if (flag == CAM_LEFT)
	{
		world_coordinate[0] = Float32_t(pixel_col_dis)*Float32_t(j) 
			- left_right_view_param.world_width
							- p_avm_param->p_avm_config_param->vehicle_param.vehicle_width / 2;
		world_coordinate[1] = left_right_view_param.world_height / 2 
			- Float32_t(pixel_row_dis)*Float32_t(i);
	}
	if (flag == CAM_RIGHT)
	{
		world_coordinate[0] = Float32_t(pixel_col_dis)*Float32_t(j)
			+ p_avm_param->p_avm_config_param->vehicle_param.vehicle_width / 2;
		world_coordinate[1] = left_right_view_param.world_height / 2 
			- Float32_t(pixel_row_dis)*Float32_t(i);
	}


}

/**************lr_single_crop**********************/
/*
* Function Name:				 lr_single_crop
* Function Function:			 lr_single_crop
* Input:
*       p_avm_param;
*		cam_flag;					 this is picture's flag is front or back
* Note:
*     the interface function should be called at the beginning of this module
* Revision:
*     Create by Cheng.Li on 2018/1/30.
*/

AVM_Error_Code_E lr_single_overlook(IN P_Avm_Param_S p_avm_param)
{
	AVM_Error_Code_E ret = ERROR_NONE;
	UInt32_t* p_lut = NULL;
	Left_Right_View_Param_S	left_right_view_param;
	CvSize size0;
	left_right_view_param = p_avm_param->p_avm_config_param
		->avm_single_view_param.left_right_view_param;
	size0 = cvSize(left_right_view_param.result_image_width,
		left_right_view_param.result_image_height);
	p_lut = p_avm_param->p_avm_lut->p_lut_single_view_left_right;
	

	Float32_t world_coordinate[3] = { 0, 0, 0 };
	//left_right_view_param.combine_expand_param_left.;
	Float32_t dx_left = (left_right_view_param.plane_expand_param_left.world_width)
		/ (Float32_t)(left_right_view_param.result_image_width/2);
	Float32_t dy_left = (left_right_view_param.plane_expand_param_left.world_height)
		/ (Float32_t)(left_right_view_param.result_image_height);
	Float32_t dx_right = (left_right_view_param.plane_expand_param_right.world_width)
		/ (Float32_t)(left_right_view_param.result_image_width/2);
	Float32_t dy_right = (left_right_view_param.plane_expand_param_right.world_height)
		/ (Float32_t)(left_right_view_param.result_image_height);

	Float32_t world_init_x_left;
	Float32_t world_init_y_left;
	Float32_t world_init_x_right;
	Float32_t world_init_y_right;

	world_init_x_left = -(left_right_view_param.plane_expand_param_left.world_width
		+ (p_avm_param->p_avm_config_param->vehicle_param.vehicle_width * 0.5 
		- left_right_view_param.plane_expand_param_left.car_body_width * dx_left));
	world_init_y_left = left_right_view_param.plane_expand_param_left.world_height * 0.5 
		- left_right_view_param.plane_expand_param_left.trans_y;
	
	world_init_x_right =(p_avm_param->p_avm_config_param->vehicle_param.vehicle_width * 0.5 - left_right_view_param.plane_expand_param_right.car_body_width * dx_right);
	
	world_init_y_right = left_right_view_param.plane_expand_param_right.world_height * 0.5 
		- left_right_view_param.plane_expand_param_right.trans_y;

	/********************extend all over result image pixel****************************/
	for (int i = 0; i < left_right_view_param.result_image_height; i++)
	{
		for (int j = 0; j < left_right_view_param.result_image_width; j++)
		{
			/***********calculate world coordinate*********/
			Float32_t image_point[2];
			Float32_t world_coord[3];
			if (j < (left_right_view_param.result_image_width / 2))
			{
				world_coord[0] = world_init_x_left + (j)* dx_left;
				world_coord[1] = world_init_y_left - (i)* dy_left;
				world_coord[2] = 0;

				world_coordinate_to_pic_coordinate(CAM_LEFT,
					world_coord, image_point, p_avm_param);


			}
			
			if (j >= (left_right_view_param.result_image_width / 2))
			{
				world_coord[0] = world_init_x_right 
					+ (j - left_right_view_param.result_image_width / 2)* dx_right;
				world_coord[1] = world_init_y_right - (i)* dy_right;
				world_coord[2] = 0;

				world_coordinate_to_pic_coordinate(CAM_RIGHT,
					world_coord, image_point, p_avm_param);
			}
			UInt32_t  shift_altogether = i*left_right_view_param.result_image_width * 2 + j * 2;
			zip_data_new(shift_altogether, image_point, 1,//����ͼ�ں�Ȩ��Ϊ1
				p_lut, NULL, NULL, p_avm_param);//Ϊ�˴������ã�����ͼ��ʹû���ں�Ȩ��Ҳʹ����·ȫ���洢��ʽ�ò����Ĵ洢0
		
		}
	}

	return ret;
}

/*************calculate_world_coordinate_combine_left_right***********************/
/*
* Function Name:				calculate_world_coordinate
* Function Function:			calculate world coordinate by picture coordinate
* Input:
*       i;                      this is picture's row coordinate
*		j;						this is picture's cols coordinate
*       world_coordinate;       this is world coordinate
		 cam_flag;
* Note:
*     the interface function should be called at the beginning of this module
* Revision:
*     Create by Cheng.Li on 2018/1/12.
*/
void lr_calculate_world_coordinate_combine(OUT Float32_t *world_coordinate,
	IN int i, IN int j, IN P_Avm_Param_S p_avm_param
	, IN Left_Right_View_Param_S left_right_view_param, int cam_flag)
{
	Float32_t pixel_row_dis;
	Float32_t pixel_col_dis;
	if (cam_flag == CAM_RIGHT)
	{
		pixel_row_dis = (left_right_view_param.combine_expand_param_right.world_height)
			/ (Float32_t)(left_right_view_param.result_image_height);//calculate every pixel distance
		pixel_col_dis = (left_right_view_param.combine_expand_param_right.plane_width)
			/ (Float32_t)(left_right_view_param.result_image_width *0.5
			*left_right_view_param.combine_expand_param_right.plane_ratio);//calculate every pixel distance//calculate every pixel distance
	}
	if (cam_flag == CAM_LEFT)
	{
		pixel_row_dis = (left_right_view_param.combine_expand_param_left.world_height)
			/ (Float32_t)(left_right_view_param.result_image_height);//calculate every pixel distance
		pixel_col_dis = (left_right_view_param.combine_expand_param_left.plane_width)
			/ (Float32_t)(left_right_view_param.result_image_width *0.5
			*left_right_view_param.combine_expand_param_left.plane_ratio);//calculate every pixel distance//calculate every pixel distance
	}

	if (cam_flag == CAM_LEFT)
	{
		world_coordinate[0] = -Float32_t(pixel_col_dis)
			*Float32_t(left_right_view_param.result_image_width/2 - j)
							- (p_avm_param->p_avm_config_param->vehicle_param.vehicle_width / 2)*0.6;
		world_coordinate[1] = left_right_view_param.combine_expand_param_left.world_height / 2
							- Float32_t(pixel_row_dis)*Float32_t(i);

	}
	if (cam_flag == CAM_RIGHT)
	{
		world_coordinate[0] = Float32_t(pixel_col_dis)*Float32_t(j) 
							+ (p_avm_param->p_avm_config_param->vehicle_param.vehicle_width / 2)*0.6;
		world_coordinate[1] = left_right_view_param.combine_expand_param_right.world_height / 2 
						- Float32_t(pixel_row_dis)*Float32_t(i);
	}
}
/************** left_right_combine_overlook_expend***********************/
/*
* Function Name:				 left_right_combine_overlook_expend
* Function Function:			 left_right_combine_overlook_expend
* Input:
*       p_avm_param;
*		cam_flag;					 this is picture's flag is front or back
* Note:
*     the interface function should be called at the beginning of this module
* Revision:
*     Create by Cheng.Li on 2018/1/30.
*/
AVM_Error_Code_E lr_combine_overlook_expend(IN P_Avm_Param_S p_avm_param)
{
	AVM_Error_Code_E ret = ERROR_NONE;
	#define R 2000.000 //
	Left_Right_View_Param_S	left_right_view_param;
	CvSize size0;
	left_right_view_param = p_avm_param->p_avm_config_param
		->avm_single_view_param.left_right_view_param;
	size0 = cvSize(left_right_view_param.result_image_width, 
		left_right_view_param.result_image_height);
	UInt32_t* p_lut = p_avm_param->p_avm_lut->p_lut_single_view_left_right;
	Float32_t x_z[2];
	Float32_t world_coordinate_0[2];
	world_coordinate_0[0] = left_right_view_param.combine_expand_param_right.plane_width 
		+ p_avm_param->p_avm_config_param->vehicle_param.vehicle_width / 2;
	world_coordinate_0[1] = 0;
	x_z[0] = left_right_view_param.combine_expand_param_right.plane_width 
		+ p_avm_param->p_avm_config_param->vehicle_param.vehicle_width / 2;//�ӽ�ƽ�沿�ֵı߽��x��z����ת����Բ��
	x_z[1] = R;

	Float32_t world_coordinate[3] = { 0, 0, 0 };
	/********************extend all over result image pixel****************************/
	for (int i = 0; i < left_right_view_param.result_image_height; i++)
	{
		for (int j = 0; j < left_right_view_param.result_image_width; j++)
		{
			/***********calculate world coordinate*********/
			Float32_t image_point[2];
			world_coordinate[2] = 0;
			if (j>left_right_view_param.result_image_width / 2)
			{
				world_coordinate_0[0] = left_right_view_param.combine_expand_param_right.plane_width 
					+ p_avm_param->p_avm_config_param->vehicle_param.vehicle_width / 2;
				world_coordinate_0[1] = 0;
				x_z[0] = left_right_view_param.combine_expand_param_right.plane_width 
					+ p_avm_param->p_avm_config_param->vehicle_param.vehicle_width / 2;//�ӽ�ƽ�沿�ֵı߽��x��z����ת����Բ��
				x_z[1] = R;

				lr_calculate_world_coordinate_combine(world_coordinate, i,
					(j - left_right_view_param.result_image_width / 2),
					p_avm_param, left_right_view_param, CAM_RIGHT);
				/**********right***********/
				if (j<left_right_view_param.result_image_width / 2 
					+ left_right_view_param.result_image_width
					*0.5*left_right_view_param.combine_expand_param_right.plane_ratio)
				{
					world_coordinate[2] = 0;
					world_coordinate_to_pic_coordinate(CAM_RIGHT, 
						world_coordinate, image_point, p_avm_param);
					UInt32_t  shift_altogether = i*left_right_view_param.result_image_width * 2 + j * 2;
					//single_view_zip_data(shift_altogether, image_point, p_lut, CAM_RIGHT);
					zip_data_new(shift_altogether, image_point, 1,
						p_lut, NULL, NULL, p_avm_param);//Ϊ�˴������ã�����ͼ��ʹû���ں�Ȩ��Ҳʹ����·ȫ���洢��ʽ�ò����Ĵ洢0

				}
				else
				{
					Float32_t m = (abs(world_coordinate[0]) 
						- left_right_view_param.combine_expand_param_right.plane_width 
						- p_avm_param->p_avm_config_param->vehicle_param.vehicle_width / 2);
					Float32_t angle_temp = atan(m / R);
					world_coordinate[0] = (world_coordinate_0[0] - x_z[0])*cos(angle_temp) 
						- (world_coordinate_0[1] - x_z[1])*sin(angle_temp) + x_z[0];///is z
					world_coordinate[2] = (world_coordinate_0[0] - x_z[0])*sin(angle_temp)
						+ (world_coordinate_0[1] - x_z[1])*cos(angle_temp) + x_z[1];//is y
					world_coordinate_to_pic_coordinate(CAM_RIGHT, world_coordinate, image_point, p_avm_param);

					UInt32_t  shift_altogether = i*left_right_view_param.result_image_width * 2 + j * 2;
					//single_view_zip_data(shift_altogether, image_point, p_lut, CAM_RIGHT);
					zip_data_new(shift_altogether, image_point, 1,
						p_lut, NULL, NULL, p_avm_param);//Ϊ�˴������ã�����ͼ��ʹû���ں�Ȩ��Ҳʹ����·ȫ���洢��ʽ�ò����Ĵ洢0
				}
			}
			/************left*******/
			if (j < left_right_view_param.result_image_width / 2)
			{

				world_coordinate_0[0] = -left_right_view_param.combine_expand_param_left.plane_width 
					- p_avm_param->p_avm_config_param->vehicle_param.vehicle_width / 2;
				world_coordinate_0[1] = 0;
				x_z[0] = -left_right_view_param.combine_expand_param_left.plane_width
					- p_avm_param->p_avm_config_param->vehicle_param.vehicle_width / 2;//�ӽ�ƽ�沿�ֵı߽��x��z������
				x_z[1] = R;//��ת��Բ��

				lr_calculate_world_coordinate_combine(world_coordinate, i, j, p_avm_param,
					left_right_view_param, CAM_LEFT);

				/************left*********/
				if (j >(left_right_view_param.result_image_width*0.5
					*(1.000 - left_right_view_param.combine_expand_param_left.plane_ratio)))
				{
					world_coordinate[2] = 0;
					world_coordinate_to_pic_coordinate(CAM_LEFT, world_coordinate, image_point, p_avm_param);

				}
				else
				{
					Float32_t m = (abs(world_coordinate[0])
						- left_right_view_param.combine_expand_param_left.plane_width 
						- p_avm_param->p_avm_config_param->vehicle_param.vehicle_width / 2);
					Float32_t angle_temp = -atan(m / R);
					world_coordinate[0] = ((world_coordinate_0[0] - x_z[0])*cos(angle_temp) 
						- (world_coordinate_0[1] - x_z[1])*sin(angle_temp) + x_z[0]);///is z
					world_coordinate[2] = (world_coordinate_0[0] - x_z[0])*sin(angle_temp) 
						+ (world_coordinate_0[1] - x_z[1])*cos(angle_temp) + x_z[1];//is y
					world_coordinate_to_pic_coordinate(CAM_LEFT, 
						world_coordinate, image_point, p_avm_param);

				}
			}

		}

	}
	return ret;
}





#if 0



/**************plane_front_rear_single_view***********************/
/*
* Function Name:				plane_front_rear_single_view
* Function Function:			plane_front_rear_single_view
* Input:
*       p_avm_param;
*		cam_flag;					 this is picture's flag is front or back
* Note:
*     the interface function should be called at the beginning of this module
* Revision:
*     Create by Cheng.Li on 2018/1/30.
*/
AVM_Error_Code_E column_front_rear_single_view(IN P_Avm_Param_S p_avm_param, IN  int cam_flag)
{
	AVM_Error_Code_E ret = ERROR_NONE;
	Front_Rear_Single_View_Param_S single_view_param;
	CvSize size0;
	UInt32_t* p_lut = NULL;
	if (cam_flag == CAM_FRONT)
	{
		single_view_param = p_avm_param->p_avm_config_param->avm_single_view_param.front_single_view_param;
		size0 = cvSize(single_view_param.result_image_width, single_view_param.result_image_height);//
		p_lut = p_avm_param->p_avm_lut->p_lut_single_view_front;
		//
	}
	if (cam_flag == _1_VIEW_REAR)
	{
		single_view_param = p_avm_param->p_avm_config_param->avm_single_view_param.rear_single_view_param;
		size0 = cvSize(single_view_param.result_image_width, single_view_param.result_image_height);//
		p_lut = p_avm_param->p_avm_lut->p_lut_single_view_rear;
		//
	}
#if DEBUG_INFO
	IplImage* image[4] = { NULL, NULL, NULL, NULL };
	ret = my_load_image(image);
	CHECK_ERROR(ret);
	IplImage *image_result_single_front_lut = cvCreateImage(size0, IPL_DEPTH_8U, 1);
	memset(image_result_single_front_lut->imageData, 0, single_view_param.result_image_width*single_view_param.result_image_height);
#endif
	Float32_t cam_ray[3];
	Float64_t tan_h = (tan((single_view_param.plane_expand_param.fov_h / 2 / 180)*PI));
	Float64_t tan_v = (tan((single_view_param.plane_expand_param.fov_v / 2 / 180)*PI));
	cam_ray[2] = -8000.00;//�������⽹��
	Float64_t  pixel_hight_dis = -(cam_ray[2] * tan_v) / (single_view_param.result_image_height / 2);//�����ͼ��ֱ������ÿ������ռ���ٺ���
	Float64_t  pixel_width_dis = -(cam_ray[2] * tan_h) / (single_view_param.result_image_width / 2);//�����ͼ��ˮƽ������ÿ������ռ���ٺ���


	for (int i = 0; i < single_view_param.result_image_height; i++)
	{
		for (int j = 0; j < single_view_param.result_image_width; j++)
		{
			/*if (cam_flag == CAM_FRONT)
			{*/
			cam_ray[1] = (Float64_t)(j - single_view_param.result_image_width / 2.00)* pixel_width_dis;//ת��Ϊ�������ϵ
			cam_ray[0] = (Float64_t)(i - single_view_param.result_image_height / 2.00)* pixel_hight_dis;//������x
			//}
			//if (cam_flag == CAM_REAR)
			//{
			//	cam_ray[1] = (Float64_t)((Float64_t(j) - ((Float64_t)(single_view_param.result_image_width) / 2.00))* pixel_width_dis);//ת��Ϊ�������ϵ
			//	cam_ray[0] = (Float64_t)((Float64_t(i) - ((Float64_t)(single_view_param.result_image_height) / 2.00))* pixel_hight_dis);
			//	//cam_ray[1] = (Float64_t)((Float64_t(single_view_param.result_image_width - j - 1) - ((Float64_t)(single_view_param.result_image_width) / 2.00))* pixel_width_dis);
			//	//cam_ray[0] = (Float64_t)((Float64_t(single_view_param.result_image_height - i - 1) - ((Float64_t)(single_view_param.result_image_height) / 2.00))* pixel_hight_dis);
			//}
			Float32_t image_point[2];
			Cam_Ray_To_Image_Point(image_point, cam_ray, &(p_avm_param->p_avm_config_param->camera_model[1].camera_model_int));
			UInt32_t  shift_altogether = i*single_view_param.result_image_width * 2 + j * 2;
			//single_view_zip_data(shift_altogether, image_point, p_lut, cam_flag);
			zip_data_new(shift_altogether, image_point, 1,
				p_lut, NULL, NULL, p_avm_param);//Ϊ�˴������ã�����ͼ��ʹû���ں�Ȩ��Ҳʹ����·ȫ���洢��ʽ�ò����Ĵ洢0
#if DEBUG_INFO
			int rgb[3];
			bilinear_interpolation(image[cam_flag], rgb, image_point[1], image_point[0]);
			image_result_single_front_lut->imageData[i *single_view_param.result_image_width + j] = (uchar)(rgb[0]);
#endif

		}
	}
#if DEBUG_INFO
	char result_name[MAX_ARRAY_SIZE];
	sprintf(result_name, "%s/single_plane_expend_front_bear.bmp", result_image_path);
	cvSaveImage(result_name, image_result_single_front_lut);
#endif
	return ret;
}


#endif
