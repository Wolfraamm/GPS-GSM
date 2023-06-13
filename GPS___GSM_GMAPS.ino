#include <SoftwareSerial.h>
#include <TinyGPS.h>

int state = 0;
const int Trig = 9; // Trigger for the SMS
float gpslat, gpslon;
char gmap[] = "https://google.com/maps/?q="; //String for gmap link

TinyGPS gps;
SoftwareSerial sgps(4, 5);
SoftwareSerial sgsm(2, 3);

void setup()
{
  sgsm.begin(9600);
  sgps.begin(9600);
}

void loop()
{
sgps.listen();
  while (sgps.available())
  {
    int c = sgps.read();
    if (gps.encode(c))
    {
      gps.f_get_position(&gpslat, &gpslon);
    }
  }
    if(digitalRead(Trig) == HIGH && state == 1) {
    Serial.println("WARNING! The driver is drunk/sleepy! Fetching location...");
    }
    
    if (digitalRead(Trig) == HIGH && state == 0) {
      Serial.println("Sending SMS...");
      sgsm.listen();
      sgsm.print("\r");
      delay(1000);
      sgsm.print("AT+CMGF=1\r");
      delay(1000);
      sgsm.print("AT+CMGS=\"+639568006501\"\r");     //*Replace XXXXXXXXXX to 10 digit mobile number & ZZ to 2 digit country code*/
      delay(1000);
      
      //The text of the message to be sent.


      sgsm.print("Location: ");
      sgsm.println(gmap);
      sgsm.print(gpslat, 6);
      sgsm.print(",");
      sgsm.print(gpslon, 6);
      
      delay(1000);
      sgsm.write(0x1A);
      delay(1000);
      state = 1;
    }
  if (digitalRead(Trig) == LOW) {
      state = 0;
    }
      delay(100);
  }
