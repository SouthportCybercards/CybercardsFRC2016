package org.usfirst.frc.team1529.robot.commands;

import org.usfirst.frc.team1529.robot.Robot;

import edu.wpi.first.wpilibj.command.Command;

public class Manual_Arm extends Command {

	//Class Constructor
    public Manual_Arm() {
    	requires(Robot.arm);
    }

    //Automatic function called when command is started
    protected void initialize() {
    	//Do Nothing
    }

    //Automatic function called periodically while command is active
    protected void execute() {
    	
    	/*
    	 * Check if Manual Arm Down is selected AND Manual Arm Up is not selected
    	 * IF true:		move arm down
    	 * 
    	 * Check if Manual Arm Down is not selected AND Manual Arm Up is selected
    	 * IF true:		make arm up
    	 * 
    	 * ELSE:		do nothing (basically keeping arm setpoint
    	 * 
    	 */
    	
    	if (Robot.oi.manual_arm_down && !Robot.oi.manual_arm_up){
    		Robot.arm.arm.set(Robot.dash.arm_down_speed);
    	} else if (!Robot.oi.manual_arm_down && Robot.oi.manual_arm_up){
    		Robot.arm.arm.set(Robot.dash.arm_up_speed);
    	} else {
    		Robot.arm.arm.set(0.0);
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
