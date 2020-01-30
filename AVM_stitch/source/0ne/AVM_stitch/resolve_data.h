#include<math.h>
#include "avm_interface.h"
#include "avm_load_parameter.h"
#include "generate_4_view_lut.h"


void resolve_data_from_lut_line(IN int shift, OUT int *rgb, IN  P_Avm_Param_S p_avm_param, UInt32_t *p_lut_line, UInt32_t *pic_coordinate);
void resolve_data_from_lut(IN int shift, IN IplImage *image, OUT int *rgb, IN  P_Avm_Param_S p_avm_param);

AVM_Error_Code_E resolve_4_view_data_from_LUT(IN P_Avm_Param_S p_avm_param,IN int small_big);



void _3_front_resolve_data_from_lut(IN int shift, IN IplImage *image, OUT int *rgb, IN  P_Avm_Param_S p_avm_param);

AVM_Error_Code_E  resolve_3_front_data_from_LUT(IN P_Avm_Param_S p_avm_param);



void _3_rear_resolve_data_from_lut(IN int shift, IN IplImage *image, OUT int *rgb, IN  P_Avm_Param_S p_avm_param);

AVM_Error_Code_E  resolve_3_rear_data_from_LUT(IN P_Avm_Param_S p_avm_param);

void reslove_single_view_from_lut(IN P_Avm_Param_S p_avm_param,IN int cam_flag);
void reslove_single_view_from_lut_0();

void front_single_resolve_data(IN int shift, IN IplImage *image, OUT int *rgb, IN  P_Avm_Param_S p_avm_param,UInt32_t* p_lut);

AVM_Error_Code_E my_load_image(IplImage* image[4]);



