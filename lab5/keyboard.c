//keyboard->i8042->CPU->ecrã

//makecode é igual ao break code mas com o bit 0
//input buffer tem de estar vazio para usar
//output buffer tem de estar cheio para usar
/*status:
0->output buffer cheio
1->input buffer cheio
2
3
4
5
6->erro de time out
7->erro de paridade
*/
/*
#include "keyboard.h"


int keyboard_hook_id=1;
uint8_t scancode;

int (keyboard_subscribe_interrupts)(uint8_t *bit_no){
  if (bit_no==NULL) return 1;
  *bit_no=BIT(keyboard_hook_id);
  if (sys_irqsetpolicy(KEY_IRQ,IRQ_REENABLE|IRQ_EXCLUSIVE, &keyboard_hook_id)) return 1;
  return 0;
  //no keyboard (ou rato), ao contario do timer que é completamente virtualizado, a policy tem d ser diferente pois nao pode passar diretamanete para o Minix, deve ir do keyboard para o nosso ficheiro
}

int (keyboard_unsubscribe_interrupts)(){
  return sys_irqrmpolicy(&keyboard_hook_id);
}

void (kbc_ih)(){
  //atualiza o scancode que vamos dar como variavel global dentro daquele ficheiro
  read_KBC_output(0x60, &scancode);
}

int (keyboard_restore)(){
  uint8_t command;
  //poderes voltar a escrever no teclado
  if (write_KBC_command(0x64,0x20)) return 1;
  if (read_KBC_output(0x60,&command)) return 1;
  command=command|BIT(0);
  if (write_KBC_command(0x64,0x60)) return 1;
  if (write_KBC_command(0x60, command))return 1;
  return 0;
  }*/
/*
0x20-> teclado vai começar a ler
0x60 ->indicar futura escrita
BIT(0)->indicar ativação de bits de interrupção
*/
//keyboard->i8042->CPU->ecrã

//makecode é igual ao break code mas com o bit 0
//input buffer tem de estar vazio para usar
//output buffer tem de estar cheio para usar
/*status:
0->output buffer cheio
1->input buffer cheio
2
3
4
5
6->erro de time out
7->erro de paridade
*/

#include "keyboard.h"


int keyboard_hook_id=1;
uint8_t scancode;

int (keyboard_subscribe_interrupts)(uint8_t *bit_no){
  if (bit_no==NULL) return 1;
  *bit_no=BIT(keyboard_hook_id);
  if (sys_irqsetpolicy(1,IRQ_REENABLE|IRQ_EXCLUSIVE,&keyboard_hook_id));
  return 0;
  //no keyboard (ou rato), ao contario do timer que é completamente virtualizado, a policy tem d ser diferente pois nao pode passar diretamanete para o Minix, deve ir do keyboard para o nosso ficheiro
}

int (keyboard_unsubscribe_interrupts)(){
  if (sys_irqrmpolicy(&keyboard_hook_id)) return 1;
  return 0;
}

void (kbc_ih)(){
  //atualiza o scancode que vamos dar como variavel global dentro daquele ficheiro
  read_KBC_output(0x60,&scancode);
}

int (keyboard_restore)(){
  uint8_t cw;
  if (write_KBC_command(0x64,0x20)) return 1;
  if (read_KBC_output(0x60,&cw)) return 1;
  cw=cw|BIT(0);
  if (write_KBC_command(0x64,0x60)) return 1;
  if (write_KBC_command(0x60, cw)) return 1;
  return 0;
  }

/*
0x20-> teclado vai começar a ler
0x60 ->indicar futura escrita
BIT(0)->indicar ativação de bits de interrupção
*/
