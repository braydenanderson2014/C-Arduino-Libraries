#include <IOStream.h> // Include the IOStream class

// Define the SD chip select pin
const int chipSelect = 10;

// Initialize the LCD
LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {
    // Initialize Serial communication
    Serial.begin(9600);

    // Initialize the LCD
    lcd.init();
    lcd.backlight();

    // Initialize SD card
    if (!SD.begin(chipSelect)) {
        Serial.println("SD card initialization failed!");
        return;
    }

    // Open a file for writing
    if (cout.openFile("example.txt", FILE_WRITE)) {
        cout.println("Hello, SD Card!");
        cout.println("This is a test.");
        cout.closeFile();
    } else {
        Serial.println("Failed to open file for writing!");
    }

    // Read and display the content of the file
    char buffer[128];
    if (cout.loadFromFile("example.txt", buffer, sizeof(buffer))) {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("File Content:");
        lcd.setCursor(0, 1);
        lcd.print(buffer);
    } else {
        Serial.println("Failed to read file!");
    }
}

void loop() {
    // Your loop code here
}