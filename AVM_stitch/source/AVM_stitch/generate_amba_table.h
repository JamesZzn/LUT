#ifndef  __GENERATE_AMBA_TABLE_H_
#define __GENERATE_AMBA_TABLE_H_
#include"generate_4_view_lut.h"
#include "single_view.h"


#define HEAD 0
uchar Amba_resolve_data_from_lut(IN int shift, IN IplImage *image, OUT int *rgb, 
	IN  P_Avm_Param_S p_avm_param, OUT Float32_t image_point0[2], UInt32_t*	p_lut);
void write_calib17_head();
void write_calib21(IN P_Avm_Param_S p_avm_param, FILE* pFile, int view_id, Int32_t Left_height,
	Int32_t Left_width, Int32_t Right_height, Int32_t Right_width
	, uchar *Amba_Lut_Left_weight, uchar *Amba_Lut_Right_weight);



void Calculate_4_view_Amba_Lut_Table(IN P_Avm_Param_S p_avm_param, IN Int32_t small_big);

AVM_Error_Code_E Calculate_single_view_Amba_Lut_Table(IN  P_Avm_Param_S p_avm_param, int cam_flag, int view_id);

AVM_Error_Code_E Calculate_3_view_Amba_Lut_Table_front(IN P_Avm_Param_S p_avm_param, int view_id);

AVM_Error_Code_E Calculate_3_view_Amba_Lut_Table_rear(IN P_Avm_Param_S p_avm_param, int  view_id);

AVM_Error_Code_E Calculate_4_view_Aemba_Lut_Table(IN P_Avm_Param_S p_avm_param, IN Int32_t small_big,  IN int view_id);

void generate_calib17_header();
AVM_Error_Code_E Tiguan_CVBS_out_amba_table(IN P_Avm_Param_S p_avm_param);
AVM_Error_Code_E Tiguan_Amba_HDMI_Table(P_Avm_Param_S p_avm_param);
AVM_Error_Code_E Tiguan_Amba_HDMI_Table_2_5(P_Avm_Param_S p_avm_param);

AVM_Error_Code_E load_HDMI_para(IN P_Avm_Param_S p_avm_param);
AVM_Error_Code_E load_HDMI_ful_screen_para(P_Avm_Param_S p_avm_param);
AVM_Error_Code_E load_CVBS_para(P_Avm_Param_S p_avm_param);
void load_big_screen_para(P_Avm_Param_S p_avm_param);

#endif
