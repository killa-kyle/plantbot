// include the library code:
#include <LiquidCrystal.h>
#include <dht.h>

dht DHT;

#define DHT11_PIN 7

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);


// this constant won't change:
const int  buttonPin = 8;    // the pin that the pushbutton is attached to
const int ledPin = 13;       // the pin that the LED is attached to

// Variables will change:
int buttonPushCounter = 0;   // counter for the number of button presses
int buttonState = 0;         // current state of the button
int lastButtonState = 0;     // previous state of the button
int menuPage = 0;

// sensor 
const int AirValue = 595;   //you need to replace this value with Value_1
const int WaterValue = 320;  //you need to replace this value with Value_2
int intervals = (AirValue - WaterValue)/3;
int soilMoistureValue = 0;


void setup() {
  // initialize the button pin as a input:
  pinMode(buttonPin, INPUT);
  // initialize the LED as an output:
  pinMode(ledPin, OUTPUT);
  // initialize serial communication:
  Serial.begin(9600);

  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  // Print a message to the LCD.
  lcd.print("JOYBOT v1.0");
  lcd.setCursor(0,1);
  lcd.print("IoT plant system");
}


void loop() {
  handleButton();
  
  
}

void handleButton(){
    // read the pushbutton input pin:
  buttonState = digitalRead(buttonPin);

  // compare the buttonState to its previous state
  if (buttonState != lastButtonState) {
    // if the state has changed, increment the counter
    if (buttonState == HIGH) {
      // if the current state is HIGH then the button went from off to on:
      buttonPushCounter++;
      menuPage++;
      
      if (menuPage > 3 ){        
        menuPage = 1;
      }
      handlePages();

    } else {
      // if the current state is LOW then the button went from on to off:
//      Serial.println("off");
    }
    // Delay a little bit to avoid bouncing
    delay(50);
  }
  // save the current state as the last state, for next time through the loop
  lastButtonState = buttonState;
}

void handlePages() {
      switch (menuPage)
    {
        case 1:
          soilMoistureCheck();
          break;
        case 2:
          soilLastChecked();
          break;
        case 3:
          tempHumidityCheck();
          break;
    }
    handleLogger();
}

void tempHumidityCheck() {
  int chk = DHT.read11(DHT11_PIN);
  int tempF = DHT.temperature * 9 / 5 + 32;
  lcd.clear();
  lcd.setCursor(0,0); 
  lcd.print("Temp: ");
  lcd.print(tempF);
  lcd.print((char)223);
  lcd.print("F");
  lcd.setCursor(0,1);
  lcd.print("Humidity: ");
  lcd.print(DHT.humidity);
  lcd.print("%");
  delay(500);
}

void soilLastChecked() {
    int val;
  lcd.clear();
  lcd.setCursor(0,0); 

  val = analogRead(0); //connect sensor to Analog 0
//  Serial.println(val); //print the value to serial port
  lcd.print("Water: ");
  lcd.print(val); // TODO: get reading and display value
  lcd.setCursor(0,1);
  lcd.print("Last: 09/01/19");
  delay(500);
  }

void soilMoistureCheck(){
  lcd.clear();
  lcd.setCursor(0,0); 
  lcd.print("Water Status:");
  
  soilMoistureValue = analogRead(A0);  //put Sensor insert into soil
 
  if(soilMoistureValue > WaterValue && soilMoistureValue < (WaterValue + intervals))
  {
    Serial.println("Very Wet");
    Serial.println("                 ");
    lcd.setCursor(0,1);
    lcd.print("Very Wet: ");
    lcd.print(soilMoistureValue);
    
  }
  else if(soilMoistureValue > (WaterValue + intervals) && soilMoistureValue < (AirValue - intervals))
  {
    Serial.println("Wet");
    Serial.println("                 ");
    lcd.setCursor(0,1);
    lcd.print("Wet: ");
    lcd.print(soilMoistureValue);
  }
  else if(soilMoistureValue < AirValue && soilMoistureValue > (AirValue - intervals))
  {
    
    Serial.println("Dry");
    Serial.println("                 ");
    lcd.setCursor(0,1);
    lcd.print("Dry: ");
    lcd.print(soilMoistureValue);
  } else {
    Serial.println("!!!!!!!!!!!!!!!!!!");
    Serial.print("Bad moisture value - ");
    Serial.print(soilMoistureValue);
    Serial.println("                   ");
    
  }
   delay(500);
}

void handleLogger(){
  Serial.println("========================");
  Serial.print("soilMoistureValue: ");
  Serial.print(soilMoistureValue);
  Serial.println("                 ");
  Serial.print("AirValue: ");
  Serial.print(AirValue);
  Serial.println("                 ");
  Serial.print("WaterValue: ");
  Serial.print(WaterValue);
  Serial.println("                 ");
  Serial.print("intervals: ");
  Serial.print(intervals);
  Serial.println("                 ");

  
//  WaterValue
//  intervals
  
  }
