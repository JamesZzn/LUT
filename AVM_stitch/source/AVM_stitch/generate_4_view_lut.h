#ifndef  __GENERATE_4_VIEW_LUT_H_
#define __GENERATE_4_VIEW_LUT_H_
#include <math.h>
#include "avm_interface.h"
#include "avm_load_parameter.h"



AVM_Error_Code_E generate_4_view_lut( IN P_Avm_Param_S p_avm_param, IN Result_Size_Param_S result_param, IN UInt32_t* p_lut);

void zip_data(IN int shift_altogether, IN Float32_t *image_point, IN Float32_t weight_Float32_t, IN UInt32_t*p_lut, IN UInt32_t camera_flag);

AVM_Error_Code_E world_coordinate_to_pic_coordinate(IN int camera_id, IN Float32_t * world_coordinate, OUT Float32_t *image_point, IN P_Avm_Param_S p_avm_param);

void calculate_world_coordinate(OUT IN Float32_t *world_coordinate, IN int i, IN int j, IN P_Avm_Param_S p_avm_param, IN Result_Size_Param_S result_big_param);

AVM_Error_Code_E  zip_data(IN int shift_altogether, IN Float32_t *image_point, IN Float32_t weight_Float32_t,
	IN UInt32_t*p_lut, IN UInt32_t camera_flag, IN UInt32_t left_right_flag);
AVM_Error_Code_E  zip_data_new(IN int shift_altogether, IN Float32_t *image_point, IN Float32_t weight_Float32_t,
	IN UInt32_t*p_lut, IN UInt32_t camera_flag, IN UInt32_t left_right_flag, IN P_Avm_Param_S p_avm_param);
Float32_t calculate_4_view_weight_curve(int camera_flag, int i, int j, Float32_t *world_coordinate, Float32_t *weight_2_dim, IN P_Avm_Param_S p_avm_param, IN Result_Size_Param_S result_param,  int view_flag);

#endif
