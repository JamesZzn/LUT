#include"generate_4_view_lut.h"
#include"resolve_data.h"


/***************/
/*
* Function Name:				calculate_weight_coordinate
* Function Function:			calculate_weight_coordinate
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
//这个函数体现着融合区域要随着配置
void calculate_weight_coordinate(IN Int32_t x, IN Int32_t y,
	OUT  Float32_t weight_coordinate[2], IN CvRect car_logo_rect, 
	IN P_Avm_Param_S p_avm_param, IN Int32_t cam_flag)
{
	
	if (cam_flag == 0)
	{
		weight_coordinate[0] = Float32_t(x - car_logo_rect.x);
		weight_coordinate[1] = Float32_t(car_logo_rect.y - y);//计算权重时 
	}
	if (cam_flag == 1)
	{
		weight_coordinate[0] = Float32_t(x - car_logo_rect.x - car_logo_rect.width);

		weight_coordinate[1] = Float32_t(car_logo_rect.y - y);//计算权重时 
	}
	if (cam_flag == 2)
	{
		weight_coordinate[0] = Float32_t(x - car_logo_rect.x);

		weight_coordinate[1] = Float32_t(car_logo_rect.y + car_logo_rect.height - y);//计算权重时 

	}
	if (cam_flag == 3)
	{
		weight_coordinate[0] = Float32_t(x - car_logo_rect.x - car_logo_rect.width);
		weight_coordinate[1] = Float32_t(car_logo_rect.y + car_logo_rect.height - y);//计算权重时 将权重坐标系建立在小车中心
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
Float32_t calculate_4_view_weight_curve(int camera_flag, int i, int j,
			Float32_t *world_coordinate, Float32_t *weight_2_dim, IN P_Avm_Param_S p_avm_param,
			IN Result_Size_Param_S result_param, int view_flag)
{
	Float32_t weight_Float32_t;
	CvRect car_logo_rect;
	Int32_t range_vertical;
	Int32_t range_0;
	Junc_View_4_Param_S junc_view_4_param;
	Float32_t fl_fusion_angle;
	Float32_t fr_fusion_angle;
	Float32_t rl_fusion_angle ;
	Float32_t rr_fusion_angle;
	if (view_flag == 0 )
	{
		junc_view_4_param = p_avm_param->p_avm_config_param->avm_2d_param.junc_view_4_param;
		range_0 = (Int32_t)(junc_view_4_param.common_config.parallel_range);//表示平行线的融合的范围
		range_vertical = (Int32_t)(junc_view_4_param.common_config.curve_range);//表示曲线的融合范围
		car_logo_rect = result_param.car_logo_rect;
		fl_fusion_angle = junc_view_4_param.common_config.fl_fusion_angle;
		fr_fusion_angle = junc_view_4_param.common_config.fr_fusion_angle;
		rl_fusion_angle = junc_view_4_param.common_config.rl_fusion_angle;
		rr_fusion_angle = junc_view_4_param.common_config.rr_fusion_angle;
	}
	if (view_flag == 1)//前三路
	{
		range_0 = (Int32_t)p_avm_param->p_avm_config_param
			->avm_2d_param.junc_view_3_front_param.common_config.parallel_range;//表示平行线的融合的范围
		range_vertical = (Int32_t)p_avm_param->p_avm_config_param
			->avm_2d_param.junc_view_3_front_param.common_config.curve_range;//表示曲线的融合范围
		car_logo_rect = result_param.car_logo_rect;
		fl_fusion_angle = p_avm_param->p_avm_config_param
			->avm_2d_param.junc_view_3_front_param.common_config.fl_fusion_angle;
		fr_fusion_angle = p_avm_param->p_avm_config_param
			->avm_2d_param.junc_view_3_front_param.common_config.fr_fusion_angle;
	}

	if (view_flag == 2)//后三路
	{
		range_0 = (Int32_t)p_avm_param->p_avm_config_param
			->avm_2d_param.junc_view_3_rear_param.common_config.parallel_range;//表示平行线的融合的范围
		range_vertical = (Int32_t)p_avm_param->p_avm_config_param
			->avm_2d_param.junc_view_3_rear_param.common_config.curve_range;//表示曲线的融合范围
		car_logo_rect = p_avm_param->p_avm_config_param
			->avm_2d_param.junc_view_3_rear_param.result_3_rear_param.car_logo_rect;

		rl_fusion_angle = p_avm_param->p_avm_config_param
			->avm_2d_param.junc_view_3_rear_param.common_config.rl_fusion_angle;
		rr_fusion_angle = p_avm_param->p_avm_config_param
			->avm_2d_param.junc_view_3_rear_param.common_config.rr_fusion_angle;

	}
	if (camera_flag == 0)//表示计算前镜头的图片展开
	{
		if (j<car_logo_rect.x || j>(car_logo_rect.x + car_logo_rect.width))
		{

			if (j<car_logo_rect.x&&i<=car_logo_rect.y)//计算左上角的融合区域
			{
#if 1
				Float32_t weight_coordinate_point[2];
				calculate_weight_coordinate(j, i, weight_coordinate_point,
					car_logo_rect, p_avm_param, 0);//这个函数里将融合时将区域朝上偏移以便避开盲区
				//Float32_t fl_fusion_angle = junc_view_4_param.common_config.fl_fusion_angle;
				/**********************************************三条平行线********************************/
				Float32_t k_ratio = tan((-fl_fusion_angle / 180)*PI);
				//由此得出融合的角平分线的直线方程: y=k_ratio * x
				Float32_t sin_temp = sin(((90 - fl_fusion_angle) / 180)*PI);
				Float32_t y_translation = (range_0 / 2) / sin_temp;//得出
				//由此得出融合的下界直线方程: y=k_ratio * x- y_translation
				//由此得出融合的上界直线方程: y=k_ratio * x+ y_translation
				/**********************************************一条垂线********************************/
				Float32_t sin_temp_vertical = sin(((90-fl_fusion_angle) / 180)*PI);
				Float32_t k_vertical = -1 / k_ratio;
				Float32_t temp_vertical = -range_vertical / sin_temp_vertical;
				//与角平分线垂直的直线是y=k_vertical*x-k_vertical*temp_vertical 
				Float32_t curve_x = (k_vertical*temp_vertical - y_translation) / (k_vertical - k_ratio);
				Float32_t curve_y = k_vertical*curve_x - k_vertical*temp_vertical;
				
				Float32_t a6 = (k_ratio*curve_x - curve_y) / (curve_x*curve_x);
				Float32_t b6 = k_ratio-2*a6*curve_x;
				//动点 的直线方y=k_vertical*x-k_vertical*weight_coordinate_point[0]+weight_coordinate_point[1]
				Float32_t b0 = -k_vertical*weight_coordinate_point[0] + weight_coordinate_point[1];
				//动点 的直线y=k_vertical*x+b0
				Float32_t border_x_0 = ((k_vertical - b6) 
					- sqrt((k_vertical - b6)*(k_vertical - b6) + 4 * a6*b0)) / (2 * a6);
				Float32_t border_x_1 = ((k_vertical - b6) 
					+ sqrt((k_vertical - b6)*(k_vertical - b6) + 4 * a6*b0)) / (2 * a6);
				Float32_t border_y_0 = k_vertical*border_x_0 + b0;
				Float32_t border_y_1 = k_vertical*border_x_1 + b0;
				if (weight_coordinate_point[1] - k_vertical*weight_coordinate_point[0]
					+ k_vertical*temp_vertical > 0)
				{
					Float32_t distance = abs(weight_coordinate_point[1] - k_ratio 
						* weight_coordinate_point[0] + y_translation)
						/ sqrt(k_ratio*k_ratio + 1);
					weight_Float32_t = distance / range_0;
					if (weight_Float32_t >= 1)
					{
						weight_Float32_t = 1;
					}
					//weight_Float32_t = 1;
					if (weight_coordinate_point[1] - k_ratio * weight_coordinate_point[0] 
						+ y_translation <= 0)
					{
						weight_Float32_t = 0;//表示当位于下界直线下方时的权重
						
					}
				}
#if 1
				if (weight_coordinate_point[1] - k_vertical*weight_coordinate_point[0] 
					+ k_vertical*temp_vertical <0)
				{
					if (weight_coordinate_point[0] < border_x_0)
					{
						weight_Float32_t = 0;
					}
					else
					{
						Float32_t distance_0 = sqrt((weight_coordinate_point[0] 
							- border_x_0)*(weight_coordinate_point[0] - border_x_0)
							+ (weight_coordinate_point[1] - border_y_0)
							*(weight_coordinate_point[1] - border_y_0));
						Float32_t all_distance =2*abs(border_y_0 - k_ratio*border_x_0) 
							/ sqrt(1 + k_ratio*k_ratio);
						weight_Float32_t = distance_0 / all_distance;
						if (weight_Float32_t > 1)
						{
							weight_Float32_t = 1;
						}
					}

				}
#endif
#endif
			}
			if (j > car_logo_rect.x + car_logo_rect.width&&i <= car_logo_rect.y)//计算右上角的融合区域
			{
#if 1
				Float32_t weight_coordinate_point[2];
				calculate_weight_coordinate(j, i, weight_coordinate_point, car_logo_rect,p_avm_param,1);
				//Float32_t fr_fusion_angle = junc_view_4_param.common_config.fr_fusion_angle;
				/**********************************************三条平行线********************************/
				Float32_t k_ratio = tan((fr_fusion_angle / 180)*PI);
				//由此得出融合的角平分线的直线方程: y=k_ratio * x
				Float32_t sin_temp = sin(((90 - fr_fusion_angle) / 180)*PI);
				Float32_t y_translation = (range_0 / 2) / sin_temp;//得出
				/*************************************************一条垂线***************************************/
				Float32_t sin_temp_vertical = sin(((90 - fr_fusion_angle )/ 180)*PI);
				Float32_t k_vertical = -1 / k_ratio;
				Float32_t temp_vertical = range_vertical / sin_temp_vertical;
				//与角平分线垂直的直线是y=k_vertical*x-k_vertical*temp_vertical 
				Float32_t curve_x = ( y_translation - k_vertical*temp_vertical ) 
					/ ( k_ratio-k_vertical );
				Float32_t curve_y = k_vertical*curve_x - k_vertical*temp_vertical;
				//以小车矩形右上角为原点建立坐标系得y=a0*x^2+b0*x;
				//导函数y=2*a0*x+b0
				//由在2*a0*curve_x+b0=k_ratio
				//a0*curve_x^2+b0*curve_x=curve_y;
				Float32_t a6 = (k_ratio*curve_x - curve_y) / (curve_x*curve_x);
				Float32_t b6 = k_ratio-2*a6*curve_x;
				//动点 的直线方y=k_vertical*x-k_vertical*weight_coordinate_point[0]+weight_coordinate_point[1]
				Float32_t b0 = -k_vertical*weight_coordinate_point[0] + weight_coordinate_point[1];
				//动点 的直线y=k_vertical*x+b0
				Float32_t border_x_0 = ((k_vertical - b6) 
					- sqrt((k_vertical - b6)*(k_vertical - b6) + 4 * a6*b0)) / (2 * a6);
				Float32_t border_x_1 = ((k_vertical - b6) 
					+ sqrt((k_vertical - b6)*(k_vertical - b6) + 4 * a6*b0)) / (2 * a6);

				Float32_t border_y_0 = k_vertical*border_x_0 + b0;
				Float32_t border_y_1 = k_vertical*border_x_1 + b0;
#if 1
				if (weight_coordinate_point[1] - k_vertical*weight_coordinate_point[0] 
					+ k_vertical*temp_vertical > 0)//在平行线融合区域内
				{
					Float32_t distance = abs(weight_coordinate_point[1] 
						- k_ratio * weight_coordinate_point[0] + y_translation)
										/ sqrt(k_ratio*k_ratio + 1);
					weight_Float32_t = distance / range_0;
					if (weight_Float32_t >= 1)
					{
						weight_Float32_t = 1;
					}
					if (weight_coordinate_point[1] - k_ratio * weight_coordinate_point[0] 
						+ y_translation <= 0)
					{
						weight_Float32_t = 0;//表示当位于下界直线下方时的权重

					}
				}
#endif
#if 1
				if (weight_coordinate_point[1] - k_vertical*weight_coordinate_point[0] 
					+ k_vertical*temp_vertical <0)
				{
					//在曲线的融合区域内求出每个点的直线方程来
					if (weight_coordinate_point[0] > border_x_1)
					{
						weight_Float32_t = 0;
					}
					else
					{
						Float32_t distance_0 = sqrt((weight_coordinate_point[0] 
							- border_x_1)*(weight_coordinate_point[0] - border_x_1)
							+ (weight_coordinate_point[1] - border_y_1)
							*(weight_coordinate_point[1] - border_y_1));
						Float32_t all_distance = 2 * abs(border_y_1 - k_ratio*border_x_1) 
							/ sqrt(1 + k_ratio*k_ratio);
						weight_Float32_t = distance_0 / all_distance;
						if (weight_Float32_t > 1)
						{
							weight_Float32_t = 1;
						}
					}

				}
#endif
#endif

			}
		}
		else
		{
			weight_Float32_t = 1.000;
		}
		weight_2_dim[i*result_param.result_image_width 
			+ j] = weight_Float32_t;//storage 2 dim weighjt
		return	weight_Float32_t;
	}
#if 1
	if (camera_flag == 1)//表示计算后镜头的图片展开
	{
		if (j<car_logo_rect.x || j>(car_logo_rect.x + car_logo_rect.width))
		{

			if (j<car_logo_rect.x&&i>=car_logo_rect.y 
				+ car_logo_rect.height)//计算左下角的融合区域
			{
#if 1
				Float32_t weight_coordinate_point[2];
				calculate_weight_coordinate(j, i, weight_coordinate_point,
					car_logo_rect, p_avm_param,2);//此处的2表示是左下角
				//Float32_t rl_fusion_angle = junc_view_4_param.common_config.rl_fusion_angle;
				/**********************************************三条平行线********************************/
				Float32_t k_ratio = tan((rl_fusion_angle / 180)*PI);
				//由此得出融合的角平分线的直线方程: y=k_ratio * x
				Float32_t sin_temp = sin(((90 - rl_fusion_angle) / 180)*PI);
				Float32_t y_translation = (range_0 / 2) / sin_temp;//得出
				//由此得出融合的下界直线方程: y=k_ratio * x- y_translation
				//由此得出融合的上界直线方程: y=k_ratio * x+ y_translation
				/*************************************************一条垂线***************************************/
				Float32_t sin_temp_vertical = sin(((90 - rl_fusion_angle) / 180)*PI);
				Float32_t k_vertical = -1 / k_ratio;
				Float32_t temp_vertical = -range_vertical / sin_temp_vertical;
				//与角平分线垂直的直线是y=k_vertical*x-k_vertical*temp_vertical 
				Float32_t curve_x = (y_translation + k_vertical*temp_vertical) 
					/ (k_vertical-k_ratio  );//垂线与两条上下界直线的交点
				Float32_t curve_y = k_vertical*curve_x - k_vertical*temp_vertical;
				//以小车矩形左上角为原点建立坐标系得y=a0*x^2+b0*x;
				//导函数y=2*a0*x+b0
				//由在2*a0*curve_x+b0=k_ratio
				//a0*curve_x^2+b0*curve_x=curve_y;
				Float32_t a6 = (k_ratio*curve_x - curve_y) / (curve_x*curve_x);
				Float32_t b6 = k_ratio - 2 * a6*curve_x;
				//动点 的直线方y=k_vertical*x-k_vertical*weight_coordinate_point[0]+weight_coordinate_point[1]
				Float32_t b0 = -k_vertical*weight_coordinate_point[0] + weight_coordinate_point[1];
				//动点 的直线y=k_vertical*x+b0
				Float32_t border_x_0 = ((k_vertical - b6) + sqrt((k_vertical - b6)
					*(k_vertical - b6) + 4 * a6*b0)) / (2 * a6);
				Float32_t border_x_1 = ((k_vertical - b6) - sqrt((k_vertical - b6)
					*(k_vertical - b6) + 4 * a6*b0)) / (2 * a6);
				Float32_t border_y_0 = k_vertical*border_x_0 + b0;
				Float32_t border_y_1 = k_vertical*border_x_1 + b0;
#if 1
				if (weight_coordinate_point[1] - k_vertical*weight_coordinate_point[0] 
					+ k_vertical*temp_vertical < 0)//在平行线融合区域内
				{
					Float32_t distance = abs(weight_coordinate_point[1] - k_ratio 
						* weight_coordinate_point[0] - y_translation)//求出与上界直线的距离
						/ sqrt(k_ratio*k_ratio + 1);
					weight_Float32_t = distance / range_0;
					if (weight_Float32_t >= 1)
					{
						weight_Float32_t = 1;
					}
					if (weight_coordinate_point[1] - k_ratio * weight_coordinate_point[0] - y_translation >= 0)
					{
						weight_Float32_t = 0;//表示当位于下界直线下方时的权重

					}
				}
#endif
#if 1
				if (weight_coordinate_point[1] - k_vertical*weight_coordinate_point[0]
					+ k_vertical*temp_vertical >0)
				{
					//在曲线的融合区域内求出每个点的直线方程来
					//Float32_t in_curve_temp = k_vertical*weight_coordinate_point[0] - weight_coordinate_point[1];
					if (weight_coordinate_point[0] < border_x_0)
					{
						weight_Float32_t = 0;

					}
					else
					{
						Float32_t distance_0 = sqrt((weight_coordinate_point[0] 
							- border_x_0)*(weight_coordinate_point[0] - border_x_0)
							+ (weight_coordinate_point[1] - border_y_0)
							*(weight_coordinate_point[1] - border_y_0));//求出此动点到曲线边界的距离
						Float32_t all_distance = 2 * abs(border_y_0 - k_ratio*border_x_0)
							/ sqrt(1 + k_ratio*k_ratio);
						weight_Float32_t = distance_0 / all_distance;
						if (weight_Float32_t > 1)
						{
							weight_Float32_t = 1;
						}
					}
				}
#endif
#endif
			}
			if (j > car_logo_rect.x + car_logo_rect.width&&i>=car_logo_rect.y 
				+ car_logo_rect.height)//计算右下角的融合区域
			{
#if 1
				Float32_t weight_coordinate_point[2];
				calculate_weight_coordinate(j, i, weight_coordinate_point, car_logo_rect, p_avm_param,3);
				//Float32_t rr_fusion_angle = junc_view_4_param.common_config.rr_fusion_angle;
				
				/*************************************三条平行线***********************************/
				Float32_t k_ratio = tan((-rr_fusion_angle / 180)*PI);
				//由此得出融合的角平分线的直线方程: y=k_ratio * x
				Float32_t sin_temp = sin(((90 - rr_fusion_angle) / 180)*PI);
				Float32_t y_translation = (range_0 / 2) / sin_temp;//得出
				//由此得出融合的下界直线方程: y=k_ratio * x- y_translation
				//由此得出融合的上界直线方程: y=k_ratio * x+ y_translation
				/******************************************一条垂线***************************************/
				Float32_t sin_temp_vertical = sin(((90-rr_fusion_angle) / 180)*PI);
				Float32_t k_vertical = -1 / k_ratio;
				Float32_t temp_vertical = range_vertical / sin_temp_vertical;//垂线与横坐标的交点
				//与角平分线垂直的直线是y=k_vertical*x-k_vertical*temp_vertical 
				Float32_t curve_x = (k_vertical*temp_vertical + y_translation) / (k_vertical - k_ratio);//求出垂涎于上界线的交点以便 确定直线
				Float32_t curve_y = k_vertical*curve_x - k_vertical*temp_vertical;
				//以小车矩形左上角为原点建立坐标系得y=a0*x^2+b0*x;
				//导函数y=2*a0*x+b0
				//由在2*a0*curve_x+b0=k_ratio
				//a0*curve_x^2+b0*curve_x=curve_y;
				Float32_t a6 = (k_ratio*curve_x - curve_y) / (curve_x*curve_x);
				Float32_t b6 = k_ratio - 2 * a6*curve_x;
				//动点 的直线方y=k_vertical*x-k_vertical*weight_coordinate_point[0]+weight_coordinate_point[1]
				Float32_t b0 = -k_vertical*weight_coordinate_point[0] + weight_coordinate_point[1];
				//动点 的直线y=k_vertical*x+b0
				Float32_t border_x_0 = ((k_vertical - b6) - sqrt((k_vertical - b6)
					*(k_vertical - b6) + 4 * a6*b0)) / (2 * a6);
				Float32_t border_x_1 = ((k_vertical - b6) + sqrt((k_vertical - b6)
					*(k_vertical - b6) + 4 * a6*b0)) / (2 * a6);
				Float32_t border_y_0 = k_vertical*border_x_0 + b0;
				Float32_t border_y_1 = k_vertical*border_x_1 + b0;
				if (weight_coordinate_point[1] - k_vertical*weight_coordinate_point[0]
					+ k_vertical*temp_vertical < 0)
				{
					Float32_t distance = abs(weight_coordinate_point[1] - k_ratio 
						* weight_coordinate_point[0] - y_translation)
						/ sqrt(k_ratio*k_ratio + 1);
					weight_Float32_t = distance / range_0;
					if (weight_Float32_t >= 1)
					{
						weight_Float32_t = 1;
					}
					if (weight_coordinate_point[1] - k_ratio * weight_coordinate_point[0] 
						- y_translation >= 0)
					{
						weight_Float32_t = 0;//表示当位于上界界直线上方时的权重

					}
				}
#if 1
				if (weight_coordinate_point[1] - k_vertical*weight_coordinate_point[0] 
					+ k_vertical*temp_vertical>0)
				{
					//在曲线的融合区域内求出每个点的直线方程来
					//Float32_t in_curve_temp = k_vertical*weight_coordinate_point[0] - weight_coordinate_point[1];
					if (weight_coordinate_point[0] > border_x_0)
					{
						weight_Float32_t = 0;
					}
					else
					{
						Float32_t distance_0 = sqrt((weight_coordinate_point[0] - border_x_0)
							*(weight_coordinate_point[0] - border_x_0)
							+ (weight_coordinate_point[1] - border_y_0)*(weight_coordinate_point[1] - border_y_0));
						Float32_t all_distance =2*abs(border_y_0 - k_ratio*border_x_0) / sqrt(1 + k_ratio*k_ratio);
						weight_Float32_t = distance_0 / all_distance;
						if (weight_Float32_t > 1)
						{
							weight_Float32_t = 1;
						}
					}
				}
#endif
#endif
			}
		}
		else
		{
			weight_Float32_t = 1.000;
		}
		weight_2_dim[i*result_param.result_image_width 
			+ j] = weight_Float32_t;//storage 2 dim weighjt
		return	weight_Float32_t;
	}
#endif

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
void calculate_world_coordinate(OUT Float32_t *world_coordinate, IN int i, IN int j, IN P_Avm_Param_S p_avm_param, Result_Size_Param_S result_param)
{
	Junc_View_4_Param_S junc_view_4_param = p_avm_param->p_avm_config_param->avm_2d_param.junc_view_4_param;
	CvRect car_logo_rect = result_param.car_logo_rect;
	Float32_t pixel_row_dis = (p_avm_param->p_avm_config_param->vehicle_param.vehicle_length 
						+ junc_view_4_param.common_config.front_view_length 
							+ junc_view_4_param.common_config.rear_view_length)
								/ (Float32_t)(result_param.result_image_height);//calculate every pixel distance
	Float32_t pixel_col_dis = (p_avm_param->p_avm_config_param->vehicle_param.vehicle_width 
						+ junc_view_4_param.common_config.left_view_length 
							+ junc_view_4_param.common_config.right_view_length)
								/ (Float32_t)(result_param.result_image_width);//calculate every pixel distance

	world_coordinate[0] = (Float32_t)(pixel_col_dis)*(Float32_t)((-(car_logo_rect.x+ car_logo_rect.width / 2)) + j);
	world_coordinate[1] = (Float32_t)(pixel_row_dis)*(Float32_t)(car_logo_rect.y  + car_logo_rect.height / 2 - i);
	
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
//返回值image_point[1]是x即列数，image_point[0]是y即行数数，
AVM_Error_Code_E world_coordinate_to_pic_coordinate(IN int camera_id, IN Float32_t * world_coordinate,
	OUT Float32_t *image_point, IN P_Avm_Param_S p_avm_param)
{
	AVM_Error_Code_E error = ERROR_NONE;
	Float32_t camera_coordinate[3];
	Float32_t width = (Float32_t)(p_avm_param->p_avm_config_param
		->camera_model[camera_id].camera_model_int.width);
	Float32_t  height = (Float32_t)(p_avm_param->p_avm_config_param
		->camera_model[camera_id].camera_model_int.height);

	Mat_Mul_Vec_3by3_DB(camera_coordinate, p_avm_param->p_avm_config_param
		->camera_model[camera_id].camera_model_ext.inv_R, world_coordinate);
	for (int i = 3; i < 6; i++)
	{
		camera_coordinate[i - 3] = p_avm_param->p_avm_config_param->
			camera_model[camera_id].camera_model_ext.inv_pose[i]
			+ camera_coordinate[i - 3];
	}
	Float32_t tmp = 0;
	tmp = camera_coordinate[0];
	camera_coordinate[0] = camera_coordinate[1];
	camera_coordinate[1] = tmp;
	camera_coordinate[2] = -camera_coordinate[2];
	Cam_Ray_To_Image_Point(image_point, camera_coordinate, &(p_avm_param->p_avm_config_param
		->camera_model[camera_id].camera_model_int));

	image_point[0] = image_point[0] < 0 ? 0 : image_point[0];
	image_point[1] = image_point[1] < 0 ? 0 : image_point[1];

	image_point[1] = image_point[1] >width - 1 ? width - 1 : image_point[1];
	image_point[0] = image_point[0] >height - 1 ? height - 1 : image_point[0];


	if (image_point[0]> p_avm_param->p_avm_config_param
		->camera_model[camera_id].camera_model_int.height
		|| image_point[1]> p_avm_param->p_avm_config_param
		->camera_model[camera_id].camera_model_int.width)//错误码如果世界坐标系计算出来不在合理范围内提示出错 
	{
		printf("计算出的鱼眼原图坐标超出范围！！相机编号%d:",camera_id);
		printf("%lf,%lf\n", image_point[0], image_point[1]);
		//error = ERROR_WORLD_COORDINATE;
		//CHECK_ERROR(error);
	}
	return error;
}



/*****************************zip data**************************/
/*
* Function Name:				zip_data
* Function Function:			zip_data
* Input:
*       shift_altogether;       this is the position you should storage
*		image_point;			the picture coordinate
*       weight_Float32_t;          the weight of pixel
* Note:
*
* Revision:
*     Create by Cheng.Li on 2018/1/12.
*/
AVM_Error_Code_E  zip_data_new(IN int shift_altogether, IN Float32_t *image_point, IN Float32_t weight_Float32_t,
	IN UInt32_t*p_lut, IN UInt32_t camera_flag, IN UInt32_t left_right_flag, IN P_Avm_Param_S p_avm_param)
{
	AVM_Error_Code_E error = ERROR_NONE;
	UInt32_t u_int = (UInt32_t)image_point[0], v_int = (UInt32_t)image_point[1];   //get integer'part of position
	Float32_t u_Float32_t = image_point[0] - u_int, v_Float32_t = image_point[1] - v_int; // get decimal'partr of position
	UInt32_t u_point_int = (UInt32_t)u_Float32_t * 255, v_point_int = (UInt32_t)v_Float32_t * 255;
	UInt32_t temp_shift = 0;
	UInt32_t temp_shift0 = 0;
	UInt32_t temp_weight;     //one pixel's weight in dst_pic
	UInt32_t white_balance = 0;
	temp_weight = (UInt32_t)(weight_Float32_t * 255);
	if (weight_Float32_t == 1.000)
	{
		white_balance = 0;
	}

#if 1
	/************为了将四个角的权重都提前计算出来****************/
	Float32_t weightUpLeft, weightUpRight, weightDownLeft, weightDownRight;
	Int32_t width = p_avm_param->p_avm_image->source_image_array[0]->width;
	Int32_t height = p_avm_param->p_avm_image->source_image_array[0]->height;
	Int32_t x0 = (Int32_t)image_point[1];
	Int32_t x1 = (Int32_t)image_point[1] + 1;
	Int32_t y0 = (Int32_t)image_point[0];
	Int32_t y1 = (Int32_t)image_point[0] + 1;
	

	x0 = x0 < 0 ? 0 : x0;
	x1 = x1 < 0 ? 0 : x1;
	y0 = y0 < 0 ? 0 : y0;
	y1 = y1 < 0 ? 0 : y1;

	x0 = x0 > width - 1 ? width - 1 : x0;
	x1 = x1 > width - 1 ? width - 1 : x1;
	y0 = y0 > height - 1 ? height - 1 : y0;
	y1 = y1 > height - 1 ? height - 1 : y1;

	weightUpLeft = (x1 - image_point[1]) * (y1 - image_point[0]);
	weightUpRight = (image_point[1]-x0 ) * (y1 - image_point[0] );

	weightDownLeft = (x1 - image_point[1]) * (image_point[0]-y0);
	weightDownRight = 1 - weightUpLeft - weightUpRight - weightDownLeft;
#endif
	UInt32_t weightUpLeft_uint = (UInt32_t)(weightUpLeft * 255), weightUpRight_uint = (UInt32_t)(weightUpRight * 255),
		weightDownLeft_uint = (UInt32_t)(weightDownLeft * 255), weightDownRight_uint = (UInt32_t)(255 - weightUpLeft_uint - weightUpRight_uint - weightDownLeft_uint);
	UInt32_t fusion_flag;
	if (temp_weight == 255)
	{
		fusion_flag = 0;
	}
	else
	{

		fusion_flag = 1;
	}
	temp_shift = (u_int << 21) + (v_int << 10) + (temp_weight << 2) + (fusion_flag << 1) + left_right_flag;
	temp_shift0 = (weightUpLeft_uint << 24) + (weightUpRight_uint << 16)
		+ (weightDownLeft_uint << 8) + (weightDownRight_uint);
	p_lut[shift_altogether] = temp_shift;
	p_lut[shift_altogether + 1] = temp_shift0;
	return error;
}

/*****************************zip data**************************/
/*
* Function Name:				zip_data
* Function Function:			zip_data
* Input:
*       shift_altogether;       this is the position you should storage
*		image_point;			the picture coordinate
*       weight_Float32_t;          the weight of pixel
* Note:
*
* Revision:
*     Create by Cheng.Li on 2018/1/12.
*/
AVM_Error_Code_E  zip_data(IN int shift_altogether, IN Float32_t *image_point, IN Float32_t weight_Float32_t,
				IN UInt32_t*p_lut, IN UInt32_t camera_flag, IN UInt32_t left_right_flag)
{
	AVM_Error_Code_E error = ERROR_NONE;
	UInt32_t u_int = (UInt32_t)image_point[0], v_int = (UInt32_t)image_point[1];   //get integer'part of position
	Float32_t u_Float32_t = image_point[0] - u_int, v_Float32_t = image_point[1] - v_int; // get decimal'partr of position
	UInt32_t u_point_int = (UInt32_t)(u_Float32_t * 255), v_point_int = (UInt32_t)(v_Float32_t * 255);
	UInt32_t temp_shift = 0;
	UInt32_t temp_shift0 = 0;
	UInt32_t temp_weight;     //one pixel's weight in dst_pic
	UInt32_t white_balance = 0;
	temp_weight = (UInt32_t )( weight_Float32_t * 255);
	if (weight_Float32_t == 1.000)
	{
		white_balance = 0;
	}

	/************为了将四个角的权重都提前计算出来****************/
	temp_shift = (u_int << 21) + (v_int << 10) + (temp_weight << 2) + (camera_flag);
	temp_shift0 = (u_point_int << 24) + (v_point_int << 16) 
		+ (left_right_flag << 15) + (white_balance<<14);
	p_lut[shift_altogether] = temp_shift;
	p_lut[shift_altogether+1] = temp_shift0;
	return error;
}


/*
void bilinear_interpolation(IplImage* p_src_img,
Int32_t rgb[3],
Float32_t x_src_loc,
Float32_t y_src_loc
)
Float32_t weightUpLeft, weightUpRight, weightDownLeft, weightDownRight;
Int32_t x0 = (Int32_t)x_src_loc;
Int32_t x1 = (Int32_t)x_src_loc + 1;
Int32_t y0 = (Int32_t)y_src_loc;
Int32_t y1 = (Int32_t)y_src_loc + 1;
Int32_t channel_id;

x0 = x0 < 0 ? 0 : x0;
x1 = x1 < 0 ? 0 : x1;
y0 = y0 < 0 ? 0 : y0;
y1 = y1 < 0 ? 0 : y1;

x0 = x0 > p_src_img->width - 1 ? p_src_img->width - 1 : x0;
x1 = x1 > p_src_img->width - 1 ? p_src_img->width - 1 : x1;
y0 = y0 > p_src_img->height - 1 ? p_src_img->height - 1 : y0;
y1 = y1 > p_src_img->height - 1 ? p_src_img->height - 1 : y1;

weightUpLeft = (x1 - x_src_loc) * (y1 - y_src_loc);
weightUpRight = (x_src_loc - x0) * (y1 - y_src_loc);
weightDownLeft = (x1 - x_src_loc) * (y_src_loc - y0);
weightDownRight = 1 - weightUpLeft - weightUpRight - weightDownLeft;

*/




#if 1
//盲区和MATLAB 最最符合的一版
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
AVM_Error_Code_E generate_4_view_lut(IN P_Avm_Param_S p_avm_param, 
			IN Result_Size_Param_S result_param, IN UInt32_t* p_lut)
{
	
#if CAR_FIXED
	AVM_Error_Code_E error = ERROR_NONE;
	Junc_View_4_Param_S junc_view_4_param = p_avm_param
		->p_avm_config_param->avm_2d_param.junc_view_4_param;
	/*****public use*****/
	Float32_t world_coordinate[3];//world_coordinate x,y,z
	world_coordinate[2] = 0;
	
	CvRect car_logo_rect = result_param.car_logo_rect;// the information of car
	int camera_size[4], LUT_shift_address;  //calculate front rear left and right shifting of LUT，计算lut表的地址偏移 
	int i, j;
	Float32_t image_point[2];//get position of wide_angle_picture 
	/**********calculate fusion equation,存放前后视图计算出来的融合权重 以便左右直接相减保证权重之和 为1**************/
	Float32_t* weight_2_dim = (Float32_t *)malloc(sizeof(Float32_t)
		*result_param.result_image_width*result_param.result_image_height);
	if (!weight_2_dim)
	{
		error = ERROR_MEMORY_MALLOC_FAIL;
		CHECK_ERROR(error);
	}
	memset(weight_2_dim, 0, sizeof(Float32_t)*result_param.result_image_width
		*result_param.result_image_height);

	/****calculate front rear left and right shifting of LUT****/
	for (Int32_t cam_id = 0; cam_id < CAMERA_NUM; cam_id++)//计算存储前后左右视图的lut表的地址偏移 
	{
		camera_size[cam_id] = result_param.lut_size[cam_id] * 2;
	}
	UInt32_t left_right_flag = 0;
	/*******************/
	Float32_t pixel_row_dis_front = (junc_view_4_param.common_config.front_view_length)
		/ (Float32_t)(car_logo_rect.y);//pixel distance
	Float32_t pixel_row_dis_middle = (p_avm_param
		->p_avm_config_param->vehicle_param.vehicle_length )
		/ (Float32_t)(car_logo_rect.height);//pixel distance
	Float32_t pixel_row_dis_back = (junc_view_4_param.common_config.rear_view_length)
		/ (Float32_t)(result_param.result_image_height - car_logo_rect.height - car_logo_rect.y);//pixel distance
	Float32_t pixel_col_dis_left = (junc_view_4_param.common_config.left_view_length)
		/ (Float32_t)(car_logo_rect.x);//pixel distance
	Float32_t pixel_col_dis_middle = (p_avm_param->p_avm_config_param->vehicle_param.vehicle_width)
		/ (Float32_t)(car_logo_rect.width);//pixel distance
	Float32_t pixel_col_dis_right = (junc_view_4_param.common_config.right_view_length)
		/ (Float32_t)(result_param.result_image_width - car_logo_rect.width - car_logo_rect.x);//pixel distance
	/********************extend all over result image pixel************************/
	
	Int32_t result_image_height = junc_view_4_param.result_big_param.result_image_height;
	Int32_t result_image_width = junc_view_4_param.result_big_param.result_image_width;
	//后盲区的变量
	Int32_t back_blind_pix = (Int32_t)(junc_view_4_param.common_config.rear_blind_length / pixel_row_dis_back);
	Int32_t Back_height = result_image_height - (car_logo_rect.y + car_logo_rect.height);
	Int32_t Back_width = result_image_width;
	Int32_t cut_blind_height = Back_height - back_blind_pix;
	Int32_t add_blind_middle = car_logo_rect.height + back_blind_pix;
	Float32_t ratio_back = (Float32_t)cut_blind_height/ (Float32_t)Back_height ;
	Float32_t ratio_middle = (Float32_t)car_logo_rect.height/(Float32_t)add_blind_middle ;
	//前盲区的变量
	Int32_t Front_blind_pix = (Int32_t)(junc_view_4_param.common_config.front_blind_length / pixel_row_dis_front);
	Int32_t Front_height = car_logo_rect.y;
	Int32_t Front_width = result_image_width;
	Int32_t Front_cut_blind_height = Front_height - Front_blind_pix;
	Int32_t Front_add_blind_middle = car_logo_rect.height + Front_blind_pix;
	Float32_t Front_ratio_front = (Float32_t)Front_cut_blind_height 
		/ (Float32_t)Front_height;
	Float32_t Front_ratio_middle = (Float32_t)car_logo_rect.height 
		/ (Float32_t)Front_add_blind_middle;
	/***********计算错误码的变量************/
	Float32_t x_world_coor_max = p_avm_param->p_avm_config_param->vehicle_param.vehicle_width / 2 
		+ (junc_view_4_param.common_config.left_view_length)*1.3;
	Float32_t y_world_coor_max = p_avm_param->p_avm_config_param->vehicle_param.vehicle_length / 2 
		+ (junc_view_4_param.common_config.front_view_length)*1.3;

	Float32_t x_world_coor_min = p_avm_param->p_avm_config_param->vehicle_param.vehicle_width / 2 ;
	Float32_t y_world_coor_min = p_avm_param->p_avm_config_param->vehicle_param.vehicle_length / 2;
	for (i = 0; i < result_param.result_image_height; i++)
	{
		for (j = 0; j < result_param.result_image_width; j++)
		{
#if 1
			/************front view*******************************/
			if (i <= car_logo_rect.y)//
			{
				if (j <= car_logo_rect.x)//如果是左上角
				{
					int a0 = car_logo_rect.y-i;//专门为了盲区而实现
					Float32_t a1 = a0*Front_ratio_front;
					Float32_t a2 = a1 + Front_blind_pix;

					/***********calculate world coordinate*********/
					world_coordinate[0] = (Float32_t)(pixel_col_dis_left)*(Float32_t)(j - car_logo_rect.x)
						- (p_avm_param->p_avm_config_param->vehicle_param.vehicle_width / 2);
					world_coordinate[1] = (Float32_t)(pixel_row_dis_front)*(Float32_t)(a2)
						+ (p_avm_param->p_avm_config_param->vehicle_param.vehicle_length / 2);
				}
				if (j >= car_logo_rect.x + car_logo_rect.width)//
				{
					int a0 = car_logo_rect.y - i;//专门为了盲区而实现
					Float32_t a1 = a0*Front_ratio_front;
					Float32_t a2 = a1 + Front_blind_pix;
					/***********calculate world coordinate*********/
					world_coordinate[0] = (Float32_t)(pixel_col_dis_right)
						*(Float32_t)(j - car_logo_rect.x - car_logo_rect.width)
						+ (p_avm_param->p_avm_config_param->vehicle_param.vehicle_width / 2);
					world_coordinate[1] = (Float32_t)(pixel_row_dis_front)*(Float32_t)(a2)
						+ (p_avm_param->p_avm_config_param->vehicle_param.vehicle_length / 2);

				}
				if (j > car_logo_rect.x&&j < car_logo_rect.x + car_logo_rect.width)//如果是前部中间的非融合区域
				{
					int a0 = car_logo_rect.y - i;//专门为了盲区而实现
					Float32_t a1 = a0*Front_ratio_front;
					Float32_t a2 = a1 + Front_blind_pix;
					/***********calculate world coordinate*********/
					world_coordinate[0] = (Float32_t)(pixel_col_dis_middle)
						*(Float32_t)(j - car_logo_rect.x - car_logo_rect.width / 2);
					world_coordinate[1] = (Float32_t)(pixel_row_dis_front)*(Float32_t)(a2)
						+ (p_avm_param->p_avm_config_param->vehicle_param.vehicle_length / 2);
				}
				if (abs(world_coordinate[0])> x_world_coor_max
				||  abs(world_coordinate[1])>y_world_coor_max 
				||  (abs(world_coordinate[0])< x_world_coor_min &&abs(world_coordinate[1])< y_world_coor_min))//错误码如果世界坐标系计算出来不在合理范围内提示出错 
				{
					printf("世界坐标计算超出合理范围！！");
					error = ERROR_WORLD_COORDINATE;
					CHECK_ERROR(error);
				}


				/********calculate weight************/
				Float32_t weight_Float32_t = calculate_4_view_weight_curve(CAM_FRONT, i, 
							j, world_coordinate, weight_2_dim, p_avm_param, result_param,0);

				if (weight_Float32_t>1 || weight_Float32_t<0)
				{
					printf("融合系数超出合理范围！！");
					error = ERROR_FUSE_WEIGHT;
					CHECK_ERROR(error);
				}

				/***********world coordinate to pic coordinate**************/
				error = world_coordinate_to_pic_coordinate(CAM_FRONT, world_coordinate, image_point, p_avm_param);//0 is front
				CHECK_ERROR(error);
				/********zip data and fill lut*****/
				LUT_shift_address = i * result_param.result_image_width * 2 + j * 2;
				if (j < result_param.result_image_width / 2)
				{
					left_right_flag = 1;
				}
				//error = zip_data(LUT_shift_address, image_point, weight_Float32_t, p_lut, CAM_FRONT, left_right_flag);
				error = zip_data_new(LUT_shift_address, image_point, weight_Float32_t, p_lut, CAM_FRONT, left_right_flag, p_avm_param);
				CHECK_ERROR(error);
			
			}
#endif
#if 1
			/*************rear view*********************************/
			if (i >= car_logo_rect.y + car_logo_rect.height)//
			{
				int a0 =i- (car_logo_rect.y + car_logo_rect.height);//专门为了盲区而实现
				Float32_t a1 = a0*ratio_back;
				Float32_t a2 = a1 + back_blind_pix;
				if (j <= car_logo_rect.x)//左下角
				{
					/***********calculate world coordinate*********/
					world_coordinate[0] = (Float32_t)(pixel_col_dis_left)*(Float32_t)(j - car_logo_rect.x)//横坐标
						- (p_avm_param->p_avm_config_param->vehicle_param.vehicle_width / 2);
					world_coordinate[1] = (Float32_t)(pixel_row_dis_back)*(Float32_t)(-a2)
						- (p_avm_param->p_avm_config_param->vehicle_param.vehicle_length / 2);
				}
				if (j >= car_logo_rect.x + car_logo_rect.width)//右下角
				{
					
					/***********calculate world coordinate*********/
					world_coordinate[0] = (Float32_t)(pixel_col_dis_right)
						*(Float32_t)(j - car_logo_rect.x - car_logo_rect.width)
						+ (p_avm_param->p_avm_config_param->vehicle_param.vehicle_width / 2);
					world_coordinate[1] = (Float32_t)(pixel_row_dis_back)*(Float32_t)(-a2)
						- (p_avm_param->p_avm_config_param->vehicle_param.vehicle_length / 2 );

				}
				if (j >= car_logo_rect.x&&j < car_logo_rect.x + car_logo_rect.width)
				{
					/***********calculate world coordinate*********/
					world_coordinate[0] = (Float32_t)(pixel_col_dis_middle)
						*(Float32_t)(j - car_logo_rect.x - car_logo_rect.width / 2);
					world_coordinate[1] = (Float32_t)(pixel_row_dis_back)*(Float32_t)(-a2)
						- (p_avm_param->p_avm_config_param->vehicle_param.vehicle_length / 2 );
				}
				if (abs(world_coordinate[0])> x_world_coor_max
					|| abs(world_coordinate[1])>y_world_coor_max
					|| (abs(world_coordinate[0])< x_world_coor_min &&abs(world_coordinate[1])< y_world_coor_min))//错误码如果世界坐标系计算出来不在合理范围内提示出错 
				{
					printf("世界坐标计算超出合理范围！！");
					error = ERROR_WORLD_COORDINATE;
					CHECK_ERROR(error);
				}
				/********calculate weight************/
				Float32_t weight_Float32_t = calculate_4_view_weight_curve(CAM_REAR, i,
					j, world_coordinate, weight_2_dim, p_avm_param, result_param,0);
				if (weight_Float32_t>1 || weight_Float32_t<0)
				{
					printf("融合系数超出合理范围！！");
					error = ERROR_FUSE_WEIGHT;
					CHECK_ERROR(error);
				}
				/***********world coordinate to pic coordinate**************/
				error = world_coordinate_to_pic_coordinate(CAM_REAR, world_coordinate, image_point, p_avm_param);//0 is front
				CHECK_ERROR(error);
				/********zip data and fill lut*****/
				LUT_shift_address = camera_size[0] 
					+ (i - (car_logo_rect.y + car_logo_rect.height))* result_param.result_image_width * 2 + j * 2;
				if (j > result_param.result_image_width / 2)
				{
					left_right_flag = 1;
				}
				//error = zip_data(LUT_shift_address, image_point, weight_Float32_t, p_lut, CAM_FRONT, left_right_flag);
				error = zip_data_new(LUT_shift_address, image_point, weight_Float32_t, p_lut, CAM_REAR, left_right_flag, p_avm_param);
				CHECK_ERROR(error);
			}
#endif
#if 1
			/*******************************************left view********************************/
			if (j < car_logo_rect.x)
			{

				if (i <= car_logo_rect.y)//因为加了盲区所以左上角跟着向上移动
				{
					int a0 = car_logo_rect.y - i;//专门为了盲区而实现
					Float32_t a1 = a0*Front_ratio_front;
					Float32_t a2 = a1 + Front_blind_pix;

					/***********calculate world coordinate*********/
					world_coordinate[0] = (Float32_t)(pixel_col_dis_left)*(Float32_t)(j - car_logo_rect.x)
						- (p_avm_param->p_avm_config_param->vehicle_param.vehicle_width / 2);
					world_coordinate[1] = (Float32_t)(pixel_row_dis_front)*(Float32_t)(a2)
						+ (p_avm_param->p_avm_config_param->vehicle_param.vehicle_length / 2);
				}
				if (i >= car_logo_rect.y + car_logo_rect.height)//左后半部
				{
					int a0 = i - (car_logo_rect.y + car_logo_rect.height);//专门为了盲区而实现
					Float32_t a1 = a0*ratio_back;
					Float32_t a2 = a1 + back_blind_pix;
					/***********calculate world coordinate*********/
					world_coordinate[0] = (Float32_t)(pixel_col_dis_left)*(Float32_t)(j - car_logo_rect.x)//横坐标
						- (p_avm_param->p_avm_config_param->vehicle_param.vehicle_width / 2);
					world_coordinate[1] = (Float32_t)(pixel_row_dis_back)*(Float32_t)(-a2)
						- (p_avm_param->p_avm_config_param->vehicle_param.vehicle_length / 2 );

				}
				if (i >= car_logo_rect.y&&i <= car_logo_rect.y + car_logo_rect.height) /*+ back_blind_pix*///中间部
				{
					Float32_t pixel_row_dis_middle_blind = (p_avm_param->p_avm_config_param->vehicle_param.vehicle_length / 2 
						+ junc_view_4_param.common_config.rear_blind_length)
						/ (Float32_t)(car_logo_rect.height/2);//pixel distance; pixel_row_dis_middle_blind 变量名应该写成 pixel_row_dis_middle_blind_rear
					Float32_t pixel_row_dis_middle_blind_front = (p_avm_param->p_avm_config_param->vehicle_param.vehicle_length / 2
						+ junc_view_4_param.common_config.front_blind_length)
						/ (Float32_t)(car_logo_rect.height / 2);//pixel distance;
					/***********calculate world coordinate*********/
					if (i > car_logo_rect.y + car_logo_rect.height / 2)
					{
						world_coordinate[0] = (Float32_t)(pixel_col_dis_left)*(Float32_t)(j - car_logo_rect.x)
							- (p_avm_param->p_avm_config_param->vehicle_param.vehicle_width / 2);
						world_coordinate[1] = (Float32_t)(pixel_row_dis_middle_blind)
							*(Float32_t)(car_logo_rect.y + car_logo_rect.height / 2 - i);
					}
					else
					{
						world_coordinate[0] = (Float32_t)(pixel_col_dis_left)*(Float32_t)(j - car_logo_rect.x)
							- (p_avm_param->p_avm_config_param->vehicle_param.vehicle_width / 2);
						world_coordinate[1] = (Float32_t)(pixel_row_dis_middle_blind_front)
							*(Float32_t)(car_logo_rect.y + car_logo_rect.height / 2 - i);
					}
				}
				if (abs(world_coordinate[0])> x_world_coor_max
					|| abs(world_coordinate[1])>y_world_coor_max
					|| (abs(world_coordinate[0])< x_world_coor_min &&abs(world_coordinate[1])< y_world_coor_min))//错误码如果世界坐标系计算出来不在合理范围内提示出错 
				{
					printf("世界坐标计算超出合理范围！！");
					error = ERROR_WORLD_COORDINATE;
					CHECK_ERROR(error);
				}
				/*********calculate weight*****/
				Float32_t weight_Float32_t;
				weight_Float32_t = 1.000000 - weight_2_dim[i*result_param.result_image_width + j];
				if (weight_Float32_t>1 || weight_Float32_t<0)
				{
					printf("融合系数超出合理范围！！");
					error = ERROR_FUSE_WEIGHT;
					CHECK_ERROR(error);
				}
				/***********world coordinate to pic coordinate**************/
				error=world_coordinate_to_pic_coordinate(CAM_LEFT, world_coordinate, image_point, p_avm_param);
				CHECK_ERROR(error);
				/********************************************zip data***************************************/
				LUT_shift_address = camera_size[0] + camera_size[1] + i * car_logo_rect.x * 2 + j * 2;
				if (i > result_param.result_image_height / 2)
				{
					left_right_flag = 1;
				}
				//error = zip_data(LUT_shift_address, image_point, weight_Float32_t, p_lut, CAM_FRONT, left_right_flag);
				error = zip_data_new(LUT_shift_address, image_point, weight_Float32_t, p_lut, CAM_LEFT, left_right_flag, p_avm_param);
				CHECK_ERROR(error);
			}
			/*************************right view****************/
			if (j >= car_logo_rect.x + car_logo_rect.width)//
			{
				/***********calculate world coordinate*********/
				if (i <= car_logo_rect.y)//
				{
					int a0 = car_logo_rect.y - i;//专门为了盲区而实现
					Float32_t a1 = a0*Front_ratio_front;
					Float32_t a2 = a1 + Front_blind_pix;
					/***********calculate world coordinate*********/
					world_coordinate[0] = (Float32_t)(pixel_col_dis_right)
						*(Float32_t)(j - car_logo_rect.x - car_logo_rect.width)
						+ (p_avm_param->p_avm_config_param->vehicle_param.vehicle_width / 2);
					world_coordinate[1] = (Float32_t)(pixel_row_dis_front)*(Float32_t)(a2)
						+ (p_avm_param->p_avm_config_param->vehicle_param.vehicle_length / 2);

				}
				if (i >= car_logo_rect.y + car_logo_rect.height)//右下角
				{
					int a0 = i - (car_logo_rect.y + car_logo_rect.height);//专门为了盲区而实现
					Float32_t a1 = a0*ratio_back;
					Float32_t a2 = a1 + back_blind_pix;
					/***********calculate world coordinate*********/
					world_coordinate[0] = (Float32_t)(pixel_col_dis_right)
						*(Float32_t)(j - car_logo_rect.x - car_logo_rect.width)
						+ (p_avm_param->p_avm_config_param->vehicle_param.vehicle_width / 2);
					world_coordinate[1] = (Float32_t)(pixel_row_dis_back)*(Float32_t)(-a2)
						- (p_avm_param->p_avm_config_param->vehicle_param.vehicle_length / 2 );

				}
				if (i >= car_logo_rect.y&&i <= car_logo_rect.y + car_logo_rect.height)
				{
					Float32_t pixel_row_dis_middle_blind = (p_avm_param->p_avm_config_param->vehicle_param.vehicle_length / 2 
						+ junc_view_4_param.common_config.rear_blind_length)
						/ (Float32_t)(car_logo_rect.height / 2);//pixel distance;
					Float32_t pixel_row_dis_middle_blind_front = (p_avm_param->p_avm_config_param->vehicle_param.vehicle_length / 2
						+ junc_view_4_param.common_config.front_blind_length)
						/ (Float32_t)(car_logo_rect.height / 2);//pixel distance;
					/***********calculate world coordinate*********/
					if (i > car_logo_rect.y + car_logo_rect.height / 2)
					{
						world_coordinate[0] = (Float32_t)(pixel_col_dis_right)
							*(Float32_t)(j - car_logo_rect.x - car_logo_rect.width)
							+ (p_avm_param->p_avm_config_param->vehicle_param.vehicle_width / 2);
						world_coordinate[1] = (Float32_t)(pixel_row_dis_middle_blind)
							*(Float32_t)(car_logo_rect.y + car_logo_rect.height / 2 - i);
					}
					else
					{
						world_coordinate[0] = (Float32_t)(pixel_col_dis_right)
							*(Float32_t)(j - car_logo_rect.x - car_logo_rect.width)
							+ (p_avm_param->p_avm_config_param->vehicle_param.vehicle_width / 2);
						world_coordinate[1] = (Float32_t)(pixel_row_dis_middle_blind_front)
							*(Float32_t)(car_logo_rect.y + car_logo_rect.height / 2 - i);
					}
				}
				if (abs(world_coordinate[0])> x_world_coor_max
					|| abs(world_coordinate[1])>y_world_coor_max
					|| (abs(world_coordinate[0])< x_world_coor_min &&abs(world_coordinate[1])< y_world_coor_min))//错误码如果世界坐标系计算出来不在合理范围内提示出错 
				{
					printf("世界坐标计算超出合理范围！！");
					error = ERROR_WORLD_COORDINATE;
					CHECK_ERROR(error);
				}
				/*********calculate weight ************/
				Float32_t weight_Float32_t;
				weight_Float32_t = 1.000000 - weight_2_dim[i*result_param.result_image_width + j];
				if (weight_Float32_t>1 || weight_Float32_t<0)
				{
					printf("融合系数超出合理范围！！");
					error = ERROR_FUSE_WEIGHT;
					CHECK_ERROR(error);
				}
				/***********world coordinate to pic coordinate**************/
				error=world_coordinate_to_pic_coordinate(CAM_RIGHT, world_coordinate, image_point, p_avm_param);
				CHECK_ERROR(error);
				/*******************zip data*******************/
				LUT_shift_address = camera_size[0] + camera_size[1] + camera_size[2]
					+ i * (result_param.result_image_width - car_logo_rect.x - car_logo_rect.width) * 2
					+ (j - car_logo_rect.x - car_logo_rect.width) * 2;
				if (i < result_param.result_image_height / 2)
				{
					left_right_flag = 1;
				}
				error = zip_data_new(LUT_shift_address, image_point, weight_Float32_t, p_lut, CAM_RIGHT, left_right_flag, p_avm_param);
				//error = zip_data(LUT_shift_address, image_point, weight_Float32_t, p_lut, CAM_FRONT, left_right_flag);
				CHECK_ERROR(error);
			}
#endif
		}
	}
	free(weight_2_dim);//释放暂存的融合权重
#endif


	//当 宏定义为CAR_DYNAMIC，表示环视图中，得小车位置是自己随着视野范围变化的
#if CAR_DYNAMIC
	AVM_Error_Code_E error = ERROR_NONE;
	Junc_View_4_Param_S junc_view_4_param = p_avm_param->p_avm_config_param->avm_2d_param.junc_view_4_param;
	/*****public use*****/
	Float32_t world_coordinate[3];//world_coordinate x,y,z
	world_coordinate[2] = 0;
	Float32_t camera_coordinate[3];
	CvRect car_logo_rect = result_param.car_logo_rect;// the information of car
	int camera_size[4], LUT_shift_address;  //calculate front rear left and right shifting of LUT，计算lut表的地址偏移 
	int i, j;
	Float32_t image_point[2];//get position of wide_angle_picture 
	/**********calculate fusion equation,存放前后视图计算出来的融合权重 以便左右直接相减保证权重之和 为1**************/
	Float32_t* weight_2_dim = (Float32_t *)malloc(sizeof(Float32_t)*result_param.result_image_width*result_param.result_image_height);
	if (!weight_2_dim)
	{
		error = ERROR_MEMORY_MALLOC_FAIL;
		CHECK_ERROR(error);
	}
	memset(weight_2_dim, 0, sizeof(Float32_t)*result_param.result_image_width
		*result_param.result_image_height);

	/****calculate front rear left and right shifting of LUT****/
	for (Int32_t cam_id = 0; cam_id < CAMERA_NUM; cam_id++)//计算存储前后左右视图的lut表的地址偏移 
	{
		camera_size[cam_id] = result_param.lut_size[cam_id] * 2;
	}

	UInt32_t left_right_flag = 0;
	/*******************/
	Float32_t pixel_row_dis = (p_avm_param->p_avm_config_param->vehicle_param.vehicle_length 
		+ junc_view_4_param.common_config.front_view_length + junc_view_4_param.common_config.rear_view_length)
		/ (Float32_t)(result_param.result_image_height);//pixel distance
	Float32_t pixel_col_dis = (p_avm_param->p_avm_config_param->vehicle_param.vehicle_width 
		+ junc_view_4_param.common_config.left_view_length + junc_view_4_param.common_config.right_view_length)
		/ (Float32_t)(result_param.result_image_width);//pixel distance
	/********************extend all over result image pixel****************************/
	for (i = 0; i < result_param.result_image_height; i++)
	{
		for (j = 0; j < result_param.result_image_width; j++)
		{
			/***********calculate world coordinate*********/
			calculate_world_coordinate(world_coordinate, i, j, p_avm_param, result_param);//世界坐标系建立在小车的正中心
			/************front view*******************************/
			if (i < car_logo_rect.y )//前车有盲区所以
			{
				/********calculate weight************/
				Float32_t weight_Float32_t = calculate_4_view_weight_curve(CAM_FRONT, i, j,
					world_coordinate, weight_2_dim, p_avm_param, result_param,0);
				/***********world coordinate to pic coordinate**************/
				world_coordinate_to_pic_coordinate(CAM_FRONT, world_coordinate, image_point, p_avm_param);//0 is front

				/********zip data and fill lut*****/
				LUT_shift_address = i * result_param.result_image_width * 2 + j * 2;
				zip_data(LUT_shift_address, image_point, weight_Float32_t, p_lut, CAM_FRONT, left_right_flag);

			}

			/*************rear view*********************************/
			if (i >= car_logo_rect.y + car_logo_rect.height )//
			{
				/********calculate weight************/
				Float32_t weight_Float32_t = calculate_4_view_weight_curve(CAM_REAR, i, j,
					world_coordinate, weight_2_dim, p_avm_param, result_param,0);
				/***********world coordinate to pic coordinate**************/
				world_coordinate_to_pic_coordinate(CAM_REAR, world_coordinate, image_point, p_avm_param);
				/************zip data *************/
				LUT_shift_address = camera_size[0] + (i - (car_logo_rect.y
					+ car_logo_rect.height)) * result_param.result_image_width * 2 + j * 2;

				zip_data(LUT_shift_address, image_point, weight_Float32_t, 
					p_lut, CAM_REAR, left_right_flag);
			}
			/*******************************************left view********************************/
			if (j < car_logo_rect.x)
			{
				/*********calculate weight*****/
				Float32_t weight_Float32_t;
				weight_Float32_t = 1.000000 - weight_2_dim[i*result_param.result_image_width + j];
				/***********world coordinate to pic coordinate**************/
				world_coordinate_to_pic_coordinate(CAM_LEFT, world_coordinate, image_point, p_avm_param);
				/********************************************zip data***************************************/
				LUT_shift_address = camera_size[0] + camera_size[1] + i * car_logo_rect.x * 2 + j * 2;


				zip_data(LUT_shift_address, image_point, weight_Float32_t,
					p_lut, CAM_LEFT, left_right_flag);
			}
			/*************************right view****************/
			if (j >= car_logo_rect.x + car_logo_rect.width)//
			{
				/*********calculate weight ************/
				Float32_t weight_Float32_t;
				weight_Float32_t = 1.000000 - weight_2_dim[i*result_param.result_image_width + j];
				/***********world coordinate to pic coordinate**************/
				world_coordinate_to_pic_coordinate(CAM_RIGHT, world_coordinate, image_point, p_avm_param);
				/*******************zip data*******************/
				LUT_shift_address = camera_size[0] + camera_size[1] + camera_size[2]
					+ i * (result_param.result_image_width - car_logo_rect.x - car_logo_rect.width) * 2
					+ (j - car_logo_rect.x - car_logo_rect.width) * 2;
				zip_data(LUT_shift_address, image_point, weight_Float32_t, p_lut, CAM_RIGHT, left_right_flag);
			}
		}
	}

	free(weight_2_dim);//释放暂存的融合权重
#endif
	return error;
}
#endif





















































