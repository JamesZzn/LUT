#include"resolve_data.h"

/*
* Function Name:				my_load_image
* Function Function:			load several image
* Input:
*     image[4]:			this is load four source image
* Note:
*     the interface function should be called at the beginning of this module
* Revision:
*     Create by Cheng.Li on 2018/1/12.
*/
AVM_Error_Code_E my_load_image(OUT IplImage* image[4])
{
	AVM_Error_Code_E ret = ERROR_NONE;
	for (int cam_id = 0; cam_id < 4; cam_id++)
	{
		char image_path[MAX_ARRAY_SIZE];
		sprintf(image_path, "%s/%s.bmp",
			source_image_path, camera_prefix_array[cam_id]);
		image[cam_id] = cvLoadImage(image_path);
		if (image[cam_id] == 0)
		{
			ret = ERROR_LOAD_IMAGE_FAIL;
			return ret;
		}
	}
	return ret;

}


/***************resolve 4 pass view data from LUT*******************/
/*
* Function Name:				resolve_data_from_lut
* Function Function:			resolve data from lut
* Input:
*       shift;                the position of data in LUT 
*		image;                source image
*       rgb;                  get value of pixel in source image
* Note:
*
* Revision:
*     Create by Cheng.Li on 2018/1/12.
*/
void resolve_data_from_lut(IN int shift, IN IplImage *image, OUT int *rgb,IN  P_Avm_Param_S p_avm_param)
{
	Float64_t image_point0[2];
	unsigned int temp0 = p_avm_param->p_avm_lut->p_lut_junc_view_4_big[shift];
	unsigned int temp1 = p_avm_param->p_avm_lut->p_lut_junc_view_4_big[shift + 1];
	int u_int = temp0 >> 21;// v_int = (int)image_point[1];
	int v_int = temp0 >> 10;
	v_int = v_int & 0x7FF;
	Float64_t u_point = (Float64_t)(temp1 >> 24) / 255.000, v_point = ((Float64_t)((temp1 >> 16) & 255)) / 255.000;
	image_point0[0] = (Float64_t)u_int + u_point;
	image_point0[1] = (Float64_t)v_int + v_point;
	unsigned int weight_int = temp0 >> 2;
	weight_int = weight_int & 0x00FF;
	Float64_t weight_Float64_t = Float64_t(weight_int) / (Float64_t)255;
	bilinear_interpolation(image, rgb, image_point0[1], image_point0[0]);
	rgb[0] = (uchar)(Float64_t(rgb[0])*weight_Float64_t);

}
/***************resolve 4 pass view data from LUT*******************/
/*
* Function Name:				resolve_data_from_lut
* Function Function:			resolve data from lut
* Input:
*       shift;                the position of data in LUT
*		image;                source image
*       rgb;                  get value of pixel in source image
* Note:
*
* Revision:
*     Create by Cheng.Li on 2018/1/12.
*/
void resolve_data_from_lut_line(IN int shift, OUT int *rgb, IN  P_Avm_Param_S p_avm_param,UInt32_t *p_lut_line,UInt32_t *pic_coordinate)
{
	Float64_t image_point0[2];
	unsigned int temp0 = p_lut_line[shift+1];
	pic_coordinate[0] = temp0 >> 21;// v_int = (int)image_point[1];
	pic_coordinate[1] = (temp0 >> 10) - ((temp0 >> 21)<<11);
	int colar_flag = temp0 >> 2;
	
	rgb[0] = 255;

}



AVM_Error_Code_E resolve_4_view_data_from_LUT(IN  P_Avm_Param_S p_avm_param, IN int small_big)
{
	AVM_Error_Code_E ret = ERROR_NONE;

	CvSize size0;
	UInt32_t *p_lut_static_line = NULL;
	UInt32_t *p_lut_dynamic_line = NULL;
	Left_Right_Single_View_Param_S left_right_single_view_param;
	int result_image_height;
	int result_image_width;
	CvRect car_logo_rect ;
	if (small_big == 0)
	{
		//single_view_param_front_rear = p_avm_param->p_avm_config_param->avm_single_view_param.front_single_view_param;
		
		//p_lut = p_avm_param->p_avm_lut->p_lut_junc_view_4_big;
		p_lut_static_line = p_avm_param->p_avm_dis_line->p_static_dis_line_4_big;
		p_lut_dynamic_line = p_avm_param->p_avm_dis_line->p_dynamic_dis_line_4_big;
		result_image_height = p_avm_param->p_avm_config_param->avm_2d_param.junc_view_4_param.result_big_param.result_image_height;
		result_image_width = p_avm_param->p_avm_config_param->avm_2d_param.junc_view_4_param.result_big_param.result_image_width;
		size0 = cvSize(result_image_width, result_image_height);//
		car_logo_rect = p_avm_param->p_avm_config_param->avm_2d_param.junc_view_4_param.result_big_param.car_logo_rect;
	}
	if (small_big == 1)
	{
		
		result_image_height = p_avm_param->p_avm_config_param->avm_2d_param.junc_view_4_param.result_small_param.result_image_height;
		result_image_width = p_avm_param->p_avm_config_param->avm_2d_param.junc_view_4_param.result_small_param.result_image_width;
		size0 = cvSize(result_image_width, result_image_height);//
		//p_lut = p_avm_param->p_avm_lut->p_lut_junc_view_4_small;
		p_lut_static_line = p_avm_param->p_avm_dis_line->p_static_dis_line_4_small;
		p_lut_dynamic_line = p_avm_param->p_avm_dis_line->p_dynamic_dis_line_4_small;
		car_logo_rect = p_avm_param->p_avm_config_param->avm_2d_param.junc_view_4_param.result_small_param.car_logo_rect;
	}

	IplImage* image[4] = { NULL, NULL, NULL, NULL };
	ret = my_load_image(image);
	IplImage *image_result_lut = cvCreateImage(size0, IPL_DEPTH_8U, 1);
	memset(image_result_lut->imageData, 0, result_image_height * result_image_width);
	
	/****malloc look up table****/
	int camera_size[4];
	for (Int32_t cam_id = 0; cam_id < CAMERA_NUM; cam_id++)
	{
		camera_size[cam_id] = p_avm_param->p_avm_config_param->avm_2d_param.junc_view_4_param.result_big_param.lut_size[cam_id] * 2;
	}

	for (int i = 0; i < result_image_height; i++)
	{
		for (int j = 0; j < result_image_width; j++)
		{
#if 1
			/************front view*******************************/
			if (i < car_logo_rect.y)
			{
				int shift = i * result_image_width * 2 + j * 2;
				int rgb[3];
				//UInt32_t mn=p_avm_param->p_avm_lut->p_lut_awb[shift];
				resolve_data_from_lut(shift, image[0], rgb, p_avm_param);
				image_result_lut->imageData[i * result_image_width + j] += (uchar)(rgb[0]);

			}
			/*************rear view*********************************/

			if ((i >= (car_logo_rect.y + car_logo_rect.height)) && (i < result_image_height))
			{
				int shift = camera_size[0] + (i - (car_logo_rect.y + car_logo_rect.height)) * result_image_width * 2 + j * 2;
				int rgb[3];
				resolve_data_from_lut(shift, image[1], rgb, p_avm_param);
				image_result_lut->imageData[i * result_image_width + j] += (uchar)(rgb[0]);
			}

			/*******************************************left view********************************/
			if (j < car_logo_rect.x)
			{
				int shift = camera_size[0] + camera_size[1] + i * car_logo_rect.x * 2 + j * 2;
				int rgb[3];
				resolve_data_from_lut(shift, image[2], rgb, p_avm_param);
				image_result_lut->imageData[i * result_image_width + j] += (uchar)(rgb[0]);

			}
#endif
#if 1
			/*************************right view****************/
			if (j >= (car_logo_rect.x + car_logo_rect.width) && j < result_image_width)
			{
				int shift = camera_size[0] + camera_size[1] + camera_size[2] 
					+ i * (result_image_width - car_logo_rect.x - car_logo_rect.width) * 2 
					+ (j - car_logo_rect.x - car_logo_rect.width) * 2;
				int rgb[3];
				resolve_data_from_lut(shift, image[3], rgb, p_avm_param);
				image_result_lut->imageData[i * result_image_width + j] += (uchar)(rgb[0]);

			}
#endif
		}

	}
#if 0
	///*********解析静态表******/
	int n = p_lut_static_line[0];
	for (int i = 1; i < p_lut_static_line[0]; i++)
	{
		UInt32_t pic_coordinate[2];
		int rgb[3];
		resolve_data_from_lut_line(i, rgb, p_avm_param, p_lut_static_line, pic_coordinate);

		image_result_lut->imageData[pic_coordinate[0] *result_image_width + pic_coordinate[1]] = (uchar)(rgb[0]);

	}

	/*********解析动态表******/   /**注意如果解析动态表出错产生中断错误考虑一下前后的视野范围要调大一下，以免距离线超出到图像范围之外产生段错误，越界错误**/
	for (int m = 50; m < 51; m++)
	{
		int n = p_lut_dynamic_line[m];
		for (int i = p_lut_dynamic_line[m]; i < p_lut_dynamic_line[m + 1]; i++)
		{
			UInt32_t pic_coordinate[2];
			int rgb[3];
			resolve_data_from_lut_line(i, rgb, p_avm_param, p_lut_dynamic_line, pic_coordinate);

			image_result_lut->imageData[pic_coordinate[0] * result_image_width + pic_coordinate[1]] = (uchar)(rgb[0]);
		}
	}
#endif

	char result_name[MAX_ARRAY_SIZE];
	sprintf(result_name, "%s/4_view%d.bmp", result_image_path,small_big);
	cvSaveImage(result_name, image_result_lut);

	return ret;
}


#if 0
AVM_Error_Code_E ret = ERROR_NONE;

Junc_View_4_Param_S junc_view_4_param = p_avm_param->p_avm_config_param->avm_2d_param.junc_view_4_param;

CvSize size0 = cvSize(junc_view_4_param.result_big_param.result_image_width, junc_view_4_param.result_big_param.result_image_height);//

IplImage *image_result_big_lut = cvCreateImage(size0, IPL_DEPTH_8U, 1);
memset(image_result_big_lut->imageData, 0, junc_view_4_param.result_big_param.result_image_height * junc_view_4_param.result_big_param.result_image_width);
IplImage *image_result_small_lut = cvCreateImage(size0, IPL_DEPTH_8U, 1);
memset(image_result_small_lut->imageData, 0, junc_view_4_param.result_big_param.result_image_height * junc_view_4_param.result_big_param.result_image_width);

CvRect car_logo_rect = junc_view_4_param.result_big_param.car_logo_rect;
/****malloc look up table****/
int camera_size[4];
for (Int32_t cam_id = 0; cam_id < CAMERA_NUM; cam_id++)
{
	camera_size[cam_id] = junc_view_4_param.result_big_param.lut_size[cam_id] * 2;
}

for (int i = 0; i < junc_view_4_param.result_big_param.result_image_height; i++)
{
	for (int j = 0; j < junc_view_4_param.result_big_param.result_image_width; j++)
	{
		/************front view*******************************/
		if (i < car_logo_rect.y)
		{
			int shift = i * junc_view_4_param.result_big_param.result_image_width * 2 + j * 2;
			int rgb[3];
			resolve_data_from_lut(shift, image[0], rgb, p_avm_param);
			image_result_big_lut->imageData[i * junc_view_4_param.result_big_param.result_image_width + j] += (uchar)(rgb[0]);
			image_result_small_lut->imageData[i * junc_view_4_param.result_small_param.result_image_width + j] += (uchar)(rgb[0]);

		}
		/*************rear view*********************************/
		if ((i >= (car_logo_rect.y + car_logo_rect.height)) && (i < junc_view_4_param.result_big_param.result_image_height))
		{
			int shift = camera_size[0] + (i - (car_logo_rect.y + car_logo_rect.height)) * junc_view_4_param.result_big_param.result_image_width * 2 + j * 2;
			int rgb[3];
			resolve_data_from_lut(shift, image[1], rgb, p_avm_param);
			image_result_big_lut->imageData[i * junc_view_4_param.result_big_param.result_image_width + j] += (uchar)(rgb[0]);
			image_result_small_lut->imageData[i * junc_view_4_param.result_small_param.result_image_width + j] += (uchar)(rgb[0]);
		}
		/*******************************************left view********************************/
		if (j < car_logo_rect.x)
		{
			int shift = camera_size[0] + camera_size[1] + i * car_logo_rect.x * 2 + j * 2;
			int rgb[3];
			resolve_data_from_lut(shift, image[2], rgb, p_avm_param);


			image_result_big_lut->imageData[i * junc_view_4_param.result_big_param.result_image_width + j] = image_result_big_lut
				->imageData[i * junc_view_4_param.result_big_param.result_image_width + j] + (uchar)(rgb[0]);
			image_result_small_lut->imageData[i * junc_view_4_param.result_small_param.result_image_width + j] += (uchar)(rgb[0]);

		}
		/*************************right view****************/
		if (j >= (car_logo_rect.x + car_logo_rect.width) && j < junc_view_4_param.result_big_param.result_image_width)
		{
			int shift = camera_size[0] + camera_size[1] + camera_size[2]
				+ i * (junc_view_4_param.result_big_param.result_image_width - car_logo_rect.x - car_logo_rect.width) * 2
				+ (j - car_logo_rect.x - car_logo_rect.width) * 2;
			int rgb[3];
			resolve_data_from_lut(shift, image[3], rgb, p_avm_param);
			image_result_big_lut->imageData[i * junc_view_4_param.result_big_param.result_image_width + j] = image_result_big_lut
				->imageData[i * junc_view_4_param.result_big_param.result_image_width + j]
				+ (uchar)(rgb[0]);
			image_result_small_lut->imageData[i * junc_view_4_param.result_small_param.result_image_width + j] += (uchar)(rgb[0]);

		}
	}

}
#endif
#if 0
//if (cam_flag == 0)
{


	Float64_t pixel_row_dis = (p_avm_param->p_avm_config_param->vehicle_param.vehicle_length + junc_view_4_param.common_config.front_view_length + junc_view_4_param.common_config.rear_view_length)
		/ (Float32_t)(junc_view_4_param.result_big_param.result_image_height);//calculate every pixel distance
	Float64_t pixel_col_dis = (p_avm_param->p_avm_config_param->vehicle_param.vehicle_width + junc_view_4_param.common_config.left_view_length + junc_view_4_param.common_config.right_view_length)
		/ (Float32_t)(junc_view_4_param.result_big_param.result_image_width);//calculate every pixel distance




	//cam_flag = 1;
	junc_view_4_param.big_view_dis_line_param.static_line_param.static_line_dis_to_car;
	int key_ponit_pic_coordinate[16][2];
	Float64_t key_point_world_coordinate[3] = { 0, 0, 0 };//2000mm
	key_point_world_coordinate[1] = p_avm_param->p_avm_config_param->vehicle_param.vehicle_length / 2.00;
	key_point_world_coordinate[0] = -p_avm_param->p_avm_config_param->vehicle_param.vehicle_width / 2.00;
	Float64_t car_length_half;
	Int32_t	static_line_dis_to_car[4];

	car_length_half = 2355.00;
	static_line_dis_to_car[0] = junc_view_4_param.big_view_dis_line_param.static_line_param.static_line_dis_to_car[0];
	static_line_dis_to_car[1] = junc_view_4_param.big_view_dis_line_param.static_line_param.static_line_dis_to_car[1];
	static_line_dis_to_car[2] = junc_view_4_param.big_view_dis_line_param.static_line_param.static_line_dis_to_car[2];
	static_line_dis_to_car[3] = junc_view_4_param.big_view_dis_line_param.static_line_param.static_line_dis_to_car[3];
	key_point_world_coordinate[0] = -p_avm_param->p_avm_config_param->vehicle_param.vehicle_width / 2.00;


	/**************calculate left*****************/
	for (int i = 0; i < 4; i++)
	{
		key_point_world_coordinate[1] = car_length_half + static_line_dis_to_car[i];
		key_ponit_pic_coordinate[i][0] = junc_view_4_param.result_small_param.result_image_height / 2 - (int)abs((key_point_world_coordinate[1] / pixel_row_dis));

		key_ponit_pic_coordinate[i][1] = junc_view_4_param.result_small_param.result_image_width / 2 - (int)abs((key_point_world_coordinate[0] / pixel_col_dis));

	}
	key_point_world_coordinate[0] = key_point_world_coordinate[0] + LADDER_ACROSS;
	for (int i = 4; i < 8; i++)
	{
		key_point_world_coordinate[1] = car_length_half + static_line_dis_to_car[i - 4];
		key_ponit_pic_coordinate[i][0] = junc_view_4_param.result_small_param.result_image_height / 2 - (int)abs((key_point_world_coordinate[1] / pixel_row_dis));
		key_ponit_pic_coordinate[i][1] = junc_view_4_param.result_small_param.result_image_width / 2 - (int)abs((key_point_world_coordinate[0] / pixel_col_dis));

	}

	/**************calculate right*****************/

	key_point_world_coordinate[0] = p_avm_param->p_avm_config_param->vehicle_param.vehicle_width / 2.00;
	for (int i = 8; i < 12; i++)
	{
		key_point_world_coordinate[1] = car_length_half + static_line_dis_to_car[i - 8];
		key_ponit_pic_coordinate[i][0] = junc_view_4_param.result_small_param.result_image_height / 2 - (int)abs((key_point_world_coordinate[1] / pixel_row_dis));

		key_ponit_pic_coordinate[i][1] = junc_view_4_param.result_small_param.result_image_width / 2 + (int)abs((key_point_world_coordinate[0] / pixel_col_dis));

	}
	key_point_world_coordinate[0] = key_point_world_coordinate[0] - LADDER_ACROSS;
	for (int i = 12; i < 16; i++)
	{
		key_point_world_coordinate[1] = car_length_half + static_line_dis_to_car[i - 12];
		key_ponit_pic_coordinate[i][0] = junc_view_4_param.result_small_param.result_image_height / 2 - (int)abs((key_point_world_coordinate[1] / pixel_row_dis));

		key_ponit_pic_coordinate[i][1] = junc_view_4_param.result_small_param.result_image_width / 2 + (int)abs((key_point_world_coordinate[0] / pixel_col_dis));

	}



	/*****************************8/

	/*****draw left ladder vertical ****/
	for (int n = 0; n < 4; n++)
	{
		for (int i = key_ponit_pic_coordinate[n][0]; i < key_ponit_pic_coordinate[n + 1][0]; i++)
		{

			for (int j = key_ponit_pic_coordinate[n][1]; j <key_ponit_pic_coordinate[n][1] + LINE_WIDTH; j++)
			{
				image_result_big_lut->imageData[i *junc_view_4_param.result_big_param.result_image_width + j] = 255;
			}
		}
	}
	for (int n = 0; n < 4; n++)
	{
		for (int i = key_ponit_pic_coordinate[n][0]; i < key_ponit_pic_coordinate[n][0] + LINE_WIDTH; i++)
		{
			for (int j = key_ponit_pic_coordinate[n][1]; j < key_ponit_pic_coordinate[n + 4][1]; j++)
			{
				image_result_big_lut->imageData[i *junc_view_4_param.result_big_param.result_image_width + j] = 255;
			}
		}
	}

	/***********draw right**************/

	for (int n = 8; n < 12; n++)
	{
		for (int i = key_ponit_pic_coordinate[n][0]; i < key_ponit_pic_coordinate[n + 1][0]; i++)
		{

			for (int j = key_ponit_pic_coordinate[n][1]; j <key_ponit_pic_coordinate[n][1] + LINE_WIDTH; j++)
			{
				image_result_big_lut->imageData[i *junc_view_4_param.result_big_param.result_image_width + j] = 255;
			}
		}
	}
	for (int n = 8; n < 12; n++)
	{
		for (int i = key_ponit_pic_coordinate[n][0]; i < key_ponit_pic_coordinate[n][0] + LINE_WIDTH; i++)
		{
			for (int j = key_ponit_pic_coordinate[n][1]; j > key_ponit_pic_coordinate[n + 4][1]; j--)
			{
				image_result_big_lut->imageData[i *junc_view_4_param.result_big_param.result_image_width + j] = 255;
			}
		}
	}


}
//if (cam_flag == 1)
{
	Float64_t pixel_row_dis = (p_avm_param->p_avm_config_param->vehicle_param.vehicle_length + junc_view_4_param.common_config.front_view_length + junc_view_4_param.common_config.rear_view_length)
		/ (Float32_t)(junc_view_4_param.result_big_param.result_image_height);//calculate every pixel distance
	Float64_t pixel_col_dis = (p_avm_param->p_avm_config_param->vehicle_param.vehicle_width + junc_view_4_param.common_config.left_view_length + junc_view_4_param.common_config.right_view_length)
		/ (Float32_t)(junc_view_4_param.result_big_param.result_image_width);//calculate every pixel distance




	//cam_flag = 1;
	junc_view_4_param.big_view_dis_line_param.static_line_param.static_line_dis_to_car;
	int key_ponit_pic_coordinate[16][2];
	Float64_t key_point_world_coordinate[3] = { 0, 0, 0 };//2000mm
	key_point_world_coordinate[1] = p_avm_param->p_avm_config_param->vehicle_param.vehicle_length / 2.00;
	key_point_world_coordinate[0] = -p_avm_param->p_avm_config_param->vehicle_param.vehicle_width / 2.00;




	Float64_t car_length_half;
	Int32_t	static_line_dis_to_car[4];





	car_length_half = -2355.00;
	static_line_dis_to_car[0] = -junc_view_4_param.big_view_dis_line_param.static_line_param.static_line_dis_to_car[0];
	static_line_dis_to_car[1] = -junc_view_4_param.big_view_dis_line_param.static_line_param.static_line_dis_to_car[1];
	static_line_dis_to_car[2] = -junc_view_4_param.big_view_dis_line_param.static_line_param.static_line_dis_to_car[2];
	static_line_dis_to_car[3] = -junc_view_4_param.big_view_dis_line_param.static_line_param.static_line_dis_to_car[3];


	key_point_world_coordinate[0] = -p_avm_param->p_avm_config_param->vehicle_param.vehicle_width / 2.00;


	/**************calculate left*****************/
	for (int i = 0; i < 4; i++)
	{
		key_point_world_coordinate[1] = car_length_half + static_line_dis_to_car[i];
		key_ponit_pic_coordinate[i][0] = junc_view_4_param.result_small_param.result_image_height / 2 + (int)abs((key_point_world_coordinate[1] / pixel_row_dis));

		key_ponit_pic_coordinate[i][1] = junc_view_4_param.result_small_param.result_image_width / 2 - (int)abs((key_point_world_coordinate[0] / pixel_col_dis));

	}
	key_point_world_coordinate[0] = key_point_world_coordinate[0] + LADDER_ACROSS;
	for (int i = 4; i < 8; i++)
	{
		key_point_world_coordinate[1] = car_length_half + static_line_dis_to_car[i - 4];
		key_ponit_pic_coordinate[i][0] = junc_view_4_param.result_small_param.result_image_height / 2 + (int)abs((key_point_world_coordinate[1] / pixel_row_dis));

		key_ponit_pic_coordinate[i][1] = junc_view_4_param.result_small_param.result_image_width / 2 - (int)abs((key_point_world_coordinate[0] / pixel_col_dis));

	}

	/**************calculate right*****************/

	key_point_world_coordinate[0] = p_avm_param->p_avm_config_param->vehicle_param.vehicle_width / 2.00;
	for (int i = 8; i < 12; i++)
	{
		key_point_world_coordinate[1] = car_length_half + static_line_dis_to_car[i - 8];
		key_ponit_pic_coordinate[i][0] = junc_view_4_param.result_small_param.result_image_height / 2 + (int)abs((key_point_world_coordinate[1] / pixel_row_dis));

		key_ponit_pic_coordinate[i][1] = junc_view_4_param.result_small_param.result_image_width / 2 + (int)abs((key_point_world_coordinate[0] / pixel_col_dis));

	}
	key_point_world_coordinate[0] = key_point_world_coordinate[0] - LADDER_ACROSS;
	for (int i = 12; i < 16; i++)
	{
		key_point_world_coordinate[1] = car_length_half + static_line_dis_to_car[i - 12];
		key_ponit_pic_coordinate[i][0] = junc_view_4_param.result_small_param.result_image_height / 2 + (int)abs((key_point_world_coordinate[1] / pixel_row_dis));

		key_ponit_pic_coordinate[i][1] = junc_view_4_param.result_small_param.result_image_width / 2 + (int)abs((key_point_world_coordinate[0] / pixel_col_dis));

	}
	/*****draw left ladder vertical ****/
	for (int n = 0; n < 4; n++)
	{
		for (int i = key_ponit_pic_coordinate[n][0]; i < key_ponit_pic_coordinate[n + 1][0]; i++)
		{

			for (int j = key_ponit_pic_coordinate[n][1]; j < key_ponit_pic_coordinate[n][1] + LINE_WIDTH; j++)
			{
				image_result_big_lut->imageData[i *junc_view_4_param.result_big_param.result_image_width + j] = 255;
			}
		}
	}
	for (int n = 0; n < 4; n++)
	{
		for (int i = key_ponit_pic_coordinate[n][0]; i < key_ponit_pic_coordinate[n][0] + LINE_WIDTH; i++)
		{
			for (int j = key_ponit_pic_coordinate[n][1]; j < key_ponit_pic_coordinate[n + 4][1]; j++)
			{
				image_result_big_lut->imageData[i *junc_view_4_param.result_big_param.result_image_width + j] = 255;
			}
		}
	}

	/***********draw right**************/

	for (int n = 8; n < 12; n++)
	{
		for (int i = key_ponit_pic_coordinate[n][0]; i < key_ponit_pic_coordinate[n + 1][0]; i++)
		{

			for (int j = key_ponit_pic_coordinate[n][1]; j < key_ponit_pic_coordinate[n][1] + LINE_WIDTH; j++)
			{
				image_result_big_lut->imageData[i *junc_view_4_param.result_big_param.result_image_width + j] = 255;
			}
		}
	}
	for (int n = 8; n < 12; n++)
	{
		for (int i = key_ponit_pic_coordinate[n][0]; i < key_ponit_pic_coordinate[n][0] + LINE_WIDTH; i++)
		{
			for (int j = key_ponit_pic_coordinate[n][1]; j > key_ponit_pic_coordinate[n + 4][1]; j--)
			{
				image_result_big_lut->imageData[i *junc_view_4_param.result_big_param.result_image_width + j] = 255;
			}
		}
	}
}
#endif

#if 0
Float64_t pixel_row_dis = (p_avm_param->p_avm_config_param->vehicle_param.vehicle_length + junc_view_4_param.common_config.front_view_length + junc_view_4_param.common_config.rear_view_length)
/ (Float32_t)(junc_view_4_param.result_big_param.result_image_height);//calculate every pixel distance
Float64_t pixel_col_dis = (p_avm_param->p_avm_config_param->vehicle_param.vehicle_width + junc_view_4_param.common_config.left_view_length + junc_view_4_param.common_config.right_view_length)
/ (Float32_t)(junc_view_4_param.result_big_param.result_image_width);//calculate every pixel distance




int cam_flag = 1;
junc_view_4_param.big_view_dis_line_param.static_line_param.static_line_dis_to_car;
int key_ponit_pic_coordinate[16][2];
Float64_t key_point_world_coordinate[3] = { 0, 0, 0 };//2000mm
key_point_world_coordinate[1] = p_avm_param->p_avm_config_param->vehicle_param.vehicle_length / 2.00;
key_point_world_coordinate[0] = -p_avm_param->p_avm_config_param->vehicle_param.vehicle_width / 2.00;




Float64_t car_length_half;
Int32_t	static_line_dis_to_car[4];

if (cam_flag == 0)
{
	car_length_half = 2355.00;
	static_line_dis_to_car[0] = junc_view_4_param.big_view_dis_line_param.static_line_param.static_line_dis_to_car[0];
	static_line_dis_to_car[1] = junc_view_4_param.big_view_dis_line_param.static_line_param.static_line_dis_to_car[1];
	static_line_dis_to_car[2] = junc_view_4_param.big_view_dis_line_param.static_line_param.static_line_dis_to_car[2];
	static_line_dis_to_car[3] = junc_view_4_param.big_view_dis_line_param.static_line_param.static_line_dis_to_car[3];
	key_point_world_coordinate[0] = -p_avm_param->p_avm_config_param->vehicle_param.vehicle_width / 2.00;


	/**************calculate left*****************/
	for (int i = 0; i < 4; i++)
	{
		key_point_world_coordinate[1] = car_length_half + static_line_dis_to_car[i];
		key_ponit_pic_coordinate[i][0] = junc_view_4_param.result_small_param.result_image_height / 2 - (int)abs((key_point_world_coordinate[1] / pixel_row_dis));

		key_ponit_pic_coordinate[i][1] = junc_view_4_param.result_small_param.result_image_width / 2 - (int)abs((key_point_world_coordinate[0] / pixel_col_dis));

	}
	key_point_world_coordinate[0] = key_point_world_coordinate[0] + LADDER_ACROSS;
	for (int i = 4; i < 8; i++)
	{
		key_point_world_coordinate[1] = car_length_half + static_line_dis_to_car[i - 4];
		key_ponit_pic_coordinate[i][0] = junc_view_4_param.result_small_param.result_image_height / 2 - (int)abs((key_point_world_coordinate[1] / pixel_row_dis));
		key_ponit_pic_coordinate[i][1] = junc_view_4_param.result_small_param.result_image_width / 2 - (int)abs((key_point_world_coordinate[0] / pixel_col_dis));

	}

	/**************calculate right*****************/

	key_point_world_coordinate[0] = p_avm_param->p_avm_config_param->vehicle_param.vehicle_width / 2.00;
	for (int i = 8; i < 12; i++)
	{
		key_point_world_coordinate[1] = car_length_half + static_line_dis_to_car[i - 8];
		key_ponit_pic_coordinate[i][0] = junc_view_4_param.result_small_param.result_image_height / 2 - (int)abs((key_point_world_coordinate[1] / pixel_row_dis));

		key_ponit_pic_coordinate[i][1] = junc_view_4_param.result_small_param.result_image_width / 2 + (int)abs((key_point_world_coordinate[0] / pixel_col_dis));

	}
	key_point_world_coordinate[0] = key_point_world_coordinate[0] - LADDER_ACROSS;
	for (int i = 12; i < 16; i++)
	{
		key_point_world_coordinate[1] = car_length_half + static_line_dis_to_car[i - 12];
		key_ponit_pic_coordinate[i][0] = junc_view_4_param.result_small_param.result_image_height / 2 - (int)abs((key_point_world_coordinate[1] / pixel_row_dis));

		key_ponit_pic_coordinate[i][1] = junc_view_4_param.result_small_param.result_image_width / 2 + (int)abs((key_point_world_coordinate[0] / pixel_col_dis));

	}

}
if (cam_flag == 1)
{
	car_length_half = -2355.00;
	static_line_dis_to_car[0] = -junc_view_4_param.big_view_dis_line_param.static_line_param.static_line_dis_to_car[0];
	static_line_dis_to_car[1] = -junc_view_4_param.big_view_dis_line_param.static_line_param.static_line_dis_to_car[1];
	static_line_dis_to_car[2] = -junc_view_4_param.big_view_dis_line_param.static_line_param.static_line_dis_to_car[2];
	static_line_dis_to_car[3] = -junc_view_4_param.big_view_dis_line_param.static_line_param.static_line_dis_to_car[3];


	key_point_world_coordinate[0] = -p_avm_param->p_avm_config_param->vehicle_param.vehicle_width / 2.00;


	/**************calculate left*****************/
	for (int i = 0; i < 4; i++)
	{
		key_point_world_coordinate[1] = car_length_half + static_line_dis_to_car[i];
		key_ponit_pic_coordinate[i][0] = junc_view_4_param.result_small_param.result_image_height / 2 + (int)abs((key_point_world_coordinate[1] / pixel_row_dis));

		key_ponit_pic_coordinate[i][1] = junc_view_4_param.result_small_param.result_image_width / 2 - (int)abs((key_point_world_coordinate[0] / pixel_col_dis));

	}
	key_point_world_coordinate[0] = key_point_world_coordinate[0] + LADDER_ACROSS;
	for (int i = 4; i < 8; i++)
	{
		key_point_world_coordinate[1] = car_length_half + static_line_dis_to_car[i - 4];
		key_ponit_pic_coordinate[i][0] = junc_view_4_param.result_small_param.result_image_height / 2 + (int)abs((key_point_world_coordinate[1] / pixel_row_dis));

		key_ponit_pic_coordinate[i][1] = junc_view_4_param.result_small_param.result_image_width / 2 - (int)abs((key_point_world_coordinate[0] / pixel_col_dis));

	}

	/**************calculate right*****************/

	key_point_world_coordinate[0] = p_avm_param->p_avm_config_param->vehicle_param.vehicle_width / 2.00;
	for (int i = 8; i < 12; i++)
	{
		key_point_world_coordinate[1] = car_length_half + static_line_dis_to_car[i - 8];
		key_ponit_pic_coordinate[i][0] = junc_view_4_param.result_small_param.result_image_height / 2 + (int)abs((key_point_world_coordinate[1] / pixel_row_dis));

		key_ponit_pic_coordinate[i][1] = junc_view_4_param.result_small_param.result_image_width / 2 + (int)abs((key_point_world_coordinate[0] / pixel_col_dis));

	}
	key_point_world_coordinate[0] = key_point_world_coordinate[0] - LADDER_ACROSS;
	for (int i = 12; i < 16; i++)
	{
		key_point_world_coordinate[1] = car_length_half + static_line_dis_to_car[i - 12];
		key_ponit_pic_coordinate[i][0] = junc_view_4_param.result_small_param.result_image_height / 2 + (int)abs((key_point_world_coordinate[1] / pixel_row_dis));

		key_ponit_pic_coordinate[i][1] = junc_view_4_param.result_small_param.result_image_width / 2 + (int)abs((key_point_world_coordinate[0] / pixel_col_dis));

	}


}

/*****draw left ladder vertical ****/
for (int n = 0; n < 4; n++)
{
	for (int i = key_ponit_pic_coordinate[n][0]; i < key_ponit_pic_coordinate[n + 1][0]; i++)
	{

		for (int j = key_ponit_pic_coordinate[n][1]; j <key_ponit_pic_coordinate[n][1] + LINE_WIDTH; j++)
		{
			image_result_big_lut->imageData[i *junc_view_4_param.result_big_param.result_image_width + j] = 255;
		}
	}
}
for (int n = 0; n < 4; n++)
{
	for (int i = key_ponit_pic_coordinate[n][0]; i < key_ponit_pic_coordinate[n][0] + LINE_WIDTH; i++)
	{
		for (int j = key_ponit_pic_coordinate[n][1]; j < key_ponit_pic_coordinate[n + 4][1]; j++)
		{
			image_result_big_lut->imageData[i *junc_view_4_param.result_big_param.result_image_width + j] = 255;
		}
	}
}

/***********draw right**************/

for (int n = 8; n < 12; n++)
{
	for (int i = key_ponit_pic_coordinate[n][0]; i < key_ponit_pic_coordinate[n + 1][0]; i++)
	{

		for (int j = key_ponit_pic_coordinate[n][1]; j <key_ponit_pic_coordinate[n][1] + LINE_WIDTH; j++)
		{
			image_result_big_lut->imageData[i *junc_view_4_param.result_big_param.result_image_width + j] = 255;
		}
	}
}
for (int n = 8; n < 12; n++)
{
	for (int i = key_ponit_pic_coordinate[n][0]; i < key_ponit_pic_coordinate[n][0] + LINE_WIDTH; i++)
	{
		for (int j = key_ponit_pic_coordinate[n][1]; j > key_ponit_pic_coordinate[n + 4][1]; j--)
		{
			image_result_big_lut->imageData[i *junc_view_4_param.result_big_param.result_image_width + j] = 255;
		}
	}
}


#if 0
int pic_ponit_0[8][2];
int  pic_ponit_1[2];
/***************draw left ladder*****************/
Float64_t world_coordinate[3] = { -900, 2355, 0 };//2000mm
int flag0 = 0;
int i = 0;
for (i = 0; i < 4; i++)
{
	world_coordinate[1] = 2355 + single_view_param.dis_line_param.static_line_param.static_line_dis_to_car[3 - i];
	calculate_dis(p_avm_param, single_view_param, world_coordinate, image_result_single_front_lut, cam_flag,
		pixel_hight_dis, pixel_width_dis, cam_ray, 0, pic_ponit_1);
	pic_ponit_0[i][0] = pic_ponit_1[0];
	pic_ponit_0[i][1] = pic_ponit_1[1];
}
world_coordinate[0] = 900;
for (; i < 8; i++)
{
	world_coordinate[1] = 2355 + single_view_param.dis_line_param.static_line_param.static_line_dis_to_car[7 - i];
	calculate_dis(p_avm_param, single_view_param, world_coordinate, image_result_single_front_lut, cam_flag,
		pixel_hight_dis, pixel_width_dis, cam_ray, 1, pic_ponit_1);
	pic_ponit_0[i][0] = pic_ponit_1[0];
	pic_ponit_0[i][1] = pic_ponit_1[1];
}


Float64_t a = abs(Float64_t(pic_ponit_0[0][1] - pic_ponit_0[1][1]) / Float64_t(pic_ponit_0[0][0] - pic_ponit_0[1][0]));
Float64_t b = abs(Float64_t(pic_ponit_0[4][1] - pic_ponit_0[5][1]) / Float64_t(pic_ponit_0[4][0] - pic_ponit_0[5][0]));

Float64_t temp_0 = pic_ponit_0[0][1];
for (int n = 0; n < 4; n++)
{
	for (int i = pic_ponit_0[n][0]; i < pic_ponit_0[n + 1][0]; i++)
	{
		temp_0 = temp_0 - a;
		for (int j = temp_0; j < temp_0 + LINE_WIDTH; j++)
		{
			image_result_single_front_lut->imageData[i *single_view_param.result_image_width + j] = 255;
		}
	}
}

temp_0 = pic_ponit_0[4][1];
for (int n = 4; n < 8; n++)
{
	for (int i = pic_ponit_0[n][0]; i < pic_ponit_0[n + 1][0]; i++)
	{
		temp_0 = temp_0 + b;
		for (int j = temp_0; j < temp_0 + LINE_WIDTH; j++)
		{
			image_result_single_front_lut->imageData[i *single_view_param.result_image_width + j] = 255;
		}
	}
}
#endif



#endif


#if 0
int pic_ponit_0[8][2];
int  pic_ponit_1[2];
/***************draw left ladder*****************/
Float64_t world_coordinate_8[3] = { -900, 4355, 0 };//2000mm


Float64_t pixel_row_dis = (p_avm_param->p_avm_config_param->vehicle_param.vehicle_length + junc_view_4_param.common_config.front_view_length + junc_view_4_param.common_config.rear_view_length)
/ (Float32_t)(junc_view_4_param.result_big_param.result_image_height);//calculate every pixel distance
Float64_t pixel_col_dis = (p_avm_param->p_avm_config_param->vehicle_param.vehicle_width + junc_view_4_param.common_config.left_view_length + junc_view_4_param.common_config.right_view_length)
/ (Float32_t)(junc_view_4_param.result_big_param.result_image_width);//calculate every pixel distance

int pic_w = junc_view_4_param.result_small_param.result_image_width / 2 - (int)abs((world_coordinate_8[0] / pixel_col_dis));
int pic_h = junc_view_4_param.result_small_param.result_image_height / 2 - (int)abs((world_coordinate_8[1] / pixel_row_dis));


//pic_w  = int(world_coordinate_8[0] / pixel_col_dis) + (car_logo_rect.x + (car_logo_rect.width / 2)) ;
//world_coordinate[1] = (Float64_t)(pixel_row_dis)*(Float64_t)((Float64_t)((car_logo_rect.y + (car_logo_rect.height / 2))) - (Float64_t)i);
//pic_h = (car_logo_rect.y + (car_logo_rect.height / 2)) - int(world_coordinate_8[1] / pixel_row_dis);

for (int j = 0; j < junc_view_4_param.result_big_param.result_image_width; j++)
{
	image_result_big_lut->imageData[pic_h* junc_view_4_param.result_small_param.result_image_width + j] = 255;

}

world_coordinate_8[1] = 3355;//2000mm

pic_h = junc_view_4_param.result_small_param.result_image_height / 2 - (int)abs((world_coordinate_8[1] / pixel_row_dis));

for (int j = 0; j < junc_view_4_param.result_big_param.result_image_width; j++)
{
	image_result_big_lut->imageData[pic_h* junc_view_4_param.result_small_param.result_image_width + j] = 255;

}

world_coordinate_8[1] = 2455;//2000mm

pic_h = junc_view_4_param.result_small_param.result_image_height / 2 - (int)abs((world_coordinate_8[1] / pixel_row_dis));

for (int j = 0; j < junc_view_4_param.result_big_param.result_image_width; j++)
{
	image_result_big_lut->imageData[pic_h* junc_view_4_param.result_small_param.result_image_width + j] = 255;

}

world_coordinate_8[1] = 2755;//2000mm

pic_h = junc_view_4_param.result_small_param.result_image_height / 2 - (int)abs((world_coordinate_8[1] / pixel_row_dis));

for (int j = 0; j < junc_view_4_param.result_big_param.result_image_width; j++)
{
	image_result_big_lut->imageData[pic_h* junc_view_4_param.result_small_param.result_image_width + j] = 255;

}


#endif




/***************resolve 3 front view data from LUT*******************/

/*
* Function Name:				_3_front_resolve_data_from_lut
* Function Function:			_3_front resolve data_from_lut
* Input:
*       shift;                the position of data in LUT
*		image;                source image
*       rgb;                  get value of pixel in source image
		p_avm_param;
* Note:
*
* Revision:
*     Create by Cheng.Li on 2018/1/17.
*/
	void _3_front_resolve_data_from_lut(IN int shift, IN IplImage *image, OUT int *rgb, IN  P_Avm_Param_S p_avm_param)
{
	Float64_t image_point0[2];
	unsigned int temp0 = p_avm_param->p_avm_lut->p_lut_junc_view_3_front[shift];
	unsigned int temp1 = p_avm_param->p_avm_lut->p_lut_junc_view_3_front[shift + 1];
	int u_int = temp0 >> 21;// v_int = (int)image_point[1];
	int v_int = temp0 >> 10;
	v_int = v_int & 0x7FF;
	Float64_t u_point = (Float64_t)(temp1 >> 24) / 255.000, v_point = ((Float64_t)((temp1 >> 16) & 255)) / 255.000;
	image_point0[0] = (Float64_t)u_int + u_point;
	image_point0[1] = (Float64_t)v_int + v_point;
	unsigned int weight_int = temp0 >> 2;
	weight_int = weight_int & 0x00FF;
	Float64_t weight_Float64_t = Float64_t(weight_int) / (Float64_t)255;
	bilinear_interpolation(image, rgb, image_point0[1], image_point0[0]);
	rgb[0] = (uchar)(Float64_t(rgb[0])*weight_Float64_t);

}

/*
* Function Name:				_3_front_resolve_data_from_lut
* Function Function:			_3_front resolve data_from_lut
* Input:
*       shift;                the position of data in LUT
*		image;                source image
*       rgb;                  get value of pixel in source image
p_avm_param;
* Note:
*
* Revision:
*     Create by Cheng.Li on 2018/1/17.
*/
AVM_Error_Code_E resolve_3_front_data_from_LUT(IN  P_Avm_Param_S p_avm_param)
{
	AVM_Error_Code_E ret = ERROR_NONE;
	IplImage* image[4] = { NULL, NULL, NULL, NULL };

	ret = my_load_image(image);

	Junc_View_3_Front_Param_S	junc_view_3_front_param = p_avm_param->p_avm_config_param->avm_2d_param.junc_view_3_front_param;
	CvRect car_logo_rect = junc_view_3_front_param.result_3_front_param.car_logo_rect;
	Vehicle_Param_S	 vehicle_param = p_avm_param->p_avm_config_param->vehicle_param;
	Float64_t pixel_row_dis = (vehicle_param.vehicle_length + junc_view_3_front_param.common_config.front_view_length)
		/ (Float64_t)(junc_view_3_front_param.result_3_front_param.result_image_height);//pixel distance
	Float64_t pixel_col_dis = (vehicle_param.vehicle_width + junc_view_3_front_param.common_config.left_view_length + junc_view_3_front_param.common_config.right_view_length)
		/ (Float64_t)(junc_view_3_front_param.result_3_front_param.result_image_height);//pixel distance
	Float64_t weight_Float64_t;
	UInt32_t *p_lut_static_line = p_avm_param->p_avm_dis_line->p_static_dis_line_3_front;
	UInt32_t *p_lut_dynamic_line = p_avm_param->p_avm_dis_line->p_dynamic_dis_line_3_front;

	CvSize size0 = cvSize(junc_view_3_front_param.result_3_front_param.result_image_width, junc_view_3_front_param.result_3_front_param.result_image_height);//

	IplImage *image_result_3_front_lut = cvCreateImage(size0, IPL_DEPTH_8U, 1);
	memset(image_result_3_front_lut->imageData, 0, junc_view_3_front_param.result_3_front_param.result_image_width 
						* junc_view_3_front_param.result_3_front_param.result_image_height);
	
	//CvRect car_logo_rect = junc_view_4_param.result_big_param.car_logo_rect;

	/****malloc look up table****/
	int camera_size[4];
	for (Int32_t cam_id = 0; cam_id < CAMERA_NUM; cam_id++)
	{
		camera_size[cam_id] = junc_view_3_front_param.result_3_front_param.lut_size[cam_id] * 2;
	}

	for (int i = 0; i < junc_view_3_front_param.result_3_front_param.result_image_height; i++)
	{
		for (int j = 0; j <junc_view_3_front_param.result_3_front_param.result_image_width; j++)
		{
			/*************************front view**********************************/
			if (i < car_logo_rect.y)
			{
				int shift = i * junc_view_3_front_param.result_3_front_param.result_image_width * 2 + j * 2;
				int rgb[3];
				_3_front_resolve_data_from_lut(shift, image[0], rgb, p_avm_param);
				image_result_3_front_lut->imageData[i * junc_view_3_front_param.result_3_front_param.result_image_width + j] += (uchar)(rgb[0]);
				
			}

			/*******************************************left view********************************/

			if (j < car_logo_rect.x)
			{
				int shift = camera_size[0]  + i * car_logo_rect.x * 2 + j * 2;
				int rgb[3];
				_3_front_resolve_data_from_lut(shift, image[2], rgb, p_avm_param);


				image_result_3_front_lut->imageData[i * junc_view_3_front_param.result_3_front_param.result_image_width + j]
						= image_result_3_front_lut->imageData[i * junc_view_3_front_param.result_3_front_param.result_image_width + j] + (uchar)(rgb[0]);
				
			}
			/*************************right view****************/
			if (j >= (car_logo_rect.x + car_logo_rect.width) && j <junc_view_3_front_param.result_3_front_param.result_image_width)
			{
				int shift = camera_size[0] + camera_size[2] 
							+ i * (junc_view_3_front_param.result_3_front_param.result_image_width - car_logo_rect.x - car_logo_rect.width) * 2 
							+ (j - car_logo_rect.x - car_logo_rect.width) * 2;
				int rgb[3];
				_3_front_resolve_data_from_lut(shift, image[3], rgb, p_avm_param);
				image_result_3_front_lut->imageData[i *junc_view_3_front_param.result_3_front_param.result_image_width + j] 
						= image_result_3_front_lut->imageData[i * junc_view_3_front_param.result_3_front_param.result_image_width + j] + (uchar)(rgb[0]);
				
			}
		}

	}
#if 0
	int n = p_lut_static_line[0];
	for (int i = 1; i < p_lut_static_line[0]; i++)
	{
		UInt32_t pic_coordinate[2];
		int rgb[3];
		resolve_data_from_lut_line(i, rgb, p_avm_param, p_lut_static_line, pic_coordinate);

		image_result_3_front_lut->imageData[pic_coordinate[0] 
			* junc_view_3_front_param.result_3_front_param.result_image_width + pic_coordinate[1]] = (uchar)(rgb[0]);

	}

	/*********解析动态表******/
	for (int m = 0; m < 59; m++)
	{
		int n = p_lut_dynamic_line[m];
		for (int i = p_lut_dynamic_line[m]; i <p_lut_dynamic_line[m + 1]; i++)
		{
			UInt32_t pic_coordinate[2];
			int rgb[3];
			resolve_data_from_lut_line(i, rgb, p_avm_param, p_lut_dynamic_line, pic_coordinate);

			image_result_3_front_lut->imageData[pic_coordinate[0] * junc_view_3_front_param.result_3_front_param.result_image_width + pic_coordinate[1]] = (uchar)(rgb[0]);
		}
	}
	
#endif






	char result_name[MAX_ARRAY_SIZE];
	sprintf(result_name, "%s/image_result_3_front_lut.bmp", result_image_path);
	cvSaveImage(result_name, image_result_3_front_lut);
	return ret;
}







/***************resolve 3 front view data from LUT*******************/

/*
* Function Name:				_3_rear_resolve_data_from_lut
* Function Function:			_3_rear_resolve_data_from_lut
* Input:
*       shift;                the position of data in LUT
*		image;                source image
*       rgb;                  get value of pixel in source image
		p_avm_param;
* Note:
*
* Revision:
*     Create by Cheng.Li on 2018/1/17.
*/
void _3_rear_resolve_data_from_lut(IN int shift, IN IplImage *image, OUT int *rgb, IN  P_Avm_Param_S p_avm_param)
{
	Float64_t image_point0[2];
	unsigned int temp0 = p_avm_param->p_avm_lut->p_lut_junc_view_3_rear[shift];
	unsigned int temp1 = p_avm_param->p_avm_lut->p_lut_junc_view_3_rear[shift + 1];
	int u_int = temp0 >> 21;// v_int = (int)image_point[1];
	int v_int = temp0 >> 10;
	v_int = v_int & 0x7FF;
	Float64_t u_point = (Float64_t)(temp1 >> 24) / 255.000, v_point = ((Float64_t)((temp1 >> 16) & 255)) / 255.000;
	image_point0[0] = (Float64_t)u_int + u_point;
	image_point0[1] = (Float64_t)v_int + v_point;
	unsigned int weight_int = temp0 >> 2;
	weight_int = weight_int & 0x00FF;
	Float64_t weight_Float64_t = Float64_t(weight_int) / (Float64_t)255;
	bilinear_interpolation(image, rgb, image_point0[1], image_point0[0]);
	rgb[0] = (uchar)(Float64_t(rgb[0])*weight_Float64_t);

}



/*
* Function Name:				_3_rear_resolve_data_from_lut
* Function Function:			_3_rear_resolve_data_from_lut
* Input:
*       shift;                the position of data in LUT
*		image;                source image
*       rgb;                  get value of pixel in source image
p_avm_param;
* Note:
*
* Revision:
*     Create by Cheng.Li on 2018/1/17.
*/
AVM_Error_Code_E resolve_3_rear_data_from_LUT(IN  P_Avm_Param_S p_avm_param)
{
	AVM_Error_Code_E ret = ERROR_NONE;
	IplImage* image[4] = { NULL, NULL, NULL, NULL };

	ret = my_load_image(image);

	Junc_View_3_Rear_Param_S	junc_view_3_rear_param = p_avm_param->p_avm_config_param->avm_2d_param.junc_view_3_rear_param;
	CvRect car_logo_rect = junc_view_3_rear_param.result_3_rear_param.car_logo_rect;
	Vehicle_Param_S	 vehicle_param = p_avm_param->p_avm_config_param->vehicle_param;
	Float64_t pixel_row_dis = (vehicle_param.vehicle_length + junc_view_3_rear_param.common_config.rear_view_length)
		/ (Float64_t)(junc_view_3_rear_param.result_3_rear_param.result_image_height);//pixel distance
	Float64_t pixel_col_dis = (vehicle_param.vehicle_width + junc_view_3_rear_param.common_config.left_view_length + junc_view_3_rear_param.common_config.right_view_length)
		/ (Float64_t)(junc_view_3_rear_param.result_3_rear_param.result_image_height);//pixel distance
	Float64_t weight_Float64_t;

	UInt32_t *p_lut_static_line = p_avm_param->p_avm_dis_line->p_static_dis_line_3_rear;
	UInt32_t *p_lut_dynamic_line = p_avm_param->p_avm_dis_line->p_dynamic_dis_line_3_rear;


	CvSize size0 = cvSize(junc_view_3_rear_param.result_3_rear_param.result_image_width, junc_view_3_rear_param.result_3_rear_param.result_image_height);//

	IplImage *image_result_3_front_lut = cvCreateImage(size0, IPL_DEPTH_8U, 1);
	memset(image_result_3_front_lut->imageData, 0, junc_view_3_rear_param.result_3_rear_param.result_image_width * junc_view_3_rear_param.result_3_rear_param.result_image_height);

	/****malloc look up table****/
	int camera_size[4];
	for (Int32_t cam_id = 0; cam_id < CAMERA_NUM; cam_id++)
	{
		camera_size[cam_id] = junc_view_3_rear_param.result_3_rear_param.lut_size[cam_id] * 2;
	}

	for (int i = 0; i < junc_view_3_rear_param.result_3_rear_param.result_image_height; i++)
	{
		for (int j = 0; j <junc_view_3_rear_param.result_3_rear_param.result_image_width; j++)
		{
			/*************************rear view**********************************/
			if ((i >= (car_logo_rect.y + car_logo_rect.height)))
			{
				//int shift = i * junc_view_3_rear_param.result_3_rear_param.result_image_width * 2 + j * 2;
				int shift = (i - (car_logo_rect.y + car_logo_rect.height)) * junc_view_3_rear_param.result_3_rear_param.result_image_width * 2 + j * 2;
				int rgb[3];
				_3_rear_resolve_data_from_lut(shift, image[1], rgb, p_avm_param);
				image_result_3_front_lut->imageData[i * junc_view_3_rear_param.result_3_rear_param.result_image_width + j] += (uchar)(rgb[0]);

			}

			/*******************************************left view********************************/
			if (j < car_logo_rect.x)
			{
				int shift = camera_size[0] + camera_size[1] + i * car_logo_rect.x * 2 + j * 2;
				int rgb[3];
				_3_rear_resolve_data_from_lut(shift, image[2], rgb, p_avm_param);

				image_result_3_front_lut->imageData[i * junc_view_3_rear_param.result_3_rear_param.result_image_width + j]
					= image_result_3_front_lut->imageData[i * junc_view_3_rear_param.result_3_rear_param.result_image_width + j] + (uchar)(rgb[0]);

			}
			/*************************right view****************/
			if (j >= (car_logo_rect.x + car_logo_rect.width) && j <junc_view_3_rear_param.result_3_rear_param.result_image_width)
			{
				int shift = camera_size[0] + camera_size[1] + camera_size[2]
					+ i * (junc_view_3_rear_param.result_3_rear_param.result_image_width - car_logo_rect.x - car_logo_rect.width) * 2
					+ (j - car_logo_rect.x - car_logo_rect.width) * 2;
				int rgb[3];
				_3_rear_resolve_data_from_lut(shift, image[3], rgb, p_avm_param);
				image_result_3_front_lut->imageData[i *junc_view_3_rear_param.result_3_rear_param.result_image_width + j]
					= image_result_3_front_lut->imageData[i * junc_view_3_rear_param.result_3_rear_param.result_image_width + j] + (uchar)(rgb[0]);

			}
		}

	}



#if 0
	int n = p_lut_static_line[0];
	for (int i = 1; i < p_lut_static_line[0]; i++)
	{
		UInt32_t pic_coordinate[2];
		int rgb[3];
		resolve_data_from_lut_line(i, rgb, p_avm_param, p_lut_static_line, pic_coordinate);

		image_result_3_front_lut->imageData[pic_coordinate[0]
			* junc_view_3_rear_param.result_3_rear_param.result_image_width + pic_coordinate[1]] = (uchar)(rgb[0]);

	}
#endif
#if 0
	/*********解析动态表******/
	for (int m = 0; m < 59; m++)
	{
		int n = p_lut_dynamic_line[m];
		for (int i = p_lut_dynamic_line[m]; i <p_lut_dynamic_line[m + 1]; i++)
		{
			UInt32_t pic_coordinate[2];
			int rgb[3];
			resolve_data_from_lut_line(i, rgb, p_avm_param, p_lut_dynamic_line, pic_coordinate);

			image_result_3_front_lut->imageData[pic_coordinate[0] * junc_view_3_rear_param.result_3_rear_param.result_image_width + pic_coordinate[1]] = (uchar)(rgb[0]);
		}
	}

#endif












	char result_name[MAX_ARRAY_SIZE];
	sprintf(result_name, "%s/image_result_3_rear_lut.bmp", result_image_path);
	cvSaveImage(result_name, image_result_3_front_lut);
	
	return ret;
}





void front_single_resolve_data(IN int shift, IN IplImage *image, OUT int *rgb, IN  P_Avm_Param_S p_avm_param, OUT UInt32_t *p_lut)
{
	Float64_t image_point0[2];
	//shift = 0;
	//unsigned int temp0 = p_avm_param->p_avm_lut->p_lut_single_view_front[shift];
	unsigned int temp0 = p_lut[shift];
	unsigned int temp1 = p_lut[shift + 1];
	int u_int = temp0 >> 21;// v_int = (int)image_point[1];
	int v_int = temp0 >> 10;
	v_int = v_int & 0x7FF;
	Float64_t u_point = (Float64_t)(temp1 >> 24) / 255.000, v_point = ((Float64_t)((temp1 >> 16) & 255)) / 255.000;
	image_point0[0] = (Float64_t)u_int + u_point;
	image_point0[1] = (Float64_t)v_int + v_point;
	unsigned int weight_int = temp0 >> 2;
	weight_int = weight_int & 0x00FF;
	Float64_t weight_Float64_t = Float64_t(weight_int) / (Float64_t)255;
	bilinear_interpolation(image, rgb, image_point0[1], image_point0[0]);
	rgb[0] = (uchar)(Float64_t(rgb[0]));

}








void reslove_single_view_from_lut(IN P_Avm_Param_S p_avm_param, IN int cam_flag)
{
	Front_Rear_Single_View_Param_S single_view_param_front_rear;
	CvSize size0;
	UInt32_t* p_lut=NULL;
	UInt32_t *p_lut_static_line =NULL;
	UInt32_t *p_lut_dynamic_line = NULL;
	Left_Right_Single_View_Param_S left_right_single_view_param;
	int result_image_height;
	int result_image_width;
	
	if (cam_flag == 0)
	{
		single_view_param_front_rear = p_avm_param->p_avm_config_param->avm_single_view_param.front_single_view_param;
		size0 = cvSize(single_view_param_front_rear.result_image_width, single_view_param_front_rear.result_image_height);//
		p_lut = p_avm_param->p_avm_lut->p_lut_single_view_front;
		p_lut_static_line = p_avm_param->p_avm_dis_line->p_static_dis_line_front;
		p_lut_dynamic_line = p_avm_param->p_avm_dis_line->p_dynamic_dis_line_front;
		result_image_height = single_view_param_front_rear.result_image_height;
		result_image_width = single_view_param_front_rear.result_image_width;

	}
	if (cam_flag == 1)
	{
		single_view_param_front_rear = p_avm_param->p_avm_config_param->avm_single_view_param.rear_single_view_param;
		size0 = cvSize(single_view_param_front_rear.result_image_width, single_view_param_front_rear.result_image_height);//
		p_lut = p_avm_param->p_avm_lut->p_lut_single_view_rear;
		p_lut_static_line = p_avm_param->p_avm_dis_line->p_static_dis_line_rear;
		p_lut_dynamic_line = p_avm_param->p_avm_dis_line->p_dynamic_dis_line_rear;
		result_image_height = single_view_param_front_rear.result_image_height;
		result_image_width = single_view_param_front_rear.result_image_width;
	}

	if (cam_flag == 2)
	{
		left_right_single_view_param = p_avm_param->p_avm_config_param->avm_single_view_param.left_single_view_param;
		size0 = cvSize(left_right_single_view_param.result_image_width, left_right_single_view_param.result_image_height);//
		p_lut = p_avm_param->p_avm_lut->p_lut_single_view_left;
		p_lut_static_line = p_avm_param->p_avm_dis_line->p_static_dis_line_rear;
		p_lut_dynamic_line = p_avm_param->p_avm_dis_line->p_dynamic_dis_line_rear;
		result_image_height = left_right_single_view_param.result_image_height;
		result_image_width = left_right_single_view_param.result_image_width;

	}
	if (cam_flag == 3)
	{
		left_right_single_view_param = p_avm_param->p_avm_config_param->avm_single_view_param.right_single_view_param;
		size0 = cvSize(left_right_single_view_param.result_image_width, left_right_single_view_param.result_image_height);//
		p_lut = p_avm_param->p_avm_lut->p_lut_single_view_right;
		p_lut_static_line = p_avm_param->p_avm_dis_line->p_static_dis_line_right;
		p_lut_dynamic_line = p_avm_param->p_avm_dis_line->p_dynamic_dis_line_right;
		result_image_height = left_right_single_view_param.result_image_height;
		result_image_width = left_right_single_view_param.result_image_width;

	}
	if (cam_flag == 4)
	{
		result_image_height = p_avm_param->p_avm_config_param->avm_single_view_param.left_right_view_param.result_image_height;
		result_image_width = p_avm_param->p_avm_config_param->avm_single_view_param.left_right_view_param.result_image_width;

		size0 = cvSize(result_image_width, result_image_height);//
		p_lut = p_avm_param->p_avm_lut->p_lut_single_view_left_right;
		p_lut_static_line = p_avm_param->p_avm_dis_line->p_static_dis_line_left_right;
		p_lut_dynamic_line = p_avm_param->p_avm_dis_line->p_dynamic_dis_line_left_right;
	}



	


	IplImage* image[4] = { NULL, NULL, NULL, NULL };

	int ret = my_load_image(image);
	//CHECK_ERROR(ret);
	IplImage *image_result_single_front_lut = cvCreateImage(size0, IPL_DEPTH_8U, 1);
	memset(image_result_single_front_lut->imageData, 0, result_image_width*result_image_height);
	for (int i = 0; i <result_image_height; i++)
	{
		for (int j = 0; j < result_image_width; j++)
		{
			int shift = i*result_image_width * 2 + j * 2;
			int rgb[3];
			front_single_resolve_data(shift, image[cam_flag], rgb, p_avm_param, p_lut);
			image_result_single_front_lut->imageData[i *result_image_width + j] = (uchar)(rgb[0]);
		}
	}
	
#if 0
	if (cam_flag == CAM_FRONT || cam_flag == CAM_REAR)
	{
		for (int i = 0; i < p_lut_static_line[0]; i++)
		{
			UInt32_t pic_coordinate[2];
			int rgb[3];
			resolve_data_from_lut_line(i, rgb, p_avm_param, p_lut_static_line, pic_coordinate);

			image_result_single_front_lut->imageData[pic_coordinate[0] * result_image_width + pic_coordinate[1]] = (uchar)(rgb[0]);

		}
	}
#if 1
		if (cam_flag == 0 || cam_flag == 1)
		{
			for (int m = 0; m < 60; m++)
			{
				int n = p_lut_dynamic_line[m];
				for (int i = p_lut_dynamic_line[m]; i < p_lut_dynamic_line[m + 1]; i++)
				{
					UInt32_t pic_coordinate[2];
					int rgb[3];
					resolve_data_from_lut_line(i, rgb, p_avm_param, p_lut_dynamic_line, pic_coordinate);

					image_result_single_front_lut->imageData[pic_coordinate[0] * result_image_width + pic_coordinate[1]] = (uchar)(rgb[0]);
				}
			}
		}
	


	//	/*for (int i = 0; i < p_lut_static_line[0]; i++)
	//	{
	//		UInt32_t pic_coordinate[2];
	//		int rgb[3];
	//		resolve_data_from_lut_line(i, rgb, p_avm_param, p_lut_static_line, pic_coordinate);
	//		image_result_single_front_lut->imageData[pic_coordinate[0] * result_image_width + pic_coordinate[1]] = (uchar)(rgb[0]);

	//	}*/
	//}
#endif
	printf("laotianyebaoyouwo wodebababaoyouewomiaozaishentudilaobaoyouwo");
#endif
	char result_name[MAX_ARRAY_SIZE];
	sprintf(result_name, "%s/画线之后的single%d.bmp", result_image_path,cam_flag);
	cvSaveImage(result_name, image_result_single_front_lut);
	
}





















