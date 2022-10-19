#include "ccolorconverter.h"

CColorConverter::CColorConverter(QObject *parent) : QObject(parent)
{

}

void CColorConverter::yuyv_to_rgb888(unsigned char* yuv,unsigned char* rgb, int imgWidth, int imgHeight)
{
    unsigned int i;
    unsigned char* y0 = yuv + 0;
    unsigned char* u0 = yuv + 1;
    unsigned char* y1 = yuv + 2;
    unsigned char* v0 = yuv + 3;

    unsigned  char* r0 = rgb + 0;
    unsigned  char* g0 = rgb + 1;
    unsigned  char* b0 = rgb + 2;
    unsigned  char* r1 = rgb + 3;
    unsigned  char* g1 = rgb + 4;
    unsigned  char* b1 = rgb + 5;

    float rt0 = 0, gt0 = 0, bt0 = 0, rt1 = 0, gt1 = 0, bt1 = 0;

    for(i = 0; i <= (imgWidth * imgHeight) / 2 ;i++)
    {
        bt0 = 1.164 * (*y0 - 16) + 2.018 * (*u0 - 128);
        gt0 = 1.164 * (*y0 - 16) - 0.813 * (*v0 - 128) - 0.394 * (*u0 - 128);
        rt0 = 1.164 * (*y0 - 16) + 1.596 * (*v0 - 128);

        bt1 = 1.164 * (*y1 - 16) + 2.018 * (*u0 - 128);
        gt1 = 1.164 * (*y1 - 16) - 0.813 * (*v0 - 128) - 0.394 * (*u0 - 128);
        rt1 = 1.164 * (*y1 - 16) + 1.596 * (*v0 - 128);

        if(rt0 > 250)  	rt0 = 255;
        if(rt0 < 0)    	rt0 = 0;

        if(gt0 > 250) 	gt0 = 255;
        if(gt0 < 0)	gt0 = 0;

        if(bt0 > 250)	bt0 = 255;
        if(bt0 < 0)	bt0 = 0;

        if(rt1 > 250)	rt1 = 255;
        if(rt1 < 0)	rt1 = 0;

        if(gt1 > 250)	gt1 = 255;
        if(gt1 < 0)	gt1 = 0;

        if(bt1 > 250)	bt1 = 255;
        if(bt1 < 0)	bt1 = 0;

        *r0 = (unsigned char)bt0;
        *g0 = (unsigned char)gt0;
        *b0 = (unsigned char)rt0;

        *r1 = (unsigned char)bt1;
        *g1 = (unsigned char)gt1;
        *b1 = (unsigned char)rt1;

        yuv = yuv + 4;
        rgb = rgb + 6;
        if(yuv == NULL)
          break;

        y0 = yuv;
        u0 = yuv + 1;
        y1 = yuv + 2;
        v0 = yuv + 3;

        r0 = rgb + 0;
        g0 = rgb + 1;
        b0 = rgb + 2;
        r1 = rgb + 3;
        g1 = rgb + 4;
        b1 = rgb + 5;
    }
}
