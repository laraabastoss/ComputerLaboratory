/*#include <lcom/lcf.h>

#include <stdint.h>

int(util_get_LSB)(uint16_t val, uint8_t *lsb) {

  if (lsb==NULL) return 1;
  *lsb=val & 0XFF;
  return 0;

}

int(util_get_MSB)(uint16_t val, uint8_t *msb) {
   if (msb==NULL) return 1;
  *msb=val >>8;
  return 0;
}

int (util_sys_inb)(int port, uint8_t *value) {
  if (value==NULL) return 1;
  uint32_t value32;
  if (sys_inb(port,&value32)!=0) return 1;
  *value=value32 & 0xFF;
  return 0;
 
}
*/
#include <lcom/lcf.h>

#include <stdint.h>

int(util_get_LSB)(uint16_t val, uint8_t *lsb) {
  if (lsb==NULL) return 1;
  *lsb=val&0xff;
  return 0;
  /*NOTA: quando o argumento é*, para mudar o valor é preicos usar um '*' antes
  não é preciso passa-lo por referencia para outras funções*/
}

int(util_get_MSB)(uint16_t val, uint8_t *msb) {
  if (msb==NULL) return 1;
  *msb=val>>8;
  return 0;
}

int (util_sys_inb)(int port, uint8_t *value) {
   if (value==NULL) return 1; // o apontador deve ser válido
   uint32_t val32;   // variável auxiliar de 32 bits
   if (sys_inb(port, &val32)) return 1;// val ficará com o valor lido de port
    *value= val32 & 0xff; // value ficará apenas com os primeiros 8 bits do resultado
    return 0;
  
}
