<h2 align="center">VFO/BFO con SI5351A ed ESP32</h2>

VFO & BFO con Si5351 gestito da ESP32 DevKit V1 per la gamma di frequenze HF, da 1 a 30 MHz.

![Version](https://img.shields.io/badge/version-2.1.0-blue)
![Hardware](https://img.shields.io/badge/hardware-V1.1-green)
![License](https://img.shields.io/badge/license-MIT-orange)

## 📺 Video del progetto

<p align="center">
  <a href="https://youtu.be/RLuy1z0uWSg">
    <img src="https://img.youtube.com/vi/RLuy1z0uWSg/maxresdefault.jpg" width="700">
  </a>
</p>

## 📋 Caratteristiche principali

- Generazione VFO e BFO indipendenti con SI5351A
- Gestione modi: AM, LSB, USB, CW
- Controllo AGC (Fast/Slow) e attenuatore (-20dB)
- Pitch BFO regolabile (±300 Hz, step 5 Hz)
- S-meter digitale con memoria di picco
- Supporto encoder ottico (400 PPR) e meccanico (24 PPR)
- Display TFT a colori (TFT_eSPI)
- Tastiera a matrice 4x4 via PCF8574T per inserimento frequenza
- Espansione I/O tramite PCF8574 (8 bit)
- Memorizzazione configurazioni su EEPROM 24LC256 (32KB)
- Firmware modulare in C++
- Calibrazione SI5351 automatica e manuale

## ✨ Novità della versione 2.1.0

Questa release introduce miglioramenti significativi nella gestione della frequenza, nell'interfaccia utente e nella calibrazione.

### 🎯 Frequenza visualizzata sempre corretta
- **IF dinamica per modalità**: la frequenza del VFO viene calcolata dinamicamente in base alla modalità selezionata (AM, LSB, USB, CW), compensando l'offset del BFO.
- **Display allineato alla stazione reale**: la frequenza mostrata sul display corrisponde sempre a quella effettiva della stazione ricevuta, senza scostamenti di 1.5 kHz.
- **Confronto diretto con altri ricevitori**: ora è possibile confrontare la frequenza visualizzata con quella di un IC-756 o di un altro apparato commerciale.

### 📻 Cambio automatico della banda laterale
- **LSB sotto 10 MHz, USB sopra 10 MHz**: al cambio banda, la modalità viene impostata automaticamente in base alla frequenza.
- **Funziona con tutte le fonti**: il cambio automatico si attiva sia cambiando banda, sia ruotando l'encoder, sia inserendo la frequenza da tastiera.
- **Rispetto delle modalità AM e CW**: se l'utente ha selezionato AM o CW, il cambio automatico non viene applicato.

### ⌨️ Miglioramenti alla tastiera 4x4
- **Limiti sui tasti `*` e `#`**: il cambio step si ferma ai limiti minimo (10 Hz) e massimo (10 kHz), evitando valori non validi.
- **Inserimento frequenza più intuitivo**: i punti separatori vengono aggiunti automaticamente dopo la 2ª e la 5ª cifra.
- **Timeout con ripristino corretto**: dopo 5 secondi di inattività, il display torna alla frequenza corrente senza lasciare la modalità input.

### 🖥️ Nuova schermata iniziale
- **Logo e informazioni di sistema**: la schermata iniziale mostra logo, versione hardware, versione firmware, autore e lista dei dispositivi I2C rilevati.
- **Rilevamento automatico EEPROM**: il sistema identifica il tipo di EEPROM (24LC32, 24LC64, 24LC128, 24LC256) in base alla dimensione configurata.

### 🔧 Calibrazione SI5351 migliorata
- **Calibrazione automatica con `CAL_FREQ`**: inserendo la frequenza misurata con un frequenzimetro, il sistema calcola e applica il fattore di correzione ottimale.
- **Calibrazione manuale con `CAL_MAN`**: per utenti esperti, è possibile impostare direttamente il fattore di correzione.
- **Istruzioni integrate**: i comandi `CAL_MAN?` e `CAL_FREQ?` mostrano le procedure passo-passo.
- **Maggiore precisione ad alta frequenza**: la calibrazione è più efficace se eseguita su frequenze elevate (es. 28 MHz).

### 🐛 Bugfix e miglioramenti
- Corretto il disallineamento di 1.5 kHz tra frequenza visualizzata e frequenza reale in LSB/USB.
- Risolto il problema del font che rimaneva ingrandito dopo un errore di inserimento frequenza.
- Corretto il salvataggio della frequenza inserita da tastiera in EEPROM.
- Migliorata la stabilità della comunicazione I2C con la EEPROM esterna.
- Rimossi i messaggi di debug ridondanti dal monitor seriale.

## 🔧 Comandi

| Comando | Descrizione |
|---------|-------------|
| **Encoder VFO** | Selezione frequenza (step 10Hz, 100Hz, 1kHz, 10kHz) |
| **Encoder BFO** | Regolazione pitch BFO (±300Hz) |
| **STEP** | Cambio step VFO |
| **BAND** | Selezione banda (160m → 10m) |
| **MODE** | Selezione demodulazione (AM/LSB/USB/CW) |
| **AGC** | Fast / Slow |
| **ATT** | Attenuatore -20dB ON/OFF |
| **Tastiera 4x4** | Inserimento diretto frequenza (0-9, A=conferma, C/#=annulla) |
| **Tasti `*` e `#`** | Step down / Step up (con limiti 10Hz-10kHz) |

## 🖥️ Comandi seriali

| Comando | Descrizione |
|---------|-------------|
| `CAL_MAN <valore>` | Imposta fattore di calibrazione manuale |
| `CAL_MAN?` | Istruzioni per la calibrazione manuale |
| `CAL_FREQ <freq>` | Calibrazione automatica (inserisci frequenza misurata) |
| `CAL_FREQ?` | Istruzioni per la calibrazione automatica |
| `CAL_READ` | Legge il fattore di calibrazione corrente |
| `CAL_RESET` | Resetta la calibrazione a 0 |
| `INFO` | Mostra informazioni di sistema |
| `HELP` | Mostra tutti i comandi disponibili |

### Esempio di calibrazione automatica

```text
> CAL_FREQ 28454710

=== CALIBRAZIONE AUTOMATICA ===
Target: 28455000 Hz
Misurato: 28454710 Hz
Errore: -290 Hz
Fattore calcolato: -10193
✅ Calibrazione automatica salvata.
```

🎬 **Guarda il video dimostrativo:**

<p align="center">
  <a href="https://youtu.be/rTzRa8fzhdM">
    <img src="https://img.youtube.com/vi/rTzRa8fzhdM/maxresdefault.jpg" width="700">
  </a>
</p>

## 🛠️ Architettura del codice
```text
VFO-BFO HF ESP32 Code/
├── src/
│ ├── main.cpp # Loop principale e gestione comandi seriale
│ ├── config.h # Pin, parametri e versioni
│ ├── display.cpp/h # Gestione display TFT e splash screen
│ ├── VFO_BFO.cpp/h # Gestione encoder VFO/BFO
│ ├── PLL.cpp/h # Configurazione SI5351
│ ├── bands.cpp/h # Gestione bande
│ ├── modes.cpp/h # Gestione modalità (AM/LSB/USB/CW)
│ ├── DigiOUT.cpp/h # Uscite digitali PCF8574
│ ├── s_meter.cpp/h # S-meter analogico
│ ├── functions.cpp/h # Gestione pulsanti AGC/ATT
│ ├── keypad.cpp/h # Gestione tastiera 4x4
│ └── EEPROM_manager.cpp/h # Memorizzazione configurazioni
└── platformio.ini # Configurazione PlatformIO
```

## 🔌 Collegamenti principali
```text
| Componente | Pin ESP32 |
|------------|-----------|
| VFO Encoder CLK | 34 |
| VFO Encoder DT | 35 |
| BFO Encoder CLK | 12 |
| BFO Encoder DT | 13 |
| STEP | 32 |
| BAND | 33 |
| MODE | 25 |
| AGC | 26 |
| ATT | 27 |
| I2C SDA | 21 |
| I2C SCL | 22 |
| S-Meter | 15 (ADC) |
```

## 📦 Dipendenze (platformio.ini)
```text
lib_deps = 
    bodmer/TFT_eSPI@^2.5.0
    madhephaestus/ESP32Encoder@^0.12.0
    robtillaart/PCF8574@^0.4.4
    robtillaart/I2CKeyPad@^0.5.2
```
## 📜 Licenza

MIT

## 👤 Autore

Scorpione Maker - IV3LDJ

GitHub: loscorpione

YouTube: @ScorpioneMaker

## ⭐ Se questo progetto ti è utile, lascia una stella su GitHub!
