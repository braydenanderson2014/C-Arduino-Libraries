/**
 * @section LICENSE
 *  Copyright (c) 2012
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 * @section DESCRIPTION
 *  This is a library for allowing the use of programs written for the Arduino
 *  with the mbed. This was started for use with the Gameduino shield Library.
 *  It is currently uncomplete and not fully tested, but I don't see myself spending time on it. 
 *  See TODOs and check http://arduino.cc/en/Reference/HomePage for missing parts
 *
 * @file arduino.c
 *
 * Example:
 * @code
 * 
 * #include "mbed.h"
 * #include "arduino.h"
 * 
 * void setup(){
 * // setup code
 * }
 *
 *void loop(){
 * // loop code
 *}
 *
 * int main() {
 *   timer_start();
 *   setup();
 *   while(1) {
 *       loop();
 *   }
 * }
 * @endcode
 */
 #include "arduino.h"
 
/*****************************************************  
  Digital I/O
 
    pinMode()- TODO
    digitalWrite()- TODO
    digitalRead()- TODO
    
******************************************************/
 
/*****************************************************  
  Analog I/O
 
    analogReference() - TODO
    analogRead() - TODO
    analogWrite()-(PWM) - TODO
    
*****************************************************/
 
/*****************************************************
  Advanced I/O
 
    tone() - TODO
    noTone() - TODO
    shiftOut() - TODO
    shiftIn() - TODO
    pulseIn() - TODO
    
*****************************************************/ 
 
/*****************************************************  
  Time
    
    millis() - need to start timer first
    micros() - need to start timer first
    delay() - done by Macro
    delayMicroseconds() - Done byMacro
    
*****************************************************/
 
Timer arduino_timer;
/** start the arduino_timer timer for millis() and micros() running.
 *
 * @param void
 */
void timer_start(void) {
    arduino_timer.start();
}
/** return a long for the amount of time since the timer was started in milliseconds.
 *
 * @param void
 */
long millis(void) {
    return arduino_timer.read_ms();
}
/** return a long for the amount of time since the timer was started in microseconds.
 *
 * @param void
 */
long micros(void) {
    return arduino_timer.read_us();
}
 
 
/*****************************************************
  Maths
 
    min()  - done by Macro
    max()  - done by Macro
    abs()  - done by Macro
    constrain() - TODO
    map() - TODO
    pow() - implemented by including math.h
    sqrt() - implemented by including math.h
    
*****************************************************/
 
 
/*****************************************************
  Trigonometry
 
    sin() - implemented by including math.h
    cos() - implemented by including math.h
    tan() - implemented by including math.h
    
*****************************************************/
 
/*****************************************************
  Random Numbers
 
    randomSeed()  - done by Macro
    random() - function below
    
*****************************************************/
 
/** generates a random number from 0 to defined number
 *
 * @param number maximum value for random number
 */
int random(int number) {
    return (rand()%number);
}
/** generates a random number between two numbers
 *
 * @param numberone minimum value for random number
 * @param numbertwo maximum value for random number
 */
int random(int numberone, int numbertwo) {
    int random = 0;
    if ((numberone < 0) && (numbertwo < 0)) {
        numberone = numberone * -1;
        numbertwo = numbertwo * -1;
        random = -1 * (rand()%(numberone + numbertwo));
    }
    if ((numbertwo < 0) && (numberone >= 0)) {
        numbertwo = numbertwo * -1;
        random = (rand()%(numberone + numbertwo)) - numbertwo;
    }
    if ((numberone < 0) && (numbertwo >= 0)) {
        numberone = numberone * -1;
        random = (rand()%(numberone + numbertwo)) - numberone;
    } else {
        random = (rand()%(numberone + numbertwo)) - min(numberone, numbertwo);
    }
    return random;
}
 
/*****************************************************
  Bits and Bytes
     
     lowByte() - function below
     highByte() - function below
     bitRead() - TODO
     bitWrite() - TODO
     bitSet() - TODO
     bitClear() - TODO
     bit() - TODO
     
*****************************************************/
 
/** returns the lower nibble of first byte
 *
 * @param high byte of which the high nibble is required
 */
byte lowByte(short int low) {
    byte bytelow = 0;
    bytelow = (low & 0xFF);
    return bytelow;
}
/** returns the higher nibble of first byte
 *
 * @param high byte of which the high nibble is required
 */
byte highByte(short int high) {
    byte bytehigh = 0;
    bytehigh = ((high >> 8) & 0xFF);
    return bytehigh;
}
 
 
 
/*****************************************************
  External Interrupts
    
    attachInterrupt() - TODO
    detachInterrupt() - TODO
    
*****************************************************/
    
/*****************************************************
  Interrupts
 
    interrupts() - TODO
    noInterrupts() - TODO
    
*****************************************************/
 
/*****************************************************
  Communication
 
    Serial - TODO
    Stream - TODO
    
*****************************************************/