void SNES::capture_screenshot() {
  flag_output_screenshot = true;
}

//used to convert pixel data to write to rgb555 format
//bitmap image via SNES::output_screenshot() function
uint16 SNES::to_rgb555(uint32 color) {
  if(video.depth == 15) {
  //rgb555
    return color & 0x7fff;
  }

  if(video.depth == 16) {
  //rgb565->rgb555
    return ((color >> 1) & 0x7fe0) | (color & 0x001f);
  }

  if(video.depth == 24 || video.depth == 32) {
  //rgb888->rgb555
    return ((color >> 9) & 0x7c00) | ((color >> 6) & 0x03e0) | ((color >> 3) & 0x001f);
  }

//unsupported color depth
  return color;
}

void SNES::output_screenshot() {
FILE *fp;
char fn[256];
int i;
//get a file name that doesn't exit...
  for(i=0;i<=999;i++) {
    sprintf(fn, "image%0.3d.bmp", i);
    fp = fopen(fn, "rb");
    if(!fp)break;
    fclose(fp);
    fp = 0;
  }
  if(i >= 1000)return;

  fp = fopen(fn, "wb");
  if(!fp)return;

video_info vi;
  get_video_info(&vi);
int width, height;
  width  = vi.width;
  height = (vi.height == 224) ? 223 : 446;
int32 fsize;
  fsize = 0x36 + (width * height * sizeof(uint16));

//header
  fputc('B', fp);
  fputc('M', fp);
//file size
  fputc(fsize,       fp);
  fputc(fsize >>  8, fp);
  fputc(fsize >> 16, fp);
  fputc(fsize >> 24, fp);
//???
  fputc(0, fp);
  fputc(0, fp);
  fputc(0, fp);
  fputc(0, fp);
//start of data
  fputc(0x36, fp);
  fputc(0x00, fp);
  fputc(0x00, fp);
  fputc(0x00, fp);
//remaining header size
  fputc(0x28, fp);
  fputc(0x00, fp);
  fputc(0x00, fp);
  fputc(0x00, fp);
//width
  fputc(width,       fp);
  fputc(width >>  8, fp);
  fputc(width >> 16, fp);
  fputc(width >> 24, fp);
//height
  fputc(height,       fp);
  fputc(height >>  8, fp);
  fputc(height >> 16, fp);
  fputc(height >> 24, fp);
//format
  fputc(0x01, fp);
  fputc(0x00, fp);
//bpp
  fputc(16, fp);
  fputc( 0, fp);
//???
  for(i=0;i<24;i++) { fputc(0, fp); }

int x, y;
uint16 c;
  for(y=height;y>=1;y--) {
    for(x=0;x<width;x++) {
      c = to_rgb555(video.data[y * width + x]);
      fputc(c,      fp);
      fputc(c >> 8, fp);
    }
  }

  fclose(fp);
  fp = 0;
}