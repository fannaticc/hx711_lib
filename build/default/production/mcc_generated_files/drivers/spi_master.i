
# 1 "mcc_generated_files/drivers/spi_master.c"

# 13 "C:\Program Files (x86)\Microchip\xc8\v2.05\pic\include\c90\stdint.h"
typedef signed char int8_t;

# 20
typedef signed int int16_t;

# 28
typedef __int24 int24_t;

# 36
typedef signed long int int32_t;

# 52
typedef unsigned char uint8_t;

# 58
typedef unsigned int uint16_t;

# 65
typedef __uint24 uint24_t;

# 72
typedef unsigned long int uint32_t;

# 88
typedef signed char int_least8_t;

# 96
typedef signed int int_least16_t;

# 109
typedef __int24 int_least24_t;

# 118
typedef signed long int int_least32_t;

# 136
typedef unsigned char uint_least8_t;

# 143
typedef unsigned int uint_least16_t;

# 154
typedef __uint24 uint_least24_t;

# 162
typedef unsigned long int uint_least32_t;

# 181
typedef signed char int_fast8_t;

# 188
typedef signed int int_fast16_t;

# 200
typedef __int24 int_fast24_t;

# 208
typedef signed long int int_fast32_t;

# 224
typedef unsigned char uint_fast8_t;

# 230
typedef unsigned int uint_fast16_t;

# 240
typedef __uint24 uint_fast24_t;

# 247
typedef unsigned long int uint_fast32_t;

# 268
typedef int32_t intmax_t;

# 282
typedef uint32_t uintmax_t;

# 289
typedef int16_t intptr_t;




typedef uint16_t uintptr_t;

# 15 "C:\Program Files (x86)\Microchip\xc8\v2.05\pic\include\c90\stdbool.h"
typedef unsigned char bool;

# 4 "C:\Program Files (x86)\Microchip\xc8\v2.05\pic\include\__size_t.h"
typedef unsigned size_t;

# 7 "C:\Program Files (x86)\Microchip\xc8\v2.05\pic\include\c90\stdarg.h"
typedef void * va_list[1];

#pragma intrinsic(__va_start)
extern void * __va_start(void);

#pragma intrinsic(__va_arg)
extern void * __va_arg(void *, ...);

# 43 "C:\Program Files (x86)\Microchip\xc8\v2.05\pic\include\c90\stdio.h"
struct __prbuf
{
char * ptr;
void (* func)(char);
};

# 29 "C:\Program Files (x86)\Microchip\xc8\v2.05\pic\include\c90\errno.h"
extern int errno;

# 12 "C:\Program Files (x86)\Microchip\xc8\v2.05\pic\include\c90\conio.h"
extern void init_uart(void);

extern char getch(void);
extern char getche(void);
extern void putch(char);
extern void ungetch(char);

extern __bit kbhit(void);

# 23
extern char * cgets(char *);
extern void cputs(const char *);

# 88 "C:\Program Files (x86)\Microchip\xc8\v2.05\pic\include\c90\stdio.h"
extern int cprintf(char *, ...);
#pragma printf_check(cprintf)



extern int _doprnt(struct __prbuf *, const register char *, register va_list);


# 180
#pragma printf_check(vprintf) const
#pragma printf_check(vsprintf) const

extern char * gets(char *);
extern int puts(const char *);
extern int scanf(const char *, ...) __attribute__((unsupported("scanf() is not supported by this compiler")));
extern int sscanf(const char *, const char *, ...) __attribute__((unsupported("sscanf() is not supported by this compiler")));
extern int vprintf(const char *, va_list) __attribute__((unsupported("vprintf() is not supported by this compiler")));
extern int vsprintf(char *, const char *, va_list) __attribute__((unsupported("vsprintf() is not supported by this compiler")));
extern int vscanf(const char *, va_list ap) __attribute__((unsupported("vscanf() is not supported by this compiler")));
extern int vsscanf(const char *, const char *, va_list) __attribute__((unsupported("vsscanf() is not supported by this compiler")));

#pragma printf_check(printf) const
#pragma printf_check(sprintf) const
extern int sprintf(char *, const char *, ...);
extern int printf(const char *, ...);

# 15 "C:\Program Files (x86)\Microchip\xc8\v2.05\pic\include\c90\stdbool.h"
typedef unsigned char bool;

# 27 "mcc_generated_files/drivers/../spi_types.h"
typedef enum {
MASTER0_CONFIG,
SPI_DEFAULT
} spi_modes;

# 35 "mcc_generated_files/drivers/../spi_driver.h"
inline void spi_close(void);

bool spi_open(spi_modes spiUniqueConfiguration);

uint8_t spi_exchangeByte(uint8_t b);

void spi_exchangeBlock(void *block, size_t blockSize);
void spi_writeBlock(void *block, size_t blockSize);
void spi_readBlock(void *block, size_t blockSize);

void spi_writeByte(uint8_t byte);
uint8_t spi_readByte(void);

void spi_isr(void);
void spi_setSpiISR(void(*handler)(void));

# 33 "mcc_generated_files/drivers/spi_master.h"
typedef enum {
MASTER0
} spi_master_configurations_t;

typedef struct { void (*spiClose)(void);
bool (*spiOpen)(void);
uint8_t (*exchangeByte)(uint8_t b);
void (*exchangeBlock)(void * block, size_t blockSize);
void (*writeBlock)(void * block, size_t blockSize);
void (*readBlock)(void * block, size_t blockSize);
void (*writeByte)(uint8_t byte);
uint8_t (*readByte)(void);
void (*setSpiISR)(void(*handler)(void));
void (*spiISR)(void);
} spi_master_functions_t;

extern const spi_master_functions_t spiMaster[];

inline bool spi_master_open(spi_master_configurations_t config);

# 29 "mcc_generated_files/drivers/spi_master.c"
inline bool MASTER0_open(void);

const spi_master_functions_t spiMaster[] = {
{ spi_close, MASTER0_open, spi_exchangeByte, spi_exchangeBlock, spi_writeBlock, spi_readBlock, spi_writeByte, spi_readByte, spi_setSpiISR, spi_isr }
};

inline bool MASTER0_open(void){
return spi_open(MASTER0_CONFIG);
}


inline bool spi_master_open(spi_master_configurations_t config){
switch(config){
case MASTER0:
return MASTER0_open();
default:
return 0;
}
}

