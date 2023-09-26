// Define los estados HIGH,LOW de cada led 
#define led1on digitalWrite(led1, HIGH);
#define led2on digitalWrite(led2, HIGH);
#define led3on digitalWrite(led3, HIGH);
#define led1off digitalWrite(led1, LOW);
#define led2off digitalWrite(led2, LOW);
#define led3off digitalWrite(led3, LOW);

//Colocamos la parte de la tarjeta encender:
#define on 9//Pin encendido
#define off 

//Mejorar ataque e implementar giro.
//Revisar velocidad máxima antes de salirse del ring por inercia, sin detectar el blanco y retroceder
//Agregar Esquivada Táctica tras tiempo determinado (Calcular tiempo máximo antes de saber qué no se mueve)
//Agregar giro y choque tras determinada distancia
//Medir distancias, aplicar estrategia zapato
//Inclinación y retroceder y esquivada táctica tras detectar no inclinación. Caso retroceder: Un motor más rápido que el otro.
//Optimizar velocidad giros
//Evasión tras el otro acercarse a gran velocidad

//AGREGAR SENSORES DE LOS LADOS

//Crea una variable a cada led 
const int led1 = 13; 
const int led2 = 12; 
const int led3 = 11; 

int adcd=0; //sensor de piso derecho QTR1
int adci=0; //sensor de piso izquierdo QTR2
int sharpd = 0; //sensor proximidad derecho QTR1 A4
int sharpi = 0; //sensor proximidad izquierdo QTR1 A2
int QT1=0;
int QT2=0;

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
  BUSCANDOSLOW,
  BUSCANDOTAC,
  ATAQUE,
  EVADIR,
  EVADIRRAND,
  EVADIRDERECHA,
  EVADIRIZQUIERDA
}ST_ROBOT;

ST_ROBOT st_robot = BUSCANDO;


int motors(int speedL, int speedR){
//  digitalWrite(in2b,HIGH);  //motor derecha reversa
//  digitalWrite(in2a,LOW); //motor izquierda adelante
//  digitalWrite(in1b,LOW); //motor derecha adelante
//  digitalWrite(in1a,LOW); //motor izquierda atras
  
  if(speedL>0 && speedR<0){ // detecta sensor derecho llanta derecha
    
      digitalWrite(in1b,LOW); 
      digitalWrite(in2a,LOW); 
      digitalWrite(in1a,HIGH); 
      digitalWrite(in2b,HIGH); 
      
      analogWrite(pwm1,speedR*(-1));
      analogWrite(pwm2,speedL);
      
  }else if(speedL<0 && speedR>0){//sensor izquierdo se prende llante izquierda
     
      digitalWrite(in2a,LOW); 
      digitalWrite(in1b,LOW); 
      digitalWrite(in1a,HIGH); 
      digitalWrite(in2b,HIGH);
      analogWrite(pwm1,speedR);
      analogWrite(pwm2,speedL*(-1));
      
      
  }else if(speedL>0 && speedR>0){
      
      digitalWrite(in2b,LOW); 
      digitalWrite(in1a,LOW);
      digitalWrite(in1b,HIGH); 
      digitalWrite(in2a,HIGH); 
      analogWrite(pwm1,speedR);
      analogWrite(pwm2,speedL);
      
  }else if(speedL<0 && speedR<0){
    
      digitalWrite(in2a,LOW); 
      digitalWrite(in1b,LOW);
      digitalWrite(in1a,HIGH); 
      digitalWrite(in2b,HIGH); 
      analogWrite(pwm1,speedR*(-1));
      analogWrite(pwm2,speedL*(-1));
  }
  
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
 adcd = analogRead(1); // Lee sensor Derecho
 adci = analogRead(3); // Lee sensor Izquierdo
 sharpi = analogRead(2);
 sharpd = analogRead(0);
}

void robot_buscando(){  //Mejorar para Tornado
    /* poner sensado aca, de todos los sensores*/

    if(adcd <= limiteb || adcd <= limiteb){
      st_robot = EVADIR;
    
    }else if(adcd >= limiten && adcd >= limiten){
      if(sharpi<400 && sharpd<400){
        motors(100,40);
        //delay(300);
        motors(0,0);
       }else{
          st_robot = ATAQUE;
       }
    }
}

void busquedaAleatoria(){ //AGREGAR SENSORES DE LOS LADOS
  if(adcd <= limiteb || adcd <= limiteb){
        st_robot = EVADIRRAND;
      
      }else if(adcd >= limiten && adcd >= limiten){
        if(sharpi<400 && sharpd<400){
          motors(200,200);
          delay(10);
          motors(0,0);
        }else{
            st_robot = ATAQUE;
        }
      }

}

void busquedaLenta(){ //AGREGAR SENSORES DE LOS LADOS
  if(adcd <= limiteb || adcd <= limiteb){
        st_robot = EVADIRRAND;
      
      }else if(adcd >= limiten && adcd >= limiten){
        if(sharpi<400 && sharpd<400){
          motors(40,40);
          delay(10);
          motors(0,0);
        }else{
            st_robot = ATAQUE;
        }
      }

}


void busquedaTac(){ //AGREGAR SENSORES DE LOS LADOS
  if(adcd <= limiteb || adcd <= limiteb){
        st_robot = EVADIRRAND;
      
      }else if(adcd >= limiten && adcd >= limiten){
        if(sharpi<400 && sharpd<400){
          motors(100,100);
          delay(100);
          motors(0,0);
          delay(900);
        }else{
            st_robot = ATAQUE;
        }
      }

}

void robot_ataque(){
        
  
  if(adcd <= limiteb || adcd <= limiteb){
    st_robot = EVADIR;
  }else if(adcd >= limiten && adcd >= limiten){
    if(sharpi>400 || sharpd>400){

      //Corregimos ataque:
      if(adcd <= limiteb || adcd <= limiteb){
        st_robot = EVADIR;
      }else{
        correccionAtaque();
      }
    }else{
      st_robot = BUSCANDO;
    }
  }
  
//  if(sharpi>400 && sharpd>400){
//    if(adcd <= limiteb && adcd <= limiteb){
//      motors(255,255);
//      delay(1000);
//      motors(0,0);
//      //delay(5000);
//      //motors(0,0);  
//    }else if(adcd >= limiten || adcd >= limiten){
//      st_robot = EVADIR;
//    }
//  }else{
//    st_robot = BUSCANDO;
//  }
}

void robot_evadir(){
   //Tener en cuenta que puede llegar a colocar problemas por no tener luz en el hueco.
  int velocidadEvasion = 255;
  //<50 es color blanco, >150 es color negro
  if(adcd <= limiteb && adci >= limiten){
    //Evada reversa hacia la izquierda:
    led2on;
    motors(50, -velocidadEvasion);
    delay(200);
    motors(0,0);
  }else if(adcd >= limiten && adci <= limiteb){
    //Evada reversa hacia la derecha:
    led1on;
    motors(-velocidadEvasion, 50);
    delay(200);
    motors(0,0);
  }else if(adcd <= limiteb && adci <= limiteb){
    //reversa
    led3on;
    motors(50, -velocidadEvasion);
    delay(200);
    motors(0,0);
  }else{
    led1off;
    led2off;
    led3off;
    st_robot = BUSCANDO;
    
  }
}

void evasionAleatoria(){
  
  int velocidadEvasion = 255;
  long tiempoGiro = random(100,300);
  //<50 es color blanco, >150 es color negro
  if(adcd <= limiteb && adci >= limiten){
    //Evada reversa hacia la izquierda:
    led2on;
    motors(50, -velocidadEvasion);
    delay(tiempoGiro);
    motors(0,0);
  }else if(adcd >= limiten && adci <= limiteb){
    //Evada reversa hacia la derecha:
    led1on;
    motors(-velocidadEvasion, 50);
    delay(tiempoGiro);
    motors(0,0);
  }else if(adcd <= limiteb && adci <= limiteb){
    //reversa
    led3on;
    motors(50, -velocidadEvasion);
    delay(tiempoGiro);
    motors(0,0);
  }else{
    led1off;
    led2off;
    led3off;
    st_robot = BUSCANDORAND;
    
  }
}

void robot_process(){
    switch (st_robot)
    {
    case BUSCANDO:
        robot_buscando();
      break;
    case BUSCANDORAND:
        busquedaAleatoria();
      break;
    case BUSCANDOSLOW:
        busquedaLenta();
      break;
    case BUSCANDOTAC:
        busquedaTac();
      break;
    case ATAQUE:
      if(sharpi>=400 && sharpd>=400){
             // enviamos valor porcentual de 100 y lo convertimos en la funcion robot_ataque()
            robot_ataque();
        }else if(sharpi<400 && sharpd<400){
            st_robot = BUSCANDO;
        }
      break;
      
    case EVADIR:
        robot_evadir();
      break;
    case EVADIRRAND:
        evasionAleatoria();
      break;
      
    default: 
        st_robot = BUSCANDO;
      break;
    }
}


void correccionAtaque(){

  int diferencia = sharpi - sharpd;
  int diferenciaDeRef = 325; //Colocamos cuanto queremos que sea la diferencia mínima:
  int retrasoCorreccion = 200;

  //Colocamos las velocidades que queremos para nuestros motores:
  int velocidadDerechaMax = 150;
  int velocidadIzquierdaMax = 150;

  int velocidadDerechaCorreccion = 50;
  int velocidadIzquierdaCorreccion = 50;
  
  if(adcd <= limiteb || adcd <= limiteb){
    st_robot = EVADIR;
  }else if(diferencia < -diferenciaDeRef){

    //El motor izquierdo debe moverse más rápido
    //Movemos el motor:
    motors(velocidadIzquierdaCorreccion,velocidadDerechaMax );
    delay(retrasoCorreccion);
    motors(0,0);
  
  }else if(diferencia > diferenciaDeRef){

    //El motor derecho debe moverse más rápido
    //Movemos el motor:
    motors(velocidadIzquierdaMax, velocidadDerechaCorreccion);
    delay(retrasoCorreccion);
    motors(0,0);
    
  }else{
      motors(255,255);
      //delay(1000);
      motors(0,0);
  }
}





void setup() {

 //Para encender el robot:
 pinMode(on, INPUT);
 pinMode(off, INPUT);
 //configuracion de condiciones iniciales
 variables();
}

void loop() {
  
  //if(digitalRead(on) == 1){
    digitalWrite(led1,HIGH);
    lecturaSensores();
    robot_process();
    digitalWrite(led1,LOW);
    digitalWrite(on,LOW);
    //motors(10,10);
 
}