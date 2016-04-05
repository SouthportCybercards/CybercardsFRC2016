#include "WPILib.h"
#include "ColorSensorMacros.h"
enum ColorSensorPosition{FRONT, BACK, BACK2}; //access front, back, or back2 color sensors with ColorSensors[FRONT], etc.
class Robot: public IterativeRobot
{
    public:
    Robot() :
		robotDrive(6, 7, 9, 8), // these must be initialized in the same order
		leftStick(0),
		rightStick(1),
		armStick(2),
		launchPad(3),
		lw(NULL),
		chooser(),
		GenericColorSensor(I2C::kOnboard, 0x29),
		Multiplexer(I2C::kOnboard, 0x70),
		//ColorSensors(),
		ColorSensorTimer(),
		autoTimer(),
		ballTimer(),
		armPushTimer(),
		tempTimer(),
		arm(3),
		ballIntake1(3),
		ballIntake2(4),
		touchSensor(2)
		//armLimitSwitch(0)
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
    Joystick leftStick, rightStick, armStick, launchPad;
    LiveWindow *lw = LiveWindow::GetInstance();
    SendableChooser *chooser;
    const std::string autoNameDefault = "Default";
    const std::string lowBarAuto = "Low Bar";
    const std::string porkCutletAuto = "Pork Cutlet";
    const std::string cheesyFriesAuto = "Cheesy Fries Auto";
    std::string autoSelected;
    USBCamera *cam;
    Encoder* armEncoder;
    I2C GenericColorSensor; //FRONT
    //I2C BackColorSensor1; //BACK
    //I2C BackColorSensor2; //BACK2
    I2C Multiplexer;
    //std::vector<I2C*> ColorSensors; //our vector of color sensor pointers; use for everything instead of ColorSensor1
    Timer ColorSensorTimer;
    Timer autoTimer;
    Timer ballTimer;
    Timer armPushTimer;
    Timer tempTimer;
    Talon arm, ballIntake1, ballIntake2;
    DigitalInput touchSensor;
    //DigitalInput armLimitSwitch;
    int setPoint = 0;
    bool overGreen = true;
    int carpetRGB[3] = {0};
    int encoderZero = 0;
    bool armStopped = false;
    ColorSensorPosition iSensorCounter = FRONT;
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
        chooser->AddObject(porkCutletAuto, (void*)&porkCutletAuto);
        chooser->AddObject(cheesyFriesAuto, (void*)&cheesyFriesAuto);
        SmartDashboard::PutData("Auto Modes", chooser);
        InitColorSensor();
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
        //std::string autoSelected = SmartDashboard::GetString("Auto Selector", autoNameDefault);
        std::cout << "Auto selected: " << autoSelected << std::endl;
        //Encoder *enc;
        //enc = new Encoder(/*DIO port 0, 1*/0, 1, false, Encoder::EncodingType::k4X);
        CalibrateSensors();
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
			/*}else if(currentTime < 6.5){
				robotDrive.TankDrive(1, -1);//turn right for 1.0
			}else if(currentTime < 8.5){  				(needs testing!)
				robotDrive.TankDrive(-0.7, -0.7); go back for 2.0
			}else if(currentTime < 11.0){
				ballIntake1.Set(-1.0); outtake for 2.5
				ballIntake2.Set(-1.0);*/
			}else{
				robotDrive.StopMotor();
				//ballIntake1.Set(0);
				//ballIntake2.Set(0);
            }
        }else if(autoSelected == porkCutletAuto){
            //Pork Cutlet here
            float currentTime = autoTimer.Get();
            if(currentTime < 1.0){
                arm.Set(-0.1); //arm slowly knocked down for 1 sec
            }else{
                CrossPorkCutlet();
            }
        }else if(autoSelected == cheesyFriesAuto){
            float currentTime = autoTimer.Get();
            if(currentTime < 1.0){
                arm.Set(0.2); //arm forced up for a full second
            }else{
                CrossCheesyFries();
            }
        } else {
            float currentTime = autoTimer.Get();
            if(currentTime < 2){
                arm.Set(-0.1); //2 seconds of knocking arm down
            }else if(currentTime < 3.5){
                arm.Set(0);
                robotDrive.TankDrive(-1.0, -1.0); //full speed for 1.5 secs. about half the field across
            }else if(currentTime < 4.5){
                robotDrive.TankDrive(-0.5, -0.5); //half speed for 1 sec because Danny wanted it?
            }else{
            	robotDrive.StopMotor();
            }
        }
        float autoTimerValue = autoTimer.Get();
        //end all the autos
        if(autoTimerValue > 15.0){
        	autoTimer.Reset(); //ends all timers on its own
        }
    }
	void TeleopInit()
	{
	}
	void TeleopPeriodic()
	{
		bool pushArmButton = launchPad.GetRawButton(3);
		//std::cout << "getRaw() = " << armEncoder->GetRaw() << "setPoint=" << setPoint << std::endl;
		//Local declarations
		//TestColorSensor();
		float driveThreshold = 0.005;
		//Get the y-axis of the joystick
		float yAxis1Raw = 1 * leftStick.GetY();//############3was 1*
		float yAxis2Raw = 1 * rightStick.GetY();//###########was 1*
		//float armStickYRaw = 1 * armStick.GetY();
		bool armStick1 = launchPad.GetRawButton(4);
		bool armStick2 = launchPad.GetRawButton(10);
		//Drive the drive motors when any input is within -driveThreshold of 0.0
		//NOTE - currently this doesn't scale up the input from 0.0 after the deadband region -- it just uses the raw value.
		float yAxis1 = DeadZone(yAxis1Raw, driveThreshold, 0.0f);
		float yAxis2 = DeadZone(yAxis2Raw, driveThreshold, 0.0f);
		//float armStickY = DeadZone(armStickYRaw, armThreshold, 0.0f);
		robotDrive.TankDrive(0.5 * -yAxis1, 0.5 * -yAxis2); // Scaling down by 0.5 for Ms. Plowie
		std::cout << "joystick = " << -yAxis1 << "joy2" << -yAxis2;
		BallIntake();
		//read arm input buttons
		//setPoint = ReadSetPointButtons(setPoint);
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
		if(pushArmButton){ //this is hardly needed anymore but will keep until we need another button
			arm.Set(-0.2); //-0.2 might be a little much
		}
		if(armStopped){
			arm.Set(0.0);
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
	//reads arm setpoint buttons and selects destination (currently not used)
	int ReadSetPointButtons(int currentPoint){
		//default set point to the current encoder value
		int point = currentPoint;
		//if set point button is pressed return its setpoint
		//this section is for if we added buttons for setpoints, now not necessary
		/*if(launchPad.GetRawButton(3)){
			point = encoderZero;
			}else if(launchPad.GetRawButton(4)){
			point = encoderZero + 100;
			}else if(launchPad.GetRawButton(5)){
			point = encoderZero + 200;
			}else if(launchPad.GetRawButton(6)){
			point = encoderZero + 250;
		}*/
		return point;
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
    //Initializes all color sensors. Will wait, which causes some auto weirdness(but it's become a thing now)
    void InitColorSensor(){
        for(int i = 0; i < 3; i++){
            Wait(0.7);
            tcaselect(i);
            //remember to or the command bit for both reading and writing everything
            GenericColorSensor.Write(TCS_COMMAND_BIT | TCS_ATIME, TCS34725_INTEGRATIONTIME_700MS);
            GenericColorSensor.Write(TCS_COMMAND_BIT | TCS_CONTROL, TCS34725_GAIN_1X); //This is the Gain
            GenericColorSensor.Write(TCS_COMMAND_BIT | TCS_ENABLE, TCS_ENABLE_PON | TCS_ENABLE_AEN | TCS_ENABLE_WEN);
        }
    }
    //This function was made to test a single color sensor. Later, there will be a single function for reading all color sensors
    //and interpreting data.
	void TestColorSensor(){
		float currentTime;
		currentTime = ColorSensorTimer.Get();
		if(currentTime == 0.0){
			ColorSensorTimer.Start();
			tcaselect(FRONT);
		}else if(ColorSensorTimer.HasPeriodPassed((float)0.7)){
			//change this to shorter time later
			if(currentTime > 10){
				ColorSensorTimer.Reset();
				tcaselect(FRONT);
			}else{
				uint8_t c = 0;
				uint8_t r = 0;
				uint8_t g = 0;
				uint8_t b = 0;
				GenericColorSensor.Read(TCS_COMMAND_BIT | TCS_CDATAL, 1, &c);
				GenericColorSensor.Read(TCS_COMMAND_BIT | TCS_RDATAL, 1, &r);
				GenericColorSensor.Read(TCS_COMMAND_BIT | TCS_GDATAL, 1, &g);
				GenericColorSensor.Read(TCS_COMMAND_BIT | TCS_BDATAL, 1, &b);
				//std::cout << currentTime << std::endl;
				std::cout << iSensorCounter << "; " << "r= " << (int)r << "g= "<< (int)g << "b= " << (int)b << "c=" << (int)c << std::endl;
				if(iSensorCounter == FRONT){
					iSensorCounter = BACK;
					tcaselect(1);
				}else if(iSensorCounter == BACK){
					iSensorCounter = BACK2;
					tcaselect(2);
				}else{
					iSensorCounter = FRONT;
					tcaselect(0);
				}
			}
		}
	}
	void BallIntake(){
		bool intakeButton = launchPad.GetRawButton(1);
		bool shootButton = launchPad.GetRawButton(6);
		//std::cout << "intake" << intakeButton << std::endl << "shoot" << shootButton << touchSensor.Get() << std::endl;
		if(intakeButton == true){// && !touchSensor.Get()){
			ballIntake1.Set(1);
			ballIntake2.Set(1);
			}else if(shootButton == true){
			ballIntake1.Set(-1);
			ballIntake2.Set(-1);
			}else{
			ballIntake1.Set(0);
			ballIntake2.Set(0);
		}
	}
	bool CheckSensorForGreen(ColorSensorPosition position){ //pass in sensor and position
		uint8_t r = 0;
		uint8_t g = 0;
		uint8_t b = 0;
		uint8_t c = 0;
		int threshold = 50;
		GenericColorSensor.Read(TCS_COMMAND_BIT | TCS_RDATAL, 1, &r);
		GenericColorSensor.Read(TCS_COMMAND_BIT | TCS_GDATAL, 1, &g);
		GenericColorSensor.Read(TCS_COMMAND_BIT | TCS_BDATAL, 1, &b);
		GenericColorSensor.Read(TCS_COMMAND_BIT | TCS_CDATAL, 1, &c);
		//float red, green, blue, clear;
		//red = (float)r; green = (float)g; blue = (float)b; clear = (float)c;
		//red /= clear; green /= clear; blue /= clear; //this averages values with c value
		//red *= 256; green *= 256; blue *= 256; //brings it back to normal rgb values; doesn't work
		//std::cout << "r = " << (int)red << "g = " << (int)green << "b = " << (int)blue << "c = " << (int)clear << std::endl;
		if(r < (carpetRGB[0] + threshold) && r > (carpetRGB[0] - threshold)){
			if(g < (carpetRGB[1] + threshold) && g > (carpetRGB[1] - threshold)){
				if(b < (carpetRGB[2] + threshold) && b > (carpetRGB[2] - threshold)){
					return true;
					std::cout << "we found green at " << position;
				}
			}
		}
		tcaselect(position);
		return false;
	}
	void CalibrateSensors(){
		uint8_t r = 0;
		uint8_t g = 0;
		uint8_t b = 0;
		uint8_t c = 0;
		carpetRGB[0] = 0;
		carpetRGB[1] = 0;
		carpetRGB[2] = 0;
		for (int i = 0; i < 3; i++){
			tcaselect(i);
			Wait(0.7); //this causes weirdness in auto
			GenericColorSensor.Read(TCS_COMMAND_BIT | TCS_RDATAL, 1, &r);
			GenericColorSensor.Read(TCS_COMMAND_BIT | TCS_GDATAL, 1, &g);
			GenericColorSensor.Read(TCS_COMMAND_BIT | TCS_BDATAL, 1, &b);
			GenericColorSensor.Read(TCS_COMMAND_BIT | TCS_CDATAL, 1, &c);
			float red = (float)r;
			float green = (float)g;
			float blue = (float)b;
			//red /= clear; green /= clear; blue /= clear; //this averages values with c value
			//red *= 256; green *= 256; blue *= 256; //brings it back to normal rgb value
			std::cout << "r " << (int)red << "g " << (int)green << "b " << (int)blue;
			carpetRGB[0] = carpetRGB[0] + (int)red;
			carpetRGB[1] = carpetRGB[1] + (int)green;
			carpetRGB[2] = carpetRGB[2] + (int)blue;
		}
		carpetRGB[0] /= 3;
		carpetRGB[1] /= 3;
		carpetRGB[2] /= 3;
		std::cout << "Done Calibrating, 0= " << carpetRGB[0] << ", "<< "1= " << carpetRGB[1] << ", " << "2= " << carpetRGB[2] << std::endl;
	}
	//sets which mux address to write to
	void tcaselect(uint8_t i) {
		if(i > 7) return;
		Multiplexer.Write(0x70, (uint8_t)1 << i);
	}
	void CrossCheesyFries(){
		std::cout << "cheesy fries being called" << std::endl;
		float currentTime = tempTimer.Get();
		if(currentTime == 0.0){
			tempTimer.Start();
		}else if(currentTime < 0.8){
			robotDrive.Drive(0.5, 0.5); //Half-speed forward for 0.8 seconds (~5 m/s for 1.8m + accel)
		}else if(currentTime < 1.5){
			arm.Set(-0.2); //drop arm for 0.7 secs. keep in mind it's still powering down
			robotDrive.StopMotor();
		}else if(currentTime < 2.75){
			robotDrive.Drive(1.0, 1.0); //full speed ahead for 1.25 secs
		}else{
			arm.Set(0); //turn everything off and reset tiemr
			robotDrive.StopMotor();
			tempTimer.Reset();
			autoTimer.Reset();
		}
	}
	void CrossPorkCutlet(){
		float currentTime = tempTimer.Get();
		if(currentTime == 0.0){
			tempTimer.Start();
		}else if(currentTime < 2.0){
			robotDrive.Drive(0.3, 0.3);
			ballIntake1.Set(-1); //drive slowly forward and outtake for 2.0
			ballIntake2.Set(-1);
		}else if(currentTime < 3.5){
			robotDrive.Drive(1.0, 1.0); //still outtaking and full speed forward for 1.5
		}else{
			robotDrive.StopMotor(); //stop everything
			ballIntake1.Set(0);
			ballIntake2.Set(0);
			tempTimer.Reset();
		}
	}
};

START_ROBOT_CLASS(Robot);
