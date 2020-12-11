/*
 * Mirakuru - HomeEasy remote socket FauxMo controller
 * by Red Skäl
 * 
 * Uses fauxmoESP by Xose Pérez.
 * https://github.com/simap/fauxmoesp
 * 
 * Button signals are taken from HE300 remote using a cheapo
 * USB logic analyser.  Only slider position 1 recorded so far.
 * 
 * fauxmoESP library requires you to use LwIP v1.4 on ESP8266
 * boards. Last I saw ESP32's were not working with this.
 */
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <fauxmoESP.h>

#define   TX_PIN    D0
#define   DELAY_HI  250     // 250
#define   DELAY_PRE 2700    // 2700
#define   DELAY_END 10300   // 10300
#define   DELAY_0   300     // 300
#define   DELAY_1   1300    // 1300

#define   WIFI_SSID   "WIFI_NAME"
#define   WIFI_PASS   "WIFI_PASS"

#define   SWITCH1     "socket1" // change these to the voice
#define   SWITCH2     "socket2" // commands you want
#define   SWITCH3     "socket3"
#define   SWITCH4     "socket4"
#define   SWITCHG     "group"

const char *btn1_on   = "\x55\x55\x96\x56\x9A\x99\x96\x55";  // 0b0101010101010101100101100101011010011010100110011001011001010101
const char *btn1_off  = "\x55\x55\x96\x56\x9A\x99\x95\x55";  // 0b0101010101010101100101100101011010011010100110011001010101010101
const char *btn2_on   = "\x55\x55\x96\x56\x9A\x99\x96\x56";  // 0b0101010101010101100101100101011010011010100110011001011001010110
const char *btn2_off  = "\x55\x55\x96\x56\x9A\x99\x95\x56";  // 0b0101010101010101100101100101011010011010100110011001010101010110
const char *btn3_on   = "\x55\x55\x96\x56\x9A\x99\x96\x59";  // 0b0101010101010101100101100101011010011010100110011001011001011001
const char *btn3_off  = "\x55\x55\x96\x56\x9A\x99\x95\x59";  // 0b0101010101010101100101100101011010011010100110011001010101011001
const char *btn4_on   = "\x55\x55\x96\x56\x9A\x99\x96\x5A";  // 0b0101010101010101100101100101011010011010100110011001011001011010
const char *btn4_off  = "\x55\x55\x96\x56\x9A\x99\x95\x5A";  // 0b0101010101010101100101100101011010011010100110011001010101011010
const char *btnG_on   = "\x55\x55\x96\x56\x9A\x99\x9A\x55";  // 0b0101010101010101100101100101011010011010100110011001101001010101
const char *btnG_off  = "\x55\x55\x96\x56\x9A\x99\x99\x55";  // 0b0101010101010101100101100101011010011010100110011001100101010101

fauxmoESP fauxmo;

// Wi-Fi Connection
void wifiSetup() {
  // Set WIFI module to STA mode
  WiFi.mode(WIFI_STA);

  // Connect
  Serial.printf("[WIFI] Connecting to %s ", WIFI_SSID);
  WiFi.begin(WIFI_SSID, WIFI_PASS);

  // Wait
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(100);
  }
  Serial.println();

  // Connected!
  Serial.printf("[WIFI] STATION Mode, SSID: %s, IP address: %s\n"
                "\t\tMac address: %s\n", WiFi.SSID().c_str(), WiFi.localIP().toString().c_str(), WiFi.macAddress().c_str());
}

// HomeEasy socket RF command transmission
void rfSend(const char *str, int reps)
{
  const char *p;
  int i, j, d;

  for (i=0; i<reps; i++) {      // transmission repeats
    p = str;

    Serial.print("\nstring: ");
    
    // send preamble
    digitalWrite(TX_PIN, HIGH);
    delayMicroseconds(DELAY_HI);
    digitalWrite(TX_PIN, LOW);
    delayMicroseconds(DELAY_PRE);

   while(*p) {
      for (j=0; j<8; j++) {
        d = ((*p >> (7- j)) & 1 ? DELAY_1 : DELAY_0);

        digitalWrite(TX_PIN, HIGH);
        delayMicroseconds(DELAY_HI);
        digitalWrite(TX_PIN, LOW);
        delayMicroseconds(d);
      }
      Serial.printf("\\x%02x", *p);
      p++;
    }

    // send end marker
    digitalWrite(TX_PIN, HIGH);
    delayMicroseconds(DELAY_HI);
    digitalWrite(TX_PIN, LOW);
    delayMicroseconds(DELAY_END);
  }

  Serial.println("\ndone\n");
}

void setup() {
  // for debugging...
  Serial.begin(9600);
  Serial.println();

  // setup up our TX pin
  pinMode(TX_PIN, OUTPUT);
  digitalWrite(TX_PIN, LOW);    // strap pin low

  // Wifi connection - borrowed from examples
  wifiSetup();

  // fauxmo setup
  fauxmo.createServer(true);
  fauxmo.setPort(80);

  // enable server
  fauxmo.enable(true);

  // add devices
  fauxmo.addDevice(SWITCH1);
  fauxmo.addDevice(SWITCH2);
  fauxmo.addDevice(SWITCH3);
  fauxmo.addDevice(SWITCH4);
  fauxmo.addDevice(SWITCHG);

  // setup callback function
  fauxmo.onSetState([](unsigned char device_id, const char * device_name, bool state, unsigned char value) {
    Serial.printf("[MAIN] Device #%d (%s) state: %s value: %d\n", device_id, device_name, state ? "ON" : "OFF", value);

    if ( (strcmp(device_name, SWITCH1) == 0) ) {
      Serial.println(" SWITCH1 toggled by Alexa");
      rfSend(state ? btn1_on : btn1_off, 5);
    } else if ( (strcmp(device_name, SWITCH2) == 0) ) {
      Serial.println(" SWITCH2 toggled by Alexa");
      rfSend(state ? btn2_on : btn2_off, 5);
    } else if ( (strcmp(device_name, SWITCH3) == 0) ) {
      Serial.println(" SWITCH3 toggled by Alexa");
      rfSend(state ? btn3_on : btn3_off, 5);
    } else if ( (strcmp(device_name, SWITCH4) == 0) ) {
      Serial.println(" SWITCH4 toggled by Alexa");
      rfSend(state ? btn4_on : btn4_off, 5);
    } else if ( (strcmp(device_name, SWITCHG) == 0) ) {
      Serial.println(" GROUP toggled by Alexa");
      rfSend(state ? btnG_on : btnG_off, 5);
    }
  });
}

void loop() {
  // setup fauxmo handler
  fauxmo.handle();
  
}
