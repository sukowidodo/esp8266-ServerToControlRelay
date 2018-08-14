#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

ESP8266WebServer server(80);
uint8_t relay[3]= {D1,D2,D3};

void handleGenericArgs() { //Handler
                           //Add a new line
String message="{";
//int last = (server.args()-1);
//for (int i = 0; i < server.args(); i++) {
  
  if(server.arg(0)==(String)relay[0] && server.arg(1)=="0"){
    digitalWrite (relay[0], HIGH );
    Serial.print((String)relay[0]+" high");
  }else if(server.arg(0)==(String)relay[0] && server.arg(1)=="1"){
    digitalWrite (relay[0], LOW );
    Serial.print((String)relay[0]+" low");
  }
  else if(server.arg(0)==(String)relay[1] && server.arg(1)=="0"){
    digitalWrite (relay[1], HIGH );
    Serial.print((String)relay[0]+" high");
  }else if(server.arg(0)==(String)relay[1] && server.arg(1)=="1"){
    digitalWrite (relay[1], LOW );
    Serial.print((String)relay[1]+" low");
  }

  message += "\"name\" :\"" + server.arg(0) + " \",";   
  message += "\"status\":"+server.arg(1) + "";             
//  if(i!=last){
 //   message +=",";
 // }
//} 
message+="}";

server.send(200, "application/json", message);       //Response to the HTTP request

}

void HandleAuto()
{
  if(server.arg(0)=="off" && server.arg(1)==imei){
    digitalWrite (relay[2], HIGH);
    Serial.print((String)relay[2]+" HIGH");
  }else if(server.arg(0)=="on" && server.arg(1)==imei){
    digitalWrite (relay[2], LOW);
    Serial.print((String)relay[2]+" low");
  }
  String message = "{";
  message += "\"imei\" :\"" + server.arg(1) + " \",";   
  message += "\"status\":\""+server.arg(0) + "\""; 
  message +="}";
  server.send(200, "application/json", message);       //Response to the HTTP request
}


void setupRelayIdiot(){
   for(int i=0;i<sizeof(relay);i++){
      pinMode(relay[i],OUTPUT);
      digitalWrite(relay[i],HIGH);
    }
}


void setup()
{
Serial.begin(115200);
Serial.println();

//disconnect any previous connections
WiFi.disconnect();
delay(1000);

//scan for number of nearby networks & print SSIDs

Serial.print("Nearby networks found  :");
Serial.println(WiFi.scanNetworks());
delay(500);

Serial.println("List of surrounding Network SSIDs…:");
int n = WiFi.scanNetworks();
for (int i = 0; i < n; i++)
{
  Serial.println(WiFi.SSID(i));
}
Serial.println();

//connect to preferred SSID

WiFi.begin("sukowi", "takonsopo");

Serial.println("Connecting");
while (WiFi.status() != WL_CONNECTED)
{
  delay(500);
  Serial.print(".");
}
Serial.println();

// print SSID name,local IP allotted ,MAC address & signal strength

Serial.println();
Serial.print("Connected to SSID          : ");
Serial.println(WiFi.SSID());

Serial.print("IP address allotted to ESP : ");
Serial.println(WiFi.localIP());

Serial.print("MAC Address of ESP         : ");
Serial.println(WiFi.macAddress());

Serial.print("Signal strength is         : ");
Serial.println(WiFi.RSSI());
setupRelayIdiot();
server.on("/args", handleGenericArgs); //Associate the handler function to the path
server.on("/motor", HandleAuto); 
  server.on ( "/inline", []() {
    server.send ( 200, "text/html", "this works as well" );
  } );
//  server.onNotFound ( handleNotFound );
  
  server.begin();
  Serial.println("HTTP server started");

}

void loop()
{
  server.handleClient();
}


