#include"fwrite_bin.h"
#include"resolve_data.h"
AVM_Error_Code_E write_3_4_view_lut(IN P_Avm_Param_S p_avm_param)
{
	AVM_Error_Code_E ret = ERROR_NONE;
	FILE *fid;
	char vehicle_param_name[MAX_ARRAY_SIZE];
	/*******存储大图的二进制表********/
	sprintf(vehicle_param_name, "%s/4_view_big.dat", lut_file_path);
	fid = fopen(vehicle_param_name, "wb");
	if (!(fid = fopen(vehicle_param_name, "wb")))
	{
		printf("File %s cannot be opened\n", vehicle_param_name);
		CHECK_ERROR(ret);
	}
	UInt32_t size_all=0;
	for (Int32_t cam_id = 0; cam_id < CAMERA_NUM; cam_id++)//计算存储前后左右视图的lut表的地址偏移 
	{
		size_all = size_all + p_avm_param->p_avm_config_param->avm_2d_param.junc_view_4_param.result_big_param.lut_size[cam_id] * 2;
	}
	fwrite(p_avm_param->p_avm_lut->p_lut_junc_view_4_big, sizeof(UInt32_t), size_all, fid);
	fclose(fid);


	/********存储小图的二进制表*************/
	sprintf(vehicle_param_name, "%s/4_view_small.dat", lut_file_path);
	fid = fopen(vehicle_param_name, "wb");
	if (!(fid = fopen(vehicle_param_name, "wb")))
	{
		printf("File %s cannot be opened\n", vehicle_param_name);
		CHECK_ERROR(ret);
	}
	size_all = 0;
	for (Int32_t cam_id = 0; cam_id < CAMERA_NUM; cam_id++)//计算存储前后左右视图的lut表的地址偏移 
	{
		size_all = size_all + p_avm_param->p_avm_config_param->avm_2d_param.junc_view_4_param.result_big_param.lut_size[cam_id] * 2;
	}
	fwrite(p_avm_param->p_avm_lut->p_lut_junc_view_4_small, sizeof(UInt32_t), size_all, fid);
	fclose(fid);



	/********存储3路前的二进制表*************/
	sprintf(vehicle_param_name, "%s/3_view_front.dat", lut_file_path);
	fid = fopen(vehicle_param_name, "wb");
	if (!(fid = fopen(vehicle_param_name, "wb")))
	{
		printf("File %s cannot be opened\n", vehicle_param_name);
		CHECK_ERROR(ret);
	}
	size_all = 0;
	for (Int32_t cam_id = 0; cam_id < CAMERA_NUM-1; cam_id++)//计算存储前后左右视图的lut表的地址偏移 
	{
		size_all = size_all + p_avm_param->p_avm_config_param->avm_2d_param.junc_view_3_front_param.result_3_front_param.lut_size[cam_id] * 2;
	}
	fwrite(p_avm_param->p_avm_lut->p_lut_junc_view_3_front, sizeof(UInt32_t), size_all, fid);
	fclose(fid);


	/********存储3路后的二进制表*************/
	sprintf(vehicle_param_name, "%s/3_view_rear.dat", lut_file_path);
	fid = fopen(vehicle_param_name, "wb");
	if (!(fid = fopen(vehicle_param_name, "wb")))
	{
		printf("File %s cannot be opened\n", vehicle_param_name);
		CHECK_ERROR(ret);
	}
	size_all = 0;
	for (Int32_t cam_id = 0; cam_id < CAMERA_NUM - 1; cam_id++)//计算存储前后左右视图的lut表的地址偏移 
	{
		size_all = size_all + p_avm_param->p_avm_config_param->avm_2d_param.junc_view_3_rear_param.result_3_rear_param.lut_size[cam_id] * 2;
	}
	fwrite(p_avm_param->p_avm_lut->p_lut_junc_view_3_rear, sizeof(UInt32_t), size_all, fid);
	fclose(fid);


	return ret;
}





AVM_Error_Code_E write_single_view_lut(IN P_Avm_Param_S p_avm_param, IN int cam_flag)
{
	AVM_Error_Code_E ret = ERROR_NONE;
	FILE *fid;
	char vehicle_param_name[MAX_ARRAY_SIZE];
	/*******存储大图的二进制表********/
	sprintf(vehicle_param_name, "%s/single_view%d.bin", lut_file_path, cam_flag);
	//fid = fopen(vehicle_param_name, "wb");
	if (!(fid = fopen(vehicle_param_name, "wb")))
	{
		printf("File %s cannot be opened\n", vehicle_param_name);
		CHECK_ERROR(ret);
	}
	Front_Rear_Single_View_Param_S single_view_param;
	CvSize size0;
	UInt32_t* p_lut = NULL;
	int result_image_width;
	int result_image_height;
	if (cam_flag == CAM_FRONT)
	{
		single_view_param = p_avm_param->p_avm_config_param->avm_single_view_param.front_single_view_param;
		size0 = cvSize(single_view_param.result_image_width, single_view_param.result_image_height);//
		p_lut = p_avm_param->p_avm_lut->p_lut_single_view_front;
		result_image_width = single_view_param.result_image_width;
		result_image_height = single_view_param.result_image_height;
	}
	if (cam_flag == CAM_REAR)
	{
		single_view_param = p_avm_param->p_avm_config_param->avm_single_view_param.rear_single_view_param;
		size0 = cvSize(single_view_param.result_image_width, single_view_param.result_image_height);//
		p_lut = p_avm_param->p_avm_lut->p_lut_single_view_rear;
		result_image_width = single_view_param.result_image_width;
		result_image_height = single_view_param.result_image_height;
	}
	if (cam_flag == CAM_LEFT)
	{
		//single_view_param = p_avm_param->p_avm_config_param->avm_single_view_param.left_single_view_param;
		size0 = cvSize(single_view_param.result_image_width, single_view_param.result_image_height);//
		p_lut = p_avm_param->p_avm_lut->p_lut_single_view_left;
		result_image_width = p_avm_param->p_avm_config_param->avm_single_view_param.left_single_view_param.result_image_width;
		result_image_height = p_avm_param->p_avm_config_param->avm_single_view_param.left_single_view_param.result_image_height;
	}
	if (cam_flag == CAM_RIGHT)
	{
		//single_view_param = p_avm_param->p_avm_config_param->avm_single_view_param.right_single_view_param;
		size0 = cvSize(single_view_param.result_image_width, single_view_param.result_image_height);//
		p_lut = p_avm_param->p_avm_lut->p_lut_single_view_right;
		result_image_width = p_avm_param->p_avm_config_param->avm_single_view_param.left_single_view_param.result_image_width;
		result_image_height = p_avm_param->p_avm_config_param->avm_single_view_param.left_single_view_param.result_image_height;
	}
	int size_all = result_image_width*result_image_height * 2;

	fwrite(p_lut, sizeof(UInt32_t), size_all, fid);
	fclose(fid);

	return ret;
}










#if 0

AVM_Error_Code_E write_single_view_lut(IN P_Avm_Param_S p_avm_param,IN int cam_flag)
{
	AVM_Error_Code_E ret = ERROR_NONE;
	FILE *fid;
	char vehicle_param_name[MAX_ARRAY_SIZE];
	/*******存储大图的二进制表********/
	sprintf(vehicle_param_name, "%s/single_view%d.dat", lut_file_path,cam_flag);
	fid = fopen(vehicle_param_name, "wb");
	if (!(fid = fopen(vehicle_param_name, "wb")))
	{
		printf("File %s cannot be opened\n", vehicle_param_name);
		CHECK_ERROR(ret);
	}
	Front_Rear_Single_View_Param_S single_view_param;
	CvSize size0;
	UInt32_t* p_lut = NULL;
	int result_image_width;
	int result_image_height;
	if (cam_flag == CAM_FRONT)
	{
		single_view_param = p_avm_param->p_avm_config_param->avm_single_view_param.front_single_view_param;
		size0 = cvSize(single_view_param.result_image_width, single_view_param.result_image_height);//
		p_lut = p_avm_param->p_avm_lut->p_lut_single_view_front;
		result_image_width = single_view_param.result_image_width;
		result_image_height = single_view_param.result_image_height;
	}
	if (cam_flag == CAM_REAR)
	{
		single_view_param = p_avm_param->p_avm_config_param->avm_single_view_param.rear_single_view_param;
		size0 = cvSize(single_view_param.result_image_width, single_view_param.result_image_height);//
		p_lut = p_avm_param->p_avm_lut->p_lut_single_view_rear;
		result_image_width = single_view_param.result_image_width;
		result_image_height = single_view_param.result_image_height;
	}
	if (cam_flag == CAM_LEFT)
	{
		//single_view_param = p_avm_param->p_avm_config_param->avm_single_view_param.left_single_view_param;
		size0 = cvSize(single_view_param.result_image_width, single_view_param.result_image_height);//
		p_lut = p_avm_param->p_avm_lut->p_lut_single_view_left;
		result_image_width = p_avm_param->p_avm_config_param->avm_single_view_param.left_single_view_param.result_image_width;
		result_image_height = p_avm_param->p_avm_config_param->avm_single_view_param.left_single_view_param.result_image_height;
	}
	if (cam_flag == CAM_RIGHT)
	{
		//single_view_param = p_avm_param->p_avm_config_param->avm_single_view_param.right_single_view_param;
		size0 = cvSize(single_view_param.result_image_width, single_view_param.result_image_height);//
		p_lut = p_avm_param->p_avm_lut->p_lut_single_view_right;
		result_image_width = p_avm_param->p_avm_config_param->avm_single_view_param.left_single_view_param.result_image_width;
		result_image_height = p_avm_param->p_avm_config_param->avm_single_view_param.left_single_view_param.result_image_height;
	}
	int size_all = result_image_width*result_image_height * 2;

	fwrite(p_lut, sizeof(UInt32_t), size_all, fid);
	fclose(fid);



#if 0
	////char vehicle_param_name[MAX_ARRAY_SIZE];
	/*******存储大图的二进制表********/
	sprintf(vehicle_param_name, "%s/single_view%d.dat", lut_file_path, cam_flag);
	fid = fopen(vehicle_param_name, "rb");
	if (!(fid = fopen(vehicle_param_name, "rb")))
	{
		printf("File %s cannot be opened\n", vehicle_param_name);

	}



	UInt32_t* p_lut_p = (UInt32_t*)malloc(sizeof(UInt32_t)*size_all);
	memset(p_lut_p, 0, sizeof(UInt32_t)*size_all);

	fread(p_lut_p, sizeof(UInt32_t), size_all, fid);
	fclose(fid);

	IplImage* image[4] = { NULL, NULL, NULL, NULL };

	 ret = my_load_image(image);
	//CHECK_ERROR(ret);
	IplImage *image_result_single_front_lut = cvCreateImage(size0, IPL_DEPTH_8U, 1);
	memset(image_result_single_front_lut->imageData, 0, result_image_width*result_image_height);
	for (int i = 0; i < result_image_height; i++)
	{
		for (int j = 0; j < result_image_width; j++)
		{
			int shift = i*result_image_width * 2 + j * 2;
			int rgb[3];
			front_single_resolve_data(shift, image[cam_flag], rgb, p_avm_param, p_lut_p);
			image_result_single_front_lut->imageData[i *result_image_width + j] = (uchar)(rgb[0]);
		}
	}
	char result_name[MAX_ARRAY_SIZE];
	sprintf(result_name, "%s/singleVVVVVVVVVVVV%d.bmp", result_image_path, cam_flag);
	cvSaveImage(result_name, image_result_single_front_lut);
#endif


	return ret;
}

#endif