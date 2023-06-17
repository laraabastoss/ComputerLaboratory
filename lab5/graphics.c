#include <lcom/lcf.h>
#include "graphics.h"
#include <math.h>
/*
int (switch_to_graphic_mode)(uint16_t mode){
   reg86_t reg86;
   memset(&reg86,0,sizeof(reg86));

   reg86.intno = 0x10;
   reg86.ah = 0x4F;
   reg86.al = 0x02;
   reg86.bx = mode | BIT(14);
   if (sys_int86(&reg86)) return 1;
   return 0;
}

int (set_frame_buffer)(uint16_t mode){

   memset(&vbe_mode_info, 0, sizeof(vbe_mode_info));
   if (vbe_get_mode_info(mode,&vbe_mode_info)) return 1;

   //tamanho do frame buffer em bytes
   unsigned int bytes_per_pixel, frame_buffer_size;
   bytes_per_pixel=(vbe_mode_info.BitsPerPixel+7)/8;
   frame_buffer_size=vbe_mode_info.XResolution * vbe_mode_info.YResolution * bytes_per_pixel;

   //preencher endereços físicos
   struct minix_mem_range physic_address;
  physic_address.mr_base = vbe_mode_info.PhysBasePtr; 
  physic_address.mr_limit = physic_address.mr_base + frame_buffer_size; 

  //alocação da memória física
  if (sys_privctl(SELF, SYS_PRIV_ADD_MEM, &physic_address)) return 1;

  //mapeamento para memória virtual e alocação desta no frame buffer
  frame_buffer=vm_map_phys(SELF, (void*) physic_address.mr_base, frame_buffer_size);
  if (frame_buffer==NULL) return 1;

   return 0;

}

int (normalize_color)(uint32_t color, uint32_t *new_color){
   if (vbe_mode_info.BitsPerPixel == 32) {
    *new_color = color;
  } 
  
  else {
    *new_color = color & (BIT(vbe_mode_info.BitsPerPixel) - 1); //todos os bits extra são colocados a zero
  }
  return 0;
}

int (vg_draw_pixel)(uint16_t x, uint16_t y, uint32_t color){
   if (x>vbe_mode_info.XResolution  || y > vbe_mode_info.YResolution) return 1;
   unsigned bytes_per_pixel = (vbe_mode_info.BitsPerPixel + 7) / 8;
   unsigned int index = (vbe_mode_info.XResolution * y + x) * bytes_per_pixel;
   if (memcpy(&frame_buffer[index], &color,bytes_per_pixel) == NULL) return 1;
   return 0;
}

int (vg_draw_hline)(uint16_t x, uint16_t y, uint16_t len, uint32_t color){
    for (unsigned i = 0 ; i < len ; i++){
      if (vg_draw_pixel(x+i, y, color)) return 1;
    }  
  return 0;
}

int (vg_draw_rectangle)(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color){
   for (unsigned i = 0 ; i <height ; i++){
      if (vg_draw_hline(x, y+i,width, color))  {
      vg_exit();
      return 1;
    }
   }  
  return 0;
}

int (print_xpm)(xpm_map_t xpm, uint16_t x, uint16_t y) {

  xpm_image_t img;

  // retorna um apontador para um array de cores preenchido de acordo com o XPM
  uint8_t *colors = xpm_load(xpm, XPM_INDEXED, &img);

  for (int h = 0 ; h < img.height ; h++) {
    for (int w = 0 ; w < img.width ; w++) {
      if (vg_draw_pixel(x + w, y + h, *colors) != 0) return 1;
      colors++; // avança para a próxima cor
    }
  }
  return 0;
}



*/
//criar um int de 32 bits com o hexadecimal da cor nos bits menos significativos
uint32_t (R)(uint32_t first){
  return ((1 << vbe_mode_info.RedMaskSize) - 1) & (first >> vbe_mode_info.RedFieldPosition);
}

uint32_t (G)(uint32_t first){
  return ((1 << vbe_mode_info.GreenMaskSize) - 1) & (first >> vbe_mode_info.GreenFieldPosition);
}

uint32_t (B)(uint32_t first){
  return ((1 << vbe_mode_info.BlueMaskSize) - 1) & (first >> vbe_mode_info.BlueFieldPosition);
}

uint32_t (Red)(unsigned j, uint8_t step, uint32_t first) {
  return (R(first) + j * step) % (1 << vbe_mode_info.RedMaskSize);
}

uint32_t (Green)(unsigned i, uint8_t step, uint32_t first) {
  return (G(first) + i * step) % (1 << vbe_mode_info.GreenMaskSize);
}

uint32_t (Blue)(unsigned j, unsigned i, uint8_t step, uint32_t first) {
  return (B(first) + (i + j) * step) % (1 << vbe_mode_info.BlueMaskSize);
}

uint32_t (color_direct_mode)(uint32_t R, uint32_t G, uint32_t B) {
  return (R << vbe_mode_info.RedFieldPosition) | (G << vbe_mode_info.GreenFieldPosition) | (B << vbe_mode_info.BlueFieldPosition);
}

uint32_t (color_indexed_mode)(uint16_t col, uint16_t row, uint8_t step, uint32_t first, uint8_t n) {
  return (first + (row * n + col) * step) % (1 << vbe_mode_info.BitsPerPixel);
}

/*

NOTAS:
memset->colocar reg86 e vbe a 0
memcpy->pintar o pixel
sys_int86->muda para modo gráfico
sys_privctl->alocar mem física
vm_map_phys->mapear para memória virtual*/

int (switch_to_graphic_mode)(uint16_t mode){
   reg86_t reg86;
   memset(&reg86,0,sizeof(reg86));

   reg86.intno = 0x10;
   reg86.ah = 0x4F;
   reg86.al = 0x02;
   reg86.bx = mode | BIT(14);
   if (sys_int86(&reg86)) return 1;
   return 0;
}
/*
int set_frame_buffer(uint16_t mode){
  //ir buscra a informação
  memset(&vbe_mode_info,0,sizeof(vbe_mode_info));
  if (vbe_get_mode_info(mode,&vbe_mode_info)) return 1;

  //calcular espaços
  unsigned int bytes_per_pixel = (vbe_mode_info.BitsPerPixel + 7)/8;
  unsigned int frame_size = vbe_mode_info.XResolution * vbe_mode_info.YResolution * bytes_per_pixel;

  //preencher memória física
  struct minix_mem_range mr;
  mr.mr_base = vbe_mode_info.PhysBasePtr;
  mr.mr_limit = mr.mr_base + frame_size;

  //alocar memória física
  if (sys_privctl(SELF, SYS_PRIV_ADD_MEM, &mr)) return 1;

  //mapear para memória virtual
  frame_buffer = vm_map_phys(SELF, (void *) mr.mr_base, frame_size);
  if (frame_buffer==NULL) return 1;
  return 0;
 
  
}*/
/**/
int (set_frame_buffer)(uint16_t mode){
    memset(&vbe_mode_info,0,sizeof(vbe_mode_info));
    if (vbe_get_mode_info(mode,&vbe_mode_info)) return 1;

    unsigned int bytes_per_pixel=(vbe_mode_info.BitsPerPixel +7)/8;
    unsigned int frame_size = vbe_mode_info.XResolution *vbe_mode_info.YResolution * bytes_per_pixel;

    struct minix_mem_range mr;
    mr.mr_base=   vbe_mode_info.PhysBasePtr;
    mr.mr_limit= mr.mr_base+frame_size;

    if (sys_privctl(SELF,SYS_PRIV_ADD_MEM,&mr)) return 1;

    frame_buffer = vm_map_phys(SELF,(void *) mr.mr_base,frame_size);
    if (frame_buffer==NULL) return 1;
    return 0;
  

}


int (normalize_color)(uint32_t color, uint32_t *new_color){
 if (vbe_mode_info.BitsPerPixel ==32){
  *new_color=color;
 }
 else{
  *new_color=color & ((BIT(vbe_mode_info.BitsPerPixel))-1);
 }
 return 0;
}
int (vg_draw_pixel)(uint16_t x, uint16_t y, uint32_t color){
  if (x>vbe_mode_info.XResolution || y>vbe_mode_info.YResolution){
    return 1;
  }
  unsigned int index = ((vbe_mode_info.XResolution *y)+x) *((vbe_mode_info.BitsPerPixel +7)/8);
  if (memcpy(&frame_buffer[index],&color, ((vbe_mode_info.BitsPerPixel +7)/8))==NULL) return 1;
  return 0;
}

int (vg_draw_hline)(uint16_t x, uint16_t y, uint16_t len, uint32_t color){
  for (int i=0;i<len;i++){
    if (vg_draw_pixel(x+i,y,color)) return 1;
  }
  return 0;
}

int (vg_draw_rectangle)(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color){
  for (int i=0;i<height;i++){
    if (vg_draw_hline(x,y+i,width,color)){
      vg_exit();
      return 1;
    }
  }
  return 0;
}
int (print_xpm)(xpm_map_t xpm, uint16_t x, uint16_t y) {

  xpm_image_t img;

  // retorna um apontador para um array de cores preenchido de acordo com o XPM
  uint8_t *colors = xpm_load(xpm, XPM_INDEXED, &img);

  for (int h = 0 ; h < img.height ; h++) {
    for (int w = 0 ; w < img.width ; w++) {
      if (vg_draw_pixel(x + w, y + h, *colors) != 0) return 1;
      colors++; // avança para a próxima cor
    }
  }
  return 0;
}

