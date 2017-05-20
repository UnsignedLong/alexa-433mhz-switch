#include <fauxmoESP.h>
#include <RCSwitch.h>

RCSwitch mySwitch = RCSwitch();

fauxmoESP fauxmo;

const char* ssid = "somwifiname";
const char* password = "secretpassphrase";

char* housecode = "111111";
char* socketcodes[] = {"01000", "00100", "00010", "00001"};

void setup() {

    Serial.begin(115200);
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
    }
    Serial.println("");
    Serial.print("Connected to ");
    Serial.println(ssid);
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());

    mySwitch.enableTransmit(0); //Wemos pin D3
    mySwitch.setRepeatTransmit(15);

    fauxmo.addDevice("Heizung");
    fauxmo.addDevice("Soundsystem");
    fauxmo.addDevice("Schrankbeleuchtung");
    fauxmo.addDevice("Stehlampe");
    fauxmo.onMessage([](unsigned char device_id, const char * device_name, bool state) {
      if(state){
        mySwitch.switchOn(housecode, socketcodes[(int)device_id]);
      } else {
        mySwitch.switchOff(housecode, socketcodes[(int)device_id]);
      }
      Serial.printf("[MAIN] Device #%d (%s) state: %s\n", device_id, device_name, state ? "ON" : "OFF");
    });


}

void loop() {
  fauxmo.handle();
}
