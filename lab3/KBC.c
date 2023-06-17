/*
#include <stdint.h>
#include <lcom/lcf.h>
#include "KBC.h"

int (read_KBC_status)(uint8_t* status){
  return util_sys_inb(0x64,status);
}

int (read_KBC_output)(uint8_t port, uint8_t *output){
   int attempts=10;uint8_t status;
  //10 attempts por 20 milisegundos
  while(attempts){
    if (read_KBC_status(&status)) return 1;
    if (status & BIT(0)){
      if (util_sys_inb(port,output)) return 1;
      if (status & BIT (6)) return 1;
      if (status & BIT(7)) return 1;
      return 0;
    }
    tickdelay(micros_to_ticks(20000));
    attempts--;
  
  } return 1;
}

int (write_KBC_command)(uint8_t port, uint8_t commandByte){
  int attempts=10;uint8_t status;
  //10 attempts por 20 milisegundos
  while(attempts){
    if (read_KBC_status(&status)) return 1;
    if (!(status & BIT(1))){
      if (sys_outb(port,commandByte)) return 1;
      return 0;
    }
    tickdelay(micros_to_ticks(20000));
    attempts--;
  } return 1;
}
*/

#include <stdint.h>
#include <lcom/lcf.h>
#include "KBC.h"

int (read_KBC_status)(uint8_t* status){
  if (util_sys_inb(0x64,status)) return 1;
  return 0;
}

int (read_KBC_output)(uint8_t port, uint8_t *output){
  int attempts=10;uint8_t status;
  while (attempts){
    if (read_KBC_status(&status)) return 1;
    if (status & BIT(0)){
      if (status &BIT(6)) return 1;
      if (status & BIT(7)) return 1;
      if (util_sys_inb(port,output)) return 1;
      return 0;
    }
  }
  return 1;
}

int (write_KBC_command)(uint8_t port, uint8_t commandByte){
    int attempts=10;uint8_t status;
  while (attempts){
    if (read_KBC_status(&status)) return 1;
    if (!(status & BIT(1))){
      if (sys_outb(port,commandByte)) return 1;
      return 0;
    }
  }
  return 1;
}

