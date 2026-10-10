#include "DigiOUT.h"
#include "config.h"
#include "modes.h" 
#include "functions.h"
#include <Arduino.h> 

// ============================================
// VARIABILI GLOBALI
// ============================================

extern unsigned long displayedFrequency;
extern int currentMode;

// Variabile per memorizzare l'ultimo stato inviato
uint8_t lastOutputState = 0xFF;  // Inizializza con un valore impossibile

// ============================================
// INIZIALIZZAZIONE PCF8574
// ============================================

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

// ============================================
// AGGIORNAMENTO USCITE
// ============================================

void updateModeOutputs() {
    uint8_t outputState = 0;
    
    // ====================================================
    // Mappatura: Selezione filtri passa banda Preselettore
    // ====================================================
    if (displayedFrequency >= 200000 && displayedFrequency < 1000000) {
        outputState = 0b000;  // da 200kHz a 1000kHz - Band=000 
    }
    else if (displayedFrequency >= 1000000 && displayedFrequency < 2000000) {
        outputState = 0b001;  // 1000kHz a 2000kHz - Band=001
    }
    else if (displayedFrequency >= 2000000 && displayedFrequency < 4000000) {
        outputState = 0b010;  // 2000kHz a 4000kHz - Band=010
    }
    else if (displayedFrequency >= 4000000 && displayedFrequency < 8000000) {
        outputState = 0b011;  // 4000kHz a 8000kHz - Band=011
    }
    else if (displayedFrequency >= 8000000 && displayedFrequency < 15000000) {
        outputState = 0b100;  // 8000kHz a 15MHz - Band=100
    }
    else if (displayedFrequency >= 15000000 && displayedFrequency <= 30000000) {
        outputState = 0b101;  // 15MHz a 30MHz - Band=101
    }
    else {
        outputState = 0b110;  // Fuori banda - band=110 (nessun filtro attivo)
    }

    // ============================================
    // BIT 3-4: Selezione modalità
    // Mappatura: AM=00, LSB=01, USB=10, CW=11
    // ============================================
    outputState |= (currentMode << 3);
    
    // ============================================
    // BIT 5: Selettore AGC (1 = Fast, 0 = Slow)
    // ============================================
    if (agcFastMode) {
        outputState |= (1 << 5);
    }

    // ============================================
    // BIT 6: Selettore ATT (1 = -20dB abilitato)
    // ============================================
    if (attenuatorEnabled) {
        outputState |= (1 << 6);
    }

    // ============================================
    // BIT 7: BFO Enable (attivo per LSB, USB, CW)
    // ============================================
    if (currentMode != MODE_AM && bfoEnabled) {
        outputState |= (1 << 7);
    }
    
    // ============================================
    // INVIO DATI (solo se cambiato)
    // ============================================
    if (outputState != lastOutputState) {
        Wire.beginTransmission(PCF8574A_ADDRESS);
        Wire.write(outputState);
        byte error = Wire.endTransmission();
        
        lastOutputState = outputState;
        
        // Debug output (commentato)
        // Serial.print("DigiOUT: Banda:");
        // Serial.print(outputState & 0x07, BIN);
        // Serial.print(" Mode:");
        // Serial.print((outputState >> 3) & 0x03, BIN);
        // Serial.print(" AGC:");
        // Serial.print((outputState >> 5) & 0x01 ? "Fast" : "Slow");
        // Serial.print(" ATT:");
        // Serial.print((outputState >> 6) & 0x01 ? "ON" : "OFF");
        // Serial.print(" BFO:");
        // Serial.print((outputState >> 7) & 0x01 ? "ON" : "OFF");
        // Serial.print(" Output:");
        // Serial.print(outputState, BIN);
        // Serial.println(error == 0 ? " [OK]" : " [ERROR]");
    }
}