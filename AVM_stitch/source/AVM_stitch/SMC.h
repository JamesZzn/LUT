/*===========================================================================*\
* smc_major_version : 1
* DATA : 2018/5/28
\*===========================================================================*/

#ifndef _SMC_H_
#define _SMC_H_
typedef unsigned char       UInt8_t;
typedef unsigned int        UInt32_t;
typedef signed int          Int32_t;
typedef float               Float32_t;

/*===========================================================================*\
* Exported Preprocessor #define Constants
\*===========================================================================*/

#define SMC_MAJOR_VERSION    ((uint8_t)1)

enum { CAMERA_VIEW_FRONT = 0, CAMERA_VIEW_REAR, CAMERA_VIEW_LEFT, CAMERA_VIEW_RIGHT, CAMERA_VIEW_CNT };
enum { LENS_LUT_LINEAR = 0, LENS_LUT_4121, LENS_LUT_4N321, LENS_LUT_4039, LENS_LUT_4051, LENS_LUT_4137, LENS_LUT_DSL215, LENS_LUT_4029, LENS_LUT_RESEVED1, LENS_LUT_RESEVED2, LENS_LUT_NUM };

/*===========================================================================*\
* Exported Type Declarations
\*===========================================================================*/
typedef struct Smc_Veh_Param_Struct
{
	Float32_t           veh_length;                                                    /* millimeter     vehicle_lengthvehicle_length  */
	Float32_t           veh_width;                                                    /* millimeter     vehicle_widthvehicle_width  */
	Float32_t           veh_fwheel2head;                                                 /* millimeter     vehicle_front_suspensioin_lengthvehicle_front_suspensioin_length  */
	Float32_t           veh_axis_length;                                                 /* millimeter     vehicle_wheel_basevehicle_wheel_base  */
	Float32_t           veh_rwheel2tail;                                                 /* millimeter     vehicle_rear_suspension_length_vehicle_rear_suspension_length   */
	Float32_t           veh_tier_r;                                                    /* millimeter     vehicle_tire_radiusvehicle_tire_radius  */
	Float32_t           vehicle_min_radius_forward_lefturn_rear_wheel_center;            /* millimeter     the_minimum_radius_when_turn_front-left_based_on_rear_wheel_center_the_minimum_radius_when_turn_front-left_based_on_rear_wheel_center   */
	Float32_t           vehicle_min_radius_forward_rightturn_rear_wheel_center;          /* millimeter     the_minimum_radius_when_turn_front-right_based_on_rear_wheel_center_the_minimum_radius_when_turn_front-right_based_on_rear_wheel_center   */
	Float32_t           vehicle_min_radius_reverse_leftturn_rear_wheel_center;           /* millimeter     the_minimum_radius_when_turn_rear-left_based_on_rear_wheel_center_the_minimum_radius_when_turn_rear-left_based_on_rear_wheel_center   */
	Float32_t           vehicle_min_radius_reverse_rightturn_rear_wheel_center;          /* millimeter     the_minimum_radius_when_turn_rear-right_based_on_rear_wheel_center_the_minimum_radius_when_turn_rear-right_based_on_rear_wheel_center   */
	Int32_t             vehicle_max_steering_wheel_angle_leftturn;                       /* degree         the_maximum_angle_of_steering_wheel_when_turn_leftthe_maximum_angle_of_steering_wheel_when_turn_left  */
	Int32_t             vehicle_max_steering_wheel_angle_rightturn;                      /* degree         the_maximum_angle_of_steering_wheel_when_turn_rightthe_maximum_angle_of_steering_wheel_when_turn_right  */
}
Smc_Veh_Param_S, *Smc_P_Veh_Param_S;

typedef struct Smc_Camera_Model_Cam_Int_Struct
{
	Int32_t             length_pol;                                                      /* pixel          length of polynomial  */
	Float32_t           pol[16];                                                       /* pixel            */
	Int32_t             length_inv_pol;                                                  /* pixel          length of inverse polynomial  */
	Float32_t           inv_pol[16];                                                     /* pixel          the coefficients of the inverse polynomial  */
	Float32_t           xc;                                                       /* N/A            column coordinate of the center  */
	Float32_t           yc;                                                       /* N/A            row coordinate of the center  */
	Float32_t           c;                                                       /* N/A            affine parameter  */
	Float32_t           d;                                                       /* radian         affine parameter  */
	Float32_t           e;                                                       /* radian         affine parameter  */
	Int32_t             fov_h;                                                       /* millimeter     camera horizontal field of view  */
	Int32_t             fov_v;                                                       /* millimeter     camera vertical field of view  */
	Int32_t             width;                                                       /* millimeter     image width  */
	Int32_t             height;                                                       /* millimeter     image height  */
	Int32_t             is_lut_used;                                                     /* N/A            whether choose to use lut represents lens distortion  */
	Int32_t             lut;                                                       /* N/A            lens distortion lut, one of lut or (pol and inv_pol) must be chosen to describe lens distortion  */
}
Smc_Camera_Model_Cam_Int_S, *Smc_P_Camera_Model_Cam_Int_S;

typedef struct Smc_Camera_Model_Cam_Ext_Struct
{
	Float32_t           angle_x;                                                       /* radian         the_rotation_angle_around_x_of_the_front_camera  */
	Float32_t           angle_y;                                                       /* radian         the_rotation_angle_around_y_of_the_front_camera  */
	Float32_t           angle_z;                                                       /* radian         the_rotation_angle_around_z_of_the_front_camera  */
	Float32_t           tx;                                                       /* millimeter     x_translation_of_the_front_camera  */
	Float32_t           ty;                                                       /* millimeter     y_translation_of_the_front_camera  */
	Float32_t           tz;                                                       /* millimeter     z_translation_of_the_front_camera  */
}
Smc_Camera_Model_Cam_Ext_S, *Smc_P_Camera_Model_Cam_Ext_S;

typedef struct Smc_Camera_Model_Struct
{
	Smc_Camera_Model_Cam_Int_S                     cam_int;
	Smc_Camera_Model_Cam_Ext_S                     cam_ext;
}
Smc_Camera_Model_S, *Smc_P_Camera_Model_S;

typedef struct Smc_Bev_Calib_Param_Bev_EOL_Param_Struct
{
	Int32_t             station_type;                                                    /* N/A            the station type  */
	Float32_t           board_veh_flboard2centeraxis;                                    /* millimeter     the distance from front left calibration board to vehicle center axis  (Eol1.0 and 2.0)  */
	Float32_t           board_veh_flboard2fwheel;                                        /* millimeter     the distance from front left calibration board to vehicle front wheel (Eol1.0 and 2.0)  */
	Int32_t             left_right_board_num;                                            /* N/A            the board num in left and right view (Eol2.0)  */
	Float32_t           station_height;                                                  /* millimeter     the height of calibration station (Eol2.0)  */
	Float32_t           station_width;                                                   /* millimeter     the width of calibration station (Eol2.0)  */
	Float32_t           board_flboard2lcboard;                                           /* millimeter     the distance from front left board to left center board (intesight_4s)  */
	Float32_t           board_blboard2lcboard;                                           /* millimeter     the distance from back left board to left center board (intesight_4s)  */
	Float32_t           front_center_deviation;                                          /* millimeter     the deviation of front center board and veh axis   */
}
Smc_Bev_Calib_Param_Bev_EOL_Param_S, *Smc_P_Bev_Calib_Param_Bev_EOL_Param_S;

typedef struct Smc_Bev_Calib_Param_Struct
{
	Smc_Bev_Calib_Param_Bev_EOL_Param_S               bev_eol_param;
}
Smc_Bev_Calib_Param_S, *Smc_P_Bev_Calib_Param_S;

typedef struct Smc_Junc_View_4_Config_Common_Struct
{
	Float32_t           front_view_length;                                               /*  millimeter     front_view_length  */
	Float32_t           rear_view_length;                                                /*  millimeter       */
	Float32_t           left_view_length;                                                /*                   */
	Float32_t           right_view_length;                                               /*                   */
	Float32_t           front_blind_length;                                              /*                   */
	Float32_t           rear_blind_length;                                               /*                   */
	Float32_t           fl_fusion_angle;                                                 /*                   */
	Float32_t           fr_fusion_angle;                                                 /*                   */
	Float32_t           rl_fusion_angle;                                                 /*                   */
	Float32_t           rr_fusion_angle;                                                 /*                   */
	Float32_t           curve_range;                                                    /*                   */
	Float32_t           parallel_range;                                                  /*                   */
}
Smc_Junc_View_4_Config_Common_S, *Smc_P_Junc_View_4_Config_Common_S;

typedef struct Smc_Small_View_Result_Size_Param_Struct
{
	Int32_t           result_image_width;                                              /*                   */
	Int32_t           result_image_height;                                             /*                   */
	Int32_t            car_logo_rect_x;                                                 /*                   */
	Int32_t            car_logo_rect_y;                                                 /*                   */
	Int32_t           car_logo_rect_width;                                             /*                   */
	Int32_t            car_logo_rect_height;                                            /*                   */
}
Smc_Small_View_Result_Size_Param_S, *Smc_P_Small_View_Result_Size_Param_S;

typedef struct Smc_Small_View_Dis_Line_Param_Struct
{
	Int32_t             is_static_line_exist;                                            /*                   */
	Int32_t             static_line_to_veh_distance_0;                                   /*                   */
	Int32_t             static_line_to_veh_distance_1;                                   /*                   */
	Int32_t             static_line_to_veh_distance_2;                                   /*                   */
	Int32_t             static_line_to_veh_distance_3;                                   /*                   */
	Int32_t             static_line_length;                                              /*                   */
	Int32_t             static_line_pixel_count;                                         /*                   */
	Int32_t             is_dynamic_line_exist;                                           /*                   */
	Int32_t             dynamic_line_count;                                              /*                   */
	Int32_t             dynamic_line_length;                                             /*                   */
}
Smc_Small_View_Dis_Line_Param_S, *Smc_P_Small_View_Dis_Line_Param_S;

typedef struct Smc_Big_View_Result_Size_Param_Struct
{
	Int32_t            result_image_width;                                              /*                   */
	Int32_t            result_image_height;                                             /*                   */
	Int32_t           car_logo_rect_x;                                                 /*                   */
	Int32_t           car_logo_rect_y;                                                 /*                   */
	Int32_t           car_logo_rect_width;                                             /*                   */
	Int32_t            car_logo_rect_height;                                            /*                   */
}
Smc_Big_View_Result_Size_Param_S, *Smc_P_Big_View_Result_Size_Param_S;

typedef struct Smc_Big_View_Dis_Line_Param_Struct
{
	Int32_t             is_static_line_exist;                                            /*                   */
	Int32_t             static_line_to_veh_distance_0;                                   /*                   */
	Int32_t             static_line_to_veh_distance_1;                                   /*                   */
	Int32_t             static_line_to_veh_distance_2;                                   /*                   */
	Int32_t             static_line_to_veh_distance_3;                                   /*                   */
	Int32_t             static_line_length;                                              /*                   */
	Int32_t             static_line_pixel_count;                                         /*                   */
	Int32_t             is_dynamic_line_exist;                                           /*                   */
	Int32_t             dynamic_line_count;                                              /*                   */
	Int32_t             dynamic_line_length;                                             /*                   */
}
Smc_Big_View_Dis_Line_Param_S, *Smc_P_Big_View_Dis_Line_Param_S;

typedef struct Smc_Junc_View_4_Param_Struct
{
	Smc_Junc_View_4_Config_Common_S               junc_view_4_config_common;
	Smc_Small_View_Result_Size_Param_S            small_view_result_size_param;
	Smc_Small_View_Dis_Line_Param_S               small_view_dis_line_param;
	Smc_Big_View_Result_Size_Param_S              big_view_result_size_param;
	Smc_Big_View_Dis_Line_Param_S                 big_view_dis_line_param;
}
Smc_Junc_View_4_Param_S, *Smc_P_Junc_View_4_Param_S;

typedef struct Smc_Awb_View_4_Config_Common_Struct
{
	Float32_t           front_view_length;                                               /*                   */
	Float32_t           rear_view_length;                                                /*                   */
	Float32_t           left_view_length;                                                /*                   */
	Float32_t           right_view_length;                                               /*                   */
	Float32_t           front_blind_length;                                              /*                   */
	Float32_t           rear_blind_length;                                               /*                   */
	Float32_t           fl_fusion_angle;                                                 /*                   */
	Float32_t           fr_fusion_angle;                                                 /*                   */
	Float32_t           rl_fusion_angle;                                                 /*                   */
	Float32_t           rr_fusion_angle;                                                 /*                   */
	Float32_t           curve_range;                                                    /*                   */
	Float32_t           parallel_range;                                                  /*                   */
}
Smc_Awb_View_4_Config_Common_S, *Smc_P_Awb_View_4_Config_Common_S;

typedef struct Smc_Result_Awb_Param_Struct
{
	Float32_t           result_image_width;                                              /*                   */
	Float32_t           result_image_height;                                             /*                   */
	Float32_t           car_logo_rect_x;                                                 /*                   */
	Float32_t           car_logo_rect_y;                                                 /*                   */
	Float32_t           car_logo_rect_width;                                             /*                   */
	Float32_t           car_logo_rect_height;                                            /*                   */
}
Smc_Result_Awb_Param_S, *Smc_P_Result_Awb_Param_S;

typedef struct Smc_Awb_View_Param_Struct
{
	Smc_Awb_View_4_Config_Common_S                awb_view_4_config_common;
	Smc_Result_Awb_Param_S                        result_awb_param;
}
Smc_Awb_View_Param_S, *Smc_P_Awb_View_Param_S;

typedef struct Smc_Junc_View_3_Config_Common_Front_Struct
{
	Float32_t           front_view_length;                                               /*                   */
	Float32_t           left_view_length;                                                /*                   */
	Float32_t           right_view_length;                                               /*                   */
	Float32_t           car_head_length;                                                 /*                   */
	Float32_t           front_blind_length;                                              /*                   */
	Float32_t           fr_fusion_angle;                                                 /*                   */
	Float32_t           rl_fusion_angle;                                                 /*                   */
	Float32_t           curve_range;                                                    /*                   */
	Float32_t           parallel_range;                                                  /*                   */
}
Smc_Junc_View_3_Config_Common_Front_S, *Smc_P_Junc_View_3_Config_Common_Front_S;

typedef struct Smc_Junc_3_Dis_Line_Param_Front_Struct
{
	Int32_t             is_static_line_exist;                                            /*                   */
	Int32_t             static_line_to_veh_distance_0;                                   /*                   */
	Int32_t             static_line_to_veh_distance_1;                                   /*                   */
	Int32_t             static_line_to_veh_distance_2;                                   /*                   */
	Int32_t             static_line_to_veh_distance_3;                                   /*                   */
	Int32_t             static_line_length;                                              /*                   */
	Int32_t             static_line_pixel_count;                                         /*                   */
	Int32_t             is_dynamic_line_exist;                                           /*                   */
	Int32_t             dynamic_line_count;                                              /*                   */
	Int32_t             dynamic_line_length;                                             /*                   */
}
Smc_Junc_3_Dis_Line_Param_Front_S, *Smc_P_Junc_3_Dis_Line_Param_Front_S;

typedef struct Smc_Result_3_Param_Front_Struct
{
	Int32_t           result_image_width;                                              /*                   */
	Int32_t            result_image_height;                                             /*                   */
	Int32_t            car_logo_rect_x;                                                 /*                   */
	Int32_t            car_logo_rect_y;                                                 /*                   */
	Int32_t            car_logo_rect_width;                                             /*                   */
	Int32_t            car_logo_rect_height;                                            /*                   */
}
Smc_Result_3_Param_Front_S, *Smc_P_Result_3_Param_Front_S;

typedef struct Smc_Junc_View_3_Front_Param_Struct
{
	Smc_Junc_View_3_Config_Common_Front_S         junc_view_3_config_common_front;
	Smc_Junc_3_Dis_Line_Param_Front_S             junc_3_dis_line_param_front;
	Smc_Result_3_Param_Front_S                    result_3_param_front;
}
Smc_Junc_View_3_Front_Param_S, *Smc_P_Junc_View_3_Front_Param_S;

typedef struct Smc_Junc_View_3_Config_Common_Rear_Struct
{
	Float32_t           front_view_length;                                               /*                   */
	Float32_t           left_view_length;                                                /*                   */
	Float32_t           right_view_length;                                               /*                   */
	Float32_t           car_head_length;                                                 /*                   */
	Float32_t           front_blind_length;                                              /*                   */
	Float32_t           fr_fusion_angle;                                                 /*                   */
	Float32_t           rl_fusion_angle;                                                 /*                   */
	Float32_t           curve_range;                                                    /*                   */
	Float32_t           parallel_range;                                                  /*                   */
}
Smc_Junc_View_3_Config_Common_Rear_S, *Smc_P_Junc_View_3_Config_Common_Rear_S;

typedef struct Smc_Junc_3_Dis_Line_Param_Rear_Struct
{
	Int32_t             is_static_line_exist;                                            /*                   */
	Int32_t             static_line_to_veh_distance_0;                                   /*                   */
	Int32_t             static_line_to_veh_distance_1;                                   /*                   */
	Int32_t             static_line_to_veh_distance_2;                                   /*                   */
	Int32_t             static_line_to_veh_distance_3;                                   /*                   */
	Int32_t             static_line_length;                                              /*                   */
	Int32_t             static_line_pixel_count;                                         /*                   */
	Int32_t             is_dynamic_line_exist;                                           /*                   */
	Int32_t             dynamic_line_count;                                              /*                   */
	Int32_t             dynamic_line_length;                                             /*                   */
}
Smc_Junc_3_Dis_Line_Param_Rear_S, *Smc_P_Junc_3_Dis_Line_Param_Rear_S;

typedef struct Smc_Result_3_Param_Rear_Struct
{
	Int32_t          result_image_width;                                              /*                   */
	Int32_t           result_image_height;                                             /*                   */
	Int32_t           car_logo_rect_x;                                                 /*                   */
	Int32_t            car_logo_rect_y;                                                 /*                   */
	Int32_t           car_logo_rect_width;                                             /*                   */
	Int32_t            car_logo_rect_height;                                            /*                   */
}
Smc_Result_3_Param_Rear_S, *Smc_P_Result_3_Param_Rear_S;

typedef struct Smc_Junc_View_3_Rear_Param_Struct
{
	Smc_Junc_View_3_Config_Common_Rear_S          junc_view_3_config_common_rear;
	Smc_Junc_3_Dis_Line_Param_Rear_S              junc_3_dis_line_param_rear;
	Smc_Result_3_Param_Rear_S                     result_3_param_rear;
}
Smc_Junc_View_3_Rear_Param_S, *Smc_P_Junc_View_3_Rear_Param_S;

typedef struct Smc_Avm_2D_Param_Struct
{
	Smc_Junc_View_4_Param_S                       junc_view_4_param;
	Smc_Awb_View_Param_S                          awb_view_param;
	Smc_Junc_View_3_Front_Param_S                 junc_view_3_front_param;
	Smc_Junc_View_3_Rear_Param_S                  junc_view_3_rear_param;
}
Smc_Avm_2D_Param_S, *Smc_P_Avm_2D_Param_S;

typedef struct Smc_Single_View_Crop_Param_Front_Struct
{
	Float32_t           roll_angle;                                                    /*                   */
	Int32_t             car_logo_rect_x;                                                 /*                   */
	Int32_t             car_logo_rect_y;                                                 /*                   */
	Int32_t             car_logo_rect_width;                                             /*                   */
	Int32_t             car_logo_rect_height;                                            /*                   */
}
Smc_Single_View_Crop_Param_Front_S, *Smc_P_Single_View_Crop_Param_Front_S;

typedef struct Smc_Single_View_Plane_Expand_Param_Front_Struct
{
	Int32_t             trans_x;                                                    /*                   */
	Int32_t             trans_y;                                                    /*                   */
	Float32_t           fov_h;                                                    /*                   */
	Float32_t           fov_v;                                                    /*                   */
}
Smc_Single_View_Plane_Expand_Param_Front_S, *Smc_P_Single_View_Plane_Expand_Param_Front_S;

typedef struct Smc_Single_View_Box_Expand_Param_Front_Struct
{
	Int32_t             center_width;                                                    /*                   */
	Int32_t             trans_y;                                                    /*                   */
	Float32_t           fov_h_center;                                                    /*                   */
	Float32_t           fov_h_side;                                                    /*                   */
	Float32_t           fiv_v;                                                    /*                   */
}
Smc_Single_View_Box_Expand_Param_Front_S, *Smc_P_Single_View_Box_Expand_Param_Front_S;

typedef struct Smc_Single_View_Dis_Line_Param_Front_Struct
{
	Int32_t             is_static_line_exist;                                            /*                   */
	Int32_t             static_line_to_veh_distance_0;                                   /*                   */
	Int32_t             static_line_to_veh_distance_1;                                   /*                   */
	Int32_t             static_line_to_veh_distance_2;                                   /*                   */
	Int32_t             static_line_to_veh_distance_3;                                   /*                   */
	Int32_t             static_line_length;                                              /*                   */
	Int32_t             static_line_pixel_count;                                         /*                   */
	Int32_t             is_dynamic_line_exist;                                           /*                   */
	Int32_t             dynamic_line_count;                                              /*                   */
	Int32_t             dynamic_line_length;                                             /*                   */
}
Smc_Single_View_Dis_Line_Param_Front_S, *Smc_P_Single_View_Dis_Line_Param_Front_S;

typedef struct Smc_Front_Single_View_Param_Struct
{
	Int32_t             result_image_height;                                             /*  millimeter       */
	Int32_t             result_image_width;                                              /*                   */
	Int32_t             expand_type;                                                    /*                   */
	Smc_Single_View_Crop_Param_Front_S            single_view_crop_param_front;
	Smc_Single_View_Plane_Expand_Param_Front_S    single_view_plane_expand_param_front;
	Smc_Single_View_Box_Expand_Param_Front_S      single_view_box_expand_param_front;
	Smc_Single_View_Dis_Line_Param_Front_S        single_view_dis_line_param_front;
}
Smc_Front_Single_View_Param_S, *Smc_P_Front_Single_View_Param_S;

typedef struct Smc_Single_View_Crop_Param_Rear_Struct
{
	Float32_t           roll_angle;                                                    /*                   */
	Int32_t           car_logo_rect_x;                                                 /*                   */
	Int32_t            car_logo_rect_y;                                                 /*                   */
	Int32_t            car_logo_rect_width;                                             /*                   */
	Int32_t            car_logo_rect_height;                                            /*                   */
}
Smc_Single_View_Crop_Param_Rear_S, *Smc_P_Single_View_Crop_Param_Rear_S;

typedef struct Smc_Single_View_Plane_Expand_Param_Rear_Struct
{
	Int32_t             trans_x;                                                    /*                   */
	Int32_t             trans_y;                                                    /*                   */
	Float32_t           fov_h;                                                    /*                   */
	Float32_t           fov_v;                                                    /*                   */
}
Smc_Single_View_Plane_Expand_Param_Rear_S, *Smc_P_Single_View_Plane_Expand_Param_Rear_S;

typedef struct Smc_Single_View_Box_Expand_Param_Rear_Struct
{
	Int32_t             center_width;                                                    /*                   */
	Int32_t             trans_y;                                                    /*                   */
	Float32_t           fov_h_center;                                                    /*                   */
	Float32_t           fov_h_side;                                                    /*                   */
	Float32_t           fiv_v;                                                    /*                   */
}
Smc_Single_View_Box_Expand_Param_Rear_S, *Smc_P_Single_View_Box_Expand_Param_Rear_S;

typedef struct Smc_Single_View_Dis_Line_Param_Rear_Struct
{
	Int32_t             is_static_line_exist;                                            /*                   */
	Int32_t             static_line_to_veh_distance_0;                                   /*                   */
	Int32_t             static_line_to_veh_distance_1;                                   /*                   */
	Int32_t             static_line_to_veh_distance_2;                                   /*                   */
	Int32_t             static_line_to_veh_distance_3;                                   /*                   */
	Int32_t             static_line_length;                                              /*                   */
	Int32_t             static_line_pixel_count;                                         /*                   */
	Int32_t             is_dynamic_line_exist;                                           /*                   */
	Int32_t             dynamic_line_count;                                              /*                   */
	Int32_t             dynamic_line_length;                                             /*                   */
}
Smc_Single_View_Dis_Line_Param_Rear_S, *Smc_P_Single_View_Dis_Line_Param_Rear_S;

typedef struct Smc_Rear_Single_View_Param_Struct
{
	Int32_t             result_image_height;                                             /*                   */
	Int32_t             result_image_width;                                              /*                   */
	Int32_t             expand_type;                                                    /*                   */
	Smc_Single_View_Crop_Param_Rear_S             single_view_crop_param_rear;
	Smc_Single_View_Plane_Expand_Param_Rear_S     single_view_plane_expand_param_rear;
	Smc_Single_View_Box_Expand_Param_Rear_S       single_view_box_expand_param_rear;
	Smc_Single_View_Dis_Line_Param_Rear_S         single_view_dis_line_param_rear;
}
Smc_Rear_Single_View_Param_S, *Smc_P_Rear_Single_View_Param_S;

typedef struct Smc_Single_View_Crop_Param_Left_Struct
{
	Float32_t           roll_angle;                                                    /*                   */
	Float32_t           car_logo_rect_x;                                                 /*                   */
	Float32_t           car_logo_rect_y;                                                 /*                   */
	Float32_t           car_logo_rect_width;                                             /*                   */
	Float32_t           car_logo_rect_height;                                            /*                   */
}
Smc_Single_View_Crop_Param_Left_S, *Smc_P_Single_View_Crop_Param_Left_S;

typedef struct Smc_Single_View_Plane_Expand_Param_Left_Struct
{
	Float32_t              trans_y;                                                    /*                   */
	Float32_t              world_width;                                                    /*                   */
	Float32_t           world_height;                                                    /*                   */
	Int32_t           car_body_width;                                                  /*                   */
}
Smc_Single_View_Plane_Expand_Param_Left_S, *Smc_P_Single_View_Plane_Expand_Param_Left_S;

typedef struct Smc_Single_View_Combine_Expand_Param_Left_Struct
{
	Int32_t             trans_y;                                                    /*                   */
	Int32_t             car_body_width;                                                  /*                   */
	Float32_t           world_height;                                                    /*                   */
	Float32_t           plane_width;                                                    /*                   */
	Float32_t           plane_ratio;                                                    /*                   */
	Int32_t             valid_angle_range;                                               /*                   */
}
Smc_Single_View_Combine_Expand_Param_Left_S, *Smc_P_Single_View_Combine_Expand_Param_Left_S;

typedef struct Smc_Single_View_Dis_Line_Param_Left_Struct
{
	Int32_t             is_static_line_exist;                                            /*                   */
	Int32_t             static_line_to_veh_distance_0;                                   /*                   */
	Int32_t             static_line_to_veh_distance_1;                                   /*                   */
	Int32_t             static_line_to_veh_distance_2;                                   /*                   */
	Int32_t             static_line_to_veh_distance_3;                                   /*                   */
	Int32_t             static_line_length;                                              /*                   */
	Int32_t             static_line_pixel_count;                                         /*                   */
	Int32_t             is_dynamic_line_exist;                                           /*                   */
	Int32_t             dynamic_line_count;                                              /*                   */
	Int32_t             dynamic_line_length;                                             /*                   */
}
Smc_Single_View_Dis_Line_Param_Left_S, *Smc_P_Single_View_Dis_Line_Param_Left_S;

typedef struct Smc_Left_Single_View_Param_Struct
{
	Int32_t             result_image_height;                                             /*                   */
	Int32_t             result_image_width;                                              /*                   */
	Int32_t             expand_type;                                                    /*                   */
	Smc_Single_View_Crop_Param_Left_S             single_view_crop_param_left;
	Smc_Single_View_Plane_Expand_Param_Left_S     single_view_plane_expand_param_left;
	Smc_Single_View_Combine_Expand_Param_Left_S   single_view_combine_expand_param_left;
	Smc_Single_View_Dis_Line_Param_Left_S         single_view_dis_line_param_left;
}
Smc_Left_Single_View_Param_S, *Smc_P_Left_Single_View_Param_S;

typedef struct Smc_Single_View_Crop_Param_Right_Struct
{
	Float32_t           roll_angle;                                                    /*                   */
	Int32_t             car_logo_rect_x;                                                 /*                   */
	Int32_t             car_logo_rect_y;                                                 /*                   */
	Int32_t             car_logo_rect_width;                                             /*                   */
	Int32_t             car_logo_rect_height;                                            /*                   */
}
Smc_Single_View_Crop_Param_Right_S, *Smc_P_Single_View_Crop_Param_Right_S;

typedef struct Smc_Single_View_Plane_Expand_Param_Right_Struct
{
	Float32_t              trans_y;                                                    /*                   */
	Float32_t             world_width;                                                    /*                   */
	Float32_t           world_height;                                                    /*                   */
	Int32_t           car_body_width;                                                  /*                   */
}
Smc_Single_View_Plane_Expand_Param_Right_S, *Smc_P_Single_View_Plane_Expand_Param_Right_S;

typedef struct Smc_Single_View_Combine_Expand_Param_Right_Struct
{
	Float32_t              trans_y;                                                    /*                   */
	Int32_t             car_body_width;                                                  /*                   */
	Float32_t           world_height;                                                    /*                   */
	Float32_t           plane_width;                                                    /*                   */
	Float32_t           plane_ratio;                                                    /*                   */
	Float32_t            valid_angle_range;                                               /*                   */
}
Smc_Single_View_Combine_Expand_Param_Right_S, *Smc_P_Single_View_Combine_Expand_Param_Right_S;

typedef struct Smc_Single_View_Dis_Line_Param_Right_Struct
{
	Int32_t             is_static_line_exist;                                            /*                   */
	Int32_t             static_line_to_veh_distance_0;                                   /*                   */
	Int32_t             static_line_to_veh_distance_1;                                   /*                   */
	Int32_t             static_line_to_veh_distance_2;                                   /*                   */
	Int32_t             static_line_to_veh_distance_3;                                   /*                   */
	Int32_t             static_line_length;                                              /*                   */
	Int32_t             static_line_pixel_count;                                         /*                   */
	Int32_t             is_dynamic_line_exist;                                           /*                   */
	Int32_t             dynamic_line_count;                                              /*                   */
	Int32_t             dynamic_line_length;                                             /*                   */
}
Smc_Single_View_Dis_Line_Param_Right_S, *Smc_P_Single_View_Dis_Line_Param_Right_S;

typedef struct Smc_Right_Single_View_Param_Struct
{
	Int32_t             result_image_height;                                             /*                   */
	Int32_t             result_image_width;                                              /*                   */
	Int32_t             expand_type;                                                    /*                   */
	Smc_Single_View_Crop_Param_Right_S            single_view_crop_param_right;
	Smc_Single_View_Plane_Expand_Param_Right_S    single_view_plane_expand_param_right;
	Smc_Single_View_Combine_Expand_Param_Right_S  single_view_combine_expand_param_right;
	Smc_Single_View_Dis_Line_Param_Right_S        single_view_dis_line_param_right;
}
Smc_Right_Single_View_Param_S, *Smc_P_Right_Single_View_Param_S;

typedef struct Smc_Crop_Param_Left_Struct
{
	Float32_t           roll_angle;                                                    /*                   */
	Int32_t           car_logo_rect_x;                                                 /*                   */
	Int32_t            car_logo_rect_y;                                                 /*                   */
	Int32_t           car_logo_rect_width;                                             /*                   */
	Int32_t           car_logo_rect_height;                                            /*                   */
}
Smc_Crop_Param_Left_S, *Smc_P_Crop_Param_Left_S;

typedef struct Smc_Plane_Expand_Param_Left_Struct
{
	Float32_t           trans_y;                                                    /*                   */
	Float32_t           world_width;                                                    /*                   */
	Float32_t           world_height;                                                    /*                   */
	Int32_t           car_body_width;                                                  /*                   */
}
Smc_Plane_Expand_Param_Left_S, *Smc_P_Plane_Expand_Param_Left_S;

typedef struct Smc_Combine_Expand_Param_Left_Struct
{
	Float32_t           trans_y;                                                    /*                   */
	Int32_t             car_body_width;                                                  /*                   */
	Float32_t           world_height;                                                    /*                   */
	Float32_t           plane_width;                                                    /*                   */
	Float32_t           plane_ratio;                                                    /*                   */
	Float32_t           valid_angle_range;                                               /*                   */
}
Smc_Combine_Expand_Param_Left_S, *Smc_P_Combine_Expand_Param_Left_S;

typedef struct Smc_dis_line_param_left_Struct
{
	Int32_t             is_static_line_exist;                                            /*                   */
	Int32_t             static_line_to_veh_distance_0;                                   /*                   */
	Int32_t             static_line_to_veh_distance_1;                                   /*                   */
	Int32_t             static_line_to_veh_distance_2;                                   /*                   */
	Int32_t             static_line_to_veh_distance_3;                                   /*                   */
	Int32_t             static_line_length;                                              /*                   */
	Int32_t             static_line_pixel_count;                                         /*                   */
	Int32_t             is_dynamic_line_exist;                                           /*                   */
	Int32_t             dynamic_line_count;                                              /*                   */
	Int32_t             dynamic_line_length;                                             /*                   */
}
Smc_dis_line_param_left_S, *Smc_P_dis_line_param_left_S;

typedef struct Smc_Crop_Param_Right_Struct
{
	Float32_t           roll_angle;                                                    /*                   */
	Int32_t         car_logo_rect_x;                                                 /*                   */
	Int32_t           car_logo_rect_y;                                                 /*                   */
	Int32_t            car_logo_rect_width;                                             /*                   */
	Int32_t            car_logo_rect_height;                                            /*                   */
}
Smc_Crop_Param_Right_S, *Smc_P_Crop_Param_Right_S;

typedef struct Smc_Plane_Expand_Param_Right_Struct
{
	Float32_t             trans_y;                                                    /*                   */
	Float32_t             world_width;                                                    /*                   */
	Float32_t           world_height;                                                    /*                   */
	Int32_t           car_body_width;                                                  /*                   */
}
Smc_Plane_Expand_Param_Right_S, *Smc_P_Plane_Expand_Param_Right_S;

typedef struct Smc_Combine_Expand_Param_Right_Struct
{
	Float32_t           trans_y;                                                    /*                   */
	Int32_t             car_body_width;                                                  /*                   */
	Float32_t           world_height;                                                    /*                   */
	Float32_t           plane_width;                                                    /*                   */
	Float32_t           plane_ratio;                                                    /*                   */
	Float32_t           valid_angle_range;                                               /*                   */
}
Smc_Combine_Expand_Param_Right_S, *Smc_P_Combine_Expand_Param_Right_S;

typedef struct Smc_dis_line_param_Right_Struct
{
	Int32_t             is_static_line_exist;                                            /*                   */
	Int32_t             static_line_to_veh_distance_0;                                   /*                   */
	Int32_t             static_line_to_veh_distance_1;                                   /*                   */
	Int32_t             static_line_to_veh_distance_2;                                   /*                   */
	Int32_t             static_line_to_veh_distance_3;                                   /*                   */
	Int32_t             static_line_length;                                              /*                   */
	Int32_t             static_line_pixel_count;                                         /*                   */
	Int32_t             is_dynamic_line_exist;                                           /*                   */
	Int32_t             dynamic_line_count;                                              /*                   */
	Int32_t             dynamic_line_length;                                             /*                   */
}
Smc_dis_line_param_Right_S, *Smc_P_dis_line_param_Right_S;

typedef struct Smc_Left_Right_View_Param_Struct
{
	Int32_t             result_image_height;                                             /*                   */
	Int32_t             result_image_width;                                              /*                   */
	Int32_t             expand_type;                                                    /*                   */
	Smc_Crop_Param_Left_S                         crop_param_left;
	Smc_Plane_Expand_Param_Left_S                 plane_expand_param_left;
	Smc_Combine_Expand_Param_Left_S               combine_expand_param_left;
	Smc_dis_line_param_left_S                     dis_line_param_left;
	Smc_Crop_Param_Right_S                        crop_param_right;
	Smc_Plane_Expand_Param_Right_S                plane_expand_param_right;
	Smc_Combine_Expand_Param_Right_S              combine_expand_param_right;
	Smc_dis_line_param_Right_S                    dis_line_param_right;
}
Smc_Left_Right_View_Param_S, *Smc_P_Left_Right_View_Param_S;

typedef struct Smc_Avm_Single_View_Param_Struct
{
	Smc_Front_Single_View_Param_S                 front_single_view_param;
	Smc_Rear_Single_View_Param_S                  rear_single_view_param;
	Smc_Left_Single_View_Param_S                  left_single_view_param;
	Smc_Right_Single_View_Param_S                 right_single_view_param;
	Smc_Left_Right_View_Param_S                   left_right_view_param;
}
Smc_Avm_Single_View_Param_S, *Smc_P_Avm_Single_View_Param_S;

typedef struct Smc_Cal_Tag
{
	UInt8_t    vehicle_type;
	UInt8_t    smc_major_version;
	UInt8_t    smc_minor_version;
	UInt8_t    smc_informal_release_version;
	Int32_t    calib_version;
	Smc_Veh_Param_S                   veh_param;
	Smc_Camera_Model_S                camera_model[CAMERA_VIEW_CNT];
	Smc_Bev_Calib_Param_S             bev_calib_param;
	Smc_Avm_2D_Param_S                avm_2d_param;
	Smc_Avm_Single_View_Param_S       avm_single_view_param;
	Float32_t    lut_table[LENS_LUT_NUM][1001];
}
Smc_Cal_S;

/*===========================================================================*\
* File Revision History (top to bottom: first revision to last revision)
*===========================================================================*
*
*   Date        userid        Description
* ----------- ----------      -----------
*2018/5/28      weixin        Comment unused functions.
\*===========================================================================*/

#endif
