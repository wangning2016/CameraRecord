#ifndef COMDEF_H
#define COMDEF_H

#include <unistd.h>
#include <string>
#include <list>

#include <fcntl.h>
#include <errno.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <linux/videodev2.h>
#include <sys/mman.h>

#include <stdio.h>
#include <stdlib.h>
#include <alsa/asoundlib.h>

using namespace std;

extern "C"
{
#include "libavcodec/avcodec.h"
#include "libswresample/swresample.h"
#include "libavformat/avformat.h"
#include "libavcodec/avcodec.h"
#include "libavutil/time.h"
#include "libavutil/opt.h"
#include "libavutil/imgutils.h"
#include "libswscale/swscale.h"
}
#include <SDL2/SDL.h>

#include "comfunction.h"

//把内存对齐定义为2个字节，
//这个可以避免BITMAPFILEHEADER
//出现4字节的对齐而使bmp位图的头出错
#pragma pack(2)

//下面两个结构是位图的结构
typedef struct BITMAPFILEHEADER
{
    u_int16_t bfType;
    u_int32_t bfSize;
    u_int16_t bfReserved1;
    u_int16_t bfReserved2;
    u_int32_t bfOffBits;
}BITMAPFILEHEADER;

typedef struct BITMAPINFOHEADER
{
    u_int32_t biSize;
    u_int32_t biWidth;
    u_int32_t biHeight;
    u_int16_t biPlanes;
    u_int16_t biBitCount;
    u_int32_t biCompression;
    u_int32_t biSizeImage;
    u_int32_t biXPelsPerMeter;
    u_int32_t biYPelsPerMeter;
    u_int32_t biClrUsed;
    u_int32_t biClrImportant;
}BITMAPINFODEADER;















#endif // COMDEF_H
