/*
 * This file defines the structure used in AVM module
 * Create By Shuo.Yan on 2017/11/23
 */

#ifndef __AVM_DATA_TYPE_H_
#define __AVM_DATA_TYPE_H_

#include "data_type.h"
#include "camera_model.h"
#include "vehicle_info.h"
#include "opencv.h"
#include"avm_config.h"

struct LutItem
{
	unsigned int dstInfo;
	unsigned int srcInfo;
	//unsigned int weightInfo;
};

// the color id represents static and dynamic line's color information
typedef enum Line_Color_Enum
{
	COLOR_RED = 0,
	COLOR_YELLOW,
	COLOR_GREEN
}Line_Color_E;

typedef enum Rotate_Flag
{
	ROTATE_PLANE_EXPEND_FRONT = 0,
	ROTATE_4_VIEW_BIG_FRONT,
	ROTATE_4_VIEW_SMALL_FRONT,
	ROTATE_PLANE_EXPEND_REAR,
	ROTATE_4_VIEW_BIG_REAR,
	ROTATE_4_VIEW_SMALL_REAR,
	ROTATE_3_VIEW_FRONT,
	ROTATE_3_VIEW_REAR
}Rotate_F;


// single view expand type, can only by 012 for front and rear view, 013 for left and right view
typedef enum Single_View_Expand_Enum
{
	TYPE_CROP = 0,
	TYPE_PLANE_EXPAND,
	TYPE_BOX_EXPAND,
	TYPE_COMBINE_EXPAND
}Single_View_Expand_E;


typedef enum View_Angle
{
	_4_VIEW_BIG = 0,
	_4_VIEW_SMALL,
	_3_VIEW_FRONT,
	_3_VIEW_REAR,
	_1_VIEW_FRONT,
	_1_VIEW_REAR,
	_1_VIEW_LEFT,
	_1_VIEW_RIGHT,
	_1_VIEW_LEFT_RIGHT
}View_Ang;



typedef struct Single_View_Crop_Param_Struct
{
	Float32_t	roll_angle;
	CvRect		crop_rect;															// the cropped rect in source image
														// inverse time-clock is positive, unit is degree
}Single_View_Crop_Param_S, *P_Single_View_Crop_Param_S;

typedef struct Single_View_Front_Rear_Plane_Expand_Param_Struct
{	
	Int32_t		trans_x;															// the x direction translation in source image, unit is pixel
	Int32_t		trans_y;															// the y direction translation in source image, unit is pixel
	Float32_t	fov_h;																// y direction field of view kept in plane expand image, unit is degree
	Float32_t	fov_v;																// y direction field of view kept in plane expand image, unit is degree
}Single_View_Front_Rear_Plane_Expand_Param_S, *P_Single_View_Front_Rear_Plane_Expand_Param_S;

typedef struct Single_View_Front_Rear_Box_Expand_Param_Struct
{
	Int32_t		center_width;														// center part length in result image, unit is pixel
	Int32_t		trans_y;															// translation in y direction, unit is pixel
	Float32_t	fov_h_center;														// center part field of view kept in box view image, unit is degree
	Float32_t	fov_h_side;															// side part field of view kept in box view image, is symmetrical by default, unit is degree
	Float32_t	fov_v;																// y direction field of view kept in box view image, unit is degree
}Single_View_Front_Rear_Box_Expand_Param_S, *P_Single_View_Front_Rear_Box_Expand_Param_S;

typedef struct Single_View_Left_Right_Plane_Expand_Param_Struct
{
	Float32_t	trans_y;															// the ROI trans y coordinate in world coordinate system, default is 0, unit is mm
	Float32_t	world_width;														// the ROI width in world coordinate system, unit is mm
	Float32_t	world_height;														// the ROI height in world coordinate system, unit is mm
	Int32_t		car_body_width;														// car body pixel in result image, unit is pixel
}Single_View_Left_Right_Plane_Expand_Param_S, *P_Single_View_Left_Right_Plane_Expand_Param_S;

typedef struct Single_View_Left_Right_Combine_Expand_Param_Struct
{
	Float32_t	trans_y;															// translation in y direction in world coordinate system, default is 0, unit is mm
	Int32_t		car_body_width;														// car body pixel in result image, unit is pixel
	Float32_t	world_height;														// visible height in world coordinate system, unit is mm
	Float32_t	plane_width;														// plane part width in world coordinate system, unit is mm
	Float32_t	plane_ratio;														// plane part ratio, N/A
	Float32_t	valid_angle_range;													// cylindrical part view range, unit is degree
}Single_View_Left_Right_Combine_Expand_Param_S, *P_Single_View_Left_Right_Combine_Expand_Param_S;

typedef struct Static_Line_Param_Struct
{
	Int32_t		is_static_line_exist;												// N/A
	Int32_t		static_line_dis_to_car[4];											// unit is mm, horizontal distance line to car
	Int32_t		static_line_length;													// unit is mm, static line length
	Int32_t		static_line_pixel_count;											// N/A, static line pixel count
}Static_Line_Param_S, *P_Static_Line_Param_S;

typedef struct Dynamic_Line_Param_Struct
{
	Int32_t		is_dynamic_line_exist;												// N/A
	Int32_t		dynamic_line_count;													// dynamic line count, default is 63 [-31, 31]
	Int32_t		dynamic_line_length;												// unit is mm, dynamic line length
	Int32_t		dynamic_line_pixel_count[MAX_DYNAMIC_LINE_COUNT];					// N/A, how many pixels per dynamic line contains
}Dynamic_Line_Param_S, *P_Dynamic_Line_Param_S;

typedef struct Dis_Line_Param_Struct
{
	Static_Line_Param_S		static_line_param;										// static line param structure
	Dynamic_Line_Param_S	dynamic_line_param;										// dynamic line param structure
}Dis_Line_Param_S, *P_Dis_Line_Param_S;

// 2d view result config parameters
typedef struct Result_Size_Param_Struct
{
	Int32_t		result_image_height;												// unit is pixel
	Int32_t		result_image_width;													// unit is pixel
	Int32_t		lut_size[CAMERA_NUM];												// unit is byte, how many byte does each part lut takes, 8 * elem_count
	CvRect		car_logo_rect;														// unit is pixel, car logo init position and size relative to image up-left point, start from 0	
}Result_Size_Param_S, *P_Result_Size_Param_S;

// 2d 4-pass view common config parameters
typedef struct Junc_View_4_Config_Common_Struct
{
	Float32_t	front_view_length;													// unit is mm
	Float32_t	rear_view_length;													// unit is mm
	Float32_t	left_view_length;													// unit is mm
	Float32_t	right_view_length;													// unit is mm
	Float32_t	front_blind_length;													// unit is mm
	Float32_t	rear_blind_length;													// unit is mm
	Float32_t	fl_fusion_angle;													// unit is degree, front and left fusion angle with x axis
	Float32_t	fr_fusion_angle;													// unit is degree, front and right fusion angle with x axis
	Float32_t	rl_fusion_angle;													// unit is degree, rear and left fusion angle with x axis
	Float32_t	rr_fusion_angle;													// unit is degree, rear and right fusion angle with x axis
	Float32_t	fl_fusion_range;													// unit is degree, front and left fusion range
	Float32_t	fr_fusion_range;													// unit is degree, front and right fusion range
	Float32_t	rl_fusion_range;													// unit is degree, rear and left fusion range
	Float32_t	rr_fusion_range;													// unit is degree, rear and right fusion range
	Float32_t	parallel_range;
	Float32_t	curve_range;
}Junc_View_4_Config_Common_S, *P_Junc_View_4_Config_Common_S;

// 2d 3-pass front view bird eye view parameters
typedef struct Junc_View_3_Front_Config_Common_Struct
{
	Float32_t	front_view_length;													// unit is mm
	Float32_t	left_view_length;													// unit is mm
	Float32_t	right_view_length;													// unit is mm
	Float32_t	car_head_length;													// unit is mm, car head image occluded length
	Float32_t	front_blind_length;													// unit is mm
	Float32_t	fl_fusion_angle;													// unit is degree, front and left fusion angle with x axis
	Float32_t	fr_fusion_angle;													// unit is degree, front and right fusion angle with x axis	
	Float32_t	fl_fusion_range;													// unit is degree, front and left fusion range
	Float32_t	fr_fusion_range;													// unit is degree, front and right fusion range	
	Float32_t	parallel_range;
	Float32_t	curve_range;
}Junc_View_3_Front_Config_Common_S, *P_Junc_View_3_Front_Config_Common_S;

// 2d 3-pass rear view bird eye view parameters
typedef struct Junc_View_3_Rear_Config_Common_Struct
{
	Float32_t	rear_view_length;													// unit is mm
	Float32_t	left_view_length;													// unit is mm
	Float32_t	right_view_length;													// unit is mm
	Float32_t	car_rear_length;													// unit is mm, car rear image occluded length
	Float32_t	rear_blind_length;													// unit is mm
	Float32_t	rl_fusion_angle;													// unit is degree, rear and left fusion angle with x axis
	Float32_t	rr_fusion_angle;													// unit is degree, rear and right fusion angle with x axis	
	Float32_t	rl_fusion_range;													// unit is degree, rear and left fusion range
	Float32_t	rr_fusion_range;													// unit is degree, rear and right fusion range	
	Float32_t	parallel_range;
	Float32_t	curve_range;
}Junc_View_3_Rear_Config_Common_S, *P_Junc_View_3_Rear_Config_Common_S;

// 2D 4-pass bird eye view parameters
typedef struct Junc_View_4_Param_Struct
{
	Junc_View_4_Config_Common_S		common_config;									// 4 pass panorama common config
	Result_Size_Param_S				result_small_param;								// small result image param
	Result_Size_Param_S				result_big_param;								// big result image param
	Dis_Line_Param_S				small_view_dis_line_param;						// 4 pass small view distance line config param
	Dis_Line_Param_S				big_view_dis_line_param;						// 4 pass big view distance line config param
}Junc_View_4_Param_S, *P_Junc_View_4_Param_S;

// 2D awb view parameters
typedef struct Awb_View_Param_Struct
{
	Junc_View_4_Config_Common_S		common_config;									// 4 pass panorama common config
	Result_Size_Param_S				result_awb_param;								// result image param	
}Awb_View_Param_S, *P_Awb_View_Param_S;

// 2D front 3-pass bird eye view parameters
typedef struct Junc_View_3_Front_Param_Struct
{
	Junc_View_3_Front_Config_Common_S	common_config;								// 3 pass front view config param
	Result_Size_Param_S					result_3_front_param;						// result image param
	Dis_Line_Param_S					junc_3_front_dis_line_param;				// 3 pass front view distance line config param
}Junc_View_3_Front_Param_S, *P_Junc_View_3_Front_Param_S;

// 2D rear 3-pass bird eye view parameters
typedef struct Junc_View_3_Rear_Param_Struct
{
	Junc_View_3_Rear_Config_Common_S	common_config;								// 3 pass rear view config param
	Result_Size_Param_S					result_3_rear_param;						// result image param
	Dis_Line_Param_S					junc_3_rear_dis_line_param;					// 3 pass rear view distance line config param
}Junc_View_3_Rear_Param_S, *P_Junc_View_3_Rear_Param_S;

// 2D multi-pass bird eye view parameters
typedef struct Avm_2D_Param_Struct
{
	Awb_View_Param_S			awb_view_param;										// awb view config information
	Junc_View_4_Param_S			junc_view_4_param;									// 4-pass junction view config information
	Junc_View_3_Front_Param_S	junc_view_3_front_param;							// front 3-pass junction view config information
	Junc_View_3_Rear_Param_S	junc_view_3_rear_param;								// rear 3-pass junction view config information
}Avm_2D_Param_S, *P_Avm_2D_Param_S;

// front single view parameters
typedef struct Front_Rear_Single_View_Param_Struct
{
	Int32_t										result_image_height;				// unit is pixel
	Int32_t										result_image_width;					// unit is pixel
	Single_View_Expand_E						expand_type;						// single view expand type
	Single_View_Crop_Param_S					crop_param;							// only valid when expand_type is TYPE_CROP
	Single_View_Front_Rear_Plane_Expand_Param_S	plane_expand_param;					// only valid when expand_type is TYPE_PLANE_EXPAND
	Single_View_Front_Rear_Box_Expand_Param_S	box_expand_param;					// only valid when expand_type is TYPE_BOX_EXPAND
	Dis_Line_Param_S							dis_line_param;						// front rear single view distance line param
}Front_Rear_Single_View_Param_S, *P_Front_Rear_Single_View_Param_S;

// left single view parameters
typedef struct Left_Right_Single_View_Param_Struct
{
	Int32_t												result_image_height;		// unit is pixel
	Int32_t												result_image_width;			// unit is pixel
	Single_View_Expand_E								expand_type;				// single view expand type
	Single_View_Crop_Param_S							crop_param;					// only valid when expand_type is TYPE_CROP
	Single_View_Left_Right_Plane_Expand_Param_S			plane_expand_param;			// only valid when expand_type is TYPE_PLANE_EXPAND
	Single_View_Left_Right_Combine_Expand_Param_Struct	combine_expand_param;		// only valid when expand_type is TYPE_COMBINE_EXPAND
	Dis_Line_Param_S									dis_line_param;				// left right single view distance line param
}Left_Right_Single_View_Param_S, *P_Left_Right_Single_View_Param_S;

// left and right together view parameters
typedef struct Left_Right_View_Param_Struct
{
	Int32_t												result_image_height;		// unit is pixel
	Int32_t												result_image_width;			// unit is pixel
	Single_View_Expand_E								expand_type;				// single view expand type
	Single_View_Crop_Param_S							crop_param_left;			// only valid when expand_type is TYPE_CROP
	Single_View_Left_Right_Plane_Expand_Param_S			plane_expand_param_left;	// only valid when expand_type is TYPE_PLANE_EXPAND
	Single_View_Left_Right_Combine_Expand_Param_Struct	combine_expand_param_left;	// only valid when expand_type is TYPE_COMBINE_EXPAND
	Dis_Line_Param_S									dis_line_param_left;		// left_right single view left distance line param
	Single_View_Crop_Param_S							crop_param_right;			// only valid when expand_type is TYPE_CROP
	Single_View_Left_Right_Plane_Expand_Param_S			plane_expand_param_right;	// only valid when expand_type is TYPE_PLANE_EXPAND
	Single_View_Left_Right_Combine_Expand_Param_Struct	combine_expand_param_right;	// only valid when expand_type is TYPE_COMBINE_EXPAND
	Dis_Line_Param_S 									dis_line_param_right;		// left_right single view right distance line param
}Left_Right_View_Param_S, *P_Left_Right_View_Param_S;

// single view parameters
typedef struct Avm_Single_View_Param_Struct
{
	Front_Rear_Single_View_Param_S		front_single_view_param;					// front single view config param
	Front_Rear_Single_View_Param_S		rear_single_view_param;						// rear single view config param
	Left_Right_Single_View_Param_S		left_single_view_param;						// left single view config param
	Left_Right_Single_View_Param_S		right_single_view_param;					// right single view config param
	Left_Right_View_Param_S				left_right_view_param;						// left and right together view config param
}Avm_Single_View_Param_S, *P_Avm_Single_View_Param_S;

// avm 3D parameters // need to by revised later
typedef struct Avm_3D_Param_Struct
{

	Float64_t m_centerHeight = 18000; // 平底球模型中球心z坐标
	Float64_t m_circleRadius = 8000; // 平底球模型中底平面半径
	CvPoint3D64f m_newObserLoc; // 观测位置
	Float64_t m_picthAngle = 0; // 相机俯仰角
	Float64_t m_rollAngle = 90; // 相机滚桶角  cheng 17.6.14 为增加斜方向视图添加
	Float64_t m_newFocal = 4; // 相机焦距
	CvSize m_dstSize;//目的尺寸

	Float64_t m_horiFov = 80; // 相机水平视场角
	Float64_t m_magScaleVert = 1;
	Float64_t m_magScaleHori = 1;
	Float64_t m_upperLimit = 200; // 单位为像素
	Float64_t m_fusionLoc[4]; // 单位为角度
	Float64_t m_fusionAngle[4]; // 单位为角度
	CvPoint2D64f m_carUpLeft;
	CvPoint2D64f m_carDownRight;


	Float64_t m_ballRadius;
	Float64_t m_sf = 10;
	CvSize m_srcSize;

	CvPoint m_minDstPoint[CAMERA_NUM]; // 前后左右四相机原图在结果图中的ROI左上顶点坐标
	CvPoint m_maxDstPoint[CAMERA_NUM]; // 前后左右四相机原图在结果图中的ROI右下顶点坐标
	//vector<LutItem> m_lut; // 查找表项写表时候去用
	bool m_is_lumi_balance = false;//z只有i==0时才为真
	Int32_t		result_image_height;												// unit is pixel
	Int32_t		result_image_width;													// unit is pixel
	CvRect		car_logo_rect[VIEW_COUNT_3D];										// 3d car log rect
}Avm_3D_Param_S, *P_Avm_3D_Param_S;

// avm config parameters
typedef struct Avm_Config_Param_Struct
{
	Vehicle_Param_S				vehicle_param;										// vehicle related param
	Camera_Model_S				camera_model[CAMERA_NUM];							// camera model
	Avm_Single_View_Param_S		avm_single_view_param;								// single view image config param
	Avm_2D_Param_S				avm_2d_param;										// 2d panorama config param
	Avm_3D_Param_S				avm_3d_param;										// 3d panorama config param
}Avm_Config_Param_S, *P_Avm_Config_Param_S;

typedef struct Avm_Lut_Struct
{
	UInt32_t*	p_lut_single_view_front;											// front single view lut pointer
	UInt32_t*	p_lut_single_view_rear;												// rear single view lut pointer
	UInt32_t*	p_lut_single_view_left;												// left single view lut pointer
	UInt32_t*	p_lut_single_view_right;											// right single view lut pointer
	UInt32_t*	p_lut_single_view_left_right;										// left and right single view lut pointer
	UInt32_t*	p_lut_junc_view_4_small;											// junction 4 pass small view lut pointer
	UInt32_t*	p_lut_junc_view_4_big;												// junction 4 pass big view lut pointer
	UInt32_t*	p_lut_junc_view_3_front;											// junction 3 pass front view lut pointer
	UInt32_t*	p_lut_junc_view_3_rear;												// junction 3 pass back view lut point
	UInt32_t*	p_lut_awb;															// 2d view for global auto white balance
	UInt32_t*	p_lut_3d[VIEW_COUNT_3D];											// 3d view lut, need to be expanded
	
}Avm_Lut_S, *P_Avm_Lut_S;

typedef struct Avm_Logo_Image_Struct
{
	P_Intesight_Image_S		logo_image_junc_view_4_small;							// small 4-pass junction view' car logo image
	P_Intesight_Image_S		logo_image_junc_view_4_big;								// big 4-pass junction view' car logo image
	P_Intesight_Image_S		logo_image_junc_view_3_front;							// front 3-pass junction view' car logo image
	P_Intesight_Image_S		logo_image_junc_view_3_rear;							// rear 3-pass junction view' car logo image
	P_Intesight_Image_S		logo_image_3d[VIEW_COUNT_3D];							// 3d view logo images
}Avm_Logo_Image_S, *P_Avm_Logo_Image_S;

typedef struct Avm_Result_Image_Struct
{
	P_Intesight_Image_S		result_image_single_view_front;							// front single view result image
	P_Intesight_Image_S		result_image_single_view_rear;							// rear single view result image
	P_Intesight_Image_S		result_image_single_view_left;							// left single view result image
	P_Intesight_Image_S		result_image_single_view_right;							// right single view result image
	P_Intesight_Image_S		result_image_single_view_left_right;					// left & right single view result image
	P_Intesight_Image_S		result_image_junc_view_4_small;							// small 4-pass junction view image
	P_Intesight_Image_S		result_image_junc_view_4_big;							// big 4-pass junction view image
	P_Intesight_Image_S		result_image_junc_view_3_front;							// front 3-pass junction view image
	P_Intesight_Image_S		result_image_junc_view_3_rear;							// rear 3-pass junction view image
	P_Intesight_Image_S		result_image_awb;										// 2d image for calculating awb param
	P_Intesight_Image_S		result_image_3d[VIEW_COUNT_3D];							// 3d result view image array
}Avm_Result_Image_S, *P_Avm_Result_Image_S;

typedef struct Avm_Image_Struct
{
	P_Intesight_Image_S		source_image_array[CAMERA_NUM];							// source image array
	Avm_Result_Image_S		result_image;											// result image structure
	Avm_Logo_Image_S		logo_image;												// logo image
}Avm_Image_S, *P_Avm_Image_S;

typedef struct Avm_Dis_Line_Struct
{
	UInt32_t*	p_static_dis_line_front;											// front single view static distance line lut
	UInt32_t*	p_dynamic_dis_line_front;											// front single view dynamic distance line lut
	UInt32_t*	p_static_dis_line_rear;												// rear single view static distance line lut
	UInt32_t*	p_dynamic_dis_line_rear;											// rear single view dynamic distance line lut
	UInt32_t*	p_static_dis_line_left;												// left single view static distance line lut
	UInt32_t*	p_dynamic_dis_line_left;											// left single view dynamic distance line lut
	UInt32_t*	p_static_dis_line_right;											// right single view static distance line lut
	UInt32_t*	p_dynamic_dis_line_right;											// right single view dynamic distance line lut
	UInt32_t*	p_static_dis_line_left_right;										// left right together single view static distance line lut
	UInt32_t*	p_dynamic_dis_line_left_right;										// left right together single view dynamic distance line lut

	UInt32_t* p_static_dis_line_4_small;											// avm 2d 4-pass small view static distance line lut
	UInt32_t* p_dynamic_dis_line_4_small;											// avm 2d 4-pass small view dynamic distance line lut
	UInt32_t* p_static_dis_line_4_big;												// avm 2d 4-pass big view static distance line lut
	UInt32_t* p_dynamic_dis_line_4_big;												// avm 2d 4-pass big view dynamic distance line lut
	UInt32_t* p_static_dis_line_3_front;											// avm 2d 3-pass front view static distance line lut
	UInt32_t* p_dynamic_dis_line_3_front;											// avm 2d 3-pass front view dynamic distance line lut
	UInt32_t* p_static_dis_line_3_rear;												// avm 2d 3-pass rear view static distance line lut
	UInt32_t* p_dynamic_dis_line_3_rear;											// avm 2d 3-pass rear view dynamic distance line lut
}Avm_Dis_Line_S, *P_Avm_Dis_Line_S;

typedef struct Avm_Param_Struct
{                                                                          
	P_Avm_Config_Param_S	p_avm_config_param;										// AVM config param
	P_Avm_Lut_S				p_avm_lut;												// AVM look up table structure
	P_Avm_Image_S			p_avm_image;											// AVM image structure
	P_Avm_Dis_Line_S		p_avm_dis_line;											// AVM distance line structure
	Int32_t                 project_flag;
}Avm_Param_S, *P_Avm_Param_S;

#endif