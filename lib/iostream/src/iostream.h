#ifndef IOSTREAM_h
#define IOSTREAM_h

#ifndef _STDARG_H
#ifndef _ANSI_STDARG_H_
#ifndef __need___va_list
#define _STDARG_H
#define _ANSI_STDARG_H_
#endif /* not __need___va_list */
#undef __need___va_list

/* Define __gnuc_va_list.  */

#ifndef __GNUC_VA_LIST
#define __GNUC_VA_LIST
typedef __builtin_va_list __gnuc_va_list;
#endif

/* Define the standard macros for the user,
   if this invocation was from the user program.  */
#ifdef _STDARG_H

#define va_start(v,l)	__builtin_va_start(v,l)
#define va_end(v)	__builtin_va_end(v)
#define va_arg(v,l)	__builtin_va_arg(v,l)
#if !defined(__STRICT_ANSI__) || __STDC_VERSION__ + 0 >= 199900L \
    || __cplusplus + 0 >= 201103L
#define va_copy(d,s)	__builtin_va_copy(d,s)
#endif
#define __va_copy(d,s)	__builtin_va_copy(d,s)

/* Define va_list, if desired, from __gnuc_va_list. */
/* We deliberately do not define va_list when called from
   stdio.h, because ANSI C says that stdio.h is not supposed to define
   va_list.  stdio.h needs to have access to that data type, 
   but must not use that name.  It should use the name __gnuc_va_list,
   which is safe because it is reserved for the implementation.  */

#ifdef _BSD_VA_LIST
#undef _BSD_VA_LIST
#endif

#if defined(__svr4__) || (defined(_SCO_DS) && !defined(__VA_LIST))
/* SVR4.2 uses _VA_LIST for an internal alias for va_list,
   so we must avoid testing it and setting it here.
   SVR4 uses _VA_LIST as a flag in stdarg.h, but we should
   have no conflict with that.  */
#ifndef _VA_LIST_
#define _VA_LIST_
#ifdef __i860__
#ifndef _VA_LIST
#define _VA_LIST va_list
#endif
#endif /* __i860__ */
typedef __gnuc_va_list va_list;
#ifdef _SCO_DS
#define __VA_LIST
#endif
#endif /* _VA_LIST_ */
#else /* not __svr4__ || _SCO_DS */

/* The macro _VA_LIST_ is the same thing used by this file in Ultrix.
   But on BSD NET2 we must not test or define or undef it.
   (Note that the comments in NET 2's ansi.h
   are incorrect for _VA_LIST_--see stdio.h!)  */
#if !defined (_VA_LIST_) || defined (__BSD_NET2__) || defined (____386BSD____) || defined (__bsdi__) || defined (__sequent__) || defined (__FreeBSD__) || defined(WINNT)
/* The macro _VA_LIST_DEFINED is used in Windows NT 3.5  */
#ifndef _VA_LIST_DEFINED
/* The macro _VA_LIST is used in SCO Unix 3.2.  */
#ifndef _VA_LIST
/* The macro _VA_LIST_T_H is used in the Bull dpx2  */
#ifndef _VA_LIST_T_H
/* The macro __va_list__ is used by BeOS.  */
#ifndef __va_list__
typedef __gnuc_va_list va_list;
#endif /* not __va_list__ */
#endif /* not _VA_LIST_T_H */
#endif /* not _VA_LIST */
#endif /* not _VA_LIST_DEFINED */
#if !(defined (__BSD_NET2__) || defined (____386BSD____) || defined (__bsdi__) || defined (__sequent__) || defined (__FreeBSD__))
#define _VA_LIST_
#endif
#ifndef _VA_LIST
#define _VA_LIST
#endif
#ifndef _VA_LIST_DEFINED
#define _VA_LIST_DEFINED
#endif
#ifndef _VA_LIST_T_H
#define _VA_LIST_T_H
#endif
#ifndef __va_list__
#define __va_list__
#endif

#endif /* not _VA_LIST_, except on certain systems */

#endif /* not __svr4__ */

#endif /* _STDARG_H */

#endif /* not _ANSI_STDARG_H_ */
#endif /* not _STDARG_H */

//^
//|
// This is a workaround for the stdarg.h file not being included. 
typedef char* va_list;

#define va_start(ap, last) (ap = (va_list)&last + sizeof(last))
#define va_arg(ap, type) (*(type*)((ap += sizeof(type)) - sizeof(type)))
#define va_end(ap) (ap = (va_list)0)

#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#include <LiquidCrystal.h>
#include <Wire.h>
#include <SDFat.h>
#include <SPI.h>
//#include <hyperdisplay.h>

#include <ColorManager.h>
#include <Properties.h>


enum displayType{
    LCD,
    I2CLCD,
};
enum LCDType{
    LCD16x2,
    LCD20x4
};
enum Color565{
    BLACK_MONO = 0x0000,
    BLUE = 0x001F,
    RED = 0xF800,
    GREEN = 0x07E0,
    CYAN = 0x07FF,
    MAGENTA = 0xF81F,
    YELLOW = 0xFFE0,
    WHITE_MONO = 0xFFFF
};
class iostream {
private:
    /*
    TODO: Text Size
    TODO: Color
    TODO: Screen Width
    TODO: Screen Height

    */
    bool scrollDisplay = true; // scroll display.
    bool scrollLeftToRight = false; // if false, scroll right to left.
    bool clearOnUpdate = true; // clear display on update.
    bool blinkCursor = false; // blink cursor.
    bool cursor = false; // Show Cursor
    displayType display = LCD; // Display Type (default LCD)
    LCDType lcdType = LCD16x2; // LCD Type (default 16x2)
    //OLEDSIZE oledSize = OLED128x64; // OLED Type (default 128x64
    //OLEDTYPE oledType = OLED1306; // OLED Type (default 1306)
    byte screenWidth = 128; // Oled Screen Width
    byte screenHeight = 64; // Oled Screen Height
    byte textSize = 1; // Text Size
    int baudRate = 9600; // Baud Rate

    //Objects
    ColorManager colorMapper = ColorManager(); // Color Mapper
    Properties prop = Properties(); // Properties
    LiquidCrystal lcd = LiquidCrystal(12, 11, 5, 4, 3, 2); // LCD
    LiquidCrystal_I2C i2cLCD = LiquidCrystal_I2C(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);  // Set the LCD I2C address
    //Adafruit_SSD1306 oled1306 = Adafruit_SSD1306(screenWidth, screenHeight, &Wire, -1); // OLED
    //ssd1309_I2C myOLED;

    

    #define SERIAL_PORT Serial
    #define WIRE_PORT Wire
    #define RES_PIN 3
    #define MY_BUFFER_LENGTH BUFFER_LENGTH
public:
    void gatherAndSetSettings(){
        prop.loadFromSD("iostream.properties");
        if(prop.exists("displayType")){ 
            String displayTypes = prop.getProperty("displayType");
            if(displayTypes == "LCD"){
                display = LCD;
            } else if(displayTypes == "I2CLCD"){
                display = I2CLCD;
            } 
        } else {
            prop.setProperty("displayType", (String)display);
        }

        if(prop.exists("lcdType")){
            String lcdTypes = prop.getProperty("lcdType");
            if(lcdTypes == "LCD16x2"){
                lcdType = LCD16x2;
            } else if(lcdTypes == "LCD20x4"){
                lcdType = LCD20x4;
            }
        } else {
            prop.setProperty("lcdType", (String)lcdType);
        }
        if(prop.exists("scrollDisplay")){
            scrollDisplay = prop.getProperty("scrollDisplay") == "true" ? true : false;
        } else {
            prop.setProperty("scrollDisplay", (String)scrollDisplay);
        }
        if(prop.exists("scrollLeftToRight")){
            scrollLeftToRight = prop.getProperty("scrollLeftToRight") == "true" ? true : false;
        } else {
            prop.setProperty("scrollLeftToRight", (String)scrollLeftToRight);
        }
        if(prop.exists("clearOnUpdate")){
            clearOnUpdate = prop.getProperty("clearOnUpdate") == "true" ? true : false;
        } else {
            prop.setProperty("clearOnUpdate", (String)clearOnUpdate);
        }
        if(prop.exists("blinkCursor")){
            blinkCursor = prop.getProperty("blinkCursor") == "true" ? true : false;
        } else {
            prop.setProperty("blinkCursor", (String)blinkCursor);
        }
        if(prop.exists("cursor")){
            cursor = prop.getProperty("cursor") == "true" ? true : false;
        } else {
            prop.setProperty("cursor", (String)cursor);
        }
        if(prop.exists("baudRate")){
            baudRate = prop.getProperty("baudRate").toInt();
        } else {
            prop.setProperty("baudRate", (String)baudRate);
        }
        if(prop.exists("screenWidth")){
            screenWidth = prop.getProperty("screenWidth").toInt();
        } else {
            prop.setProperty("screenWidth", (String)screenWidth);
        }
        if(prop.exists("screenHeight")){
            screenHeight = prop.getProperty("screenHeight").toInt();
        } else {
            prop.setProperty("screenHeight", (String)screenHeight);
        }
        if(prop.exists("textSize")){
            textSize = prop.getProperty("textSize").toInt();
        } else {
            prop.setProperty("textSize", (String)textSize);
        }
        if(prop.exists("textColor")){
            String COLOR = prop.getProperty("color");
            colorMapper.addColor(colorMapper.hexToColor(COLOR.toInt()));
        } else {
            colorMapper.addColor(colorMapper.hexToColor(0xFFFFFF));//White
            prop.setProperty("textColor", (String)colorMapper.colorToHex(colorMapper.getColor(0)));
        }

        prop.saveToSD("iostream.properties");
    }

    void toggleSetting(String& setting){
        if(setting == "scrollDisplay"){
            scrollDisplay = !scrollDisplay;
        } else if(setting == "scrollLeftToRight"){
            scrollLeftToRight = !scrollLeftToRight;
        } else if(setting == "clearOnUpdate"){
            clearOnUpdate = !clearOnUpdate;
        } else if(setting == "blinkCursor"){
            blinkCursor = !blinkCursor;
        } else if(setting == "cursor"){
            cursor = !cursor;
        }
    }
    void begin(displayType display = LCD, LCDType lcdType = LCD16x2) {
        Serial.begin(baudRate);
        while(!Serial);
        if (display == LCD) {
            if(lcdType == LCD16x2){
                lcd.begin(16, 2);
                Serial.println("[IOSTREAM]: LCD 16x2 Display Initialized [OK]");
            } else if(lcdType == LCD20x4){
                lcd.begin(20, 4);
                Serial.println("[IOSTREAM]: LCD 20x4 Display Initialized [OK]");
            }
        } else if (display == I2CLCD) {
            if(lcdType == LCD16x2){
                i2cLCD.begin(16, 2);
                Serial.println("[IOSTREAM]: I2C LCD 16x2 Display Initialized [OK]");
            } else if(lcdType == LCD20x4){
                i2cLCD.begin(20, 4);
                Serial.println("[IOSTREAM]: I2C LCD 20x4 Display Initialized [OK]");
            }
        } 
    
        /*
          if(!oled.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3C for 128x64
            Serial.println(F("SSD1306 allocation failed"));
            for(;;); // Don't proceed, loop forever
        }
        */

    }

    void setColor(Color565 color){
        colorMapper.addColor(colorMapper.hexToColor(color));
        //oled.setTextColor(color);
    }

    void setColor(byte r, byte g, byte b){
        ColorManager colorMapper = ColorManager(int(r), int(g), int(b));
        this -> colorMapper = colorMapper;
        //oled.setTextColor(this -> colorMapper.colorToHex(colorMapper.getColor(0)));
    }

    void print(const char* str) {
        Serial.print(str);
    }
    
    void println(const char* str) {
        Serial.println(str);
    }

    //%d or %i: Signed integer.
    //%u: Unsigned integer.
    //%f: Floating-point number (double). For float arguments, float is promoted to double when passed through ....
    //%s: Null-terminated string.
    //%c: Single character.
    //%x or %X: Unsigned hexadecimal integer.
    //%p: Pointer address.
    //%%: A literal % character.  
    void printf(const char* format, ...){
        va_list args;
        va_start(args, format); // Initialize the argument list
        while (*format != '\0') {
            if (*format == '%') {
                format++; // Move past '%'
                switch (*format) {
                    case 's': {
                        // Handle string argument
                        char *str = va_arg(args, char*);
                        Serial.print(str);
                        break;
                    }
                    case 'd': {
                        // Handle integer argument
                        int i = va_arg(args, int);
                        Serial.print(i);
                        break;
                    }
                    case '%': {
                        // Handle escaped '%'
                        Serial.print('%');
                        break;
                    }
                    default:
                        // Unknown format specifier, print as-is
                        Serial.print('%');
                        Serial.print(*format);
                        break;
                }
            } else {
                // Not a format specifier, print the character
                Serial.print(*format);
            }
            format++;
        }
        va_end(args); // Clean up the argument list
    }

//TODO: Add scroll ability
    void printLCD(const char* str, int line = 0, bool clear = false) {
        if(clearOnUpdate){
            clearScreen(lcdType);
        }
        if (clear) {
            lcd.setCursor(0, line);
            lcd.print("                ");
        }
        lcd.setCursor(0, line);
        lcd.print(str);
        if (strlen(str) > 16) {
            for (int i = 0; i < strlen(str) - 16; i++) {
                if(prop.getProperty("scrollDisplay") == "true"){
                    if(prop.getProperty("scrollLeftToRight") == "true"){
                        lcd.scrollDisplayLeft();
                    } else {
                        lcd.scrollDisplayRight();
                    }
                }
                if(prop.getProperty("blinkCursor") == "true"){
                    lcd.blink();
                } else {
                    lcd.noBlink();
                }
                delay(200);
            }
        }
    }

    void clearScreen(LCDType lcdType){
        if(this -> lcdType == LCD16x2){
            for(int i = 0; i < 2; i++){
                for(int j = 0; j < 16; j++){
                    lcd.setCursor(j, i);
                    lcd.print(" ");
                }
            }
        } else if(this -> lcdType == LCD20x4){
            for(int i = 0; i < 3; i++){
                for(int j = 0; j < 20; j++){
                    lcd.setCursor(j, i);
                    lcd.print(" ");
                }
            }
        } else {
            //oled.clearDisplay(); 
        }
    }


    void printI2CLCD(const char* str, int line, bool clear = false) {
        if(clearOnUpdate){
            clearScreen(lcdType);
        }
        if (clear) {
            i2cLCD.setCursor(0, line);
            i2cLCD.print("                ");
        }
        i2cLCD.setCursor(0, line);
        i2cLCD.print(str);
        if (strlen(str) > 16) {
            for (int i = 0; i < strlen(str) - 16; i++) {
                if(prop.getProperty("scrollDisplay") == "true"){
                    if(prop.getProperty("scrollLeftToRight") == "true"){
                        lcd.scrollDisplayLeft();
                    } else {
                        lcd.scrollDisplayRight();
                    }
                }
                if(prop.getProperty("blinkCursor") == "true"){
                    lcd.blink();
                } else {
                    lcd.noBlink();
                }
                delay(200);
            }
        }
    }

    /*
    bool saveToFile(const char* filename, const char* data) {
        File file = SD.open(filename, FILE_WRITE);
        if (!file) {
            return false;
        }
        file.print(data);
        file.close();
        return true;
    }
    */
    
    // Load data from a file on the SD card
    /*
    bool loadFromFile(const char* filename, char* buffer, size_t bufferSize) {
        File file = SD.open(filename);
        if (!file) {
            return false;
        }
        size_t bytesRead = file.readBytes(buffer, bufferSize);
        buffer[bytesRead] = '\0';
        file.close();
        return true;
    }*/
    //... Add other I/O methods as needed 
};

extern iostream cout; // Typically, cout is used for console output.

#endif //IOSTREAM_h
