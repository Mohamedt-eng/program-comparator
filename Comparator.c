//pic 16f628A
// Lcd pinout settings 
//#include <ctype.h>
sbit LCD_RS at RB5_bit;
sbit LCD_EN at RB4_bit;
sbit LCD_D7 at RB3_bit;
sbit LCD_D6 at RB2_bit;
sbit LCD_D5 at RB1_bit;
sbit LCD_D4 at RB0_bit;
//Pin direction
sbit LCD_RS_Direction at TRISB5_bit;
sbit LCD_EN_Direction at TRISB4_bit;
sbit LCD_D7_Direction at TRISB3_bit;
sbit LCD_D6_Direction at TRISB2_bit;
sbit LCD_D5_Direction at TRISB1_bit;
sbit LCD_D4_Direction at TRISB0_bit;
/*###############################*/
#define pin_ra3 RA3_bit
void init_D();
void convert_value();
long resolution_ADC(long resolution);
long value;
/*################################*/

void main(){
        init_D();                         //function inisialization CM
        Lcd_Init();                      //function Inisialization LCD
        Lcd_Cmd(_LCD_CLEAR);             //clear Lcd
        Lcd_Cmd(_LCD_CURSOR_OFF);
        Lcd_Out(1,6,"<Value>");
        while(1){
             value = resolution_ADC(1024);
             convert_value();
        }
}
/*################################*/
void init_D(){
 VRCON         = 0XEC;                   //"11101100" Enable,Low range,(VR<3:0>/ 24)*VDD
 TRISA.F3      = 0X00;                  //Config Register TRISA Pin "11110011"
 CMCON         = 0X06;                  //Config MODE comparator   "00000110"
 delay_us(10);
}
/*###############################*/
long resolution_ADC(long resolution){
    long result    = 0x00;              //
    OPTION_REG.b5  = 0;                 // Internal cycle clock (CLKOUT)
    PSA_bit        = 0x01;              // Prescaler is assigned to the WDT
    CMCON          = 0X03;              // comparator mode CM<2:0> = 011
    while (resolution){
        TMR0     = 225;
        T0IF_bit = 0;
        if(C1OUT_bit) {
                pin_ra3= 1;
        }
        else{
                pin_ra3 = 0;
                result += 1;
        }
        resolution -=1;
        while(!T0IF_bit);
    }
    CMCON = 0X06;                          // comparator mode CM<2:0> = 110
    return (result);
}
/*#################################*/
void convert_value(){                      // conversion resolution value

    unsigned char* val[4] = {"","","",""};
    val[0] = value/1000;
    val[1] = (value%1000)/100;
    val[2] = (value%100)/10;
    val[3] =  value%10;
    //============================
    Lcd_Chr(2,8,val[0]+48);               // Output Lcd value+0 Zero instead of space
    Lcd_Chr_Cp(val[1]+48);
    Lcd_chr_Cp(val[2]+48);
    Lcd_Chr_Cp(val[3]+48);
    /******************************/
    delay_ms(50);
} 






/*void calcvol() {
    long volt;
    char nmv[3];// = {"","",""};

    //nmv = (value/1024)*5  ;
    //volt = value/1023*5;
    IntToHex((value/24)*5*2,nmv);
    IntToStrWithZeros(volt,nmv);
    Lcd_Out(2,12,nmv);
}*/

/*####################################*/
/*void interrupt(){
   if(CMIF_bit){
       C1OUT_bit = 0;                      // Disable comparators
       C2OUT_bit = 0;

       Lcd_Cmd(_LCD_CLEAR);               // Clear display
       Lcd_Out(1,2,"VALUE >");
       Lcd_Out(2,2,read_VR());
       //delay_ms(2200);
       //value   =     5;
   }
   CMIF_bit=0;
}*/