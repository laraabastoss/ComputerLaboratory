/*#include <lcom/lcf.h>

#include <lcom/lab3.h>

#include <stdbool.h>
#include <stdint.h>
#include "i8042.h"
#include "keyboard.h"
#include "KBC.c"
#include "timer.c"
#include "i8254.h"


extern uint8_t scancode;
extern int counter;
//quando se cahamr o ficheiro keyboard.c vais conseguir ver a variavel scancode
int main(int argc, char *argv[]) {
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need it]
  lcf_trace_calls("/home/lcom/labs/lab3/trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  lcf_log_output("/home/lcom/labs/lab3/output.txt");

  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv))
    return 1;

  // LCF clean up tasks
  // [must be the last statement before return]
  lcf_cleanup();

  return 0;
}

int(kbd_test_scan)() {
  uint8_t keyboard_mask;
  int ipc_status;
  message msg;
  if (keyboard_subscribe_interrupts(&keyboard_mask)) return 1;
  while(scancode!=BREAK_ESC){
    //(...)
       if ( driver_receive(ANY, &msg, &ipc_status) != 0 ) { 
         printf("Error");
         continue;
     }
      if (is_ipc_notify(ipc_status)) { 
         switch (_ENDPOINT_P(msg.m_source)) {
            case HARDWARE:               
                if (msg.m_notify.interrupts & keyboard_mask) { 
                   kbc_ih();
                   kbd_print_scancode(!(MAKE_CODE & scancode),scancode==TWO_BYTES?2:1,&scancode);
                }
              break;
            default:
              break;  
         }
     } else { 
     }
 
  }if (keyboard_unsubscribe_interrupts()) return 1;

  return 1;
}

int(kbd_test_poll)() {
  //utilizar polling
  while(scancode!=BREAK_ESC){
    if (read_KBC_output(0x60, &scancode)==0)

     kbd_print_scancode(!(MAKE_CODE & scancode),scancode==TWO_BYTES?2:1,&scancode);
  }
  if (keyboard_restore()) return 1;
  return 0;
}

int(kbd_test_timed_scan)(uint8_t n) {
  uint8_t keyboard_mask, timer_mask;
  int ipc_status, time_passed=0;
  message msg;
  if (keyboard_subscribe_interrupts(&keyboard_mask)) return 1;
  if (timer_subscribe_int(&timer_mask)) return 1;
  while(scancode!=BREAK_ESC && time_passed<n){
    //(...)
       if ( driver_receive(ANY, &msg, &ipc_status) != 0 ) { 
         printf("Error");
         continue;
     }
      if (is_ipc_notify(ipc_status)) {
         switch (_ENDPOINT_P(msg.m_source)) {
            case HARDWARE:
                if (msg.m_notify.interrupts & timer_mask){
                    timer_int_handler();
                    if (counter%60==0){
                        time_passed++;
                         }
                }              
                if (msg.m_notify.interrupts & keyboard_mask) { 
                   kbc_ih();
                   kbd_print_scancode(!(MAKE_CODE & scancode),scancode==TWO_BYTES?2:1,&scancode);
                   time_passed=0;
                   counter=0;
                }
                
              break;
            default:
              break;  
         }
     } else { 
     }
 
  }if (keyboard_unsubscribe_interrupts()) return 1;
  if (timer_unsubscribe_int()) return 1;
  return 0;
}
*/

#include <lcom/lcf.h>

#include <lcom/lab3.h>

#include <stdbool.h>
#include <stdint.h>
#include "i8042.h"
#include "keyboard.h"
#include "KBC.c"
#include "timer.c"
#include "i8254.h"


extern uint8_t scancode;
extern int counter;
//quando se cahamr o ficheiro keyboard.c vais conseguir ver a variavel scancode
int main(int argc, char *argv[]) {
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need it]
  lcf_trace_calls("/home/lcom/labs/lab3/trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  lcf_log_output("/home/lcom/labs/lab3/output.txt");

  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv))
    return 1;

  // LCF clean up tasks
  // [must be the last statement before return]
  lcf_cleanup();

  return 0;
}

int(kbd_test_scan)() {
  uint8_t keyboard_mask;
  int ipc_status;
  message msg;
  if (keyboard_subscribe_interrupts(&keyboard_mask)) return 1;
  while(scancode!=BREAK_ESC){
    //(...)
       if ( driver_receive(ANY, &msg, &ipc_status) != 0 ) { 
         printf("Error");
         continue;
     }
      if (is_ipc_notify(ipc_status)) { 
         switch (_ENDPOINT_P(msg.m_source)) {
            case HARDWARE:               
                if (msg.m_notify.interrupts & keyboard_mask) { 
                   kbc_ih();
                   kbd_print_scancode(!(MAKE_CODE & scancode),scancode==TWO_BYTES?2:1,&scancode);
                }
              break;
            default:
              break;  
         }
     } else { 
     }
 
  }if (keyboard_unsubscribe_interrupts()) return 1;

  return 0;
}

int(kbd_test_poll)() {
  while (scancode!=BREAK_ESC){
    if (read_KBC_output(0x60,&scancode)==0){
      kbd_print_scancode(!(MAKE_CODE & scancode),scancode==TWO_BYTES?2:1,&scancode);
    }
  }
  if (keyboard_restore()) return 1;
  return 0;
}

int(kbd_test_timed_scan)(uint8_t n) {
  message msg;
  uint8_t timer_mask,keyboard_mask;
  int time_passed=0;
  //inicializar a 0!!!!!!!!!!
  int ipc_status;
  if (timer_subscribe_int(&timer_mask)) return 1;
  if (keyboard_subscribe_interrupts(&keyboard_mask)) return 1;
  while(scancode!=BREAK_ESC && time_passed<n){
    //(...)
       if ( driver_receive(ANY, &msg, &ipc_status) != 0 ) { 
         printf("Error");
         continue;
     }
      if (is_ipc_notify(ipc_status)) { 
         switch (_ENDPOINT_P(msg.m_source)) {
            case HARDWARE:   
                if (msg.m_notify.interrupts & timer_mask) {
                  timer_int_handler();
                  if (counter%60==0){
                    time_passed++;
                  }
                }          
                if (msg.m_notify.interrupts & keyboard_mask) { 
                  kbc_ih();
                  kbd_print_scancode(!(MAKE_CODE & scancode),scancode==TWO_BYTES?2:1,&scancode);
                  counter=0;
                  time_passed=0;
                }
              break;
            default:
              break;  
         }
     } else { 
     }
 
  }
  if (timer_unsubscribe_int()) return 1;
  if (keyboard_unsubscribe_interrupts()) return 1;
  return 0;
}
