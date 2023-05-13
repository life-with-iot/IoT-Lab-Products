//R&D LABS 0.10.168.104
#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>
const char* ssid = "R&D LABS";
const char* pwd = "";
String get_host = "http://10.10.110.2/labswitches/";
String voltage, current, postData;
WiFiServer server(80);
#define Relay1 D1
#define Relay2 D2
#define Relay3 D3
#define Relay4 D4
#define Relay5 D5
#define Relay6 D6
#define Relay7 D7
#define Relay8 D8

/////////////////// current sensor
#define VIN A0 // define the Arduino pin A0 as voltage input (V in)
const float VCC   = 5.0;// supply voltage is from 4.5 to 5.5V. Normally 5V.
const int model = 2;   // enter the model number (see below)

//float cutOffLimit = 1.01;// set the current which below that value, doesn't matter. Or set 0.5
float cutOffLimit = 0.01;// set the current which below that value, doesn't matter. Or set 0.5

/*
          "ACS712ELCTR-05B-T",// for model use 0
          "ACS712ELCTR-20A-T",// for model use 1
          "ACS712ELCTR-30A-T"// for model use 2
  sensitivity array is holding the sensitivy of the  ACS712
  current sensors. Do not change. All values are from page 5  of data sheet
*/
float sensitivity[] = {
  0.185,// for ACS712ELCTR-05B-T
  0.100,// for ACS712ELCTR-20A-T
  0.066// for ACS712ELCTR-30A-T

};


const float QOV =   0.5 * VCC;// set quiescent Output voltage of 0.5V
float voltage1;// internal variable for voltage
////////////////////////

void setup() {
  pinMode(Relay1, OUTPUT);
  pinMode(Relay2, OUTPUT);
  pinMode(Relay3, OUTPUT);
  pinMode(Relay4, OUTPUT);
  pinMode(Relay5, OUTPUT);
  pinMode(Relay6, OUTPUT);
  pinMode(Relay7, OUTPUT);
  pinMode(Relay8, OUTPUT);
  delay(500);
  Serial.begin(115200);
  WiFi.mode(WIFI_OFF);        //Prevents reconnection issue (taking too long to connect)
  WiFi.mode(WIFI_STA);        //This line hides the viewing of ESP as wifi hotspot

  WiFi.begin(ssid, pwd);    //Connect to your WiFi router
  //  WiFi.begin(ssid, password);     //Connect to your WiFi router
  Serial.println("");

  Serial.print("Connecting");
  // Wait for connection
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(400);
    Serial.print(".");
  }

  //If connection successful show IP address in serial monitor
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());  //IP address assigned to your ESP
}


void loop()
{

  WiFiClient client = server.available();
  HTTPClient http;
  int i = 1;
  String y = String(i);

  String url1 = get_host + "value.php?id=1";
  http.begin(url1);
  int httpCode1 = http.GET();
  String payload1 = http.getString(); 
  Serial.println(payload1);
  if ((payload1 == "ON") || (payload1 == "")) {
    digitalWrite(Relay1, LOW);
    for (i = 2; i <= 8; i++) {
      String url = get_host + "value.php?id=" + i;
      http.begin(url);
      int httpCode = http.GET();
      String payload = http.getString();
      if ((i == 2) && (payload == "ON")) {
        digitalWrite(Relay2, LOW);
      }
      if ((i == 2) && (payload == "OFF")) {
        digitalWrite(Relay2, HIGH);
      }
      if ((i == 3) && (payload == "ON")) {
        digitalWrite(Relay3, LOW);
      }
      if ((i == 3) && (payload == "OFF")) {
        digitalWrite(Relay3, HIGH);
      }
      if ((i == 4) && (payload == "ON")) {
        digitalWrite(Relay4, LOW);
      }
      if ((i == 4) && (payload == "OFF")) {
        digitalWrite(Relay4, HIGH);
      }
      if ((i == 5) && (payload == "ON")) {
        digitalWrite(Relay5, LOW);
      }
      if ((i == 5) && (payload == "OFF")) {
        digitalWrite(Relay5, HIGH);
      }
      if ((i == 6) && (payload == "ON")) {
        digitalWrite(Relay6, LOW);
      }
      if ((i == 6) && (payload == "OFF")) {
        digitalWrite(Relay6, HIGH);
      }
      if ((i == 7) && (payload == "ON")) {
        digitalWrite(Relay7, LOW);
      }
      if ((i == 7) && (payload == "OFF")) {
        digitalWrite(Relay7, HIGH);
      }
      if ((i == 8) && (payload == "ON")) {
        digitalWrite(Relay8, LOW);
      }
      if ((i == 8) && (payload == "OFF")) {
        digitalWrite(Relay8, HIGH);
      }
    }
  }
  else
  {
    digitalWrite(Relay1, HIGH);
    digitalWrite(Relay2, HIGH);
    digitalWrite(Relay3, HIGH);
    digitalWrite(Relay4, HIGH);
    digitalWrite(Relay5, HIGH);
    digitalWrite(Relay6, HIGH);
    digitalWrite(Relay7, HIGH);
    digitalWrite(Relay8, HIGH);

  }

  http.end();

  /////////////////// current sensor
  //Robojax.com ACS712 Current Sensor
  float voltage_raw =   (5.0 / 1023.0) * analogRead(VIN); // Read the voltage from sensor
  //  voltage =  voltage_raw - QOV + 0.012 ;// 0.000 is a value to make voltage zero when there is no current
  voltage1 =  voltage_raw - QOV - 0.12; // 0.000 is a value to make voltage zero when there is no current
  float current1 = voltage1 / sensitivity[model];

  //  int randnum1 = random(200, 230);
  //  voltage = (String)randnum1;
  //  int randnum2 = random(1, 10);
  //  current = (String)randnum2;

  voltage = (String)voltage1;
  current = (String)current1;

  postData = "&voltage=" + voltage + "&current=" + current ;

  http.begin("http://10.10.110.2/labswitches/postdemo.php");              //Specify request destination
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");    //Specify content-type header

  int httpCode = http.POST(postData);   //Send the request
  String payload = http.getString();    //Get the response payload

  Serial.println(httpCode);   //Print HTTP return code
  Serial.println(payload);    //Print request response payload

  http.end();
}
