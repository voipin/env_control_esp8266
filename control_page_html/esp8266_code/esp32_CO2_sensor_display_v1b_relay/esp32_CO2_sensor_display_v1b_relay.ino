#include <Wire.h>
#include <U8g2lib.h>
#include <stdio.h>
#include "Adafruit_SGP30.h"
#include "FS.h"
#include "SPIFFS.h"
#include <WiFi.h>
#include <WebServer.h>
#include "Vector.h"




WebServer server(80);

#include "DHT.h"

#define DHTTYPE DHT22

// Set WiFi credentials
#define WIFI_SSID "KidsWifi"
#define WIFI_PASS "xxyy123456"

// Set WiFi credentials
#define AP_SSID "mush-room"
#define AP_PASS "8675309"


Adafruit_SGP30 sgp;

U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, U8X8_PIN_NONE);

const char* ssid = "mush-room";
const char* pass = "86753099";

const char* wifi_ssid = "KidsWifi";
const char* wifi_pass = "xxyy123456";


// const char COPYRIGHT_SYMBOL[] = { 0xa9, '\0' };
void u8g2_prepare() {
u8g2.setFont(u8g2_font_helvB18_tf); u8g2.setFontRefHeightExtendedText(); u8g2.setDrawColor(1); u8g2.setFontPosTop(); u8g2.setFontDirection(0);
}

char *menuDisplay[] = {"one", "two", "three"};
char *charSet[]= {"A", "B", "C"};

//char *menuDisplay[] = {};

/* return absolute humidity [mg/m^3] with approximation formula
* @param temperature [°C]
* @param humidity [%RH]
*/
uint8_t DHTPin = 7; 
DHT dht(DHTPin, DHTTYPE); 
//const int Push_button_1 = 13;

uint8_t Push_button_2 = 5;
uint8_t Push_button_1 = 6;
uint8_t Push_button_3 = 7;

uint8_t Relay1 = 8;


int loopTime = 1000;
bool displayed = 0;

int menu_state = 0;
int menuSize = 0 ;
int charCount = 65;
int totalChars = 0;

String state_menu = "sensor_data";
String stateMenu = "sensor_data";

char wifi_sid[10];
String wifi_pw = "";
char wifi_sid_add[0];

float Temperature;
float Humidity;

int alert_temp = 85;
int alert_co2 = 600;
int alert_hum = 60;

int relay_temp = 85;
int relay_co2 = 600;
int relay_hum = 50;

bool relay_hum_toggle = 0;
bool relay_fan_toggle = 0;

String sendHTML1() {

String ptr ="<!doctype html>\n";
ptr +="<html lang=\"en\">\n";
ptr +="<head>\n";
ptr +="</head>\n";
ptr +="<body>\n";
ptr +="HI\n";
ptr +="</body>\n";
ptr +="</html>\n";



}


String sendHTML(String form_relay_toggle_fan) {

String ptr ="<!doctype html>\n";
ptr +="<html lang=\"en\">\n";
ptr +="<head>\n";
ptr +="    <meta charset=\"utf-8\">\n";
ptr +="    <meta name=\"viewport\" content=\"width=device-width, initial-scale=1, shrink-to-fit=no\">\n";
ptr +="    <meta name=\"description\" content=\"\">\n";
ptr +="    <meta name=\"author\" content=\"\">\n";
ptr +="    <link rel=\"icon\" href=\"/docs/4.0/assets/img/favicons/favicon.ico\">\n";
ptr +="    <title>Pricing example for Bootstrap</title>\n";
ptr +="    \n";
ptr +="    <!-- Bootstrap core CSS -->\n";
ptr +="    <link href=\"bootstrap.min.css\" rel=\"stylesheet\">\n";
ptr +="    <!-- Custom styles for this template -->\n";
ptr +="    <link href=\"pricing.css\" rel=\"stylesheet\">\n";
ptr +="</head>\n";
ptr +="<body>\n";
ptr +="<style>\n";
ptr +="    table {\n";
ptr +="        width:100%;\n";
ptr +="    }\n";
ptr +="    li {\n";
ptr +="        text-align: left;\n";
ptr +="        margin: 5px 0;\n";
ptr +="    }\n";
ptr +="    .li-more-margin {\n";
ptr +="        margin: 10px 0;\n";
ptr +="    }\n";
ptr +="    .relay-text{\n";
ptr +="        text-align: left;\n";
ptr +="        width:75%;\n";
ptr +="    }\n";
ptr +="    .relay-input{\n";
ptr +="        width:25%;\n";
ptr +="    }\n";
ptr +="    .btn {\n";
ptr +="        margin-top: 30px;\n";
ptr +="    }\n";
ptr +="    .temp-input{\n";
ptr +="        width:25%;\n";
ptr +="    }\n";
ptr +="    .card-body {\n";
ptr +="        align-content: center;\n";
ptr +="    }\n";
ptr +="    input {\n";
ptr +="        height: 30px;\n";
ptr +="        flex: 0 0 200px;\n";
ptr +="        margin-left: 10px;\n";
ptr +="    }\n";
ptr +="</style>\n";
ptr +="<div class=\"form-group\">\n";
ptr +="<form class=\"form-group\" action=\"/submit_page\">\n";
ptr +="<div class=\"pricing-header px-3 py-3 pt-md-5 pb-md-4 mx-auto text-center\">\n";
ptr +="    <h1 class=\"display-4\">Control</h1>\n";
ptr +="    <p class=\"lead\">Control all systems</p>\n";
ptr +="</div>\n";
ptr +="<div class=\"container\">\n";
ptr +="    <div class=\"card-deck text-center\">\n";
ptr +="        <div class=\"card box-shadow mx-auto\" style=\"width: 18rem;\">\n";
ptr +="            <div class=\"card-header\">\n";
ptr +="                <h4 class=\"my-0 font-weight-normal\">Readings</h4>\n";
ptr +="            </div>\n";
ptr +="            <div class=\"card-body mx-auto\">\n";
ptr +="                <ul class=\"list-inline\" >\n";
ptr +="                    <li>Temp 80</li>\n";
ptr +="                    <li>Co2 600 ppm</li>\n";
ptr +="                    <li>Humidity 45%</li>\n";
ptr +="                </ul>\n";
ptr +="            </div>\n";
ptr +="        </div>\n";
ptr +="        <div class=\"card box-shadow mx-auto\" style=\"width: 18rem;\" >\n";
ptr +="            <div class=\"card-header\">\n";
ptr +="                <h4 class=\"my-0 font-weight-normal\">Wifi</h4>\n";
ptr +="            </div>\n";
ptr +="            <div class=\"card-body mx-auto\"\">\n";
ptr +="                <ul class=\"list-unstyled\">\n";
ptr +="                    <li class=\"li-more-margin\">Enable Local Network : <input name=\"cl_active\" type=\"checkbox\"/></li>\n";
ptr +="                    <li>SSID <input name=\"cl_ssid\" size=\"10\"/></li>\n";
ptr +="                    <li>Pass <input name=\"cl_pass\" size=\"10\"/></li>\n";
ptr +="                </ul>\n";
ptr +="                <ul class=\"list-unstyled\">\n";
ptr +="                    <li class=\"li-more-margin\">Enable AP Network : <input class=\"c\" name=\"ap_active\" type=\"checkbox\"/></li>\n";
ptr +="                    <li>SSID <input name=\"ap_ssid\" size=\"10\" value=\"mush-room\"/></li>\n";
ptr +="                    <li>Pass <input name=\"ap_pass\" size=\"10\" value=\"86753099\"/></li>\n";
ptr +="                </ul>\n";
ptr +="            </div>\n";
ptr +="            <button type=\"submit\" class=\"btn btn-lg btn-block btn-primary\">Save</button>\n";
ptr +="        </div>\n";
ptr +="        <div class=\"card box-shadow mx-auto\" style=\"width: 18rem;\">\n";
ptr +="            <div class=\"card-header\">\n";
ptr +="                <h4 class=\"my-0 font-weight-normal\">Relay Limit</h4>\n";
ptr +="            </div>\n";
ptr +="            <div class=\"row\">\n";
ptr +="            <div class=\"card-body mx-auto\">\n";

ptr +="                <ul class=\"list-unstyled\">\n";

ptr +="                    <li class=\"list-group-item d-flex justify-content-between align-items-center\">\n";
ptr +="                    <table>\n";
ptr +="                        <tr>\n";
ptr +="                            <td class=\"relay-text\">Temp</td>\n";
ptr +="                            <td class=\"temp-input\" ><input name=\"relay_limit_temp\" size=\"3\" value=\"80\"/> </td>\n";
ptr +="                        </tr>\n";
ptr +="                    </table>\n";
ptr +="                    </li>\n";
ptr +="                    <li class=\"list-group-item d-flex justify-content-between align-items-center\">\n";
ptr +="                        <table>\n";
ptr +="                            <tr>\n";
ptr +="                                <td class=\"relay-text\"> Co2 </td>\n";
ptr +="                                <td class=\"relay-input\"> <input size=\"3\" name=\"relay_limit_co2\" value=\"600\"/> </td>\n";
ptr +="                            </tr>\n";
ptr +="                        </table>\n";
ptr +="                    </li>\n";
ptr +="                    <li class=\"list-group-item d-flex justify-content-between align-items-center\">\n";
ptr +="                        <table>\n";
ptr +="                            <tr>\n";
ptr +="                                <td class=\"relay-text\"> Humidity </td>\n";
ptr +="                                <td class=\"relay-input\"> <input name=\"relay_limit_hum\" size=\"3\" value=\"45\"/> </td>\n";
ptr +="                            </tr>\n";
ptr +="                        </table>\n";
ptr +="                    </li>\n";



ptr +="                </ul>\n";

ptr +="            </div>\n";

ptr +="        </div>\n";
ptr +="            <button type=\"submit\" class=\"btn btn-lg btn-block btn-primary\">Save</button>\n";
ptr +="        </div>\n";

ptr +="        <div class=\"card box-shadow mx-auto\" style=\"width: 18rem;\">\n";
ptr +="            <div class=\"card-header\">\n";
ptr +="                <h4 class=\"my-0 font-weight-normal\">Relay Toggle</h4>\n";
ptr +="            </div>\n";
ptr +="            <div class=\"row\">\n";




ptr +="                <div class=\"card-body mx-auto\">\n";


ptr +="                    <ul class=\"list-group\">\n";
ptr +="                        <li class=\"list-group-item d-flex justify-content-between align-items-center\">\n";
ptr +="                            <table>\n";
ptr +="                                <tr>\n";
ptr +="                                    <td class=\"relay-text\">Fan</td>\n";
if (form_relay_toggle_fan == "on"){
  ptr +="                                    <td class=\"relay-input\"><input value=\"on\" name=\"relay_toggle_fan\" type=\"checkbox\" checked/></td>\n";

  }else
  {
    ptr +="                                    <td class=\"relay-input\"> <input name=\"relay_toggle_fan\" type=\"checkbox\" /></td>\n";
    }

ptr +="                                </tr>\n";

ptr +="                            </table>\n";
ptr +="                            <span class=\"badge badge-primary badge-pill\">14</span>\n";
ptr +="                        </li>\n";
ptr +="                        <li class=\"list-group-item d-flex justify-content-between align-items-center\">\n";

ptr +="                            <table>\n";
ptr +="                                <tr>\n";
ptr +="                                    <td class=\"relay-text\" >Humidity </td>\n";
ptr +="                                    <td class=\"relay-input\"> <input name=\"relay_toggle_hum\" type=\"checkbox\" /> </td>\n";
ptr +="                                </tr>\n";

ptr +="                            </table>\n";

ptr +="                            <span class=\"badge badge-primary badge-pill\">2</span>\n";
ptr +="                        </li>\n";

ptr +="                    </ul>\n";

ptr +="                </div>\n";

ptr +="            </div>\n";
ptr +="            <button type=\"submit\" class=\"btn btn-lg btn-block btn-primary\">Save</button>\n";
ptr +="        </div>\n";
ptr +="</form>\n";
ptr +="    <footer class=\"pt-4 my-md-5 pt-md-5 border-top\">\n";
ptr +="        <div class=\"row\">\n";
ptr +="            <div class=\"col-12 col-md\">\n";

ptr +="            </div>\n";
ptr +="            <div class=\"col-6 col-md\">\n";

ptr +="            </div>\n";
ptr +="            <div class=\"col-6 col-md\">\n";

ptr +="            </div>\n";
ptr +="            <div class=\"col-6 col-md\">\n";

ptr +="            </div>\n";
ptr +="        </div>\n";
ptr +="    </footer>\n";
ptr +="</div>\n";



ptr +="<!-- Bootstrap core JavaScript\n";
ptr +="================================================== -->\n";
ptr +="<!-- Placed at the end of the document so the pages load faster -->\n";
ptr +="<script src=\"jquery-slim.min.js\"><\/script>\n";
ptr +="<script src=\"popper.min.js\"></script>\n";
ptr +="<script src=\"bootstrap.min.js\"></script>\n";
ptr +="<script src=\"holder.min.js\"></script>\n";
ptr +="<script>\n";
ptr +="    Holder.addTheme('thumb', {\n";
ptr +="        bg: '#55595c',\n";
ptr +="        fg: '#eceeef',\n";
ptr +="        text: 'Thumbnail'\n";
ptr +="    });\n";
ptr +="</script>\n";
ptr +="</body>\n";
ptr +="</html>\n";

return ptr;

}



uint32_t getAbsoluteHumidity(float temperature, float humidity) {
    // approximation formula from Sensirion SGP30 Driver Integration chapter 3.15
    const float absoluteHumidity = 216.7f * ((humidity / 100.0f) * 6.112f * exp((17.62f * temperature) / (243.12f + temperature)) / (273.15f + temperature)); // [g/m^3]
    const uint32_t absoluteHumidityScaled = static_cast<uint32_t>(1000.0f * absoluteHumidity); // [mg/m^3]
    return absoluteHumidityScaled;
};


int menuState(int button, int button2, int button3){

char relay_temp_s[3];
char buff[4];
char *buff1;
char asciiChar[0];

// menu state 0 Display data
// state_menu main_menu
// setup menu 
// 1.env
// 2.wifi
// 3.exit

bool exitBool = (stateMenu=="env_menu" ||stateMenu == "env_alert_1" | stateMenu == "env_alert_temp" ||
 stateMenu == "env_alert_2" ||  stateMenu == "env_alert_co2" || stateMenu == "env_alert_hum" || stateMenu == "wifi"
 || stateMenu == "main_menu" || stateMenu == "relay" || stateMenu == "relay_toggle" || stateMenu == "relay_temp" ||
 stateMenu == "relay_limit" || stateMenu == "relay_co2" || stateMenu == "relay_hum" || stateMenu == "relay_toggle_fan" ||
 stateMenu == "relay_toggle_hum");

//( menu_state == 1 || menu_state == 2 || menu_state ==4 )
//provide exit back to sensor_data
if (button==HIGH){
  //  menuState(Push_button_2);
  //menuState(Push_button_2);
   Serial.println("button HIGH");
   }
  if (button2==HIGH){
  //  menuState(Push_button_2);
  //menuState(Push_button_2);
   Serial.println("button2 HIGH");
   }
  if (button3==HIGH){
  //  menuState(Push_button_3);
  //menuState(Push_button_3);
  Serial.println("button3 HIGH");
   }




if (  exitBool && button3 == HIGH ){
      //exit
      
      stateMenu = "sensor_data";
      displayed = 0;
      loopTime = 500;
      return 0;
    }


  
// go from sensor data to main_menu
if (stateMenu == "sensor_data" && button2 == HIGH ){
      
      stateMenu = "main_menu";
      displayed = 0;
      loopTime = 200;
      return 0;
    
    }


//go from main_menu to env_main_menu
if (stateMenu == "main_menu" && button == HIGH){
  //env menu base
      menu_state = 2;
      stateMenu = "env_menu";
      displayed = 0;
      loopTime = 200;
      
      menuDisplay[0] = { "env-menu" };
      menuDisplay[1] = { "alert" };
      menuDisplay[2] = { "relay" };
      return 0;

    }
// menu state env_main_menu
// env menu
// 1.alert
// 2.relay
// 3.exit

    if (stateMenu == "main_menu" && button2 == HIGH){
      stateMenu = "wifi";
      displayed = 0;
      loopTime = 200;
      //menuDisplay[0] = { "wifi-menu" };
      //menuDisplay[1] = { ssid };
      //menuDisplay[2] = { pass };
      return 0;

    }
    
    if (stateMenu == "wifi" && button == HIGH){
      stateMenu = "wifi_sid";
      displayed = 0;
      loopTime = 200;
      
        //strcat(wifi_sid_add,charSet[charCount]);
        snprintf(asciiChar,2,"%c",33);
        
      //strcpy(wifi_sid_add,charSet[charCount]);
      //strcpy(wifi_sid_add,asciiChar);
      strcat(wifi_sid_add,asciiChar);
      
      return 0;
    }

    

    if (stateMenu == "wifi_sid" && button == HIGH){
      stateMenu = "wifi_sid";
      displayed = 0;
      loopTime = 200;

      charCount = charCount + 1;
      if (charCount > 126){
        charCount = 33;
      } 
      snprintf(asciiChar,2,"%c",charCount);
        
      //strcpy(wifi_sid_add,charSet[charCount]);
      strcpy(wifi_sid_add,asciiChar);
      
      //strcpy(wifi_sid_add,charSet[charCount]);
      
      return 0;
    }

    if (stateMenu == "wifi_sid" && button2 == HIGH){
      stateMenu = "wifi_sid";
      displayed = 0;
      loopTime = 200;
      //charCount = charCount - 1;

      //if (charCount < 33) {
      //  charCount = 126;
      //}

      snprintf(asciiChar,2,"%c",charCount);
        
      //strcpy(wifi_sid_add,charSet[charCount]);
      strcpy(wifi_sid_add,asciiChar);

      return 0;
    }

    if (stateMenu == "wifi_sid" && button3 == HIGH){
      stateMenu = "wifi_sid";
      displayed = 0;
      loopTime = 200;
      
      //strcat(wifi_sid,charSet[charCount]);
      snprintf(asciiChar,2,"%c",charCount);
        
      //strcpy(wifi_sid_add,charSet[charCount]);
      strcat(wifi_sid,asciiChar);

      

      charCount = 65;
      totalChars = totalChars + 1;
      return 0;
    
    }

    
    


    if (stateMenu == "wifi" && button == HIGH){
      stateMenu = "wifi_pw";
      displayed = 0;
      loopTime = 200;
      
      return 0;
    }


//menu state relay
// relay menu
// 1. limit
// 2. toggle
    

//menu relay_1
// 1. co2/hum
// 2. temp
// 3. exit
    if (stateMenu == "relay" && button == HIGH){
      stateMenu = "relay_limit";
      displayed = 0;
      loopTime = 200;
      menuDisplay[0] = { "relay-limit" };
      menuDisplay[1] = { "temp" };
      menuDisplay[2] = { "co2/hum" };

      return 0;
    }
    if (stateMenu == "relay" && button2 == HIGH){
      stateMenu = "relay_toggle";
      displayed = 0;
      loopTime = 200;
      menuDisplay[0] = { "relay-toggle" };
      menuDisplay[1] = { "fan" };
      menuDisplay[2] = { "hum" };

      return 0;
    }
    if (stateMenu == "relay_toggle" && button == HIGH){
      stateMenu = "relay_toggle_fan";
      displayed = 0;
      loopTime = 200;
      return 0;
    }
    if (stateMenu == "relay_toggle_fan" && button == HIGH){
      stateMenu = "relay_toggle_fan";
      displayed = 0;
      relay_fan_toggle = 0;
      loopTime = 200;
      return 0;
    }
    if (stateMenu == "relay_toggle_fan" && button2 == HIGH){
      stateMenu = "relay_toggle_fan";
      displayed = 0;
      relay_fan_toggle = 1;
      loopTime = 200;
      return 0;
    }

     if (stateMenu == "relay_toggle_hum" && button == HIGH){
      stateMenu = "relay_toggle_hum";
      displayed = 0;
      relay_hum_toggle = 0;
      loopTime = 200;
      return 0;
    }
    if (stateMenu == "relay_toggle_hum" && button2 == HIGH){
      stateMenu = "relay_toggle_hum";
      displayed = 0;
      relay_hum_toggle = 1;
      loopTime = 200;
      return 0;
    }




    if (stateMenu == "relay_limit" && button == HIGH){
      stateMenu = "relay_temp";
      displayed = 0;
      loopTime = 200;
      

      return 0;
    }
    if (stateMenu == "relay_limit" && button2 == HIGH){
      stateMenu = "relay_limit_1";
      displayed = 0;
      loopTime = 200;
      menuDisplay[0] = { "relay-limit" };
      menuDisplay[1] = { "co2" };
      menuDisplay[2] = { "hum" };
      return 0;
    }

    if (stateMenu == "relay_limit_1" && button == HIGH){
      stateMenu = "relay_co2";
      displayed = 0;
      loopTime = 200;
      
      return 0;
    }
    if (stateMenu == "relay_co2" && button == HIGH){
      //env menu temp
      stateMenu = "relay_co2";
      displayed = 0;
      relay_co2 = relay_co2 +1;
      
      loopTime = 200;
      return 0;
    }
  if (stateMenu == "relay_co2" && button2 == HIGH){
      stateMenu = "relay_co2";
      displayed = 0;
      relay_co2 = relay_co2 -1;
      loopTime = 200;
      return 0;
    }
    


    if (stateMenu == "relay_limit_1" && button == HIGH){
      stateMenu = "relay_hum";
      displayed = 0;
      loopTime = 200;
      
      return 0;
    }
    if (stateMenu == "relay_hum" && button == HIGH){
      //env menu temp
      stateMenu = "relay_hum";
      displayed = 0;
      relay_hum = relay_hum +1;
      
      loopTime = 200;
      return 0;
    }
  if (stateMenu == "relay_hum" && button2 == HIGH){
      stateMenu = "relay_hum";
      displayed = 0;
      relay_hum = relay_hum -1;
      loopTime = 200;
      return 0;
    }



    if (stateMenu == "relay_temp" && button == HIGH){
      //env menu temp
      stateMenu = "relay_temp";
      displayed = 0;
      relay_temp = relay_temp +1;
      
      loopTime = 200;
      return 0;
    }
  if (stateMenu == "relay_temp" && button2 == HIGH){
      stateMenu = "relay_temp";
      displayed = 0;
      relay_temp = relay_temp -1;
      loopTime = 200;
      return 0;
    }

//menu relay_2


if (stateMenu == "env_menu" && button == HIGH){
      stateMenu = "env_alert";
      displayed = 0;
      loopTime = 200;
      return 0;
    }
// menu state env_menu_alert_1
// 1. temp
// 2. co2 / hum
// 3 .exit
   if (stateMenu == "env_menu" && button2 == HIGH){
      stateMenu = "relay";
      displayed = 0;
      loopTime = 200;
      menuDisplay[0] = { "relay-menu" };
      menuDisplay[1] = { "limit" };
      menuDisplay[2] = { "toggle" };
      return 0;
    }
  if (stateMenu == "env_alert" && button == HIGH){
      //env menu temp
      stateMenu = "env_alert_1";
      displayed = 0;
      loopTime = 200;
      return 0;
    }
  if (stateMenu == "env_alert_1" && button == HIGH){
      //env menu temp
      stateMenu = "env_alert_temp";
      displayed = 0;
      loopTime = 200;
      return 0;
    }
// menu state env_menu_alert_temp
//env alert temp
//set temp to alert : alert_temp
// 3. save and exit

if (stateMenu == "env_alert_temp" && button == HIGH){
      //env menu temp
      stateMenu = "env_alert_temp";
      displayed = 0;
      alert_temp = alert_temp +1;
      loopTime = 200;
      return 0;
    }
  if (stateMenu == "env_alert_temp" && button2 == HIGH){
      stateMenu = "env_alert_temp";
      displayed = 0;
      alert_temp = alert_temp -1;
      loopTime = 200;
      return 0;
    }

  if (stateMenu == "env_alert" && button2 == HIGH){
      //env menu temp
      stateMenu = "env_alert_2";
      displayed = 0;
      loopTime = 200;
      return 0;
    }

//menu state env_menu_sub_2
// 1. co2
// 2. hum
// 3 .exit

  if (stateMenu == "env_alert_2" && button == HIGH){
      //env menu temp
      stateMenu = "env_alert_co2";
      displayed = 0;
      loopTime = 10;
      return 0;
    }

//menu state env_menu_alert_co2
//env alert co2
//set co2 to alert : alert_co2
// 3. save and exit

    if (stateMenu == "env_alert_co2" && button == HIGH){
      //env menu temp
      stateMenu = "env_alert_co2";
      displayed = 0;
      alert_co2 = alert_co2 +1;
      loopTime = 200;
      return 0;
    }
    if (stateMenu == "env_alert_co2" && button2 == HIGH){
      stateMenu = "env_alert_co2";
      displayed = 0;
      alert_co2 = alert_co2 -1;
      loopTime = 200;
      return 0;
    }
    


    if (stateMenu == "env_alert_2" && button2 == HIGH){
      //env menu temp
      stateMenu = "env_alert_hum";
      displayed = 0;
      loopTime = 200;
      return 0;
    }


    if (stateMenu == "env_alert_hum" && button == HIGH){
      //env menu temp
      stateMenu = "env_alert_hum";
      displayed = 0;
      alert_hum = alert_hum +1;
      loopTime = 200;
      return 0;
    }
    if (stateMenu == "env_alert_hum" && button2 == HIGH){
      stateMenu = "env_alert_hum";
      displayed = 0;
      alert_hum = alert_hum -1;
      loopTime = 200;
      return 0;
    }
}

void drawMenu(){
       //char *menuDisplay[] = {"one", "two", "three"};
       u8g2.begin();
       u8g2.enableUTF8Print();
       u8g2.setFont(u8g2_font_helvB08_tf);
       u8g2.setFontDirection(0);
  //u8g2.clearBuffer();
    
       u8g2.setCursor(12,10);
       u8g2.print(menuDisplay[0]);
    
    
      u8g2.setCursor(12,30);
      u8g2.print("1. ");
      u8g2.print(menuDisplay[1]);
    
    
      u8g2.setCursor(12,40);
      u8g2.print("2. ");
      u8g2.print(menuDisplay[2]);
    
      u8g2.setCursor(12,50);
      u8g2.print("3. exit ");
    
    u8g2.sendBuffer();
  
}




void mainMenu(){
  u8g2.begin();
  u8g2.enableUTF8Print();   
  //8g2.setFont(u8g2_font_helvB18_tf);
  u8g2.setFont(u8g2_font_helvB08_tf);
  u8g2.setFontDirection(0);
  //u8g2.clearBuffer();
  u8g2.setCursor(12,10);
  u8g2.print("setup menu");
  u8g2.setCursor(12,30);
  u8g2.print("1. env");
  u8g2.setCursor(12,40);
  u8g2.print("2. wifi ");
  u8g2.setCursor(12,50);
  u8g2.print("3. exit ");
  
  u8g2.sendBuffer();
}

void wifi(){
  u8g2.begin();
  u8g2.enableUTF8Print();   
  //8g2.setFont(u8g2_font_helvB18_tf);
  u8g2.setFont(u8g2_font_helvB08_tf);
  u8g2.setFontDirection(0);
  //u8g2.clearBuffer();
  u8g2.setCursor(12,10);
  u8g2.print("url http://");
  u8g2.print(WiFi.softAPIP());
  u8g2.setCursor(12,30);
  u8g2.print("ssid: ");
  u8g2.print(ssid);
  u8g2.setCursor(12,40);
  u8g2.print("pw: ");
  u8g2.print(pass);
  u8g2.setCursor(12,50);
  u8g2.print("3. save and exit");
  u8g2.sendBuffer();

}

void relay(){
  u8g2.begin();
  u8g2.enableUTF8Print();   
  //8g2.setFont(u8g2_font_helvB18_tf);
  u8g2.setFont(u8g2_font_helvB08_tf);
  u8g2.setFontDirection(0);
  //u8g2.clearBuffer();
  u8g2.setCursor(12,10);
  u8g2.print("Relay set");
  u8g2.setCursor(12,30);
  u8g2.print("1. value");
  u8g2.setCursor(12,40);
  u8g2.print("2. toggle ");
  u8g2.setCursor(12,50);
  u8g2.print("3. exit ");
  
  u8g2.sendBuffer();

}

void envAlert1(){
  u8g2.begin();
  u8g2.enableUTF8Print();   
  //8g2.setFont(u8g2_font_helvB18_tf);
  u8g2.setFont(u8g2_font_helvB08_tf);
  u8g2.setFontDirection(0);
  //u8g2.clearBuffer();
  u8g2.setCursor(12,10);
  u8g2.print("env menu alert");
  u8g2.setCursor(12,30);
  u8g2.print("1. temp");
  u8g2.setCursor(12,40);
  u8g2.print("2. co2 ");
  u8g2.setCursor(12,50);
  u8g2.print("3. exit ");
  
  u8g2.sendBuffer();

}


void envAlert(){
  u8g2.begin();
  u8g2.enableUTF8Print();   
  //8g2.setFont(u8g2_font_helvB18_tf);
  u8g2.setFont(u8g2_font_helvB08_tf);
  u8g2.setFontDirection(0);
  //u8g2.clearBuffer();
  u8g2.setCursor(12,10);
  u8g2.print("env menu alert");
  u8g2.setCursor(12,30);
  u8g2.print("1. temp");
  u8g2.setCursor(12,40);
  u8g2.print("2. co2 ");
  u8g2.setCursor(12,50);
  u8g2.print("3. exit ");
  
  u8g2.sendBuffer();

}
void envAlert2(){
  u8g2.begin();
  u8g2.enableUTF8Print();   
  //8g2.setFont(u8g2_font_helvB18_tf);
  u8g2.setFont(u8g2_font_helvB08_tf);
  u8g2.setFontDirection(0);
  //u8g2.clearBuffer();
  u8g2.setCursor(12,10);
  u8g2.print("env menu alert");
  u8g2.setCursor(12,30);
  u8g2.print("1. co2");
  u8g2.setCursor(12,40);
  u8g2.print("2. hum ");
  u8g2.setCursor(12,50);
  u8g2.print("3. exit ");
  
  u8g2.sendBuffer();

}
void envAlertTemp(){
  u8g2.begin();
  u8g2.enableUTF8Print();   
  //8g2.setFont(u8g2_font_helvB18_tf);
  u8g2.setFont(u8g2_font_helvB08_tf);
  u8g2.setFontDirection(0);
  //u8g2.clearBuffer();
  u8g2.setCursor(12,10);
  u8g2.print("env alert temp");
  u8g2.setCursor(12,30);
  u8g2.print("Set temp to alert : ");
  
  u8g2.print(alert_temp);
  u8g2.setCursor(12,50);
  u8g2.print("3. save and exit");
  
  u8g2.sendBuffer();

}
void envAlertCo2(){
  u8g2.begin();
  u8g2.enableUTF8Print();   
  //8g2.setFont(u8g2_font_helvB18_tf);
  u8g2.setFont(u8g2_font_helvB08_tf);
  u8g2.setFontDirection(0);
  //u8g2.clearBuffer();
  u8g2.setCursor(12,10);
  u8g2.print("env alert co2");
  u8g2.setCursor(12,30);
  u8g2.print("Set co2 to alert : ");
  
  u8g2.print(alert_co2);
  u8g2.setCursor(12,50);
  u8g2.print("3. save and exit");
  
  u8g2.sendBuffer();

}

void envAlertHum(){
  u8g2.begin();
  u8g2.enableUTF8Print();   
  //8g2.setFont(u8g2_font_helvB18_tf);
  u8g2.setFont(u8g2_font_helvB08_tf);
  u8g2.setFontDirection(0);
  //u8g2.clearBuffer();
  u8g2.setCursor(12,10);
  u8g2.print("env alert hum");
  u8g2.setCursor(12,30);
  u8g2.print("Set Hum to alert : ");
  
  u8g2.print(alert_hum);
  u8g2.setCursor(12,50);
  u8g2.print("3. save and exit");
  
  u8g2.sendBuffer();

}

void relayTemp(){
  u8g2.begin();
  u8g2.enableUTF8Print();   
  //8g2.setFont(u8g2_font_helvB18_tf);
  u8g2.setFont(u8g2_font_helvB08_tf);
  u8g2.setFontDirection(0);
  //u8g2.clearBuffer();
  u8g2.setCursor(12,10);
  u8g2.print("relay-temp");
  u8g2.setCursor(12,30);
  u8g2.print("Set relay Temp : ");
  
  u8g2.print(relay_temp);
  u8g2.setCursor(12,50);
  u8g2.print("3. save and exit");
  
  u8g2.sendBuffer();

}

void relayCo2(){
  u8g2.begin();
  u8g2.enableUTF8Print();   
  //8g2.setFont(u8g2_font_helvB18_tf);
  u8g2.setFont(u8g2_font_helvB08_tf);
  u8g2.setFontDirection(0);
  //u8g2.clearBuffer();
  u8g2.setCursor(12,10);
  u8g2.print("relay-temp");
  u8g2.setCursor(12,30);
  u8g2.print("Set relay Co2 : ");
  
  u8g2.print(relay_co2);
  u8g2.setCursor(12,50);
  u8g2.print("3. save and exit");
  
  u8g2.sendBuffer();

}

void relayHum(){
  u8g2.begin();
  u8g2.enableUTF8Print();   
  //8g2.setFont(u8g2_font_helvB18_tf);
  u8g2.setFont(u8g2_font_helvB08_tf);
  u8g2.setFontDirection(0);
  //u8g2.clearBuffer();
  u8g2.setCursor(12,10);
  u8g2.print("relay-temp");
  u8g2.setCursor(12,30);
  u8g2.print("Set relay Hum : ");
  
  u8g2.print(relay_hum);
  u8g2.setCursor(12,50);
  u8g2.print("3. save and exit");
  
  u8g2.sendBuffer();

}

void relayToggleHum(){
  u8g2.begin();
  u8g2.enableUTF8Print();   
  //8g2.setFont(u8g2_font_helvB18_tf);
  u8g2.setFont(u8g2_font_helvB08_tf);
  u8g2.setFontDirection(0);
  //u8g2.clearBuffer();
  u8g2.setCursor(12,10);
  u8g2.print("relay-hum toggle");
  u8g2.setCursor(12,30);
  u8g2.print("Set relay Hum : ");
  
  u8g2.print(relay_hum_toggle);
  u8g2.setCursor(12,50);
  u8g2.print("3. save and exit");
  
  u8g2.sendBuffer();

}

void relayToggleFan(){
  u8g2.begin();
  u8g2.enableUTF8Print();   
  //8g2.setFont(u8g2_font_helvB18_tf);
  u8g2.setFont(u8g2_font_helvB08_tf);
  u8g2.setFontDirection(0);
  //u8g2.clearBuffer();
  u8g2.setCursor(12,10);
  u8g2.print("relay-fan toggle");
  u8g2.setCursor(12,30);
  u8g2.print("Set relay fan : ");
  
  u8g2.print(relay_fan_toggle);
  u8g2.setCursor(12,50);
  u8g2.print("3. save and exit");
  
  u8g2.sendBuffer();

}
void WifiSid(){
  u8g2.begin();
  u8g2.enableUTF8Print();   
  //8g2.setFont(u8g2_font_helvB18_tf);
  u8g2.setFont(u8g2_font_helvB08_tf);
  u8g2.setFontDirection(0);
  //u8g2.clearBuffer();
  u8g2.setCursor(12,10);
  u8g2.print("Wifi sid");
  u8g2.setCursor(12,30);

  
  u8g2.print(wifi_sid);
  u8g2.print(wifi_sid_add);

  u8g2.setCursor(12,50);
  u8g2.print("3. save and exit");
  
  u8g2.sendBuffer();

}







void displayMenu(){
  if (!displayed){
  
  if (stateMenu=="main_menu"){
    mainMenu();
  }
  if (stateMenu=="env_menu"){
    drawMenu();
  }
  if (stateMenu=="relay"){
    drawMenu();
  }
  if (stateMenu=="relay_limit"){
    drawMenu();
  }
  if (stateMenu=="relay_limit_1"){
    drawMenu();
  }
  if (stateMenu=="relay_toggle"){
    drawMenu();
  }
  if (stateMenu=="wifi_sid"){
    WifiSid();
  }
  if (stateMenu=="relay_toggle_hum"){
    relayToggleHum();
  }
  if (stateMenu=="relay_toggle_fan"){
    relayToggleFan();
  }

  if (stateMenu=="relay_temp"){
    relayTemp();
  }
  if (stateMenu=="relay_co2"){
    relayCo2();
  }
  if (stateMenu=="relay_hum"){
    relayHum();
  }
  




  if (stateMenu=="wifi"){
    wifi();
  }


  if (stateMenu=="env_alert"){
    envAlert();
  
  }
  if (stateMenu=="env_alert_1"){
    envAlert1();
  }
  if (stateMenu=="env_alert_2"){
    envAlert2();
  }
  if (stateMenu=="env_alert_temp"){
    envAlertTemp();
  }
  if (stateMenu=="env_alert_co2"){
    envAlertCo2();
  }
  if (stateMenu=="env_alert_hum"){
    envAlertHum();
  }


  
  }
  displayed = 1;
}

byte history[1024];

void displayEnvData(){
  //u8g2.begin();
  u8g2.enableUTF8Print();   
  //8g2.setFont(u8g2_font_helvB18_tf);
  //u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_helvB08_tf);
  u8g2.setFontDirection(0);
  u8g2.clearBuffer();
  u8g2.setCursor(12,10);
  //u8g2.setCursor(12,10);
  u8g2.print("CO2 :");
  //u8g2.setCursor(45,55);
  u8g2.print(sgp.eCO2);
  u8g2.setCursor(12,30);
  u8g2.print("TVOC :");
  //u8g2.setCursor(45,55);
  u8g2.print(sgp.rawEthanol);
  u8g2.setCursor(12,40);
  u8g2.print("TEMP :");
  u8g2.print(Temperature);
  u8g2.setCursor(12,50);
  u8g2.print("Humid :");
  u8g2.print(Humidity);
  //u8g2.sendBuffer();
  //updateOnChange();
  
  if (memcmp(history,u8g2.getBufferPtr(),1024)){
    memcpy(history, u8g2.getBufferPtr(), 1024);
    
    Serial.println("changed!");
    u8g2.sendBuffer();
    
    }    

}

void webUpdate(){

  u8g2.begin();
  u8g2.enableUTF8Print();   
  //8g2.setFont(u8g2_font_helvB18_tf);
  u8g2.setFont(u8g2_font_helvB08_tf);
  u8g2.setFontDirection(0);
  //u8g2.clearBuffer();
  u8g2.setCursor(12,10);
  //u8g2.setCursor(12,10);
  u8g2.print("update via");
  u8g2.setCursor(12,20);
  u8g2.print("web interface...");
  u8g2.sendBuffer();
  delay(1000);
  
  //updateOnChange();

  
  
  
};


void updateOnChange(){
  
  //Serial.println("checking for update);
  if (memcmp(history,u8g2.getBufferPtr(),1024)){
    memcpy(history, u8g2.getBufferPtr(), 1024);
    Serial.println("changed!");
    u8g2.sendBuffer();
    
    }
  
  };


void handleRoot();
void handleNotFound();


void setup() {
  Serial.begin(115200);
  Wire.begin();  
  sgp.begin();
  u8g2.begin();
  WiFi.mode(WIFI_AP_STA);
  WiFi.softAP(ssid, pass);
  WiFi.begin(wifi_ssid, wifi_pass);
  Serial.print("Connecting to ");
  Serial.print(wifi_ssid);
  while (WiFi.status() != WL_CONNECTED )
  {
    delay(1000);
    Serial.print(".");
    }

  Serial.println();
  Serial.print("Connected to WIFI 1234 ");
  Serial.println(WiFi.localIP());
  
  server.begin();
  u8g2.enableUTF8Print();
  SPIFFS.begin();
  //readStoredData();
  
  //pinMode(Push_button_1,INPUT);
  //pinMode(Push_button_2,INPUT);
  //pinMode(Push_button_3,INPUT);
  //pinMode(Relay1,OUTPUT);
  //WiFi.mode(WIFI_AP);
  Serial.println("Server loaded");
  server.on("/", handleRoot);
  server.on("/submit_page", handleForm);
  server.on("/test", fileindex);
  server.onNotFound(handleNotFound);
  server.on("/bootstrap.min.css", bootstrap);
  server.on("/jquery-slim.min.css", jquery);
  server.on("bootstrap.min.css", bootstrap);
  server.on("/popper.min.js", popper);
  server.on("/bootstrap.min.js", bootstrapmin);
  server.on("bootstrap.min.js", bootstrapmin);

  server.on("/relay_on", handle_RelayOn);
  server.on("/relay_off", handle_RelayOff);

   


  Serial.print("IP address for network ");
  //Serial.print(AP_SSID);
  //Serial.print(" : ");
  //Serial.print(WiFi.softAPIP());


 // u8g2.setFont(u8g2_font_6x10_tf);
  //u8g2.setFontDirection(0);
  //u8g2.clearBuffer();
  //u8g2.setCursor(0,15);
  //u8g2.print("CO2: ");
 // u8g2.setCursor(0,40);
 // u8g2.print(Serial.print(sgp.eCO2));
  //u8g2.sendBuffer();
  //Serial.println("SGP30 test");

  if (! sgp.begin()){
    Serial.println("Sensor not found :(");
    //while (1);
  }
  Serial.print("Found SGP30 serial #");
  Serial.print(sgp.serialnumber[0], HEX);
  Serial.print(sgp.serialnumber[1], HEX);
  Serial.println(sgp.serialnumber[2], HEX);

  // If you have a baseline measurement from before you can assign it to start, to 'self-calibrate'
  //sgp.setIAQBaseline(0x8E68, 0x8F41);  // Will vary for each sensor!
}

int counter = 0;

void loop() {

  server.handleClient();
  if (stateMenu == "sensor_data") {
   
   displayEnvData();

  }
  return;
  

  int data1 = digitalRead(Push_button_1);
  int data2 = digitalRead(Push_button_2);
  int data3 = digitalRead(Push_button_3);

  
  
  menuState(data1,data2,data3);

  //Serial.println(stateMenu);
  
  if (stateMenu != "sensor_data") {
   
   //delay(200);
   
   displayMenu();
   

  }
   



  // If you have a temperature / humidity sensor, you can set the absolute humidity to enable the humditiy compensation for the air quality signals
  //float temperature = 22.1; // [°C]
  //float humidity = 45.2; // [%RH]
  //sgp.setHumidity(getAbsoluteHumidity(temperature, humidity));
  
  //Temperature = dht.readTemperature(); // Gets the values of the temperature
  //Humidity = dht.readHumidity(); // Gets the values of the humidity 

  
  
    
  //u8g2.sendBuffer();
  //delay(5000);
  
  //u8g2.clearBuffer();

  if (stateMenu == "sensor_data") {
   
   displayEnvData();

  }
  

  
  counter++;
  if (counter == 300) {
    counter = 0;

    uint16_t TVOC_base, eCO2_base;
    if (! sgp.getIAQBaseline(&eCO2_base, &TVOC_base)) {
      Serial.println("Failed to get baseline readings");
      return;
    }
    Serial.print("****Baseline values: eCO2: 0x"); Serial.print(eCO2_base, HEX);
    Serial.print(" & TVOC: 0x"); Serial.println(TVOC_base, HEX);
    
    if (! sgp.IAQmeasure()) {
    Serial.println("Measurement failed");
    //return;
    //delay(5000);
  }
  Serial.print("TVOC "); Serial.print(sgp.TVOC); Serial.print(" ppb\t");
  Serial.print("eCO2 "); Serial.print(sgp.eCO2); Serial.println(" ppm");

  if (! sgp.IAQmeasureRaw()) {
    Serial.println("Raw Measurement failed");
    //return;
  }
  Serial.print("Raw H2 "); Serial.print(sgp.rawH2); Serial.print(" \t");
  Serial.print("Raw Ethanol "); Serial.print(sgp.rawEthanol); Serial.println("");




  }
  
}

void handleRoot() {
  String form_relay_toggle_fan="";
  server.send(200, "text/html", sendHTML(form_relay_toggle_fan) );   // Send HTTP status 200 (Ok) and send some text to the browser/client
}

void handleNotFound(){
  server.send(404, "text/plain", "404: Not found"); // Send HTTP status 404 (Not Found) when there's no handler for the URI in the request
}

void bootstrap(){
   File file = SPIFFS.open("/bootstrap.min.css", "r"); 
   size_t sent = server.streamFile(file, "text/css");
}
void jquery(){
   File file = SPIFFS.open("/jquery-slim.min.js", "r"); 
   size_t sent = server.streamFile(file, "application/javascript");
}



void popper(){
   File file = SPIFFS.open("/popper.min.js", "r"); 
   size_t sent = server.streamFile(file, "application/javascript");
}

void bootstrapmin(){
   File file = SPIFFS.open("/bootstrap.min.js", "r"); 
   size_t sent = server.streamFile(file, "application/javascript");
}
void fileindex(){
   File file = SPIFFS.open("/index.html", "r"); 
   size_t sent = server.streamFile(file, "text/html");
}

void handleForm() {

  Serial.println("handleForm"); 

   String form_ap_pass = server.arg("ap_pass");
   String form_ap_ssid = server.arg("ap_ssid");
   String form_ap_active = server.arg("ap_active");
   String form_cl_pass = server.arg("cl_pass");
   String form_cl_ssid = server.arg("cl_ssid");
   String form_cl_active = server.arg("cl_active");
   String form_relay_limit_temp = server.arg("relay_limit_temp");
   String form_relay_limit_hum = server.arg("relay_limit_hum");
   String form_relay_limit_co2 = server.arg("relay_limit_co2");
   String form_relay_toggle_fan = server.arg("relay_toggle_fan");
   String form_relay_toggle_hum = server.arg("relay_limit_hum");

  Serial.println(form_ap_active);
  Serial.println(form_ap_pass);
  Serial.println(form_ap_ssid);
  Serial.println(form_cl_pass);
  Serial.println(form_cl_ssid);
  Serial.println(form_relay_limit_temp);
  Serial.println(form_relay_limit_co2);
  Serial.println(form_relay_toggle_hum);
  Serial.println(form_relay_toggle_fan);
  

  if (form_relay_toggle_fan == "on")
  {
    handle_RelayOn();
    }
    else
    {
      handle_RelayOff();
      }
    
  server.send(200, "text/html", sendHTML(form_relay_toggle_fan) );
  
  webUpdate();

  
}

void handle_RelayOn(){
Serial.println("relay on");
 digitalWrite(Relay1, HIGH);
};

void handle_RelayOff(){
  Serial.println("relay off");
 digitalWrite(Relay1, LOW);
};

void readStoredData(){

  if (!SPIFFS.begin(true)) {
    Serial.println("Error file system has occured while mounting");
    }

   File file = SPIFFS.open("/wifi.txt");

   if (!file) {
    Serial.println("failed to open file for reading");
    return;
   }

   Vector<String> v;

   while (file.available()) {
    v.push_back(file.readStringUntil('\n'));
    
    }
   file.close();

   for (String s : v ) {
    Serial.println(s);
    
    }
    
   }
  
