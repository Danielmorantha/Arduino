//NAMA  : DANIEL MORANTHA
//NIM : 2019230088
//PROGRAM FLIP FLOP 2 LAMPU

void setup() {
  // Inisiasi pin led sebagai output
pinMode(0, OUTPUT);
pinMode(1, OUTPUT);
}

void loop() {
  digitalWrite(0, HIGH); //Tambah tegangan ke led 1 agar menyala
  digitalWrite(1, LOW); //Hilangkan tegangan ke led 1 agar tidak menyala
  delay(1000); //Delay selama 1 detik
  
  digitalWrite(0, LOW); //Hilangkan tegangan ke led 0 tidak agar menyala
  digitalWrite(1, HIGH); //Tambah tegangan ke led 1 agar  menyala
  delay(1000); //Delay selama 1 detik
  
}
