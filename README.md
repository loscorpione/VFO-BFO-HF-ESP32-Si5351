Questo progetto descrive la realizzazione di un VFO / BFO digitale
basato su ESP32 DevKit 32D e generatore di frequenza Si5351A.


![20260215_215529](https://github.com/user-attachments/assets/e533f9a2-6b42-49ce-a9d0-9d0ee1ba99a8)


Il sistema è stato progettato per applicazioni radioamatoriali,
in particolare per ricevitori HF autocostruiti, ma può essere
utilizzato anche in apparati sperimentali e RTX fino a circa 160 MHz.


Caratteristiche:
- Generazione VFO e BFO indipendenti
- Gestione modi: AM, LSB, USB, CW
- Controllo AGC e attenuatore
- Pitch regolabile del BFO
- S-meter digitale con memoria di picco
- Encoder meccanici e ottici
- Display TFT a colori
- Espansione I/O tramite PCF8574
- Memorizzazione configurazioni su EEPROM 24LC32
- Firmware modulare in C++
- Funzione di calibrazione del Si5351

Architettura codice:
Il codice è strutturato in modo modulare:

- VFO_BFO → gestione frequenze
- PLL → configurazione Si5351
- bands / modes → logica operativa
- DigiOUT → uscite digitali PCF8574
- s_meter → lettura analogica
- display → interfaccia grafica
- EEPROM_manager → salvataggio configurazioni

Il firmware è sviluppato con PlatformIO su VS Code.

Questa scelta permette:
- gestione strutturata del progetto
- controllo delle librerie
- maggiore scalabilità rispetto all’Arduino IDE

⚠️ Il firmware è in continua evoluzione.

I comandi sono:
- ENCODER 1: come selettore di frequenza.
- ENCODER 2: come variazione Pitch del BFO
- STEP: per cambio frequenza (10Hz, 100Hz, 1KHz, 10KHZ) con indicazione su display.
- BAND: per la selezione della banda con indicazione su display delle bande radioamatoriali e non.
- MODE: seleziona il tipo di demodulazione, con indicazione su pin binaria.
- AGC: slow e fast
- ATT: selettore attenuatore

 
 
[![Guarda il video](https://img.youtube.com/vi/RLuy1z0uWSg/maxresdefault.jpg)](https://www.youtube.com/watch?v=RLuy1z0uWSg)

