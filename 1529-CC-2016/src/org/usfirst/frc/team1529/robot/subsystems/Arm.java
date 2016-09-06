package org.usfirst.frc.team1529.robot.subsystems;

import org.usfirst.frc.team1529.robot.Statics;

import edu.wpi.first.wpilibj.CounterBase.EncodingType;
import edu.wpi.first.wpilibj.Encoder;
import edu.wpi.first.wpilibj.PIDController;
import edu.wpi.first.wpilibj.PIDOutput;
import edu.wpi.first.wpilibj.PIDSource;
import edu.wpi.first.wpilibj.PIDSourceType;
import edu.wpi.first.wpilibj.Talon;
import edu.wpi.first.wpilibj.command.Subsystem;

public class Arm extends Subsystem implements PIDSource, PIDOutput {

	//Subsystem Motor(s) and Component(s)
    public Talon arm = new Talon(Statics.arm_motor);
    public Encoder encoder = new Encoder(Statics.encoder_ch_a, Statics.encoder_ch_b, false, EncodingType.k4X);
    public PIDController arm_pid;

    //Class Constructor
    public Arm(){
    	//Motor Initial Settings
    	arm.setInverted(false);
    	
    	//Encoder Initial Settings
    	encoder.setDistancePerPulse(5.0);
    	encoder.setMaxPeriod(0.1);
    	encoder.setMinRate(10.0);
    	encoder.setReverseDirection(false);
    	encoder.setSamplesToAverage(7);
    	
    	//PID Controller Initial Settings
    	arm_pid = new PIDController(Statics.arm_kp,
    			Statics.arm_ki,
    			Statics.arm_kd,
    			this, this);
    	arm_pid.setAbsoluteTolerance(0.1);
    	arm_pid.setOutputRange(0.0, 1.0);
    }

    //Required Subsystem Function
    public void initDefaultCommand() {
		//Do Nothing
    }

	@Override
	public void pidWrite(double output) {
		arm.set(output);
	}

	@Override
	public void setPIDSourceType(PIDSourceType pidSource) {
		//Do Nothing
	}

	@Override
	public PIDSourceType getPIDSourceType() {
		return PIDSourceType.kDisplacement;
	}

	@Override
	public double pidGet() {
		return encoder.get();
	}
}

