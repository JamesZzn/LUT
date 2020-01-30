
#ifndef __3D_LICHENG_H_
#define __3D_LICHENG_H_

CvPoint3D32f licheng_dstToCalib(CvPoint3D32f dstPoint, int flag, P_Avm_Param_S  p_avm_param);
CvPoint2D32f licheng_calibToSrc(CvPoint3D32f calibPoint, int flag, P_Avm_Param_S  p_avm_param);
void licheng_writeLutItem(CvPoint resultPoint, CvPoint2D32f srcPoint, float fusionWeight, int flag, bool isPlane, bool isRight, P_Avm_Param_S p_avm_param);

void licheng_ballToSrc(CvPoint resultPoint, double alpha, double beta, double upperLimitAngle, double weight, int flag, P_Avm_Param_S p_avm_param);

void licheng_modelToBall(CvPoint resultPoint, double alpha, double beta, P_Avm_Param_S p_avm_param);

void licheng_calSlope(CvPoint2D32f carVertex[4], float slope[4][2], P_Avm_Param_S  p_avm_param);

int licheng_binFileGen(const char* binFileName, CvSize srcSize, CvSize dstSize, LutItem * lutInfo,
	CvRect effectROI[CAMERA_NUM], int countArroundLut, int view_id);

void licheng_bilinInterp(IplImage* srcImg, float result[3], int xSrcLoc, int ySrcLoc,
	double weightUpLeft, double weightUpRight, double weightDownLeft, double weightDownRight);
int licheng_testLut(IplImage* lutImg, IplImage* srcImage[CAMERA_NUM], int lutCount, CvSize srcSize,
	CvSize dstSize, LutItem * lutItem, int* pGains);
int licheng_readBinFile(const char* binFileName, const char* panoramaImageName, IplImage* srcImages[CAMERA_NUM], int* pGains);
void licheng_modelToPlane(CvPoint resultPoint, CvPoint3D32f dstPoint, CvPoint2D32f carVertex[4], float slope[4][2], float alpha, P_Avm_Param_S  p_avm_param);
void  generate_3D(P_Avm_Param_S p_avm_param);
#endif