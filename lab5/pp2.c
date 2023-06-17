// IMPORTANT: you must include the following line in all your C files
#include <lcom/lcf.h>
#include <stdint.h>
#include <stdio.h>
​
int main(int argc, char *argv[]) {
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");
​
  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need it]
  lcf_trace_calls("/home/lcom/labs/lab5/trace.txt");
​
  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  lcf_log_output("/home/lcom/labs/lab5/output.txt");
​
  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv))
    return 1;
​
  // LCF clean up tasks
  // [must be the last statement before return]
  lcf_cleanup();
​
  return 0;
}
​

​#define BIT(n) 1<<(n);
​
enum lpv_dir_t{
    lpv_hor,
    lpv_vert
};
​
typedef struct {
    uint16_t x_res; // horizontal resolution
    uint16_t y_res; // vertical resolution
    uint8_t bpp;    // bits per pixel
    uint8_t r_sz;   // red component size
    uint8_t r_pos;  // red component LSB position
    uint8_t g_sz;   // green component size
    uint8_t g_pos;  // green component LSB position
    uint8_t b_sz;   // blue component size
    uint8_t b_pos;  // blue component LSB position
    phys_addr_t pysh_addr; // video ram base phys address
} lpv_mode_info_t;
/*
lpv_mode_info_t mode_info;
uint8_t* frame_buffer;

int (set_frame_buffer)(uint8_t mode){
    memset(&mode_info, 0, sizeof(mode_info));
    if (lpv_get_mode_info(mode, & mode_info)) return 1;

    size_t frame_size = mode_info.x_res* mode_info.y_res * ((mode_info.bpp+7)/8);


    frame_buffer = vm_map_phys(mode_info.pysh_addr, frame_size);
    if (frame_buffer==NULL) return 1;

}

int (draw_pixel)(uint16_t x, uint16_t y,uint32_t color){
    if (x> mode_info.x_res || y>mode_info.y_res){
        return 1;
    }
    unsigned int index=(mode_info.x_res*y+x)*((mode_info.bpp+7)/8);
    if (memcpy(&frame_buffer[index], &color,((mode_info.bpp+7)/8) )==NULL){
        return 1;
    }
    return 0;
}

int (normalize_color)(uint32_t color, uint32_t *newcolor){
    uint32_t red= (color & 0x1F0000) >> 5;
    uint32_t green= (color & 0x003F00) >> 3;
    uint32_t blue= (color & 0x00001F);

    *newcolor = red | green | blue;

    return 0;

}
​

​
int (pp_test_line)(uint8_t mode, enum lpv_dir_t dir, uint16_t x, uint16_t y, uint16_t len, uint32_t color, uint32_t delay){

    if (set_frame_buffer(mode)) return 1;

    if (lpv_set_mode(mode)) return 1;

    if (mode==0|| mode>4){
        return 1;
    }
    uint32_t newcolor;
    if (mode==3){
        normalize_color(color, &newcolor);
    }
    else{
        &newcolor=color;
    }

    switch (dir){
        case lpv_hor: 
            for (int i=0;i<len;i++){
                if (draw_pixel( x+i,  y, newcolor)){
                    break;
                }
            }
        case lpv_vert:
            for (int i=0;i<len;i++){
                if (draw_pixel( x, y+1, newcolor)){
                    break;
                }
            }
        default:
            break;

    }

    sleep(delay);
    return vg_exit();

}*/
//1-declarar variáveis globais 
lpv_mode_info_t mode_info;
uint8_t* frame_buffer;

//2-função set frame buffer
int (set_frame_buffer)(uint8_t mode){
    memset(&mode_info,0,sizeof(mode_info));
    if (lpv_get_mode_info(mode,&mode_info)) return 1;

    size_t frame_size = mode_info.x_res * mode_info.y_res * ((mode_info.bpp+7)/8);
    frame_buffer=video_map_phys(mode_info.phys_addr, frame_size);
    if (frame_buffer==NULL) return 1;
    return 0;
}

//4-normalizar a cor
int (normalize_color)(uint32_t color,uint32_t* new_color ){
    uint32_t red = (color & 0x1f0000) >>5;
    uint32_t green = (color & 0x003f00) >>3;
    uint32_t blue = (color & 0x00001f);
    *new_color=red|green|blue;
    return 0;
}
int(vg_draw_pixel)(uint16_t x, uint16_t y, uint32_tcolor){
    if (x>mode_info.x_res || y>mode_info.y_res) return 0; // não dá erro, só sai
    unsigned int index =((mode_info.x_res * y )+ x) * ((mode_info+7)/8);
    if (memcpy(&frame_buffer[index],&color,  ((mode_info.bpp+7)/8))==NULL) return 1;
    return 0;
}
int (pp_test_line)(uint8_t mode, enum lpv_dir_t dir, uint16_t x, uint16_t y, uint16_t len, uint32_t color, uint32_t delay){

 if (set_frame_buffer(mode)) return 1;
 //3-mudar o modo
 if (mode<0 || mode>4){
    return 1;
 }
 
 if (lpv_set_mode(mode)) return 1;
 //4
 uint32_t new_color;
 if (mode==3){

   if (normalize_color(color,&new_color)) return 1;
 }
 if (mode==4){
     new_color = color & ((BIT(mode_info.bpp))-1);
 }
 else{
    new_color=color;
 }

switch (dir)
{
case lpv_hor:
    for (int i=0;i<len;i++){
        if (vg_draw_pixel(x+i,y,new_color)) return 1;
    }
    break;
case lpv_ver:
    for (int i=0;i<len;i++){
        if (vg_draw_pixel(x,y+i,new_color)) return 1;
    }
    break;
default:
    break;
}
sleep(delay);
if (lpv_set_mode(0)) return 1;
}

/*
RESUMO:
1-variaveis globais : mode_info  e frame_buffer (uint8_t *)
2-fazer função set_frame_buffer e chama-la :
2.1-fazer o memset para 0
2.2- it buscar o mode_info
2.3- descorbrir bytes_per_pixel e frame_size (size_t)
2.4-vm_map_phys() para mapear memoria virtual
3-dar set ao modo gráfico -> funçao que eles dão
4-fazer funcçao de normalizar a cor para os casos necessários
5-de acordo com a a direção da linha desenha-la
NOTA: saber se é para dar erro ou so nao desenhar
6-sleep os segundos que forem precisos
7-dar set ao modo 0




RATO:
1-subscrever interrupções 
2-ler a control word se for preciso presevar mas sempre escrever
3-loop
4-dentro do if:
mouse_ih()->colocar cada leitura nos bytes
colocar no packet
dar display do packet
5-dessubscrever
6-escrever comando para deixar de ler dados
*/

