/*
status:
0->output buffer cheio
1->input cheio
2->
3
4
5->mouse
6->erro timeout
7->erro paridade


no projeto:
importante priemiro testar se houve interrupcao no teclado, porque este tem prioridade ao mouse



control:
0->RB
1->MB
2->LB
3->1
4->delta x
5->delta y
6->overflow x
7->overflow y

*/
#include <lcom/lcf.h>

#include "KBC.c"
#include "i8042.h"
#include "i8254.h"
#include "mouse.h"
#include "timer.c"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

/* para as três primeiras funções */
extern struct packet mouse_packet;
extern uint8_t byte_index;
extern int counter;

int main(int argc, char *argv[]) {
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need/ it]
  lcf_trace_calls("/home/lcom/labs/lab4/trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  lcf_log_output("/home/lcom/labs/lab4/output.txt");
  

  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv))
    return 1;

  // LCF clean up tasks
  // [must be the last statement before return]
  lcf_cleanup();

  return 0;
}

typedef enum {
  START,
  LINE_DOWN,
  TOP,
  LINE_UP,
  FINISH
} SystemState;

SystemState state = START;
uint16_t x_len_total = 0;

int(mouse_test_packet)(uint32_t cnt) {
  int ipc_status;
  message msg;
  uint8_t mouse_mask;
  if (mouse_mode_d4(0xF4))
    return 1;
  if (mouse_subscribe_int(&mouse_mask))
    return 1;
  while (cnt) { // Só termina quando lermos @cnt pacotes

    if (driver_receive(ANY, &msg, &ipc_status) != 0) {
      printf("Error");
      continue;
    }

    if (is_ipc_notify(ipc_status)) {
      switch (_ENDPOINT_P(msg.m_source)) {
        case HARDWARE:
          if (msg.m_notify.interrupts & mouse_mask) { // Se for uma interrupção do rato
            mouse_ih();
            mouse_sync_bytes();
            if (byte_index == 3) {
              mouse_bytes_to_packet();
              mouse_print_packet(&mouse_packet);
              byte_index = 0;
              cnt--;
            }
          }
      }
    }
  }

  if (mouse_unsubscribe_int())
    return 1;
   if (mouse_disable_stream())
    return 1;
  return 0;
}

int(mouse_test_async)(uint8_t idle_time) {
  int ipc_status, seconds = 0;
  message msg;
  uint8_t mouse_mask, timer_mask;
  uint16_t timer_frequency = sys_hz();
  if (timer_subscribe_int(&timer_mask))
    return 1;
    
  if (mouse_mode_d4(0xF4))
    return 1;
  if (mouse_subscribe_int(&mouse_mask))
    return 1;
  
  while (seconds < idle_time) { // Só termina quando lermos @cnt pacotes

    if (driver_receive(ANY, &msg, &ipc_status) != 0) {
      printf("Error");
      continue;
    }

    if (is_ipc_notify(ipc_status)) {
      switch (_ENDPOINT_P(msg.m_source)) {
        case HARDWARE:
          if (msg.m_notify.interrupts & timer_mask) {
            timer_int_handler();
            if (counter % timer_frequency == 0) {
              seconds++;
            }
          }
          if (msg.m_notify.interrupts & mouse_mask) { // Se for uma interrupção do rato
            mouse_ih();
            mouse_sync_bytes();
            if (byte_index == 3) {
              mouse_bytes_to_packet();
              mouse_print_packet(&mouse_packet);
              byte_index = 0;
            }
            counter = 0;
            seconds = 0;
          }
          break;
      }
    }
  }
  if (timer_unsubscribe_int())
    return 1;
 
  if (mouse_unsubscribe_int())
    return 1;

   if (mouse_mode_d4(0xF5))
    return 1;
  return 0;
}

void updateStateMachine(uint8_t x_len, uint8_t tolerance) {
  switch (state) {

    case START:
       printf("start");
      // transição I
      // se só o left-button estiver pressionado
      if (mouse_packet.lb && !mouse_packet.rb && !mouse_packet.mb) {
        state = LINE_UP;
      }

      break;

    case LINE_UP:


      if (mouse_packet.lb && !mouse_packet.rb && !mouse_packet.mb && mouse_packet.delta_x>0 &&  mouse_packet.delta_y / mouse_packet.delta_x >= 1){
        break;
      }

      else if (!mouse_packet.lb){
        state = TOP;
        break;
      }

      else{
        state = START;
        break;
      }

    case TOP:


      if (mouse_packet.rb){
        state = LINE_DOWN;
        break;
      }

      else{
         state = TOP;
         break;
      }



    case LINE_DOWN:


      if (!mouse_packet.lb && mouse_packet.rb && !mouse_packet.mb && mouse_packet.delta_x>0 &&  mouse_packet.delta_y / mouse_packet.delta_x <= -1){
        break;
      }

      else if (!mouse_packet.rb && x_len_total>x_len){
        state = END;
      }

      else{
        state = START;
      }

    case END:
     printf("end");
      break;
  }
  x_len_total = 0 > x_len_total + mouse_packet.delta_x ? 0 : x_len_total + mouse_packet.delta_x;
}

int(mouse_test_gesture)(uint8_t x_len, uint8_t tolerance) {
  int ipc_status;
  message msg;
  uint8_t mouse_mask;
  if (mouse_mode_d4(0xF4))
    return 1;
  if (mouse_subscribe_int(&mouse_mask))
    return 1;
 
  while (state != FINISH) { // Só termina quando lermos @cnt pacotes

    if (driver_receive(ANY, &msg, &ipc_status) != 0) {
      printf("Error");
      continue;
    }

    if (is_ipc_notify(ipc_status)) {
      switch (_ENDPOINT_P(msg.m_source)) {
        case HARDWARE:
          if (msg.m_notify.interrupts & mouse_mask) { // Se for uma interrupção do rato
            mouse_ih();
            mouse_sync_bytes();
            if (byte_index == 3) {
              mouse_bytes_to_packet();
              mouse_print_packet(&mouse_packet);
              updateStateMachine(x_len, tolerance);
              byte_index = 0;
            }
          }
          break;
      }
    }
  }


  if (mouse_unsubscribe_int(0))
    return 1;

    if (mouse_disable_stream())
    return 1;
  return 0;
}
