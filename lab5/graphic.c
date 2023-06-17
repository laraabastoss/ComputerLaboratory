/*um ecrã é um array de bytes


transformação de uma coordenadas de área para uma coordenadas lineares com offset
uint32_t offset(x,y):
    (x+y*res)*ndebytesporpixel

    *frame + offset


MINIX->modo texto
    modo gráfico (transforma a janela normal que nós vemos numa janela de tamanho definido com um numero fixo de bytes por pixel)
    byte_per_pixeç=(mode.bpp+7)/8
    há uma fumção que passa de modo texto para modo gráfica que já é dada



 1-  descobrir resolução em x
    resolução em y
    número de pixels
    mode phys_addres (endereço físico para onde a placa gráfica está a apontar)

2- alocar memória

ppline(modo, x, y, comprimento,color):
    lpu_mode_nfp_t node_info;
    if(lpu_get_mode_info(&mode_info)) return 1; //arranjar as configurações

    size_t size=node_info.resX*mode_info.res_y
    uint8_t *frame_buffer;
    frame_buffer=video_map_phys(node_info.phys_addres, size)

 
   if(lpu_set_mode(mode)) return 1;
   vg.exit()

uint8_t *frame
draw_pixel(x,y,uint32_t color):
   uint offset=(xres*y+x)(bpp+1)/8
   memcpy(&frame[offset],&color,(bpp+7)/8)



size_t=xres-yres byte_per_pixel
var global:
lpv_mode_info mode_info
uint8_t bye_per_pixel
uint8_t* frame_buffer

mode [0,4]
pp_draw_line(x,y,len,mode,uint32_t color,dir){

primeiro dar fill às infromações do modo: lpv_get_mode_info(modo, &mode_info);

bytes_per_pixel =( mode_info.bpp + 7 ) / 8

size_t size = mode_info.xres * mode_info.yres * byte_per_pixel;

frame_buffer = video_mp_phys(mode_info.phys_adds , size);

if (frame_buffer == NULL) return 1;

if (lpv_set_mode(mode)) return 1;



}

int drawpixel(x,y,uint32_t color){

    int offset = ((y - mode_info_xres) + x) - byte_por_pixel

    if (memcopy(&frame_buffer[offset], &color, byte_per_pixel)) return 1;

   /* 5:6:5

   00000000 00000000 RRRRRGGG GGGBBBBB

   00000000 00RRRRRR 00GGGGGG 000BBBBB
}
 int normalize(uint32_t color, uint32_t newcolor){
    uint32_t red = (color & 0x1F0000) >> 6;
    uint32_t green = (color & 0x003F00) >> 3;
    uint32_t blue = (color & 0x100001F) ;
    *newcolor = red|green|blue;
    return 0;
 }

 XPM
 static char* [ ] -> xpm_map_t
*/

