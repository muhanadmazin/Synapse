/*************************************************************************************************************************
    DEVELOPED BY:   ARTUR FERREIRA MOREIRA
    DATE:           FEBRUARY, 14 OF 2017
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

Controllium controllium("Letters");

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

  for(int i = 0; i < 6; ++i)
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

  ControlliumDevice letterA("LETTER_A_0", DEVICE_TYPE_BUTTON, "0");
  ControlliumDevice letterB("LETTER_B_0", DEVICE_TYPE_BUTTON, "0");
  ControlliumDevice letterC("LETTER_C_0", DEVICE_TYPE_BUTTON, "0");
  ControlliumDevice letterD("LETTER_D_0", DEVICE_TYPE_BUTTON, "0");

  controllium.AddDevice(letterA);
  controllium.AddDevice(letterB);
  controllium.AddDevice(letterC);
  controllium.AddDevice(letterD);
}

void loop()
{
  if(millis() - lastTime > 2000)
  {
    lastTime = millis();

    digitalWrite(D[5], LOW);
    blink(D[5]);

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

  if(controllium.Update(200))
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
  }

  //LETTERS TO PINS
  float letterAValue = (float)controllium.GetDevice(0)->GetDoubleValue();
  float letterBValue = (float)controllium.GetDevice(1)->GetDoubleValue();
  float letterCValue = (float)controllium.GetDevice(2)->GetDoubleValue();
  float letterDValue = (float)controllium.GetDevice(3)->GetDoubleValue();

  if(letterAValue >= 0)
    digitalWrite(D[0], (int)letterAValue);

  if(letterBValue >= 0)
    digitalWrite(D[1], (int)letterBValue);

  if(letterCValue >= 0)
    digitalWrite(D[2], (int)letterCValue);

  if(letterDValue >= 0)
    digitalWrite(D[3], (int)letterDValue);
}
