// Define los estados HIGH,LOW de cada led 
#define led1on digitalWrite(led1, HIGH);
#define led2on digitalWrite(led2, HIGH);
#define led3on digitalWrite(led3, HIGH);
#define led1off digitalWrite(led1, LOW);
#define led2off digitalWrite(led2, LOW);
#define led3off digitalWrite(led3, LOW);

//Colocamos la parte de la tarjeta encender:
int Inicio = 9; 
int Stop = 10; 
int Estado;
int count=0;
int count2=0;
//Agregar busqueda aleatoria y Evadir RAND
//Mejorar ataque e implementar giro.
//Revisar velocidad máxima antes de salirse del ring por inercia, sin detectar el blanco y retroceder
//Agregar giro y choque tras determinada distancia
//Inclinación y retroceder y esquivada táctica tras detectar no inclinación. Caso retroceder: Un motor más rápido que el otro.
//Optimizar velocidad giros


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

const int in1a = 19; //direccion motor 1(derecho) hacia adelante
const int in1b = 3; //direccion motor 1(derecho) reversa

const int in2a = 4; //direccion motor 2(izquierdo) hacia adelante
const int in2b = 2; //direccion motor 2(izquierdo) reversa

const int pwm1 = 5; //PWM motor 1 Derecho
const int pwm2 = 6; //PWM motor 2 Izquierdo

const int sw1 = 8; //PWM motor 1 Derecho
const int sw2 = 7; //PWM motor 2 Izquierdo

bool circular= false;
bool evade= false;
bool switcher= false;
bool tornade = false;
bool activate = false;

 int limiteb1 = 50;
 int limiten1 = 80;
 int limiteb2 = 50;
 int limiten2 = 80;
//Colocamos para leer sensor:
int read_on;

int maxDistanceMini = 700; 
int maxDistanceBig1 = 300; 
int maxDistanceBig2 = 350; 

const unsigned long calibracionTemp = 3000;
int adc[2];
int sensoresPisoMin[] = {1023,1023}; 
int sensoresPisoMax[] = {0,0}; 

unsigned long startMillis;  //some global variables available anywhere in the program
unsigned long currentMillis;

typedef enum{
  BUSCANDO = 0,
  ATAQUE,
  BUSCANDO_EV,
  ATAQUE_EV,
  EVADIR
}ST_ROBOT;

typedef enum{
  NOEVADIR = 0,
  EVADIRFULL,
  EVADIRIZQUIERDA,
  EVADIRDERECHA
}ST_EVADIR;

ST_ROBOT st_robot = BUSCANDO;
ST_EVADIR st_evadir = NOEVADIR;


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

  pinMode(sw1,INPUT);
  pinMode(sw2,INPUT);
}

void lecturaSensores(){
  //lectura de los sensores de piso y los sharp:
 adcd = analogRead(6); // Lee sensor Derecho
 adci = analogRead(7); // Lee sensor Izquierdo
 sharpi = analogRead(2);
 sharpd = analogRead(1);
 sidei = analogRead(4);
 sided = analogRead(0);
}

void robot_buscando(){ 
    lecturaSensores();
    if(adci <= limiteb1 || adcd <= limiteb2){
      st_robot = EVADIR;
    
    }else{
      led1off;
      led2off;
      led3off;
      if(sharpi<maxDistanceBig1 && sharpd<maxDistanceBig2){
          motors(200,200);
          if(sidei<maxDistanceMini){
            led1on;
            led2on;
            led3off;
              while(sharpd<maxDistanceBig2 && adci >= limiten1 && adcd >= limiten1){
                lecturaSensores();
                motors(-255,100);
              }
          }
          if(sided<maxDistanceMini){
            led1on;
            led2off;
            led3on;
              while(sharpi<maxDistanceBig1 && adci >= limiten1 && adcd >= limiten1){
                lecturaSensores();
                motors(100,-255);
              }
          }
      }else{
        st_robot = ATAQUE;
      }
    }
}

void robot_buscandoEv(){ 
    lecturaSensores();
    if(adci <= limiteb1 || adcd <= limiteb2){
      st_robot = EVADIR;
    
    }else{
      led1off;
      led2off;
      led3off;
      if(sharpi<maxDistanceBig1 && sharpd<maxDistanceBig2){
          motors(200,200);
          if(sidei<maxDistanceMini){
            led1on;
            led2on;
            led3off;
            motors(-255, -255);
            delay(200); //250
            motors(-255, -100);
            delay(200); //250
            motors(0,0);
          }
          if(sided<maxDistanceMini){
            led1on;
            led2off;
            led3on;
            motors(-255, -255);
            delay(200); //250
            motors(-100, -255);
            delay(200); //250
            motors(0,0);
          }
      }else{
        st_robot = ATAQUE;
      }
    }
}

void robot_buscandoB(){ 
    lecturaSensores();
    if(adci <= limiteb1 || adcd <= limiteb2){
      st_robot = EVADIR;
    
    }else{
      led1off;
      led2off;
      led3off;
      if(sharpi<maxDistanceBig1 && sharpd<maxDistanceBig2){
          motors(75,150);
          if(sidei<maxDistanceMini){
            led1on;
            led2on;
            led3off;
              while(sharpd<maxDistanceBig2 && adci >= limiten1 && adcd >= limiten1){
                lecturaSensores();
                motors(-255,100);
              }
          }
          if(sided<maxDistanceMini){
            led1on;
            led2off;
            led3on;
              while(sharpi<maxDistanceBig1 && adci >= limiten1 && adcd >= limiten1){
                lecturaSensores();
                motors(100,-255);
              }
          }
      }else{
        st_robot = ATAQUE;
      }
    }
}

void robot_buscandoBEv(){ 
    lecturaSensores();
    if(adci <= limiteb1 || adcd <= limiteb2){
      st_robot = EVADIR;
    
    }else{
      led1off;
      led2off;
      led3off;
      if(sharpi<maxDistanceBig1 && sharpd<maxDistanceBig2){
          motors(150,75);
          if(sidei<maxDistanceMini){
            led1on;
            led2on;
            led3off;
            motors(-255, -255);
            delay(200); //250
            motors(-255, -100);
            delay(200); //250
            motors(0,0);
          }
          if(sided<maxDistanceMini){
            led1on;
            led2off;
            led3on;
            motors(-255, -255);
            delay(200); //250
            motors(-100, -255);
            delay(200); //250
            motors(0,0);
          }
      }else{
        st_robot = ATAQUE;
      }
    }
}


void robot_ataque(){
        
  
  if(adci <= limiteb1 || adcd <= limiteb2){
    st_robot = EVADIR;
  }else if(adci >= limiten1 && adcd >= limiten2){
    if(sharpi>maxDistanceBig1 && sharpd>maxDistanceBig2 && adci >= limiten1 && adcd >= limiten2){
        led1on;
        led2on;
        led3on;
        motors(255,255);
        lecturaSensores();
    }else if(sharpi<maxDistanceBig1 && sharpd>maxDistanceBig2 && adci >= limiten1 && adcd >= limiten2){
        led1on;
        led2on;
        led3off;
        motors(255,-255);
        lecturaSensores();
    }else if(sharpi>maxDistanceBig1 && sharpd<maxDistanceBig2 && adci >= limiten1 && adcd >= limiten2){
        led1off;
        led2on;
        led3on;
        motors(-255,255);
        lecturaSensores();
    }else if(adci <= limiteb1 || adcd <= limiteb2){
        st_robot = EVADIR;
    }else{
      st_robot = BUSCANDO;
    }
  }
  
}


void robot_evadir(){
  led1off;
  led2off;
  led3off;
  long tiempoGiro = random(400,600);
  if(adcd <= limiteb2 && adci >= limiten1){
    st_evadir = EVADIRDERECHA;
  }else if(adcd >= limiten2 && adci <= limiteb1){
    st_evadir = EVADIRIZQUIERDA;
  }else if(adcd <= limiteb2 && adci <= limiteb1){
    st_evadir = EVADIRFULL;
  }else{
    st_evadir = NOEVADIR;
    
  }
  switch(st_evadir){
    case EVADIRDERECHA:
    led2on;
    led1off;
    led3off;
    motors(-65, -255);
    delay(tiempoGiro); //250
    motors(0,0);
    break;
    case EVADIRIZQUIERDA:
    led1on;
    led2off;
    led3off;
    motors(-255, -65);
    delay(tiempoGiro); //250
    motors(0,0);
    break;
    case EVADIRFULL:
    led1on;
    led2on;
    led3off;
    motors(-255, -175);
    delay(tiempoGiro); //400
    motors(0,0);
    break;
    case NOEVADIR:
    led1off;
    led2off;
    led3off;
    st_robot = BUSCANDO;
    break;


  }
}

void robot_process(){
    switch (st_robot)
    {
    
    case BUSCANDO:
        robot_buscando();
      break;
    case ATAQUE:
        robot_ataque();
      break;
          
    case EVADIR:
        robot_evadir();
      break;
      
    default: 
        st_robot = BUSCANDO;
      break;
    }
}

void robot_processEv(){
    switch (st_robot)
    {
    
    case BUSCANDO:
        robot_buscandoEv();
      break;
    case ATAQUE:
        robot_ataque();
      break;
          
    case EVADIR:
        robot_evadir();
      break;
      
    default: 
        st_robot = BUSCANDO;
      break;
    }
}

void robot_processB(){
    switch (st_robot)
    {
    
    case BUSCANDO:
        robot_buscandoB();
      break;
    case ATAQUE:
        robot_ataque();
      break;
          
    case EVADIR:
        robot_evadir();
      break;
      
    default: 
        st_robot = BUSCANDO;
      break;
    }
}

void robot_processBEv(){
    switch (st_robot)
    {
    
    case BUSCANDO:
        robot_buscandoBEv();
      break;
    case ATAQUE:
        robot_ataque();
      break;
          
    case EVADIR:
        robot_evadir();
      break;
      
    default: 
        st_robot = BUSCANDO;
      break;
    }
}


void robot_calibracion(){ //Checkear si decimales
  led1off;
  led2off;
  led3off;
  adc[1] = analogRead(6); // Lee sensor Derecho
  adc[0] = analogRead(7); // Lee sensor Izquierdo
  for(int i=0; i<2;i++){
    if(sensoresPisoMax[i] < adc[i]){
      sensoresPisoMax[i] = adc[i];
    }
    if(sensoresPisoMin[i] > adc[i]){
      sensoresPisoMin[i] = adc[i];
    }
  }
  int lim1 = sensoresPisoMin[0]+15;
  int lim2 = sensoresPisoMax[0]-45;
  int lim3 = sensoresPisoMin[1]+15;
  int lim4 = sensoresPisoMax[1]-45;

  limiteb1 = (lim1+lim2)/2;
  limiten1 = (lim1+lim2)/2;
  limiteb2 = (lim3+lim4)/2;
  limiten2 = (lim3+lim4)/2;
}


void setup() {
 variables();
 startMillis = millis();
}

void loop() {

  currentMillis = millis();  //get the current "time" (actually the number of milliseconds since the program started)
  if (currentMillis - startMillis < calibracionTemp)  //test whether the period has elapsed
  {
    robot_calibracion();
    delay(10);   }
  Estado = digitalRead(Inicio);  
  if(Estado == 1){
    led1off;
    led2off;
    led3off;
    activate=true;
  }
  if(Estado == 0){
   //if(digitalRead(sw2)==HIGH){
      activate=false;
      motors(0,0);
  }
if(digitalRead(sw1)==HIGH){
    led1on;
    led2off;
    led3off;
    circular = true;
  }
  if(digitalRead(sw2)==HIGH){
    led1off;
    led2on;
    led3off;
    evade = true;
  }
  
   if(activate && !evade && !circular){
    lecturaSensores();
    robot_process();
   }
   if(activate && evade && !circular){
    lecturaSensores();
    robot_processEv();
   }
   if(activate && !evade && circular){
    lecturaSensores();
    robot_processB();
   }
   if(activate && evade && circular){
    lecturaSensores();
    robot_processBEv();
   }

}
