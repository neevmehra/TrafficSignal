/* ECE319K_Lab4main.c
 * Traffic light FSM
 * ECE319H students must use pointers for next state
 * ECE319K students can use indices or pointers for next state
 * Obedient Oscar Portillo & Nice Neev Mehra
*/


#include <ti/devices/msp/msp.h>
#include "../inc/LaunchPad.h"
#include "../inc/Clock.h"
#include "../inc/UART.h"
#include "../inc/Timer.h"
#include "../inc/Dump.h"  // student's Lab 3
#include <stdio.h>
#include <string.h>
// put both EIDs in the next two lines */
const char EID1[] = "NM34484"; //  ;replace abc123 with your EID
const char EID2[] = "OAP444"; //  ;replace abc123 with your EID

#define WHITE (RED|GREEN|BLUE)

// initialize all 6 LED outputs and 3 switch inputs
// assumes LaunchPad_Init resets and powers A and B
void Traffic_Init(void){ // assumes LaunchPad_Init resets and powers A and B
 // write this
    IOMUX->SECCFG.PINCM[PB15INDEX] = (uint32_t) 0x00040081; // input west
    IOMUX->SECCFG.PINCM[PB16INDEX] = (uint32_t) 0x00040081; // input south
    IOMUX->SECCFG.PINCM[PB17INDEX] = (uint32_t) 0x00040081; // input walk

    IOMUX->SECCFG.PINCM[PB27INDEX] = (uint32_t)0x00000081; // Don't Walk & Walk Lights
    IOMUX->SECCFG.PINCM[PB26INDEX] = (uint32_t)0x00000081;
    IOMUX->SECCFG.PINCM[PB22INDEX] = (uint32_t)0x00000081;

    IOMUX->SECCFG.PINCM[PB2INDEX] = (uint32_t) 0x00000081; // Option A South Lights
    IOMUX->SECCFG.PINCM[PB1INDEX] = (uint32_t) 0x00000081;
    IOMUX->SECCFG.PINCM[PB0INDEX] = (uint32_t) 0x00000081;

    IOMUX->SECCFG.PINCM[PB8INDEX] = (uint32_t) 0x00000081; // Option X West Lights
    IOMUX->SECCFG.PINCM[PB7INDEX] = (uint32_t) 0x00000081;
    IOMUX->SECCFG.PINCM[PB6INDEX] = (uint32_t) 0x00000081;

    GPIOB->DOE31_0 |= 0x0C4001C7;  // enable outputs

}
/* Activate LEDs
* Inputs: west is 3-bit value to three east/west LEDs
*         south is 3-bit value to three north/south LEDs
*         walk is 3-bit value to 3-color positive logic LED on PB22,PB26,PB27
* Output: none
* - west =1 sets west green
* - west =2 sets west yellow
* - west =4 sets west red
* - south =1 sets south green
* - south =2 sets south yellow
* - south =4 sets south red
* - walk=0 to turn off LED
* - walk bit 22 sets blue color
* - walk bit 26 sets red color
* - walk bit 27 sets green color
* Feel free to change this. But, if you change the way it works, change the test programs too
* Be friendly*/
void Traffic_Out(uint32_t west, uint32_t south, uint32_t walk){
    // write this

    //West
    if(west == 1){
        GPIOB->DOUT31_0 |= 1<<6; // Green LED
    }

    if(west == 2){
        GPIOB->DOUT31_0 |= 1<<7; // Yellow LED
    }

    if(west == 4){
        GPIOB->DOUT31_0 |= 1<<8; // Red LED
    }

    //South
    if(south == 1){
        GPIOB->DOUT31_0 |= 1; // Green LED
    }

    if(south == 2){
        GPIOB->DOUT31_0 |= 1<<1; // Yellow LED
    }

    if(south == 4){
        GPIOB->DOUT31_0 |= 1<<2; // Red LED
    }

    //Walk
    if(walk == 1){
        GPIOB->DOUT31_0 |= (1<<22 | 1<<26 | 1<<27); // White
    }

    if(walk == 2){
        GPIOB->DOUT31_0 |= 1<<26; // Red
    }

    if(walk == 0){
        GPIOB->DOUT31_0 &= ~(1<<22 | 1<<26 | 1<<27); // Off
    }

}
/* Read sensors
 * Input: none
 * Output: sensor values
 * - bit 0 is west car sensor
 * - bit 1 is south car sensor
 * - bit 2 is walk people sensor
* Feel free to change this. But, if you change the way it works, change the test programs too
 */
uint32_t Traffic_In(void){

    uint32_t input = (GPIOB->DIN31_0 & (1<<17 | 1<<16 | 1<<15))>>15; // Verifies which switches are on and shifts them
    return input;

}
// use main1 to determine Lab4 assignment
void Lab4Grader(int mode);
void Grader_Init(void);
int main1(void){ // main1
  Clock_Init80MHz(0);
  LaunchPad_Init();
  Lab4Grader(0); // print assignment, no grading
  while(1){
  }
}
// use main2 to debug LED outputs
// at this point in ECE319K you need to be writing your own test functions
// modify this program so it tests your Traffic_Out  function
int main2(void){ // main2
  Clock_Init80MHz(0);
  LaunchPad_Init();
  Grader_Init(); // execute this line before your code
  LaunchPad_LED1off();
  Traffic_Init(); // your Lab 4 initialization
  if((GPIOB->DOE31_0 & 0x20)==0){
    UART_OutString("access to GPIOB->DOE31_0 should be friendly.\n\r");
  }
  Debug_Init();   // Lab 3 debugging
  UART_Init();
  UART_OutString("Lab 4, Fall 2024, Step 1. Debug LEDs\n\r");
  UART_OutString("EID1= "); UART_OutString((char*)EID1); UART_OutString("\n\r");
  UART_OutString("EID2= "); UART_OutString((char*)EID2); UART_OutString("\n\r");
  while(1){
    for(uint32_t i=1; i<8; i = i<<1){ //1,2,4
      Traffic_Out(i,0,0); // Your Lab 4 output
      Debug_Dump(i);
      Clock_Delay(40000000); // 0.5s
    }
    for(uint32_t i=1; i<8; i = i<<1){ //1,2,4
      Traffic_Out(0,i,0); // Your Lab 4 output
      Debug_Dump(i<<3);
      Clock_Delay(40000000); // 0.5s
    }
    Traffic_Out(0,0,RED); // Your Lab 4 output
    Debug_Dump(RED);
    Clock_Delay(40000000); // 0.5s
    Traffic_Out(0,0,WHITE); // Your Lab 4 output
    Debug_Dump(WHITE);
    Clock_Delay(40000000); // 0.5s
    if((GPIOB->DOUT31_0&0x20) == 0){
      UART_OutString("DOUT not friendly\n\r");
    }
  }
}
// use main3 to debug the three input switches
// at this point in ECE319K you need to be writing your own test functions
// modify this program so it tests your Traffic_In  function
int main3(void){ // main3
  uint32_t last=0,now;
  Clock_Init80MHz(0);
  LaunchPad_Init();
  Traffic_Init(); // your Lab 4 initialization
  Debug_Init();   // Lab 3 debugging
  UART_Init();
  __enable_irq(); // UART uses interrupts
  UART_OutString("Lab 4, Fall 2024, Step 2. Debug switches\n\r");
  UART_OutString("EID1= "); UART_OutString((char*)EID1); UART_OutString("\n\r");
  UART_OutString("EID2= "); UART_OutString((char*)EID2); UART_OutString("\n\r");
  while(1){
    now = Traffic_In(); // Your Lab4 input
    if(now != last){ // change
      UART_OutString("Switch= 0x"); UART_OutUHex(now); UART_OutString("\n\r");
      Debug_Dump(now);
    }
    last = now;
    Clock_Delay(800000); // 10ms, to debounce switch
  }
}
// use main4 to debug using your dump
// proving your machine cycles through all states
int main4(void){// main4
uint32_t input;
  Clock_Init80MHz(0);
  LaunchPad_Init();
  LaunchPad_LED1off();
  Traffic_Init(); // your Lab 4 initialization
  // set initial state
  Debug_Init();   // Lab 3 debugging
  UART_Init();
  __enable_irq(); // UART uses interrupts
  UART_OutString("Lab 4, Fall 2024, Step 3. Debug FSM cycle\n\r");
  UART_OutString("EID1= "); UART_OutString((char*)EID1); UART_OutString("\n\r");
  UART_OutString("EID2= "); UART_OutString((char*)EID2); UART_OutString("\n\r");

  // initialize your FSM

  struct State {
      uint32_t Out;
      uint32_t Time;
      uint32_t NextState[8]; // Three parameters for my FSM
  };

  typedef const struct State State_t; // Defines "struct State" as a constant and refers to it as State_t

#define WestGreen 0 // Also our safe mode
#define WestYellow 1
#define WestRed 2
#define SouthGreen 3
#define SouthYellow 4
#define SouthRed 5
#define WalkWhite 6
#define WalkRed1 7
#define WalkOff1 8
#define WalkRed2 9
#define WalkOff2 10
#define WalkRed 11

  State_t FSM[12] = {
          {((1<<26)+(1<<6)+(1<<2)), 3, {WestGreen, WestGreen, WestYellow, WestYellow, WestYellow, WestYellow, WestYellow, WestYellow}}, // West Green
          {((1<<7)+(1<<2)+(1<<26)), 3, {WestRed, WestRed, WestRed, WestRed, WestRed, WestRed, WestRed, WestRed}}, // West Yellow
          {((1<<8)+(1<<2)+(1<<26)), 3, {SouthGreen, WestGreen, SouthGreen, SouthGreen, WalkWhite, WalkWhite, SouthGreen, SouthGreen}}, // West Red
          {((1<<8)+(1<<0)+(1<<26)), 3, {SouthGreen, SouthYellow, SouthGreen, SouthYellow, SouthYellow, SouthYellow, SouthYellow, SouthYellow}}, // South Green
          {((1<<8)+(1<<1)+(1<<26)), 3, {SouthRed, SouthRed, SouthRed, SouthRed, SouthRed, SouthRed, SouthRed, SouthRed}}, // South Yellow
          {((1<<8)+(1<<2)+(1<<26)), 3, {WalkWhite, WestGreen, SouthGreen, WestGreen, WalkWhite, WestGreen, WalkWhite, WalkWhite}}, // South Red
          {((1<<8)+(1<<2)+(1<<26)+(1<<22)+(1<<27)), 3, {WalkWhite, WalkRed1, WalkRed1, WalkRed1, WalkWhite, WalkRed1, WalkRed1, WalkRed1}}, // Walk White
          {((1<<8)+(1<<2)+(1<<26)), 3, {WalkOff1, WalkOff1, WalkOff1, WalkOff1, WalkOff1, WalkOff1, WalkOff1, WalkOff1}}, // WalkRed One
          {((1<<8)+(1<<2)), 3, {WalkRed2, WalkRed2, WalkRed2, WalkRed2, WalkRed2, WalkRed2, WalkRed2, WalkRed2}}, // WalkOff One
          {((1<<8)+(1<<2)+(1<<26)), 3, {WalkOff2, WalkOff2, WalkOff2, WalkOff2, WalkOff2, WalkOff2, WalkOff2, WalkOff2}}, // WalkRed Two
          {((1<<8)+(1<<2)), 3, {WalkRed, WalkRed, WalkRed, WalkRed, WalkRed, WalkRed, WalkRed, WalkRed}}, // WalkOff Two
          {((1<<8)+(1<<2)+(1<<26)), 3, {WestGreen, WestGreen, SouthGreen, SouthGreen, WalkWhite, WestGreen, SouthGreen, WestGreen}}, // WalkRed

    };


  SysTick_Init(); // Initialize SysTick for software waits

  uint32_t States = WestGreen;

  while(1){
      // 1) output depending on state using Traffic_Out

      GPIOB->DOUT31_0 = (GPIOB->DOUT31_0 & (~0x0C4001C7)) | FSM[States].Out;

      // call your Debug_Dump logging your state number and output

      Debug_Dump(GPIOB->DOUT31_0 | (States<<25));

      // 2) wait depending on state

      SysTick_Wait10ms(FSM[States].Time);

      // 3) hard code this so input always shows all switches pressed

      uint32_t input = 7;

      // 4) next depends on state and input

      States = FSM[States].NextState[input];

  }
}



// use main5 to grade
int main(void){  // main5
  Clock_Init80MHz(0);
  LaunchPad_Init();
  Grader_Init(); // execute this line before your code
  Traffic_Init(); // your Lab 4 initialization
  SysTick_Init();   // Initialize SysTick for software waits
  Debug_Init(); // Lab 3
  // initialize your FSM

  struct State {
      uint32_t Out;
      uint32_t Time;
      uint32_t NextState[8];
  };

  typedef const struct State State_t;

#define WestGreen 0 // Also our safe mode
#define WestYellow 1
#define WestRed 2
#define SouthGreen 3
#define SouthYellow 4
#define SouthRed 5
#define WalkWhite 6
#define WalkRed1 7
#define WalkOff1 8
#define WalkRed2 9
#define WalkOff2 10
#define WalkRed 11

  State_t FSM[12] = {
        {((1<<26)+(1<<6)+(1<<2)), 20, {WestGreen, WestGreen, WestYellow, WestYellow, WestYellow, WestYellow, WestYellow, WestYellow}}, // West Green
        {((1<<7)+(1<<2)+(1<<26)), 20, {WestRed, WestRed, WestRed, WestRed, WestRed, WestRed, WestRed, WestRed}}, // West Yellow
        {((1<<8)+(1<<2)+(1<<26)), 20, {SouthGreen, WestGreen, SouthGreen, SouthGreen, WalkWhite, WalkWhite, SouthGreen, SouthGreen}}, // West Red
        {((1<<8)+(1<<0)+(1<<26)), 20, {SouthGreen, SouthYellow, SouthGreen, SouthYellow, SouthYellow, SouthYellow, SouthYellow, SouthYellow}}, // South Green
        {((1<<8)+(1<<1)+(1<<26)), 20, {SouthRed, SouthRed, SouthRed, SouthRed, SouthRed, SouthRed, SouthRed, SouthRed}}, // South Yellow
        {((1<<8)+(1<<2)+(1<<26)), 20, {WalkWhite, WestGreen, SouthGreen, WestGreen, WalkWhite, WestGreen, WalkWhite, WalkWhite}}, // South Red
        {((1<<8)+(1<<2)+(1<<26)+(1<<22)+(1<<27)), 20, {WalkWhite, WalkRed1, WalkRed1, WalkRed1, WalkWhite, WalkRed1, WalkRed1, WalkRed1}}, // Walk White
        {((1<<8)+(1<<2)+(1<<26)), 20, {WalkOff1, WalkOff1, WalkOff1, WalkOff1, WalkOff1, WalkOff1, WalkOff1, WalkOff1}}, // WalkRed One
        {((1<<8)+(1<<2)), 20, {WalkRed2, WalkRed2, WalkRed2, WalkRed2, WalkRed2, WalkRed2, WalkRed2, WalkRed2}}, // WalkOff One
        {((1<<8)+(1<<2)+(1<<26)), 20, {WalkOff2, WalkOff2, WalkOff2, WalkOff2, WalkOff2, WalkOff2, WalkOff2, WalkOff2}}, // WalkRed Two
        {((1<<8)+(1<<2)), 20, {WalkRed, WalkRed, WalkRed, WalkRed, WalkRed, WalkRed, WalkRed, WalkRed}}, // WalkOff Two
        {((1<<8)+(1<<2)+(1<<26)), 20, {WestGreen, WestGreen, SouthGreen, SouthGreen, WalkWhite, WestGreen, SouthGreen, WestGreen}}, // WalkRed

  };


  SysTick_Init(); // Initialize SysTick for software waits

  uint32_t States = WestGreen;
  uint32_t input;

  Lab4Grader(1); // activate UART, grader and interrupts
  while(1){

            // 1) output depending on state using Traffic_Out

            GPIOB->DOUT31_0 = (GPIOB->DOUT31_0 & (~0x0C4001C7)) | FSM[States].Out;

            // call your Debug_Dump logging your state number and output



            // 2) wait depending on state

            SysTick_Wait10ms(FSM[States].Time);

            // 3) grab input

            input = (GPIOB->DIN31_0 & 0x38000)>>15;

            // 4) next depends on state and input

            States = FSM[States].NextState[input];

  }
}
