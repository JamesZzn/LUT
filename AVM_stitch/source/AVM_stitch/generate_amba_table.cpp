#include"generate_amba_table.h"
#include"avm_error.h"
#include"resolve_data.h"
#include"avm_interface.h"
#include"generate_4_view_lut.h"
#include"generate_3_view_lut.h"
#include"handle_picture_major_process.h"
#include"fwrite_bin.h"
#include"SMC.h"
#define	Amba_Vector_Height		960		//安霸相机 的 原图 高和宽 
#define Amba_Vector_Width		1280   //安霸相机 的 原图 高和宽 
Int16_t Amba_front[61 * 81 * 2], Amba_back[61 * 81 * 2], Amba_left[61 * 81 * 2],
Amba_right[61 * 81 * 2];
/**************handle_static_dynamic_line***********************/
/*
* Function Name:				Tiguan_Amba_HDMI_Table
* Function Function:		Tiguan_Amba_HDMI_Table
*       p_avm_param;
* Note:
*     the interface function should be called at the beginning of this module
* Revision:
*     Create by Cheng.Li on 2018/5/2.
*/
AVM_Error_Code_E Tiguan_Amba_HDMI_Table(P_Avm_Param_S p_avm_param)
{
	
	AVM_Error_Code_E error = ERROR_NONE;
#if HEAD
	char *size_17=(char *)malloc(1894912);
	char *size_21=(char *)malloc(1382912 * 37);
	FILE* pFile = fopen("../../lut_table/Amba/HDMI/calib17.bin", "wb");
	fwrite(size_17, sizeof(char), 1894912, pFile);
	fclose(pFile);

	pFile = fopen("../../lut_table/Amba/HDMI/calib21.bin", "wb");
	fwrite(size_21, sizeof(char), 1382912 * 37, pFile);
	fclose(pFile);
	free(size_17);
	free(size_21);
#endif
	
	error = handle_3_4_view(p_avm_param);
	CHECK_ERROR(error);
	error = handle_front_rear_single_view(p_avm_param);
	CHECK_ERROR(error);
	error = handle_left_right_single_view(p_avm_param);
	CHECK_ERROR(error);
	
	///*从表中解析数据存储结果上位机验证*/
	error = resolve_4_view_data_from_LUT(p_avm_param, _4_VIEW_BIG);
	CHECK_ERROR(error);
	error = resolve_4_view_data_from_LUT(p_avm_param, _4_VIEW_SMALL);
	CHECK_ERROR(error);


	error = resolve_3_front_data_from_LUT(p_avm_param, _3_VIEW_FRONT);
	CHECK_ERROR(error);
	error = resolve_3_front_data_from_LUT(p_avm_param, _3_VIEW_REAR);
	CHECK_ERROR(error);

	//reslove_column_single_view_from_lut(p_avm_param, _1_VIEW_REAR);
	//reslove_column_single_view_from_lut_front(p_avm_param, _1_VIEW_FRONT);


	error = reslove_single_view_from_lut(p_avm_param, _1_VIEW_FRONT);
	CHECK_ERROR(error);
	error = reslove_single_view_from_lut(p_avm_param, _1_VIEW_REAR);
	CHECK_ERROR(error);
	CHECK_ERROR(error);
	error = reslove_single_view_from_lut(p_avm_param, _1_VIEW_FRONT);
	CHECK_ERROR(error);
	error = reslove_single_view_from_lut(p_avm_param, _1_VIEW_REAR);
	CHECK_ERROR(error);
	error = reslove_single_view_from_lut(p_avm_param, _1_VIEW_LEFT);
	CHECK_ERROR(error);
	error = reslove_single_view_from_lut(p_avm_param, _1_VIEW_RIGHT);
	CHECK_ERROR(error);
	error = reslove_single_view_from_lut(p_avm_param, _1_VIEW_LEFT_RIGHT);
	CHECK_ERROR(error);
#if 1
	/***********HDMI画线函数*********************/
	error = handle_static_dynamic_line(p_avm_param);
	CHECK_ERROR(error);
#endif
#if 1
#if 1
	///***小图的转表*****/
	error = Calculate_4_view_Aemba_Lut_Table(p_avm_param, _4_VIEW_SMALL, 0);//小图	
	CHECK_ERROR(error);
	error = Calculate_4_view_Aemba_Lut_Table(p_avm_param, _4_VIEW_BIG, 12);//大图 
	CHECK_ERROR(error);
	error = Calculate_4_view_Aemba_Lut_Table(p_avm_param, _3_VIEW_FRONT, 17);//小图	
	CHECK_ERROR(error);
	error = Calculate_4_view_Aemba_Lut_Table(p_avm_param, _3_VIEW_REAR, 18);//大图 
	CHECK_ERROR(error);
#endif
#if 1
	error = Calculate_single_view_Amba_Lut_Table(p_avm_param, _1_VIEW_FRONT, 6);//前单视图转表
	CHECK_ERROR(error);
	error = Calculate_single_view_Amba_Lut_Table(p_avm_param, _1_VIEW_REAR, 8);//后单视图转表
	CHECK_ERROR(error);
	error = Calculate_single_view_Amba_Lut_Table(p_avm_param, _1_VIEW_LEFT, 10);//左单视图转表
	CHECK_ERROR(error);
	error = Calculate_single_view_Amba_Lut_Table(p_avm_param, _1_VIEW_RIGHT, 11);//右单视图转表
	CHECK_ERROR(error);
	error = Calculate_single_view_Amba_Lut_Table(p_avm_param, _1_VIEW_LEFT_RIGHT, 16);//左右拼接单视图转表
	CHECK_ERROR(error);
#endif
#if 1
	error = load_HDMI_ful_screen_para(p_avm_param);
	CHECK_ERROR(error);
	/****BOX展开*****/
	p_avm_param->p_avm_config_param
		->avm_single_view_param.front_single_view_param.expand_type = TYPE_BOX_EXPAND;
	p_avm_param->p_avm_config_param
		->avm_single_view_param.rear_single_view_param.expand_type = TYPE_BOX_EXPAND;
	error = handle_front_rear_single_view(p_avm_param);
	CHECK_ERROR(error);

	error = reslove_single_view_from_lut(p_avm_param, _1_VIEW_FRONT);
	CHECK_ERROR(error);
	error = reslove_single_view_from_lut(p_avm_param, _1_VIEW_REAR);
	CHECK_ERROR(error);
	error = Calculate_single_view_Amba_Lut_Table(p_avm_param, _1_VIEW_FRONT, 2);//全屏前单视图box转表
	CHECK_ERROR(error);
	error = Calculate_single_view_Amba_Lut_Table(p_avm_param, _1_VIEW_REAR, 3);//全屏后单视图box转表
	CHECK_ERROR(error);
#endif
#endif
	return error;
}



/**************handle_static_dynamic_line***********************/
/*
* Function Name:				Tiguan_Amba_HDMI_Table
* Function Function:		Tiguan_Amba_HDMI_Table
*       p_avm_param;
* Note:
*     the interface function should be called at the beginning of this module
* Revision:
*     Create by Cheng.Li on 2018/5/2.
*/
AVM_Error_Code_E Tiguan_Amba_HDMI_Table_2_5(P_Avm_Param_S p_avm_param)
{
	AVM_Error_Code_E error = ERROR_NONE;
#if HEAD
	char *size_17 = (char *)malloc(1894912);
	char *size_21 = (char *)malloc(1382912 * 37);
	FILE* pFile = fopen("../../lut_table/Amba/HDMI/calib17.bin", "wb");
	fwrite(size_17, sizeof(char), 1894912, pFile);
	fclose(pFile);

	pFile = fopen("../../lut_table/Amba/HDMI/calib21.bin", "wb");
	fwrite(size_21, sizeof(char), 1382912 * 37, pFile);
	fclose(pFile);
	free(size_17);
	free(size_21);
#endif

	error = handle_3_4_view(p_avm_param);
	CHECK_ERROR(error);
	error = handle_front_rear_single_view(p_avm_param);
	CHECK_ERROR(error);
	error = handle_left_right_single_view(p_avm_param);
	CHECK_ERROR(error);
	printf("1.sucess\n");
	///*从表中解析数据存储结果上位机验证*/
	error = resolve_4_view_data_from_LUT(p_avm_param, _4_VIEW_BIG);
	CHECK_ERROR(error);
	error = resolve_4_view_data_from_LUT(p_avm_param, _4_VIEW_SMALL);
	CHECK_ERROR(error);
	error = reslove_single_view_from_lut(p_avm_param, _1_VIEW_FRONT);
	CHECK_ERROR(error);
	error = reslove_single_view_from_lut(p_avm_param, _1_VIEW_REAR);
	CHECK_ERROR(error);
	error = reslove_single_view_from_lut(p_avm_param, _1_VIEW_LEFT);
	CHECK_ERROR(error);
	error = reslove_single_view_from_lut(p_avm_param, _1_VIEW_RIGHT);
	CHECK_ERROR(error);

	printf("2.sucess\n");
	/***********HDMI画线函数*********************/
	error = handle_static_dynamic_line(p_avm_param);
	CHECK_ERROR(error);
	printf("3.line sucess\n");

	///***小图的转表*****/
	
	error = Calculate_single_view_Amba_Lut_Table(p_avm_param, _1_VIEW_FRONT, 6);//前单视图转表
	CHECK_ERROR(error);
	error = Calculate_single_view_Amba_Lut_Table(p_avm_param, _1_VIEW_REAR, 8);//后单视图转表
	CHECK_ERROR(error);
	error = Calculate_single_view_Amba_Lut_Table(p_avm_param, _1_VIEW_LEFT, 10);//左单视图转表
	CHECK_ERROR(error);
	error = Calculate_single_view_Amba_Lut_Table(p_avm_param, _1_VIEW_RIGHT, 11);//右单视图转表
	CHECK_ERROR(error);
	
	printf("4.amba_line sucess\n");
	return error;
}


/*
* Function Name:				Tiguan_CVBS_out_amba_table
* Function Function:			load config param for AVM module
* Input:
*     avm_handle:				the memory need to be freed
* Note:
*     the interface function should be called after the module finish its function
* Revision:
*     Create by Shuo.Yan on 2017/11/23.
*/
AVM_Error_Code_E  Tiguan_CVBS_out_amba_table(IN P_Avm_Param_S p_avm_param)
{
	
	AVM_Error_Code_E error = ERROR_NONE;
#if 1
	error = handle_3_4_view(p_avm_param);
	CHECK_ERROR(error);
	error = handle_front_rear_single_view(p_avm_param);
	CHECK_ERROR(error);
	error = handle_left_right_single_view(p_avm_param);
	CHECK_ERROR(error);


	/*从表中解析数据存储结果上位机验证*/
	error = resolve_4_view_data_from_LUT(p_avm_param, _4_VIEW_BIG);
	CHECK_ERROR(error);
	error = resolve_4_view_data_from_LUT(p_avm_param, _4_VIEW_SMALL);
	CHECK_ERROR(error);
	error = reslove_single_view_from_lut(p_avm_param, _1_VIEW_FRONT);
	CHECK_ERROR(error);
	error = reslove_single_view_from_lut(p_avm_param, _1_VIEW_REAR);
	CHECK_ERROR(error);
	
	error = reslove_single_view_from_lut(p_avm_param, _1_VIEW_FRONT);
	CHECK_ERROR(error);
	error = reslove_single_view_from_lut(p_avm_param, _1_VIEW_REAR);
	CHECK_ERROR(error);

	error = reslove_single_view_from_lut(p_avm_param, _1_VIEW_LEFT);
	CHECK_ERROR(error);
	error = reslove_single_view_from_lut(p_avm_param, _1_VIEW_RIGHT);
	CHECK_ERROR(error);
	error = reslove_single_view_from_lut(p_avm_param, _1_VIEW_LEFT_RIGHT);
	CHECK_ERROR(error);
	///*************************CVBS的小屏画线生成****************************/
	error = handle_static_dynamic_line(p_avm_param);//小屏的画线函数
	CHECK_ERROR(error);
	/******验证保存距离线小屏***/
	error = reslove_line_lut(p_avm_param, _1_VIEW_FRONT);
	CHECK_ERROR(error);
	error = reslove_line_lut(p_avm_param, _1_VIEW_REAR);
	CHECK_ERROR(error);



	///**********************************8安霸转表程序*****/
	error = Calculate_4_view_Aemba_Lut_Table(p_avm_param, _4_VIEW_BIG, 12);//大图 
	CHECK_ERROR(error);
	error = Calculate_4_view_Aemba_Lut_Table(p_avm_param, _4_VIEW_SMALL, 0);//小图
	CHECK_ERROR(error);
	
	
	error = Calculate_4_view_Aemba_Lut_Table(p_avm_param, _3_VIEW_FRONT, 17);//前三路
	CHECK_ERROR(error);
	error = Calculate_4_view_Aemba_Lut_Table(p_avm_param, _3_VIEW_REAR, 18);//后三路
	CHECK_ERROR(error);
	
	error = Calculate_single_view_Amba_Lut_Table(p_avm_param, _1_VIEW_FRONT, 6);//前单视图转表
	CHECK_ERROR(error);
	error = Calculate_single_view_Amba_Lut_Table(p_avm_param, _1_VIEW_REAR, 8);//后单视图转表
	CHECK_ERROR(error);
	error = Calculate_single_view_Amba_Lut_Table(p_avm_param, _1_VIEW_LEFT, 10);//左单视图转表
	CHECK_ERROR(error);
	error = Calculate_single_view_Amba_Lut_Table(p_avm_param, _1_VIEW_RIGHT, 11);//右单视图转表
	CHECK_ERROR(error);
	error = Calculate_single_view_Amba_Lut_Table(p_avm_param, _1_VIEW_LEFT_RIGHT, 16);//左右拼接单视图转表
	CHECK_ERROR(error);

	/****BOX展开*****///因为安霸平台的既有BOX又有平面展开所以手动修改前视图类型
	p_avm_param->p_avm_config_param
		->avm_single_view_param.front_single_view_param.expand_type = TYPE_BOX_EXPAND;
	p_avm_param->p_avm_config_param
		->avm_single_view_param.rear_single_view_param.expand_type = TYPE_BOX_EXPAND;
	error = handle_front_rear_single_view(p_avm_param);   //
	CHECK_ERROR(error);
	error = reslove_single_view_from_lut(p_avm_param, _1_VIEW_FRONT);
	CHECK_ERROR(error);
	error = reslove_single_view_from_lut(p_avm_param, _1_VIEW_REAR);
	CHECK_ERROR(error);
	error = Calculate_single_view_Amba_Lut_Table(p_avm_param, _1_VIEW_FRONT, 7);//小屏前单视图box转表
	CHECK_ERROR(error);//
	error = Calculate_single_view_Amba_Lut_Table(p_avm_param, _1_VIEW_REAR, 9);//小屏后单视图box转表
	CHECK_ERROR(error);





	///*****************写二进制文件*********************/
	error = write_3_4_view_lut(p_avm_param);
	CHECK_ERROR(error);
	error = write_single_view_lut(p_avm_param, _1_VIEW_FRONT);
	CHECK_ERROR(error);
	error = write_single_view_lut(p_avm_param, _1_VIEW_REAR);
	CHECK_ERROR(error);
	error = write_single_view_lut(p_avm_param, _1_VIEW_LEFT);
	CHECK_ERROR(error);
	error = write_single_view_lut(p_avm_param, _1_VIEW_RIGHT);
	CHECK_ERROR(error);
	error = write_single_view_lut(p_avm_param, _1_VIEW_LEFT_RIGHT);
	CHECK_ERROR(error);
	error = write_line_lut(p_avm_param, _1_VIEW_FRONT);
	CHECK_ERROR(error);
	error = write_line_lut(p_avm_param, _1_VIEW_REAR);
	CHECK_ERROR(error);

#endif


	/***********读BIN文件 ************/
	error = read_bin(p_avm_param, _4_VIEW_BIG);
	CHECK_ERROR(error);
	error = read_bin(p_avm_param, _4_VIEW_SMALL);
	CHECK_ERROR(error);
	error = read_bin(p_avm_param, _3_VIEW_FRONT);
	CHECK_ERROR(error);
	error = read_bin(p_avm_param, _3_VIEW_REAR);
	CHECK_ERROR(error);
	error = read_bin(p_avm_param, _1_VIEW_FRONT);
	CHECK_ERROR(error);
	error = read_bin(p_avm_param, _1_VIEW_REAR);
	CHECK_ERROR(error);
	error = read_bin(p_avm_param, _1_VIEW_LEFT);
	CHECK_ERROR(error);
	error = read_bin(p_avm_param, _1_VIEW_RIGHT);
	CHECK_ERROR(error);
	error = read_bin(p_avm_param, _1_VIEW_LEFT_RIGHT);
	CHECK_ERROR(error);
	error = read_line_bin(p_avm_param, _1_VIEW_FRONT);
	CHECK_ERROR(error);
	error = read_line_bin(p_avm_param, _1_VIEW_REAR);
	CHECK_ERROR(error);



	/******验证保存距离线小屏***/
	error = reslove_line_lut(p_avm_param, _1_VIEW_FRONT);
	CHECK_ERROR(error);
	error = reslove_line_lut(p_avm_param, _1_VIEW_REAR);
	CHECK_ERROR(error);









#if 1
	AVM_Deinit_release(p_avm_param);//因为大屏小屏只有查找表和结果图大小不同释放内存
	
	
	////此处为了支持Tiguan项目临时加了大屏幕的配置，为了程序不在重复配置，将小屏与全屏的配置分开，全屏配置是一个独立的TXT文件
	load_big_screen_para(p_avm_param);
	CHECK_ERROR(error);
	/*计算生成裕兰的表*/
	error = handle_3_4_view(p_avm_param);
	CHECK_ERROR(error);
	error = handle_front_rear_single_view(p_avm_param);
	CHECK_ERROR(error);
	error = handle_left_right_single_view(p_avm_param);
	CHECK_ERROR(error);


	/*从表中解析数据存储结果上位机验证*/
	error = reslove_single_view_from_lut(p_avm_param, _1_VIEW_FRONT);
	CHECK_ERROR(error);
	error = reslove_single_view_from_lut(p_avm_param, _1_VIEW_REAR);
	CHECK_ERROR(error);
	error = reslove_single_view_from_lut(p_avm_param, _1_VIEW_FRONT);
	CHECK_ERROR(error);
	error = reslove_single_view_from_lut(p_avm_param, _1_VIEW_REAR);
	CHECK_ERROR(error);
	error = reslove_single_view_from_lut(p_avm_param, _1_VIEW_LEFT);
	CHECK_ERROR(error);
	error = reslove_single_view_from_lut(p_avm_param, _1_VIEW_RIGHT);
	CHECK_ERROR(error);
	error = reslove_single_view_from_lut(p_avm_param, _1_VIEW_LEFT_RIGHT);
	CHECK_ERROR(error);

	/******验证保存距离线小屏***/
	///*************************CVBS的大屏画线生成****************************/
	error = handle_static_dynamic_line(p_avm_param);//小屏的画线函数
	CHECK_ERROR(error);
	error = reslove_line_lut(p_avm_param, _1_VIEW_FRONT);
	CHECK_ERROR(error);
	error = reslove_line_lut(p_avm_param, _1_VIEW_REAR);
	CHECK_ERROR(error);


	error = Calculate_4_view_Aemba_Lut_Table(p_avm_param, _3_VIEW_FRONT, 14);//前三路
	CHECK_ERROR(error);
	error = Calculate_4_view_Aemba_Lut_Table(p_avm_param, _3_VIEW_REAR, 15);//后三路
	CHECK_ERROR(error);

	error = Calculate_single_view_Amba_Lut_Table(p_avm_param, _1_VIEW_FRONT, 19);//全屏前单视图转表
	CHECK_ERROR(error);
	error = Calculate_single_view_Amba_Lut_Table(p_avm_param, _1_VIEW_REAR, 1);//全屏后单视图转表
	CHECK_ERROR(error);
	error = Calculate_single_view_Amba_Lut_Table(p_avm_param, _1_VIEW_LEFT, 4);//全屏左单视图转表
	CHECK_ERROR(error);
	error = Calculate_single_view_Amba_Lut_Table(p_avm_param, _1_VIEW_RIGHT, 5);//全屏右单视图转表
	CHECK_ERROR(error);
	error = Calculate_single_view_Amba_Lut_Table(p_avm_param, _1_VIEW_LEFT_RIGHT, 13);//全屏左右拼接单视图转表

	////为了前后的BOX展开再一次计算一次前后展开
	/****BOX展开*****/
	p_avm_param->p_avm_config_param
		->avm_single_view_param.front_single_view_param.expand_type = TYPE_BOX_EXPAND;
	p_avm_param->p_avm_config_param
		->avm_single_view_param.rear_single_view_param.expand_type = TYPE_BOX_EXPAND;
	error = handle_front_rear_single_view(p_avm_param);
	CHECK_ERROR(error);
	error = reslove_single_view_from_lut(p_avm_param, _1_VIEW_FRONT);
	error = reslove_single_view_from_lut(p_avm_param, _1_VIEW_REAR);
	error = Calculate_single_view_Amba_Lut_Table(p_avm_param, _1_VIEW_FRONT, 2);//全屏前单视图box转表
	CHECK_ERROR(error);
	error = Calculate_single_view_Amba_Lut_Table(p_avm_param, _1_VIEW_REAR, 3);//全屏后单视图box转表
	CHECK_ERROR(error);

#endif
	return error;
}

/**************load_ful_screen_para***********************/
/*
* Function Name:				load_ful_screen_para
* Function Function:		load_ful_screen_para
*       p_avm_param;
* Note:
*   load_ful_screen_para
* Revision:
*     Create by Cheng.Li on 2018/5/2.
*/
void load_big_screen_para(P_Avm_Param_S p_avm_param)
{
#if USE_SMC
	Smc_Cal_S Smc;
	char SMC_path_name[MAX_ARRAY_SIZE];
	sprintf(SMC_path_name, "%s/SMC/big.bin", config_param_path);

	FILE* pf = fopen(SMC_path_name, "rb+");
	fread(&Smc, 1, sizeof(Smc_Cal_S), pf);
	Smc_to_p_avm_config_param(p_avm_param, Smc);
	 AVM_Malloc_Lut_Memory(
		p_avm_param->p_avm_config_param, &p_avm_param->p_avm_lut);
	 AVM_Malloc_Image_Memory(
		 p_avm_param->p_avm_config_param, &p_avm_param->p_avm_image);
#else
	
	P_Avm_Config_Param_S p_avm_config_param = p_avm_param->p_avm_config_param;
	// load junc 3-pass front view config parameters
	char avm_junc_3_front_param_name[MAX_ARRAY_SIZE];
	sprintf(avm_junc_3_front_param_name, "%s/avm_2d_junction_3_front_param_ful_screen.txt",
		config_param_path);
	int ret = Load_Junc_View_3_Front_Param(
		&(p_avm_config_param->avm_2d_param.junc_view_3_front_param),
		avm_junc_3_front_param_name, &p_avm_config_param->vehicle_param);
	
	// load junc 3-pass rear view config parameters
	char avm_junc_3_rear_param_name[MAX_ARRAY_SIZE];
	sprintf(avm_junc_3_rear_param_name, "%s/avm_2d_junction_3_rear_param_ful_screen.txt",
		config_param_path);
	ret = Load_Junc_View_3_Rear_Param(
		&p_avm_config_param->avm_2d_param.junc_view_3_rear_param,
		avm_junc_3_rear_param_name, &p_avm_config_param->vehicle_param);
	// load front view config parameters
	char front_single_view_param_name[MAX_ARRAY_SIZE];
	sprintf(front_single_view_param_name, "%s/avm_single_view_front_ful_screen.txt",
		config_param_path);
	ret = Load_Front_Rear_Single_View_Param(
		&p_avm_config_param->avm_single_view_param.front_single_view_param,
		front_single_view_param_name);
	

	// load rear view config parameters
	char rear_single_view_param_name[MAX_ARRAY_SIZE];
	sprintf(rear_single_view_param_name, "%s/avm_single_view_rear_ful_screen.txt",
		config_param_path);
	ret = Load_Front_Rear_Single_View_Param(
		&p_avm_config_param->avm_single_view_param.rear_single_view_param,
		rear_single_view_param_name);
	

	// load left view config parameters
	char left_single_view_param_name[MAX_ARRAY_SIZE];
	sprintf(left_single_view_param_name, "%s/avm_single_view_left_ful_screen.txt",
		config_param_path);
	ret = Load_Left_Right_Single_View_Param(
		&p_avm_config_param->avm_single_view_param.left_single_view_param,
		left_single_view_param_name);
	

	// load right view config parameters
	char right_single_view_param_name[MAX_ARRAY_SIZE];
	sprintf(right_single_view_param_name, "%s/avm_single_view_right_ful_screen.txt",
		config_param_path);
	ret = Load_Left_Right_Single_View_Param(
		&p_avm_config_param->avm_single_view_param.right_single_view_param,
		right_single_view_param_name);
	
	char left_right_single_view_param_name[MAX_ARRAY_SIZE];
	sprintf(left_right_single_view_param_name, "%s/avm_single_view_left_right_ful_screen.txt",
		config_param_path);
	// load left and right together view config parameters
	ret = Load_Left_Right_Together_View_Param(
		&p_avm_config_param->avm_single_view_param.left_right_view_param,
		left_right_single_view_param_name, right_single_view_param_name);

	ret = AVM_Malloc_Lut_Memory(
		p_avm_param->p_avm_config_param, &p_avm_param->p_avm_lut);
	AVM_Malloc_Image_Memory(
		p_avm_param->p_avm_config_param, &p_avm_param->p_avm_image);
#endif
	
}

/**************load_HDMI_ful_screen_para***********************/
/*
* Function Name:			load_HDMI_ful_screen_para
* Function Function:		load_HDMI_ful_screen_para
*       p_avm_param;
* Note:
*   load_ful_screen_para
* Revision:
*     Create by Cheng.Li on 2018/5/2.
*/
AVM_Error_Code_E load_HDMI_ful_screen_para(P_Avm_Param_S p_avm_param)
{
	AVM_Error_Code_E ret=ERROR_NONE;
#if USE_SMC
	Smc_Cal_S Smc;
	char SMC_path_name[MAX_ARRAY_SIZE];
	sprintf(SMC_path_name, "%s/HDMI_config/HDMI_big_para.bin", config_param_path);

	FILE* pf = fopen(SMC_path_name, "rb+");
	fread(&Smc, 1, sizeof(Smc_Cal_S), pf);
	Smc_to_p_avm_config_param(p_avm_param, Smc);
#else



	P_Avm_Config_Param_S p_avm_config_param = p_avm_param->p_avm_config_param;
	// load junc 3-pass front view config parameters
	char avm_junc_3_front_param_name[MAX_ARRAY_SIZE];
	sprintf(avm_junc_3_front_param_name, "%s/avm_2d_junction_3_front_param_ful_screen.txt",
		config_param_path);
	ret = Load_Junc_View_3_Front_Param(
		&(p_avm_config_param->avm_2d_param.junc_view_3_front_param),
		avm_junc_3_front_param_name, &p_avm_config_param->vehicle_param);

	// load junc 3-pass rear view config parameters
	char avm_junc_3_rear_param_name[MAX_ARRAY_SIZE];
	sprintf(avm_junc_3_rear_param_name, "%s/avm_2d_junction_3_rear_param_ful_screen.txt",
		config_param_path);
	ret = Load_Junc_View_3_Rear_Param(
		&p_avm_config_param->avm_2d_param.junc_view_3_rear_param,
		avm_junc_3_rear_param_name, &p_avm_config_param->vehicle_param);
	// load front view config parameters
	char front_single_view_param_name[MAX_ARRAY_SIZE];
	sprintf(front_single_view_param_name, "%s/avm_single_view_front_ful_screen.txt",
		config_param_path);
	ret = Load_Front_Rear_Single_View_Param(
		&p_avm_config_param->avm_single_view_param.front_single_view_param,
		front_single_view_param_name);
	// load rear view config parameters
	char rear_single_view_param_name[MAX_ARRAY_SIZE];
	sprintf(rear_single_view_param_name, "%s/avm_single_view_rear_ful_screen.txt",
		config_param_path);
	ret = Load_Front_Rear_Single_View_Param(
		&p_avm_config_param->avm_single_view_param.rear_single_view_param,
		rear_single_view_param_name);


	// load left view config parameters
	char left_single_view_param_name[MAX_ARRAY_SIZE];
	sprintf(left_single_view_param_name, "%s/avm_single_view_left_ful_screen.txt",
		config_param_path);
	ret = Load_Left_Right_Single_View_Param(
		&p_avm_config_param->avm_single_view_param.left_single_view_param,
		left_single_view_param_name);


	// load right view config parameters
	char right_single_view_param_name[MAX_ARRAY_SIZE];
	sprintf(right_single_view_param_name, "%s/avm_single_view_right_ful_screen.txt",
		config_param_path);
	ret = Load_Left_Right_Single_View_Param(
		&p_avm_config_param->avm_single_view_param.right_single_view_param,
		right_single_view_param_name);

	char left_right_single_view_param_name[MAX_ARRAY_SIZE];
	sprintf(left_right_single_view_param_name, "%s/avm_single_view_left_right_ful_screen.txt",
		config_param_path);
	// load left and right together view config parameters
	ret = Load_Left_Right_Together_View_Param(
		&p_avm_config_param->avm_single_view_param.left_right_view_param,
		left_right_single_view_param_name, right_single_view_param_name);

	ret = AVM_Malloc_Lut_Memory(
		p_avm_param->p_avm_config_param, &p_avm_param->p_avm_lut);
#endif
	return ret;
}
/**************load_HDMI_para***********************/
/*
* Function Name:			load_HDMI_para
* Function Function:		load_HDMI_para
*       p_avm_param;
* Note:
*   load_ful_screen_para
* Revision:
*     Create by Cheng.Li on 2018/5/2.
*/
AVM_Error_Code_E  load_HDMI_para(IN P_Avm_Param_S p_avm_param)
{
	AVM_Error_Code_E ret=ERROR_NONE;

#if USE_SMC
	Smc_Cal_S Smc;
	char SMC_path_name[MAX_ARRAY_SIZE];
	sprintf(SMC_path_name, "%s/HDMI_config/HDMI_small_para.bin", config_param_path);

	FILE* pf = fopen(SMC_path_name, "rb+");
	fread(&Smc, 1, sizeof(Smc_Cal_S), pf);
	Smc_to_p_avm_config_param(p_avm_param, Smc);

#else
	P_Avm_Config_Param_S p_avm_config_param = p_avm_param->p_avm_config_param;
	char vehicle_param_name[MAX_ARRAY_SIZE];
	sprintf(vehicle_param_name, "%s/HDMI_config/vehicle_info.txt", config_param_path);
	ret = (AVM_Error_Code_E)Get_Vehicle_Info(
		&p_avm_config_param->vehicle_param, vehicle_param_name);
	CHECK_ERROR(ret);

	// load intrinsic and extrinsic parameters
	for (Int32_t cam_id = 0; cam_id < CAMERA_NUM; cam_id++)
	{
		// load camera intrinsic parameters
		char intrinsic_param_name[MAX_ARRAY_SIZE];
		sprintf(intrinsic_param_name, "%s/HDMI_config/ocam_intrinsic_%s.txt",
			config_param_path, camera_prefix_array[cam_id]);
		ret = (AVM_Error_Code_E)Load_Camera_Model_Int(
			&(p_avm_config_param->camera_model[cam_id].camera_model_int),
			intrinsic_param_name);
		CHECK_ERROR(ret);
		// load camera extrinsic parameters
		char extrinsic_param_name[MAX_ARRAY_SIZE];
		sprintf(extrinsic_param_name, "%s/HDMI_config/ocam_extrinsic_%s.txt",
			config_param_path, camera_prefix_array[cam_id]);
		ret = (AVM_Error_Code_E)Load_Camera_Model_Ext(
			&p_avm_config_param->camera_model[cam_id].camera_model_ext,
			extrinsic_param_name);
		CHECK_ERROR(ret);

		Sync_Camera_Model_Ext(&(p_avm_config_param->camera_model[cam_id].camera_model_ext));
	}

	// load junc 4-pass view config parameters, 
	// both small and big view are loaded in this function
	char avm_junc_4_param_name[MAX_ARRAY_SIZE];
	sprintf(avm_junc_4_param_name, "%s/HDMI_config/avm_2d_junction_4_param.txt",
		config_param_path);
	 ret = Load_Junc_View_4_Param(
		&p_avm_config_param->avm_2d_param.junc_view_4_param, 
		avm_junc_4_param_name, &p_avm_config_param->vehicle_param);
	CHECK_ERROR(ret);
	// load junc 3-pass front view config parameters
	char avm_junc_3_front_param_name[MAX_ARRAY_SIZE];
	sprintf(avm_junc_3_front_param_name, "%s/HDMI_config/avm_2d_junction_3_front_param.txt",
		config_param_path);
	ret = Load_Junc_View_3_Front_Param(
		&(p_avm_config_param->avm_2d_param.junc_view_3_front_param),
		avm_junc_3_front_param_name, &p_avm_config_param->vehicle_param);
	CHECK_ERROR(ret);
	// load junc 3-pass rear view config parameters
	char avm_junc_3_rear_param_name[MAX_ARRAY_SIZE];
	sprintf(avm_junc_3_rear_param_name, "%s/HDMI_config/avm_2d_junction_3_rear_param.txt",
		config_param_path);
	ret = Load_Junc_View_3_Rear_Param(
		&p_avm_config_param->avm_2d_param.junc_view_3_rear_param,
		avm_junc_3_rear_param_name, &p_avm_config_param->vehicle_param);
	CHECK_ERROR(ret);
	// load awb config parameters
	char awb_param_name[MAX_ARRAY_SIZE];
	sprintf(awb_param_name, "%s/HDMI_config/avm_2d_awb_param.txt", config_param_path);
	ret = Load_Awb_View_Param(&p_avm_config_param->avm_2d_param.awb_view_param,
		awb_param_name, &p_avm_config_param->vehicle_param);
	CHECK_ERROR(ret);

	// load front view config parameters
	char front_single_view_param_name[MAX_ARRAY_SIZE];
	sprintf(front_single_view_param_name, "%s/HDMI_config/avm_single_view_front.txt",
		config_param_path);
	ret = Load_Front_Rear_Single_View_Param(
		&p_avm_config_param->avm_single_view_param.front_single_view_param,
		front_single_view_param_name);
	CHECK_ERROR(ret);

	// load rear view config parameters
	char rear_single_view_param_name[MAX_ARRAY_SIZE];
	sprintf(rear_single_view_param_name, "%s/HDMI_config/avm_single_view_rear.txt",
		config_param_path);
	ret = Load_Front_Rear_Single_View_Param(
		&p_avm_config_param->avm_single_view_param.rear_single_view_param,
		rear_single_view_param_name);
	CHECK_ERROR(ret);

	// load left view config parameters
	char left_single_view_param_name[MAX_ARRAY_SIZE];
	sprintf(left_single_view_param_name, "%s/HDMI_config/avm_single_view_left.txt",
		config_param_path);
	ret = Load_Left_Right_Single_View_Param(
		&p_avm_config_param->avm_single_view_param.left_single_view_param,
		left_single_view_param_name);
	CHECK_ERROR(ret);

	// load right view config parameters
	char right_single_view_param_name[MAX_ARRAY_SIZE];
	sprintf(right_single_view_param_name, "%s/HDMI_config/avm_single_view_right.txt",
		config_param_path);
	ret = Load_Left_Right_Single_View_Param(
		&p_avm_config_param->avm_single_view_param.right_single_view_param,
		right_single_view_param_name);
	//CHECK_ERROR(ret);
	char left_right_single_view_param_name[MAX_ARRAY_SIZE];
	sprintf(left_right_single_view_param_name, "%s/HDMI_config/avm_single_view_left_right.txt",
		config_param_path);
	// load left and right together view config parameters
	ret = Load_Left_Right_Together_View_Param(
		&p_avm_config_param->avm_single_view_param.left_right_view_param,
		left_right_single_view_param_name, right_single_view_param_name);
	CHECK_ERROR(ret);

	//重新下载了参数要重新申请内存
	ret = AVM_Malloc_Lut_Memory(
		p_avm_param->p_avm_config_param, &p_avm_param->p_avm_lut);
	CHECK_ERROR(ret);
#endif
	return ret;
}


/**************load_CVBS_para***********************/
/*
* Function Name:			load_HDMI_para
* Function Function:		load_HDMI_para
*       p_avm_param;
* Note:
*   load_ful_screen_para
* Revision:
*     Create by Cheng.Li on 2018/5/2.
*/
AVM_Error_Code_E load_CVBS_para(P_Avm_Param_S p_avm_param)
{
	AVM_Error_Code_E ret = ERROR_NONE;

#if USE_SMC
		Smc_Cal_S Smc;
	char SMC_path_name[MAX_ARRAY_SIZE];
	sprintf(SMC_path_name, "%s/CVBS_config/small.bin", config_param_path);

	FILE* pf = fopen(SMC_path_name, "rb+");
	if (!pf)
	{
		printf("File %s cannot be opened\n", SMC_path_name);
		ret = ERROR_OPEN_FILE_FIAL;
		//CHECK_ERROR(ret`);
	}
	fread(&Smc, 1, sizeof(Smc_Cal_S), pf);
	Smc_to_p_avm_config_param(p_avm_param, Smc);
#else

	P_Avm_Config_Param_S p_avm_config_param = p_avm_param->p_avm_config_param;
	char vehicle_param_name[MAX_ARRAY_SIZE];
	sprintf(vehicle_param_name, "%s/CVBS_config/vehicle_info.txt", config_param_path);
	ret = (AVM_Error_Code_E)Get_Vehicle_Info(
		&p_avm_config_param->vehicle_param, vehicle_param_name);
	CHECK_ERROR(ret);

	// load intrinsic and extrinsic parameters
	for (Int32_t cam_id = 0; cam_id < CAMERA_NUM; cam_id++)
	{
		// load camera intrinsic parameters
		char intrinsic_param_name[MAX_ARRAY_SIZE];
		sprintf(intrinsic_param_name, "%s/CVBS_config/ocam_intrinsic_%s.txt",
			config_param_path, camera_prefix_array[cam_id]);
		ret = (AVM_Error_Code_E)Load_Camera_Model_Int(
			&(p_avm_config_param->camera_model[cam_id].camera_model_int),
			intrinsic_param_name);
		CHECK_ERROR(ret);
		// load camera extrinsic parameters
		char extrinsic_param_name[MAX_ARRAY_SIZE];
		sprintf(extrinsic_param_name, "%s/CVBS_config/ocam_extrinsic_%s.txt",
			config_param_path, camera_prefix_array[cam_id]);
		ret = (AVM_Error_Code_E)Load_Camera_Model_Ext(
			&p_avm_config_param->camera_model[cam_id].camera_model_ext,
			extrinsic_param_name);
		CHECK_ERROR(ret);

		Sync_Camera_Model_Ext(&(p_avm_config_param->camera_model[cam_id].camera_model_ext));
	}

	// load junc 4-pass view config parameters, 
	// both small and big view are loaded in this function
	char avm_junc_4_param_name[MAX_ARRAY_SIZE];
	sprintf(avm_junc_4_param_name, "%s/CVBS_config/avm_2d_junction_4_param.txt",
		config_param_path);
	 ret = Load_Junc_View_4_Param(
		&p_avm_config_param->avm_2d_param.junc_view_4_param,
		avm_junc_4_param_name, &p_avm_config_param->vehicle_param);
	CHECK_ERROR(ret);
	// load junc 3-pass front view config parameters
	char avm_junc_3_front_param_name[MAX_ARRAY_SIZE];
	sprintf(avm_junc_3_front_param_name, "%s/CVBS_config/avm_2d_junction_3_front_param.txt",
		config_param_path);
	ret = Load_Junc_View_3_Front_Param(
		&(p_avm_config_param->avm_2d_param.junc_view_3_front_param),
		avm_junc_3_front_param_name, &p_avm_config_param->vehicle_param);
	CHECK_ERROR(ret);
	// load junc 3-pass rear view config parameters
	char avm_junc_3_rear_param_name[MAX_ARRAY_SIZE];
	sprintf(avm_junc_3_rear_param_name, "%s/CVBS_config/avm_2d_junction_3_rear_param.txt",
		config_param_path);
	ret = Load_Junc_View_3_Rear_Param(
		&p_avm_config_param->avm_2d_param.junc_view_3_rear_param,
		avm_junc_3_rear_param_name, &p_avm_config_param->vehicle_param);
	//CHECK_ERROR(ret);
	// load awb config parameters
	char awb_param_name[MAX_ARRAY_SIZE];
	sprintf(awb_param_name, "%s/CVBS_config/avm_2d_awb_param.txt", config_param_path);
	ret = Load_Awb_View_Param(&p_avm_config_param->avm_2d_param.awb_view_param,
		awb_param_name, &p_avm_config_param->vehicle_param);
	CHECK_ERROR(ret);

	// load front view config parameters
	char front_single_view_param_name[MAX_ARRAY_SIZE];
	sprintf(front_single_view_param_name, "%s/CVBS_config/avm_single_view_front.txt",
		config_param_path);
	ret = Load_Front_Rear_Single_View_Param(
		&p_avm_config_param->avm_single_view_param.front_single_view_param,
		front_single_view_param_name);
	CHECK_ERROR(ret);

	// load rear view config parameters
	char rear_single_view_param_name[MAX_ARRAY_SIZE];
	sprintf(rear_single_view_param_name, "%s/CVBS_config/avm_single_view_rear.txt",
		config_param_path);
	ret = Load_Front_Rear_Single_View_Param(
		&p_avm_config_param->avm_single_view_param.rear_single_view_param,
		rear_single_view_param_name);
	CHECK_ERROR(ret);

	// load left view config parameters
	char left_single_view_param_name[MAX_ARRAY_SIZE];
	sprintf(left_single_view_param_name, "%s/CVBS_config/avm_single_view_left.txt",
		config_param_path);
	ret = Load_Left_Right_Single_View_Param(
		&p_avm_config_param->avm_single_view_param.left_single_view_param,
		left_single_view_param_name);
	CHECK_ERROR(ret);

	// load right view config parameters
	char right_single_view_param_name[MAX_ARRAY_SIZE];
	sprintf(right_single_view_param_name, "%s/CVBS_config/avm_single_view_right.txt",
		config_param_path);
	ret = Load_Left_Right_Single_View_Param(
		&p_avm_config_param->avm_single_view_param.right_single_view_param,
		right_single_view_param_name);
	CHECK_ERROR(ret);
	char left_right_single_view_param_name[MAX_ARRAY_SIZE];
	sprintf(left_right_single_view_param_name, "%s/CVBS_config/avm_single_view_left_right.txt",
		config_param_path);
	// load left and right together view config parameters
	ret = Load_Left_Right_Together_View_Param(
		&p_avm_config_param->avm_single_view_param.left_right_view_param,
		left_right_single_view_param_name, right_single_view_param_name);
	CHECK_ERROR(ret);

	//重新下载了参数要重新申请内存
	ret = AVM_Malloc_Lut_Memory(
		p_avm_param->p_avm_config_param, &p_avm_param->p_avm_lut);
	//CHECK_ERROR(ret);
#endif
	return ret;
}



/***************resolve 4 pass view data from LUT*******************/
/*
* Function Name:				resolve_data_from_lut
* Function Function:			resolve data from lut
* Input:
*       shift;                the position of data in LUT 
*		image;                source image
*       rgb;                  get value of pixel in source image
* Note:
*
* Revision:
*     Create by Cheng.Li on 2018/1/12.
*/
uchar Amba_resolve_data_from_lut(IN int shift, IN IplImage *image, OUT int *rgb, 
	IN  P_Avm_Param_S p_avm_param, OUT Float32_t image_point0[2], UInt32_t*p_lut)
{
#if 1
	//Float32_t image_point0[2];
	UInt32_t temp0 = p_lut[shift];
	UInt32_t temp1 = p_lut[shift + 1];
	Int32_t v_int = temp0 >> 21;// v_int = (int)image_point[1];第一位存储是图像的高坐标
	Int32_t  u_int = temp0 >> 10;
	u_int = u_int & 0x7FF;
	Float32_t weightUpLeft = (Float32_t)(temp1 >> 24) / 255.000,
		weightUpRight = ((Float32_t)((temp1 >> 16) & 255)) / 255.000,
		weightDownLeft = ((Float32_t)((temp1 >> 8) & 255)) / 255.000,
		weightDownRight = 1 - weightUpLeft - weightUpRight - weightDownLeft;
	bilinear_interpolation_new(image, rgb, u_int, v_int,
		weightUpLeft, weightUpRight, weightDownLeft, weightDownRight);

	UInt32_t weight_int = temp0 >> 2;
	weight_int = weight_int & 255;
	Float32_t weight_Float32_t = Float32_t(weight_int) / (Float32_t)255;

	rgb[0] = (uchar)(Float32_t(rgb[0])*weight_Float32_t);
	rgb[1] = (uchar)(Float32_t(rgb[1])*weight_Float32_t);
	rgb[2] = (uchar)(Float32_t(rgb[2])*weight_Float32_t);


	//image_point0[1] = (Float32_t)u_int + weightUpRight / (weightUpLeft + weightUpRight);
	//image_point0[0] = (Float32_t)v_int + weightDownLeft / (weightDownLeft + weightUpLeft);
	image_point0[1] = (Float32_t)u_int;
	image_point0[0] = (Float32_t)v_int;
	uchar weight_char = (uchar)weight_int;
	return weight_char;
#endif

#if 0
	unsigned int temp0 = p_lut[shift];
	unsigned int temp1 = p_lut[shift + 1];
	int u_int = temp0 >> 21;// v_int = (int)image_point[1];
	int v_int = temp0 >> 10;
	v_int = v_int & 0x7FF;
	Float32_t u_point = (Float32_t)(temp1 >> 24) / 255.000, v_point = 
		((Float32_t)((temp1 >> 16) & 255)) / 255.000;
	image_point0[0] = (Float32_t)u_int + u_point;
	image_point0[1] = (Float32_t)v_int + v_point;
	unsigned int weight_int = temp0 >> 2;
	weight_int = weight_int & 0x00FF;
	Float32_t weight_Float32_t = Float32_t(weight_int) / (Float32_t)255;
	bilinear_interpolation(image, rgb, image_point0[1], image_point0[0]);
	rgb[0] = (uchar)(Float32_t(rgb[0])*weight_Float32_t);
	uchar weight_char = (uchar)weight_int;
	return weight_char;


#endif
}







//权威 4路备份 
/*
* Function Name:				 Calculate_4_view_Aemba_Lut_Table
* Function Function:			 Calculate_4_view_Aemba_Lut_Table
* Input:
*     avm_handle:				the memory need to be freed
* Note:
*      Calculate_4_view_Aemba_Lut_Table
* Revision:
*     Create by Shuo.Yan on 2017/11/23.
*/


AVM_Error_Code_E Calculate_4_view_GPU_heng_Lut_Table(IN P_Avm_Param_S p_avm_param,
	IN Int32_t view_flag, IN int view_id)
{
	AVM_Error_Code_E error = ERROR_NONE;
	int result_image_height;
	int result_image_width;
	CvRect car_logo_rect;
	UInt32_t* p_lut = NULL;
	int camera_size[4];
	char lut_path[MAX_ARRAY_SIZE];;
	if (view_flag == _4_VIEW_BIG)//大图 
	{
		p_lut = p_avm_param->p_avm_lut->p_lut_junc_view_4_big;
		result_image_height = p_avm_param->p_avm_config_param
			->avm_2d_param.junc_view_4_param.result_big_param.result_image_height;
		result_image_width = p_avm_param->p_avm_config_param
			->avm_2d_param.junc_view_4_param.result_big_param.result_image_width;
		car_logo_rect = p_avm_param->p_avm_config_param
			->avm_2d_param.junc_view_4_param.result_big_param.car_logo_rect;
		/****malloc look up table****/
		for (Int32_t cam_id = 0; cam_id < CAMERA_NUM; cam_id++)
		{
			camera_size[cam_id] = p_avm_param->p_avm_config_param
				->avm_2d_param.junc_view_4_param.result_big_param.lut_size[cam_id] * 2;
		}
	}
	if (view_flag == _4_VIEW_SMALL)//小图
	{
		result_image_height = p_avm_param->p_avm_config_param
			->avm_2d_param.junc_view_4_param.result_small_param.result_image_height;
		result_image_width = p_avm_param->p_avm_config_param
			->avm_2d_param.junc_view_4_param.result_small_param.result_image_width;
		p_lut = p_avm_param->p_avm_lut->p_lut_junc_view_4_small;
		car_logo_rect = p_avm_param->p_avm_config_param
			->avm_2d_param.junc_view_4_param.result_small_param.car_logo_rect;
		/****malloc look up table****/
		for (Int32_t cam_id = 0; cam_id < CAMERA_NUM; cam_id++)
		{
			camera_size[cam_id] = p_avm_param->p_avm_config_param
				->avm_2d_param.junc_view_4_param.result_small_param.lut_size[cam_id] * 2;
		}
	}
	if (view_flag == _3_VIEW_FRONT)//小图
	{
		result_image_height = p_avm_param->p_avm_config_param
			->avm_2d_param.junc_view_3_front_param.result_3_front_param.result_image_height;
		result_image_width = p_avm_param->p_avm_config_param
			->avm_2d_param.junc_view_3_front_param.result_3_front_param.result_image_width;
		p_lut = p_avm_param->p_avm_lut->p_lut_junc_view_3_front;

		car_logo_rect = p_avm_param->p_avm_config_param
			->avm_2d_param.junc_view_3_front_param.result_3_front_param.car_logo_rect;
		/****malloc look up table****/
		for (Int32_t cam_id = 0; cam_id < CAMERA_NUM; cam_id++)
		{
			camera_size[cam_id] = p_avm_param->p_avm_config_param
				->avm_2d_param.junc_view_3_front_param.result_3_front_param.lut_size[cam_id] * 2;
		}
	}
	if (view_flag == _3_VIEW_REAR)//小图
	{
		result_image_height = p_avm_param->p_avm_config_param
			->avm_2d_param.junc_view_3_rear_param.result_3_rear_param.result_image_height;
		result_image_width = p_avm_param->p_avm_config_param
			->avm_2d_param.junc_view_3_rear_param.result_3_rear_param.result_image_width;
		p_lut = p_avm_param->p_avm_lut->p_lut_junc_view_3_rear;

		car_logo_rect = p_avm_param->p_avm_config_param
			->avm_2d_param.junc_view_3_rear_param.result_3_rear_param.car_logo_rect;
		/****malloc look up table****/
		for (Int32_t cam_id = 0; cam_id < CAMERA_NUM; cam_id++)
		{
			camera_size[cam_id] = p_avm_param->p_avm_config_param
				->avm_2d_param.junc_view_3_rear_param.result_3_rear_param.lut_size[cam_id] * 2;
		}
	}
	IplImage* image[4] = { NULL, NULL, NULL, NULL };
	my_load_image(image, p_avm_param);
	//此处的前后左右的高宽是指的在安霸存储表内的高宽，即旋转到原图的高宽，不是指显示在屏幕上的高宽
	Int32_t Front_width, Front_height, Right_width, Right_height,
		Left_width, Left_height, Back_width, Back_height;
	Front_width = (Int32_t)(ceil(result_image_width / 32.00) * 32);
	Front_height = (Int32_t)(ceil(car_logo_rect.y / 32.00) * 32);
	Back_width = (Int32_t)(ceil(result_image_width / 32.00) * 32);
	Back_height = (Int32_t)(ceil((result_image_height - car_logo_rect.y
		- car_logo_rect.height) / 32.00) * 32);
	Left_width = (Int32_t)(ceil(result_image_height / 32.00) * 32);
	Left_height = (Int32_t)(ceil(car_logo_rect.x / 32.00) * 32);
	Right_width = (Int32_t)((ceil(result_image_height / 32.00)) * 32);
	Right_height = (Int32_t)(ceil((result_image_width - car_logo_rect.x
		- car_logo_rect.width) / 32.00) * 32);
	Float32_t* Amba_Lut_Front_x = (Float32_t*)malloc(Front_height* Front_width*sizeof(Float32_t));
	Float32_t* Amba_Lut_Front_y = (Float32_t*)malloc(Front_height* Front_width*sizeof(Float32_t));
	Float32_t* Amba_Lut_Back_x = (Float32_t*)malloc(Back_height*Back_width*sizeof(Float32_t));
	Float32_t* Amba_Lut_Back_y = (Float32_t*)malloc(Back_height*Back_width*sizeof(Float32_t));
	Float32_t* Amba_Lut_Left_x = (Float32_t*)malloc(Left_height *Left_width*sizeof(Float32_t));
	Float32_t* Amba_Lut_Left_y = (Float32_t*)malloc(Left_height *Left_width*sizeof(Float32_t));
	Float32_t* Amba_Lut_Right_x = (Float32_t*)malloc(Right_height* Right_width*sizeof(Float32_t));
	Float32_t* Amba_Lut_Right_y = (Float32_t*)malloc(Right_height* Right_width*sizeof(Float32_t));
	//申请权重
	uchar* Amba_Lut_Left_weight = (uchar*)malloc(Left_height *Left_width*sizeof(uchar));
	uchar* Amba_Lut_Right_weight = (uchar*)malloc(Right_height* Right_width*sizeof(uchar));
	//Amba_Lut_Front_x[344] = 55.77;
	//提前计算出超出后的线性变化
	Float32_t image_point0[2];
	//此处的代码是从自己的裕兰表里面取数据然后转成安霸的表再转的过程中需要扩充到32的倍数所以程序读起来很麻烦。因为涉及到边界预测
#if 1
	if (view_flag == _4_VIEW_SMALL || view_flag == _4_VIEW_BIG || view_flag == _3_VIEW_FRONT)//小图
	{
		for (int i = 0; i < Front_height; i++)
		{
			for (int j = 0; j < Front_width; j++)
			{
				Float32_t image_point0[2];
				/************front view*******************************/
				int rgb[3];
				int shift;
				if (i < car_logo_rect.y&&j < result_image_width)//此处直接在裕兰表里数数据
				{
					shift = i * result_image_width * 2 + j * 2;
					Amba_resolve_data_from_lut(shift, image[0], rgb, p_avm_param, image_point0, p_lut);
					Amba_Lut_Front_x[i*Front_width + j] = image_point0[1];
					Amba_Lut_Front_y[i*Front_width + j] = image_point0[0];
				}
				

			}
		}
	}
#if 1
	if (view_flag == _4_VIEW_SMALL || view_flag == _4_VIEW_BIG || view_flag == _3_VIEW_REAR)//小图
	{
		//back
		for (int i = 0; i < Back_height; i++)
		{
			for (int j = 0; j < Back_width; j++)
			{
				int temp_0 = Back_height - (result_image_height
					- car_logo_rect.y - car_logo_rect.height) - 1;//表示扩充 出来的多余 的 行数
				/*************rear view*********************************/
				if (i > temp_0&&j < result_image_width)//此处是裕兰表里的点
				{
					int shift = camera_size[0] + (i - temp_0 - 1)
						* result_image_width * 2 + j * 2;//
					int rgb[3];
					Amba_resolve_data_from_lut(shift, image[1], rgb, p_avm_param, image_point0, p_lut);
					if (p_avm_param->project_flag == CVBS)
					{
						Amba_Lut_Back_x[i * Back_width + j] = image_point0[1];
						Amba_Lut_Back_y[i * Back_width + j] = image_point0[0];
					}
					if (p_avm_param->project_flag == HDMI)
					{
						Amba_Lut_Back_x[(Back_height - i - 1) * Back_width + Back_width - j - 1] = image_point0[1];
						Amba_Lut_Back_y[(Back_height - i - 1) * Back_width + Back_width - j - 1] = image_point0[0];
					}
				}
				
			}
		}

	}
#endif
	//left
	for (int i = 0; i < Left_width; i++)
	{
		for (int j = 0; j <Left_height; j++)
		{
			if (j<car_logo_rect.x)//此处是内测的点
			{
				int shift = camera_size[0] + camera_size[1] + (i)* car_logo_rect.x * 2 + j * 2;
				int rgb[3];
				uchar left_weight = Amba_resolve_data_from_lut(shift, image[2], rgb, p_avm_param, image_point0, p_lut);
				Amba_Lut_Left_x[j*Left_width + (result_image_height - 1 - i)] = image_point0[1];
				Amba_Lut_Left_y[j*Left_width + (result_image_height - 1 - i)] = image_point0[0];
				if (view_flag == _4_VIEW_BIG)//大图的权重需要反转一下
				{
					Amba_Lut_Left_weight[(Left_height - j - 1)*Left_width + (i)] = left_weight;
				}
				
			}
			
		}
	}
	//right
	for (int i = 0; i <Right_width; i++)
	{
		for (int j = 0; j <Right_height; j++)
		{
			int temp_0;
			/*************************right view****************/
			if (view_flag == _4_VIEW_SMALL&&p_avm_param->project_flag == CVBS)//CVBS小图预测的比较多
			{
				temp_0 = Right_height - 18 - (result_image_width - car_logo_rect.x - car_logo_rect.width) - 1;//扩充 出来之后多余的行数
				if (j <= temp_0)//最内侧的左边的点
				{
					Amba_Lut_Right_weight[i*Right_height + j] = 0;
				}
				if (j > temp_0&&j < 78 && i < result_image_height)//取点不需要预测 的
				{
					int shift = camera_size[0] + camera_size[1] + camera_size[2]
						+ i * (result_image_width - car_logo_rect.x - car_logo_rect.width) * 2
						+ (j - temp_0 - 1) * 2;
					int rgb[3];
					uchar Right_weight = Amba_resolve_data_from_lut(shift, image[3], rgb, p_avm_param, image_point0, p_lut);
					Amba_Lut_Right_x[(Right_height - 1 - j)*Right_width + i] = image_point0[1];
					Amba_Lut_Right_y[(Right_height - 1 - j)*Right_width + i] = image_point0[0];
					Amba_Lut_Right_weight[i*Right_height + j] = Right_weight;
				}
				if (j >= 78)//预测右边超出的点
				{

					int j0 = 69;
					int shift = camera_size[0] + camera_size[1] + camera_size[2]
						+ i * (result_image_width - car_logo_rect.x - car_logo_rect.width) * 2 + (j0)* 2;
					int rgb[3];
					Float32_t  image_point_1[2], image_point_2[2];
					Amba_resolve_data_from_lut(shift, image[3], rgb, p_avm_param, image_point_1, p_lut);
					j0 = 68;
					shift = camera_size[0] + camera_size[1] + camera_size[2]
						+ i * (result_image_width - car_logo_rect.x - car_logo_rect.width) * 2 + (j0)* 2;
					Amba_resolve_data_from_lut(shift, image[3], rgb, p_avm_param, image_point_2, p_lut);

					Float32_t dx = image_point_1[1] - image_point_2[1];
					Float32_t dy = image_point_1[0] - image_point_2[0];

					Amba_Lut_Right_x[(Right_height - 1 - j)*Right_width + i] = image_point_1[1] + (j - 77)*dx;
					Amba_Lut_Right_y[(Right_height - 1 - j)*Right_width + i] = image_point_1[0] + (j - 77)*dy;

					Amba_Lut_Right_weight[i*Right_height + j] = 0;

				}
			}
			else
			{
				temp_0 = Right_height - (result_image_width
					- car_logo_rect.x - car_logo_rect.width) - 1;//扩充 出来之后多余的行数
				if (j >temp_0 && i<result_image_height)//直接在玉兰表里面去数据
				{
					int shift = camera_size[0] + camera_size[1] + camera_size[2]
						+ i * (result_image_width - car_logo_rect.x - car_logo_rect.width) * 2
						+ (j - temp_0 - 1) * 2;
					int rgb[3];
					uchar Right_weight = Amba_resolve_data_from_lut(shift,
						image[3], rgb, p_avm_param, image_point0, p_lut);
					Amba_Lut_Right_x[(Right_height - 1 - j)*Right_width + i] = image_point0[1];
					Amba_Lut_Right_y[(Right_height - 1 - j)*Right_width + i] = image_point0[0];
					if (view_flag == _4_VIEW_BIG)//大图的权重需要反转一下
					{
						Amba_Lut_Right_weight[(Right_height - 1 - j)*Right_width + i] = Right_weight;
					}
					else
					{
						Amba_Lut_Right_weight[i*Right_height + j] = Right_weight;
					}
				}
				if (j <= temp_0 && i<result_image_height)//预测数据
				{
					int j0 = 1;
					int shift = camera_size[0] + camera_size[1] + camera_size[2]
						+ i * (result_image_width - car_logo_rect.x - car_logo_rect.width) * 2 + (j0)* 2;
					int rgb[3];
					Float32_t  image_point_1[2], image_point_2[2];
					Amba_resolve_data_from_lut(shift, image[3], rgb,
						p_avm_param, image_point_1, p_lut);
					j0 = 2;
					shift = camera_size[0] + camera_size[1] + camera_size[2]
						+ i * (result_image_width - car_logo_rect.x - car_logo_rect.width) * 2 + (j0)* 2;
					Amba_resolve_data_from_lut(shift, image[3], rgb, p_avm_param, image_point_2, p_lut);

					Float32_t dx = image_point_1[1] - image_point_2[1];
					Float32_t dy = image_point_1[0] - image_point_2[0];
					Amba_Lut_Right_x[(Right_height - 1 - j)*Right_width + i] = image_point_1[1] + (temp_0 - j)*dx;
					Amba_Lut_Right_y[(Right_height - 1 - j)*Right_width + i] = image_point_1[0] + (temp_0 - j)*dy;
					if (view_flag == _4_VIEW_BIG)//大图的权重需要反转一下
					{
						Amba_Lut_Right_weight[(Right_height - 1 - j)*Right_width + i] = 0;
					}
					else
					{
						Amba_Lut_Right_weight[i*Right_height + j] = 0;

					}
				}

			}
		}
	}
#endif





	




#if 1
	CvSize size1;
	IplImage *Amba_image_result_lut;
	IplImage *image_amplify;
	int rgb[3];
	char result_name[MAX_ARRAY_SIZE];
	//free(Amba_Lut_Back_x);
	//free(Amba_Lut_Back_y);
	//后的验证
	size1 = cvSize(Back_width, Back_height);
	Amba_image_result_lut = cvCreateImage(size1, IPL_DEPTH_8U, 1);


	for (int i = 0; i < Back_height; i++)
	{
		for (int j = 0; j < Back_width; j++)
		{
			bilinear_interpolation(image[1], rgb, Amba_Lut_Back_x[i* Back_width + j], Amba_Lut_Back_y[i* Back_width + j]);
			rgb[0] = (uchar)(Float64_t(rgb[0]));

			Amba_image_result_lut->imageData[i *Back_width + j] = (uchar)(rgb[0]);
		}
	}

	sprintf(result_name, "%s/四路的后视图.bmp", result_image_path);
	cvSaveImage(result_name, Amba_image_result_lut);


	//size1 = cvSize(Amba_Vector_Width, Amba_Vector_Height);
	//image_amplify = cvCreateImage(size1, IPL_DEPTH_8U, 1);
	//memset(image_amplify->imageData, 0, Amba_Vector_Height*Amba_Vector_Width);
	//rgb[3];
	//for (int i = 0; i < Amba_Vector_Height; i++)
	//{
	//	for (int j = 0; j < Amba_Vector_Width; j++)
	//	{

	//		bilinear_interpolation(image[1], rgb, Amba_Lut_Back_1280x960_x[i][j], Amba_Lut_Back_1280x960_y[i][j]);
	//		rgb[0] = (uchar)(Float64_t(rgb[0]));


	//		image_amplify->imageData[i *Amba_Vector_Width + j] = (uchar)(rgb[0]);
	//	}
	//}
	//result_name[MAX_ARRAY_SIZE];
	//sprintf(result_name, "%s/四路放大的后视图.bmp", result_image_path, 1);
	//cvSaveImage(result_name, image_amplify);

#endif




#if 1
	{
		CvSize size1;
		IplImage *Amba_image_result_lut;
		IplImage *image_amplify;
		int rgb[3];
		char result_name[MAX_ARRAY_SIZE];
		//free(Amba_Lut_Back_x);
		//free(Amba_Lut_Back_y);
		//后的验证
		size1 = cvSize(Right_width, Right_height);
		Amba_image_result_lut = cvCreateImage(size1, IPL_DEPTH_8U, 1);


		for (int i = 0; i < Right_height; i++)
		{
			for (int j = 0; j < Right_width; j++)
			{
				bilinear_interpolation(image[3], rgb, Amba_Lut_Right_x[i*Right_width + j], Amba_Lut_Right_y[i* Right_width + j]);
				rgb[0] = (uchar)(Float64_t(rgb[0]));

				Amba_image_result_lut->imageData[i *Right_width + j] = (uchar)(rgb[0]);
			}
		}

		sprintf(result_name, "%s/066666四路的youyoupppp视图.bmp", result_image_path);
		cvSaveImage(result_name, Amba_image_result_lut);
	}

#endif














	/**************/


	free(Amba_Lut_Front_x);
	free(Amba_Lut_Front_y);

	free(Amba_Lut_Back_x);
	free(Amba_Lut_Back_y);
	free(Amba_Lut_Left_x);
	free(Amba_Lut_Left_y);
	free(Amba_Lut_Right_x);
	free(Amba_Lut_Right_y);
	free(Amba_Lut_Left_weight);
	free(Amba_Lut_Right_weight);
	return error;
}











//权威 4路备份 
/*
* Function Name:				 Calculate_4_view_Aemba_Lut_Table
* Function Function:			 Calculate_4_view_Aemba_Lut_Table
* Input:
*     avm_handle:				the memory need to be freed
* Note:
*      Calculate_4_view_Aemba_Lut_Table
* Revision:
*     Create by Shuo.Yan on 2017/11/23.
*/


AVM_Error_Code_E Calculate_4_view_Aemba_Lut_Table(IN P_Avm_Param_S p_avm_param,
	IN Int32_t view_flag,  IN int view_id)
{
	AVM_Error_Code_E error = ERROR_NONE;
	int result_image_height;
	int result_image_width;
	CvRect car_logo_rect;
	UInt32_t* p_lut = NULL;
	int camera_size[4];
	char lut_path[MAX_ARRAY_SIZE];;
	if (view_flag == _4_VIEW_BIG)//大图 
	{
		p_lut = p_avm_param->p_avm_lut->p_lut_junc_view_4_big;
		result_image_height = p_avm_param->p_avm_config_param
			->avm_2d_param.junc_view_4_param.result_big_param.result_image_height;
		result_image_width = p_avm_param->p_avm_config_param
			->avm_2d_param.junc_view_4_param.result_big_param.result_image_width;
		car_logo_rect = p_avm_param->p_avm_config_param
			->avm_2d_param.junc_view_4_param.result_big_param.car_logo_rect;
		/****malloc look up table****/
		for (Int32_t cam_id = 0; cam_id < CAMERA_NUM; cam_id++)
		{
			camera_size[cam_id] = p_avm_param->p_avm_config_param
				->avm_2d_param.junc_view_4_param.result_big_param.lut_size[cam_id] * 2;
		}
	}
	if (view_flag == _4_VIEW_SMALL)//小图
	{
		result_image_height = p_avm_param->p_avm_config_param
			->avm_2d_param.junc_view_4_param.result_small_param.result_image_height;
		result_image_width = p_avm_param->p_avm_config_param
			->avm_2d_param.junc_view_4_param.result_small_param.result_image_width;
		p_lut = p_avm_param->p_avm_lut->p_lut_junc_view_4_small;
		car_logo_rect = p_avm_param->p_avm_config_param
			->avm_2d_param.junc_view_4_param.result_small_param.car_logo_rect;
		/****malloc look up table****/
		for (Int32_t cam_id = 0; cam_id < CAMERA_NUM; cam_id++)
		{
			camera_size[cam_id] = p_avm_param->p_avm_config_param
				->avm_2d_param.junc_view_4_param.result_small_param.lut_size[cam_id] * 2;
		}
	}
	if (view_flag == _3_VIEW_FRONT)//小图
	{
		result_image_height = p_avm_param->p_avm_config_param
			->avm_2d_param.junc_view_3_front_param.result_3_front_param.result_image_height;
		result_image_width = p_avm_param->p_avm_config_param
			->avm_2d_param.junc_view_3_front_param.result_3_front_param.result_image_width;
		p_lut = p_avm_param->p_avm_lut->p_lut_junc_view_3_front;

		car_logo_rect = p_avm_param->p_avm_config_param
			->avm_2d_param.junc_view_3_front_param.result_3_front_param.car_logo_rect;
		/****malloc look up table****/
		for (Int32_t cam_id = 0; cam_id < CAMERA_NUM; cam_id++)
		{
			camera_size[cam_id] = p_avm_param->p_avm_config_param
				->avm_2d_param.junc_view_3_front_param.result_3_front_param.lut_size[cam_id] * 2;
		}
	}
	if (view_flag == _3_VIEW_REAR)//小图
	{
		result_image_height = p_avm_param->p_avm_config_param
			->avm_2d_param.junc_view_3_rear_param.result_3_rear_param.result_image_height;
		result_image_width = p_avm_param->p_avm_config_param
			->avm_2d_param.junc_view_3_rear_param.result_3_rear_param.result_image_width;
		p_lut = p_avm_param->p_avm_lut->p_lut_junc_view_3_rear;

		car_logo_rect = p_avm_param->p_avm_config_param
			->avm_2d_param.junc_view_3_rear_param.result_3_rear_param.car_logo_rect;
		/****malloc look up table****/
		for (Int32_t cam_id = 0; cam_id < CAMERA_NUM; cam_id++)
		{
			camera_size[cam_id] = p_avm_param->p_avm_config_param
				->avm_2d_param.junc_view_3_rear_param.result_3_rear_param.lut_size[cam_id] * 2;
		}
	}
	IplImage* image[4] = { NULL, NULL, NULL, NULL };
	my_load_image(image, p_avm_param);
	//此处的前后左右的高宽是指的在安霸存储表内的高宽，即旋转到原图的高宽，不是指显示在屏幕上的高宽
	Int32_t Front_width, Front_height, Right_width, Right_height,
		Left_width, Left_height,Back_width,Back_height;
	Front_width = (Int32_t)(ceil(result_image_width / 32.00) * 32);
	Front_height = (Int32_t)(ceil(car_logo_rect.y / 32.00) * 32);
	Back_width = (Int32_t)(ceil(result_image_width / 32.00) * 32);
	Back_height = (Int32_t)(ceil((result_image_height - car_logo_rect.y
		- car_logo_rect.height) / 32.00) * 32);
	Left_width = (Int32_t)(ceil(result_image_height / 32.00) * 32);
	Left_height = (Int32_t)(ceil(car_logo_rect.x / 32.00) * 32);
	Right_width = (Int32_t)((ceil(result_image_height / 32.00)) * 32);
	Right_height = (Int32_t)(ceil((result_image_width - car_logo_rect.x
		- car_logo_rect.width) / 32.00) * 32);
	Float32_t* Amba_Lut_Front_x = (Float32_t*)malloc(Front_height* Front_width*sizeof(Float32_t));
	Float32_t* Amba_Lut_Front_y = (Float32_t*)malloc(Front_height* Front_width*sizeof(Float32_t));
	Float32_t* Amba_Lut_Back_x = (Float32_t*)malloc(Back_height*Back_width*sizeof(Float32_t));
	Float32_t* Amba_Lut_Back_y = (Float32_t*)malloc(Back_height*Back_width*sizeof(Float32_t));
	Float32_t* Amba_Lut_Left_x = (Float32_t*)malloc(Left_height *Left_width*sizeof(Float32_t));
	Float32_t* Amba_Lut_Left_y = (Float32_t*)malloc(Left_height *Left_width*sizeof(Float32_t));
	Float32_t* Amba_Lut_Right_x = (Float32_t*)malloc(Right_height* Right_width*sizeof(Float32_t));
	Float32_t* Amba_Lut_Right_y = (Float32_t*)malloc(Right_height* Right_width*sizeof(Float32_t));
	//申请权重
	uchar* Amba_Lut_Left_weight = (uchar*)malloc(Left_height *Left_width*sizeof(uchar));
	uchar* Amba_Lut_Right_weight = (uchar*)malloc(Right_height* Right_width*sizeof(uchar));
	//Amba_Lut_Front_x[344] = 55.77;
	//提前计算出超出后的线性变化
	Float32_t image_point0[2];
	//此处的代码是从自己的裕兰表里面取数据然后转成安霸的表再转的过程中需要扩充到32的倍数所以程序读起来很麻烦。因为涉及到边界预测
#if 1
	if (view_flag == _4_VIEW_SMALL || view_flag == _4_VIEW_BIG || view_flag == _3_VIEW_FRONT)//小图
	{
		for (int i = 0; i < Front_height; i++)
		{
			for (int j = 0; j < Front_width; j++)
			{
				Float32_t image_point0[2];
				/************front view*******************************/
				int rgb[3];
				int shift;
				if (i < car_logo_rect.y&&j < result_image_width)//此处直接在裕兰表里数数据
				{
					shift = i * result_image_width * 2 + j * 2;
					Amba_resolve_data_from_lut(shift, image[0], rgb, p_avm_param, image_point0, p_lut);
					Amba_Lut_Front_x[i*Front_width + j] = image_point0[1];
					Amba_Lut_Front_y[i*Front_width + j] = image_point0[0];
				}
				if (i >= car_logo_rect.y)//如果扩充的点行数超过预测小车盖住 的 内部 预测
				{
					int i0 = car_logo_rect.y - 1;
					int rgb[3];
					Float32_t  image_point_1[2], image_point_2[2];
					shift = i0 * result_image_width * 2 + j * 2;
					Amba_resolve_data_from_lut(shift, image[0], rgb, p_avm_param, image_point_1, p_lut);
					i0 = car_logo_rect.y - 2;
					shift = i0 * result_image_width * 2 + j * 2;
					Amba_resolve_data_from_lut(shift, image[0], rgb, p_avm_param, image_point_2, p_lut);
					Float32_t dx = image_point_1[1] - image_point_2[1];
					Float32_t dy = image_point_1[0] - image_point_2[0];
					Amba_Lut_Front_x[i*Front_width + j] = image_point_1[1] + (i - car_logo_rect.y + 1)*dx;
					Amba_Lut_Front_y[i*Front_width + j] = image_point_1[0] + (i - car_logo_rect.y + 1)*dy;
				}
				
			}
		}
	}
#if 1
	if (view_flag == _4_VIEW_SMALL || view_flag == _4_VIEW_BIG || view_flag == _3_VIEW_REAR)//小图
	{
		//back
		for (int i = 0; i < Back_height; i++)
		{
			for (int j = 0; j < Back_width; j++)
			{
				int temp_0 = Back_height - (result_image_height
					- car_logo_rect.y - car_logo_rect.height) - 1;//表示扩充 出来的多余 的 行数
				/*************rear view*********************************/
				if (i > temp_0&&j < result_image_width)//此处是裕兰表里的点
				{
					int shift = camera_size[0] + (i - temp_0 - 1)
						* result_image_width * 2 + j * 2;//
					int rgb[3];
					Amba_resolve_data_from_lut(shift, image[1], rgb, p_avm_param, image_point0, p_lut);
					if (p_avm_param->project_flag == CVBS)
					{
						Amba_Lut_Back_x[i * Back_width + j] = image_point0[1];
						Amba_Lut_Back_y[i * Back_width + j] = image_point0[0];
					}
					if (p_avm_param->project_flag == HDMI)
					{
						Amba_Lut_Back_x[(Back_height - i - 1) * Back_width + Back_width - j - 1] = image_point0[1];
						Amba_Lut_Back_y[(Back_height - i - 1) * Back_width + Back_width - j - 1] = image_point0[0];
					}
				}
				if (i <= temp_0)
				{
					int shift = camera_size[0] + (0)* result_image_width * 2 + j * 2;
					int rgb[3];
					Float32_t  image_point_1[2], image_point_2[2];
					Amba_resolve_data_from_lut(shift, image[0], rgb, p_avm_param, image_point_1, p_lut);
					shift = camera_size[0] + (1) * result_image_width * 2 + j * 2;
					Amba_resolve_data_from_lut(shift, image[0], rgb, p_avm_param, image_point_2, p_lut);
					Float32_t dx = image_point_1[1] - image_point_2[1];
					Float32_t dy = image_point_1[0] - image_point_2[0];
					if (p_avm_param->project_flag == CVBS)
					{
						Amba_Lut_Back_x[i*Back_width + j] = image_point_1[1] + (temp_0 - i)*dx;
						Amba_Lut_Back_y[i*Back_width + j] = image_point_1[0] + (temp_0 - i)*dy;
					}
					if (p_avm_param->project_flag == HDMI)
					{
						Amba_Lut_Back_x[(Back_height - i - 1)* Back_width + Back_width - j - 1] = image_point0[1];
						Amba_Lut_Back_y[(Back_height - i - 1) * Back_width + Back_width - j - 1] = image_point0[0];
					}
				}
			}
		}

	}
#endif
	//left
	for (int i = 0; i < Left_width ; i++)
	{
		for (int j = 0; j <Left_height; j++)
		{
			if (j<car_logo_rect.x)//此处是内测的点
			{
				int shift = camera_size[0] + camera_size[1] + (i)* car_logo_rect.x * 2 + j * 2;
				int rgb[3];
				uchar left_weight=Amba_resolve_data_from_lut(shift, image[2], rgb, p_avm_param, image_point0, p_lut);
				Amba_Lut_Left_x[j*Left_width + (result_image_height - 1 - i)] = image_point0[1];
				Amba_Lut_Left_y[j*Left_width + (result_image_height - 1 - i)] = image_point0[0];
				if (view_flag == _4_VIEW_BIG)//大图的权重需要反转一下
				{
					Amba_Lut_Left_weight[(Left_height-j-1)*Left_width + (i)] = left_weight;
				}
				else
				{
					Amba_Lut_Left_weight[i*Left_height + j] = left_weight;//填充图像的权值
				}
			}
			if (j >= car_logo_rect.x)//预测的点
			{
				int j0 = car_logo_rect.x-1;
				int shift = camera_size[0] + camera_size[1] + (i)* car_logo_rect.x * 2 + j0 * 2;
				int rgb[3];
				Float32_t  image_point_1[2], image_point_2[2];
				Amba_resolve_data_from_lut(shift, image[2], rgb, p_avm_param, image_point_1, p_lut);
				j0 = car_logo_rect.x - 2;
				shift = camera_size[0] + camera_size[1] + (i)* car_logo_rect.x * 2 + j0 * 2;
				Amba_resolve_data_from_lut(shift, image[2], rgb, p_avm_param, image_point_2, p_lut);

				Float32_t dx = image_point_1[1] - image_point_2[1];
				Float32_t dy = image_point_1[0] - image_point_2[0];
				Amba_Lut_Left_x[j*Left_width + (result_image_height - 1 - i)] = image_point_1[1] + (j - j0)*dx;
				Amba_Lut_Left_y[j*Left_width + (result_image_height - 1 - i)] = image_point_1[0] + (j - j0)*dy;
				if (view_flag == _4_VIEW_BIG)//大图的权重需要反转一下
				{
					Amba_Lut_Left_weight[(Left_height - j - 1)*Left_width + (i)] = 0;
				}
				else
				{
					Amba_Lut_Left_weight[i*Left_height + j] = 0;
				}
			}
		}
	}
	//right
	for (int i = 0; i <Right_width; i++)
	{
		for (int j = 0; j <Right_height; j++)
		{
			int temp_0;
			/*************************right view****************/
			if (view_flag == _4_VIEW_SMALL&&p_avm_param->project_flag == CVBS)//CVBS小图预测的比较多
			{
				temp_0 = Right_height - 18 - (result_image_width - car_logo_rect.x - car_logo_rect.width) - 1;//扩充 出来之后多余的行数
				if (j <= temp_0)//最内侧的左边的点
				{
						Amba_Lut_Right_weight[i*Right_height + j] = 0;
				}
				if (j > temp_0&&j < 78 && i < result_image_height)//取点不需要预测 的
				{
					int shift = camera_size[0] + camera_size[1] + camera_size[2]
						+ i * (result_image_width - car_logo_rect.x - car_logo_rect.width) * 2
						+ (j - temp_0 - 1) * 2;
					int rgb[3];
					uchar Right_weight = Amba_resolve_data_from_lut(shift, image[3], rgb, p_avm_param, image_point0, p_lut);
					Amba_Lut_Right_x[(Right_height - 1 - j)*Right_width + i] = image_point0[1];
					Amba_Lut_Right_y[(Right_height - 1 - j)*Right_width + i] = image_point0[0];
						Amba_Lut_Right_weight[i*Right_height + j] = Right_weight;
				}
				if (j >= 78)//预测右边超出的点
				{

					int j0 = 69;
					int shift = camera_size[0] + camera_size[1] + camera_size[2] 
						+ i * (result_image_width - car_logo_rect.x - car_logo_rect.width) * 2 + (j0)* 2;
					int rgb[3];
					Float32_t  image_point_1[2], image_point_2[2];
					Amba_resolve_data_from_lut(shift, image[3], rgb, p_avm_param, image_point_1, p_lut);
					j0 = 68;
					shift = camera_size[0] + camera_size[1] + camera_size[2] 
						+ i * (result_image_width - car_logo_rect.x - car_logo_rect.width) * 2 + (j0)* 2;
					Amba_resolve_data_from_lut(shift, image[3], rgb, p_avm_param, image_point_2, p_lut);

					Float32_t dx = image_point_1[1] - image_point_2[1];
					Float32_t dy = image_point_1[0] - image_point_2[0];

					Amba_Lut_Right_x[(Right_height - 1 - j)*Right_width + i] = image_point_1[1] + (j - 77)*dx;
					Amba_Lut_Right_y[(Right_height - 1 - j)*Right_width + i] = image_point_1[0] + (j - 77)*dy;

					Amba_Lut_Right_weight[i*Right_height + j] = 0;
						
				}
			}
			else
			{
				temp_0 = Right_height  - (result_image_width 
					- car_logo_rect.x - car_logo_rect.width) - 1;//扩充 出来之后多余的行数
				if (j >temp_0 && i<result_image_height)//直接在玉兰表里面去数据
				{
					int shift = camera_size[0] + camera_size[1] + camera_size[2]
						+ i * (result_image_width - car_logo_rect.x - car_logo_rect.width) * 2
						+ (j - temp_0 - 1) * 2;
					int rgb[3];
					uchar Right_weight=Amba_resolve_data_from_lut(shift,
						image[3], rgb, p_avm_param, image_point0, p_lut);
					Amba_Lut_Right_x[(Right_height - 1 - j)*Right_width + i] = image_point0[1];
					Amba_Lut_Right_y[(Right_height - 1 - j)*Right_width + i] = image_point0[0];
					if (view_flag == _4_VIEW_BIG)//大图的权重需要反转一下
					{
						Amba_Lut_Right_weight[(Right_height - 1 - j)*Right_width + i] = Right_weight;
					}
					else
					{
						Amba_Lut_Right_weight[i*Right_height + j] = Right_weight;
					}
				}
				if (j <= temp_0 && i<result_image_height)//预测数据
				{
					int j0 =1;
					int shift = camera_size[0] + camera_size[1] + camera_size[2] 
						+ i * (result_image_width - car_logo_rect.x - car_logo_rect.width) * 2 + (j0) * 2;
					int rgb[3];
					Float32_t  image_point_1[2], image_point_2[2];
					Amba_resolve_data_from_lut(shift, image[3], rgb, 
						p_avm_param, image_point_1, p_lut);
					j0 = 2;
					shift = camera_size[0] + camera_size[1] + camera_size[2] 
						+ i * (result_image_width - car_logo_rect.x - car_logo_rect.width) * 2 + (j0)* 2;
					Amba_resolve_data_from_lut(shift, image[3], rgb, p_avm_param, image_point_2, p_lut);

					Float32_t dx = image_point_1[1] - image_point_2[1];
					Float32_t dy = image_point_1[0] - image_point_2[0];
					Amba_Lut_Right_x[(Right_height - 1 - j)*Right_width + i] = image_point_1[1] + (temp_0 - j)*dx;
					Amba_Lut_Right_y[(Right_height - 1 - j)*Right_width + i] = image_point_1[0] + (temp_0 - j)*dy;
					if (view_flag == _4_VIEW_BIG)//大图的权重需要反转一下
					{
						Amba_Lut_Right_weight[(Right_height - 1 - j)*Right_width + i] = 0;
					}
					else
					{
						Amba_Lut_Right_weight[i*Right_height + j] =0;

					}
				}
				
			}
		}
	}
#endif





	FILE* pFile;
	//生成81*61向量表,水平方向与垂直方向
	for (Int32_t i = 0; i < 61; i++)
	{
		for (Int32_t j = 0; j < 81; j++)
		{
			Float32_t x_coord, y_coord;
			Int32_t ver_grid_index = i * 16;
			Int32_t hor_grid_index = j * 16;
			ver_grid_index = ver_grid_index >= 959 ? 959 : ver_grid_index;
			hor_grid_index = hor_grid_index >= 1279 ? 1279 : hor_grid_index;
			//front
			Float32_t temp_x = (Float32_t)(ver_grid_index) / (1.0 * Amba_Vector_Height / Front_height);
			Float32_t temp_y = (Float32_t)(hor_grid_index) / (1.0 * Amba_Vector_Width / Front_width);
			Int32_t x0 = (Int32_t)temp_x;
			Int32_t x1 = (Int32_t)temp_x + 1;
			Int32_t y0 = (Int32_t)temp_y;
			Int32_t y1 = (Int32_t)temp_y + 1;
			x1 = x1 >= Front_height ? (Front_height - 1) : x1;
			y1 = y1 >= Front_width ? (Front_width - 1) : y1;
			temp_x = temp_x > x1 ? x1 : temp_x;
			temp_y = temp_y > y1 ? y1 : temp_y;
			Float32_t weightUpLeft = (x1 - temp_x) * (y1 - temp_y);
			Float32_t weightDownLeft = (temp_x - x0) * (y1 - temp_y);
			Float32_t weightUpRight = (x1 - temp_x) * (temp_y - y0);
			Float32_t weightDownRight = 1 - weightUpLeft - weightUpRight - weightDownLeft;
			if (view_flag == _4_VIEW_SMALL || view_flag == _4_VIEW_BIG || view_flag == _3_VIEW_FRONT)//小图
			{
				x_coord = weightUpLeft * Amba_Lut_Front_x[x0*Front_width + y0]
					+ weightUpRight * Amba_Lut_Front_x[x0*Front_width + y1]
					+ weightDownLeft * Amba_Lut_Front_x[x1*Front_width + y0]
					+ weightDownRight * Amba_Lut_Front_x[x1*Front_width + y1];
				y_coord = weightUpLeft * Amba_Lut_Front_y[x0*Front_width + y0]
					+ weightUpRight * Amba_Lut_Front_y[x0*Front_width + y1]
					+ weightDownLeft * Amba_Lut_Front_y[x1*Front_width + y0]
					+ weightDownRight * Amba_Lut_Front_y[x1*Front_width + y1];
				Amba_front[2 * (81 * i + j)] =
					(Int16_t)((x_coord - 16 * j) * 16);
				Amba_front[2 * (81 * i + j) + 1] =
					(Int16_t)((y_coord - 16 * i) * 16);
			}
			if (view_flag == _4_VIEW_SMALL || view_flag == _4_VIEW_BIG || view_flag == _3_VIEW_REAR)//小图
			{
				//back///////////////////////////
				temp_x = (Float32_t)(ver_grid_index) / (1.0 * Amba_Vector_Height / Back_height);//等价于i*(Back_height/Amba_Vector_Height)
				temp_y = (Float32_t)(hor_grid_index) / (1.0 * Amba_Vector_Width / Back_width);
				x0 = (Int32_t)temp_x;
				x1 = (Int32_t)temp_x + 1;
				y0 = (Int32_t)temp_y;
				y1 = (Int32_t)temp_y + 1;

				x1 = x1 >= Back_height ? (Back_height - 1) : x1;
				y1 = y1 >= Back_width ? (Back_width - 1) : y1;
				temp_x = temp_x > x1 ? x1 : temp_x;
				temp_y = temp_y > y1 ? y1 : temp_y;

				weightUpLeft = (x1 - temp_x) * (y1 - temp_y);
				weightDownLeft = (temp_x - x0) * (y1 - temp_y);
				weightUpRight = (x1 - temp_x) * (temp_y - y0);
				weightDownRight = 1 - weightUpLeft - weightUpRight - weightDownLeft;

				x_coord = weightUpLeft * Amba_Lut_Back_x[x0*Back_width + y0]
					+ weightUpRight * Amba_Lut_Back_x[x0*Back_width + y1]
					+ weightDownLeft * Amba_Lut_Back_x[x1*Back_width + y0]
					+ weightDownRight * Amba_Lut_Back_x[x1*Back_width + y1];
				y_coord = weightUpLeft * Amba_Lut_Back_y[x0*Back_width + y0]
					+ weightUpRight * Amba_Lut_Back_y[x0*Back_width + y1]
					+ weightDownLeft * Amba_Lut_Back_y[x1*Back_width + y0]
					+ weightDownRight * Amba_Lut_Back_y[x1*Back_width + y1];
				Amba_back[2 * (81 * i + j)] =
					(Int16_t)((x_coord - 16 * j) * 16);
				Amba_back[2 * (81 * i + j) + 1] =
					(Int16_t)((y_coord - 16 * i) * 16);
			}
			//left
			temp_x = (Float32_t)(ver_grid_index) / (1.0 * Amba_Vector_Height / Left_height);
			temp_y = (Float32_t)(hor_grid_index) / (1.0 * Amba_Vector_Width / Left_width);
			x0 = (Int32_t)temp_x;
			x1 = (Int32_t)temp_x + 1;
			y0 = (Int32_t)temp_y;
			y1 = (Int32_t)temp_y + 1;
			x1 = x1 >= Left_height ? (Left_height - 1) : x1;
			y1 = y1 >= Left_width ? (Left_width - 1) : y1;
			temp_x = temp_x > x1 ? x1 : temp_x;
			temp_y = temp_y > y1 ? y1 : temp_y;
			weightUpLeft = (x1 - temp_x) * (y1 - temp_y);
			weightDownLeft = (temp_x - x0) * (y1 - temp_y);
			weightUpRight = (x1 - temp_x) * (temp_y - y0);
			weightDownRight = 1 - weightUpLeft - weightUpRight - weightDownLeft;
			x_coord = weightUpLeft * Amba_Lut_Left_x[x0*Left_width + y0]
				+ weightUpRight * Amba_Lut_Left_x[x0*Left_width + y1]
				+ weightDownLeft * Amba_Lut_Left_x[x1*Left_width + y0]
				+ weightDownRight * Amba_Lut_Left_x[x1*Left_width + y1];
			y_coord = weightUpLeft * Amba_Lut_Left_y[x0*Left_width + y0]
				+ weightUpRight * Amba_Lut_Left_y[x0*Left_width + y1]
				+ weightDownLeft * Amba_Lut_Left_y[x1*Left_width + y0]
				+ weightDownRight * Amba_Lut_Left_y[x1*Left_width + y1];

			Amba_left[2 * (81 * i + j)] = 
				(Int16_t)((x_coord - 16 * j) * 16);
			Amba_left[2 * (81 * i + j) + 1] = 
				(Int16_t)((y_coord - 16 * i) * 16);
			//right

			temp_x = (Float32_t)(ver_grid_index) / (1.0 * Amba_Vector_Height / Right_height);
			temp_y = (Float32_t)(hor_grid_index) / (1.0 * Amba_Vector_Width / Right_width);
			x0 = (Int32_t)temp_x;
			x1 = (Int32_t)temp_x + 1;
			y0 = (Int32_t)temp_y;
			y1 = (Int32_t)temp_y + 1;
			
			x1 = x1 >= Right_height ? (Right_height - 1) : x1;
			y1 = y1 >= Right_width ? (Right_width - 1) : y1;

			temp_x = temp_x > x1 ? x1 : temp_x;
			temp_y = temp_y > y1 ? y1 : temp_y;

			weightUpLeft = (x1 - temp_x) * (y1 - temp_y);
			weightDownLeft = (temp_x - x0) * (y1 - temp_y);
			weightUpRight = (x1 - temp_x) * (temp_y - y0);
			weightDownRight = 1 - weightUpLeft - weightUpRight - weightDownLeft;

			x_coord = weightUpLeft * Amba_Lut_Right_x[x0*Right_width + y0]
				+ weightUpRight * Amba_Lut_Right_x[x0*Right_width + y1]
				+ weightDownLeft * Amba_Lut_Right_x[x1*Right_width + y0]
				+ weightDownRight * Amba_Lut_Right_x[x1*Right_width + y1];
			y_coord = weightUpLeft * Amba_Lut_Right_y[x0*Right_width + y0]
				+ weightUpRight * Amba_Lut_Right_y[x0*Right_width + y1]
				+ weightDownLeft * Amba_Lut_Right_y[x1*Right_width + y0]
				+ weightDownRight * Amba_Lut_Right_y[x1*Right_width + y1];

			//printf("%f,%f\n", x_coord, y_coord);
			Amba_right[2 * (81 * i + j)] = 
				(Int16_t)((x_coord - 16 * j) * 16);
			Amba_right[2 * (81 * i + j) + 1] = 
				(Int16_t)((y_coord - 16 * i) * 16);

		}
		

	}


#if 1
	sprintf(lut_path, "%s/calib17.bin",
		lut_file_path);
	pFile = fopen(lut_path, "rb+");
	if (!pFile)
	{
		printf("安霸表17和21这两个表不存在 %s cannot be opened\n", lut_path);
		CHECK_ERROR(error);
	}
	for (Int32_t cam_id = 0; cam_id < CAMERA_NUM; cam_id++)
	{
		int target_pos = 94720 * view_id + 2624 + 23024 * cam_id + 64;
		switch (cam_id)
		{
		case 0:
			fseek(pFile, target_pos, SEEK_SET);
			fwrite(Amba_front, sizeof(Int16_t), 61 * 81 * 2, pFile);
			break;
		case 1:
			fseek(pFile, target_pos, SEEK_SET);
			fwrite(Amba_back, sizeof(Int16_t), 61 * 81 * 2, pFile);

			break;
		case 2:
			fseek(pFile, target_pos, SEEK_SET);
			fwrite(Amba_left, sizeof(Int16_t), 61 * 81 * 2, pFile);

			break;
		case 3:
			fseek(pFile, target_pos, SEEK_SET);
			fwrite(Amba_right, sizeof(Int16_t), 61 * 81 * 2, pFile);
			break;
		default:
			break;
		}
	}
	fclose(pFile);
#endif
	
	sprintf(lut_path, "%s/calib21.bin",
		lut_file_path);
	pFile = fopen(lut_path, "rb+");
	if (!pFile)
	{
		printf("安霸表17和21这两个表不存在 %s cannot be opened\n", lut_path);
		CHECK_ERROR(error);
	}
	write_calib21(p_avm_param,pFile, view_id, Left_height,
		Left_width, Right_height, Right_width
		, Amba_Lut_Left_weight, Amba_Lut_Right_weight);
	fclose(pFile);




	/**************/

#if 0
	{
		CvSize size1;
		IplImage *Amba_image_result_lut;
		IplImage *image_amplify;
		int rgb[3];
		char result_name[MAX_ARRAY_SIZE];
		//free(Amba_Lut_Back_x);
		//free(Amba_Lut_Back_y);
		//后的验证
		size1 = cvSize(Front_width, Front_height);
		Amba_image_result_lut = cvCreateImage(size1, IPL_DEPTH_8U, 1);


		for (int i = 0; i < Front_height; i++)
		{
			for (int j = 0; j < Front_width; j++)
			{
				bilinear_interpolation(image[0], rgb, Amba_Lut_Front_x[i* Front_width + j], Amba_Lut_Front_y[i* Front_width + j]);
				rgb[0] = (uchar)(Float64_t(rgb[0]));

				Amba_image_result_lut->imageData[i *Front_width + j] = (uchar)(rgb[0]);
			}
		}

		sprintf(result_name, "%s/066666四路的qian视图.bmp", result_image_path);
		cvSaveImage(result_name, Amba_image_result_lut);
	}

#endif



#if 1
	CvSize size1;
	IplImage *Amba_image_result_lut;
	IplImage *image_amplify;
	int rgb[3];
	char result_name[MAX_ARRAY_SIZE];
	//free(Amba_Lut_Back_x);
	//free(Amba_Lut_Back_y);
	//后的验证
	size1 = cvSize(Back_width, Back_height);
	Amba_image_result_lut = cvCreateImage(size1, IPL_DEPTH_8U, 1);


	for (int i = 0; i < Back_height; i++)
	{
		for (int j = 0; j < Back_width; j++)
		{
			bilinear_interpolation(image[1], rgb, Amba_Lut_Back_x[i* Back_width + j], Amba_Lut_Back_y[i* Back_width + j]);
			rgb[0] = (uchar)(Float64_t(rgb[0]));

			Amba_image_result_lut->imageData[i *Back_width + j] = (uchar)(rgb[0]);
		}
	}

	sprintf(result_name, "%s/四路的后视图.bmp", result_image_path);
	cvSaveImage(result_name, Amba_image_result_lut);


	//size1 = cvSize(Amba_Vector_Width, Amba_Vector_Height);
	//image_amplify = cvCreateImage(size1, IPL_DEPTH_8U, 1);
	//memset(image_amplify->imageData, 0, Amba_Vector_Height*Amba_Vector_Width);
	//rgb[3];
	//for (int i = 0; i < Amba_Vector_Height; i++)
	//{
	//	for (int j = 0; j < Amba_Vector_Width; j++)
	//	{

	//		bilinear_interpolation(image[1], rgb, Amba_Lut_Back_1280x960_x[i][j], Amba_Lut_Back_1280x960_y[i][j]);
	//		rgb[0] = (uchar)(Float64_t(rgb[0]));


	//		image_amplify->imageData[i *Amba_Vector_Width + j] = (uchar)(rgb[0]);
	//	}
	//}
	//result_name[MAX_ARRAY_SIZE];
	//sprintf(result_name, "%s/四路放大的后视图.bmp", result_image_path, 1);
	//cvSaveImage(result_name, image_amplify);

#endif




#if 1
	{
		CvSize size1;
		IplImage *Amba_image_result_lut;
		IplImage *image_amplify;
		int rgb[3];
		char result_name[MAX_ARRAY_SIZE];
		//free(Amba_Lut_Back_x);
		//free(Amba_Lut_Back_y);
		//后的验证
		size1 = cvSize(Right_width, Right_height);
		Amba_image_result_lut = cvCreateImage(size1, IPL_DEPTH_8U, 1);


		for (int i = 0; i < Right_height; i++)
		{
			for (int j = 0; j < Right_width; j++)
			{
				bilinear_interpolation(image[3], rgb, Amba_Lut_Right_x[i*Right_width + j], Amba_Lut_Right_y[i* Right_width + j]);
				rgb[0] = (uchar)(Float64_t(rgb[0]));

				Amba_image_result_lut->imageData[i *Right_width + j] = (uchar)(rgb[0]);
			}
		}

		sprintf(result_name, "%s/066666四路的youyoupppp视图.bmp", result_image_path);
		cvSaveImage(result_name, Amba_image_result_lut);
	}

#endif














	/**************/


	free(Amba_Lut_Front_x);
	free(Amba_Lut_Front_y );

	free(Amba_Lut_Back_x);
	free(Amba_Lut_Back_y);
	free(Amba_Lut_Left_x);
	free(Amba_Lut_Left_y);
	free(Amba_Lut_Right_x);
	free(Amba_Lut_Right_y);
	free(Amba_Lut_Left_weight);
	free(Amba_Lut_Right_weight);
	return error;
}

#if 1


/**************handle_static_dynamic_line***********************/
/*
* Function Name:				  Calculate_single_view_Amba_Lut_Table
* Function Function:			 Calculate_single_view_Amba_Lut_Table
*       p_avm_param;
* Note:
*     the interface function should be called at the beginning of this module
* Revision:
*     Create by Cheng.Li on 2018/5/2.
*/
AVM_Error_Code_E Calculate_single_view_Amba_Lut_Table(IN  P_Avm_Param_S p_avm_param, int cam_flag, int view_id)
{
	AVM_Error_Code_E error = ERROR_NONE;
	FILE* pFile;
	Front_Rear_Single_View_Param_S single_view_param_front_rear;
	Left_Right_Single_View_Param_S left_right_single_view_param;
	CvSize size0;
	UInt32_t* p_lut = NULL;
	char lut_path[MAX_ARRAY_SIZE];

	int result_image_height;
	int result_image_width;
	
	if (cam_flag == _1_VIEW_FRONT)
	{
		single_view_param_front_rear = p_avm_param->p_avm_config_param
			->avm_single_view_param.front_single_view_param;
		size0 = cvSize(single_view_param_front_rear.result_image_width,
			single_view_param_front_rear.result_image_height);//
		p_lut = p_avm_param->p_avm_lut->p_lut_single_view_front;
	
		result_image_height = single_view_param_front_rear.result_image_height;
		result_image_width = single_view_param_front_rear.result_image_width;
	}
	if (cam_flag == _1_VIEW_REAR)
	{
		single_view_param_front_rear = p_avm_param->p_avm_config_param
			->avm_single_view_param.rear_single_view_param;
		size0 = cvSize(single_view_param_front_rear.result_image_width, 
			single_view_param_front_rear.result_image_height);//
		p_lut = p_avm_param->p_avm_lut->p_lut_single_view_rear;
	
		result_image_height = single_view_param_front_rear.result_image_height;
		result_image_width = single_view_param_front_rear.result_image_width;
		
	}

	if (cam_flag == _1_VIEW_LEFT)//左单视图
	{
		left_right_single_view_param = p_avm_param->p_avm_config_param
			->avm_single_view_param.left_single_view_param;
		size0 = cvSize(left_right_single_view_param.result_image_width,
			left_right_single_view_param.result_image_height);//
		p_lut = p_avm_param->p_avm_lut->p_lut_single_view_left;
		
		result_image_height = left_right_single_view_param.result_image_height;
		result_image_width = left_right_single_view_param.result_image_width;
	
	}
	if (cam_flag == _1_VIEW_RIGHT)//右单视图
	{
		left_right_single_view_param = p_avm_param->p_avm_config_param
			->avm_single_view_param.right_single_view_param;
		size0 = cvSize(left_right_single_view_param.result_image_width,
			left_right_single_view_param.result_image_height);//
		p_lut = p_avm_param->p_avm_lut->p_lut_single_view_right;
	
		result_image_height = left_right_single_view_param.result_image_height;
		result_image_width = left_right_single_view_param.result_image_width;
		
	}
	if (cam_flag == _1_VIEW_LEFT_RIGHT)//左右拼接
	{
		result_image_height = p_avm_param->p_avm_config_param
			->avm_single_view_param.left_right_view_param.result_image_height;
		result_image_width = p_avm_param->p_avm_config_param
			->avm_single_view_param.left_right_view_param.result_image_width;
		size0 = cvSize(result_image_width, result_image_height);//
		p_lut = p_avm_param->p_avm_lut->p_lut_single_view_left_right;
	}
	IplImage* image[4] = { NULL, NULL, NULL, NULL };
	my_load_image(image, p_avm_param);
	//CHECK_ERROR(ret);
	IplImage *image_result_single_front_lut = cvCreateImage(size0, IPL_DEPTH_8U, 1);
	memset(image_result_single_front_lut->imageData, 0, result_image_width*result_image_height);
	Float32_t image_point0[2];
	Int32_t Front_height = Int32_t(ceil(result_image_height / 32.00) * 32);
	Int32_t Front_width = Int32_t(ceil(result_image_width / 32.00) * 32);

	Int32_t Back_height = Int32_t(ceil(result_image_height / 32.00) * 32);
	Int32_t Back_width = Int32_t(ceil(result_image_width / 32.00) * 32);

	Int32_t Left_height = Int32_t(ceil(result_image_width / 32.00) * 32);
	Int32_t Left_width = Int32_t(ceil(result_image_height / 32.00) * 32);
	Int32_t Right_height = Int32_t(ceil(result_image_width / 32.00) * 32);
	Int32_t Right_width = Int32_t(ceil(result_image_height / 32.00) * 32);
	if (cam_flag == _1_VIEW_LEFT_RIGHT)//左右拼接
	{
		Left_height = result_image_width / 2;//左右拼接的高宽
		Left_width = result_image_height;
		Right_height = result_image_width / 2;
		Right_width = result_image_height;
	}
	Float32_t* Amba_Lut_Front_x = (Float32_t*)malloc(Front_height* Front_width*sizeof(Float32_t));
	Float32_t* Amba_Lut_Front_y = (Float32_t*)malloc(Front_height* Front_width*sizeof(Float32_t));

	Float32_t* Amba_Lut_Back_x = (Float32_t*)malloc(Back_height* Back_width*sizeof(Float32_t));
	Float32_t* Amba_Lut_Back_y = (Float32_t*)malloc(Back_height* Back_width*sizeof(Float32_t));

	Float32_t* Amba_Lut_Left_x = (Float32_t*)malloc(Left_height *Left_width*sizeof(Float32_t));
	Float32_t* Amba_Lut_Left_y = (Float32_t*)malloc(Left_height *Left_width*sizeof(Float32_t));

	Float32_t* Amba_Lut_Right_x = (Float32_t*)malloc(Right_height* Right_width*sizeof(Float32_t));
	Float32_t* Amba_Lut_Right_y = (Float32_t*)malloc(Right_height* Right_width*sizeof(Float32_t));
	CvSize size1;
	IplImage *image_amplify;
	

	size1 = cvSize(result_image_width, result_image_height);
	image_amplify = cvCreateImage(size1, IPL_DEPTH_8U, 1);
	memset(image_amplify->imageData, 0, Right_width*Right_height);
	for (int i = 0; i <result_image_height; i++)//这里的i j是按照结果图去扫描的
	{
		for (int j = 0; j < result_image_width; j++)
		{
			int shift = i * result_image_width * 2 + j * 2;
			int rgb[3];
			Amba_resolve_data_from_lut(shift, image[3], rgb, p_avm_param, image_point0, p_lut);

			if (cam_flag == _1_VIEW_FRONT)
			{
				Amba_Lut_Front_x[i*Front_width + j] = image_point0[1];
				Amba_Lut_Front_y[i*Front_width + j] = image_point0[0];
			}
			if (cam_flag == _1_VIEW_REAR)
			{
				if (p_avm_param->project_flag == HDMI)//CVBS是倒装后镜头，HDMI是正装镜头所以要加一步反转
				{
					Amba_Lut_Back_x[i * Back_width + (result_image_width - j - 1)] = image_point0[1];
					Amba_Lut_Back_y[i * Back_width + (result_image_width - j - 1)] = image_point0[0];
				}
				if (p_avm_param->project_flag == CVBS)//CVBS是倒装后镜头，HDMI是正装镜头所以要加一步反转
				{
					Amba_Lut_Back_x[(Back_height - i - 1)*Back_width + (result_image_width - j - 1)] = image_point0[1];
					Amba_Lut_Back_y[(Back_height - i - 1)*Back_width + (result_image_width - j - 1)] = image_point0[0];
				}
			}
			if (cam_flag == _1_VIEW_LEFT)
			{
				Amba_Lut_Left_x[j*Left_width + (Left_width -1- i)] = image_point0[1];
				Amba_Lut_Left_y[j*Left_width + (Left_width -1- i)] = image_point0[0];
			}
			if (cam_flag == _1_VIEW_RIGHT)
			{
				Amba_Lut_Right_x[(Right_height - 1 - j)*Right_width + i] = image_point0[1];
				Amba_Lut_Right_y[(Right_height - 1 - j)*Right_width + i] = image_point0[0];
			}

			if (cam_flag == _1_VIEW_LEFT_RIGHT)//处理左右拼接的代码
			{
				if (j<result_image_width / 2)//左边的
				{
					Amba_Lut_Left_x[j* Left_width + (Left_width - 1 - i)] = image_point0[1];
					Amba_Lut_Left_y[j* Left_width + (Left_width - 1 - i)] = image_point0[0];
					
				}
				if (j >= result_image_width / 2)//右边的
				{
					image_result_single_front_lut->imageData[i *result_image_width + j] = (uchar)(rgb[0]);
					Amba_Lut_Right_x[(Right_height - 1 - (j - result_image_width / 2))* Right_width + i] = image_point0[1];
					Amba_Lut_Right_y[(Right_height - 1 - (j - result_image_width / 2))* Right_width + i] = image_point0[0];
				}
			}
		}
	}


	Int32_t  amplify_width, amplify_height;
	
	if (cam_flag == _1_VIEW_REAR || cam_flag == _1_VIEW_FRONT)//为了与 
	{
		amplify_width = Front_width;
		amplify_height = Front_height;
	}
	if (cam_flag == _1_VIEW_LEFT || cam_flag == _1_VIEW_RIGHT || cam_flag == _1_VIEW_LEFT_RIGHT)
	{
		amplify_width = Left_width;
		amplify_height = Left_height;
	}
	/************************/
#if 1
	//生成81*61向量表,水平方向与垂直方向
	for (Int32_t i = 0; i < 61; i++)
	{
		for (Int32_t j = 0; j < 81; j++)
		{
			Float32_t x_coord, y_coord;
			Int32_t ver_grid_index = i * 16;
			Int32_t hor_grid_index = j * 16;
			ver_grid_index = ver_grid_index >= 959 ? 959 : ver_grid_index;
			hor_grid_index = hor_grid_index >= 1279 ? 1279 : hor_grid_index;
			//front
			Float32_t temp_x = (Float32_t)(ver_grid_index) / (1.0 * Amba_Vector_Height / amplify_height);
			Float32_t temp_y = (Float32_t)(hor_grid_index) / (1.0 * Amba_Vector_Width / amplify_width);
			Int32_t x0 = (Int32_t)temp_x;
			Int32_t x1 = (Int32_t)temp_x + 1;
			Int32_t y0 = (Int32_t)temp_y;
			Int32_t y1 = (Int32_t)temp_y + 1;
			x1 = x1 >= amplify_height ? (amplify_height - 1) : x1;
			y1 = y1 >= amplify_width ? (amplify_width - 1) : y1;
			temp_x = temp_x > x1 ? x1 : temp_x;
			temp_y = temp_y > y1 ? y1 : temp_y;
			Float32_t weightUpLeft = (x1 - temp_x) * (y1 - temp_y);
			Float32_t weightDownLeft = (temp_x - x0) * (y1 - temp_y);
			Float32_t weightUpRight = (x1 - temp_x) * (temp_y - y0);
			Float32_t weightDownRight = 1 - weightUpLeft - weightUpRight - weightDownLeft;
			if (cam_flag == _1_VIEW_FRONT)//前单视图
			{
				x_coord = weightUpLeft * Amba_Lut_Front_x[x0*Front_width + y0]
					+ weightUpRight * Amba_Lut_Front_x[x0*Front_width + y1]
					+ weightDownLeft * Amba_Lut_Front_x[x1*Front_width + y0]
					+ weightDownRight * Amba_Lut_Front_x[x1*Front_width + y1];
				y_coord = weightUpLeft * Amba_Lut_Front_y[x0*Front_width + y0]
					+ weightUpRight * Amba_Lut_Front_y[x0*Front_width + y1]
					+ weightDownLeft * Amba_Lut_Front_y[x1*Front_width + y0]
					+ weightDownRight * Amba_Lut_Front_y[x1*Front_width + y1];
				Amba_front[2 * (81 * i + j)] =
					(Int16_t)((x_coord - 16 * j) * 16);
				Amba_front[2 * (81 * i + j) + 1] =
					(Int16_t)((y_coord - 16 * i) * 16);
			}
			//back///////////////////////////
			
			if (cam_flag == _1_VIEW_REAR)//后单视图
			{
				x_coord = weightUpLeft * Amba_Lut_Back_x[x0*Back_width + y0]
					+ weightUpRight * Amba_Lut_Back_x[x0*Back_width + y1]
					+ weightDownLeft * Amba_Lut_Back_x[x1*Back_width + y0]
					+ weightDownRight * Amba_Lut_Back_x[x1*Back_width + y1];
				y_coord = weightUpLeft * Amba_Lut_Back_y[x0*Back_width + y0]
					+ weightUpRight * Amba_Lut_Back_y[x0*Back_width + y1]
					+ weightDownLeft * Amba_Lut_Back_y[x1*Back_width + y0]
					+ weightDownRight * Amba_Lut_Back_y[x1*Back_width + y1];
				Amba_back[2 * (81 * i + j)] =
					(Int16_t)((x_coord - 16 * j) * 16);
				Amba_back[2 * (81 * i + j) + 1] =
					(Int16_t)((y_coord - 16 * i) * 16);
			}
			//left
			if (cam_flag == _1_VIEW_LEFT || cam_flag == _1_VIEW_LEFT_RIGHT)//左单视图
			{
				x_coord = weightUpLeft * Amba_Lut_Left_x[x0*Left_width + y0]
					+ weightUpRight * Amba_Lut_Left_x[x0*Left_width + y1]
					+ weightDownLeft * Amba_Lut_Left_x[x1*Left_width + y0]
					+ weightDownRight * Amba_Lut_Left_x[x1*Left_width + y1];
				y_coord = weightUpLeft * Amba_Lut_Left_y[x0*Left_width + y0]
					+ weightUpRight * Amba_Lut_Left_y[x0*Left_width + y1]
					+ weightDownLeft * Amba_Lut_Left_y[x1*Left_width + y0]
					+ weightDownRight * Amba_Lut_Left_y[x1*Left_width + y1];
				Amba_left[2 * (81 * i + j)] =
					(Int16_t)((x_coord - 16 * j) * 16);
				Amba_left[2 * (81 * i + j) + 1] =
					(Int16_t)((y_coord - 16 * i) * 16);
			}
			//right
			if (cam_flag == _1_VIEW_RIGHT || cam_flag == _1_VIEW_LEFT_RIGHT)//右单视图
			{
				x_coord = weightUpLeft * Amba_Lut_Right_x[x0*Right_width + y0]
					+ weightUpRight * Amba_Lut_Right_x[x0*Right_width + y1]
					+ weightDownLeft * Amba_Lut_Right_x[x1*Right_width + y0]
					+ weightDownRight * Amba_Lut_Right_x[x1*Right_width + y1];
				y_coord = weightUpLeft * Amba_Lut_Right_y[x0*Right_width + y0]
					+ weightUpRight * Amba_Lut_Right_y[x0*Right_width + y1]
					+ weightDownLeft * Amba_Lut_Right_y[x1*Right_width + y0]
					+ weightDownRight * Amba_Lut_Right_y[x1*Right_width + y1];
				Amba_right[2 * (81 * i + j)] =
					(Int16_t)((x_coord - 16 * j) * 16);
				Amba_right[2 * (81 * i + j) + 1] =
					(Int16_t)((y_coord - 16 * i) * 16);
			}
		}
	}
#endif
#if 1
	
		//更改calib17.bin数据区
		sprintf(lut_path, "%s/calib17.bin",
			lut_file_path);
		pFile = fopen(lut_path, "rb+");
		if (!pFile)
		{
			printf("安霸表17和21这两个表不存在 %s cannot be opened\n", lut_path);
			CHECK_ERROR(error);
		}
		if (cam_flag == _1_VIEW_LEFT_RIGHT)//专门用于左右拼接的
		{
			for (Int32_t cam_id = 2; cam_id < CAMERA_NUM; cam_id++)
			{
				int target_pos = 94720 * view_id + 2624 + 23024 * cam_id + 64;
				switch (cam_id)
				{
				case 0:
					fseek(pFile, target_pos, SEEK_SET);
					fwrite(Amba_front, sizeof(Int16_t), 61 * 81 * 2, pFile);
					
					break;
				case 1:
					fseek(pFile, target_pos, SEEK_SET);
					fwrite(Amba_back, sizeof(Int16_t), 61 * 81 * 2, pFile);
					break;
				case 2:
					fseek(pFile, target_pos, SEEK_SET);
					fwrite(Amba_left, sizeof(Int16_t), 61 * 81 * 2, pFile);
					break;
				case 3:
					fseek(pFile, target_pos, SEEK_SET);
					fwrite(Amba_right, sizeof(Int16_t), 61 * 81 * 2, pFile);
					break;
				default:
					break;
				}
			}
		}
		else
		{
			FILE* pFile_2_5;
			//填充总的总的大表
			int target_pos = 94720 * view_id + 2624 + 23024 * (cam_flag-4) + 64;
			switch (cam_flag - 4)
			{
			case 0:
				//generate_calib17_fov_header(view_id, cam_flag, pFile);
				fseek(pFile, target_pos, SEEK_SET);
				fwrite(Amba_front, sizeof(Int16_t), 61 * 81 * 2, pFile);
				///*********2.5*******/
				sprintf(lut_path, "%s/2.5/2D_FrontSingle_RawEncode.bin",
					lut_file_path);
				pFile_2_5 = fopen(lut_path, "wb");
				fwrite(Amba_front, sizeof(Int16_t), 61 * 81 * 2, pFile_2_5);
				break;
			case 1:
				//generate_calib17_fov_header(view_id, cam_flag, pFile);
				fseek(pFile, target_pos, SEEK_SET);
				fwrite(Amba_back, sizeof(Int16_t), 61 * 81 * 2, pFile);

				/*********2.5*******/
				sprintf(lut_path, "%s/2.5/2D_BackSingle_RawEncode.bin",
					lut_file_path);
				pFile_2_5 = fopen(lut_path, "wb");
				fwrite(Amba_back, sizeof(Int16_t), 61 * 81 * 2, pFile_2_5);
				break;
			case 2:
				//generate_calib17_fov_header(view_id, cam_flag, pFile);
				fseek(pFile, target_pos, SEEK_SET);
				fwrite(Amba_left, sizeof(Int16_t), 61 * 81 * 2, pFile);

				/*********2.5*******/
				sprintf(lut_path, "%s/2.5/2D_LeftSingle_RawEncode.bin",
					lut_file_path);
				pFile_2_5 = fopen(lut_path, "wb");
				fwrite(Amba_left, sizeof(Int16_t), 61 * 81 * 2, pFile_2_5);


				break;
			case 3:
				//generate_calib17_fov_header(view_id, cam_flag, pFile);
				fseek(pFile, target_pos, SEEK_SET);
				fwrite(Amba_right, sizeof(Int16_t), 61 * 81 * 2, pFile);

				/*********2.5*******/
				sprintf(lut_path, "%s/2.5/2D_RightSingle_RawEncode.bin",
					lut_file_path);
				pFile_2_5 = fopen(lut_path, "wb");
				fwrite(Amba_right, sizeof(Int16_t), 61 * 81 * 2, pFile_2_5);
				break;
			default:
				break;
			}
			fclose(pFile_2_5);
		}
		fclose(pFile);
		
#endif
		return error;
	
}


#endif
/**************write_calib21***********************/
/*
* Function Name:				 write_calib21
* Function Function:			write_calib21
*       p_avm_param;
* Note:
*     write_calib21
* Revision:
*     Create by Cheng.Li on 2018/5/2.
*/



/******


******/
void write_calib21(IN P_Avm_Param_S p_avm_param,FILE* pFile, int view_id, Int32_t Left_height,
	Int32_t Left_width, Int32_t Right_height, Int32_t Right_width
	, uchar *Amba_Lut_Left_weight, uchar *Amba_Lut_Right_weight)
{
	if (p_avm_param->project_flag == HDMI)
	{
		//HDMI
		int target_pos;
		for (Int32_t cam_id = 2; cam_id < CAMERA_NUM; cam_id++)
		{
			target_pos = 4194816 * view_id + 1048576 * (cam_id - 2) + 4;//
			switch (cam_id)
			{
			case 2:
				fseek(pFile, target_pos, SEEK_SET);
				fwrite(Amba_Lut_Left_weight, sizeof(uchar), Left_height *Left_width, pFile);
				break;
			case 3:
				fseek(pFile, target_pos, SEEK_SET);
				fwrite(Amba_Lut_Right_weight, sizeof(uchar), Right_height *Right_width, pFile);
				break;
			default:
				break;
			}
		}
	}if (p_avm_param->project_flag == CVBS)
	{
		//CVBS
		int target_pos = 1382912 * view_id;//
		fseek(pFile, target_pos, SEEK_SET);
		int enable = 1;//写入使能
		fwrite(&enable, sizeof(int), 1, pFile);
		for (Int32_t cam_id = 2; cam_id < CAMERA_NUM; cam_id++)
		{
			target_pos = 1382912 * view_id + 345600 * (cam_id - 2) + 4;//
			switch (cam_id)
			{
			case 2:
				fseek(pFile, target_pos, SEEK_SET);
				fwrite(Amba_Lut_Left_weight, sizeof(uchar), Left_height *Left_width, pFile);
				break;
			case 3:

				fseek(pFile, target_pos, SEEK_SET);
				fwrite(Amba_Lut_Right_weight, sizeof(uchar), Right_height *Right_width, pFile);
				break;
			default:
				break;
			}
		}
	}
}






