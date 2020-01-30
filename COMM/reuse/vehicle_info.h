/*
 * vehicle info structure
 * Create by Shuo.Yan on 2017/11/22
 */
#ifndef  __VEHICLE_INFO_H_
#define __VEHICLE_INFO_H_

#include "data_type.h"
#include <stdio.h>

typedef struct Vehicle_Param_Struct
{
	Float32_t	vehicle_length;
	Float32_t	vehicle_width;
	Float32_t	front_overhang_length;
	Float32_t	wheel_base;
	Float32_t	rear_overhang_length;
	Float32_t	front_wheel_tread;
	Float32_t	rear_wheel_tread;
}Vehicle_Param_S, *P_Vehicle_Param_S;

Int32_t Get_Vehicle_Info(
	P_Vehicle_Param_S p_vehicle_param, IN const char* file_name);

#endif
 