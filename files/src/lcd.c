

#include "lcd.h"    // <= su propio archivo de cabecera (opcional)
/*==================[definiciones y macros]==================================*/

/*==================[definiciones de datos internos]=========================*/
int i=0;

/*==================[definiciones de datos externos]=========================*/

/*==================[declaraciones de funciones internas]====================*/

static void lcdEnablePulse( void );

static void lcdSendNibble( uint8_t nibble );

/*==================[declaraciones de funciones externas]====================*/

/*==================[definiciones de funciones internas]=====================*/

static void lcdEnablePulse( void )
{
   lcdPinWrite( LCD_HD44780_EN, ON );     // EN = 1 for H-to-L pulse
   //lcdDelay_us( LCD_EN_PULSE_WAIT_US );   // Wait to make EN wider
   for(i=0;i<(10000)*3/5;i++);

   lcdPinWrite( LCD_HD44780_EN, OFF );    // EN = 0 for H-to-L pulse
}

static void lcdSendNibble( uint8_t nibble )
{
   lcdPinWrite( LCD_HD44780_D7, ( nibble & 0x80 ) );
   lcdPinWrite( LCD_HD44780_D6, ( nibble & 0x40 ) );
   lcdPinWrite( LCD_HD44780_D5, ( nibble & 0x20 ) );
   lcdPinWrite( LCD_HD44780_D4, ( nibble & 0x10 ) );
}

/*==================[definiciones de funciones externas]=====================*/

void lcdCommand( uint8_t cmd ){

   lcdSendNibble( cmd & 0xF0 );          // Send high nibble to D7-D4

   lcdPinWrite( LCD_HD44780_RS, OFF );   // RS = 0 for command
   lcdPinWrite( LCD_HD44780_RW, OFF );   // RW = 0 for write

   lcdEnablePulse();
   //lcdDelay_us( LCD_LOW_WAIT_US );       // Wait
   for(i=0;i<(10000)*3/5;i++);


   lcdSendNibble( cmd << 4 );            // Send low nibble to D7-D4
   lcdEnablePulse();
}

void lcdData( uint8_t data ){

   lcdSendNibble( data & 0xF0 );         // Send high nibble to D7-D4

   lcdPinWrite( LCD_HD44780_RS, ON );    // RS = 1 for data
   lcdPinWrite( LCD_HD44780_RW, OFF );   // RW = 0 for write

   lcdEnablePulse();

   lcdSendNibble( data << 4 );           // Send low nibble to D7-D4
   lcdEnablePulse();
}

void lcdInit( uint16_t lineWidth, uint16_t amountOfLines,
              uint16_t charWidth, uint16_t charHeight ){

   // Configure LCD Pins as Outputs

   lcdConfigPinAsOutput( LCD_HD44780_RS );
   lcdConfigPinAsOutput( LCD_HD44780_RW );
   lcdConfigPinAsOutput( LCD_HD44780_EN );

   lcdConfigPinAsOutput( LCD_HD44780_D4 );
   lcdConfigPinAsOutput( LCD_HD44780_D5 );
   lcdConfigPinAsOutput( LCD_HD44780_D6 );
   lcdConfigPinAsOutput( LCD_HD44780_D7 );


   // Configure LCD for 4-bit mode

   lcdPinWrite( LCD_HD44780_RW, OFF );   // RW = 0
   lcdPinWrite( LCD_HD44780_RS, OFF );   // RS = 0

   lcdPinWrite( LCD_HD44780_EN, OFF );   // EN = 0

   //lcdDelay_ms( LCD_STARTUP_WAIT_MS );   // Wait for stable power

   for(i=0;i<(100000000)*3/5;i++);
   lcdCommand( 0x33 );                   // Command 0x33 for 4-bit mode
   //lcdCommandDelay();                    // Wait
   for(i=0;i<(10000)*3/5;i++);
   lcdCommand( 0x32 );                   // Command 0x32 for 4-bit mode
   //lcdCommandDelay();                    // Wait
   for(i=0;i<(10000)*3/5;i++);
   lcdCommand( 0x28 );                   // Command 0x28 for 4-bit mode
  // lcdCommandDelay();                    // Wait
   for(i=0;i<(10000)*3/5;i++);

   // Initialize LCD

   lcdCommand( 0x0E );                   // Command 0x0E for display on, cursor on
   //lcdCommandDelay();                    // Wait
   for(i=0;i<(10000)*3/5;i++);
   lcdClear();                           // Command for clear LCD

   lcdCommand( 0x06 );                   // Command 0x06 for Shift cursor right
  // lcdCommandDelay();                    // Wait
   for(i=0;i<(10000)*3/5;i++);
  // lcdDelay_ms( 1 );
   for(i=0;i<(100000000)/5000;i++);
}

void lcdGoToXY( uint8_t x, uint8_t y )
{
   uint8_t firstCharAdress[] = { 0x80, 0xC0, 0x94, 0xD4 };   // See table 12-5
   lcdCommand( firstCharAdress[ y - 1 ] + x - 1 );
   //lcdDelay_us( LCD_HIGH_WAIT_US );      // Wait
   for(i=0;i<(10000)*3/5;i++);

}

void lcdClear( void )
{
   lcdCommand( 0x01 );                   // Command 0x01 for clear LCD
   //lcdDelay_ms(LCD_CLR_DISP_WAIT_MS);    // Wait
   for(i=0;i<(300000000)/5000;i++);

}

void lcdSendStringRaw( char* str )
{
   uint8_t i = 0;
   while( str[i] != 0 ){
      lcdData( str[i] );
      i++;
   }
}

void lcdCreateChar( uint8_t charnum, const char* chardata )
{
	uint8_t i;
	charnum &= 0x07;
	lcdCommand( E_SET_CGRAM_ADDR | (charnum << 3) );
	for (i = 0; i < 8; i++){
		lcdData( chardata[i] );
	}
   delay(1);
}

