


const float WHEEL_DIAMETER = 4; //the diameter of the wheels in inches
const int UPR = 1440;          //the Encoder Units Per Rotation.
float WHEEL_CIRCUMFERENCE = PI*WHEEL_DIAMETER; //The calculated circumference of the wheel
float DISTANCE_PER_UNIT = WHEEL_CIRCUMFERENCE/UPR; //Distance traveled per encoder unit
float DEGREES_PER_UNIT = UPR/360;

const float BASE_WIDTH = 13; //base width in inches from one wheel to the other
const float TURN_CIRCUMFERENCE = PI*BASE_WIDTH;
const float TURN_UNITS_PER_DEGREE = (TURN_CIRCUMFERENCE/360)/DISTANCE_PER_UNIT;
const float DRAG_MULTIPLIER = 1.15; //value used to correct for wheel drag when using encoders to rotate
long value = 0;
int floorLight = 0;

void init(){
	floorLight = SensorValue[light];
}

void driveForward(int speed){
	motor[leftMotor]  = speed;
	motor[rightMotor] = speed;
}

void driveBackward(int speed){
	motor[leftMotor]  = -speed;
	motor[rightMotor] = -speed;
}

void stopMotors(){
	motor[leftMotor] = 0;
	motor[rightMotor] = 0;
}

void resetDriveEncoders(){
	nMotorEncoder[leftMotor] = 0;
	nMotorEncoder[rightMotor] = 0;
}

void forward(int inches, int speed){
	long units = inches/DISTANCE_PER_UNIT; //this calculates the number of encoder ticks necessary to drive the inputted number of inches
	resetDriveEncoders();
	driveForward(speed);
	while(nMotorEncoder[rightMotor] < units){
		value = nMotorEncoder[rightMotor]; //for debugging
	} //wait for motors to finish moving
	stopMotors();
	wait10Msec(10);
}

void fowardUntilLine(int threshold, int speed){
	int baseline = SensorValue[light];
	driveForward(speed);
	while(SensorValue[light] < baseline+threshold){}
	stopMotors();
	wait10Msec(10);
}

void backward(int inches, int speed){
	long units = inches/DISTANCE_PER_UNIT; //this calculates the number of encoder ticks necessary to drive the inputted number of inches
	resetDriveEncoders();

	driveBackward(speed);

	while(nMotorEncoder[rightMotor] > -units){
		value = nMotorEncoder[rightMotor]; //for debugging
	} //wait for motors to finish moving
	stopMotors();
	wait10Msec(10);
}

void backwardTime(int seconds, int speed){
	driveBackward(speed);
	wait10Msec(seconds*100);
	stopMotors();
}

void forwardTime(int seconds, int speed){
	driveForward(speed);
	wait10Msec(seconds*100);
	stopMotors();
	wait10Msec(10);
}


void lineFollow(int inches, int threshold, bool lineOnLeft, int speed){
	int baseline = floorLight;//SensorValue[light];
	long units = inches/DISTANCE_PER_UNIT;
	resetDriveEncoders();
	while(nMotorEncoder[leftMotor] < units && nMotorEncoder[rightMotor] < units){
		if(SensorValue[light] < baseline + threshold){
			motor[rightMotor] = lineOnLeft? speed : speed/2;
			motor[leftMotor]  = lineOnLeft? speed/2 : speed;
		}else{
			motor[rightMotor] = lineOnLeft? speed/2 : speed;
			motor[leftMotor]  = lineOnLeft? speed : speed/2;
		}
	}
	stopMotors();

}

void turnLeftEncoder(float degrees, int power){
	resetDriveEncoders();
	long target = ((float)degrees*TURN_UNITS_PER_DEGREE*DRAG_MULTIPLIER);
	motor[rightMotor] =  power;
	motor[leftMotor]  = -power;
	while(nMotorEncoder[leftMotor] > -target && nMotorEncoder[rightMotor] < target){
	//while(true){
		value = nMotorEncoder[rightMotor];
	}
	stopMotors();
	wait10Msec(25);
}

void turnRightEncoder(float degrees, int power){
	resetDriveEncoders();
	int target = ((float)degrees*TURN_UNITS_PER_DEGREE*DRAG_MULTIPLIER);
	motor[rightMotor] = -power;
	motor[leftMotor]  =  power;
	while(nMotorEncoder[leftMotor] < target && nMotorEncoder[rightMotor] > -target){}
	stopMotors();
	wait10Msec(25);
}

void turnLeft(float degrees, int power)
{
	wait10Msec(10);
	SensorValue[gyro] = 0;
	//degrees=-degrees; // for gyro mounted upside down
	motor[rightMotor] =  power;
	motor[leftMotor]  = -power;
	while(SensorValue[gyro] > -degrees*10)
	{
		nxtDisplayTextLine(2, "Gyro: %d", SensorValue[gyro]);
	}
	stopMotors();
	wait10Msec(10);
}

void turnRight(float degrees, int power){
	SensorValue[gyro] = 0;
	motor[rightMotor] = -power;
	motor[leftMotor]  =  power;
	while(SensorValue[gyro] < degrees*10)
	{
		nxtDisplayTextLine(2, "Gyro: %d", SensorValue[gyro]);
	}
	stopMotors();
	wait10Msec(10);
}


void moveArm(int degrees, int speed){
	int target = (degrees*DEGREES_PER_UNIT)*2; //multiplying by two for gear ratio
	nMotorEncoder[armMotor] = 0;

	if(degrees > 0){
		motor[armMotor] = speed;
		while(nMotorEncoder[armMotor] < target){}
	}else{
		motor[armMotor] = -speed;
		while(nMotorEncoder[armMotor] > target){}
	}
	motor[armMotor] = 0; //target reached
	wait10Msec(10);
}

void dispense_all(){
	motor[intakeMotor] = -100;
	wait10Msec(150);
	motor[intakeMotor] = 0;
}
