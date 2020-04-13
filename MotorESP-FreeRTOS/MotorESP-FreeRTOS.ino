#include <Arduino_FreeRTOS.h>
#include <Wire.h>
#include <Adafruit_MotorShield.h>

#define MULT_TURN_ON_ITSELF 1.2962 // 35:27
#define MULT_TURN_ON_WHEEL 2.5925 //  70:27
#define CM_WITH_ONESTEP 0.47 //cm


//Freertos methods
void TaskGoStraight(void *pvParameters); 
void TaskGoStraightOf(void *pvParameters); 
void TaskTurnWithTheLeftWheel(void *pvParameters); 
void TaskTurnWithTheRightWheel(void *pvParameters); 
void TaskTurnOnItself(void *pvParameters);

//Struct for rotation
typedef struct
{
    uint32_t degreesRotation;
    bool isForward;
} RotationData;

// Create the motor shield object with the default I2C address
Adafruit_MotorShield AFMS = Adafruit_MotorShield();
// Connect a stepper motor with 200 steps per revolution (1.8 degree)
Adafruit_StepperMotor *leftMotor = AFMS.getStepper(200, 1);
Adafruit_StepperMotor *rightMotor = AFMS.getStepper(200, 2);


void setup() {
  Serial.begin(9600);
  Serial.println("Stepper test!");
  AFMS.begin();  // create with the default frequency 1.6KHz

  //Set rpm
  leftMotor->setSpeed(10);  // 10 rpm
  rightMotor->setSpeed(10);  // 10 rpm

  //Create task here
}


/*
 * Description:
 *    Go Straight of 0,47 cm
 * 
 * Parameters:
 *    None
 * 
 * Returns:
 *    None
 * 
 * Notes and Warnings:
 *    Blocking method
 */
void TaskGoStraight(void *pvParameters) {
  (void) pvParameters;
  leftMotor->onestep(FORWARD, SINGLE);
  rightMotor->onestep(BACKWARD, SINGLE);

  
  //xTaskCreate documentation: https://www.freertos.org/a00125.html
}

/*
 * Description:
 *    Go Straight of X cm
 * 
 * Parameters:
 *    cm: cm to perform
 * 
 * Returns:
 *    None
 * 
 * Notes and Warnings:
 *    Blocking method
 */
void TaskGoStraightOf(void *pvParameters) {
  unsigned int stepToDo = ( ( uint32_t ) pvParameters ) / CM_WITH_ONESTEP;
  for(int i = 0; i < stepToDo; i++){
    leftMotor->onestep(FORWARD, SINGLE);
    rightMotor->onestep(BACKWARD, SINGLE);
  }
}



/*
 * Description:
 *    Turn on only with a wheel (Rotation center: left wheel)
 * 
 * Parameters:
 *    degreesRotation: rotation degrees of ants
 *    isForward:
 *      - true: turn on forward (senso orario)
 *      - false: turn on backward (senso antiorario)
 * 
 * Returns:
 *    None
 * 
 * Notes and Warnings:
 *    blocking methods
 */
void TaskTurnWithTheLeftWheel(void *pvParameters) {
  RotationData data = ((RotationData&) pvParameters);
  rightMotor->step(data.degreesRotation*MULT_TURN_ON_WHEEL, (data.isForward)? BACKWARD:FORWARD, SINGLE);
}

/*
 * Description:
 *    Turn on only with a wheel (Rotation center: right wheel)
 * 
 * Parameters:
 *    degreesRotation: rotation degrees of ants
 *    isForward:
 *      - true: turn on forward (senso orario)
 *      - false: turn on backward (senso antiorario)
 * 
 * Returns:
 *    None
 * 
 * Notes and Warnings:
 *    blocking methods
 */
void TaskTurnWithTheRightWheel(void *pvParameters) {
  RotationData data = ((RotationData&) pvParameters);
  leftMotor->step(data.degreesRotation*MULT_TURN_ON_WHEEL, (data.isForward)? FORWARD:BACKWARD, SINGLE);
}


/*
 * Description:
 *    Turn on itself (Rotation center equals to 1/2 of distance between the wheels)
 * 
 * Parameters:
 *    rotationDegrees: rotation degrees of ants
 * 
 * Returns:
 *    None
 * 
 * Notes and Warnings:
 *    blocking method
 */
void TaskTurnOnItself(void *pvParameters){
  unsigned int stepToDo;
  int rotationDegrees = ((int) pvParameters);
  if(rotationDegrees > 0){
    stepToDo = (unsigned int)(rotationDegrees*MULT_TURN_ON_ITSELF);
    for(unsigned int i = 0; i < stepToDo; i++){
      rightMotor->onestep(FORWARD,SINGLE);
      leftMotor->onestep(FORWARD,SINGLE);
    }
  }else{
    stepToDo = (unsigned int)(-rotationDegrees*MULT_TURN_ON_ITSELF);
    for(unsigned int i = 0; i < stepToDo; i++){
      rightMotor->onestep(BACKWARD,SINGLE);
      leftMotor->onestep(BACKWARD,SINGLE);
    }
  }
}



void loop() {}
