
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>



#define DHTPIN 25     // Digital pin connected to the DHT sensor 
#define DHTTYPE    DHT11     // DHT 11
DHT_Unified dht(DHTPIN, DHTTYPE);
const int trigPin = 22;
const int echoPin = 23;
long duration;
int distance;

#define rainSensorPin1 27
#define rainSensorPin2 26

#define encoder 14
unsigned int rpm;
volatile byte pulses;
unsigned long TIME;
unsigned int pulse_per_turn = 20; 

TaskHandle_t Task1;
TaskHandle_t Task2;

void count(){
  pulses++;
}

void setup() {
  Serial.begin(9600);
  dht.begin();
  sensor_t sensor;
  dht.temperature().getSensor(&sensor);
  dht.humidity().getSensor(&sensor);
  pinMode(rainSensorPin1,INPUT);
  pinMode(rainSensorPin2,INPUT);
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input

  rpm = 0;
  pulses = 0;
  TIME = 0;
  pinMode(encoder, INPUT);
  attachInterrupt(digitalPinToInterrupt(encoder), count, FALLING);

  //create a task that will be executed in the Task1code() function, with priority 1 and executed on core 0
  xTaskCreatePinnedToCore(
                    Task1code,   /* Task function. */
                    "Task1",     /* name of task. */
                    10000,       /* Stack size of task */
                    NULL,        /* parameter of the task */
                    1,           /* priority of the task */
                    &Task1,      /* Task handle to keep track of created task */
                    0);          /* pin task to core 0 */                  
  delay(500); 

  //create a task that will be executed in the Task2code() function, with priority 1 and executed on core 1
  xTaskCreatePinnedToCore(
                    Task2code,   /* Task function. */
                    "Task2",     /* name of task. */
                    10000,       /* Stack size of task */
                    NULL,        /* parameter of the task */
                    1,           /* priority of the task */
                    &Task2,      /* Task handle to keep track of created task */
                    1);          /* pin task to core 1 */
    delay(500); 

}

//Task1code: blinks an LED every 1000 ms
void Task1code( void * pvParameters ){
  Serial.print("Task1 running on core ");
  Serial.println(xPortGetCoreID());

  for(;;){
    sensors_event_t event;
    dht.temperature().getEvent(&event);
    if (isnan(event.temperature)) {
      Serial.println(F("Error reading temperature!"));
    }
    else {
      Serial.println(F("Temperatura (C): "));
      Serial.println(event.temperature);
    }
    // Get humidity event and print its value.
    dht.humidity().getEvent(&event);
    if (isnan(event.relative_humidity)) {
      Serial.println(F("Error reading humidity!"));
    }
    else {
      Serial.println(F("Humedad (%): "));
      Serial.println(event.relative_humidity);
    }
    int lluvia1 = analogRead(rainSensorPin1);
    int outputValue1 = map(lluvia1, 0, 4095, 100, 0);
    int lluvia2 = analogRead(rainSensorPin2);
    int outputValue2 = map(lluvia2, 0, 4095, 100, 0);
    Serial.println("Sensor de Lluvia 1 (%):");
    Serial.println(outputValue1);
    Serial.println("Sensor de Lluvia 2 (%):");
    Serial.println(outputValue2);
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    // Sets the trigPin on HIGH state for 10 micro seconds
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);
    // Reads the echoPin, returns the sound wave travel time in microseconds
    duration = pulseIn(echoPin, HIGH);
    // Calculating the distance
    distance = duration * 0.034 / 2;
    if(distance >500){
      distance = 0;
    }
    // Prints the distance on the Serial Monitor
    Serial.println("Distancia (cm): ");
    Serial.println(distance);
    delay(1000);
  } 
}

//Task2code: blinks an LED every 700 ms
void Task2code( void * pvParameters ){
  Serial.print("Task2 running on core ");
  Serial.println(xPortGetCoreID());

  for(;;){
    if (millis() - TIME >= 100){ // updating every 0.1 second
    detachInterrupt(digitalPinToInterrupt(encoder)); // turn off trigger
    //calcuate for rpm 
    rpm = (60 *100 / pulse_per_turn)/ (millis() - TIME) * pulses;
    TIME = millis();
    pulses = 0;
    int velocidad= (rpm*6.28)/60;
    //print output 
    Serial.println("Velocidad del Viento (cm/s): ");
    Serial.println(velocidad);
    //trigger count function everytime the encoder turns from HIGH to LOW
    attachInterrupt(digitalPinToInterrupt(encoder), count, FALLING);
    }
  }
}

void loop() {
  


}