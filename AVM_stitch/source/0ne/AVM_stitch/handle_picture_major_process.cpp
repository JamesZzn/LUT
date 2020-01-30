#include"handle_picture_major_process.h"
#include "generate_4_view_lut.h"
#include "generate_3_view_lut.h"
#include "single_view.h"
#include "resolve_data.h"
#include"draw_static_dynamic_line.h"


/**************handle_3_4_view***********************/
/*
* Function Name:				handle_3_4_view
* Function Function:			handle_3_4_view
* Input:
*       p_avm_param;
* Note:
*     the interface function should be called at the beginning of this module
* Revision:
*     Create by Cheng.Li on 2018/5/2.
*/
AVM_Error_Code_E handle_3_4_view(IN P_Avm_Param_S p_avm_param)
{
	AVM_Error_Code_E ret = ERROR_NONE;
	/******生成大图的表*****/
	ret = generate_4_view_lut(
		 p_avm_param, p_avm_param->p_avm_config_param->avm_2d_param.junc_view_4_param.result_big_param,
		 p_avm_param->p_avm_lut->p_lut_junc_view_4_big);
	CHECK_ERROR(ret);

	/******生成小图的表*****/
	ret = generate_4_view_lut(p_avm_param, p_avm_param->p_avm_config_param->avm_2d_param.junc_view_4_param.result_small_param
		, p_avm_param->p_avm_lut->p_lut_junc_view_4_small);
	CHECK_ERROR(ret);
#if 1
	/******生成亮度平衡的表*****/
	ret = generate_4_view_lut(p_avm_param, p_avm_param->p_avm_config_param->avm_2d_param.awb_view_param.result_awb_param
		, p_avm_param->p_avm_lut->p_lut_awb);
#endif
	/******生成前3路视图的表*****/
	ret = generate_3_front_view_lut(p_avm_param);
	CHECK_ERROR(ret);
	/******生成后3路视图*****/
	ret = generate_3_rear_view_lut(p_avm_param);
	CHECK_ERROR(ret);


	return ret;
}

/**************handle_front_rear_single_vieww***********************/
/*
* Function Name:				handle_front_rear_single_view
* Function Function:			handle_front_rear_single_view
* Input:
*       p_avm_param;
* Note:
*     the interface function should be called at the beginning of this module
* Revision:
*     Create by Cheng.Li on 2018/5/2.
*/
AVM_Error_Code_E handle_front_rear_single_view(IN P_Avm_Param_S p_avm_param)
{
	AVM_Error_Code_E ret = ERROR_NONE;
	/**********front and  rear**********/
	Single_View_Expand_E expand_type = p_avm_param->p_avm_config_param->avm_single_view_param.front_single_view_param.expand_type;
	switch (expand_type)
	{
		case TYPE_CROP:
			ret = crop_front_rear_single_view(p_avm_param, CAM_FRONT);//前视图原图裁剪
			CHECK_ERROR(ret);
			break;
		case TYPE_PLANE_EXPAND:
			ret = plane_front_rear_single_view(p_avm_param, CAM_FRONT);//前视图平面展开   
			CHECK_ERROR(ret);
			break;
		case TYPE_BOX_EXPAND:
			ret = box_front_rear_single_view(p_avm_param, CAM_FRONT);//前视图盒子展开
			CHECK_ERROR(ret);
			break;

			
	}
#if 0
	expand_type = p_avm_param->p_avm_config_param->avm_single_view_param.rear_single_view_param.expand_type;
	switch (expand_type)
	{
			case TYPE_CROP:
				ret = crop_front_rear_single_view(p_avm_param, CAM_REAR);//后原图裁剪
				CHECK_ERROR(ret);
				break;
			case TYPE_PLANE_EXPAND:
				//ret = plane_front_rear_single_view(p_avm_param, CAM_REAR); //后平面展开
				CHECK_ERROR(ret);
				break;
			case TYPE_BOX_EXPAND:
				ret = box_front_rear_single_view(p_avm_param, CAM_REAR);//后盒子展开
				CHECK_ERROR(ret);
				break;
	}
#endif
	return ret;
}


/************** handle_left_right_single_view***********************/
/*
* Function Name:				 handle_left_right_single_view
* Function Function:			 handle_left_right_single_view
*       p_avm_param;
* Note:
*     the interface function should be called at the beginning of this module
* Revision:
*     Create by Cheng.Li on 2018/5/2.
*/
AVM_Error_Code_E handle_left_right_single_view(IN P_Avm_Param_S p_avm_param)
{
	AVM_Error_Code_E ret = ERROR_NONE;
	/*********左视图**********/
	Single_View_Expand_E expand_type = p_avm_param->p_avm_config_param->avm_single_view_param.left_single_view_param.expand_type;//
	switch (expand_type)
	{
		case TYPE_CROP:
			ret = left_right_crop_expend(p_avm_param, CAM_LEFT);//原图裁剪
			CHECK_ERROR(ret);
			break;
		case TYPE_PLANE_EXPAND:
			ret = left_right_overlook_expend(p_avm_param, CAM_LEFT);//俯视图展开
			CHECK_ERROR(ret);
			break;
		case TYPE_BOX_EXPAND://为了方便伪3D展开叫盒子展开
			ret = left_right_combine_overlook_expend(p_avm_param, CAM_LEFT);//柱面展开
			CHECK_ERROR(ret);
			break;
		case TYPE_COMBINE_EXPAND:
			
			break;

	}
	/******右视图*****/
	expand_type = p_avm_param->p_avm_config_param->avm_single_view_param.right_single_view_param.expand_type;
	switch (expand_type)
	{
		case TYPE_CROP:
			ret = left_right_crop_expend(p_avm_param, CAM_RIGHT);//原图裁剪
			CHECK_ERROR(ret);
			break;
		case TYPE_PLANE_EXPAND:
			ret = left_right_overlook_expend(p_avm_param, CAM_RIGHT);//俯视图展开
			CHECK_ERROR(ret);
			break;
		case TYPE_BOX_EXPAND://柱面展开
			ret = left_right_combine_overlook_expend(p_avm_param, CAM_RIGHT);
			CHECK_ERROR(ret);
			break;
		case TYPE_COMBINE_EXPAND:
			break;
	}

	/******左右拼接图*************/
	expand_type = p_avm_param->p_avm_config_param->avm_single_view_param.left_right_view_param.expand_type;
	switch (expand_type)
	{
		case TYPE_CROP:

		case TYPE_PLANE_EXPAND:
			ret = lr_single_overlook(p_avm_param);//左右拼接
			CHECK_ERROR(ret);
			break;
		case TYPE_BOX_EXPAND://为了方便伪3D展开叫盒子展开
			ret = lr_combine_overlook_expend(p_avm_param);
			CHECK_ERROR(ret);
			break;
		

	}



	return ret;
}


/**************handle_static_dynamic_line***********************/
/*
* Function Name:				 handle_static_dynamic_line
* Function Function:			handle_static_dynamic_line
*       p_avm_param;
* Note:
*     the interface function should be called at the beginning of this module
* Revision:
*     Create by Cheng.Li on 2018/5/2.
*/
AVM_Error_Code_E handle_static_dynamic_line(IN P_Avm_Param_S p_avm_param)
{
	AVM_Error_Code_E ret = ERROR_NONE;
	bool	is_static_line_exist;
	bool	is_dynamic_line_exist;
	

	/******前单视图的静态线********/
	is_static_line_exist = p_avm_param->p_avm_config_param
		->avm_single_view_param.front_single_view_param.dis_line_param.static_line_param.is_static_line_exist;
	if(is_static_line_exist)//判断静态线是否需要
	{
		ret = draw_static_plane_front_rear_single_view_line(p_avm_param, CAM_FRONT);
		CHECK_ERROR(ret);
	}

	is_dynamic_line_exist = p_avm_param->p_avm_config_param
		->avm_single_view_param.front_single_view_param.dis_line_param.dynamic_line_param.is_dynamic_line_exist;
	if (is_dynamic_line_exist)
	{
		ret = draw_dynamic_plane_front_rear_single_view_line(p_avm_param, CAM_FRONT);
		CHECK_ERROR(ret);
	}
#if 0
	/******后单视图的静态线********/
	is_static_line_exist = p_avm_param->p_avm_config_param
		->avm_single_view_param.rear_single_view_param.dis_line_param.static_line_param.is_static_line_exist;
	if (is_static_line_exist)//判断静态线是否需要
	{
		ret = draw_static_plane_front_rear_single_view_line(p_avm_param, CAM_REAR);
		CHECK_ERROR(ret);
	}


	/******前单视图的动态线********/
	is_dynamic_line_exist = p_avm_param->p_avm_config_param
		->avm_single_view_param.front_single_view_param.dis_line_param.dynamic_line_param.is_dynamic_line_exist;
	if (is_dynamic_line_exist)
	{
		ret = draw_dynamic_plane_front_rear_single_view_line(p_avm_param, CAM_FRONT);
		CHECK_ERROR(ret);
	}


	/******后单视图的动态线********/
	is_dynamic_line_exist = p_avm_param->p_avm_config_param
		->avm_single_view_param.rear_single_view_param.dis_line_param.dynamic_line_param.is_dynamic_line_exist;
	if (is_dynamic_line_exist)
	{
		ret = draw_dynamic_plane_front_rear_single_view_line(p_avm_param, CAM_REAR);
		CHECK_ERROR(ret);
	}

	/******4 view********/
	/******4路环视图的静态线********/
	/*big*/
	is_static_line_exist = p_avm_param->p_avm_config_param->
				avm_2d_param.junc_view_4_param.big_view_dis_line_param.static_line_param.is_static_line_exist;
	if (is_static_line_exist)
	{
		ret = draw_static_line_4_view(p_avm_param, 0);//0是big
		CHECK_ERROR(ret);
	}
	/*small*/
	is_static_line_exist = p_avm_param->p_avm_config_param
		->avm_2d_param.junc_view_4_param.small_view_dis_line_param.static_line_param.is_static_line_exist;
	if (is_static_line_exist)
	{
		ret = draw_static_line_4_view(p_avm_param, 1);//0是big
		CHECK_ERROR(ret);
	}
	///******4路环视图的动态线********/
	/*big*/
	is_dynamic_line_exist = p_avm_param->p_avm_config_param
		->avm_2d_param.junc_view_4_param.big_view_dis_line_param.dynamic_line_param.is_dynamic_line_exist;
	if (is_dynamic_line_exist)
	{
		ret = draw_dynamic_4_view_line(p_avm_param, 0);//0是big
		CHECK_ERROR(ret);
	}
	/*small*/
	is_dynamic_line_exist = p_avm_param->p_avm_config_param
		->avm_2d_param.junc_view_4_param.small_view_dis_line_param.dynamic_line_param.is_dynamic_line_exist;
	if (is_dynamic_line_exist)
	{
		ret = draw_dynamic_4_view_line(p_avm_param, 1);//1是small
		CHECK_ERROR(ret);
	}





	/******3 view********/
	/******3路环视图的静态线********/
	
	is_static_line_exist = p_avm_param->p_avm_config_param->
		avm_2d_param.junc_view_3_front_param.junc_3_front_dis_line_param.static_line_param.is_static_line_exist;
	if (is_static_line_exist)
	{
		ret = draw_static_line_3_view(p_avm_param, 0);//0标志是前三路
		CHECK_ERROR(ret);
	}

	is_static_line_exist = p_avm_param->p_avm_config_param->
		avm_2d_param.junc_view_3_rear_param.junc_3_rear_dis_line_param.static_line_param.is_static_line_exist;
	if (is_static_line_exist)
	{
		ret = draw_static_line_3_view(p_avm_param, 1);//1标识是后三路
		CHECK_ERROR(ret);
	}





	///******3路环视图的动态线********/
	
	is_dynamic_line_exist = p_avm_param->p_avm_config_param
		->avm_2d_param.junc_view_3_front_param.junc_3_front_dis_line_param.dynamic_line_param.is_dynamic_line_exist;
	if (is_dynamic_line_exist)
	{
		ret = draw_dynamic_3_view_line(p_avm_param, 0);
		CHECK_ERROR(ret);
	}
	
#if 1
	///******3路环视图的动态线********/
	is_dynamic_line_exist = p_avm_param->p_avm_config_param
		->avm_2d_param.junc_view_3_rear_param.junc_3_rear_dis_line_param.dynamic_line_param.is_dynamic_line_exist;
	if (is_dynamic_line_exist)
	{
		ret = draw_dynamic_3_view_line(p_avm_param, 1);
		CHECK_ERROR(ret);
	}
#endif
#endif


}

