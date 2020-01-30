
#include <math.h>
#include "avm_interface.h"
#include "avm_load_parameter.h"



AVM_Error_Code_E generate_3_front_view_lut( IN P_Avm_Param_S p_avm_param);




void _3_front_view_zip_data(IN int shift_altogether, IN Float64_t *image_point, IN Float64_t weight_Float64_t, IN P_Avm_Param_S p_avm_param);

void _3_front_view_world_coordinate_to_pic_coordinate(IN int camera_id, IN Float64_t * world_coordinate, OUT Float64_t *image_point, IN P_Avm_Param_S p_avm_param);

void _3_front_view_calculate_world_coordinate(OUT IN Float64_t *world_coordinate, IN int i, IN int j, IN P_Avm_Param_S p_avm_param);

Float64_t  _3_front_view_calculate_front_weight(int i, int j, Float64_t *world_coordinate, Float64_t *weight_2_dim, IN P_Avm_Param_S p_avm_param);


AVM_Error_Code_E generate_3_rear_view_lut(IN P_Avm_Param_S p_avm_param);

void _3_rear_view_zip_data(IN int shift_altogether, IN Float64_t *image_point, IN Float64_t weight_Float64_t, IN P_Avm_Param_S p_avm_param);

void _3_rear_view_world_coordinate_to_pic_coordinate(IN int camera_id, IN Float64_t * world_coordinate, OUT Float64_t *image_point, IN P_Avm_Param_S p_avm_param);


void  _3_rear_view_calculate_world_coordinate(OUT Float64_t *world_coordinate, IN int i, IN int j, IN P_Avm_Param_S p_avm_param);

Float64_t  _3_rear_view_calculate_front_weight(int i, int j, Float64_t *world_coordinate, Float64_t *weight_2_dim, IN P_Avm_Param_S p_avm_param);




Float64_t _3_view_calculate_weight(int i, int j, Float64_t *world_coordinate, Float64_t *weight_2_dim, IN P_Avm_Param_S p_avm_param, IN Int32_t view_angle);

void  _3_view_calculate_world_coordinate(OUT Float64_t *world_coordinate, IN int i, IN int j, IN P_Avm_Param_S p_avm_param, IN int angle_view);

void _3_view_world_coordinate_to_pic_coordinate(IN int camera_id, IN Float64_t * world_coordinate, OUT Float64_t *image_point, IN P_Avm_Param_S p_avm_param);

void _3_view_zip_data(IN int shift_altogether, IN Float64_t *image_point, IN Float64_t weight_Float64_t, IN P_Avm_Param_S p_avm_param,
	IN UInt32_t* p_lut, IN UInt32_t camera_flag);

