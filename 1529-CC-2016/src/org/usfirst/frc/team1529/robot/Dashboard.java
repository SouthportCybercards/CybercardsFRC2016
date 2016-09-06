package org.usfirst.frc.team1529.robot;

import edu.wpi.first.wpilibj.smartdashboard.SmartDashboard;

public class Dashboard {
	
	//Variables
	public double deadband;
	public double drive_modifier;
	public double carry_setpoint;
	public double pickup_setpoint;
	public double arm_down_speed;
	public double arm_up_speed;
	
	//Function to call to pull settings from dashboard
	public void getDashValues(){
		deadband = SmartDashboard.getNumber("Controller Deadband", 0.005);
		drive_modifier = SmartDashboard.getNumber("Drive Speed Percentage", 1.0);
		carry_setpoint = SmartDashboard.getNumber("Carry Position Encoder Value", 1.0);
		pickup_setpoint = SmartDashboard.getNumber("Pickup Position Encoder Value", 0.0);
		arm_down_speed = SmartDashboard.getNumber("Manual Arm Down Speed Value", -0.2);
		arm_up_speed = SmartDashboard.getNumber("Manual Arm Up Speed Value", 0.5);
	}
	
	//Function to call to push data values to dashboard
	public void putDashValues(){
    	SmartDashboard.putData("Arm PID", Robot.arm.arm_pid);
    	SmartDashboard.putNumber("Arm Position 1", Robot.arm.pidGet());
    	SmartDashboard.putBoolean("Arm PID at Position", Robot.arm.arm_pid.onTarget());
    	SmartDashboard.putNumber("Drive Left Output", Robot.oi.driver_left_final);
    	SmartDashboard.putNumber("Drive Right Output", Robot.oi.driver_right_final);
	}
}
