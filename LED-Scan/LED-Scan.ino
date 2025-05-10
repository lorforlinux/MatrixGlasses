// Copyright (c) 2025 Deepak Khatri - deepak@upsidedownlabs.tech

// ─── PIN ASSIGNMENTS ───────────────────────────────────────────────────────────
const uint8_t DATA_COL = 9;   // Columns → 74HC595 DS
const uint8_t CLK_COL  = 15;  // Columns → 74HC595 SH_CP
const uint8_t DATA_ROW = 18;  // Rows    → 74HC595 DS
const uint8_t CLK_ROW  = 20;  // Rows    → 74HC595 SH_CP
const uint8_t LATCH    = 14;  // Shared  → 74HC595 ST_CP

// ─── MATRIX DIMENSIONS ─────────────────────────────────────────────────────────
const uint8_t NUM_COLS = 24;
const uint8_t NUM_ROWS = 8;

// ─── FUNCTION TO SET A SINGLE LED ──────────────────────────────────────────────
void setLED(uint8_t col, uint8_t row) {
  uint8_t colData[3] = {0, 0, 0}; // 3 bytes for 24 columns
  uint8_t rowData = 0xFF;         // All rows HIGH (inactive)

  // Set the specific column bit
  if (col < NUM_COLS) {
    colData[col / 8] = 1 << (col % 8);
  }

  // Set the specific row bit to LOW (active)
  if (row < NUM_ROWS) {
    rowData = ~(1 << row);
  }

  // Shift out the data
  digitalWrite(LATCH, LOW);
    // Shift out column data (MSB first)
    for (int8_t i = 2; i >= 0; i--) {
      shiftOut(DATA_COL, CLK_COL, MSBFIRST, colData[i]);
    }
    // Shift out row data
    shiftOut(DATA_ROW, CLK_ROW, MSBFIRST, rowData);
  digitalWrite(LATCH, HIGH);
}

void setup() {
  pinMode(DATA_COL, OUTPUT);
  pinMode(CLK_COL, OUTPUT);
  pinMode(DATA_ROW, OUTPUT);
  pinMode(CLK_ROW, OUTPUT);
  pinMode(LATCH, OUTPUT);
}

void loop() {
  // Iterate through each LED in the matrix
  for (uint8_t row = 0; row < NUM_ROWS; row++) {
    for (uint8_t col = 0; col < NUM_COLS; col++) {
      setLED(col, row);
      delay(100); // Delay to observe the LED
    }
  }
}