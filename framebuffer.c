#include "ioc.h"
#include "mailbox_vc.h"
#include "terminal.h"

unsigned int width, height, pitch, isrgb;
unsigned char* framebuffer;

void fb_init()
{
	mbox[0] = 35 * 4;//length of message in bytes
	mbox[1] = MBOX_REQUEST;

	mbox[2] = MBOX_TAG_SETPHYWH;//tag identifyer
	mbox[3] = 8;//value size in bytes
	mbox[4] = 8;//value size in bytes again
	mbox[5] = 1920; //value width
	mbox[6] = 1080; //value height

	mbox[7] = MBOX_TAG_SETVIRTWH;
	mbox[8] = 8;
	mbox[9] = 8;
	mbox[10] = 1920;
	mbox[11] = 1080;

	mbox[12] = MBOX_TAG_SETVIRTOFF;
	mbox[13] = 8;
	mbox[14] = 8;
	mbox[15] = 0;//value x
	mbox[16] = 0;//value y

	mbox[17] = MBOX_TAG_SETDEPTH;
	mbox[18] = 4;
	mbox[19] = 4;
	mbox[20] = 32;//bits per pixel

	mbox[21] = MBOX_TAG_SETPXLORDR;
	mbox[22] = 4;
	mbox[23] = 4;
	mbox[24] = 1;//RGB

	mbox[25] = MBOX_TAG_GETFB;
	mbox[26] = 8;
	mbox[27] = 8;
	mbox[28] = 4096;//FRAMEBUFFERINFO.POINTER
	mbox[29] = 0;//FRAMEBUFFERINFO.SIZE

	mbox[30] = MBOX_TAG_GETPITCH;
	mbox[31] = 4;
	mbox[32] = 4;
	mbox[33] = 0; //bytes per line

	mbox[34] = MBOX_TAG_LAST;

	//check call is successufl and we have a pointer with depth 32
	if(mbox_call(MBOX_CH_PROP) && mbox[20] == 32 && mbox[28] != 0)
	{
		mbox[28] &= 0x3FFFFFFF; //convert GPU address to ARM address
		width = mbox[10];	//Actual physical width (?? not virtual??)
		height = mbox[11];	//Actual physical height (?? not virtual??)
		pitch = mbox[33]; 	//Number of bytes per line
		isrgb = mbox[24];	//pixel order
		framebuffer = (unsigned char*)((long)mbox[28]);
	}
}

void drawPixel(int x, int y, unsigned char attr)
{
    int offs = (y * pitch) + (x * 4);
    *((unsigned int*)(framebuffer + offs)) = vgapal[attr & 0x0f];
}

void drawRect(int x1, int y1, int x2, int y2, unsigned char attr, int fill)
{
    int y=y1;

    while (y <= y2) {
       int x=x1;
       while (x <= x2) {
	  if ((x == x1 || x == x2) || (y == y1 || y == y2)) drawPixel(x, y, attr);
	  else if (fill) drawPixel(x, y, (attr & 0xf0) >> 4);
          x++;
       }
       y++;
    }
}

void drawLine(int x1, int y1, int x2, int y2, unsigned char attr)
{
    int dx, dy, p, x, y;

    dx = x2-x1;
    dy = y2-y1;
    x = x1;
    y = y1;
    p = 2*dy-dx;

    while (x<x2) {
       if (p >= 0) {
          drawPixel(x,y,attr);
          y++;
          p = p+2*dy-2*dx;
       } else {
          drawPixel(x,y,attr);
          p = p+2*dy;
       }
       x++;
    }
}

void drawCircle(int x0, int y0, int radius, unsigned char attr, int fill)
{
    int x = radius;
    int y = 0;
    int err = 0;

    while (x >= y) {
	if (fill) {
	   drawLine(x0 - y, y0 + x, x0 + y, y0 + x, (attr & 0xf0) >> 4);
	   drawLine(x0 - x, y0 + y, x0 + x, y0 + y, (attr & 0xf0) >> 4);
	   drawLine(x0 - x, y0 - y, x0 + x, y0 - y, (attr & 0xf0) >> 4);
	   drawLine(x0 - y, y0 - x, x0 + y, y0 - x, (attr & 0xf0) >> 4);
	}
	drawPixel(x0 - y, y0 + x, attr);
	drawPixel(x0 + y, y0 + x, attr);
	drawPixel(x0 - x, y0 + y, attr);
        drawPixel(x0 + x, y0 + y, attr);
	drawPixel(x0 - x, y0 - y, attr);
	drawPixel(x0 + x, y0 - y, attr);
	drawPixel(x0 - y, y0 - x, attr);
	drawPixel(x0 + y, y0 - x, attr);

	if (err <= 0) {
	    y += 1;
	    err += 2*y + 1;
	}

	if (err > 0) {
	    x -= 1;
	    err -= 2*x + 1;
	}
    }
}

void drawChar(unsigned char ch, int x, int y, unsigned char attr, unsigned int zoom)
{
	unsigned char* glyph = (unsigned char* )&font + (ch < FONT_NUMGLYPHS ? ch : 0) * FONT_BPG;

	for (unsigned int i = 1; i < (FONT_HEIGHT*zoom); i++)
	{
		for (unsigned int j = 0; j < (FONT_WIDTH*zoom); j++)
		{
			unsigned char mask = 1 << j;
			unsigned char col = (*glyph & mask) ? attr & 0x0F : ((attr & 0xF0) >> 4);

			drawPixel(x+j, y+i, col);
		}
		glyph += (i%zoom) ? 0 : FONT_BPL;
	}
}

void drawString(int x, int y, char* string, unsigned char attr, unsigned int zoom)
{
	while(*string)
	{
		if(*string == '\r')
		{
			x = 0; //I don't quite understand the '\r' checks it doesnt do anything
		}
		else if(*string == '\n')
		{
			x = 0;
			y += FONT_HEIGHT;
		}
		else
		{
			drawChar(*string, x, y, attr, zoom);
			x += FONT_WIDTH;
		}
		string++;
	}
}

