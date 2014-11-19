

#include "LedControlMS.h"

/*

 Now we need a LedControl to work with.
 ***** These pin numbers will probably not work with your hardware *****
 pin 12 is connected to the DataIn 
 pin 11 is connected to the CLK 
 pin 10 is connected to LOAD 
 We have only a single MAX72XX.
 */
LedControl lc=LedControl(12,11,10,1);

int led = 13;
int switchpin = 8;
int sensepin = A0;
int treshold = 900;
boolean statusdarurat = false;
boolean pintubolehdibuka = true; // representasi kondisi pintu

void setup(){
  analogReference(DEFAULT);
  Serial.begin(9600);
  
  pinMode(led,OUTPUT);
  pinMode(switchpin,INPUT);
  
  /*
   The MAX72XX is in power-saving mode on startup,
   we have to do a wakeup call
   */
  lc.shutdown(0,false);
  /* Set the brightness to a medium values */
  lc.setIntensity(0,8);
  /* and clear the display */
  lc.clearDisplay(0);
}

void loop(){
  /**
  * Tahap I
  * Mendeteksi kondisi darurat
  */
  if(digitalRead(switchpin) == HIGH){
    statusdarurat= true;
    //digitalWrite(led,HIGH);
  }else{
    statusdarurat=false;
    //digitalWrite(led,LOW);
  }
  
  
  
  /**
  * Tahap II
  * Membaca Nilai pembacaan sensor
  */
  // Membaca sensor
  int val = analogRead(sensepin);
  Serial.print("Kadar amonia:");
  Serial.println(val);// digunakan unntuk menampilkan bera nilai yang dideteksi sensor pada layar komputer.
  delay(800);
  // jika nilai sensor yang dibaca melebihi ambang batas, berarti ditemukan zat amonia diudara
  // pintu dikunci jika ditemukan amonia dan kondisi tidak dalam keadaan darurat, selain itu semua, pintu boleh dibuka
  if((val>treshold) && (!statusdarurat)){
    pintubolehdibuka=false;
  }else{
    pintubolehdibuka=true;
  }




  /**
  * Tahap III
  * Menentukan aksi yang dipilih apakah pintu dibuka / ditutup.
  */
  if(pintubolehdibuka){
    DisplaySenyum();
    BukaPintu();
  }else{
    DisplaySedih();
    TutupPintu();
  }
  
}


void DisplaySenyum(){
  /**
  *  Gunakan Library Max7219 untuk menggambar smiley senyum
  *
  */  
  byte rows[8] = {B00111100,
                  B01000010,
                  B10101001,
                  B10000101,
                  B10000101,
                  B10101001,
                  B01000010,
                  B00111100};
  for(int i=0; i<8; i++){
    lc.setRow(0,i,rows[i]);
  }
}

void DisplaySedih(){
  /**
  *  Gunakan Library Max7219 untuk menggambar smiley sedih
  *
  */    
  byte rows[8] = {B00111100,
                  B01000010,
                  B10100101,
                  B10001001,
                  B10001001,
                  B10100101,
                  B01000010,
                  B00111100};
  for(int i=0; i<8; i++){
    lc.setRow(0,i,rows[i]);
  } 
}

void BukaPintu(){
  // Tulis kode yang merepresentasikan pintu dibuka, untuk sementara ini, dianggap pintu
  // terbuka pin led mati..
  digitalWrite(led,LOW);
}

void TutupPintu(){
  // Tulis kode yang merepresentasikan pintu ditutup, untuk sementara ini, dianggap pintu
  // terbuka pin led hidup..
  digitalWrite(led,HIGH);
}


