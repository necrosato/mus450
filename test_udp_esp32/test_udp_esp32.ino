/*
 *  This sketch sends random data over UDP on a ESP32 device
  *
 */
#include <WiFi.h>
#include <WiFiUdp.h>

// WiFi network name and password:
const char * networkName = "moo";
const char * networkPswd = "HCMOO_7120";

//IP address to send UDP data to:
// either use the ip address of the server or 
// a network broadcast address
const char * udpAddress = "10.0.0.92";
const int udpPort = 6666;

//Are we currently connected?
boolean connected = false; 
//The udp library class
WiFiUDP udp;

void setup(){
  // Initilize hardware serial:
  Serial.begin(115200);
  
  //Connect to the WiFi network
  connectToWiFi(networkName, networkPswd);
}

#define OSC_LEN 32
char *osc_address = "/osc/esp"; //8
char *osc_type_tag = ",i"; //2
char *osc_data = "123"; //2
char osc_message[OSC_LEN] = "";

void loop(){
    int i;
    for (i = 0; i < OSC_LEN; i++) {
        osc_message[i] = 0;
    }
    strcpy(osc_message, osc_address);
    int message_len = strlen(osc_message);
    message_len += ((message_len%4)+4);
    strcpy(osc_message+message_len, osc_type_tag);
    message_len += strlen(osc_type_tag);
    message_len += ((message_len%4)+4);
    strcpy(osc_message+message_len, osc_data);
    message_len += strlen(osc_data);
    message_len += ((message_len%4)+4);
    Serial.print("Message length: ");
    Serial.print(message_len);
    Serial.print("Message: ");
    for (i = 0; i < message_len; i++) {
        Serial.print(osc_message[i]);
    }
    Serial.println("");
    
    
    
  //only send data when connected
  if(connected){
    //Send a packet
    udp.beginPacket(udpAddress,udpPort);
    udp.write((uint_8 *)osc_message, message_len);
    udp.endPacket();
  }
  //Wait for 1 second
  delay(1000);
}

void connectToWiFi(const char * ssid, const char * pwd){
  Serial.println("Connecting to WiFi network: " + String(ssid));

  // delete old config
  WiFi.disconnect(true);
  //register event handler
  WiFi.onEvent(WiFiEvent);
  
  //Initiate connection
  WiFi.begin(ssid, pwd);

  Serial.println("Waiting for WIFI connection...");
}

//wifi event handler
void WiFiEvent(WiFiEvent_t event){
    switch(event) {
      case SYSTEM_EVENT_STA_GOT_IP:
          //When connected set 
          Serial.print("WiFi connected! IP address: ");
          Serial.println(WiFi.localIP());  
          //initializes the UDP state
          //This initializes the transfer buffer
          udp.begin(WiFi.localIP(),udpPort);
          connected = true;
          break;
      case SYSTEM_EVENT_STA_DISCONNECTED:
          Serial.println("WiFi lost connection");
          connected = false;
          break;
    }
}
