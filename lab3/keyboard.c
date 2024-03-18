#include <minix/sysutil.h>
#include <lcom/lcf.h>
#include "i8042.h"


uint8_t scancode;
int keyboard_hook_id=1;
bool isMake;
int size;
int curr_byte=0;
int counter;
uint8_t scancode_array[2];
bool wait = false;



int (read_KBC_status)(uint8_t* status){
  if (util_sys_inb(INPUT_BUFFER_KEYBOARD,status)) return 1;
  return 0;
}

int (read_KBC_output)(uint8_t port, uint8_t *output){
  
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

        //read and return
        if (util_sys_inb(port,output)) return 1;
        return 0;
      } 
      
      else return 1;
    }
    tickdelay(micros_to_ticks(DELAY_US));
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


int (keyboard_subscribe_interrupts)(uint8_t *bit_no){
  if (bit_no==NULL) return 1;
  *bit_no=BIT(keyboard_hook_id);
  if (sys_irqsetpolicy(1,IRQ_REENABLE|IRQ_EXCLUSIVE,&keyboard_hook_id));
  return 0;
  //NOTA: no keyboard (ou rato), ao contario do timer que é completamente virtualizado, a policy tem d ser diferente pois nao pode passar diretamanete para o Minix, deve ir do keyboard para o nosso ficheiro
}

int (keyboard_unsubscribe_interrupts)(){
  if (sys_irqrmpolicy(&keyboard_hook_id)) return 1;
  return 0;
}


  //atualiza o scancode que vamos dar como variavel global dentro daquele ficheiro
  //read_KBC_output(0x60,&scancode);

 /*void (kbc_ih)() {

  uint8_t status;
  if (util_sys_inb(0x64, &status) == 1) return;
  counter++;
  if ((status & 0x01) == 0) return;
  // check communication errors  
  if ((status & 0xC0) != 0) return;

  if (util_sys_inb(0x60, &scancode2[idx]) == 1) return;
  //printf(idx);
  //printf(scancode2[idx]);
  counter++;
  if ((scancode2[idx] & BIT(7))) make = false;
  else make = true;
  if (scancode2[idx] == 0xE0) {
    isScancode = true;
    size = 2;
    idx = 1;
    scancode2[0] = 0xE0;
  }
  else if (isScancode == false) {
    size = 1;
  }
  else {
    if ((scancode2[idx] & BIT(7))) make = false;
    else make = true;
    idx = 0;
    isScancode = false;
  }
}*/






int (enable_interrupts)(){
  
  //warn you are going to read
  if (write_KBC_command(0x64,0x20)) return 1;
  //read
  uint8_t cw;
  if (read_KBC_output(0x60,&cw)) return 1;
  //activate BIT 0 -> turn on interrupts
  cw=cw|BIT(0);
  //warn you are going to write
  if (write_KBC_command(0x64,0x60)) return 1;
  //wrire
  if (write_KBC_command(0x60, cw)) return 1;
  return 0;
  }

/*
0x20-> teclado vai começar a ler
0x60 ->indicar futura escrita
BIT(0)->indicar ativação de bits de interrupção
*/


/*void (kbc_ih)(){
  //atualiza o scancode que vamos dar como variavel global dentro daquele ficheiro
  read_KBC_output(0x60, &scancode);
}*/

void (kbc_ih)() {
  if (wait!=true)  size = 1;
  read_KBC_output(0x60, &scancode_array[curr_byte]);
  wait = false;
  if (scancode_array[curr_byte] == 0xE0) {
    wait = true;
    size = 2;
    curr_byte = 1;
    scancode_array[0] = 0xE0;
  }
  else if (wait == false) {
    
    if ((scancode_array[curr_byte] & BIT(7))) isMake = false;
    else isMake = true;
    curr_byte = 0;

  }
}
