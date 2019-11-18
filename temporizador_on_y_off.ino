#include <Wire.h>
#include <LiquidCrystal_I2C.h> //libreria obtenida de https://github.com/johnrickman/LiquidCrystal_I2C
#include <EEPROM.h>

LiquidCrystal_I2C lcd(0x27,16,2);

volatile unsigned int cuenta = 0;
bool ESTADO = false;
int conf=8;
int subir=9;
int bajar=10;
int guardar=11;
int marcha=12;
//int led_run=13;
int led_activo=13;

int horas=0;
int minutos=0;
int segundos=0;

int iniciar=0;
long tiempo1=0;
long tiempo2=0;
int pause=1;
int horasOn=0;
int minutosOn=0;
int segundosOn=0;
int horasOf=0;
int minutosOf=0;
int segundosOf=0;

int horasOnActual=0;
int minutosOnActual=0;
int segundosOnActual=0;
int horasOfActual=0;
int minutosOfActual=0;
int segundosOfActual=0;

int segundosOnActual2=0;
int minutosOnActual2=0;
int horasOnActual2=0;
int segundosOfActual2=0;
int minutosOfActual2=0;
int horasOfActual2=0;

long n=0;
int dir=0;
int valor=0;

int configuracion=0;
void setup() {
  Serial.begin(9600);
  pinMode(conf,INPUT_PULLUP);
  pinMode(subir,INPUT_PULLUP);
  pinMode(bajar,INPUT_PULLUP);
  pinMode(guardar,INPUT_PULLUP);
  pinMode(marcha,INPUT_PULLUP);
//  pinMode(led_run,OUTPUT);
  pinMode(led_activo,OUTPUT);
  
  
  lcd.init();                      // initialize the lcd 
  // Print a message to the LCD.
  lcd.backlight();
  lcd.setCursor(3,0);
  lcd.print("Iniciando");
  lcd.setCursor(2,1);
  lcd.print("Temporizador");
  delay(1000);
  lcd.clear();
//  digitalWrite(led_run,LOW);
   digitalWrite(led_activo,LOW);
 
}

void loop() {
 //////////////////////////////////modo configuracion///////////////////////////////////
  if(digitalRead(guardar)==LOW)
    {lcd.noBlink();
     configuracion=0;
     }
  
  if(configuracion==1)
  {
  lcd.blink();
//  digitalWrite(led_run,LOW);
  digitalWrite(led_activo,LOW);
  iniciar=0;
   horasOn=EEPROM.read(0);
  minutosOn=EEPROM.read(1);
  segundosOn=EEPROM.read(2); 

  horasOf=EEPROM.read(3);
  minutosOf=EEPROM.read(4);
  segundosOf=EEPROM.read(5);
  imprimir_tiempo(0,horasOn,minutosOn,segundosOn);  
  imprimir_tiempo(1,horasOf,minutosOf,segundosOf);
  }
  
  if(digitalRead(conf)==LOW)
  { configuracion++;
  if(configuracion==7)
  configuracion=1;
    delay(400);
   }

  if(digitalRead(subir)==LOW)
  { lcd.noBlink();
    valor = EEPROM.read(configuracion-1);
    valor++;
    if(valor>59)
    valor=0;
    if(valor<10){
     lcd.print('0');
      }
    lcd.print(valor); 
    EEPROM.write(configuracion-1, valor);
    delay(400);
  }
  
  if(digitalRead(bajar)==LOW)
  { lcd.noBlink();
    valor = EEPROM.read(configuracion-1);
    valor--;
    if(valor<0)
    valor=59;
    if(valor<10){
    lcd.print('0');}
    lcd.print(valor);
    EEPROM.write(configuracion-1, valor);
    delay(400);
  }

  switch(configuracion) {
    case 1:  lcd.setCursor(6,0);  lcd.blink();break;
    case 2:  lcd.setCursor(9,0); lcd.blink();break;
    case 3:  lcd.setCursor(12,0); lcd.blink();break;
    case 4:  lcd.setCursor(6,1); lcd.blink();break;
    case 5:  lcd.setCursor(9,1); lcd.blink();break;
    case 6:  lcd.setCursor(12,1); lcd.blink();break;
    //default: configuracion=1;
    }
   
    delay(100);

   //////////////////////////////////////////////modo stand by//////////////////////////// 
  while(configuracion ==0)
  {
  
  lcd.setCursor(0,0);
  lcd.print("T_ON:");
  horasOn=EEPROM.read(0);
  minutosOn=EEPROM.read(1);
  segundosOn=EEPROM.read(2); 

  horasOf=EEPROM.read(3);
  minutosOf=EEPROM.read(4);
  segundosOf=EEPROM.read(5);
  
   if(iniciar==0)
  {horasOnActual=horasOn;
   minutosOnActual=minutosOn;
   segundosOnActual=segundosOn;
   horasOnActual2=horasOn;
   minutosOnActual2=minutosOn;
   segundosOnActual2=segundosOn; 

   horasOfActual=horasOf;
   minutosOfActual=minutosOf;
   segundosOfActual=segundosOf;
   horasOfActual2=horasOf;
   minutosOfActual2=minutosOf;
   segundosOfActual2=segundosOf; 
    
   tiempo1=horasOn*3600+minutosOn*60+segundosOn;
   tiempo2=horasOf*3600+minutosOf*60+segundosOf;
  n=0;
//  digitalWrite(led_run,LOW);
  digitalWrite(led_activo,LOW);
  //iniciar=1;
  }
  
  if(digitalRead(marcha)==LOW&&pause==1)//al presionar la marcha por primera vez se activa el ciclo
  {pause=0;
   if(iniciar==0)
   iniciar=1;
   SREG = (SREG & 0b01111111); //Desabilitar interrupciones   https://hetpro-store.com/TUTORIALES/arduino-timer/
   TIMSK2 = TIMSK2|0b00000001; //Habilita la interrupcion por desbordamiento
   TCCR2B = 0b00000010; //Configura preescala para que FT2 sea de 1MHz    
   SREG = (SREG & 0b01111111) | 0b10000000; //Habilitar interrupciones //Desabilitar interrupciones
   delay(1000);
   }

    
  if(digitalRead(marcha)==LOW&&pause==0) //al presionar de nuevo marcha por segunda vez se pausa el ciclo
  {pause=1;
   digitalWrite(led_activo,LOW);
   delay(1000);
    }
    
   if(iniciar==1&&pause==0)
   {
   digitalWrite(led_activo,HIGH);
   segundosOnActual2=segundosOnActual-n;
   if(segundosOnActual2==0&&minutosOnActual2==0&&horasOnActual2==0)
   {  iniciar=2;n=0;
      minutos=0;horas=0;segundos=0;
      horasOnActual=horasOn;
      minutosOnActual=minutosOn;
      segundosOnActual=segundosOn;
      horasOnActual2=horasOn;
      minutosOnActual2=minutosOn;
      segundosOnActual2=segundosOn; 

      digitalWrite(led_activo,LOW);
   }
   else if(segundosOnActual2<0)
    {
    segundosOnActual2=59;
    segundosOnActual=59;
    n=0;
    minutos++;
    minutosOnActual2=minutosOnActual-minutos;
    if(minutosOnActual2<0)
        {
          minutosOnActual2=59;
          minutosOnActual=59;
          minutos=0;
          horas++;
          horasOnActual2=horasOnActual-horas;
          if(horasOnActual2<0)
            {iniciar=2;n=0;
            digitalWrite(led_activo,LOW);
              }
      //    horasOnActual=horasOnActual2;
          }
    // minutosOnActual=minutosOnActual2;     
    }
   // segundosOnActual=segundosOnActual2;
   
   
   }
   
    

   if(iniciar==2&&pause==0)
   {
    
   //digitalWrite(led_activo,HIGH);
   segundosOfActual2=segundosOfActual-n;
   if(segundosOfActual2==0&&minutosOfActual2==0&&horasOfActual2==0)
   {  iniciar=1;
      n=0;
      minutos=0;horas=0;segundos=0;
      horasOfActual=horasOf;
      minutosOfActual=minutosOf;
      segundosOfActual=segundosOf;
      horasOfActual2=horasOf;
      minutosOfActual2=minutosOf;
      segundosOfActual2=segundosOf; 
   
      digitalWrite(led_activo,HIGH);
   }
   else if(segundosOfActual2<0)
    {
    segundosOfActual2=59;
    segundosOfActual=59;
    n=0;
    minutos++;
    minutosOfActual2=minutosOfActual-minutos;
    if(minutosOfActual2<0)
        {
          minutosOfActual2=59;
          minutosOfActual=59;
          minutos=0;
          horas++;
          horasOfActual2=horasOfActual-horas;
          if(horasOfActual2<0)
            {iniciar=1;
            digitalWrite(led_activo,LOW);
              }
      //    horasOnActual=horasOnActual2;
          }
    // minutosOnActual=minutosOnActual2;     
    }
   // segundosOnActual=segundosOnActual2;
   
    }
   
  imprimir_tiempo(0,horasOnActual2,minutosOnActual2,segundosOnActual2);
  
  if(digitalRead(conf)==LOW)
  { configuracion=1;
    } 
    
  lcd.setCursor(0,1);
  lcd.print("TOFF:");
  imprimir_tiempo(1,horasOfActual2,minutosOfActual2,segundosOfActual2);
  delay(50); 
  }
}

void imprimir_tiempo(int fila,int hora1, int min1, int seg1)
{
  lcd.setCursor(6,fila);
  if(hora1<10){
    lcd.print('0');
  }
  lcd.print(hora1); 
   
  lcd.setCursor(8,fila);
  lcd.print(':');
  lcd.setCursor(9,fila);
  if(min1<10){
    lcd.print('0');
  }
  lcd.print(min1); 

  lcd.setCursor(11,fila);
  lcd.print(':');
  lcd.setCursor(12,fila);
  if(seg1<10)
  {
    lcd.print('0');
  }
  lcd.print(seg1); 
  
  
  }

   ISR(TIMER2_OVF_vect){
    if((iniciar==2||iniciar==1)&&pause==0)
    {cuenta++;
    if(cuenta > 3922) {
      n++;
     // digitalWrite(13,ESTADO);
     // ESTADO = !ESTADO;
      cuenta=0;
    }
    }
   }
