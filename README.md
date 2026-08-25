## INSTALLAZIONE
Scarica ed installa arduino-cli da arduino.cc 

Inizializza la configurazione
```sh
arduino-cli config init --overwrite
```

Aggiungi l'URL del board manager Heltec
```sh
arduino-cli config add board_manager.additional_urls https://github.com/HelTecAutomation/Heltec_nRF52/releases/download/1.7.0/package_heltec_nrf_index.json
```

Aggiorna l'indice
```sh
arduino-cli core update-index
```

Installa il core Heltec nRF52
```sh
arduino-cli core install Heltec_nRF52:Heltec_nRF52
```

Verifica il nome esatto della tua board
```sh
arduino-cli board listall Heltec_nRF52
```

Installa le librerie necessarie
```sh
arduino-cli lib install "TinyGPSPlus"
arduino-cli lib install "Adafruit GFX Library"
arduino-cli lib install "Adafruit BusIO"
arduino-cli lib install "Adafruit ST7735 and ST7789 Library"
```

Collega la board e trova la porta
```sh
arduino-cli board list
```

Compila
```sh
arduino-cli compile -b Heltec_nRF52:Heltec_nRF52:<NOME_BOARD> .
```

upload
```sh
arduino-cli upload -p COM6 -b Heltec_nRF52:Heltec_nRF52:<NOME_BOARD>
```

Shortcut per powershell
```sh
 arduino-cli compile -b Heltec_nRF52:Heltec_nRF52:HT-n5262  .; if ($?) {arduino-cli upload -p COM6 -b Heltec_nRF52:Heltec_nRF52:HT-n5262 .}; if ($?) {arduino-cli monitor -p COM6 -c baudrate=115200} 
```