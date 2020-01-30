/* data_type.h provides basic datatype and 
 * structure's definition of InteSight.
 * Create by Shuo.Yan on 2017/10/31 InteSight
 */
#ifndef __DATA_TYPE_H_
#define __DATA_TYPE_H_


#define CHESS_BOARD_NUM 80

#ifndef PI
#define PI (3.14159265358979323846264338327950288)
#endif

#define IN
#define OUT

#ifdef __cplusplus 
#define INTESIGHT_API extern "C"
#else
#define INTESIGHT_API
#endif

/* ================================================================= *\
 * define uniformed type name
\* ================================================================= */
typedef signed char			Int8_t;
typedef unsigned char		UInt8_t;
typedef signed short		Int16_t;
typedef unsigned short		UInt16_t;
typedef signed int			Int32_t;
typedef unsigned int		UInt32_t;
typedef long long			Int64_t;
typedef unsigned long long	UInt64_t;
typedef float				Float32_t;
typedef double				Float64_t;
typedef void*				Handle;

#define FREE_POINTER(p)		{ if(p != NULL) {free(p); p = NULL;} }

typedef enum Image_Type_Enum
{
	IMAGE_TYPE_YUV420 = 0,
	IMAGE_TYPE_YUV422,
	IMAGE_TYPE_YUV444,
	IMAGE_TYPE_NV12,
	IMAGE_TYPE_UYVY,
	IMAGE_TYPE_RGB,
	IMAGE_TYPE_RGBA,
}Image_Type_E;

typedef struct Intesight_Image_Struct
{
	Image_Type_E	image_type;			// image type
	Int32_t			height;				// image height
	Int32_t			width;				// image width
	Int32_t			channels;			// image channels
	UInt8_t*		p_data;				// image data's init address
}Intesight_Image_S, *P_Intesight_Image_S;

typedef enum Camera_Id_Enum
{
	CAM_FRONT = 0,
	CAM_REAR,
	CAM_LEFT,
	CAM_RIGHT
}Camera_Id_E;


#endif