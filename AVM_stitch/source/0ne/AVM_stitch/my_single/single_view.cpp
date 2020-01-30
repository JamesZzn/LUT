
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
		camera_flag；
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
									/ Float64_t(single_view_param.result_image_width);//求出原图与裁剪框 的大小比例

	Float64_t scale_coordinate_row = Float64_t(single_view_param.crop_param.crop_rect.height) 
									/ Float64_t(single_view_param.result_image_height);//求出原图与裁剪框 的大小比例
	for (int i = 0; i < single_view_param.result_image_height; i++)
	{
		for (int j = 0; j < single_view_param.result_image_width; j++)
		{
			Float64_t image_point[2];
			if (cam_flag == CAM_FRONT)//前摄像头
			{
				image_point[0] = single_view_param.crop_param.crop_rect.y + i*scale_coordinate_row;
				image_point[1] = single_view_param.crop_param.crop_rect.x + j*scale_coordinate_col;
			}
			if (cam_flag == CAM_REAR)//
			{
				image_point[0] = single_view_param.crop_param.crop_rect.y 
								+ (single_view_param.result_image_height - 1 - i)*scale_coordinate_row;//因为后摄像头是反装的所以结果图反转一下
				image_point[1] = single_view_param.crop_param.crop_rect.x 
								+ (single_view_param.result_image_width - j - 1)*scale_coordinate_col;//因为后摄像头是反装的所以结果图反转一下
			}

			UInt32_t  shift_altogether = i*single_view_param.result_image_width * 2 + j * 2;//求出填充表的偏移地址
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
	cam_ray[2] = -8000.00;//给出虚拟焦距
	Float64_t  pixel_hight_dis = -(cam_ray[2] * tan_v) / (right_single_view_param.result_image_height / 2);//求出在图像垂直方向上每个像素占多少毫米
	Float64_t  pixel_width_dis = -(cam_ray[2] * tan_h) / (right_single_view_param.result_image_width / 2);//求出在图像水平方向上每个像素占多少毫米


	for (int i = 0; i < right_single_view_param.result_image_height; i++)
	{
		for (int j = 0; j < right_single_view_param.result_image_width; j++)
		{
			/*if (cam_flag == CAM_FRONT)
			{*/
			cam_ray[1] = (Float64_t)(j - right_single_view_param.result_image_width / 2.00)* pixel_width_dis;//转化为相机坐标系
			cam_ray[0] = (Float64_t)(i - right_single_view_param.result_image_height / 2.00)* pixel_hight_dis;//朝下是x
			//}
			//if (cam_flag == CAM_REAR)
			//{
			//	cam_ray[1] = (Float64_t)((Float64_t(j) - ((Float64_t)(single_view_param.result_image_width) / 2.00))* pixel_width_dis);//转化为相机坐标系
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
AVM_Error_Code_E  plane_front_rear_single_view(IN P_Avm_Param_S p_avm_param, IN  int cam_flag)
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
	if (cam_flag == CAM_REAR)
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
	Float64_t cam_ray[3];
	Float64_t tan_h = (tan((single_view_param.plane_expand_param.fov_h / 2 / 180)*PI));
	Float64_t tan_v = (tan((single_view_param.plane_expand_param.fov_v / 2 / 180)*PI));
	cam_ray[2] = -8000.00;//给出虚拟焦距
	Float64_t  pixel_hight_dis = -(cam_ray[2] * tan_v) / (single_view_param.result_image_height / 2);//求出在图像垂直方向上每个像素占多少毫米
	Float64_t  pixel_width_dis = -(cam_ray[2] * tan_h) / (single_view_param.result_image_width / 2);//求出在图像水平方向上每个像素占多少毫米


	for (int i = 0; i < single_view_param.result_image_height; i++)
	{
		for (int j = 0; j < single_view_param.result_image_width; j++)
		{
			/*if (cam_flag == CAM_FRONT)
			{*/
				cam_ray[1] = (Float64_t)(j - single_view_param.result_image_width / 2.00)* pixel_width_dis;//转化为相机坐标系
				cam_ray[0] = (Float64_t)(i - single_view_param.result_image_height/ 2.00)* pixel_hight_dis;//朝下是x
			//}
			//if (cam_flag == CAM_REAR)
			//{
			//	cam_ray[1] = (Float64_t)((Float64_t(j) - ((Float64_t)(single_view_param.result_image_width) / 2.00))* pixel_width_dis);//转化为相机坐标系
			//	cam_ray[0] = (Float64_t)((Float64_t(i) - ((Float64_t)(single_view_param.result_image_height) / 2.00))* pixel_hight_dis);
			//	//cam_ray[1] = (Float64_t)((Float64_t(single_view_param.result_image_width - j - 1) - ((Float64_t)(single_view_param.result_image_width) / 2.00))* pixel_width_dis);
			//	//cam_ray[0] = (Float64_t)((Float64_t(single_view_param.result_image_height - i - 1) - ((Float64_t)(single_view_param.result_image_height) / 2.00))* pixel_hight_dis);
			//}
			Float64_t image_point[2];
			Cam_Ray_To_Image_Point(image_point, cam_ray, &(p_avm_param->p_avm_config_param->camera_model[0].camera_model_int));
			UInt32_t  shift_altogether = i*single_view_param.result_image_width * 2 + j * 2;
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
	if (cam_flag == CAM_FRONT)
	{
		single_view_param = p_avm_param->p_avm_config_param->avm_single_view_param.front_single_view_param;
		p_lut = p_avm_param->p_avm_lut->p_lut_single_view_front;
	}
	if (cam_flag == CAM_REAR)
	{
		single_view_param = p_avm_param->p_avm_config_param->avm_single_view_param.rear_single_view_param;
		p_lut = p_avm_param->p_avm_lut->p_lut_single_view_rear;
	}
#if DEBUG_INFO
	CvSize size0 = cvSize(single_view_param.result_image_width, single_view_param.result_image_height);//
	IplImage *image_result_single_front_lut = cvCreateImage(size0, IPL_DEPTH_8U, 1);
	memset(image_result_single_front_lut->imageData, 0, single_view_param.result_image_width*single_view_param.result_image_height);
#endif
	Float64_t cam_ray[3];
	Float64_t tan_h = tan((single_view_param.box_expand_param.fov_h_center / 2 / 180)*PI);
	Float64_t tan_v = tan((single_view_param.box_expand_param.fov_v / 2 / 180)*PI);
	cam_ray[2] = -8000.00;
	Float64_t   con_num = cam_ray[2];
	Float64_t  pixel_center_width_dis = -(cam_ray[2] * tan_h) / (single_view_param.box_expand_param.center_width / 2);//以中间视角为依据求现实的每个像素代表现实的宽
	Float64_t  pixel_center_height_dis = -(cam_ray[2] * tan_v) / (single_view_param.result_image_height / 2);//以中间视角为依据求现实的每个像素代表现实的宽
	Float64_t  camera_coor0 = -(single_view_param.box_expand_param.center_width / 2)*pixel_center_width_dis;
	Float64_t  camera_coor1 = -camera_coor0;

	double angle = single_view_param.box_expand_param.fov_h_side;

	for (int i = 0; i < single_view_param.result_image_height; i++)
	{
		for (int j = 0; j < single_view_param.result_image_width; j++)
		{
			if (j>((single_view_param.result_image_width / 2) - (single_view_param.box_expand_param.center_width / 2))//中间的视角
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
			if (j<=((single_view_param.result_image_width / 2) - (single_view_param.box_expand_param.center_width / 2)))//两侧展开的视角
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
			if (j >= ((single_view_param.result_image_width / 2) + (single_view_param.box_expand_param.center_width / 2)))//两侧展开的视角
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

		}
	}
#if DEBUG_INFO
	char result_name[MAX_ARRAY_SIZE];
	sprintf(result_name, "%s/box_bear.bmp", result_image_path);
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
									/ Float64_t(single_view_param.result_image_width);//表示矩形的宽影射到
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
							 - (image_point[1] - single_view_param.crop_param.crop_rect.y)*sin((single_view_param.crop_param.roll_angle / 180)*PI) //为坐标旋转公式绕左上点旋转用户定义的角度
							 + single_view_param.crop_param.crop_rect.x;//
			image_point_0[1] = (image_point[0] - single_view_param.crop_param.crop_rect.x)*sin((single_view_param.crop_param.roll_angle / 180)*PI)
							 + (image_point[1] - single_view_param.crop_param.crop_rect.y)*cos((single_view_param.crop_param.roll_angle / 180)*PI) 
							 + single_view_param.crop_param.crop_rect.y;//
			UInt32_t  shift_altogether = i*single_view_param.result_image_width * 2 + j * 2;
			single_view_zip_data(shift_altogether, image_point, p_lut, cam_flag);//将数据压入lut表
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
	Float64_t world_coordinate[3] = { 0, 0, 0 };
	/********************extend all over result image pixel****************************/
	for (int i = 0; i < single_view_param.result_image_height; i++)
	{
		for (int j = 0; j < single_view_param.result_image_width; j++)
		{
			/***********calculate world coordinate*********/
			Float64_t image_point[2];
			calculate_world_coordinate_left_right(world_coordinate, i, j, p_avm_param, single_view_param,cam_flag);//由图像坐标转化为世界坐标
			UInt32_t  shift_altogether = i*single_view_param.result_image_width * 2 + j * 2;
			single_view_zip_data(shift_altogether, image_point, p_lut, cam_flag);//填充lut表
			world_coordinate_to_pic_coordinate(cam_flag, world_coordinate, image_point, p_avm_param);
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
    #define R 1000.000//定义的R是柱面圆心坐标
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
					->p_avm_config_param->vehicle_param.vehicle_width / 2;//平面部分的边界点x和z的坐标
		x_z[1] = R;//旋转的圆心
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
				+ p_avm_param->p_avm_config_param->vehicle_param.vehicle_width / 2;//视界平面部分的边界点x和z的旋转坐标圆心
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
	/********************extend all over result image pixel****************************/
	for (int i = 0; i < left_right_view_param.result_image_height; i++)
	{
		for (int j = 0; j < left_right_view_param.result_image_width; j++)
		{
			/***********calculate world coordinate*********/
			Float64_t image_point[2];
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
							- p_avm_param->p_avm_config_param->vehicle_param.vehicle_width / 2;
		world_coordinate[1] = left_right_view_param.combine_expand_param_left.world_height / 2
							- Float64_t(pixel_row_dis)*Float64_t(i);

	}
	if (cam_flag == CAM_RIGHT)
	{
		world_coordinate[0] = Float64_t(pixel_col_dis)*Float64_t(j) 
							+ p_avm_param->p_avm_config_param->vehicle_param.vehicle_width / 2;
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
	x_z[0] = left_right_view_param.combine_expand_param_right.plane_width + p_avm_param->p_avm_config_param->vehicle_param.vehicle_width / 2;//视界平面部分的边界点x和z的旋转坐标圆心
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
				x_z[0] = left_right_view_param.combine_expand_param_right.plane_width + p_avm_param->p_avm_config_param->vehicle_param.vehicle_width / 2;//视界平面部分的边界点x和z的旋转坐标圆心
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
				x_z[0] = -left_right_view_param.combine_expand_param_left.plane_width - p_avm_param->p_avm_config_param->vehicle_param.vehicle_width / 2;//视界平面部分的边界点x和z的坐标
				x_z[1] = R;//旋转的圆心

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
	printf("laotianyenabaoyouwolaotianyebaoyouwolaotianyebaoyouwomiaozhuangnanalingxingxuxianrenbaoyouwowodebabwodebababaoyouwowodebababaoyouwowodebababaoyouwomiaozhuangnanlingxingxingrenbaoyouwou");
	char result_name[MAX_ARRAY_SIZE];
	sprintf(result_name, "%s/6666666666666666666666666666666666666666888888888888888888888888888888888combine_overlook_lr.bmp", result_image_path);
	cvSaveImage(result_name, image_result_single_front_lut);
#endif
	return ret;
}