#include "avm_utility.h"

/*
* Function Name:				Avm_Memory_Malloc_UInt32_t
* Function Function:			malloc UInt32_t type pointer memory
* Input:
*     p_address:				UInt32_t type memory address
*     size:						memory size to be malloc
* Note:
* Revision:
*     Create by Shuo.Yan on 2017/11/29.
*/
AVM_Error_Code_E Avm_Memory_Malloc_UInt32(UInt32_t** p_address, Int32_t size)
{
	AVM_Error_Code_E ret = ERROR_NONE;
	UInt32_t* address = NULL;

	address = (UInt32_t*)malloc(size);
	if (!address)
	{
		ret = ERROR_MEMORY_MALLOC_FAIL;
		CHECK_ERROR(ret);
	}
	memset(address, 0, size);
	
	(*p_address) = address;

	return ret;
}

/*
* Function Name:				Avm_Memory_Malloc_UInt8
* Function Function:			malloc UInt8_t type pointer memory
* Input:
*     p_address:				UInt8_t type memory address
*     size:						memory size to be malloc
* Note:
* Revision:
*     Create by Shuo.Yan on 2017/11/29.
*/
AVM_Error_Code_E Avm_Memory_Malloc_UInt8(UInt8_t** p_address, Int32_t size)
{
	AVM_Error_Code_E ret = ERROR_NONE;
	UInt8_t* address = NULL;

	address = (UInt8_t*)malloc(size);
	if (!address)
	{
		ret = ERROR_MEMORY_MALLOC_FAIL;
		CHECK_ERROR(ret);
	}
	memset(address, 0, size);

	(*p_address) = address;

	return ret;
}
