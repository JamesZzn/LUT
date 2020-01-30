
#ifndef _SINGLE_VIEW_H_
#define _SINGLE_VIEW_H_


#include <math.h>
#include "avm_interface.h"
#include "avm_load_parameter.h"
AVM_Error_Code_E single_view_zip_data(IN int shift_altogether, IN Float32_t *image_point, IN UInt32_t*p_lut, IN UInt32_t camera_flag);

AVM_Error_Code_E crop_front_rear_single_view(IN P_Avm_Param_S p_avm_param,IN int cam_flag);

AVM_Error_Code_E plane_front_rear_single_view(IN P_Avm_Param_S p_avm_param,IN int cam_flag);
AVM_Error_Code_E SLAM_3_plane_front_rear_single_view(IN P_Avm_Param_S p_avm_param, IN int cam_flag);
AVM_Error_Code_E  SLAM_plane_front_rear_single_view(IN P_Avm_Param_S p_avm_param, IN  int cam_flag);
AVM_Error_Code_E  SLAM_2_plane_front_rear_single_view(IN P_Avm_Param_S p_avm_param, IN  int cam_flag);
AVM_Error_Code_E  column_world_coord_front_rear_single_view(IN P_Avm_Param_S p_avm_param, IN  int cam_flag);
AVM_Error_Code_E  column_world_coord_left_right_single_view(IN P_Avm_Param_S p_avm_param, IN  int cam_flag);
AVM_Error_Code_E  SLAM_5_column_world_coord_left_right_single_view(IN P_Avm_Param_S p_avm_param, IN  int cam_flag);
AVM_Error_Code_E  column_camera_coord_front_rear_single_view(IN P_Avm_Param_S p_avm_param, IN  int cam_flag);

AVM_Error_Code_E box_front_rear_single_view(IN P_Avm_Param_S p_avm_param, IN int cam_flag);

AVM_Error_Code_E  left_right_crop_expend(IN P_Avm_Param_S p_avm_param,IN int cam_flag);

void calculate_world_coordinate_left_right(OUT Float32_t *world_coordinate, IN int i, IN int j, IN P_Avm_Param_S p_avm_param
	, IN Left_Right_Single_View_Param_S left_right_view_param, int cam_flag);

AVM_Error_Code_E left_right_overlook_expend(IN P_Avm_Param_S p_avm_param,IN int cam_flag);

void calculate_world_coordinate_combine_left_right(OUT Float32_t *world_coordinate, IN int i, IN int j, IN P_Avm_Param_S p_avm_param
	, IN Left_Right_Single_View_Param_S left_right_view_param, int cam_flag);

AVM_Error_Code_E left_right_combine_overlook_expend(IN P_Avm_Param_S p_avm_param,IN int cam_flag);

void calculate_world_coordinate_comine_left_right(OUT Float32_t *world_coordinate, IN int i, IN int j, IN P_Avm_Param_S p_avm_param
	, IN Single_View_Left_Right_Plane_Expand_Param_S left_right_view_param, int flag);

AVM_Error_Code_E lr_single_overlook(IN P_Avm_Param_S p_avm_param);

void lr_calculate_world_coordinate_combine(OUT Float32_t *world_coordinate, IN int i, IN int j, IN P_Avm_Param_S p_avm_param
	, IN Left_Right_View_Param_S left_right_view_param, int cam_flag);

AVM_Error_Code_E lr_combine_overlook_expend(IN P_Avm_Param_S p_avm_param);

#endif






