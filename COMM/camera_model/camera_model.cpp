/*------------------------------------------------------------------------------
	camera_model.cpp realize the basic operating functions defined in camera_model.h
	Add by Shuo.Yan on 2017/10/31 InteSight
------------------------------------------------------------------------------*/
#include "camera_model.h"
#include"avm_config.h"
/*
 * Function Name:				Load_Camera_Model_Int
 * Function Function:			load camera intrinsic param from file(*.txt)
 * Input:
 *     p_camera_model_int:		camera intrinsic param structure
 *     file_name:				file name
 * Note:
 *     In camera coordinate system, to down is x positive direction, to right is y positive direction, to back is z positive z direction
 * Revision:
 *     Rename by Shuo.Yan on 2017/11/23
 */
Int32_t Load_Camera_Model_Int(
	OUT P_Camera_Model_Int_S p_camera_model_int, IN const char* file_name)
{
	Int32_t ret = 0;
	FILE *f;
	char buf[MAX_ARRAY_SIZE];

	Float32_t	*pol = p_camera_model_int->pol;
	Float32_t	*inv_pol = p_camera_model_int->inv_pol;
	Float32_t	*xc = &(p_camera_model_int->xc);
	Float32_t	*yc = &(p_camera_model_int->yc);
	Float32_t	*c = &(p_camera_model_int->c);
	Float32_t	*d = &(p_camera_model_int->d);
	Float32_t	*e = &(p_camera_model_int->e);
	Int32_t		*width = &(p_camera_model_int->width);
	Int32_t		*height = &(p_camera_model_int->height);
	Int32_t		*length_pol = &(p_camera_model_int->length_pol);
	Int32_t		*length_invpol = &(p_camera_model_int->length_inv_pol);
	
	Int32_t i;

	//Open file
	if (!(f = fopen(file_name, "r")))
	{
		printf("File %s cannot be opened\n", file_name);
		ret = 1;
		return ret;
	}
	//Read polynomial coefficients
	fgets(buf, MAX_ARRAY_SIZE, f);
	fscanf(f, "\n");
	fscanf(f, "%d", length_pol);
	for (i = 0; i < *length_pol; i++)
	{
		fscanf(f, " %f", &pol[i]);
	}

	//Read inverse polynomial coefficients
	fscanf(f, "\n");
	fgets(buf, MAX_ARRAY_SIZE, f);
	fscanf(f, "\n");
	fscanf(f, "%d", length_invpol);
	for (i = 0; i < *length_invpol; i++)
	{
		fscanf(f, " %f", &inv_pol[i]);
	}

	//Read center coordinates
	fscanf(f, "\n");
	fgets(buf, MAX_ARRAY_SIZE, f);
	fscanf(f, "\n");
	fscanf(f, "%f %f\n", yc, xc);

	//Read affine coefficients
	fgets(buf, MAX_ARRAY_SIZE, f);
	fscanf(f, "\n");
	fscanf(f, "%f %f %f\n", c, d, e);

	//Read image size
	fgets(buf, MAX_ARRAY_SIZE, f);
	fscanf(f, "\n");
	fscanf(f, "%d %d", height, width);

	fclose(f);
	return ret;
}

void Cam_Ray_To_World_Ray(OUT Float32_t world_ray[3],
	IN Float32_t cam_ray[3], IN P_Camera_Model_Ext_S p_camera_model_ext)
{
	Float64_t coef;

	world_ray[0] = (p_camera_model_ext->R[0] * cam_ray[0]
		+ p_camera_model_ext->R[1] * cam_ray[1]
		+ p_camera_model_ext->R[2] * cam_ray[2]
		);
	world_ray[1] = (p_camera_model_ext->R[3] * cam_ray[0]
		+ p_camera_model_ext->R[4] * cam_ray[1]
		+ p_camera_model_ext->R[5] * cam_ray[2]
		);
	world_ray[2] = (p_camera_model_ext->R[6] * cam_ray[0]
		+ p_camera_model_ext->R[7] * cam_ray[1]
		+ p_camera_model_ext->R[8] * cam_ray[2]
		);

	coef = p_camera_model_ext->pose[5] / (-world_ray[2]);
	world_ray[0] = world_ray[0] * coef + p_camera_model_ext->pose[3];
	world_ray[1] = world_ray[1] * coef + p_camera_model_ext->pose[4];
	world_ray[2] = world_ray[2] * coef + p_camera_model_ext->pose[5];
}
/*
 * Function Name:				Image_Point_To_Cam_Ray
 * Function Function:			calculate image point's coordinate in camera coordinate system
 * Input:
 *     point3D:					incline ray vector in camera coordinate system(x to up(the 1st element), y to right(the 2nd element), z to back(the 3rd element))
 *     point2D:					image point's coordinate in image coordinate system(x to right(the 1st element), y to down(the 2nd element))
 *     p_camera_model_int:		fisheye camera model
 * Note:
 *     In camera coordinate system, to down is x positive direction, to right is y positive direction, to back is z positive z direction
 * Revision:
 */
void Image_Point_To_Cam_Ray(OUT Float32_t cam_ray[3], 
	IN Float32_t image_point[2], 
	IN P_Camera_Model_Int_S p_camera_model_int)
{
	Float32_t* pol = p_camera_model_int->pol;
	Float32_t  xc = (p_camera_model_int->xc);
	Float32_t  yc = (p_camera_model_int->yc);
	Float32_t  c = (p_camera_model_int->c);
	Float32_t  d = (p_camera_model_int->d);
	Float32_t  e = (p_camera_model_int->e);
	Int32_t    length_pol = (p_camera_model_int->length_pol);

	// 1/det(A), where A = [c,d;e,1] as in the Matlab file
	Float32_t  invdet = 1 / (c - d * e); 

	Float32_t  xp = invdet 
		* ((image_point[0] - yc) - d * (image_point[1] - xc));
	Float32_t  yp = invdet
		* (-e * (image_point[0] - yc) + c * (image_point[1] - xc));

	//distance [pixels] of  the point from the image center
	Float32_t  r = sqrt(xp*xp + yp*yp); 
	Float32_t  zp = pol[0];
	Float32_t  r_i = 1;
	Int32_t i;

	for (i = 1; i < length_pol; i++)
	{
		r_i *= r;
		zp += r_i * pol[i];
	}

	//normalize to unit norm
	Float32_t invnorm = 1 / sqrt(xp*xp + yp*yp + zp*zp);

	cam_ray[0] = invnorm * xp;
	cam_ray[1] = invnorm * yp;
	cam_ray[2] = invnorm * zp;
}

/*
 * Function Name :			Cam_Ray_To_Image_Point
 * Function Function :		calculate image point's coordinate in camera coordinate system
 * Input :
 *     point2D :			image point's coordinate in image coordinate system(y to down(the 1st element, x to right(the 2nd element) ))
 * 	   point3D :			incline ray vector in camera coordinate system(x to up(the 1st element), y to right(the 2nd element), z to back(the 3rd element))
 * 	   p_camera_model_int : fisheye camera model
 * Note :
 *     In camera coordinate system, to down is x positive direction, to right is y positive direction, to back is z positive z direction
 * Revision :
 *     Rename by Shuo.Yan on 2017/11/23
 *///image_point[0]
void Cam_Ray_To_Image_Point(OUT Float32_t image_point[2], 
	IN Float32_t cam_ray[3], IN P_Camera_Model_Int_S p_camera_model_int)
{
	Float32_t* inv_pol = p_camera_model_int->inv_pol;
	Float32_t  xc = (p_camera_model_int->xc);
	Float32_t  yc = (p_camera_model_int->yc);
	Float32_t  c = (p_camera_model_int->c);
	Float32_t  d = (p_camera_model_int->d);
	Float32_t  e = (p_camera_model_int->e);
	Int32_t    width = (p_camera_model_int->width);
	Int32_t    height = (p_camera_model_int->height);
	Int32_t    length_invpol = (p_camera_model_int->length_inv_pol);
	Float32_t  norm = sqrt(cam_ray[0] * cam_ray[0] + cam_ray[1] * cam_ray[1]);
	Float32_t  theta = atan(cam_ray[2] / norm);
	Float32_t  t, t_i;
	Float32_t  rho, x, y;
	Float32_t  invnorm;
	Int32_t i;

	if (norm != 0)
	{
		invnorm = 1 / norm;
		t = theta;
		rho = inv_pol[0];
		t_i = 1;

		for (i = 1; i < length_invpol; i++)
		{
			t_i *= t;
			rho += t_i * inv_pol[i];//相当于一个公式y=a0+a1*x+a2*x^2+a3*x^3,x相当于入射角x=theta = atan(cam_ray[2] / norm);
		}

		x = cam_ray[0] * invnorm * rho;//norm = sqrt(cam_ray[0] * cam_ray[0] + cam_ray[1] * cam_ray[1]);invnorm = 1 / norm;//cam_ray[0] *1 / sqrt(cam_ray[0] * cam_ray[0] + cam_ray[1] * cam_ray[1])
		y = cam_ray[1] * invnorm * rho;//

		image_point[0] = x * c + y * d + yc;
		image_point[1] = x * e + y * 1 + xc;
	}
	else
	{
		image_point[0] = yc;
		image_point[1] = xc;
	}
}

/*
 * Function Name :			Load_Camera_Model_Ext
 * Function Function :		Load camera extrinsic param(Euler angle & trans vector from camera coordinate system to vehicle coordinate system)
 * Input :
 *     p_camera_model_ext:	camera extrinsic param structure
 * 	   file_name :			extrinsic file name for one camera
 * Note :
 *     the function read the Euler angle and translation vector from camera to vehicle coordinate system to camera extrinsic param structure
 * Revision :
 *     Add by Shuo.Yan on 2017/11/23
 */
Int32_t Load_Camera_Model_Ext(
	OUT P_Camera_Model_Ext_S p_camera_model_ext, IN const char* file_name)
{
	Int32_t ret = 0;
	FILE* f;
	Int32_t param_id = 0;
	char buf[MAX_ARRAY_SIZE];
	if (!(f = fopen(file_name, "r")))
	{
		printf("Extrinsic param file %s cannot be opened\n", file_name);
		ret = 1;
		return ret;
	}
	/*for (param_id = 0; param_id < 3; param_id++)
	{
		fscanf(f, "%f ", &p_camera_model_ext->inv_R[param_id]);
	}
	fscanf(f, "\n");
	for (param_id = 3; param_id < 6; param_id++)
	{
		fscanf(f, "%f ", &p_camera_model_ext->inv_R[param_id]);
	}
	fscanf(f, "\n");
	for (param_id = 6; param_id < 9; param_id++)
	{
		fscanf(f, "%f ", &p_camera_model_ext->inv_R[param_id]);
	}
	fscanf(f, "\n");
	for (param_id = 3; param_id < 6; param_id++)
	{
		fscanf(f, "%f ", &p_camera_model_ext->inv_pose[param_id]);
	}*/
	fgets(buf, MAX_ARRAY_SIZE, f);
	fscanf(f, "\n");
	for (param_id = 0; param_id < 6; param_id++)
	{
		fscanf(f, "%f ", &p_camera_model_ext->pose[param_id]);
	}
	/*for (param_id = 0; param_id < 6; param_id++)
	{
		printf("gg%f\t ", p_camera_model_ext->inv_pose[param_id]);
	}*/
	//while (1);
	fclose(f);

	return ret;
}

/*
 * Function Name :			Sync_Camera_Model_Ext
 * Function Function :		Update all extrinsic params by given angle and translation vector from camera to vehicle coordinate system
 * Input :
 *     p_camera_model_ext:	camera extrinsic param structure
 * Note :
 *     The function keep each camera extrinsic parameter' representation format in consistent with each other. only pose array is correctly 
 *     in this function by default, once this array is updated, all other extrinsic param should by updated correspondingly to keep consistency.
 * Revision :
 *     Add by Shuo.Yan on 2017/11/23
 */
void Sync_Camera_Model_Ext(OUT IN P_Camera_Model_Ext_S p_camera_model_ext)
{
	// Update rotation matrix by Euler angle
	Cvt_Angles_To_Rotation_DB(p_camera_model_ext->R, p_camera_model_ext->pose);

	// Update inv_R and translation vector in inv_pose by R and t
	Inverse_RT_DB(p_camera_model_ext->inv_R, &p_camera_model_ext->inv_pose[3], 
		p_camera_model_ext->R, &p_camera_model_ext->pose[3]);

	// Update Euler angle vector in inv_pose by inv_R
	Cvt_Rotation_To_Angles_DB(p_camera_model_ext->inv_R, 
		p_camera_model_ext->inv_pose);
}
