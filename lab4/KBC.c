#include <minix/sysutil.h>
#include <lcom/lcf.h>
#include "i8042.h"


int (read_KBC_status)(uint8_t* status){

 if (util_sys_inb(0x64,status)) return 1;
 return 0;

}

int (read_KBC_output)(uint8_t port, uint8_t *output, uint8_t mouse){
  
  //loop to check every 20ms
  int keep_trying = 0;
  while (keep_trying < 20){

    //read status
    uint8_t status;
    if (read_KBC_status(&status)) return 1;

    //if buffer  full
    if (status & OUT_BUFFER_FULL){
      
      //if there are no errors
      if (!(((status & TIMEOUT_ERROR) || (status & PARITY_ERROR))) ){

        if (mouse && !(status & BIT(5))) return 1;
        if (!mouse && (status & BIT(5))) return 1;

        //read and return
        if (util_sys_inb(port,output)) return 1;
        return 0;
      } 
      
      else return 1;
    }
    tickdelay(micros_to_ticks(20000));
    keep_trying++;
  }

  return 1;
}


int (write_KBC_command)(uint8_t port, uint8_t commandByte){

  int keep_trying=0;
  while (keep_trying<20){

    //read status
    uint8_t status;
    if (read_KBC_status(&status)) return 1;

    //if buffer not full
    if (!(status & BIT(1))){

      //write and return
      if (sys_outb(port,commandByte)) return 1;
      return 0;
    }
    tickdelay(micros_to_ticks(DELAY_US));
    keep_trying++;
  }
  return 1;

}

