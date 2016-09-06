package org.usfirst.frc.team1529.robot.commands;

import org.usfirst.frc.team1529.robot.Robot;

import edu.wpi.first.wpilibj.command.Command;

public class Shoot extends Command {
	
	//Class Constructor
    public Shoot() {
        requires(Robot.collector);
    }

    //Automatic function called when command is started
    protected void initialize() {
    	//Do Nothing
    }

    //Automatic function called periodically while command is active
    protected void execute() {
    	//Run collector motor in reverse
    	Robot.collector.motor.set(-1.0);
    }

    //Automatic function called when command is ready to finish
    protected boolean isFinished() {
        return false;
    }

    //Automatic function called when command is ended/cancelled
    protected void end() {
    	//Stop collector motor
    	Robot.collector.motor.set(0.0);
    }

    //Automatic function called when command is interrupted
    protected void interrupted() {
    	//Do Nothing
    }
}
