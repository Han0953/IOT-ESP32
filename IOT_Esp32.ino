/*
 * KODE LENGKAP IOT SMART HOME (Versi Final & Stabil)
 * Disesuaikan untuk Arduino IDE
 * Menggunakan metode non-blocking untuk animasi yang lebih stabil.
 */

#include <WiFi.h>
#include <WebServer.h>
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Max72xxPanel.h>

// --- KONFIGURASI JARINGAN ---
const char* ssid = "SmartHome";
const char* password = "";

// --- KONFIGURASI PIN PERANGKAT ---
const int PIN_RELAY_LAMPU = 27;
const int PIN_RELAY_KIPAS = 26;

// --- KONFIGURASI PIN DISPLAY MAX7219 ---
const int pinCS = 5;
const int numberOfHorizontalDisplays = 4;
const int numberOfVerticalDisplays = 0;

// Inisialisasi Web Server
WebServer server(80);

// Inisialisasi display
Max72xxPanel matrix = Max72xxPanel(pinCS, numberOfHorizontalDisplays, numberOfVerticalDisplays);

// --- Variabel untuk Animasi Non-Blocking ---
String runningText = "SELAMAT DATANG DI SMART HOME"; // Teks awal berjalan dan jangan terbalik
int scroll_x; // Posisi x untuk scroll
unsigned long lastScrollTime = 0;
const int SCROLL_SPEED = 50; // Kecepatan scroll dalam milidetik

// --- FUNGSI-FUNGSI KONTROL (WEB HANDLER) ---

void handleLampuON() {
  digitalWrite(PIN_RELAY_LAMPU, HIGH);
  server.send(200, "text/plain", "Lampu ON");
}

void handleLampuOFF() {
  digitalWrite(PIN_RELAY_LAMPU, LOW);
  server.send(200, "text/plain", "Lampu OFF");
}

void handleKipasON() {
  digitalWrite(PIN_RELAY_KIPAS, HIGH);
  server.send(200, "text/plain", "Kipas ON");
}

void handleKipasOFF() {
  digitalWrite(PIN_RELAY_KIPAS, LOW);
  server.send(200, "text/plain", "Kipas OFF");
}

void handleUbahText() {
  if (server.hasArg("pesan")) {
    runningText = server.arg("pesan");
    runningText.toUpperCase();
    scroll_x = matrix.width(); // Reset posisi scroll saat teks diubah
    server.send(200, "text/plain", "Teks diubah menjadi: " + runningText);
  } else {
    server.send(400, "text/plain", "Error: Argumen 'pesan' tidak ditemukan");
  }
}

void handleRoot() {
  server.send(200, "text/plain", "Server ESP32 Aktif!");
}

// --- FUNGSI SETUP ---
void setup() {
  Serial.begin(115200);

  pinMode(PIN_RELAY_LAMPU, OUTPUT);
  pinMode(PIN_RELAY_KIPAS, OUTPUT);
  digitalWrite(PIN_RELAY_LAMPU, LOW);
  digitalWrite(PIN_RELAY_KIPAS, LOW);

  // Setup display
  matrix.setIntensity(7);
  matrix.setRotation(4); // Nilai rotasi terakhir yang kita coba
  scroll_x = matrix.width(); // Atur posisi awal scroll
  
  // Membuat Access Point (AP)
  Serial.print("\nMembuat Access Point: ");
  Serial.println(ssid);
  WiFi.softAP(ssid, password);

  IPAddress myIP = WiFi.softAPIP();
  Serial.print("Alamat IP AP: ");
  Serial.println(myIP);

  // Daftarkan URL endpoint
  server.on("/", handleRoot);
  server.on("/lampu_on", handleLampuON);
  server.on("/lampu_off", handleLampuOFF);
  server.on("/kipas_on", handleKipasON);
  server.on("/kipas_off", handleKipasOFF);
  server.on("/ubah_text", handleUbahText);

  server.begin();
  Serial.println("HTTP server dimulai");
}

// --- FUNGSI LOOP (Struktur Non-Blocking yang Benar) ---
void loop() {
  // Selalu tangani permintaan dari client. Ini akan berjalan sangat cepat dan responsif.
  server.handleClient();

  // Cek apakah sudah waktunya untuk menggeser teks
  if (millis() - lastScrollTime > SCROLL_SPEED) {
    lastScrollTime = millis(); // Catat waktu terakhir scroll

    // Hapus layar
    matrix.fillScreen(LOW);

    // Atur posisi kursor
    matrix.setCursor(scroll_x, 0);

    // Cetak string
    matrix.print(runningText);

    // Tampilkan ke display
    matrix.write();

    // Geser posisi untuk frame berikutnya
    scroll_x--;

    // Jika teks sudah hilang dari layar, reset posisinya ke kanan
    // 6 adalah lebar karakter (5) + spasi (1)
    if (scroll_x > -(runningText.length() * 6)) {
      scroll_x = matrix.width();
    }
  }
}