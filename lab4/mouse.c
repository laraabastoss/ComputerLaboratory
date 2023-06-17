#include "mouse.h"

#define IRQ_MOUSE 12
uint8_t byte; //byte mais recentemente lido
uint8_t bytes[3]; //veem sempre por ordem
uint8_t byte_index = 0;
int mouse_hook_id=2;
struct packet mouse_packet;  
int (mouse_subscribe_int)(uint8_t *bit_no){
  if (bit_no==NULL) return 1;
  *bit_no=BIT(mouse_hook_id);
  if (sys_irqsetpolicy(12,IRQ_REENABLE|IRQ_EXCLUSIVE, &mouse_hook_id)) return 1;
  return 0;
}

int (mouse_unsubscribe_int)(){
 if (sys_irqrmpolicy(&mouse_hook_id)) return 1;
 return 0;
}

void (mouse_ih)(){
  read_KBC_output(0x60,&byte,1);
}

void (mouse_sync_bytes)(){
  if ((byte_index==0) && (byte & BIT(3))){
    byte_index++;
    bytes[0]=byte;
  }
  else if (byte_index>0){
     bytes[byte_index]=byte;
    byte_index++;
  }
}

//MUITO IMPORTANTE escreve se o comando 0xD4 na porta de input para depois se escrever o comando a seguir na de output
int (mouse_write_data)(uint8_t command){
  int attempts =10; uint8_t mouse_response;
  do {
    attempts--;
    if (write_KBC_command(0x64, 0xD4)) return 1;              // Ativar do modo D4 do i8042
    if (write_KBC_command(0x60, command)) return 1;           // O comando para o rato é escrito na porta 0x60
    tickdelay(micros_to_ticks(20000));                        // Esperar alguns milissegundos
    if (util_sys_inb(0x60, &mouse_response)) return 1;        // Ler a resposta da porta do output buffer
    if (mouse_response == 0xFA) return 0;                      // Se a resposta for ACK, interromper o ciclo
  } while (mouse_response != 0xFA && attempts);       

 return 1;
}

void (mouse_bytes_to_packet)(){
    for (int i = 0 ; i < 3 ; i++) {
    mouse_packet.bytes[i] = bytes[i];
  }
  mouse_packet.lb = bytes[0] & BIT(0);
  mouse_packet.mb = bytes[0] & BIT(2);
  mouse_packet.rb = bytes[0] & BIT(1);
  mouse_packet.x_ov = bytes[0] & BIT(6);
  mouse_packet.y_ov = bytes[0] & BIT(7);
  mouse_packet.delta_x = (bytes[0] & BIT(4)) ? (0xFF00 | bytes[1]) : bytes[1];
  mouse_packet.delta_y = (bytes[0] & BIT(5)) ? (0xFF00 | bytes[2]) : bytes[2];
}

