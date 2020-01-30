
//#include "windows.h"
#include"avm_data_type.h"
#include"resolve_data.h"

vector<LutItem> m_lut; // 查找表项写表时候去用




/*
* Function Name:			licheng_dstToCalib
* Function Function:		licheng_dstToCalib
* Input:
* licheng_dstToCalib
* Note:
*licheng_calibToSrc
* Revision:
*     Create by Shuo.Yan on 2017/11/23.
*/
CvPoint3D32f licheng_dstToCalib(CvPoint3D32f dstPoint, int flag, P_Avm_Param_S  p_avm_param)
{
	float x, y, z;
	
	Camera_Model_Ext_S camera_model_ext;
	camera_model_ext = p_avm_param->p_avm_config_param->camera_model[flag].camera_model_ext;

	p_avm_param->p_avm_config_param->camera_model[flag].camera_model_ext.inv_R[0];

	y = (camera_model_ext.inv_R[0] * dstPoint.x + camera_model_ext.inv_R[1] * dstPoint.y + camera_model_ext.inv_R[2] * dstPoint.z + camera_model_ext.inv_pose[3]);
	x= (camera_model_ext.inv_R[3] * dstPoint.x + camera_model_ext.inv_R[4] * dstPoint.y + camera_model_ext.inv_R[5] * dstPoint.z + camera_model_ext.inv_pose[4]);
	z = -(camera_model_ext.inv_R[6] * dstPoint.x + camera_model_ext.inv_R[7] * dstPoint.y + camera_model_ext.inv_R[8] * dstPoint.z + camera_model_ext.inv_pose[5]);

	if (z == 0)
		return cvPoint3D32f(0, 0, 0);
	return cvPoint3D32f(x, y, z);

}


/*
* Function Name:			licheng_calibToSrc
* Function Function:		licheng_calibToSrc
* Input:
*  licheng_calibToSrc
* Note:
*licheng_calibToSrc
* Revision:
*     Create by Shuo.Yan on 2017/11/23.
*/
CvPoint2D32f licheng_calibToSrc(CvPoint3D32f calibPoint, int flag, P_Avm_Param_S  p_avm_param)
{
#if 1
	// 计算校正图中的点在原始图像中的坐标
	CvPoint2D32f srcPoint;
	
	Float32_t cam_ray[3];
	cam_ray[0] = calibPoint.x;
	cam_ray[1] = calibPoint.y;
	cam_ray[2] = calibPoint.z;
	Float32_t image_point[2];
	Cam_Ray_To_Image_Point(image_point, cam_ray, &(p_avm_param->p_avm_config_param
		->camera_model[flag].camera_model_int));

	srcPoint.x = image_point[1];
	srcPoint.y = image_point[0];

	return srcPoint;
#endif

}


/*
* Function Name:			licheng_writeLutItem
* Function Function:		licheng_writeLutItem
* Input:
*  licheng_writeLutItem
* Note:
* licheng_writeLutItem
* Revision:
*     Create by Shuo.Yan on 2017/11/23.
*/
void licheng_writeLutItem(CvPoint resultPoint, CvPoint2D32f srcPoint, float fusionWeight, int flag, bool isPlane, bool isRight, P_Avm_Param_S p_avm_param)
{
	Avm_3D_Param_S param_3D = p_avm_param->p_avm_config_param->avm_3d_param;
	int xSrcLoc, ySrcLoc; // 原图中位置，目的图位置
	int wt_up, wt_left; // 融合权重，双线性插值权重
	int fusionWtInt = (int)(fusionWeight * 511);
	bool isFusion = false;

	LutItem m_lutItem;
	
	if (1023 != fusionWtInt)
	{
		isFusion = true;
	}

	if (p_avm_param->p_avm_config_param->avm_3d_param.m_minDstPoint[flag].x > resultPoint.x)  p_avm_param->p_avm_config_param->avm_3d_param.m_minDstPoint[flag].x = resultPoint.x;
	if (p_avm_param->p_avm_config_param->avm_3d_param.m_minDstPoint[flag].y > resultPoint.y)  p_avm_param->p_avm_config_param->avm_3d_param.m_minDstPoint[flag].y = resultPoint.y;
	if (p_avm_param->p_avm_config_param->avm_3d_param.m_maxDstPoint[flag].x < resultPoint.x)  p_avm_param->p_avm_config_param->avm_3d_param.m_maxDstPoint[flag].x = resultPoint.x;
	if (p_avm_param->p_avm_config_param->avm_3d_param.m_maxDstPoint[flag].y < resultPoint.y)  p_avm_param->p_avm_config_param->avm_3d_param.m_maxDstPoint[flag].y = resultPoint.y;

	if (srcPoint.x > 0 && srcPoint.x <= p_avm_param->p_avm_config_param->avm_3d_param.m_srcSize.width - 1 
		&& srcPoint.y > 0 && srcPoint.y <= p_avm_param->p_avm_config_param->avm_3d_param.m_srcSize.height - 1)
	{
		xSrcLoc = (int)srcPoint.x;
		ySrcLoc = (int)srcPoint.y;

		double tempWeight1 = srcPoint.y - ySrcLoc;
		double tempWeight2 = srcPoint.x - xSrcLoc;

		wt_up = (1.0 - tempWeight1) * 31;
		wt_left = (1.0 - tempWeight2) * 31;

		m_lutItem.dstInfo = ((resultPoint.y * p_avm_param->p_avm_config_param->avm_3d_param.m_dstSize.width + resultPoint.x) << 12) + (fusionWtInt << 3) + (isRight << 2) + (isPlane << 1) + isFusion;
		m_lutItem.srcInfo = ((ySrcLoc * p_avm_param->p_avm_config_param->avm_3d_param.m_srcSize.width + xSrcLoc) << 12) + (wt_up << 7) + (wt_left << 2) + flag;

		m_lut.push_back(m_lutItem);
	}
	else
	{
		xSrcLoc = 3;
		ySrcLoc = 1;

		wt_up = 31;
		wt_left = 31;

		m_lutItem.dstInfo = ((resultPoint.y *  p_avm_param->p_avm_config_param->avm_3d_param.m_dstSize.width + resultPoint.x) << 12) + (fusionWtInt << 2) + (isPlane << 1) + isFusion;
		m_lutItem.srcInfo = ((ySrcLoc *  p_avm_param->p_avm_config_param->avm_3d_param.m_srcSize.width + xSrcLoc) << 12) + (wt_up << 7) + (wt_left << 2) + flag;

		m_lut.push_back(m_lutItem);
	}

}


/*
* Function Name:			licheng_ballToSrc
* Function Function:		licheng_ballToSrc
* Input:
*  licheng_ballToSrc
* Note:
* licheng_ballToSrc
* Revision:
*     Create by Shuo.Yan on 2017/11/23.
*/
void licheng_ballToSrc(CvPoint resultPoint, double alpha, double beta, double upperLimitAngle, double weight, int flag, P_Avm_Param_S p_avm_param)
{
	Avm_3D_Param_S param_3D = p_avm_param->p_avm_config_param->avm_3d_param;
	// angleLoc用来计算结果图中横向缩放比
	double angleLoc;
	if (flag == 0) angleLoc = (alpha < 180 ? 0 : 2 * PI);
	if (flag == 3) angleLoc = PI / 2;
	if (flag == 1) angleLoc = PI;
	if (flag == 2) angleLoc = 1.5 * PI;

	CvPoint3D32f dstPoint; // 地面圆边界上像素点的坐标

	dstPoint.x = p_avm_param->p_avm_config_param->avm_3d_param.m_ballRadius * cos(beta * PI / 180)* sin(alpha * PI / 180);
	dstPoint.y = p_avm_param->p_avm_config_param->avm_3d_param.m_ballRadius * cos(beta * PI / 180)*cos(alpha * PI / 180);
	dstPoint.z = p_avm_param->p_avm_config_param->avm_3d_param.m_centerHeight + p_avm_param->p_avm_config_param->avm_3d_param.m_ballRadius * sin(beta * PI / 180);

	CvPoint3D32f calibPoint = licheng_dstToCalib(dstPoint, flag,p_avm_param);  // 底平面圆边界在校正图中坐标
	CvPoint2D32f srcPoint = licheng_calibToSrc(calibPoint, flag, p_avm_param); // 底平面圆边界在原图中坐标

	licheng_writeLutItem(resultPoint, srcPoint, weight, flag, false, false, p_avm_param);
}

void licheng_modelToBall(CvPoint resultPoint, double alpha, double beta, P_Avm_Param_S p_avm_param)
{
	Avm_3D_Param_S param_3D = p_avm_param->p_avm_config_param->avm_3d_param;
	double upperLimitAngle = asin((param_3D.m_upperLimit - param_3D.m_centerHeight) / param_3D.m_ballRadius);
	if (alpha <= param_3D.m_fusionLoc[0] - param_3D.m_fusionAngle[0]) // 前侧右半部分
	{
		int flag = 0;
		licheng_ballToSrc(resultPoint, alpha, beta, upperLimitAngle, 1, flag,p_avm_param);
	}
	else if (alpha > param_3D.m_fusionLoc[0] - param_3D.m_fusionAngle[0] && alpha <  param_3D.m_fusionLoc[0] + param_3D.m_fusionAngle[0]) // 前侧与右侧的融合区域
	{
		int flag = 0;
		double weightRight = (alpha - (param_3D.m_fusionLoc[0] - param_3D.m_fusionAngle[0])) / (2 * param_3D.m_fusionAngle[0]);
		double weightFront = 1 - weightRight;
		licheng_ballToSrc(resultPoint, alpha, beta, upperLimitAngle, weightFront, flag, p_avm_param);

		flag = 3;
		licheng_ballToSrc(resultPoint, alpha, beta, upperLimitAngle, weightRight, flag, p_avm_param);
	}
	else if (alpha >= param_3D.m_fusionLoc[0] + param_3D.m_fusionAngle[0] && alpha <= param_3D.m_fusionLoc[1] - param_3D.m_fusionAngle[1]) // 右侧图像
	{
		int flag = 3;
		licheng_ballToSrc(resultPoint, alpha, beta, upperLimitAngle, 1, flag, p_avm_param);
	}
	else if (alpha >  param_3D.m_fusionLoc[1] - param_3D.m_fusionAngle[1] && alpha <  param_3D.m_fusionLoc[1] + param_3D.m_fusionAngle[1]) // 右侧与后侧融合区域
	{
		int flag = 1;
		double weightRear = (alpha - (param_3D.m_fusionLoc[1] - param_3D.m_fusionAngle[1])) / (2 * param_3D.m_fusionAngle[1]);
		double weightRight = 1 - weightRear;
		licheng_ballToSrc(resultPoint, alpha, beta, upperLimitAngle, weightRear, flag, p_avm_param);

		flag = 3;
		licheng_ballToSrc(resultPoint, alpha, beta, upperLimitAngle, weightRight, flag, p_avm_param);
	}
	else if (alpha >= param_3D.m_fusionLoc[1] + param_3D.m_fusionAngle[1] && alpha <= param_3D.m_fusionLoc[2] - param_3D.m_fusionAngle[2]) // 后侧图像
	{
		int flag = 1;
		licheng_ballToSrc(resultPoint, alpha, beta, upperLimitAngle, 1, flag, p_avm_param);
	}
	else if (alpha >  param_3D.m_fusionLoc[2] - param_3D.m_fusionAngle[2] && alpha <  param_3D.m_fusionLoc[2] + param_3D.m_fusionAngle[2]) // 后侧与左侧融合区域
	{
		int flag = 1;
		double weightLeft = (alpha - (param_3D.m_fusionLoc[2] - param_3D.m_fusionAngle[2])) / (2 * param_3D.m_fusionAngle[2]);
		double weightRear = 1 - weightLeft;
		licheng_ballToSrc(resultPoint, alpha, beta, upperLimitAngle, weightRear, flag, p_avm_param);

		flag = 2;
		licheng_ballToSrc(resultPoint, alpha, beta, upperLimitAngle, weightLeft, flag, p_avm_param);
	}
	else if (alpha >= param_3D.m_fusionLoc[2] + param_3D.m_fusionAngle[2] && alpha <= param_3D.m_fusionLoc[3] - param_3D.m_fusionAngle[3]) // 左侧图像
	{
		int flag = 2;
		licheng_ballToSrc(resultPoint, alpha, beta, upperLimitAngle, 1, flag, p_avm_param);
	}
	else if (alpha >  param_3D.m_fusionLoc[3] - param_3D.m_fusionAngle[3] && alpha <  param_3D.m_fusionLoc[3] + param_3D.m_fusionAngle[3]) // 左侧与前侧融合区域
	{
		int flag = 0;
		double weightFront = (alpha - (param_3D.m_fusionLoc[3] - param_3D.m_fusionAngle[3])) / (2 * param_3D.m_fusionAngle[3]);
		double weightLeft = 1 - weightFront;
		licheng_ballToSrc(resultPoint, alpha, beta, upperLimitAngle, weightFront, flag, p_avm_param);

		flag = 2;
		licheng_ballToSrc(resultPoint, alpha, beta, upperLimitAngle, weightLeft, flag, p_avm_param);
	}
	else // 前侧图像左半部分
	{
		int flag = 0;
		licheng_ballToSrc(resultPoint, alpha, beta, upperLimitAngle, 1, flag, p_avm_param);
	}
}



/*
* Function Name:			 licheng_calSlope
* Function Function:		 licheng_calSlope
* Input:
*  licheng_binFileGen
* Note:
*  licheng_calSlope
* Revision:
*     Create by Shuo.Yan on 2017/11/23.
*/
void licheng_calSlope(CvPoint2D32f carVertex[4], float slope[4][2], P_Avm_Param_S  p_avm_param)
{
	Avm_3D_Param_S param_3D = p_avm_param->p_avm_config_param->avm_3d_param;
	carVertex[0].y = param_3D.m_carUpLeft.y; // 左上顶点坐标
	carVertex[0].x = param_3D.m_carUpLeft.x;
	carVertex[1].y = param_3D.m_carUpLeft.y; // 右上顶点坐标
	carVertex[1].x = param_3D.m_carDownRight.x;
	carVertex[3].y = param_3D.m_carDownRight.y; // 左下顶点坐标
	carVertex[3].x = param_3D.m_carUpLeft.x;
	carVertex[2].y = param_3D.m_carDownRight.y; // 右下顶点坐标
	carVertex[2].x = param_3D.m_carDownRight.x;

	CvPoint2D32f ballVertex[4][2]; // 融合边界八顶点在鸟瞰图中的坐标(与carVertex对应，逆时针方向序号增大)

	//cheng 17.5.26
	//融合区的8条线，左上两条，右上两条，右下两条，左下两条 ,起点为carlog四个顶点，终点为融合区的顶点在平底圆上的8个点
	//slope 为8条线的斜率

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 2; j++)
		{
			ballVertex[i][j].x = (float)(param_3D.m_circleRadius * sin((param_3D.m_fusionLoc[(i + 3) % 4] + (2 * j - 1) * param_3D.m_fusionAngle[(i + 3) % 4]) / 180 * PI));
			ballVertex[i][j].y = (float)(param_3D.m_circleRadius * cos((param_3D.m_fusionLoc[(i + 3) % 4] + (2 * j - 1) * param_3D.m_fusionAngle[(i + 3) % 4]) / 180 * PI));
			slope[i][j] = (float)((ballVertex[i][j].y - carVertex[i].y) / (ballVertex[i][j].x - carVertex[i].x));
		}
	}
}






/*
* Function Name:			licheng_binFileGen
* Function Function:		licheng_binFileGen
* Input:
*  licheng_binFileGen
* Note:
* licheng_binFileGen
* Revision:
*     Create by Shuo.Yan on 2017/11/23.
*/
int licheng_binFileGen(const char* binFileName, CvSize srcSize, CvSize dstSize, LutItem* lutInfo, 
	CvRect effectROI[CAMERA_NUM], int countArroundLut, int view_id)
{
	int count = 0;
	FILE *fp = fopen(binFileName, "wb"); // 创建只写文件，若文件存在则清零，不存在则创建
	if (fp == 0)
	{
		cout << "打开文件失败：！" << endl;
		return -1;
	}
	// 得到版本信息
	int verPartInt = 1, verPartDec = 0, verCarType = 1;
	//SYSTEMTIME m_time;
	//GetLocalTime(&m_time);
	//int verMonth = m_time.wMonth;
	//int verDate = m_time.wDay;

	//unsigned int  versionInfo = (verPartInt << 28) + (verPartDec << 24) + (verCarType << 16) + (verMonth << 8) + verDate;
	unsigned int  versionInfo = (verPartInt << 28) + (verPartDec << 24) + (verCarType << 16);


	countArroundLut *= 12;
	// 写入版本信息
	fwrite(&versionInfo, sizeof(int), 1, fp);
	fwrite(&dstSize.width, sizeof(int), 1, fp);
	fwrite(&dstSize.height, sizeof(int), 1, fp);
	fwrite(&srcSize.width, sizeof(int), 1, fp);
	fwrite(&srcSize.height, sizeof(int), 1, fp);
	fwrite(&countArroundLut, sizeof(int), 1, fp);

	count += sizeof(int)* 6;

#ifdef EFFECTIVE_ROI
	for (int cam_id = 0; cam_id < CAMERA_NUM; cam_id++)
	{
		fwrite(&effectROI[cam_id].x, sizeof(int), 1, fp);
		fwrite(&effectROI[cam_id].y, sizeof(int), 1, fp);
		fwrite(&effectROI[cam_id].width, sizeof(int), 1, fp);
		fwrite(&effectROI[cam_id].height, sizeof(int), 1, fp);
	}
	count += sizeof(int)* 4 * CAMERA_NUM;
#endif 

	// 将全景查找表写入bin文件
	for (int i = 0; i < countArroundLut / 12; i++)
	{
		fwrite(&lutInfo[i].dstInfo, sizeof(unsigned int), 1, fp);
		fwrite(&lutInfo[i].srcInfo, sizeof(unsigned int), 1, fp);
		//fwrite(&lutInfo[i].weightInfo,sizeof(unsigned int),1,fp);
		count += sizeof(unsigned int)* 2;
	}

	int block_size;
	if (view_id == 0)
	{
		block_size = (1 << 19); // 512K
	}
	else
	{
		block_size = (4 << 20); // 4M
	}
	unsigned int* tmp = (unsigned int*)malloc(sizeof(unsigned int)* (block_size - count) >> 2);
	memset(tmp, 0, sizeof(unsigned int)* (block_size - count) >> 2);
	//fwrite(tmp, sizeof(unsigned int), (block_size - count) >> 2, fp);
	free(tmp);
	fclose(fp);
	return 0;
}

/********双线性插值********/


/*
* Function Name:			licheng_bilinInterp
* Function Function:		licheng_bilinInterp
* Input:
*   licheng_bilinInterp
* Note:
*  licheng_bilinInterp
* Revision:
*     Create by Shuo.Yan on 2017/11/23.
*/

void licheng_bilinInterp(IplImage* srcImg, float result[3], int xSrcLoc, int ySrcLoc,
	double weightUpLeft, double weightUpRight, double weightDownLeft, double weightDownRight)
{
	int x0 = xSrcLoc;
	int x1 = xSrcLoc + 1;
	int y0 = ySrcLoc;
	int y1 = ySrcLoc + 1;
	if (x0<0 || y0<0 || x1 > srcImg->width - 1 || y1 > srcImg->height - 1)
	{
		//cout<<"双线性插值越界";
	}
	else
	{
		uchar* p = (uchar*)(srcImg->imageData + y0*srcImg->widthStep);
		uchar* q = (uchar*)(srcImg->imageData + y1*srcImg->widthStep);

		for (int i = 0; i <= 2; i++)
		{
			if (x0 == 0 || y0 == 0 || x0 == srcImg->width - 1 || y0 == srcImg->height - 1)
			{
				result[i] = p[x0 * srcImg->nChannels + i];
			}
			else
			{
				result[i] = (uchar)(weightUpLeft*p[x0*srcImg->nChannels + i]
					+ weightUpRight*p[x1*srcImg->nChannels + i]
					+ weightDownLeft*q[x0*srcImg->nChannels + i]
					+ weightDownRight*q[x1*srcImg->nChannels + i]);
			}
		}
	}
}


/*********读表测试*************/

int licheng_testLut(IplImage* lutImg, IplImage* srcImage[CAMERA_NUM], int lutCount, CvSize srcSize,
	CvSize dstSize, LutItem* lutItem, int* pGains)
{

	float  bgr[3];
	uchar* r = (uchar*)lutImg->imageData;

	float varN = (Float32_t)25, varG = (Float32_t)0.01;

	int countFusion[8] = { 0, 0, 0, 0, 0, 0, 0, 0 }; // 各融合区域像素数统计

	int sumYuv[3][8] =
	{
		{ 0, 0, 0, 0, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, 0, 0, 0, 0 }
	}; // 各融合区域亮度和

	int aveYuv[3][8] =
	{
		{ 0, 0, 0, 0, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, 0, 0, 0, 0 }
	}; // 各融合区域亮度和


	for (int i = 0; i <= lutCount - 1; i++)
	{
		int label, xSrcLoc, ySrcLoc, xDstLoc, yDstLoc, lumaValue = 0;
		double fusionWeight, weightUpLeft, weightUpRight, weightDownLeft, weightDownRight, weightUp, weightLeft;
		bool isFusion, isPlane, isLumiCounted, isRight;

		fusionWeight = ((lutItem[i].dstInfo >> 3) & 0x1FF) / 511.0;

		yDstLoc = (lutItem[i].dstInfo >> 12) / dstSize.width;
		xDstLoc = (lutItem[i].dstInfo >> 12) % dstSize.width;
		isRight = (lutItem[i].dstInfo >> 2) & 0x1;
		isFusion = (lutItem[i].dstInfo >> 1) & 0x1;
		isPlane = lutItem[i].dstInfo & 0x1;

		isLumiCounted = isFusion & isPlane;

		ySrcLoc = (lutItem[i].srcInfo >> 12) / srcSize.width;
		xSrcLoc = (lutItem[i].srcInfo >> 12) % srcSize.width;
		label = lutItem[i].srcInfo & 0x3;

		weightUp = ((lutItem[i].srcInfo >> 7) & 0x1F) / 31.0;
		weightLeft = ((lutItem[i].srcInfo >> 2) & 0x1F) / 31.0;

		weightUpLeft = weightUp * weightLeft;
		weightUpRight = weightUp * (1 - weightLeft);
		weightDownLeft = (1 - weightUp) * weightLeft;
		weightDownRight = 1 - weightUpLeft - weightUpRight - weightDownLeft;


		licheng_bilinInterp(srcImage[label], bgr, xSrcLoc, ySrcLoc, weightUpLeft, weightUpRight, weightDownLeft, weightDownRight);

		bgr[0] < 0 ? 0 : (bgr[0] > 255 ? 255 : bgr[0]);
		bgr[1] < 0 ? 0 : (bgr[1] > 255 ? 255 : bgr[1]);
		bgr[2] < 0 ? 0 : (bgr[2] > 255 ? 255 : bgr[2]);

#ifdef AUTO_YUV_BALANCE
#endif

		r[yDstLoc * lutImg->widthStep + xDstLoc * lutImg->nChannels + 0] += (uchar)(bgr[0] * fusionWeight);
		r[yDstLoc * lutImg->widthStep + xDstLoc * lutImg->nChannels + 1] += (uchar)(bgr[1] * fusionWeight);
		r[yDstLoc * lutImg->widthStep + xDstLoc * lutImg->nChannels + 2] += (uchar)(bgr[2] * fusionWeight);
	}

#ifdef AUTO_YUV_BALANCE
	
#endif
	return 0;
}





/*
* Function Name:			licheng_readBinFile
* Function Function:		licheng_readBinFile
* Input:
*    licheng_readBinFile
* Note:
*  licheng_readBinFile
* Revision:
*     Create by Shuo.Yan on 2017/11/23.
*/

int licheng_readBinFile(const char* binFileName, const char* panoramaImageName, IplImage* srcImages[CAMERA_NUM], int* pGains)
{
	FILE* binFile = fopen(binFileName, "rb");
	if (binFile == 0)
	{
		printf("Can not open %s !", binFileName);
		return -1;
	}

	// 读入版本信息
	unsigned int versionInfo;
	fread(&versionInfo, sizeof(unsigned int), 1, binFile);
	int versionInt = versionInfo >> 28;
	int versionDec = (versionInfo >> 24) % 16;
	int vehicleType = (versionInfo >> 16) % 256;
	int month = (versionInfo >> 8) % 256;
	int date = versionInfo % 256;

	//cout<<setw(10)<<"版本号：\t"<<std::right<<std::right<<versionInt<<"."<<versionDec<<endl;
	//cout<<setw(10)<<"车型：\t"<<std::right<<vehicleType<<endl;
	//cout<<setw(10)<<"日期：\t"<<std::right<<month<<" 月 "<<date<<" 日"<<endl;

	// 读入参数表信息
	int  lutCount;
	CvSize srcSize, dstSize;
	fread(&dstSize, sizeof(dstSize), 1, binFile);
	fread(&srcSize, sizeof(srcSize), 1, binFile);
	fread(&lutCount, sizeof(unsigned int), 1, binFile);
	lutCount /= 12;

#ifdef EFFECTIVE_ROI
	CvRect effectROI[4];
	for (int cam_id = 0; cam_id < CAMERA_NUM; cam_id++)
	{
		fread(&effectROI[cam_id].x, sizeof(int), 1, binFile);
		fread(&effectROI[cam_id].y, sizeof(int), 1, binFile);
		fread(&effectROI[cam_id].width, sizeof(int), 1, binFile);
		fread(&effectROI[cam_id].height, sizeof(int), 1, binFile);
	}
#endif 

	// 处理全景表
	LutItem* aroundLut = new LutItem[lutCount];
	for (int i = 0; i < lutCount; i++)
	{
		fread(&aroundLut[i].dstInfo, sizeof(unsigned int), 1, binFile);
		fread(&aroundLut[i].srcInfo, sizeof(unsigned int), 1, binFile);
	}

	IplImage* lutImg = cvCreateImage(dstSize, IPL_DEPTH_8U, 3);
	uchar* pLut = (uchar*)lutImg->imageData;
	//先将全景表所有元素置零，避免之后+=时出现错误
	for (int i = 0; i <= dstSize.height - 1; i++)
	{
		for (int j = 0; j <= dstSize.width - 1; j++)
		{
			pLut[i * lutImg->widthStep + j * lutImg->nChannels + 0] = 0;
			pLut[i * lutImg->widthStep + j * lutImg->nChannels + 1] = 0;
			pLut[i * lutImg->widthStep + j * lutImg->nChannels + 2] = 0;
		}
	}

	int errorInfo = 0;
	errorInfo = licheng_testLut(lutImg, srcImages, lutCount, srcSize, dstSize, aroundLut, pGains); //  显示全景拼接结果
	if (errorInfo != 0) return errorInfo;

	// 保存全景图至文件夹
	cvSaveImage(panoramaImageName, lutImg);

	cvReleaseImage(&lutImg);
	fclose(binFile);
	delete[] aroundLut;

	return 0;
}








#define THRSH 3000
#define _FUSION_COEFFIC_ALPHA_ 1
/****************************************************************************************
** Copyright (C) 2014-2024, InteSight.
** All rights reserved.
** Function: modelToPlane
** Description: calculate the location in original image of the point on model bottom
** Parameters:
resultPoint: the coordinate of point on model bottom;
dstPoint: the corresponding location of resultPoint on original image;
carVertex: the 4 car vertexes;
slope: slope of the 8 boundaries
** Return: void
** Other:
** Author: Shuo Yan
** Edit Time: 2017/5/1
****************************************************************************************/
void licheng_modelToPlane(CvPoint resultPoint, CvPoint3D32f dstPoint, CvPoint2D32f carVertex[4], float slope[4][2], float alpha, P_Avm_Param_S  p_avm_param)
{
	// 先将四块未进行融合的区域赋值给鸟瞰拼接结果
	if (dstPoint.x <  carVertex[0].x && dstPoint.y >= slope[0][1] * (dstPoint.x - carVertex[0].x) + carVertex[0].y
		|| dstPoint.x >= carVertex[0].x && dstPoint.x <= carVertex[1].x && dstPoint.y >= carVertex[0].y
		|| dstPoint.x >  carVertex[1].x && dstPoint.y >= slope[1][0] * (dstPoint.x - carVertex[1].x) + carVertex[1].y) // 前侧未融合区域
	{
		int flag = 0;
		// 利用相机外参计算地面像素点在经纬展开图中的位置
		CvPoint3D32f calibPoint = licheng_dstToCalib(dstPoint, flag, p_avm_param);
		// 利用相机内参计算该点在原图中的位置
		CvPoint2D32f srcPoint = licheng_calibToSrc(calibPoint, flag, p_avm_param);
		// 将该位置写入查找表项
		if (!p_avm_param->p_avm_config_param->avm_3d_param.m_is_lumi_balance)
		{
			licheng_writeLutItem(resultPoint, srcPoint, 1.0, flag, true, false, p_avm_param);
		}
		p_avm_param->p_avm_config_param->avm_3d_param.m_is_lumi_balance;

	}
	else if (dstPoint.x >= carVertex[1].x && dstPoint.y <= slope[1][1] * (dstPoint.x - carVertex[1].x) + carVertex[1].y && dstPoint.y >= slope[2][0] * (dstPoint.x - carVertex[2].x) + carVertex[2].y) // 右侧未融合区域
	{
		int flag = 3;
		CvPoint3D32f calibPoint = licheng_dstToCalib(dstPoint, flag, p_avm_param);
		CvPoint2D32f srcPoint = licheng_calibToSrc(calibPoint, flag, p_avm_param);
		if (!p_avm_param->p_avm_config_param->avm_3d_param.m_is_lumi_balance)
		{
			licheng_writeLutItem(resultPoint, srcPoint, 1.0, flag, true, false, p_avm_param);
		}
	}
	else if (dstPoint.x < carVertex[3].x && dstPoint.y <= slope[3][0] * (dstPoint.x - carVertex[3].x) + carVertex[3].y
		|| dstPoint.x >= carVertex[3].x && dstPoint.x <= carVertex[2].x && dstPoint.y <= carVertex[2].y
		|| dstPoint.x > carVertex[2].x && dstPoint.y <= slope[2][1] * (dstPoint.x - carVertex[2].x) + carVertex[2].y) // 后侧未融合区域
	{
		int flag = 1;
		CvPoint3D32f calibPoint = licheng_dstToCalib(dstPoint, flag, p_avm_param);
		CvPoint2D32f srcPoint = licheng_calibToSrc(calibPoint, flag, p_avm_param);
		if (!p_avm_param->p_avm_config_param->avm_3d_param.m_is_lumi_balance)
		{
			licheng_writeLutItem(resultPoint, srcPoint, 1.0, flag, true, false, p_avm_param);
		}
	}
	else if (dstPoint.x <= carVertex[3].x && dstPoint.y >= slope[3][1] * (dstPoint.x - carVertex[3].x) + carVertex[3].y && dstPoint.y <= slope[0][0] * (dstPoint.x - carVertex[0].x) + carVertex[0].y) // 左侧未融合区域
	{
		int flag = 2;
		CvPoint3D32f calibPoint = licheng_dstToCalib(dstPoint, flag, p_avm_param);
		CvPoint2D32f srcPoint = licheng_calibToSrc(calibPoint, flag, p_avm_param);
		if (!p_avm_param->p_avm_config_param->avm_3d_param.m_is_lumi_balance)
		{
			licheng_writeLutItem(resultPoint, srcPoint, 1.0, flag, true, false, p_avm_param);
		}
	}

	// 接下来分别处理四块融合区域 (都按行融合)
	else if (dstPoint.y > (slope[0][0] * (dstPoint.x - carVertex[0].x) + carVertex[0].y)
		&& dstPoint.y < (slope[0][1] * (dstPoint.x - carVertex[0].x) + carVertex[0].y)) // 左上融合区域
	{
		int flag = 0;

#ifdef 	_FUSION_COEFFIC_ALPHA_
		//cheng 17.6.24
		//融合区两条边界直线的夹角 
		float m_alpha = 0.0;
		m_alpha = atan(abs((slope[0][0] - slope[0][1]) / (1 + slope[0][0] * slope[0][1])));

		if (m_alpha >(PI / 2))
			m_alpha = PI - m_alpha;

		float m_deta = 0.0;//基于起始边界的偏角
		float m_slope = 0.0;//任一目标点到car顶点的斜率

		m_slope = (dstPoint.y - carVertex[0].y) / (dstPoint.x - carVertex[0].x);

		m_deta = atan(abs((m_slope - slope[0][1]) / (1 + m_slope*slope[0][1])));

		if (m_deta > (PI / 2))
			m_deta = PI - m_deta;

		double weightFront = (m_alpha - m_deta) / m_alpha;

#else


#endif

		if (dstPoint.y <= carVertex[0].y)
		{
			weightFront = 0; // new added
		}

		float weightLeft = 1 - weightFront;
		CvPoint3D32f calibPoint = licheng_dstToCalib(dstPoint, flag, p_avm_param);
		CvPoint2D32f srcPoint = licheng_calibToSrc(calibPoint, flag, p_avm_param);
		if (dstPoint.x > -THRSH && dstPoint.x < THRSH)
		{
			licheng_writeLutItem(resultPoint, srcPoint, weightFront, flag, true, false, p_avm_param);
		}
		else
		{
			if (!p_avm_param->p_avm_config_param->avm_3d_param.m_is_lumi_balance)
			{
				//licheng_writeLutItem(resultPoint, srcPoint, weightFront, flag, true, false, p_avm_param);
				licheng_writeLutItem(resultPoint, srcPoint, weightFront, flag, false, false, p_avm_param);
			}
		}

		flag = 2;
		calibPoint = licheng_dstToCalib(dstPoint, flag, p_avm_param);
		srcPoint = licheng_calibToSrc(calibPoint, flag, p_avm_param);
		if (dstPoint.x > -THRSH && dstPoint.x < THRSH)
		{
			licheng_writeLutItem(resultPoint, srcPoint, weightLeft, flag, true, true, p_avm_param);
		}
		else
		{
			if (!p_avm_param->p_avm_config_param->avm_3d_param.m_is_lumi_balance)
			{
				licheng_writeLutItem(resultPoint, srcPoint, weightLeft, flag, false, true, p_avm_param);
			}
		}

	}
	else if (dstPoint.y > slope[1][1] * (dstPoint.x - carVertex[1].x) + carVertex[1].y && dstPoint.y < slope[1][0] * (dstPoint.x - carVertex[1].x) + carVertex[1].y) // 右上融合区域
	{
		int flag = 0;

#ifdef 	_FUSION_COEFFIC_ALPHA_
		//cheng 17.6.24
		//融合区两条边界直线的夹角 
		float m_alpha = 0.0;
		m_alpha = atan(abs((slope[1][0] - slope[1][1]) / (1 + slope[1][0] * slope[1][1])));

		if (m_alpha >(PI / 2))
			m_alpha = PI - m_alpha;

		float m_deta = 0.0;//基于起始边界的偏角
		float m_slope = 0.0;//任一目标点到car顶点的斜率

		m_slope = (dstPoint.y - carVertex[1].y) / (dstPoint.x - carVertex[1].x);

		m_deta = atan(abs((m_slope - slope[1][0]) / (1 + m_slope*slope[1][0])));

		if (m_deta > (PI / 2))
			m_deta = PI - m_deta;

		double weightFront = (m_alpha - m_deta) / m_alpha;

#else

#endif


		if (dstPoint.y <= carVertex[1].y) weightFront = 0; // new added
		float weightRight = 1 - weightFront;
		CvPoint3D32f calibPoint = licheng_dstToCalib(dstPoint, flag, p_avm_param);
		CvPoint2D32f srcPoint = licheng_calibToSrc(calibPoint, flag, p_avm_param);
		if (dstPoint.x > -THRSH && dstPoint.x < THRSH)
		{
			licheng_writeLutItem(resultPoint, srcPoint, weightFront, flag, true, true, p_avm_param);
		}
		else
		{
			if (!p_avm_param->p_avm_config_param->avm_3d_param.m_is_lumi_balance)
			{
				licheng_writeLutItem(resultPoint, srcPoint, weightFront, flag, false, true, p_avm_param);
			}
		}

		flag = 3;
		calibPoint = licheng_dstToCalib(dstPoint, flag, p_avm_param);
		srcPoint = licheng_calibToSrc(calibPoint, flag, p_avm_param);
		if (dstPoint.x > -THRSH && dstPoint.x < THRSH)
		{
			licheng_writeLutItem(resultPoint, srcPoint, weightRight, flag, true, false, p_avm_param);
		}
		else
		{
			if (!p_avm_param->p_avm_config_param->avm_3d_param.m_is_lumi_balance)
			{
				licheng_writeLutItem(resultPoint, srcPoint, weightRight, flag, false, false, p_avm_param);
			}
		}
	}
	else if (dstPoint.y > slope[2][1] * (dstPoint.x - carVertex[2].x) + carVertex[2].y 
		&& dstPoint.y < slope[2][0] * (dstPoint.x - carVertex[2].x) + carVertex[2].y) // 右下融合区域
	{
		int flag = 1;

#ifdef 	_FUSION_COEFFIC_ALPHA_
		//cheng 17.6.24
		//融合区两条边界直线的夹角 
		float m_alpha = 0.0;
		m_alpha = atan(abs((slope[2][0] - slope[2][1]) / (1 + slope[2][0] * slope[2][1])));

		if (m_alpha >(PI / 2))
			m_alpha = PI - m_alpha;

		float m_deta = 0.0;//基于起始边界的偏角
		float m_slope = 0.0;//任一目标点到car顶点的斜率

		m_slope = (dstPoint.y - carVertex[2].y) / (dstPoint.x - carVertex[2].x);

		m_deta = atan(abs((m_slope - slope[2][0]) / (1 + m_slope*slope[2][0])));

		if (m_deta > (PI / 2))
			m_deta = PI - m_deta;

		double weightRight = (m_alpha - m_deta) / m_alpha;
#else

#endif		

		if (dstPoint.y >= carVertex[2].y) weightRight = 1; // new added
		float weightRear = 1 - weightRight;
		CvPoint3D32f calibPoint = licheng_dstToCalib(dstPoint, flag, p_avm_param);
		CvPoint2D32f srcPoint = licheng_calibToSrc(calibPoint, flag, p_avm_param);
		if (dstPoint.x > -THRSH && dstPoint.x < THRSH)
		{
			licheng_writeLutItem(resultPoint, srcPoint, weightRear, flag, true, false, p_avm_param);
		}
		else
		{
			if (!p_avm_param->p_avm_config_param->avm_3d_param.m_is_lumi_balance)
			{
				licheng_writeLutItem(resultPoint, srcPoint, weightRear, flag, false, false, p_avm_param);
			}
		}

		flag = 3;
		calibPoint = licheng_dstToCalib(dstPoint, flag, p_avm_param);
		srcPoint = licheng_calibToSrc(calibPoint, flag, p_avm_param);
		if (dstPoint.x > -THRSH && dstPoint.x < THRSH)
		{
			licheng_writeLutItem(resultPoint, srcPoint, weightRight, flag, true, true, p_avm_param);
		}
		else
		{
			if (!p_avm_param->p_avm_config_param->avm_3d_param.m_is_lumi_balance)
			{
				licheng_writeLutItem(resultPoint, srcPoint, weightRight, flag, false, true, p_avm_param);
			}
		}

	}
	else if (dstPoint.y > slope[3][0] * (dstPoint.x - carVertex[3].x) 
		+ carVertex[3].y && dstPoint.y < slope[3][1] * (dstPoint.x - carVertex[3].x) + carVertex[3].y) // 左下融合区域
	{
		int flag = 1;

#ifdef 	_FUSION_COEFFIC_ALPHA_ 
		//cheng 17.6.24
		//融合区两条边界直线的夹角 
		float m_alpha = 0.0;
		m_alpha = atan(abs((slope[3][0] - slope[3][1]) / (1 + slope[3][0] * slope[3][1])));

		if (m_alpha >(PI / 2))
			m_alpha = PI - m_alpha;

		float m_deta = 0.0;//基于起始边界的偏角
		float m_slope = 0.0;//任一目标点到car顶点的斜率

		m_slope = (dstPoint.y - carVertex[3].y) / (dstPoint.x - carVertex[3].x);

		m_deta = atan(abs((m_slope - slope[3][1]) / (1 + m_slope*slope[3][1])));

		if (m_deta > (PI / 2))
			m_deta = PI - m_deta;

		double weightLeft = (m_alpha - m_deta) / m_alpha;

#else

#endif		



		if (dstPoint.y >= carVertex[3].y) weightLeft = 1; // new added
		float weightRear = 1 - weightLeft;
		CvPoint3D32f calibPoint = licheng_dstToCalib(dstPoint, flag, p_avm_param);
		CvPoint2D32f srcPoint = licheng_calibToSrc(calibPoint, flag, p_avm_param);
		if (dstPoint.x > -THRSH && dstPoint.x < THRSH)
		{
			licheng_writeLutItem(resultPoint, srcPoint, weightRear, flag, true, true, p_avm_param);
		}
		else
		{
			if (!p_avm_param->p_avm_config_param->avm_3d_param.m_is_lumi_balance)
			{
				licheng_writeLutItem(resultPoint, srcPoint, weightRear, flag, false, true, p_avm_param);
			}
		}

		flag = 2;
		calibPoint = licheng_dstToCalib(dstPoint, flag, p_avm_param);
		srcPoint = licheng_calibToSrc(calibPoint, flag, p_avm_param);
		if (dstPoint.x > -THRSH && dstPoint.x < THRSH)
		{
			licheng_writeLutItem(resultPoint, srcPoint, weightLeft, flag, true, false, p_avm_param);
		}
		else
		{
			if (!p_avm_param->p_avm_config_param->avm_3d_param.m_is_lumi_balance)
			{
				licheng_writeLutItem(resultPoint, srcPoint, weightLeft, flag, false, false, p_avm_param);
			}
		}
	}
}



/*
* Function Name:			set_Model_3D_Param
* Function Function:		set_Model_3D_Param
* Input:
*    set_Model_3D_Param
* Note:
*  set_Model_3D_Param
* Revision:
*     Create by Shuo.Yan on 2017/11/23.
*/
void set_Model_3D_Param(const char* modelParamName, P_Avm_3D_Param_S p_Avm_3D_Param_s)
{
	//strcpy(m_dirName, modelParamName);
	char str[100];
	FILE* fp = fopen(modelParamName, "r");
	fscanf(fp, "%s%lf", str, & p_Avm_3D_Param_s->m_centerHeight);//m_centerHeight
	fscanf(fp, "%s%lf", str, & p_Avm_3D_Param_s->m_circleRadius);//m_circleRadius
	fscanf(fp, "%s", str);
	fscanf(fp, "%f", & p_Avm_3D_Param_s->m_newObserLoc.x);//m_newObserLoc.x
	fscanf(fp, "%f", &p_Avm_3D_Param_s->m_newObserLoc.y); //m_newObserLoc.y
	fscanf(fp, "%f", & p_Avm_3D_Param_s->m_newObserLoc.z);//m_newObserLoc.z
	fscanf(fp, "%s%lf", str, & p_Avm_3D_Param_s->m_picthAngle);//m_picthAngle
	fscanf(fp, "%s%lf", str, &p_Avm_3D_Param_s->m_rollAngle);// m_rollAngle
	fscanf(fp, "%s%lf", str, &p_Avm_3D_Param_s->m_newFocal); //m_newFocal
	fscanf(fp, "%s", str);
	fscanf(fp, "%d", &p_Avm_3D_Param_s->m_dstSize.width);// m_dstSize.width
	fscanf(fp, "%d", &p_Avm_3D_Param_s->m_dstSize.height);//m_dstSize.height

	fscanf(fp, "%s%lf", str, &p_Avm_3D_Param_s->m_horiFov);//m_horiFov
	fscanf(fp, "%s%lf", str, &p_Avm_3D_Param_s->m_magScaleVert);//m_magScaleVert
	fscanf(fp, "%s%lf", str, &p_Avm_3D_Param_s->m_magScaleHori);// p_Avm_3D_Param_s->m_magScaleHori
	fscanf(fp, "%s%lf", str, &p_Avm_3D_Param_s->m_upperLimit);// p_Avm_3D_Param_s->m_upperLimit
	fscanf(fp, "%s", str);
	for (int i = 0; i < 4; i++)
		fscanf(fp, "%lf", &p_Avm_3D_Param_s->m_fusionLoc[i]);// p_Avm_3D_Param_s->m_fusionLoc[i]
	fscanf(fp, "%s", str);
	for (int i = 0; i < 4; i++)
		fscanf(fp, "%lf", &p_Avm_3D_Param_s->m_fusionAngle[i]); //p_Avm_3D_Param_s->m_fusionAngle[i])

	fscanf(fp, "%s", str);
	fscanf(fp, "%f%f", &p_Avm_3D_Param_s->m_carUpLeft.x, &p_Avm_3D_Param_s->m_carUpLeft.y); //p_Avm_3D_Param_s->m_carUpLeft.x, p_Avm_3D_Param_s->m_carUpLeft.y
	fscanf(fp, "%s", str);
	fscanf(fp, "%f%f", &p_Avm_3D_Param_s->m_carDownRight.x, &p_Avm_3D_Param_s->m_carDownRight.y); //p_Avm_3D_Param_s->m_carDownRight.x, p_Avm_3D_Param_s->m_carDownRight.y
	fclose(fp);
}



/*
* Function Name:				generate_3D
* Function Function:			generate_3D
* Input:
*     P_Avm_Param_S p_avm_param
* Note:
*    P_Avm_Param_S p_avm_param
* Revision:
*     Create by Shuo.Yan on 2017/11/23.
*/

void  generate_3D(P_Avm_Param_S p_avm_param)
{

	IplImage* srcImages[CAMERA_NUM];
	my_load_image(srcImages, p_avm_param);

	for (int number = 1; number < 17; number++)
	{
		m_lut.clear();
		char path[MAX_ARRAY_SIZE];
		config_param_path = "../../config/T2_config/3D";
		source_image_path = "../../image/source/T2";
		result_image_path = "../../image/result/T2/3D";
		lut_file_path = "../../lut_table/T2/3D";
		sprintf(path, "%s/%s",
			config_param_path, ConfigFileName_c[number]);

		set_Model_3D_Param(path, &p_avm_param->p_avm_config_param->avm_3d_param);
		Avm_3D_Param_S param_3D ;
		p_avm_param->p_avm_config_param->avm_3d_param.m_ballRadius = sqrt(p_avm_param->p_avm_config_param->
			avm_3d_param.m_centerHeight*p_avm_param->p_avm_config_param->avm_3d_param.m_centerHeight 
			+ p_avm_param->p_avm_config_param->avm_3d_param.m_circleRadius*p_avm_param->
			p_avm_config_param->avm_3d_param.m_circleRadius);
		param_3D = p_avm_param->p_avm_config_param->avm_3d_param;;
		
		p_avm_param->p_avm_config_param->avm_3d_param.m_srcSize.width =
			p_avm_param->p_avm_config_param->camera_model[0].camera_model_int.width;
		p_avm_param->p_avm_config_param->avm_3d_param.m_srcSize.height =
			p_avm_param->p_avm_config_param->camera_model[0].camera_model_int.height;
		////double m_ballRadius; // 平底球模型中大球半径
		//CvSize m_srcSize;//原图像尺寸

		CvPoint3D32f dstLoc, imgCenterLoc, modelCenter, pedPoint;
		double pitchAngle = param_3D.m_picthAngle / 180 * PI;
		double rollAngle = param_3D.m_rollAngle / 180 * PI;
		double pitchAngle1 = pitchAngle - PI * 0.5;

		//平底球模型中心在世界坐标系中的坐标
		modelCenter = cvPoint3D32f(0, 0, param_3D.m_centerHeight);

		// cheng 17.5.26 最终虚拟成像的中心在中心坐标系下的坐标
		// 目标结果图中心在模型中的坐标
		double image_center_cam[3] = { 0, 0, param_3D.m_newFocal };
		double initTrans[3] = { param_3D.m_newObserLoc.x, param_3D.m_newObserLoc.y, param_3D.m_newObserLoc.z };

		imgCenterLoc.x = param_3D.m_newObserLoc.x + param_3D.m_newFocal * sin(pitchAngle) * sin(rollAngle);
		imgCenterLoc.y = param_3D.m_newObserLoc.y + param_3D.m_newFocal * sin(pitchAngle) * cos(rollAngle);
		imgCenterLoc.z = param_3D.m_newObserLoc.z - param_3D.m_newFocal * cos(pitchAngle);

		CvPoint2D32f carVertex[4]; // 小车四顶点的坐标
		float slope[4][2];	// 四边的斜率
		licheng_calSlope(carVertex, slope, p_avm_param); // 计算融合边界的斜率
		for (int i = 0; i <= param_3D.m_dstSize.height - 1; i++)
		{
			for (int j = 0; j <= param_3D.m_dstSize.width - 1; j++)
			{
				// 计算多视角结果图上各点在球模型中的实际位置
				CvPoint resultPoint = cvPoint(j, i);
				double f_new(param_3D.m_newFocal), scale;
				scale = tan(param_3D.m_horiFov / 360 * PI) * 2;
				int angleSyb = param_3D.m_picthAngle > 0 ? (1) : (-1); // 角度正负号标签
				int rollangleSyb = rollAngle > (PI / 2) ? (-1) : (1); // 角度正负号标签

				//cheng 17.5.26 成像平面中的每个点映射到在中心坐标系下的坐标,相似三角形
				// 图像上像素点在模型中的坐标

				if (param_3D.m_newObserLoc.y == 0 && param_3D.m_newObserLoc.x != 0 && 0)
				{
					dstLoc.x = imgCenterLoc.x + f_new * scale / (param_3D.m_dstSize.width - 1) 
						* ((param_3D.m_dstSize.height - 1) / 2.0 - i) * cos(pitchAngle) * sin(rollAngle);
					dstLoc.y = imgCenterLoc.y + f_new * scale / (param_3D.m_dstSize.width - 1) 
						* (j - (param_3D.m_dstSize.width - 1) / 2.0) * sin(rollAngle);
					dstLoc.z = imgCenterLoc.z + f_new * scale / (param_3D.m_dstSize.width - 1) 
						* ((param_3D.m_dstSize.height - 1) / 2.0 - i) * sin(pitchAngle) * sin(rollAngle);
				}
				else
				{
					double trans_x, trans_y, trans_z;

					trans_x = f_new * scale / (param_3D.m_dstSize.width - 1) 
						* (j - (param_3D.m_dstSize.width - 1) / 2.0) * param_3D.m_magScaleHori;
					trans_y = f_new * scale / (param_3D.m_dstSize.width - 1) 
						* ((param_3D.m_dstSize.height - 1) / 2.0 - i) * param_3D.m_magScaleVert;
					trans_z = trans_y;

					dstLoc.x = imgCenterLoc.x + trans_y * cos(pitchAngle) 
						* sin(rollAngle) + trans_x * cos(rollAngle);
					dstLoc.y = imgCenterLoc.y + trans_y * cos(pitchAngle) 
						* cos(rollAngle) - trans_x * sin(rollAngle);
					dstLoc.z = imgCenterLoc.z + trans_z * sin(pitchAngle);
				}

				//cheng 17.6.1
				//点到直线的求法，为文档中第4中求法:两向量垂直，数量积为0,直线的方向向量与点到垂足的向量垂直
				//先求垂足，垂足到点的距离即点到直线的距离

				double obsPixelVec[3]; // 定义观测点与像素点连线的方向向量
				obsPixelVec[0] = param_3D.m_newObserLoc.x - dstLoc.x;
				obsPixelVec[1] = param_3D.m_newObserLoc.y - dstLoc.y;
				obsPixelVec[2] = param_3D.m_newObserLoc.z - dstLoc.z;

				double coef = obsPixelVec[0] * (modelCenter.x - param_3D.m_newObserLoc.x) 
					+ obsPixelVec[1] * (modelCenter.y - param_3D.m_newObserLoc.y) + obsPixelVec[2] * (modelCenter.z - param_3D.m_newObserLoc.z);

				coef /= obsPixelVec[0] * obsPixelVec[0] + obsPixelVec[1] * obsPixelVec[1] 
					+ obsPixelVec[2] * obsPixelVec[2];

				// 计算过新观测位置与当前像素点的直线与过模型中心并与直线垂直平面的垂足坐标
				//圆心到射线的垂足坐标 cheng 17.5.26
				pedPoint.x = obsPixelVec[0] * coef + param_3D.m_newObserLoc.x;
				pedPoint.y = obsPixelVec[1] * coef + param_3D.m_newObserLoc.y;
				pedPoint.z = obsPixelVec[2] * coef + param_3D.m_newObserLoc.z;

				// 模型中心到直线的距离
				double disCenToLine = sqrt((modelCenter.x - pedPoint.x)*(modelCenter.x - pedPoint.x) +
					(modelCenter.y - pedPoint.y)*(modelCenter.y - pedPoint.y) + (modelCenter.z - pedPoint.z)*(modelCenter.z - pedPoint.z));

				//通过映射到中心坐标系下的Z坐标判断是否在底平面或者球上 cheng 17.5.26
				if (dstLoc.z - param_3D.m_newObserLoc.z > 0) // 射线不可能与底平面有交点
				{
					// 计算射线与球面交点
					double footPtToBall = sqrt(param_3D.m_ballRadius*param_3D.m_ballRadius 
						- disCenToLine*disCenToLine); // 垂足到球面的距离
					CvPoint3D32f lineBall = cvPoint3D32f(pedPoint.x, pedPoint.y, pedPoint.z);
					double obsPixelVecLen = sqrt(obsPixelVec[0] * obsPixelVec[0] 
						+ obsPixelVec[1] * obsPixelVec[1] + obsPixelVec[2] * obsPixelVec[2]);
					lineBall.x -= footPtToBall * obsPixelVec[0] / obsPixelVecLen;
					lineBall.y -= footPtToBall * obsPixelVec[1] / obsPixelVecLen;
					lineBall.z -= footPtToBall * obsPixelVec[2] / obsPixelVecLen;

					float directionVec[3], alpha, beta;
					directionVec[0] = lineBall.x - modelCenter.x;
					directionVec[1] = lineBall.y - modelCenter.y;
					directionVec[2] = lineBall.z - modelCenter.z;

					alpha = atan2(directionVec[0], directionVec[1]); // 保存水平旋转角, 取值范围为[-PI, PI]
					if (alpha < 0)	alpha += (Float32_t)(2 * PI); // 将角度范围转行为[0, 2*PI]
					alpha *= (Float32_t)(180 / PI); // 弧度转化为角度
					// 保存垂直俯仰角
					beta = atan2(directionVec[2], sqrt(directionVec[0] * directionVec[0] 
						+ directionVec[1] * directionVec[1]));  // 取值范围为[-PI/2, PI/2];				
					beta *= (Float32_t)(180 / PI); // 弧度转化为角度
					if (!param_3D.m_is_lumi_balance)
					{
						licheng_modelToBall(resultPoint, alpha, beta,  p_avm_param);
					}
				}
				else // 直线与球面及底面均有交点
				{
					CvPoint3D32f linePlane; // 直线与平面交点
					linePlane.z = 0;
					linePlane.y = param_3D.m_newObserLoc.y - param_3D.m_newObserLoc.z / obsPixelVec[2] * obsPixelVec[1];
					linePlane.x = param_3D.m_newObserLoc.x - param_3D.m_newObserLoc.z / obsPixelVec[2] * obsPixelVec[0];

					// 垂足到直线与底平面交点的距离
					double disPedPlane = sqrt((linePlane.x - pedPoint.x)*(linePlane.x - pedPoint.x) 
						+ (linePlane.y - pedPoint.y)*(linePlane.y - pedPoint.y) + (linePlane.z - pedPoint.z)*(linePlane.z - pedPoint.z));

					if (disPedPlane*disPedPlane + disCenToLine*disCenToLine <= param_3D.m_ballRadius*param_3D.m_ballRadius) /*直线与平面相交*/
					{
						float directionVec[3], alpha;
						directionVec[0] = linePlane.x - modelCenter.x;
						directionVec[1] = linePlane.y - modelCenter.y;
						directionVec[2] = linePlane.z - modelCenter.z;

						alpha = atan2(directionVec[0], directionVec[1]); // 保存水平旋转角, 取值范围为[-PI, PI]
						if (alpha < 0)	alpha += (Float32_t)(2 * PI); // 将角度范围转行为[0, 2*PI]
						alpha *= (Float32_t)(180 / PI); // 弧度转化为角度

						//cheng 17.6.7
						CvPoint3D32f dstPoint = cvPoint3D32f(linePlane.x, linePlane.y, 0);

						licheng_modelToPlane(resultPoint, dstPoint, carVertex, slope, alpha, p_avm_param);


					}
					else// 与球面相交
					{
						// 计算射线与球面交点
						double footPtToBall = sqrt(param_3D.m_ballRadius*param_3D.m_ballRadius - disCenToLine*disCenToLine); // 垂足到球面的距离
						CvPoint3D32f lineBall = cvPoint3D32f(pedPoint.x, pedPoint.y, pedPoint.z);
						lineBall.x -= footPtToBall * obsPixelVec[0] / sqrt(obsPixelVec[0] * obsPixelVec[0] + obsPixelVec[1] * obsPixelVec[1] + obsPixelVec[2] * obsPixelVec[2]);
						lineBall.y -= footPtToBall * obsPixelVec[1] / sqrt(obsPixelVec[0] * obsPixelVec[0] + obsPixelVec[1] * obsPixelVec[1] + obsPixelVec[2] * obsPixelVec[2]);
						lineBall.z -= footPtToBall * obsPixelVec[2] / sqrt(obsPixelVec[0] * obsPixelVec[0] + obsPixelVec[1] * obsPixelVec[1] + obsPixelVec[2] * obsPixelVec[2]);

						float directionVec[3], alpha, beta;
						directionVec[0] = lineBall.x - modelCenter.x;
						directionVec[1] = lineBall.y - modelCenter.y;
						directionVec[2] = lineBall.z - modelCenter.z;

						alpha = atan2(directionVec[0], directionVec[1]); // 保存水平旋转角, 取值范围为[-PI, PI]
						if (alpha < 0)	alpha += (Float32_t)(2 * PI); // 将角度范围转行为[0, 2*PI]
						alpha *= (Float32_t)(180 / PI); // 弧度转化为角度
						// 保存垂直俯仰角
						beta = atan(directionVec[2] / sqrt(directionVec[0] * directionVec[0] + directionVec[1] * directionVec[1]));  // 取值范围为[-PI/2, PI/2];
						beta *= (Float32_t)(180 / PI); // 弧度转化为角度
						if (!param_3D.m_is_lumi_balance)
						{
							licheng_modelToBall(resultPoint, alpha, beta,p_avm_param);
						}
					}
				}///
			}
		}
		param_3D = p_avm_param->p_avm_config_param->avm_3d_param;
		/***********写文件夹*************/

		int countLutItem = (int)m_lut.size();
		LutItem* lutHeader = &m_lut[0];
		CvRect effectROI[CAMERA_NUM];
		for (int i = 0; i < 4; i++)
		{
			effectROI[i].x = param_3D.m_minDstPoint[i].x;
			effectROI[i].y = param_3D.m_minDstPoint[i].y;
			effectROI[i].width = param_3D.m_maxDstPoint[i].x - param_3D.m_minDstPoint[i].x + 1;
			effectROI[i].height = param_3D.m_maxDstPoint[i].y - param_3D.m_minDstPoint[i].y + 1;
		}
		//写文件函数将

		char result_name[1024];
		sprintf(result_name, "%s/%d.bin", lut_file_path, number);
		licheng_binFileGen(result_name, param_3D.m_srcSize, param_3D.m_dstSize, lutHeader, effectROI, countLutItem, number);//binFileGenInte
		
		int gains[12];
		char result_pic[1024];
		sprintf(result_pic, "%s/%d.bmp", result_image_path, number);
		licheng_readBinFile(result_name, result_pic, srcImages, gains);
	}
	
}