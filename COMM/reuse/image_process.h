/*
* This file provides some basic none encoded image processing function for all projects.
* Create by Shuo.Yan on 2016/5/13
*/
#ifndef __IMAGE_PROCESS_H_
#define __IMAGE_PROCESS_H_

#include "opencv.h"
#include "data_type.h"
#include "math_common.h"
#include "stdio.h"

Int32_t Create_Intesight_Image(P_Intesight_Image_S* pp_image, Image_Type_E image_type, Int32_t height, Int32_t width);
void Release_Intesight_Image(P_Intesight_Image_S p_image);

void bilinear_interpolation(IplImage* p_src_img, 
							Int32_t rgb[3], 
							Float32_t x_src_loc, 
							Float32_t y_src_loc);
void bilinear_interpolation_new(IplImage* p_src_img,
	Int32_t rgb[3],
	Int32_t x0,
	Int32_t y0,
	Float32_t weightUpLeft,
	Float32_t weightUpRight,
	Float32_t weightDownLeft,
	Float32_t weightDownRight
	);

void bgr_to_gray(IplImage* p_rgb_img, IplImage* p_gray_img);

void bgr_to_yuv420(const IplImage* p_rgb_img, uchar* p_yuv420_img);

void yuv420_to_bgr(IplImage *p_bgr_img, uchar* p_uuv420_img);

void yuv422_to_bgr(IplImage *p_bgr_img, uchar* p_yuv422_img);

void nv12_to_bgr(IplImage *p_bgr_img, uchar* p_nv12_img);

void yuv420_cut(uchar* p_dst_yuv420_img, uchar* p_src_yuv420_img, 
				Int32_t src_height, Int32_t src_width, 
				Int32_t dst_height, Int32_t dst_width);

void yuv422_cut(uchar* p_dst_yuv422_img, uchar* p_src_yuv422_img, 
				Int32_t src_height, Int32_t src_width, 
				Int32_t dst_height, Int32_t dst_width);

void nv12_cut(uchar* p_dst_nv12_img, uchar* p_src_nv12_img, 
			  Int32_t src_height, Int32_t src_width, 
			  Int32_t dst_height, Int32_t dst_width);

void rgba_cut(uchar* p_dst_rgba_img, uchar* p_src_rgba_img, 
			  Int32_t src_height, Int32_t src_width, 
			  Int32_t dst_height, Int32_t dst_width);

void flip_image(IplImage *p_bgr_img, bool is_horizon);

void raw_to_bgr(IplImage *p_bgr_img, uchar* p_raw_img);

void rgba_to_bgr(IplImage *p_bgr_img, uchar* p_raw_img);

int load_image(FILE* p_file, uchar* p_image, Int32_t height,
	Int32_t width, Image_Type_E image_type);

void split_raw_data(const char* file_name, const char* path_name, 
	const Int32_t start_frame, const Int32_t frame_num,
	const Int32_t height, const Int32_t width);

void image_copy_roi(const IplImage* p_src_img, IplImage* p_dst_img, CvRect roi);

void split_raw_data2(const char* file_name, const char* path_name,
	const Int32_t start_frame, const Int32_t frame_num,
	const Int32_t height, const Int32_t width);

#endif