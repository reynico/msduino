// Made with love by Matias Ozdy (Stuff) and Nicolas Rey (Nano)

#include <LiquidCrystal.h>
LiquidCrystal lcd(12, 11, 10, 9, 8, 7);
int index = 0;
byte inData[209];

void setup() {
  lcd.begin(20, 4);
  Serial.begin(115200);
  lcd.setCursor(0,0);
  lcd.print(" Megaviewer  2!");
  delay(2000);
  lcd.clear();
}

void loop() {
  Serial.write("A");
  while(Serial.available()<20) {}
  if (Serial.available() >0) {
    delay(15);
    for(index=0;index<209;index++) {
      inData[index] = Serial.read();
    }
  }
  //Do-magic
  showData();
  delay(100);
}

void showData() {
  //Tomo dos bytes para cada valor que lo necesite
  int rpm1 = inData[6];
  int rpm2 = inData[7];
  int adv  = inData[9];
  int map1 = inData[18];
  int map2 = inData[19];
  int mat1 = inData[20];
  int mat2 = inData[21];
  int clt1 = inData[22];
  int clt2 = inData[23];
  int tps1 = inData[24];
  int tps2 = inData[25];
  int bat1 = inData[26];
  int bat2 = inData[27];
  int afr1 = inData[28];
  int afr2 = inData[29];

  //Calculo el valor final sumando los valores altos y bajos
  float clt = (clt1*256+clt2)*0.1;
  float afr = (afr1*256+afr2)*0.1;
  float map = (map1*256+map2)*0.1;
  float mat = (mat1*256+mat2)*0.1;
  float bat = (bat1*256+bat2)*0.1;
  float tps = (tps1*256+tps2)*0.1;

  lcd.setCursor(0,0);
  lcd.write("ADV: ");
  lcd.print("   ");
  lcd.setCursor(4,0);
  lcd.print(adv* 0.1,1);

  lcd.setCursor(0,1);
  lcd.write("AFR: ");
  lcd.print("   ");
  lcd.setCursor(4,1);
  lcd.print(afr,1);

  lcd.setCursor(0,2);
  lcd.write("RPM: ");
  lcd.print("   ");
  lcd.setCursor(4,2);
  lcd.print(rpm1*256+rpm2);

  lcd.setCursor(0,3);
  lcd.write("CLT: ");
  lcd.print("   ");
  lcd.setCursor(4,3);
  lcd.print(clt,0);
  lcd.print("gr");

  //Valores de la derecha
  lcd.setCursor(10,0);
  lcd.write("MAP: ");
  lcd.print("   ");
  lcd.setCursor(14,0);
  lcd.print(map,0);
  lcd.print("kpa");

  lcd.setCursor(10,1);
  lcd.write("MAT: ");
  lcd.print("   ");
  lcd.setCursor(14,1);
  lcd.print(mat,0);
  lcd.print("gr");

  lcd.setCursor(10,2);
  lcd.write("BAT: ");
  lcd.print("   ");
  lcd.setCursor(14,2);
  lcd.print(bat,1);
  lcd.print("v");

  lcd.setCursor(10,3);
  lcd.write("TPS: ");
  lcd.print("   ");
  lcd.setCursor(14,3);
  lcd.print(tps,0);
  lcd.print("%");
}
