#include "WPILib.h"
class Robot: public IterativeRobot
{
    public:
    Robot() :
		robotDrive(6, 7, 9, 8), // these must be initialized in the same order
		leftStick(0),
		rightStick(1),
		launchPad(2),
		lw(NULL),
		chooser(),
		autoTimer(),
		ballTimer(),
		armPushTimer(),
		defenseTimer(),
		arm(3),
		ballIntake(4)
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
    Joystick leftStick, rightStick, launchPad;
    LiveWindow *lw = LiveWindow::GetInstance();
    SendableChooser *chooser;
    const std::string autoNameDefault = "Default";
    const std::string lowBarAuto = "Low Bar";
    const std::string lowBarScoringAuto = "Low Bar Scoring";
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
    int setPoint = 0;
    int encoderZero = 0;
    bool armStopped = false;
    bool stopArmButton = false;
    bool stopArmButtonPrevious = false;
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
        chooser->AddObject(lowBarScoringAuto, (void*)&lowBarScoringAuto);
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
        if(autoSelected == lowBarAuto){
            float currentTime = autoTimer.Get();
            if(currentTime < 2.0){
                arm.Set(-0.1);
            }else if(currentTime < 5.5){//was 3.5#############
				arm.Set(0);
				robotDrive.TankDrive(-0.6, -0.6);
			}else{
				robotDrive.StopMotor();
            }
        }else if(autoSelected == lowBarScoringAuto){
			float currentTime = autoTimer.Get();
			if(currentTime < 2.0){ // arm down
				arm.Set(-0.3);
			}else if(currentTime < 5.77){//driving forward
				arm.Set(0);
				robotDrive.TankDrive(0.6, 0.6);
			}else if(currentTime < 6.149){ // 0.232 s of turning
				robotDrive.TankDrive(1, 0);
			}else if(currentTime < 8.552){
				robotDrive.TankDrive(0.75, 0.75);
			}else if(currentTime < 9.84){
				ballIntake.Set(1.0);
			}else{
				robotDrive.StopMotor();
				ballIntake.Set(0);
			}
		}else if(autoSelected == porkCutletAuto){
            //Pork Cutlet here
            float currentTime = autoTimer.Get();
            if(currentTime < 1.0){
                arm.Set(-0.1);
            }else{
                CrossPorkCutlet();
            }
        }else if(autoSelected == cheesyFriesAuto){
        	float currentTime = autoTimer.Get();
        	if(currentTime < 2.0){
        		arm.Set(0.2);
        	}else if(currentTime < 2.53){
				robotDrive.TankDrive(0.5, 0.5);
			}else if(currentTime < 4.53){
				arm.Set(-0.2);
				robotDrive.StopMotor();
			}else if(currentTime < 4.63){
				robotDrive.TankDrive(-0.5, -0.5);
			}else if(currentTime < 5.88){
				robotDrive.TankDrive(1.0, 1.0);
				arm.Set(0);
			}else{
				robotDrive.StopMotor();
				defenseTimer.Reset();
			}
        } else {
            float currentTime = autoTimer.Get();
            if(currentTime < 2){
                arm.Set(-0.1);
            }else if(currentTime < 3.5){
                arm.Set(0);
                robotDrive.TankDrive(-1.0, -1.0);
            }else if(currentTime < 4.5){
                robotDrive.TankDrive(-0.5, -0.5);
            }else{
            robotDrive.StopMotor();
            }
        }
        //end all the autos
        if(autoTimer.Get() > 15){
        	autoTimer.Reset();
        }
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
		float yAxis1Raw = 1 * leftStick.GetY();
		float yAxis2Raw = 1 * rightStick.GetY();
		bool armStick1 = launchPad.GetRawButton(4);
		bool armStick2 = launchPad.GetRawButton(10);
		//Drive the drive motors when any input is within -driveThreshold of 0.0
		//NOTE - currently this doesn't scale up the input from 0.0 after the deadband region -- it just uses the raw value.
		float yAxis1 = DeadZone(yAxis1Raw, driveThreshold, 0.0f);
		float yAxis2 = DeadZone(yAxis2Raw, driveThreshold, 0.0f);
		robotDrive.TankDrive(-yAxis1,-yAxis2); // drive
		BallIntake();
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
		//drives the arm motor to a setpoint
	void MoveArmToSetPoint(int setPoint){
		int clampValue = 12;
		int armEncoderRaw = armEncoder->GetRaw();
		//if encoder is too high, we need to drive arm up
		//if encoder is too low, we need to drive down
		int difference = armEncoderRaw - setPoint; // The difference; it will be positive if
		//arm needs to go up, negative if arm needs to go down e.g. enc 99- set 98 = 1 delta or -98 - -97 = -1 delta
		difference = std::min(difference, clampValue);
		difference = std::max(difference, -clampValue); //clamping values to -12(needs tweaking)
		float speed = (difference / (clampValue * 1.66)); //will return value between -0.6 to 0.6 proportional to
		//int speed = 0.262 * std::cbrt(difference);
		//the difference
		//This equation can be replaced by the equation (0.262 * cuberoot(x))
		std::cout << "Arm Speed " << speed << std::endl;
		arm.Set(speed);
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
		}else if(currentTime < 0.53){
			robotDrive.Drive(0.5, 0.5);
		}else if(currentTime < 2.53){
			arm.Set(-0.2);
			robotDrive.StopMotor();
		}else if(currentTime < 2.63){
			robotDrive.Drive(-0.5, -0.5);
		}else if(currentTime < 3.88){
			robotDrive.Drive(1.0, 1.0);
			arm.Set(0);
		}else{
			robotDrive.StopMotor();
			defenseTimer.Reset();
		}
	}
	void CrossPorkCutlet(){
		float currentTime = defenseTimer.Get();
		if(currentTime == 0.0){
			defenseTimer.Start();
		}else if(currentTime < 2.0){
			robotDrive.Drive(0.3, 0.3);
			ballIntake.Set(-1);
		}else if(currentTime < 3.5){
			robotDrive.Drive(1.0, 1.0);
		}else{
			robotDrive.StopMotor();
			ballIntake.Set(0);
			defenseTimer.Reset();
		}
	}
};

START_ROBOT_CLASS(Robot);
