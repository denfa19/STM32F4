#include "util.h"

char *sput_i(int integer, char *string);
char *sput_ip1(int integer, char *string);

/**
 * Integer to ascii converter.
 */
void itoa(int integer, char *string)
{
	if (0 > integer) {
		++integer;
		*string++ = '-';
		*sput_ip1(-integer, string) = '\0';
	} else {
		*sput_i(integer, string) = '\0';
	}
}

char *sput_i(int integer, char *string)
{
	if (integer / 10 != 0) {
		string = sput_i(integer / 10, string);
	}
	*string++ = (char)('0' + integer % 10);
	return string;
}

char *sput_ip1(int integer, char *string)
{
	int digit;

	digit = (integer % 10 + 1) % 10;
	if (integer / 10 != 0) {
		string = (digit == 0 ? sput_ip1 : sput_i)(integer / 10, string);
		*string++ = (char)('0' + digit);
	} else {
		if (digit == 0) {
			*string++ = '1';
		}
		*string++ = (char)('0' + digit);
	}
	return string;
}

/**
 * Float to ascii converter.
 */
void ftoa(float Value, char* Buffer)
 {
     union float_val
     {
         float f;
     
         struct
         {
             unsigned int    mantissa_lo : 16;
             unsigned int    mantissa_hi : 7;    
             unsigned int     exponent : 8;
             unsigned int     sign : 1;
         } the_float_struct;
     } helper;
     
     unsigned long mantissa;
     signed char exponent;
     unsigned int int_part;
     char frac_part[3];
     int i, count = 0;
     
     helper.f = Value;
     //mantissa is LS 23 bits
     mantissa = helper.the_float_struct.mantissa_lo;
     mantissa += ((unsigned long) helper.the_float_struct.mantissa_hi << 16);
     //add the 24th bit to get 1.mmmm^eeee format
     mantissa += 0x00800000;
     //exponent is biased by 127
     exponent = (signed char) helper.the_float_struct.exponent - 127;
     
     //too big to shove into 8 chars
     if (exponent > 18)
     {
         Buffer[0] = 'I';
         Buffer[1] = 'n';
         Buffer[2] = 'f';
         Buffer[3] = '\0';
         return;
     }
     
     //too small to resolve (resolution of 1/8)
     if (exponent < -3)
     {
         Buffer[0] = '0';
         Buffer[1] = '\0';
         return;
     }
     
     count = 0;
     
     //add negative sign (if applicable)
     if (helper.the_float_struct.sign)
     {
         Buffer[0] = '-';
         count++;
     }
     
     //get the integer part
     int_part = mantissa >> (23 - exponent);    
     //convert to string
     itoa(int_part, &Buffer[count]);
     
     //find the end of the integer
     for (i = 0; i < 8; i++)
         if (Buffer[i] == '\0')
         {
             count = i;
             break;
         }        
 
     //not enough room in the buffer for the frac part    
     if (count > 5)
         return;
     
     //add the decimal point    
     Buffer[count++] = '.';
     
     //use switch to resolve the fractional part
     switch (0x7 & (mantissa  >> (20 - exponent)))
     {
         case 0:
             frac_part[0] = '0';
             frac_part[1] = '0';
             frac_part[2] = '0';
             break;
         case 1:
             frac_part[0] = '1';
             frac_part[1] = '2';
             frac_part[2] = '5';            
             break;
         case 2:
             frac_part[0] = '2';
             frac_part[1] = '5';
             frac_part[2] = '0';            
             break;
         case 3:
             frac_part[0] = '3';
             frac_part[1] = '7';
             frac_part[2] = '5';            
             break;
         case 4:
             frac_part[0] = '5';
             frac_part[1] = '0';
             frac_part[2] = '0';            
             break;
         case 5:
             frac_part[0] = '6';
             frac_part[1] = '2';
             frac_part[2] = '5';            
             break;
         case 6:
             frac_part[0] = '7';
             frac_part[1] = '5';
             frac_part[2] = '0';            
             break;
         case 7:
             frac_part[0] = '8';
             frac_part[1] = '7';
             frac_part[2] = '5';                    
             break;
     }
     
     //add the fractional part to the output string
     for (i = 0; i < 3; i++)
         if (count < 7)
             Buffer[count++] = frac_part[i];
     
     //make sure the output is terminated
     Buffer[count] = '\0';
 }
