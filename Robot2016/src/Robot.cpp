#include "WPILib.h"
<<<<<<< HEAD
#include "ColorSensorMacros.h"
class Robot: public IterativeRobot

=======

class Robot: public IterativeRobot
>>>>>>> d894856734c4b92737f1ad3884a2268fb537fedb
{
public:
	Robot() :
		robotDrive(6, 7, 9, 8),	// these must be initialized in the same order
		leftStick(0),
		rightStick(1),
		lw(NULL),
<<<<<<< HEAD
		chooser(),
		ColorSensor(I2C::kOnboard, 0x29)

=======
		chooser()
>>>>>>> d894856734c4b92737f1ad3884a2268fb537fedb
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
<<<<<<< HEAD
	I2C ColorSensor;

=======
>>>>>>> d894856734c4b92737f1ad3884a2268fb537fedb

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
<<<<<<< HEAD
=======

>>>>>>> d894856734c4b92737f1ad3884a2268fb537fedb
		chooser = new SendableChooser();
		chooser->AddDefault(autoNameDefault, (void*)&autoNameDefault);
		chooser->AddObject(autoNameCustom, (void*)&autoNameCustom);
		SmartDashboard::PutData("Auto Modes", chooser);

<<<<<<< HEAD
		ColorSensor.Write(TCS_ENABLE, TCS_ENABLE_PON);
		ColorSensor.Write(TCS_ENABLE, TCS_ENABLE_PON | TCS_ENABLE_AEN);
		ColorSensor.Write(TCS_ATIME, uint8_t(50));
		ColorSensor.Write(TCS_CONTROL, uint8_t(0x00));
=======
>>>>>>> d894856734c4b92737f1ad3884a2268fb537fedb
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
<<<<<<< HEAD

		//Local declarations
		float driveThreshold = 0.005;
		uint8_t *c = NULL;
		uint8_t *r = NULL;
		uint8_t *g = NULL;
		uint8_t *b = NULL;
=======
		//Local declarations
		float driveThreshold = 0.005;
>>>>>>> d894856734c4b92737f1ad3884a2268fb537fedb

		//Get the y-axis of the joystick

		float yAxis1Raw = 1 * leftStick.GetY();
		float yAxis2Raw = 1 * rightStick.GetY();
<<<<<<< HEAD
=======

>>>>>>> d894856734c4b92737f1ad3884a2268fb537fedb
		//Drive the drive motors when any input is within  -driveThreshold of 0.0
		//NOTE - currently this doesn't scale up the input from 0.0 after the deadband region -- it just uses the raw value.
		float yAxis1 = DeadZone(yAxis1Raw, driveThreshold, 0.0f);
		float yAxis2 = DeadZone(yAxis2Raw, driveThreshold, 0.0f);
<<<<<<< HEAD

=======
>>>>>>> d894856734c4b92737f1ad3884a2268fb537fedb
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
<<<<<<< HEAD
		ColorSensor.Read(TCS_CDATAL, 1, c);
		ColorSensor.Read(TCS_RDATAL, 1, r);
		ColorSensor.Read(TCS_GDATAL, 1, g);
		ColorSensor.Read(TCS_BDATAL, 1, b);
		//std::cout << "r= " << r << "g= "<< g << "b= " << b << "c=" << c << std::endl;
		//printf("r=%c g=%c b=%c c=%c", *r, *g, *b, *c);

		//DriverStation::ReportError(r);
		//+ "g=" + *g + "b=" + *b + "c=" + *c);
=======
>>>>>>> d894856734c4b92737f1ad3884a2268fb537fedb
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

<<<<<<< HEAD

};

START_ROBOT_CLASS(Robot);
=======
};

START_ROBOT_CLASS(Robot)
>>>>>>> d894856734c4b92737f1ad3884a2268fb537fedb
