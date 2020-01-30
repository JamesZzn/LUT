/* This file defines some utility functions used in AVM_stitch module
 * Add by Shuo.Yan on 2017/10/31 InteSight
 */
#ifndef __AVM_UTILITY_H_
#define __AVM_UTILITY_H_

#include "avm_config.h"
#include "avm_error.h"

AVM_Error_Code_E Avm_Memory_Malloc_UInt32(UInt32_t** p_address, Int32_t size);

AVM_Error_Code_E Avm_Memory_Malloc_UInt8(UInt8_t** p_address, Int32_t size);

#endif