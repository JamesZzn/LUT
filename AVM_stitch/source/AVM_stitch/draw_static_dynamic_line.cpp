#include"draw_static_dynamic_line.h"
#include"resolve_data.h"


#define DEBUG_AMBA_LINE 1

int height_coorld = 0, width_coord = 0, my_a = 0;


/**************plane_front_rear_single_view***********************/
/*���ʹ��
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
//#define DEBUG_INFO 1
AVM_Error_Code_E  plane_front_rear_single_view_calculate_key_point_coordinate_one_point(IN P_Avm_Param_S p_avm_param, 
	IN  int cam_flag, Float32_t world_coord[3], IN Int32_t key_ponit_pic_coordinate[2])
{
	AVM_Error_Code_E ret = ERROR_NONE;
	Front_Rear_Single_View_Param_S single_view_param;
	CvSize size0;
	UInt32_t* p_lut = NULL;
	Int32_t is_front_cam;
	Float32_t z_up_down, x_left_right_shift;;
	if (cam_flag == CAM_FRONT)
	{
		single_view_param = p_avm_param->p_avm_config_param
			->avm_single_view_param.front_single_view_param;
		size0 = cvSize(single_view_param.result_image_width,
			single_view_param.result_image_height);//
		p_lut = p_avm_param->p_avm_lut->p_lut_single_view_front;
		is_front_cam = 1;
		z_up_down = single_view_param.plane_expand_param.trans_y;
		x_left_right_shift = single_view_param.plane_expand_param.trans_x;
		//
	}
	if (cam_flag == CAM_REAR)
	{
		single_view_param = p_avm_param->p_avm_config_param
			->avm_single_view_param.rear_single_view_param;
		size0 = cvSize(single_view_param.result_image_width,
			single_view_param.result_image_height);//
		p_lut = p_avm_param->p_avm_lut->p_lut_single_view_rear;
		is_front_cam = -1;
		z_up_down = single_view_param.plane_expand_param.trans_y;
		x_left_right_shift = single_view_param.plane_expand_param.trans_x;
	}

	if (p_avm_param->p_avm_config_param
		->avm_single_view_param.front_single_view_param.expand_type == TYPE_CROP)//
	{
#if 1
	
		/***********world coordinate to pic coordinate**************/
		Float32_t  key_ponit_pic_coordinate_temp[2];
		
		world_coordinate_to_pic_coordinate(cam_flag, world_coord, key_ponit_pic_coordinate_temp, p_avm_param);//0 is front
		//key_ponit_pic_coordinate[0]=key_ponit_pic_coordinate_temp[0];
		//key_ponit_pic_coordinate[1]=key_ponit_pic_coordinate_temp[1];


		Float32_t scale_coordinate_col = Float32_t(single_view_param.crop_param.crop_rect.width)
			/ Float32_t(single_view_param.result_image_width);//���ԭͼ��ü��� �Ĵ�С����

		Float32_t scale_coordinate_row = Float32_t(single_view_param.crop_param.crop_rect.height)
			/ Float32_t(single_view_param.result_image_height);//���ԭͼ��ü��� �Ĵ�С����
		key_ponit_pic_coordinate[0] = (Int32_t)((key_ponit_pic_coordinate_temp[0] - single_view_param.crop_param.crop_rect.y) / scale_coordinate_row);
		key_ponit_pic_coordinate[1] = (Int32_t)((key_ponit_pic_coordinate_temp[1] - single_view_param.crop_param.crop_rect.x) / scale_coordinate_col);
		/*if (key_ponit_pic_coordinate_temp[0] < result_image_height&&key_ponit_pic_coordinate_temp[0]>0 && j < result_image_width&&j>0)
		{
		image_result_single_front_lut->imageData[key_ponit_pic_coordinate_temp[0] * result_image_width + j] = 255;
		}*/
	}
	if (p_avm_param->p_avm_config_param
		->avm_single_view_param.front_single_view_param.expand_type == TYPE_PLANE_EXPAND)
	{
#if 1

		Float32_t focal_length = 1000; // unit is mm
		Float32_t fov_h = (single_view_param.plane_expand_param.fov_h*PI) / 180;
		Float32_t fov_v = (single_view_param.plane_expand_param.fov_v*PI) / 180;
		Int32_t result_width = single_view_param.result_image_width;
		Int32_t result_height = single_view_param.result_image_height;

		// ����չ��ͼ��ÿ�������ڸ߿����������ʵ�ʾ���
		Float32_t	mm_per_pixel_h = focal_length * tan(fov_h * 0.5)
			* 2 / result_width;
		Float32_t  mm_per_pixel_v = focal_length * tan(fov_v * 0.5)
			* 2 / result_height;
		Float32_t pose_t_0[3];
		Float32_t pose_t[3];
		Float32_t inv_pose[3];
		for (int i = 3; i < 6; i++)
		{
			inv_pose[i - 3] = p_avm_param->p_avm_config_param
				->camera_model[cam_flag].camera_model_ext.inv_pose[i];
		}
		p_avm_param->p_avm_config_param->camera_model[cam_flag].camera_model_ext.inv_R;
		Mat_Mul_Vec_3by3(pose_t_0, p_avm_param->p_avm_config_param
			->camera_model[cam_flag].camera_model_ext.R, inv_pose);
		pose_t[0] = -pose_t_0[0];
		pose_t[1] = -pose_t_0[1];
		pose_t[2] = -pose_t_0[2];
		Float32_t pitch_angle;//�˴�����������¸�����
		if (cam_flag == CAM_FRONT)
		{
			pitch_angle = (Float32_t)(TILT_ANGLE_F * PI) / 180;//�˴�����������¸�����
		}
		if (cam_flag == CAM_REAR)
		{
			pitch_angle = (Float32_t)(TILT_ANGLE_B * PI) / 180;//�˴�����������¸�����
		}

		Float32_t init_world_coord_x = pose_t[0] - is_front_cam
			* mm_per_pixel_h * result_width * 0.5 + x_left_right_shift;
		Float32_t init_world_coord_y = pose_t[1] + is_front_cam
			* (focal_length * cos(pitch_angle) + mm_per_pixel_v
			* result_height * 0.5 * sin(pitch_angle));
		Float32_t init_world_coord_z = pose_t[2] - focal_length
			* sin(pitch_angle) + mm_per_pixel_v * result_height
			* 0.5 * cos(pitch_angle) + z_up_down;
		Float32_t delta_x = mm_per_pixel_h;
		Float32_t delta_y = mm_per_pixel_v * sin(pitch_angle);
		Float32_t delta_z = mm_per_pixel_v * cos(pitch_angle);
		
		

		if (cam_flag == CAM_FRONT)
		{
			//ֱ�߲���
			Float32_t  m1 = pose_t[0], m2 = pose_t[1], m3 = pose_t[2];//ֱ�߹�һ�����㼴 �������������ϵ������
			Float32_t  v1 = pose_t[0] - world_coord[0], v2 = pose_t[1]
				- world_coord[1], v3 = pose_t[2] - world_coord[2];//ֱ�ߵķ�������
			//ƽ�����
			//����ƽ��Ķ��㣬���������¸����򶨵�Ҳ���¸������˴�ѡȡ�Ķ��������+���⽹��
			Float32_t n1 = pose_t[0], n2, n3;
			n2 = (pose_t[1] + focal_length - pose_t[1])*cos(-pitch_angle)
				- (pose_t[2] - pose_t[2])*sin(-pitch_angle) + pose_t[1];///is y //������ת��ʽ
			n3 = (pose_t[1] + focal_length - pose_t[1])*sin(-pitch_angle)
				+ (pose_t[2] - pose_t[2])*cos(-pitch_angle) + pose_t[2];//is z
			Float32_t	vp1 = 0, vp2 = cos(pitch_angle), vp3 = -sin(pitch_angle);//���� ƽ��ķ�������
			Float32_t t = ((n1 - m1)*vp1 + (n2 - m2)*vp2 + (n3 - m3)*vp3)
				/ (vp1*v1 + vp2*v2 + vp3*v3);//��ֱ�ߵĲ������̺�ƽ��ĵ㷢�������ֱ����ƽ�������

			Float32_t x_intersection = m1 + v1*t;//���ֱ����ƽ���ϵĽ��� 
			Float32_t y_intersection = m2 + v2*t;
			Float32_t z_intersection = m3 + v3*t;
			key_ponit_pic_coordinate[1] = (Int32_t)((x_intersection - init_world_coord_x) / delta_x);
			key_ponit_pic_coordinate[0] = (Int32_t)((init_world_coord_z - z_intersection) / delta_z);//�ڴ˽���������ϵ�ľ���ӳ���ƽ��չ������������
		}
		if (cam_flag == CAM_REAR)
		{
			//ֱ�߲���
			Float32_t  m1 = pose_t[0], m2 = pose_t[1], m3 = pose_t[2];//ֱ�߹�һ�����㼴 �������������ϵ������
			Float32_t  v1 = pose_t[0] - world_coord[0], v2 = pose_t[1]
				- world_coord[1], v3 = pose_t[2] - world_coord[2];//ֱ�ߵķ�������

			//ƽ�����
			//����ƽ��Ķ��㣬���������¸����򶨵�Ҳ���¸������˴�ѡȡ�Ķ��������+���⽹��
			Float32_t n1 = pose_t[0], n2, n3;
			n2 = (pose_t[1] - focal_length - pose_t[1])*cos(pitch_angle)
				- (pose_t[2] - pose_t[2])*sin(pitch_angle) + pose_t[1];///is y //������ת��ʽ
			n3 = (pose_t[1] - focal_length - pose_t[1])*sin(pitch_angle)
				+ (pose_t[2] - pose_t[2])*cos(pitch_angle) + pose_t[2];//is z

			Float32_t	vp1 = 0, vp2 = -cos(pitch_angle), vp3 = -sin(pitch_angle);//���� ƽ��ķ�������
			Float32_t t = ((n1 - m1)*vp1 + (n2 - m2)*vp2 + (n3 - m3)*vp3)
				/ (vp1*v1 + vp2*v2 + vp3*v3);//��ֱ�ߵĲ������̺�ƽ��ĵ㷢�������ֱ����ƽ�������

			Float32_t x_intersection = m1 + v1*t;//���ֱ����ƽ���ϵĽ��� 
			Float32_t y_intersection = m2 + v2*t;
			Float32_t z_intersection = m3 + v3*t;
			key_ponit_pic_coordinate[1] = (Int32_t)(-(x_intersection
				- init_world_coord_x) / delta_x);//ͼ��ĺ�����
			key_ponit_pic_coordinate[0] = (Int32_t)((init_world_coord_z
				- z_intersection) / delta_z);//ͼ����������ڴ˽���������ϵ�ľ���ӳ���ƽ��չ������������
		}
	}
#endif
#endif
	return ret;







}




/*
*���ʹ��
* Function Name:				zip_data_draw_line
* Function Function:			zip_data_draw_line
* Input:
*       shift;               p_avm_param
*		image;                cam_flag
p_avm_param;
* Note:
*
* Revision:
*     Create by Cheng.Li on 2018/1/17.
*/
void zip_data_draw_line(IN int shift_altogether, IN UInt32_t*p_lut,
	IN UInt32_t camera_flag,int i,int j,int color_flag)
{
	p_lut[0] = shift_altogether;//
	//UInt32_t temp_shift = (i << 21) + (j << 10) + (color_flag << 2) + (camera_flag);
	UInt32_t temp_shift = (i << 21) + (j << 10) + color_flag ;
	p_lut[shift_altogether] = temp_shift;
}



/*
*���ʹ��
* Function Name:				zip_data_draw_line
* Function Function:			zip_data_draw_line
* Input:
*       shift;               p_avm_param
*		image;                cam_flag
p_avm_param;
* Note:
*
* Revision:
*     Create by Cheng.Li on 2018/1/17.
*/
void zip_data_draw_dynamic_line(IN int shift_altogether, IN UInt32_t*p_lut,
	IN UInt32_t camera_flag, int i, int j, int color_flag)
{
	p_lut[0] = shift_altogether;//
	UInt32_t temp_shift = (i << 21) + (j << 10) + color_flag ;
	p_lut[shift_altogether] = temp_shift;
}




/*
* Function Name:				draw_static_plane_front_rear_single_view_line
* Function Function:			draw_static_plane_front_rear_single_view_line
/*************************
* Input:
*       shift;               p_avm_param
*		image;                cam_flag
p_avm_param;
* Note:
*
* Revision:
*     Create by Cheng.Li on 2018/1/17.
*/


AVM_Error_Code_E original_crop_draw_static_plane_front_rear_single_view_line(IN P_Avm_Param_S p_avm_param, IN int cam_flag)
{


	AVM_Error_Code_E ret = ERROR_NONE;
	Front_Rear_Single_View_Param_S single_view_param;
	CvSize size0;
	UInt32_t* p_lut = NULL;
	UInt32_t* p_lut_line = NULL;
	if (cam_flag == CAM_FRONT)
	{
		single_view_param = p_avm_param->p_avm_config_param->avm_single_view_param.front_single_view_param;
		size0 = cvSize(single_view_param.result_image_width, single_view_param.result_image_height);//
		p_lut = p_avm_param->p_avm_lut->p_lut_single_view_front;
		p_lut_line = p_avm_param->p_avm_dis_line->p_static_dis_line_front;
	}
	if (cam_flag == CAM_REAR)
	{
		single_view_param = p_avm_param->p_avm_config_param->avm_single_view_param.rear_single_view_param;
		size0 = cvSize(single_view_param.result_image_width, single_view_param.result_image_height);//
		p_lut = p_avm_param->p_avm_lut->p_lut_single_view_rear;
		p_lut_line = p_avm_param->p_avm_dis_line->p_static_dis_line_rear;
	}
#if DEBUG_AMBA_LINE 
	CvSize size1;

	char result_name[MAX_ARRAY_SIZE];
	size1 = cvSize(single_view_param.result_image_width, single_view_param.result_image_height);
	IplImage *color_image = cvCreateImage(size1, IPL_DEPTH_8U, 3);
	memset(color_image->imageData, 0, single_view_param.result_image_width*single_view_param.result_image_height*3);
	IplImage *image_result_single_front_lut = NULL;
		sprintf(result_name, "%s/%s_%dX%d.bmp", result_image_path, view_name[cam_flag + 4],
			single_view_param.result_image_width, single_view_param.result_image_height);
	image_result_single_front_lut = cvLoadImage(result_name);
#endif
	int altogether_point_number=0;
	/*********************������***************************/
	Float32_t static_line_dis_to_car[4];//4�������ߵľ��복��ľ���

	Float32_t word_coordinate[3];//ӳ�䵽���ͼ�е���������ϵ�ı�����
	static_line_dis_to_car[0] = (single_view_param.dis_line_param.static_line_param.static_line_dis_to_car[0]
		+ p_avm_param->p_avm_config_param->vehicle_param.vehicle_length / 2.0);
	static_line_dis_to_car[1] = (single_view_param.dis_line_param.static_line_param.static_line_dis_to_car[1]
		+ p_avm_param->p_avm_config_param->vehicle_param.vehicle_length / 2.0);
	static_line_dis_to_car[2] = (single_view_param.dis_line_param.static_line_param.static_line_dis_to_car[2]
		+ p_avm_param->p_avm_config_param->vehicle_param.vehicle_length / 2.0);
	static_line_dis_to_car[3] = (single_view_param.dis_line_param.static_line_param.static_line_dis_to_car[3]
		+ p_avm_param->p_avm_config_param->vehicle_param.vehicle_length / 2.0);
	
	word_coordinate[0] = p_avm_param->p_avm_config_param->vehicle_param.vehicle_width / 2.0;// { -920, -2355, 0 };
	word_coordinate[1] = (p_avm_param->p_avm_config_param->vehicle_param.vehicle_length / 2.0);
	word_coordinate[2] = 0;
	Int32_t key_ponit_pic_coordinate[2];
	Float32_t dx = (Float32_t)single_view_param.dis_line_param.static_line_param.static_line_dis_to_car[3] / DEUCE_LONG;
	for (int i = 0; i < DEUCE_LONG; i++)
	{
		word_coordinate[1] = abs(word_coordinate[1]);//Ϊ�˴�������ȡ����ֵ��Ϊ�������и��ص�ǰ�����Ҷ��ǶԳƵ����Բ��þ���ֵ
		word_coordinate[1] = word_coordinate[1] + dx;
		if (cam_flag == CAM_REAR)
		{
			word_coordinate[1] = -word_coordinate[1];
		}
		if (cam_flag == CAM_FRONT)
		{
			word_coordinate[1] = word_coordinate[1];
		}
		for (int n = 0; n < 2; n++)
		{
			word_coordinate[0] = -word_coordinate[0];//��Ϊ�����ǶԳƵ�����ȡ���Ϳ��Լ��㳵�������ҶԳƵ�
			plane_front_rear_single_view_calculate_key_point_coordinate_one_point(p_avm_param, cam_flag, word_coordinate, key_ponit_pic_coordinate);
#if 1
			for (int j = key_ponit_pic_coordinate[1] - LINE_WIDTH_DENSE; j < key_ponit_pic_coordinate[1]; j++)
			{
				
				int color_flag = 0;
				if (key_ponit_pic_coordinate[0] < single_view_param.result_image_height&&key_ponit_pic_coordinate[0]>0
					&& j < single_view_param.result_image_width&&j>0)
				{
					altogether_point_number++;
					image_result_single_front_lut->imageData[key_ponit_pic_coordinate[0] * single_view_param.result_image_width + j] = 255;
					if (abs(word_coordinate[1]) <= abs(static_line_dis_to_car[0]))//��һ��������
					{
#if DEBUG_AMBA_LINE 
						color_image->imageData[key_ponit_pic_coordinate[0] * single_view_param.result_image_width * 3 + j * 3 + 2] = 255;//��
						//color_image->imageData[key_ponit_pic_coordinate[0] * single_view_param.result_image_width * 3 + j * 3 + 1] = 255;
#endif
						color_flag = COLOR_RED;
						
					}
					if (abs(word_coordinate[1]) <= abs(static_line_dis_to_car[1]) && abs(word_coordinate[1]) > abs(static_line_dis_to_car[0]))//��һ������ڶ���������
					{
#if DEBUG_AMBA_LINE 
						color_image->imageData[key_ponit_pic_coordinate[0] * single_view_param.result_image_width * 3 + j * 3 + 1] = 255;//��ɫ����
						color_image->imageData[key_ponit_pic_coordinate[0] * single_view_param.result_image_width * 3 + j * 3 + 2] = 255;
#endif
						color_flag = COLOR_YELLOW;
						
					}
					if (abs(word_coordinate[1]) <=abs(static_line_dis_to_car[2]) && abs(word_coordinate[1]) > abs(static_line_dis_to_car[1]))//��2�������3��������
					{
						color_image->imageData[key_ponit_pic_coordinate[0] * single_view_param.result_image_width * 3 + j * 3 + 1] = 255;
						color_flag = COLOR_GREEN;
						
					}
					if (abs(word_coordinate[1]) <=abs(static_line_dis_to_car[3]) && abs(word_coordinate[1]) > abs(static_line_dis_to_car[2]))//��2�������3��������
					{
						color_image->imageData[key_ponit_pic_coordinate[0] * single_view_param.result_image_width * 3 + j * 3 + 1] = 255;
						color_flag = COLOR_GREEN;
						//color_flag = 6;
					}
					
					zip_data_draw_line(altogether_point_number, p_lut_line, cam_flag, key_ponit_pic_coordinate[0], j, color_flag);
					
					
				}
				
			}//��һ����
#endif
			//Int32_t every_angle_size = DEFAULT_STATIC_DIS_LINE_SIZE;

			//every_angle_size=
			//p_lut_line[every_angle_size - n] = altogether_point_number;
		}


	}

#if 1
	/*******************������******************/
	for (int n = 0; n < 3; n++)
	{
		word_coordinate[0] = -(p_avm_param->p_avm_config_param->vehicle_param.vehicle_width / 2.0);// ���Ǵӳ������࿪ʼ
		word_coordinate[1] = p_avm_param->p_avm_config_param->vehicle_param.vehicle_length / 2.0 
			+ single_view_param.dis_line_param.static_line_param.static_line_dis_to_car[n];
		Float32_t dx = (Float32_t)p_avm_param->p_avm_config_param->vehicle_param.vehicle_width / DEUCE_LONG;
		for (int i = 0; i < DEUCE_LONG; i++)
		{
			word_coordinate[1] = abs(word_coordinate[1]);//Ϊ�˴�������ȡ����ֵ��Ϊ�������и��ص�ǰ�����Ҷ��ǶԳƵ����Բ��þ���ֵ
			word_coordinate[0] = word_coordinate[0] + dx;
			if (cam_flag == CAM_REAR)
			{
				word_coordinate[1] = -word_coordinate[1];//�󳵵�y����ȡ����
			}
			if (cam_flag == CAM_FRONT)
			{
				word_coordinate[1] = word_coordinate[1];
			}
			plane_front_rear_single_view_calculate_key_point_coordinate_one_point(p_avm_param, cam_flag, word_coordinate, key_ponit_pic_coordinate);
			for (int i = key_ponit_pic_coordinate[0] - LINE_WIDTH_DENSE; i < key_ponit_pic_coordinate[0]; i++)
			{
				if (key_ponit_pic_coordinate[0] < single_view_param.result_image_height&&key_ponit_pic_coordinate[0]>0 && i < single_view_param.result_image_height&&i>0)
				{
					altogether_point_number++;
					int color_flag = 0;
					image_result_single_front_lut->imageData[i * single_view_param.result_image_width + key_ponit_pic_coordinate[1]] = 255;
					if (n == 0)
					{
						color_image->imageData[i * single_view_param.result_image_width * 3 + key_ponit_pic_coordinate[1] * 3 + 2] = 255;
						color_image->imageData[i * single_view_param.result_image_width * 3 + key_ponit_pic_coordinate[1] * 3 + 0] = 0;
						color_image->imageData[i * single_view_param.result_image_width * 3 + key_ponit_pic_coordinate[1] * 3 + 1] = 0;
						color_flag = COLOR_RED;
						
					}
					if (n == 1)
					{
						color_image->imageData[i * single_view_param.result_image_width * 3 + key_ponit_pic_coordinate[1] * 3 + 1] = 255;
						color_image->imageData[i * single_view_param.result_image_width * 3 + key_ponit_pic_coordinate[1] * 3 + 2] = 255;
						color_flag = COLOR_YELLOW;
						
					}
					if (n == 2)
					{
						color_image->imageData[i * single_view_param.result_image_width * 3 + key_ponit_pic_coordinate[1] * 3+1] = 255;
						color_flag = COLOR_GREEN;
						
					}
					zip_data_draw_line(altogether_point_number, p_lut_line, cam_flag, i, key_ponit_pic_coordinate[1], color_flag);
				}
				
			}
		}//��������
		//Int32_t every_angle_size = DEFAULT_STATIC_DIS_LINE_SIZE;
		//every_angle_size=
		//p_lut_line[every_angle_size - n-2] = altogether_point_number;
	}
#endif
#if DEBUG_AMBA_LINE
		//char result_name[MAX_ARRAY_SIZE];
		sprintf(result_name, "%s/distance_line/original_line%d��̬��.bmp", result_image_path, cam_flag);
		cvSaveImage(result_name, image_result_single_front_lut);
		sprintf(result_name, "%s/distance_line/color_line%d��̬��.bmp", result_image_path, cam_flag);
		cvSaveImage(result_name, color_image);
		cvReleaseImage(&color_image);
		cvReleaseImage(&image_result_single_front_lut);

#endif

	return ret;

}


#if 0
/*
* Function Name:				draw_static_plane_front_rear_single_view_line
* Function Function:			draw_static_plane_front_rear_single_view_line
/*************************
* Input:
*       shift;               p_avm_param
*		image;                cam_flag
p_avm_param;
* Note:
*
* Revision:
*     Create by Cheng.Li on 2018/1/17.
*/


AVM_Error_Code_E static_line_left_right(IN P_Avm_Param_S p_avm_param, IN int cam_flag)
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
		/ (Float32_t)(left_right_view_param.result_image_width / 2);
	Float32_t dy_left = (left_right_view_param.plane_expand_param_left.world_height)
		/ (Float32_t)(left_right_view_param.result_image_height);
	Float32_t dx_right = (left_right_view_param.plane_expand_param_right.world_width)
		/ (Float32_t)(left_right_view_param.result_image_width / 2);
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

	world_init_x_right = (p_avm_param->p_avm_config_param->vehicle_param.vehicle_width * 0.5 - left_right_view_param.plane_expand_param_right.car_body_width * dx_right);
	//world_init_x_right = world_init_x_right -500;
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
			//single_view_zip_data(shift_altogether, image_point, p_lut, 2);
			zip_data_new(shift_altogether, image_point, 1,
				p_lut, NULL, NULL, p_avm_param);//Ϊ�˴������ã�����ͼ��ʹû���ں�Ȩ��Ҳʹ����·ȫ���洢��ʽ�ò����Ĵ洢0

		}
	}

#if DEBUG_AMBA_LINE 
	CvSize size1;
	int rgb[3];
	char result_name[MAX_ARRAY_SIZE];
	size1 = cvSize(single_view_param.result_image_width, single_view_param.result_image_height);
	IplImage *color_image = cvCreateImage(size1, IPL_DEPTH_8U, 3);
	memset(color_image->imageData, 0, single_view_param.result_image_width*single_view_param.result_image_height * 3);
	IplImage *image_result_single_front_lut = NULL;
	sprintf(result_name, "%s/%s_%dX%d.bmp", result_image_path, view_name[cam_flag + 4],
		single_view_param.result_image_width, single_view_param.result_image_height);
	image_result_single_front_lut = cvLoadImage(result_name);
#endif
	int altogether_point_number = 1;
	/*********************������***************************/
	Float32_t word_coordinate[3];//ӳ�䵽���ͼ�е���������ϵ�ı�����
	Int32_t key_ponit_pic_coordinate[2];
#if 1

	/*******************������******************/
	for (int n = 0; n < 4; n++)
	{
		word_coordinate[0] = (p_avm_param->p_avm_config_param->vehicle_param.vehicle_width / 2.0
			+ single_view_param.dis_line_param.static_line_param.static_line_dis_to_car[n]);// ���Ǵӳ������࿪ʼ
		word_coordinate[1] = world_init_y_left*0.8;
		if (cam_flag == CAM_LEFT)
		{
			word_coordinate[0] = -word_coordinate[0];//�󳵵�y����ȡ����
		}
		if (cam_flag == CAM_RIGHT)
		{
			word_coordinate[0] = word_coordinate[0];
		}
		Float32_t dy_deuce = ((Float32_t)single_view_param.plane_expand_param.world_width*line_length) / DEUCE_LONG;
		for (int i = 0; i < DEUCE_LONG; i++)
		{
			word_coordinate[1] = word_coordinate[1] - dy_deuce;
			key_ponit_pic_coordinate[0] = -(word_coordinate[1] - world_init_y_left) / dy;
			key_ponit_pic_coordinate[1] = -(word_coordinate[0] - world_init_x_left) / dx;
			key_ponit_pic_coordinate[1] = single_view_param.result_image_width - key_ponit_pic_coordinate[1];
			for (int i = key_ponit_pic_coordinate[0] - LINE_WIDTH; i < key_ponit_pic_coordinate[0]; i++)
			{
				altogether_point_number++;
				int color_flag = 0;

				if (key_ponit_pic_coordinate[0] < single_view_param.result_image_height&&key_ponit_pic_coordinate[0]>0 && i < single_view_param.result_image_height&&i>0)
				{
					image_result_single_front_lut->imageData[i * single_view_param.result_image_width + key_ponit_pic_coordinate[1]] = 255;
					if (n == 0)
					{
						color_image->imageData[i * single_view_param.result_image_width * 3 + key_ponit_pic_coordinate[1] * 3 + 2] = 255;
						color_image->imageData[i * single_view_param.result_image_width * 3 + key_ponit_pic_coordinate[1] * 3 + 0] = 0;
						color_image->imageData[i * single_view_param.result_image_width * 3 + key_ponit_pic_coordinate[1] * 3 + 1] = 0;
						color_flag = COLOR_RED;

					}
					if (n == 1)
					{
						color_image->imageData[i * single_view_param.result_image_width * 3 + key_ponit_pic_coordinate[1] * 3 + 1] = 255;
						//color_image->imageData[i * single_view_param.result_image_width * 3 + key_ponit_pic_coordinate[1] * 3 + 2] = 255;
						color_flag = COLOR_YELLOW;

					}
					if (n == 2)
					{
						color_image->imageData[i * single_view_param.result_image_width * 3 + key_ponit_pic_coordinate[1] * 3 + 1] = 255;
						color_flag = COLOR_GREEN;

					}
				}
				zip_data_draw_line(altogether_point_number, p_lut_line, cam_flag, i, key_ponit_pic_coordinate[1], color_flag);
			}
		}
	}
#endif
#if DEBUG_AMBA_LINE
	//char result_name[MAX_ARRAY_SIZE];
	sprintf(result_name, "%s/distance_line/original_line%d��̬��.bmp", result_image_path, cam_flag);
	cvSaveImage(result_name, image_result_single_front_lut);
	sprintf(result_name, "%s/distance_line/color_line%d��̬��.bmp", result_image_path, cam_flag);
	cvSaveImage(result_name, color_image);
	cvReleaseImage(&color_image);
	cvReleaseImage(&image_result_single_front_lut);

#endif

	return ret;

}

#endif

#if 1
/*
* Function Name:				draw_static_plane_front_rear_single_view_line
* Function Function:			draw_static_plane_front_rear_single_view_line
/*************************
* Input:
*       shift;               p_avm_param
*		image;                cam_flag
p_avm_param;
* Note:
*
* Revision:
*     Create by Cheng.Li on 2018/1/17.
*/


AVM_Error_Code_E static_line_left_right(IN P_Avm_Param_S p_avm_param, IN int cam_flag)
{


	AVM_Error_Code_E ret = ERROR_NONE;
	Left_Right_Single_View_Param_S single_view_param;
	CvSize size0;
	UInt32_t* p_lut = NULL;
	UInt32_t* p_lut_line = NULL;
	Float32_t line_length;
	if (cam_flag == CAM_LEFT)
	{
		single_view_param = p_avm_param->p_avm_config_param->avm_single_view_param.left_single_view_param;
		size0 = cvSize(single_view_param.result_image_width, single_view_param.result_image_height);//
		p_lut_line = p_avm_param->p_avm_dis_line->p_static_dis_line_left;
		line_length = (Float32_t)0.2;

	}
	if (cam_flag == CAM_RIGHT)
	{
		single_view_param = p_avm_param->p_avm_config_param->avm_single_view_param.right_single_view_param;
		size0 = cvSize(single_view_param.result_image_width, single_view_param.result_image_height);//
		p_lut_line = p_avm_param->p_avm_dis_line->p_static_dis_line_right;
		line_length = (Float32_t)0.3;
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

#if DEBUG_AMBA_LINE 
	CvSize size1;

	char result_name[MAX_ARRAY_SIZE];
	size1 = cvSize(single_view_param.result_image_width, single_view_param.result_image_height);
	IplImage *color_image = cvCreateImage(size1, IPL_DEPTH_8U, 3);
	memset(color_image->imageData, 0, single_view_param.result_image_width*single_view_param.result_image_height * 3);
	IplImage *image_result_single_front_lut = NULL;
	sprintf(result_name, "%s/%s_%dX%d.bmp", result_image_path, view_name[cam_flag + 4],
		single_view_param.result_image_width, single_view_param.result_image_height);
	image_result_single_front_lut = cvLoadImage(result_name);
#endif
	int altogether_point_number = 1;
	/*********************������***************************/
	Float32_t word_coordinate[3];//ӳ�䵽���ͼ�е���������ϵ�ı�����
	Int32_t key_ponit_pic_coordinate[2];
#if 1
	
	/*******************������******************/
	for (int n = 0; n < 4; n++)
	{
		word_coordinate[0] = (p_avm_param->p_avm_config_param->vehicle_param.vehicle_width / 2.0 
			+ single_view_param.dis_line_param.static_line_param.static_line_dis_to_car[n]);// ���Ǵӳ������࿪ʼ
		word_coordinate[1] = world_init_y_left*0.8;
		if (cam_flag == CAM_LEFT)
		{
			word_coordinate[0] = -word_coordinate[0];//�󳵵�y����ȡ����
		}
		if (cam_flag == CAM_RIGHT)
		{
			word_coordinate[0] = word_coordinate[0];
		}
		Float32_t dy_deuce = ((Float32_t)single_view_param.plane_expand_param.world_width*line_length) / DEUCE_LONG;
		for (int i = 0; i < DEUCE_LONG; i++)
		{
			word_coordinate[1] = word_coordinate[1] - dy_deuce;
			key_ponit_pic_coordinate[0] = (Int32_t)(-(word_coordinate[1] - world_init_y_left) / dy);
			key_ponit_pic_coordinate[1] = (Int32_t)(-(word_coordinate[0] - world_init_x_left) / dx);
			key_ponit_pic_coordinate[1] = (Int32_t)(single_view_param.result_image_width - key_ponit_pic_coordinate[1]);
			for (int i = key_ponit_pic_coordinate[0] - LINE_WIDTH; i < key_ponit_pic_coordinate[0]; i++)
			{
				altogether_point_number++;
				int color_flag = 0;

				if (key_ponit_pic_coordinate[0] < single_view_param.result_image_height&&key_ponit_pic_coordinate[0]>0 && i < single_view_param.result_image_height&&i>0)
				{
					image_result_single_front_lut->imageData[i * single_view_param.result_image_width + key_ponit_pic_coordinate[1]] = 255;
					if (n == 0)
					{
						color_image->imageData[i * single_view_param.result_image_width * 3 + key_ponit_pic_coordinate[1] * 3 + 2] = 255;
						color_image->imageData[i * single_view_param.result_image_width * 3 + key_ponit_pic_coordinate[1] * 3 + 0] = 0;
						color_image->imageData[i * single_view_param.result_image_width * 3 + key_ponit_pic_coordinate[1] * 3 + 1] = 0;
						color_flag = COLOR_RED;

					}
					if (n == 1)
					{
						color_image->imageData[i * single_view_param.result_image_width * 3 + key_ponit_pic_coordinate[1] * 3 + 1] = 255;
						//color_image->imageData[i * single_view_param.result_image_width * 3 + key_ponit_pic_coordinate[1] * 3 + 2] = 255;
						color_flag = COLOR_YELLOW;

					}
					if (n == 2)
					{
						color_image->imageData[i * single_view_param.result_image_width * 3 + key_ponit_pic_coordinate[1] * 3 + 1] = 255;
						color_flag = COLOR_GREEN;

					}
				}
				zip_data_draw_line(altogether_point_number, p_lut_line, cam_flag, i, key_ponit_pic_coordinate[1], color_flag);
			}
		}
	}
#endif
#if DEBUG_AMBA_LINE
	//char result_name[MAX_ARRAY_SIZE];
	sprintf(result_name, "%s/distance_line/original_line%d��̬��.bmp", result_image_path, cam_flag);
	cvSaveImage(result_name, image_result_single_front_lut);
	sprintf(result_name, "%s/distance_line/color_line%d��̬��.bmp", result_image_path, cam_flag);
	cvSaveImage(result_name, color_image);
	cvReleaseImage(&color_image);
	cvReleaseImage(&image_result_single_front_lut);

#endif

	return ret;

}

#endif



#if 1


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

AVM_Error_Code_E lr_single_static_line(IN P_Avm_Param_S p_avm_param)
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
		/ (Float32_t)(left_right_view_param.result_image_width / 2);
	Float32_t dy_left = (left_right_view_param.plane_expand_param_left.world_height)
		/ (Float32_t)(left_right_view_param.result_image_height);
	Float32_t dx_right = (left_right_view_param.plane_expand_param_right.world_width)
		/ (Float32_t)(left_right_view_param.result_image_width / 2);
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

	world_init_x_right = (p_avm_param->p_avm_config_param->vehicle_param.vehicle_width * 0.5 - left_right_view_param.plane_expand_param_right.car_body_width * dx_right);
	world_init_y_right = left_right_view_param.plane_expand_param_right.world_height * 0.5
		- left_right_view_param.plane_expand_param_right.trans_y;

	Float32_t word_coordinate[3];//ӳ�䵽���ͼ�е���������ϵ�ı�����
	Int32_t key_ponit_pic_coordinate[2];
	Float32_t line_length=1.2;
#if DEBUG_AMBA_LINE 
	CvSize size1;

	char result_name[MAX_ARRAY_SIZE];
	size1 = cvSize(left_right_view_param.result_image_width, left_right_view_param.result_image_height);
	IplImage *color_image = cvCreateImage(size1, IPL_DEPTH_8U, 3);
	memset(color_image->imageData, 0, left_right_view_param.result_image_width*left_right_view_param.result_image_height * 3);
	IplImage *image_result_single_front_lut = NULL;
	sprintf(result_name, "%s/%s_%dX%d.bmp", result_image_path, view_name[8],
		left_right_view_param.result_image_width, left_right_view_param.result_image_height);
	image_result_single_front_lut = cvLoadImage(result_name);
#endif
	int altogether_point_number = 1;


	for (int n = 0; n < 4; n++)
	{
		word_coordinate[0] = (p_avm_param->p_avm_config_param->vehicle_param.vehicle_width / 2.0
			+ p_avm_param->p_avm_config_param->avm_single_view_param.left_single_view_param.
			dis_line_param.static_line_param.static_line_dis_to_car[n]);// ���Ǵӳ������࿪ʼ
		word_coordinate[1] = world_init_y_left*0.8;
		word_coordinate[0] = -word_coordinate[0];//�󳵵�y����ȡ����
	    Float32_t dy_deuce = (p_avm_param->p_avm_config_param->avm_single_view_param.
		  left_single_view_param.dis_line_param.static_line_param.static_line_length*line_length) / DEUCE_LONG;


		for (int i = 0; i < DEUCE_LONG; i++)
		{
			word_coordinate[1] = word_coordinate[1] - dy_deuce;
			key_ponit_pic_coordinate[1] = (Int32_t)((word_coordinate[0] - world_init_x_left) / dx_left);
			key_ponit_pic_coordinate[0] = (Int32_t)(-(word_coordinate[1] - world_init_y_left) / dy_left);
			
			for (int i = key_ponit_pic_coordinate[0] - LINE_WIDTH; i < key_ponit_pic_coordinate[0]; i++)
			{
				altogether_point_number++;
				int color_flag = 0;

				if (key_ponit_pic_coordinate[0] < left_right_view_param.result_image_height&&key_ponit_pic_coordinate[0]>0 && i < left_right_view_param.result_image_height&&i>0)
				{
					image_result_single_front_lut->imageData[i * left_right_view_param.result_image_width + key_ponit_pic_coordinate[1]] = 255;
					if (n == 0)
					{
						color_image->imageData[i * left_right_view_param.result_image_width * 3 + key_ponit_pic_coordinate[1] * 3 + 2] = 255;
						color_image->imageData[i * left_right_view_param.result_image_width * 3 + key_ponit_pic_coordinate[1] * 3 + 0] = 0;
						color_image->imageData[i * left_right_view_param.result_image_width * 3 + key_ponit_pic_coordinate[1] * 3 + 1] = 0;
						color_flag = COLOR_RED;

					}
					if (n == 1)
					{
						color_image->imageData[i * left_right_view_param.result_image_width * 3 + key_ponit_pic_coordinate[1] * 3 + 1] = 255;
						//color_image->imageData[i * single_view_param.result_image_width * 3 + key_ponit_pic_coordinate[1] * 3 + 2] = 255;
						color_flag = COLOR_YELLOW;

					}
					if (n == 2)
					{
						color_image->imageData[i * left_right_view_param.result_image_width * 3 + key_ponit_pic_coordinate[1] * 3 + 1] = 255;
						color_flag = COLOR_GREEN;

					}
				}
				zip_data_draw_line(altogether_point_number, p_lut, 2, i, key_ponit_pic_coordinate[1], color_flag);
			}
		}
	}


	for (int n = 0; n < 4; n++)
	{
		word_coordinate[0] = (p_avm_param->p_avm_config_param->vehicle_param.vehicle_width / 2.0
			+ p_avm_param->p_avm_config_param->avm_single_view_param.left_single_view_param.
			dis_line_param.static_line_param.static_line_dis_to_car[n]);// ���Ǵӳ������࿪ʼ
		word_coordinate[1] = world_init_y_left*0.8;
		
		word_coordinate[0] = word_coordinate[0];
		
		Float32_t dy_deuce = (p_avm_param->p_avm_config_param->avm_single_view_param.
			left_single_view_param.dis_line_param.static_line_param.static_line_length*line_length) / DEUCE_LONG;
		for (int i = 0; i < DEUCE_LONG; i++)
		{
			word_coordinate[1] = word_coordinate[1] - dy_deuce;
			key_ponit_pic_coordinate[1] = (Int32_t)((word_coordinate[0] - world_init_x_right) / dx_left + left_right_view_param.result_image_width / 2);
			key_ponit_pic_coordinate[0] = (Int32_t)(-(word_coordinate[1] - world_init_y_right) / dy_left);

			for (int i = key_ponit_pic_coordinate[0] - LINE_WIDTH; i < key_ponit_pic_coordinate[0]; i++)
			{
				altogether_point_number++;
				int color_flag = 0;

				if (key_ponit_pic_coordinate[0] < left_right_view_param.result_image_height&&key_ponit_pic_coordinate[0]>0 && i < left_right_view_param.result_image_height&&i>0)
				{
					image_result_single_front_lut->imageData[i * left_right_view_param.result_image_width + key_ponit_pic_coordinate[1]] = 255;
					if (n == 0)
					{
						color_image->imageData[i * left_right_view_param.result_image_width * 3 + key_ponit_pic_coordinate[1] * 3 + 2] = 255;
						color_image->imageData[i * left_right_view_param.result_image_width * 3 + key_ponit_pic_coordinate[1] * 3 + 0] = 0;
						color_image->imageData[i * left_right_view_param.result_image_width * 3 + key_ponit_pic_coordinate[1] * 3 + 1] = 0;
						color_flag = COLOR_RED;

					}
					if (n == 1)
					{
						color_image->imageData[i * left_right_view_param.result_image_width * 3 + key_ponit_pic_coordinate[1] * 3 + 1] = 255;
						//color_image->imageData[i * single_view_param.result_image_width * 3 + key_ponit_pic_coordinate[1] * 3 + 2] = 255;
						color_flag = COLOR_YELLOW;

					}
					if (n == 2)
					{
						color_image->imageData[i * left_right_view_param.result_image_width * 3 + key_ponit_pic_coordinate[1] * 3 + 1] = 255;
						color_flag = COLOR_GREEN;

					}
				}
				zip_data_draw_line(altogether_point_number, p_lut, 2, i, key_ponit_pic_coordinate[1], color_flag);
			}
		}
	}
	sprintf(result_name, "%s/distance_line/����riginal_line%d��̬��.bmp", result_image_path, 5);
	cvSaveImage(result_name, image_result_single_front_lut);
	sprintf(result_name, "%s/distance_line/color_line%d��̬��.bmp", result_image_path, 5);
	cvSaveImage(result_name, color_image);
	cvReleaseImage(&color_image);
	cvReleaseImage(&image_result_single_front_lut);
	return ret;
}
#endif


/**************plane_front_rear_single_view***********************/
/*���ʹ��
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
//#define DEBUG_INFO 1
AVM_Error_Code_E  left_right_calculate_key_point_coordinate_one_point(IN P_Avm_Param_S p_avm_param,
	IN  int cam_flag, Float32_t world_coord[3], IN Int32_t key_ponit_pic_coordinate[2])
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

	key_ponit_pic_coordinate[0] = (Int32_t)(-(world_coord[1] - world_init_y_left) / dy);
	key_ponit_pic_coordinate[1] = (Int32_t)(-(world_coord[0] - world_init_x_left) / dx);

#if 0
	/********************extend all over result image pixel****************************/
	for (int i = 0; i < single_view_param.result_image_height; i++)
	{
		for (int j = 0; j < single_view_param.result_image_width; j++)
		{
			Float32_t world_coord[3];
			world_coord[0] = world_init_x_left + (j)* dx;
			world_coord[1] = world_init_y_left - (i)* dy;
			world_coord[2] = 0;
			Float32_t image_point[2];
			world_coordinate_to_pic_coordinate(cam_flag, world_coord,
				image_point, p_avm_param);
			UInt32_t  shift_altogether = i*single_view_param.result_image_width * 2
				+ j * 2;
			
		
		}
	}
#endif

	return ret;






}

/*
���ʹ��
* Function Name:				draw_circle_trail
* Function Function:			draw_circle_trail
/*************************
* Input:
*       shift;               p_avm_param
*		image;                cam_flag
p_avm_param;
* Note:
*
* Revision:
*     Create by Cheng.Li on 2018/1/17.
*/
//#define DEBUG_INFO_0 1

void draw_circle_trail(Int32_t rotate_flag,Float32_t centre_rotate_coordinate[3], 
						Float32_t car_head[3], 
						Float32_t angle_average,
						P_Avm_Param_S p_avm_param,
						Float32_t  pixel_hight_dis,
						Float32_t  pixel_width_dis,
						Int32_t dynamic_line_length,
						Float32_t *cam_ray,
						Int32_t cam_flag,
						UInt32_t * p_lut_line,
						UInt32_t *p_rotate_altogether_add,
						Int32_t result_image_height,
						Int32_t result_image_width, IplImage *image_result_single_front_lut, 
						Int32_t *every_angle_point_num, Float32_t rotate_angle)
{
	
	Float32_t angle_base = 0;
	int pic_coor_temp[2];
	int my_c = 0;
	for (int i = 0; i < EQUALITY_ANGLE; i++)
	{
		Float32_t world_circle[3] = { 0, 0, 0 };
		angle_base = angle_base + angle_average;

		world_circle[0] = (car_head[0] - centre_rotate_coordinate[0])*cos(angle_base) //������ת��ʽ
			- (car_head[1] - centre_rotate_coordinate[1])*sin(angle_base)
			+ centre_rotate_coordinate[0];///is x
		world_circle[1] = (car_head[0] - centre_rotate_coordinate[0])*sin(angle_base)
			+ (car_head[1] - centre_rotate_coordinate[1])*cos(angle_base)
			+ centre_rotate_coordinate[1];//is y
		int key_ponit_pic_coordinate_temp[2];
		if (rotate_flag == ROTATE_PLANE_EXPEND_FRONT || rotate_flag == ROTATE_PLANE_EXPEND_REAR)//ƽ��չ���Ķ�̬�� 
		{
			plane_front_rear_single_view_calculate_key_point_coordinate_one_point(p_avm_param, cam_flag, world_circle, key_ponit_pic_coordinate_temp);
		}
		if (rotate_flag == ROTATE_4_VIEW_BIG_REAR || rotate_flag == ROTATE_4_VIEW_BIG_FRONT)//4·����ͼ��ͼ�Ķ�̬��
		{
			key_ponit_pic_coordinate_temp[0] = (Int32_t)(p_avm_param->p_avm_config_param->avm_2d_param.junc_view_4_param.result_big_param.car_logo_rect.y
				+ p_avm_param->p_avm_config_param->avm_2d_param.junc_view_4_param.result_big_param.car_logo_rect.height / 2
				- (world_circle[1] / pixel_hight_dis));
			key_ponit_pic_coordinate_temp[1] = (Int32_t)(p_avm_param->p_avm_config_param->avm_2d_param.junc_view_4_param.result_big_param.car_logo_rect.x
				+ p_avm_param->p_avm_config_param->avm_2d_param.junc_view_4_param.result_big_param.car_logo_rect.width / 2
					+ (world_circle[0] / pixel_width_dis));
				
		}

		if (rotate_flag == ROTATE_4_VIEW_SMALL_FRONT || rotate_flag == ROTATE_4_VIEW_SMALL_REAR)//4·����ͼСͼ�Ķ�̬��
		{
			key_ponit_pic_coordinate_temp[0] = (Int32_t)(p_avm_param->p_avm_config_param->avm_2d_param.junc_view_4_param.result_small_param.car_logo_rect.y
				+ p_avm_param->p_avm_config_param->avm_2d_param.junc_view_4_param.result_small_param.car_logo_rect.height / 2
				- (world_circle[1] / pixel_hight_dis));
			key_ponit_pic_coordinate_temp[1] = (Int32_t)(p_avm_param->p_avm_config_param->avm_2d_param.junc_view_4_param.result_small_param.car_logo_rect.x
				+ p_avm_param->p_avm_config_param->avm_2d_param.junc_view_4_param.result_small_param.car_logo_rect.width / 2
				+ (world_circle[0] / pixel_width_dis));

		}
		if (rotate_flag == ROTATE_3_VIEW_FRONT)//3ǰ·����ͼ��ͼ�Ķ�̬��
		{
			Float32_t m = p_avm_param->p_avm_config_param->vehicle_param.vehicle_length / 2 - p_avm_param->p_avm_config_param->avm_2d_param.junc_view_3_front_param.common_config.car_head_length;
			key_ponit_pic_coordinate_temp[0] = (Int32_t)(result_image_height - (world_circle[1] - m) / pixel_hight_dis);
			key_ponit_pic_coordinate_temp[1] = (Int32_t)(p_avm_param->p_avm_config_param->avm_2d_param.junc_view_3_front_param.result_3_front_param.car_logo_rect.x
				+ p_avm_param->p_avm_config_param->avm_2d_param.junc_view_3_front_param.result_3_front_param.car_logo_rect.width / 2
				+ (world_circle[0] / pixel_width_dis));

		} 

		if (rotate_flag == ROTATE_3_VIEW_REAR)//3��·����ͼ��ͼ�Ķ�̬��
		{
			Float32_t m = (p_avm_param->p_avm_config_param->vehicle_param.vehicle_length / 2 
				- p_avm_param->p_avm_config_param->avm_2d_param.junc_view_3_rear_param.common_config.car_rear_length);//�������һ���ȥ����ͼ©���ĳ�β��
			key_ponit_pic_coordinate_temp[0] = (Int32_t)(-(world_circle[1] + m) / pixel_hight_dis);/* p_avm_param->p_avm_config_param->avm_2d_param.junc_view_3_front_param.result_3_front_param.car_logo_rect.y
																											 - */
			key_ponit_pic_coordinate_temp[1] = (Int32_t)(p_avm_param->p_avm_config_param->avm_2d_param.junc_view_3_rear_param.result_3_rear_param.car_logo_rect.x
				+ p_avm_param->p_avm_config_param->avm_2d_param.junc_view_3_rear_param.result_3_rear_param.car_logo_rect.width / 2
				+ (world_circle[0] / pixel_width_dis));

		}
#if 0
		//���ؼ���
		if ((pic_coor_temp[0] != key_ponit_pic_coordinate_temp[0] || pic_coor_temp[1] != key_ponit_pic_coordinate_temp[1]))
		{
			for (int j = key_ponit_pic_coordinate_temp[1] - LINE_WIDTH; j < key_ponit_pic_coordinate_temp[1]; j++)
			{
				cpu_fill_point(every_angle_point_num, p_lut_line, image_result_single_front_lut, 
					cam_flag,key_ponit_pic_coordinate_temp,  j,  i,
					result_image_height,
					result_image_width, car_head, rotate_flag);
			}
		}
#endif
#if 1
		
		//ÿһ��ֻ��һ������
		if (pic_coor_temp[0] == key_ponit_pic_coordinate_temp[0] || i == EQUALITY_ANGLE-1)//pic_coor_temp[0] == key_ponit_pic_coordinate_temp[0] || pic_coor_temp[1] == key_ponit_pic_coordinate_temp[1]
		{
			for (int j = key_ponit_pic_coordinate_temp[1] - LINE_WIDTH; j < key_ponit_pic_coordinate_temp[1]; j++)
			{
				gpu_fill_point(every_angle_point_num, p_lut_line, image_result_single_front_lut,
					cam_flag, key_ponit_pic_coordinate_temp, j, i,
					result_image_height,
					result_image_width, car_head, rotate_flag);
			}
		}
		else
		{
			for (int j = key_ponit_pic_coordinate_temp[1] - LINE_WIDTH; j < key_ponit_pic_coordinate_temp[1]; j++)
			{
				gpu_fill_point(every_angle_point_num, p_lut_line, image_result_single_front_lut,
					cam_flag, key_ponit_pic_coordinate_temp, j, i,
					result_image_height,
					result_image_width, car_head, rotate_flag);
			}


		}
#endif

#if 0
		///CVBSCPU����
		if ((pic_coor_temp[0] != key_ponit_pic_coordinate_temp[0] || pic_coor_temp[1] != key_ponit_pic_coordinate_temp[1]) || (i == EQUALITY_ANGLE - 2))
		{
			for (int j = key_ponit_pic_coordinate_temp[1] - LINE_WIDTH; j < key_ponit_pic_coordinate_temp[1]; j++)
			{
				cpu_fill_point(every_angle_point_num, p_lut_line, image_result_single_front_lut, 
					 cam_flag,key_ponit_pic_coordinate_temp,  j,  i,
					result_image_height,
					result_image_width, car_head, rotate_flag);
			}
		}
#endif
		pic_coor_temp[0] = key_ponit_pic_coordinate_temp[0];//�����ϴλ����ĵ�������Ա㵱��һ�ν��бȽ��Ա�ȷ������ĵ�û���ظ���
		pic_coor_temp[1] = key_ponit_pic_coordinate_temp[1];
	}
	
}



/*
*Function Name :fill_point
* Function Function : fill_point
/*************************
* Input:
*       shift;               p_avm_param
*		image;                cam_flag
p_avm_param;
* Note:
*
* Revision:
*     Create by Cheng.Li on 2018/1/17.
*/
void gpu_fill_point(Int32_t *every_angle_point_num, UInt32_t * p_lut_line, IplImage *image_result_single_front_lut, 
	Int32_t cam_flag, Int32_t  key_ponit_pic_coordinate_temp[2], Int32_t j, Int32_t i,
	Int32_t result_image_height,
	Int32_t result_image_width, Float32_t car_head[3], Int32_t rotate_flag)
{
	int step = image_result_single_front_lut->widthStep / sizeof(uchar);//�Զ�����һ�ж����ֽ�
	int chanels = image_result_single_front_lut->nChannels;//��ͨ��
	
	
#if  DEBUG_AMBA_LINE 
	if (key_ponit_pic_coordinate_temp[0] < result_image_height&&key_ponit_pic_coordinate_temp[0]>0 && j < result_image_width&&j>0)
	{
		int my_interval=height_coorld - 19;//���һ�汸��int my_interval=height_coorld - 19;
		
		if (key_ponit_pic_coordinate_temp[0] <= my_interval || my_a == 0 || i == EQUALITY_ANGLE-1)
		{
			//if (i != EQUALITY_ANGLE - 1)
			//{
			
				height_coorld = key_ponit_pic_coordinate_temp[0];
				width_coord = j;
				my_a = 1;
			//}
			int color_flag = COLOR_YELLOW;
			(*every_angle_point_num)++;
			zip_data_draw_dynamic_line((*every_angle_point_num), p_lut_line, cam_flag, key_ponit_pic_coordinate_temp[0], j, color_flag);
			if (chanels == 3)
			{
				image_result_single_front_lut->imageData[key_ponit_pic_coordinate_temp[0] * step + j*chanels] = 0;
				image_result_single_front_lut->imageData[key_ponit_pic_coordinate_temp[0] * step + j*chanels + 1] = 255;
				image_result_single_front_lut->imageData[key_ponit_pic_coordinate_temp[0] * step + j*chanels + 2] = 255;
			}
			if (chanels == 1)
			{
				image_result_single_front_lut->imageData[key_ponit_pic_coordinate_temp[0] * step + j*chanels] = 255;

			}
		}
#endif
#if 0//������ͼ�õ�
		//�������С����
		if (i == EQUALITY_ANGLE - 2)
		{
			//p_avm_param->
			if (car_head[0] < 0 && cam_flag == ROTATE_PLANE_EXPEND_FRONT)//ǰ��
			{
				for (int m = key_ponit_pic_coordinate_temp[0]; m < key_ponit_pic_coordinate_temp[0] + 3; m++)
				{
					for (int n = j; n < j + 20; n++)
					{
						if (chanels == 3)
						{
							image_result_single_front_lut->imageData[m * step + n*chanels] = 0;
							image_result_single_front_lut->imageData[m * step + n*chanels + 1] = 255;
							image_result_single_front_lut->imageData[m * step + n*chanels + 2] = 255;
						}
						if (chanels == 1)
						{
							image_result_single_front_lut->imageData[m * step + n*chanels] = 255;
						}

						(*every_angle_point_num)++;
						int color_flag = COLOR_YELLOW;
						zip_data_draw_dynamic_line((*every_angle_point_num), p_lut_line, cam_flag, m, n, color_flag);
					}
				}
			}
			if (car_head[0] > 0 && cam_flag == ROTATE_PLANE_EXPEND_FRONT)//ǰ��
			{
				for (int m = key_ponit_pic_coordinate_temp[0]; m < key_ponit_pic_coordinate_temp[0] + 3; m++)
				{
					for (int n = j - 20; n < j; n++)
					{
						if (chanels == 3)
						{
							image_result_single_front_lut->imageData[m * step + n*chanels] = 0;
							image_result_single_front_lut->imageData[m * step + n*chanels + 1] = 255;
							image_result_single_front_lut->imageData[m * step + n*chanels + 2] = 255;
						}
						if (chanels == 1)
						{
							image_result_single_front_lut->imageData[m * step + n*chanels] = 255;

						}

						(*every_angle_point_num)++;
						int color_flag = COLOR_YELLOW;
						zip_data_draw_dynamic_line((*every_angle_point_num), p_lut_line, cam_flag, m, n, color_flag);
					}
				}
			}

			//////////////////////��
			if (car_head[0] < 0 && rotate_flag == ROTATE_PLANE_EXPEND_REAR)//�� ��
			{
				for (int m = key_ponit_pic_coordinate_temp[0]; m < key_ponit_pic_coordinate_temp[0] + 3; m++)
				{
					for (int n = j - 20; n < j; n++)
					{
						if (chanels == 3)
						{
							image_result_single_front_lut->imageData[m * step + n*chanels] = 0;
							image_result_single_front_lut->imageData[m * step + n*chanels + 1] = 255;
							image_result_single_front_lut->imageData[m * step + n*chanels + 2] = 255;
						}
						if (chanels == 1)
						{
							image_result_single_front_lut->imageData[m * step + n*chanels] = 255;

						}

						(*every_angle_point_num)++;
						int color_flag = COLOR_YELLOW;
						zip_data_draw_dynamic_line((*every_angle_point_num), p_lut_line, cam_flag, m, n, color_flag);
					}
				}
			}
			if (car_head[0] > 0 && rotate_flag == ROTATE_PLANE_EXPEND_REAR)//����
			{
				for (int m = key_ponit_pic_coordinate_temp[0]; m < key_ponit_pic_coordinate_temp[0] + 3; m++)
				{
					for (int n = j; n < j + 20; n++)
					{
						if (chanels == 3)
						{
							image_result_single_front_lut->imageData[m * step + n*chanels] = 0;
							image_result_single_front_lut->imageData[m * step + n*chanels + 1] = 255;
							image_result_single_front_lut->imageData[m * step + n*chanels + 2] = 255;
						}
						if (chanels == 1)
						{
							image_result_single_front_lut->imageData[m * step + n*chanels] = 255;

						}

						(*every_angle_point_num)++;
						int color_flag = COLOR_YELLOW;
						zip_data_draw_dynamic_line((*every_angle_point_num), p_lut_line, cam_flag, m, n, color_flag);
					}
				}
			}
		}


#endif

	}
}




#if 1
/*
*Function Name :fill_point
* Function Function : fill_point
/*************************
* Input:
*       shift;               p_avm_param
*		image;                cam_flag
p_avm_param;
* Note:
*
* Revision:
*     Create by Cheng.Li on 2018/1/17.
*/
void cpu_fill_point(Int32_t *every_angle_point_num, UInt32_t * p_lut_line, IplImage *image_result_single_front_lut,
	Int32_t cam_flag, Int32_t  key_ponit_pic_coordinate_temp[2], Int32_t j, Int32_t i,
	Int32_t result_image_height,
	Int32_t result_image_width, Float32_t car_head[3], Int32_t rotate_flag)
{
	int step = image_result_single_front_lut->widthStep / sizeof(uchar);//�Զ�����һ�ж����ֽ�
	int chanels = image_result_single_front_lut->nChannels;//��ͨ��

	int color_flag = COLOR_YELLOW;

#if  DEBUG_AMBA_LINE 
	if (key_ponit_pic_coordinate_temp[0] < result_image_height&&key_ponit_pic_coordinate_temp[0]>0 && j < result_image_width&&j>0)
	{
		(*every_angle_point_num)++;
		zip_data_draw_dynamic_line((*every_angle_point_num), p_lut_line, cam_flag, key_ponit_pic_coordinate_temp[0], j, color_flag);
		if (chanels == 3)
		{
			image_result_single_front_lut->imageData[key_ponit_pic_coordinate_temp[0] * step + j*chanels] = 0;
			image_result_single_front_lut->imageData[key_ponit_pic_coordinate_temp[0] * step + j*chanels + 1] = 255;
			image_result_single_front_lut->imageData[key_ponit_pic_coordinate_temp[0] * step + j*chanels + 2] = 255;
		}
		if (chanels == 1)
		{
			image_result_single_front_lut->imageData[key_ponit_pic_coordinate_temp[0] * step + j*chanels] = 255;

		}
#endif
#if 1
		//�������С����
		if (i == EQUALITY_ANGLE - 2)
		{
			//p_avm_param->
			if (car_head[0] < 0 && cam_flag == ROTATE_PLANE_EXPEND_FRONT)//ǰ��
			{
				for (int m = key_ponit_pic_coordinate_temp[0]; m < key_ponit_pic_coordinate_temp[0] + 3; m++)
				{
					for (int n = j; n < j + 20; n++)
					{
						if (chanels == 3)
						{
							image_result_single_front_lut->imageData[m * step + n*chanels] = 0;
							image_result_single_front_lut->imageData[m * step + n*chanels + 1] = 255;
							image_result_single_front_lut->imageData[m * step + n*chanels + 2] = 255;
						}
						if (chanels == 1)
						{
							image_result_single_front_lut->imageData[m * step + n*chanels] = 255;
						}

						(*every_angle_point_num)++;
						int color_flag = COLOR_YELLOW;
						zip_data_draw_dynamic_line((*every_angle_point_num), p_lut_line, cam_flag, m, n, color_flag);
					}
				}
			}
			if (car_head[0] > 0 && cam_flag == ROTATE_PLANE_EXPEND_FRONT)//ǰ��
			{
				for (int m = key_ponit_pic_coordinate_temp[0]; m < key_ponit_pic_coordinate_temp[0] + 3; m++)
				{
					for (int n = j - 20; n < j; n++)
					{
						if (chanels == 3)
						{
							image_result_single_front_lut->imageData[m * step + n*chanels] = 0;
							image_result_single_front_lut->imageData[m * step + n*chanels + 1] = 255;
							image_result_single_front_lut->imageData[m * step + n*chanels + 2] = 255;
						}
						if (chanels == 1)
						{
							image_result_single_front_lut->imageData[m * step + n*chanels] = 255;

						}

						(*every_angle_point_num)++;
						int color_flag = COLOR_YELLOW;
						zip_data_draw_dynamic_line((*every_angle_point_num), p_lut_line, cam_flag, m, n, color_flag);
					}
				}
			}

			//////////////////////��
			if (car_head[0] < 0 && rotate_flag == ROTATE_PLANE_EXPEND_REAR)//�� ��
			{
				for (int m = key_ponit_pic_coordinate_temp[0]; m < key_ponit_pic_coordinate_temp[0] + 3; m++)
				{
					for (int n = j - 20; n < j; n++)
					{
						if (chanels == 3)
						{
							image_result_single_front_lut->imageData[m * step + n*chanels] = 0;
							image_result_single_front_lut->imageData[m * step + n*chanels + 1] = 255;
							image_result_single_front_lut->imageData[m * step + n*chanels + 2] = 255;
						}
						if (chanels == 1)
						{
							image_result_single_front_lut->imageData[m * step + n*chanels] = 255;

						}

						(*every_angle_point_num)++;
						int color_flag = COLOR_YELLOW;
						zip_data_draw_dynamic_line((*every_angle_point_num), p_lut_line, cam_flag, m, n, color_flag);
					}
				}
			}
			if (car_head[0] > 0 && rotate_flag == ROTATE_PLANE_EXPEND_REAR)//����
			{
				for (int m = key_ponit_pic_coordinate_temp[0]; m < key_ponit_pic_coordinate_temp[0] + 3; m++)
				{
					for (int n = j; n < j + 20; n++)
					{
						if (chanels == 3)
						{
							image_result_single_front_lut->imageData[m * step + n*chanels] = 0;
							image_result_single_front_lut->imageData[m * step + n*chanels + 1] = 255;
							image_result_single_front_lut->imageData[m * step + n*chanels + 2] = 255;
						}
						if (chanels == 1)
						{
							image_result_single_front_lut->imageData[m * step + n*chanels] = 255;

						}

						(*every_angle_point_num)++;
						int color_flag = COLOR_YELLOW;
						zip_data_draw_dynamic_line((*every_angle_point_num), p_lut_line, cam_flag, m, n, color_flag);
					}
				}
			}
		}


#endif

	}










}




#endif

/*���ʹ��
* Function Name:			calculate_rotate_coordinate_0
* Function Function:		calculate_rotate_coordinate_0
/*************************
* Input:
*       shift;               p_avm_param
*		image;                cam_flag
p_avm_param;
* Note:
*
* Revision:
*     Create by Cheng.Li on 2018/1/17.
*/

void revolve_one_trail_from_one_angle(Int32_t rotate_flag, P_Avm_Param_S p_avm_param,
	Int32_t		dynamic_line_length,
	Float32_t rotate_angle, 
	Float32_t  pixel_hight_dis, 
	Float32_t  pixel_width_dis,
	int cam_flag, 
	Float32_t *cam_ray, 
	UInt32_t * p_lut_line,
	Int32_t result_image_height,
	Int32_t result_image_width, IplImage *image_result_single_front_lut)
{

	static UInt32_t rotate_altogether_add=101;
	
	Float32_t centre_rotate_coordinate[3];
	Float32_t car_head[3] = { 0, 0, 0 };
	Float32_t rotate_radius;
	Float32_t angle_0;
	Float32_t angle_average;
	Int32_t point_num = 0;
	Int32_t dynamic_line_count = p_avm_param->p_avm_config_param->avm_single_view_param.front_single_view_param.
		dis_line_param.dynamic_line_param.dynamic_line_count;
	Int32_t every_angle_size = DEFAULT_DYNAMIC_DIS_LINE_SIZE * MAX_DYNAMIC_LINE_COUNT
		/ (p_avm_param->p_avm_config_param->avm_single_view_param.front_single_view_param.
		dis_line_param.dynamic_line_param.dynamic_line_count*2+1);
	Int32_t address_shift = (Int32_t)(every_angle_size*(rotate_angle + dynamic_line_count));
	p_lut_line = p_lut_line + address_shift;
	int a=DEFAULT_DYNAMIC_DIS_LINE_SIZE * MAX_DYNAMIC_LINE_COUNT;

	if (rotate_angle > 0)//�Ƕȴ���������ת��
	{
		/**************��ת��*********************/
		centre_rotate_coordinate[0] = -(p_avm_param->p_avm_config_param->vehicle_param.wheel_base) / tan((rotate_angle / 180.00)*PI) ;//�ٶ���Բ������X����
		centre_rotate_coordinate[1] = p_avm_param->p_avm_config_param->vehicle_param.vehicle_length / 2 - p_avm_param->p_avm_config_param
			->vehicle_param.wheel_base - p_avm_param->p_avm_config_param->vehicle_param.front_overhang_length;//�ٶ���Բ������Y����
		/****��һ����������**/
		car_head[0] = -p_avm_param->p_avm_config_param->vehicle_param.vehicle_width / 2;//��x���� �����ͷǰ100mm�Ա�ȷ��Բ��
		if (rotate_flag == ROTATE_4_VIEW_BIG_FRONT || rotate_flag == ROTATE_PLANE_EXPEND_FRONT 
			|| rotate_flag == ROTATE_4_VIEW_SMALL_FRONT || rotate_flag == ROTATE_3_VIEW_FRONT)
		{
			car_head[1] = p_avm_param->p_avm_config_param->vehicle_param.vehicle_length / 2 + 0;//ȷ��Y����
		}
		if (rotate_flag == ROTATE_4_VIEW_BIG_REAR || rotate_flag == ROTATE_PLANE_EXPEND_REAR 
			|| rotate_flag == ROTATE_4_VIEW_SMALL_REAR || rotate_flag == ROTATE_3_VIEW_REAR)//����ǻ����������Ĺ켣��Y�������ǳ����ĺ��
		{
	
			car_head[1] = -p_avm_param->p_avm_config_param->vehicle_param.vehicle_length / 2 - 0;//ȷ��Y����
		}

		rotate_radius = sqrt((car_head[0] - centre_rotate_coordinate[0])*(car_head[0] - centre_rotate_coordinate[0])
							+ (car_head[1] - centre_rotate_coordinate[1])*(car_head[1] - centre_rotate_coordinate[1]));//�������ת�İ뾶
		angle_0 =dynamic_line_length / rotate_radius;//���������һ����ʾ�Ļ��ȽǶȣ��û����Ļ��߳�������ת�뾶�ó����Ƚ� 
		if (rotate_flag == ROTATE_4_VIEW_BIG_REAR || rotate_flag == ROTATE_4_VIEW_SMALL_REAR || rotate_flag == ROTATE_PLANE_EXPEND_REAR || rotate_flag == ROTATE_3_VIEW_REAR)
		{
			angle_average = -angle_0 / EQUALITY_ANGLE;//����ʾ�����л�����ƽ��5000,�󵹳�ʱ���Ƕ�ƽ����Ϊ��ֵ����ת��������ʱ�� 
		}
		else
		{
			angle_average = angle_0 / EQUALITY_ANGLE;//����ʾ�����л�����ƽ��5000
		}
		my_a = 0;
		/********ȷ����Բ����뾶���Լ���ͷǰ�Ĺ켣��ʼ��֮��ʼ���û�Բ���ĺ����� �켣��******/
		draw_circle_trail(rotate_flag,centre_rotate_coordinate,
							car_head, angle_average,p_avm_param,
							pixel_hight_dis, pixel_width_dis, dynamic_line_length,
							cam_ray, cam_flag, p_lut_line, &rotate_altogether_add,
							result_image_height, result_image_width, image_result_single_front_lut, &point_num, rotate_angle);




		Int32_t every_angle_size = DEFAULT_DYNAMIC_DIS_LINE_SIZE * MAX_DYNAMIC_LINE_COUNT
			/ (p_avm_param->p_avm_config_param->avm_single_view_param.front_single_view_param.
			dis_line_param.dynamic_line_param.dynamic_line_count * 2 + 1);
		
		//every_angle_size=
		p_lut_line[every_angle_size - 1] = point_num;

		my_a = 0;
		/********* ���ڶ����켣�� ************/
		/*�ڶ����켣��ֻ��X���������Ƶ������ұ�X������ ��ͷ���ұ�*********/
		car_head[0] = p_avm_param->p_avm_config_param->vehicle_param.vehicle_width / 2;
		
		rotate_radius = sqrt((car_head[0] - centre_rotate_coordinate[0])*(car_head[0] - centre_rotate_coordinate[0])
			+ (car_head[1] - centre_rotate_coordinate[1])*(car_head[1] - centre_rotate_coordinate[1]));//�������ת�İ뾶
		angle_0 = dynamic_line_length / rotate_radius;//���������һ����ʾ�Ļ��ȽǶȣ��û����Ļ��߳�������ת�뾶�ó����Ƚ� 

		if (rotate_flag == ROTATE_4_VIEW_BIG_REAR || rotate_flag == ROTATE_4_VIEW_SMALL_REAR || rotate_flag == ROTATE_PLANE_EXPEND_REAR || rotate_flag == ROTATE_3_VIEW_REAR)
		{
			angle_average = -angle_0 / EQUALITY_ANGLE;//����ʾ�����л�����ƽ��5000
		}
		else
		{
			angle_average = angle_0 / EQUALITY_ANGLE;//����ʾ�����л�����ƽ��5000
		}
		/********ȷ����Բ����뾶���Լ���ͷǰ�Ĺ켣��ʼ��֮��ʼ���û�Բ���ĺ����� �켣��******/
		draw_circle_trail(rotate_flag, centre_rotate_coordinate, car_head, angle_average, p_avm_param,
						pixel_hight_dis, pixel_width_dis, dynamic_line_length,
						cam_ray, cam_flag, p_lut_line, &rotate_altogether_add, result_image_height, result_image_width, image_result_single_front_lut, &point_num, rotate_angle);
		

	}
	/*******��ת��*********/
	if (rotate_angle < 0)
	{
		/***************************��ת��*********************/
		centre_rotate_coordinate[0] = (p_avm_param->p_avm_config_param->vehicle_param.wheel_base) / tan((abs(rotate_angle) / 180.00)*PI);//�ٶ���Բ������X����
		centre_rotate_coordinate[1] = p_avm_param->p_avm_config_param->vehicle_param.vehicle_length / 2 - p_avm_param->p_avm_config_param->vehicle_param.wheel_base 
			- p_avm_param->p_avm_config_param->vehicle_param.front_overhang_length;//�ٶ���Բ������Y����
		/****��һ����������**/
			//-0p_avm_param->p_avm_config_param->vehicle_param.wheel_base / 2;//�ٶ���Բ������Y����

		/****��һ����**/
		car_head[0] = p_avm_param->p_avm_config_param->vehicle_param.vehicle_width / 2;//x ���������ͷǰ100mm�Ա�ȷ��Բ��
		//car_head[1] = p_avm_param->p_avm_config_param->vehicle_param.vehicle_length / 2 + 100;
		if (rotate_flag == ROTATE_4_VIEW_BIG_FRONT || rotate_flag == ROTATE_PLANE_EXPEND_FRONT || rotate_flag == ROTATE_3_VIEW_FRONT)
		{
			car_head[1] = p_avm_param->p_avm_config_param->vehicle_param.vehicle_length / 2 + 100;
		}
		if (rotate_flag == ROTATE_4_VIEW_BIG_REAR || rotate_flag == ROTATE_PLANE_EXPEND_REAR || rotate_flag == ROTATE_3_VIEW_REAR)
		{
			//car_head[1] = -p_avm_param->p_avm_config_param->vehicle_param.vehicle_length / 2 - 100;
			car_head[1] = -p_avm_param->p_avm_config_param->vehicle_param.vehicle_length / 2 - 100;
		}
		rotate_radius = sqrt((car_head[0] - centre_rotate_coordinate[0])*(car_head[0] - centre_rotate_coordinate[0])
			+ (car_head[1] - centre_rotate_coordinate[1])*(car_head[1] - centre_rotate_coordinate[1]));//�������ת�İ뾶
		angle_0 = dynamic_line_length / rotate_radius;//���������һ����ʾ�Ļ��ȽǶ�

		if (rotate_flag == ROTATE_4_VIEW_BIG_REAR || rotate_flag == ROTATE_4_VIEW_SMALL_REAR || rotate_flag == ROTATE_PLANE_EXPEND_REAR || rotate_flag == ROTATE_3_VIEW_REAR)
		{
			angle_average = angle_0 / EQUALITY_ANGLE;//����ʾ�����л�����ƽ��5000
		}
		else
		{
			angle_average = -angle_0 / EQUALITY_ANGLE;//����ʾ�����л�����ƽ��5000
		}
		/********ȷ����Բ����뾶���Լ���ͷǰ�Ĺ켣��ʼ��֮��ʼ���û�Բ���ĺ����� �켣��******/
		my_a = 0;
		draw_circle_trail(rotate_flag, centre_rotate_coordinate, car_head, angle_average, p_avm_param,
			pixel_hight_dis, pixel_width_dis, dynamic_line_length,
			cam_ray, cam_flag, p_lut_line, &rotate_altogether_add, result_image_height, result_image_width, image_result_single_front_lut, &point_num, rotate_angle);


		Int32_t every_angle_size = DEFAULT_DYNAMIC_DIS_LINE_SIZE * MAX_DYNAMIC_LINE_COUNT
			/ (p_avm_param->p_avm_config_param->avm_single_view_param.front_single_view_param.
			dis_line_param.dynamic_line_param.dynamic_line_count * 2 + 1);

		//every_angle_size=
		p_lut_line[every_angle_size - 1] = point_num;



		/*********draw ���ڶ����켣�� ************/
		car_head[0] = -p_avm_param->p_avm_config_param->vehicle_param.vehicle_width / 2;
		
		rotate_radius = sqrt((car_head[0] - centre_rotate_coordinate[0])*(car_head[0] - centre_rotate_coordinate[0])
			+ (car_head[1] - centre_rotate_coordinate[1])*(car_head[1] - centre_rotate_coordinate[1]));

		angle_0 = dynamic_line_length / rotate_radius;
		if (rotate_flag == ROTATE_4_VIEW_BIG_REAR || rotate_flag == ROTATE_4_VIEW_SMALL_REAR || rotate_flag == ROTATE_PLANE_EXPEND_REAR || rotate_flag == ROTATE_3_VIEW_REAR)
		{
			angle_average = angle_0 / EQUALITY_ANGLE;//����ʾ�����л�����ƽ��5000
		}
		else
		{
			angle_average = -angle_0 / EQUALITY_ANGLE;//����ʾ�����л�����ƽ��5000
		}
		point_num;
		/********ȷ����Բ����뾶���Լ���ͷǰ�Ĺ켣��ʼ��֮��ʼ���û�Բ���ĺ����� �켣��******/
		my_a = 0;
		draw_circle_trail(rotate_flag, centre_rotate_coordinate,
			car_head, angle_average,
			p_avm_param, pixel_hight_dis, pixel_width_dis, dynamic_line_length, cam_ray,
			cam_flag, p_lut_line, &rotate_altogether_add, result_image_height, result_image_width, image_result_single_front_lut, &point_num, rotate_angle);


	}
	/*******ת�����*********/
	if (rotate_angle == 0)
	//if (0)
	{
		int dEUCE_LONG=15;
		Int32_t key_ponit_pic_coordinate[2];
		int step = image_result_single_front_lut->widthStep / sizeof(uchar);//�Զ�����һ�ж����ֽ�
		int chanels = image_result_single_front_lut->nChannels;//��ͨ��
		/**************************����Ϊ�˼���һ����ת��Ƕȣ���Ϊת����0�޷���Բ���ķ�ʽ***************************/
		/*********************������***************************/
			Float32_t dis_car[3];
			if (cam_flag == CAM_REAR)
			{
				dis_car[0] = -p_avm_param->p_avm_config_param->vehicle_param.vehicle_width / 2.0;// { -920, -2355, 0 };
				dis_car[1] = -(p_avm_param->p_avm_config_param->vehicle_param.vehicle_length / 2.0);
				dis_car[2] = 0;
				
			}
			if (cam_flag == CAM_FRONT)
			{
				dis_car[0] = -p_avm_param->p_avm_config_param->vehicle_param.vehicle_width / 2.0;// { -920, 2355, 0 };
				dis_car[1] = (p_avm_param->p_avm_config_param->vehicle_param.vehicle_length / 2.0);
				dis_car[2] = 0;
			}
			for (int i = 0; i < dEUCE_LONG; i++)
			{
				Float32_t dx = (Float32_t)dynamic_line_length / dEUCE_LONG;;
				if (cam_flag == CAM_REAR)
				{
					dis_car[1] = dis_car[1] - dx;
				}
				if (cam_flag == CAM_FRONT)
				{
					dis_car[1] = dis_car[1] + dx;
				}
				plane_front_rear_single_view_calculate_key_point_coordinate_one_point(p_avm_param, cam_flag, dis_car, key_ponit_pic_coordinate);
				for (int j = key_ponit_pic_coordinate[1] - 1; j < key_ponit_pic_coordinate[1]; j++)
				{
					
					if (key_ponit_pic_coordinate[0] < result_image_height&&key_ponit_pic_coordinate[0]>0 && j < result_image_width&&j>0)
					{
						point_num++;
						int color_flag = 1;
						zip_data_draw_dynamic_line(point_num, p_lut_line, cam_flag, key_ponit_pic_coordinate[0], j, color_flag);
						if (chanels == 3)
						{
							image_result_single_front_lut->imageData[key_ponit_pic_coordinate[0] * step + j*chanels] = 0;
							image_result_single_front_lut->imageData[key_ponit_pic_coordinate[0] * step + j*chanels + 1] = 255;
							image_result_single_front_lut->imageData[key_ponit_pic_coordinate[0] * step + j*chanels + 2] = 255;
						}
						if (chanels == 1)
						{
							image_result_single_front_lut->imageData[key_ponit_pic_coordinate[0] * step + j*chanels] = 255;
						}
					}
				}
			}



			Int32_t every_angle_size = DEFAULT_DYNAMIC_DIS_LINE_SIZE * MAX_DYNAMIC_LINE_COUNT
				/ (p_avm_param->p_avm_config_param->avm_single_view_param.front_single_view_param.
				dis_line_param.dynamic_line_param.dynamic_line_count * 2 + 1);

			//every_angle_size=
			p_lut_line[every_angle_size - 1] = point_num;


			/**********��2���켣��*******************/
			if (cam_flag == CAM_REAR)
			{
				dis_car[0] = p_avm_param->p_avm_config_param->vehicle_param.vehicle_width / 2.0;// 
				dis_car[1] = -(p_avm_param->p_avm_config_param->vehicle_param.vehicle_length / 2.0);
			}
			if (cam_flag == CAM_FRONT)
			{
				dis_car[0] = p_avm_param->p_avm_config_param->vehicle_param.vehicle_width / 2.0;// { -920, -2355, 0 };
				dis_car[1] = (p_avm_param->p_avm_config_param->vehicle_param.vehicle_length / 2.0);
			}
			for (int i = 0; i < dEUCE_LONG; i++)
			{
				Float32_t dx = (Float32_t)dynamic_line_length / dEUCE_LONG;
				if (cam_flag == CAM_REAR)
				{
					dis_car[1] = dis_car[1] - dx;
				}
				if (cam_flag == CAM_FRONT)
				{
					dis_car[1] = dis_car[1] + dx;
				}
				plane_front_rear_single_view_calculate_key_point_coordinate_one_point(p_avm_param, cam_flag, dis_car, key_ponit_pic_coordinate);
				for (int j = key_ponit_pic_coordinate[1] -1; j < key_ponit_pic_coordinate[1]; j++)
				{
					
					if (key_ponit_pic_coordinate[0] < result_image_height&&key_ponit_pic_coordinate[0]>0 && j < result_image_width&&j>0)
					{
						point_num++;
						int color_flag = 1;
						zip_data_draw_dynamic_line(point_num, p_lut_line, cam_flag, key_ponit_pic_coordinate[0], j, color_flag);
						if (chanels == 3)
						{
							image_result_single_front_lut->imageData[key_ponit_pic_coordinate[0] * step + j*chanels] = 0;
							image_result_single_front_lut->imageData[key_ponit_pic_coordinate[0] * step + j*chanels + 1] = 255;
							image_result_single_front_lut->imageData[key_ponit_pic_coordinate[0] * step + j*chanels + 2] = 255;
						}
						if (chanels == 1)
						{
							image_result_single_front_lut->imageData[key_ponit_pic_coordinate[0] * step + j*chanels] = 255;
						}
					}
				}
			}
	}
	point_num;
}
/*���ʹ��
* Function Name:			draw_dynamic_plane_front_rear_single_view_line
* Function Function:			draw_dynamic_plane_front_rear_single_view_line
/*************************
* Input:
*       shift;               p_avm_param
*		image;                cam_flag
p_avm_param;
* Note:
*
* Revision:
*     Create by Cheng.Li on 2018/1/17.
*/
AVM_Error_Code_E  draw_dynamic_plane_front_rear_single_view_line(IN P_Avm_Param_S p_avm_param,IN int cam_flag)
{
	AVM_Error_Code_E ret = ERROR_NONE;
	Front_Rear_Single_View_Param_S single_view_param;
	CvSize size0;
	UInt32_t* p_lut = NULL;
	UInt32_t* p_lut_line = NULL;
	Int32_t		dynamic_line_count;
	Int32_t rotate_flag = 0;
	if (cam_flag == CAM_FRONT)
	{
		single_view_param = p_avm_param->p_avm_config_param->avm_single_view_param.front_single_view_param;
		size0 = cvSize(single_view_param.result_image_width, single_view_param.result_image_height);//
		p_lut = p_avm_param->p_avm_lut->p_lut_single_view_front;
		p_lut_line = p_avm_param->p_avm_dis_line->p_dynamic_dis_line_front;

		dynamic_line_count = p_avm_param->p_avm_config_param->avm_single_view_param.front_single_view_param.dis_line_param.dynamic_line_param.dynamic_line_count;
		rotate_flag = ROTATE_PLANE_EXPEND_FRONT;
	}
	if (cam_flag == CAM_REAR)
	{
		single_view_param = p_avm_param->p_avm_config_param->avm_single_view_param.rear_single_view_param;
		size0 = cvSize(single_view_param.result_image_width, single_view_param.result_image_height);//
		p_lut = p_avm_param->p_avm_lut->p_lut_single_view_rear;
		p_lut_line = p_avm_param->p_avm_dis_line->p_dynamic_dis_line_rear;
		dynamic_line_count = p_avm_param->p_avm_config_param->avm_single_view_param.
		rear_single_view_param.dis_line_param.dynamic_line_param.dynamic_line_count;//�û����õĶ�̬�߸���
		rotate_flag = ROTATE_PLANE_EXPEND_REAR;
	}
	Float32_t cam_ray[3];
	Float32_t tan_h = (tan((single_view_param.plane_expand_param.fov_h / 2 / 180)*PI));
	Float32_t tan_v = (tan((single_view_param.plane_expand_param.fov_v / 2 / 180)*PI));
	cam_ray[2] = -8000.00;
	Float32_t  pixel_hight_dis = -(cam_ray[2] * tan_v) / (single_view_param.result_image_height / 2);//
	Float32_t  pixel_width_dis = -(cam_ray[2] * tan_h) / (single_view_param.result_image_width / 2);
	

	/*********draw dynamic line***********/
	Float32_t rotate_angle = 0.0000;
	IplImage *image_result_single_front_lut =NULL;
	char image_path[MAX_ARRAY_SIZE];
	for (int i = -dynamic_line_count; i <= dynamic_line_count; i++)
	//int i = 2;
	{
#if  DEBUG_AMBA_LINE 
			size0 = cvSize(single_view_param.result_image_width, single_view_param.result_image_height);
			image_result_single_front_lut = cvCreateImage(size0, IPL_DEPTH_8U, 3);
			memset(image_result_single_front_lut->imageData, 0, single_view_param.result_image_width*single_view_param.result_image_height * 3);
#endif
			rotate_angle = (Float32_t )(i);
		revolve_one_trail_from_one_angle(rotate_flag, p_avm_param,
			single_view_param.dis_line_param.dynamic_line_param.dynamic_line_length,
			rotate_angle, pixel_hight_dis, pixel_width_dis,
			cam_flag, cam_ray, p_lut_line,
			single_view_param.result_image_height, single_view_param.result_image_width, image_result_single_front_lut);

#if   DEBUG_AMBA_LINE 
#if RELEASE==0
		if (i <= 0)
		{
			sprintf(image_path, "%s/distance_line/myline%d��ת%d���Ƕ�.bmp", result_image_path, cam_flag, -i);
		}
		if (i > 0)
		{
			sprintf(image_path, "%s/distance_line/myline%d��ת%d���Ƕ�.bmp", result_image_path, cam_flag, i);
		}
		cvSaveImage(image_path, image_result_single_front_lut);
#endif
#endif
	}
	return ret;
}
