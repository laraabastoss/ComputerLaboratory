// IMPORTANT: you must include the following line in all your C files
#include <lcom/lcf.h>

#include "graphics.h"
#include "keyboard.h"

// Any header files included below this line should have been created by you
extern vbe_mode_info_t vbe_mode_info;
extern uint8_t scancode;

int main(int argc, char *argv[]) {
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need it]
  lcf_trace_calls("/home/lcom/labs/lab5/trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  lcf_log_output("/home/lcom/labs/lab5/output.txt");

  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv))
    return 1;

  // LCF clean up tasks
  // [must be the last statement before return]
  lcf_cleanup();

  return 0;
}
/*
int(video_test_init)(uint16_t mode, uint8_t delay) {

  if (switch_to_graphic_mode(mode)) return 1;
  sleep(delay);
  if (vg_exit()) return 1;

  return 0;
}

int (exit_with_ESC)() {

  int ipc_status;
  message msg;
  uint8_t keyboard_mask;

  if (keyboard_subscribe_interrupts(&keyboard_mask) != 0) return 1;

  while (scancode != BREAK_ESC){
    if (driver_receive(ANY, &msg, &ipc_status) != 0) { 
      printf("driver_receive failed");
      continue;
    }
    if (is_ipc_notify(ipc_status)) {
      switch (_ENDPOINT_P(msg.m_source)) {
        case HARDWARE: 
          if (msg.m_notify.interrupts & keyboard_mask) 
            kbc_ih();
            break;
        default:
          break; 
      }
    }
  }

  if (keyboard_unsubscribe_interrupts() != 0) return 1;
  return 0;
}

int(video_test_rectangle)(uint16_t mode, uint16_t x, uint16_t y,
                          uint16_t width, uint16_t height, uint32_t color) {
  
if (set_frame_buffer(mode)) return 1; //construção do frame buffer virtual e físico
if (switch_to_graphic_mode(mode)) return 1; //mudaça para modo gráfico

uint32_t new_color;
if (normalize_color(color,&new_color)) return 1;
if (vg_draw_rectangle(x, y, width, height, new_color)) return 1;
if (exit_with_ESC()) return 1;
if (vg_exit() != 0) return 1;

return 0;
}

int(video_test_pattern)(uint16_t mode, uint8_t no_rectangles, uint32_t first, uint8_t step) {

  if (set_frame_buffer(mode)) return 1; //construção do frame buffer virtual e físico
  if (switch_to_graphic_mode(mode)) return 1; //mudaça para modo gráfico

  int height = vbe_mode_info.YResolution / no_rectangles;
  int width = vbe_mode_info.XResolution / no_rectangles;

  uint32_t color=0;
  for (int i=0;i<no_rectangles;i++){
    for (int j=0;j<no_rectangles;j++){
        if (vbe_mode_info.MemoryModel==0x06){
            uint32_t R = Red(j, step, first);
            uint32_t G = Green(i, step, first);
            uint32_t B = Blue(j, i, step, first);
            color = color_direct_mode(R, G, B);
        }

        else {
        color = color_indexed_mode(j, i, step, first, no_rectangles);
       }

        if (vg_draw_rectangle(j * width, i * height, width, height, color)) return 1;
    }
  }
  if (exit_with_ESC()) return 1;
  if (vg_exit() != 0) return 1;

  return 0;


}*/
int (exit_with_ESC)() {

  int ipc_status;
  message msg;
  uint8_t keyboard_mask;

  if (keyboard_subscribe_interrupts(&keyboard_mask) != 0) return 1;

  while (scancode != BREAK_ESC){
    if (driver_receive(ANY, &msg, &ipc_status) != 0) { 
      printf("driver_receive failed");
      continue;
    }
    if (is_ipc_notify(ipc_status)) {
      switch (_ENDPOINT_P(msg.m_source)) {
        case HARDWARE: 
          if (msg.m_notify.interrupts & keyboard_mask) 
            kbc_ih();
            break;
        default:
          break; 
      }
    }
  }

  if (keyboard_unsubscribe_interrupts() != 0) return 1;
  return 0;
}
int (video_test_init)(uint16_t mode, uint8_t delay){
  if (switch_to_graphic_mode(mode)) return 1;
  sleep(delay);
  vg_exit();
  return 0;
}

int (video_test_rectangle)(uint16_t mode, uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color){
if (set_frame_buffer(mode)) return 1; //construção do frame buffer virtual e físico
if (switch_to_graphic_mode(mode)) return 1; //mudaça para modo gráfico

uint32_t new_color;
if (normalize_color(color,&new_color)) return 1;
if (vg_draw_rectangle(x, y, width, height, new_color)) return 1;
if (exit_with_ESC()) return 1;
if (vg_exit() != 0) return 1;
return 0;

}

int(video_test_pattern)(uint16_t mode, uint8_t no_rectangles, uint32_t first, uint8_t step) {

  if (set_frame_buffer(mode)) return 1; //construção do frame buffer virtual e físico
  if (switch_to_graphic_mode(mode)) return 1; //mudaça para modo gráfico

  int height = vbe_mode_info.YResolution / no_rectangles;
  int width = vbe_mode_info.XResolution / no_rectangles;

  uint32_t color=0;
  for (int i=0;i<no_rectangles;i++){
    for (int j=0;j<no_rectangles;j++){
        if (vbe_mode_info.MemoryModel==0x06){
            uint32_t R = Red(j, step, first);
            uint32_t G = Green(i, step, first);
            uint32_t B = Blue(j, i, step, first);
            color = color_direct_mode(R, G, B);
        }

        else {
        color = color_indexed_mode(j, i, step, first, no_rectangles);
       }

        if (vg_draw_rectangle(j * width, i * height, width, height, color)) return 1;
    }
  }
  if (exit_with_ESC()) return 1;
  if (vg_exit() != 0) return 1;

  return 0;


}


int(video_test_xpm)(xpm_map_t xpm, uint16_t x, uint16_t y) {
  /* To be completed */
   if (set_frame_buffer(0x105 )) return 1;
  if (switch_to_graphic_mode(0x105 )) return 1;
  if (print_xpm(xpm,x,y)) return 1;
  if (exit_with_ESC()) return 1;
  if (vg_exit()) return 1;
  return 0;

}

int(video_test_move)(xpm_map_t xpm, uint16_t xi, uint16_t yi, uint16_t xf, uint16_t yf,
                     int16_t speed, uint8_t fr_rate) {
  /* To be completed */
  printf("%s(%8p, %u, %u, %u, %u, %d, %u): under construction\n",
         __func__, xpm, xi, yi, xf, yf, speed, fr_rate);

  return 1;
}

int(video_test_controller)() {
  /* To be completed */
  printf("%s(): under construction\n", __func__);

  return 1;
}

