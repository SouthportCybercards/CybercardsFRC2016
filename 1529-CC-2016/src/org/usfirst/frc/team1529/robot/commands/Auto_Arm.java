package org.usfirst.frc.team1529.robot.commands;

import org.usfirst.frc.team1529.robot.Robot;

import edu.wpi.first.wpilibj.command.Command;

public class Auto_Arm extends Command {

	//Command Variables
	public double arm_setpoint;
	
	//Class Constructor
    public Auto_Arm() {
        requires(Robot.arm);
    }

    //Automatic function called when command is started
    protected void initialize() {
    	arm_setpoint = Robot.arm.encoder.getDistance();
    }

    //Automatic function called periodically while command is active
    protected void execute() {
    	
    	//Output arm setpoint to PID Controller
    	Robot.arm.pidWrite(arm_setpoint);
    	
    	/*
    	 * Check if Carry Position is selected AND Pickup Position is not selected
    	 * IF true:		make arm setpoint equal to carry setpoint
    	 * 
    	 * Check if Pickup Position is selected AND Carry Position is not selected
    	 * IF true:		make arm setpoint equal to pickup setpoint
    	 * 
    	 * ELSE:		do nothing (basically keeping arm setpoint
    	 * 
    	 */
    	
    	if (Robot.oi.carry_position && !Robot.oi.pickup_position){
    		arm_setpoint = Robot.dash.carry_setpoint;
    	} else if (!Robot.oi.carry_position && Robot.oi.pickup_position){
    		arm_setpoint = Robot.dash.pickup_setpoint;    		
    	} else {
    		//Do Nothing
    	}
    }
    
    //Automatic function called when command is ready to finish
    protected boolean isFinished() {
        return false;
    }

    //Automatic function called when command is ended/cancelled
    protected void end() {
		//Do Nothing
    }
    
    //Automatic function called when command is interrupted
    protected void interrupted() {
		//Do Nothing    
    }
}
