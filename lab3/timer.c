#include <lcom/lcf.h>
#include <lcom/timer.h>

#include <stdint.h>

#include "i8254.h"


int hook_id=0;
int counter=0;

int (timer_set_frequency)(uint8_t timer, uint32_t freq) {
  //define a frequencia no timer
  if (freq<19||freq>TIMER_FREQ) return 1;
  uint16_t counter=TIMER_FREQ/freq;
  uint8_t config;
  if (timer_get_conf(timer, &config)) return 1;
  config= config & 0x0f;
  config=config | BIT(5) | BIT(4);
  uint8_t MSB,LSB;
  util_get_LSB(counter,&LSB);
  util_get_MSB(counter,&MSB);
  if (sys_outb(0x43, config)) return 1;
  if (sys_outb(0x40+timer, LSB)) return 1;
  if (sys_outb(0X40+timer, MSB)) return 1;
  return 0;
}

int (timer_subscribe_int)(uint8_t *bit_no) {
    //diz ao CPU para indicar um bit na IRQ_LINE
    if (bit_no==NULL) return 1;
    //3ª argumento de 0 a 7
    *bit_no=BIT(hook_id);
    //hook_id é definido por nos (pode ou nao ser igual ao IRQ_LINE) para na cadeia de caracterees retornada pelo CPU vr se esse bit está a 1, ou seja; ver se ocorreu uma interrupcao desse dispositivo
    if (sys_irqsetpolicy(IRQ_LINE,IRQ_REENABLE,&hook_id)) return 1;
    //ORDEM É IMPORTANTE!!!
    return 0;

 
}

int (timer_unsubscribe_int)() {
  //função para dar unsubscribe ao interrupt do timer
  if (sys_irqrmpolicy(&hook_id)) return 1;
  return 0;
}

void (timer_int_handler)() {
   counter++;
}

int (timer_get_conf)(uint8_t timer, uint8_t *st) {
  //ver a configuração que tens no momento
  //st é configuração
  //timer é o n do timer
  if (st==NULL || timer>2||timer<0) return 1;
  uint8_t controlword=BIT(7) | BIT(6) | BIT(5) | BIT(timer+1);
  if (sys_outb(0x43, controlword)) return 1;
  if (util_sys_inb(0x40+timer,st)) return 1;
  return 0;

}

int (timer_display_conf)(uint8_t timer, uint8_t st,
                        enum timer_status_field field) {
  //dá print da configuração
 union timer_status_field_val temp;
  switch(field){
    case tsf_all:
      temp.byte=st;
      return timer_print_config(timer, field, temp);
    case tsf_initial:
      temp.in_mode = ((st & (BIT(4) | BIT(5)))>>4);
      return timer_print_config(timer, field, temp);
    case tsf_mode:
      temp.count_mode = ((st & (BIT(1) | BIT(2) | BIT(3)))>>1);
      return timer_print_config(timer, field, temp);
    case tsf_base:
      temp.bcd= (st& BIT(0));
      return timer_print_config(timer, field, temp);

  }
  return 0;
}

