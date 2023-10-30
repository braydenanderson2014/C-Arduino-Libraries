#ifndef _ARDUINO_H_
#define _ARDUINO_H_
 
#include "mbed.h"
#include "math.h"
// Macros
 
#define PI 3.1415926535897932384626433832795
#define HALF_PI 1.5707963267948966192313216916398
#define TWO_PI 6.283185307179586476925286766559
#define DEG_TO_RAD 0.017453292519943295769236907684886
#define RAD_TO_DEG 57.295779513082320876798154814105

#define radians(deg) ((deg)*DEG_TO_RAD)
#define degrees(rad) ((rad)*RAD_TO_DEG)
#define sq(x) ((x)*(x))

#define pgm_read_word(x)          (*(const short int*)x) 
#define pgm_read_dword_near(x)    (*(const int*)x)
#define pgm_read_word_near(x)     (*(const unsigned int*)x)
#define pgm_read_int_near(x)      (*(const int*)x)
#define pgm_read_int(x)           (*(const int*)x)
#define pgm_read_byte(x)          (*(const char*)x)
#define pgm_read_byte_near(x)     (*(const char*)x)
#define PROGMEM                   const
#define char(x)                   ((char)x)
#define byte(x)                   ((byte)x)
#define int(x)                    ((int)x)
#define word(x)                   ((word)x)
#define long(x)                   ((long)x)
#define float(x)                  ((float)x)

#define in_range(c, lo, up)  ((uint8_t)c >= lo && (uint8_t)c <= up)
#define isprint(c)           in_range(c, 0x20, 0x7f)
#define isdigit(c)           in_range(c, '0', '9')
#define isxdigit(c)          (isdigit(c) || in_range(c, 'a', 'f') || in_range(c, 'A', 'F'))
#define islower(c)           in_range(c, 'a', 'z')
#define isspace(c)           (c == ' ' || c == '\f' || c == '\n' || c == '\r' || c == '\t' || c == '\v')
 
/** Macro for delay() 
 *
 * @param void
 */
#define delay(x)                  (wait_ms(x))
/** Macro for delayMicroseconds()  
 *
 * @param void
 */
#define delayMicroseconds(x)      (wait_us(x))
 
/** Macro for min() 
 *
 * @param any
 */
#define min(a,b)                  ((a)<(b)?(a):(b))
/** Macro for max()
 *
 * @param any
 */
#define max(a,b)                  ((a)>(b)?(a):(b))
/** Macro for abs()
 *
 * @param any
 */
#define abs(x)                    ((x)>0?(x):(x*-1))
 
/** Macro for randomSeed()
 *
 * @param int
 */
#define randomSeed(x)             srand(x)
 
#define bitRead(value, bit) (((value) >> (bit)) & 0x01)
#define bitSet(value, bit) ((value) |= (1UL << (bit)))
#define bitClear(value, bit) ((value) &= ~(1UL << (bit)))
#define bitWrite(value, bit, bitvalue) (bitvalue ? bitSet(value, bit) : bitClear(value, bit))
 
// typedefs
 
typedef unsigned char prog_uchar;
typedef unsigned char prog_uint8_t;
typedef unsigned int prog_uint16_t;
typedef unsigned int prog_uint32_t;
typedef unsigned char byte;
typedef bool boolean;
typedef unsigned char prog_uchar;
typedef signed char prog_char;
typedef signed long int word;
 
// function prototypes
 
void timer_start(void);
long millis(void);
long micros(void);
byte lowByte(short int low);
byte highByte(short int high);

int random(int number);
int random(int numberone, int numbertwo);
#endif