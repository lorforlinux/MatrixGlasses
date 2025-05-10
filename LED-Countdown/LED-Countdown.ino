// Copyright (c) 2025 Deepak Khatri - deepak@upsidedownlabs.tech
// Copyright (c) 2025 Krishnanshu Mittal - krishnanshu@upsidedownlabs.tech

#define BOOT_PIN 9

// Pin Assignments
const uint8_t DATA_COL = 9;   // Columns → 74HC595 DS
const uint8_t CLK_COL  = 15;  // Columns → 74HC595 SH_CP
const uint8_t DATA_ROW = 18;  // Rows    → 74HC595 DS
const uint8_t CLK_ROW  = 20;  // Rows    → 74HC595 SH_CP
const uint8_t LATCH    = 14;  // Shared  → 74HC595 ST_CP

// Matrix Dimensions
const uint8_t NUM_COLS = 24;
const uint8_t NUM_ROWS = 8;

// Each digit is defined as a 4x5 pattern (columns 4-7, rows 1-5)
const bool digitPatterns[10][7][5] = {
  // 0
  {
    {0, 1, 1, 1, 0},
    {1, 0, 0, 0, 1},
    {1, 0, 0, 1, 1},
    {1, 0, 1, 0, 1},
    {1, 1, 0, 0, 1},
    {1, 0, 0, 0, 1},
    {0, 1, 1, 1, 0}
  },
  // 1
  {
    {0, 0, 1, 0, 0},
    {0, 1, 1, 0, 0},
    {1, 0, 1, 0, 0},
    {0, 0, 1, 0, 0},
    {0, 0, 1, 0, 0},
    {0, 0, 1, 0, 0},
    {1, 1, 1, 1, 1}
  },
  // 2
  {
    {0, 1, 1, 1, 0},
    {1, 0, 0, 0, 1},
    {0, 0, 0, 0, 1},
    {0, 0, 1, 1, 0},
    {0, 1, 0, 0, 0},
    {1, 0, 0, 0, 0},
    {1, 1, 1, 1, 1}
  },
  // 3
  {
    {1, 1, 1, 1, 0},
    {0, 0, 0, 0, 1},
    {0, 0, 0, 0, 1},
    {0, 0, 1, 1, 0},
    {0, 0, 0, 0, 1},
    {0, 0, 0, 0, 1},
    {1, 1, 1, 1, 0}
  },
  // 4
  {
    {0, 0, 0, 1, 0},
    {0, 0, 1, 1, 0},
    {0, 1, 0, 1, 0},
    {1, 0, 0, 1, 0},
    {1, 1, 1, 1, 1},
    {0, 0, 0, 1, 0},
    {0, 0, 0, 1, 0}
  },
  // 5
  {
    {1, 1, 1, 1, 1},
    {1, 0, 0, 0, 0},
    {1, 1, 1, 1, 0},
    {0, 0, 0, 0, 1},
    {0, 0, 0, 0, 1},
    {1, 0, 0, 0, 1},
    {0, 1, 1, 1, 0}
  },
  // 6
  {
    {0, 1, 1, 1, 0},
    {1, 0, 0, 0, 1},
    {1, 0, 0, 0, 0},
    {1, 1, 1, 1, 0},
    {1, 0, 0, 0, 1},
    {1, 0, 0, 0, 1},
    {0, 1, 1, 1, 0}
  },
  // 7
  {
    {1, 1, 1, 1, 1},
    {0, 0, 0, 0, 1},
    {0, 0, 0, 1, 0},
    {0, 0, 1, 0, 0},
    {0, 0, 1, 0, 0},
    {0, 0, 1, 0, 0},
    {0, 0, 1, 0, 0}
  },
  // 8
  {
    {0, 1, 1, 1, 0},
    {1, 0, 0, 0, 1},
    {1, 0, 0, 0, 1},
    {0, 1, 1, 1, 0},
    {1, 0, 0, 0, 1},
    {1, 0, 0, 0, 1},
    {0, 1, 1, 1, 0}
  },
  // 9
  {
    {0, 1, 1, 1, 0},
    {1, 0, 0, 0, 1},
    {1, 0, 0, 0, 1},
    {0, 1, 1, 1, 1},
    {0, 0, 0, 0, 1},
    {1, 0, 0, 0, 1},
    {0, 1, 1, 1, 0}
  }
};


// LED State Tracking
bool ledStates[NUM_COLS][NUM_ROWS] = {false};

// Initialize all hardware pins
void initPins() {
  pinMode(DATA_COL, OUTPUT);
  pinMode(CLK_COL, OUTPUT);
  pinMode(DATA_ROW, OUTPUT);
  pinMode(CLK_ROW, OUTPUT);
  pinMode(LATCH, OUTPUT);
  digitalWrite(LATCH, LOW);
}

// Clear all LED states
void clearAllLEDs() {
  for (uint8_t col = 0; col < NUM_COLS; col++) {
    for (uint8_t row = 0; row < NUM_ROWS; row++) {
      ledStates[col][row] = false;
    }
  }
}

// Set an individual LED state (in memory)
void setLED(uint8_t col, uint8_t row, bool state) {
  if (col < NUM_COLS && row < NUM_ROWS) {
    ledStates[col][row] = state;
  }
}

// Refresh the entire display (one row at a time)
void refreshDisplay() {
  for (uint8_t row = 0; row < NUM_ROWS; row++) {
    uint8_t colData[3] = {0, 0, 0};
    
    // Build column data for this row
    for (uint8_t col = 0; col < NUM_COLS; col++) {
      if (ledStates[col][row]) {
        colData[col / 8] |= (1 << (col % 8));
      }
    }
    
    // Activate only this row (active low)
    uint8_t rowData = ~(1 << row);
    
    // Update shift registers
    digitalWrite(LATCH, LOW);
    for (int8_t i = 2; i >= 0; i--) {
      shiftOut(DATA_COL, CLK_COL, MSBFIRST, colData[i]);
    }
    shiftOut(DATA_ROW, CLK_ROW, MSBFIRST, rowData);
    digitalWrite(LATCH, HIGH);
    
    // Brief delay for LED persistence
    delayMicroseconds(300);
  }
}

// ─── DISPLAY DIGIT FUNCTION ────────────────────────────────────────────────
void displayDigit(uint8_t digit, uint8_t startCol, uint8_t startRow) {
  if (digit > 9) return; // Only 0-9 supported
  
  for (uint8_t row = 0; row < 7; row++) {
    for (uint8_t col = 0; col < 5; col++) {
      setLED(startCol + col, startRow + row, digitPatterns[digit][row][col]);
    }
  }
}

void setup() {
  initPins();
  clearAllLEDs();
}

void loop() {
  for(int i = 99; i >= 0; i--) {
    clearAllLEDs();            // Clear before displaying new digit
    displayDigit(int(i/10), 3, 0);     // Display the digit
    displayDigit(int(i%10), 16, 0);
    
    // Display for 500ms with continuous refresh
    unsigned long startTime = millis();
    while(millis() - startTime < 500) {
      refreshDisplay();        // Keep refreshing while displaying
    }
  }
}