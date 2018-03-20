#define POSITION_POSEE 20
#define FILTRE_IR 20

/*
int moyen1;
int moyen2;
*/

int beDetect;
const int capteurIR = A2;   //capteur IR

int valeurLue; //debug
const int servoPin = 9;          //PIN de la led à changer


int valeurLueBrut1; //debug
int valeurLueBrut2; //debug
int valeurAcc1; //debug

int detection;

// Import biblio gestion servo-moteur
#include <Servo.h>

//atomic print
#include <util/atomic.h>

//creation de l'objet servo
Servo servo;

const int ledPin = 13;          //PIN de la led à changer
int  tab_valeur_lue[20]; //debug
int moyenne;

int detecter() {
     /*moyen1=800;
     moyen2=900;
     moyen1/=10;
     moyen2/=10;
     
     valeurLue=abs(moyen1-moyen2);
     */
    
     static int valeurLueBrut[20];
     static int valeurAcc[20];

     // Lecture capteur
     valeurLueBrut1 = analogRead(capteurIR);
     valeurLueBrut[0] = valeurLueBrut1;
     //valeurLueBrut[1] = valeurLueBrut2;
          
     //valeurAcc[0] = abs(valeurLueBrut[0] - valeurLueBrut[1]);
     //valeurAcc1 = valeurAcc[0];
     
     for(int bg = 0 ; bg < FILTRE_IR ; bg++){
       valeurLueBrut[FILTRE_IR-bg] = valeurLueBrut[FILTRE_IR-(bg+1)] ; 
       //valeurAcc[FILTRE_IR-bg] = valeurAcc[FILTRE_IR-(bg+1)];
     }
     
     int bg;
     int somme;

     for(bg = 0 ; bg < FILTRE_IR ; bg++){
       somme +=  valeurLueBrut[FILTRE_IR];
     }
     moyenne = somme/FILTRE_IR;
     int seuil = 90;
     //90 -> 30cm
     //137 -> 20cm
     //250 -> 10cm
     //450 -> 5cm
     
     //Pour valeur brut
     //int seuil = 200;
     if (moyenne > seuil) {
       detection = 1;
     }
    return detection; 
     memcpy(tab_valeur_lue,valeurLueBrut,sizeof(valeurLueBrut));
}

void monter() {
    // En bas roues relevées, position servo 20°
    //servo.write(POSITION_POSEE); //Necessaire ?
    //Serial.println("Monter - Roue en l'air - Plateau en bas ");
    servo.write(POSITION_POSEE);
}

void Robot50HzInterrupt() {   
    valeurLue=5; //debug
    beDetect = detecter();
    
    if(beDetect){
        digitalWrite(ledPin, HIGH);
    }
}

void setup() {

  int CalibrationIR;
  Serial.begin(115200);

  servo.attach(servoPin);
  digitalWrite(ledPin, LOW);

  /*
  CalibrationIR = 0;
  for(int z=0;z<10;z++){
    detecter();
    CalibrationIR += valeurLue;
    delay(100);
  }
  CalibrationIR = CalibrationIR / 10;
  Serial.println(CalibrationIR);
  delay(1000);
  */
}

void loop() {
     ATOMIC_BLOCK(ATOMIC_RESTORESTATE){
       /*
       Serial.print("Debug moyen1: ");
       Serial.println(moyen1);
       Serial.print("Debug moyen2: ");
       Serial.println(moyen2);
       Serial.print("Debug value: ");
       Serial.println(valeurLue);
       */
       Serial.print("Debug value brut : ");
       Serial.println(valeurLueBrut1);


       /*
       Serial.println("Value of array");
       for (int i=0; i<20; i++) {
         Serial.print(">>");
         Serial.println(i);
         Serial.println(tab_valeur_lue[i]);
       }
       */
       Serial.print("Moyenne : ");
       Serial.println(moyenne);

       Serial.print("Detection: ");
       Serial.println(detection);
       detection=0;
       digitalWrite(ledPin, LOW);
       Serial.print("Detection: ");
       Serial.println(detection);
     }

     delay(250);
       digitalWrite(ledPin, LOW);
     monter();
}
