#include "WPILib.h"
#include "ColorSensorMacros.h"
enum ColorSensorPosition{FRONT, BACK, BACK2}; //access front, back, or back2 color sensors with ColorSensors[FRONT], etc.
class Robot: public IterativeRobot
{
public:
	Robot() :
		robotDrive(6, 7, 9, 8),	// these must be initialized in the same order
		leftStick(0),
		rightStick(1),
		armStick(2),
		launchPad(3),
		lw(NULL),
		chooser(),
		FrontColorSensor(I2C::kOnboard, 0x29),
		//BackColorSensor1(I2C::kOnboard, 0x29),
		//BackColorSensor2(I2C::kOnboard, 0x29),
		Multiplexer(I2C::kOnboard, 0x70),
		//ColorSensors(),
		ColorSensorTimer(),
		autoTimer(),
		ballTimer(),
		arm(3),
		ballIntake1(3),
		ballIntake2(4),
		touchSensor(2)
		//armLimitSwitch(0)
	{
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
	const std::string autoNameCustom = "My Auto";
	std::string autoSelected;
	USBCamera *cam;
	Encoder* armEncoder;
	I2C FrontColorSensor; //FRONT
	//I2C BackColorSensor1; //BACK
	//I2C BackColorSensor2; //BACK2
	I2C Multiplexer;
	//std::vector<I2C*> ColorSensors; //our vector of color sensor pointers; use for everything instead of ColorSensor1
	Timer ColorSensorTimer;
	Timer autoTimer;
	Timer ballTimer;
	Talon arm, ballIntake1, ballIntake2;
	DigitalInput touchSensor;
	//DigitalInput armLimitSwitch;
	int setPoint = 0;
	bool overGreen = true;
	int carpetRGB[3] = {0};
	int encoderZero = 0;
	ColorSensorPosition iSensorCounter = FRONT;

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
		chooser->AddObject(autoNameCustom, (void*)&autoNameCustom);
		SmartDashboard::PutData("Auto Modes", chooser);
		//ColorSensors.push_back(&FrontColorSensor);
		//ColorSensors.push_back(&BackColorSensor1);
		//ColorSensors.push_back(&BackColorSensor2);
		//for(unsigned int i = 0; i < ColorSensors.size(); i++){//loops through vector and initializes all color sensors.
		InitColorSensor();
		//}
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
		//std::cout << "Auto selected: " << autoSelected << std::endl;
		//Encoder *enc;
		//enc = new Encoder(/*DIO port 0, 1*/0, 1, false, Encoder::EncodingType::k4X);
		if(autoSelected == autoNameCustom){
			//Custom Auto goes here
		} else {
			CalibrateSensors();
			autoTimer.Start();
			/*
			 * Specify defences, where etc
			 * Calibrate sensors
			 */
			/*Autoinit: getEncoderValue (on the arm)
			 * calculate a 75 degree angle based on the arm starting at 90 degrees
			 * calibrate the color sensors
			 *
			 *
			 */
			//Default Auto goes here
		}
	}

	void AutonomousPeriodic()
	{
		if(autoSelected == autoNameCustom){
			//Custom Auto goes here
		} else {
			//LOGIC FOR STOPPING ROBOT
			double currentTime = autoTimer.Get();
			/*if(currentTime < 0.5){
				arm.Set(0.15);
			}else if(armLimitSwitch.Get() == 1){
				arm.Set(0);
				encoderZero = armEncoder->GetRaw();
				setPoint = encoderZero;
				std::cout << "encoder being zeroed; value = " << encoderZero << std::endl;
			}else if(currentTime > 6){
				arm.Set(0);
				//failsafe
			}*/
			bool frontGreen = CheckSensorForGreen(FRONT);  //ColorSensors[FRONT], FRONT);
			bool backGreen = CheckSensorForGreen(BACK);
			bool back2Green = CheckSensorForGreen(BACK2);
			bool overGreenPrevious = overGreen;
			if(frontGreen && backGreen && back2Green){ //checking all sensors for this
				overGreen = true;
				std::cout << "whole robot is over green" << std::endl;
			}else{
				overGreen = false;
			}
			if(currentTime < 2){
				arm.Set(-0.1);
			}else if(currentTime < 4.5){
				arm.Set(0);
				robotDrive.TankDrive(-1.0, -1.0);
			}else{
				robotDrive.StopMotor();
			}

			//if(overGreenPqrevious == false && overGreen == true && currentTime >= 2.0){
				//making sure we've driven at least 2.0 secs
			//	robotDrive.StopMotor();
			//}else
			//if(currentTime > 3.5){
			//	robotDrive.StopMotor();
			//}

			//Default Auto goes here
			/*
			 * Auton()
			 * 		ColorSense()
			 * 		ReadOtherSensors(
			 * 		DriveMotors()
			 * 		LiftGate() ...?
			 *
			 * 		read arm encoder
			 * 		drive arm at full speed to 75 degrees (not needed with abs encoder)
			 * 		drive arm slowly to zero switch (not needed with abs encoder)
			 * 		drive arm up to 20 degrees (not needed with abs encoder)
			 * 		check sensors
			 *
			 * 		intake:
			 * 		read controls
			 * 			make controls for intake cancel, shot cancel, shot trigger, intake trigger
			 * 			read limit switches--all wired to one io port
			 * 			drive (direction*deadzone*speed)
			 * 			do debouncing
			 * 			shot timer to turn off roller automatically
			 * 			dont intake if limit switches are triggered
			 */
		}
	}

	void TeleopInit()
	{


	}

	void TeleopPeriodic()
	{
		//TestColorSensor(ColorSensors[FRONT]);//testing first sensor, remove later
		//std::cout << "getRaw() = " << armEncoder->GetRaw() << "setPoint=" << setPoint << std::endl;
		//Local declarations
		TestColorSensor();
		float driveThreshold = 0.005;
		float armThreshold = 0.2;
		//Get the y-axis of the joystickk

		float yAxis1Raw = 1 * leftStick.GetY();
		float yAxis2Raw = 1 * rightStick.GetY();
		float armStickYRaw = 1 * armStick.GetY();


		//Drive the drive motors when any input is within  -driveThreshold of 0.0
		//NOTE - currently this doesn't scale up the input from 0.0 after the deadband region -- it just uses the raw value.
		float yAxis1 = DeadZone(yAxis1Raw, driveThreshold, 0.0f);
		float yAxis2 = DeadZone(yAxis2Raw, driveThreshold, 0.0f);
		float armStickY = DeadZone(armStickYRaw, armThreshold, 0.0f);

		robotDrive.TankDrive(-yAxis1,-yAxis2); 	// drive
		BallIntake();
		//read arm input buttons
		//setPoint = ReadSetPointButtons(setPoint);

		//Drive arm to set point
		if(armStickY != 0.0f){
			//if joystick back(positive value)
			if(armStickY > 0){
				arm.Set(0.7);//drive arm up prev 0.7
			}else if(armStickY < 0){
				arm.Set(0);//drive arm down prev 0
			}

			setPoint = armEncoder->GetRaw();
		}
		else{
			MoveArmToSetPoint(setPoint);
		}
		std::cout << "encoder value = " << armEncoder->GetRaw() << " " << "setPoint = " << setPoint << " " << armStickY << std::endl;


		/*{Time()
		 * INPUT[Camera() complete
		 * 			SendToBase() complete
		 * 			CheckForBall()
		 * INPUT[ReadControl() complete
		 * 			Deadzone() complete
		 * INPUT[Read sensor()
		 * 			Deadzone()
		 * DRIVE[Drivebase(auto, ) complete
		 * DRIVE[DriveLift()
		 * 			read()
		 * 			drive()
		 * DRIVE[DriveHook()
		 * DRIVE[DriveBallStuff()
		 *
		 * read controls
		 * move arm to set points (0, 20, 90, 110 degrees)
		 * "deal with the gas cylinder"
		 */
	}

	//drives the arm motor to a setpoint
	void MoveArmToSetPoint(int setPoint){
		//if arm is down too much
		if(armEncoder->GetRaw() > (setPoint + 2)){
			arm.Set(0.5); //drive arm up prev 0.6
			//if arm is too far up
		}else if(armEncoder->GetRaw() < (setPoint - 10)){
			arm.Set(-0.1); //drive arm down prev -0.2
		}else{
			arm.Set(0.3); //default to driving up so the arm shakes less prev 0.3
		}
	}

	//reads arm setpoint buttons and selects destination
	int ReadSetPointButtons(int currentPoint){
		//default set point to the current encoder value
		int point = currentPoint;

		//if set point button is pressed return its setpoint
		if(launchPad.GetRawButton(3)){
			point = encoderZero;
		}else if(launchPad.GetRawButton(4)){
			point = encoderZero + 100;
		}else if(launchPad.GetRawButton(5)){
			point = encoderZero + 200;
		}else if(launchPad.GetRawButton(6)){
			point = encoderZero + 250;
		}

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
	//Initializes a single color sensor. Pointer should be passed in with &sensor or from the vector of *I2C's
	void InitColorSensor(){
		for(int i = 0; i < 3; i++){
			Wait(0.7);
			tcaselect(i);
			//remember to or the command bit for both reading and writing everything
			FrontColorSensor.Write(TCS_COMMAND_BIT | TCS_ATIME, TCS34725_INTEGRATIONTIME_700MS);
			FrontColorSensor.Write(TCS_COMMAND_BIT | TCS_CONTROL, TCS34725_GAIN_1X); //This is the Gain
			FrontColorSensor.Write(TCS_COMMAND_BIT | TCS_ENABLE, TCS_ENABLE_PON | TCS_ENABLE_AEN | TCS_ENABLE_WEN);
		}
	}
	//This function was made to test a single color sensor. Later, there will be a single function for reading all color sensors
	//and interpreting data.

	void TestColorSensor(){
		double currentTime;
		currentTime = ColorSensorTimer.Get();
		if(currentTime == 0.0){
			ColorSensorTimer.Start();
			tcaselect(FRONT);
		}else if(ColorSensorTimer.HasPeriodPassed((double)0.7)){ //change this to shorter time later
			if(currentTime > 10){
				ColorSensorTimer.Reset();
				tcaselect(FRONT);
			}else{
				uint8_t c = 0;
				uint8_t r = 0;
				uint8_t g = 0;
				uint8_t b = 0;
				FrontColorSensor.Read(TCS_COMMAND_BIT | TCS_CDATAL, 1, &c);
				FrontColorSensor.Read(TCS_COMMAND_BIT | TCS_RDATAL, 1, &r);
				FrontColorSensor.Read(TCS_COMMAND_BIT | TCS_GDATAL, 1, &g);
				FrontColorSensor.Read(TCS_COMMAND_BIT | TCS_BDATAL, 1, &b);
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

		bool intakeButton = armStick.GetRawButton(1);
		bool shootButton = armStick.GetRawButton(6);
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
		FrontColorSensor.Read(TCS_COMMAND_BIT | TCS_RDATAL, 1, &r);
		FrontColorSensor.Read(TCS_COMMAND_BIT | TCS_GDATAL, 1, &g);
		FrontColorSensor.Read(TCS_COMMAND_BIT | TCS_BDATAL, 1, &b);
		FrontColorSensor.Read(TCS_COMMAND_BIT | TCS_CDATAL, 1, &c);
		double red, green, blue, clear;
		red = (double)r; green = (double)g; blue = (double)b; clear = (double)c;
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
			Wait(0.7);
			FrontColorSensor.Read(TCS_COMMAND_BIT | TCS_RDATAL, 1, &r);
			FrontColorSensor.Read(TCS_COMMAND_BIT | TCS_GDATAL, 1, &g);
			FrontColorSensor.Read(TCS_COMMAND_BIT | TCS_BDATAL, 1, &b);
			FrontColorSensor.Read(TCS_COMMAND_BIT | TCS_CDATAL, 1, &c);

			double red  = (double)r;
			double green  = (double)g;
			double blue  = (double)b;

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
};

START_ROBOT_CLASS(Robot);
