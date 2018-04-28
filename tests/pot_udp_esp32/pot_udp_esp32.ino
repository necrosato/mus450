#include <WiFi.h>
#include <WiFiUdp.h>

#define POT_PIN 34
// this esp's ap credentials
const char AP_NAME[] = "espnode";
const char WiFiAPPSK[] = "security123";
// WiFi network name and password:
const char * networkName = "moo";
const char * networkPswd = "HCMOO_7120";

int wifiStatus;
IPAddress ip(6,6,6,1);                // this node's ap ip
IPAddress gateway(6,6,6,1);           // this node's ap default router
IPAddress subnet(255,255,255,128);    // subnet mask, this node's ap subnet addres: 6.6.6.0, broadcast: 6.6.6.127

//IP address to send UDP data to:
// either use the ip address of the server or 
// a network broadcast address
const char *udpAddress = "6.6.6.127";
IPAddress udpIPAddress(6,6,6,127);
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
// function to build up an osc packet with address and type tag
int prepare_osc_message(unsigned char *mbuf, char *address, size_t address_size, char *type_tag, size_t type_tag_size) {
    memcpy(mbuf, address, address_size);
    int message_len = address_size;
    message_len += (4-(message_len%4));
    memcpy(mbuf+message_len, type_tag, type_tag_size);
    message_len += type_tag_size;
    message_len += (4-(message_len%4));
    return message_len;
}

// function to put any 32 bit data type into the data field of an osc message
int put_osc_data_32(unsigned char *mbuf, size_t size, void *datap) {
    unsigned data = *((unsigned*)datap);
    for (int i = 0; i < 4; i++) {
        mbuf[size+(3-i)] = (unsigned char)((data>>(i*8))&0x000000FF);
    }
    return size+4;
}

void print_osc_message(unsigned char *mbuf) {
    for (int i = 0; i < OSC_LEN; i++) {
        if (mbuf[i] == 0) {
            Serial.print(" ");
        } else { Serial.print((char)mbuf[i]); }
    } Serial.println();
}

int osc_data = 0;
void loop(){
    char *osc_address = "int";
    char *osc_type_tag = ",i"; //2
    unsigned char osc_message[OSC_LEN] = {0};
    int message_len = prepare_osc_message(osc_message, osc_address, strlen(osc_address), osc_type_tag, strlen(osc_address));
    message_len = put_osc_data_32(osc_message, message_len,(void*)&osc_data);
    print_osc_message(osc_message);
    osc_data = analogRead(POT_PIN);
    
  //only send data when connected
  if(connected){
    //Send a packet
    //udp.beginPacket(udpAddress,udpPort);
    udp.beginMulticast(udpIPAddress,udpPort);
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

  WiFi.softAPConfig(ip, gateway, subnet);
  WiFi.softAP(AP_NAME, WiFiAPPSK, 6, 0);
  Serial.print("This AP's IP address is: ");
  Serial.println(WiFi.softAPIP());  

  //Initiate connection
  WiFi.begin(ssid, pwd);
  udp.begin(ip,udpPort);

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
          //udp.begin(WiFi.localIP(),udpPort);
          connected = true;
          break;
      case SYSTEM_EVENT_STA_DISCONNECTED:
          Serial.println("WiFi lost connection");
          connected = false;
          break;
    }
}
