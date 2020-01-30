/*
 * This head file provides function declaration for all config file operations through IO
 * Create by Shuo.Yan on 2017/11/23
 */

#ifndef __AVM_LOAD_PARAMETER_H_
#define __AVM_LOAD_PARAMETER_H_

#include "avm_data_type.h"
#include "avm_error.h"
#include <stdio.h>

AVM_Error_Code_E Load_Junc_View_4_Param(
	OUT P_Junc_View_4_Param_S p_junc_view_4_param, IN const char* file_name,
	IN P_Vehicle_Param_S p_vehicle_param);

AVM_Error_Code_E Load_Junc_View_3_Front_Param(
	OUT P_Junc_View_3_Front_Param_S p_junc_view_3_front_param, IN const char* file_name, IN P_Vehicle_Param_S p_vehicle_param);

AVM_Error_Code_E Load_Junc_View_3_Rear_Param(
	OUT P_Junc_View_3_Rear_Param_S p_junc_view_3_rear_param, IN const char* file_name, IN P_Vehicle_Param_S p_vehicle_param);

AVM_Error_Code_E Load_Awb_View_Param(
	OUT P_Awb_View_Param_S p_awb_view_param, IN const char* file_name, IN P_Vehicle_Param_S p_vehicle_param);

AVM_Error_Code_E Load_Front_Rear_Single_View_Param(
	OUT P_Front_Rear_Single_View_Param_S p_fr_single_view_param,
	IN const char* file_name);

AVM_Error_Code_E Load_Left_Right_Single_View_Param(
	OUT P_Left_Right_Single_View_Param_S p_lr_single_view_param,
	IN const char* file_name);

AVM_Error_Code_E Load_Left_Right_Together_View_Param(
	OUT P_Left_Right_View_Param_S p_lr_together_view_param,
	IN const char* file_name_left, 
	IN const char* file_name_right);

#endif