// Define los estados HIGH,LOW de cada led 
#define led1on digitalWrite(led1, HIGH);
#define led2on digitalWrite(led2, HIGH);
#define led3on digitalWrite(led3, HIGH);
#define led1off digitalWrite(led1, LOW);
#define led2off digitalWrite(led2, LOW);
#define led3off digitalWrite(led3, LOW);

//Crea una variable a cada led 
const int led1 = 13; 
const int led2 = 12; 
const int led3 = 11; 

int adcd=0; //sensor de piso derecho QTR1
int adci=0; //sensor de piso izquierdo QTR2
int sharpd = 0; //sensor proximidad derecho QTR1 A4
int sharpi = 0; //sensor proximidad izquierdo QTR1 A2
int sided = 0; //sensor proximidad derecho QTR1 A4
int sidei = 0; //sensor proximidad izquierdo QTR1 A2

const int in1a = 19; //direccion motor 1 derecha
const int in2a = 4; //direccion motor 2 derecha
const int in1b = 3; //direccion motor 1 izquierda
const int in2b = 2; //direccion motor 2 izquierda
const int pwm1 = 5; //PWM motor 1 Derecho
const int pwm2 = 6; //PWM motor 2 Izquierdo

const int lim = 50;
const int lim2 = 120;

typedef enum{
  BUSCANDO = 0,
  ATAQUE,
  EVADIR,
}ST_ROBOT;

ST_ROBOT st_robot = BUSCANDO;


int motors(int speedL, int speedR){
  if(speedL>0){
    digitalWrite(in2b,LOW);
    digitalWrite(in1b,HIGH);
  }  
  else{
    digitalWrite(in2b,HIGH);
    digitalWrite(in1b,LOW);
    speedL=speedL*-1;
  }

  if(speedR>0){
    digitalWrite(in2a,LOW);
    digitalWrite(in1a,HIGH);
   }  
  else{
    digitalWrite(in2a,HIGH);
    digitalWrite(in1a,LOW);
    speedR=speedR*-1;
  }
  analogWrite(pwm1,speedR);
  analogWrite(pwm2,speedL);
}




void variables(){
  //configuracion de condiciones iniciales
  pinMode(led1,OUTPUT);
  pinMode(led2,OUTPUT);
  pinMode(led3,OUTPUT);
  
  pinMode(in1a,OUTPUT);
  pinMode(in1b,OUTPUT);
  pinMode(in2a,OUTPUT);
  pinMode(in2b,OUTPUT);
  
  pinMode(pwm1,OUTPUT);
  pinMode(pwm2,OUTPUT);
}

void lecturaSensores(){
  //lectura de los sensores de piso y los sharp:
 adcd = analogRead(6); // Lee sensor Derecho
 delay(5);
 adci = analogRead(7); // Lee sensor Izquierdo
 delay(5);
 sharpi = analogRead(4);
 delay(5);
 sharpd = analogRead(2);
 delay(5);
 sidei = analogRead(3);
 delay(5);
 sided = analogRead(0);
 delay(5);
}

void robot_buscando(){
    /* poner sensado aca, de todos los sensores*/

    if(adci <= lim || adcd <= lim){
      led1off;
      led2off;
      led3on;
      st_robot = EVADIR;
    
    }else if(adci >= lim2 && adcd >= lim2){
      if(sharpd>400){
        led1on;
        led2off;
        led3off;
        motors(90,50);
        delay(300);
        motors(0,0);
       }else{
          st_robot = ATAQUE;
       }
    }
}

void robot_ataque(){
        
  
  if(adci <= lim || adcd <= lim){
    st_robot = EVADIR;
  }else if(adci >= lim2 && adcd >= lim2){
    if(sharpd<400){

      //Corregimos ataque:
      if(adci <= lim || adcd <= lim){
        st_robot = EVADIR;
      }else{
        led1off;
        led2off;
        led3on;
        motors(60,60);
        delay(300);
        motors(0,0);
      }
    }else{
      st_robot = BUSCANDO;
    }
  }

}

void robot_evadir(){
  if(adci >= lim2 && adcd <= lim){
    led1off;
    led3off;
    led2on;
    motors(-65, -65);
    delay(200);
    motors(0,0);
  }
  else{
    st_robot = BUSCANDO;
  }
}

void robot_process(){
    switch (st_robot)
    {
    case BUSCANDO:
        robot_buscando();
      break;
    case ATAQUE:
      if(sharpd<400){  
          robot_ataque();
        }else if(sharpd>400){
          st_robot = BUSCANDO;
        }
      break;
      
    case EVADIR:
        robot_evadir();
      break;
      
    default: 
        st_robot = BUSCANDO;
      break;
    }
}







void setup() {
 //configuracion de condiciones iniciales
 variables();
}

void loop() {
    lecturaSensores();
    robot_process();
 
}