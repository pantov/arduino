//
//


String ver="sputnik_59"; 
String wifi_ssid     = "sky"; // INSERT HERE Your_wifi_ssid
String wifi_password = "1234paradize$"; // INSERT HERE Your_wifi_password

/*
bortx_59  ChipId()to terminal; added stop statement
bortx_58  new id demo as id device ESP.GetChipId(); ignore operator if begin with /
          added TimeZone into reserv3
bortx_57  added $morning, $evening ; softserial was removed - $input from Serial;
          added $ptime,  sec in a scan pause  
bortx_56  added using <>;trans;goto or w/o ones; added multi $input soft pin7 to X[] <3>333</3> XML          
          added changing of Auto mode is immideately on panel;couner X0 on D0 show count w/o calculation
          save in pre_sync old data
bortx_54  reordering a panel fields A0,D0-D8,S0-S4,Auto  day of start in <> bracket f/ex <31>
          it is not a good idea extract digits from data string, not extract from arrays
          and we refuse with vars in []  f/ex [Temp] ONLY STANDART VARS $Xn $Sn $Dn and $A0  
bortx_53  copy  bortx_52t with debug procedur. 
bortx_52t added pre_sync(); preset and precorrect tome_on_off like former [11:11:11] and {sec}  
bortx_51t added $input - digital text in <> recieved as serial from pin D7
          now it possible save in eprom $S0-$S4 by oper E($S0)
          added start_mode if  mission == "demo" - like answer dialog "demo" and "y" 
          added Unix Date YYYY-MM-DD - $udate 
          M(); - send mail - including in brekets is processing by substitution but not by calculation      
bortx_50t added $ntime to SFC (ex. 21.59) sub sync call msk_time.cgi set time_on_off 
bortx_49t added $retro,$random, now possible (x<=y) (x>=y);uncodidional trans:(=){10}; comments after ';'   
bortx_48t eprom 4096 ;no more 40 sym can read in E_read()  
bortx_47t $X0.....$X9 Debug  
bortx_46t Calculation : "A~B" means "A-B"  
bortx_45t Substitution
tuningNOW[i+10] control array "S[]" too
bortx_44t create counter for d0. measure will act in during 50 or 100 ms delay!
My discovery  !!! even D0 in output mode D0 can read HIGH/LOW level
bortx_43t it is possible 3 variants answer on question bortx_id? ; 
                nothing - old id;"demo" - demo_+modificated mac(look at code); another answer - this is new id   
               as result new id will output like context bortx_id=..= ; it will be recieved by _wizard.au3
               bortx_id will define after register in Wifi net
bortx_43t  time_on_off (time/sec from start){sec since last restart}
bortx_38t  new version s88 script
bortx_37t  added field time_on_off(unix) instead vendor; set and check mail via biosoft24 and via server
-------    added note=zzzz;
bortx_31-36t  added email operator M=mes;
bortx_31t new rulles to write down eprom.txt
bortx_30t add button Auto and S0..S4  -SetPoints
bortx_21t - termometr 18b20 to pin D5
bortx_21 - eprom size =1024 ; added command epromr (read eprom and write to file eprom.txt),epromw file->eprom  
-------- - prompt for input ssid, psw, bortx_id if push 'f9' in during scan wifi station 
-------- - if TuningRW[i] == 'U' then mode= INPUT_PULLUP
bortx_20 - added tuning_RW  - mode READ/WRITE D0-D8; examples 111110000 D0-D4 WRITE  
bort _19 - REMOVED added tuningSHOW - show  or not FORMAT's  and data's items !!! WAS REMOVED !!!
bortx_18 - added tuningPANEL  - config to panel for variables position R - only read
bortx_17 - difference from bortx_16 --- ONLY max time waitin PING from 120 to 150 sec
must  cript password from esp to server 
bortx_16 marker not only 0-1, but marker*1000 + pause since last ping 
         if last_error start with !, thus readonly cancel
bortx 14 send version? mission etc to root and dir like status.txt
bortx_13  bortx_id -> only read  
bortx_12 added memory of digital outputs 
bortx_11 change tuningSIGN-> tuningSIGN add tuningNOW  whether send changing var imedatoly or by T_scan
bortx_07 restart by absence of ping or command 70 sec
bortx_06 made warnuig if the html form item length more than 19 chars 
bortx_05 now firmwate may be downloaded via proxy too nothng to to for that - automate
bortx_04 remove bugs status; insert delay in loop; beep on BuiltInLed 
bortx_03 remove bugs
b0rtx_02 - added SetFormat, timing
prototype - bx_08 - firmware
bx_08 set format D0,D1..D8 in eprom  Vars1 D0=Diod;D2=3M1 Vars2 D3=3M2;..etc it made in func SetFormat() 


*/

#include <ESP8266WiFi.h>
#include <EEPROM.h>
#include <ESP8266WebServer.h>
#include <ESP8266httpUpdate.h>

#include <OneWire.h>
#include <DallasTemperature.h>
#define ONE_WIRE_BUS 14
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
static const uint8_t D0   = 16;
static const uint8_t D1   = 5;
static const uint8_t D2   = 4;
static const uint8_t D3   = 0;
static const uint8_t D4   = 2;
static const uint8_t D5   = 14;
static const uint8_t D6   = 12;
static const uint8_t D7   = 13;
static const uint8_t D8   = 15;
static const uint8_t RX   = 3;
static const uint8_t TX   = 1;

String Morning(void); //  return f/ex "10.40" 
String Evening(void); //  return f/ex "18.40"
void pre_sync(void);
void get_command(void);
void myResetFunc(void);
void(* resetFunc) (void) = 0;
String val(String, String);
int mem(String);
void E_write(String,int);
String E_read(int);
void handleRoot(void); 
void go_stet2(void);
void led(int);
void control(void);// set 'send_cito>0' if is must send imedately 
void status_save(void);
void set_data(void);
String wifi_scan(void);
String SetFormat(void);
String setSHOW(String);
void epromr(void);
void epromw(void);
String dialog(String);
String preproc(void); 
void proc(void);
void activ(void);
int trans(void);
String between(String,String,String);
String extract_digi(String);
void mail(String);
void sync(void);
void note(String);
void s_delay(int);
String substitution(String);
String subs_rand(String);
String calculation(String);
String Ntime(void);
String Input(void);
String Temp(void);
String Udate="0000-00-00";
String Format="";
String answer_clear_eprom="";
String answer_bortx_id="";
String tmpt="";
String last_error="";
String mission="";
String uri="";
String uri2="";
String ch="0";
String  line; 
int long_polling6010=0;
char sbuf;
long millis_old=0;
long t_stop_scan=0;
int Ptime=0;
String st="";
int iukaz=0;
int pozd;
int ukazfaz=1; //point 1-st string pfase
int count0=0;


float coeff_k=1;
float coeff_b=0;
long count_time=0; // periode an activ counter

String outputM="000000000000000"; //len=15
String res_wifi_scan="";
String bortx_id="";
String bortx_pass="";
String time_on_off="";
String name_bin="";
String host_source="bortx.ru"; // DEFAULT PROXY
String tuningSIGN="";
String tuningNOW="";
String tuningPANEL="";
String tuning_RW="";
int httpPort = 0;
String payload="";
String input0="0"; //memory for sub Input()

int ConnHostPort6010 = 6010;
int ConnHostPort6110 = 6110;


int status_proxy=0;

String Status="try wifi connecting...";
int err_ping=0;
long min_old=0;
long min_new=0;
long last_ping=0; // counter bad pings
String error="";
long last_sec_control=0; // last time sec control()
long last_conn_time=0;


int send_cito;
int syn=0;
int recv_topanel=0; // nomber recvievd "topanel"
int stet2=0;

long start_led=0;

// Pinout : don't use D3! -> FLASH !!! GPIO_00

int ukaz[]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}; //p1-p20 
String name_var_D[]={"D0","D1","D2","D3","D4","D5","D6","D7","D8"};
String name_var_S[]={"S0","S1","S2","S3","S4", "S5","S6","S7","S8","S9"}; // now use only 0-4
int d_name[]={D0,D1,D2,D3,D4,D6,D7,D8}; //D name 
int d_val[]    ={0,0,0,0,0,0,0,0,0}; // d value
int d_val_old[]={2,2,2,2,2,2,2,2,2}; // d value old
String s_val[]={"0","1","2","3","4","5","6","7","8","9" };  // now use only 0-4  
String s_val_old[]={"10","11","12", "13","14","15","16","17", "18","19"}; // now use only 0-4   
String X[]={"0","1","2", "3","4","5","6","7", "8","9"};
String morning[]={"4.00","8.50", "7.50",  "6.40", "5.20", "4.20", "3.40", "4.00", "5.00", "6.00", "7.00", "8.00",  "9.00"};
String evening[]={"21.00","16.30", "17.30","18.30","19.30","20.30","21.20","21.00","20.00","18.40","17.30"," 16.30","16.00"};


int dBeep=LED_BUILTIN;
int vd0=0;
int vd0_old=0;
int Tc_On=0;
int Tc_Off=0;
int BegWith=0;
int T_start = 0; //hhmm
int T_fin =0;    //hhmm
int Auto = 0; // 0/1 -> on/off
int Auto_old=0; // 0/1 -> on/off

int Marker = 1; // Show restart - reconnect power 220 -> start this scetch

float TEM_3H=0;
int A_0=0;
int TEM_YCT=0;
int CBET_YCT=0;

int Power=0;
int Power_old=0;
int Timer=0;
int Retro=0;
int T_scan=0;
int T_scan_old=0;



String str1;
String str2;
unsigned char* buf1 = new unsigned char[100];
unsigned char* buf2 = new unsigned char[100];
unsigned char* buf3 = new unsigned char[100];

String par_data="";
String cmd="";
String var="";
String strout=""; // included format
String gline="";

String names[] = { "ssid","password","proxy_ip","proxy_port","bortx_id","bortx_pass","Vars1","Vars2","Vars3","Vars4","Vars5","T_scan","reserv1","reserv2","TimeZone", "outputM","time_on_off","mission","email","tuning_RW","tuningSIGN","tuningNOW","tuningPANEL","S0","S1","S2","S3","S4", "last_error" ,"Auto", "p1","p2","p3","p4","p5","p6","p7","p8","p9","p10","p11","p12","p13","p14","p15","p16","p17","p18","p19","p20"};
int Nnames=50; // nomber  array's elements. Former array TuningNOW, mission, tuningSHOW, tuningPANEL, tuning_RW are taken. 

ESP8266WebServer server(80);


/////////////////////////////// S E T U P ////////////////////////////////////
void setup() {

// Start up the library one-ware ds18b20 
sensors.begin();
Serial.begin(115200);
delay(10);
Serial.println("\n\n++++++++++++++++++++push return ++++++++++++++++++++\n");



//swSer.begin(9600);
//swSer.begin(38400);

delay(10);
Serial.println();
Serial.println("############ ChipId #############");
Serial.println(String(ESP.getChipId()));
Serial.println("URL dashboard: http://bortx.ru/"+String(ESP.getChipId()));
Serial.println("############ ChipId #############");

// prepare ssid & password from eprom

String start_mode=E_read(mem("mission"));

//////////////  String -> const
if (  wifi_ssid != "" &&  wifi_ssid !=  E_read(mem("ssid"))  )
{
E_write(wifi_ssid,mem("ssid"));
E_write(wifi_password,mem("password"));
start_mode="demo";
};


//Serial.println("read eprom 3000:"+E_read(3000));


//////////////////////////   dialog (input)   ////////////////////////
Serial.println("\n\n++++++++++++++++++++push return ++++++++++++++++++++\n");
Serial.println("+++++++++++++++++++++++++++ to +++++++++++++++++++++++++\n");
Serial.println("+++++++++++++++++++++++++   dialog  ++++++++++++++++++++\n");
delay (1000);

if (Serial.available() && Serial.read() == 102 && Serial.read() == 57){ // wait for input
String tmpm=dialog("SSID =? (if notihg then old value)");
Serial.println("======================= your input:"+tmpm);
if (tmpm !=""){E_write(tmpm,mem("ssid"));};

tmpm=dialog("Password SSID =? (if notihg then old value)");
Serial.println("======================= your input:"+tmpm);
if (tmpm !=""){E_write(tmpm,mem("password"));};                    
                       
answer_bortx_id=dialog("bortx_id =? (if notihg then old value)");
Serial.println("======================= your input:"+tmpm);

answer_clear_eprom=dialog("Reset eprom =? (if 'y' => reset)");
Serial.println("======================= your input:"+tmpm);
        
                    
                                                                                                }; // end input , dialog

if (start_mode=="demo"){answer_bortx_id="demo"; answer_clear_eprom="y";};



Serial.println("");
Serial.println("Nnames="+String(Nnames));

bortx_id=E_read(mem("bortx_id"));

if  (answer_clear_eprom.indexOf("y") != -1)
{
//E_write("",mem("ssid"));
//E_write("",mem("password"));
E_write("0",mem("proxy_ip"));
E_write("0",mem("proxy_port"));
E_write(bortx_id,mem("bortx_id"));
E_write("",mem("bortx_pass"));
E_write("D4=LED;",mem("Vars1"));
E_write("A0=Uao;",mem("Vars2"));
E_write("k=0.005;b=0;",mem("Vars3"));
E_write("0",mem("Vars4"));
E_write("0",mem("Vars5"));
E_write("2",mem("T_scan"));
E_write("Reset [00:00:00]<11-11-11>{0}",mem("time_on_off"));
E_write("0123!5678",mem("tuningSIGN"));
E_write("000000000000000",mem("outputM"));
E_write("012345678",mem("tuningNOW"));
E_write("unknown_mission",mem("mission"));
E_write("0",mem("last_error"));
E_write("reg@bortx.ru",mem("email"));
E_write("RWWWWWRRRRWWWWW",mem("tuningPANEL"));
E_write("111110000",mem("tuning_RW")); // like for verision 18
E_write("0",mem("Auto")); 

E_write("",mem("reserv1")); 
E_write("",mem("reserv2")); 
E_write("3",mem("TimeZone"));

for (int i=1;i<=20;i++){E_write("",mem("p"+String(i)));}; // script to zero
for (int i=0;i<=4;i++){E_write("0",mem("S"+String(i)));}; // S to zero
// small script autoincrement S4 
E_write("1:$S4=$random*100;",mem("p1"));
E_write("trans{1};",mem("p2"));

};


Serial.println();
Serial.println(String(LED_BUILTIN));
Serial.println(String(D4));

bortx_pass=E_read(mem("bortx_pass"));
mission=E_read(mem("mission"));
last_error=E_read(mem("last_error"));
Power=d_val[4];
Power_old=Power;

Format="format="+SetFormat()+"T_scan,Marker;";


Auto=E_read(mem("Auto")).toInt();
Auto_old=Auto;

if (Auto == 1 && preproc()=="error") {Auto=0;}

tuningSIGN=E_read(mem("tuningSIGN"));
tuningNOW=E_read(mem("tuningNOW"));
tuningPANEL=E_read(mem("tuningPANEL"));
tuning_RW=E_read(mem("tuning_RW"));
outputM=E_read(mem("outputM"));
if (outputM.length() < 15){outputM="000000000000000";E_write("000000000000000",mem("outputM"));};

//set s_val
for (int i=0;i<=4;i++){s_val[i]=E_read(mem("S"+String(i)));};


//set d_val

// d0-d8 set mode input(default)/output(W)/INPUT_PULLUP(U)
for (int i=0;i<=8;i++)      {
if (tuning_RW[i]=='1')
{
pinMode(d_name[i], OUTPUT);
if (outputM[i]=='0'){d_val[i]=0;}else{d_val[i]=1;};
}else
{if (tuning_RW[i]=='u' || tuning_RW[i]=='U') {pinMode(d_name[i],INPUT_PULLUP);};};// default mode=INPUT
                            };   // end for
  

T_scan=E_read(mem("T_scan")).toInt();
if (T_scan<1){T_scan=1;};
if (T_scan>300){T_scan=300;};
T_scan_old=T_scan; 

bortx_id=E_read(mem("bortx_id"));

pinMode(LED_BUILTIN, OUTPUT);
digitalWrite(LED_BUILTIN, LOW);

pinMode(dBeep, OUTPUT);
tone(dBeep,3000,500);

 
//E_write("9035302240",mem("password"));
 
 Serial.println();
 Serial.println(E_read(mem("password")));

// setting host && httpPort - correction if proxy was setted
String proxy_name=E_read(mem("proxy_ip")); 
if  (proxy_name.indexOf(".") > -1) {status_proxy=1; host_source=proxy_name;httpPort=E_read(mem("proxy_port")).toInt();ConnHostPort6110=httpPort;ConnHostPort6010=httpPort;}
  
pre_sync(); // correct time with time_on_off [11:33:11] and {sec}; 
  
  // We start by connecting to a WiFi network

  Serial.println();
  

  /* Explicitly set the ESP8266 to be a WiFi-client, otherwise, it by default,
     would try to act as both a client and an access-point and could cause
     network-issues with your other WiFi-devices on your WiFi-network. */


WiFi.mode(WIFI_AP_STA);

//////////////  String -> const
str1 = "BortX:"+bortx_id; str1.getBytes(buf1, 100, 0);
const char *ssidAP = (const char*)buf1;

Serial.println("Creating AP:<"+String(ssidAP)+">");

WiFi.softAP(ssidAP, ""); //  -  YMHAR_PO3ETKA PULS/WIFI
  

server.on("/", handleRoot);
server.begin();
Serial.println("HTTP server started");

str1 = E_read(mem("ssid"));   

str1.getBytes(buf1, 100, 0);
const char *ssid = (const char*)buf1;
Serial.println("ssid:"+str1);
//////////////  String -> const

str1 = E_read(mem("password"));   

str1.getBytes(buf2, 100, 0);
const char *password = (const char*)buf2;
Serial.println("password:"+str1);

 Serial.println();
 Serial.print("Connecting to ROUTER:<");
 Serial.println(String(ssid)+"><"+String(password)+">");

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
  server.handleClient(); 
  control();
  s_delay (100);
  
  go_stet2();
  digitalWrite(LED_BUILTIN,stet2);  
  Serial.print(".");
  if (millis()>600000){Serial.println("\n\r*** No wifi connection 10 min. Reset. ***"); myResetFunc();};
                                          };

  // Status="connected";
  Serial.println("");
  Serial.println("#WiFi_connected");
  Serial.println("IP address: ");
  
  // define bortx_id ............ leter
  
if  (answer_bortx_id.indexOf("demo") != -1)
{
bortx_id=String(ESP.getChipId());
Serial.println("bortx_id="+String(ESP.getChipId()));

Serial.println("#WiFi_connected");
E_write(bortx_id,mem("bortx_id"));
};

if  (answer_bortx_id.indexOf("demo") == -1 && answer_bortx_id != ""  )
{
bortx_id=answer_bortx_id;
E_write(bortx_id,mem("bortx_id"));
};
  

Serial.println("#bortx_id="+bortx_id+";");

   
  Serial.println(WiFi.localIP());
sync();
mail("start_$ntime_$udate");

};

///////////////////////////////////////////////////////////////////////////////////////// LOOP
///////////////////////////////////////////////////////////////////////////////////////// LOOP
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
WiFiClient client6010; //connect to port 6010
WiFiClient client6110; //connect to port 6110
//String Format="format="+SetFormat()+"T_scan,Marker;";

///////////////////// ///////////////////////////////////////////////L O O P/////////////
/////////////////////////////////////////////////////////////////////////////////////////


void loop() {
server.handleClient();  // Auto by get info from browser, processing in  handleRoot()
s_delay(50);
//Serial.print(",");
String str3 = host_source; str3.getBytes(buf3, 100, 0);
const char *host = (const char*)buf3;

     
line="";
while (client6010.available()) {sbuf=client6010.read();line=line+String(sbuf);};
gline=line;
if (line !=""){
long_polling6010=0;   
Serial.println("recieved:**<"+line+">"); //recieved:
if (line.indexOf("ping=")>0 || line.indexOf("command=")>0)
{last_ping=millis();Serial.println("Command or Ping recieved");}; //time last ping
              };


if (long_polling6010==0){  
 Serial.print(String(host)+"/"+String(ConnHostPort6010));
 
if (client6010.connect(host, ConnHostPort6010)) 
{Serial.println(" connection OK");
s_delay(50);
String url = "http://bortx.ru:6010?id="+bortx_id+";device=esp;pass="+bortx_pass+";";
client6010.print(String("GET ")+url+" HTTP/1.1\r\nHost: "+host+"\r\nConnection: close\r\n\r\n");
long_polling6010=1;   
}else{s_delay(50);Serial.println(" connection failed");};
                      }; 

if (line !=""){get_command();};

control();

if (millis()<t_stop_scan && send_cito==0 && Marker==0){return;};


////////////////////////////////////////////////////////////////////////////       SEND DATA

 Serial.print(String(host)+"/"+String(ConnHostPort6110));
 if (client6110.connect(host, ConnHostPort6110)) 
 {Serial.println(" connection OK");}else{Serial.println(" connection failed");};
s_delay(50);
 set_data();
 
 String url = "http://bortx.ru:6110?id="+bortx_id+";"+Format+";"+par_data+";device=esp;pass="+bortx_pass+";";
 Format=""; // use Format only one time
 //Serial.println("url="+url);;
 Serial.println("url=bortx.ru/"+bortx_id);
 // This will send the request to the server
 client6110.flush();
 client6110.print(String("GET ")+url+" HTTP/1.1\r\nHost: "+host+"\r\nConnection: close\r\n\r\n");
 
t_stop_scan=(1+int(millis()/(T_scan*1000)))*T_scan*1000;
Ptime=0; // counter scan time
Serial.println(String(T_scan));
Serial.println(String(millis()));
Serial.print("t_stop_scan:"+String(t_stop_scan));

if (Marker==1){  // send status
delay(1000);
Marker=0;
t_httpUpdate_return ret;
uri="http://bortx.ru/cgi-bin/write_file.cgi?file=/";
uri2=uri;
uri=uri+bortx_id+"/"+ch+"/status.txt&append=no&text=version=%27";
uri2=uri2+"status.txt&append=no&text=version=%27";
  uri=uri+ver+"%27%3BtuningPANEL=%27"+tuningPANEL+"%27%3Bip_local=%27"+WiFi.localIP().toString()+"%27%3Bmission=%27"+mission+"%27%3B%2F%2F"+bortx_id;
uri2=uri2+ver+"%27%3BtuningPANEL=%27"+tuningPANEL+"%27%3Bip_local=%27"+WiFi.localIP().toString()+"%27%3Bmission=%27"+mission+"%27%3B%2F%2F"+bortx_id;


if (status_proxy){
ret= ESPhttpUpdate.update(host_source,httpPort, uri);
ret= ESPhttpUpdate.update(host_source,httpPort, uri2);
}else{
ret = ESPhttpUpdate.update(uri);  
ret = ESPhttpUpdate.update(uri2); 
      };
            
            }; // end marker 
return;
};



////////////////////////////////////////////////////
 
String val(String mes,String key)
{
//Serial.println("val:"+mes);
int pos1=mes.indexOf(key+"=");
//Serial.println("pos1:"+String(pos1));
if (pos1 == -1){return "";};
//Serial.println("calc_pos2");
int pos2=mes.indexOf(";",pos1+1);
//Serial.println(String(pos1)+"&"+String(pos2));
String tmp=mes.substring(pos1+1+key.length(),pos2);
//Serial.println("="+tmp+"=");
return tmp;
};

/////////////////////////////////
 

/////////////////////////////////////////////// EWRITE
void E_write(String x,int from) {
EEPROM.begin(4096); 
  
  // put string to eprom from address n:
int i=-1;

while (1)
{
i=i+1;
EEPROM.write(from+i, x[i]);
if (x[i] == 0  || from+i > 4000) {break;};
};
EEPROM.commit();
EEPROM.end(); 
                                }; //end Ewrite



/////////////////////////////////////////////// EREAD V48 no more 40 sym read
String E_read(int from)   {
  // get string from eprom from address 'from':
EEPROM.begin(4096); 
int i=-1;
String y="";

while (1)
  {
i=i+1;
if (EEPROM.read(from+i)==0 || from+i > 4000 || i > 40){break;};

y=y+char(EEPROM.read(from+i));
//Serial.print(".");
//Serial.println(EEPROM.read(from+i));

  }; //Wend
//Serial.print(y);
EEPROM.end(); 
return y;
      }; // end E_read

///////////////////////////////////////////

int mem(String name)
{
for (int i=0;i<=Nnames;i++)
{
if (names[i] == name) {return 40*i;};
};
Serial.println ("\n\r***\n\r<"+name+">Func mem() -> NO SUCH NAME\n\r*****");
E_write("no_such_name_"+name+"_?",mem("last_error"));

//delay(5000);
//resetFunc();
return 3000;
};

//////////////////////////////////////////////////
  
////////////////////////////////////////////////////////////////////////////////////////////// 
////////////////////////////////////////////////            INTERUPT   /////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////

void handleRoot() {
String tmpstr="";
if (server.argName(0)== "restart") {myResetFunc();};
if (server.args() > 0) {  // write argumentes to eprom
for (int i=0;i<=server.args();i++){
  if (server.arg(0)=="clear") {E_write("",mem(names[i]));}else
  {
 tmpstr=server.arg(i);
 tmpstr.trim();
 tmpstr.replace(" ","");
//********                      bortx
if (tmpstr.length()>39){tmpstr="No_more_39_chars!";}; 
//*****************
//if (tmpstr != "") {E_write(tmpstr,mem(names[i]));};
E_write(tmpstr,mem(names[i]));
  
  };
// if (server.arg(0).substring(0,0)!=" " && server.arg(0)!="") {E_write(server.arg(i),mem(names[i]));}; 
 
                        
                        }; // end cicle for  
  
                        
                        }; // end if nomber of argumentes > 0 


//String href=location.href;
// server.argName(i) //notes name arguments

String  txt="<meta name='viewport' content='width=240'>";
txt=txt+"<meta http-equiv='Cache-Control' content='no-cache' />";
txt=txt+ver;


txt=txt+"<body style='font-family:Courier;'> Configuration<hr>";  
//txt=txt+"<script>document.write(' Configuration<hr><form name=form1 method=POST action='+location.href+' >');</script>";  
txt=txt+"<br><u>"+String(int(millis()/1000))+" sec working</u><br>"; 
if (WiFi.status() == 3)
{txt=txt+"<br>Wifi_Router: Connected. IP="+ WiFi.localIP().toString() +"<br>";}else
{txt=txt+"<br>Wifi_Router: Not connected.  <br>";};

if (error != ""){txt=txt+"<br>ERROR: "+error+"<br>";};

txt=txt+"<br>bortx_id: "+String(bortx_id)+"<br>";
if (status_proxy==1)
{txt=txt+"<br>proxy/port: "+String(host_source)+"/"+String(httpPort)+"<br>";};

txt=txt+"<script>document.write('<form  method=POST action='+location.href+' >');</script>";  
txt=txt+"<input type='hidden' name='restart' value='restart'>. . .<input type=submit value=RESTART></form>";

txt=txt+"<script>document.write('<form name=form1 method=POST action='+location.href+' >');</script>";  
txt=txt+"<table width=100% border=1><col width='10'> <col width='10'>";
for (int i=0;i<Nnames;i++)
{
if (i==2){txt=txt+"</table><hr>. . .<input type='submit' value='SAVE'><table width=100% border=1><col width='10'>";};

str1=E_read(mem(names[i]));
str2="";
if (names[i] == "bortx_id" || names[i] == "outputM" || names[i] == "time_on_off") {str2="readonly";};
txt=txt+"<tr><td>"+names[i]+"<td>"; txt=txt+"<input "+str2+" value='"+str1+"'  name='"+names[i]+"'>";
}; // end cycle for


Serial.println(txt);
//server.send(200,"text/html", txt+res_wifi_scan);
server.send(200,"text/html", txt);
Serial.println(server.args());


};// end handleRoot()

////////////////////////////////////////////////
void go_stet2(void)
{
stet2++;
if (stet2 == 2){stet2=0;};

};


/////////////////////////////////

void led(int led_status) /////// led on/off 
{
digitalWrite(LED_BUILTIN,led_status);
};

/////////////////////////////////            ***    C O N T R O L   ***control

void control() /////// control 
{

Power=d_val[4];
if ( Power == 1 && millis()-start_led>975){digitalWrite(LED_BUILTIN, HIGH);};
if ( Power == 0 && millis()-start_led>25){digitalWrite(LED_BUILTIN, HIGH);};


send_cito=0; 


//----------------------------------------                    
 A_0=analogRead(A0);

for(int i=0;i<=8;i++){
if (tuning_RW[i]!='1'){continue;};
if (d_val[i] != d_val_old[i])
{
if (d_val[i]) {outputM[i]='1';}else{outputM[i]='0';};
send_cito++;
d_val_old[i]=d_val[i];
if (tuningSIGN[i]!='!'){digitalWrite(d_name[i], d_val[i]);}else {digitalWrite(d_name[i], !d_val[i]);};
};
                     }; // end for

for(int i=0;i<=8;i++){
if (tuning_RW[i]=='1'){continue;};
d_val[i]=digitalRead (d_name[i]); 
//Serial.println(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>"+String(LOW)+String(HIGH)+String(d_val[i]));

if (d_val[i] != d_val_old[i])
{
if (tuningNOW[i] != '!') {send_cito++;}; // a fast changing vars don't send by changing 
d_val_old[i]=d_val[i];
};
                     }; // end for

for(int i=0;i<=4;i++){

if (s_val[i] != s_val_old[i])
{
if (tuningNOW[i+9] != '!') {send_cito++;}; // a fast changing vars don't send by changing 
s_val_old[i]=s_val[i];
};                     
                     }; // end for

if (Auto!=Auto_old){Auto_old=Auto;send_cito++;};                     

if (send_cito > 0){E_write(outputM,mem("outputM"));};
if (T_scan != T_scan_old){send_cito++;T_scan_old=T_scan;};  
//-----------------------------------------------------------------

long sec=int(millis()/1000);
if (last_sec_control == sec){return;}; 
last_sec_control = sec;
Serial.println(" -----  >C O N T R O L< :"+String(sec)+ "   TimeOut:"+String((millis()-last_ping)/1000));
Serial.println(String(millis())+"/"+String(count_time));

X[0]=String(X[0].toInt()+count0);
count0=0;
count_time=0;

Ptime++; // inc time of scan

if (sec%100 == 0 && sec>=100) // check on even
{
tmpt="~"+E_read(mem("time_on_off"));
E_write(between(tmpt,"~",">")+">{"+String(sec)+"}",mem("time_on_off"));  
};

if (Auto==1){proc();};

if (millis()-last_ping>150000)
{
Serial.print("******** NO replay from 6010 150 sec => Restart **********");
E_write("TimeOut_150",mem("last_error"));
resetFunc();
};

digitalWrite(LED_BUILTIN, LOW);
start_led=millis();

};


/////////////////////////////
 
void myResetFunc()
{
status_save();
resetFunc();    
};

//////////////////////////

void status_save()
{

};

//////////////////////////

void set_data()
{

//par_data="&call=data="+String(A_0*coeff_k+coeff_b);
par_data="data="+String(A_0*coeff_k+coeff_b);

// d0-d8 set variables

for (int i=0;i<=8;i++)    { //

if (tuning_RW[i]=='1')
{
                // -----------------  do it case write
	    par_data=par_data+","+String(d_val[i]);
                // ------------------
};

if (tuning_RW[i]!='1')
{
 	
 	// ------------------------ do it case read
 	if (tuningSIGN[i]!='!'){ 
 	par_data=par_data+","+String(d_val[i]);
	 }else
	{
	par_data=par_data+","+String(!d_val[i]);
	};
             // ------------------------
 };            
                                       };   //end for

// add s_val
for (int i=0;i<=4;i++){par_data=par_data+","+s_val[i];};

// add Auto
par_data=par_data+","+String(Auto);

int Marker_err=Marker*1000+(millis()-last_ping)/1000;
par_data=par_data+","+String(T_scan)+","+String(Marker_err)+","+String(syn)+";";

};


///////////////////////////
/////////////////////////////////            ***    G E T _ C O M M A N D  ***
void get_command()     
{

long tmp=0;
String cmd="";
String var="";

Serial.print("commanda:<"+gline+">;");
cmd=val(gline,"command");
var=val(gline,"var");

if (cmd=="reset"){Serial.print("******** RESET BY COMMAND FROM PANEL **********"); myResetFunc();};

if (cmd.indexOf("epromr") != -1){epromr();};
if (cmd.indexOf("epromw") != -1){epromw();bortx_pass=E_read(mem("bortx_pass"));};

// another CHANGE

if (cmd.indexOf("update") != -1){
name_bin = cmd.substring(7,30);

Serial.print("******** UPDATE BY PANEL COMMAND FROM <"+ver+"> WITH <"+name_bin+">  **********");
t_httpUpdate_return ret;

if (status_proxy){
ret= ESPhttpUpdate.update(host_source,httpPort, "http://bortx.ru/firmware/"+name_bin);
}else{
ret = ESPhttpUpdate.update("http://bortx.ru/firmware/"+name_bin);};

Serial.println("");
Serial.println(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>");
Serial.println("Result update:"+String(ret));
Serial.println(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>");
Serial.println("");
delay (5000);
myResetFunc();
};

  
if (cmd != "")                                  { // begin if command ;
tone(dBeep,3000,500);
if (cmd == "off") {cmd="0";}
if (cmd == "on") {cmd="1";}
tmp=cmd.toInt();
syn=val(gline,"syn").toInt();

if (var=="Auto" && tmp==1){
if ( preproc()!="error" )
{Serial.println("PREPROC=OK"); Auto=1;E_write(String("1"),mem("Auto")); E_write("",mem("last_error"));}
else{Auto=0;E_write("0",mem("Auto"));}; 
                            }; // end if var Automat == 1

if (var=="Auto" && tmp==0) 
{Auto=0;E_write("0",mem("Auto"));}; // end if var Automat == 0
               
if (var=="T_scan"){if(tmp<1){tmp=1;};if(tmp>300){tmp=300;};T_scan=tmp;E_write(String(tmp),mem("T_scan"));};

for (int i=0;i<=8;i++) // only for input
{
if (tuning_RW[i]!='1'){continue;}; // only for input
//Serial.println(name_var_D[i]);
if (var == name_var_D[i]){
//Serial.println("It is!!!!!!!!!!!!!!!!!!");
if(tmp<0){tmp=0;};
if(tmp>1){tmp=1;};
d_val[i]=tmp;            };
};


for (int i=0;i<=4;i++) // only for input
{
if (var == name_var_S[i]){
E_write(cmd,mem("S"+String(i)));
s_val[i]=cmd;
                         }; // end if

};  // end for
                                                          
                                                          
                                                          }; // end if command

  
};

//////////////////////////////////
  /////////////////
  // WiFi.scanNetworks will return the number of networks found
  /////////////////

String wifi_scan(){
WiFi.mode(WIFI_STA);
WiFi.disconnect();
String txt="<br><hr>";
  int n = WiFi.scanNetworks();
  Serial.println("scan done");
  if (n == 0) {Serial.println("no networks found");txt=txt+"no networks found"; return txt;};
  
    Serial.print(n);
    Serial.println(" networks found");
    for (int i = 0; i < n; ++i)
    {
      // Print SSID and RSSI for each network found
      Serial.print(i + 1); txt=txt+String(i+1)+":";
      Serial.print(": ");
      Serial.print(WiFi.SSID(i)); txt=txt+WiFi.SSID(i)+ "/" +WiFi.RSSI(i)+"<br>" ; 
      Serial.print(" (");
      Serial.print(WiFi.RSSI(i));
      Serial.print(")");
      Serial.println((WiFi.encryptionType(i) == ENC_TYPE_NONE)?" ":"*");
      
      delay(10);
    };
   Serial.println("");
return txt;
                          };

/////////////////////////////////////////

String SetFormat(){  // Set Format and parallel set coefficients k,b,
String item="";
String strf="";
strf=E_read(mem("Vars1"))+E_read(mem("Vars2"))+E_read(mem("Vars3"))+E_read(mem("Vars4"))+E_read(mem("Vars5"));

item=val(strf,"k");
if (item != ""){coeff_k=item.toFloat();};
item=val(strf,"b");
if (item != ""){coeff_b=item.toFloat();};

item=val(strf,"A0");
if (item==""){strout="A0,";}else{strout=item+",";};

for (int i=0;i<=8;i++)
{
item=val(strf,name_var_D[i]);
if (item==""){strout=strout+name_var_D[i];}else{strout=strout+item;name_var_D[i]=item;};
strout=strout+",";
};

for (int i=0;i<=4;i++) // for  vars S - setpoints
{
item=val(strf,name_var_S[i]);
if (item==""){strout=strout+name_var_S[i];}else{strout=strout+item;name_var_S[i]=item;};
strout=strout+",";
};

strout=strout+"Auto,";
return strout; 
};

///////////////////////////////
/*
String setSHOW(String xshow){
String strout="";
int J=-1;
for (int i=0;i<xshow.length();i++)
{
if (J==-1){strout=strout+xshow[i];}else{if(tuningSHOW[J] != '!'){strout=strout+xshow[i];};};
if (xshow[i] == '=' || xshow[i] ==','){J++;}
};
return strout;
                        };
///////////////////////////////

*/

void epromr(void){
t_httpUpdate_return ret2;
String txt="";
for (int i=0;i<Nnames;i++)
{
str1=E_read(mem(names[i]));
txt=txt+"m['"+names[i]+"']='"+str1+"';%0A";
//string.replace(substring1, substring2)
txt.replace(";","%3B");
txt.replace("'","%27");
txt.replace("+","%2B");
//txt.replace("[","%5B");
//txt.replace("]","%5D");
};
//txt="good_day\n";
uri="http://bortx.ru/cgi-bin/write_file.cgi?append=no&file=/"+bortx_id+"/"+ch+"/eprom.txt&text="+txt;
//String uri="http://bortx.ru/cgi-bin/write_file.cgi?file=/";

uri2=uri;
uri2=uri2+"eprom.txt&append=no&text="+txt;
Serial.println("uri:"+uri);
if (status_proxy){
ret2= ESPhttpUpdate.update(host_source,httpPort, uri);
//ret2= ESPhttpUpdate.update(host_source,httpPort, uri2);
}else{
ret2 = ESPhttpUpdate.update(uri);  
//ret2 = ESPhttpUpdate.update(uri2); 
      };
};
///////////////////////////////

void epromw(void){
      HTTPClient http;
http.begin("http://bortx.ru/"+bortx_id+"/"+ch+"/eprom.txt"); //HTTP

        int httpCode = http.GET();

        // httpCode will be negative on error
        if(httpCode > 0) {
            // HTTP header has been send and Server response header has been handled
            Serial.printf("[HTTP] GET... code: %d\n", httpCode);

            // file found at server
            if(httpCode == HTTP_CODE_OK) {
                payload = http.getString();
                Serial.println(payload+"\n");

payload.replace("\n","|");
payload.replace("\r","");
payload.replace(" ","");
payload.replace("m['","");
payload.replace("']='","=");
payload.replace("';","");

//for (int i=0;i<150;i++){
//Serial.println(payload[i]);
//};return;


// write to eprom                 
String str_name="";    
String str_value="";
int name_value=1; // 1 - pick var's name until "=" , 2 - pick var's value until "|" 
for (int i=0;i<= payload.length();i++){
if (payload[i] == '=' && name_value==1){name_value=2;continue;};
if (payload[i] == '|') {
Serial.println(str_name+"="+str_value);
if (str_name != "") {E_write(str_value,mem(str_name));};
str_name="";    
str_value="";
name_value=1;
continue;              }; //end if

// any another symbol
if (name_value==1){str_name=str_name+payload[i];  continue;}
if (name_value==2){str_value=str_value+payload[i];  continue;}    
    
                                      };   // end for
          
            }
        } else {
            Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
        }

//        http.end();



 http.end();

                 };


/////////////////////////////////////////////////////
String dialog(String prompt){

char inChar;
String bu="";
Serial.println(prompt+"\n\n\n");
while (Serial.available()!=0){inChar =Serial.read();}; // clear bufer Serial
while (Serial.available()==0){s_delay(100);}; //wait string from terminal
while (Serial.available() != 0){
inChar = Serial.read();
bu += inChar;
                               };
               
bu.replace("\n","");
bu.replace("\r","");
bu.replace(" ","");
return bu;
};


////////////////////////////////////////
String preproc(void){
st="";
iukaz=0;
pozd; //position of ":"
for (int i=1;i<=20;i++) // create ukaz[iukaz] array of labels
{
st=E_read(mem("p"+String(i)));
// check ignore oper if begin with /
if (st[0] == '/'){continue;};
// search "!="
if (st.indexOf("!")>-1){E_write("use_less_more_brackets_error_p"+String(i),mem("last_error"));return "error";};
// search ":"
pozd=st.indexOf(":");
if (pozd ==-1){continue;}
if (pozd == 0){E_write("error_p"+String(i),mem("last_error"));return "error";};
st=st.substring(0,pozd);
if (st.length()<=0){E_write("err_no_Label_p"+String(i),mem("last_error"));return "error";};
iukaz=st.toInt();
if (iukaz<1 || iukaz >20){E_write("Label_less_1_or_more_20_p"+String(i),mem("last_error"));return "error";};
Serial.println(String(iukaz)+"=p"+String(i));
ukaz[iukaz]=i;

}; // end for

//check link from label in {} to label L:
String st1;
for (int i=1;i<=20;i++)
{
st=E_read(mem("p"+String(i)));
// check ignore oper if begin with /
if (st[0] == '/'){continue;};
//check if S only "$S" or "[S"
for (int k=0;k<=39;k++)   {
if (st[k]==0){break;};
if (st[k] == 'S' && st[k-1]!= '$' && st[k-1]!= '[')
{E_write("use_low_case_if_not_var_$Sn_line_p"+String(i),mem("last_error"));return "error";};
if (st[k] == 'X' && st[k-1]!= '$' && st[k-1]!= '[')
{E_write("use_low_case_if_not_var_$Xn_line_p"+String(i),mem("last_error"));return "error";};
                          };

//Serial.println("strout="+strout);
st1=st; // check includ [xxx] Format
String tstr=between(st1,"[","]");
if (st1.indexOf("[")>-1  && strout.indexOf(tstr) == -1){E_write("no_such_var_in_[]_line_p"+String(i),mem("last_error"));return "error";};

st1=st; // check placing ; and placing () {} at the same time

st1.replace("$S","");
st1.replace("$X","");
st1.replace("$D","");
st1.replace("$A","");
st1.replace("$timer","");
st1.replace("$ntime","");
st1.replace("$udate",""); 
st1.replace("$retro","");
st1.replace("$random","");
st1.replace("$input","");
st1.replace("$temp","");
st1.replace("$morning","");
st1.replace("$evening","");
st1.replace("$ptime","");

if (st1.indexOf("$") > -1)
{E_write("No_such_var_with_$_line_p"+String(i),mem("last_error"));return "error";};

st1=st; // check placing ; and placing () {} at the same time
st1.replace("{","");
st1.replace("}","");
st1.replace("(","");
st1.replace(")","");
st1.replace(";","");
if (st.length()-st1.length() != 5 && st.length()-st1.length()!=3  &&  st.length()-st1.length()!=1  &&      st1.length()>0 )
{E_write("Look_at_{}();_in_p"+String(i),mem("last_error"));return "error";};
// search "{"
if (st.indexOf("{") == -1){continue;};

st=between(st,"{","}");
//Serial.println("st="+st+"pozd"+String(pozd));
//if (st.length()<=0){E_write("error_p"+String(i),mem("last_error"));return "error";};
iukaz=st.toInt();
Serial.println("i="+String(i)+" label_iukaz"+String(iukaz));
if (iukaz<1 || iukaz >19){E_write("bad_link_p"+String(i),mem("last_error"));return "error";};
if (ukaz[iukaz] ==0){E_write("bad_link_p"+String(i),mem("last_error"));return "error";};
Serial.println(String(iukaz)+"=p"+String(i));

}; // end for

Timer=0;
iukaz=1;
ukazfaz=1;
}; //end func
 
 //////////////////////
void proc(void){
//return;
Serial.println(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>  enter proc >>>>>>>>>>>>>>>>>>>>>>>>>>>>");
Serial.println("timer == "+String(Timer)); 
int ii;
for (int i=ukazfaz;i<=20;i++)
{
st=E_read(mem("p"+String(i))); //read operator 
// check ignore oper if begin with /
if (st[0] == '/'){continue;};

if (st.indexOf(":") > -1 && i != ukazfaz) {Timer++; return;}; // end of phase

pozd=st.indexOf(":"); //delete label
st=st.substring(pozd+1); 
st.replace("if(","("); //delete if
st.replace("goto","trans"); //delete goto
st.replace("stop",""); //delete goto
st.replace(")trans{","){"); //unconditional trans
st.replace("trans{","(=){"); //unconditional trans
if (st.indexOf("=") == -1 && st.indexOf("{") == -1 && st.indexOf("(") == -1 ){continue;}; // the null operator if 

// check ";" , remove all after ";"
pozd=st.indexOf(";"); //delete label
if (pozd == -1){
Auto=0;
E_write("0",mem("Auto"));
E_write("No_';'_in_p"+String(iukaz),mem("last_error")); 
return;       };
st.remove(pozd+1); //delete all after ";"

if (Timer == 0 && st.indexOf("{") == -1) { // do active operator on string st
Serial.println("timer=="+String(Timer)+ " st="+st);
activ();
}; //end if activ
if (Timer > 0 && st.indexOf("{") > -1){ // do trans (if) operator 
Serial.println("timer=="+String(Timer)+ " st="+st);
// if trans w/o condition
//Serial.println("^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^="+between(st,"(",")")+"=");
//if (between(st,"(",")")==""){st.replace(";","(=);");};
ii=trans();
if (ii > 0){ // new phase
//note("NEW_PHASE:"+String(ii));
Retro=Timer;
Timer=0;
ukazfaz=ukaz[ii];
return;    };

}; //end if trans

};// end for
Timer++; // end phase without '*' or ':' 
return;
}; //end func

/////////////////////////////////////////
void activ(void){ // do active operation st=$D1=1; or email M(mes);
// oper in st w/o label:

Serial.println(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>  enter activ >>>>>>>>>>>>>>>>>>>>>>>>>>>>");
String outs=st;
Serial.println("Out="+outs);

if (st.substring(0,2) =="$D")
{
String sn=String(st[2]);
int n=sn.toInt(); 
outs=substitution(between(outs,"=",";"));
outs=calculation(outs);
d_val[n]=outs.toInt();
return;
};

if (st.substring(0,2) =="$S")
{
String sn=String(st[2]);
int n=sn.toInt();
outs=substitution(between(outs,"=",";"));
outs=calculation(outs);
s_val[n]=outs;
return;
};

if (st.substring(0,2) =="$X")
{
String sn=String(st[2]);
int n=sn.toInt();
outs=substitution(between(outs,"=",";"));
outs=calculation(outs);
X[n]=outs;
return;
};

if (st[0]=='M')     // send Email
{
mail(between(st,"(",")"));
return;
};

if (st[0]=='N')    // note to panel
{
note(between(st,"(",")"));
return;
};


if (st[0]=='E')   // story var $S0-$S4 - in eprom ; for ex. E($S4); use only $Sn but not name in [] 
{
String sn=String(st[4]);
int n=sn.toInt();
if (n<0 || n>4 ){return;};
E_write(s_val[n],mem("S"+String(n)));
return;
};
               
                }; // end func

///////////////////////////////////////// TRANS TRANS TRANS TRANS TRANS TRANS TRANS TRANS                           

int trans(void){ // do trans operation if ([Temp]>30){10}; rerurns new phase if return>0 
Serial.println(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>  enter trans >>>>>>>>>>>>>>>>>>>>>>>>>>>>");
//get name left part (Temp) and right part (30) 
String st1=st;
String digital_left="?";
String digital_right="?";
int ii;

st1.replace("<>","=");
st1.replace(">","=");
st1.replace("<","=");
st1.replace("==","=");


//extract digital from Format,strout /data/
digital_left=between(st1,"(","=");
digital_right=between(st1,"=",")");

Serial.println("1111111111.digital_left/digital_right"+digital_left+"/"+digital_right);

digital_left=substitution(digital_left);
digital_left=calculation(digital_left);
digital_right=substitution(digital_right);
digital_right=calculation(digital_right);

Serial.println("2222222222.digital_left/digital_right"+digital_left+"/"+digital_right);

ii=between(st,"{","}").toInt();
if (st.indexOf(">=")>-1 && digital_left.toFloat() >=  digital_right.toFloat())  {return ii;};
if (st.indexOf("<=")>-1 && digital_left.toFloat() <=  digital_right.toFloat())  {return ii;};
if (st.indexOf("==") > -1 && digital_left.toFloat() == digital_right.toFloat()){return ii;};  
if (st.indexOf("=") > -1 && digital_left.toFloat() == digital_right.toFloat()){return ii;};  
if (st.indexOf("<>")>-1 && digital_left.toFloat() !=  digital_right.toFloat())  {return ii;};
if (st.indexOf(">")>-1 && digital_left.toFloat() >  digital_right.toFloat())  {return ii;};
if (st.indexOf("<")>-1 && digital_left.toFloat() <  digital_right.toFloat())  {return ii;};


return 0;

                }; // end func
                
//////////////////////
String between(String sta,String beg,String fin)
//now possible between(word,  "," , ",") - between two same symbols 
{
int be=sta.indexOf(beg); //delete label
if (be== -1){return "";};
int en=sta.indexOf(fin,be+1); //delete label
String stas=sta.substring(be+1,en);
return stas;
}; // end func

////////////////////////////////// extract_digi extract_digi extract_digi extract_digi


String extract_digi(String dat){ // extract digital from Format,strout /data/
//Serial.println("dat==================================="+dat+"======");

dat.replace("$",""); // remove "$"
int n=String(dat[1]).toInt();
if (dat[0]=='X' && isdigit(dat[1]))  
{
return X[n];  
};

if (dat[0]=='S' && isdigit(dat[1]))  
{
return s_val[n];   
};

if (dat[0]=='D' && isdigit(dat[1]))  
{
return String(d_val[n]);   
};

if (dat[0]=='A' && isdigit(dat[1]))  
{
return String(A_0*coeff_k+coeff_b);   
};


if (dat.toFloat() == 0.0 && dat[0]=='0') {return "0";};
if (dat.toFloat() > 0.0 || dat.toFloat() < 0.0) {return dat;};
// try extract 

int poza=strout.indexOf(dat);
if (poza == -1){
Auto=0;
E_write("0",mem("Auto"));
E_write("No_var_"+dat+"_p"+String(iukaz),mem("last_error")); 
return "?";     };

//count "," until poza
int stet_zpt=0; //
for (int i=0;i<poza;i++){if (
  strout[i] == ','){stet_zpt++;};};
//
if (stet_zpt==0) {return between(par_data,"=",",");};
int stet_zpt2=0;
for (int i=0;i<=100;i++){
Serial.print(String(par_data[i]));
if (par_data[i] != ','){continue;};
stet_zpt2++;
if (stet_zpt ==  stet_zpt2)
   {poza=par_data.indexOf(",",i+1);
    return par_data.substring(i+1,poza);
   };

}; // end for par_data

// data is not prepared yet

if (dat[0]=='S' && isdigit(dat[1]))  
{
return s_val[n];   
};

return "?";

};

////////////////////////////////////

void sync(void){ // call counter1.cgi set time_on_off
String strt="";
Serial.println(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>  enter SYNC >>>>>>>>>>>>>>>>>>>>>>>>>>>>");
HTTPClient http;
String tmptz=E_read(mem("TimeZone"));
uri="http://bortx.ru/cgi-bin/GMT.cgi?tz="+tmptz;

http.begin(uri); //HTTP
int httpCode = http.GET();
Serial.println("httpcode="+String(httpCode));
payload = http.getString();
Serial.println(payload);
if (payload.indexOf("GMT_TIME") >-1)
{
String day1=between(payload,"="," ");
Udate=between(payload,"="," ");
strt=between(payload," ",";");
E_write("Sync_time["+strt+"]<"+day1+">{0}",mem("time_on_off"));}
else {E_write("Cont_time[0:0:0]<11-11-11>{0}",mem("time_on_off"));};
http.end();
time_on_off=E_read(mem("time_on_off"));
return;
             
               };


///////////////////////////////

void mail(String mes){
Serial.println(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>  enter MAIL >>>>>>>>>>>>>>>>>>>>>>>>>>>>");
String adr=E_read(mem("email"));
if (adr.indexOf("@") == -1){return;};
mes=substitution(mes);
String urin="http://bortx.ru:6110/?id="+bortx_id+";device=esp;mail="+adr+"/from:"+bortx_id+"/mes:"+mes+",msec:"+String(millis())+"/;";
HTTPClient http;
http.begin(urin); //HTTP
int httpCode = http.GET();
payload = http.getString();
Serial.println(payload);
http.end();

return;
             
               };


///////////////////////////////


void note(String mes){
//return;
String urin="http://bortx.ru:6110/?id="+bortx_id+";device=esp;note="+mes+";\r\n\r\n";
HTTPClient http2;
http2.begin(urin); //HTTP
int httpCode = http2.GET();
http2.end();

             
                   };


///////////////////////////////
void s_delay(int paus){ // dealay & counter
int start=millis();
int count_cycles=0;
while (millis()<start+paus)
{
count_cycles++;
vd0=digitalRead(16); 
if (vd0 != vd0_old){vd0_old=vd0; count0++;}; 
delay(5);
};
//Serial.println("cccccccccccccccccccccccccccccccccccccc"+String(count_cycles)+"======");
count_time=count_time+paus;
           
                     }; // end s_delay
                 

///////////////////////////////////////////////////////////// SUBSTITUTION
String substitution(String before){ // operator's string

if (before.indexOf("$random") >=0) {before.replace("$random",String(random(1000)/1000.0));};
if (before.indexOf("$timer") >=0) {before.replace("$timer",String(Timer));};
if (before.indexOf("$retro") >=0) {before.replace("$retro",String(Retro));};
if (before.indexOf("$ntime") >=0) {before.replace("$ntime",String(Ntime()));};
if (before.indexOf("$udate") >=0) {before.replace("$udate",Udate);};
if (before.indexOf("$input") >=0) {before.replace("$input",Input());};
if (before.indexOf("$temp") >=0) {before.replace("$temp",Temp());};
if (before.indexOf("$morning") >=0) {before.replace("$morning",Morning());};
if (before.indexOf("$evening") >=0) {before.replace("$evening",Evening());};
if (before.indexOf("$ptime") >=0) {before.replace("$ptime",String(Ptime));};


String var_name="";
int pos_dollar;
int pos_end;

int mstet=0;
while ( before.indexOf("$") >=0 && mstet < 10)      { // replace var_name $x to digits
mstet++;
pos_dollar=before.indexOf("$");
var_name=before.substring(pos_dollar+1,pos_dollar+3); 
before.replace("$"+var_name,extract_digi(var_name));
                                                    };  // end while

mstet=0;
while ( before.indexOf("[") >=0 && mstet < 10) { // replace var_name [] to digits
mstet++;
pos_dollar=before.indexOf("[");
var_name=between(before,"[","]"); 
before.replace("["+var_name+"]",extract_digi(var_name));
                                               
                                               };  // end while

return before;
};

///////////////////////////////////////////////////////////// CALCULATION
String calculation(String before){ // operator's string

int pos;
int posR;
int posL;
String res;
String znak;
String all_znak="/*+~";
float PR;
float PL;
for (int j=0;j<=3;j++)
{znak=all_znak[j];
//Serial.println("before:"+before);
//Serial.println("ZZZZZZZZZNNNNNNAAAAAKKKKKKK:="+znak);
Serial.print(znak);
while ( before.indexOf(znak) !=-1 ) { // 
pos=before.indexOf(znak);
// search start left operand
posL=pos;
for (int k=pos-1;k>=0;k--)
{
if (isdigit(before[k]) || before[k]== '.' || before[k]== '-'){posL--;continue;}else{break;}; 
};
PL=before.substring(posL,pos).toFloat();

posR=pos+1;
for (int k=pos+1;k<=100;k++)
{
  
if (isdigit(before[k]) || before[k]== '.' || before[k]== '-'){posR++;continue;}else{break;}; 
};

Serial.println("L&R:"+String(posL)+"&"+String(posR));


PR=before.substring(pos+1,posR).toFloat();

//Serial.println("L&R:"+String(posL)+"&"+String(posR));
//Serial.println("PL&PR:"+String(PL)+"&"+String(PR));

if (znak=="*") {res=String(PL*PR);};
if (znak=="/") {res=String(PL/PR);};
if (znak=="+") {res=String(PL+PR);};
if (znak=="~") {res=String(PL-PR);};

 
//Serial.println("left="+before.substring(0,posL));
//Serial.println("right="+before.substring(posR));

Serial.println("res="+res); 

before=before.substring(0,posL)+res+before.substring(posR); 
Serial.println("CALK:"+before);

                                 }; //wend

}; // end for


Serial.println("CALK:"+before);
return before;
};

////////////////////////
String Ntime(void){ // return time like string with decimal point "21.55"

String tt=time_on_off;
String ttt=between(time_on_off,"[",":");
long dhour=ttt.toInt();
tt.replace("["+ttt+":","[");
long dmin=between(tt,"[",":").toInt();
long dsec=between(tt,":","]").toInt();
long allsec=dhour*3600+dmin*60+dsec+millis()/1000;
//Serial.println("h/m/s:"+String(dhour)+"/"+String(dmin)+"/"+String(dsec));
dhour=allsec/3600;
dhour=dhour%24;
dmin=(allsec%3600)/60;
//Serial.println("MINMINMIN:"+String(dmin));
if (dmin>=10){return String(dhour)+"."+String(dmin);}else{return String(dhour)+".0"+String(dmin);}; 

};

////////////////////////////////////////////
String Morning(void){
String tt=E_read(mem("time_on_off"));
String mon=between(tt,"-","-");
int I=mon.toInt();
return morning[I];
};

////////////////////////////////////////////
String Evening(void){

String tt=E_read(mem("time_on_off"));
//Serial.println("**********evening1***********="+tt);
String mon=between(tt,"-","-");
//Serial.println("**********evening1***********="+mon);
int I=mon.toInt();
return evening[I];
};

////////////////////////////////////////////
String Input(void){ // return string getting by software serial to 7th pin

String sy="";
String sym="";
String nl=""; // new line
String pl=""; // pay load line
int chan=0; // X1:X9 will filles by steram <4:-125> means ; X[4]=-125
int chan_end=0;
int stat=0; // status 0-read w/o write, 1 start write(<)  
int nsym_old=0;
int nsym=Serial.available();
Serial.println("**********Input from D7***********    Len="+String(nsym));
if (nsym<1)return input0; // return with old value 

for (int i=0;i<=128;i++)
{
s_delay(50);
nsym=Serial.available();
if (nsym == nsym_old){break;}; 
nsym_old=nsym;
};

for (int i=1;i<=nsym;i++){
sbuf=Serial.read();
sym=String(sbuf);
nl=nl+sym;
};

Serial.println(nl);


nl.replace(">","");
nl.replace("</","/");
nl.replace(" ","");
nl.replace("+","");

//Serial.println(nl);

for (int i=0; i<nl.length();i++) { // 
sym=nl[i];
//Serial.println(sym+"="+String(sym.toInt()));

 if (sym.toInt()==0  && sym != "." &&  sym !="/" && sym !="<" && sym !="-" && sym !="0"){continue;}; 
if (sym=="<" ){sy=nl[i+1];chan=sy.toInt(); pl="";stat=1;i++;continue;};  
if (stat==0){continue;};
// below stat=1

if (sym=="/"){ // if /
//Serial.println("payload="+pl+"&ch="+String(chan));
stat=0;
sy=nl[i+1];
chan_end=sy.toInt();
if (chan_end != chan || chan>9 || chan<0) {continue;};
if (pl==""){continue;};
if (chan==0) {input0=pl;};
if (chan !=0) {X[chan]=pl;}; 
            }; //end if /
pl=pl+sym;
                                  }; // end for

return input0;
};

////////////////////////////////////////////
String Temp(void){ // command to measure and return old temperature ds18b20 
sensors.requestTemperatures();
String temp=String(sensors.getTempCByIndex(0));
Serial.println("temp="+temp);
return temp;
};

////////////////////////////////////////////
void pre_sync(){ // first set pre_sync then if there is time from server - set new time
String tt=E_read(mem("time_on_off"));
Serial.println("TIME_pre_SYNC_BEFORE="+tt);
String sec_old=between(tt,"{","}");
String data_old=between(tt,"<",">");
String ttt=between(tt,"[",":");
long dhour=ttt.toInt();
tt.replace("["+ttt+":","[");
long dmin=between(tt,"[",":").toInt();
long dsec=between(tt,":","]").toInt();
long allsec=dhour*3600+dmin*60+dsec+millis()/1000+sec_old.toInt();
dhour=allsec/3600;
dhour=dhour%24;
dmin=(allsec%3600)/60;
tt="Pre_sync["+String(dhour)+":"+String(dmin)+":00]{0}<"+data_old+">";
Serial.println("TIME_pre_SYNC1_AFTER="+tt);
E_write(tt,mem("time_on_off"));
return;
};





               
