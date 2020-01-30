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
	/******���ɴ�ͼ�ı�*****/
	ret = generate_4_view_lut(
		 p_avm_param, p_avm_param->p_avm_config_param->avm_2d_param.junc_view_4_param.result_big_param,
		 p_avm_param->p_avm_lut->p_lut_junc_view_4_big);
	CHECK_ERROR(ret);

	/******����Сͼ�ı�*****/
	ret = generate_4_view_lut(p_avm_param, p_avm_param->p_avm_config_param->avm_2d_param.junc_view_4_param.result_small_param
		, p_avm_param->p_avm_lut->p_lut_junc_view_4_small);
	CHECK_ERROR(ret);
#if 1
	/******��������ƽ��ı�*****/
	ret = generate_4_view_lut(p_avm_param, p_avm_param->p_avm_config_param->avm_2d_param.awb_view_param.result_awb_param
		, p_avm_param->p_avm_lut->p_lut_awb);
#endif
	/******����ǰ3·��ͼ�ı�*****/
	ret = generate_3_front_view_lut(p_avm_param);
	CHECK_ERROR(ret);
	/******���ɺ�3·��ͼ*****/
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
			ret = crop_front_rear_single_view(p_avm_param, CAM_FRONT);//ǰ��ͼԭͼ�ü�
			CHECK_ERROR(ret);
			break;
		case TYPE_PLANE_EXPAND:
			ret = plane_front_rear_single_view(p_avm_param, CAM_FRONT);//ǰ��ͼƽ��չ��   
			CHECK_ERROR(ret);
			break;
		case TYPE_BOX_EXPAND:
			ret = box_front_rear_single_view(p_avm_param, CAM_FRONT);//ǰ��ͼ����չ��
			CHECK_ERROR(ret);
			break;

			
	}
#if 0
	expand_type = p_avm_param->p_avm_config_param->avm_single_view_param.rear_single_view_param.expand_type;
	switch (expand_type)
	{
			case TYPE_CROP:
				ret = crop_front_rear_single_view(p_avm_param, CAM_REAR);//��ԭͼ�ü�
				CHECK_ERROR(ret);
				break;
			case TYPE_PLANE_EXPAND:
				//ret = plane_front_rear_single_view(p_avm_param, CAM_REAR); //��ƽ��չ��
				CHECK_ERROR(ret);
				break;
			case TYPE_BOX_EXPAND:
				ret = box_front_rear_single_view(p_avm_param, CAM_REAR);//�����չ��
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
	/*********����ͼ**********/
	Single_View_Expand_E expand_type = p_avm_param->p_avm_config_param->avm_single_view_param.left_single_view_param.expand_type;//
	switch (expand_type)
	{
		case TYPE_CROP:
			ret = left_right_crop_expend(p_avm_param, CAM_LEFT);//ԭͼ�ü�
			CHECK_ERROR(ret);
			break;
		case TYPE_PLANE_EXPAND:
			ret = left_right_overlook_expend(p_avm_param, CAM_LEFT);//����ͼչ��
			CHECK_ERROR(ret);
			break;
		case TYPE_BOX_EXPAND://Ϊ�˷���α3Dչ���к���չ��
			ret = left_right_combine_overlook_expend(p_avm_param, CAM_LEFT);//����չ��
			CHECK_ERROR(ret);
			break;
		case TYPE_COMBINE_EXPAND:
			
			break;

	}
	/******����ͼ*****/
	expand_type = p_avm_param->p_avm_config_param->avm_single_view_param.right_single_view_param.expand_type;
	switch (expand_type)
	{
		case TYPE_CROP:
			ret = left_right_crop_expend(p_avm_param, CAM_RIGHT);//ԭͼ�ü�
			CHECK_ERROR(ret);
			break;
		case TYPE_PLANE_EXPAND:
			ret = left_right_overlook_expend(p_avm_param, CAM_RIGHT);//����ͼչ��
			CHECK_ERROR(ret);
			break;
		case TYPE_BOX_EXPAND://����չ��
			ret = left_right_combine_overlook_expend(p_avm_param, CAM_RIGHT);
			CHECK_ERROR(ret);
			break;
		case TYPE_COMBINE_EXPAND:
			break;
	}

	/******����ƴ��ͼ*************/
	expand_type = p_avm_param->p_avm_config_param->avm_single_view_param.left_right_view_param.expand_type;
	switch (expand_type)
	{
		case TYPE_CROP:

		case TYPE_PLANE_EXPAND:
			ret = lr_single_overlook(p_avm_param);//����ƴ��
			CHECK_ERROR(ret);
			break;
		case TYPE_BOX_EXPAND://Ϊ�˷���α3Dչ���к���չ��
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
	

	/******ǰ����ͼ�ľ�̬��********/
	is_static_line_exist = p_avm_param->p_avm_config_param
		->avm_single_view_param.front_single_view_param.dis_line_param.static_line_param.is_static_line_exist;
	if(is_static_line_exist)//�жϾ�̬���Ƿ���Ҫ
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
	/******����ͼ�ľ�̬��********/
	is_static_line_exist = p_avm_param->p_avm_config_param
		->avm_single_view_param.rear_single_view_param.dis_line_param.static_line_param.is_static_line_exist;
	if (is_static_line_exist)//�жϾ�̬���Ƿ���Ҫ
	{
		ret = draw_static_plane_front_rear_single_view_line(p_avm_param, CAM_REAR);
		CHECK_ERROR(ret);
	}


	/******ǰ����ͼ�Ķ�̬��********/
	is_dynamic_line_exist = p_avm_param->p_avm_config_param
		->avm_single_view_param.front_single_view_param.dis_line_param.dynamic_line_param.is_dynamic_line_exist;
	if (is_dynamic_line_exist)
	{
		ret = draw_dynamic_plane_front_rear_single_view_line(p_avm_param, CAM_FRONT);
		CHECK_ERROR(ret);
	}


	/******����ͼ�Ķ�̬��********/
	is_dynamic_line_exist = p_avm_param->p_avm_config_param
		->avm_single_view_param.rear_single_view_param.dis_line_param.dynamic_line_param.is_dynamic_line_exist;
	if (is_dynamic_line_exist)
	{
		ret = draw_dynamic_plane_front_rear_single_view_line(p_avm_param, CAM_REAR);
		CHECK_ERROR(ret);
	}

	/******4 view********/
	/******4·����ͼ�ľ�̬��********/
	/*big*/
	is_static_line_exist = p_avm_param->p_avm_config_param->
				avm_2d_param.junc_view_4_param.big_view_dis_line_param.static_line_param.is_static_line_exist;
	if (is_static_line_exist)
	{
		ret = draw_static_line_4_view(p_avm_param, 0);//0��big
		CHECK_ERROR(ret);
	}
	/*small*/
	is_static_line_exist = p_avm_param->p_avm_config_param
		->avm_2d_param.junc_view_4_param.small_view_dis_line_param.static_line_param.is_static_line_exist;
	if (is_static_line_exist)
	{
		ret = draw_static_line_4_view(p_avm_param, 1);//0��big
		CHECK_ERROR(ret);
	}
	///******4·����ͼ�Ķ�̬��********/
	/*big*/
	is_dynamic_line_exist = p_avm_param->p_avm_config_param
		->avm_2d_param.junc_view_4_param.big_view_dis_line_param.dynamic_line_param.is_dynamic_line_exist;
	if (is_dynamic_line_exist)
	{
		ret = draw_dynamic_4_view_line(p_avm_param, 0);//0��big
		CHECK_ERROR(ret);
	}
	/*small*/
	is_dynamic_line_exist = p_avm_param->p_avm_config_param
		->avm_2d_param.junc_view_4_param.small_view_dis_line_param.dynamic_line_param.is_dynamic_line_exist;
	if (is_dynamic_line_exist)
	{
		ret = draw_dynamic_4_view_line(p_avm_param, 1);//1��small
		CHECK_ERROR(ret);
	}





	/******3 view********/
	/******3·����ͼ�ľ�̬��********/
	
	is_static_line_exist = p_avm_param->p_avm_config_param->
		avm_2d_param.junc_view_3_front_param.junc_3_front_dis_line_param.static_line_param.is_static_line_exist;
	if (is_static_line_exist)
	{
		ret = draw_static_line_3_view(p_avm_param, 0);//0��־��ǰ��·
		CHECK_ERROR(ret);
	}

	is_static_line_exist = p_avm_param->p_avm_config_param->
		avm_2d_param.junc_view_3_rear_param.junc_3_rear_dis_line_param.static_line_param.is_static_line_exist;
	if (is_static_line_exist)
	{
		ret = draw_static_line_3_view(p_avm_param, 1);//1��ʶ�Ǻ���·
		CHECK_ERROR(ret);
	}





	///******3·����ͼ�Ķ�̬��********/
	
	is_dynamic_line_exist = p_avm_param->p_avm_config_param
		->avm_2d_param.junc_view_3_front_param.junc_3_front_dis_line_param.dynamic_line_param.is_dynamic_line_exist;
	if (is_dynamic_line_exist)
	{
		ret = draw_dynamic_3_view_line(p_avm_param, 0);
		CHECK_ERROR(ret);
	}
	
#if 1
	///******3·����ͼ�Ķ�̬��********/
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

