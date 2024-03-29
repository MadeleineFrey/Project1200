/* mipslab.h
   Header file for all labs.
   This file written 2015 by F Lundevall
   Some parts are original code written by Axel Isaksson

   Latest update 2015-08-28 by F Lundevall

   For copyright and licensing, see file COPYING */
#define BTN_UP 0x1     // Binary 001
#define BTN_DOWN 0x2    // Binary 010
#define BTN_SELECT 0x4  // Binary 100 
#define BTN_RETURN 0x8 //Binary 1000

extern int ADXL_VALUE;
extern int BTN_VALUE;
extern int random;

#define TMR1_FLAG 0x10
#define TMR2_FLAG 0x100
#define TMR3_FLAG 0x1000
#define TMR4_FLAG 0x10000
#define TMR5_FLAG 0x100000

/* Declare display-related functions from mipslabfunc.c */
void start_init (void);
void display_image(int x, const uint8_t *data);
void display_init(void);
void display_string(int line, char *s);
void display_update(void);
uint8_t spi_send_recv(uint8_t data);

/* Declare lab-related functions from mipslabfunc.c */
char * itoaconv( int num );
void labwork(void);
void quicksleep(int cyc);
void labinit( void );

void start_menu (void);
void displayMenu(pointer);
 void play (void);

//Timers
void timer2_conf (float setTime);
void timer2Start (void);
void timer2Stop (void);

void timer3_conf (float setTime);
void timer3Start (void);
void timer3Stop (void);

void timer4_conf (float setTime);
void timer4Start (void);
void timer4Stop (void);

void timer5_conf (float setTime);
void timer5Start (void);
void timer5Stop (void);
void timer5_interrupt (void);

void wait_0_1 (void);
void wait_0_2 (void);
void wait_0_5 (void);
void wait_1 (void);
void wait_2 (void);
void wait_3 (void);
void wait_4 (void);
void wait_5 (void);

//Game
void play (void);
void run();

/* Declare bitmap array containing font */
extern const uint8_t const font[128*8];
/* Declare bitmap array containing icon */
extern const uint8_t const icon[128];
extern const uint8_t const icon2[128];
/* Declare text buffer for display output */
extern char textbuffer[4][16];

/* Declare functions written by students.
   Note: Since we declare these functions here,
   students must define their functions with the exact types
   specified in the laboratory instructions. */
/* Written as part of asm lab: delay, time2string */
/* Written as part of i/o lab: getbtns, getsw, enable_interrupt */
int getbtns(void);
int getsw(void);
void enable_interrupt(void);


//I2C accelerometer
void i2c_init(void);
void adxl_init (void);
void yaxis_data (int * data);
void xaxis_data (int * data);
void zaxis_data (int * data);
int adxl_rand (int timer);

void new_highscore (int score);
void struct_init (void);
void icon_move (void);