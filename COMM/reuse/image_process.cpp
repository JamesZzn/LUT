/*
 * This file provides some basic none encoded image processing function for all projects.
 * Create by Shuo.Yan on 2016/5/13
 */

#include "image_process.h"

/*
 * Function Name:				Create_Yulan_Image
 * Function Function:			create image structure
 * Input:
 *     pp_image:				the address of image address
 *     image_type:				image type, must be one in Image_Type_Tag structure
 *     height:					image height
 *     width:					image width
 * Note:
 * Revision:
 *     Create by Shuo.Yan on 2017/11/25.
 */
Int32_t Create_Intesight_Image(P_Intesight_Image_S* pp_image, Image_Type_E image_type, Int32_t height, Int32_t width)
{	
	Int32_t ret = 0;
	Int32_t image_content_size;

	// malloc image structure memory
	P_Intesight_Image_S p_image = NULL;
	p_image = (P_Intesight_Image_S)malloc(sizeof(Intesight_Image_S));
	if (!p_image)
	{
		ret = 2;
		return ret;
	}
	memset(p_image, 0, sizeof(Intesight_Image_S));

	// set image pointer and malloc image content memory
	switch (image_type)
	{
	case IMAGE_TYPE_YUV420:
	case IMAGE_TYPE_NV12:
		image_content_size = sizeof(UInt8_t)* height * width * 3 >> 1;
		break;
	case IMAGE_TYPE_YUV422:
	case IMAGE_TYPE_UYVY:
		image_content_size = sizeof(UInt8_t)* height * width*2;
		break;
	case IMAGE_TYPE_YUV444:
	case IMAGE_TYPE_RGB:
		image_content_size = sizeof(UInt8_t)* height * width * 3;
		break;
	case IMAGE_TYPE_RGBA:
		image_content_size = sizeof(UInt8_t)* height * width * 4;
		break;
	default:
		printf("unsupported image type.\n");
		ret = 3;
		return ret;
	}

	p_image->image_type = image_type;
	p_image->height = height;
	p_image->width = width;
	p_image->p_data = (UInt8_t*)malloc(image_content_size);
	if (!p_image)
	{
		ret = 2;
		return ret;
	}
	memset(p_image->p_data, 0, image_content_size);

	*pp_image = p_image;
	return ret;
}

/*
 * Function Name:				Release_Yulan_Image
 * Function Function:			release image structure
 * Input:
 *     p_image:				the address of image address
 * Note:
 * Revision:
 *     Create by Shuo.Yan on 2017/11/25.
 */
void Release_Intesight_Image(P_Intesight_Image_S p_image)
{
	FREE_POINTER(p_image->p_data);
	FREE_POINTER(p_image);
}

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
void bilinear_interpolation(IplImage* p_src_img, 
							Int32_t rgb[3],
							Float32_t x_src_loc, 
							Float32_t y_src_loc 
						    )
{
	Float32_t weightUpLeft, weightUpRight, weightDownLeft, weightDownRight;
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
void bilinear_interpolation_new(IplImage* p_src_img,
	Int32_t rgb[3],
	Int32_t x0,
	Int32_t y0,
	Float32_t weightUpLeft,
	Float32_t weightUpRight,
	Float32_t weightDownLeft,
	Float32_t weightDownRight
	)
{

	Int32_t x1 = x0 + 1;
	Int32_t y1 = y0 + 1;
	
	x0 = x0 < 0 ? 0 : x0;
	x1 = x1 < 0 ? 0 : x1;
	y0 = y0 < 0 ? 0 : y0;
	y1 = y1 < 0 ? 0 : y1;

  	x0 = x0 > p_src_img->width - 1 ? p_src_img->width - 1 : x0;
	x1 = x1 > p_src_img->width - 1 ? p_src_img->width - 1 : x1;
	y0 = y0 > p_src_img->height - 1 ? p_src_img->height - 1 : y0;
	y1 = y1 > p_src_img->height - 1 ? p_src_img->height - 1 : y1;


	Int32_t channel_id;
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
void bilinear_interpolation_new_yuv(IplImage* p_src_img,
	Int32_t rgb[3],
	Int32_t x0,
	Int32_t y0,
	Float32_t weightUpLeft,
	Float32_t weightUpRight,
	Float32_t weightDownLeft,
	Float32_t weightDownRight
	)
{

	Int32_t x1 = x0 + 1;
	Int32_t y1 = y0 + 1;

	x0 = x0 < 0 ? 0 : x0;
	x1 = x1 < 0 ? 0 : x1;
	y0 = y0 < 0 ? 0 : y0;
	y1 = y1 < 0 ? 0 : y1;

	x0 = x0 > p_src_img->width - 1 ? p_src_img->width - 1 : x0;
	x1 = x1 > p_src_img->width - 1 ? p_src_img->width - 1 : x1;
	y0 = y0 > p_src_img->height - 1 ? p_src_img->height - 1 : y0;
	y1 = y1 > p_src_img->height - 1 ? p_src_img->height - 1 : y1;


	


#if 0
	Int32_t channel_id;
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
#endif
}
/*
    Function Name:      bgr_to_gray
    Function Function : change bgr image into gray image
    Input             :
      p_rgb_img       : rgb image
      p_gray_img      : i420 image
    Return            : void
    Note              :
    Revision History  : Author         Data             Changes
						YanShuo        2017/10/31       Create
*/
void bgr_to_gray(IplImage* p_rgb_img, IplImage* p_gray_img)
{
	uchar b, g, r, y;

	Int32_t Y;
	Int32_t height, width;
	height = p_gray_img->height;
	width = p_gray_img->width;

	uchar* p_bgr = (uchar*)p_rgb_img->imageData;
	uchar* p_gray = (uchar*)p_gray_img->imageData;

	/*convert bgr image to yuv420 image*/
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			b = p_bgr[i * p_rgb_img->widthStep + j * p_rgb_img->nChannels + 0];
			g = p_bgr[i * p_rgb_img->widthStep + j * p_rgb_img->nChannels + 1];
			r = p_bgr[i * p_rgb_img->widthStep + j * p_rgb_img->nChannels + 2];

			Y = (Int32_t)(0.299 * r + 0.587 * g + 0.114 * b);
			Y = Y < 0 ? 0 : (Y > 255 ? 255 : Y);
			y = (uchar)Y;
			p_gray[i * p_gray_img->widthStep + j] = y;
		}
	}
}

/*
	Function Name:      bgr_to_yuv420
	Function Function : change bgr image into yuv420 image
	Input             : 
	  p_rgb_img       : the rgb image
	  p_yuv420_img    : I420 image
	Return            : void
	Note              : 
	Revision History  : Author         Data             Changes
						YanShuo        2017/10/31       Create
*/
void bgr_to_yuv420(const IplImage* p_rgb_img, uchar* p_yuv420_img)
{
	uchar b, g, r, y, u, v;
	
	Int32_t Y, U, V;
	Int32_t height, width, half_height, half_width, pitch;
	FILE* p_file = NULL;
	height = p_rgb_img->height;
	width = p_rgb_img->width;
	half_height = height >> 1;
	half_width = width >> 1;
	pitch = height * width;

	uchar* p_brg_data = (uchar*)p_rgb_img->imageData;

	/*convert bgr image to yuv420 image*/
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			if (1 == p_rgb_img->nChannels)
			{
				p_yuv420_img[i * width + j] = p_brg_data[i * p_rgb_img->widthStep + j];
				if ( !(i & 0x1) && (!(j & 0x1)) )
				{
					p_yuv420_img[pitch + (i >> 1) * half_width + (j >> 1)] = 128;
					p_yuv420_img[(pitch * 5 >> 2) + (i >> 1) * half_width + (j >> 1)] = 128;
				}
			}
			else
			{
				b = p_brg_data[i * p_rgb_img->widthStep + j * p_rgb_img->nChannels + 0];
				g = p_brg_data[i * p_rgb_img->widthStep + j * p_rgb_img->nChannels + 1];
				r = p_brg_data[i * p_rgb_img->widthStep + j * p_rgb_img->nChannels + 2];

				Y = (Int32_t)( 0.299 * r + 0.587 * g + 0.114 * b);
				Y = Y < 0 ? 0 : (Y > 255 ? 255 : Y);
				y = (uchar)Y;
				p_yuv420_img[i * width + j] = Y;
				if (!(i & 0x1) && (!(j & 0x1)))
				{
					U = (Int32_t)(-0.169 * r - 0.331 * g + 0.500 * b + 128);
					V = (Int32_t)( 0.500 * r - 0.418 * g - 0.082 * b + 128);
					U = U < 0 ? 0 : (U > 255 ? 255 : U);
					V = V < 0 ? 0 : (V > 255 ? 255 : V);
					u = (uchar)U;
					v = (uchar)V;
					p_yuv420_img[pitch + (i >> 1) * half_width + (j >> 1)] = u;
					p_yuv420_img[(pitch * 5 >> 2) + (i >> 1) * half_width + (j >> 1)] = v;
				}
			}			
		}
	}
}

/*
	Function Name:      yuv420_to_bgr
	Function Function : Convert YUV420 image into IplImage* BGR image
	Input             : 
	     p_bgr_img    : The converted BGR image
	  p_yuv420_img    : The source YUV420 image
	Return            : Void
	Note              : 
	Revision History  : Author         Data             Changes
	                    YanShuo        2017/10/31       Create
*/
void yuv420_to_bgr(IplImage* p_bgr_img, uchar* p_yuv420_img)
{
	Int32_t	width = p_bgr_img->width;
	Int32_t	height = p_bgr_img->height;
	Int32_t half_width = width >> 1;
	Int32_t half_height = height >> 1;
	Int32_t	y_pitch = width * height;
	Int32_t	uv_pitch = y_pitch >> 2;
	Int32_t	i, j, i_half, j_half;

	uchar* p_brg_data = (uchar*)p_bgr_img->imageData;
	uchar tempY, tempU, tempV;

	for (i = 0; i < height; i++)
	{
		i_half = i >> 1;
		for (j = 0; j < width; j++)
		{
			j_half = j >> 1;
			tempY = p_yuv420_img[i * width + j];
			tempU = p_yuv420_img[y_pitch + i_half * half_width + j_half];
			tempV = p_yuv420_img[y_pitch + uv_pitch + i_half * half_width + j_half];

			Int32_t Y = tempY;
			Int32_t U = tempU - 128;
			Int32_t V = tempV - 128;

			Int32_t R = (Int32_t)(Y + 1.4022 * V);
			Int32_t G = (Int32_t)(Y - 0.3456 * U - 0.7145 * V);
			Int32_t B = (Int32_t)(Y + 1.771  * U);

			p_brg_data[i*p_bgr_img->widthStep + j*p_bgr_img->nChannels + 0] =
				B < 0 ? 0 : (B > 255 ? 255 : (uchar)B);
			p_brg_data[i*p_bgr_img->widthStep + j*p_bgr_img->nChannels + 1] =
				G < 0 ? 0 : (G > 255 ? 255 : (uchar)G);
			p_brg_data[i*p_bgr_img->widthStep + j*p_bgr_img->nChannels + 2] =
				R < 0 ? 0 : (R > 255 ? 255 : (uchar)R);
		}
	}
}

/*
	Function Name:      yuv422_to_bgr
	Function Function : Convert YUV422 image into IplImage* BGR image
	Input             : 
	     p_bgr_img    : The converted BGR image
	  p_yuv422_img    : The source YUV422 image
	Return            : Void
	Note              : 
	Revision History  : Author         Data             Changes
	                    YanShuo        2017/10/31       Create
*/
void yuv422_to_bgr(IplImage *p_bgr_img, uchar* p_yuv422_img)
{
	Int32_t	width = p_bgr_img->width;
	Int32_t	height = p_bgr_img->height;
	Int32_t half_width = width >> 1;
	Int32_t half_height = height >> 1;
	Int32_t	y_pitch = width * height;
	Int32_t	uv_pitch = y_pitch >> 1;
	Int32_t	i, j, j_half;

	uchar* p_brg_data = (uchar*)p_bgr_img->imageData;
	uchar tempY, tempU, tempV;

	for (i = 0; i < height; i++)
	{
		for (j = 0; j < width; j++)
		{
			j_half = j >> 1;
			tempY = p_yuv422_img[i * width + j];
			tempU = p_yuv422_img[y_pitch + i * half_width + j_half];
			tempV = p_yuv422_img[y_pitch + uv_pitch + i * half_width + j_half];

			Int32_t Y = tempY;
			Int32_t U = tempU - 128;
			Int32_t V = tempV - 128;

			Int32_t R = (Int32_t)(Y + 1.4022 * V);
			Int32_t G = (Int32_t)(Y - 0.3456 * U - 0.7145 * V);
			Int32_t B = (Int32_t)(Y + 1.771  * U);

			p_brg_data[i*p_bgr_img->widthStep + j*p_bgr_img->nChannels + 0] =
				B < 0 ? 0 : (B > 255 ? 255 : (uchar)B);
			p_brg_data[i*p_bgr_img->widthStep + j*p_bgr_img->nChannels + 1] =
				G < 0 ? 0 : (G > 255 ? 255 : (uchar)G);
			p_brg_data[i*p_bgr_img->widthStep + j*p_bgr_img->nChannels + 2] =
				R < 0 ? 0 : (R > 255 ? 255 : (uchar)R);
		}
	}
}

/*
	Function Name:      nv12_to_bgr
	Function Function : Convert Nv12 image into IplImage* BGR image
	Input             : 
	     p_bgr_img    : The converted BGR image
		p_nv12_img    : The source Nv12 image
	Return            : Void
	Note              : 
	Revision History  : Author         Data             Changes
	                    YanShuo        2017/10/31       Create
*/
void nv12_to_bgr(IplImage *p_bgr_img, uchar* p_nv12_img)
{
	Int32_t	width = p_bgr_img->width;
	Int32_t	height = p_bgr_img->height;
	Int32_t halfWidth = width >> 1;
	Int32_t halfHeight = height >> 1;
	Int32_t	y_pitch = width * height;
	Int32_t	uv_pitch = y_pitch >> 2;
	Int32_t	i, j, i_half, j_half;

	uchar* p_brg_data = (uchar*)p_bgr_img->imageData;
	uchar tempY, tempU, tempV;

	for (i = 0; i < height; i++)
	{
		i_half = i >> 1;
		for (j = 0; j < width; j++)
		{
			j_half = j >> 1;
			tempY = p_nv12_img[i * width + j];
			tempU = p_nv12_img[y_pitch + i_half * width + (j_half << 1)];
			tempV = p_nv12_img[y_pitch + i_half * width + (j_half << 1) + 1];

			Int32_t Y = tempY;
			Int32_t U = tempU - 128;
			Int32_t V = tempV - 128;

			Int32_t R = (Int32_t)(Y + 1.4022 * V);
			Int32_t G = (Int32_t)(Y - 0.3456 * U - 0.7145 * V);
			Int32_t B = (Int32_t)(Y + 1.771  * U);

			p_brg_data[i*p_bgr_img->widthStep + j*p_bgr_img->nChannels + 0] =
				B < 0 ? 0 : (B > 255 ? 255 : (uchar)B);
			p_brg_data[i*p_bgr_img->widthStep + j*p_bgr_img->nChannels + 1] =
				G < 0 ? 0 : (G > 255 ? 255 : (uchar)G);
			p_brg_data[i*p_bgr_img->widthStep + j*p_bgr_img->nChannels + 2] =
				R < 0 ? 0 : (R > 255 ? 255 : (uchar)R);
		}
	}
}

/*
	Function Name:      yuv420_cut
	Function Function : Cut image
	Input             : 
	 p_dst_yuv420_img : The cut YUV420 image
	 p_src_yuv420_img : The source YUV420 image
		src_height    : source image height
		src_width     : source image width
		dst_height    : cut image height
		dst_height    : cut image width
	Return            : Void
	Note              : 
	Revision History  : Author         Data             Changes
	                    YanShuo        2017/10/31       Create
*/
void yuv420_cut(uchar* p_dst_yuv420_img, uchar* p_src_yuv420_img, 
				Int32_t src_height, Int32_t src_width, 
				Int32_t dst_height, Int32_t dst_width)
{
	Int32_t half_src_width = src_width >> 1;
	Int32_t half_dst_width = dst_width >> 1;
	Int32_t	y_src_pitch = src_width * src_height;
	Int32_t	y_dst_pitch = dst_width * dst_height;
	Int32_t	uv_src_pitch = y_src_pitch >> 2;
	Int32_t	uv_dst_pitch = y_dst_pitch >> 2;
	Int32_t	i, j, i_half, j_half;

	for (i = 0; i < src_height; i++)
	{
		i_half = i >> 1;
		for (j = 0; j < src_width; j++)
		{
			j_half = j >> 1;
			if(j < dst_width && i < dst_height)
			{
				p_dst_yuv420_img[i * dst_width + j] = p_src_yuv420_img[i * src_width + j];
				p_dst_yuv420_img[y_dst_pitch + i_half * half_dst_width + j_half] =
					p_src_yuv420_img[y_src_pitch + i_half * half_src_width + j_half];
				p_dst_yuv420_img[y_dst_pitch + uv_dst_pitch + i_half * half_dst_width + j_half] =
					p_src_yuv420_img[y_src_pitch + uv_src_pitch + i_half * half_src_width + j_half];
			}
		}
	}
}

/*
	Function Name:      yuv422_cut
	Function Function : Cut image
	Input             : 
	 p_dst_yuv422_img : The cut YUV422 image
	 p_src_yuv422_img : The source YUV422 image
		src_height    : source image height
		src_width     : source image width
		dst_height    : cut image height
		dst_height    : cut image width
	Return            : Void
	Note              : 
	Revision History  : Author         Data             Changes
	                    YanShuo        2017/10/31       Create
*/
void yuv422_cut(uchar* p_dst_yuv422_img, uchar* p_src_yuv422_img, 
				Int32_t src_height, Int32_t src_width, 
				Int32_t dst_height, Int32_t dst_width)
{
	Int32_t half_src_width = src_width >> 1;
	Int32_t half_dst_width = dst_width >> 1;
	Int32_t	y_src_pitch = src_width * src_height;
	Int32_t	y_dst_pitch = dst_width * dst_height;
	Int32_t	uv_src_pitch = y_src_pitch >> 1;
	Int32_t	uv_dst_pitch = y_dst_pitch >> 1;
	Int32_t	i, j, j_half;

	for (i = 0; i < src_height; i++)
	{
		for (j = 0; j < src_width; j++)
		{
			j_half = j >> 1;
			if(j < dst_width && i < dst_height)
			{
				p_dst_yuv422_img[i * dst_width + j] = p_src_yuv422_img[i * src_width + j];
				p_dst_yuv422_img[y_dst_pitch + i * half_dst_width + j_half] =
					p_src_yuv422_img[y_src_pitch + i * half_src_width + j_half];
				p_dst_yuv422_img[y_dst_pitch + uv_dst_pitch + i * half_dst_width + j_half] =
					p_src_yuv422_img[y_src_pitch + uv_src_pitch + i * half_src_width + j_half];
			}
		}
	}
}

/*
	Function Name:      nv12_cut
	Function Function : Cut image
	Input             : 
	 p_dst_nv12_img   : The cut NV12 image
	 p_src_nv12_img   : The source NV12 image
		src_height    : source image height
		src_width     : source image width
		dst_height    : cut image height
		dst_height    : cut image width
	Return            : Void
	Note              : 
	Revision History  : Author         Data             Changes
	                    YanShuo        2017/10/31       Create
*/
void nv12_cut(uchar* p_dst_nv12_img, uchar* p_src_nv12_img, 
			  Int32_t src_height, Int32_t src_width, 
			  Int32_t dst_height, Int32_t dst_width)
{
	Int32_t	y_src_pitch = src_width * src_height;
	Int32_t	y_dst_pitch = dst_width * dst_height;

	Int32_t	i, j, i_half, j_half;

	for (i = 0; i < src_height; i++)
	{
		i_half = i >> 1;
		for (j = 0; j < src_width; j++)
		{
			j_half = j >> 1;
			if(j < dst_width && i < dst_height)
			{
				p_dst_nv12_img[i * dst_width + j] = p_src_nv12_img[i * src_width + j];
				p_dst_nv12_img[y_dst_pitch + i_half * dst_width + j] =
					p_src_nv12_img[y_src_pitch + i_half * src_width + j];
			}
		}
	}
}

/*
	Function Name:      rgba_cut
	Function Function : Cut image
	Input             : 
	 p_dst_rgba_img   : The cut rgba image
	 p_src_rgba_img   : The source rgba image
		 src_height   : source image height
		  src_width   : source image width
		 dst_height   : cut image height
		 dst_height   : cut image width
	Return            : Void
	Note              : 
	Revision History  : Author         Data             Changes
	                    YanShuo        2017/10/31        Create
*/
void rgba_cut(uchar* p_dst_rgba_img, uchar* p_src_rgba_img, 
			  Int32_t src_height, Int32_t src_width, 
			  Int32_t dst_height, Int32_t dst_width)
{
	Int32_t	y_src_pitch = src_width * src_height;
	Int32_t	y_dst_pitch = dst_width * dst_height;

	Int32_t	i, j, k;

	for (i = 0; i < src_height; i++)
	{
		for (j = 0; j < src_width; j++)
		{
			if(j < dst_width && i < dst_height)
			{
				for(k = 0; k < 4; k++)
				{
					p_dst_rgba_img[i * dst_width * 4 + j * 4 + k] = p_src_rgba_img[i * src_width * 4 + j * 4 + k];
				}
			}
		}
	}
}

/*
	Function Name:      flip_image
	Function Function : flip image
	Input             : 
	     p_bgr_img    : The BGR image to be flipped
		is_horizon    : Horizontal flip or vertical flip
	Return            : Void
	Note              : 
	Revision History  : Author         Data             Changes
	                    YanShuo        2017/10/31       Create
*/
void flip_image(IplImage *p_bgr_img, bool is_horizon)
{
	Int32_t	width = p_bgr_img->width;
	Int32_t	height = p_bgr_img->height;
	Int32_t	i, j, k;

	uchar* p_brg_data = (uchar*)p_bgr_img->imageData;

	if(is_horizon)
	{
		for (i = 0; i < height; i++)
		{
			for (j = 0; j < (width >> 1); j++)
			{
				for (k = 0; k < p_bgr_img->nChannels; k++)
				{
					uchar tmp = p_brg_data[i*p_bgr_img->widthStep + j*p_bgr_img->nChannels + k];
					p_brg_data[i*p_bgr_img->widthStep + j*p_bgr_img->nChannels + k] =
						p_brg_data[i*p_bgr_img->widthStep + (width - 1 - j)*p_bgr_img->nChannels + k];
					p_brg_data[i*p_bgr_img->widthStep + (width - 1 - j)*p_bgr_img->nChannels + k] = tmp;
				}
			}
		}
	}
	else
	{
		for (j = 0; j < (width >> 1); j++)
		{
			for (i = 0; i < height; i++)
			{
				for (k = 0; k < p_bgr_img->nChannels; k++)
				{
					uchar tmp = p_brg_data[i*p_bgr_img->widthStep + j*p_bgr_img->nChannels + k];
					p_brg_data[i*p_bgr_img->widthStep + j*p_bgr_img->nChannels + k] =
						p_brg_data[(height - 1 - i)*p_bgr_img->widthStep + j*p_bgr_img->nChannels + k];
					p_brg_data[(height - 1 - i)*p_bgr_img->widthStep + j*p_bgr_img->nChannels + k] = tmp;
				}
			}
		}
	}
	
}


/*
	Function Name:      raw_to_bgr
	Function Function : Convert raw data into IplImage* BGR image
	Input             : 
	    p_bgr_img     : The converted BGR image
		p_raw_img     : The raw data
	Return            : Void
	Note              : it's incorrect now, need to be updated
	Revision History  : Author         Data             Changes
	                    YanShuo        2017/10/31       Create
*/
void raw_to_bgr(IplImage *p_bgr_img, uchar* p_raw_img)
{
	uchar* pData = (uchar*)p_bgr_img->imageData;
	for (int i = 0; i < p_bgr_img->height; i++)
	{
		for (int j = 0; j < p_bgr_img->width; j++)
		{
			pData[i * p_bgr_img->widthStep + j * p_bgr_img->nChannels + 0] =
				p_raw_img[i * p_bgr_img->width + 0];
			pData[i * p_bgr_img->widthStep + j * p_bgr_img->nChannels + 1] =
				p_raw_img[i * p_bgr_img->width + 1];
			pData[i * p_bgr_img->widthStep + j * p_bgr_img->nChannels + 2] =
				p_raw_img[i * p_bgr_img->width + 2];
		}
	}
}

/*
	Function Name:      rgba_to_bgr
	Function Function : Convert raw data into IplImage* BGR image
	Input             : 
	    p_bgr_img     : The converted BGR image
		p_raw_img     : The raw data
	Return            : Void
	Note              : it's incorrect now, need to be updated
	Revision History  : Author         Data             Changes
	                    YanShuo        2017/2/23        Create
*/
void rgba_to_bgr(IplImage *p_bgr_img, uchar* p_raw_img)
{
	Int32_t pitch = p_bgr_img->width * p_bgr_img->height;
	uchar* p_data = (uchar*)p_bgr_img->imageData;
	for (int i = 0; i < p_bgr_img->height; i++)
	{
		for (int j = 0; j < p_bgr_img->width; j++)
		{
			p_data[i * p_bgr_img->widthStep + j * p_bgr_img->nChannels + 0] =
				p_raw_img[i * p_bgr_img->width * 4 + j * 4 + 2];
			p_data[i * p_bgr_img->widthStep + j * p_bgr_img->nChannels + 1] =
				p_raw_img[i * p_bgr_img->width * 4 + j * 4 + 1];
			p_data[i * p_bgr_img->widthStep + j * p_bgr_img->nChannels + 2] =
				p_raw_img[i * p_bgr_img->width * 4 + j * 4 + 0];
		}
	}
}

/*
	Function Name:      load_image
	Function Function : Load file into memory
	Input             : 
	      p_file      : The file pointer
		   p_img      : The image to be loaded
		  height      : The image height
		   width      : The image width
	  image_type      : The image type is in one of yuv422, yuv420 and bgr
	Return            : Error code
	Note              : 
	Revision History  : Author         Data             Changes
	                    YanShuo        2017/10/31       Create
*/
int load_image(FILE* p_file, uchar* p_img, Int32_t height, 
			   Int32_t width, Image_Type_E image_type)
{	
	int ret;
	if(!p_file)
	{
		printf("Open file error.\n");
		return -1;
	}
	
	switch (image_type)
	{
	case IMAGE_TYPE_YUV420:
		ret = (int)fread(p_img, sizeof(uchar), height * width * 3 >> 1, p_file);
		break;
	case IMAGE_TYPE_YUV422:
		ret = (int)fread(p_img, sizeof(uchar), height * width * 2, p_file);
		break;
	case IMAGE_TYPE_NV12:
		ret = (int)fread(p_img, sizeof(uchar), height * width * 3 >> 1, p_file);
		break;
	case IMAGE_TYPE_RGB:
		ret = (int)fread(p_img, sizeof(uchar), height * width * 3, p_file);
		break;
	case IMAGE_TYPE_RGBA:
		ret = (int)fread(p_img, sizeof(uchar), height * width * 4, p_file);
		break;
	default:
		break;
	}
	return ret;
}

/*
	Function Name:      split_raw_data
	Function Function : split raw data into four camera's data and only choose pointed frames
	Input             : 
	      file_name   : The raw data file name
		  path_name   : The raw data path name
	    start_frame   : The start frame's id
		  frame_num   : Valid frame number
		     height   : The image height
			  width   : The image width
	 
	Return            : Error code
	Note              : 
	Revision History  : Author         Data             Changes
	                    YanShuo        2017/10/31       Create
*/
void split_raw_data(const char* file_name, const char* path_name, const Int32_t start_frame, 
				    const Int32_t frame_num, const Int32_t height, const Int32_t width)
{
	char front_name[200], rear_name[200], left_name[200], right_name[200];
	FILE* p_file_video = NULL, *p_file_front = NULL, *p_file_rear = NULL;
	FILE* p_file_left = NULL, *p_file_right = NULL;
	Int32_t image_size = sizeof(uchar) * height * width * 3 >> 1;
	int i;
	
	uchar* p_front_image = (uchar*)malloc(image_size);
	uchar* p_rear_image = (uchar*)malloc(image_size);
	uchar* p_left_image = (uchar*)malloc(image_size);
	uchar* p_right_image = (uchar*)malloc(image_size);

	sprintf(front_name, "%s/Front.nv12", path_name);
	sprintf(rear_name, "%s/Rear.nv12", path_name);
	sprintf(left_name, "%s/Left.nv12", path_name);
	sprintf(right_name, "%s/Right.nv12", path_name);

	p_file_video = fopen(file_name, "rb");
	p_file_front = fopen(front_name, "wb");
	p_file_rear  = fopen(rear_name, "wb");
	p_file_left  = fopen(left_name, "wb");
	p_file_right = fopen(right_name, "wb");

	fseek(p_file_video, image_size * start_frame, 0);
	for(i = 0; i < (frame_num >> 2); i++)
	{
		fread(p_front_image, image_size, 1, p_file_video);
		fread(p_rear_image, image_size, 1, p_file_video);
		fread(p_left_image, image_size, 1, p_file_video);
		fread(p_right_image, image_size, 1, p_file_video);

		fwrite(p_front_image, image_size, 1, p_file_front);
		fwrite(p_rear_image, image_size, 1, p_file_rear);
		fwrite(p_left_image, image_size, 1, p_file_left);
		fwrite(p_right_image, image_size, 1, p_file_right);
	}

	if (p_front_image)
	{
		free(p_front_image);
	}
	if (p_rear_image)
	{
		free(p_rear_image);
	}
	if (p_left_image)
	{
		free(p_left_image);
	}
	if (p_right_image)
	{
		free(p_right_image);
	}

	fclose(p_file_video);
	fclose(p_file_front);
	fclose(p_file_rear);
	fclose(p_file_left);
	fclose(p_file_right);
}

void split_raw_data2(const char* file_name, const char* path_name, const Int32_t start_frame,
	const Int32_t frame_num, const Int32_t height, const Int32_t width)
{
	FILE* p_file_video = NULL, *p_file_segment = NULL;
	Int32_t image_size = sizeof(uchar)* height * width * 3 >> 1;
	int i;

	uchar* p_image = (uchar*)malloc(image_size);

	p_file_video = fopen(file_name, "rb");
	p_file_segment = fopen(path_name, "wb");

	fseek(p_file_video, image_size * start_frame, 0);
	for (i = 0; i < frame_num; i++)
	{
		fread(p_image, image_size, 1, p_file_video);

		fwrite(p_image, image_size, 1, p_file_segment);
	}

	if (p_image)
	{
		free(p_image);
	}

	fclose(p_file_video);
	fclose(p_file_segment);
}

/*
    Function Name:      image_copy_roi
    Function Function : Convert source image into dst image's roi region with scale
    Input             :
        p_src_img     : The source image
        p_dst_img     : The destination image
		      roi     : The roi of dst image
    Return            : Void
    Note              :
    Revision History  : Author         Data             Changes
                        YanShuo        2017/10/31       Create
*/
void image_copy_roi(const IplImage* p_src_img, IplImage* p_dst_img, CvRect roi)
{
	Int32_t	i, j, k;

	uchar* p_src_data = (uchar*)p_src_img->imageData;
	uchar* p_dst_data = (uchar*)p_src_img->imageData;

	for (i = 0; i < roi.height; i++)
	{
		for (j = 0; j < roi.width; j++)
		{			
			Point2f srcPoint;
			
			srcPoint.x = (float)j / roi.width * p_src_img->width;
			srcPoint.y = (float)i / roi.height * p_src_img->height;

			if (srcPoint.x >= p_src_img->width - 1)
				srcPoint.x -= 1;
			if (srcPoint.x >= p_src_img->height - 1)
				srcPoint.y -= 1;

			CvPoint upLeftPoint;
			upLeftPoint.x = (int)srcPoint.x;
			upLeftPoint.y = (int)srcPoint.y;

			Float32_t weight[4];
			weight[0] = (1 - (srcPoint.x - upLeftPoint.x)) * (1 - (srcPoint.y - upLeftPoint.y));
			weight[1] = (srcPoint.x - upLeftPoint.x) * (1 - (srcPoint.y - upLeftPoint.y));
			weight[2] = (1 - (srcPoint.x - upLeftPoint.x)) * (srcPoint.y - upLeftPoint.y);
			weight[3] = 1 - weight[0] - weight[1] - weight[2];

			double result[3];

			for (k = 0; k < MIN(p_dst_img->nChannels, p_src_img->nChannels); k++)
			{
				result[k] = p_src_data[upLeftPoint.y * p_src_img->widthStep + upLeftPoint.x * p_src_img->nChannels + k] * weight[0] +
					p_src_data[upLeftPoint.y * p_src_img->widthStep + (upLeftPoint.x + 1) * p_src_img->nChannels + k] * weight[1] +
					p_src_data[(upLeftPoint.y + 1) * p_src_img->widthStep + upLeftPoint.x * p_src_img->nChannels + k] * weight[2] +
					p_src_data[(upLeftPoint.y + 1) * p_src_img->widthStep + (upLeftPoint.x + 1) * p_src_img->nChannels + k] * weight[3];
			}
			
			for (k = 0; k < MIN(p_src_img->nChannels, p_src_img->nChannels); k++)
			{ 
				p_dst_data[i * p_dst_img->widthStep + j * p_dst_img->nChannels + k] =
					result[k] < 0 ? 0 : (result[k] > 255 ? 255 : (uchar)result[k]);
			}
		}
	}
}

