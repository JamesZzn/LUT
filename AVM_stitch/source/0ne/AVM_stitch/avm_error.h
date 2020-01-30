/*------------------------------------------------------------------------------
avm_error.h define the error code's definition returned by AVM_Stitch module
Add by Shuo.Yan on 2017/10/31 InteSight
------------------------------------------------------------------------------*/
#ifndef __AVM_ERROR_H__
#define __AVM_ERROR_H__

#define CHECK_ERROR(status)																\
if (ERROR_NONE != status)																\
{																						\
    printf("AVM MODULE occured error on file (%s), line(%s)\n", __FILE__, __LINE__);	\
	return status;																		\
}

typedef enum AVM_Error_Code_Enum
{
	ERROR_NONE = (0),
	ERROR_OPEN_FILE_FIAL,	
	ERROR_MEMORY_MALLOC_FAIL,
	ERROR_UNSURPPORT_IMAGE_FORMAT,
	ERROR_LOAD_IMAGE_FAIL,
	ERROR_LOAD_CAMERA_MODEL_FAIL
}AVM_Error_Code_E;

#endif