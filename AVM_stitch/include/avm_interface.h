/*
 * This file is the AVM module's interface head file
 * Create by Shuo.Yan on 2017/11/23
 */
#ifndef __AVM_INTERFACE_H_
#define __AVM_INTERFACE_H_

#include "data_type.h"
#include "avm_error.h"
#include "camera_model.h"
#include "avm_config.h"
#include "avm_data_type.h"
#include "avm_utility.h"
#include "vehicle_info.h"
#include "image_process.h"
#include"SMC.h"
using namespace std;

INTESIGHT_API AVM_Error_Code_E AVM_Init(OUT Handle* p_avm_handle);
INTESIGHT_API AVM_Error_Code_E AVM_Process(OUT IN Handle avm_handle);
AVM_Error_Code_E my_test(IN  P_Avm_Param_S p_avm_param);
AVM_Error_Code_E S32V(IN  P_Avm_Param_S p_avm_param);
INTESIGHT_API void AVM_Deinit(IN Handle avm_handle);
INTESIGHT_API void AVM_Deinit_release(IN Handle avm_handle);
AVM_Error_Code_E AVM_Malloc_Lut_Memory(
	IN P_Avm_Config_Param_S p_avm_config_param,
	OUT P_Avm_Lut_S* pp_avm_lut);
AVM_Error_Code_E AVM_Malloc_Image_Memory(
	IN P_Avm_Config_Param_S p_avm_config_param,
	OUT P_Avm_Image_S* pp_avm_image);

AVM_Error_Code_E Smc_to_p_avm_config_param(
	OUT IN P_Avm_Param_S p_avm_param, IN  Smc_Cal_S Smc);
static AVM_Error_Code_E AVM_Load_Config_Param(OUT IN P_Avm_Param_S p_avm_param,
	OUT IN P_Avm_Config_Param_S p_avm_config_param);
//void AVM_Release_Lut_Memory(OUT IN P_Avm_Lut_S p_avm_lut);
//static void AVM_Release_Dis_Line_Memory(OUT IN P_Avm_Dis_Line_S p_avm_dis_line);
//static void AVM_Release_Image_Memory(OUT IN P_Avm_Image_S p_avm_image);
#endif