/*************************************************************************************************************************
    DEVELOPED BY:   ARTUR FERREIRA MOREIRA
    DATE:           MARCH, 28 OF 2017
    EMAIL:          artur31415926@gmail.com
    WEBPAGE:        http://asgardlab.netne.net/
    INSTAGRAM:      https://www.instagram.com/artur31415/
    GOOGLEPLAY:     https://play.google.com/store/apps/developer?id=Synapse
    YOUTUBE:        https://www.youtube.com/channel/UC6blOB30re0J-Oiksqaob1g/videos
**************************************************************************************************************************/

#include <Controllium.h>

#include <ESP8266WiFi.h>

const char* ssid = "YOUR_SSID";
const char* pass = "YOUR_PASS";

//ESP8266 DIGITAL PINS
int D[] = {16, 5, 4, 0, 2, 14, 12, 13, 15};

long lastTime = 0;
long lastTimeUIBlink = 0;

int LedAnimationCounter = 1;

Controllium controllium("ESP_GpiosControl");//Name of this device, if you have more than one connected, provide a different name to each

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////                                              FUNCTIONS BEGIN                                                  /////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void blink(int pin)
{
  for(int i = 0; i < 2; ++i)
  {
    digitalWrite(pin, LOW);
    delay(50);
    digitalWrite(pin, HIGH);
    delay(50);
  }

  digitalWrite(pin, LOW);
  delay(100);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////                                              FUNCTIONS END                                                    /////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void setup()
{
  Serial.begin(115200);

  for(int i = 0; i < 9; ++i)
  {
    pinMode(D[i], OUTPUT);
    digitalWrite(D[i], LOW);
  }

   // setting up Station AP
  WiFi.begin(ssid, pass);

  // Wait for connect to AP
  Serial.print("\n\nConnecting to ");
  Serial.println(ssid);
  int tries=0;
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    tries++;
    if (tries > 30)
    {
      break;
    }
  }
  Serial.println();

  if (tries > 30)
  {
    Serial.println("Failed To Connect!");
    int animationDelay = 50;
    while(1)
    {
      delay(200);

      for(int i = 0; i < 5; ++i)
      {
        digitalWrite(D[i], HIGH);
        delay(animationDelay);
        digitalWrite(D[i], LOW);
        delay(animationDelay);
      }

      delay(200);

      for(int i = 4; i >= 0; --i)
      {
        digitalWrite(D[i], HIGH);
        delay(animationDelay);
        digitalWrite(D[i], LOW);
        delay(animationDelay);
      }
    }
  }


  Serial.println("Connected!");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
  Serial.print("Udp server started at port ");
  Serial.println(UDP_DEFAULT_PORT);

  controllium.BeginUdpServer(UDP_DEFAULT_PORT);

  ControlliumDevice gpio0("Gpio 0", DEVICE_TYPE_BUTTON, "0");
  ControlliumDevice gpio1("Gpio 1", DEVICE_TYPE_BUTTON, "0");
  ControlliumDevice gpio2("Gpio 2", DEVICE_TYPE_BUTTON, "0");
  ControlliumDevice gpio3("Gpio 3", DEVICE_TYPE_BUTTON, "0");
  ControlliumDevice gpio4("Gpio 4", DEVICE_TYPE_BUTTON, "0");
  ControlliumDevice gpio5("Gpio 5", DEVICE_TYPE_BUTTON, "0");
  ControlliumDevice gpio6("Gpio 6", DEVICE_TYPE_BUTTON, "0");
  ControlliumDevice gpio7("Gpio 7", DEVICE_TYPE_BUTTON, "0");
  ControlliumDevice gpio8("Gpio 8", DEVICE_TYPE_BUTTON, "0");


  controllium.AddDevice(gpio0);
  controllium.AddDevice(gpio1);
  controllium.AddDevice(gpio2);
  controllium.AddDevice(gpio3);
  controllium.AddDevice(gpio4);
  controllium.AddDevice(gpio5);
  controllium.AddDevice(gpio6);
  controllium.AddDevice(gpio7);
  controllium.AddDevice(gpio8);
}

void loop()
{
  if(millis() - lastTime > 2000)
  {
    lastTime = millis();

    
    //blink(D[5]);

    Serial.print("ClientsCount = ");
    Serial.print(controllium.NumberOfClients());
    Serial.println("\n");

    if(controllium.NumberOfClients() > 0)
    {
      for(int i = 0; i < controllium.NumberOfClients(); ++i)
      {
        Serial.print("CLIENT ");
        Serial.print(i);
        Serial.print(">\t\tIP = {");
        Serial.print(controllium.GetClient(i)->GetIp());
        Serial.print("}\t\t\tIsTimeOut = ");
        Serial.println(controllium.GetClient(i)->IsTimeOut());
      }
    }
    Serial.println("=======================================\n");

  }

  if(controllium.Update(200))//RETURNS TRUE IF A NEW MSG WAS RECEIVED
  {
    Serial.print("DeviceCount = ");
    Serial.println(controllium.NumberOfDevices());

    for(int i = 0; i < controllium.NumberOfDevices(); ++i)
    {
      Serial.print("DEVICE ");
      Serial.print(i);
      Serial.print(":\t\t");
      Serial.print(controllium.GetDevice(i)->GetName());
      Serial.print("\t\tValue = ");
      Serial.println(controllium.GetDevice(i)->GetValueToString());
    }

    /////////////////////////////////////////////////////////////////////////////////
    //CONTROL USAGE PART
    digitalWrite(D[0], (int)controllium.GetDevice("Gpio 0")->GetDoubleValue());
    digitalWrite(D[1], (int)controllium.GetDevice("Gpio 1")->GetDoubleValue());
    digitalWrite(D[2], (int)controllium.GetDevice("Gpio 2")->GetDoubleValue());
    digitalWrite(D[3], (int)controllium.GetDevice("Gpio 3")->GetDoubleValue());
    digitalWrite(D[4], (int)controllium.GetDevice("Gpio 4")->GetDoubleValue());
    digitalWrite(D[5], (int)controllium.GetDevice("Gpio 5")->GetDoubleValue());
    digitalWrite(D[6], (int)controllium.GetDevice("Gpio 6")->GetDoubleValue());
    digitalWrite(D[7], (int)controllium.GetDevice("Gpio 7")->GetDoubleValue());
    digitalWrite(D[8], (int)controllium.GetDevice("Gpio 8")->GetDoubleValue());
  }
  
}
