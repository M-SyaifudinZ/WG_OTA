# WG_OTA — ESP32 WireGuard + ElegantOTA

Firmware ESP32 yang konek ke WiFi, bikin tunnel VPN ke server WireGuard, lalu jalanin web server dengan OTA update (ElegantOTA) di jaringan VPN tersebut.

## Fitur

- Koneksi WiFi otomatis (STA mode)
- Sinkronisasi waktu via NTP (dibutuhkan untuk handshake WireGuard)
- Tunnel WireGuard ke server VPN
- Web server async (`ESPAsyncWebServer`)
- OTA update lewat browser dengan autentikasi (`ElegantOTA`)

## Struktur Project

```
.
├── include/
│   ├── config.h            # kredensial ASLI
│   └── config_public.h     # template
├── src/
│   └── main.cpp
├── platformio.ini
└── .gitignore
```

## Persiapan (Setup Awal)

1. Clone repo ini

   ```bash
   git clone git@github.com:M-SyaifudinZ/WG_OTA.git
   cd WG_OTA
   ```

2. Copy template config jadi config asli

   ```bash
   cp include/config_public.h include/config.h
   ```

3. Buka `include/config.h`, isi dengan data kamu:

   | Define                          | Keterangan                                                        |
   | ------------------------------- | ----------------------------------------------------------------- |
   | `WIFI_SSID` / `WIFI_PASSWORD`   | Kredensial WiFi lokal                                             |
   | `WG_LOCAL_IP`                   | IP device ini di dalam jaringan VPN, format `a,b,c,d` tanpa kutip |
   | `WG_PRIVATE_KEY`                | Private key WireGuard milik device ini                            |
   | `WG_ENDPOINT_ADDRESS`           | IP / domain server WireGuard                                      |
   | `WG_ENDPOINT_PUBLIC_KEY`        | Public key server WireGuard                                       |
   | `WG_ENDPOINT_PORT`              | Port server WireGuard (angka, contoh `51820`)                     |
   | `OTA_USERNAME` / `OTA_PASSWORD` | Login untuk halaman OTA (`/update`)                               |

4. Build & upload lewat PlatformIO
   ```bash
   pio run -t upload
   pio device monitor
   ```

## Update Firmware via OTA

Setelah device konek WiFi/VPN, buka:

```
http://<ip-device>/update
```

Login pakai `OTA_USERNAME` / `OTA_PASSWORD` dari `config.h`, lalu upload file `.bin` hasil build.

## Catatan Keamanan

- **Jangan pernah commit `include/config.h`** — file ini sudah otomatis di-ignore lewat `.gitignore`. Sebelum commit apapun, selalu cek dulu:
  ```bash
  git status
  ```
  pastikan `include/config.h` **tidak** muncul di daftar file yang mau di-commit.
- Kalau private key atau password OTA pernah gak sengaja ke-push / ke-share, anggap sudah bocor — generate ulang keypair WireGuard dan ganti password OTA.

## Lisensi

Bebas dipakai/dimodifikasi untuk keperluan pribadi.
