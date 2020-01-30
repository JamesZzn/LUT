#include"generate_4_view_lut.h"



/***************/
/*
* Function Name:				calculate_front_weight
* Function Function:			calculate front weight
* Input:
*       i;
*		j;
*       world_coordinate;
*		weight_2_dim;
* Note:
*     the interface function should be called at the beginning of this module
* Revision:
*     Create by Cheng.Li on 2018/1/12.
*/
Float64_t calculate_4_view_weight_fixed(int camera_flag, int i, int j, Float64_t *world_coordinate, Float64_t *weight_2_dim, IN P_Avm_Param_S p_avm_param, IN Result_Size_Param_S result_param)
{


	Junc_View_4_Param_S junc_view_4_param = p_avm_param->p_avm_config_param->avm_2d_param.junc_view_4_param;
	CvRect car_logo_rect = result_param.car_logo_rect;
	Float64_t weight_Float64_t;

	if (camera_flag == 0)
	{
		/***calculate front view weight***/
		Float64_t fl_fusion_angle = junc_view_4_param.common_config.fl_fusion_angle;
		Float64_t fl_fusion_range = junc_view_4_param.common_config.fl_fusion_range;
		Float64_t fl_fusion_0 = fl_fusion_angle - fl_fusion_range / 2;

		Float64_t fr_fusion_angle = junc_view_4_param.common_config.fr_fusion_angle;
		Float64_t fr_fusion_range = junc_view_4_param.common_config.fr_fusion_range;
		Float64_t fr_fusion_0 = fr_fusion_angle - (fr_fusion_range / 2);


		Float64_t world_coordinate_0[2] ;
		world_coordinate_0[0] = -p_avm_param->p_avm_config_param->vehicle_param.vehicle_width / 2;
		world_coordinate_0[1] = p_avm_param->p_avm_config_param->vehicle_param.vehicle_length / 2;

		Float64_t world_coordinate_1[2];
		world_coordinate_1[0] = p_avm_param->p_avm_config_param->vehicle_param.vehicle_width / 2;
		world_coordinate_1[1] = p_avm_param->p_avm_config_param->vehicle_param.vehicle_length / 2;

		if (j<car_logo_rect.x || j>(car_logo_rect.x + car_logo_rect.width))
		{

			if (world_coordinate[0] < 0)
			{
				Float64_t temp_angle = atan((world_coordinate[1] - world_coordinate_0[1]) / (world_coordinate[0] - world_coordinate_0[0]));
				Float64_t temp_point_abgle = (temp_angle / 3.1415) * 180;
				weight_Float64_t = (abs(temp_point_abgle) - fl_fusion_0) / fl_fusion_range;
				if (weight_Float64_t >= 1)
				{
					weight_Float64_t = 1;
				}
				if (weight_Float64_t <= 0)
				{
					weight_Float64_t = 0;
				}
			}
			if (world_coordinate[0] > 0)
			{
				Float64_t temp_angle = atan((world_coordinate[1] - world_coordinate_1[1]) / (world_coordinate[0] - world_coordinate_1[0]));
				Float64_t temp_point_abgle = (temp_angle / 3.1415) * 180;
				weight_Float64_t = (abs(temp_point_abgle) - fr_fusion_0) / fr_fusion_range;
				if (weight_Float64_t >= 1)
				{
					weight_Float64_t = 1;
				}
				if (weight_Float64_t <= 0)
				{
					weight_Float64_t = 0;
				}
			}
		}
		else
		{
			weight_Float64_t = 1.000;
		}
		weight_2_dim[i*result_param.result_image_width + j] = weight_Float64_t;//storage 2 dim weighjt
		return	weight_Float64_t;
	}

	if (camera_flag == 1)
	{
		/***calculate rear view weight***/
		Float64_t rl_fusion_angle = junc_view_4_param.common_config.rl_fusion_angle;
		Float64_t rl_fusion_range = junc_view_4_param.common_config.rl_fusion_range;
		Float64_t rl_fusion_0 = rl_fusion_angle - rl_fusion_range / 2;

		Float64_t rr_fusion_angle = junc_view_4_param.common_config.rr_fusion_angle;
		Float64_t rr_fusion_range = junc_view_4_param.common_config.rr_fusion_range;
		Float64_t rr_fusion_0 = rr_fusion_angle - (rr_fusion_range / 2);
		Float64_t world_coordinate_2[2];
		world_coordinate_2[0] = -p_avm_param->p_avm_config_param->vehicle_param.vehicle_width / 2;
		world_coordinate_2[1] = -p_avm_param->p_avm_config_param->vehicle_param.vehicle_length / 2;

		Float64_t world_coordinate_3[2];
		world_coordinate_3[0] = p_avm_param->p_avm_config_param->vehicle_param.vehicle_width / 2;
		world_coordinate_3[1] = -p_avm_param->p_avm_config_param->vehicle_param.vehicle_length / 2;

		if (j<car_logo_rect.x || j>(car_logo_rect.x + car_logo_rect.width))
		{
			if (world_coordinate[0] < 0)
			{
				Float64_t temp_angle = atan((world_coordinate[1] - world_coordinate_2[1]) / (world_coordinate[0] - world_coordinate_2[0]));
				Float64_t temp_point_abgle = (temp_angle / 3.1415) * 180;
				weight_Float64_t = (abs(temp_point_abgle) - rr_fusion_0) / rr_fusion_range;
				if (weight_Float64_t >= 1)
				{
					weight_Float64_t = 1;
				}
				if (weight_Float64_t <= 0)
				{
					weight_Float64_t = 0;
				}
			}
			if (world_coordinate[0] > 0)
			{
				Float64_t temp_angle = atan((world_coordinate[1] - world_coordinate_3[1]) / (world_coordinate[0] - world_coordinate_3[0]));
				Float64_t temp_point_abgle = (temp_angle / 3.1415) * 180;
				weight_Float64_t = (abs(temp_point_abgle) - rl_fusion_0) / rl_fusion_range;
				if (weight_Float64_t >= 1)
				{
					weight_Float64_t = 1;
				}
				if (weight_Float64_t <= 0)
				{
					weight_Float64_t = 0;
				}
			}

		}
		else
		{
			weight_Float64_t = 1.000;
		}
		weight_2_dim[i*result_param.result_image_width + j] = weight_Float64_t;//storage 2 dim weighjt
		return	weight_Float64_t;
	}
}





void calculate_weight_coordinate(IN Int32_t x, IN Int32_t y, OUT  Float64_t weight_coordinate[2], IN CvRect car_logo_rect, IN P_Avm_Param_S p_avm_param,IN Int32_t cam_flag)
{
	Junc_View_4_Param_S junc_view_4_param = p_avm_param->p_avm_config_param->avm_2d_param.junc_view_4_param;

	Float64_t pixel_row_dis = (p_avm_param->p_avm_config_param->vehicle_param.vehicle_length + junc_view_4_param.common_config.front_view_length
		+ junc_view_4_param.common_config.rear_view_length)
		/ (Float32_t)(junc_view_4_param.result_big_param.result_image_height);//calculate every pixel distance
	Float64_t pixel_col_dis = (p_avm_param->p_avm_config_param->vehicle_param.vehicle_width + junc_view_4_param.common_config.left_view_length
		+ junc_view_4_param.common_config.right_view_length)
		/ (Float32_t)(junc_view_4_param.result_big_param.result_image_width);//calculate every pixel distance

	if (cam_flag == 0)
	{
		weight_coordinate[0] = x - car_logo_rect.x;
		weight_coordinate[1] = car_logo_rect.y - junc_view_4_param.common_config.front_blind_length / pixel_row_dis - y;//计算权重时 将权重坐标系建立在小车中心
	}
	if (cam_flag == 1)
	{
		weight_coordinate[0] = x - car_logo_rect.x - car_logo_rect.width;

		weight_coordinate[1] = car_logo_rect.y - junc_view_4_param.common_config.front_blind_length / pixel_row_dis - y;//计算权重时 将权重坐标系建立在小车中心
	}
	if (cam_flag == 2)
	{
		weight_coordinate[0] = x - car_logo_rect.x;

		weight_coordinate[1] = car_logo_rect.y + car_logo_rect.height + junc_view_4_param.common_config.rear_blind_length / pixel_row_dis - y;//计算权重时 将权重坐标系建立在小车中心

	}
	if (cam_flag == 3)
	{
		weight_coordinate[0] = x - car_logo_rect.x - car_logo_rect.width;
		weight_coordinate[1] = car_logo_rect.y + car_logo_rect.height + junc_view_4_param.common_config.rear_blind_length / pixel_row_dis - y;//计算权重时 将权重坐标系建立在小车中心



	}


}
/***************/
/*
* Function Name:				calculate_front_weight
* Function Function:			calculate front weight
* Input:
*       i;
*		j;
*       world_coordinate;
*		weight_2_dim;
* Note:
*    二次函数的曲线融合
* Revision:
*     Create by Cheng.Li on 2018/1/12.
*/
Float64_t calculate_4_view_weight_curve(int camera_flag, int i, int j, Float64_t *world_coordinate, Float64_t *weight_2_dim, IN P_Avm_Param_S p_avm_param, IN Result_Size_Param_S result_param)
{

	
	Junc_View_4_Param_S junc_view_4_param = p_avm_param->p_avm_config_param->avm_2d_param.junc_view_4_param;
	Int32_t range_0 = junc_view_4_param.common_config.parallel_range;//表示平行线的融合的范围
	Int32_t range_vertical = junc_view_4_param.common_config.curve_range;//表示曲线的融合范围
	CvRect car_logo_rect = result_param.car_logo_rect;
	Float64_t weight_Float64_t;
	
	Float64_t pixel_row_dis = (p_avm_param->p_avm_config_param->vehicle_param.vehicle_length + junc_view_4_param.common_config.front_view_length + junc_view_4_param.common_config.rear_view_length)
		/ (Float64_t)(junc_view_4_param.result_big_param.result_image_height);//pixel distance
	Float64_t pixel_col_dis = (p_avm_param->p_avm_config_param->vehicle_param.vehicle_width + junc_view_4_param.common_config.left_view_length + junc_view_4_param.common_config.right_view_length)
		/ (Float64_t)(junc_view_4_param.result_big_param.result_image_width);//pixel distance
	
	if (camera_flag == 0)//表示计算前镜头的图片展开
	{
		if (j<car_logo_rect.x || j>(car_logo_rect.x + car_logo_rect.width))
		{

			if (j<car_logo_rect.x&&i<=car_logo_rect.y)//计算左上角的融合区域
			{
#if 1
				Float64_t weight_coordinate_point[2];
				calculate_weight_coordinate(j, i, weight_coordinate_point, car_logo_rect, p_avm_param,0);
				Float64_t fl_fusion_angle = junc_view_4_param.common_config.fl_fusion_angle;
				
				/**********************************************三条平行线********************************/
				Float64_t k_ratio = tan((-fl_fusion_angle / 180)*PI);
				//由此得出融合的角平分线的直线方程: y=k_ratio * x
				Float64_t sin_temp = sin(((90 - fl_fusion_angle) / 180)*PI);
				Float64_t y_translation = (range_0 / 2) / sin_temp;//得出
				//由此得出融合的下界直线方程: y=k_ratio * x- y_translation
				//由此得出融合的上界直线方程: y=k_ratio * x+ y_translation
				/**********************************************一条垂线********************************/
				Float64_t sin_temp_vertical = sin(((90-fl_fusion_angle) / 180)*PI);
				Float64_t k_vertical = -1 / k_ratio;
				Float64_t temp_vertical = -range_vertical / sin_temp_vertical;
				//与角平分线垂直的直线是y=k_vertical*x-k_vertical*temp_vertical 

				Float64_t curve_x = (k_vertical*temp_vertical - y_translation) / (k_vertical - k_ratio);
				Float64_t curve_y = k_vertical*curve_x - k_vertical*temp_vertical;
				
				//Float64_t a0 = curve_y / (curve_x*curve_x);
				////以小车矩形左上角为原点建立坐标系得y=a0*x^2;
				////动点 的直线方y=k_vertical*x-k_vertical*weight_coordinate_point[0]+weight_coordinate_point[1]
				//Float64_t b0 = -k_vertical*weight_coordinate_point[0] + weight_coordinate_point[1];
				////动点 的直线y=k_vertical*x+b0
				//Float64_t border_x_0 = (k_vertical - sqrt(k_vertical*k_vertical + 4 * a0*b0)) / (2 * a0);
				//Float64_t border_x_1 = (k_vertical + sqrt(k_vertical*k_vertical + 4 * a0*b0)) / (2 * a0);
				//以小车矩形左上角为原点建立坐标系得y=a0*x^2+b0*x;
				//导函数y=2*a0*x+b0
				//由在2*a0*curve_x+b0=k_ratio
				//a0*curve_x^2+b0*curve_x=curve_y;


				Float64_t a6 = (k_ratio*curve_x - curve_y) / (curve_x*curve_x);
				Float64_t b6 = k_ratio-2*a6*curve_x;
				//动点 的直线方y=k_vertical*x-k_vertical*weight_coordinate_point[0]+weight_coordinate_point[1]
				Float64_t b0 = -k_vertical*weight_coordinate_point[0] + weight_coordinate_point[1];
				//动点 的直线y=k_vertical*x+b0
				Float64_t border_x_0 = ((k_vertical - b6) - sqrt((k_vertical - b6)*(k_vertical - b6) + 4 * a6*b0)) / (2 * a6);
				Float64_t border_x_1 = ((k_vertical - b6) + sqrt((k_vertical - b6)*(k_vertical - b6) + 4 * a6*b0)) / (2 * a6);
				
				

				Float64_t border_y_0 = k_vertical*border_x_0 + b0;
				Float64_t border_y_1 = k_vertical*border_x_1 + b0;
				if (weight_coordinate_point[1] - k_vertical*weight_coordinate_point[0] + k_vertical*temp_vertical > 0)
				{
					Float64_t distance = abs(weight_coordinate_point[1] - k_ratio * weight_coordinate_point[0] + y_translation)
						/ sqrt(k_ratio*k_ratio + 1);
					weight_Float64_t = distance / range_0;
					if (weight_Float64_t >= 1)
					{
						weight_Float64_t = 1;
					}
					//weight_Float64_t = 1;
					if (weight_coordinate_point[1] - k_ratio * weight_coordinate_point[0] + y_translation <= 0)
					{
						weight_Float64_t = 0;//表示当位于下界直线下方时的权重
						
					}
				}
#if 1
				if (weight_coordinate_point[1] - k_vertical*weight_coordinate_point[0] + k_vertical*temp_vertical <0)
				{
					//在曲线的融合区域内求出每个点的直线方程来
					//Float64_t in_curve_temp = k_vertical*weight_coordinate_point[0] - weight_coordinate_point[1];
					if (weight_coordinate_point[0] < border_x_0)
					{
						weight_Float64_t = 0;
					
					}
					else
					{
						Float64_t distance_0 = sqrt((weight_coordinate_point[0] - border_x_0)*(weight_coordinate_point[0] - border_x_0)
							+ (weight_coordinate_point[1] - border_y_0)*(weight_coordinate_point[1] - border_y_0));
						Float64_t all_distance =2*abs(border_y_0 - k_ratio*border_x_0) / sqrt(1 + k_ratio*k_ratio);
						weight_Float64_t = distance_0 / all_distance;
						if (weight_Float64_t > 1)
						{
							weight_Float64_t = 1;
						}
						//weight_Float64_t = 1;
						

					}

				}
#endif
#endif
			}
			if (j > car_logo_rect.x + car_logo_rect.width&&i <= car_logo_rect.y)//计算右上角的融合区域
			{
#if 1
				Float64_t weight_coordinate_point[2];
				calculate_weight_coordinate(j, i, weight_coordinate_point, car_logo_rect,p_avm_param,1);
				Float64_t fr_fusion_angle = junc_view_4_param.common_config.fr_fusion_angle;
				

				/**********************************************三条平行线********************************/
				Float64_t k_ratio = tan((fr_fusion_angle / 180)*PI);
				//由此得出融合的角平分线的直线方程: y=k_ratio * x
				Float64_t sin_temp = sin(((90 - fr_fusion_angle) / 180)*PI);
				Float64_t y_translation = (range_0 / 2) / sin_temp;//得出
				//由此得出融合的下界直线方程: y=k_ratio * x- y_translation
				//由此得出融合的上界直线方程: y=k_ratio * x+ y_translation

				/*************************************************一条垂线***************************************/
				Float64_t sin_temp_vertical = sin(((90 - fr_fusion_angle )/ 180)*PI);
				Float64_t k_vertical = -1 / k_ratio;
				Float64_t temp_vertical = range_vertical / sin_temp_vertical;
				//与角平分线垂直的直线是y=k_vertical*x-k_vertical*temp_vertical 


				Float64_t curve_x = ( y_translation - k_vertical*temp_vertical ) / ( k_ratio-k_vertical );
				Float64_t curve_y = k_vertical*curve_x - k_vertical*temp_vertical;
			
				//以小车矩形右上角为原点建立坐标系得y=a0*x^2+b0*x;
				//导函数y=2*a0*x+b0
				//由在2*a0*curve_x+b0=k_ratio
				//a0*curve_x^2+b0*curve_x=curve_y;
				Float64_t a6 = (k_ratio*curve_x - curve_y) / (curve_x*curve_x);
				Float64_t b6 = k_ratio-2*a6*curve_x;
				//动点 的直线方y=k_vertical*x-k_vertical*weight_coordinate_point[0]+weight_coordinate_point[1]
				Float64_t b0 = -k_vertical*weight_coordinate_point[0] + weight_coordinate_point[1];
				//动点 的直线y=k_vertical*x+b0
				Float64_t border_x_0 = ((k_vertical - b6) - sqrt((k_vertical - b6)*(k_vertical - b6) + 4 * a6*b0)) / (2 * a6);
				Float64_t border_x_1 = ((k_vertical - b6) + sqrt((k_vertical - b6)*(k_vertical - b6) + 4 * a6*b0)) / (2 * a6);

				Float64_t border_y_0 = k_vertical*border_x_0 + b0;
				Float64_t border_y_1 = k_vertical*border_x_1 + b0;
#if 1
				if (weight_coordinate_point[1] - k_vertical*weight_coordinate_point[0] + k_vertical*temp_vertical > 0)//在平行线融合区域内
				{
					Float64_t distance = abs(weight_coordinate_point[1] - k_ratio * weight_coordinate_point[0] + y_translation)
										/ sqrt(k_ratio*k_ratio + 1);
					weight_Float64_t = distance / range_0;
					if (weight_Float64_t >= 1)
					{
						weight_Float64_t = 1;
					}
					//weight_Float64_t = 1;
					if (weight_coordinate_point[1] - k_ratio * weight_coordinate_point[0] + y_translation <= 0)
					{
						weight_Float64_t = 0;//表示当位于下界直线下方时的权重

					}
				}
#endif
#if 1
				if (weight_coordinate_point[1] - k_vertical*weight_coordinate_point[0] + k_vertical*temp_vertical <0)
				{
					//在曲线的融合区域内求出每个点的直线方程来
					//Float64_t in_curve_temp = k_vertical*weight_coordinate_point[0] - weight_coordinate_point[1];
					if (weight_coordinate_point[0] > border_x_1)
					{
						weight_Float64_t = 0;

					}
					else
					{

						Float64_t distance_0 = sqrt((weight_coordinate_point[0] - border_x_1)*(weight_coordinate_point[0] - border_x_1)
							+ (weight_coordinate_point[1] - border_y_1)*(weight_coordinate_point[1] - border_y_1));
						Float64_t all_distance = 2 * abs(border_y_1 - k_ratio*border_x_1) / sqrt(1 + k_ratio*k_ratio);
						weight_Float64_t = distance_0 / all_distance;
						if (weight_Float64_t > 1)
						{
							weight_Float64_t = 1;
						}
						//weight_Float64_t = 1;
					}

				}
#endif
#endif

			}
		}
		else
		{
			weight_Float64_t = 1.000;
		}
		//weight_Float64_t = 1;
		weight_2_dim[i*result_param.result_image_width + j] = weight_Float64_t;//storage 2 dim weighjt
		return	weight_Float64_t;
	}
#if 1
	if (camera_flag == 1)//表示计算后镜头的图片展开
	{
		if (j<car_logo_rect.x || j>(car_logo_rect.x + car_logo_rect.width))
		{

			if (j<car_logo_rect.x&&i>=car_logo_rect.y + car_logo_rect.height)//计算左下角的融合区域
			{
#if 1
				Float64_t weight_coordinate_point[2];
				calculate_weight_coordinate(j, i, weight_coordinate_point, car_logo_rect, p_avm_param,2);
				Float64_t rl_fusion_angle = junc_view_4_param.common_config.rl_fusion_angle;
				
				/**********************************************三条平行线********************************/
				Float64_t k_ratio = tan((rl_fusion_angle / 180)*PI);
				//由此得出融合的角平分线的直线方程: y=k_ratio * x
				Float64_t sin_temp = sin(((90 - rl_fusion_angle) / 180)*PI);
				Float64_t y_translation = (range_0 / 2) / sin_temp;//得出
				//由此得出融合的下界直线方程: y=k_ratio * x- y_translation
				//由此得出融合的上界直线方程: y=k_ratio * x+ y_translation

				/*************************************************一条垂线***************************************/
				Float64_t sin_temp_vertical = sin(((90 - rl_fusion_angle) / 180)*PI);
				Float64_t k_vertical = -1 / k_ratio;
				Float64_t temp_vertical = -range_vertical / sin_temp_vertical;
				//与角平分线垂直的直线是y=k_vertical*x-k_vertical*temp_vertical 


				Float64_t curve_x = (y_translation + k_vertical*temp_vertical) / (k_vertical-k_ratio  );//垂线与两条上下界直线的交点
				Float64_t curve_y = k_vertical*curve_x - k_vertical*temp_vertical;


				
				//Float64_t a0 = curve_y / (curve_x*curve_x);
				////以小车矩形右上角为原点建立坐标系得y=a0*x^2;
				////动点 的直线方y=k_vertical*x-k_vertical*weight_coordinate_point[0]+weight_coordinate_point[1]
				//Float64_t b0 = -k_vertical*weight_coordinate_point[0] + weight_coordinate_point[1];
				////动点 的直线y=k_vertical*x+b0
				//Float64_t border_x_0 = (k_vertical + sqrt(k_vertical*k_vertical + 4 * a0*b0)) / (2 * a0);
				//Float64_t border_x_1 = (k_vertical - sqrt(k_vertical*k_vertical + 4 * a0*b0)) / (2 * a0);
				//






				//以小车矩形左上角为原点建立坐标系得y=a0*x^2+b0*x;
				//导函数y=2*a0*x+b0
				//由在2*a0*curve_x+b0=k_ratio
				//a0*curve_x^2+b0*curve_x=curve_y;
				Float64_t a6 = (k_ratio*curve_x - curve_y) / (curve_x*curve_x);
				Float64_t b6 = k_ratio - 2 * a6*curve_x;
				//动点 的直线方y=k_vertical*x-k_vertical*weight_coordinate_point[0]+weight_coordinate_point[1]
				Float64_t b0 = -k_vertical*weight_coordinate_point[0] + weight_coordinate_point[1];
				//动点 的直线y=k_vertical*x+b0
				Float64_t border_x_0 = ((k_vertical - b6) + sqrt((k_vertical - b6)*(k_vertical - b6) + 4 * a6*b0)) / (2 * a6);
				Float64_t border_x_1 = ((k_vertical - b6) - sqrt((k_vertical - b6)*(k_vertical - b6) + 4 * a6*b0)) / (2 * a6);


				Float64_t border_y_0 = k_vertical*border_x_0 + b0;
				Float64_t border_y_1 = k_vertical*border_x_1 + b0;
#if 1
				if (weight_coordinate_point[1] - k_vertical*weight_coordinate_point[0] + k_vertical*temp_vertical < 0)//在平行线融合区域内
				{
					Float64_t distance = abs(weight_coordinate_point[1] - k_ratio * weight_coordinate_point[0] - y_translation)//求出与上界直线的距离
						/ sqrt(k_ratio*k_ratio + 1);
					weight_Float64_t = distance / range_0;
					if (weight_Float64_t >= 1)
					{
						weight_Float64_t = 1;
					}
					//weight_Float64_t = 1;
					if (weight_coordinate_point[1] - k_ratio * weight_coordinate_point[0] - y_translation >= 0)
					{
						weight_Float64_t = 0;//表示当位于下界直线下方时的权重

					}
				}
#endif
#if 1
				if (weight_coordinate_point[1] - k_vertical*weight_coordinate_point[0] + k_vertical*temp_vertical >0)
				{
					//在曲线的融合区域内求出每个点的直线方程来
					//Float64_t in_curve_temp = k_vertical*weight_coordinate_point[0] - weight_coordinate_point[1];
					if (weight_coordinate_point[0] < border_x_0)
					{
						weight_Float64_t = 0;

					}
					else
					{

						Float64_t distance_0 = sqrt((weight_coordinate_point[0] - border_x_0)*(weight_coordinate_point[0] - border_x_0)
							+ (weight_coordinate_point[1] - border_y_0)*(weight_coordinate_point[1] - border_y_0));//求出此动点到曲线边界的距离
						Float64_t all_distance = 2 * abs(border_y_0 - k_ratio*border_x_0) / sqrt(1 + k_ratio*k_ratio);
						weight_Float64_t = distance_0 / all_distance;
						if (weight_Float64_t > 1)
						{
							weight_Float64_t = 1;
						}
						//weight_Float64_t = 1;
						
					}
					

				}
#endif
#endif
			}
			if (j > car_logo_rect.x + car_logo_rect.width&&i>=car_logo_rect.y + car_logo_rect.height)//计算右下角的融合区域
			{
#if 1
				Float64_t weight_coordinate_point[2];
				calculate_weight_coordinate(j, i, weight_coordinate_point, car_logo_rect, p_avm_param,3);
				Float64_t rr_fusion_angle = junc_view_4_param.common_config.rr_fusion_angle;
				
				/*************************************三条平行线***********************************/
				Float64_t k_ratio = tan((-rr_fusion_angle / 180)*PI);
				//由此得出融合的角平分线的直线方程: y=k_ratio * x
				Float64_t sin_temp = sin(((90 - rr_fusion_angle) / 180)*PI);
				Float64_t y_translation = (range_0 / 2) / sin_temp;//得出
				//由此得出融合的下界直线方程: y=k_ratio * x- y_translation
				//由此得出融合的上界直线方程: y=k_ratio * x+ y_translation

				/******************************************一条垂线***************************************/
				Float64_t sin_temp_vertical = sin(((90-rr_fusion_angle) / 180)*PI);
				Float64_t k_vertical = -1 / k_ratio;
				Float64_t temp_vertical = range_vertical / sin_temp_vertical;//垂线与横坐标的交点
				//与角平分线垂直的直线是y=k_vertical*x-k_vertical*temp_vertical 


				Float64_t curve_x = (k_vertical*temp_vertical + y_translation) / (k_vertical - k_ratio);//求出垂涎于上界线的交点以便 确定直线
				Float64_t curve_y = k_vertical*curve_x - k_vertical*temp_vertical;


				/*
				Float64_t a0 = curve_y / (curve_x*curve_x);
				//以小车矩形左上角为原点建立坐标系得y=a0*x^2;
				//动点 的直线方y=k_vertical*x-k_vertical*weight_coordinate_point[0]+weight_coordinate_point[1]
				Float64_t b0 = -k_vertical*weight_coordinate_point[0] + weight_coordinate_point[1];
				//动点 的直线y=k_vertical*x+b0
				Float64_t border_x_0 = (k_vertical - sqrt(k_vertical*k_vertical + 4 * a0*b0)) / (2 * a0);//是两个解中最大的值
				Float64_t border_x_1 = (k_vertical + sqrt(k_vertical*k_vertical + 4 * a0*b0)) / (2 * a0);
				*/

				//以小车矩形左上角为原点建立坐标系得y=a0*x^2+b0*x;
				//导函数y=2*a0*x+b0
				//由在2*a0*curve_x+b0=k_ratio
				//a0*curve_x^2+b0*curve_x=curve_y;
				Float64_t a6 = (k_ratio*curve_x - curve_y) / (curve_x*curve_x);
				Float64_t b6 = k_ratio - 2 * a6*curve_x;
				//动点 的直线方y=k_vertical*x-k_vertical*weight_coordinate_point[0]+weight_coordinate_point[1]
				Float64_t b0 = -k_vertical*weight_coordinate_point[0] + weight_coordinate_point[1];
				//动点 的直线y=k_vertical*x+b0
				Float64_t border_x_0 = ((k_vertical - b6) - sqrt((k_vertical - b6)*(k_vertical - b6) + 4 * a6*b0)) / (2 * a6);
				Float64_t border_x_1 = ((k_vertical - b6) + sqrt((k_vertical - b6)*(k_vertical - b6) + 4 * a6*b0)) / (2 * a6);




				Float64_t border_y_0 = k_vertical*border_x_0 + b0;
				Float64_t border_y_1 = k_vertical*border_x_1 + b0;
				if (weight_coordinate_point[1] - k_vertical*weight_coordinate_point[0] + k_vertical*temp_vertical < 0)
				{
					Float64_t distance = abs(weight_coordinate_point[1] - k_ratio * weight_coordinate_point[0] - y_translation)
						/ sqrt(k_ratio*k_ratio + 1);
					weight_Float64_t = distance / range_0;
					if (weight_Float64_t >= 1)
					{
						weight_Float64_t = 1;
					}
					//weight_Float64_t = 1;
					if (weight_coordinate_point[1] - k_ratio * weight_coordinate_point[0] - y_translation >= 0)
					{
						weight_Float64_t = 0;//表示当位于上界界直线上方时的权重

					}
				}
#if 1
				if (weight_coordinate_point[1] - k_vertical*weight_coordinate_point[0] + k_vertical*temp_vertical>0)
				{
					//在曲线的融合区域内求出每个点的直线方程来
					//Float64_t in_curve_temp = k_vertical*weight_coordinate_point[0] - weight_coordinate_point[1];
					if (weight_coordinate_point[0] > border_x_0)
					{
						weight_Float64_t = 0;

					}
					else
					{

						Float64_t distance_0 = sqrt((weight_coordinate_point[0] - border_x_0)*(weight_coordinate_point[0] - border_x_0)
							+ (weight_coordinate_point[1] - border_y_0)*(weight_coordinate_point[1] - border_y_0));
						Float64_t all_distance =2*abs(border_y_0 - k_ratio*border_x_0) / sqrt(1 + k_ratio*k_ratio);
						weight_Float64_t = distance_0 / all_distance;
						if (weight_Float64_t > 1)
						{
							weight_Float64_t = 1;
						}
						//weight_Float64_t = 1;
						
					}

				}
#endif
#endif
			}
		}
		else
		{
			weight_Float64_t = 1.000;
		}
		//weight_Float64_t = 1;
		weight_2_dim[i*result_param.result_image_width + j] = weight_Float64_t;//storage 2 dim weighjt
		return	weight_Float64_t;
	}
#endif



	
}





/***************/
/*
* Function Name:				calculate_front_weight
* Function Function:			calculate front weight
* Input:
*       i;
*		j;
*       world_coordinate;
*		weight_2_dim;
* Note:
*     the interface function should be called at the beginning of this module
* Revision:
*     Create by Cheng.Li on 2018/1/12.
*/
Float64_t calculate_4_view_weight(int camera_flag, int i, int j, Float64_t *world_coordinate, Float64_t *weight_2_dim, IN P_Avm_Param_S p_avm_param, IN Result_Size_Param_S result_param)
{


	Junc_View_4_Param_S junc_view_4_param = p_avm_param->p_avm_config_param->avm_2d_param.junc_view_4_param;
	CvRect car_logo_rect = result_param.car_logo_rect;
	Float64_t weight_Float64_t;

	Float64_t pixel_row_dis = (p_avm_param->p_avm_config_param->vehicle_param.vehicle_length + junc_view_4_param.common_config.front_view_length + junc_view_4_param.common_config.rear_view_length)//专门为了盲区 而求出的像素距离
		/ (Float64_t)(junc_view_4_param.result_big_param.result_image_height);//pixel distance
	Float64_t pixel_col_dis = (p_avm_param->p_avm_config_param->vehicle_param.vehicle_width + junc_view_4_param.common_config.left_view_length + junc_view_4_param.common_config.right_view_length)
		/ (Float64_t)(junc_view_4_param.result_big_param.result_image_width);//pixel distance
	if (camera_flag == 0)
	{
		/***calculate front view weight***/
		Float64_t fl_fusion_angle = junc_view_4_param.common_config.fl_fusion_angle;
		Float64_t fl_fusion_range = junc_view_4_param.common_config.fl_fusion_range;
		Float64_t fl_fusion_0 = fl_fusion_angle - fl_fusion_range / 2;

		Float64_t fr_fusion_angle = junc_view_4_param.common_config.fr_fusion_angle;
		Float64_t fr_fusion_range = junc_view_4_param.common_config.fr_fusion_range;
		Float64_t fr_fusion_0 = fr_fusion_angle - (fr_fusion_range / 2);


		Float64_t world_coordinate_0[2];
		calculate_world_coordinate(world_coordinate_0, car_logo_rect.y - junc_view_4_param.common_config.front_blind_length / pixel_row_dis,
								car_logo_rect.x, p_avm_param);

		Float64_t world_coordinate_1[2];
		calculate_world_coordinate(world_coordinate_1, car_logo_rect.y - junc_view_4_param.common_config.front_blind_length / pixel_row_dis,
								car_logo_rect.x + car_logo_rect.width, p_avm_param);

		if (j<car_logo_rect.x || j>(car_logo_rect.x + car_logo_rect.width))//对于前前区域 来说只需在左右角的地方需要融合中间的权值 设为1 
		{

			if (world_coordinate[0] < 0)//小于0说明在左上角
			{
				Float64_t temp_angle = atan((world_coordinate[1] - world_coordinate_0[1]) 
							/ (world_coordinate[0] - world_coordinate_0[0]));//求出当前扫描到的点的与小车矩形左上点的连线所生成的夹角
				Float64_t temp_point_abgle = (temp_angle / 3.1415) * 180;//转化成角度
				weight_Float64_t = (abs(temp_point_abgle) - fl_fusion_0) / fl_fusion_range;//角度减去左下 的角度站整个的角度求权值
				if (weight_Float64_t >= 1)
				{
					weight_Float64_t = 1;
				}
				if (weight_Float64_t <= 0)
				{
					weight_Float64_t = 0;
				}
			}
			if (world_coordinate[0] > 0)
			{
				Float64_t temp_angle = atan((world_coordinate[1] - world_coordinate_1[1]) / (world_coordinate[0] - world_coordinate_1[0]));
				Float64_t temp_point_abgle = (temp_angle / 3.1415) * 180;
				weight_Float64_t = (abs(temp_point_abgle) - fr_fusion_0) / fr_fusion_range;
				if (weight_Float64_t >= 1)
				{
					weight_Float64_t = 1;
				}
				if (weight_Float64_t <= 0)
				{
					weight_Float64_t = 0;
				}
			}
		}
		else
		{
			weight_Float64_t = 1.000;
		}
		weight_2_dim[i*result_param.result_image_width + j] = weight_Float64_t;//storage 2 dim weighjt
		return	weight_Float64_t;
	}

	if (camera_flag == 1)
	{
		/***calculate rear view weight***/
		Float64_t rl_fusion_angle = junc_view_4_param.common_config.rl_fusion_angle;
		Float64_t rl_fusion_range = junc_view_4_param.common_config.rl_fusion_range;
		Float64_t rl_fusion_0 = rl_fusion_angle - rl_fusion_range / 2;

		Float64_t rr_fusion_angle = junc_view_4_param.common_config.rr_fusion_angle;
		Float64_t rr_fusion_range = junc_view_4_param.common_config.rr_fusion_range;
		Float64_t rr_fusion_0 = rr_fusion_angle - (rr_fusion_range / 2);
		Float64_t world_coordinate_2[2];
		calculate_world_coordinate(world_coordinate_2,
			car_logo_rect.y + car_logo_rect.height + junc_view_4_param.common_config.rear_blind_length / pixel_row_dis,
			car_logo_rect.x, p_avm_param);
		Float64_t world_coordinate_3[2];
		calculate_world_coordinate(world_coordinate_3,
			car_logo_rect.y + car_logo_rect.height + junc_view_4_param.common_config.rear_blind_length / pixel_row_dis, 
			car_logo_rect.x + car_logo_rect.width, p_avm_param);

		if (j<car_logo_rect.x || j>(car_logo_rect.x + car_logo_rect.width))
		{
			if (world_coordinate[0] < 0)
			{
				Float64_t temp_angle = atan((world_coordinate[1] - world_coordinate_2[1]) / (world_coordinate[0] - world_coordinate_2[0]));
				Float64_t temp_point_abgle = (temp_angle / 3.1415) * 180;
				weight_Float64_t = (abs(temp_point_abgle) - rr_fusion_0) / rr_fusion_range;
				if (weight_Float64_t >= 1)
				{
					weight_Float64_t = 1;
				}
				if (weight_Float64_t <= 0)
				{
					weight_Float64_t = 0;
				}
			}
			if (world_coordinate[0] > 0)
			{
				Float64_t temp_angle = atan((world_coordinate[1] - world_coordinate_3[1]) / (world_coordinate[0] - world_coordinate_3[0]));
				Float64_t temp_point_abgle = (temp_angle / 3.1415) * 180;
				weight_Float64_t = (abs(temp_point_abgle) - rl_fusion_0) / rl_fusion_range;
				if (weight_Float64_t >= 1)
				{
					weight_Float64_t = 1;
				}
				if (weight_Float64_t <= 0)
				{
					weight_Float64_t = 0;
				}
			}
			
		}
		else
		{
			weight_Float64_t = 1.000;
		}
		weight_2_dim[i*result_param.result_image_width + j] = weight_Float64_t;//storage 2 dim weighjt
		return	weight_Float64_t;
	}
}
/**************calculate world coordinate***********************/

/*
* Function Name:				calculate_world_coordinate
* Function Function:			calculate world coordinate by picture coordinate
* Input:
*       i;                      this is picture's row coordinate
*		j;						this is picture's cols coordinate
*       world_coordinate;       this is world coordinate
* Note:
*     the interface function should be called at the beginning of this module
* Revision:
*     Create by Cheng.Li on 2018/1/12.
*/
void calculate_world_coordinate(OUT Float64_t *world_coordinate, IN int i, IN int j, IN P_Avm_Param_S p_avm_param)
{
	Junc_View_4_Param_S junc_view_4_param = p_avm_param->p_avm_config_param->avm_2d_param.junc_view_4_param;
	CvRect car_logo_rect=junc_view_4_param.result_big_param.car_logo_rect;
	Float64_t pixel_row_dis = (p_avm_param->p_avm_config_param->vehicle_param.vehicle_length + junc_view_4_param.common_config.front_view_length 
							+ junc_view_4_param.common_config.rear_view_length)
								/ (Float32_t)(junc_view_4_param.result_big_param.result_image_height);//calculate every pixel distance
	Float64_t pixel_col_dis = (p_avm_param->p_avm_config_param->vehicle_param.vehicle_width + junc_view_4_param.common_config.left_view_length 
							+ junc_view_4_param.common_config.right_view_length)
								/ (Float32_t)(junc_view_4_param.result_big_param.result_image_width);//calculate every pixel distance

	world_coordinate[0] = (Float64_t)(pixel_col_dis)*(Float64_t)((-(car_logo_rect.x+ car_logo_rect.width / 2)) + j);
	world_coordinate[1] = (Float64_t)(pixel_row_dis)*(Float64_t)(car_logo_rect.y  + car_logo_rect.height / 2 - i);
	
}

/***************world coordinate to pic coordinate**********************/

/*
* Function Name:				world_coordinate_to_pic_coordinate
* Function Function:			world coordinate to pic_coordinate
* Input:
*       camera_id;             this is you shuold look for which picture
*		world_coordinate;
*       image_point;           this is return the picture coordinate
* Note:
*
* Revision:
*     Create by Cheng.Li on 2018/1/12.
*/
void world_coordinate_to_pic_coordinate(IN int camera_id, IN Float64_t * world_coordinate, OUT Float64_t *image_point, IN P_Avm_Param_S p_avm_param)
{
	Float64_t camera_coordinate[3];

	Mat_Mul_Vec_3by3_DB(camera_coordinate, p_avm_param->p_avm_config_param->camera_model[camera_id].camera_model_ext.inv_R, world_coordinate);
	
	for (int i = 3; i < 6; i++)
	{
		camera_coordinate[i - 3] = p_avm_param->p_avm_config_param->camera_model[camera_id].camera_model_ext.inv_pose[i] + camera_coordinate[i - 3];
	}

	Float64_t tmp = 0;
	tmp = camera_coordinate[0];
	camera_coordinate[0] = camera_coordinate[1];
	camera_coordinate[1] = tmp;
	camera_coordinate[2] = -camera_coordinate[2];
	Cam_Ray_To_Image_Point(image_point, camera_coordinate, &(p_avm_param->p_avm_config_param->camera_model[camera_id].camera_model_int));

}




/*****************************zip data**************************/
/*
* Function Name:				zip_data
* Function Function:			zip_data
* Input:
*       shift_altogether;       this is the position you should storage
*		image_point;			the picture coordinate
*       weight_Float64_t;          the weight of pixel
* Note:
*
* Revision:
*     Create by Cheng.Li on 2018/1/12.
*/
void zip_data(IN int shift_altogether, IN Float64_t *image_point, IN Float64_t weight_Float64_t,
				IN UInt32_t*p_lut, IN UInt32_t camera_flag, IN UInt32_t left_right_flag)
{

	UInt32_t u_int = (UInt32_t)image_point[0], v_int = (UInt32_t)image_point[1];   //get integer'part of position
	Float64_t u_Float64_t = image_point[0] - u_int, v_Float64_t = image_point[1] - v_int; // get decimal'partr of position
	UInt32_t u_point_int = u_Float64_t * 255, v_point_int = v_Float64_t * 255;
	UInt32_t temp_shift = 0;
	UInt32_t temp_shift0 = 0;
	UInt32_t temp_weight;     //one pixel's weight in dst_pic
	UInt32_t white_balance = 0;
    
	temp_weight = weight_Float64_t * 255;
	if (weight_Float64_t == 1.000)
	{
		white_balance = 0;
	}
	temp_shift = (u_int << 21) + (v_int << 10) + (temp_weight << 2) + (camera_flag);
	temp_shift0 = (u_point_int << 24) + (v_point_int << 16) + (left_right_flag << 15) + (white_balance<<14);
	p_lut[shift_altogether] = temp_shift;
	p_lut[shift_altogether+1] = temp_shift0;

}




/*******generate_4_view_lut********/
/*
* Function Name:				init_lut
* Function Function:			this function is calculate source picture coordinate and zip data into LUT
* Input:
*       image[4];
* Note:
*
* Revision:
*     Create by Cheng.Li on 2018/1/12.
*/
AVM_Error_Code_E generate_4_view_lut(IN P_Avm_Param_S p_avm_param, IN Result_Size_Param_S result_param, IN UInt32_t* p_lut)
{

#if CAR_FIXED

	AVM_Error_Code_E error = ERROR_NONE;
	Junc_View_4_Param_S junc_view_4_param = p_avm_param->p_avm_config_param->avm_2d_param.junc_view_4_param;
	/*****public use*****/
	Float64_t world_coordinate[3];//world_coordinate x,y,z
	world_coordinate[2] = 0;
	Float64_t camera_coordinate[3];
	CvRect car_logo_rect = result_param.car_logo_rect;// the information of car
	int camera_size[4], LUT_shift_address;  //calculate front rear left and right shifting of LUT，计算lut表的地址偏移 
	int i, j;
	Float64_t image_point[2];//get position of wide_angle_picture 
	/**********calculate fusion equation,存放前后视图计算出来的融合权重 以便左右直接相减保证权重之和 为1**************/
	Float64_t* weight_2_dim = (Float64_t *)malloc(sizeof(Float64_t)*result_param.result_image_width*result_param.result_image_height);
	if (!weight_2_dim)
	{
		error = ERROR_MEMORY_MALLOC_FAIL;
		CHECK_ERROR(error);
	}
	memset(weight_2_dim, 0, sizeof(Float64_t)*result_param.result_image_width
		*result_param.result_image_height);

	/****calculate front rear left and right shifting of LUT****/
	for (Int32_t cam_id = 0; cam_id < CAMERA_NUM; cam_id++)//计算存储前后左右视图的lut表的地址偏移 
	{
		camera_size[cam_id] = result_param.lut_size[cam_id] * 2;
	}

	UInt32_t left_right_flag=0;
	/*******************/
	Float64_t pixel_row_dis_front = (junc_view_4_param.common_config.front_view_length)
		/ (Float64_t)(car_logo_rect.y);//pixel distance
	Float64_t pixel_row_dis_middle = (p_avm_param->p_avm_config_param->vehicle_param.vehicle_length)
		/ (Float64_t)(car_logo_rect.height);//pixel distance
	Float64_t pixel_row_dis_back = (junc_view_4_param.common_config.rear_view_length)
		/ (Float64_t)(result_param.result_image_height - car_logo_rect.height - car_logo_rect.y);//pixel distance


	Float64_t pixel_col_dis_left = (junc_view_4_param.common_config.left_view_length)
		/ (Float64_t)(car_logo_rect.x);//pixel distance
	Float64_t pixel_col_dis_middle = (p_avm_param->p_avm_config_param->vehicle_param.vehicle_width)
		/ (Float64_t)(car_logo_rect.width);//pixel distance
	Float64_t pixel_col_dis_right = (junc_view_4_param.common_config.right_view_length)
		/ (Float64_t)(result_param.result_image_width - car_logo_rect.width - car_logo_rect.x);//pixel distance

	/********************extend all over result image pixel************************/
	for (i = 0; i < result_param.result_image_height; i++)
	{
		for (j = 0; j < result_param.result_image_width; j++)
		{
#if 1
			/************front view*******************************/
			if (i <= car_logo_rect.y - junc_view_4_param.common_config.front_blind_length / pixel_row_dis_front)//
			{
				if (j <= car_logo_rect.x)//如果是左上角
				{
					/***********calculate world coordinate*********/
					world_coordinate[0] = (Float64_t)(pixel_col_dis_left)*(Float64_t)(j - car_logo_rect.x)
						- (p_avm_param->p_avm_config_param->vehicle_param.vehicle_width / 2);
					world_coordinate[1] = (Float64_t)(pixel_row_dis_front)*(Float64_t)(car_logo_rect.y  - i)
						+ (p_avm_param->p_avm_config_param->vehicle_param.vehicle_length / 2);

				}
				if (j >= car_logo_rect.x + car_logo_rect.width)//+ junc_view_4_param.common_config.rear_blind_length / pixel_col_dis///如果是右上角
				{
					/***********calculate world coordinate*********/
					world_coordinate[0] = (Float64_t)(pixel_col_dis_right)*(Float64_t)(j - car_logo_rect.x - car_logo_rect.width)
						+ (p_avm_param->p_avm_config_param->vehicle_param.vehicle_width / 2);
					world_coordinate[1] = (Float64_t)(pixel_row_dis_front)*(Float64_t)(car_logo_rect.y - i)
						+ (p_avm_param->p_avm_config_param->vehicle_param.vehicle_length / 2);

				}
				if (j > car_logo_rect.x&&j < car_logo_rect.x + car_logo_rect.width)//如果是前部中间的非融合区域
				{
					/***********calculate world coordinate*********/
					world_coordinate[0] = (Float64_t)(pixel_col_dis_middle)*(Float64_t)(j - car_logo_rect.x - car_logo_rect.width/2);
					world_coordinate[1] = (Float64_t)(pixel_row_dis_front)*(Float64_t)(car_logo_rect.y - i)
						+ (p_avm_param->p_avm_config_param->vehicle_param.vehicle_length / 2);
				}

				/********calculate weight************/
				//Float64_t weight_Float64_t = calculate_4_view_weight_fixed(CAM_FRONT, i, j, world_coordinate, weight_2_dim, p_avm_param, result_param);
				//Float64_t weight_Float64_t = calculate_4_view_weight(CAM_FRONT, i, j, world_coordinate, weight_2_dim, p_avm_param, result_param);
				Float64_t weight_Float64_t = calculate_4_view_weight_curve(CAM_FRONT, i, j, world_coordinate, weight_2_dim, p_avm_param, result_param);


				/***********world coordinate to pic coordinate**************/
				world_coordinate_to_pic_coordinate(CAM_FRONT, world_coordinate, image_point, p_avm_param);//0 is front

				/********zip data and fill lut*****/
				LUT_shift_address = i * result_param.result_image_width * 2 + j * 2;
				zip_data(LUT_shift_address, image_point, weight_Float64_t, p_lut, CAM_FRONT, left_right_flag);

			}
#if 1
			/*************rear view*********************************/
			if (i >= car_logo_rect.y + car_logo_rect.height + (junc_view_4_param.common_config.rear_blind_length / pixel_row_dis_back))//
			{
				if (j <= car_logo_rect.x)//左下角
				{
					/***********calculate world coordinate*********/
					world_coordinate[0] = (Float64_t)(pixel_col_dis_left)*(Float64_t)(j - car_logo_rect.x)//横坐标
										- (p_avm_param->p_avm_config_param->vehicle_param.vehicle_width / 2);
					world_coordinate[1] = (Float64_t)(pixel_row_dis_back)*(Float64_t)(car_logo_rect.y + car_logo_rect.height - i)
										- (p_avm_param->p_avm_config_param->vehicle_param.vehicle_length / 2);

				}
				if (j >= car_logo_rect.x + car_logo_rect.width)//右下角
				{
					/***********calculate world coordinate*********/
					world_coordinate[0] = (Float64_t)(pixel_col_dis_right)*(Float64_t)(j - car_logo_rect.x - car_logo_rect.width)
						+ (p_avm_param->p_avm_config_param->vehicle_param.vehicle_width / 2);
					world_coordinate[1] = (Float64_t)(pixel_row_dis_back)*(Float64_t)(car_logo_rect.y + car_logo_rect.height - i)
						- (p_avm_param->p_avm_config_param->vehicle_param.vehicle_length / 2);

				}
				if (j >= car_logo_rect.x&&j < car_logo_rect.x + car_logo_rect.width)
				{
					/***********calculate world coordinate*********/
					world_coordinate[0] = (Float64_t)(pixel_col_dis_middle)*(Float64_t)(j - car_logo_rect.x - car_logo_rect.width / 2);
					world_coordinate[1] = (Float64_t)(pixel_row_dis_back)*(Float64_t)(car_logo_rect.y + car_logo_rect.height - i)
						- (p_avm_param->p_avm_config_param->vehicle_param.vehicle_length / 2);
				}

				/********calculate weight************/
				//Float64_t weight_Float64_t = calculate_4_view_weight_fixed(CAM_REAR, i, j, world_coordinate, weight_2_dim, p_avm_param, result_param);
				//Float64_t weight_Float64_t = calculate_4_view_weight(CAM_FRONT, i, j, world_coordinate, weight_2_dim, p_avm_param, result_param);
				Float64_t weight_Float64_t = calculate_4_view_weight_curve(CAM_REAR, i, j, world_coordinate, weight_2_dim, p_avm_param, result_param);


				/***********world coordinate to pic coordinate**************/
				world_coordinate_to_pic_coordinate(CAM_REAR, world_coordinate, image_point, p_avm_param);//0 is front

				/********zip data and fill lut*****/
				LUT_shift_address = camera_size[0] + (i - (car_logo_rect.y + car_logo_rect.height)) * result_param.result_image_width * 2 + j * 2;
				zip_data(LUT_shift_address, image_point, weight_Float64_t, p_lut, CAM_REAR, left_right_flag);
			}
#endif
#endif
#if 1
			/*******************************************left view********************************/
			if (j < car_logo_rect.x)
			{

				if (i<= car_logo_rect.y)//
				{
					/***********calculate world coordinate*********/
					world_coordinate[0] = (Float64_t)(pixel_col_dis_left)*(Float64_t)(j - car_logo_rect.x)
										- (p_avm_param->p_avm_config_param->vehicle_param.vehicle_width / 2);
					world_coordinate[1] = (Float64_t)(pixel_row_dis_front)*(Float64_t)(car_logo_rect.y - i)
										+ (p_avm_param->p_avm_config_param->vehicle_param.vehicle_length / 2);

				}
				if (i >= car_logo_rect.y + car_logo_rect.height)//
				{
					/***********calculate world coordinate*********/
					world_coordinate[0] = (Float64_t)(pixel_col_dis_left)*(Float64_t)(j - car_logo_rect.x)//横坐标
										- (p_avm_param->p_avm_config_param->vehicle_param.vehicle_width / 2);
					world_coordinate[1] = (Float64_t)(pixel_row_dis_back)*(Float64_t)(car_logo_rect.y + car_logo_rect.height - i)
										- (p_avm_param->p_avm_config_param->vehicle_param.vehicle_length / 2);

				}
				if (i >= car_logo_rect.y&&i <= car_logo_rect.y + car_logo_rect.height)
				{
					/***********calculate world coordinate*********/
					world_coordinate[0] = (Float64_t)(pixel_col_dis_left)*(Float64_t)(j - car_logo_rect.x)
										- (p_avm_param->p_avm_config_param->vehicle_param.vehicle_width / 2);
					world_coordinate[1] = (Float64_t)(pixel_row_dis_middle)*(Float64_t)(car_logo_rect.y + car_logo_rect.height/2 - i);
				}

				/*********calculate weight*****/
				Float64_t weight_Float64_t;
				weight_Float64_t = 1.000000 - weight_2_dim[i*result_param.result_image_width + j];
				/***********world coordinate to pic coordinate**************/
				world_coordinate_to_pic_coordinate(CAM_LEFT, world_coordinate, image_point, p_avm_param);
				/********************************************zip data***************************************/
				LUT_shift_address = camera_size[0] + camera_size[1] + i * car_logo_rect.x * 2 + j * 2;


				zip_data(LUT_shift_address, image_point, weight_Float64_t, p_lut, CAM_LEFT, left_right_flag);
			}

			/*************************right view****************/
			if (j >= car_logo_rect.x + car_logo_rect.width)//+ junc_view_4_param.common_config.rear_blind_length / pixel_col_dis
			{
				/***********calculate world coordinate*********/
				if (i <= car_logo_rect.y)//
				{
					/***********calculate world coordinate*********/
					world_coordinate[0] = (Float64_t)(pixel_col_dis_right)*(Float64_t)(j - car_logo_rect.x - car_logo_rect.width)
						+ (p_avm_param->p_avm_config_param->vehicle_param.vehicle_width / 2);
					world_coordinate[1] = (Float64_t)(pixel_row_dis_front)*(Float64_t)(car_logo_rect.y - i)
						+ (p_avm_param->p_avm_config_param->vehicle_param.vehicle_length / 2);

				}
				if (i >= car_logo_rect.y + car_logo_rect.height)//右下角
				{
					/***********calculate world coordinate*********/
					world_coordinate[0] = (Float64_t)(pixel_col_dis_right)*(Float64_t)(j - car_logo_rect.x - car_logo_rect.width)
						+ (p_avm_param->p_avm_config_param->vehicle_param.vehicle_width / 2);
					world_coordinate[1] = (Float64_t)(pixel_row_dis_back)*(Float64_t)(car_logo_rect.y + car_logo_rect.height - i)
						- (p_avm_param->p_avm_config_param->vehicle_param.vehicle_length / 2);

				}
				if (i >= car_logo_rect.y&&i <= car_logo_rect.y + car_logo_rect.height)
				{
					/***********calculate world coordinate*********/
					world_coordinate[0] = (Float64_t)(pixel_col_dis_right)*(Float64_t)(j - car_logo_rect.x - car_logo_rect.width)
						+ (p_avm_param->p_avm_config_param->vehicle_param.vehicle_width / 2);
					world_coordinate[1] = (Float64_t)(pixel_row_dis_middle)*(Float64_t)(car_logo_rect.y + car_logo_rect.height / 2 - i);
				}
				/*********calculate weight ************/
				Float64_t weight_Float64_t;
				weight_Float64_t = 1.000000 - weight_2_dim[i*result_param.result_image_width + j];
				/***********world coordinate to pic coordinate**************/
				world_coordinate_to_pic_coordinate(CAM_RIGHT, world_coordinate, image_point, p_avm_param);
				/*******************zip data*******************/
				LUT_shift_address = camera_size[0] + camera_size[1] + camera_size[2]
					+ i * (result_param.result_image_width - car_logo_rect.x - car_logo_rect.width) * 2
					+ (j - car_logo_rect.x - car_logo_rect.width) * 2;
				zip_data(LUT_shift_address, image_point, weight_Float64_t, p_lut, CAM_RIGHT, left_right_flag);
			}
#endif
		}
	}

	free(weight_2_dim);//释放暂存的融合权重
#endif



#if CAR_DYNAMIC
	AVM_Error_Code_E error = ERROR_NONE;
	Junc_View_4_Param_S junc_view_4_param = p_avm_param->p_avm_config_param->avm_2d_param.junc_view_4_param;
	/*****public use*****/
	Float64_t world_coordinate[3];//world_coordinate x,y,z
	world_coordinate[2] = 0;
	Float64_t camera_coordinate[3];
	CvRect car_logo_rect = result_param.car_logo_rect;// the information of car
	int camera_size[4], LUT_shift_address;  //calculate front rear left and right shifting of LUT，计算lut表的地址偏移 
	int i, j;
	Float64_t image_point[2];//get position of wide_angle_picture 
	/**********calculate fusion equation,存放前后视图计算出来的融合权重 以便左右直接相减保证权重之和 为1**************/
	Float64_t* weight_2_dim = (Float64_t *)malloc(sizeof(Float64_t)*result_param.result_image_width*result_param.result_image_height);
	if (!weight_2_dim)
	{
		error = ERROR_MEMORY_MALLOC_FAIL;
		CHECK_ERROR(error);
	}
	memset(weight_2_dim, 0, sizeof(Float64_t)*result_param.result_image_width
		*result_param.result_image_height);

	/****calculate front rear left and right shifting of LUT****/
	for (Int32_t cam_id = 0; cam_id < CAMERA_NUM; cam_id++)//计算存储前后左右视图的lut表的地址偏移 
	{
		camera_size[cam_id] = result_param.lut_size[cam_id] * 2;
	}

	UInt32_t left_right_flag=0;
	/*******************/
	Float64_t pixel_row_dis = (p_avm_param->p_avm_config_param->vehicle_param.vehicle_length + junc_view_4_param.common_config.front_view_length + junc_view_4_param.common_config.rear_view_length)
		/ (Float64_t)(junc_view_4_param.result_big_param.result_image_height);//pixel distance
	Float64_t pixel_col_dis = (p_avm_param->p_avm_config_param->vehicle_param.vehicle_width + junc_view_4_param.common_config.left_view_length + junc_view_4_param.common_config.right_view_length)
		/ (Float64_t)(junc_view_4_param.result_big_param.result_image_width);//pixel distance
	/********************extend all over result image pixel****************************/
	for (i = 0; i < result_param.result_image_height; i++)
	{
		for (j = 0; j < result_param.result_image_width; j++)
		{
			/***********calculate world coordinate*********/
			calculate_world_coordinate(world_coordinate, i, j, p_avm_param);//世界坐标系建立在小车的正中心
			/************front view*******************************/
			if (i < car_logo_rect.y - junc_view_4_param.common_config.front_blind_length / pixel_row_dis)//前车有盲区所以
			{
				/********calculate weight************/
				Float64_t weight_Float64_t = calculate_4_view_weight_curve(CAM_FRONT, i, j, world_coordinate, weight_2_dim, p_avm_param, result_param);
					
				//Float64_t weight_Float64_t = calculate_4_view_weight(CAM_FRONT, i, j, world_coordinate, weight_2_dim, p_avm_param, result_param);

				/***********world coordinate to pic coordinate**************/
				world_coordinate_to_pic_coordinate(CAM_FRONT, world_coordinate, image_point, p_avm_param);//0 is front

				/********zip data and fill lut*****/
				LUT_shift_address = i * result_param.result_image_width * 2 + j * 2;
				zip_data(LUT_shift_address, image_point, weight_Float64_t, p_lut, CAM_FRONT, left_right_flag);

			}

			/*************rear view*********************************/
			if (i >= car_logo_rect.y + car_logo_rect.height + junc_view_4_param.common_config.rear_blind_length / pixel_row_dis)//
			{
				/********calculate weight************/
				Float64_t weight_Float64_t = calculate_4_view_weight_curve(CAM_REAR, i, j, world_coordinate, weight_2_dim, p_avm_param, result_param);
				//Float64_t weight_Float64_t = calculate_4_view_weight(CAM_REAR, i, j, world_coordinate, weight_2_dim, p_avm_param, result_param);

				/***********world coordinate to pic coordinate**************/
				world_coordinate_to_pic_coordinate(CAM_REAR, world_coordinate, image_point, p_avm_param);
				/************zip data *************/
				LUT_shift_address = camera_size[0] + (i - (car_logo_rect.y + car_logo_rect.height)) * result_param.result_image_width * 2 + j * 2;
				
				zip_data(LUT_shift_address, image_point, weight_Float64_t, p_lut, CAM_REAR, left_right_flag);
			}
#if 0
			/*******************************************left view********************************/
			if (j < car_logo_rect.x)
			{
				/*********calculate weight*****/
				Float64_t weight_Float64_t;
				weight_Float64_t = 1.000000 - weight_2_dim[i*result_param.result_image_width + j];
				/***********world coordinate to pic coordinate**************/
				world_coordinate_to_pic_coordinate(CAM_LEFT, world_coordinate, image_point, p_avm_param);
				/********************************************zip data***************************************/
				LUT_shift_address = camera_size[0] + camera_size[1] + i * car_logo_rect.x * 2 + j * 2;
				
				
				zip_data(LUT_shift_address, image_point, weight_Float64_t, p_lut, CAM_LEFT, left_right_flag);
			}
			/*************************right view****************/
			if (j >= car_logo_rect.x + car_logo_rect.width)//+ junc_view_4_param.common_config.rear_blind_length / pixel_col_dis
			{
				/*********calculate weight ************/
				Float64_t weight_Float64_t;
				weight_Float64_t = 1.000000 - weight_2_dim[i*result_param.result_image_width + j];
				/***********world coordinate to pic coordinate**************/
				world_coordinate_to_pic_coordinate(CAM_RIGHT, world_coordinate, image_point, p_avm_param);
				/*******************zip data*******************/
				LUT_shift_address = camera_size[0] + camera_size[1] + camera_size[2]
					+ i * (result_param.result_image_width - car_logo_rect.x - car_logo_rect.width) * 2
					+ (j - car_logo_rect.x - car_logo_rect.width) * 2;
				zip_data(LUT_shift_address, image_point, weight_Float64_t, p_lut, CAM_RIGHT, left_right_flag);
			}
#endif
		}
	}

	free(weight_2_dim);//释放暂存的融合权重
#endif
	return error;
}

