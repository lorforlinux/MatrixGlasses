// Copyright (c) 2025 Deepak Khatri - deepak@upsidedownlabs.tech
// Copyright (c) 2025 Krishnanshu Mittal - krishnanshu@upsidedownlabs.tech

/*
  To display a custom text or pattern on the LED Matrix:
  1. Go to https://dotmatrixtool.com
  2. Select the dimensions of the LED Matrix(Width:24 x Height:8)
  3. Draw the pattern or text you want to display by clicking and dragging on the Matrix. You can clear any selected box by CTRL + LEFT Click
  4. Click on Generate button to generate the data in hex format.
  5. Copy the data (Only the hex values) and paste it in our data[] array in code below.
*/

// Pin Definitions
const uint8_t DATA_COL = 9;
const uint8_t CLK_COL  = 15;
const uint8_t DATA_ROW = 18;
const uint8_t CLK_ROW  = 20;
const uint8_t LATCH    = 14;

// Matrix Dimensions
const uint8_t NUM_COLS = 24;
const uint8_t NUM_ROWS = 8;

// Generated Data from DotMatrixTool

const uint8_t data[] =
{
// Paste below your custom data generated from https://dotmatrixtool.com
0x00, 0x1e, 0x21, 0x21, 0x21, 0x00, 0x00, 0x0e, 0x11, 0x09, 0x05, 0x03, 0x03, 0x05, 0x09, 0x11, 0x0e, 0x00, 0x00, 0x1f, 0x20, 0x20, 0x20, 0x00
};


void setup() {
  pinMode(DATA_COL, OUTPUT);
  pinMode(CLK_COL, OUTPUT);
  pinMode(DATA_ROW, OUTPUT);
  pinMode(CLK_ROW, OUTPUT);
  pinMode(LATCH, OUTPUT);
}

void loop() {
  for (uint8_t row = 0; row < NUM_ROWS; row++) {
    uint8_t rowData = ~(1 << row); // Active low for current row
    uint8_t colData[3] = {0, 0, 0}; // For 24 columns

    for (uint8_t col = 0; col < NUM_COLS; col++) {
      if (data[col] & (1 << row)) {
        colData[col / 8] |= (1 << (col % 8));
      }
    }

    digitalWrite(LATCH, LOW);
    // Shift out column data (MSB first)
    for (int8_t i = 2; i >= 0; i--) {
      shiftOut(DATA_COL, CLK_COL, MSBFIRST, colData[i]);
    }
    // Shift out row data
    shiftOut(DATA_ROW, CLK_ROW, MSBFIRST, rowData);
    digitalWrite(LATCH, HIGH);

    delayMicroseconds(100); // Adjust for brightness and flicker
  }
}