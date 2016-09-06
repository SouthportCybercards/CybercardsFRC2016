package org.usfirst.frc.team1529.robot.subsystems;

import org.usfirst.frc.team1529.robot.Statics;

import edu.wpi.first.wpilibj.Talon;
import edu.wpi.first.wpilibj.command.Subsystem;

public class Ball_Collector extends Subsystem {
    
	//Subsystem Motor(s) and Components
	public Talon motor = new Talon(Statics.arm_motor);

    //Class Constructor
    public Ball_Collector(){
    	//Motor Initial Settings
    	motor.setInverted(false);
    }

    //Required Subsystem Function
    public void initDefaultCommand() {
    	//Do Nothing
    }   
}

