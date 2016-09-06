package org.usfirst.frc.team1529.robot.commands;

import org.usfirst.frc.team1529.robot.Robot;

import edu.wpi.first.wpilibj.command.Command;

import edu.wpi.first.wpilibj.internal.HardwareTimer;

/**
 *6ba
 *
 *cross lowbar with boulder
 *come back to centerline
 *point turn 90* clockwise
 *drive fullspeed field length
 *stop before wall or at 15 sec
 *
 *todo
 *implement hardware timer to time 15sec auto
 *implem ecnoders
 *impl commands see abv
 *research other javateams' auto code and ''''''modify it''''''to suit
 */
public class Autonomous extends Command {

    public Autonomous() {
    	requires(Robot.drive);
    	requires(Robot.arm);
    	requires(Robot.collector);
    }

    // Called just before this Command runs the first time
    protected void initialize() {
    	
    }

    // Called repeatedly when this Command is scheduled to run
    protected void execute() {
    	
    }

    // Make this return true when this Command no longer needs to run execute()
    protected boolean isFinished() {
        return false;
    }

    // Called once after isFinished returns true
    protected void end() {
    }

    // Called when another command which requires one or more of the same
    // subsystems is scheduled to run
    protected void interrupted() {
    }
}
