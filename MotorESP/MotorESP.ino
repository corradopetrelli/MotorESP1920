#include <Wire.h>
#include <Adafruit_MotorShield.h>

#define DISTANZA_RUOTE 15 //cm
#define CIRCONFERENZA_RUOTA 18.85 //cm
#define CM_CON_ONESTEP 0.47 //cm

//Note
//5 RIVOLUZIONI (5*200 steps = 1000 steps) DELLA RUOTA EQUIVALE A GIRARE SU SE STESSA QUINDI 2PI
//Quindi se voglio girare di 45° (ricavando il n° di step da fare): 1000 : 360 = x : 45 quindi x = 125


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


  //TEST CODE
  for(int i = 0; i < 200; i++)
    vaiDritto();
  giraSuSeStesso(45,true);
  for(int i = 0; i < 200; i++)
    vaiDritto(); 
  
}


/*
 * Description:
 *    Vai dritto di 0,47 cm
 * 
 * Parameters:
 *    None
 * 
 * Returns:
 *    None
 * 
 * Notes and Warnings:
 *    Metodo bloccante
 */
void vaiDritto() {
  leftMotor->onestep(FORWARD, SINGLE);
  rightMotor->onestep(BACKWARD, SINGLE);
}

/*
 * Description:
 *    Vai dritto di X cm
 * 
 * Parameters:
 *    cm: numero di centrimetri da percorrere
 * 
 * Returns:
 *    None
 * 
 * Notes and Warnings:
 *    Metodo bloccante
 */
void vaiDritto(int cm) {
  int stepToDo = cm / CM_CON_ONESTEP;
  for(int i = 0; i < stepToDo; i++){
    leftMotor->onestep(FORWARD, SINGLE);
    rightMotor->onestep(BACKWARD, SINGLE);
  }
}



/*
 * Description:
 *    Gira solamente con una ruota verso sinistra (Centro di rotazione: ruota sinistra)
 * 
 * Parameters:
 *    gradi: gradi di rotazione
 *    isAvanti:
 *      - true: gira in avanti (senso antiorario)
 *      - false: gira all'indietro (senso orario)
 * 
 * Returns:
 *    None
 * 
 * Notes and Warnings:
 *    Metodo bloccante
 */
void giraSuUnaRuotaVersoSinistra(int gradi, boolean isAvanti) {
  rightMotor->step(5 * gradi, (isAvanti)? BACKWARD:FORWARD, SINGLE);
}

/*
 * Description:
 *    Gira solamente con una ruota verso destra (Centro di rotazione: ruota destra)
 * 
 * Parameters:
 *    gradi: gradi di rotazione
 *    isAvanti:
 *      - true: gira in avanti (senso orario)
 *      - false: gira all'indietro (senso antiorario)
 * 
 * Returns:
 *    None
 * 
 * Notes and Warnings:
 *    Metodo bloccante
 */
void giraSuUnaRuotaVersoDestra(int gradi, boolean isAvanti) {
  leftMotor->step(5 * gradi, (isAvanti)? FORWARD:BACKWARD, SINGLE);
}


/*
 * Description:
 *    Gira su se stesso (Centro di rotazione: 1/2 della distanza tra le ruote)
 * 
 * Parameters:
 *    gradi: gradi di rotazione
 *    isOrario:
 *      - true: gira in senso orario
 *      - false: gira in senso antiorario
 * 
 * Returns:
 *    None
 * 
 * Notes and Warnings:
 *    Metodo bloccante
 */
void giraSuSeStesso(int gradi, boolean isOrario){
  //Se 5*gradi è dispari si perde uno step da fare (mezzo centimetro per ruota)
  int stepToDo = (int)((5*gradi)/2);
  for(int i = 0; i < stepToDo; i++){
    rightMotor->onestep((isOrario)?FORWARD:BACKWARD,DOUBLE);
    leftMotor->onestep((isOrario)?FORWARD:BACKWARD,DOUBLE);
  }

}



void loop() {}
