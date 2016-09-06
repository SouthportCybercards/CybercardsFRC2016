package org.usfirst.frc.team1529.robot.commands;

import org.usfirst.frc.team1529.robot.Robot;

import edu.wpi.first.wpilibj.command.Command;

public class Drive extends Command {

	//Command Variables
	public double leftDriveRaw, rightDriveRaw;
	public double leftDriveFinal, rightDriveFinal;

	//Class Constructor
    public Drive() {
        requires(Robot.drive);
    }

    //Function to get OI values
    public void getSticks(double leftSpeed, double rightSpeed){
    	leftDriveRaw = leftSpeed;
    	rightDriveRaw = rightSpeed;
    }
    
    //Automatic function called when command is started
    protected void initialize() {
		//Do Nothing
    }

    //Automatic function called periodically while command is active
    protected void execute() {
    	//Multiply OI values and speed modifier values from Dashboard
    	leftDriveFinal = leftDriveRaw * Robot.dash.drive_modifier;
    	rightDriveFinal = rightDriveRaw * Robot.dash.drive_modifier;

    	//Output final values to Drivetrain subsystem
    	Robot.drive.drive.tankDrive(leftDriveFinal, rightDriveFinal);
    }

    //Automatic function called when command is ready to finish
    protected boolean isFinished() {
        return false;
    }

    //Automatic function called when command is ended/cancelled
    protected void end() {
    	//Stop Drivetrain subsystem
    	Robot.drive.drive.tankDrive(0.0, 0.0);
    }

    //Automatic function called when command is interrupted
    protected void interrupted() {
		//Do Nothing
    }
}
