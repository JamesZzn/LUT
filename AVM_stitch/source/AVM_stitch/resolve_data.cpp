#include"resolve_data.h"
#include"avm_error.h"
#include "avm_config.h"
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
AVM_Error_Code_E my_load_image(OUT IplImage* image[4], IN  P_Avm_Param_S p_avm_param)
{
	AVM_Error_Code_E ret = ERROR_NONE;
#if RELEASE==1

	for (int cam_id = 0; cam_id < 4; cam_id++)
	{
		char image_path[MAX_ARRAY_SIZE];
		sprintf(image_path, "%s/%s.bmp",
			source_image_path, camera_prefix_array[cam_id]);//因为与别人的图片名不相同后图片别人叫back，我的叫rear所以加代码兼容
		if (cam_id == 1)
		{
			sprintf(image_path, "%s/back.bmp",
				source_image_path);

		}

		image[cam_id] = cvLoadImage(image_path);
		if (image[cam_id] == 0)
		{
			ret = ERROR_IMAGE_FAIL;;
			CHECK_ERROR(ret);
		}
	}


#else


		for (int cam_id = 0; cam_id < 4; cam_id++)
		{
			char image_path[MAX_ARRAY_SIZE];
			sprintf(image_path, "%s/%s.bmp",
				source_image_path, camera_prefix_array[cam_id]);
			if (cam_id == 1)
			{
				sprintf(image_path, "%s/back.bmp",
					source_image_path);

			}
			image[cam_id] = cvLoadImage(image_path);
			if (image[cam_id] == 0)
			{
				ret = ERROR_IMAGE_FAIL;;
				CHECK_ERROR(ret);
			}
		}
#endif
	return ret;

}

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
AVM_Error_Code_E my_load_image_yuv(IN  P_Avm_Param_S p_avm_param)
{
	AVM_Error_Code_E ret = ERROR_NONE;


	for (int cam_id = 0; cam_id < 4; cam_id++)
	{
		char image_path[MAX_ARRAY_SIZE];
		sprintf(image_path, "%s/%s.yuv",
			source_image_path, camera_prefix_array[cam_id]);
		FILE*	p = fopen(image_path, "rb");
		if (!p)
			printf("Can not open file");



		

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
void resolve_data_from_lut(IN int shift, IN IplImage *image, OUT int *rgb, 
	IN  P_Avm_Param_S p_avm_param, IN UInt32_t*	p_lut)
{
	Float32_t image_point0[2];
	UInt32_t temp0 = p_lut[shift];
	UInt32_t temp1 = p_lut[shift + 1];
	Int32_t u_int = temp0 >> 21;// v_int = (int)image_point[1];
	Int32_t v_int = temp0 >> 10;
	v_int = v_int & 0x7FF;
	Float32_t u_point = (Float32_t)(temp1 >> 24) / 255.000, v_point = ((Float32_t)((temp1 >> 16) & 255)) / 255.000;
	image_point0[0] = (Float32_t)u_int + u_point;
	image_point0[1] = (Float32_t)v_int + v_point;
	UInt32_t weight_int = temp0 >> 2;
	weight_int = weight_int & 0x00FF;
	Float32_t weight_Float32_t = Float32_t(weight_int) / (Float32_t)255;
	bilinear_interpolation(image, rgb, image_point0[1], image_point0[0]);
	rgb[0] = (uchar)(Float32_t(rgb[0])*weight_Float32_t);

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
void resolve_data_from_lut_new(IN int shift, IN IplImage *image, OUT int *rgb,
	IN  P_Avm_Param_S p_avm_param, IN UInt32_t*	p_lut)
{
	
	UInt32_t temp0 = p_lut[shift];
	UInt32_t temp1 = p_lut[shift + 1];
	Int32_t v_int = temp0 >> 21;// 第一位存储是图像的高坐标
	Int32_t  u_int = temp0 >> 10;
	u_int = u_int & 0x7FF;
	Float32_t weightUpLeft = (Float32_t)(temp1 >> 24) / 255.000,
		weightUpRight = ((Float32_t)((temp1 >> 16) & 255)) / 255.000,
		weightDownLeft=((Float32_t)((temp1 >> 8) & 255)) / 255.000, 
		weightDownRight = 1 - weightUpLeft - weightUpRight - weightDownLeft;
	bilinear_interpolation_new(image, rgb, u_int, v_int,
		weightUpLeft,weightUpRight,  weightDownLeft,weightDownRight);

	UInt32_t weight_int = temp0 >> 2;
	weight_int = weight_int & 255;
	Float32_t weight_Float32_t = Float32_t(weight_int) / (Float32_t)255;


	

	
	rgb[0] = (uchar)(Float32_t(rgb[0])*weight_Float32_t);
	rgb[1] = (uchar)(Float32_t(rgb[1])*weight_Float32_t);
	rgb[2] = (uchar)(Float32_t(rgb[2])*weight_Float32_t);
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
void resolve_data_from_lut_new_gpu(IN int shift, IN int shift_gpu, IN IplImage *image, OUT int *rgb,
	IN  P_Avm_Param_S p_avm_param, IN UInt32_t*	p_lut,IN UInt32_t*gpu_lut)
{

	UInt32_t temp0 = p_lut[shift];
	UInt32_t temp1 = p_lut[shift + 1];
	UInt32_t v_int = temp0 >> 21;// 第一位存储是图像的高坐标
	UInt32_t  u_int = temp0 >> 10;
	u_int = u_int & 0x7FF;
	Float32_t weightUpLeft = (Float32_t)(temp1 >> 24) / 255.000,
		weightUpRight = ((Float32_t)((temp1 >> 16) & 255)) / 255.000,
		weightDownLeft = ((Float32_t)((temp1 >> 8) & 255)) / 255.000,
		weightDownRight = 1 - weightUpLeft - weightUpRight - weightDownLeft;
	bilinear_interpolation_new(image, rgb, u_int, v_int,
		weightUpLeft, weightUpRight, weightDownLeft, weightDownRight);

	UInt32_t weight_int = temp0 >> 2;
	weight_int = weight_int & 255;
	Float32_t weight_Float32_t = Float32_t(weight_int) / (Float32_t)255;

	gpu_lut[shift_gpu / 2] = (((v_int << 11) + u_int) << 10) + (UInt32_t)(weight_Float32_t * 1023);



	rgb[0] = (uchar)(Float32_t(rgb[0])*weight_Float32_t);
	rgb[1] = (uchar)(Float32_t(rgb[1])*weight_Float32_t);
	rgb[2] = (uchar)(Float32_t(rgb[2])*weight_Float32_t);
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
void resolve_data_from_lut_new_yuv(IN Int32_t shift, OUT Int32_t *rgb,
	IN  P_Avm_Param_S p_avm_param, IN UInt32_t*	p_lut,IN UInt32_t cam_id)
{
	
	UInt32_t temp0 = p_lut[shift];
	UInt32_t temp1 = p_lut[shift + 1];
	Int32_t v_int = temp0 >> 21;// v_int = (int)image_point[1];
	Int32_t u_int = temp0 >> 10;
	u_int = u_int & 0x7FF;
	
	Float32_t weightUpLeft = (Float32_t)(temp1 >> 24) / 255.000,
		weightUpRight = ((Float32_t)((temp1 >> 16) & 255)) / 255.000,
		weightDownLeft = ((Float32_t)((temp1 >> 8) & 255)) / 255.000,
		weightDownRight = 1 - weightUpLeft - weightUpRight - weightDownLeft;
	

	UInt32_t weight_int = temp0 >> 2;
	weight_int = weight_int & 255;
	Float32_t weight_Float32_t = Float32_t(weight_int) / (Float32_t)255;

	rgb[0] = (Int32_t )(p_avm_param->p_avm_image->source_image_array[cam_id]->p_data[v_int*
		p_avm_param->p_avm_image->source_image_array[cam_id]->width * 2 + u_int * 2] * weight_Float32_t);
	rgb[1] = (Int32_t)(p_avm_param->p_avm_image->source_image_array[cam_id]->p_data[v_int*
		p_avm_param->p_avm_image->source_image_array[cam_id]->width * 2 + u_int * 2+1] * weight_Float32_t);

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
	
	UInt32_t temp0 = p_lut_line[shift + 1];
	pic_coordinate[0] = temp0 >> 21;// v_int = (int)image_point[1];
	pic_coordinate[1] = (temp0 >> 10) - ((temp0 >> 21)<<11);
	//int colar_flag = (temp0 >> 2) - ((temp0 >> 10) << 8);
	int colar_flag = (temp0) - ((temp0 >> 10)<<10);

	if (colar_flag == COLOR_RED)
	{
		rgb[2] = 255;
	}
	if (colar_flag == COLOR_YELLOW)
	{
		rgb[1] = 255;
		rgb[2] = 255;
	}
	if (colar_flag == COLOR_GREEN)
	{
		rgb[1] = 255;;
	}
	int a = 0;

}








/************** resolve_4_view_data_from_LUT***********************/
/*
* Function Name:				 resolve_4_view_data_from_LUT
* Function Function:			resolve_4_view_data_from_LUT
*       p_avm_param;
* Note:
*     the interface function should be called at the beginning of this module
* Revision:
*     Create by Cheng.Li on 2018/5/2.
*/
AVM_Error_Code_E GPU_resolve_4_view_data_from_LUT(IN  P_Avm_Param_S p_avm_param, IN int view_id)
{
	AVM_Error_Code_E ret = ERROR_NONE;
	CvSize size0;
	Int32_t result_image_height;
	Int32_t result_image_width;
	CvRect car_logo_rect;
	UInt32_t * p_lut = NULL;
	Int32_t camera_size[4];
	char result_name[MAX_ARRAY_SIZE];
	UInt8_t*		p_data;
	if (view_id == _4_VIEW_BIG)//大图
	{
		p_lut = p_avm_param->p_avm_lut->p_lut_junc_view_4_big;
		result_image_height = p_avm_param->p_avm_config_param->avm_2d_param.junc_view_4_param.result_big_param.result_image_height;
		result_image_width = p_avm_param->p_avm_config_param->avm_2d_param.junc_view_4_param.result_big_param.result_image_width;
		size0 = cvSize(result_image_width, result_image_height);//
		car_logo_rect = p_avm_param->p_avm_config_param->avm_2d_param.junc_view_4_param.result_big_param.car_logo_rect;
		p_data = p_avm_param->p_avm_image->result_image.result_image_junc_view_4_big->p_data;
		for (Int32_t cam_id = 0; cam_id < CAMERA_NUM; cam_id++)
		{
			camera_size[cam_id] = p_avm_param->p_avm_config_param->avm_2d_param.junc_view_4_param.result_big_param.lut_size[cam_id] * 2;
		}
	}
	if (view_id == _4_VIEW_SMALL)//小图
	{
		result_image_height = p_avm_param->p_avm_config_param->avm_2d_param.junc_view_4_param.result_small_param.result_image_height;
		result_image_width = p_avm_param->p_avm_config_param->avm_2d_param.junc_view_4_param.result_small_param.result_image_width;
		size0 = cvSize(result_image_width, result_image_height);//
		p_lut = p_avm_param->p_avm_lut->p_lut_junc_view_4_small;
		car_logo_rect = p_avm_param->p_avm_config_param->avm_2d_param.junc_view_4_param.result_small_param.car_logo_rect;
		p_data = p_avm_param->p_avm_image->result_image.result_image_junc_view_4_small->p_data;
		for (Int32_t cam_id = 0; cam_id < CAMERA_NUM; cam_id++)
		{
			camera_size[cam_id] = p_avm_param->p_avm_config_param->avm_2d_param.junc_view_4_param.result_small_param.lut_size[cam_id] * 2;
		}
	}
#if RGB
	IplImage* image[4] = { NULL, NULL, NULL, NULL };
	ret = my_load_image(image, p_avm_param);

	int chanels = image[2]->nChannels;//几通道
	IplImage *image_result_lut = cvCreateImage(size0, IPL_DEPTH_8U, chanels);//原图 几通道就建立  
	memset(image_result_lut->imageData, 0, result_image_height * result_image_width*chanels);
	int step = image_result_lut->widthStep / sizeof(uchar);//自动计算一行多少元素 
	/****malloc look up table****/
#endif

#if YUV
	ret = my_load_image_yuv(p_avm_param);
#endif
	for (int i = 0; i < result_image_height; i++)
	{
		for (int j = 0; j < result_image_width; j++)
		{
#if 1

			/************front view*******************************/
			if (i < car_logo_rect.y)
			{
				int shift = i * result_image_width * 2 + j * 2;


				int shift0 = (result_image_width - i)*  car_logo_rect.y * 2 + j * 2;




				int rgb[3];
#if RGB
				resolve_data_from_lut_new(shift, image[0], rgb, p_avm_param, p_lut);
				if (image_result_lut->nChannels == 1)
				{
					image_result_lut->imageData[i *  step + j* chanels] += (uchar)(rgb[0]);
				}
				else
				{
					image_result_lut->imageData[i *  step + j* chanels] += (uchar)(rgb[0]);
					image_result_lut->imageData[i *  step + j* chanels + 1] += (uchar)(rgb[1]);
					image_result_lut->imageData[i *  step + j* chanels + 2] += (uchar)(rgb[2]);
				}
#endif
#if YUV
				rgb[0] = 0;
				rgb[1] = 0;
				resolve_data_from_lut_new_yuv(shift, rgb, p_avm_param, p_lut, CAM_FRONT);
				p_data[i* result_image_width * 2 + j * 2] += (uchar)(rgb[0]);
				p_data[i* result_image_width * 2 + j * 2 + 1] += (uchar)(rgb[1]);
#endif
			}

			/*************rear view*********************************/

			if ((i >= (car_logo_rect.y + car_logo_rect.height)) && (i < result_image_height))
			{
				int shift = camera_size[0] + (i - (car_logo_rect.y + car_logo_rect.height)) * result_image_width * 2 + j * 2;

				int shift1 = (result_image_width - (i - (car_logo_rect.y + car_logo_rect.height))) * car_logo_rect.y * 2 + j * 2;


				int rgb[3];
#if RGB
				resolve_data_from_lut_new(shift, image[1], rgb, p_avm_param, p_lut);
				if (image_result_lut->nChannels == 1)
				{
					image_result_lut->imageData[i *  step + j* chanels] += (uchar)(rgb[0]);
				}
				else
				{
					image_result_lut->imageData[i *  step + j* chanels] += (uchar)(rgb[0]);
					image_result_lut->imageData[i *  step + j* chanels + 1] += (uchar)(rgb[1]);
					image_result_lut->imageData[i *  step + j* chanels + 2] += (uchar)(rgb[2]);
				}
#endif
#if YUV
				resolve_data_from_lut_new_yuv(shift, rgb, p_avm_param, p_lut, CAM_REAR);
				p_data[i* result_image_width * 2 + j * 2] += (uchar)(rgb[0]);
				p_data[i* result_image_width * 2 + j * 2 + 1] += (uchar)(rgb[1]);
#endif
			}
#endif
#if 1
			/*******************************************left view********************************/
			if (j < car_logo_rect.x)
			{
				int shift = camera_size[0] + camera_size[1] + i * car_logo_rect.x * 2 + j * 2;
				int rgb[3];
#if RGB
				resolve_data_from_lut_new(shift, image[2], rgb, p_avm_param, p_lut);
				if (image_result_lut->nChannels == 1)
				{
					image_result_lut->imageData[i *  step + j* chanels] += (uchar)(rgb[0]);
				}
				else
				{
					image_result_lut->imageData[i *  step + j* chanels] += (uchar)(rgb[0]);
					image_result_lut->imageData[i *  step + j* chanels + 1] += (uchar)(rgb[1]);
					image_result_lut->imageData[i *  step + j* chanels + 2] += (uchar)(rgb[2]);
				}
#endif
#if YUV
				resolve_data_from_lut_new_yuv(shift, rgb, p_avm_param, p_lut, CAM_LEFT);

				p_data[i* result_image_width * 2 + j * 2] += (uchar)(rgb[0]);
				p_data[i* result_image_width * 2 + j * 2 + 1] += (uchar)(rgb[1]);
#endif
			}
			/*************************right view****************/
			if (j >= (car_logo_rect.x + car_logo_rect.width) && j < result_image_width)
			{
				int shift = camera_size[0] + camera_size[1] + camera_size[2]
					+ i * (result_image_width - car_logo_rect.x - car_logo_rect.width) * 2
					+ (j - car_logo_rect.x - car_logo_rect.width) * 2;

				int rgb[3];
#if RGB
				resolve_data_from_lut_new(shift, image[3], rgb, p_avm_param, p_lut);
				if (image_result_lut->nChannels == 1)
				{
					image_result_lut->imageData[i *  step + j* chanels] += (uchar)(rgb[0]);
				}
				else
				{
					image_result_lut->imageData[i *  step + j* chanels] += (uchar)(rgb[0]);
					image_result_lut->imageData[i *  step + j* chanels + 1] += (uchar)(rgb[1]);
					image_result_lut->imageData[i *  step + j* chanels + 2] += (uchar)(rgb[2]);
				}
#endif
#if YUV
				resolve_data_from_lut_new_yuv(shift, rgb, p_avm_param, p_lut, CAM_RIGHT);
				p_data[i* result_image_width * 2 + j * 2] += rgb[0];
				p_data[i* result_image_width * 2 + j * 2 + 1] += rgb[1];
#endif
			}
#endif
		}

	}

#if RGB

	sprintf(result_name, "%s/%s_%dX%d.bmp", result_image_path, view_name[view_id], result_image_width, result_image_height);
	cvSaveImage(result_name, image_result_lut);
#endif

#if YUV
	FILE *fid;
	sprintf(result_name, "%s/%s_%dX%d.yuv", result_image_path, view_name[view_id], result_image_width, result_image_height);
	fid = fopen(result_name, "wb");
	fwrite(p_data, sizeof(UInt8_t), result_image_height*result_image_width * 2 * sizeof(UInt8_t), fid);
	fclose(fid);
#endif
#if RGB
	cvReleaseImage(&image_result_lut);
	cvReleaseImage(&image[0]);
	cvReleaseImage(&image[1]);
	cvReleaseImage(&image[2]);
	cvReleaseImage(&image[3]);
#endif
	return ret;
}











#if 0
void resize_image(unsigned char *p_dst_img, unsigned char* p_source_yuv422_img, int width, int height)
{
	int tempY, tempU, tempV;
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			int width_scale = 1280/width;
			int height_scale = 720 / height;
			p_dst_img[i *width * 2 + j * 2 + 1] = p_source_yuv422_img[i * height_scale * 2 + j *width_scale * 2 + 1];
			p_dst_img[i *width * 2 + j * 2 + 1] = tempY ;



			p_dst_img[i *width * 2 + j * 2] = p_source_yuv422_img[i * height_scale * 2 + j *width_scale * 2 + 1];

			p_dst_img[i *width * 2 + j * 2] = p_source_yuv422_img[i * height_scale * 2 + j *width_scale * 2 + 1];

			



		}



	}




}
#endif










/************** resolve_4_view_data_from_LUT***********************/
/*
* Function Name:				 resolve_4_view_data_from_LUT
* Function Function:			resolve_4_view_data_from_LUT
*       p_avm_param;
* Note:
*     the interface function should be called at the beginning of this module
* Revision:
*     Create by Cheng.Li on 2018/5/2.
*/
AVM_Error_Code_E resolve_4_view_data_from_LUT(IN  P_Avm_Param_S p_avm_param, IN int view_id)
{
	AVM_Error_Code_E ret = ERROR_NONE;
	CvSize size0;
	Int32_t result_image_height;
	Int32_t result_image_width;
	CvRect car_logo_rect ;
	UInt32_t * p_lut=NULL;
	Int32_t camera_size[4];
	char result_name[MAX_ARRAY_SIZE];
	UInt8_t*		p_data;
	if (view_id == _4_VIEW_BIG)//大图
	{
		p_lut = p_avm_param->p_avm_lut->p_lut_junc_view_4_big;
		result_image_height = p_avm_param->p_avm_config_param->avm_2d_param.junc_view_4_param.result_big_param.result_image_height;
		result_image_width = p_avm_param->p_avm_config_param->avm_2d_param.junc_view_4_param.result_big_param.result_image_width;
		size0 = cvSize(result_image_width, result_image_height);//
		car_logo_rect = p_avm_param->p_avm_config_param->avm_2d_param.junc_view_4_param.result_big_param.car_logo_rect;
		p_data = p_avm_param->p_avm_image->result_image.result_image_junc_view_4_big->p_data;
		for (Int32_t cam_id = 0; cam_id < CAMERA_NUM; cam_id++)
		{
			camera_size[cam_id] = p_avm_param->p_avm_config_param->avm_2d_param.junc_view_4_param.result_big_param.lut_size[cam_id] * 2;
		}
	}
	if (view_id == _4_VIEW_SMALL)//小图
	{
		result_image_height = p_avm_param->p_avm_config_param->avm_2d_param.junc_view_4_param.result_small_param.result_image_height;
		result_image_width = p_avm_param->p_avm_config_param->avm_2d_param.junc_view_4_param.result_small_param.result_image_width;
		size0 = cvSize(result_image_width, result_image_height);//
		p_lut = p_avm_param->p_avm_lut->p_lut_junc_view_4_small;
		car_logo_rect = p_avm_param->p_avm_config_param->avm_2d_param.junc_view_4_param.result_small_param.car_logo_rect;
		p_data = p_avm_param->p_avm_image->result_image.result_image_junc_view_4_small->p_data;
		for (Int32_t cam_id = 0; cam_id < CAMERA_NUM; cam_id++)
		{
			camera_size[cam_id] = p_avm_param->p_avm_config_param->avm_2d_param.junc_view_4_param.result_small_param.lut_size[cam_id] * 2;
		}
	}
#if RGB
	IplImage* image[4] = { NULL, NULL, NULL, NULL };
	ret = my_load_image(image, p_avm_param);

	int chanels = image[2]->nChannels;//几通道
	IplImage *image_result_lut = cvCreateImage(size0, IPL_DEPTH_8U, chanels);//原图 几通道就建立  
	memset(image_result_lut->imageData, 0, result_image_height * result_image_width*chanels);
	int step = image_result_lut->widthStep / sizeof(uchar);//自动计算一行多少元素 
	/****malloc look up table****/
#endif

	

#if YUV
	ret = my_load_image_yuv( p_avm_param);

	//int width = 1280;
	//int height = 720;
	//FILE* p_file;
	//FILE*	p = fopen("image1.bin", "rb");
	////uchar*a = (uchar *)malloc(width*height * 2 * 1);
	//uchar a[1280 * 720 * 2];
	//load_image(p, a, width, height, IMAGE_TYPE_YUV422);
	//for (int i = 0; i < 720; i++)
	//{
	//	for (int j = 0; j < 1280; j++)
	//	{
	//		//fwrite
	//		FILE *fid;
	//		char res[100];
	//		sprintf(res, "77777.yuv");
	//		fid = fopen(res, "wb");
	//		fwrite(a, sizeof(UInt8_t),
	//		1280*720 * 2 * sizeof(UInt8_t), fid);
	//		fclose(fid);
	//		printf("%d,%d\n",a[i * 1280 * 2 + j * 2],
	//			a[i * 1280 * 2 + j * 2 + 1]);
	//	}
	//	int b = 4 - 1;

	//}


#endif

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
#if RGB
				resolve_data_from_lut_new(shift, image[0], rgb, p_avm_param, p_lut);
				
				if (image_result_lut->nChannels == 1)
				{
					image_result_lut->imageData[i *  step + j* chanels] += (uchar)(rgb[0]);
				}
				else
				{
					image_result_lut->imageData[i *  step + j* chanels] += (uchar)(rgb[0]);
					image_result_lut->imageData[i *  step + j* chanels + 1] += (uchar)(rgb[1]);
					image_result_lut->imageData[i *  step + j* chanels + 2] += (uchar)(rgb[2]);
				}
#endif
#if YUV
				rgb[0] = 0;
				rgb[1] = 0;
				resolve_data_from_lut_new_yuv(shift,  rgb, p_avm_param, p_lut,CAM_FRONT);
				p_data[i* result_image_width * 2 + j * 2] += (uchar)(rgb[0]);
				p_data[i* result_image_width * 2 + j * 2 + 1] += (uchar)(rgb[1]);
#endif
			}

			/*************rear view*********************************/

			if ((i >= (car_logo_rect.y + car_logo_rect.height)) && (i < result_image_height))
			{
				int shift = camera_size[0] + (i - (car_logo_rect.y + car_logo_rect.height)) * result_image_width * 2 + j * 2;
				
				int rgb[3];
#if RGB
				resolve_data_from_lut_new(shift, image[1], rgb, p_avm_param,p_lut);
				
				if (image_result_lut->nChannels == 1)
				{
					image_result_lut->imageData[i *  step + j* chanels] += (uchar)(rgb[0]);
				}
				else
				{
					image_result_lut->imageData[i *  step + j* chanels] += (uchar)(rgb[0]);
					image_result_lut->imageData[i *  step + j* chanels + 1] += (uchar)(rgb[1]);
					image_result_lut->imageData[i *  step + j* chanels + 2] += (uchar)(rgb[2]);
				}
#endif
#if YUV
				resolve_data_from_lut_new_yuv(shift, rgb, p_avm_param, p_lut,CAM_REAR);
				p_data[i* result_image_width * 2 + j * 2] += (uchar)(rgb[0]);
				p_data[i* result_image_width * 2 + j * 2 + 1] += (uchar)(rgb[1]);
#endif
			}
#endif
#if 1
			/*******************************************left view********************************/
			if (j < car_logo_rect.x)
			{
				int shift = camera_size[0] + camera_size[1] + i * car_logo_rect.x * 2 + j * 2;
				
				int rgb[3];
#if RGB
				resolve_data_from_lut_new(shift, image[2], rgb, p_avm_param,p_lut);
				
				if (image_result_lut->nChannels == 1)
				{
					image_result_lut->imageData[i *  step + j* chanels] += (uchar)(rgb[0]);
				}
				else
				{
					image_result_lut->imageData[i *  step + j* chanels] += (uchar)(rgb[0]);
					image_result_lut->imageData[i *  step + j* chanels + 1] += (uchar)(rgb[1]);
					image_result_lut->imageData[i *  step + j* chanels + 2] += (uchar)(rgb[2]);
				}
#endif
#if YUV
				resolve_data_from_lut_new_yuv(shift, rgb, p_avm_param, p_lut,CAM_LEFT);

				p_data[i* result_image_width * 2 + j * 2] += (uchar)(rgb[0]);
				p_data[i* result_image_width * 2 + j * 2 + 1] += (uchar)(rgb[1]);
#endif
			}
			/*************************right view****************/
			if (j >= (car_logo_rect.x + car_logo_rect.width) && j < result_image_width)
			{
				int shift = camera_size[0] + camera_size[1] + camera_size[2] 
					+ i * (result_image_width - car_logo_rect.x - car_logo_rect.width) * 2 
					+ (j - car_logo_rect.x - car_logo_rect.width) * 2;

	
				int rgb[3];
#if RGB
				resolve_data_from_lut_new(shift, image[3], rgb, p_avm_param,p_lut);
				
				if (image_result_lut->nChannels == 1)
				{
					image_result_lut->imageData[i *  step + j* chanels] += (uchar)(rgb[0]);
				}
				else
				{
					image_result_lut->imageData[i *  step + j* chanels] += (uchar)(rgb[0]);
					image_result_lut->imageData[i *  step + j* chanels + 1] += (uchar)(rgb[1]);
					image_result_lut->imageData[i *  step + j* chanels + 2] += (uchar)(rgb[2]);
				}
#endif
#if YUV
				resolve_data_from_lut_new_yuv(shift, rgb, p_avm_param, p_lut, CAM_RIGHT);
				p_data[i* result_image_width * 2 + j * 2] += rgb[0];
				p_data[i* result_image_width * 2 + j * 2 + 1] += rgb[1];
#endif
			}
#endif
		}

	}

#if RGB
	
	sprintf(result_name, "%s/%s_%dX%d.bmp", result_image_path, view_name[view_id], result_image_width, result_image_height);
	cvSaveImage(result_name, image_result_lut);
#endif

#if YUV
	FILE *fid;
	sprintf(result_name, "%s/%s_%dX%d.yuv", result_image_path, view_name[view_id], result_image_width, result_image_height);
	fid = fopen(result_name, "wb");
	fwrite(p_data, sizeof(UInt8_t), result_image_height*result_image_width * 2 * sizeof(UInt8_t), fid);
	fclose(fid);
#endif
#if RGB
	cvReleaseImage(&image_result_lut);
	cvReleaseImage(&image[0]);
	cvReleaseImage(&image[1]);
	cvReleaseImage(&image[2]);
	cvReleaseImage(&image[3]);
#endif
	return ret;
}




/************** resolve_4_view_data_from_LUT***********************/
/*
* Function Name:				 resolve_4_view_data_from_LUT
* Function Function:			resolve_4_view_data_from_LUT
*       p_avm_param;
* Note:
*     the interface function should be called at the beginning of this module
* Revision:
*     Create by Cheng.Li on 2018/5/2.
*/
AVM_Error_Code_E write_gpu_LUT(IN  P_Avm_Param_S p_avm_param, IN int view_id)
{
	AVM_Error_Code_E ret = ERROR_NONE;
	CvSize size0;
	Int32_t result_image_height;
	Int32_t result_image_width;
	CvRect car_logo_rect;
	UInt32_t * p_lut = NULL;
	Int32_t camera_size[4];
	char result_name[MAX_ARRAY_SIZE];
	UInt8_t*		p_data;
	if (view_id == _4_VIEW_BIG)//大图
	{
		p_lut = p_avm_param->p_avm_lut->p_lut_junc_view_4_big;
		result_image_height = p_avm_param->p_avm_config_param->avm_2d_param.junc_view_4_param.result_big_param.result_image_height;
		result_image_width = p_avm_param->p_avm_config_param->avm_2d_param.junc_view_4_param.result_big_param.result_image_width;
		size0 = cvSize(result_image_width, result_image_height);//
		car_logo_rect = p_avm_param->p_avm_config_param->avm_2d_param.junc_view_4_param.result_big_param.car_logo_rect;
		p_data = p_avm_param->p_avm_image->result_image.result_image_junc_view_4_big->p_data;
		for (Int32_t cam_id = 0; cam_id < CAMERA_NUM; cam_id++)
		{
			camera_size[cam_id] = p_avm_param->p_avm_config_param->avm_2d_param.junc_view_4_param.result_big_param.lut_size[cam_id] * 2;
		}
	}
	if (view_id == _4_VIEW_SMALL)//小图
	{
		result_image_height = p_avm_param->p_avm_config_param->avm_2d_param.junc_view_4_param.result_small_param.result_image_height;
		result_image_width = p_avm_param->p_avm_config_param->avm_2d_param.junc_view_4_param.result_small_param.result_image_width;
		size0 = cvSize(result_image_width, result_image_height);//
		p_lut = p_avm_param->p_avm_lut->p_lut_junc_view_4_small;
		car_logo_rect = p_avm_param->p_avm_config_param->avm_2d_param.junc_view_4_param.result_small_param.car_logo_rect;
		p_data = p_avm_param->p_avm_image->result_image.result_image_junc_view_4_small->p_data;
		for (Int32_t cam_id = 0; cam_id < CAMERA_NUM; cam_id++)
		{
			camera_size[cam_id] = p_avm_param->p_avm_config_param->avm_2d_param.junc_view_4_param.result_small_param.lut_size[cam_id] * 2;
		}
	}
#if RGB
	IplImage* image[4] = { NULL, NULL, NULL, NULL };
	ret = my_load_image(image, p_avm_param);

	int chanels = image[2]->nChannels;//几通道
	IplImage *image_result_lut = cvCreateImage(size0, IPL_DEPTH_8U, chanels);//原图 几通道就建立  
	memset(image_result_lut->imageData, 0, result_image_height * result_image_width*chanels);
	int step = image_result_lut->widthStep / sizeof(uchar);//自动计算一行多少元素 
	/****malloc look up table****/
#endif



#if YUV
	ret = my_load_image_yuv(p_avm_param);

#endif

#if 1
	//

	//UInt32_t *gpu_front = (UInt32_t *)malloc(car_logo_rect.y*result_image_width*sizeof(UInt32_t));//第一版 的 s32V前后左右必须对称
	//UInt32_t *gpu_back = (UInt32_t *)malloc(car_logo_rect.y*result_image_width*sizeof(UInt32_t));
	//UInt32_t *gpu_left = (UInt32_t *)malloc(car_logo_rect.x*result_image_height*sizeof(UInt32_t));
	//UInt32_t *gpu_right = (UInt32_t *)malloc(car_logo_rect.x*result_image_height*sizeof(UInt32_t));
	UInt32_t *gpu_front = (UInt32_t *)malloc(car_logo_rect.y*result_image_width*sizeof(UInt32_t));//第一版 的 s32V前后左右必须对称
	UInt32_t *gpu_back = (UInt32_t *)malloc((result_image_height - car_logo_rect.y - car_logo_rect.height)*result_image_width*sizeof(UInt32_t));
	UInt32_t *gpu_left = (UInt32_t *)malloc(car_logo_rect.x*result_image_height*sizeof(UInt32_t));
	UInt32_t *gpu_right = (UInt32_t *)malloc((result_image_width - car_logo_rect.x - car_logo_rect.width)*result_image_height*sizeof(UInt32_t));


#endif
	for (int i = 0; i < result_image_height; i++)
	{
		for (int j = 0; j < result_image_width; j++)
		{
#if 1

			/************front view*******************************/
			if (i < car_logo_rect.y)
			{
				int shift = i * result_image_width * 2 + j * 2;
				int shift_gpu = i * result_image_width * 2 + j * 2;
				int rgb[3];
#if RGB
				resolve_data_from_lut_new(shift, image[0], rgb, p_avm_param, p_lut);
				resolve_data_from_lut_new_gpu(shift, shift_gpu, image[0], rgb, p_avm_param, p_lut, gpu_front);
				if (image_result_lut->nChannels == 1)
				{
					image_result_lut->imageData[i *  step + j* chanels] += (uchar)(rgb[0]);
				}
				else
				{
					image_result_lut->imageData[i *  step + j* chanels] += (uchar)(rgb[0]);
					image_result_lut->imageData[i *  step + j* chanels + 1] += (uchar)(rgb[1]);
					image_result_lut->imageData[i *  step + j* chanels + 2] += (uchar)(rgb[2]);
				}
#endif
#if YUV
				rgb[0] = 0;
				rgb[1] = 0;
				resolve_data_from_lut_new_yuv(shift, rgb, p_avm_param, p_lut, CAM_FRONT);
				p_data[i* result_image_width * 2 + j * 2] += (uchar)(rgb[0]);
				p_data[i* result_image_width * 2 + j * 2 + 1] += (uchar)(rgb[1]);
#endif
			}

			/*************rear view*********************************/

			if ((i >= (car_logo_rect.y + car_logo_rect.height)) && (i < result_image_height))
			{
				int shift = camera_size[0] + (i - (car_logo_rect.y + car_logo_rect.height)) * result_image_width * 2 + j * 2;
				int shift_gpu = (i - (car_logo_rect.y + car_logo_rect.height)) * result_image_width * 2 + j * 2;
				int rgb[3];
#if RGB
				resolve_data_from_lut_new(shift, image[1], rgb, p_avm_param, p_lut);
				resolve_data_from_lut_new_gpu(shift, shift_gpu, image[1], rgb, p_avm_param, p_lut, gpu_back);
				if (image_result_lut->nChannels == 1)
				{
					image_result_lut->imageData[i *  step + j* chanels] += (uchar)(rgb[0]);
				}
				else
				{
					image_result_lut->imageData[i *  step + j* chanels] += (uchar)(rgb[0]);
					image_result_lut->imageData[i *  step + j* chanels + 1] += (uchar)(rgb[1]);
					image_result_lut->imageData[i *  step + j* chanels + 2] += (uchar)(rgb[2]);
				}
#endif
#if YUV
				resolve_data_from_lut_new_yuv(shift, rgb, p_avm_param, p_lut, CAM_REAR);
				p_data[i* result_image_width * 2 + j * 2] += (uchar)(rgb[0]);
				p_data[i* result_image_width * 2 + j * 2 + 1] += (uchar)(rgb[1]);
#endif
			}
#endif
#if 1
			/*******************************************left view********************************/
			if (j < car_logo_rect.x)
			{
				int shift = camera_size[0] + camera_size[1] + i * car_logo_rect.x * 2 + j * 2;
				int shift_gpu = i * car_logo_rect.x * 2 + j * 2;
				int rgb[3];
#if RGB         
				resolve_data_from_lut_new(shift, image[2], rgb, p_avm_param, p_lut);
				resolve_data_from_lut_new_gpu(shift, shift_gpu, image[2], rgb, p_avm_param, p_lut, gpu_left);
				if (image_result_lut->nChannels == 1)
				{
					image_result_lut->imageData[i *  step + j* chanels] += (uchar)(rgb[0]);
				}
				else
				{
					image_result_lut->imageData[i *  step + j* chanels] += (uchar)(rgb[0]);
					image_result_lut->imageData[i *  step + j* chanels + 1] += (uchar)(rgb[1]);
					image_result_lut->imageData[i *  step + j* chanels + 2] += (uchar)(rgb[2]);
				}
#endif
#if YUV
				resolve_data_from_lut_new_yuv(shift, rgb, p_avm_param, p_lut, CAM_LEFT);

				p_data[i* result_image_width * 2 + j * 2] += (uchar)(rgb[0]);
				p_data[i* result_image_width * 2 + j * 2 + 1] += (uchar)(rgb[1]);
#endif
			}
			/*************************right view****************/
			if (j >= (car_logo_rect.x + car_logo_rect.width) && j < result_image_width)
			{
				int shift = camera_size[0] + camera_size[1] + camera_size[2]
					+ i * (result_image_width - car_logo_rect.x - car_logo_rect.width) * 2
					+ (j - car_logo_rect.x - car_logo_rect.width) * 2;

				int shift_gpu = i * (result_image_width - car_logo_rect.x - car_logo_rect.width) * 2
					+ (j - car_logo_rect.x - car_logo_rect.width) * 2;
				int rgb[3];
#if RGB
				resolve_data_from_lut_new(shift, image[3], rgb, p_avm_param, p_lut);
				resolve_data_from_lut_new_gpu(shift, shift_gpu, image[3], rgb, p_avm_param, p_lut, gpu_right);
				if (image_result_lut->nChannels == 1)
				{
					image_result_lut->imageData[i *  step + j* chanels] += (uchar)(rgb[0]);
				}
				else
				{
					image_result_lut->imageData[i *  step + j* chanels] += (uchar)(rgb[0]);
					image_result_lut->imageData[i *  step + j* chanels + 1] += (uchar)(rgb[1]);
					image_result_lut->imageData[i *  step + j* chanels + 2] += (uchar)(rgb[2]);
				}
#endif
#if YUV
				resolve_data_from_lut_new_yuv(shift, rgb, p_avm_param, p_lut, CAM_RIGHT);
				p_data[i* result_image_width * 2 + j * 2] += rgb[0];
				p_data[i* result_image_width * 2 + j * 2 + 1] += rgb[1];
#endif
			}
#endif
		}

	}
	char lut_path[MAX_ARRAY_SIZE];

	
	

	FILE *gpu;
	/*******存储大图的二进制表********/
	sprintf(lut_path, "%s/Lut_Front.bin", lut_file_path);
	gpu = fopen(lut_path, "wb");
	fwrite(gpu_front, sizeof(UInt32_t), car_logo_rect.y*result_image_width, gpu);
	fclose(gpu);

	sprintf(lut_path, "%s/Lut_Back.bin", lut_file_path);
	gpu = fopen(lut_path, "wb");
	fwrite(gpu_back, sizeof(UInt32_t), car_logo_rect.y*result_image_width, gpu);
	fclose(gpu);

	sprintf(lut_path, "%s/Lut_Left.bin", lut_file_path);
	gpu = fopen(lut_path, "wb");
	fwrite(gpu_left, sizeof(UInt32_t), car_logo_rect.x*result_image_height, gpu);
	fclose(gpu);

	sprintf(lut_path, "%s/Lut_Right.bin", lut_file_path);
	gpu = fopen(lut_path, "wb");
	fwrite(gpu_right, sizeof(UInt32_t), car_logo_rect.x*result_image_height, gpu);
	fclose(gpu);

#if RGB

	sprintf(result_name, "%s/GPU_%s_%dX%d.bmp", result_image_path, view_name[view_id], result_image_width, result_image_height);
	cvSaveImage(result_name, image_result_lut);
#endif

#if YUV
	/*FILE *fid;
	sprintf(result_name, "%s/%s_%dX%d.yuv", result_image_path, view_name[view_id], result_image_width, result_image_height);
	fid = fopen(result_name, "wb");
	fwrite(p_data, sizeof(UInt8_t), result_image_height*result_image_width * 2 * sizeof(UInt8_t), fid);
	fclose(fid);*/
#endif
#if RGB
	cvReleaseImage(&image_result_lut);
	cvReleaseImage(&image[0]);
	cvReleaseImage(&image[1]);
	cvReleaseImage(&image[2]);
	cvReleaseImage(&image[3]);
#endif
	return ret;
}



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
	Float32_t image_point0[2];
	unsigned int temp0 = p_avm_param->p_avm_lut->p_lut_junc_view_3_front[shift];
	unsigned int temp1 = p_avm_param->p_avm_lut->p_lut_junc_view_3_front[shift + 1];
	int u_int = temp0 >> 21;// v_int = (int)image_point[1];
	int v_int = temp0 >> 10;
	v_int = v_int & 0x7FF;
	Float32_t u_point = (Float32_t)(temp1 >> 24) / 255.000, v_point = ((Float32_t)((temp1 >> 16) & 255)) / 255.000;
	image_point0[0] = (Float32_t)u_int + u_point;
	image_point0[1] = (Float32_t)v_int + v_point;
	unsigned int weight_int = temp0 >> 2;
	weight_int = weight_int & 0x00FF;
	Float32_t weight_Float32_t = Float32_t(weight_int) / (Float32_t)255;
	bilinear_interpolation(image, rgb, image_point0[1], image_point0[0]);
	rgb[0] = (uchar)(Float32_t(rgb[0])*weight_Float32_t);

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
	AVM_Error_Code_E resolve_3_front_data_from_LUT(IN  P_Avm_Param_S p_avm_param, IN int view_id)
{
	AVM_Error_Code_E ret = ERROR_NONE;

	Int32_t result_image_height;
	Int32_t result_image_width;
	CvRect car_logo_rect;
	UInt32_t * p_lut = NULL;
	Int32_t camera_size[CAMERA_NUM];
	char result_name[MAX_ARRAY_SIZE];
	UInt8_t*		p_data;
	if (view_id == _3_VIEW_FRONT)
	{
		result_image_height = p_avm_param->p_avm_config_param->avm_2d_param.junc_view_3_front_param.result_3_front_param.result_image_height;
		result_image_width = p_avm_param->p_avm_config_param->avm_2d_param.junc_view_3_front_param.result_3_front_param.result_image_width;
		car_logo_rect = p_avm_param->p_avm_config_param->avm_2d_param.junc_view_3_front_param.result_3_front_param.car_logo_rect;
		p_lut =p_avm_param->p_avm_lut->p_lut_junc_view_3_front;
		p_data = p_avm_param->p_avm_image->result_image.result_image_junc_view_3_front->p_data;
		for (Int32_t cam_id = 0; cam_id < CAMERA_NUM; cam_id++)
		{
			camera_size[cam_id] = p_avm_param->p_avm_config_param->avm_2d_param.junc_view_3_front_param.result_3_front_param.lut_size[cam_id] * 2;
		}
	}
	if (view_id == _3_VIEW_REAR)
	{
		result_image_height = p_avm_param->p_avm_config_param->avm_2d_param.junc_view_3_rear_param.result_3_rear_param.result_image_height;
		result_image_width = p_avm_param->p_avm_config_param->avm_2d_param.junc_view_3_rear_param.result_3_rear_param.result_image_width;
		car_logo_rect = p_avm_param->p_avm_config_param->avm_2d_param.junc_view_3_rear_param.result_3_rear_param.car_logo_rect;
		p_lut = p_avm_param->p_avm_lut->p_lut_junc_view_3_rear;
		p_data = p_avm_param->p_avm_image->result_image.result_image_junc_view_3_rear->p_data;
		for (Int32_t cam_id = 0; cam_id < CAMERA_NUM; cam_id++)
		{
			camera_size[cam_id] = p_avm_param->p_avm_config_param->avm_2d_param.junc_view_3_rear_param.result_3_rear_param.lut_size[cam_id] * 2;
		}
	}



#if RGB
	IplImage* image[4] = { NULL, NULL, NULL, NULL };
	ret = my_load_image(image, p_avm_param);

	CvSize size0 = cvSize(result_image_width, result_image_height);//
	int chanels = image[2]->nChannels;//几通道
	IplImage *image_result_lut = cvCreateImage(size0, IPL_DEPTH_8U, chanels);//原图 几通道就建立  
	memset(image_result_lut->imageData, 0, result_image_height * result_image_width*chanels);
	int step = image_result_lut->widthStep / sizeof(uchar);//自动计算一行多少元素 
	/****malloc look up table****/
	IplImage *image_result_3_front_lut = cvCreateImage(size0, IPL_DEPTH_8U, 1);
	memset(image_result_3_front_lut->imageData, 0, result_image_width *result_image_height);
#endif

#if YUV
	ret = my_load_image_yuv(p_avm_param);
#endif

	

	for (int i = 0; i < result_image_height; i++)
	{
		for (int j = 0; j <result_image_width; j++)
		{
			if (view_id == _3_VIEW_FRONT)
			{

				/*************************front view**********************************/
				if (i < car_logo_rect.y)
				{
					int shift = i * result_image_width * 2 + j * 2;
					int rgb[3];
			
#if RGB
					resolve_data_from_lut_new(shift, image[0], rgb, p_avm_param, p_lut);
					if (image_result_lut->nChannels == 1)
					{
						image_result_lut->imageData[i *  step + j* chanels] += (uchar)(rgb[0]);
					}
					else
					{
						image_result_lut->imageData[i *  step + j* chanels] += (uchar)(rgb[0]);
						image_result_lut->imageData[i *  step + j* chanels + 1] += (uchar)(rgb[1]);
						image_result_lut->imageData[i *  step + j* chanels + 2] += (uchar)(rgb[2]);
					}
#endif
#if YUV
					resolve_data_from_lut_new_yuv(shift, rgb, p_avm_param, p_lut, CAM_FRONT);
					p_data[i* result_image_width * 2 + j * 2] += (uchar)(rgb[0]);
					p_data[i* result_image_width * 2 + j * 2 + 1] += (uchar)(rgb[1]);
#endif
				}

			}
			if (view_id == _3_VIEW_REAR)
			{
				/*************************rear view**********************************/
				if ((i >= (car_logo_rect.y + car_logo_rect.height)))
				{
					int shift = (i - (car_logo_rect.y + car_logo_rect.height))
						* result_image_width * 2 + j * 2;
					int rgb[3];

#if RGB
					resolve_data_from_lut_new(shift, image[1], rgb, p_avm_param,p_lut);
					if (image_result_lut->nChannels == 1)
					{
						image_result_lut->imageData[i *  step + j* chanels] += (uchar)(rgb[0]);
					}
					else
					{
						image_result_lut->imageData[i *  step + j* chanels] += (uchar)(rgb[0]);
						image_result_lut->imageData[i *  step + j* chanels + 1] += (uchar)(rgb[1]);
						image_result_lut->imageData[i *  step + j* chanels + 2] += (uchar)(rgb[2]);
					}
#endif
#if YUV
					resolve_data_from_lut_new_yuv(shift, rgb, p_avm_param, p_lut, CAM_REAR);
					p_data[i* result_image_width * 2 + j * 2] += (uchar)(rgb[0]);
					p_data[i* result_image_width * 2 + j * 2 + 1] += (uchar)(rgb[1]);
#endif

				}
			}
			/*******************************************left view********************************/
			if (j < car_logo_rect.x)
			{
				int shift = camera_size[0] + camera_size[1] + i * car_logo_rect.x * 2 + j * 2;
				int rgb[3];
			
#if RGB
				resolve_data_from_lut_new(shift, image[2], rgb, p_avm_param,p_lut);
				if (image_result_lut->nChannels == 1)
				{
					image_result_lut->imageData[i *  step + j* chanels] += (uchar)(rgb[0]);
				}
				else
				{
					image_result_lut->imageData[i *  step + j* chanels] += (uchar)(rgb[0]);
					image_result_lut->imageData[i *  step + j* chanels + 1] += (uchar)(rgb[1]);
					image_result_lut->imageData[i *  step + j* chanels + 2] += (uchar)(rgb[2]);
				}
#endif
#if YUV
				resolve_data_from_lut_new_yuv(shift, rgb, p_avm_param, p_lut, CAM_LEFT);
				p_data[i* result_image_width * 2 + j * 2] += (uchar)(rgb[0]);
				p_data[i* result_image_width * 2 + j * 2 + 1] += (uchar)(rgb[1]);
#endif
			}
			/*************************right view****************/
			if (j >= (car_logo_rect.x + car_logo_rect.width) && j <result_image_width)
			{
				Int32_t shift = camera_size[0] + camera_size[1] + camera_size[2] + i * (result_image_width - car_logo_rect.x - car_logo_rect.width) * 2
							+ (j - car_logo_rect.x - car_logo_rect.width) * 2;
				Int32_t rgb[3];
				
#if RGB
				resolve_data_from_lut_new(shift, image[3], rgb, p_avm_param,p_lut);
				if (image_result_lut->nChannels == 1)
				{
					image_result_lut->imageData[i *  step + j* chanels] += (uchar)(rgb[0]);
				}
				else
				{
					image_result_lut->imageData[i *  step + j* chanels] += (uchar)(rgb[0]);
					image_result_lut->imageData[i *  step + j* chanels + 1] += (uchar)(rgb[1]);
					image_result_lut->imageData[i *  step + j* chanels + 2] += (uchar)(rgb[2]);
				}
#endif
#if YUV
				resolve_data_from_lut_new_yuv(shift, rgb, p_avm_param, p_lut, CAM_RIGHT);
				p_data[i* result_image_width * 2 + j * 2] += (uchar)(rgb[0]);
				p_data[i* result_image_width * 2 + j * 2 + 1] += (uchar)(rgb[1]);
#endif
				
			}
		}

	}


#if YUV
	FILE *fid;
	sprintf(result_name, "%s/%s_%dX%d.yuv", result_image_path, view_name[view_id], result_image_width, result_image_height);
	fid = fopen(result_name, "wb");
	fwrite(p_data, sizeof(UInt8_t), result_image_height*result_image_width * 2 * sizeof(UInt8_t), fid);
	fclose(fid);
#endif


#if RGB
	sprintf(result_name, "%s/%s_%dX%d.bmp", result_image_path, view_name[view_id],
		result_image_width, result_image_height);
	cvSaveImage(result_name, image_result_lut);
	
	cvReleaseImage(&image_result_lut);
	cvReleaseImage(&image[0]);
	cvReleaseImage(&image[1]);
	cvReleaseImage(&image[2]);
	cvReleaseImage(&image[3]);
#endif

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
	Float32_t image_point0[2];
	unsigned int temp0 = p_avm_param->p_avm_lut->p_lut_junc_view_3_rear[shift];
	unsigned int temp1 = p_avm_param->p_avm_lut->p_lut_junc_view_3_rear[shift + 1];
	int u_int = temp0 >> 21;// v_int = (int)image_point[1];
	int v_int = temp0 >> 10;
	v_int = v_int & 0x7FF;
	Float32_t u_point = (Float32_t)(temp1 >> 24) / 255.000, v_point = ((Float32_t)((temp1 >> 16) & 255)) / 255.000;
	image_point0[0] = (Float32_t)u_int + u_point;
	image_point0[1] = (Float32_t)v_int + v_point;
	unsigned int weight_int = temp0 >> 2;
	weight_int = weight_int & 0x00FF;
	Float32_t weight_Float32_t = Float32_t(weight_int) / (Float32_t)255;
	bilinear_interpolation(image, rgb, image_point0[1], image_point0[0]);
	rgb[0] = (uchar)(Float32_t(rgb[0])*weight_Float32_t);

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

	ret = my_load_image(image, p_avm_param);

	Junc_View_3_Rear_Param_S	junc_view_3_rear_param = p_avm_param->p_avm_config_param
		->avm_2d_param.junc_view_3_rear_param;
	CvRect car_logo_rect = junc_view_3_rear_param.result_3_rear_param.car_logo_rect;
	Vehicle_Param_S	 vehicle_param = p_avm_param->p_avm_config_param->vehicle_param;
	Float32_t pixel_row_dis = (vehicle_param.vehicle_length + junc_view_3_rear_param.common_config.rear_view_length)
		/ (Float32_t)(junc_view_3_rear_param.result_3_rear_param.result_image_height);//pixel distance
	Float32_t pixel_col_dis = (vehicle_param.vehicle_width + junc_view_3_rear_param.common_config.left_view_length 
		+ junc_view_3_rear_param.common_config.right_view_length)
		/ (Float32_t)(junc_view_3_rear_param.result_3_rear_param.result_image_height);//pixel distance
	

	UInt32_t *p_lut_static_line = p_avm_param->p_avm_dis_line->p_static_dis_line_3_rear;
	UInt32_t *p_lut_dynamic_line = p_avm_param->p_avm_dis_line->p_dynamic_dis_line_3_rear;


	CvSize size0 = cvSize(junc_view_3_rear_param.result_3_rear_param.result_image_width,
		junc_view_3_rear_param.result_3_rear_param.result_image_height);//

	IplImage *image_result_3_front_lut = cvCreateImage(size0, IPL_DEPTH_8U, 1);
	memset(image_result_3_front_lut->imageData, 0, junc_view_3_rear_param.result_3_rear_param.result_image_width 
		* junc_view_3_rear_param.result_3_rear_param.result_image_height);

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
				int shift = (i - (car_logo_rect.y + car_logo_rect.height))
					* junc_view_3_rear_param.result_3_rear_param.result_image_width * 2 + j * 2;
				int rgb[3];
				//_3_rear_resolve_data_from_lut(shift, image[1], rgb, p_avm_param);
				resolve_data_from_lut_new(shift, image[1], rgb, p_avm_param, p_avm_param->p_avm_lut->p_lut_junc_view_3_rear);
				image_result_3_front_lut->imageData[i
					* junc_view_3_rear_param.result_3_rear_param.result_image_width + j] += (uchar)(rgb[0]);

			}
			/*******************************************left view********************************/
			if (j < car_logo_rect.x)
			{
				int shift = camera_size[0] + camera_size[1] + i * car_logo_rect.x * 2 + j * 2;
				int rgb[3];
				//_3_rear_resolve_data_from_lut(shift, image[2], rgb, p_avm_param);

				resolve_data_from_lut_new(shift, image[2], rgb, p_avm_param, p_avm_param->p_avm_lut->p_lut_junc_view_3_rear);

				image_result_3_front_lut->imageData[i * junc_view_3_rear_param.result_3_rear_param.result_image_width + j]
					= image_result_3_front_lut->imageData[i * junc_view_3_rear_param.result_3_rear_param.result_image_width
					+ j] + (uchar)(rgb[0]);

			}
			/*************************right view****************/
			if (j >= (car_logo_rect.x + car_logo_rect.width) && j <junc_view_3_rear_param.result_3_rear_param.result_image_width)
			{
				int shift = camera_size[0] + camera_size[1] + camera_size[2]
					+ i * (junc_view_3_rear_param.result_3_rear_param.result_image_width 
					- car_logo_rect.x - car_logo_rect.width) * 2
					+ (j - car_logo_rect.x - car_logo_rect.width) * 2;
				int rgb[3];
				//_3_rear_resolve_data_from_lut(shift, image[3], rgb, p_avm_param);
				resolve_data_from_lut_new(shift, image[3], rgb, p_avm_param, p_avm_param->p_avm_lut->p_lut_junc_view_3_rear);
				image_result_3_front_lut->imageData[i *junc_view_3_rear_param.result_3_rear_param.result_image_width + j]
					= image_result_3_front_lut->imageData[i * junc_view_3_rear_param.result_3_rear_param.result_image_width 
					+ j] + (uchar)(rgb[0]);
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
			if (pic_coordinate[0] < junc_view_3_rear_param.result_3_rear_param.result_image_height
				&& pic_coordinate[0]>0 && pic_coordinate[1] < junc_view_3_rear_param.result_3_rear_param.result_image_width&&pic_coordinate[1]>0)
			{
				image_result_3_front_lut->imageData[pic_coordinate[0] * junc_view_3_rear_param.result_3_rear_param.result_image_width + pic_coordinate[1]] = (uchar)(rgb[0]);
			}
		}
	}

#endif

	
	char result_name[MAX_ARRAY_SIZE];
	//sprintf(result_name, "%s/image_result_3_back_lut_%dX%d.bmp", result_image_path,
			//junc_view_3_rear_param.result_3_rear_param.result_image_width, junc_view_3_rear_param.result_3_rear_param.result_image_height);
	sprintf(result_name, "%s/%s_%dX%d.bmp", result_image_path, view_name[_3_VIEW_REAR],
		junc_view_3_rear_param.result_3_rear_param.result_image_width, junc_view_3_rear_param.result_3_rear_param.result_image_height);
	cvSaveImage(result_name, image_result_3_front_lut);
	
	cvReleaseImage(&image_result_3_front_lut);
	cvReleaseImage(&image[0]);
	cvReleaseImage(&image[1]);
	cvReleaseImage(&image[2]);
	cvReleaseImage(&image[3]);
	return ret;
}





void front_single_resolve_data(IN int shift, IN IplImage *image, OUT int *rgb, IN  P_Avm_Param_S p_avm_param, OUT UInt32_t *p_lut)
{
	Float32_t image_point0[2];
	//shift = 0;
	//unsigned int temp0 = p_avm_param->p_avm_lut->p_lut_single_view_front[shift];
	unsigned int temp0 = p_lut[shift];
	unsigned int temp1 = p_lut[shift + 1];
	int u_int = temp0 >> 21;// v_int = (int)image_point[1];
	int v_int = temp0 >> 10;
	v_int = v_int & 0x7FF;
	Float32_t u_point = (Float32_t)(temp1 >> 24) / 255.000, v_point = ((Float32_t)((temp1 >> 16) & 255)) / 255.000;
	image_point0[0] = (Float32_t)u_int + u_point;
	image_point0[1] = (Float32_t)v_int + v_point;
	unsigned int weight_int = temp0 >> 2;
	weight_int = weight_int & 0x00FF;
	Float32_t weight_Float32_t = Float32_t(weight_int) / (Float32_t)255;
	bilinear_interpolation(image, rgb, image_point0[1], image_point0[0]);
	rgb[0] = (uchar)(Float32_t(rgb[0]));

}



/*
* Function Name:				reslove_single_view_from_lut
* Function Function:			reslove_single_view_from_lut
* Input:
*      IN P_Avm_Param_S p_avm_param
*      IN int cam_flag
p_avm_param;
* Note:
*		reslove_single_view_from_lut
* Revision:
*     Create by Cheng.Li on 2018/1/17.
*/


AVM_Error_Code_E  reslove_line_lut(IN P_Avm_Param_S p_avm_param, IN int cam_flag)
{
	AVM_Error_Code_E ret = ERROR_NONE;
	Front_Rear_Single_View_Param_S single_view_param;
	CvSize size0;
	UInt32_t* p_lut = NULL;
	UInt32_t *p_lut_static_line = NULL;
	UInt32_t *p_lut_dynamic_line = NULL;
	UInt32_t result_image_height;
	UInt32_t result_image_width;
	if (cam_flag == _1_VIEW_FRONT)
	{
		single_view_param = p_avm_param->p_avm_config_param->avm_single_view_param.front_single_view_param;
		size0 = cvSize(single_view_param.result_image_width, single_view_param.result_image_height);//
		p_lut = p_avm_param->p_avm_lut->p_lut_single_view_front;
		p_lut_static_line = p_avm_param->p_avm_dis_line->p_static_dis_line_front;
		p_lut_dynamic_line = p_avm_param->p_avm_dis_line->p_dynamic_dis_line_front;
		result_image_height = single_view_param.result_image_height;
		result_image_width = single_view_param.result_image_width;
	}
	if (cam_flag == _1_VIEW_REAR)
	{
		single_view_param = p_avm_param->p_avm_config_param->avm_single_view_param.rear_single_view_param;
		size0 = cvSize(single_view_param.result_image_width, single_view_param.result_image_height);//
		p_lut = p_avm_param->p_avm_lut->p_lut_single_view_rear;
		p_lut_static_line = p_avm_param->p_avm_dis_line->p_static_dis_line_rear;
		p_lut_dynamic_line = p_avm_param->p_avm_dis_line->p_dynamic_dis_line_rear;
		result_image_height = single_view_param.result_image_height;
		result_image_width = single_view_param.result_image_width;
	}
#if 1
	CvSize size1;
	char result_name[MAX_ARRAY_SIZE];

	size1 = cvSize(single_view_param.result_image_width, single_view_param.result_image_height);
	IplImage *color_image = cvCreateImage(size1, IPL_DEPTH_8U, 3);
	memset(color_image->imageData, 0, single_view_param.result_image_width*single_view_param.result_image_height * 3);
#endif
	Int32_t every_angle_size_static = DEFAULT_STATIC_DIS_LINE_SIZE;

	//every_angle_size=
	
	/*for (int m = 0; m < 5; m++)
	{
		int a = p_lut_static_line[every_angle_size_static - m];
		printf("点的个数：%d\n", p_lut_static_line[every_angle_size_static - m]);

	}*/
	for (UInt32_t i = 0; i < p_lut_static_line[0]; i++)
	{
		UInt32_t pic_coordinate[2];
		Int32_t rgb[3] = { 0 };
		resolve_data_from_lut_line(i, rgb, p_avm_param, p_lut_static_line, pic_coordinate);
		if (pic_coordinate[0] < result_image_height&&pic_coordinate[1] < result_image_width)
		{
			//image_result_single_front_lut->imageData[pic_coordinate[0] * result_image_width + pic_coordinate[1]] = (uchar)(rgb[0]);
			color_image->imageData[pic_coordinate[0] * single_view_param.result_image_width * 3 + pic_coordinate[1] * 3 + 0] =  (uchar)(rgb[0]); //
			color_image->imageData[pic_coordinate[0] * single_view_param.result_image_width * 3 + pic_coordinate[1] * 3 + 1] =(uchar)(rgb[1]);//
			color_image->imageData[pic_coordinate[0] * single_view_param.result_image_width * 3 + pic_coordinate[1] * 3 + 2] =(uchar)(rgb[2]);//
		}

	}
	sprintf(result_name, "%s/lut_line/距离线%d_%dX%d.bmp", result_image_path, cam_flag, result_image_width, result_image_height);
	cvSaveImage(result_name, color_image);

	   


#if 1
	
	memset(color_image->imageData, 0, single_view_param.result_image_width*single_view_param.result_image_height * 3);
	if (cam_flag == _1_VIEW_FRONT || cam_flag == _1_VIEW_REAR)
	{
		Int32_t dynamic_line_count = p_avm_param->p_avm_config_param->avm_single_view_param.front_single_view_param.
			dis_line_param.dynamic_line_param.dynamic_line_count;
		dynamic_line_count = 31;
		UInt32_t *p_lut_dynamic_line_temp = NULL;
		for (int rotate_angle = -dynamic_line_count; rotate_angle <= dynamic_line_count; rotate_angle++)
		{
			Int32_t every_angle_size = DEFAULT_DYNAMIC_DIS_LINE_SIZE * MAX_DYNAMIC_LINE_COUNT
				/ (p_avm_param->p_avm_config_param->avm_single_view_param.front_single_view_param.
				dis_line_param.dynamic_line_param.dynamic_line_count * 2 + 1);
			int v = (rotate_angle + dynamic_line_count);
			Int32_t address_shift = every_angle_size*(rotate_angle + dynamic_line_count);
			p_lut_dynamic_line_temp = p_lut_dynamic_line + address_shift;

			int a000 = p_lut_dynamic_line_temp[every_angle_size - 1];
			int b00=p_lut_dynamic_line_temp[0];
			//for (UInt32_t i = 0; i < p_lut_dynamic_line_temp[0]; i++)
#if 1
			for (UInt32_t i = 0; i < b00; i++)
			{
				
				UInt32_t pic_coordinate[2];
				int rgb[3] = {0};
				resolve_data_from_lut_line(i, rgb, p_avm_param, p_lut_dynamic_line_temp, pic_coordinate);
				//if (pic_coordinate[0] < result_image_height&&pic_coordinate[1] < result_image_width)
				//{
					//image_result_single_front_lut->imageData[pic_coordinate[0] * result_image_width + pic_coordinate[1]] = (uchar)(rgb[0]);
					color_image->imageData[pic_coordinate[0] * single_view_param.result_image_width * 3 + pic_coordinate[1] * 3 + 0] = (uchar)(rgb[0]);
					color_image->imageData[pic_coordinate[0] * single_view_param.result_image_width * 3 + pic_coordinate[1] * 3 + 1] = (uchar)(rgb[1]);
					color_image->imageData[pic_coordinate[0] * single_view_param.result_image_width * 3 + pic_coordinate[1] * 3 + 2] = (uchar)(rgb[2]);
				//}
			}
#endif
#if 0
			for (UInt32_t i = 0; i <b00; i++)
			{
				UInt32_t pic_coordinate[2];
				int rgb[3] = { 0 };
				resolve_data_from_lut_line(i, rgb, p_avm_param, p_lut_dynamic_line_temp, pic_coordinate);
				//if (pic_coordinate[0] < result_image_height&&pic_coordinate[1] < result_image_width)
				//{
				//image_result_single_front_lut->imageData[pic_coordinate[0] * result_image_width + pic_coordinate[1]] = (uchar)(rgb[0]);
				color_image->imageData[pic_coordinate[0] * single_view_param.result_image_width * 3 + pic_coordinate[1] * 3 + 0] = (uchar)(rgb[0]);
				color_image->imageData[pic_coordinate[0] * single_view_param.result_image_width * 3 + pic_coordinate[1] * 3 + 1] = (uchar)(rgb[1]);
				color_image->imageData[pic_coordinate[0] * single_view_param.result_image_width * 3 + pic_coordinate[1] * 3 + 2] = (uchar)(rgb[2]);
				//}
			}
#endif
			sprintf(result_name, "%s/lut_line/%s_%dX%d_第%d个.bmp", result_image_path, view_name[cam_flag], result_image_width, result_image_height, v);
			cvSaveImage(result_name, color_image);

#if 0
			if (rotate_angle < 0) 
			{
				sprintf(result_name, "%s/lut_line/%s_%dX%d_左转%d度.bmp", result_image_path, view_name[cam_flag], result_image_width, result_image_height, rotate_angle);
				cvSaveImage(result_name, color_image);
			}
			if (rotate_angle > 0)
			{
				sprintf(result_name, "%s/lut_line/%s_%dX%d_右转%d度.bmp", result_image_path, view_name[cam_flag], result_image_width, result_image_height, rotate_angle);
				cvSaveImage(result_name, color_image);
			}
#endif
			memset(color_image->imageData, 0, single_view_param.result_image_width*single_view_param.result_image_height * 3);
		}


	}


#endif




	return ret;

}


#if 0
//YUVz转换

/*
* Function Name:				reslove_single_view_from_lut
* Function Function:			reslove_single_view_from_lut
* Input:
*      IN P_Avm_Param_S p_avm_param
*      IN int cam_flag
p_avm_param;
* Note:
*		reslove_single_view_from_lut
* Revision:
*     Create by Cheng.Li on 2018/1/17.
*/


AVM_Error_Code_E  reslove_single_view_from_lut(IN P_Avm_Param_S p_avm_param, IN int cam_flag)
{

	AVM_Error_Code_E ret = ERROR_NONE;

	Front_Rear_Single_View_Param_S single_view_param_front_rear;
	CvSize size0;
	UInt32_t* p_lut = NULL;
	UInt32_t *p_lut_static_line = NULL;
	UInt32_t *p_lut_dynamic_line = NULL;

	Left_Right_Single_View_Param_S left_right_single_view_param;
	UInt32_t result_image_height;
	UInt32_t result_image_width;
	UInt32_t type;
	if (cam_flag == _1_VIEW_FRONT)
	{
		single_view_param_front_rear = p_avm_param->p_avm_config_param->avm_single_view_param.front_single_view_param;
		size0 = cvSize(single_view_param_front_rear.result_image_width, single_view_param_front_rear.result_image_height);//
		p_lut = p_avm_param->p_avm_lut->p_lut_single_view_front;
		p_lut_static_line = p_avm_param->p_avm_dis_line->p_static_dis_line_front;
		p_lut_dynamic_line = p_avm_param->p_avm_dis_line->p_dynamic_dis_line_front;
		result_image_height = single_view_param_front_rear.result_image_height;
		result_image_width = single_view_param_front_rear.result_image_width;
		type = p_avm_param->p_avm_config_param->avm_single_view_param.front_single_view_param.expand_type;
	}
	if (cam_flag == _1_VIEW_REAR)
	{
		single_view_param_front_rear = p_avm_param->p_avm_config_param->avm_single_view_param.rear_single_view_param;
		size0 = cvSize(single_view_param_front_rear.result_image_width, single_view_param_front_rear.result_image_height);//
		p_lut = p_avm_param->p_avm_lut->p_lut_single_view_rear;
		p_lut_static_line = p_avm_param->p_avm_dis_line->p_static_dis_line_rear;
		p_lut_dynamic_line = p_avm_param->p_avm_dis_line->p_dynamic_dis_line_rear;
		result_image_height = single_view_param_front_rear.result_image_height;
		result_image_width = single_view_param_front_rear.result_image_width;
		type = p_avm_param->p_avm_config_param->avm_single_view_param.rear_single_view_param.expand_type;
	}

	if (cam_flag == _1_VIEW_LEFT)
	{
		left_right_single_view_param = p_avm_param->p_avm_config_param->avm_single_view_param.left_single_view_param;
		size0 = cvSize(left_right_single_view_param.result_image_width, left_right_single_view_param.result_image_height);//
		p_lut = p_avm_param->p_avm_lut->p_lut_single_view_left;
		p_lut_static_line = p_avm_param->p_avm_dis_line->p_static_dis_line_rear;
		p_lut_dynamic_line = p_avm_param->p_avm_dis_line->p_dynamic_dis_line_rear;
		result_image_height = left_right_single_view_param.result_image_height;
		result_image_width = left_right_single_view_param.result_image_width;
		type = p_avm_param->p_avm_config_param->avm_single_view_param.left_single_view_param.expand_type;
	}
	if (cam_flag == _1_VIEW_RIGHT)
	{
		left_right_single_view_param = p_avm_param->p_avm_config_param->avm_single_view_param.right_single_view_param;
		size0 = cvSize(left_right_single_view_param.result_image_width, left_right_single_view_param.result_image_height);//
		p_lut = p_avm_param->p_avm_lut->p_lut_single_view_right;
		p_lut_static_line = p_avm_param->p_avm_dis_line->p_static_dis_line_right;
		p_lut_dynamic_line = p_avm_param->p_avm_dis_line->p_dynamic_dis_line_right;
		result_image_height = left_right_single_view_param.result_image_height;
		result_image_width = left_right_single_view_param.result_image_width;
		type = p_avm_param->p_avm_config_param->avm_single_view_param.right_single_view_param.expand_type;

	}
	if (cam_flag == _1_VIEW_LEFT_RIGHT)
	{
		result_image_height = p_avm_param->p_avm_config_param->avm_single_view_param.left_right_view_param.result_image_height;
		result_image_width = p_avm_param->p_avm_config_param->avm_single_view_param.left_right_view_param.result_image_width;

		size0 = cvSize(result_image_width, result_image_height);//
		p_lut = p_avm_param->p_avm_lut->p_lut_single_view_left_right;
		p_lut_static_line = p_avm_param->p_avm_dis_line->p_static_dis_line_left_right;
		p_lut_dynamic_line = p_avm_param->p_avm_dis_line->p_dynamic_dis_line_left_right;
		type = p_avm_param->p_avm_config_param->avm_single_view_param.left_right_view_param.expand_type;
	}
	IplImage* image[4] = { NULL, NULL, NULL, NULL };
	ret = my_load_image(image, p_avm_param);
	//CHECK_ERROR(ret);
	IplImage *image_result_single_front_lut = cvCreateImage(size0, IPL_DEPTH_8U, 3);
	memset(image_result_single_front_lut->imageData, 0, result_image_width*result_image_height * 3);




	FILE* pf = fopen("back.yuv", "rb");
	uchar* p_data = (uchar*)malloc(1280 * 720 * 2);
	fread(p_data, 1, 1280 * 720 * 2, pf);
	int tempY, tempU, tempV;

	for (UInt32_t i = 0; i < result_image_height; i++)
	{
		for (UInt32_t j = 0; j < result_image_width; j++)
		{

			int shift = i*result_image_width * 2 + j * 2;
			int rgb[3];
			UInt32_t temp0 = p_lut[shift];
			UInt32_t temp1 = p_lut[shift + 1];
			Int32_t v_int = temp0 >> 21;// 第一位存储是图像的高坐标
			Int32_t  u_int = temp0 >> 10;
			u_int = u_int & 0x7FF;

			tempY = p_data[v_int * 1280 * 2 + u_int * 2 + 1];
			tempU = p_data[v_int * 1280 * 2 + ((u_int >> 1) << 1) * 2];
			tempV = p_data[v_int * 1280 * 2 + (((u_int >> 1) << 1) + 1) * 2];

			int Y = tempY;
			int U = tempU - 128;
			int V = tempV - 128;

			int R = (int)(Y + 1.3722 * V);
			int G = (int)(Y - 0.3456 * U - 0.6945 * V);
			int B = (int)(Y + 1.771  * U);

			image_result_single_front_lut->imageData[i* result_image_width * 3 + j * 3 + 0] = (uchar)B;
			image_result_single_front_lut->imageData[i* result_image_width * 3 + j * 3 + 1] = (uchar)G;
			image_result_single_front_lut->imageData[i* result_image_width * 3 + j * 3 + 2] = (uchar)R;

		}
	}





	//
	//for (UInt32_t i = 0; i <result_image_height; i++)
	//{
	//	for (UInt32_t j = 0; j < result_image_width; j++)
	//	{

	//		int shift = i*result_image_width * 2 + j * 2;
	//		int rgb[3];


	//		








	//		UInt32_t temp0 = p_lut[shift];
	//		UInt32_t temp1 = p_lut[shift + 1];
	//		Int32_t v_int = temp0 >> 21;// 第一位存储是图像的高坐标
	//		Int32_t  u_int = temp0 >> 10;
	//		u_int = u_int & 0x7FF;
	//		image_result_single_front_lut->imageData[i *result_image_width * 3 + j * 3 + 0] = image[0]->imageData[v_int * 1280 * 3 + u_int * 3 + 0];

	//		image_result_single_front_lut->imageData[i *result_image_width * 3 + j * 3 + 1] = image[0]->imageData[v_int * 1280 * 3 + u_int * 3 + 1];
	//		image_result_single_front_lut->imageData[i *result_image_width * 3 + j * 3 + 2] = image[0]->imageData[v_int * 1280 * 3 + u_int * 3 + 2];



	//	}
	//}

	if (type == TYPE_BOX_EXPAND)
	{
		char result_name[MAX_ARRAY_SIZE];
		//sprintf(result_name, "%s/单视图BOX展开%d_%dX%d.bmp", result_image_path, cam_flag, result_image_width, result_image_height);
		sprintf(result_name, "%s/%sBOX_%dX%d.bmp", result_image_path, view_name[cam_flag], result_image_width, result_image_height);
		cvSaveImage(result_name, image_result_single_front_lut);
	}
	else
	{
		char result_name[MAX_ARRAY_SIZE];
		//sprintf(result_name, "%s/单视图%d_%dX%d.bmp", result_image_path, cam_flag, result_image_width, result_image_height);
		sprintf(result_name, "%s/%s_%dX%d.bmp", result_image_path, view_name[cam_flag], result_image_width, result_image_height);
		cvSaveImage(result_name, image_result_single_front_lut);

	}

	cvReleaseImage(&image_result_single_front_lut);
	cvReleaseImage(&image[0]);
	cvReleaseImage(&image[1]);
	cvReleaseImage(&image[2]);
	cvReleaseImage(&image[3]);
	return ret;
}



#endif





#if 0
//自己调试
/*
* Function Name:				reslove_single_view_from_lut
* Function Function:			reslove_single_view_from_lut
* Input:
*      IN P_Avm_Param_S p_avm_param
*      IN int cam_flag
p_avm_param;
* Note:
*		reslove_single_view_from_lut
* Revision:
*     Create by Cheng.Li on 2018/1/17.
*/


AVM_Error_Code_E  reslove_single_view_from_lut(IN P_Avm_Param_S p_avm_param, IN int cam_flag)
{

	AVM_Error_Code_E ret = ERROR_NONE;

	Front_Rear_Single_View_Param_S single_view_param_front_rear;
	CvSize size0;
	UInt32_t* p_lut=NULL;
	UInt32_t *p_lut_static_line =NULL;
	UInt32_t *p_lut_dynamic_line = NULL;

	Left_Right_Single_View_Param_S left_right_single_view_param;
	UInt32_t result_image_height;
	UInt32_t result_image_width;
	UInt32_t type;
	if (cam_flag == _1_VIEW_FRONT)
	{
		single_view_param_front_rear = p_avm_param->p_avm_config_param->avm_single_view_param.front_single_view_param;
		size0 = cvSize(single_view_param_front_rear.result_image_width, single_view_param_front_rear.result_image_height);//
		p_lut = p_avm_param->p_avm_lut->p_lut_single_view_front;
		p_lut_static_line = p_avm_param->p_avm_dis_line->p_static_dis_line_front;
		p_lut_dynamic_line = p_avm_param->p_avm_dis_line->p_dynamic_dis_line_front;
		result_image_height = single_view_param_front_rear.result_image_height;
		result_image_width = single_view_param_front_rear.result_image_width;
		type = p_avm_param->p_avm_config_param->avm_single_view_param.front_single_view_param.expand_type;
	}
	if (cam_flag == _1_VIEW_REAR)
	{
		single_view_param_front_rear = p_avm_param->p_avm_config_param->avm_single_view_param.rear_single_view_param;
		size0 = cvSize(single_view_param_front_rear.result_image_width, single_view_param_front_rear.result_image_height);//
		p_lut = p_avm_param->p_avm_lut->p_lut_single_view_rear;
		p_lut_static_line = p_avm_param->p_avm_dis_line->p_static_dis_line_rear;
		p_lut_dynamic_line = p_avm_param->p_avm_dis_line->p_dynamic_dis_line_rear;
		result_image_height = single_view_param_front_rear.result_image_height;
		result_image_width = single_view_param_front_rear.result_image_width;
		type = p_avm_param->p_avm_config_param->avm_single_view_param.rear_single_view_param.expand_type;
	}

	if (cam_flag == _1_VIEW_LEFT)
	{
		left_right_single_view_param = p_avm_param->p_avm_config_param->avm_single_view_param.left_single_view_param;
		size0 = cvSize(left_right_single_view_param.result_image_width, left_right_single_view_param.result_image_height);//
		p_lut = p_avm_param->p_avm_lut->p_lut_single_view_left;
		p_lut_static_line = p_avm_param->p_avm_dis_line->p_static_dis_line_rear;
		p_lut_dynamic_line = p_avm_param->p_avm_dis_line->p_dynamic_dis_line_rear;
		result_image_height = left_right_single_view_param.result_image_height;
		result_image_width = left_right_single_view_param.result_image_width;
		type = p_avm_param->p_avm_config_param->avm_single_view_param.left_single_view_param.expand_type;
	}
	if (cam_flag == _1_VIEW_RIGHT)
	{
		left_right_single_view_param = p_avm_param->p_avm_config_param->avm_single_view_param.right_single_view_param;
		size0 = cvSize(left_right_single_view_param.result_image_width, left_right_single_view_param.result_image_height);//
		p_lut = p_avm_param->p_avm_lut->p_lut_single_view_right;
		p_lut_static_line = p_avm_param->p_avm_dis_line->p_static_dis_line_right;
		p_lut_dynamic_line = p_avm_param->p_avm_dis_line->p_dynamic_dis_line_right;
		result_image_height = left_right_single_view_param.result_image_height;
		result_image_width = left_right_single_view_param.result_image_width;
		type = p_avm_param->p_avm_config_param->avm_single_view_param.right_single_view_param.expand_type;

	}
	if (cam_flag == _1_VIEW_LEFT_RIGHT)
	{
		result_image_height = p_avm_param->p_avm_config_param->avm_single_view_param.left_right_view_param.result_image_height;
		result_image_width = p_avm_param->p_avm_config_param->avm_single_view_param.left_right_view_param.result_image_width;

		size0 = cvSize(result_image_width, result_image_height);//
		p_lut = p_avm_param->p_avm_lut->p_lut_single_view_left_right;
		p_lut_static_line = p_avm_param->p_avm_dis_line->p_static_dis_line_left_right;
		p_lut_dynamic_line = p_avm_param->p_avm_dis_line->p_dynamic_dis_line_left_right;
		type = p_avm_param->p_avm_config_param->avm_single_view_param.left_right_view_param.expand_type;
	}
	IplImage* image[4] = { NULL, NULL, NULL, NULL };
	ret = my_load_image(image, p_avm_param);
	//CHECK_ERROR(ret);
	IplImage *image_result_single_front_lut = cvCreateImage(size0, IPL_DEPTH_8U, 3);
	memset(image_result_single_front_lut->imageData, 0, result_image_width*result_image_height*3);

	for (UInt32_t i = 0; i <result_image_height; i++)
	{
		for (UInt32_t j = 0; j < result_image_width; j++)
		{
			
				int shift = i*result_image_width * 2 + j * 2;
				int rgb[3];


				//int shift = i*result_image_width * 2 + j * 2;
				//int rgb[3];
				
				//resolve_data_from_lut_new(shift, image[cam_flag - 4], rgb, p_avm_param, p_lut);
				//image_result_single_front_lut->imageData[i *result_image_width + j] = (uchar)(rgb[0]);



			
				 UInt32_t temp0 = p_lut[shift];
				 UInt32_t temp1 = p_lut[shift + 1];
				 Int32_t v_int = temp0 >> 21;// 第一位存储是图像的高坐标
				 Int32_t  u_int = temp0 >> 10;
				 u_int = u_int & 0x7FF;
					image_result_single_front_lut->imageData[i *result_image_width*3 + j*3+0] = image[0]->imageData[v_int *1280 * 3 + u_int * 3 + 0];

					image_result_single_front_lut->imageData[i *result_image_width * 3 + j * 3 + 1] = image[0]->imageData[v_int *1280 * 3 + u_int * 3 + 1];
					image_result_single_front_lut->imageData[i *result_image_width * 3 + j * 3 + 2] = image[0]->imageData[v_int *1280 * 3 + u_int * 3 + 2];

					
			
		}
	}

	if (type == TYPE_BOX_EXPAND)
	{
		char result_name[MAX_ARRAY_SIZE];
		//sprintf(result_name, "%s/单视图BOX展开%d_%dX%d.bmp", result_image_path, cam_flag, result_image_width, result_image_height);
		sprintf(result_name, "%s/%sBOX_%dX%d.bmp", result_image_path, view_name[cam_flag], result_image_width, result_image_height);
		cvSaveImage(result_name, image_result_single_front_lut);
	}
	else
	{
		char result_name[MAX_ARRAY_SIZE];
		//sprintf(result_name, "%s/单视图%d_%dX%d.bmp", result_image_path, cam_flag, result_image_width, result_image_height);
		sprintf(result_name, "%s/%s_%dX%d.bmp", result_image_path, view_name[cam_flag], result_image_width, result_image_height);
		cvSaveImage(result_name, image_result_single_front_lut);

	}

	cvReleaseImage(&image_result_single_front_lut);
	cvReleaseImage(&image[0]);
	cvReleaseImage(&image[1]);
	cvReleaseImage(&image[2]);
	cvReleaseImage(&image[3]);
	return ret;
}



#endif





#if 1

/*
* Function Name:				reslove_single_view_from_lut
* Function Function:			reslove_single_view_from_lut
* Input:
*      IN P_Avm_Param_S p_avm_param
*      IN int cam_flag
p_avm_param;
* Note:
*		reslove_single_view_from_lut
* Revision:
*     Create by Cheng.Li on 2018/1/17.
*/


AVM_Error_Code_E  reslove_single_view_from_lut(IN P_Avm_Param_S p_avm_param, IN int cam_flag)
{

	AVM_Error_Code_E ret = ERROR_NONE;

	Front_Rear_Single_View_Param_S single_view_param_front_rear;
	CvSize size0;
	UInt32_t* p_lut=NULL;
	UInt32_t *p_lut_static_line =NULL;
	UInt32_t *p_lut_dynamic_line = NULL;

	Left_Right_Single_View_Param_S left_right_single_view_param;
	UInt32_t result_image_height;
	UInt32_t result_image_width;
	UInt32_t type;
	if (cam_flag == _1_VIEW_FRONT)
	{
		single_view_param_front_rear = p_avm_param->p_avm_config_param->avm_single_view_param.front_single_view_param;
		size0 = cvSize(single_view_param_front_rear.result_image_width, single_view_param_front_rear.result_image_height);//
		p_lut = p_avm_param->p_avm_lut->p_lut_single_view_front;
		p_lut_static_line = p_avm_param->p_avm_dis_line->p_static_dis_line_front;
		p_lut_dynamic_line = p_avm_param->p_avm_dis_line->p_dynamic_dis_line_front;
		result_image_height = single_view_param_front_rear.result_image_height;
		result_image_width = single_view_param_front_rear.result_image_width;
		type = p_avm_param->p_avm_config_param->avm_single_view_param.front_single_view_param.expand_type;
	}
	if (cam_flag == _1_VIEW_REAR)
	{
		single_view_param_front_rear = p_avm_param->p_avm_config_param->avm_single_view_param.rear_single_view_param;
		size0 = cvSize(single_view_param_front_rear.result_image_width, single_view_param_front_rear.result_image_height);//
		p_lut = p_avm_param->p_avm_lut->p_lut_single_view_rear;
		p_lut_static_line = p_avm_param->p_avm_dis_line->p_static_dis_line_rear;
		p_lut_dynamic_line = p_avm_param->p_avm_dis_line->p_dynamic_dis_line_rear;
		result_image_height = single_view_param_front_rear.result_image_height;
		result_image_width = single_view_param_front_rear.result_image_width;
		type = p_avm_param->p_avm_config_param->avm_single_view_param.rear_single_view_param.expand_type;
	}

	if (cam_flag == _1_VIEW_LEFT)
	{
		left_right_single_view_param = p_avm_param->p_avm_config_param->avm_single_view_param.left_single_view_param;
		size0 = cvSize(left_right_single_view_param.result_image_width, left_right_single_view_param.result_image_height);//
		p_lut = p_avm_param->p_avm_lut->p_lut_single_view_left;
		p_lut_static_line = p_avm_param->p_avm_dis_line->p_static_dis_line_rear;
		p_lut_dynamic_line = p_avm_param->p_avm_dis_line->p_dynamic_dis_line_rear;
		result_image_height = left_right_single_view_param.result_image_height;
		result_image_width = left_right_single_view_param.result_image_width;
		type = p_avm_param->p_avm_config_param->avm_single_view_param.left_single_view_param.expand_type;
	}
	if (cam_flag == _1_VIEW_RIGHT)
	{
		left_right_single_view_param = p_avm_param->p_avm_config_param->avm_single_view_param.right_single_view_param;
		size0 = cvSize(left_right_single_view_param.result_image_width, left_right_single_view_param.result_image_height);//
		p_lut = p_avm_param->p_avm_lut->p_lut_single_view_right;
		p_lut_static_line = p_avm_param->p_avm_dis_line->p_static_dis_line_right;
		p_lut_dynamic_line = p_avm_param->p_avm_dis_line->p_dynamic_dis_line_right;
		result_image_height = left_right_single_view_param.result_image_height;
		result_image_width = left_right_single_view_param.result_image_width;
		type = p_avm_param->p_avm_config_param->avm_single_view_param.right_single_view_param.expand_type;

	}
	if (cam_flag == _1_VIEW_LEFT_RIGHT)
	{
		result_image_height = p_avm_param->p_avm_config_param->avm_single_view_param.left_right_view_param.result_image_height;
		result_image_width = p_avm_param->p_avm_config_param->avm_single_view_param.left_right_view_param.result_image_width;

		size0 = cvSize(result_image_width, result_image_height);//
		p_lut = p_avm_param->p_avm_lut->p_lut_single_view_left_right;
		p_lut_static_line = p_avm_param->p_avm_dis_line->p_static_dis_line_left_right;
		p_lut_dynamic_line = p_avm_param->p_avm_dis_line->p_dynamic_dis_line_left_right;
		type = p_avm_param->p_avm_config_param->avm_single_view_param.left_right_view_param.expand_type;
	}
	IplImage* image[4] = { NULL, NULL, NULL, NULL };
	ret = my_load_image(image, p_avm_param);
	//CHECK_ERROR(ret);
	IplImage *image_result_single_front_lut = cvCreateImage(size0, IPL_DEPTH_8U, 3);
	memset(image_result_single_front_lut->imageData, 0, result_image_width*result_image_height*3);
	
	for (UInt32_t i = 0; i <result_image_height; i++)
	{
		for (UInt32_t j = 0; j < result_image_width; j++)
		{
			if (cam_flag != _1_VIEW_LEFT_RIGHT)
			{
				int shift = i*result_image_width * 2 + j * 2;
				int rgb[3];
				//front_single_resolve_data(shift, image[cam_flag-4], rgb, p_avm_param, p_lut);
				resolve_data_from_lut_new(shift, image[cam_flag - 4], rgb, p_avm_param, p_lut);
				image_result_single_front_lut->imageData[i *result_image_width * 3 + j * 3] = (uchar)(rgb[0]);
				image_result_single_front_lut->imageData[i *result_image_width * 3 + j * 3+1] = (uchar)(rgb[1]);
				image_result_single_front_lut->imageData[i *result_image_width * 3 + j * 3+2] = (uchar)(rgb[2]);
			}
			else
			{
				int shift = i*result_image_width * 2 + j * 2;
				int rgb[3];
				if (j<result_image_width/2)//左边的
				{
					//front_single_resolve_data(shift, image[2], rgb, p_avm_param, p_lut);
					resolve_data_from_lut_new(shift, image[2], rgb, p_avm_param, p_lut);
					image_result_single_front_lut->imageData[i *result_image_width + j] = (uchar)(rgb[0]);
				}
				else//左边的
				{

					//front_single_resolve_data(shift, image[3], rgb, p_avm_param, p_lut);
					resolve_data_from_lut_new(shift, image[3], rgb, p_avm_param, p_lut);
					image_result_single_front_lut->imageData[i *result_image_width + j] = (uchar)(rgb[0]);
				}
			}
		}
	}
	
#if 1
	if ((type == TYPE_CROP || type == TYPE_PLANE_EXPAND) && (cam_flag == _1_VIEW_REAR || cam_flag == _1_VIEW_FRONT))
	{
		for (UInt32_t i = 0; i < p_lut_static_line[0]; i++)
		{
			UInt32_t pic_coordinate[2];
			int rgb[3];
			resolve_data_from_lut_line(i, rgb, p_avm_param, p_lut_static_line, pic_coordinate);
			if ((pic_coordinate[0] < result_image_height)&&(pic_coordinate[1] < result_image_width))
			{
				image_result_single_front_lut->imageData[pic_coordinate[0] * result_image_width + pic_coordinate[1]] = (uchar)(rgb[0]);
			}

		}
	}
	
#endif

#if 0
	if (cam_flag == _1_VIEW_FRONT || cam_flag == _1_VIEW_REAR)
	{
		UInt32_t *p_lut_dynamic_line_temp = NULL;
		for (int rotate_angle = 16; rotate_angle <= 16; rotate_angle++)
		{
			
			Int32_t dynamic_line_count = p_avm_param->p_avm_config_param->avm_single_view_param.front_single_view_param.
				dis_line_param.dynamic_line_param.dynamic_line_count;
			Int32_t every_angle_size = DEFAULT_DYNAMIC_DIS_LINE_SIZE * MAX_DYNAMIC_LINE_COUNT
				/ (p_avm_param->p_avm_config_param->avm_single_view_param.front_single_view_param.
				dis_line_param.dynamic_line_param.dynamic_line_count * 2 + 1);
			Int32_t address_shift = every_angle_size*(rotate_angle + dynamic_line_count);
			p_lut_dynamic_line_temp = p_lut_dynamic_line + address_shift;
			for (int i = 0; i < p_lut_dynamic_line_temp[0]; i++)
			{
				UInt32_t pic_coordinate[2];
				int rgb[3];
				resolve_data_from_lut_line(i, rgb, p_avm_param, p_lut_dynamic_line_temp, pic_coordinate);
				if (pic_coordinate[0] < result_image_height&&pic_coordinate[1] < result_image_width)
				{
					image_result_single_front_lut->imageData[pic_coordinate[0] * result_image_width + pic_coordinate[1]] = (uchar)(rgb[0]);
				}
			}
		}
		
	}

#endif
	if (type == TYPE_BOX_EXPAND)
	{
		char result_name[MAX_ARRAY_SIZE];
		//sprintf(result_name, "%s/单视图BOX展开%d_%dX%d.bmp", result_image_path, cam_flag, result_image_width, result_image_height);
		sprintf(result_name, "%s/%sBOX_%dX%d.bmp", result_image_path, view_name[cam_flag], result_image_width, result_image_height);
		cvSaveImage(result_name, image_result_single_front_lut);
	}
	else
	{
		char result_name[MAX_ARRAY_SIZE];
		//sprintf(result_name, "%s/单视图%d_%dX%d.bmp", result_image_path, cam_flag, result_image_width, result_image_height);
		sprintf(result_name, "%s/%s_%dX%d.bmp", result_image_path, view_name[cam_flag], result_image_width, result_image_height);
		cvSaveImage(result_name, image_result_single_front_lut);

	}
		
		cvReleaseImage(&image_result_single_front_lut);
		cvReleaseImage(&image[0]);
		cvReleaseImage(&image[1]);
		cvReleaseImage(&image[2]);
		cvReleaseImage(&image[3]);





		//{
		//	/*******存储大图的二进制表********/
		//	int result_image_width = 660, result_image_height = 370;
		//	int *distance = (int *)malloc(result_image_width*result_image_height*sizeof(int)* 2);//s32V前后左右必须对称

		//	char lut_path[MAX_ARRAY_SIZE];
		//	sprintf(lut_path, "%s/distance.bin", lut_file_path);
		//	FILE*fid = fopen(lut_path, "rb");





		//	fread(distance, sizeof(int), 660 * 370 * 2, fid);
		//	fclose(fid);
		//	int x1=60, y1=171;
		//	int dis_x = distance[y1*result_image_width * 2 + x1 * 2];
		//	//sprintf(heightText, "%d", dis_x);
		//	//cv::putText(img, heightText, point1, cv::FONT_HERSHEY_COMPLEX, 1, color);

		//	int dis_y = distance[y1* result_image_width * 2 + x1 * 2 + 1];
		//	//sprintf(heightText, "%d", dis_y);
		//	//point1.y = point1.y + 80;
		//	//cv::putText(img, heightText, point1, cv::FONT_HERSHEY_COMPLEX, 1, color);
		//	printf("%d,%d\n", dis_y, dis_x);


		//	int a = 9 - 3;


		//}


	return ret;
}



#endif







#if 1

/*
* Function Name:				reslove_single_view_from_lut
* Function Function:			reslove_single_view_from_lut
* Input:
*      IN P_Avm_Param_S p_avm_param
*      IN int cam_flag
p_avm_param;
* Note:
*		reslove_single_view_from_lut
* Revision:
*     Create by Cheng.Li on 2018/1/17.
*/


AVM_Error_Code_E  reslove_column_single_view_from_lut(IN P_Avm_Param_S p_avm_param, IN int cam_flag)
{

	AVM_Error_Code_E ret = ERROR_NONE;

	Front_Rear_Single_View_Param_S single_view_param_front_rear;
	CvSize size0;
	UInt32_t* p_lut = NULL;
	UInt32_t *p_lut_static_line = NULL;
	UInt32_t *p_lut_dynamic_line = NULL;

	Left_Right_Single_View_Param_S left_right_single_view_param;
	UInt32_t result_image_height;
	UInt32_t result_image_width;
	UInt32_t type;
	if (cam_flag == _1_VIEW_FRONT)
	{
		single_view_param_front_rear = p_avm_param->p_avm_config_param->avm_single_view_param.front_single_view_param;
		size0 = cvSize(single_view_param_front_rear.result_image_width, single_view_param_front_rear.result_image_height);//
		p_lut = p_avm_param->p_avm_lut->p_lut_single_view_front;
		p_lut_static_line = p_avm_param->p_avm_dis_line->p_static_dis_line_front;
		p_lut_dynamic_line = p_avm_param->p_avm_dis_line->p_dynamic_dis_line_front;
		result_image_height = single_view_param_front_rear.result_image_height;
		result_image_width = single_view_param_front_rear.result_image_width;
		type = p_avm_param->p_avm_config_param->avm_single_view_param.front_single_view_param.expand_type;
	}
	if (cam_flag == _1_VIEW_REAR)
	{
		single_view_param_front_rear = p_avm_param->p_avm_config_param->avm_single_view_param.rear_single_view_param;
		size0 = cvSize(single_view_param_front_rear.result_image_width, single_view_param_front_rear.result_image_height);//
		p_lut = p_avm_param->p_avm_lut->p_lut_single_view_rear;
		p_lut_static_line = p_avm_param->p_avm_dis_line->p_static_dis_line_rear;
		p_lut_dynamic_line = p_avm_param->p_avm_dis_line->p_dynamic_dis_line_rear;
		result_image_height = single_view_param_front_rear.result_image_height;
		result_image_width = single_view_param_front_rear.result_image_width;
		type = p_avm_param->p_avm_config_param->avm_single_view_param.rear_single_view_param.expand_type;
	}

	if (cam_flag == _1_VIEW_LEFT)
	{
		left_right_single_view_param = p_avm_param->p_avm_config_param->avm_single_view_param.left_single_view_param;
		size0 = cvSize(left_right_single_view_param.result_image_width, left_right_single_view_param.result_image_height);//
		p_lut = p_avm_param->p_avm_lut->p_lut_single_view_left;
		p_lut_static_line = p_avm_param->p_avm_dis_line->p_static_dis_line_rear;
		p_lut_dynamic_line = p_avm_param->p_avm_dis_line->p_dynamic_dis_line_rear;
		result_image_height = left_right_single_view_param.result_image_height;
		result_image_width = left_right_single_view_param.result_image_width;
		type = p_avm_param->p_avm_config_param->avm_single_view_param.left_single_view_param.expand_type;
	}
	if (cam_flag == _1_VIEW_RIGHT)
	{
		left_right_single_view_param = p_avm_param->p_avm_config_param->avm_single_view_param.right_single_view_param;
		size0 = cvSize(left_right_single_view_param.result_image_width, left_right_single_view_param.result_image_height);//
		p_lut = p_avm_param->p_avm_lut->p_lut_single_view_right;
		p_lut_static_line = p_avm_param->p_avm_dis_line->p_static_dis_line_right;
		p_lut_dynamic_line = p_avm_param->p_avm_dis_line->p_dynamic_dis_line_right;
		result_image_height = left_right_single_view_param.result_image_height;
		result_image_width = left_right_single_view_param.result_image_width;
		type = p_avm_param->p_avm_config_param->avm_single_view_param.right_single_view_param.expand_type;

	}
	if (cam_flag == _1_VIEW_LEFT_RIGHT)
	{
		result_image_height = p_avm_param->p_avm_config_param->avm_single_view_param.left_right_view_param.result_image_height;
		result_image_width = p_avm_param->p_avm_config_param->avm_single_view_param.left_right_view_param.result_image_width;

		size0 = cvSize(result_image_width, result_image_height);//
		p_lut = p_avm_param->p_avm_lut->p_lut_single_view_left_right;
		p_lut_static_line = p_avm_param->p_avm_dis_line->p_static_dis_line_left_right;
		p_lut_dynamic_line = p_avm_param->p_avm_dis_line->p_dynamic_dis_line_left_right;
		type = p_avm_param->p_avm_config_param->avm_single_view_param.left_right_view_param.expand_type;
	}
	IplImage* image = NULL;
	IplImage *image_result_single_front_lut = cvCreateImage(size0, IPL_DEPTH_8U,3);
	memset(image_result_single_front_lut->imageData, 0, result_image_width*result_image_height);

	for (int a = 3000; a < 7500; a++)
	{
		char image_path[MAX_ARRAY_SIZE];
		//sprintf(image_path, "D:\\front_园区\\front_%d.bmp",
			//a);
		sprintf(image_path, "D:\\back_园区\\back_%d.bmp",a);
		//D:\桌面本分\Front_地库
		image = cvLoadImage(image_path);
		for (UInt32_t i = 0; i < result_image_height; i++)
		{
			for (UInt32_t j = 0; j < result_image_width; j++)
			{
				int shift = i*result_image_width * 2 + j * 2;
				int rgb[3];
				resolve_data_from_lut_new(shift, image, rgb, p_avm_param, p_lut);
				image_result_single_front_lut->imageData[i *result_image_width * 3 + j * 3] = (uchar)(rgb[0]);
				image_result_single_front_lut->imageData[i *result_image_width * 3 + j * 3+1] = (uchar)(rgb[1]);
				image_result_single_front_lut->imageData[i *result_image_width * 3 + j * 3+2] = (uchar)(rgb[2]);

			}
		}


		char result_name[MAX_ARRAY_SIZE];
		sprintf(result_name, "D:/柱面展开/后/back_%d.bmp", a);
		cvSaveImage(result_name, image_result_single_front_lut);

	}
	
	return ret;
}



#endif







#if 1

/*
* Function Name:				reslove_single_view_from_lut
* Function Function:			reslove_single_view_from_lut
* Input:
*      IN P_Avm_Param_S p_avm_param
*      IN int cam_flag
p_avm_param;
* Note:
*		reslove_single_view_from_lut
* Revision:
*     Create by Cheng.Li on 2018/1/17.
*/


AVM_Error_Code_E  reslove_column_single_view_from_lut_front(IN P_Avm_Param_S p_avm_param, IN int cam_flag)
{

	AVM_Error_Code_E ret = ERROR_NONE;

	Front_Rear_Single_View_Param_S single_view_param_front_rear;
	CvSize size0;
	UInt32_t* p_lut = NULL;
	UInt32_t *p_lut_static_line = NULL;
	UInt32_t *p_lut_dynamic_line = NULL;

	Left_Right_Single_View_Param_S left_right_single_view_param;
	UInt32_t result_image_height;
	UInt32_t result_image_width;
	UInt32_t type;
	if (cam_flag == _1_VIEW_FRONT)
	{
		single_view_param_front_rear = p_avm_param->p_avm_config_param->avm_single_view_param.front_single_view_param;
		size0 = cvSize(single_view_param_front_rear.result_image_width, single_view_param_front_rear.result_image_height);//
		p_lut = p_avm_param->p_avm_lut->p_lut_single_view_front;
		p_lut_static_line = p_avm_param->p_avm_dis_line->p_static_dis_line_front;
		p_lut_dynamic_line = p_avm_param->p_avm_dis_line->p_dynamic_dis_line_front;
		result_image_height = single_view_param_front_rear.result_image_height;
		result_image_width = single_view_param_front_rear.result_image_width;
		type = p_avm_param->p_avm_config_param->avm_single_view_param.front_single_view_param.expand_type;
	}
	if (cam_flag == _1_VIEW_REAR)
	{
		single_view_param_front_rear = p_avm_param->p_avm_config_param->avm_single_view_param.rear_single_view_param;
		size0 = cvSize(single_view_param_front_rear.result_image_width, single_view_param_front_rear.result_image_height);//
		p_lut = p_avm_param->p_avm_lut->p_lut_single_view_rear;
		p_lut_static_line = p_avm_param->p_avm_dis_line->p_static_dis_line_rear;
		p_lut_dynamic_line = p_avm_param->p_avm_dis_line->p_dynamic_dis_line_rear;
		result_image_height = single_view_param_front_rear.result_image_height;
		result_image_width = single_view_param_front_rear.result_image_width;
		type = p_avm_param->p_avm_config_param->avm_single_view_param.rear_single_view_param.expand_type;
	}

	if (cam_flag == _1_VIEW_LEFT)
	{
		left_right_single_view_param = p_avm_param->p_avm_config_param->avm_single_view_param.left_single_view_param;
		size0 = cvSize(left_right_single_view_param.result_image_width, left_right_single_view_param.result_image_height);//
		p_lut = p_avm_param->p_avm_lut->p_lut_single_view_left;
		p_lut_static_line = p_avm_param->p_avm_dis_line->p_static_dis_line_rear;
		p_lut_dynamic_line = p_avm_param->p_avm_dis_line->p_dynamic_dis_line_rear;
		result_image_height = left_right_single_view_param.result_image_height;
		result_image_width = left_right_single_view_param.result_image_width;
		type = p_avm_param->p_avm_config_param->avm_single_view_param.left_single_view_param.expand_type;
	}
	if (cam_flag == _1_VIEW_RIGHT)
	{
		left_right_single_view_param = p_avm_param->p_avm_config_param->avm_single_view_param.right_single_view_param;
		size0 = cvSize(left_right_single_view_param.result_image_width, left_right_single_view_param.result_image_height);//
		p_lut = p_avm_param->p_avm_lut->p_lut_single_view_right;
		p_lut_static_line = p_avm_param->p_avm_dis_line->p_static_dis_line_right;
		p_lut_dynamic_line = p_avm_param->p_avm_dis_line->p_dynamic_dis_line_right;
		result_image_height = left_right_single_view_param.result_image_height;
		result_image_width = left_right_single_view_param.result_image_width;
		type = p_avm_param->p_avm_config_param->avm_single_view_param.right_single_view_param.expand_type;

	}
	if (cam_flag == _1_VIEW_LEFT_RIGHT)
	{
		result_image_height = p_avm_param->p_avm_config_param->avm_single_view_param.left_right_view_param.result_image_height;
		result_image_width = p_avm_param->p_avm_config_param->avm_single_view_param.left_right_view_param.result_image_width;

		size0 = cvSize(result_image_width, result_image_height);//
		p_lut = p_avm_param->p_avm_lut->p_lut_single_view_left_right;
		p_lut_static_line = p_avm_param->p_avm_dis_line->p_static_dis_line_left_right;
		p_lut_dynamic_line = p_avm_param->p_avm_dis_line->p_dynamic_dis_line_left_right;
		type = p_avm_param->p_avm_config_param->avm_single_view_param.left_right_view_param.expand_type;
	}
	IplImage* image = NULL;
	IplImage *image_result_single_front_lut = cvCreateImage(size0, IPL_DEPTH_8U, 3);
	memset(image_result_single_front_lut->imageData, 0, result_image_width*result_image_height);

	for (int a =4109; a < 8000; a++)
	{
		char image_path[MAX_ARRAY_SIZE];
		//sprintf(image_path, "D:\\front_园区\\front_%d.bmp",
		//a);
		sprintf(image_path, "D:\\front_园区\\front_%d.bmp", a);
		//D:\桌面本分\Front_地库
		image = cvLoadImage(image_path);
		for (UInt32_t i = 0; i < result_image_height; i++)
		{
			for (UInt32_t j = 0; j < result_image_width; j++)
			{
				int shift = i*result_image_width * 2 + j * 2;
				int rgb[3];
				resolve_data_from_lut_new(shift, image, rgb, p_avm_param, p_lut);
				image_result_single_front_lut->imageData[i *result_image_width * 3 + j * 3] = (uchar)(rgb[0]);
				image_result_single_front_lut->imageData[i *result_image_width * 3 + j * 3 + 1] = (uchar)(rgb[1]);
				image_result_single_front_lut->imageData[i *result_image_width * 3 + j * 3 + 2] = (uchar)(rgb[2]);

			}
		}


		char result_name[MAX_ARRAY_SIZE];
		sprintf(result_name, "D:/柱面展开/前/front_%d.bmp", a);
		cvSaveImage(result_name, image_result_single_front_lut);

	}

	return ret;
}



#endif




