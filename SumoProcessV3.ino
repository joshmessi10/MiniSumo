// Define los estados HIGH,LOW de cada led 
#define led1on digitalWrite(led1, HIGH);
#define led2on digitalWrite(led2, HIGH);
#define led3on digitalWrite(led3, HIGH);
#define led1off digitalWrite(led1, LOW);
#define led2off digitalWrite(led2, LOW);
#define led3off digitalWrite(led3, LOW);

//Colocamos la parte de la tarjeta encender:
#define on 9//Pin encendido
#define off 10

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

const int limiteb = 50;
const int limiten = 150;

//Colocamos para leer sensor:
int read_on;





typedef enum{
  BUSCANDO = 0,
  BUSCANDORAND,
  BUSCANDOTAC,
  ATAQUE,
  EVADIR,
  EVADIRRAND
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

void robot_buscando(){  //Tornado
    /* poner sensado aca, de todos los sensores*/

    if(adci <= limiteb || adcd <= limiteb){
      st_robot = EVADIR;
    
    }else if(adci >= limiten && adcd >= limiten){
      led1on;
      led2on;
      led3off;
      if(sharpi<400 && sharpd>400 && sidei>400 && sided>400){
        motors(150,25);
        delay(300);
        motors(0,0);
       }else{
          st_robot = ATAQUE;
       }
    }
}
void robot_busquedaAleatoria(){ //AGREGAR SENSORES DE LOS LADOS
  if(adci <= limiteb || adcd <= limiteb){
      st_robot = EVADIRRAND;
    
    }else if(adci >= limiten && adcd >= limiten){
      led1on;
      led2on;
      led3off;
      if(sharpi<400 && sharpd>400 && sidei>400 && sided>400){
          motors(100,100);
          delay(200);
          motors(0,0);
          delay(800);
       }else{
          st_robot = ATAQUE;
       }
    }

}
void robot_buscTac(){  //Tornado

    if(adci <= limiteb || adcd <= limiteb){
      st_robot = EVADIR;
    
    }else if(adci >= limiten && adcd >= limiten){
      led1on;
      led2on;
      led3off;
      if(sharpi<400 && sharpd>400 && sidei>400 && sided>400){
          motors(200,200);
          delay(10);
          motors(0,0);
       }else{
          st_robot = ATAQUE;
       }
    }
}




void robot_ataque(){
        
  
  if(adci <= limiteb || adcd <= limiteb){
    st_robot = EVADIR;
  }else if(adci >= limiten && adcd >= limiten){
    if(sharpi>400 || sharpd<400 || sidei<400 || sided<400){
      if(sharpi>400 || sharpd<400){
        led1on;
        led2on;
        led3on;
        motors(100,100);
        delay(5);
        motors(0,0);
      }
      if(sidei<400){
        led1on;
        led2off;
        led3off;
        motors(30,100);
        delay(5);
        motors(0,0);
      }
      if(sided<400){
        led1off;
        led2off;
        led3on;
        motors(100,30);
        delay(5);
        motors(0,0);
      }
      
    }else{
      st_robot = BUSCANDO;
    }
  }
  
}

void robot_evadir(){
  led1off;
  led2off;
  led3off;
  if(adcd <= limiteb && adci >= limiten){
    //Evada reversa hacia la izquierda:
    led2on;
    motors(50, -100);
    delay(200);
    motors(0,0);
  }else if(adcd >= limiten && adci <= limiteb){
    //Evada reversa hacia la derecha:
    led1on;
    motors(-100, 50);
    delay(200);
    motors(0,0);
  }else if(adcd <= limiteb && adci <= limiteb){
    //reversa
    led3on;
    motors(-100, -100);
    delay(200);
    motors(0,0);
  }else{
    led1off;
    led2off;
    led3off;
    st_robot = BUSCANDO;
    
  }
}

void robot_evadirRand(){
  led1off;
  led2off;
  led3off;
  long tiempoGiro = random(100,300);
  if(adcd <= limiteb && adci >= limiten){
    //Evada reversa hacia la izquierda:
    led2on;
    motors(50, -100);
    delay(tiempoGiro);
    motors(0,0);
  }else if(adcd >= limiten && adci <= limiteb){
    //Evada reversa hacia la derecha:
    led1on;
    motors(-100, 50);
    delay(tiempoGiro);
    motors(0,0);
  }else if(adcd <= limiteb && adci <= limiteb){
    //reversa
    led3on;
    motors(-100, -100);
    delay(tiempoGiro);
    motors(0,0);
  }else{
    led1off;
    led2off;
    led3off;
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
      if(sharpi>=400 || sharpd<400){
             // enviamos valor porcentual de 100 y lo convertimos en la funcion robot_ataque()
            robot_ataque();
        }else if(sharpi<400 && sharpd>400){
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
 variables();
}

void loop() {
    lecturaSensores();
    robot_process();
 
}
