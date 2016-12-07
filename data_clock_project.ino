#include <util/delay.h>
#include <LiquidCrystal.h>
#include <Wire.h>  
#include <IRremote.h>

LiquidCrystal lcd(12, NULL, 11, 9,8,7,6);
volatile int secunda = 0;
volatile int minut = 0;
volatile int ora = 0;
volatile int zi = 10;
volatile int luna = 11;
volatile int an = 2016;
volatile int zile_luna [] = {31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
volatile int ora_mod = 1;

volatile boolean stop_command = false;
volatile int n;

String zile_nume1 [7] = {"SUN","MON","TUE", "WEN","THU", "FRI", "SAT"};


const int backlight = 5;
volatile boolean backlight_on;
volatile byte set_mode;
volatile int brightness = 0;
int fadeAmount = 5 ;


int speakerPin = 5;
volatile int set_display = 0;

const int RECV_PIN = 10;
IRrecv irrecv(RECV_PIN);
decode_results results;
 int ir_result;

//----------------------VARIABILE ALARMA---------------------

volatile int s_a = 0;
volatile int m_a = 5;
volatile int o_a = 0;
volatile int set_mode_alarma = 0;


//----------------------VARIABILE MELODII---------------------
const int length = 69;
const char * notes = "ddaagfedcdefga ddaagfedcdefga avgavCDagfdefgfgavaagfedfedgfgavCDagfed";
const int beats[77] = {2,2,2,2,2,2,2,2,2,2,2,2,2,4,2,2,2,2,2,2,2,2,2,2,2,2,2,2,4,2,2,2,2,2,2,2,2,2,2,2,2,2,2,4,2,2,4,2,2,2,2,2,2,4,1,1 ,2,4,2,2,2,2,2,2,2,2,2,2,8 };
const int tempo[6] = {150,150,150,150,150,150};

//--------------------VARIABILE TEMPERATURA -----------
const int tmp102Address = 0x48; 
float temperatura = 0;

//----------------------SETUP---------------------
void setup() {


Serial.begin(9600);

 TIMSK1 = (1 << TOIE1); // activare timer overflow
 TCNT1=0x0BDC;
 TCCR1A = 0;
 TCCR1B = (1 << CS12); // timer start
 
 //activare int0 si int1
 EIMSK |= (1 << INT0);
 EIMSK |= (1 << INT1);
 //activare PCI2 (buton 3 - PCINT20 - PCI2)
 PCICR |= (1 << PCIE2);
 PCMSK2 |= (1<<PCINT20);

 //la pornire lumina de fundal a afisajului este pornita
 pinMode(backlight, OUTPUT);
 digitalWrite(backlight, HIGH);
 backlight_on = true;

 //setare mod functionare normala
 set_mode = 0;

 //configurare LCD
 lcd.begin(16, 2);
 lcd.noCursor();

 irrecv.enableIRIn(); // Start the receiver
 //irrecv.blink13(true);

Wire.begin(); 

 pinMode(speakerPin, OUTPUT); //for Alarm
}

//----------------------LOOP----------------------
void loop(){
 
   int number = nume_2(zi, luna, an);
 switch(set_display)
 {
  case 0: //standard display
 lcd.setCursor(0, 0);
 if (zi<10) lcd.print("0");
 lcd.print(zi);
 lcd.setCursor(2,0);
 lcd.print("/");
 if (luna<10) lcd.print("0");
 lcd.print(luna);
 lcd.setCursor(5,0);
 lcd.print("/");
 lcd.print(an);
 lcd.setCursor(11,0);

 //int number = nume_2(zi, luna, an);
 lcd.print(zile_nume1[number]);
 Serial.println(number);

 if(ora_mod == 0)
 {
  lcd.setCursor(0, 1);
 if (ora<10) lcd.print("0");
 lcd.print(ora);
 }
 else 
 {
  lcd.setCursor(0, 1);
  if(ora == 0) lcd.print("12");
  else{
 if (ora<10) lcd.print("0");
 if(ora > 12) 
 {
  if(ora-12 < 10) lcd.print("0");
  lcd.print(ora-12);
 }
 else 
 {
  //if(ora == 0) lcd.print("")
  lcd.print(ora);
 }
  }
 lcd.setCursor(9,1);
 if(ora > 12) lcd.print("PM");
 else lcd.print("AM");
  }
 
 lcd.setCursor(2,1);
 lcd.print(":");
 if (minut<10) lcd.print("0");
 lcd.print(minut);
 lcd.setCursor(5,1);
 lcd.print(":");
 if (secunda<10) lcd.print("0");
 lcd.print(secunda);
 lcd.print(" "); 

 remote_control();

 if (set_mode!=0)
 {
 lcd.setCursor(12, 1);
 lcd.print("SET");
 switch (set_mode) {
 case 1:
 lcd.print("Z");
 break;
 case 2:
 lcd.print("L");
 break;
 case 3:
 lcd.print("A");
 break;
 case 4:
 lcd.print("O");
 break;
 case 5:
 lcd.print("M");
 break;
 case 6:
 lcd.print("S");
 break;
 }
 }
 delay(500);
 lcd.clear();
 break;
 
 case 1: //alarm dsplay
 remote_control();
 lcd.setCursor(0,0);
 lcd.print("SET ALARM");
 
 if(ora_mod == 0)
 {
  lcd.setCursor(0, 1);
 if (o_a<10) lcd.print("0");
 lcd.print(o_a);
 }
 else 
 {
  lcd.setCursor(0, 1);
  if(o_a == 0) lcd.print("12");
  else{
 if (o_a<10) lcd.print("0");
 if(o_a > 12) 
 {
  if(o_a-12 < 10) lcd.print("0");
  lcd.print(o_a-12);
 }
 else 
 {
  //if(ora == 0) lcd.print("")
  lcd.print(o_a);
 }
  }
 lcd.setCursor(9,1);
 if(o_a > 12) lcd.print("PM");
 else lcd.print("AM");
  }
 
 lcd.setCursor(2,1);
 lcd.print(":");
 if (m_a<10) lcd.print("0");
 lcd.print(m_a);
 lcd.setCursor(5,1);
 lcd.print(":");
 if (s_a<10) lcd.print("0");
 lcd.print(s_a);
 lcd.print(" "); 
 
 //lcd.setCursor()
 //delay(500);
 //lcd.clear();

if (set_mode_alarma!=0)
 {
 lcd.setCursor(12, 1);
 lcd.print("SET");
 switch (set_mode_alarma) {
 case 1:
 lcd.print("O");
 break;
 case 2:
 lcd.print("M");
 break;
 case 3:
 lcd.print("S");
 break;
 }
 }
 delay(500);
 lcd.clear();
 //temperatura = getTemperature(); 
 //Serial.println(temperatura); 
 break;

 case 2:
 temperatura = getTemperature(); 
  remote_control();
 lcd.setCursor(0,0);
 lcd.print("TEMPERATURA");
 lcd.setCursor(0,1);
 lcd.print(temperatura);
 lcd.print("'C");
 delay(1000);
  lcd.clear();
 break;
 }
 if((o_a == ora)&&(m_a == minut)&&(s_a == secunda))
 alarma();
 if(set_mode_alarma == 4) set_mode_alarma = 0;
 if(set_mode_alarma < 0) set_mode_alarma = 3;
}

//----------------------CRONOMETRUL----------------------
ISR(TIMER1_OVF_vect) {
 TCNT1=0x0BDC;
 secunda++;
  
 if (secunda == 60)
 { secunda = 0;
 minut++;
 if (minut>=60)
 {
 minut = 0;
 ora ++;
 if (ora >= 24)
 {
  ora = 0;
 zi++;
 if (zi > zile_luna[luna-1])
 {
 zi = 1;
 luna++;
 if (luna > 12)
 {
 luna = 1;
 an++;
 if (an%4 == 0) zile_luna[1] = 29;
 else zile_luna[1] = 28;
 }
 }
 }
 }
 }
 
}

//----------------------BUTONUL_1(MODE)----------------------
ISR(INT0_vect) {
  if(stop_command == false) stop_command = true;
  switch(set_display){
 case 0:
 set_mode++;
 if (set_mode==7) set_mode=0;
 _delay_ms(400); 
 break;
 case 1:
 set_mode_alarma++;
 if (set_mode_alarma==4) set_mode_alarma=0;
 _delay_ms(400); 
 break;
  }
  }

//----------------------BUTONUL_2(SET)-----------------------
ISR(INT1_vect)
{ switch(set_display)
{case 0:
  if (set_mode!=0)
 switch (set_mode) {
 case 1:
 zi++;
 if (zi==zile_luna[luna-1]) zi=1;
 break;
 case 2:
 luna++;
 if (luna==13) luna=1;
if (zi>zile_luna[luna-1]) zi=zile_luna[luna-1];
break;
 case 3:
 an++;
 if (an==2020) an=2013;
 if (an%4 == 0) zile_luna[1] = 29;
 else zile_luna[1] = 28;
 if (zi>zile_luna[luna-1]) zi=zile_luna[luna-1];
 break;
 case 4:
 ora++;
 if (ora==24) ora=0;
 break;
 case 5:
 minut++;
 if (minut==60) minut=0;
 break;
 case 6:
 secunda++;
 if (secunda==60) secunda=0;
 break;
 }
 if(set_mode == 0) 
 if(ora_mod == 0) ora_mod = 1;
 else ora_mod = 0;
 _delay_ms(400); 
 break;
//------------------ALARMA------------
 case 1:
  if (set_mode_alarma!=0)
 switch (set_mode_alarma) {
 case 1:
 o_a++;
 if (o_a==24) o_a=0;
 break;
 case 2:
 m_a++;
 if (m_a==60) m_a=0;
 break;
 case 3:
 s_a++;
 if (s_a==60) s_a=0;
 break;
 }
 if(set_mode_alarma == 0) 
 if(ora_mod == 0) ora_mod = 1;
 else ora_mod = 0;
 _delay_ms(400); 
}
 }

//----------------------BUTONUL_3(LIGHT OFF/ON)----------------------
 ISR(PCINT2_vect) {
_delay_ms(20);
if (!(PIND & 0x10))
{
  switch(set_display){
    case 0: set_display = 1;
           break;
    case 1: set_display = 2;
          break;
    case 2: set_display = 0;
          break;
    }
  /*
 if (backlight_on) {
 digitalWrite(backlight, LOW);
 backlight_on = false;
 }
 else {
 digitalWrite(backlight, HIGH);
 backlight_on = true;
 }*/
//----------------------DIM LIGHT----------------------
/*
if (backlight_on)
{backlight_on = false;
while (brightness <= 255) {
   analogWrite(backlight, brightness);
   brightness += fadeAmount;
   delay(500);
 }
}
else{
   backlight_on = true;
while (brightness >= 0) {
  analogWrite(backlight, brightness);
  brightness -= fadeAmount;
  delay(500);
 }
}
 */
}
}
//----------------------NUME ZI----------------------

int leap (int year)
{
  return year*365 + (year/4) - (year/100) + (year/400);
}


int zeller (int year, int month, int day)
{
  year += ((month+9)/12) - 1;
  month = (month+9) % 12;
  return leap (year) + month*30 + ((6*month+5)/10) + day + 1;
}

/* Returns the day of week (1=Monday, 7=Sunday) for a given date.
 */
int dow (int year, int month, int day)
{
  return (zeller (year, month, day) % 7) + 1;
}

int nume_2(int d,int m,int y)
{
  return (d+= m<3?y--:y-2,23*m/9+d+4+y/4-y/100+y/400)%7 ;
  }

//----------------------REMOTE-IR----------------------
void remote_control()
{
  ir_result = 0;
 if (irrecv.decode(&results)) {
 // ir_result = results.value;
 //String ir_result = (rezultate,HEX)
 
 switch(results.value){
 case 0xFFC23D : switch(set_display){
    case 0: set_display = 1;
           break;
    case 1: set_display = 2;
          break;
    case 2: set_display = 0;
          break;
    }
    break;
 case 0xFF906F : if(ora_mod == 0) ora_mod = 1; //EQ
                 else ora_mod = 0;
                 break;
 case 0xFFA25D :switch(set_display)
 {              
                case 0 :
                set_mode--; //CH-
                break;
                case 1 :
                set_mode_alarma--; //CH-
                break;
 }
 break;
 case 0xFFE21D: switch(set_display)
 {              
                case 0 :
                set_mode++; //CH+
                break;
                case 1 :
                set_mode_alarma++; //CH+
                break;
 }
                break;
 
 case 0xFFE01F :          //----Decrementare IR  (-)
 switch(set_display){
  case 0:
 
   switch (set_mode) {
 case 1:
 zi--;
 if (zi==0) zi==zile_luna[luna-1];
 break;
 case 2:
 luna--;
  if (luna==0) luna=12;
if (zi>zile_luna[luna-1]) zi=zile_luna[luna-1];
break; 

 case 3:
 an--;
 if (an==2013) an=2020;
 if (an%4 == 0) zile_luna[1] = 29;
 else zile_luna[1] = 28;
 if (zi>zile_luna[luna-1]) zi=zile_luna[luna-1];
 break;
 case 4:
 ora--;
 if(ora < 0) ora=23;
 if(ora == 24) ora = 0;
 
 break;
 case 5:
 minut--;
 if (minut < 0) minut=59;
 break;
 case 6:
 secunda--;
 if (secunda < 0) secunda=59;
 break;
 }
  break;

  
 case 1:
  switch (set_mode_alarma) {
 case 1:
 o_a--;
 if(o_a < 0) o_a=23;
 if(o_a == 24) o_a = 0;
 
 break;
 case 2:
 m_a--;
 if (m_a < 0) m_a=59;
 break;
 case 3:
 s_a--;
 if (s_a < 0) s_a=59;
 break;
 }
  break;
 }
   break;
 case 0xFFA857 :  //-----Incremantare IR  (+)
 switch(set_display)
  {case 0:
 
    switch (set_mode) {
 case 1:
 zi++;
 if (zi==zile_luna[luna-1]) zi=1;
 break;
 case 2:
 luna++;
 if (luna==13) luna=1;
if (zi>zile_luna[luna-1]) zi=zile_luna[luna-1];
break;
 case 3:
 an++;
 if (an==2020) an=2013;
 if (an%4 == 0) zile_luna[1] = 29;
 else zile_luna[1] = 28;
 if (zi>zile_luna[luna-1]) zi=zile_luna[luna-1];
 break;
 case 4:
 ora++;
 if (ora==24) ora=0;
 break;
 case 5:
 minut++;
 if (minut==60) minut=0;
 break;
 case 6:
 secunda++;
 if (secunda==60) secunda=0;
 break;
 
    }
    break;
 case 1:
  switch (set_mode_alarma) {
 case 1:
 o_a++;
 if (o_a==24) o_a=0;
 break;
 case 2:
 m_a++;
 if (m_a==60) m_a=0;
 break;
 case 3:
 s_a++;
 if (s_a==60) s_a=0;
 break;
    }
 }
    break;
 
 }
 
 if (set_mode==7) set_mode=0;
 if (set_mode==255) set_mode=7;
  if (set_mode_alarma==4) set_mode=0;
 if (set_mode_alarma==255) set_mode=0;
 Serial.println(results.value,DEC);
 Serial.println(set_mode);
  irrecv.resume();
 }
 //ir_result = 0;
 }

void playTone(int tone, int duration)  
{   for (long i = 0; i < duration * 1000L; i += tone * 2)    
{      
  digitalWrite(speakerPin, HIGH);       
  delayMicroseconds(tone);       
  digitalWrite(speakerPin, LOW);       
  delayMicroseconds(tone);   
  } 
  }  
void playNote(char note, int duration)  
{    const char names[] = { 'c', 'd', 'e', 'f', 's', 'g', 'a', 'v', 'b', 'C', 'D', 'E' }; // note    
const int tones[] = { 1915, 1700, 1519, 1432, 1352, 1275, 1136, 1073, 1014, 956, 852, 758 };  // frecvente  
  for (int i = 0; i < 12; i++)    
  {       
    if (names[i] == note) playTone(tones[i], duration);  
    } 
    }  

void alarma()
{
  stop_command = false;
  lcd.setCursor(0,0);
  lcd.print("TREZIREA!");
  lcd.clear();
  for (n = 0; n < length; n++)   
{
  if(stop_command == true){ 
      //stop_command = false;
      goto  stop_alarm ;
  }
  
 if (notes[n] == ' ') 
 delay(beats[n] * tempo[1]); // pauză      
 else playNote(notes[n], beats[n] * tempo[1]); 
   // pauza intre note     
  } 
   delay(tempo[1] / 2); 
    stop_alarm:
   stop_command = false;
 }

 float getTemperature() 
    {   byte MSB = 0x00;   
    byte LSB = 0x00;   
    int TempCitita = 0;   
    float TempCelsius = 0.0;  
  Wire.beginTransmission(tmp102Address);   
  Wire.write(0x00);   
  Wire.endTransmission();   
  Wire.requestFrom(tmp102Address,2);    
  Wire.endTransmission();   
  MSB = Wire.read();   
  LSB = Wire.read();  
  //temperatura este pe 12 biti cu ultimii 4 biti 0 astfel trebuie    
  //shift-ata la stanga cu 4 pozitii   
  TempCitita = ((MSB << 8) | LSB) >> 4;    
  TempCelsius = TempCitita * 0.0625;  
  return TempCelsius; 
  } 

    

  
