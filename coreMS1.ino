// Megasquirt Viewer
// A project made to display significant information through Megasquirt-I into an HD44780 based display

#include <LiquidCrystal.h>
LiquidCrystal lcd(2, 3, 4, 5, 6, 7);
int temperatura[]={435 ,369 ,334 ,311 ,294 ,281 ,270 ,260 ,252 ,245 ,239 ,233 ,228 ,223 ,218 ,214 ,210 ,207 ,203 ,200 ,197 ,194 ,191 ,189 ,186 ,184 ,182 ,179 ,177 ,175 ,173 ,171 ,169 ,168 ,166 ,164 ,162 ,161 ,159 ,158 ,156 ,155 ,153 ,152 ,151 ,149 ,148 ,147 ,145 ,144 ,143 ,142 ,141 ,139 ,138 ,137 ,136 ,135 ,134 ,133 ,132 ,131 ,130 ,129 ,128 ,127 ,126 ,125 ,124 ,123 ,122 ,121 ,121 ,120 ,119 ,118 ,117 ,116 ,115 ,115 ,114 ,113 ,112 ,111 ,111 ,110 ,109 ,108 ,108 ,107 ,106 ,105 ,105 ,104 ,103 ,102 ,102 ,101 ,100 ,100 ,99 ,98 ,97 ,97 ,96 ,95 ,95 ,94 ,93 ,93 ,92 ,91 ,91 ,90 ,89 ,89 ,88 ,87 ,87 ,86 ,85 ,85 ,84 ,84 ,83 ,82 ,82 ,81 ,80 ,80 ,79 ,78 ,78 ,77 ,77 ,76 ,75 ,75 ,74 ,73 ,73 ,72 ,72 ,71 ,70 ,70 ,69 ,68 ,68 ,67 ,67 ,66 ,65 ,65 ,64 ,63 ,63 ,62 ,61 ,61 ,60 ,60 ,59 ,58 ,58 ,57 ,56 ,56 ,55 ,54 ,54 ,53 ,52 ,52 ,51 ,50 ,50 ,49 ,48 ,48 ,47 ,46 ,46 ,45 ,44 ,43 ,43 ,42 ,41 ,41 ,40 ,39 ,38 ,38 ,37 ,36 ,35 ,35 ,34 ,33 ,32 ,31 ,30 ,30 ,29 ,28 ,27 ,26 ,25 ,24 ,23 ,23 ,22 ,21 ,20 ,19 ,18 ,17 ,16 ,15 ,13 ,12 ,11 ,10 ,9 ,8 ,6 ,5 ,4 ,2 ,1 ,0 ,-1 ,-3 ,-4 ,-6 ,-8 ,-10 ,-11 ,-13 ,-16 ,-18 ,-20 ,-23 ,-26 ,-29 ,-32 ,-36 ,-40 ,-45 ,-51 ,-58 ,-68 ,-85};
boolean ini=false;
int i=0,readB,hiResRPM;
double iTimeFull,iTimeX,iTimeL,iTimeM, afr, lambda, battv;
int clt, cltF, mapp, avance, nivel = 0, iat, iatF;
int buttonPin1=A5, buttonState1, buttonLastState1, screenNumber, shiftPin=11;
boolean launchcontrol, boostcontrol;

byte senial1[8] = {
  B10000,
  B10000,
  B10000,
  B10000,
  B10000,
  B10000,
  B10000,
  B10000,
};

byte senial2[8] = {
  B11000,
  B11000,
  B11000,
  B11000,
  B11000,
  B11000,
  B11000,
  B11000,
};

byte senial3[8] = {
  B11100,
  B11100,
  B11100,
  B11100,
  B11100,
  B11100,
  B11100,
  B11100,
};

byte senial4[8] = {
  B11110,
  B11110,
  B11110,
  B11110,
  B11110,
  B11110,
  B11110,
  B11110,
};

byte senial5[8] = {
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
};

byte unmedio[8] = {
  B10000,
  B10001,
  B10010,
  B10100,
  B01011,
  B10001,
  B00010,
  B00011,
};

byte trescuartos[8] = {
  B11000,
  B01000,
  B11000,
  B01011,
  B11000,
  B00101,
  B00111,
  B00001,
};

byte uncuarto[8] = {
  B10000,
  B10001,
  B10010,
  B10100,
  B01000,
  B10101,
  B00111,
  B00001,
};

void setup() {
  lcd.createChar(1, senial1);
  lcd.createChar(2, senial2);
  lcd.createChar(3, senial3);
  lcd.createChar(4, senial4);
  lcd.createChar(5, senial5);
  lcd.createChar(6, unmedio);
  lcd.begin(16,2);
  lcd.print(" MEGAVIEWER 0.2");
  lcd.setCursor(0, 1);
  delay(3000);
  lcd.setCursor(0, 0);
  lcd.print("Conectando...   ");
  Serial.begin(9600);
  while(!ini){
    Serial.write('T');
    delay(200);
    if(Serial.available()>0)
      lcd.setCursor(0,1);
    while(Serial.available()>0){
      ini=true;
      lcd.setCursor(i%16, i/16);
      lcd.write(Serial.read());
      i++;
    }
  }
  delay(2000);
  lcd.clear();
}

void shifts() {
 if(afr>14 && mapp>90 && hiResRPM>2000) {
   digitalWrite(shiftPin, HIGH);
 } else {
   digitalWrite(shiftPin, LOW);
 }
}

float boostcalc() {
  float boost = (mapp*(.00488)/(.022)*(0.147)-1.3);
  return boost;
}


void getmegadata() {
   if(Serial.available()>0)
    Serial.flush();
  Serial.print('R');
  i=0;
  delay(100);

  if(Serial.available()<39)
    delay(100);
  else if (Serial.available()==39){
    lcd.home();
    for(i=0;i<39;i++){
      readB=Serial.read();
      switch(i){

        case 23: //LSB for hi-res RPM 
          iTimeL = readB;
        break;
        case 22:
          iTimeM = readB;
        break;

        case 13: //hi-res RPM
          iTimeFull = ((iTimeX*65536)+(iTimeM*256)+iTimeL);
          hiResRPM = (60000000*2/(iTimeFull*4));
        break;
      
        case 5: //IAT
          iatF=temperatura[readB]; //Obtengo la temperatura en Fahrenheit
          iat=(iatF - 32)* 5/9; //La convierto a Celsius
        break;
  
        case 6: //CLT
          cltF=temperatura[readB]; //Obtengo la temperatura en Fahrenheit
          clt=(cltF - 32)* 5/9; //La convierto a Celsius
        break;

        case 4: //MAP
          mapp=readB;
        break;

        case 24: //Avance 
          avance=(int)((readB*0.352)-10);       
        break;
        
        case 8:
          battv = (readB / 255.0 * 30.0);
        break;
        
        case 9: // AFR
          afr = (10 + (readB * 0.039216));
          lambda = afr/14.7; //Por las dudas
        break;
        
        case 31: // Booster
          if(bitRead(readB,3))
            boostcontrol=1;
        break;
        
        case 34: // Launch
          if(bitRead(readB,3))
            launchcontrol=1;
        break;
        
        case 37: //MSB for hi-res RPM
          iTimeX = readB;
        break;       
       
    }
  }
}else {
    lcd.setCursor(0,1);
    lcd.print("error:"+Serial.available());
    serialFlush();
  }
  delay(50); 
}



void loop() {
  getmegadata();
  shifts();
  buttonState1 = digitalRead(buttonPin1);

  if (buttonState1 != buttonLastState1) {
    if (buttonState1 == HIGH) {
      screenNumber++;
      lcd.clear();
      if (screenNumber >= 7) {
  screenNumber = 0;
      }
    }
  }
  buttonLastState1 = buttonState1;
  if (screenNumber == 0) {
    Screen1();
  }
  if (screenNumber == 1) {
    Screen2();
  }
  if (screenNumber == 2) {
    Screen3();
  }
  if (screenNumber == 3) {
    Screen4();
  }
  if (screenNumber == 4) {
    Screen5();
  }
  if (screenNumber == 5) {
    Screen6();
  }  
  if (screenNumber == 6) {
    Screen7();
  }  
}

void serialFlush(void){
  while(Serial.available()>0)
    Serial.read();
}

void Screen1() {
  lcd.setCursor(0, 0);
  lcd.print("RPM:");
  lcd.print("     ");
  lcd.setCursor(4, 0);
  lcd.print(hiResRPM);
  lcd.setCursor(9, 0);
  lcd.print("MAP:");
  lcd.print("     ");
  lcd.setCursor(13, 0);
  lcd.print(mapp);
  lcd.setCursor(0, 1);
  lcd.print("ADV:");
  lcd.print("     ");
  lcd.setCursor(4, 1);
  lcd.print(avance);
  lcd.print((char)223); 
  lcd.setCursor(9, 1);
  lcd.print("TMP:");
  lcd.print("     ");
  lcd.setCursor(13, 1);
  lcd.print(clt);
  lcd.print((char)223);
}

void Screen2() {
  lcd.setCursor(0, 0);
  lcd.print("RPM:");
  lcd.print("     ");
  lcd.setCursor(4, 0);
  lcd.print(hiResRPM);
  lcd.print("");
  nivelgraph(map(hiResRPM,600,7800,1,90));
}

void Screen3() {
  lcd.setCursor(0, 0);
  lcd.print("MAP:");
  lcd.print("     ");
  lcd.setCursor(4, 0);
  lcd.print(mapp);
  lcd.print("");
}

void Screen4() {
  lcd.setCursor(0,0);
  lcd.print("0");
  lcd.setCursor(7,0);
  lcd.write(byte(6));
  lcd.setCursor(15,0);
  lcd.print("1");
  nivel = analogRead(A3);
  int nivelnum = (nivel / 12.8);
  nivelgraph(nivelnum);
}

void Screen5() {
  lcd.setCursor(0, 0);
  lcd.print("TMP:");
  lcd.print("    ");
  lcd.setCursor(4, 0);
  lcd.print(clt);
  lcd.print((char)223);
  lcd.print("");
  nivelgraph(map(clt,10,110,1,90));
}

void Screen6() {
  lcd.setCursor(0, 0);
  lcd.print("AFR:");
  lcd.print("    ");
  lcd.setCursor(4, 0);
  lcd.print(afr);
  lcd.print("");
  nivelgraph(map(afr,10,20,1,90));
}

void Screen7() {
  lcd.setCursor(0, 0);
  lcd.print("BAT:");
  lcd.print("    ");
  lcd.setCursor(4, 0);
  lcd.print(battv);
  lcd.print("");
  nivelgraph(map(battv,10,16,1,90));
}

void nivelgraph(int lineas){
  int bloque = (lineas / 5);
  int linea = (lineas % 5);
  lcd.setCursor(0, 1);
  for (int i=0; i<bloque;i++) {
    lcd.write(byte(5));  //Imprimo bloque entero
  }
  lcd.write(byte(linea + 1));
  for (int i=(bloque + 1); i<17; i ++) {
    lcd.print(" "); //Limpio espacios cuando voy para atras
  }  
}
