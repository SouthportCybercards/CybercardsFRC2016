
package org.usfirst.frc.team1529.robot;

import org.usfirst.frc.team1529.robot.subsystems.Arm;
import org.usfirst.frc.team1529.robot.subsystems.Ball_Collector;
import org.usfirst.frc.team1529.robot.subsystems.Drivetrain;

import edu.wpi.first.wpilibj.IterativeRobot;
import edu.wpi.first.wpilibj.command.Scheduler;
import edu.wpi.first.wpilibj.livewindow.LiveWindow;


public class Robot extends IterativeRobot {

	//Class creations
	public static OI oi;
	public static Dashboard dash;
	public static Arm arm;
	public static Ball_Collector collector;
	public static Drivetrain drive;

	//Create new instances
    public void robotInit() {
		oi = new OI();
		arm = new Arm();
		collector = new Ball_Collector();
		drive = new Drivetrain();
    }
	
    public void disabledInit(){
    	oi.stopDrivetrain();
    }
	
	public void disabledPeriodic() {
		Scheduler.getInstance().run();
		dash.putDashValues();
	}

    public void autonomousInit() {
    	oi.startDrivetrain();
    	dash.getDashValues();
    }

    public void autonomousPeriodic() {
        Scheduler.getInstance().run();
        dash.putDashValues();
    }

    public void teleopInit() {
    	oi.startDrivetrain();
    	dash.getDashValues();
    }

    public void teleopPeriodic() {
        Scheduler.getInstance().run();
        oi.getOI();
        dash.putDashValues();
    }
    
    public void testPeriodic() {
        LiveWindow.run();
    }
}
