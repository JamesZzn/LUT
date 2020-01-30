/*
* This file provides some basic matrix operations.
* Create by Shuo.Yan on 2016/5/13
*/

#include <stdlib.h>
#include <math.h>
#include "math_common.h"

/*
* Function Name:		Mat_Determinant_3by3
* Function Function:	calculate matrix determinant
* Input:
*     M:				the input and output vector
* Note:
*     (1) the function is designed for float type variables
* Revision:
*     (1) Shuo.Yan on 2017/11/22, create log.
*/
Float32_t Mat_Determinant_3by3(IN Float32_t M[9])
{
	return (M[0]*M[4]*M[8]-M[0]*M[5]*M[7]-M[3]*M[1]*M[8]+M[3]*
		M[2]*M[7]+M[6]*M[1]*M[5]-M[6]*M[2]*M[4]);
}

/*
* Function Name:		Mat_Determinant_3by3_DB
* Function Function:	calculate matrix determinant
* Input:
*     M:				the input and output vector
* Note:
*     (1) the function is designed for Float32_t type variables
* Revision:
*     (1) Shuo.Yan on 2017/11/22, create log.
*/
Float32_t Mat_Determinant_3by3_DB(IN Float32_t M[9])
{
	return (M[0] * M[4] * M[8] - M[0] * M[5] * M[7] - M[3] * M[1] * M[8] + M[3] *
		M[2] * M[7] + M[6] * M[1] * M[5] - M[6] * M[2] * M[4]);
}

/*
* Function Name:		Inv_Mat_3by3
* Function Function:	inverse matrix
* Input:
*     pOut:				the output vector
*     pIn:				the input vector
* Note:
*     (1) the function is designed for float type variables
* Revision:
*     (1) Shuo.Yan on 2017/11/22, create log.
*/
unsigned char Inv_Mat_3by3(OUT Float32_t pOut[9], IN Float32_t pIn[9])
{
	Float32_t v0, v1, v2, dDet;
	v0 = pIn[4]*pIn[8] - pIn[5]*pIn[7];
	v1 = pIn[5]*pIn[6] - pIn[3]*pIn[8];
	v2 = pIn[3]*pIn[7] - pIn[4]*pIn[6];
	dDet = v0*pIn[0] + v1*pIn[1] + v2*pIn[2];
	if(dDet == 0)
	{
		return 0;
	}
	else
	{
		dDet = (Float32_t)1.0/dDet;
		pOut[0] = v0*dDet;
		pOut[1] = (pIn[2]*pIn[7]-pIn[1]*pIn[8])*dDet;
		pOut[2] = (pIn[1]*pIn[5]-pIn[2]*pIn[4])*dDet;
		pOut[3] = v1*dDet;
		pOut[4] = (pIn[0]*pIn[8]-pIn[2]*pIn[6])*dDet;
		pOut[5] = (pIn[2]*pIn[3]-pIn[0]*pIn[5])*dDet;
		pOut[6] = v2*dDet;
		pOut[7] = (pIn[1]*pIn[6]-pIn[0]*pIn[7])*dDet;
		pOut[8] = (pIn[0]*pIn[4]-pIn[1]*pIn[3])*dDet;
	}
	return 1;
}

/*
* Function Name:		Inv_Mat_3by3_DB
* Function Function:	inverse matrix
* Input:
*     pOut:				the output vector
*     pIn:				the input vector
* Note:
*     (1) the function is designed for Float32_t type variables
* Revision:
*     (1) Shuo.Yan on 2017/11/22, create log.
*/
unsigned char Inv_Mat_3by3_DB(OUT Float32_t pOut[9], IN Float32_t pIn[9])
{
	Float32_t v0, v1, v2, dDet;
	v0 = pIn[4] * pIn[8] - pIn[5] * pIn[7];
	v1 = pIn[5] * pIn[6] - pIn[3] * pIn[8];
	v2 = pIn[3] * pIn[7] - pIn[4] * pIn[6];
	dDet = v0*pIn[0] + v1*pIn[1] + v2*pIn[2];
	if (dDet == 0)
	{
		return 0;
	}
	else
	{
		dDet = (Float32_t)1.0 / dDet;
		pOut[0] = v0*dDet;
		pOut[1] = (pIn[2] * pIn[7] - pIn[1] * pIn[8])*dDet;
		pOut[2] = (pIn[1] * pIn[5] - pIn[2] * pIn[4])*dDet;
		pOut[3] = v1*dDet;
		pOut[4] = (pIn[0] * pIn[8] - pIn[2] * pIn[6])*dDet;
		pOut[5] = (pIn[2] * pIn[3] - pIn[0] * pIn[5])*dDet;
		pOut[6] = v2*dDet;
		pOut[7] = (pIn[1] * pIn[6] - pIn[0] * pIn[7])*dDet;
		pOut[8] = (pIn[0] * pIn[4] - pIn[1] * pIn[3])*dDet;
	}
	return 1;
}

/*
* Function Name:		Transpose_Mat
* Function Function:	transpose matrix
* Input:
*     At:				the output vector
*     A:				the input matrix
*     row:				source matrix rows
*     col:				source matrix cols
* Note:
*     (1) the function is designed for float type variables
* Revision:
*     (1) Shuo.Yan on 2017/11/22, create log.
*/
void Transpose_Mat(OUT Float32_t *At, IN Float32_t *A, IN int row, IN int col)
{
	int i,j;
	for(i=0; i < row; i++)
	{
		for(j=0; j < col; j++)
		{
			At[j*row+i] = A[i*col+j];
		}
	}
}

/*
* Function Name:		Transpose_Mat_DB
* Function Function:	transpose matrix
* Input:
*     At:				the output vector
*     A:				the input matrix
*     row:				source matrix rows
*     col:				source matrix cols
* Note:
*     (1) the function is designed for Float32_t type variables
* Revision:
*     (1) Shuo.Yan on 2017/11/22, create log.
*/
void Transpose_Mat_DB(
	OUT Float32_t *At, IN Float32_t *A, IN int row, IN int col)
{
	int i, j;
	for (i = 0; i < row; i++)
	{
		for (j = 0; j < col; j++)
		{
			At[j*row + i] = A[i*col + j];
		}
	}
}

/*
* Function Name:		Transpose_Mat_3by3
* Function Function:	transpose matrix
* Input:
*     At:               the transposed matrix
*     A:				the input matrix
* Note:
*     (1) the function is designed for float type variables
* Revision:
*     (1) Shuo.Yan on 2017/11/22, create log.
*/
void Transpose_Mat_3by3(OUT Float32_t At[9], IN Float32_t A[9])
{
	//swap A[1] A[3]
	At[0] = A[0]; At[1] = A[3]; At[2] = A[6];
	//swap A[2] A[6]
	At[3] = A[1]; At[4] = A[4]; At[5] = A[7];
	//swap A[5] A[7]
	At[6] = A[2]; At[7] = A[5]; At[8] = A[8];
}

/*
* Function Name:		Transpose_Mat_3by3_DB
* Function Function:	transpose matrix
* Input:
*     At:               the transposed matrix
*     A:				the input vector
* Note:
*     (1) the function is designed for Float32_t type variables
* Revision:
*     (1) Shuo.Yan on 2017/11/22, create log.
*/
void Transpose_Mat_3by3_DB(OUT Float32_t At[9], IN Float32_t A[9])
{
	//swap A[1] A[3]
	At[0] = A[0]; At[1] = A[3]; At[2] = A[6];
	//swap A[2] A[6]
	At[3] = A[1]; At[4] = A[4]; At[5] = A[7];
	//swap A[5] A[7]
	At[6] = A[2]; At[7] = A[5]; At[8] = A[8];
}

/*
* Function Name:		Transpose_Mat_3by3_InPlace
* Function Function:	transpose matrix in place
* Input:
*     A:				the input and output vector
* Note:
*     (1) the function is designed for float type variables
* Revision:
*     (1) Shuo.Yan on 2017/11/22, create log.
*/
void Transpose_Mat_3by3_InPlace(OUT IN Float32_t A[9])
{
	Float32_t tmp;
	//swap A[1] A[3]
	tmp = A[1]; A[1] = A[3]; A[3] = tmp;
	//swap A[2] A[6]
	tmp = A[2]; A[2] = A[6]; A[6] = tmp;
	//swap A[5] A[7]
	tmp = A[5]; A[5] = A[7]; A[7] = tmp;
}

/*
* Function Name:		Transpose_Mat_3by3_InPlace_DB
* Function Function:	transpose matrix in place
* Input:
*     A:				the input and output matrix
* Note:
*     (1) the function is designed for Float32_t type variables
* Revision:
*     (1) Shuo.Yan on 2017/11/22, create log.
*/
void Transpose_Mat_3by3_InPlace_DB(OUT IN Float32_t A[9])
{
	Float32_t tmp;
	//swap A[1] A[3]
	tmp = A[1]; A[1] = A[3]; A[3] = tmp;
	//swap A[2] A[6]
	tmp = A[2]; A[2] = A[6]; A[6] = tmp;
	//swap A[5] A[7]
	tmp = A[5]; A[5] = A[7]; A[7] = tmp;
}

/*
* Function Name:		Mat_Mul_3by3
* Function Function:	calculate a 3*3 matrix multiply a 3*1 vector
* Input:
*     Cout:				result vector
*     Ain:				the input matrix
*     Bin:				the input vector
* Note:
*     (1) the function is designed for float type variables
* Revision:
*     (1) Shuo.Yan on 2017/11/22, create log.
*/
void Mat_Mul_3by3(
	OUT Float32_t Cout[9], IN Float32_t Ain[9], IN Float32_t Bin[9])
{
	Cout[0] = Ain[0] * Bin[0] + Ain[1] * Bin[3] + Ain[2] * Bin[6];
	Cout[1] = Ain[0] * Bin[1] + Ain[1] * Bin[4] + Ain[2] * Bin[7];
	Cout[2] = Ain[0] * Bin[2] + Ain[1] * Bin[5] + Ain[2] * Bin[8];

	Cout[3] = Ain[3] * Bin[0] + Ain[4] * Bin[3] + Ain[5] * Bin[6];
	Cout[4] = Ain[3] * Bin[1] + Ain[4] * Bin[4] + Ain[5] * Bin[7];
	Cout[5] = Ain[3] * Bin[2] + Ain[4] * Bin[5] + Ain[5] * Bin[8];

	Cout[6] = Ain[6] * Bin[0] + Ain[7] * Bin[3] + Ain[8] * Bin[6];
	Cout[7] = Ain[6] * Bin[1] + Ain[7] * Bin[4] + Ain[8] * Bin[7];
	Cout[8] = Ain[6] * Bin[2] + Ain[7] * Bin[5] + Ain[8] * Bin[8];
}

/*
* Function Name:		Mat_Mul_3by3_DB
* Function Function:	calculate a 3*3 matrix multiply a 3*1 vector
* Input:
*     Cout:				result vector
*     Ain:				the input matrix
*     Bin:				the input vector
* Note:
*     (1) the function is designed for Float32_t type variables
* Revision:
*     (1) Shuo.Yan on 2017/11/22, create log.
*/
void Mat_Mul_3by3_DB(
	OUT Float32_t Cout[9], IN Float32_t Ain[9], IN Float32_t Bin[9])
{
	Cout[0] = Ain[0] * Bin[0] + Ain[1] * Bin[3] + Ain[2] * Bin[6];
	Cout[1] = Ain[0] * Bin[1] + Ain[1] * Bin[4] + Ain[2] * Bin[7];
	Cout[2] = Ain[0] * Bin[2] + Ain[1] * Bin[5] + Ain[2] * Bin[8];

	Cout[3] = Ain[3] * Bin[0] + Ain[4] * Bin[3] + Ain[5] * Bin[6];
	Cout[4] = Ain[3] * Bin[1] + Ain[4] * Bin[4] + Ain[5] * Bin[7];
	Cout[5] = Ain[3] * Bin[2] + Ain[4] * Bin[5] + Ain[5] * Bin[8];

	Cout[6] = Ain[6] * Bin[0] + Ain[7] * Bin[3] + Ain[8] * Bin[6];
	Cout[7] = Ain[6] * Bin[1] + Ain[7] * Bin[4] + Ain[8] * Bin[7];
	Cout[8] = Ain[6] * Bin[2] + Ain[7] * Bin[5] + Ain[8] * Bin[8];
}

/*
* Function Name:		Mat_Mul_Vec_3by3
* Function Function:	calculate a 3*3 matrix multiply a 3*1 vector
* Input:
*     Cout:				result vector
*     Ain:				the input matrix
*     Bin:				the input vector
* Note:
*     (1) the function is designed for float type variables
* Revision:
*     (1) Shuo.Yan on 2017/11/22, create log.
*/
void Mat_Mul_Vec_3by3(
	OUT Float32_t Cout[3], IN Float32_t Ain[9], IN Float32_t Bin[3])
{
	Cout[0] = Ain[0] * Bin[0] + Ain[1] * Bin[1] + Ain[2] * Bin[2];
	Cout[1] = Ain[3] * Bin[0] + Ain[4] * Bin[1] + Ain[5] * Bin[2];
	Cout[2] = Ain[6] * Bin[0] + Ain[7] * Bin[1] + Ain[8] * Bin[2];
}

/*
* Function Name:		Mat_Mul_Vec_3by3_DB
* Function Function:	calculate a 3*3 matrix multiply a 3*1 vector
* Input:
*     Cout:				result vector
*     Ain:				the input matrix
*     Bin:				the input vector
* Note:
*     (1) the function is designed for Float32_t type variables
* Revision:
*     (1) Shuo.Yan on 2017/11/22, create log.
*/
void Mat_Mul_Vec_3by3_DB(
	OUT Float32_t Cout[3], IN Float32_t Ain[9], IN Float32_t Bin[3])
{
	Cout[0] = Ain[0] * Bin[0] + Ain[1] * Bin[1] + Ain[2] * Bin[2];
	Cout[1] = Ain[3] * Bin[0] + Ain[4] * Bin[1] + Ain[5] * Bin[2];
	Cout[2] = Ain[6] * Bin[0] + Ain[7] * Bin[1] + Ain[8] * Bin[2];
}

/*
* Function Name:		Inverse_RT_InPlace
* Function Function:	inverse rotation matrix and trans vector in place
* Input:
*     R:				inversed rotation matrix
*     T:				inversed transition vector
* Note:
*     (1) the function is designed for float type variables
* Revision:
*     (1) Shuo.Yan on 2017/11/22, create log.
*/
void Inverse_RT_InPlace(OUT IN Float32_t R[9], OUT IN Float32_t T[3])
{
	Float32_t t[3];
	Transpose_Mat_3by3_InPlace(R);
	Mat_Mul_Vec_3by3(t, R, T);
	T[0] = -t[0]; T[1] = -t[1]; T[2] = -t[2];
}

/*
* Function Name:		Inverse_RT_InPlace_DB
* Function Function:	inverse rotation matrix and trans vector in place
* Input:
*     R:				inversed rotation matrix
*     T:				inversed transition vector
* Note:
*     (1) the function is designed for Float32_t type variables
* Revision:
*     (1) Shuo.Yan on 2017/11/22, create log.
*/
void Inverse_RT_InPlace_DB(OUT IN Float32_t R[9], OUT IN Float32_t T[3])
{
	Float32_t t[3];
	Transpose_Mat_3by3_InPlace_DB(R);
	Mat_Mul_Vec_3by3_DB(t, R, T);
	T[0] = -t[0]; T[1] = -t[1]; T[2] = -t[2];
}

/*
* Function Name:		Inverse_RT
* Function Function:	inverse rotation matrix and trans vector
* Input:
*     Rout:				inversed rotation matrix
*     Tout:				inversed transition vector
*     Rin:				origin rotation matrix
*     Tout:				origin transition vector
* Note:
*     (1) the function is designed for float type variables
* Revision:
*     (1) Shuo.Yan on 2017/11/22, create log.
*/
void Inverse_RT(OUT Float32_t Rout[9], OUT Float32_t Tout[3], 
	IN Float32_t Rin[9], IN Float32_t Tin[3])
{
	Transpose_Mat_3by3(Rout, Rin);
	Mat_Mul_Vec_3by3(Tout, Rout, Tin);
	Tout[0] = -Tout[0]; Tout[1] = -Tout[1]; Tout[2] = -Tout[2];
}

/*
* Function Name:		Inverse_RT_DB
* Function Function:	inverse rotation matrix and trans vector
* Input:
*     Rout:				inversed rotation matrix
*     Tout:				inversed transition vector
*     Rin:				origin rotation matrix
*     Tout:				origin transition vector
* Note:
*     (1) the function is designed for Float32_t type variables
* Revision:
*     (1) Shuo.Yan on 2017/11/22, create log.
*/
void Inverse_RT_DB(OUT Float32_t Rout[9], OUT Float32_t Tout[3], 
	IN Float32_t Rin[9], IN Float32_t Tin[3])
{
	Transpose_Mat_3by3_DB(Rout, Rin);
	Mat_Mul_Vec_3by3_DB(Tout, Rout, Tin);
	Tout[0] = -Tout[0]; Tout[1] = -Tout[1]; Tout[2] = -Tout[2];
}

/*
* Function Name:		Cvt_Rotation_To_Angles
* Function Function:	convert rotation matrix to Euler angles
* Input:
*     R:				rotation matrix
*     angles:		    Euler angle
* Note:
*     (1) the function is designed for float type variables
* Revision:
*     (1) Shuo.Yan on 2017/11/22, create log.
*/
void Cvt_Rotation_To_Angles(IN const Float32_t R[9], OUT Float32_t angles[3])
{
	angles[0] = atan2f(R[7], R[8]);
	angles[1] = atan2f(-R[6], (float)sqrt(R[7] * R[7] + R[8] * R[8]));
	angles[2] = atan2f(R[3], R[0]);
}

/*
 * Function Name:		Cvt_Rotation_To_Angles_DB
 * Function Function:	convert rotation matrix to Euler angles
 * Input:
 *     R:				rotation matrix
 *     angles:		    Euler angle
 * Note:
 *     (1) the function is designed for Float32_t type variables
 * Revision:
 *     (1) Shuo.Yan on 2017/11/22, create log.
 */
void Cvt_Rotation_To_Angles_DB(IN const Float32_t R[9], OUT Float32_t angles[3])
{
	angles[0] = atan2(R[7], R[8]);
	angles[1] = atan2(-R[6], sqrt(R[7]*R[7]+R[8]*R[8]));
	angles[2] = atan2(R[3], R[0]);
}

/*
 * Function Name:		Cvt_Angles_To_Rotation
 * Function Function:	convert Euler angles to rotation matrix
 * Input:
 *   angles:		    Euler angle
 *        R:			rotation matrix
 * Note:
 *     (1) the function is designed for float type variables
 * Revision:
 *     (1) Shuo.Yan on 2017/11/22, create log.
 */
void Cvt_Angles_To_Rotation(OUT Float32_t R[9], IN const Float32_t angles[3])
{
	Float32_t cx = cosf(angles[0]), cy = cosf(angles[1]), cz = cosf(angles[2]);
	Float32_t sx = sinf(angles[0]), sy = sinf(angles[1]), sz = sinf(angles[2]);
	R[0] = cy*cz;	R[1] = -cx*sz+sx*sy*cz;	R[2] = sx*sz+cx*sy*cz;
	R[3] = cy*sz;	R[4] = cx*cz+sx*sy*sz;	R[5] = -sx*cz+cx*sy*sz;
	R[6] = -sy;		R[7] = sx*cy;			R[8] = cx*cy;
}

/*
 * Function Name:		Cvt_Angles_To_Rotation_DB
 * Function Function:	convert Euler angles to rotation matrix
 * Input:
 *     R:				rotation matrix
 *     angles:		    Euler angle
 * Note:
 *     (1) the function is designed for Float32_t type variables
 * Revision:
 *     (1) Shuo.Yan on 2017/11/22, create log.
 */
void Cvt_Angles_To_Rotation_DB(OUT Float32_t R[9], IN const Float32_t angles[3])
{
	Float32_t cx = cos(angles[0]), cy = cos(angles[1]), cz = cos(angles[2]);
	Float32_t sx = sin(angles[0]), sy = sin(angles[1]), sz = sin(angles[2]);
	R[0] = cy*cz;	R[1] = -cx*sz+sx*sy*cz;	R[2] = sx*sz+cx*sy*cz;
	R[3] = cy*sz;	R[4] = cx*cz+sx*sy*sz;	R[5] = -sx*cz+cx*sy*sz;
	R[6] = -sy;		R[7] = sx*cy;			R[8] = cx*cy;
}

/*
* Function Name:		Inverse_Angles
* Function Function:	inverse angles and trans in place
* Input:
*     angles_out:		Euler angle out
*     trans_out:		transition vector out
*     angles_in:		Euler angle in
*     trans_in:			transition vector in
* Note:
*     (1) the function realize Euler angle and transition vector conversion between camera and world
*         coordinate system.
*     (2) the function is designed for float type variables
* Revision:
*     (1) Shuo.Yan on 2017/11/22, create log.
*/
void Inverse_Angles(OUT Float32_t angles_out[3], OUT Float32_t trans_out[3], 
	IN Float32_t angles_in[3], IN Float32_t trans_in[3])
{
	Float32_t R_in[9], R_out[9];
	Cvt_Angles_To_Rotation(R_in, angles_in); // convert Euler angle to rotation matrix
	Inverse_RT(R_out, trans_out, R_in, trans_in); // update R = R' and t = -R'T
	Cvt_Rotation_To_Angles(R_out, angles_out); // update r in order x, y and z
}

/*
* Function Name:		Inverse_Angles_DB
* Function Function:	inverse angles and trans in place
* Input:
*     angles_out:		Euler angle out
*     trans_out:		transition vector out
*     angles_in:		Euler angle in
*     trans_in:			transition vector in
* Note:
*     (1) the function realize Euler angle and transition vector conversion between camera and world
*         coordinate system.
*     (2) the function is designed for Float32_t type variables
* Revision:
*     (1) Shuo.Yan on 2017/11/22, create log.
*/
void Inverse_Angles_DB(OUT Float32_t angles_out[3], OUT Float32_t trans_out[3], 
	IN Float32_t angles_in[3], IN Float32_t trans_in[3])
{
	Float32_t R_in[9], R_out[9];
	Cvt_Angles_To_Rotation_DB(R_in, angles_in); // convert Euler angle to rotation matrix
	Inverse_RT_DB(R_out, trans_out, R_in, trans_in); // update R = R' and t = -R'T
	Cvt_Rotation_To_Angles_DB(R_out, angles_out); // update r in order x, y and z
}

/*
 * Function Name:		Inverse_Angles_InPlace
 * Function Function:	inverse angles and trans in place
 * Input:
 *     angles:			Euler angle
 *     trans:			transition vector
 * Note:
 *     (1) the function realize Euler angle and transition vector conversion between camera and world
 *         coordinate system in place.
 *     (2) the function is designed for float type variables
 * Revision:
 *     (1) Shuo.Yan on 2017/11/22, create log.
*/
void Inverse_Angles_InPlace(
	OUT IN Float32_t angles[3], OUT IN Float32_t trans[3])
{
	Float32_t R[9];
	Cvt_Angles_To_Rotation(R, angles); // convert Euler angle to rotation matrix
	Inverse_RT_InPlace(R, trans); // update R = R' and t = -R'T
	Cvt_Rotation_To_Angles(R, angles); // update r in order x, y and z
}

/*
 * Function Name:		Inverse_Angles_InPlace_DB
 * Function Function:	inverse angles and trans in place
 * Input:
 *     angles:			Euler angle
 *     trans:			transition vector
 * Note:
 *     (1) the function realize Euler angle and transition vector conversion between camera and world
 *	       coordinate system in place.
 *     (2) the function is designed for Float32_t type variables
 * Revision:
 * 	   (1) Shuo.Yan on 2017/11/22, create log.
 */
void Inverse_Angles_InPlace_DB(
	OUT IN Float32_t angles[3], OUT IN Float32_t trans[3])
{
	Float32_t R[9];
	Cvt_Angles_To_Rotation_DB(R, angles); // convert Euler angle to rotation matrix
	Inverse_RT_InPlace_DB(R, trans); // update R = R' and t = -R'T
	Cvt_Rotation_To_Angles_DB(R, angles); // update r in order x, y and z
}

/*
 * Function Name:		svd
 * Function Function:	solve homogeneous function group
 * Input:
 *     a:				source array(m*n)
 *     m:				equation num
 *     n:				variable num, must be 9 for solve homography matrix
 *     w:				eigen value(m)
 *     v:				eigen vector(n*n)
 * Output:
 *     error_code:
 *         0:           success
 *         1:           multiple solutions
 *         2:           insufficient memory
 *         3:           svd function doesn't convergence
 * Note:
 *     (1) the input a should be a two dimensional (m * n) matrix;
 *     (2) m should bigger than n to ensure function group is solvable;
 *     (3) the column vector corresponding to the minimum eigen value in range of[0, n-1]
 *     	is the function group's solution.
 * Revision:
 *     (1) Shuo.Yan on 2017/11/16, remove Macro dependency of SIGN and MAX and add comment.
 */
Int32_t svd(IN Float32_t a[][9], IN Int32_t m, IN Int32_t n, 
	OUT Float32_t w[], OUT Float32_t v[][9])
{
	Int32_t ret = 0;
	Int32_t flag, i, its, j, jj, k, l, nm;
	Float32_t c, f, h, s, x, y, z, tmp;
	Float32_t anorm = 0.0, g = 0.0, scale = 0.0;
	Float32_t *rv1;

	if (m < n) // The matrix is not full rank, has multi solutions
	{
		ret = 1;
		return ret;
	}

	rv1 = (Float32_t *)malloc(n*sizeof(Float32_t));
	if (!rv1) //Insufficient memory
	{
		ret = 2;
		return ret;
	}

	/* Householder reduction to bidiagonal form */
	for (i = 0; i < n; i++)
	{
		/* left-hand reduction */
		l = i + 1;
		rv1[i] = scale * g;
		g = s = scale = 0.0;
		if (i < m)
		{
			for (k = i; k < m; k++)
			{
				scale += fabs((Float32_t)a[k][i]);
			}
			if (scale)
			{
				for (k = i; k < m; k++)
				{
					a[k][i] = ((Float32_t)a[k][i] / scale);
					s += ((Float32_t)a[k][i] * (Float32_t)a[k][i]);
				}
				f = (Float32_t)a[i][i];
				g = (f >= 0.0 ? -sqrt(s) : sqrt(s));
				h = f * g - s;
				a[i][i] = (f - g);
				if (i != n - 1)
				{
					for (j = l; j < n; j++)
					{
						for (s = 0.0, k = i; k < m; k++)
						{
							s += ((Float32_t)a[k][i] * (Float32_t)a[k][j]);
						}
						f = s / h;
						for (k = i; k < m; k++)
						{
							a[k][j] += (f * (Float32_t)a[k][i]);
						}
					}
				}
				for (k = i; k < m; k++)
				{
					a[k][i] = ((Float32_t)a[k][i] * scale);
				}
			}
		}
		w[i] = (scale * g);

		/* right-hand reduction */
		g = s = scale = 0.0;
		if (i < m && i != n - 1)
		{
			for (k = l; k < n; k++)
			{
				scale += fabs((Float32_t)a[i][k]);
			}
			if (scale)
			{
				for (k = l; k < n; k++)
				{
					a[i][k] = ((Float32_t)a[i][k] / scale);
					s += ((Float32_t)a[i][k] * (Float32_t)a[i][k]);
				}
				f = (Float32_t)a[i][l];
				g = (f >= 0.0 ? -sqrt(s) : sqrt(s));
				h = f * g - s;
				a[i][l] = (f - g);
				for (k = l; k < n; k++)
				{
					rv1[k] = (Float32_t)a[i][k] / h;
				}
				if (i != m - 1)
				{
					for (j = l; j < m; j++)
					{
						for (s = 0.0, k = l; k < n; k++)
						{
							s += ((Float32_t)a[j][k] * (Float32_t)a[i][k]);
						}
						for (k = l; k < n; k++)
						{
							a[j][k] += (s * rv1[k]);
						}
					}
				}
				for (k = l; k < n; k++)
				{
					a[i][k] = ((Float32_t)a[i][k] * scale);
				}
			}
		}
		if (anorm < fabs((Float32_t)w[i]) + fabs(rv1[i]))
			anorm = fabs((Float32_t)w[i]) + fabs(rv1[i]);
	}

	/* accumulate the right-hand transformation */
	for (i = n - 1; i >= 0; i--)
	{
		if (i < n - 1)
		{
			if (g)
			{
				for (j = l; j < n; j++)
				{
					v[j][i] = (((Float32_t)a[i][j] / (Float32_t)a[i][l]) / g);
				}
				/* Float32_t division to avoid underflow */
				for (j = l; j < n; j++)
				{
					for (s = 0.0, k = l; k < n; k++)
					{
						s += ((Float32_t)a[i][k] * (Float32_t)v[k][j]);
					}
					for (k = l; k < n; k++)
					{
						v[k][j] += (s * (Float32_t)v[k][i]);
					}
				}
			}
			for (j = l; j < n; j++)
			{
				v[i][j] = v[j][i] = 0.0;
			}
		}
		v[i][i] = 1.0;
		g = rv1[i];
		l = i;
	}

	/* accumulate the left-hand transformation */
	for (i = n - 1; i >= 0; i--)
	{
		l = i + 1;
		g = (Float32_t)w[i];
		if (i < n - 1)
		{
			for (j = l; j < n; j++)
			{
				a[i][j] = 0.0;
			}
		}
		if (g)
		{
			g = 1.0 / g;
			if (i != n - 1)
			{
				for (j = l; j < n; j++)
				{
					for (s = 0.0, k = l; k < m; k++)
					{
						s += ((Float32_t)a[k][i] * (Float32_t)a[k][j]);
					}
					f = (s / (Float32_t)a[i][i]) * g;
					for (k = i; k < m; k++)
					{
						a[k][j] += (f * (Float32_t)a[k][i]);
					}
				}
			}
			for (j = i; j < m; j++)
			{
				a[j][i] = ((Float32_t)a[j][i] * g);
			}
		}
		else
		{
			for (j = i; j < m; j++)
			{
				a[j][i] = 0.0;
			}
		}
		++a[i][i];
	}

	/* diagonalize the bidiagonal form */
	for (k = n - 1; k >= 0; k--)
	{                             /* loop over singular values */
		for (its = 0; its < 30; its++)
		{                         /* loop over allowed iterations */
			flag = 1;
			for (l = k; l >= 0; l--)
			{                     /* test for splitting */
				nm = l - 1;
				if (fabs(rv1[l]) + anorm == anorm)
				{
					flag = 0;
					break;
				}
				if (fabs((Float32_t)w[nm]) + anorm == anorm)
					break;
			}
			if (flag)
			{
				c = 0.0;
				s = 1.0;
				for (i = l; i <= k; i++)
				{
					f = s * rv1[i];
					if (fabs(f) + anorm != anorm)
					{
						g = (Float32_t)w[i];
						h = sqrt(f*f + g*g);
						w[i] = h;
						h = 1.0 / h;
						c = g * h;
						s = (-f * h);
						for (j = 0; j < m; j++)
						{
							y = (Float32_t)a[j][nm];
							z = (Float32_t)a[j][i];
							a[j][nm] = (y * c + z * s);
							a[j][i] = (z * c - y * s);
						}
					}
				}
			}
			z = (Float32_t)w[k];
			if (l == k)
			{                  /* convergence */
				if (z < 0.0)
				{              /* make singular value nonnegative */
					w[k] = (-z);
					for (j = 0; j < n; j++)
					{
						v[j][k] = (-v[j][k]);
					}
				}
				break;
			}
			if (its >= 30) // svd doesn't convergence after 30,000 times iterations
			{
				free((void*)rv1);				
				ret = 3;
				return ret;
			}

			/* shift from bottom 2 x 2 minor */
			x = (Float32_t)w[l];
			nm = k - 1;
			y = (Float32_t)w[nm];
			g = rv1[nm];
			h = rv1[k];
			f = ((y - z) * (y + z) + (g - h) * (g + h)) / (2.0 * h * y);
			g = sqrt(f*f + 1.0);

			tmp = (f >= 0.0 ? sqrt(g) : -sqrt(g));

			f = ((x - z) * (x + z) + h * ((y / (f + tmp)) - h)) / x;

			/* next QR transformation */
			c = s = 1.0;
			for (j = l; j <= nm; j++)
			{
				i = j + 1;
				g = rv1[i];
				y = (Float32_t)w[i];
				h = s * g;
				g = c * g;
				z = sqrt(f*f + h*h);
				rv1[j] = z;
				c = f / z;
				s = h / z;
				f = x * c + g * s;
				g = g * c - x * s;
				h = y * s;
				y = y * c;
				for (jj = 0; jj < n; jj++)
				{
					x = (Float32_t)v[jj][j];
					z = (Float32_t)v[jj][i];
					v[jj][j] = (x * c + z * s);
					v[jj][i] = (z * c - x * s);
				}
				z = sqrt(f*f + h*h);
				w[j] = z;
				if (z)
				{
					z = 1.0 / z;
					c = f * z;
					s = h * z;
				}
				f = (c * g) + (s * y);
				x = (c * y) - (s * g);
				for (jj = 0; jj < m; jj++)
				{
					y = (Float32_t)a[jj][j];
					z = (Float32_t)a[jj][i];
					a[jj][j] = (y * c + z * s);
					a[jj][i] = (z * c - y * s);
				}
			}
			rv1[l] = 0.0;
			rv1[k] = f;
			w[k] = x;
		}
	}
	if (rv1)
		free(rv1);
	return ret;
}