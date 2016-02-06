#include "WPILib.h"
#include "ColorSensorMacros.h"
enum ColorSensors{FRONT, BACK, BACK2}; //access front, back, or back2 color sensors with ColorSensors[FRONT], etc.
class Robot: public IterativeRobot
{
public:
	Robot() :
		robotDrive(6, 7, 9, 8),	// these must be initialized in the same order
		leftStick(0),
		rightStick(1),
		lw(NULL),
		chooser(),
		ColorSensor1(I2C::kOnboard, 0x29),
		ColorSensors(),
		ColorSensorTimer()
	{
		robotDrive.SetExpiration(0.1);
		robotDrive.SetInvertedMotor(robotDrive.kFrontLeftMotor,false);
		robotDrive.SetInvertedMotor(robotDrive.kFrontRightMotor,false);
		robotDrive.SetInvertedMotor(robotDrive.kRearLeftMotor,false);
		robotDrive.SetInvertedMotor(robotDrive.kRearRightMotor,false);
		cam = new USBCamera("cam0", true);
		armEncoder = new AnalogInput(1); //dunno channel
	}
private:
	RobotDrive robotDrive;
	Joystick leftStick, rightStick;
	LiveWindow *lw = LiveWindow::GetInstance();
	SendableChooser *chooser;
	const std::string autoNameDefault = "Default";
	const std::string autoNameCustom = "My Auto";
	std::string autoSelected;
	USBCamera *cam;
	AnalogInput* armEncoder;
	I2C ColorSensor1; //first color sensor, will be renamed to be more specific (eg frontcolorsensor)
	std::vector<I2C*> ColorSensors; //our vector of color sensor pointers; use for everything instead of ColorSensor1
	Timer ColorSensorTimer;

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
		ColorSensors.push_back(&ColorSensor1);
		for(unsigned int i = 0; i < ColorSensors.size(); i++){//loops through vector and initializes all color sensors.
			InitColorSensor(ColorSensors[i]);
		}
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
		if(autoSelected == autoNameCustom){
			//Custom Auto goes here
		} else {


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
			//Default Auto goes here
			/*
			 * Auton()
			 * 		ColorSense()
			 * 		ReadOtherSensors()
			 * 		DriveMotors()
			 * 		LiftGate() ...?
			 *
			 * 		read arm encoder
			 * 		drive arm at full speed to 75 degrees
			 * 		drive arm slowly to zero switch
			 * 		drive arm up to 20 degrees
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
		TestColorSensor(ColorSensors[FRONT]);//testing first sensor, remove later

		//Local declarations
		float driveThreshold = 0.005;
		//Get the y-axis of the joystick

		float yAxis1Raw = 1 * leftStick.GetY();
		float yAxis2Raw = 1 * rightStick.GetY();
		//Drive the drive motors when any input is within  -driveThreshold of 0.0
		//NOTE - currently this doesn't scale up the input from 0.0 after the deadband region -- it just uses the raw value.
		float yAxis1 = DeadZone(yAxis1Raw, driveThreshold, 0.0f);
		float yAxis2 = DeadZone(yAxis2Raw, driveThreshold, 0.0f);

		robotDrive.TankDrive(-yAxis1,-yAxis2); 	// drive
		/*{Time()
		 * INPUT[Camera()
		 * 			SendToBase()
		 * 			CheckForBall()
		 * INPUT[ReadControl()
		 * 			Deadzone()
		 * INPUT[Read sensor()
		 * 			Deadzone()
		 * DRIVE[Drivebase(auto, )
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
	void InitColorSensor(I2C* sensor){
		//remember to or the command bit for both reading and writing everything
		sensor->Write(TCS_COMMAND_BIT | TCS_ATIME, TCS34725_INTEGRATIONTIME_50MS);
		sensor->Write(TCS_COMMAND_BIT | TCS_CONTROL, TCS34725_GAIN_4X); //This is the Gain
		sensor->Write(TCS_COMMAND_BIT | TCS_ENABLE, TCS_ENABLE_PON | TCS_ENABLE_WEN | TCS_ENABLE_AEN);
	}
	//This function was made to test a single color sensor. Later, there will be a single function for reading all color sensors
	//and interpreting data.
	void TestColorSensor(I2C* sensor){
		double currentTime;

		currentTime = ColorSensorTimer.Get();
		if(currentTime == 0.0){
			ColorSensorTimer.Start();
		}else if(ColorSensorTimer.HasPeriodPassed((double)1.0)){
			uint8_t c = 0;
			uint8_t r = 0;
			uint8_t g = 0;
			uint8_t b = 0;
			sensor->Read(TCS_COMMAND_BIT | TCS_CDATAL, 1, &c);
			sensor->Read(TCS_COMMAND_BIT | TCS_RDATAL, 1, &r);
			sensor->Read(TCS_COMMAND_BIT | TCS_GDATAL, 1, &g);
			sensor->Read(TCS_COMMAND_BIT | TCS_BDATAL, 1, &b);
			std::cout << "r= " << (int)r << "g= "<< (int)g << "b= " << (int)b << "c=" << (int)c << std::endl;
		}

	}
};

START_ROBOT_CLASS(Robot);
