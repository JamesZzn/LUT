/*------------------------------------------------------------------------------
avm_error.h define the error code's definition returned by AVM_Stitch module
Add by Shuo.Yan on 2017/10/31 InteSight
------------------------------------------------------------------------------*/
#ifndef __AVM_ERROR_H__
#define __AVM_ERROR_H__

#define CHECK_ERROR(status)																\
if (ERROR_NONE != status)																\
{																						\
    printf("AVM MODULE occured error on file (%s), line(%d)\n", __FILE__, __LINE__);	\
    while (1);																			\
	return status;																		\
}

typedef enum AVM_Error_Code_Enum
{
	ERROR_NONE = (0),
	ERROR_OPEN_FILE_FIAL,	
	ERROR_MEMORY_MALLOC_FAIL,
	ERROR_UNSURPPORT_IMAGE_FORMAT,
	ERROR_IMAGE_FAIL,
	ERROR_EXTEND,
	ERROR_OVERSTEP_PIC,
	ERROR_CUMPUTE,
	ERROR_PARA_CONFIG,
	ERROR_DENOMINATOR_ZEORO,
	ERROR_ZIP_YULAN_TABLE,//裕兰压缩表的出错
	ERROR_ZIP_ANBA_TABLE,//安霸压缩表出错
	ERROR_FUSE_WEIGHT,//融合权重出错
	ERROR_CAMERA_COORDINATE,//相机坐标系出错
	ERROR_PIC_COORDINATE,//图像坐标系出错
	ERROR_WORLD_COORDINATE//世界坐标系出错
}AVM_Error_Code_E;

#endif