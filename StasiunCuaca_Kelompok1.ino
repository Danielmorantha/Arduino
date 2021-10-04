/*
Kelompok 1 : Stasiun Cuaca
NAMA        : Daniel Morantha ( 2019230088 )

*/

#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <Adafruit_BME280.h>
#include <Adafruit_Sensor.h>

#define SEALEVELPRESSURE_HPA (1013.25)

// sambung wifi
const char* ssid = "wifi";
const char* password = "12345678";

//buat webs server sinkron
AsyncWebServer server(80);

AsyncEventSource events("/events");

// waktu pending
unsigned long lastTime = 0;  
unsigned long timerDelay = 30000;

// memanggil sensor BME
Adafruit_BME280 bme;

float suhu_celcius;
float suhu_fahrenheit;
float Kelembapan_Udara;
float Tekanan_Udara;
float Ketinggian_Permukaan_Laut;
// Init BME280
void initBME(){
    if (!bme.begin(0x76)) {
    Serial.println("Gagal, Periksa Kabel");
    while (1);
  }
}

void getSensorReadings(){
  suhu_celcius              = bme.readTemperature();
  suhu_fahrenheit           = 1.8 * bme.readTemperature() + 32;
  Kelembapan_Udara          = bme.readHumidity();
  Tekanan_Udara             = bme.readPressure()/ 100.0F;
  Ketinggian_Permukaan_Laut = bme.readAltitude(SEALEVELPRESSURE_HPA);
}

// Initialize WiFi
void initWiFi() {
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
    Serial.print("Connecting to WiFi ..");
    while (WiFi.status() != WL_CONNECTED) {
        Serial.print('.');
        delay(1000);
    }
    Serial.println(WiFi.localIP());
}

String processor(const String& var){
  getSensorReadings();
  //Serial.println(var);
  if(var == "SuhuCelcius"){
    return String(suhu_celcius);
  }
  else if(var == "SuhuFahrenheit"){
    return String(suhu_fahrenheit);
  }
  else if(var == "KelembapanUdara"){
    return String(Kelembapan_Udara);
  }
  else if(var == "TekananUdara"){
    return String(Tekanan_Udara);
  }
  else if(var == "KetinggianPermukaanLaut"){
    return String(Ketinggian_Permukaan_Laut);
  }
  return String();
}

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html>
<head>
  <title>Stasiun Cuaca</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <link rel="stylesheet" href="https://use.fontawesome.com/releases/v5.7.2/css/all.css" integrity="sha384-fnmOCqbTlWIlj8LyTjo7mOUStjsKC4pOpQbqyi7RrhN7udi9RwhKkMHpvLbHG9Sr" crossorigin="anonymous">
  <link rel="icon" href="data:,">
  <style>
    html {font-family: Arial; display: inline-block; text-align: center;}
    p { font-size: 1.2rem;}
    body {  margin: 0;}
    .topnav { overflow: hidden; background-color: #50B8B4; color: white; font-size: 1rem; }
    .content { padding: 20px; }
    .card { background-color: white; box-shadow: 2px 2px 12px 1px rgba(140,140,140,.5); }
    .cards { max-width: 800px; margin: 0 auto; display: grid; grid-gap: 2rem; grid-template-columns: repeat(auto-fit, minmax(200px, 1fr)); }
    .reading { font-size: 1.4rem; }
  </style>
</head>
<body>
  <div class="topnav">
    <h1>Stasiun Cuaca Kelompok 1</h1>
  </div>
  <div class="content">
    <div class="cards">
      <div class="card">
        <p><i class="fas fa-thermometer-half" style="color:#00add6;"></i> Suhu Celcius</p><p><span class="reading"><span id="SuhuCelcius">%SuhuCelcius%</span> &deg;C</span></p>
      </div>
      <div class="card">
        <p><i class="fas fa-thermometer-three-quarters" style="color:#db1514;"></i> Suhu Fahrenheit</p><p><span class="reading"><span id="SuhuFahrenheit">%SuhuFahrenheit%</span> &deg;F</span></p>
      </div>
      <div class="card">
        <p><i class="fas fa-tint" style="color:#dbd414;"></i> Kelembapan Udara</p><p><span class="reading"><span id="KelembapanUdara">%KelembapanUdara%</span>&percnt;</span></p>
      </div>
      <div class="card">
        <p><i class="fas fa-wind" style="color:#0d9eab;"></i> Tekanan Udara</p><p><span class="reading"><span id="TekananUdara">%TekananUdara%</span> hPa</span></p>
      </div>
      <div class="card">
        <p><i class="fas fa-water" style="color:#0d9eab;"></i>Ketinggian Permukaan Laut</p><p><span class="reading"><span id="KetinggianPermukaanLaut">%KetinggianPermukaanLaut%</span> m</span></p>
      </div>
    </div>
  </div>
<script>
if (!!window.EventSource) {
 var source = new EventSource('/events');
 
 source.addEventListener('open', function(e) {
  console.log("Events Connected");
 }, false);
 source.addEventListener('error', function(e) {
  if (e.target.readyState != EventSource.OPEN) {
    console.log("Events Disconnected");
  }
 }, false);
 
 source.addEventListener('message', function(e) {
  console.log("message", e.data);
 }, false);
 
 source.addEventListener('SuhuCelcius', function(e) {
  console.log("SuhuCelcius", e.data);
  document.getElementById("SuhuCelcius").innerHTML = e.data;
 }, false);

  source.addEventListener('SuhuFahrenheit', function(e) {
  console.log("SuhuFahrenheit", e.data);
  document.getElementById("SuhuFahrenheit").innerHTML = e.data;
 }, false);


 
 source.addEventListener('KelembapanUdara', function(e) {
  console.log("KelembapanUdara", e.data);
  document.getElementById("KelembapanUdara").innerHTML = e.data;
 }, false);
 
 source.addEventListener('TekananUdara', function(e) {
  console.log("TekananUdara", e.data);
  document.getElementById("TekananUdara").innerHTML = e.data;
 }, false);

 source.addEventListener('KetinggianPermukaanLaut', function(e) {
  console.log("KetinggianPermukaanLaut", e.data);
  document.getElementById("KetinggianPermukaanLaut").innerHTML = e.data;
 }, false);
}
</script>
</body>
</html>)rawliteral";

void setup() {
  Serial.begin(9600);
  initWiFi();
  initBME();


  // sinkron web server
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html, processor);
  });

  events.onConnect([](AsyncEventSourceClient *client){
    if(client->lastId()){
      Serial.printf("Menghubungkan kembali: %u\n", client->lastId());
    }

    client->send("Berhasil!", NULL, millis(), 10000);
  });
  server.addHandler(&events);
  server.begin();
}

void loop() {
  if ((millis() - lastTime) > timerDelay) {
    getSensorReadings();
    Serial.printf("Suhu Celcius = %.2f ºC \n", suhu_celcius);
    Serial.printf("Suhu Fahrenheit = %.2f ºF \n", suhu_fahrenheit);
    Serial.printf("Kelembapan Udara = %.2f % \n", Tekanan_Udara);
    Serial.printf("Ketinggian Permukaan Laut = %.2f m \n", Ketinggian_Permukaan_Laut);
    Serial.printf("Tekanan Udara = %.2f hPa \n", Tekanan_Udara);
    Serial.println();

    // Kirim data sensor ke web secara sinkron
    events.send("ping",NULL,millis());
    events.send(String(suhu_celcius).c_str(),"SuhuCelcius",millis());
    events.send(String(suhu_fahrenheit).c_str(),"SuhuFahrenheit",millis());
    events.send(String(Kelembapan_Udara).c_str(),"KelembapanUdara",millis());
    events.send(String(Tekanan_Udara).c_str(),"TekananUdara",millis());
    events.send(String(Ketinggian_Permukaan_Laut).c_str(),"KetinggianPermukaanLaut",millis());

    lastTime = millis();
  }
}
