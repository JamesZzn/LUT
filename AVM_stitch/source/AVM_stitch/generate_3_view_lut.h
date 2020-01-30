#ifndef  __GENERATE_3_VIEW_LUT_H_
#define __GENERATE_3_VIEW_LUT_H_

#include <math.h>
#include "avm_interface.h"
#include "avm_load_parameter.h"



AVM_Error_Code_E generate_3_front_view_lut( IN P_Avm_Param_S p_avm_param);




AVM_Error_Code_E _3_front_view_zip_data(IN int shift_altogether, IN Float32_t *image_point, IN Float32_t weight_Float32_t, IN P_Avm_Param_S p_avm_param);

void _3_front_view_world_coordinate_to_pic_coordinate(IN int camera_id, IN Float32_t * world_coordinate, OUT Float32_t *image_point, IN P_Avm_Param_S p_avm_param);

void _3_front_view_calculate_world_coordinate(OUT IN Float32_t *world_coordinate, IN int i, IN int j, IN P_Avm_Param_S p_avm_param);

AVM_Error_Code_E generate_3_rear_view_lut(IN P_Avm_Param_S p_avm_param);

AVM_Error_Code_E _3_rear_view_zip_data(IN int shift_altogether, IN Float32_t *image_point, IN Float32_t weight_Float32_t, IN P_Avm_Param_S p_avm_param);

void _3_rear_view_world_coordinate_to_pic_coordinate(IN int camera_id, IN Float32_t * world_coordinate, OUT Float32_t *image_point, IN P_Avm_Param_S p_avm_param);


void  _3_rear_view_calculate_world_coordinate(OUT Float32_t *world_coordinate, IN int i, IN int j, IN P_Avm_Param_S p_avm_param);

Float32_t  _3_rear_view_calculate_front_weight(int i, int j, Float32_t *world_coordinate, Float32_t *weight_2_dim, IN P_Avm_Param_S p_avm_param);

#endif

