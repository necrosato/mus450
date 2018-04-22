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
char *osc_address = "nsato/esp";
char *osc_type_tag = ",i"; //2
char osc_message[OSC_LEN] = "";
uint8_t data_count = 0;

int prepare_osc_message(char *mbuf, char *address, char *type_tag) {
    strncpy(mbuf, address, 16);
    int message_len = strlen(mbuf);
    message_len += (4-(message_len%4));
    strncpy(mbuf+message_len, type_tag, 4);
    message_len += strlen(type_tag);
    message_len += (4-(message_len%4));
    return message_len;
}

void loop(){
    int i;
    for (i = 0; i < OSC_LEN; i++) {
        osc_message[i] = 0;
    }
    int message_len = prepare_osc_message(osc_message, osc_address, osc_type_tag);
    osc_message[message_len+3] = (char) data_count;
    data_count = (data_count + 1) % 128;
    message_len+=4;
    Serial.print("Message length: ");
    Serial.println(message_len);
    
  //only send data when connected
  if(connected){
    //Send a packet
    udp.beginPacket(udpAddress,udpPort);
    udp.write((uint8_t *)osc_message, message_len);
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
