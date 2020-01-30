
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

void single_view_zip_data(IN int shift_altogether, IN Float64_t *image_point, IN UInt32_t*p_lut, IN UInt32_t camera_flag)
{

	UInt32_t u_int = (UInt32_t)image_point[0], v_int = (UInt32_t)image_point[1];   //get integer'part of position
	Float64_t u_Float64_t = image_point[0] - u_int, v_Float64_t = image_point[1] - v_int; // get decimal'partr of position
	UInt32_t u_point_int = u_Float64_t * 255, v_point_int = v_Float64_t * 255;
	UInt32_t temp_shift = 0;
	UInt32_t temp_shift0 = 0;
	UInt32_t temp_weight = 1;     //one pixel's weight in dst_pic
	temp_shift = (u_int << 21) + (v_int << 10) + (temp_weight << 2) + (camera_flag);
	temp_shift0 = (u_point_int << 24) + (v_point_int << 16);
	p_lut[shift_altogether] = temp_shift;
	p_lut[shift_altogether + 1] = temp_shift0;
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
	CvSize size0;
	UInt32_t* p_lut = NULL;
	if (cam_flag == CAM_FRONT)
	{
		single_view_param = p_avm_param->p_avm_config_param->avm_single_view_param.front_single_view_param;
		size0 = cvSize(single_view_param.result_image_width, single_view_param.result_image_height);//
		p_lut = p_avm_param->p_avm_lut->p_lut_single_view_front;
	}
	if (cam_flag == CAM_REAR)
	{
		single_view_param = p_avm_param->p_avm_config_param->avm_single_view_param.rear_single_view_param;
		size0 = cvSize(single_view_param.result_image_width, single_view_param.result_image_height);//
		p_lut = p_avm_param->p_avm_lut->p_lut_single_view_rear;
	}
#if DEBUG_INFO
	IplImage* image[4] = { NULL, NULL, NULL, NULL };
	ret = my_load_image(image);
	CHECK_ERROR(ret);
	IplImage *image_result_single_front_lut = cvCreateImage(size0, IPL_DEPTH_8U, 1);
	memset(image_result_single_front_lut->imageData, 0, single_view_param.result_image_width*single_view_param.result_image_height);
#endif	
	Float64_t scale_coordinate_col = Float64_t(single_view_param.crop_param.crop_rect.width) 
									/ Float64_t(single_view_param.result_image_width);//���ԭͼ��ü��� �Ĵ�С����

	Float64_t scale_coordinate_row = Float64_t(single_view_param.crop_param.crop_rect.height) 
									/ Float64_t(single_view_param.result_image_height);//���ԭͼ��ü��� �Ĵ�С����
	for (int i = 0; i < single_view_param.result_image_height; i++)
	{
		for (int j = 0; j < single_view_param.result_image_width; j++)
		{
			Float64_t image_point[2];
			if (cam_flag == CAM_FRONT)//ǰ����ͷ
			{
				image_point[0] = single_view_param.crop_param.crop_rect.y + i*scale_coordinate_row;
				image_point[1] = single_view_param.crop_param.crop_rect.x + j*scale_coordinate_col;
			}
			if (cam_flag == CAM_REAR)//
			{
				image_point[0] = single_view_param.crop_param.crop_rect.y 
								+ (single_view_param.result_image_height - 1 - i)*scale_coordinate_row;//��Ϊ������ͷ�Ƿ�װ�����Խ��ͼ��תһ��
				image_point[1] = single_view_param.crop_param.crop_rect.x 
								+ (single_view_param.result_image_width - j - 1)*scale_coordinate_col;//��Ϊ������ͷ�Ƿ�װ�����Խ��ͼ��תһ��
			}

			UInt32_t  shift_altogether = i*single_view_param.result_image_width * 2 + j * 2;//��������ƫ�Ƶ�ַ
			single_view_zip_data(shift_altogether, image_point, p_lut, cam_flag);
#if DEBUG_INFO
			int rgb[3];
			bilinear_interpolation(image[cam_flag], rgb, image_point[1], image_point[0]);
			image_result_single_front_lut->imageData[i *single_view_param.result_image_width + j] = (uchar)(rgb[0]);
#endif
		}
	}
#if DEBUG_INFO
	char result_name[MAX_ARRAY_SIZE];
	sprintf(result_name, "%s/crop_front_rear.bmp", result_image_path);
	cvSaveImage(result_name, image_result_single_front_lut);
#endif
	return ret;

}











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
AVM_Error_Code_E  plane_rightsingle_view(IN P_Avm_Param_S p_avm_param, IN  int cam_flag)
{
	AVM_Error_Code_E ret = ERROR_NONE;

	CvSize size0;
	UInt32_t* p_lut = NULL;
	
	Left_Right_Single_View_Param_S		right_single_view_param= p_avm_param->p_avm_config_param->avm_single_view_param.right_single_view_param;
	size0 = cvSize(right_single_view_param.result_image_width, right_single_view_param.result_image_height);//
	p_lut = p_avm_param->p_avm_lut->p_lut_single_view_right;

#if DEBUG_INFO
	IplImage* image[4] = { NULL, NULL, NULL, NULL };
	ret = my_load_image(image);
	CHECK_ERROR(ret);
	IplImage *image_result_single_front_lut = cvCreateImage(size0, IPL_DEPTH_8U, 1);
	memset(image_result_single_front_lut->imageData, 0, single_view_param.result_image_width*single_view_param.result_image_height);
#endif
	Float64_t fov_h = 130;
	Float64_t fov_v = 130;
	Float64_t cam_ray[3];
	Float64_t tan_h = (tan((fov_h / 2 / 180)*PI));
	Float64_t tan_v = (tan((fov_v / 2 / 180)*PI));
	cam_ray[2] = -8000.00;//�������⽹��
	Float64_t  pixel_hight_dis = -(cam_ray[2] * tan_v) / (right_single_view_param.result_image_height / 2);//�����ͼ��ֱ������ÿ������ռ���ٺ���
	Float64_t  pixel_width_dis = -(cam_ray[2] * tan_h) / (right_single_view_param.result_image_width / 2);//�����ͼ��ˮƽ������ÿ������ռ���ٺ���


	for (int i = 0; i < right_single_view_param.result_image_height; i++)
	{
		for (int j = 0; j < right_single_view_param.result_image_width; j++)
		{
			/*if (cam_flag == CAM_FRONT)
			{*/
			cam_ray[1] = (Float64_t)(j - right_single_view_param.result_image_width / 2.00)* pixel_width_dis;//ת��Ϊ�������ϵ
			cam_ray[0] = (Float64_t)(i - right_single_view_param.result_image_height / 2.00)* pixel_hight_dis;//������x
			//}
			//if (cam_flag == CAM_REAR)
			//{
			//	cam_ray[1] = (Float64_t)((Float64_t(j) - ((Float64_t)(single_view_param.result_image_width) / 2.00))* pixel_width_dis);//ת��Ϊ�������ϵ
			//	cam_ray[0] = (Float64_t)((Float64_t(i) - ((Float64_t)(single_view_param.result_image_height) / 2.00))* pixel_hight_dis);
			//	//cam_ray[1] = (Float64_t)((Float64_t(single_view_param.result_image_width - j - 1) - ((Float64_t)(single_view_param.result_image_width) / 2.00))* pixel_width_dis);
			//	//cam_ray[0] = (Float64_t)((Float64_t(single_view_param.result_image_height - i - 1) - ((Float64_t)(single_view_param.result_image_height) / 2.00))* pixel_hight_dis);
			//}
			Float64_t image_point[2];
			Cam_Ray_To_Image_Point(image_point, cam_ray, &(p_avm_param->p_avm_config_param->camera_model[0].camera_model_int));
			UInt32_t  shift_altogether = i*right_single_view_param.result_image_width * 2 + j * 2;
			single_view_zip_data(shift_altogether, image_point, p_lut, cam_flag);
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
#define DEBUG_INFO 1
AVM_Error_Code_E  plane_front_rear_single_view(IN P_Avm_Param_S p_avm_param, IN  int cam_flag)
{
	AVM_Error_Code_E ret = ERROR_NONE;
	Front_Rear_Single_View_Param_S single_view_param;
	CvSize size0;
	UInt32_t* p_lut = NULL;
	Int32_t is_front_cam;
	if (cam_flag == CAM_FRONT)
	{
		single_view_param = p_avm_param->p_avm_config_param->avm_single_view_param.front_single_view_param;
		size0 = cvSize(single_view_param.result_image_width, single_view_param.result_image_height);//
		p_lut = p_avm_param->p_avm_lut->p_lut_single_view_front;
		is_front_cam = 1;
		//
	}
	if (cam_flag == CAM_REAR)
	{
		single_view_param = p_avm_param->p_avm_config_param->avm_single_view_param.rear_single_view_param;
		size0 = cvSize(single_view_param.result_image_width, single_view_param.result_image_height);//
		p_lut = p_avm_param->p_avm_lut->p_lut_single_view_rear;
		is_front_cam = -1;
		//
	}
#if DEBUG_INFO
	IplImage* image[4] = { NULL, NULL, NULL, NULL };
	ret = my_load_image(image);
	CHECK_ERROR(ret);
	IplImage *image_result_single_front_lut = cvCreateImage(size0, IPL_DEPTH_8U, 1);
	memset(image_result_single_front_lut->imageData, 0, single_view_param.result_image_width*single_view_param.result_image_height);
#endif

	Float64_t focal_length = 1000; // unit is mm
	Float64_t fov_h = (single_view_param.plane_expand_param.fov_h*PI)/180;
	Float64_t fov_v = (single_view_param.plane_expand_param.fov_v*PI) / 180;
	Int32_t result_width = single_view_param.result_image_width;
	Int32_t result_height = single_view_param.result_image_height;

	// ����չ��ͼ��ÿ�������ڸ߿����������ʵ�ʾ���
	Float64_t	mm_per_pixel_h = focal_length * tan(fov_h * 0.5) * 2 / result_width;
	Float64_t  mm_per_pixel_v = focal_length * tan(fov_v * 0.5) * 2 / result_height;
	Float64_t pose_t_0[3];
	Float64_t pose_t[3];
	Float64_t inv_pose[3];
	for (int i = 3; i < 6; i++)
	{
		inv_pose[i-3]=p_avm_param->p_avm_config_param->camera_model[cam_flag].camera_model_ext.inv_pose[i];
	}
	p_avm_param->p_avm_config_param->camera_model[cam_flag].camera_model_ext.inv_R;
	Mat_Mul_Vec_3by3(pose_t_0, p_avm_param->p_avm_config_param->camera_model[cam_flag].camera_model_ext.R,inv_pose);
	pose_t[0] = -pose_t_0[0];
	pose_t[1] = -pose_t_0[1];
	pose_t[2] = -pose_t_0[2];
	Float64_t pitch_angle = (8*PI) / 180;//�˴�����������¸�����

	Float64_t init_world_coord_x = pose_t[0] - is_front_cam * mm_per_pixel_h * result_width * 0.5;
	Float64_t init_world_coord_y = pose_t[1] + is_front_cam * (focal_length * cos(pitch_angle) + mm_per_pixel_v * result_height * 0.5 * sin(pitch_angle));
	Float64_t init_world_coord_z = pose_t[2] - focal_length * sin(pitch_angle) + mm_per_pixel_v * result_height * 0.5 * cos(pitch_angle);
	Float64_t delta_x = mm_per_pixel_h;
	Float64_t delta_y = mm_per_pixel_v * sin(pitch_angle);
	Float64_t delta_z = mm_per_pixel_v * cos(pitch_angle);
	Float64_t world_coordinate[3];
	Float64_t image_point[2];


	world_coordinate[0] = -920; //-920;
	world_coordinate[1] = 2755;
	world_coordinate[2] = 0;
	Float64_t image_point_400mm[2];
	world_coordinate_to_pic_coordinate(CAM_FRONT, world_coordinate, image_point_400mm, p_avm_param);//0 is front

	world_coordinate[0] = -920;//-920;
	world_coordinate[1] = 3355;
	world_coordinate[2] = 0;
	Float64_t image_point_1000mm[2];
	world_coordinate_to_pic_coordinate(CAM_FRONT, world_coordinate, image_point_1000mm, p_avm_param);//0 is front

	world_coordinate[0] = -920;//-920;
	world_coordinate[1] = 4355;
	world_coordinate[2] = 0;
	Float64_t image_point_2000mm[2];
	world_coordinate_to_pic_coordinate(CAM_FRONT, world_coordinate, image_point_2000mm, p_avm_param);//0 is front

	world_coordinate[0] = -920;// -920;
	world_coordinate[1] = 4855;
	world_coordinate[2] = 0;
	Float64_t image_point_2500mm[2];
	world_coordinate_to_pic_coordinate(CAM_FRONT, world_coordinate, image_point_2500mm, p_avm_param);//0 is front

	int image_400mm[2], image_1000mm[2], image_2000mm[2], image_2500mm[2];

	for (int i = 0; i < single_view_param.result_image_height; i++)
	{
		for (int j = 0; j < single_view_param.result_image_width; j++)
		{

			world_coordinate[0] = init_world_coord_x + is_front_cam * j * delta_x;
			world_coordinate[1] = init_world_coord_y - is_front_cam * i * delta_y;
			world_coordinate[2] = init_world_coord_z - i * delta_z;
			
			world_coordinate_to_pic_coordinate(CAM_FRONT, world_coordinate, image_point, p_avm_param);//0 is front
			/*
			if ((image_point[0]> (image_point_400mm[0] - 5) && image_point[0] < (image_point_400mm[0] + 5)) && (image_point[1]> (image_point_400mm[1] - 5) && image_point[1] < (image_point_400mm[1] + 5)))
			{
				printf("%lf,%lf\t", image_point[0], image_point[1]);
				printf("%lf,%lf\n", image_point_400mm[0], image_point_400mm[1]);
			}*/
			if ((image_point[0]> (image_point_400mm[0] - 2) && image_point[0] < (image_point_400mm[0] + 2))
				   && (image_point[1]> (image_point_400mm[1] - 3) && image_point[1] < (image_point_400mm[1] + 3)))
			{
				image_400mm[0] = i;
				image_400mm[1] = j;
			}

			if ((image_point[0]> (image_point_1000mm[0] - 2) && image_point[0] < (image_point_1000mm[0] + 2))
				&& (image_point[1]> (image_point_1000mm[1] - 2) && image_point[1] < (image_point_1000mm[1] + 2)))
			{
				image_1000mm[0] = i;
				image_1000mm[1] = j;
			}
			if ((image_point[0]> (image_point_2000mm[0] - 2) && image_point[0] < (image_point_2000mm[0] + 2))
				&& (image_point[1]> (image_point_2000mm[1] - 2) && image_point[1] < (image_point_2000mm[1] + 2)))
			{
				image_2000mm[0] = i;
				image_2000mm[1] = j;
			}

			if ((image_point[0]> (image_point_2500mm[0] - 2) && image_point[0] < (image_point_2500mm[0] + 2))
				&& (image_point[1]> (image_point_2500mm[1] - 2) && image_point[1] < (image_point_2500mm[1] + 2)))
			{
				image_2500mm[0] = i;
				image_2500mm[1] = j;
			}



			UInt32_t  shift_altogether = i*single_view_param.result_image_width * 2 + j * 2;
			single_view_zip_data(shift_altogether, image_point, p_lut, cam_flag);

#if DEBUG_INFO
			
			int rgb[3];
			bilinear_interpolation(image[cam_flag], rgb, image_point[1], image_point[0]);
			image_result_single_front_lut->imageData[i *single_view_param.result_image_width + j] = (uchar)(rgb[0]);
#endif

		}
	}

	printf("laotianye wodebababaoyouwodebababaoyouwomiaozhuangnanlingxingxuxianrenwodebababaoyouwo laotianyebaoyouwolaotianye baoyouwo laotianyebaoyouwo");
#if DEBUG_INFO
	char result_name[MAX_ARRAY_SIZE];
	sprintf(result_name, "%s/ǰ��ͼչ��778866.bmp", result_image_path);
	cvSaveImage(result_name, image_result_single_front_lut);
#endif
	return ret;
}

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
#if DEBUG_INFO
	IplImage* image[4] = { NULL, NULL, NULL, NULL };
	ret = my_load_image(image);
	CHECK_ERROR(ret);
#endif
	Float64_t 	z_shift;
	Float64_t 	is_front_cam;
	if (cam_flag == CAM_FRONT)
	{
		single_view_param = p_avm_param->p_avm_config_param->avm_single_view_param.front_single_view_param;
		p_lut = p_avm_param->p_avm_lut->p_lut_single_view_front;
		z_shift = -396.88666888;
		is_front_cam = 1;
	}
	if (cam_flag == CAM_REAR)
	{
		single_view_param = p_avm_param->p_avm_config_param->avm_single_view_param.rear_single_view_param;
		p_lut = p_avm_param->p_avm_lut->p_lut_single_view_rear;
		z_shift = -480.88666888;
		is_front_cam = -1;
		
	}
#if DEBUG_INFO
	CvSize size0 = cvSize(single_view_param.result_image_width, single_view_param.result_image_height);//
	IplImage *image_result_single_front_lut = cvCreateImage(size0, IPL_DEPTH_8U, 1);
	memset(image_result_single_front_lut->imageData, 0, single_view_param.result_image_width*single_view_param.result_image_height);
#endif
#if 0
	Float64_t cam_ray[3];
	Float64_t tan_h = tan((single_view_param.box_expand_param.fov_h_center / 2 / 180)*PI);
	Float64_t tan_v = tan((single_view_param.box_expand_param.fov_v / 2 / 180)*PI);
	cam_ray[2] = -8000.00;
	Float64_t   con_num = cam_ray[2];
	Float64_t  pixel_center_width_dis = -(cam_ray[2] * tan_h) / (single_view_param.box_expand_param.center_width / 2);//���м��ӽ�Ϊ��������ʵ��ÿ�����ش�����ʵ�Ŀ�
	Float64_t  pixel_center_height_dis = -(cam_ray[2] * tan_v) / (single_view_param.result_image_height / 2);//���м��ӽ�Ϊ��������ʵ��ÿ�����ش�����ʵ�Ŀ�
	Float64_t  camera_coor0 = -(single_view_param.box_expand_param.center_width / 2)*pixel_center_width_dis;
	Float64_t  camera_coor1 = -camera_coor0;
	double angle = single_view_param.box_expand_param.fov_h_side;
#endif

	Float64_t fov_h_center = single_view_param.box_expand_param.fov_h_center*PI/180;
	Float64_t fov_h_side = single_view_param.box_expand_param.fov_h_side*PI/180;
	Float64_t fov_v = single_view_param.box_expand_param.fov_v*PI/180;

	Int32_t result_width = single_view_param.result_image_width;
	Int32_t result_height = single_view_param.result_image_height;
	Int32_t box_center_width = single_view_param.box_expand_param.center_width;

	Float64_t focal_length = 1000; //unit is mm
	Float64_t box_fold_angle = (fov_h_center + fov_h_side) * 0.5;
	Float64_t box_side_width = (result_width - box_center_width) * 0.5;

	//����չ��ͼ��ÿ�������ڸ߿����������ʵ�ʾ���
	Float64_t 	mm_per_pixel_h_center = focal_length * tan(fov_h_center * 0.5) * 2 / box_center_width;
	Float64_t mm_per_pixel_h_side = focal_length / cos(fov_h_center * 0.5) * sin(fov_h_side * 0.5) * 2 / box_side_width;
	Float64_t mm_per_pixel_v = focal_length * tan(fov_v * 0.5) * 2 / result_height;

	Float64_t pose_t_0[3];
	Float64_t pose_t[3];
	Float64_t inv_pose[3];
	for (int i = 3; i < 6; i++)
	{
		inv_pose[i - 3] = p_avm_param->p_avm_config_param->camera_model[cam_flag].camera_model_ext.inv_pose[i];
	}
	p_avm_param->p_avm_config_param->camera_model[cam_flag].camera_model_ext.inv_R;
	Mat_Mul_Vec_3by3(pose_t_0, p_avm_param->p_avm_config_param->camera_model[cam_flag].camera_model_ext.R, inv_pose);
	pose_t[0] = -pose_t_0[0];
	pose_t[1] = -pose_t_0[1];
	pose_t[2] = -pose_t_0[2];

	



		//set init point's coordinate of each part
	Float64_t init_world_coord_x_left = pose_t[0] - is_front_cam * (mm_per_pixel_h_center * box_center_width * 0.5 + mm_per_pixel_h_side * box_side_width * cos(box_fold_angle));
	Float64_t init_world_coord_y_left = pose_t[1] + is_front_cam * (focal_length - mm_per_pixel_h_side * box_side_width * sin(box_fold_angle));
	Float64_t init_world_coord_z_left = pose_t[2] + mm_per_pixel_v * result_height * 0.5 + z_shift;

	Float64_t init_world_coord_x_center = pose_t[0] - is_front_cam * mm_per_pixel_h_center * box_center_width * 0.5;
	Float64_t init_world_coord_y_center = pose_t[1] + is_front_cam * focal_length;
	Float64_t init_world_coord_z_center = pose_t[2] + mm_per_pixel_v * result_height * 0.5 + z_shift;

	Float64_t init_world_coord_x_right = pose_t[0] + is_front_cam * mm_per_pixel_h_center * box_center_width * 0.5;
	Float64_t init_world_coord_y_right = pose_t[1] + is_front_cam * focal_length;
	Float64_t init_world_coord_z_right = pose_t[2] + mm_per_pixel_v * result_height * 0.5 + z_shift;



	for (int i = 0; i < single_view_param.result_image_height; i++)
	{
		for (int j = 0; j < single_view_param.result_image_width; j++)
		{
			Float64_t world_coord[3];
			if (j < box_side_width)
			{
				world_coord[0] = init_world_coord_x_left + is_front_cam * j * mm_per_pixel_h_side * cos(box_fold_angle);
				world_coord[1] = init_world_coord_y_left + is_front_cam * j * mm_per_pixel_h_side * sin(box_fold_angle);
				world_coord[2] = init_world_coord_z_left - i * mm_per_pixel_v;
			}
			else if(j <= box_side_width + box_center_width)
			{
				world_coord[0] = init_world_coord_x_center + is_front_cam * (j - box_side_width) * mm_per_pixel_h_center;
				world_coord[1] = init_world_coord_y_center;
				world_coord[2] = init_world_coord_z_center - i * mm_per_pixel_v;
			}
			else
			{
				world_coord[0] = init_world_coord_x_right + is_front_cam * (j - box_side_width - box_center_width) * mm_per_pixel_h_side * cos(box_fold_angle);
				world_coord[1] = init_world_coord_y_right - is_front_cam * (j- box_side_width - box_center_width) * mm_per_pixel_h_side * sin(box_fold_angle);
				world_coord[2] = init_world_coord_z_right -i* mm_per_pixel_v;
			}
			Float64_t image_point[2];
			world_coordinate_to_pic_coordinate(CAM_FRONT, world_coord, image_point, p_avm_param);//0 is front
			UInt32_t  shift_altogether = i*single_view_param.result_image_width * 2 + j * 2;
			single_view_zip_data(shift_altogether, image_point, p_lut, cam_flag);

#if DEBUG_INFO

			int rgb[3];
			bilinear_interpolation(image[cam_flag], rgb, image_point[1], image_point[0]);
			image_result_single_front_lut->imageData[i *single_view_param.result_image_width + j] = (uchar)(rgb[0]);
#endif
				// get point's image coordinate
				//camera_coord = inv_R * world_coord' + inv_t;

#if 0
			if (j>((single_view_param.result_image_width / 2) - (single_view_param.box_expand_param.center_width / 2))//�м���ӽ�
				&& j < ((single_view_param.result_image_width / 2) + (single_view_param.box_expand_param.center_width / 2)))
			{

				cam_ray[1] = (Float64_t)((Float64_t(j) - ((Float64_t)(single_view_param.result_image_width) 
							/ 2.00))* pixel_center_width_dis);
				cam_ray[0] = (Float64_t)((Float64_t(i) - ((Float64_t)(single_view_param.result_image_height)
							/ 2.00))* pixel_center_height_dis);

				Float64_t image_point[2];
				Cam_Ray_To_Image_Point(image_point, cam_ray, &(p_avm_param->p_avm_config_param->camera_model[0].camera_model_int));
				if (cam_flag == CAM_FRONT)
				{
					/*******zip data***********/
					UInt32_t  shift_altogether = i*single_view_param.result_image_width * 2 + j * 2;
					single_view_zip_data(shift_altogether, image_point, p_lut, cam_flag);
#if DEBUG_INFO
					int rgb[3];
					bilinear_interpolation(image[cam_flag], rgb, image_point[1], image_point[0]);
					image_result_single_front_lut->imageData[i *single_view_param.result_image_width + j] = (uchar)(rgb[0]);
#endif
				}
				if (cam_flag == CAM_REAR)
				{
					/*******zip data***********/
					UInt32_t  shift_altogether = (single_view_param.result_image_height - i - 1)
											  *single_view_param.result_image_width * 2 
											  + (single_view_param.result_image_width - j-1) * 2;
					single_view_zip_data(shift_altogether, image_point, p_lut, cam_flag);
#if DEBUG_INFO
					int rgb[3];
					bilinear_interpolation(image[cam_flag], rgb, image_point[1], image_point[0]);
					image_result_single_front_lut->imageData[(single_view_param.result_image_height - i - 1) 
													*single_view_param.result_image_width 
													+ (single_view_param.result_image_width - j - 1)] = (uchar)(rgb[0]);
#endif
				}
			}
			if (j<=((single_view_param.result_image_width / 2) - (single_view_param.box_expand_param.center_width / 2)))//����չ�����ӽ�
			{

				cam_ray[1] = (Float64_t)((Float64_t(j) - ((Float64_t)(single_view_param.result_image_width) / 2.00))* pixel_center_width_dis);
				cam_ray[0] = (Float64_t)((Float64_t(i) - ((Float64_t)(single_view_param.result_image_height) / 2.00))* pixel_center_height_dis);
				cam_ray[2] = (con_num - con_num)*cos((angle / 180)*PI) - (cam_ray[1] - camera_coor0)*sin((angle / 180)*PI) + con_num;///is z
				cam_ray[1] = (con_num - con_num)*sin((angle / 180)*PI) + (cam_ray[1] - camera_coor0)*cos((angle / 180)*PI) + camera_coor0;//is y
				Float64_t image_point[2];
				Cam_Ray_To_Image_Point(image_point, cam_ray, &(p_avm_param->p_avm_config_param->camera_model[0].camera_model_int));
				if (cam_flag == 0)
				{
					/*******zip data***********/
					UInt32_t  shift_altogether = i*single_view_param.result_image_width * 2 + j * 2;
					single_view_zip_data(shift_altogether, image_point, p_lut, cam_flag);
#if DEBUG_INFO
					int rgb[3];
					bilinear_interpolation(image[cam_flag], rgb, image_point[1], image_point[0]);
					image_result_single_front_lut->imageData[i *single_view_param.result_image_width + j] = (uchar)(rgb[0]);
#endif
				}
				if (cam_flag == 1)
				{
					/*******zip data***********/
					UInt32_t  shift_altogether = (single_view_param.result_image_height - i - 1)*single_view_param.result_image_width * 2
						    + (single_view_param.result_image_width - j - 1) * 2;
					single_view_zip_data(shift_altogether, image_point, p_lut, cam_flag);
#if DEBUG_INFO
					int rgb[3];
					bilinear_interpolation(image[cam_flag], rgb, image_point[1], image_point[0]);
					image_result_single_front_lut->imageData[(single_view_param.result_image_height - i - 1) 
						*single_view_param.result_image_width + (single_view_param.result_image_width - j - 1)] = (uchar)(rgb[0]);
#endif
				}
			}
			if (j >= ((single_view_param.result_image_width / 2) + (single_view_param.box_expand_param.center_width / 2)))//����չ�����ӽ�
			{

				cam_ray[1] = (Float64_t)((Float64_t(j) - ((Float64_t)(single_view_param.result_image_width) / 2.00))* pixel_center_width_dis);
				cam_ray[0] = (Float64_t)((Float64_t(i) - ((Float64_t)(single_view_param.result_image_height) / 2.00))* pixel_center_height_dis);
				cam_ray[2] = (con_num - con_num)*cos((-angle / 180)*PI) - (cam_ray[1] - camera_coor1)*sin((-angle / 180)*PI) + con_num;///is z
				cam_ray[1] = (con_num - con_num)*sin((-angle / 180)*PI) + (cam_ray[1] - camera_coor1)*cos((-angle / 180)*PI) + camera_coor1;//is y
				Float64_t image_point[2];
				Cam_Ray_To_Image_Point(image_point, cam_ray, &(p_avm_param->p_avm_config_param->camera_model[0].camera_model_int));
				if (cam_flag == 0)
				{
					/*******zip data***********/
					UInt32_t  shift_altogether = i*single_view_param.result_image_width * 2 + j * 2;
					single_view_zip_data(shift_altogether, image_point, p_lut, cam_flag);
#if DEBUG_INFO
					int rgb[3];
					bilinear_interpolation(image[cam_flag], rgb, image_point[1], image_point[0]);
					image_result_single_front_lut->imageData[i *single_view_param.result_image_width + j] = (uchar)(rgb[0]);
#endif
				}
				if (cam_flag == 1)
				{
					/*******zip data***********/
					UInt32_t  shift_altogether = (single_view_param.result_image_height - i - 1)*single_view_param.result_image_width * 2 + (single_view_param.result_image_width - j - 1) * 2;
					single_view_zip_data(shift_altogether, image_point, p_lut, cam_flag);
#if DEBUG_INFO
					int rgb[3];
					bilinear_interpolation(image[cam_flag], rgb, image_point[1], image_point[0]);
					image_result_single_front_lut->imageData[(single_view_param.result_image_height - i - 1)
					*single_view_param.result_image_width + (single_view_param.result_image_width - j - 1)] = (uchar)(rgb[0]);
#endif
				}
			}
#endif

		}
	}
#if DEBUG_INFO
	char result_name[MAX_ARRAY_SIZE];
	sprintf(result_name, "%s/box________.bmp", result_image_path);
	cvSaveImage(result_name, image_result_single_front_lut);
#endif
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
#if DEBUG_INFO
	IplImage* image[4] = { NULL, NULL, NULL, NULL };
	ret = my_load_image(image);
	CHECK_ERROR(ret);
	IplImage *image_result_single_front_lut = cvCreateImage(size0, IPL_DEPTH_8U, 1);
	memset(image_result_single_front_lut->imageData, 0, single_view_param.result_image_width*single_view_param.result_image_height);
#endif
	Float64_t scale_coordinate_col = Float64_t(single_view_param.crop_param.crop_rect.width) 
									/ Float64_t(single_view_param.result_image_width);//��ʾ���εĿ�Ӱ�䵽
	Float64_t scale_coordinate_row = Float64_t(single_view_param.crop_param.crop_rect.height) 
									/ Float64_t(single_view_param.result_image_height);

	for (int i = 0; i < single_view_param.result_image_height; i++)
	{
		for (int j = 0; j < single_view_param.result_image_width; j++)
		{
			Float64_t image_point[2];
			Float64_t image_point_0[2];
			/**************************************************/
			image_point[1] = single_view_param.crop_param.crop_rect.y + i*scale_coordinate_row;
			image_point[0] = single_view_param.crop_param.crop_rect.x + j*scale_coordinate_col;
			image_point_0[0] = (image_point[0] - single_view_param.crop_param.crop_rect.x)*cos((single_view_param.crop_param.roll_angle / 180)*PI)
							 - (image_point[1] - single_view_param.crop_param.crop_rect.y)*sin((single_view_param.crop_param.roll_angle / 180)*PI) //Ϊ������ת��ʽ�����ϵ���ת�û�����ĽǶ�
							 + single_view_param.crop_param.crop_rect.x;//
			image_point_0[1] = (image_point[0] - single_view_param.crop_param.crop_rect.x)*sin((single_view_param.crop_param.roll_angle / 180)*PI)
							 + (image_point[1] - single_view_param.crop_param.crop_rect.y)*cos((single_view_param.crop_param.roll_angle / 180)*PI) 
							 + single_view_param.crop_param.crop_rect.y;//
			UInt32_t  shift_altogether = i*single_view_param.result_image_width * 2 + j * 2;
			single_view_zip_data(shift_altogether, image_point, p_lut, cam_flag);//������ѹ��lut��
#if DEBUG_INFO
			int rgb[3];
			bilinear_interpolation(image[cam_flag], rgb, image_point_0[0], image_point_0[1]);
			image_result_single_front_lut->imageData[i *single_view_param.result_image_width + j] = (uchar)(rgb[0]);
#endif
		}
	}
#if DEBUG_INFO
	char result_name[MAX_ARRAY_SIZE];
	sprintf(result_name, "%s/crop_left.bmp", result_image_path);
	cvSaveImage(result_name, image_result_single_front_lut);
#endif
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
void calculate_world_coordinate_left_right(OUT Float64_t *world_coordinate, IN int i, IN int j, IN P_Avm_Param_S p_avm_param
	, IN Left_Right_Single_View_Param_S left_right_view_param, int flag)
{
	Float64_t pixel_row_dis = (left_right_view_param.plane_expand_param.world_height)
		/ (Float32_t)(left_right_view_param.result_image_height);//calculate every pixel distance
	Float64_t pixel_col_dis = (left_right_view_param.plane_expand_param.world_width)
		/ (Float32_t)(left_right_view_param.result_image_width);//calculate every pixel distance//calculate every pixel distance
	if (flag == CAM_LEFT)
	{
		world_coordinate[0] = Float64_t(pixel_col_dis)*Float64_t(j) - left_right_view_param.plane_expand_param.world_width
						- p_avm_param->p_avm_config_param->vehicle_param.vehicle_width / 2;
		world_coordinate[1] = left_right_view_param.plane_expand_param.world_height / 2 - Float64_t(pixel_row_dis)*Float64_t(i);
	}
	if (flag == CAM_RIGHT)
	{
		world_coordinate[0] = Float64_t(pixel_col_dis)*Float64_t(j) + p_avm_param->p_avm_config_param->vehicle_param.vehicle_width / 2;
		world_coordinate[1] = left_right_view_param.plane_expand_param.world_height / 2 - Float64_t(pixel_row_dis)*Float64_t(i);
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
#if DEBUG_INFO
	IplImage* image[4] = { NULL, NULL, NULL, NULL };
	ret = my_load_image(image);
	IplImage *image_result_single_front_lut = cvCreateImage(size0, IPL_DEPTH_8U, 1);
	memset(image_result_single_front_lut->imageData, 0, single_view_param.result_image_width*single_view_param.result_image_height);
#endif
	
	Float64_t dx = (single_view_param.plane_expand_param.world_width)
		/ (Float32_t)(single_view_param.result_image_width);
	Float64_t dy = (single_view_param.plane_expand_param.world_height)
		/ (Float32_t)(single_view_param.result_image_height);
	Float64_t world_init_x_left;
	Float64_t world_init_y_left;

	if (cam_flag == CAM_LEFT)
	{
		world_init_x_left = -(single_view_param.plane_expand_param.world_width
			+ (p_avm_param->p_avm_config_param->vehicle_param.vehicle_width * 0.5 - single_view_param.plane_expand_param.car_body_width * dx));

		world_init_y_left = single_view_param.plane_expand_param.world_height * 0.5 - single_view_param.plane_expand_param.trans_y;
	}


	if (cam_flag == CAM_RIGHT)
	{
		world_init_x_left = p_avm_param->p_avm_config_param->vehicle_param.vehicle_width * 0.5 - single_view_param.plane_expand_param.car_body_width * dx;
		world_init_y_left = single_view_param.plane_expand_param.world_height * 0.5 - single_view_param.plane_expand_param.trans_y;
	}

	/********************extend all over result image pixel****************************/
	for (int i = 0; i < single_view_param.result_image_height; i++)
	{
		for (int j = 0; j < single_view_param.result_image_width; j++)
		{

			
			Float64_t world_coord[3];
			//= [world_init_x_left + (j - 1) * dx; world_init_y_left - (i - 1) * dy;  0;  1];
			world_coord[0] = world_init_x_left + (j) * dx;
			world_coord[1] = world_init_y_left - (i) * dy;
			world_coord[2] = 0;
			Float64_t image_point[2];
			world_coordinate_to_pic_coordinate(cam_flag, world_coord, image_point, p_avm_param);






#if 0
			/***********calculate world coordinate*********/
			Float64_t image_point[2];
			calculate_world_coordinate_left_right(world_coordinate, i, j, p_avm_param, single_view_param,cam_flag);//��ͼ������ת��Ϊ��������
			UInt32_t  shift_altogether = i*single_view_param.result_image_width * 2 + j * 2;
			single_view_zip_data(shift_altogether, image_point, p_lut, cam_flag);//���lut��
			world_coordinate_to_pic_coordinate(cam_flag, world_coordinate, image_point, p_avm_param);
#endif 
#if DEBUG_INFO
			int rgb[3];
			bilinear_interpolation(image[cam_flag], rgb, image_point[1], image_point[0]);
			image_result_single_front_lut->imageData[i *single_view_param.result_image_width + j] = (uchar)(rgb[0]);
#endif
		}
	}
#if DEBUG_INFO
	char result_name[MAX_ARRAY_SIZE];
	sprintf(result_name, "%s/over_look_lr.bmp", result_image_path);
	cvSaveImage(result_name, image_result_single_front_lut);
#endif
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
void calculate_world_coordinate_combine_left_right(OUT Float64_t *world_coordinate, IN int i, IN int j, IN P_Avm_Param_S p_avm_param
	, IN Left_Right_Single_View_Param_S left_right_view_param,int cam_flag)
{
	Float64_t pixel_row_dis = (left_right_view_param.combine_expand_param.world_height)
		/ (Float32_t)(left_right_view_param.result_image_height);//calculate every pixel distance
	Float64_t pixel_col_dis = (left_right_view_param.combine_expand_param.plane_width)
		/ (Float32_t)(left_right_view_param.result_image_width *left_right_view_param.combine_expand_param.plane_ratio);//calculate every pixel distance//calculate every pixel distance
	if (cam_flag == CAM_LEFT)
	{
		world_coordinate[0] = -Float64_t(pixel_col_dis)*Float64_t(left_right_view_param.result_image_width - j) - p_avm_param->p_avm_config_param->vehicle_param.vehicle_width / 2;
	}
	if (cam_flag == CAM_RIGHT)
	{
		world_coordinate[0] = Float64_t(pixel_col_dis)*Float64_t(j) + p_avm_param->p_avm_config_param->vehicle_param.vehicle_width / 2;
	}
	world_coordinate[1] = left_right_view_param.combine_expand_param.world_height / 2 - Float64_t(pixel_row_dis)*Float64_t(i);
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

	AVM_Error_Code_E ret = ERROR_NONE;
    #define R 1000.000//�����R������Բ������
	Left_Right_Single_View_Param_S single_view_param;
	CvSize size0;
	UInt32_t* p_lut = NULL;
	Float64_t x_z[2];
	Float64_t world_coordinate_0[2];
	if (cam_flag == CAM_LEFT)
	{
		single_view_param = p_avm_param->p_avm_config_param->avm_single_view_param.left_single_view_param;
		size0 = cvSize(single_view_param.result_image_width, single_view_param.result_image_height);//
		p_lut = p_avm_param->p_avm_lut->p_lut_single_view_left;
		
		world_coordinate_0[0] = -single_view_param.combine_expand_param.plane_width 
								- p_avm_param->p_avm_config_param->vehicle_param.vehicle_width / 2;
		world_coordinate_0[1] = 0;
		x_z[0] = -single_view_param.combine_expand_param.plane_width - p_avm_param
					->p_avm_config_param->vehicle_param.vehicle_width / 2;//ƽ�沿�ֵı߽��x��z������
		x_z[1] = R;//��ת��Բ��
	}
	if (cam_flag == CAM_RIGHT)
	{
		single_view_param = p_avm_param->p_avm_config_param->avm_single_view_param.right_single_view_param;
		size0 = cvSize(single_view_param.result_image_width, single_view_param.result_image_height);//
		p_lut = p_avm_param->p_avm_lut->p_lut_single_view_right;
		world_coordinate_0[0] = single_view_param.combine_expand_param.plane_width 
							+ p_avm_param->p_avm_config_param->vehicle_param.vehicle_width / 2;
		world_coordinate_0[1] = 0;
		x_z[0] = single_view_param.combine_expand_param.plane_width 
				+ p_avm_param->p_avm_config_param->vehicle_param.vehicle_width / 2;//�ӽ�ƽ�沿�ֵı߽��x��z����ת����Բ��
		x_z[1] = R;
	}

#if DEBUG_INFO
	IplImage* image[4] = { NULL, NULL, NULL, NULL };
	ret = my_load_image(image);
	IplImage *image_result_single_front_lut = cvCreateImage(size0, IPL_DEPTH_8U, 1);
	memset(image_result_single_front_lut->imageData, 0
			, single_view_param.result_image_width*single_view_param.result_image_height);
#endif
	Float64_t world_coordinate[3] = { 0, 0, 0 };
	/********************extend all over result image pixel****************************/
	for (int i = 0; i < single_view_param.result_image_height; i++)
	{
		for (int j = 0; j < single_view_param.result_image_width; j++)
		{
			/***********calculate world coordinate*********/
			Float64_t image_point[2];
			world_coordinate[2] = 0;
			calculate_world_coordinate_combine_left_right(world_coordinate, i, j, p_avm_param, single_view_param,cam_flag);
			if (cam_flag == CAM_RIGHT)
			{
				/**********right***********/
				if (j<=single_view_param.result_image_width*single_view_param.combine_expand_param.plane_ratio)
				{
					world_coordinate[2] = 0;
					world_coordinate_to_pic_coordinate(cam_flag, world_coordinate, image_point, p_avm_param);
					UInt32_t  shift_altogether = i*single_view_param.result_image_width * 2 + j * 2;
					single_view_zip_data(shift_altogether, image_point, p_lut, cam_flag);
#if DEBUG_INFO
					int rgb[3];
					bilinear_interpolation(image[cam_flag], rgb, image_point[1], image_point[0]);
					image_result_single_front_lut->imageData[i *single_view_param.result_image_width + j] = (uchar)(rgb[0]);
#endif
				}
				if (j>=single_view_param.result_image_width*single_view_param.combine_expand_param.plane_ratio + 1)
				{
					Float64_t m = (abs(world_coordinate[0]) - single_view_param.combine_expand_param.plane_width 
								- p_avm_param->p_avm_config_param->vehicle_param.vehicle_width / 2);
					Float64_t angle_temp = atan(m / R);
					world_coordinate[0] = (world_coordinate_0[0] - x_z[0])*cos(angle_temp) 
										- (world_coordinate_0[1] - x_z[1])*sin(angle_temp) + x_z[0];///is z
					world_coordinate[2] = (world_coordinate_0[0] - x_z[0])*sin(angle_temp)
										+ (world_coordinate_0[1] - x_z[1])*cos(angle_temp) + x_z[1];//is y
					world_coordinate_to_pic_coordinate(cam_flag, world_coordinate, image_point, p_avm_param);

					UInt32_t  shift_altogether = i*single_view_param.result_image_width * 2 + j * 2;
					single_view_zip_data(shift_altogether, image_point, p_lut, cam_flag);
#if DEBUG_INFO
					int rgb[3];
					bilinear_interpolation(image[cam_flag], rgb, image_point[1], image_point[0]);
					image_result_single_front_lut->imageData[i *single_view_param.result_image_width + j] = (uchar)(rgb[0]);
#endif
				}

			}
			if (cam_flag == CAM_LEFT)
			{
				/************left*********/
				if (j >= single_view_param.result_image_width*(1.000 - single_view_param.combine_expand_param.plane_ratio))
				{
					world_coordinate[2] = 0;
					world_coordinate_to_pic_coordinate(cam_flag, world_coordinate, image_point, p_avm_param);
					UInt32_t  shift_altogether = i*single_view_param.result_image_width * 2 + j * 2;
					single_view_zip_data(shift_altogether, image_point, p_lut, cam_flag);
#if DEBUG_INFO
					int rgb[3];
					bilinear_interpolation(image[cam_flag], rgb, image_point[1], image_point[0]);
					image_result_single_front_lut->imageData[i *single_view_param.result_image_width + j] = (uchar)(rgb[0]);
#endif
				}
				if (j <= single_view_param.result_image_width*(1.000 - single_view_param.combine_expand_param.plane_ratio) - 1)
				{
					Float64_t m = (abs(world_coordinate[0]) - single_view_param.combine_expand_param.plane_width
								- p_avm_param->p_avm_config_param->vehicle_param.vehicle_width / 2);
					Float64_t angle_temp = -atan(m / R);
					world_coordinate[0] = ((world_coordinate_0[0] - x_z[0])*cos(angle_temp) 
										- (world_coordinate_0[1] - x_z[1])*sin(angle_temp) + x_z[0]);///is z
					world_coordinate[2] = (world_coordinate_0[0] - x_z[0])*sin(angle_temp) 
										+ (world_coordinate_0[1] - x_z[1])*cos(angle_temp) + x_z[1];//is y
					world_coordinate_to_pic_coordinate(cam_flag, world_coordinate, image_point, p_avm_param);
					UInt32_t  shift_altogether = i*single_view_param.result_image_width * 2 + j * 2;
					single_view_zip_data(shift_altogether, image_point, p_lut, cam_flag);
#if DEBUG_INFO
					int rgb[3];
					bilinear_interpolation(image[cam_flag], rgb, image_point[1], image_point[0]);
					image_result_single_front_lut->imageData[i *single_view_param.result_image_width + j] = (uchar)(rgb[0]);
#endif
				}
			}

		}
	}
#if DEBUG_INFO
	char result_name[MAX_ARRAY_SIZE];
	sprintf(result_name, "%s/combine_overlook_lr.bmp", result_image_path);
	cvSaveImage(result_name, image_result_single_front_lut);
#endif
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
void calculate_world_coordinate_comine_left_right(OUT Float64_t *world_coordinate, IN int i, IN int j, IN P_Avm_Param_S p_avm_param
	, IN Single_View_Left_Right_Plane_Expand_Param_S left_right_view_param, int flag)
{
	Float64_t pixel_row_dis = (left_right_view_param.world_height)
		/ (Float32_t)(p_avm_param->p_avm_config_param->avm_single_view_param.left_right_view_param.result_image_height);//calculate every pixel distance
	Float64_t pixel_col_dis = (left_right_view_param.world_width)
		/ ((Float32_t)p_avm_param->p_avm_config_param->avm_single_view_param.left_right_view_param.result_image_width/2.000);//calculate every pixel distance//calculate every pixel distance
	if (flag == CAM_LEFT)
	{
		world_coordinate[0] = Float64_t(pixel_col_dis)*Float64_t(j) - left_right_view_param.world_width
							- p_avm_param->p_avm_config_param->vehicle_param.vehicle_width / 2;
		world_coordinate[1] = left_right_view_param.world_height / 2 - Float64_t(pixel_row_dis)*Float64_t(i);
	}
	if (flag == CAM_RIGHT)
	{
		world_coordinate[0] = Float64_t(pixel_col_dis)*Float64_t(j) + p_avm_param->p_avm_config_param->vehicle_param.vehicle_width / 2;
		world_coordinate[1] = left_right_view_param.world_height / 2 - Float64_t(pixel_row_dis)*Float64_t(i);
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
#define DEBUG_INFO 1

AVM_Error_Code_E lr_single_overlook(IN P_Avm_Param_S p_avm_param)
{
	AVM_Error_Code_E ret = ERROR_NONE;
	UInt32_t* p_lut = NULL;
	Left_Right_View_Param_S	left_right_view_param;
	CvSize size0;
	left_right_view_param = p_avm_param->p_avm_config_param->avm_single_view_param.left_right_view_param;
	size0 = cvSize(left_right_view_param.result_image_width, left_right_view_param.result_image_height);
	p_lut = p_avm_param->p_avm_lut->p_lut_single_view_left_right;
	
#if DEBUG_INFO
	IplImage* image[4] = { NULL, NULL, NULL, NULL };
	ret = my_load_image(image);
	IplImage *image_result_single_front_lut = cvCreateImage(size0, IPL_DEPTH_8U, 1);
	memset(image_result_single_front_lut->imageData, 0, left_right_view_param.result_image_width*left_right_view_param.result_image_height);
#endif
	Float64_t world_coordinate[3] = { 0, 0, 0 };
	//left_right_view_param.combine_expand_param_left.;
	Float64_t dx_left = (left_right_view_param.plane_expand_param_left.world_width)
		/ (Float32_t)(left_right_view_param.result_image_width/2);
	Float64_t dy_left = (left_right_view_param.plane_expand_param_left.world_height)
		/ (Float32_t)(left_right_view_param.result_image_height);
	Float64_t dx_right = (left_right_view_param.plane_expand_param_right.world_width)
		/ (Float32_t)(left_right_view_param.result_image_width/2);
	Float64_t dy_right = (left_right_view_param.plane_expand_param_right.world_height)
		/ (Float32_t)(left_right_view_param.result_image_height);

	Float64_t world_init_x_left;
	Float64_t world_init_y_left;
	Float64_t world_init_x_right;
	Float64_t world_init_y_right;

	world_init_x_left = -(left_right_view_param.plane_expand_param_left.world_width
		+ (p_avm_param->p_avm_config_param->vehicle_param.vehicle_width * 0.5 - left_right_view_param.plane_expand_param_left.car_body_width * dx_left));
	world_init_y_left = left_right_view_param.plane_expand_param_left.world_height * 0.5 - left_right_view_param.plane_expand_param_left.trans_y;
	
	world_init_x_right =(p_avm_param->p_avm_config_param->vehicle_param.vehicle_width * 0.5 - left_right_view_param.plane_expand_param_right.car_body_width * dx_right);
	world_init_y_right = left_right_view_param.plane_expand_param_right.world_height * 0.5 - left_right_view_param.plane_expand_param_right.trans_y;






	/********************extend all over result image pixel****************************/
	for (int i = 0; i < left_right_view_param.result_image_height; i++)
	{
		for (int j = 0; j < left_right_view_param.result_image_width; j++)
		{
			/***********calculate world coordinate*********/
			Float64_t image_point[2];
			Float64_t world_coord[3];
			if (j < (left_right_view_param.result_image_width / 2))
			{
				world_coord[0] = world_init_x_left + (j)* dx_left;
				world_coord[1] = world_init_y_left - (i)* dy_left;
				world_coord[2] = 0;

				world_coordinate_to_pic_coordinate(CAM_LEFT, world_coord, image_point, p_avm_param);
				int rgb[3];
				bilinear_interpolation(image[CAM_LEFT], rgb, image_point[1], image_point[0]);
				image_result_single_front_lut->imageData[i *left_right_view_param.result_image_width + j] = (uchar)(rgb[0]);

			}
			
			if (j > (left_right_view_param.result_image_width / 2))
			{
				world_coord[0] = world_init_x_right + (j - left_right_view_param.result_image_width / 2)* dx_right;
				world_coord[1] = world_init_y_right - (i)* dy_right;
				world_coord[2] = 0;

				world_coordinate_to_pic_coordinate(CAM_RIGHT, world_coord, image_point, p_avm_param);
				int rgb[3];
				bilinear_interpolation(image[CAM_RIGHT], rgb, image_point[1], image_point[0]);
				image_result_single_front_lut->imageData[i *left_right_view_param.result_image_width + j] = (uchar)(rgb[0]);

			}



#if 0
			if (j < (left_right_view_param.result_image_width / 2))
					{
				calculate_world_coordinate_comine_left_right(world_coordinate, i, j, p_avm_param,
									p_avm_param->p_avm_config_param->avm_single_view_param.left_right_view_param.plane_expand_param_left, CAM_LEFT);
				world_coordinate_to_pic_coordinate(CAM_LEFT, world_coordinate, image_point, p_avm_param);
#if DEBUG_INFO
				int rgb[3];
				bilinear_interpolation(image[CAM_LEFT], rgb, image_point[1], image_point[0]);
				image_result_single_front_lut->imageData[i *left_right_view_param.result_image_width + j] = (uchar)(rgb[0]);
#endif
			}

			if (j > (left_right_view_param.result_image_width / 2))
			{
				calculate_world_coordinate_comine_left_right(world_coordinate, i, j - (left_right_view_param.result_image_width / 2), p_avm_param,
									p_avm_param->p_avm_config_param->avm_single_view_param.left_right_view_param.plane_expand_param_right, CAM_RIGHT);
				world_coordinate_to_pic_coordinate(CAM_RIGHT, world_coordinate, image_point, p_avm_param);
#if DEBUG_INFO
				int rgb[3];
				bilinear_interpolation(image[CAM_RIGHT], rgb, image_point[1], image_point[0]);
				image_result_single_front_lut->imageData[i *left_right_view_param.result_image_width + j] = (uchar)(rgb[0]);
#endif
			}
#endif

			UInt32_t  shift_altogether = i*left_right_view_param.result_image_width * 2 + j * 2;
			single_view_zip_data(shift_altogether, image_point, p_lut, 2);
		
		}
	}
#if DEBUG_INFO
	char result_name[MAX_ARRAY_SIZE];
	sprintf(result_name, "%s/over_look_combine.bmp", result_image_path);
	cvSaveImage(result_name, image_result_single_front_lut);
#endif
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
void lr_calculate_world_coordinate_combine(OUT Float64_t *world_coordinate, IN int i, IN int j, IN P_Avm_Param_S p_avm_param
	, IN Left_Right_View_Param_S left_right_view_param, int cam_flag)
{
	Float64_t pixel_row_dis;
	Float64_t pixel_col_dis;
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
		world_coordinate[0] = -Float64_t(pixel_col_dis)*Float64_t(left_right_view_param.result_image_width/2 - j)
							- (p_avm_param->p_avm_config_param->vehicle_param.vehicle_width / 2)*0.6;
		world_coordinate[1] = left_right_view_param.combine_expand_param_left.world_height / 2
							- Float64_t(pixel_row_dis)*Float64_t(i);

	}
	if (cam_flag == CAM_RIGHT)
	{
		world_coordinate[0] = Float64_t(pixel_col_dis)*Float64_t(j) 
							+ (p_avm_param->p_avm_config_param->vehicle_param.vehicle_width / 2)*0.6;
		world_coordinate[1] = left_right_view_param.combine_expand_param_right.world_height / 2 
						- Float64_t(pixel_row_dis)*Float64_t(i);
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
	left_right_view_param = p_avm_param->p_avm_config_param->avm_single_view_param.left_right_view_param;
	size0 = cvSize(left_right_view_param.result_image_width, left_right_view_param.result_image_height);
	UInt32_t* p_lut = p_avm_param->p_avm_lut->p_lut_single_view_left_right;
	Float64_t x_z[2];
	Float64_t world_coordinate_0[2];
	world_coordinate_0[0] = left_right_view_param.combine_expand_param_right.plane_width + p_avm_param->p_avm_config_param->vehicle_param.vehicle_width / 2;
	world_coordinate_0[1] = 0;
	x_z[0] = left_right_view_param.combine_expand_param_right.plane_width + p_avm_param->p_avm_config_param->vehicle_param.vehicle_width / 2;//�ӽ�ƽ�沿�ֵı߽��x��z����ת����Բ��
	x_z[1] = R;
#if DEBUG_INFO
	IplImage* image[4] = { NULL, NULL, NULL, NULL };
	ret = my_load_image(image);
	IplImage *image_result_single_front_lut = cvCreateImage(size0, IPL_DEPTH_8U, 1);
	memset(image_result_single_front_lut->imageData, 0, left_right_view_param.result_image_width*left_right_view_param.result_image_height);
#endif
	Float64_t world_coordinate[3] = { 0, 0, 0 };
	/********************extend all over result image pixel****************************/
	for (int i = 0; i < left_right_view_param.result_image_height; i++)
	{
		for (int j = 0; j < left_right_view_param.result_image_width; j++)
		{
			/***********calculate world coordinate*********/
			Float64_t image_point[2];
			world_coordinate[2] = 0;
			if (j>left_right_view_param.result_image_width / 2)
			{

				world_coordinate_0[0] = left_right_view_param.combine_expand_param_right.plane_width + p_avm_param->p_avm_config_param->vehicle_param.vehicle_width / 2;
				world_coordinate_0[1] = 0;
				x_z[0] = left_right_view_param.combine_expand_param_right.plane_width + p_avm_param->p_avm_config_param->vehicle_param.vehicle_width / 2;//�ӽ�ƽ�沿�ֵı߽��x��z����ת����Բ��
				x_z[1] = R;

				lr_calculate_world_coordinate_combine(world_coordinate, i, (j - left_right_view_param.result_image_width / 2), p_avm_param, left_right_view_param, CAM_RIGHT);
				/**********right***********/
				if (j<left_right_view_param.result_image_width / 2 + left_right_view_param.result_image_width*0.5*left_right_view_param.combine_expand_param_right.plane_ratio)
				{
					world_coordinate[2] = 0;
					world_coordinate_to_pic_coordinate(CAM_RIGHT, world_coordinate, image_point, p_avm_param);
					UInt32_t  shift_altogether = i*left_right_view_param.result_image_width * 2 + j * 2;
					single_view_zip_data(shift_altogether, image_point, p_lut, CAM_RIGHT);
#if DEBUG_INFO
					int rgb[3];
					bilinear_interpolation(image[CAM_RIGHT], rgb, image_point[1], image_point[0]);
					image_result_single_front_lut->imageData[i *left_right_view_param.result_image_width + j] = (uchar)(rgb[0]);
#endif
				}
				else
				{
					Float64_t m = (abs(world_coordinate[0]) - left_right_view_param.combine_expand_param_right.plane_width - p_avm_param->p_avm_config_param->vehicle_param.vehicle_width / 2);
					Float64_t angle_temp = atan(m / R);
					world_coordinate[0] = (world_coordinate_0[0] - x_z[0])*cos(angle_temp) - (world_coordinate_0[1] - x_z[1])*sin(angle_temp) + x_z[0];///is z
					world_coordinate[2] = (world_coordinate_0[0] - x_z[0])*sin(angle_temp) + (world_coordinate_0[1] - x_z[1])*cos(angle_temp) + x_z[1];//is y
					world_coordinate_to_pic_coordinate(CAM_RIGHT, world_coordinate, image_point, p_avm_param);

					UInt32_t  shift_altogether = i*left_right_view_param.result_image_width * 2 + j * 2;
					single_view_zip_data(shift_altogether, image_point, p_lut, CAM_RIGHT);
#if DEBUG_INFO
					int rgb[3];
					bilinear_interpolation(image[CAM_RIGHT], rgb, image_point[1], image_point[0]);
					image_result_single_front_lut->imageData[i *left_right_view_param.result_image_width + j] = (uchar)(rgb[0]);
#endif
				}
			}
			/************left*******/
			if (j < left_right_view_param.result_image_width / 2)
			{

				world_coordinate_0[0] = -left_right_view_param.combine_expand_param_left.plane_width - p_avm_param->p_avm_config_param->vehicle_param.vehicle_width / 2;
				world_coordinate_0[1] = 0;
				x_z[0] = -left_right_view_param.combine_expand_param_left.plane_width - p_avm_param->p_avm_config_param->vehicle_param.vehicle_width / 2;//�ӽ�ƽ�沿�ֵı߽��x��z������
				x_z[1] = R;//��ת��Բ��

				lr_calculate_world_coordinate_combine(world_coordinate, i, j, p_avm_param, left_right_view_param, CAM_LEFT);

				/************left*********/
				if (j >(left_right_view_param.result_image_width*0.5*(1.000 - left_right_view_param.combine_expand_param_left.plane_ratio)))
				{
					world_coordinate[2] = 0;
					world_coordinate_to_pic_coordinate(CAM_LEFT, world_coordinate, image_point, p_avm_param);
					//UInt32_t  shift_altogether = i*left_right_view_param.result_image_width * 2 + j * 2;
					//single_view_zip_data(shift_altogether, image_point, p_lut, cam_flag);
#if DEBUG_INFO
					int rgb[3];
					bilinear_interpolation(image[CAM_LEFT], rgb, image_point[1], image_point[0]);
					image_result_single_front_lut->imageData[i *left_right_view_param.result_image_width + j] = (uchar)(rgb[0]);
#endif
				}
				//if (j < left_right_view_param.result_image_width*0.5*(1.000 - left_right_view_param.combine_expand_param_right.plane_ratio) - 1)
				else
				{
					Float64_t m = (abs(world_coordinate[0]) - left_right_view_param.combine_expand_param_left.plane_width - p_avm_param->p_avm_config_param->vehicle_param.vehicle_width / 2);
					Float64_t angle_temp = -atan(m / R);
					world_coordinate[0] = ((world_coordinate_0[0] - x_z[0])*cos(angle_temp) - (world_coordinate_0[1] - x_z[1])*sin(angle_temp) + x_z[0]);///is z
					world_coordinate[2] = (world_coordinate_0[0] - x_z[0])*sin(angle_temp) + (world_coordinate_0[1] - x_z[1])*cos(angle_temp) + x_z[1];//is y
					world_coordinate_to_pic_coordinate(CAM_LEFT, world_coordinate, image_point, p_avm_param);
					
#if DEBUG_INFO
					//UInt32_t  shift_altogether = i*left_right_view_param.result_image_width * 2 + j * 2;
					//single_view_zip_data(shift_altogether, image_point, p_lut, cam_flag);
					int rgb[3];
					bilinear_interpolation(image[CAM_LEFT], rgb, image_point[1], image_point[0]);
					image_result_single_front_lut->imageData[i *left_right_view_param.result_image_width + j] = (uchar)(rgb[0]);
#endif
				}
			}

		}

	}
#if DEBUG_INFO
	/*printf("laotianye miaozhuangnanlingxingxuxianrenbaoyouwo wodebab baoyouwowodebaba baoyouwo miaozhuangnanalingxingxuxianrenbaoyoulaotianyelaotianyebaoyuouwo miaozhuangnanlingxingxuxianrenbaoyoulaotianyebaoyuowolaotianye miaozhuangnanlingxingxuxianrenbaoyouwobaoyouwobaoyouwobaoyouwowodebabbaoyouwowodebababaoyouwuiwodebababaoyouwolaotianyenabaoyouwolaotianyebaoyouwolaotianyebaoyouwomiaozhuangnanalingxingxuxianrenbaoyouwowodebabwodebababaoyouwowodebababaoyouwowodebababaoyouwomiaozhuangnanlingxingxingrenbaoyouwou");
	char result_name[MAX_ARRAY_SIZE];
	sprintf(result_name, "%s/66666668888888888666666666666666666666666666666666666666888888888888888888888888888888888combine_overlook_lr.bmp", result_image_path);
	cvSaveImage(result_name, image_result_single_front_lut);*/
#endif
	return ret;
}