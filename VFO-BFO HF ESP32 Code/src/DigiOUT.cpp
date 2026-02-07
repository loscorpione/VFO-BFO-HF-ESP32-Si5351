#include "DigiOUT.h"
#include "config.h"
#include "modes.h" 
#include "functions.h"
#include <Arduino.h> 

// Dichiarazione delle variabili globali necessarie
extern unsigned long displayedFrequency;
extern int currentMode;

// Variabile per memorizzare l'ultimo stato inviato
uint8_t lastOutputState = 0xFF; // Inizializza con un valore impossibile

void setupDigiOUT() {
  
  // Reset del PCF8574 - imposta tutte le uscite a 0
  Wire.beginTransmission(PCF8574A_ADDRESS);
  Wire.write(0b00000000);
  Wire.endTransmission();
  delay(50);
  
  // Forza l'aggiornamento al primo avvio
  lastOutputState = 0xFF;
  
  // Aggiorna con lo stato corretto
  updateModeOutputs();
}

void updateModeOutputs() {
  uint8_t outputState = 0;
  
  // Uscita binaria selezione filtri passa banda (bit 0-2)
  if (displayedFrequency >= 1600000 && displayedFrequency < 2500000) {
    outputState = 0b001;  // 160m
  }
  else if (displayedFrequency >= 2500000 && displayedFrequency < 4700000) {
    outputState = 0b010;  // 80m
  }
  else if (displayedFrequency >= 4700000 && displayedFrequency < 7500000) {
    outputState = 0b011;  // 60m
  }
  else if (displayedFrequency >= 7500000 && displayedFrequency < 14500000) {
    outputState = 0b100;  // 40m/30m
  }
  else if (displayedFrequency >= 14500000 && displayedFrequency < 21500000) {
    outputState = 0b101;  // 20m/17m
  }
  else if (displayedFrequency >= 21500000 && displayedFrequency <= 33000000) {
    outputState = 0b110;  // 15m/12m/10m
  }
  else {
    outputState = 0b000;  // Fuori banda
  }

 // Uscita binaria selezione modalità (bit 3-4)
  // Mappatura: AM=00, LSB=01, USB=10, CW=11
  outputState |= (currentMode << 3);
  
  // Uscita binaria selettore AGC (bit 5)
  if (agcFastMode) {
    outputState |= (1 << 5);  // AGC Fast = 1
  }
  // NOTA: se AGC Slow, bit rimane a 0

  // Uscita binaria selettore ATT (bit 6)
  if (attenuatorEnabled) {
    outputState |= (1 << 6);  // ATT -20dB abilitato = 1
  }
  // NOTA: se ATT disabilitato, bit rimane a 0

   // Uscita binaria BFO Enable (bit 7)
  // BFO attivo solo per LSB, USB, CW (non per AM)
  if (currentMode != MODE_AM && bfoEnabled) {
    outputState |= (1 << 7);  // BFO Enable = 1
  }
  // NOTA: per AM, bit rimane a 0
  
  if (outputState != lastOutputState) {
    Wire.beginTransmission(PCF8574A_ADDRESS);
    Wire.write(outputState);
    byte error = Wire.endTransmission();
    
    lastOutputState = outputState;
    
    // Debug output
    // Serial.print("DigiOUT: Banda:");
    // Serial.print(outputState & 0x0F, BIN);
    // Serial.print(" Mode:");
    // Serial.print((outputState >> 4) & 0x03, BIN);
    // Serial.print(" AGC:");
    // Serial.print((outputState >> 6) & 0x01 ? "Fast" : "Slow");
    // Serial.print(" ATT:");
    // Serial.print((outputState >> 7) & 0x01 ? "ON" : "OFF");
    // Serial.print(" Output:");
    // Serial.print(outputState, BIN);
    // Serial.println(error == 0 ? " [OK]" : " [ERROR]");
  }
}

