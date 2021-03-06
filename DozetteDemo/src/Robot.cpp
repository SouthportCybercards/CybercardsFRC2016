#include "WPILib.h"
class Robot: public IterativeRobot
{
    public:
    Robot() :
		robotDrive(6, 7, 9, 8), // these must be initialized in the same order
		tapeDrive(0),//#######tapemeasure is on motor port 0
		winchDrive(1),//####thrusterbox is on motor port 1
		leftStick(0),//###usb 0
		rightStick(1),//####usb1
		launchPad(2),//####usb2
		lw(NULL),
		chooser(),
		autoTimer(),
		ballTimer(),
		armPushTimer(),
		defenseTimer(),
		arm(3),//#####motor port 3
		ballIntake(4),//####motor port 4
		touchSensor(2)//####DIO2
    {
    	//ROBOT CONSTRUCTOR
        robotDrive.SetExpiration(0.1);
        robotDrive.SetInvertedMotor(robotDrive.kFrontLeftMotor,false);
        robotDrive.SetInvertedMotor(robotDrive.kFrontRightMotor,false);
        robotDrive.SetInvertedMotor(robotDrive.kRearLeftMotor,false);
        robotDrive.SetInvertedMotor(robotDrive.kRearRightMotor,false);
        cam = new USBCamera("cam0", true);
        armEncoder = new Encoder(0, 1, false, Encoder::EncodingType::k1X);
    }
    private:
    RobotDrive robotDrive;
    Talon tapeDrive;
    Talon winchDrive;
    Joystick leftStick, rightStick, launchPad;
    LiveWindow *lw = LiveWindow::GetInstance();
    SendableChooser *chooser;
    const std::string autoNameDefault = "Default";
    const std::string lowBarAuto = "Low Bar";
    const std::string porkCutletAuto = "Pork Cutlet";
    const std::string cheesyFriesAuto = "Cheesy Fries Auto";
    std::string autoSelected;
    USBCamera *cam;
    Encoder* armEncoder;
    Timer autoTimer;
    Timer ballTimer;
    Timer armPushTimer;
    Timer defenseTimer;
    Talon arm, ballIntake;
    DigitalInput touchSensor;
    int setPoint = 0;
    int encoderZero = 0;
    bool armStopped = false;
    bool stopArmButton = false;
    bool stopArmButtonPrevious = false;
    float demoMultiplier = 0.75;//#######1 for regular speed, .75 for demo

    void RobotInit()
    {
        CameraServer::GetInstance()->SetQuality(100);
        cam->OpenCamera();
        cam->SetBrightness(10);
        cam->SetExposureAuto();
        cam->SetWhiteBalanceAuto();
        cam->UpdateSettings();
        cam->StartCapture();
        //cool camera functions; makes it look good
        std::shared_ptr<USBCamera> cameraptr(cam);
        CameraServer::GetInstance()->StartAutomaticCapture(cameraptr);
        chooser = new SendableChooser();
        chooser->AddDefault(autoNameDefault, (void*)&autoNameDefault);
        chooser->AddObject(lowBarAuto, (void*)&lowBarAuto);
        chooser->AddObject(porkCutletAuto, (void*)&porkCutletAuto);
        chooser->AddObject(cheesyFriesAuto, (void*)&cheesyFriesAuto);
        SmartDashboard::PutData("Auto Modes", chooser);
        armEncoder->SetMaxPeriod(.1);
        armEncoder->SetMinRate(10);
        armEncoder->SetDistancePerPulse(5);
        armEncoder->SetReverseDirection(false);
        armEncoder->SetSamplesToAverage(7);
    }
    /**
    * This autonomous (along with the chooser code above) shows how to select between different autonomous modes
    * using the dashboard. The sendable chooser code works with the Java SmartDashboard.
    *
    * You can add additional auto modes by adding additional comparisons to the if-else structure below with additional strings.
    * If using the SendableChooser make sure to add them to the chooser code above as well.
    */
    void AutonomousInit()
    {
        autoSelected = *((std::string*)chooser->GetSelected());
        std::cout << "Auto selected: " << autoSelected << std::endl;
        autoTimer.Start();
    }
    void AutonomousPeriodic()
    {

    }
	void TeleopInit()
	{
	}
	void TeleopPeriodic()
	{
		bool pushArmButton = launchPad.GetRawButton(3);
		//Local declarations
		float driveThreshold = 0.005;
		//Get the y-axis of the joystickk
		float yAxis1Raw = 1 * demoMultiplier * leftStick.GetY();
		float yAxis2Raw = 1 * demoMultiplier * rightStick.GetY();
		bool armStick1 = launchPad.GetRawButton(4);
		bool armStick2 = launchPad.GetRawButton(10);
		//Drive the drive motors when any input is within -driveThreshold of 0.0
		//NOTE - currently this doesn't scale up the input from 0.0 after the deadband region -- it just uses the raw value.
		float yAxis1 = DeadZone(yAxis1Raw, driveThreshold, 0.0f);
		float yAxis2 = DeadZone(yAxis2Raw, driveThreshold, 0.0f);
		robotDrive.TankDrive(-yAxis1,-yAxis2); // drive
		BallIntake();
		NEMOControl();
		//read arm input buttons
		//Drive arm to set point
		if(!pushArmButton && !armStopped){
			if(armStick1 || armStick2){
				//if joystick back(positive value)
				if(armStick2){
					arm.Set(0.5);//drive arm up prev 0.7
					}else if(armStick1){
					arm.Set(-0.2);//drive arm down prev 0
				}
				setPoint = armEncoder->GetRaw();
			}
			else{
				MoveArmToSetPoint(setPoint);
			}
		}
		stopArmButtonPrevious = stopArmButton;
		stopArmButton = launchPad.GetRawButton(2);
		//armstopped starts out false. turn it to true if the button goes from false to true(button press),
		if(stopArmButtonPrevious == false && stopArmButton == true){
			armStopped = !armStopped;
		}
		if(pushArmButton){
			arm.Set(-0.2); //-0.2 might be a little much
		}
		if(armStopped){
			arm.Set(0.0);
		}
	}
	/*
	 * N ew
	 * E scalation
	 * M odule
	 * O utcome
	 *
	 * basically scaling mechanism
	 */
	void NEMOControl(){
		bool nemoStickUp = launchPad.GetRawButton(5);
		bool nemoStickDown = launchPad.GetRawButton(7);
		if(nemoStickUp){
			tapeDrive.Set(1);
		}else if(!nemoStickUp && !nemoStickDown){
			tapeDrive.Set(0);
		}else if(nemoStickDown){
			winchDrive.Set(1);
			tapeDrive.Set(-1);
		}else if(!nemoStickDown){
			winchDrive.Set(0);
		}else{//#########just for safety#########
			winchDrive.Set(0);
			tapeDrive.Set(0);

		}

	}
		//drives the arm motor to a setpoint
	void MoveArmToSetPoint(int setPoint){
		//if arm is down too much
		if(armEncoder->GetRaw() > (setPoint + 2)){
			arm.Set(0.25); //drive arm up prev 0.6
			//if arm is too far up
			}else if(armEncoder->GetRaw() < (setPoint - 2)){
			arm.Set(-0.6); //drive arm down prev -0.2
			}else{
			arm.Set(0.0); //default to driving up so the arm shakes less prev 0.3
		}
	}
    void TestPeriodic()
    {
        lw->Run();
    }
    float DeadZone(float value, float margin, float target){
        //deadzone code, will return og value if > or < drivethreshold
        if(value != target){
            if(value > target + margin || value < target - margin){
                return value;
            }
        }
        return target;
    }
	void BallIntake(){
		bool intakeButton = launchPad.GetRawButton(1);
		bool shootButton = launchPad.GetRawButton(6);
		if(intakeButton == true){
			ballIntake.Set(1);
		}else if(shootButton == true){
			ballIntake.Set(-1);
		}else{
			ballIntake.Set(0);
		}
	}
	void CrossCheesyFries(){
		std::cout << "cheesy fries being called" << std::endl;
		float currentTime = defenseTimer.Get();
		if(currentTime == 0.0){
			defenseTimer.Start();
		}else if(currentTime < 1.05){
			robotDrive.Drive(0.5, 0.5);
		}else if(currentTime < 1.75){
			arm.Set(-0.2);
			robotDrive.StopMotor();
		}else if(currentTime < 3.0){
			robotDrive.Drive(1.0, 1.0);
		}else{
			robotDrive.StopMotor();
			defenseTimer.Reset();
			autoTimer.Reset();
		}
	}
	void CrossPorkCutlet(){
		float currentTime = defenseTimer.Get();
		if(currentTime == 0.0){
			defenseTimer.Start();
		}else if(currentTime < 2.25){
			robotDrive.Drive(0.3, 0.3);
			ballIntake.Set(-1);
		}else if(currentTime < 3.75){
			robotDrive.Drive(1.0, 1.0);
		}else{
			robotDrive.StopMotor();
			ballIntake.Set(0);
			defenseTimer.Reset();
		}
	}
};

START_ROBOT_CLASS(Robot);
