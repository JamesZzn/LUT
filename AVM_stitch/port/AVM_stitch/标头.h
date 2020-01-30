




/**
*  @file AmbaCalibrationAvm.h
*
*  @copyright Copyright (c) 2015 Ambarella, Inc.
*
*  This file and its contents ("Software") are protected by intellectual property rights including, without limitation,
*  U.S. and/or foreign copyrights.  This Software is also the confidential and proprietary information of Ambarella, Inc.
*  and its licensors.  You may not use, reproduce, disclose, distribute, modify, or otherwise prepare derivative works
*  of this Software or any portion thereof except pursuant to a signed license agreement or nondisclosure agreement with
*  Ambarella, Inc. or its authorized affiliates.  In the absence of such an agreement, you agree to promptly notify and
*  return this Software to Ambarella, Inc.
*
*  THIS SOFTWARE IS PROVIDED "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
*  WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL
*  AMBARELLA, INC. OR ITS AFFILIATES BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
*  DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
*  COMPUTER FAILURE OR MALFUNCTION; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
*  CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
*  SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*
*  @details header file for surrounfing view calibration
*
*/
#ifndef __AMBA_CALIBRATION_AVM_H__
#define __AMBA_CALIBRATION_AVM_H__
#include "AmbaCalibrationMgr.h"
#include "AmbaDSP_WarpCore.h"
#include "AmbaCalib_Warp.h"
#include "AmbaCalib_Detect.h"
#ifndef ALIGN_PAD_SIZE
#define ALIGN_PAD_SIZE(size, alignbyte) ((!!((size)%(alignbyte))) ? (((alignbyte)-((size)%(alignbyte)))) : (0))
#endif
//Total 12388 bytes
#define WARP_MAX_WIDTH  (82)
#define WARP_MAX_HEIGHT (70)
#define WARP_MAX_SIZE (WARP_MAX_WIDTH*WARP_MAX_HEIGHT)
// 64x48. Maximum size for 64  tile is 4032x3008
//                     for 128 tile is 8064x6016

#define MAX_WARP_TABLE_COUNT (4)
#define MAX_VOUT_BUFFRE_COUNT (4)
#define MAX_BLEND_TABLE_SIZE (720*480)
#define WARP_CONTROL_S_DATA_SIZE (sizeof(UINT32) + sizeof(UINT32) + sizeof(AVM_Storage_s) + sizeof(Vout_Buffer_s) * MAX_VOUT_BUFFRE_COUNT + sizeof(Warp_Storage_s) * MAX_WARP_TABLE_COUNT)
#define WARP_NAND_RESERVED ((512 - (WARP_CONTROL_S_DATA_SIZE%512)) >> 2)

#define CAL_WARP_BASE          (0x00000000)
#define CAL_WARP_DATA(x)       ((x) + CAL_WARP_BASE)
#define CAL_WARP_ENABLE        CAL_WARP_DATA(0x00)
#define CAL_WARP_TABLE_COUNT   CAL_WARP_DATA(0x04)
#define CAL_AVM_BASE           CAL_WARP_DATA(0x08)    //store surrounding view info
#define CAL_VOUT_BUFFER_BASE(x)(CAL_AVM_BASE + sizeof(AVM_Storage_s) + sizeof(Vout_Buffer_s)*(x)) //store surrounding vout buffer view info
#define CAL_WARP_TABLE_BASE    (CAL_VOUT_BUFFER_BASE(4) + sizeof(UINT32)*WARP_NAND_RESERVED)//CAL_WARP_DATA(0x40)
#define CAL_WARP_TABLE(x)      (CAL_WARP_TABLE_BASE + sizeof(Warp_Storage_s)*(x))

//define warp kind
#define NORMAL_WARP   (0)
#define BIRDVIEW_WARP (1)
#define MAINVIEW_WARP (2)
#define RECORD_WARP   (3)
#define BIRDVIEW_WARP_3D (4)
#define MULTI_BLEND      (5)
#define CROSSVIEW_WARP   (6)

#define MAX_OVERLAP_POINT_NUM (8)
#if defined (CONFIG_APP_MOTORVU360_2)
#define MULTI_BLEND_2D_MAX MAX_AVM_BIRDVIEW_COUNT
#else
#define MULTI_BLEND_2D_MAX 1
#endif

/**
* @defgroup AmbaCalibrationAvm
* @brief header file for warp calibration
*
*/

/**
* @addtogroup AmbaCalibrationAvm
* @{
*/

/**
* Structure of warp storage header
*/

//warp information for each site, to be similar with IK
//typedef union Warp_Storage_Header_s_ {
typedef struct Warp_Storage_Header_s_ {            //64 bytes
	UINT32                              Enable;            /**< Function Enable control*/
	UINT32                              Version;        /* 0x20130101 */ /**< Structure Version */
	int                                 HorGridNum;     /**< Horizontal grid number */
	int                                 VerGridNum;     /**< Vertical grid number */
	int                                 TileWidthExp;   /**< Exponent of Tile Width. 4:16, 5:32, 6:64, 7:128, 8:256, 9:512 */
	int                                 TileHeightExp;  /**< Exponent of Tile Height. 4:16, 5:32, 6:64, 7:128, 8:256, 9:512 */
	AMBA_DSP_IMG_VIN_SENSOR_GEOMETRY_s  VinSensorGeo;   /**< Vin sensor geometry when calibrating */
	UINT32                              Warp2StageFlag; /**< Reserved for extension */
	UINT32                              ROIFlag;        /**< Reserved for extension */
	UINT32                              ByPassMode;     /**< Reserved for extension */
	UINT32                              WarpZoomStep;   /**< zoom step number*/
	UINT32                              WarpChannel;    /**< channel ID*/
} Warp_Storage_Header_s;

/**
* Warp storage structure
*/

typedef struct Warp_Storage_s_ {
	Warp_Storage_Header_s WarpHeader;    /**< Warp Storage Header */
	//6KB for x,y each = 12KB
	AMBA_DSP_IMG_GRID_POINT_s WarpVector[WARP_MAX_SIZE]; /**< warp vectors */
} Warp_Storage_s;

/**
* Warp table information
*/

typedef struct Cal_Warp_Table_Info_s_ {
	AMBA_DSP_IMG_GRID_POINT_s *Wp;     /**< warp vectors */
	UINT32    GridW;                /**< Width of warp grid */
	UINT32    GridH;                /**< Height of warp grid */
	UINT32    TileW;                /**< Width of warp tile */
	UINT32    TileH;                /**< Height of warp tile */
	UINT32    ImgW;                    /**< Image width */
	UINT32    ImgH;                    /**< Image height */
	UINT32    StartX;                /**< Start coordinates on X-axis */
	UINT32    StartY;                /**< Start coordinates on Y-axis */
	INT32   OffSensorFlag;            /**< Enable flag of off sensor calibration */
} Cal_Warp_Table_Info_s;

/**
* AVM area information
*/

typedef struct AVM_Rect_s_ {
	AMBA_DSP_IMG_GRID_POINT_s Start;  /**< topleft point */
	UINT16 Width;                     /**< width of rectangle  */
	UINT16 Height;                    /**< height of rectangle */
} AVM_Rect_s;

/**
* AVM storage information
*/

typedef struct AVM_Storage_s_ {
	AVM_Rect_s Birdeye;                     /**< surrounding view area in real world */
	AVM_Rect_s Car;                         /**< car allocate place and size in the real world */
	UINT32 CarPointNUM;
	WARP_Position_Cal_Info_s PositionCalInfo[4];
	Warp_TOOL_CHECKPOINT_INFO_s CheckPosition[CHECK_POINT_NUM];
	AMBA_DSP_IMG_GRID_POINT_s CarPositionPoint[4];    /**< 4 car position points */
	AMBA_DSP_IMG_GRID_POINT_s BlendOverLapPoint[4][MAX_OVERLAP_POINT_NUM];    /**< 4 blending area: LeftUp, LeftDown, RightUp, RightDown */
} AVM_Storage_s;

/**
* Vout buffer information
*/

typedef struct Vout_Buffer_s_ {
	UINT16 StartX;
	UINT16 StartY;
	UINT16 Width;
	UINT16 Height;
	UINT16  RotateType; //AMP_ROTATION_e
} Vout_Buffer_s;

/**
* AVM raw capture information
*/

typedef struct AVM_CalPositionInfo_s_ {
	UINT32 Enable;            /**< Function Enable control */
	double SensorCellSize;
	double LensDistortionReal[64];
	double LensDistortionExpect[64];
	UINT16 LensDistortionNum;
	Double_XYPoint_s OpticalCenter[4]; /**< optical center on calibration window */
	AVM_Rect_s RealCar;           /**< Car size and position */
	Warp_TOOL_ThreeD_Double_POINT_s CameraPosition[4]; /**< 4 channel camera positions */
	Warp_TOOL_ThreeD_Double_POINT_s TargetPoint[32];
	Warp_TOOL_Position_ID_s RawPoint[4][12];
	UINT32 CameraRotate[4];        /**< Reserved for extension */
	UINT8 OptimizeOpticalCenterEnable;  /**< Enable work of optical center optimization */
	UINT8 OptimizeHomoMatrixEnable;     /**< Enable work of homography matrix optimization */
	UINT16 Reserved1;                   /**< Reserved for extension */
	UINT32 Reserved2[10];               /**< Reserved for extension */
} AVM_CalPositionInfo_s;

/**
* AVM homogenous matrix transformation information
*/

typedef struct AVM_HomoTransInfo_s_ {
	UINT32 Enable;            								/* Function Enable control */
	double SensorCellSize;									/* sensor cell size, units: mm */
	double LensDistortionReal[128];							/* Distortion information */
	double LensDistortionExpect[128];						/* Distortion information */
	UINT16 LensDistortionNum;								/* total number of 1D distortion table*/
	double FocalLength[4][2];								/* focal length in x and y of each lens, units: mm */
	Double_XYPoint_s OpticalCenter[4]; 			/* optical center on calibration window */
	UINT16 CoordinateSystem;								/* choose coordinate system */
	int RotationOrder[1][4];								/* order of rotation */
	AVM_Rect_s RealCar;           							/* Car size and position */
	Warp_TOOL_ThreeD_Double_POINT_s CameraPosition[4]; 	/* 4 channel camera positions */
	Warp_TOOL_ThreeD_Double_POINT_s CameraRotation[4]; 	/* 4 channel camera rotation */
	Warp_TOOL_ThreeD_Double_POINT_s TargetPoint[32];		/* target points on real world */
	Warp_TOOL_ThreeD_Double_POINT_s RotTargetPoint[32];	/* rotated target points on real world */
	double ProMatrix[1][9];									/* projection matrix from intrinsic parmeters */
	double RotMatrix[1][9];									/* rotation matrix */
	double RotMatrixMul[1][9];								/* rotation matrix multiple R1*R2 */
	double InvRotMatrix[1][9];								/* inverse rotation matrix */
	double TraMatrix[1][3];	 								/* translation matrix */
	double MulRotMatrix[1][3];								/* multiple by rotation matrix */
	double CameraRotationTemp[1][3]; 						/* Temproal camera rotation */
	double RotationAngleTemp[1][3]; 						/* Temproal camera rotation angle */
	Warp_TOOL_ThreeD_Double_POINT_s RawPoint[4][32];		/* raw points on sensor */
	double SensorROI[4][2];									/* sensor width and height of each sensor */
	int DebugMode;											/* Debug Mode */
	double DistortRatio;									/* Ratio of real and expect */
	double Skew;											/* Skew */
	double RearWheel2Tail;									/* distance between rear wheel and tail */
	int TargetNum;											/* number of target point */
	int CameraNum[5];										/* number of camera for optical center, position, rotation, focal length, sensor ROI*/
} AVM_HomoTransInfo_s;


/**
* Warp table information
* (For partial loading, size must be multiple of 512)
*/
typedef struct Warp_Control_s_ {
	UINT32 Enable;            /**< Function Enable control */
	UINT32 WarpTableCount;    /**< Number of warp tables */
	AVM_Storage_s AVMInfo;  /**< AVM infomation */
	Vout_Buffer_s VoutBuffer[MAX_VOUT_BUFFRE_COUNT];  /**< Vout Buffer infomation */
	UINT32 Reserved[WARP_NAND_RESERVED];        /**< Reserved for extension */
	Warp_Storage_s *WarpTable[MAX_WARP_TABLE_COUNT]; /** Warp table information */
} Warp_Control_s;

#define CAL_AVM_VIEWS_BLEND_INFO(x) (0x00 + sizeof(AVM_Blending_Table_s)*(x))
#if defined (CONFIG_SVC_AUTOAPPS_AVM)
#define CAL_AVM_VIEWS_BLEND_INFO_PARTIAL_LR(x)  (0x00 + sizeof(UINT32) + sizeof(UINT8)*MAX_BLEND_TABLE_SIZE*(2))
#define CAL_AVM_VIEWS_BLEND_INFO_PARTIAL_LRB(x) (0x00 + sizeof(UINT32) + sizeof(UINT8)*MAX_BLEND_TABLE_SIZE*(3))
#endif
typedef struct AVM_Blending_Table_s_ {
	UINT32 Enable;
	UINT8 BlendParaLeft[MAX_BLEND_TABLE_SIZE]; // For partial loading, structure size should be 512 multiple. Minus 4 here
	UINT8 BlendParaRight[MAX_BLEND_TABLE_SIZE];
	UINT8 BlendParaBack[MAX_BLEND_TABLE_SIZE];
	UINT8 BlendParaFront[MAX_BLEND_TABLE_SIZE];
	UINT8 Reserved[ALIGN_PAD_SIZE(sizeof(UINT32)+sizeof(UINT32)+MAX_BLEND_TABLE_SIZE * 4, 512)];
	UINT32 CheckSum;
} AVM_Blending_Table_s;

typedef struct AVM_Blending_Control_s_ {
	AVM_Blending_Table_s AVMView[MAX_AVM_VIEW_COUNT];
} AVM_Blending_Control_s;

#define CAL_AVM_VIEWS_WARP_INFO(x) (CAL_WARP_TABLE(4)*(x))

typedef struct AVM_Warp_Control_s_ {
	Warp_Control_s AVMView[MAX_AVM_BIRDVIEW_COUNT];
} AVM_Warp_Control_s;

extern Warp_Control_s AppWarpControl;
extern Warp_Control_s AppMainViewWarpControl;
extern Warp_Control_s AppRecordWarpControl;
extern AVM_Warp_Control_s App3DBirdViewWarpControl;
extern AVM_Blending_Control_s *AppMultiBlendControl;
extern Warp_Control_s AppCrossViewWarpControl;
extern AVM_Warp_Control_s AppBirdViewWarpControl;












