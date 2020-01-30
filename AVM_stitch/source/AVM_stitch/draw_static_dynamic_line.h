#ifndef  __DRAW_STATIC_DYNAMIC_LINE_H_
#define __DRAW_STATIC_DYNAMIC_LINE_H_
#include <math.h>
#include "avm_interface.h"
#include "avm_load_parameter.h"

void zip_data_draw_line(IN int shift_altogether, IN UInt32_t*p_lut, IN UInt32_t camera_flag, int i, int j, int color_flag);
AVM_Error_Code_E draw_static_plane_front_rear_single_view_line(IN P_Avm_Param_S p_avm_param, IN int cam_flag);
AVM_Error_Code_E original_crop_draw_static_plane_front_rear_single_view_line(IN P_Avm_Param_S p_avm_param, IN int cam_flag);
AVM_Error_Code_E  draw_dynamic_plane_front_rear_single_view_line(IN P_Avm_Param_S p_avm_param, IN int cam_flag);
void calculate_pic_coordinate_from_world_coordinate_0(P_Avm_Param_S p_avm_param, Front_Rear_Single_View_Param_S single_view_param,
	Float32_t *world_coordinate,  int cam_flag,
	Float32_t pixel_hight_dis, Float32_t pixel_width_dis, Float32_t *cam_ray,  int *pic_point_coordinate);
void calculate_rotate_coordinate_0(P_Avm_Param_S p_avm_param, Front_Rear_Single_View_Param_S single_view_param,
	Float32_t rotate_angle, Float32_t  pixel_hight_dis, Float32_t  pixel_width_dis,
	int cam_flag, IplImage *image_result_single_front_lut, Float32_t *cam_ray, int rotate_flag, UInt32_t * p_lut_line);
AVM_Error_Code_E draw_static_line_4_view(IN P_Avm_Param_S p_avm_param, IN  int cam_flag);
void calculate_static_rotate_coordinate(IN P_Avm_Param_S p_avm_param, IN  Junc_View_4_Param_S single_view_param,
	IN Float32_t rotate_angle, IN  int cam_flag, IN  IplImage *image_result_single_front_lut, IN  UInt32_t * p_lut_line, IN int small_big);
AVM_Error_Code_E  draw_dynamic_4_view_line(IN P_Avm_Param_S p_avm_param, IN int cam_flag);
void calculate_pic_coordinate_from_world_coordinate(P_Avm_Param_S p_avm_param, 
	Float32_t *world_coordinate,  int cam_flag,
	Float32_t pixel_hight_dis, Float32_t pixel_width_dis, Float32_t *cam_ray, int *pic_point_coordinate, Int32_t result_image_height, Int32_t result_image_width);
void calculate_dis(P_Avm_Param_S p_avm_param, Front_Rear_Single_View_Param_S single_view_param,
	Float32_t *world_coordinate, IplImage *image_result_single_front_lut, int cam_flag,
	Float32_t pixel_hight_dis, Float32_t pixel_width_dis, Float32_t *cam_ray, int flag, int *pic_point_1);

void calculate_static_rotate_coordinate_rear(P_Avm_Param_S p_avm_param, Junc_View_4_Param_S single_view_param,
	Float32_t rotate_angle, int cam_flag, IplImage *image_result_single_front_lut, UInt32_t * p_lut_line, int small_big);

void front_rear_draw_across_line(IN Int32_t key_ponit_pic_coordinate[16][2],
	OUT Int32_t *p_point_num, OUT UInt32_t *p_lut_line, IN Int32_t cam_flag, IplImage *image_result_single_front_lut);
void front_rear_draw_vertical_line(IN Int32_t key_ponit_pic_coordinate[16][2], OUT Int32_t *p_point_num,
	OUT UInt32_t *p_lut_line, IN Int32_t cam_flag, IplImage *image_result_single_front_lut);

void front_rear_draw_vertical_line_REAR(IN Int32_t key_ponit_pic_coordinate[16][2], OUT Int32_t *p_point_num, OUT UInt32_t *p_lut_line,
				IN Int32_t cam_flag);
AVM_Error_Code_E  draw_static_line_3_view(IN P_Avm_Param_S p_avm_param, IN int cam_flag);
AVM_Error_Code_E  draw_dynamic_3_view_line(IN P_Avm_Param_S p_avm_param, IN int cam_flag);

AVM_Error_Code_E  plane_front_rear_single_view_calculate_key_point_coordinate(IN P_Avm_Param_S p_avm_param, 
	IN  int cam_flag,IN Int32_t key_ponit_pic_coordinate[16][2]);

AVM_Error_Code_E  plane_front_rear_single_view_calculate_key_point_coordinate_one_point(IN P_Avm_Param_S p_avm_param, 
	IN  int cam_flag, Float32_t world_coord[3], IN Int32_t key_ponit_pic_coordinate[2]);






void gpu_fill_point(Int32_t *every_angle_point_num, UInt32_t * p_lut_line, IplImage *image_result_single_front_lut,
	Int32_t cam_flag, Int32_t  key_ponit_pic_coordinate_temp[2], Int32_t j, Int32_t i,
	Int32_t result_image_height,
	Int32_t result_image_width, Float32_t car_head[3], Int32_t rotate_flag);
void cpu_fill_point(Int32_t *every_angle_point_num, UInt32_t * p_lut_line, IplImage *image_result_single_front_lut,
	Int32_t cam_flag, Int32_t  key_ponit_pic_coordinate_temp[2], Int32_t j, Int32_t i,
	Int32_t result_image_height,
	Int32_t result_image_width, Float32_t car_head[3], Int32_t rotate_flag);

AVM_Error_Code_E static_line_left_right(IN P_Avm_Param_S p_avm_param, IN int cam_flag);
AVM_Error_Code_E lr_single_static_line(IN P_Avm_Param_S p_avm_param);
#endif