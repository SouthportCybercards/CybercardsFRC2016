package org.usfirst.frc.team1529.robot.subsystems;

import org.usfirst.frc.team1529.robot.Statics;

import edu.wpi.first.wpilibj.RobotDrive;
import edu.wpi.first.wpilibj.RobotDrive.MotorType;
import edu.wpi.first.wpilibj.command.Subsystem;

public class Drivetrain extends Subsystem {

	//Subsystem Motor(s) and Component(s)
    public RobotDrive drive = new RobotDrive(Statics.front_left,
    		Statics.rear_left,
    		Statics.front_right,
    		Statics.rear_right);

    //Class Constructor
    public Drivetrain(){
    	//Robot Drive System Initial Settings
    	drive.setExpiration(0.1);
    	drive.setInvertedMotor(MotorType.kFrontLeft, false);
    	drive.setInvertedMotor(MotorType.kFrontRight, false);
    	drive.setInvertedMotor(MotorType.kRearLeft, false);
    	drive.setInvertedMotor(MotorType.kRearRight, false);
    }
    
    //Required Subsystem Function
    public void initDefaultCommand() {
    	//Do Nothing
    }
}

