# IoT Smart Home ESP32 dengan Running Text (MAX7219)

Proyek Smart Home berbasis ESP32 yang beroperasi sebagai jaringan mandiri (Access Point). Sistem ini dapat mengontrol perangkat listrik (seperti lampu dan kipas) melalui relay, serta menampilkan pesan teks berjalan (running text) secara dinamis pada modul LED Matrix MAX7219. Seluruh sistem dapat dikendalikan menggunakan aplikasi Android (dibuat via MIT App Inventor).

## 🌟 Fitur Utama
* **Standalone WiFi (Access Point):** ESP32 memancarkan jaringan WiFi sendiri ("SmartHome"), sehingga tidak memerlukan router atau koneksi internet luar.
* **Kontrol Perangkat AC/DC:** Menggunakan modul relay untuk menghidupkan/mematikan 2 perangkat (Lampu dan Kipas).
* **Dynamic Running Text:** Menampilkan teks berjalan pada modul MAX7219 (4 blok). Teks dapat diubah secara *real-time* melalui aplikasi.
* **Sistem Non-Blocking:** Menggunakan library `MD_Parola` dan fungsi `millis()` agar animasi teks tidak mengganggu responsivitas kontrol relay.

## 🛠️ Komponen Perangkat Keras (Hardware)
1.  ESP32 Development Board
2.  Modul Dot Matrix MAX7219 (4 in 1)
3.  Modul Relay 2-Channel
4.  Power Supply Eksternal 5V (Minimal 1A - **Sangat Penting** untuk kestabilan LED Matrix)
5.  Kabel Jumper secukupnya

## 🔌 Skema Rangkaian (Wiring)

**PENTING:** Modul MAX7219 membutuhkan daya yang cukup besar. Sangat disarankan untuk memberi daya `VCC` MAX7219 langsung dari Power Supply Eksternal 5V, bukan dari pin 5V ESP32 untuk menghindari *brownout* atau display yang error.

| MAX7219 | ESP32 / Power Supply |
| :--- | :--- |
| VCC | (+) Power Supply Eksternal 5V |
| GND | (-) Power Supply & GND ESP32 |
| DIN | GPIO 23 |
| CS | GPIO 5 |
| CLK | GPIO 18 |

| Relay 2-Channel | ESP32 |
| :--- | :--- |
| VCC | VIN / 5V ESP32 |
| GND | GND ESP32 |
| IN1 (Lampu) | GPIO 27 |
| IN2 (Kipas) | GPIO 26 |

## 📚 Library yang Dibutuhkan
Pastikan Anda telah menginstal library berikut melalui **Library Manager** di Arduino IDE sebelum melakukan *compile* dan *upload*:
1.  `MD_Parola` oleh majicDesigns
2.  `MD_MAX72XX` oleh majicDesigns

*(Library `WiFi` dan `WebServer` sudah bawaan dari board package ESP32).*

## 🚀 Cara Penggunaan
1.  Rangkai komponen sesuai skema tabel di atas.
2.  Buka kode program (`.ino`) di Arduino IDE.
3.  Upload kode ke ESP32.
4.  Buka pengaturan WiFi di HP Anda, cari dan sambungkan ke jaringan bernama **"SmartHome"**.
5.  Buka aplikasi kontrol Anda (MIT App Inventor) atau browser web, lalu akses alamat IP ESP32 (biasanya `192.168.4.1`).

## 📡 API Endpoints (Untuk Aplikasi)
Sistem ini menggunakan HTTP GET requests sederhana untuk dikontrol:
* `/lampu_on` : Menyalakan lampu
* `/lampu_off` : Mematikan lampu
* `/kipas_on` : Menyalakan kipas
* `/kipas_off` : Mematikan kipas
* `/ubah_text?pesan=TEKS_BARU` : Mengubah tulisan running text menjadi "TEKS_BARU"
