#ifndef  __FWRITE_BIN_H_
#define __FWRITE_BIN_H_
#include <math.h>
#include "avm_interface.h"
#include "avm_load_parameter.h"

AVM_Error_Code_E write_3_4_view_lut(IN P_Avm_Param_S p_avm_param);
AVM_Error_Code_E write_single_view_lut(IN P_Avm_Param_S p_avm_param, IN int cam_flag);
AVM_Error_Code_E write_line_lut(IN P_Avm_Param_S p_avm_param, IN int cam_flag);
AVM_Error_Code_E read_line_bin(IN  P_Avm_Param_S p_avm_param, IN int cam_flag);
AVM_Error_Code_E read_bin(IN  P_Avm_Param_S p_avm_param, IN int view_flag);

#endif