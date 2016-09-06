package org.usfirst.frc.team1529.robot;

public class Statics {
	
	//Arm PID
	final public static double arm_kp = 0.010;
	final public static double arm_ki = 0.000;
	final public static double arm_kd = 0.000;
	
	//PWM Mapping
	final public static int tape_drive = 0;
	final public static int winch_drive = 1;
	final public static int arm_motor = 3;
	final public static int ball_intake = 4;
	final public static int front_right = 6;
	final public static int front_left = 7;
	final public static int rear_right = 8;
	final public static int rear_left = 9;
	
	//DIO Mapping
	final public static int encoder_ch_a = 0;
	final public static int encoder_ch_b = 1;
	final public static int left_drive_encoder_ch_a = 3;
	final public static int left_drive_encoder_ch_b = 4;
	final public static int right_drive_encoder_ch_a = 5;
	final public static int right_drive_encoder_ch_b = 6;
	final public static int touch_sensor = 2;
	final public static int speed_switch = 8;
	
	//USB Mapping
	final public static int drive_stick_left = 0;
	final public static int drive_stick_right = 1;
	final public static int op_launchpad = 2;
	
	//Launchpad Mapping
	final public static int intake_button = 1;
	final public static int pickup_position = 2;
	final public static int carry_position = 3;
	final public static int arm_stick_1 = 4;
	final public static int nemo_stick_up = 5;
	final public static int shoot_button = 6;
	final public static int nemo_stick_down = 7;
	final public static int arm_stick_2 = 10;
	
	//encoder scaling
	final public static int l_drv_enc_scale = 1024;//|-set to no. of encoder steps equal -|
	final public static int r_drv_enc_scale = 1024;//|-  to moving robot 1 ft forward    -|
}
