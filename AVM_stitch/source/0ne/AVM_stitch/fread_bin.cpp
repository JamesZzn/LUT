#include "fread_bin.h"

/*
Function Name:      bilinear_interpolation
Function Function : finish image interpolation
Input             :
p_src_img    : rgb image
rgb    : interpolation result
x_src_loc    : source x location (double)
y_src_loc    : source y location (double)
Return            : void
Note              :
Revision History  : Author         Data             Changes
YanShuo        2017/10/31        Create
*/
void bilinear_interpolation_0(IplImage* p_src_img,
	Int32_t rgb[3],
	Float64_t x_src_loc,
	Float64_t y_src_loc
	)
{
	Float64_t weightUpLeft, weightUpRight, weightDownLeft, weightDownRight;
	Int32_t x0 = (Int32_t)x_src_loc;
	Int32_t x1 = (Int32_t)x_src_loc + 1;
	Int32_t y0 = (Int32_t)y_src_loc;
	Int32_t y1 = (Int32_t)y_src_loc + 1;
	Int32_t channel_id;

	x0 = x0 < 0 ? 0 : x0;
	x1 = x1 < 0 ? 0 : x1;
	y0 = y0 < 0 ? 0 : y0;
	y1 = y1 < 0 ? 0 : y1;

	x0 = x0 > p_src_img->width - 1 ? p_src_img->width - 1 : x0;
	x1 = x1 > p_src_img->width - 1 ? p_src_img->width - 1 : x1;
	y0 = y0 > p_src_img->height - 1 ? p_src_img->height - 1 : y0;
	y1 = y1 > p_src_img->height - 1 ? p_src_img->height - 1 : y1;

	weightUpLeft = (x1 - x_src_loc) * (y1 - y_src_loc);
	weightUpRight = (x_src_loc - x0) * (y1 - y_src_loc);
	weightDownLeft = (x1 - x_src_loc) * (y_src_loc - y0);
	weightDownRight = 1 - weightUpLeft - weightUpRight - weightDownLeft;

	uchar* p = (uchar*)(p_src_img->imageData + y0*p_src_img->widthStep);
	uchar* q = (uchar*)(p_src_img->imageData + y1*p_src_img->widthStep);

	for (channel_id = 0; channel_id < p_src_img->nChannels; channel_id++)
	{
		if (x0 == 0 || y0 == 0 || x0 == p_src_img->width - 1 || y0 == p_src_img->height - 1)
			rgb[channel_id] = p[x0 * p_src_img->nChannels + channel_id];
		else
			rgb[channel_id] = (Int32_t)(weightUpLeft * p[x0 * p_src_img->nChannels + channel_id]
			+ weightUpRight * p[x1 * p_src_img->nChannels + channel_id]
			+ weightDownLeft * q[x0 * p_src_img->nChannels + channel_id]
			+ weightDownRight * q[x1 * p_src_img->nChannels + channel_id]);
	}
}

void front_single_resolve_data_0(int shift, IplImage *image, int *rgb, UInt32_t *p_lut)
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
	bilinear_interpolation_0(image, rgb, image_point0[1], image_point0[0]);
	rgb[0] = (uchar)(Float64_t(rgb[0]));

}

void my_load_image(OUT IplImage* image[4])
{
	for (int cam_id = 0; cam_id < 4; cam_id++)
	{
		char image_path[MAX_ARRAY_SIZE];
		sprintf(image_path, "%s/%s.bmp",
			source_image_path, camera_prefix_array[cam_id]);
		image[cam_id] = cvLoadImage(image_path);
		if (image[cam_id] == 0)
		{
			
			
		}
	}
	

}
void reslove_single_view_from_lut_0()
{
	
	FILE *fid;
	char vehicle_param_name[MAX_ARRAY_SIZE];

	sprintf(vehicle_param_name, "%s/single_view%d.bin", lut_file_path, 0);
	fid = fopen(vehicle_param_name, "rb");
	if (!(fid = fopen(vehicle_param_name, "rb")))
	{
		printf("File %s cannot be opened\n", vehicle_param_name);

	}

	CvSize size0;
	int result_image_width = 900;
	int result_image_height = 1000;

	size0 = cvSize(result_image_width, result_image_height);//

	int size_all = result_image_width*result_image_height * 2;


	UInt32_t* p_lut_p = (UInt32_t*)malloc(sizeof(UInt32_t)*size_all);
	memset(p_lut_p, 0, sizeof(UInt32_t)*size_all);

	fread(p_lut_p, sizeof(UInt32_t), size_all, fid);
	fclose(fid);

	IplImage* image[4] = { NULL, NULL, NULL, NULL };

	 my_load_image(image);
	//CHECK_ERROR(ret);
	IplImage *image_result_single_front_lut = cvCreateImage(size0, IPL_DEPTH_8U, 1);
	memset(image_result_single_front_lut->imageData, 0, result_image_width*result_image_height);
	for (int i = 0; i < result_image_height; i++)
	{
		for (int j = 0; j < result_image_width; j++)
		{
			int shift = i*result_image_width * 2 + j * 2;
			int rgb[3];
			front_single_resolve_data_0(shift, image[0], rgb, p_lut_p);
			image_result_single_front_lut->imageData[i *result_image_width + j] = (uchar)(rgb[0]);
		}
	}
	char result_name[MAX_ARRAY_SIZE];
	sprintf(result_name, "%s/zzzzzzzzzzzzzzzzzzzzzzzzzz.bmp", result_image_path);
	cvSaveImage(result_name, image_result_single_front_lut);



}
