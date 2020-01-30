#include"draw_static_dynamic_line.h"
#include"resolve_data.h"


//红 黄 绿  青 蓝 紫 
/*
* Function Name:				calculate_pic_coordinate_from_world_coordinate_0
* Function Function:			calculate_pic_coordinate_from_world_coordinate_0
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
void calculate_pic_coordinate_from_world_coordinate_0(P_Avm_Param_S p_avm_param, Front_Rear_Single_View_Param_S single_view_param,
	Float64_t *world_coordinate, int cam_flag,
	Float64_t pixel_hight_dis, Float64_t pixel_width_dis, Float64_t *cam_ray,  int *pic_point_coordinate)
{

	

	Float64_t camera_coordinate[3];
	Float64_t camera_coordinate_0[3];

	Mat_Mul_Vec_3by3_DB(camera_coordinate, p_avm_param->p_avm_config_param->camera_model[cam_flag].camera_model_ext.inv_R, world_coordinate);
	for (int i = 3; i < 6; i++)
	{
		camera_coordinate[i - 3] = p_avm_param->p_avm_config_param->camera_model[cam_flag].camera_model_ext.inv_pose[i] + camera_coordinate[i - 3];
	}
	Float64_t tmp = 0;
	tmp = camera_coordinate[0];
	camera_coordinate[0] = camera_coordinate[1];
	camera_coordinate[1] = tmp;
	camera_coordinate[2] = -camera_coordinate[2];

	Float64_t temp_rate = cam_ray[2] / camera_coordinate[2];
	camera_coordinate_0[0] = camera_coordinate[0] * temp_rate;
	camera_coordinate_0[1] = camera_coordinate[1] * temp_rate;
	camera_coordinate_0[2] = cam_ray[2];
	if (cam_flag == CAM_FRONT)
	{
		int  pic_hight = ((camera_coordinate_0[0] / Float64_t(pixel_hight_dis)) + (single_view_param.result_image_height / 2));
		pic_point_coordinate[0] = pic_hight;
		int  pic_width = (camera_coordinate_0[1] / Float64_t(pixel_width_dis)) + (single_view_param.result_image_width / 2);
		pic_point_coordinate[1] = pic_width;
	}

	if (cam_flag == CAM_REAR)
	{
		int  pic_hight = ((camera_coordinate_0[0] / Float64_t(pixel_hight_dis)) + (single_view_param.result_image_height / 2));
		pic_point_coordinate[0] = pic_hight;
		int  pic_width = (camera_coordinate_0[1] / Float64_t(pixel_width_dis)) + (single_view_param.result_image_width / 2);
		pic_point_coordinate[1] = pic_width;
		
	}


}



/*
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
void zip_data_draw_line(IN int shift_altogether, IN UInt32_t*p_lut, IN UInt32_t camera_flag,int i,int j,int color_flag)
{
	//p_lut[0] = shift_altogether;//
	UInt32_t temp_shift = (i << 21) + (j << 10) + (color_flag << 2) + (camera_flag);
	p_lut[shift_altogether] = temp_shift;
}









/*
* Function Name:				calculate_key_point_coordinate
* Function Function:			calculate_key_point_coordinate
* Input:
/*************************
*       shift;               p_avm_param
*		image;                cam_flag
p_avm_param;
* Note:
*
* Revision:
*     Create by Cheng.Li on 2018/1/17.
*/
void calculate_key_point_coordinate(IN Int32_t cam_flag, Front_Rear_Single_View_Param_S single_view_param,
					IN P_Avm_Param_S p_avm_param, OUT Int32_t key_ponit_pic_coordinate[16][2])
{
	Float64_t cam_ray[3] = { 0, 0, -8000 };//虚拟焦距是8000

	Float64_t tan_h = (tan((single_view_param.plane_expand_param.fov_h / 2 / 180)*PI));
	Float64_t tan_v = (tan((single_view_param.plane_expand_param.fov_v / 2 / 180)*PI));
	Float64_t  pixel_hight_dis = -(cam_ray[2] * tan_v) / (single_view_param.result_image_height / 2);//
	Float64_t  pixel_width_dis = -(cam_ray[2] * tan_h) / (single_view_param.result_image_width / 2);
	Float64_t image_point[2];


	int key_ponit_pic_coordinate_temp[2];
	Float64_t key_point_world_coordinate[3] = { 0, 0, 0 };//2000mm
	Float64_t car_length_half;
	Int32_t	static_line_dis_to_car[4];
	if (cam_flag == CAM_FRONT)
	{
		car_length_half = p_avm_param->p_avm_config_param->vehicle_param.vehicle_length / 2;
		static_line_dis_to_car[0] = single_view_param.dis_line_param.static_line_param.static_line_dis_to_car[0];
		static_line_dis_to_car[1] = single_view_param.dis_line_param.static_line_param.static_line_dis_to_car[1];
		static_line_dis_to_car[2] = single_view_param.dis_line_param.static_line_param.static_line_dis_to_car[2];
		static_line_dis_to_car[3] = single_view_param.dis_line_param.static_line_param.static_line_dis_to_car[3];
		key_point_world_coordinate[0] = -p_avm_param->p_avm_config_param->vehicle_param.vehicle_width / 2.00;
	}
	if (cam_flag == CAM_REAR)
	{
		car_length_half = -p_avm_param->p_avm_config_param->vehicle_param.vehicle_length / 2;
		static_line_dis_to_car[0] = -single_view_param.dis_line_param.static_line_param.static_line_dis_to_car[0];
		static_line_dis_to_car[1] = -single_view_param.dis_line_param.static_line_param.static_line_dis_to_car[1];
		static_line_dis_to_car[2] = -single_view_param.dis_line_param.static_line_param.static_line_dis_to_car[2];
		static_line_dis_to_car[3] = -single_view_param.dis_line_param.static_line_param.static_line_dis_to_car[3];
		key_point_world_coordinate[0] = -p_avm_param->p_avm_config_param->vehicle_param.vehicle_width / 2.00;
	}
	/************************calculate left key coordinate************************/
	for (int i = 0; i < 4; i++)
	{
		key_point_world_coordinate[1] = car_length_half + static_line_dis_to_car[i];
		/*calculate_pic_coordinate_from_world_coordinate_0(p_avm_param, single_view_param,
			key_point_world_coordinate, cam_flag,
			pixel_hight_dis, pixel_width_dis, cam_ray, key_ponit_pic_coordinate_temp);*/
		plane_front_rear_single_view_calculate_key_point_coordinate_one_point(p_avm_param, cam_flag, key_point_world_coordinate, key_ponit_pic_coordinate_temp);

		key_ponit_pic_coordinate[i][0] = key_ponit_pic_coordinate_temp[0];
		key_ponit_pic_coordinate[i][1] = key_ponit_pic_coordinate_temp[1];
	}
	key_point_world_coordinate[0] = key_point_world_coordinate[0] + LINE_ACROSS_EXTEND;
	for (int i = 4; i < 8; i++)
	{
		key_point_world_coordinate[1] = car_length_half + static_line_dis_to_car[i - 4];
		/*calculate_pic_coordinate_from_world_coordinate_0(p_avm_param, single_view_param
			, key_point_world_coordinate, cam_flag,
			pixel_hight_dis, pixel_width_dis, cam_ray, key_ponit_pic_coordinate_temp);*/
		plane_front_rear_single_view_calculate_key_point_coordinate_one_point(p_avm_param, cam_flag, key_point_world_coordinate, key_ponit_pic_coordinate_temp);
		key_ponit_pic_coordinate[i][0] = key_ponit_pic_coordinate_temp[0];
		key_ponit_pic_coordinate[i][1] = key_ponit_pic_coordinate_temp[1];

	}
#if 1
	/****************calculate right key coordinate*************************/
	key_point_world_coordinate[0] = p_avm_param->p_avm_config_param->vehicle_param.vehicle_width / 2.00;
	for (int i = 8; i < 12; i++)
	{
		key_point_world_coordinate[1] = car_length_half + static_line_dis_to_car[i - 8];
		/*calculate_pic_coordinate_from_world_coordinate_0(p_avm_param, single_view_param
			, key_point_world_coordinate, cam_flag,
			pixel_hight_dis, pixel_width_dis, cam_ray, key_ponit_pic_coordinate_temp);*/
		plane_front_rear_single_view_calculate_key_point_coordinate_one_point(p_avm_param, cam_flag, key_point_world_coordinate, key_ponit_pic_coordinate_temp);
		key_ponit_pic_coordinate[i][0] = key_ponit_pic_coordinate_temp[0];
		key_ponit_pic_coordinate[i][1] = key_ponit_pic_coordinate_temp[1];

	}
	key_point_world_coordinate[0] = key_point_world_coordinate[0] - 600;
	for (int i = 12; i < 16; i++)
	{

		key_point_world_coordinate[1] = car_length_half + static_line_dis_to_car[i - 12];
		/*calculate_pic_coordinate_from_world_coordinate_0(p_avm_param, single_view_param
			, key_point_world_coordinate, cam_flag,
			pixel_hight_dis, pixel_width_dis, cam_ray, key_ponit_pic_coordinate_temp);*/
		plane_front_rear_single_view_calculate_key_point_coordinate_one_point(p_avm_param, cam_flag, key_point_world_coordinate, key_ponit_pic_coordinate_temp);
		key_ponit_pic_coordinate[i][0] = key_ponit_pic_coordinate_temp[0];
		key_ponit_pic_coordinate[i][1] = key_ponit_pic_coordinate_temp[1];
	}
#endif

}



/*
* Function Name:				front_rear_draw_across_line
* Function Function:			front_rear_draw_across_line
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
void front_rear_draw_across_line(IN Int32_t key_ponit_pic_coordinate[16][2], OUT Int32_t *p_point_num, OUT UInt32_t *p_lut_line,IN Int32_t cam_flag)
{

	/**********画出左边***********************/

	for (int n = 1; n < 4; n++)
	{
		for (int i = key_ponit_pic_coordinate[n][0]; i < key_ponit_pic_coordinate[n][0] + LINE_WIDTH; i++)// key_ponit_pic_coordinate[n][0]存储第几行信息依据这个第几行朝下增加线的宽度
		{
			for (int j = key_ponit_pic_coordinate[n][1]; j < key_ponit_pic_coordinate[n + 4][1]; j++)//key_ponit_pic_coordinate[n][1]与 key_ponit_pic_coordinate[n + 4][1]存储的是左右两端的列坐标
			{
				int color_flag = 0;
				(*p_point_num)++;
				zip_data_draw_line((*p_point_num), p_lut_line, cam_flag, i, j, color_flag);
#if DEBUG_INFO
				image_result_single_front_lut->imageData[i *single_view_param.result_image_width + j] = 255;
#endif
			}
		}
	}
	/******画出右边横线*******/
	for (int n = 9; n < 12; n++)
	{
		for (int i = key_ponit_pic_coordinate[n][0]; i < key_ponit_pic_coordinate[n][0] + LINE_WIDTH; i++)
		{
			for (int j = key_ponit_pic_coordinate[n][1]; j > key_ponit_pic_coordinate[n + 4][1]; j--)
			{
				int color_flag = 0;
				(*p_point_num)++;
				zip_data_draw_line((*p_point_num), p_lut_line, cam_flag, i, j, color_flag);
#if DEBUG_INFO
				image_result_single_front_lut->imageData[i *single_view_param.result_image_width + j] = 255;
#endif
			}
		}
	}



}



/*
* Function Name:				front_rear_draw_vertical_line
* Function Function:			front_rear_draw_across_line
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

void front_rear_draw_vertical_line_REAR(IN Int32_t key_ponit_pic_coordinate[16][2], OUT Int32_t *p_point_num, OUT UInt32_t *p_lut_line, IN Int32_t cam_flag)
{
	/*****draw vertical line********/
	Float64_t a = abs(Float64_t(key_ponit_pic_coordinate[0][1] - key_ponit_pic_coordinate[1][1])//第几列
		/ Float64_t(key_ponit_pic_coordinate[0][0] - key_ponit_pic_coordinate[1][0]));//求出当线是斜线时像素每下降一行左 右 移几个像素才能确保无缝连接  
	Float64_t b = abs(Float64_t(key_ponit_pic_coordinate[8][1] - key_ponit_pic_coordinate[9][1])
		/ Float64_t(key_ponit_pic_coordinate[8][0] - key_ponit_pic_coordinate[9][0]));
	Float64_t temp_0 = key_ponit_pic_coordinate[0][1];
	int color_flag = 0;
	for (int n = 0; n < 3; n++)
	{
		a = abs(Float64_t(key_ponit_pic_coordinate[n][1] - key_ponit_pic_coordinate[n + 1][1])//第几列
			/ Float64_t(key_ponit_pic_coordinate[n][0] - key_ponit_pic_coordinate[n + 1][0]));//求出当线是斜线时像素每下降一行左 右 移几个像素才能确保无缝连接  
		for (int i = key_ponit_pic_coordinate[n][0]; i <key_ponit_pic_coordinate[n + 1][0]; i++)
		{
			temp_0 = temp_0 + a;//每下降一行对应的列左右移动几列
			for (int j = temp_0; j < temp_0 + LINE_WIDTH; j++)//在列的方向上左右增加线的宽度
			{
				(*p_point_num)++;
				zip_data_draw_line(*p_point_num, p_lut_line, cam_flag, i, j, color_flag);
#if DEBUG_INFO_1
				image_result_single_front_lut->imageData[i *single_view_param.result_image_width + j] = 255;
#endif
			}
		}
	}

	temp_0 = key_ponit_pic_coordinate[8][1];
	for (int n = 8; n < 12; n++)
	{
		b = abs(Float64_t(key_ponit_pic_coordinate[n][1] - key_ponit_pic_coordinate[n + 1][1])
			/ Float64_t(key_ponit_pic_coordinate[n][0] - key_ponit_pic_coordinate[n + 1][0]));
		for (int i = key_ponit_pic_coordinate[n][0]; i <key_ponit_pic_coordinate[n + 1][0]; i++)
		{
			temp_0 = temp_0 - b;
			for (int j = temp_0; j < temp_0 + LINE_WIDTH; j++)
			{
				(*p_point_num)++;
				zip_data_draw_line(*p_point_num, p_lut_line, cam_flag, i, j, color_flag);
#if DEBUG_INFO_1
				image_result_single_front_lut->imageData[i *single_view_param.result_image_width + j] = 255;
#endif
			}
		}
	}
#if DEBUG_INFO_1
	//char result_name[MAX_ARRAY_SIZE];
	//sprintf(result_name, "%s/line000.bmp", result_image_path);
	//cvSaveImage(result_name, image_result_single_front_lut);
#endif
}





















/*
* Function Name:				front_rear_draw_vertical_line
* Function Function:			front_rear_draw_across_line
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
void front_rear_draw_vertical_line_rear(IN Int32_t key_ponit_pic_coordinate[16][2], OUT Int32_t *p_point_num, OUT UInt32_t *p_lut_line, IN Int32_t cam_flag)
{
	/*****draw vertical line********/
	Float64_t a = abs(Float64_t(key_ponit_pic_coordinate[0][1] - key_ponit_pic_coordinate[1][1])//第几列
		/ Float64_t(key_ponit_pic_coordinate[0][0] - key_ponit_pic_coordinate[1][0]));//求出当线是斜线时像素每下降一行左 右 移几个像素才能确保无缝连接  
	Float64_t b = abs(Float64_t(key_ponit_pic_coordinate[8][1] - key_ponit_pic_coordinate[9][1])
		/ Float64_t(key_ponit_pic_coordinate[8][0] - key_ponit_pic_coordinate[9][0]));
	Float64_t temp_0 = key_ponit_pic_coordinate[0][1];
	int color_flag = 0;
	for (int n = 0; n < 3; n++)
	{
		a = abs(Float64_t(key_ponit_pic_coordinate[n][1] - key_ponit_pic_coordinate[n + 1][1])//第几列
			/ Float64_t(key_ponit_pic_coordinate[n][0] - key_ponit_pic_coordinate[n + 1][0]));//求出当线是斜线时像素每下降一行左 右 移几个像素才能确保无缝连接  
		for (int i = key_ponit_pic_coordinate[n][0]; i < key_ponit_pic_coordinate[n + 1][0]; i++)
		{
			temp_0 = temp_0 + a;//每下降一行对应的列左右移动几列
			for (int j = temp_0; j < temp_0 + LINE_WIDTH; j++)//在列的方向上左右增加线的宽度
			{
				(*p_point_num)++;
				zip_data_draw_line(*p_point_num, p_lut_line, cam_flag, i, j, color_flag);
#if DEBUG_INFO
				image_result_single_front_lut->imageData[i *single_view_param.result_image_width + j] = 255;
#endif
			}
		}
	}

	temp_0 = key_ponit_pic_coordinate[8][1];
	for (int n = 8; n < 12; n++)
	{
		b = abs(Float64_t(key_ponit_pic_coordinate[n][1] - key_ponit_pic_coordinate[n + 1][1])
			/ Float64_t(key_ponit_pic_coordinate[n][0] - key_ponit_pic_coordinate[n + 1][0]));
		for (int i = key_ponit_pic_coordinate[n][0]; i <key_ponit_pic_coordinate[n + 1][0]; i++)
		{
			temp_0 = temp_0 - b;
			for (int j = temp_0; j < temp_0 + LINE_WIDTH; j++)
			{
				(*p_point_num)++;
				zip_data_draw_line(*p_point_num, p_lut_line, cam_flag, i, j, color_flag);
#if DEBUG_INFO
				image_result_single_front_lut->imageData[i *single_view_param.result_image_width + j] = 255;
#endif
			}
		}
	}
#if DEBUG_INFO
	char result_name[MAX_ARRAY_SIZE];
	sprintf(result_name, "%s/line000.bmp", result_image_path);
	cvSaveImage(result_name, image_result_single_front_lut);
#endif
}






/*
* Function Name:				front_rear_draw_vertical_line
* Function Function:			front_rear_draw_across_line
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
void front_rear_draw_vertical_line(IN Int32_t key_ponit_pic_coordinate[16][2], OUT Int32_t *p_point_num, OUT UInt32_t *p_lut_line, IN Int32_t cam_flag)
{
	/*****draw vertical line********/
	Float64_t a = abs(Float64_t(key_ponit_pic_coordinate[0][1] - key_ponit_pic_coordinate[1][1])//第几列
				/ Float64_t(key_ponit_pic_coordinate[0][0] - key_ponit_pic_coordinate[1][0]));//求出当线是斜线时像素每下降一行左 右 移几个像素才能确保无缝连接  
	Float64_t b = abs(Float64_t(key_ponit_pic_coordinate[8][1] - key_ponit_pic_coordinate[9][1])
				/ Float64_t(key_ponit_pic_coordinate[8][0] - key_ponit_pic_coordinate[9][0]));
	Float64_t temp_0 = key_ponit_pic_coordinate[0][1];
	int color_flag=0;
	for (int n = 0; n < 3; n++)
	{
		a = abs(Float64_t(key_ponit_pic_coordinate[n][1] - key_ponit_pic_coordinate[n+1][1])//第几列
			/ Float64_t(key_ponit_pic_coordinate[n][0] - key_ponit_pic_coordinate[n+1][0]));//求出当线是斜线时像素每下降一行左 右 移几个像素才能确保无缝连接  
		for (int i = key_ponit_pic_coordinate[n][0]; i >key_ponit_pic_coordinate[n + 1][0]; i--)
		{
			temp_0 = temp_0 + a;//每下降一行对应的列左右移动几列
			for (int j = temp_0; j < temp_0 + LINE_WIDTH; j++)//在列的方向上左右增加线的宽度
			{
				(*p_point_num)++;
				zip_data_draw_line(*p_point_num, p_lut_line, cam_flag, i, j, color_flag);
#if DEBUG_INFO
				image_result_single_front_lut->imageData[i *single_view_param.result_image_width + j] = 255;
#endif
			}
		}
	}

	temp_0 = key_ponit_pic_coordinate[8][1];
	for (int n = 8; n < 12; n++)
	{
		b = abs(Float64_t(key_ponit_pic_coordinate[n][1] - key_ponit_pic_coordinate[n+1][1])
			/ Float64_t(key_ponit_pic_coordinate[n][0] - key_ponit_pic_coordinate[n+1][0]));
		for (int i = key_ponit_pic_coordinate[n][0]; i >key_ponit_pic_coordinate[n + 1][0]; i--)
		{
			temp_0 = temp_0 - b;
			for (int j = temp_0; j < temp_0 + LINE_WIDTH; j++)
			{
				(*p_point_num)++;
				zip_data_draw_line(*p_point_num, p_lut_line, cam_flag, i, j, color_flag);
#if DEBUG_INFO
				image_result_single_front_lut->imageData[i *single_view_param.result_image_width + j] = 255;
#endif
			}
		}
	}
#if DEBUG_INFO
	char result_name[MAX_ARRAY_SIZE];
	sprintf(result_name, "%s/line000.bmp", result_image_path);
	cvSaveImage(result_name, image_result_single_front_lut);
#endif
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
//#define DEBUG_INFO_0 1
AVM_Error_Code_E draw_static_plane_front_rear_single_view_line(IN P_Avm_Param_S p_avm_param,IN int cam_flag)
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
	Int32_t key_ponit_pic_coordinate[16][2] = {0};//用来存储16个关键点的坐标


	/********static line************/
	/*********calculate key point coordinate*****计算出16个关键点*********************************/
	calculate_key_point_coordinate(cam_flag, single_view_param, p_avm_param, key_ponit_pic_coordinate);
	

	
#if DEBUG_INFO_0
	IplImage *image_result_single_front_lut = cvCreateImage(size0, IPL_DEPTH_8U, 1);
	memset(image_result_single_front_lut->imageData, 0, single_view_param.result_image_width*single_view_param.result_image_height);


	IplImage* image[4] = { NULL, NULL, NULL, NULL };
	ret = my_load_image(image);
	//CHECK_ERROR(ret);

	Float64_t cam_ray[3] = { 0, 0, -8000 };

	Float64_t tan_h = (tan((single_view_param.plane_expand_param.fov_h / 2 / 180)*PI));
	Float64_t tan_v = (tan((single_view_param.plane_expand_param.fov_v / 2 / 180)*PI));
	Float64_t  pixel_hight_dis = -(cam_ray[2] * tan_v) / (single_view_param.result_image_height / 2);//
	Float64_t  pixel_width_dis = -(cam_ray[2] * tan_h) / (single_view_param.result_image_width / 2);
	Float64_t image_point[2];
	for (int i = 0; i < single_view_param.result_image_height; i++)
	{
		for (int j = 0; j < single_view_param.result_image_width; j++)
		{
			if (cam_flag == 0)
			{
				cam_ray[1] = (Float64_t)((Float64_t(j) - ((Float64_t)(single_view_param.result_image_width) / 2.00))* pixel_width_dis);
				cam_ray[0] = (Float64_t)((Float64_t(i) - ((Float64_t)(single_view_param.result_image_height) / 2.00))* pixel_hight_dis);
			}
			if (cam_flag == 1)
			{
				cam_ray[1] = (Float64_t)((Float64_t(single_view_param.result_image_width - j - 1) 
							- ((Float64_t)(single_view_param.result_image_width) / 2.00))* pixel_width_dis);
				cam_ray[0] = (Float64_t)((Float64_t(single_view_param.result_image_height - i - 1)
							- ((Float64_t)(single_view_param.result_image_height) / 2.00))* pixel_hight_dis);
			}
			Cam_Ray_To_Image_Point(image_point, cam_ray, &(p_avm_param->p_avm_config_param->camera_model[0].camera_model_int));
#if DEBUG_INFO_0
			int rgb[3];
			bilinear_interpolation(image[cam_flag], rgb, image_point[1], image_point[0]);
			image_result_single_front_lut->imageData[i *single_view_param.result_image_width + j] = (uchar)(rgb[0]);
#endif
		}
	}
#endif
	
	/***********依据 关键点 将关键点用一定宽度的线将关键点画出来形成车道**************/
	int point_num = 1;//记录下一共在图像像上一共画了几个点，每画一个点记录一下以便为地址偏移做准备，lut	表的第一个位置存放一共有多少个点 
	int color_flag = COLOR_RED;//颜色标签
	/**draw across line***/
	/**连接两个相邻的 关键点横线的部分**/
	front_rear_draw_across_line(key_ponit_pic_coordinate, &point_num,p_lut_line, cam_flag);
	/*****draw vertical line 连接关键点竖线 的部分********/
	if (cam_flag == CAM_REAR)
	{
		front_rear_draw_vertical_line_REAR(key_ponit_pic_coordinate, &point_num, p_lut_line, cam_flag);
	}
	if (cam_flag == CAM_FRONT)
	{
		front_rear_draw_vertical_line(key_ponit_pic_coordinate, &point_num, p_lut_line, cam_flag);
	}
	p_lut_line[0]=point_num;



	

	return ret;
}






















/*
* Function Name:			calculate_pic_coordinate_from_world_coordinate
* Function Function:		calculate_pic_coordinate_from_world_coordinate
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


void calculate_pic_coordinate_from_world_coordinate(P_Avm_Param_S p_avm_param,
					Float64_t *world_coordinate, int cam_flag,
					Float64_t pixel_hight_dis, Float64_t pixel_width_dis, Float64_t *cam_ray,
					int *pic_point_coordinate, Int32_t result_image_height, Int32_t result_image_width)
{
#if 1
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


	Float64_t focal_length = 1000; // unit is mm
	Float64_t fov_h = (single_view_param.plane_expand_param.fov_h*PI)/180;
	Float64_t fov_v = (single_view_param.plane_expand_param.fov_v*PI) / 180;
	Int32_t result_width = single_view_param.result_image_width;
	Int32_t result_height = single_view_param.result_image_height;

	// 虚拟展开图中每个像素在高宽方向所代表的实际距离
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
	Float64_t pitch_angle = (30*PI) / 180;//此处配置相机上下俯仰角

	Float64_t init_world_coord_x = pose_t[0] - is_front_cam * mm_per_pixel_h * result_width * 0.5;
	Float64_t init_world_coord_y = pose_t[1] + is_front_cam * (focal_length * cos(pitch_angle) + mm_per_pixel_v * result_height * 0.5 * sin(pitch_angle));
	Float64_t init_world_coord_z = pose_t[2] - focal_length * sin(pitch_angle) + mm_per_pixel_v * result_height * 0.5 * cos(pitch_angle);
	Float64_t delta_x = mm_per_pixel_h;
	Float64_t delta_y = mm_per_pixel_v * sin(pitch_angle);
	Float64_t delta_z = mm_per_pixel_v * cos(pitch_angle);
	//Float64_t world_coordinate[3];
	Float64_t image_point[2];

	//world_coordinate[0] = init_world_coord_x + is_front_cam * j * delta_x;
	//world_coordinate[1] = init_world_coord_y - is_front_cam * i * delta_y;


	pic_point_coordinate[0] = (world_coordinate[0] - init_world_coord_x)*is_front_cam / delta_x;
	pic_point_coordinate[1] = (world_coordinate[1] - init_world_coord_y)*is_front_cam / delta_y;
#endif
#if 0
	Float64_t camera_coordinate[3];
	Float64_t camera_coordinate_0[3];

	Mat_Mul_Vec_3by3_DB(camera_coordinate, 
					p_avm_param->p_avm_config_param->camera_model[cam_flag].camera_model_ext.inv_R, world_coordinate);
	for (int i = 3; i < 6; i++)
	{
		camera_coordinate[i - 3] = p_avm_param->p_avm_config_param->camera_model[cam_flag].camera_model_ext.inv_pose[i]
							+ camera_coordinate[i - 3];
	}
	Float64_t tmp = 0;
	tmp = camera_coordinate[0];
	camera_coordinate[0] = camera_coordinate[1];
	camera_coordinate[1] = tmp;
	camera_coordinate[2] = -camera_coordinate[2];



	//将世界坐标系中的距离线映射到相机坐标系了
	Float64_t temp_rate = cam_ray[2] / camera_coordinate[2];//求出Z坐标的比值
	camera_coordinate_0[0] = camera_coordinate[0] * temp_rate;
	camera_coordinate_0[1] = camera_coordinate[1] * temp_rate;
	camera_coordinate_0[2] = cam_ray[2];
	int  pic_hight = (camera_coordinate_0[0] / Float64_t(pixel_hight_dis)) + (result_image_height / 2);
	pic_point_coordinate[0] = pic_hight;
	int  pic_width = (camera_coordinate_0[1] / Float64_t(pixel_width_dis)) + (result_image_width / 2);
	pic_point_coordinate[1] = pic_width;
#endif

}



/*************************/
void draw_circle_trail(Int32_t rotate_flag,Float64_t centre_rotate_coordinate[3], 
						Float64_t car_head[3], 
						Float64_t angle_average,
						P_Avm_Param_S p_avm_param,
						Float64_t  pixel_hight_dis,
						Float64_t  pixel_width_dis,
						Int32_t dynamic_line_length,
						Float64_t *cam_ray,
						int cam_flag,
						UInt32_t * p_lut_line,
						UInt32_t *p_rotate_altogether_add,
						Int32_t result_image_height,
						Int32_t result_image_width)
{

	Float64_t angle_base = 0;
	int pic_coor_temp[2];
	for (int i = 0; i < EQUALITY_ANGLE; i++)
	{
		Float64_t world_circle[3] = { 0, 0, 0 };
		angle_base = angle_base + angle_average;

		world_circle[0] = (car_head[0] - centre_rotate_coordinate[0])*cos(angle_base) //坐标旋转公式
			- (car_head[1] - centre_rotate_coordinate[1])*sin(angle_base)
			+ centre_rotate_coordinate[0];///is z
		world_circle[1] = (car_head[0] - centre_rotate_coordinate[0])*sin(angle_base)
			+ (car_head[1] - centre_rotate_coordinate[1])*cos(angle_base)
			+ centre_rotate_coordinate[1];//is y
		int key_ponit_pic_coordinate_temp[2];
		if (rotate_flag == ROTATE_PLANE_EXPEND_FRONT || rotate_flag == ROTATE_PLANE_EXPEND_REAR)//平面展开的动态线 
		{

			/*calculate_pic_coordinate_from_world_coordinate(p_avm_param,
				 world_circle,
				cam_flag,
				pixel_hight_dis, pixel_width_dis, cam_ray, key_ponit_pic_coordinate_temp,
				result_image_height,result_image_width);*/
			plane_front_rear_single_view_calculate_key_point_coordinate_one_point(p_avm_param, cam_flag, world_circle, key_ponit_pic_coordinate_temp);
		}
		if (rotate_flag == ROTATE_4_VIEW_BIG_REAR || rotate_flag == ROTATE_4_VIEW_BIG_FRONT)//4路环视图大图的动态线
		{
			key_ponit_pic_coordinate_temp[0] = p_avm_param->p_avm_config_param->avm_2d_param.junc_view_4_param.result_big_param.car_logo_rect.y 
				+ p_avm_param->p_avm_config_param->avm_2d_param.junc_view_4_param.result_big_param.car_logo_rect.height / 2
				- (world_circle[1] / pixel_hight_dis);
			key_ponit_pic_coordinate_temp[1] = p_avm_param->p_avm_config_param->avm_2d_param.junc_view_4_param.result_big_param.car_logo_rect.x 
				+ p_avm_param->p_avm_config_param->avm_2d_param.junc_view_4_param.result_big_param.car_logo_rect.width / 2
					+ (world_circle[0] / pixel_width_dis);
				
		}

		if (rotate_flag == ROTATE_4_VIEW_SMALL_FRONT || rotate_flag == ROTATE_4_VIEW_SMALL_REAR)//4路环视图小图的动态线
		{
			key_ponit_pic_coordinate_temp[0] = p_avm_param->p_avm_config_param->avm_2d_param.junc_view_4_param.result_small_param.car_logo_rect.y
				+ p_avm_param->p_avm_config_param->avm_2d_param.junc_view_4_param.result_small_param.car_logo_rect.height / 2
				- (world_circle[1] / pixel_hight_dis);
			key_ponit_pic_coordinate_temp[1] = p_avm_param->p_avm_config_param->avm_2d_param.junc_view_4_param.result_small_param.car_logo_rect.x
				+ p_avm_param->p_avm_config_param->avm_2d_param.junc_view_4_param.result_small_param.car_logo_rect.width / 2
				+ (world_circle[0] / pixel_width_dis);

		}
		if (rotate_flag == ROTATE_3_VIEW_FRONT)//3前路环视图大图的动态线
		{
			Float64_t m = (p_avm_param->p_avm_config_param->vehicle_param.vehicle_length / 2 - p_avm_param->p_avm_config_param->avm_2d_param.junc_view_3_front_param.common_config.car_head_length);
			key_ponit_pic_coordinate_temp[0] = result_image_height-(world_circle[1] - m) / pixel_hight_dis;/* p_avm_param->p_avm_config_param->avm_2d_param.junc_view_3_front_param.result_3_front_param.car_logo_rect.y
				- */
			key_ponit_pic_coordinate_temp[1] = p_avm_param->p_avm_config_param->avm_2d_param.junc_view_3_front_param.result_3_front_param.car_logo_rect.x
				+ p_avm_param->p_avm_config_param->avm_2d_param.junc_view_3_front_param.result_3_front_param.car_logo_rect.width / 2
				+ (world_circle[0] / pixel_width_dis);

		} 

		if (rotate_flag == ROTATE_3_VIEW_REAR)//3后路环视图大图的动态线
		{
			Float64_t m = (p_avm_param->p_avm_config_param->vehicle_param.vehicle_length / 2 
				- p_avm_param->p_avm_config_param->avm_2d_param.junc_view_3_rear_param.common_config.car_rear_length);//求出汽车一半减去后视图漏出的车尾部
			key_ponit_pic_coordinate_temp[0] = -(world_circle[1] + m) / pixel_hight_dis;/* p_avm_param->p_avm_config_param->avm_2d_param.junc_view_3_front_param.result_3_front_param.car_logo_rect.y
																											 - */
			key_ponit_pic_coordinate_temp[1] = p_avm_param->p_avm_config_param->avm_2d_param.junc_view_3_rear_param.result_3_rear_param.car_logo_rect.x
				+ p_avm_param->p_avm_config_param->avm_2d_param.junc_view_3_rear_param.result_3_rear_param.car_logo_rect.width / 2
				+ (world_circle[0] / pixel_width_dis);

		}


		if (pic_coor_temp[0] != key_ponit_pic_coordinate_temp[0] || pic_coor_temp[1] != key_ponit_pic_coordinate_temp[1])
		{
			for (int j = key_ponit_pic_coordinate_temp[1] - LINE_WIDTH; j < key_ponit_pic_coordinate_temp[1]; j++)
			{
				(*p_rotate_altogether_add)++;
				int color_flag = 0;
				zip_data_draw_line((*p_rotate_altogether_add), p_lut_line, cam_flag, key_ponit_pic_coordinate_temp[0], j, color_flag);
				//image_result_single_front_lut->imageData[key_ponit_pic_coordinate_temp[0] * single_view_param.result_image_width + j] = 255;

			}
		}
		pic_coor_temp[0] = key_ponit_pic_coordinate_temp[0];//存留上次画出的点的坐标以便当下一次进行比较以便确定保存的点没有重复的
		pic_coor_temp[1] = key_ponit_pic_coordinate_temp[1];
	}
}
/*
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
	Float64_t rotate_angle, 
	Float64_t  pixel_hight_dis, 
	Float64_t  pixel_width_dis,
	int cam_flag, 
	Float64_t *cam_ray, 
	UInt32_t * p_lut_line,
	Int32_t result_image_height,
	Int32_t result_image_width)
{

	static UInt32_t rotate_altogether_add=100;
	int rotate_only_add = 0;
	int pic_coor_temp[2];

	Float64_t centre_rotate_coordinate[3];
	Float64_t car_head[3] = { 0, 0, 0 };
	Float64_t rotate_radius;
	Float64_t angle_0;
	Float64_t angle_average;
	if (rotate_angle > 0)//角度大于零是左转弯
	{
		int shift_angle = 30 + rotate_angle;
		p_lut_line[shift_angle] = rotate_altogether_add;
		/**************左转弯*********************/
		centre_rotate_coordinate[0] = -(p_avm_param->p_avm_config_param->vehicle_param.wheel_base / tan((rotate_angle / 180.00)*PI)
									+ p_avm_param->p_avm_config_param->vehicle_param.vehicle_width / 2);//假定出圆心坐标X坐标
		centre_rotate_coordinate[1] = -p_avm_param->p_avm_config_param->vehicle_param.wheel_base / 2;//假定出圆心坐标Y坐标

		/****第一条弯曲的线**/
		car_head[0] = -p_avm_param->p_avm_config_param->vehicle_param.vehicle_width / 2;//是x坐标 求出车头前100mm以便确定圆弧
		if (rotate_flag == ROTATE_4_VIEW_BIG_FRONT || rotate_flag == ROTATE_PLANE_EXPEND_FRONT || rotate_flag == ROTATE_4_VIEW_SMALL_FRONT || rotate_flag == ROTATE_3_VIEW_FRONT)
		{
			car_head[1] = p_avm_param->p_avm_config_param->vehicle_param.vehicle_length / 2 + 100;//确定Y坐标
		}
		if (rotate_flag == ROTATE_4_VIEW_BIG_REAR || rotate_flag == ROTATE_PLANE_EXPEND_REAR || rotate_flag == ROTATE_4_VIEW_SMALL_REAR || rotate_flag == ROTATE_3_VIEW_REAR)//如果是画车辆倒车的轨迹则Y坐标是是车辆的后边
		{
			car_head[1] = -p_avm_param->p_avm_config_param->vehicle_param.vehicle_length / 2 - 100;//确定Y坐标
		}

		rotate_radius = sqrt((car_head[0] - centre_rotate_coordinate[0])*(car_head[0] - centre_rotate_coordinate[0])
							+ (car_head[1] - centre_rotate_coordinate[1])*(car_head[1] - centre_rotate_coordinate[1]));//求出车旋转的半径
		angle_0 =dynamic_line_length / rotate_radius;//求出画面上一共显示的弧度角度，用画出的弧线长比上旋转半径得出弧度角 
		if (rotate_flag == ROTATE_4_VIEW_BIG_REAR || rotate_flag == ROTATE_4_VIEW_SMALL_REAR || rotate_flag == ROTATE_PLANE_EXPEND_REAR || rotate_flag == ROTATE_3_VIEW_REAR)
		{
			angle_average = -angle_0 / EQUALITY_ANGLE;//将显示的所有弧度数平分5000,后倒车时将角度平分设为负值则旋转方向是逆时针 
		}
		else
		{
			angle_average = angle_0 / EQUALITY_ANGLE;//将显示的所有弧度数平分5000
		}
		
		/********确定出圆心与半径，以及车头前的轨迹起始点之后开始调用画圆弧的函数画 轨迹，******/
		draw_circle_trail(rotate_flag,centre_rotate_coordinate,
							car_head, angle_average,p_avm_param,
							pixel_hight_dis, pixel_width_dis, dynamic_line_length,
							cam_ray, cam_flag, p_lut_line, &rotate_altogether_add,
							result_image_height,result_image_width);

		/********* 画第二条轨迹线 ************/
		/*第二条轨迹线只是X的坐标右移到车的右边X是正的 车头的右边*********/
		car_head[0] = p_avm_param->p_avm_config_param->vehicle_param.vehicle_width / 2;
		
		rotate_radius = sqrt((car_head[0] - centre_rotate_coordinate[0])*(car_head[0] - centre_rotate_coordinate[0])
			+ (car_head[1] - centre_rotate_coordinate[1])*(car_head[1] - centre_rotate_coordinate[1]));//求出车旋转的半径
		angle_0 = dynamic_line_length / rotate_radius;//求出画面上一共显示的弧度角度，用画出的弧线长比上旋转半径得出弧度角 

		if (rotate_flag == ROTATE_4_VIEW_BIG_REAR || rotate_flag == ROTATE_4_VIEW_SMALL_REAR || rotate_flag == ROTATE_PLANE_EXPEND_REAR || rotate_flag == ROTATE_3_VIEW_REAR)
		{
			angle_average = -angle_0 / EQUALITY_ANGLE;//将显示的所有弧度数平分5000
		}
		else
		{
			angle_average = angle_0 / EQUALITY_ANGLE;//将显示的所有弧度数平分5000
		}


		/********确定出圆心与半径，以及车头前的轨迹起始点之后开始调用画圆弧的函数画 轨迹，******/
		draw_circle_trail(rotate_flag, centre_rotate_coordinate, car_head, angle_average, p_avm_param,
						pixel_hight_dis, pixel_width_dis, dynamic_line_length,
						cam_ray, cam_flag, p_lut_line, &rotate_altogether_add, result_image_height, result_image_width);
	}
	/*******右转弯*********/
	if (rotate_angle < 0)
	{
		rotate_angle = abs(rotate_angle);
		int shift_angle = int(30-rotate_angle);
		p_lut_line[shift_angle] = rotate_altogether_add;
		/***************************右转弯*********************/
		centre_rotate_coordinate[0] = (p_avm_param->p_avm_config_param->vehicle_param.wheel_base / tan((rotate_angle / 180.00)*PI)
									+ p_avm_param->p_avm_config_param->vehicle_param.vehicle_width / 2);//假定出圆心坐标
		centre_rotate_coordinate[1] = -p_avm_param->p_avm_config_param->vehicle_param.wheel_base / 2;//假定出圆心坐标


		/****第一条线**/
		car_head[0] = p_avm_param->p_avm_config_param->vehicle_param.vehicle_width / 2;//x 坐标求出车头前100mm以便确定圆弧
		//car_head[1] = p_avm_param->p_avm_config_param->vehicle_param.vehicle_length / 2 + 100;
		if (rotate_flag == ROTATE_4_VIEW_BIG_FRONT || rotate_flag == ROTATE_PLANE_EXPEND_FRONT || rotate_flag == ROTATE_3_VIEW_FRONT)
		{
			car_head[1] = p_avm_param->p_avm_config_param->vehicle_param.vehicle_length / 2 + 100;
		}
		if (rotate_flag == ROTATE_4_VIEW_BIG_REAR || rotate_flag == ROTATE_PLANE_EXPEND_REAR || rotate_flag == ROTATE_3_VIEW_REAR)
		{
			car_head[1] = -p_avm_param->p_avm_config_param->vehicle_param.vehicle_length / 2 - 100;
		}
		rotate_radius = sqrt((car_head[0] - centre_rotate_coordinate[0])*(car_head[0] - centre_rotate_coordinate[0])
			+ (car_head[1] - centre_rotate_coordinate[1])*(car_head[1] - centre_rotate_coordinate[1]));//求出车旋转的半径
		angle_0 = dynamic_line_length / rotate_radius;//求出画面上一共显示的弧度角度

		if (rotate_flag == ROTATE_4_VIEW_BIG_REAR || rotate_flag == ROTATE_4_VIEW_SMALL_REAR || rotate_flag == ROTATE_PLANE_EXPEND_REAR || rotate_flag == ROTATE_3_VIEW_REAR)
		{
			angle_average = angle_0 / EQUALITY_ANGLE;//将显示的所有弧度数平分5000
		}
		else
		{
			angle_average = -angle_0 / EQUALITY_ANGLE;//将显示的所有弧度数平分5000
		}

		//angle_average = -angle_0 / EQUALITY_ANGLE;//将显示的所有弧度数平分5000
		/********确定出圆心与半径，以及车头前的轨迹起始点之后开始调用画圆弧的函数画 轨迹，******/
		draw_circle_trail(rotate_flag, centre_rotate_coordinate, car_head, angle_average, p_avm_param,
			pixel_hight_dis, pixel_width_dis, dynamic_line_length,
			cam_ray, cam_flag, p_lut_line, &rotate_altogether_add, result_image_height, result_image_width);

		/*********draw 画第二条轨迹线 ************/
		car_head[0] = -p_avm_param->p_avm_config_param->vehicle_param.vehicle_width / 2;
		
		rotate_radius = sqrt((car_head[0] - centre_rotate_coordinate[0])*(car_head[0] - centre_rotate_coordinate[0])
			+ (car_head[1] - centre_rotate_coordinate[1])*(car_head[1] - centre_rotate_coordinate[1]));

		angle_0 = dynamic_line_length / rotate_radius;
		if (rotate_flag == ROTATE_4_VIEW_BIG_REAR || rotate_flag == ROTATE_4_VIEW_SMALL_REAR || rotate_flag == ROTATE_PLANE_EXPEND_REAR || rotate_flag == ROTATE_3_VIEW_REAR)
		{
			angle_average = angle_0 / EQUALITY_ANGLE;//将显示的所有弧度数平分5000
		}
		else
		{
			angle_average = -angle_0 / EQUALITY_ANGLE;//将显示的所有弧度数平分5000
		}
		/********确定出圆心与半径，以及车头前的轨迹起始点之后开始调用画圆弧的函数画 轨迹，******/
		draw_circle_trail(rotate_flag, centre_rotate_coordinate,
			car_head, angle_average,
			p_avm_param, pixel_hight_dis, pixel_width_dis, dynamic_line_length, cam_ray,
			cam_flag, p_lut_line, &rotate_altogether_add, result_image_height, result_image_width);
	}

}



/*
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
		dynamic_line_count = 
		p_avm_param->p_avm_config_param->avm_single_view_param.front_single_view_param.dis_line_param.dynamic_line_param.dynamic_line_count;//用户配置的动态线个数
		rotate_flag = ROTATE_PLANE_EXPEND_REAR;
	}
	Float64_t cam_ray[3];
	Float64_t tan_h = (tan((single_view_param.plane_expand_param.fov_h / 2 / 180)*PI));
	Float64_t tan_v = (tan((single_view_param.plane_expand_param.fov_v / 2 / 180)*PI));
	cam_ray[2] = -8000.00;
	Float64_t  pixel_hight_dis = -(cam_ray[2] * tan_v) / (single_view_param.result_image_height / 2);//
	Float64_t  pixel_width_dis = -(cam_ray[2] * tan_h) / (single_view_param.result_image_width / 2);
	Float64_t image_point[2];
	IplImage *image_result_single_front_lut = cvCreateImage(size0, IPL_DEPTH_8U, 1);
	memset(image_result_single_front_lut->imageData, 0, single_view_param.result_image_width*single_view_param.result_image_height);
	/*********draw dynamic line***********/
	Float64_t rotate_angle = 0.0000;

	for (Float64_t rotate_angle =20; rotate_angle < 25; rotate_angle++)
	{
		//revolve_one_trail_from_one_angle
		revolve_one_trail_from_one_angle(rotate_flag, p_avm_param,
			single_view_param.dis_line_param.dynamic_line_param.dynamic_line_length,
								rotate_angle, pixel_hight_dis, pixel_width_dis,
								cam_flag, cam_ray, p_lut_line,
								single_view_param.result_image_height, single_view_param.result_image_width);
	}
#if DEBUG_INFO
	IplImage* image[4] = { NULL, NULL, NULL, NULL };
	ret = my_load_image(image);
	CHECK_ERROR(ret);

	
	for (int i = 0; i < single_view_param.result_image_height; i++)
	{
		for (int j = 0; j < single_view_param.result_image_width; j++)
		{
			if (cam_flag == 0)
			{
				cam_ray[1] = (Float64_t)((Float64_t(j) - ((Float64_t)(single_view_param.result_image_width) / 2.00))* pixel_width_dis);
				cam_ray[0] = (Float64_t)((Float64_t(i) - ((Float64_t)(single_view_param.result_image_height) / 2.00))* pixel_hight_dis);
			}
			if (cam_flag == 1)
			{
				cam_ray[1] = (Float64_t)((Float64_t(single_view_param.result_image_width - j - 1) - ((Float64_t)(single_view_param.result_image_width) / 2.00))* pixel_width_dis);
				cam_ray[0] = (Float64_t)((Float64_t(single_view_param.result_image_height - i - 1) - ((Float64_t)(single_view_param.result_image_height) / 2.00))* pixel_hight_dis);
			}

			Cam_Ray_To_Image_Point(image_point, cam_ray, &(p_avm_param->p_avm_config_param->camera_model[0].camera_model_int));
			int rgb[3];
			bilinear_interpolation(image[cam_flag], rgb, image_point[1], image_point[0]);
			image_result_single_front_lut->imageData[i *single_view_param.result_image_width + j] = (uchar)(rgb[0]);

		}
	}
	

	char result_name[MAX_ARRAY_SIZE];
	sprintf(result_name, "%s/dynanmic.bmp", result_image_path);
	cvSaveImage(result_name, image_result_single_front_lut);


#endif
	return ret;
}






/*************************/
void awb_4_view_calculate_key_point_coordinate(OUT Int32_t key_ponit_pic_coordinate[16][2],
							IN Float64_t key_point_world_coordinate[2],
							IN Float64_t car_length_half, 
							IN P_Avm_Param_S p_avm_param, 
							CvRect car_logo_rect, 
							Int32_t static_line_dis_to_car[4],
							Float64_t pixel_row_dis,
							Float64_t pixel_col_dis)
{

	
	/**************calculate left*****************/
	
	for (int i = 0; i < 4; i++)
	{
		//int a = key_ponit_pic_coordinate[i][0];
		key_point_world_coordinate[1] = car_length_half + static_line_dis_to_car[i];
		key_ponit_pic_coordinate[i][0] = car_logo_rect.y + car_logo_rect.height / 2
										- (key_point_world_coordinate[1] / pixel_row_dis);
		key_ponit_pic_coordinate[i][1] = car_logo_rect.x + car_logo_rect.width / 2 
										+ (key_point_world_coordinate[0] / pixel_col_dis);
	}
	key_point_world_coordinate[0] = key_point_world_coordinate[0] + LINE_ACROSS_EXTEND;
	for (int i = 4; i < 8; i++)
	{
		key_point_world_coordinate[1] = car_length_half + static_line_dis_to_car[i - 4];
		key_ponit_pic_coordinate[i][0] = car_logo_rect.y + car_logo_rect.height / 2 - (key_point_world_coordinate[1] / pixel_row_dis);
		key_ponit_pic_coordinate[i][1] = car_logo_rect.x + car_logo_rect.width / 2 + (key_point_world_coordinate[0] / pixel_col_dis);
	}
	/**************calculate right*****************/
	key_point_world_coordinate[0] = p_avm_param->p_avm_config_param->vehicle_param.vehicle_width / 2.00;
	for (int i = 8; i < 12; i++)
	{
		key_point_world_coordinate[1] = car_length_half + static_line_dis_to_car[i - 8];
		key_ponit_pic_coordinate[i][0] = car_logo_rect.y + car_logo_rect.height / 2 - (key_point_world_coordinate[1] / pixel_row_dis);
		key_ponit_pic_coordinate[i][1] = car_logo_rect.x + car_logo_rect.width / 2 + (key_point_world_coordinate[0] / pixel_col_dis);
	}
	key_point_world_coordinate[0] = key_point_world_coordinate[0] - LINE_ACROSS_EXTEND;
	for (int i = 12; i < 16; i++)
	{
		key_point_world_coordinate[1] = car_length_half + static_line_dis_to_car[i - 12];
		key_ponit_pic_coordinate[i][0] = car_logo_rect.y + car_logo_rect.height / 2 - (key_point_world_coordinate[1] / pixel_row_dis);
		key_ponit_pic_coordinate[i][1] = car_logo_rect.x + car_logo_rect.width / 2 + (key_point_world_coordinate[0] / pixel_col_dis);
	}
}

# if 1
/*
* Function Name:			draw_static_line_4_view
* Function Function:			draw_static_line_4_view
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
//#define  DEBUG_INFO
AVM_Error_Code_E  draw_static_line_4_view(IN P_Avm_Param_S p_avm_param,IN int cam_flag)
{

	AVM_Error_Code_E ret = ERROR_NONE;
	UInt32_t *p_lut_static_line=NULL;
	Result_Size_Param_S	 result_param;
	Junc_View_4_Param_S junc_view_4_param = p_avm_param->p_avm_config_param->avm_2d_param.junc_view_4_param;
	Dis_Line_Param_S dis_line_param;
	CvRect		car_logo_rect;
	if (cam_flag == 0)
	{
		p_lut_static_line = p_avm_param->p_avm_dis_line->p_static_dis_line_4_big;
		result_param = p_avm_param->p_avm_config_param->avm_2d_param.junc_view_4_param.result_big_param;
		dis_line_param=junc_view_4_param.big_view_dis_line_param;
		car_logo_rect=p_avm_param->p_avm_config_param->avm_2d_param.junc_view_4_param.result_big_param.car_logo_rect;
	}
	if (cam_flag == 1)
	{
		p_lut_static_line = p_avm_param->p_avm_dis_line->p_static_dis_line_4_small;
		result_param = p_avm_param->p_avm_config_param->avm_2d_param.junc_view_4_param.result_small_param;
		dis_line_param = junc_view_4_param.small_view_dis_line_param;
		car_logo_rect = p_avm_param->p_avm_config_param->avm_2d_param.junc_view_4_param.result_small_param.car_logo_rect;
	}
	Float64_t pixel_row_dis = (p_avm_param->p_avm_config_param->vehicle_param.vehicle_length
		+ junc_view_4_param.common_config.front_view_length + junc_view_4_param.common_config.rear_view_length)
		/ (Float32_t)(result_param.result_image_height);//calculate every pixel distance
	Float64_t pixel_col_dis = (p_avm_param->p_avm_config_param->vehicle_param.vehicle_width + junc_view_4_param.common_config.left_view_length
		+ junc_view_4_param.common_config.right_view_length)
		/ (Float32_t)(result_param.result_image_width);//calculate every pixel distance
	int add_point_shift=1;
	
	
#if DEBUG_INFO
	CvSize size0 = cvSize(result_param.result_image_width, result_param.result_image_height);//
	IplImage *image_result_big_lut = cvCreateImage(size0, IPL_DEPTH_8U, 1);
	memset(image_result_big_lut->imageData, 0
		, result_param.result_image_width*result_param.result_image_height);
#endif
	/*********画前车道************/
	int key_ponit_pic_coordinate[16][2];
	Float64_t key_point_world_coordinate[3] = { 0, 0, 0 };//2000mm
	Float64_t car_length_half;
	Int32_t	static_line_dis_to_car[4];
	car_length_half = p_avm_param->p_avm_config_param->vehicle_param.vehicle_length / 2.00;
	static_line_dis_to_car[0] = dis_line_param.static_line_param.static_line_dis_to_car[0];
	static_line_dis_to_car[1] = dis_line_param.static_line_param.static_line_dis_to_car[1];
	static_line_dis_to_car[2] = dis_line_param.static_line_param.static_line_dis_to_car[2];
	static_line_dis_to_car[3] = dis_line_param.static_line_param.static_line_dis_to_car[3];
	key_point_world_coordinate[0] = -p_avm_param->p_avm_config_param->vehicle_param.vehicle_width / 2.00;

	/**********计算16个关键点*******/
	awb_4_view_calculate_key_point_coordinate(key_ponit_pic_coordinate,
												key_point_world_coordinate,
												car_length_half,
												p_avm_param,
													car_logo_rect,
												static_line_dis_to_car,
												pixel_row_dis,
												pixel_col_dis);
		/*****************************/
		/*****draw 横线部分 ****/
		front_rear_draw_across_line(key_ponit_pic_coordinate, &add_point_shift, p_lut_static_line, cam_flag);
		/*****画竖线部分 ****/
		front_rear_draw_vertical_line(key_ponit_pic_coordinate, &add_point_shift, p_lut_static_line,cam_flag);

	/********画4路的后车道*********/
		car_length_half = -2355.00;
		static_line_dis_to_car[0] = -dis_line_param.static_line_param.static_line_dis_to_car[0];
		static_line_dis_to_car[1] = -dis_line_param.static_line_param.static_line_dis_to_car[1];
		static_line_dis_to_car[2] = -dis_line_param.static_line_param.static_line_dis_to_car[2];
		static_line_dis_to_car[3] = -dis_line_param.static_line_param.static_line_dis_to_car[3];
		key_point_world_coordinate[0] = -p_avm_param->p_avm_config_param->vehicle_param.vehicle_width / 2.00;
		awb_4_view_calculate_key_point_coordinate(key_ponit_pic_coordinate,
													key_point_world_coordinate,
													car_length_half,
													p_avm_param,
													car_logo_rect,
													static_line_dis_to_car,
													pixel_row_dis,
													pixel_col_dis);
		/*****************************/
		/*****draw 横线部分 ****/

		front_rear_draw_across_line(key_ponit_pic_coordinate, &add_point_shift, p_lut_static_line, cam_flag);
		/*****画竖线部分 ****/
		//front_rear_draw_vertical_line(key_ponit_pic_coordinate, &add_point_shift, p_lut_static_line, cam_flag);
		front_rear_draw_vertical_line_REAR(key_ponit_pic_coordinate, &add_point_shift, p_lut_static_line, cam_flag);
		p_lut_static_line[0] = add_point_shift;//静态表表头填充的是此表中一共多少个点

#if DEBUG_INFO
	char result_name[MAX_ARRAY_SIZE];
	sprintf(result_name, "%s/combine_overlook_lr.bmp", result_image_path);
	cvSaveImage(result_name, image_result_big_lut);
#endif
	return ret;
}


#endif




/*
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
AVM_Error_Code_E  draw_dynamic_4_view_line(IN P_Avm_Param_S p_avm_param,IN int cam_flag)
{

	AVM_Error_Code_E ret = ERROR_NONE;
	UInt32_t *p_lut_dynamic_line = NULL;
	Result_Size_Param_S	 result_param;
	Junc_View_4_Param_S junc_view_4_param = p_avm_param->p_avm_config_param->avm_2d_param.junc_view_4_param;
	Dis_Line_Param_S dis_line_param;

	Int32_t dynamic_line_count;
	if (cam_flag == 0)
	{
		p_lut_dynamic_line = p_avm_param->p_avm_dis_line->p_dynamic_dis_line_4_big;
		result_param = p_avm_param->p_avm_config_param->avm_2d_param.junc_view_4_param.result_big_param;
		dis_line_param = junc_view_4_param.big_view_dis_line_param;

		dynamic_line_count = p_avm_param->p_avm_config_param->avm_single_view_param.front_single_view_param.dis_line_param.dynamic_line_param.dynamic_line_count;
	}
	if (cam_flag == 1)
	{
		p_lut_dynamic_line = p_avm_param->p_avm_dis_line->p_dynamic_dis_line_4_small;
		result_param = p_avm_param->p_avm_config_param->avm_2d_param.junc_view_4_param.result_small_param;
		dis_line_param = junc_view_4_param.small_view_dis_line_param;

		dynamic_line_count = p_avm_param->p_avm_config_param->avm_single_view_param.rear_single_view_param.dis_line_param.dynamic_line_param.dynamic_line_count;
	}
	CvSize size0 = cvSize(result_param.result_image_width, result_param.result_image_height);//
	IplImage *image_result_4_view_lut = cvCreateImage(size0, IPL_DEPTH_8U, 1);
	memset(image_result_4_view_lut->imageData, 0, result_param.result_image_width*result_param.result_image_height);
	
	Float64_t pixel_row_dis = (p_avm_param->p_avm_config_param->vehicle_param.vehicle_length
		+ junc_view_4_param.common_config.front_view_length + junc_view_4_param.common_config.rear_view_length)
		/ (Float32_t)(result_param.result_image_height);//calculate every pixel distance
	Float64_t pixel_col_dis = (p_avm_param->p_avm_config_param->vehicle_param.vehicle_width
		+ junc_view_4_param.common_config.left_view_length + junc_view_4_param.common_config.right_view_length)
		/ (Float32_t)(result_param.result_image_width);//calculate every pixel distance
#if DEBUG_INFO
	IplImage* image[4] = { NULL, NULL, NULL, NULL };
	ret = my_load_image(image);
	CvRect car_logo_rect = junc_view_4_param.result_big_param.car_logo_rect;
	/****malloc look up table****/
	int camera_size[4];
	for (Int32_t cam_id = 0; cam_id < CAMERA_NUM; cam_id++)
	{
		camera_size[cam_id] = junc_view_4_param.result_big_param.lut_size[cam_id] * 2;
	}

	for (int i = 0; i < result_param.result_image_height; i++)
	{
		for (int j = 0; j < result_param.result_image_width; j++)
		{
			/************front view*******************************/
			if (i < car_logo_rect.y)
			{
				int shift = i * junc_view_4_param.result_big_param.result_image_width * 2 + j * 2;
				int rgb[3];
				resolve_data_from_lut(shift, image[0], rgb, p_avm_param);
				image_result_4_view_lut->imageData[i * result_param.result_image_width + j] += (uchar)(rgb[0]);
			}
			/*************rear view*********************************/

			if ((i >= (car_logo_rect.y + car_logo_rect.height)) && (i < junc_view_4_param.result_big_param.result_image_height))
			{
				int shift = camera_size[0] + (i - (car_logo_rect.y + car_logo_rect.height)) * junc_view_4_param.result_big_param.result_image_width * 2 + j * 2;
				int rgb[3];
				resolve_data_from_lut(shift, image[1], rgb, p_avm_param);
				image_result_4_view_lut->imageData[i * result_param.result_image_width + j] += (uchar)(rgb[0]);
				
			}
			/*******************************************left view********************************/
			if (j < car_logo_rect.x)
			{
				int shift = camera_size[0] + camera_size[1] + i * car_logo_rect.x * 2 + j * 2;
				int rgb[3];
				resolve_data_from_lut(shift, image[2], rgb, p_avm_param);
				image_result_4_view_lut->imageData[i * result_param.result_image_width + j] += (uchar)(rgb[0]);
			}
			/*************************right view****************/
			if (j >= (car_logo_rect.x + car_logo_rect.width) && j < junc_view_4_param.result_big_param.result_image_width)
			{
				int shift = camera_size[0] + camera_size[1] + camera_size[2]
					+ i * (junc_view_4_param.result_big_param.result_image_width - car_logo_rect.x - car_logo_rect.width) * 2
					+ (j - car_logo_rect.x - car_logo_rect.width) * 2;
				int rgb[3];
				resolve_data_from_lut(shift, image[3], rgb, p_avm_param);
				image_result_4_view_lut->imageData[i * result_param.result_image_width + j] += (uchar)(rgb[0]);
			}
		}

	}
	
	
#endif
	/*********draw dynamic line***********/
	Float64_t cam_ray[3];
	for (Float64_t rotate_angle = -30; rotate_angle <30; rotate_angle++)
	{
		/*calculate_static_rotate_coordinate_rear(p_avm_param, junc_view_4_param, 
			rotate_angle, cam_flag, image_result_4_view_lut, p_lut_dynamic_line, 0);*/
			revolve_one_trail_from_one_angle(ROTATE_4_VIEW_BIG_REAR, p_avm_param,
			junc_view_4_param.big_view_dis_line_param.dynamic_line_param.dynamic_line_length,
			rotate_angle, pixel_row_dis, pixel_col_dis,
			cam_flag, cam_ray, p_lut_dynamic_line,
			junc_view_4_param.result_big_param.result_image_height, junc_view_4_param.result_big_param.result_image_width);
	}

	/*char result_name[MAX_ARRAY_SIZE];
	sprintf(result_name, "%s/my_888.bmp", result_image_path);
	cvSaveImage(result_name, image_result_4_view_lut);*/
	return ret;
}






#if 0

/*
* Function Name:			calculate_pic_coordinate_from_world_coordinate
* Function Function:		calculate_pic_coordinate_from_world_coordinate
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


void calculate_pic_coordinate_from_world_coordinate(P_Avm_Param_S p_avm_param,
	Float64_t *world_coordinate, int cam_flag,
	Float64_t pixel_hight_dis, Float64_t pixel_width_dis, Float64_t *cam_ray,
	int *pic_point_coordinate, Int32_t result_image_height, Int32_t result_image_width)
{

	Float64_t camera_coordinate[3];
	Float64_t camera_coordinate_0[3];

	Mat_Mul_Vec_3by3_DB(camera_coordinate,
		p_avm_param->p_avm_config_param->camera_model[cam_flag].camera_model_ext.inv_R, world_coordinate);
	for (int i = 3; i < 6; i++)
	{
		camera_coordinate[i - 3] = p_avm_param->p_avm_config_param->camera_model[cam_flag].camera_model_ext.inv_pose[i]
			+ camera_coordinate[i - 3];
	}
	Float64_t tmp = 0;
	tmp = camera_coordinate[0];
	camera_coordinate[0] = camera_coordinate[1];
	camera_coordinate[1] = tmp;
	camera_coordinate[2] = -camera_coordinate[2];

	Float64_t temp_rate = cam_ray[2] / camera_coordinate[2];
	camera_coordinate_0[0] = camera_coordinate[0] * temp_rate;
	camera_coordinate_0[1] = camera_coordinate[1] * temp_rate;
	camera_coordinate_0[2] = cam_ray[2];
	int  pic_hight = (camera_coordinate_0[0] / Float64_t(pixel_hight_dis)) + (result_image_height / 2);
	pic_point_coordinate[0] = pic_hight;
	int  pic_width = (camera_coordinate_0[1] / Float64_t(pixel_width_dis)) + (result_image_width / 2);
	pic_point_coordinate[1] = pic_width;
}

#endif



//#define DEBUG_INFO 1

/*
* Function Name:			calculate_static_rotate_coordinate
* Function Function:			calculate_static_rotate_coordinate
* Input:
*       shift;               p_avm_param
*		image;                cam_flag
p_avm_param;
* Note:
*
* Revision:
*     Create by Cheng.Li on 2018/1/17.
*/
void calculate_static_rotate_coordinate_rear(P_Avm_Param_S p_avm_param, Junc_View_4_Param_S single_view_param,
	Float64_t rotate_angle, int cam_flag, IplImage *image_result_single_front_lut, UInt32_t * p_lut_line, int small_big)
{

	static UInt32_t rotate_altogether_add = 100;
	int rotate_only_add = 0;
	int pic_coor_temp[2];

	Junc_View_4_Param_S junc_view_4_param = p_avm_param->p_avm_config_param->avm_2d_param.junc_view_4_param;
	Result_Size_Param_S result_param;
	if (small_big == 0)
	{
		result_param = junc_view_4_param.result_big_param;
	}
	if (small_big == 1)
	{
		result_param = junc_view_4_param.result_small_param;
	}
	CvRect car_logo_rect = junc_view_4_param.result_big_param.car_logo_rect;
	Float64_t pixel_row_dis = (p_avm_param->p_avm_config_param->vehicle_param.vehicle_length
		+ junc_view_4_param.common_config.front_view_length + junc_view_4_param.common_config.rear_view_length)
		/ (Float32_t)(result_param.result_image_height);//calculate every pixel distance
	Float64_t pixel_col_dis = (p_avm_param->p_avm_config_param->vehicle_param.vehicle_width
		+ junc_view_4_param.common_config.left_view_length + junc_view_4_param.common_config.right_view_length)
		/ (Float32_t)(result_param.result_image_width);//calculate every pixel distance

	/************rotate_angle 大于0是左转弯******************/
	if (rotate_angle >0)
	{
#if 1
		int shift_angle = 30 + rotate_angle;
		p_lut_line[shift_angle] = rotate_altogether_add;
		/**************左转弯*********************/
		/****第一条线**/
		{
			Float64_t centre_rotate_coordinate[3];
			Float64_t car_head[3] = { 0, 0, 0 };

			centre_rotate_coordinate[0] = -(p_avm_param->p_avm_config_param->vehicle_param.wheel_base / tan((rotate_angle / 180.00)*PI)
				+ p_avm_param->p_avm_config_param->vehicle_param.vehicle_width / 2);//假定出圆心坐标
			centre_rotate_coordinate[1] = -p_avm_param->p_avm_config_param->vehicle_param.wheel_base / 2;//假定出圆心坐标
			car_head[0] = -p_avm_param->p_avm_config_param->vehicle_param.vehicle_width / 2;//求出车头前100mm以便确定圆弧
			car_head[1] = -p_avm_param->p_avm_config_param->vehicle_param.vehicle_length / 2 - 100;
			Float64_t rotate_radius = sqrt((car_head[0] - centre_rotate_coordinate[0])*(car_head[0] - centre_rotate_coordinate[0])
				+ (car_head[1] - centre_rotate_coordinate[1])*(car_head[1] - centre_rotate_coordinate[1]));//求出车旋转的半径
			Float64_t angle_0;
			if (small_big == 0)
			{
				angle_0 = single_view_param.big_view_dis_line_param
					.dynamic_line_param.dynamic_line_length / rotate_radius;//求出画面上一共显示的弧度角度
			}
			if (small_big == 1)
			{
				angle_0 = single_view_param.small_view_dis_line_param
					.dynamic_line_param.dynamic_line_length / rotate_radius;//求出画面上一共显示的弧度角度
			}
			Float64_t angle_average = -angle_0 / 5000;//将显示的所有弧度数平分5000
			Float64_t angle_base = 0;
			for (int i = 0; i < 5000; i++)
			{
				Float64_t world_circle[3] = { 0, 0, 0 };
				angle_base = angle_base + angle_average;
				world_circle[0] = (car_head[0] - centre_rotate_coordinate[0])*cos(angle_base)
					- (car_head[1] - centre_rotate_coordinate[1])*sin(angle_base) + centre_rotate_coordinate[0];///is z
				world_circle[1] = (car_head[0] - centre_rotate_coordinate[0])*sin(angle_base)
					+ (car_head[1] - centre_rotate_coordinate[1])*cos(angle_base) + centre_rotate_coordinate[1];//is y
				int key_ponit_pic_coordinate_temp[2];
				key_ponit_pic_coordinate_temp[0] = car_logo_rect.y + car_logo_rect.height / 2 -(world_circle[1] / pixel_row_dis);
				key_ponit_pic_coordinate_temp[1] = car_logo_rect.x + car_logo_rect.width / 2 +(world_circle[0] / pixel_col_dis);

				if (pic_coor_temp[0] != key_ponit_pic_coordinate_temp[0] || pic_coor_temp[1] != key_ponit_pic_coordinate_temp[1])
				{
					for (int j = key_ponit_pic_coordinate_temp[1] - LINE_WIDTH; j < key_ponit_pic_coordinate_temp[1]; j++)
					{
						rotate_altogether_add++;
						rotate_only_add++;
						int color_flag = 0;
						zip_data_draw_line(rotate_altogether_add, p_lut_line, cam_flag, key_ponit_pic_coordinate_temp[0], j, color_flag);
#if DEBUG_INFO
						image_result_single_front_lut->imageData[key_ponit_pic_coordinate_temp[0] * result_param.result_image_width + j] = 255;
#endif
					}
				}
				pic_coor_temp[0] = key_ponit_pic_coordinate_temp[0];
				pic_coor_temp[1] = key_ponit_pic_coordinate_temp[1];

			}
		}
		/*********draw second************/
		{
			Float64_t centre_rotate_coordinate[3];
			Float64_t car_head[3] = { 0, 0, 0 };

			centre_rotate_coordinate[0] = -(p_avm_param->p_avm_config_param->vehicle_param.wheel_base / tan((rotate_angle / 180.00)*PI)
				+ p_avm_param->p_avm_config_param->vehicle_param.vehicle_width / 2);//假定出圆心坐标
			centre_rotate_coordinate[1] = -p_avm_param->p_avm_config_param->vehicle_param.wheel_base / 2;//假定出圆心坐标

			//centre_rotate_coordinate[0] = -(p_avm_param->p_avm_config_param
			//	->vehicle_param.wheel_base / tan((rotate_angle / 180.00)*PI)
			//	+ p_avm_param->p_avm_config_param->vehicle_param.vehicle_width / 2);//车辆转弯时绕哪个点转弯做圆周运动
			//centre_rotate_coordinate[1] = -p_avm_param->p_avm_config_param->vehicle_param.vehicle_length / 2;

			car_head[0] = p_avm_param->p_avm_config_param->vehicle_param.vehicle_width / 2;
			car_head[1] = -p_avm_param->p_avm_config_param->vehicle_param.vehicle_length / 2 - 100;

			/*Float64_t rotate_radius = sqrt((car_head[0] - centre_rotate_coordinate[0])
			*(car_head[0] - centre_rotate_coordinate[0])
			+ (car_head[1] - centre_rotate_coordinate[1])*(car_head[1] - centre_rotate_coordinate[1]));*/
			Float64_t rotate_radius = sqrt((car_head[0] - centre_rotate_coordinate[0])*(car_head[0] - centre_rotate_coordinate[0])
				+ (car_head[1] - centre_rotate_coordinate[1])*(car_head[1] - centre_rotate_coordinate[1]));//求出车旋转的半径
			Float64_t angle_0;
			if (small_big == 0)
			{
				angle_0 = single_view_param.big_view_dis_line_param.dynamic_line_param.dynamic_line_length / rotate_radius;//求出画面上一共显示的弧度角度
			}
			if (small_big == 1)
			{
				angle_0 = single_view_param.small_view_dis_line_param.dynamic_line_param.dynamic_line_length / rotate_radius;//求出画面上一共显示的弧度角度
			}
			Float64_t angle_average = angle_0 / 5000;
			Float64_t angle_base = angle_0;
			angle_base = 0.000;

			for (int i = 0; i < 5000; i++)
			{
				Float64_t world_circle[3] = { 0, 0, 0 };

				angle_base = angle_base - angle_average;
				//angle_base = 0;
				world_circle[0] = (car_head[0] - centre_rotate_coordinate[0])*cos(angle_base) - (car_head[1] - centre_rotate_coordinate[1])*sin(angle_base) + centre_rotate_coordinate[0];///is z
				world_circle[1] = (car_head[0] - centre_rotate_coordinate[0])*sin(angle_base) + (car_head[1] - centre_rotate_coordinate[1])*cos(angle_base) + centre_rotate_coordinate[1];//is y
				int key_ponit_pic_coordinate_temp[2];
				key_ponit_pic_coordinate_temp[0] = car_logo_rect.y + car_logo_rect.height / 2
					-(world_circle[1] / pixel_row_dis);
				key_ponit_pic_coordinate_temp[1] = car_logo_rect.x + car_logo_rect.width / 2
					+ (world_circle[0] / pixel_col_dis);
				if (pic_coor_temp[0] != key_ponit_pic_coordinate_temp[0] || pic_coor_temp[1] != key_ponit_pic_coordinate_temp[1])
				{
					for (int j = key_ponit_pic_coordinate_temp[1] - LINE_WIDTH; j < key_ponit_pic_coordinate_temp[1]; j++)
					{
						rotate_altogether_add++;
						rotate_only_add++;
						int color_flag = 0;
						zip_data_draw_line(rotate_altogether_add, p_lut_line, cam_flag, key_ponit_pic_coordinate_temp[0], j, color_flag);
#if DEBUG_INFO
						image_result_single_front_lut->imageData[key_ponit_pic_coordinate_temp[0] * result_param.result_image_width + j] = 255;
#endif
					}
				}
				pic_coor_temp[0] = key_ponit_pic_coordinate_temp[0];
				pic_coor_temp[1] = key_ponit_pic_coordinate_temp[1];
			}
		}
#endif
	}
	/************rotate_angle 小于0是右转弯******************/
	if (rotate_angle <= 0)
	{
		rotate_angle = abs(rotate_angle);
		int shift_angle = int(30 - rotate_angle);
		p_lut_line[shift_angle] = rotate_altogether_add;
		/***************************后面的右转弯*********************/
		{
#if 1
			/****第一条线**/
			{
				Float64_t centre_rotate_coordinate[3];
				Float64_t car_head[3] = { 0, 0, 0 };
				centre_rotate_coordinate[0] = (p_avm_param->p_avm_config_param->vehicle_param.wheel_base / tan((rotate_angle / 180.00)*PI)
					+ p_avm_param->p_avm_config_param->vehicle_param.vehicle_width / 2);//假定出圆心坐标
				centre_rotate_coordinate[1] = -p_avm_param->p_avm_config_param->vehicle_param.wheel_base / 2;//假定出圆心坐标
				car_head[0] = p_avm_param->p_avm_config_param->vehicle_param.vehicle_width / 2;//求出车头前100mm以便确定圆弧
				car_head[1] = -p_avm_param->p_avm_config_param->vehicle_param.vehicle_length / 2 - 100;
				Float64_t rotate_radius = sqrt((car_head[0] - centre_rotate_coordinate[0])*(car_head[0] - centre_rotate_coordinate[0])
					+ (car_head[1] - centre_rotate_coordinate[1])*(car_head[1] - centre_rotate_coordinate[1]));//求出车旋转的半径
				Float64_t angle_0;
				if (small_big == 0)
				{
					angle_0 = single_view_param.big_view_dis_line_param.dynamic_line_param.dynamic_line_length / rotate_radius;//求出画面上一共显示的弧度角度
				}
				if (small_big == 1)
				{
					angle_0 = single_view_param.small_view_dis_line_param.dynamic_line_param.dynamic_line_length / rotate_radius;//求出画面上一共显示的弧度角度
				}
				Float64_t angle_average = angle_0 / 5000;//将显示的所有弧度数平分5000
				Float64_t angle_base = 0;
				for (int i = 0; i < 5000; i++)
				{
					Float64_t world_circle[3] = { 0, 0, 0 };
					angle_base = angle_base + angle_average;
					world_circle[0] = (car_head[0] - centre_rotate_coordinate[0])*cos(angle_base)
						- (car_head[1] - centre_rotate_coordinate[1])*sin(angle_base) + centre_rotate_coordinate[0];///is z
					world_circle[1] = (car_head[0] - centre_rotate_coordinate[0])*sin(angle_base)
						+ (car_head[1] - centre_rotate_coordinate[1])*cos(angle_base) + centre_rotate_coordinate[1];//is y
					int key_ponit_pic_coordinate_temp[2];
					key_ponit_pic_coordinate_temp[0] = car_logo_rect.y + car_logo_rect.height / 2
						- (world_circle[1] / pixel_row_dis);
					key_ponit_pic_coordinate_temp[1] = car_logo_rect.x + car_logo_rect.width / 2
						+ (world_circle[0] / pixel_col_dis);
					if (pic_coor_temp[0] != key_ponit_pic_coordinate_temp[0] || pic_coor_temp[1] != key_ponit_pic_coordinate_temp[1])
					{
						for (int j = key_ponit_pic_coordinate_temp[1] - LINE_WIDTH; j < key_ponit_pic_coordinate_temp[1]; j++)
						{
							rotate_altogether_add++;
							rotate_only_add++;
							int color_flag = 0;
							zip_data_draw_line(rotate_altogether_add, p_lut_line, cam_flag, key_ponit_pic_coordinate_temp[0], j, color_flag);
#if DEBUG_INFO
							image_result_single_front_lut->imageData[key_ponit_pic_coordinate_temp[0] * result_param.result_image_width + j] = 255;
#endif
						}
					}
					pic_coor_temp[0] = key_ponit_pic_coordinate_temp[0];
					pic_coor_temp[1] = key_ponit_pic_coordinate_temp[1];

				}
			}
#endif
			/*********draw second************/
			{
			Float64_t centre_rotate_coordinate[3];
			Float64_t car_head[3] = { 0, 0, 0 };
			centre_rotate_coordinate[0] = (p_avm_param->p_avm_config_param->vehicle_param.wheel_base / tan((rotate_angle / 180.00)*PI)
				+ p_avm_param->p_avm_config_param->vehicle_param.vehicle_width / 2);//假定出圆心坐标
			centre_rotate_coordinate[1] = -p_avm_param->p_avm_config_param->vehicle_param.wheel_base / 2;//假定出圆心坐标
			car_head[0] = -p_avm_param->p_avm_config_param->vehicle_param.vehicle_width / 2;
			car_head[1] = -p_avm_param->p_avm_config_param->vehicle_param.vehicle_length / 2 - 100;
			Float64_t rotate_radius = sqrt((car_head[0] - centre_rotate_coordinate[0])*(car_head[0] - centre_rotate_coordinate[0])
				+ (car_head[1] - centre_rotate_coordinate[1])*(car_head[1] - centre_rotate_coordinate[1]));
			Float64_t angle_0;
			if (small_big == 0)
			{
				angle_0 = single_view_param.big_view_dis_line_param.dynamic_line_param.dynamic_line_length / rotate_radius;//求出画面上一共显示的弧度角度
			}
			if (small_big == 1)
			{
				angle_0 = single_view_param.small_view_dis_line_param.dynamic_line_param.dynamic_line_length / rotate_radius;//求出画面上一共显示的弧度角度
			}
			Float64_t angle_average = angle_0 / 5000;
			Float64_t angle_base = 0;
			for (int i = 0; i < 5000; i++)
			{
				Float64_t world_circle[3] = { 0, 0, 0 };
				angle_base = angle_base + angle_average;
				world_circle[0] = (car_head[0] - centre_rotate_coordinate[0])*cos(angle_base) - (car_head[1]
					- centre_rotate_coordinate[1])*sin(angle_base) + centre_rotate_coordinate[0];///is z
				world_circle[1] = (car_head[0] - centre_rotate_coordinate[0])*sin(angle_base) + (car_head[1]
					- centre_rotate_coordinate[1])*cos(angle_base) + centre_rotate_coordinate[1];//is y
				int key_ponit_pic_coordinate_temp[2];
				key_ponit_pic_coordinate_temp[0] = car_logo_rect.y + car_logo_rect.height / 2
					+ (int)abs((world_circle[1] / pixel_row_dis));
				key_ponit_pic_coordinate_temp[1] = car_logo_rect.x + car_logo_rect.width / 2
					+ (world_circle[0] / pixel_col_dis);
				if (pic_coor_temp[0] != key_ponit_pic_coordinate_temp[0] || pic_coor_temp[1] != key_ponit_pic_coordinate_temp[1])
				{
					for (int j = key_ponit_pic_coordinate_temp[1] - LINE_WIDTH; j < key_ponit_pic_coordinate_temp[1]; j++)
					{
						rotate_altogether_add++;
						rotate_only_add++;
						int color_flag = 0;
						zip_data_draw_line(rotate_altogether_add, p_lut_line, cam_flag, key_ponit_pic_coordinate_temp[0], j, color_flag);
#if DEBUG_INFO
						image_result_single_front_lut->imageData[key_ponit_pic_coordinate_temp[0] * result_param.result_image_width + j] = 255;
#endif
					}
				}
				pic_coor_temp[0] = key_ponit_pic_coordinate_temp[0];
				pic_coor_temp[1] = key_ponit_pic_coordinate_temp[1];

			}
		}

		}

	}



}








/*************************/
void awb_3_view_calculate_key_point_coordinate(OUT Int32_t key_ponit_pic_coordinate[16][2],
	IN Float64_t key_point_world_coordinate[2],
	IN Float64_t car_length_half,
	IN P_Avm_Param_S p_avm_param,
	CvRect car_logo_rect,
	Int32_t static_line_dis_to_car[4],
	Float64_t pixel_row_dis,
	Float64_t pixel_col_dis)
{
	/**************calculate left*****************/
	key_point_world_coordinate[0] = -p_avm_param->p_avm_config_param->vehicle_param.vehicle_width / 2.00;
	for (int i = 0; i < 4; i++)
	{
		
		key_point_world_coordinate[1] = car_length_half + static_line_dis_to_car[i];
		Float64_t temp=key_point_world_coordinate[1];
		key_ponit_pic_coordinate[i][0] = car_logo_rect.y - (temp / pixel_row_dis);//key_ponit_pic_coordinate里面的第一个存储的是关键点的高,
		//此处将car_length_half赋值为0，因为car_logo_rect.y是图像的高从上到下的计数
		key_ponit_pic_coordinate[i][1] = car_logo_rect.x + car_logo_rect.width / 2
			+ (key_point_world_coordinate[0] / pixel_col_dis);
	}
	key_point_world_coordinate[0] = key_point_world_coordinate[0] + LINE_ACROSS_EXTEND;
	for (int i = 4; i < 8; i++)
	{
		key_point_world_coordinate[1] = car_length_half + static_line_dis_to_car[i - 4];
		Float64_t temp = key_point_world_coordinate[1];
		key_ponit_pic_coordinate[i][0] = car_logo_rect.y - (temp / pixel_row_dis);
		key_ponit_pic_coordinate[i][1] = car_logo_rect.x + car_logo_rect.width / 2 + (key_point_world_coordinate[0] / pixel_col_dis);
	}
	/**************calculate right*****************/
	key_point_world_coordinate[0] = p_avm_param->p_avm_config_param->vehicle_param.vehicle_width / 2.00;
	for (int i = 8; i < 12; i++)
	{
		key_point_world_coordinate[1] = car_length_half + static_line_dis_to_car[i - 8];
		Float64_t temp = key_point_world_coordinate[1];
		key_ponit_pic_coordinate[i][0] = car_logo_rect.y - (temp / pixel_row_dis);
		key_ponit_pic_coordinate[i][1] = car_logo_rect.x + car_logo_rect.width / 2 + (key_point_world_coordinate[0] / pixel_col_dis);
	}
	key_point_world_coordinate[0] = key_point_world_coordinate[0] - LINE_ACROSS_EXTEND;
	for (int i = 12; i < 16; i++)
	{
		key_point_world_coordinate[1] = car_length_half + static_line_dis_to_car[i - 12];
		Float64_t temp = key_point_world_coordinate[1];
		key_ponit_pic_coordinate[i][0] = car_logo_rect.y - (temp / pixel_row_dis);
		key_ponit_pic_coordinate[i][1] = car_logo_rect.x + car_logo_rect.width / 2 + (key_point_world_coordinate[0] / pixel_col_dis);
	}
}











/*
* Function Name:			draw_static_line_4_view
* Function Function:			draw_static_line_4_view
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
//#define  DEBUG_INFO
AVM_Error_Code_E  draw_static_line_3_view(IN P_Avm_Param_S p_avm_param, IN int cam_flag)
{

	AVM_Error_Code_E ret = ERROR_NONE;
	UInt32_t *p_lut_static_line = NULL;
	
	Dis_Line_Param_S dis_line_param;
	CvRect		car_logo_rect;
	Float64_t pixel_row_dis;
	Float64_t pixel_col_dis;
	Float64_t car_length_half = 0;
	Int32_t	static_line_dis_to_car[4];
	if (cam_flag == 0)
	{
		Junc_View_3_Front_Param_S junc_view_3_front_param = p_avm_param->p_avm_config_param->avm_2d_param.junc_view_3_front_param;
		p_lut_static_line = p_avm_param->p_avm_dis_line->p_static_dis_line_3_front;
		dis_line_param = junc_view_3_front_param.junc_3_front_dis_line_param;
		car_logo_rect = p_avm_param->p_avm_config_param->avm_2d_param.junc_view_3_front_param.result_3_front_param.car_logo_rect;
		pixel_row_dis = (junc_view_3_front_param.common_config.front_view_length + junc_view_3_front_param.common_config.car_head_length)
								/ (Float32_t)(junc_view_3_front_param.result_3_front_param.result_image_height);//calculate every pixel distance
		pixel_col_dis = (p_avm_param->p_avm_config_param->vehicle_param.vehicle_width + junc_view_3_front_param.common_config.left_view_length
							+ junc_view_3_front_param.common_config.right_view_length)
							/ (Float32_t)(junc_view_3_front_param.result_3_front_param.result_image_width);//calculate every pixel distance
	
		static_line_dis_to_car[0] = dis_line_param.static_line_param.static_line_dis_to_car[0];
		static_line_dis_to_car[1] = dis_line_param.static_line_param.static_line_dis_to_car[1];
		static_line_dis_to_car[2] = dis_line_param.static_line_param.static_line_dis_to_car[2];
		static_line_dis_to_car[3] = dis_line_param.static_line_param.static_line_dis_to_car[3];
	}
	
	if (cam_flag == 1)
	{
		Junc_View_3_Rear_Param_S junc_view_3_rear_param = p_avm_param->p_avm_config_param->avm_2d_param.junc_view_3_rear_param;
		p_lut_static_line = p_avm_param->p_avm_dis_line->p_static_dis_line_3_rear;
		dis_line_param = junc_view_3_rear_param.junc_3_rear_dis_line_param;
		car_logo_rect = p_avm_param->p_avm_config_param->avm_2d_param.junc_view_3_rear_param.result_3_rear_param.car_logo_rect;
		pixel_row_dis = (junc_view_3_rear_param.common_config.rear_view_length + junc_view_3_rear_param.common_config.car_rear_length)
			/ (Float32_t)(junc_view_3_rear_param.result_3_rear_param.result_image_height);//calculate every pixel distance
		pixel_col_dis = (p_avm_param->p_avm_config_param->vehicle_param.vehicle_width + junc_view_3_rear_param.common_config.left_view_length
			+ junc_view_3_rear_param.common_config.right_view_length)
			/ (Float32_t)(junc_view_3_rear_param.result_3_rear_param.result_image_width);//calculate every pixel distance
		car_logo_rect.y = car_logo_rect.height;

		static_line_dis_to_car[0] = -dis_line_param.static_line_param.static_line_dis_to_car[0];
		static_line_dis_to_car[1] = -dis_line_param.static_line_param.static_line_dis_to_car[1];
		static_line_dis_to_car[2] = -dis_line_param.static_line_param.static_line_dis_to_car[2];
		static_line_dis_to_car[3] = -dis_line_param.static_line_param.static_line_dis_to_car[3];
	}


	int add_point_shift = 1;


#if DEBUG_INFO
	CvSize size0 = cvSize(result_param.result_image_width, result_param.result_image_height);//
	IplImage *image_result_big_lut = cvCreateImage(size0, IPL_DEPTH_8U, 1);
	memset(image_result_big_lut->imageData, 0
		, result_param.result_image_width*result_param.result_image_height);
#endif
	/*********画前车道************/
	int key_ponit_pic_coordinate[16][2];
	Float64_t key_point_world_coordinate[3] = { 0, 0, 0 };//2000mm
	
	
	/**********计算16个关键点*******/
	awb_3_view_calculate_key_point_coordinate(key_ponit_pic_coordinate,
		key_point_world_coordinate,
		car_length_half,
		p_avm_param,
		car_logo_rect,
		static_line_dis_to_car,
		pixel_row_dis,
		pixel_col_dis);
	/*****************************/
	/*****draw 横线部分 ****/
	front_rear_draw_across_line(key_ponit_pic_coordinate, &add_point_shift, p_lut_static_line, cam_flag);
	/*****画竖线部分 ****/
	if (cam_flag == 1)
	{
		front_rear_draw_vertical_line_rear(key_ponit_pic_coordinate, &add_point_shift, p_lut_static_line, cam_flag);
	}
	if (cam_flag == 0)//0是前 
	{
		front_rear_draw_vertical_line(key_ponit_pic_coordinate, &add_point_shift, p_lut_static_line, cam_flag);
	}

#if 0
	/********画4路的后车道*********/
	car_length_half = -2355.00;
	static_line_dis_to_car[0] = -dis_line_param.static_line_param.static_line_dis_to_car[0];
	static_line_dis_to_car[1] = -dis_line_param.static_line_param.static_line_dis_to_car[1];
	static_line_dis_to_car[2] = -dis_line_param.static_line_param.static_line_dis_to_car[2];
	static_line_dis_to_car[3] = -dis_line_param.static_line_param.static_line_dis_to_car[3];
	key_point_world_coordinate[0] = -p_avm_param->p_avm_config_param->vehicle_param.vehicle_width / 2.00;
	awb_4_view_calculate_key_point_coordinate(key_ponit_pic_coordinate,
		key_point_world_coordinate,
		car_length_half,
		p_avm_param,
		car_logo_rect,
		static_line_dis_to_car,
		pixel_row_dis,
		pixel_col_dis);
	/*****************************/
	/*****draw 横线部分 ****/

	front_rear_draw_across_line(key_ponit_pic_coordinate, &add_point_shift, p_lut_static_line, cam_flag);
	/*****画竖线部分 ****/
	//front_rear_draw_vertical_line(key_ponit_pic_coordinate, &add_point_shift, p_lut_static_line, cam_flag);
	front_rear_draw_vertical_line_REAR(key_ponit_pic_coordinate, &add_point_shift, p_lut_static_line, cam_flag);
	
#endif
#if DEBUG_INFO
	char result_name[MAX_ARRAY_SIZE];
	sprintf(result_name, "%s/combine_overlook_lr.bmp", result_image_path);
	cvSaveImage(result_name, image_result_big_lut);
#endif
	p_lut_static_line[0] = add_point_shift;//静态表表头填充的是此表中一共多少个点
	return ret;
}








/*
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
AVM_Error_Code_E  draw_dynamic_3_view_line(IN P_Avm_Param_S p_avm_param, IN int cam_flag)
{
#if 0
	AVM_Error_Code_E ret = ERROR_NONE;
	UInt32_t *p_lut_dynamic_line = NULL;
	Result_Size_Param_S	 result_param;
	Junc_View_4_Param_S junc_view_4_param = p_avm_param->p_avm_config_param->avm_2d_param.junc_view_4_param;
	Dis_Line_Param_S dis_line_param;

	Int32_t dynamic_line_count;
	if (cam_flag == 0)
	{
		p_lut_dynamic_line = p_avm_param->p_avm_dis_line->p_dynamic_dis_line_4_big;
		result_param = p_avm_param->p_avm_config_param->avm_2d_param.junc_view_4_param.result_big_param;
		dis_line_param = junc_view_4_param.big_view_dis_line_param;

		dynamic_line_count = p_avm_param->p_avm_config_param->avm_single_view_param.front_single_view_param.dis_line_param.dynamic_line_param.dynamic_line_count;
	}
	if (cam_flag == 1)
	{
		p_lut_dynamic_line = p_avm_param->p_avm_dis_line->p_dynamic_dis_line_4_small;
		result_param = p_avm_param->p_avm_config_param->avm_2d_param.junc_view_4_param.result_small_param;
		dis_line_param = junc_view_4_param.small_view_dis_line_param;

		dynamic_line_count = p_avm_param->p_avm_config_param->avm_single_view_param.rear_single_view_param.dis_line_param.dynamic_line_param.dynamic_line_count;
	}
	CvSize size0 = cvSize(result_param.result_image_width, result_param.result_image_height);//
	IplImage *image_result_4_view_lut = cvCreateImage(size0, IPL_DEPTH_8U, 1);
	memset(image_result_4_view_lut->imageData, 0, result_param.result_image_width*result_param.result_image_height);

	Float64_t pixel_row_dis = (p_avm_param->p_avm_config_param->vehicle_param.vehicle_length
		+ junc_view_4_param.common_config.front_view_length + junc_view_4_param.common_config.rear_view_length)
		/ (Float32_t)(result_param.result_image_height);//calculate every pixel distance
	Float64_t pixel_col_dis = (p_avm_param->p_avm_config_param->vehicle_param.vehicle_width
		+ junc_view_4_param.common_config.left_view_length + junc_view_4_param.common_config.right_view_length)
		/ (Float32_t)(result_param.result_image_width);//calculate every pixel distance
#endif


	AVM_Error_Code_E ret = ERROR_NONE;
	UInt32_t *p_lut_dynamic_line = NULL;
	
	Dis_Line_Param_S dis_line_param;
	CvRect		car_logo_rect;
	Float64_t pixel_row_dis;
	Float64_t pixel_col_dis;
	Int32_t		dynamic_line_length;
	Int32_t		result_image_height;												// unit is pixel
	Int32_t		result_image_width;
	Int32_t  f_r_flag;
	if (cam_flag == 0)
	{
		Junc_View_3_Front_Param_S junc_view_3_front_param = p_avm_param->p_avm_config_param->avm_2d_param.junc_view_3_front_param;
		p_lut_dynamic_line = p_avm_param->p_avm_dis_line->p_dynamic_dis_line_3_front;
		
		dis_line_param = junc_view_3_front_param.junc_3_front_dis_line_param;
		car_logo_rect = p_avm_param->p_avm_config_param->avm_2d_param.junc_view_3_front_param.result_3_front_param.car_logo_rect;
		pixel_row_dis = (junc_view_3_front_param.common_config.front_view_length + junc_view_3_front_param.common_config.car_head_length)
			/ (Float32_t)(junc_view_3_front_param.result_3_front_param.result_image_height);//calculate every pixel distance
		pixel_col_dis = (p_avm_param->p_avm_config_param->vehicle_param.vehicle_width + junc_view_3_front_param.common_config.left_view_length
			+ junc_view_3_front_param.common_config.right_view_length)
			/ (Float32_t)(junc_view_3_front_param.result_3_front_param.result_image_width);//calculate every pixel distance

		dynamic_line_length = junc_view_3_front_param.junc_3_front_dis_line_param.dynamic_line_param.dynamic_line_length;
		result_image_height = junc_view_3_front_param.result_3_front_param.result_image_height;
		result_image_width = junc_view_3_front_param.result_3_front_param.result_image_width;
		f_r_flag = ROTATE_3_VIEW_FRONT;
	}
	if (cam_flag == 1)
	{
		Junc_View_3_Rear_Param_S junc_view_3_rear_param = p_avm_param->p_avm_config_param->avm_2d_param.junc_view_3_rear_param;
		p_lut_dynamic_line = p_avm_param->p_avm_dis_line->p_dynamic_dis_line_3_rear;

		dis_line_param = junc_view_3_rear_param.junc_3_rear_dis_line_param;
		car_logo_rect = p_avm_param->p_avm_config_param->avm_2d_param.junc_view_3_front_param.result_3_front_param.car_logo_rect;
		pixel_row_dis = (junc_view_3_rear_param.common_config.rear_view_length + junc_view_3_rear_param.common_config.car_rear_length)
			/ (Float32_t)(junc_view_3_rear_param.result_3_rear_param.result_image_height);//calculate every pixel distance
		pixel_col_dis = (p_avm_param->p_avm_config_param->vehicle_param.vehicle_width + junc_view_3_rear_param.common_config.left_view_length
			+ junc_view_3_rear_param.common_config.right_view_length)
			/ (Float32_t)(junc_view_3_rear_param.result_3_rear_param.result_image_width);//calculate every pixel distance
		dynamic_line_length = junc_view_3_rear_param.junc_3_rear_dis_line_param.dynamic_line_param.dynamic_line_length;

		result_image_height = junc_view_3_rear_param.result_3_rear_param.result_image_height;
		result_image_width = junc_view_3_rear_param.result_3_rear_param.result_image_width;
		f_r_flag = ROTATE_3_VIEW_REAR;
	}

#if DEBUG_INFO
	IplImage* image[4] = { NULL, NULL, NULL, NULL };
	ret = my_load_image(image);
	CvRect car_logo_rect = junc_view_4_param.result_big_param.car_logo_rect;
	/****malloc look up table****/
	int camera_size[4];
	for (Int32_t cam_id = 0; cam_id < CAMERA_NUM; cam_id++)
	{
		camera_size[cam_id] = junc_view_4_param.result_big_param.lut_size[cam_id] * 2;
	}

	for (int i = 0; i < result_param.result_image_height; i++)
	{
		for (int j = 0; j < result_param.result_image_width; j++)
		{
			/************front view*******************************/
			if (i < car_logo_rect.y)
			{
				int shift = i * junc_view_4_param.result_big_param.result_image_width * 2 + j * 2;
				int rgb[3];
				resolve_data_from_lut(shift, image[0], rgb, p_avm_param);
				image_result_4_view_lut->imageData[i * result_param.result_image_width + j] += (uchar)(rgb[0]);
			}
			/*************rear view*********************************/

			if ((i >= (car_logo_rect.y + car_logo_rect.height)) && (i < junc_view_4_param.result_big_param.result_image_height))
			{
				int shift = camera_size[0] + (i - (car_logo_rect.y + car_logo_rect.height)) * junc_view_4_param.result_big_param.result_image_width * 2 + j * 2;
				int rgb[3];
				resolve_data_from_lut(shift, image[1], rgb, p_avm_param);
				image_result_4_view_lut->imageData[i * result_param.result_image_width + j] += (uchar)(rgb[0]);

			}
			/*******************************************left view********************************/
			if (j < car_logo_rect.x)
			{
				int shift = camera_size[0] + camera_size[1] + i * car_logo_rect.x * 2 + j * 2;
				int rgb[3];
				resolve_data_from_lut(shift, image[2], rgb, p_avm_param);
				image_result_4_view_lut->imageData[i * result_param.result_image_width + j] += (uchar)(rgb[0]);
			}
			/*************************right view****************/
			if (j >= (car_logo_rect.x + car_logo_rect.width) && j < junc_view_4_param.result_big_param.result_image_width)
			{
				int shift = camera_size[0] + camera_size[1] + camera_size[2]
					+ i * (junc_view_4_param.result_big_param.result_image_width - car_logo_rect.x - car_logo_rect.width) * 2
					+ (j - car_logo_rect.x - car_logo_rect.width) * 2;
				int rgb[3];
				resolve_data_from_lut(shift, image[3], rgb, p_avm_param);
				image_result_4_view_lut->imageData[i * result_param.result_image_width + j] += (uchar)(rgb[0]);
			}
		}

	}


#endif
	/*********draw dynamic line***********/


	Float64_t cam_ray[3];
	for (Float64_t rotate_angle = -30; rotate_angle <30; rotate_angle++)
	{
		/*calculate_static_rotate_coordinate_rear(p_avm_param, junc_view_4_param,
		rotate_angle, cam_flag, image_result_4_view_lut, p_lut_dynamic_line, 0);*/
		revolve_one_trail_from_one_angle(f_r_flag, p_avm_param,
			dynamic_line_length,
			rotate_angle, pixel_row_dis, pixel_col_dis,
			cam_flag, cam_ray, p_lut_dynamic_line,
			result_image_height,
			result_image_width);
	}

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
AVM_Error_Code_E  plane_front_rear_single_view_calculate_key_point_coordinate(IN P_Avm_Param_S p_avm_param, IN  int cam_flag, IN Int32_t key_ponit_pic_coordinate[16][2])
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
	Float64_t fov_h = (single_view_param.plane_expand_param.fov_h*PI) / 180;
	Float64_t fov_v = (single_view_param.plane_expand_param.fov_v*PI) / 180;
	Int32_t result_width = single_view_param.result_image_width;
	Int32_t result_height = single_view_param.result_image_height;

	// 虚拟展开图中每个像素在高宽方向所代表的实际距离
	Float64_t	mm_per_pixel_h = focal_length * tan(fov_h * 0.5) * 2 / result_width;
	Float64_t  mm_per_pixel_v = focal_length * tan(fov_v * 0.5) * 2 / result_height;
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
	Float64_t pitch_angle = (38.6666*PI) / 180;//此处配置相机上下俯仰角

	Float64_t init_world_coord_x = pose_t[0] - is_front_cam * mm_per_pixel_h * result_width * 0.5;
	Float64_t init_world_coord_y = pose_t[1] + is_front_cam * (focal_length * cos(pitch_angle) + mm_per_pixel_v * result_height * 0.5 * sin(pitch_angle));
	Float64_t init_world_coord_z = pose_t[2] - focal_length * sin(pitch_angle) + mm_per_pixel_v * result_height * 0.5 * cos(pitch_angle);
	Float64_t delta_x = mm_per_pixel_h;
	Float64_t delta_y = mm_per_pixel_v * sin(pitch_angle);
	Float64_t delta_z = mm_per_pixel_v * cos(pitch_angle);
	Float64_t world_coordinate[3];
	Float64_t image_point[2];


	world_coordinate[0] = 0;
	world_coordinate[1] = p_avm_param->p_avm_config_param->avm_single_view_param.front_single_view_param.dis_line_param.static_line_param.static_line_dis_to_car[0];
	world_coordinate[2] = 0;
	Float64_t image_point_400mm[2];
	world_coordinate_to_pic_coordinate(CAM_FRONT, world_coordinate, image_point_400mm, p_avm_param);//0 is front

	world_coordinate[0] = 0;
	world_coordinate[1] = p_avm_param->p_avm_config_param->avm_single_view_param.front_single_view_param.dis_line_param.static_line_param.static_line_dis_to_car[1];
	world_coordinate[2] = 0;
	Float64_t image_point_1000mm[2];
	world_coordinate_to_pic_coordinate(CAM_FRONT, world_coordinate, image_point_1000mm, p_avm_param);//0 is front

	world_coordinate[0] = 0;
	world_coordinate[1] = p_avm_param->p_avm_config_param->avm_single_view_param.front_single_view_param.dis_line_param.static_line_param.static_line_dis_to_car[2];
	world_coordinate[2] = 0;
	Float64_t image_point_2000mm[2];
	world_coordinate_to_pic_coordinate(CAM_FRONT, world_coordinate, image_point_2000mm, p_avm_param);//0 is front

	world_coordinate[0] = 0;
	world_coordinate[1] = p_avm_param->p_avm_config_param->avm_single_view_param.front_single_view_param.dis_line_param.static_line_param.static_line_dis_to_car[3];
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
			if (floor(image_point_400mm[0]) == floor(image_point[0]) && floor(image_point_400mm[1]) == floor(image_point[1]))
			{
				image_400mm[0] = i;
				image_400mm[1] = j;
			}
			if (floor(image_point_1000mm[0]) == floor(image_point[0]) && floor(image_point_1000mm[1]) == floor(image_point[1]))
			{
				image_1000mm[0] = i;
				image_1000mm[1] = j;
			}
			if (floor(image_point_2000mm[0]) == floor(image_point[0]) && floor(image_point_2000mm[1]) == floor(image_point[1]))
			{
				image_2000mm[0] = i;
				image_2000mm[1] = j;
			}

			if (floor(image_point_2500mm[0]) == floor(image_point[0]) && floor(image_point_2500mm[1]) == floor(image_point[1]))
			{
				image_2500mm[0] = i;
				image_2500mm[1] = j;
			}



			//UInt32_t  shift_altogether = i*single_view_param.result_image_width * 2 + j * 2;
			//single_view_zip_data(shift_altogether, image_point, p_lut, cam_flag);

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
	sprintf(result_name, "%s/前视图展开66666666.bmp", result_image_path);
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
//#define DEBUG_INFO 1
AVM_Error_Code_E  plane_front_rear_single_view_calculate_key_point_coordinate_one_point(IN P_Avm_Param_S p_avm_param, IN  int cam_flag, Float64_t world_coord[3], IN Int32_t key_ponit_pic_coordinate[2])
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
//
//#if DEBUG_INFO
//	IplImage* image[4] = { NULL, NULL, NULL, NULL };
//	ret = my_load_image(image);
//	CHECK_ERROR(ret);
//	IplImage *image_result_single_front_lut = cvCreateImage(size0, IPL_DEPTH_8U, 1);
//	memset(image_result_single_front_lut->imageData, 0, single_view_param.result_image_width*single_view_param.result_image_height);
//#endif

	Float64_t focal_length = 1000; // unit is mm
	Float64_t fov_h = (single_view_param.plane_expand_param.fov_h*PI) / 180;
	Float64_t fov_v = (single_view_param.plane_expand_param.fov_v*PI) / 180;
	Int32_t result_width = single_view_param.result_image_width;
	Int32_t result_height = single_view_param.result_image_height;

	// 虚拟展开图中每个像素在高宽方向所代表的实际距离
	Float64_t	mm_per_pixel_h = focal_length * tan(fov_h * 0.5) * 2 / result_width;
	Float64_t  mm_per_pixel_v = focal_length * tan(fov_v * 0.5) * 2 / result_height;
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
	Float64_t pitch_angle = (8*PI) / 180;//此处配置相机上下俯仰角

	Float64_t init_world_coord_x = pose_t[0] - is_front_cam * mm_per_pixel_h * result_width * 0.5;
	Float64_t init_world_coord_y = pose_t[1] + is_front_cam * (focal_length * cos(pitch_angle) + mm_per_pixel_v * result_height * 0.5 * sin(pitch_angle));
	Float64_t init_world_coord_z = pose_t[2] - focal_length * sin(pitch_angle) + mm_per_pixel_v * result_height * 0.5 * cos(pitch_angle);
	Float64_t delta_x = mm_per_pixel_h;
	Float64_t delta_y = mm_per_pixel_v * sin(pitch_angle);
	Float64_t delta_z = mm_per_pixel_v * cos(pitch_angle);
	Float64_t world_coordinate[3];
	Float64_t image_point[2];

	

	//直线参数
	Float64_t  m1 = pose_t[0], m2 = pose_t[1], m3 = pose_t[2];//直线过一个定点即 相机在世界坐标系的坐标
	Float64_t  v1 = pose_t[0] - world_coord[0], v2 = pose_t[1] - world_coord[1], v3 = pose_t[2] - world_coord[2];//直线的方向向量

	//平面参数
	//配置平面的定点，如果相机上下俯仰则定点也上下俯仰，此处选取的顶点是相机+虚拟焦距
	Float64_t n1 = pose_t[0], n2 , n3 ;
	n2 = (pose_t[1] + focal_length - pose_t[1])*cos(-pitch_angle) //坐标旋转公式
		- (pose_t[2] - pose_t[2])*sin(-pitch_angle)
		+ pose_t[1];///is y
	n3 = (pose_t[1] + focal_length - pose_t[1])*sin(-pitch_angle)
		+ (pose_t[2] - pose_t[2])*cos(-pitch_angle)
		+ +pose_t[2];//is z

	Float64_t	vp1 = 0, vp2 = cos(pitch_angle), vp3 = -sin(pitch_angle);//配置 平面的方向向量


	Float64_t t=((n1-m1)*vp1+(n2-m2)*vp2+(n3-m3)*vp3)/(vp1*v1+vp2*v2+vp3*v3);//以直线的参数方程和平面的点发方程求出直线与平面的坐标

	Float64_t x_intersection=m1+v1*t;//求出直线在平面上的交点 
	Float64_t y_intersection=m2+v2*t;
	Float64_t z_intersection=m3+v3*t;


	key_ponit_pic_coordinate[1] = (x_intersection - init_world_coord_x) / delta_x;
	key_ponit_pic_coordinate[0] = (init_world_coord_z - z_intersection) / delta_z;//在此将世界坐标系的距离映射成平面展开的像素坐标
	
	return ret;
}






