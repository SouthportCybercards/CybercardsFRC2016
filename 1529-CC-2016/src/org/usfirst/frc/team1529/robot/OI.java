package org.usfirst.frc.team1529.robot;

import org.usfirst.frc.team1529.robot.commands.Auto_Arm;
import org.usfirst.frc.team1529.robot.commands.Drive;
import org.usfirst.frc.team1529.robot.commands.Intake;
import org.usfirst.frc.team1529.robot.commands.Manual_Arm;
import org.usfirst.frc.team1529.robot.commands.Shoot;

import edu.wpi.first.wpilibj.Joystick;
import edu.wpi.first.wpilibj.buttons.JoystickButton;

public class OI {
	
	//Variables
    public boolean manual_arm_up;
    public boolean manual_arm_down;
    public boolean carry_position;
    public boolean pickup_position;
	public double driver_left_final, driver_right_final;
	
	//Creation of commands for OI to call/start/stop
	Intake intake_command = new Intake();
	Shoot shoot_command = new Shoot();
	Drive drive_command = new Drive();
	Auto_Arm auto_command = new Auto_Arm();
	Manual_Arm manual_command = new Manual_Arm();
	
	//Joystick creation
    Joystick driver_left = new Joystick(Statics.drive_stick_left);
    Joystick driver_right = new Joystick(Statics.drive_stick_right);
    Joystick operator = new Joystick(Statics.op_launchpad);

    //Joystick Button creation
    JoystickButton nemo_up = new JoystickButton(operator, Statics.nemo_stick_up);
    JoystickButton nemo_down = new JoystickButton(operator, Statics.nemo_stick_down);
    JoystickButton intake_in = new JoystickButton(operator, Statics.intake_button);
    JoystickButton intake_out = new JoystickButton(operator, Statics.shoot_button);
    
    //Function to call to get controls values and call commands
    public void getOI(){
    	
    	/*
    	 * INTAKE CONTROLS
    	 * 
    	 * Check if intake command is running.
    	 * If intake command isn't running, start shoot command on button press
    	 * 
    	 * Check if shoot command is running.
    	 * If shoot command isn't running, start intake command on button press
    	 * 
    	 */
    	
    	if (!intake_command.isRunning()){
        	intake_out.whenPressed(shoot_command);
    	}
    	
    	if (!shoot_command.isRunning()){
    		intake_in.whenPressed(intake_command);
    	}
    	
		/*	
		 * DRIVETRAIN CONTROLS
		 * 
		 * Check if left drive stick is > deadband OR < -deadband
		 * IF		left drive stick is, output drive stick value
		 * ELSE		output zero
		 * 
		 * Check if right drive stick is > deadband OR < -deadband
		 * IF		right drive stick is, output drive stick value
		 * ELSE		output zero
		 * 
		 * Output values to drive command
		 * 
		 */
    	
    	if((driver_left.getRawAxis(Statics.drive_stick_left) > Robot.dash.deadband) ||
    			(driver_left.getRawAxis(Statics.drive_stick_left) < (-1.0 * Robot.dash.deadband))){
    		driver_left_final = driver_left.getRawAxis(Statics.drive_stick_left);
    	} else {
    		driver_left_final = 0.0;
    	}
    	

    	if((driver_right.getRawAxis(Statics.drive_stick_right) > Robot.dash.deadband) ||
    			(driver_right.getRawAxis(Statics.drive_stick_right) < (-1.0 * Robot.dash.deadband))){
    		driver_right_final = driver_left.getRawAxis(Statics.drive_stick_right);
    	} else {
    		driver_right_final = 0.0;
    	}

    	drive_command.getSticks(driver_left_final, driver_right_final);
    	
    	
    	/*
    	 * ARM CONTROLS
    	 * 
    	 * Get button values from Launch Pad
    	 * 
    	 * Check if Manual Arm Up or Down button pressed
    	 * IF		manual buttons in use, cancel auto arm control AND start manual arm control
    	 * ELSE		cancel manual arm control AND start auto arm control
    	 * 
    	 */
    	
    	manual_arm_up = operator.getRawButton(Statics.arm_stick_2);
    	manual_arm_down = operator.getRawButton(Statics.arm_stick_1);
    	carry_position = operator.getRawButton(Statics.carry_position);
    	pickup_position = operator.getRawButton(Statics.pickup_position);
    	
    	if (manual_arm_up || manual_arm_down){
    		auto_command.cancel();
    		manual_command.start();
    	} else {
    		manual_command.cancel();
    		auto_command.start();
    	}
    }
    
    //Function call to start drive controls
    public void startDrivetrain(){
    	drive_command.start();
    }
    
    //Function call to stop drive controls
    public void stopDrivetrain(){
    	drive_command.cancel();
    }
}