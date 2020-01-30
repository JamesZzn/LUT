/*
 * This file load vehicle related info into memory,
 * Created by Shuo.Yan on 2017/11/23
 */

#include "vehicle_info.h"
#include"avm_config.h"
/*
* Function Name:				Get_Vehicle_Info
* Function Function:			get vehicle param from file
* Input:
*     p_vehicle_param:			vehicle param structure
*     file_name:				vehicle param info file name
* Note:
*     
* Revision:
*/
Int32_t Get_Vehicle_Info(OUT P_Vehicle_Param_S p_vehicle_param, IN const char* file_name)
{
	Int32_t ret = 0;
	FILE *f;
	char buf[MAX_ARRAY_SIZE];
	
	if (!(f = fopen(file_name, "r")))
	{
		printf("File %s cannot be opened\n", file_name);
		ret = 1;
		return ret;
	}

	fscanf(f, "vehilce_length: %f %s\n", &p_vehicle_param->vehicle_length, buf);
	fscanf(f, "vehilce_width: %f %s\n", &p_vehicle_param->vehicle_width, buf);
	fscanf(f, "wheel_base: %f %s\n", &p_vehicle_param->wheel_base, buf);
	fscanf(f, "front_overhang_length: %f %s\n", 
		&p_vehicle_param->front_overhang_length, buf);
	fscanf(f, "rear_overhang_length: %f %s\n", 
		&p_vehicle_param->rear_overhang_length, buf);
	fscanf(f, "front_wheel_tread: %f %s\n", 
		&p_vehicle_param->front_wheel_tread, buf);
	fscanf(f, "rear_wheel_tread: %f %s",
		&p_vehicle_param->rear_wheel_tread, buf);

	fclose(f);

	return ret;
}