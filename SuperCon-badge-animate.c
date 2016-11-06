/* 
 * File:   SuperCon-badge-animate.c
 * Author: szczys@hotmail.com
 *
 * User code should be placed in this file. Follow the examples below:
 *   Write to display through Buffer[] and call displayLatch() to show changes on badge
 *   use getTime() for upcounting (approximate) milliseconds for non-blocking delays
 *   call getControl() to return a key mask of debounced button presses
 *   call pollAccel() to populate accelerometer data and read AccX(high/low), AccY(high/low), AccZ(high/low)
 *
 * MIT License (see license.txt)
 * Copyright (c) 2016 Hackaday.com
 */

#include "HaD_Badge.h"

const uint16_t image_target[] = {
    0b0000001111000000,
    0b0000010000100000,
    0b0000100000010000,
    0b0001000000001000,
    0b0010000000000100,
    0b0100000000000010,
    0b1000011111100001,
    0b1000010000100001,
    0b1000010000100001,
    0b1000011111100001,
    0b0100000000000010,
    0b0010000000000100,
    0b0001000000001000,
    0b0000100000010000,
    0b0000010000100000,
    0b0000001111000000,
};
const uint16_t image_x[] = {
    0b1100000000000001,
    0b0110000000000011,
    0b0011000000000110,
    0b0001100000001100,
    0b0000110000011000,
    0b0000011000110000,
    0b0000001101100000,
    0b0000000111000000,
    0b0000000111000000,
    0b0000001101100000,
    0b0000011000110000,
    0b0000110000011000,
    0b0001100000001100,
    0b0011000000000110,
    0b0110000000000011,
    0b1100000000000001,
};

const uint16_t image_had[] = {
0xdffb
,0xeff7
,0x4ff2
,0x07e0
,0xe427
,0xf00f
,0xf00f
,0xf24f
,0xf66f
,0xf00f
,0xf81f
,0xe007
,0x0420
,0x4ff2
,0xeff7
,0xdffb
};

const uint8_t font_8x5[] = {
0x00,0x40,0x40,0x40,0x40,0x00,0x40,0x00,0x00,0x50,0x50,0x00,0x00,0x00,0x00,0x00,0x00,0x50,0xF8,0x50,0x50,0xF8,0x50,0x00,0x70,0xA8,0xA0,0x70,0x28,0xA8,0x70,0x20,0x00,0x10,0x90,0x20,
0x20,0x48,0x40,0x00,0x40,0xA0,0xA0,0x40,0xA8,0x90,0x68,0x00,0x00,0x10,0x30,0x20,0x00,0x00,0x00,0x00,0x30,0x60,0x60,0x60,0x60,0x60,0x30,0x00,0x60,0x30,0x30,0x30,0x30,0x30,0x60,0x00,
0x20,0xA8,0x70,0xF8,0x70,0xA8,0x20,0x00,0x00,0x20,0x20,0xF8,0x20,0x20,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x20,0x20,0x40,0x00,0x00,0x00,0x78,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x20,0x00,0x00,0x10,0x10,0x20,0x20,0x40,0x40,0x00,0x70,0x88,0x98,0xA8,0xC8,0x88,0x70,0x00,0x10,0x30,0x10,0x10,0x10,0x10,0x38,0x00,0x70,0x88,0x08,0x10,0x20,0x40,0xF8,0x00,
0x70,0x88,0x08,0x30,0x08,0x88,0x70,0x00,0x10,0x30,0x50,0x90,0xF8,0x10,0x10,0x00,0xF8,0x80,0x80,0xF0,0x08,0x08,0xF0,0x00,0x70,0x88,0x80,0xF0,0x88,0x88,0x70,0x00,0xF8,0x08,0x08,0x10,
0x10,0x10,0x10,0x00,0x70,0x88,0x88,0x70,0x88,0x88,0x70,0x00,0x70,0x88,0x88,0x78,0x08,0x08,0x10,0x00,0x00,0x00,0x20,0x00,0x00,0x20,0x00,0x00,0x00,0x00,0x20,0x00,0x20,0x20,0x40,0x00,
0x08,0x10,0x20,0x40,0x20,0x10,0x08,0x00,0x00,0x00,0x78,0x00,0x78,0x00,0x00,0x00,0x40,0x20,0x10,0x08,0x10,0x20,0x40,0x00,0x30,0x48,0x08,0x10,0x10,0x00,0x10,0x00,0x70,0x88,0xB8,0xB8,
0xB8,0x80,0x70,0x00,0x70,0x88,0x88,0xF8,0x88,0x88,0x88,0x00,0xF0,0x88,0x88,0xF0,0x88,0x88,0xF0,0x00,0x70,0x88,0x80,0x80,0x80,0x88,0x70,0x00,0xF0,0x88,0x88,0x88,0x88,0x88,0xF0,0x00,
0xF8,0x80,0x80,0xE0,0x80,0x80,0xF8,0x00,0xF8,0x80,0x80,0xE0,0x80,0x80,0x80,0x00,0x70,0x88,0x80,0x80,0xB8,0x88,0x70,0x00,0x88,0x88,0x88,0xF8,0x88,0x88,0x88,0x00,0xF8,0x20,0x20,0x20,
0x20,0x20,0xF8,0x00,0x78,0x10,0x10,0x10,0x10,0x90,0x60,0x00,0x88,0x90,0xA0,0xC0,0xA0,0x90,0x88,0x00,0x80,0x80,0x80,0x80,0x80,0x80,0xF8,0x00,0x88,0xD8,0xA8,0x88,0x88,0x88,0x88,0x00,
0x88,0xC8,0xC8,0xA8,0x98,0x98,0x88,0x00,0x70,0x88,0x88,0x88,0x88,0x88,0x70,0x00,0xF0,0x88,0x88,0xF0,0x80,0x80,0x80,0x00,0x70,0x88,0x88,0x88,0xA8,0x90,0x68,0x00,0xF0,0x88,0x88,0xF0,
0xA0,0x90,0x88,0x00,0x70,0x88,0x80,0x70,0x08,0x88,0x70,0x00,0xF8,0x20,0x20,0x20,0x20,0x20,0x20,0x00,0x88,0x88,0x88,0x88,0x88,0x88,0x70,0x00,0x88,0x88,0x88,0x50,0x50,0x20,0x20,0x00,
0x88,0x88,0x88,0xA8,0xA8,0xA8,0x50,0x00,0x88,0x88,0x50,0x20,0x50,0x88,0x88,0x00,0x88,0x50,0x20,0x20,0x20,0x20,0x20,0x00,0xF8,0x08,0x10,0x20,0x20,0x40,0xF8,0x00,0x38,0x20,0x20,0x20,
0x20,0x20,0x20,0x38,0x80,0x40,0x40,0x20,0x20,0x10,0x10,0x08,0xE0,0x20,0x20,0x20,0x20,0x20,0x20,0xE0,0x00,0x20,0x50,0x88,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFC,
0x00,0x40,0x20,0x10,0x00,0x00,0x00,0x00,0x00,0x00,0x60,0x90,0x90,0x90,0x68,0x00,0x00,0x80,0x80,0xE0,0x90,0x90,0x60,0x00,0x00,0x00,0x60,0x80,0x80,0x80,0x60,0x00,0x00,0x10,0x10,0x70,
0x90,0x90,0x60,0x00,0x00,0x00,0x70,0x88,0xF0,0x80,0x70,0x00,0x20,0x50,0x40,0xE0,0x40,0x40,0x40,0x00,0x00,0x30,0x48,0x30,0x08,0x10,0x28,0x10,0x00,0x40,0x40,0x70,0x48,0x48,0x48,0x00,
0x00,0x20,0x00,0x20,0x20,0x20,0x20,0x00,0x00,0x10,0x00,0x10,0x10,0x10,0x90,0x60,0x00,0x40,0x40,0x50,0x60,0x50,0x48,0x00,0x00,0x60,0x20,0x20,0x20,0x20,0x70,0x00,0x00,0x00,0x00,0x50,
0xA8,0xA8,0xA8,0x00,0x00,0x00,0x00,0x20,0x50,0x50,0x50,0x00,0x00,0x00,0x00,0x30,0x48,0x48,0x30,0x00,0x00,0x00,0x30,0x48,0x48,0x70,0x40,0x40,0x00,0x00,0x30,0x48,0x48,0x70,0xC0,0x40,
0x00,0x00,0x58,0x60,0x40,0x40,0x40,0x00,0x00,0x00,0x30,0x40,0x30,0x08,0x70,0x00,0x00,0x20,0x70,0x20,0x20,0x20,0x10,0x00,0x00,0x00,0x00,0x48,0x48,0x48,0x30,0x00,0x00,0x00,0x00,0x50,
0x50,0x50,0x20,0x00,0x00,0x00,0x00,0x88,0x88,0xA8,0x50,0x00,0x00,0x00,0x50,0x50,0x20,0x50,0x50,0x00,0x00,0x00,0x50,0x50,0x30,0x10,0x50,0x20,0x00,0x00,0x70,0x10,0x20,0x40,0x70,0x00,
};
const uint8_t font_5x8[] = {
0x00,0x7A,0x00,0x00,0x00,0x00,0x60,0x00,0x60,0x00,0x24,0x7E,0x24,0x7E,0x24,0x64,0x92,0xFF,0x92,0x4C,0x20,0x06,0x18,0x60,0x04,0x6C,0x92,0x6A,0x04,0x0A,0x00,0x00,0x30,0x60,0x00,0x00,
0x00,0x00,0x7C,0x82,0x00,0x82,0x7C,0x00,0x00,0x54,0x38,0xFE,0x38,0x54,0x10,0x10,0x7C,0x10,0x10,0x00,0x01,0x06,0x00,0x00,0x00,0x10,0x10,0x10,0x10,0x00,0x00,0x02,0x00,0x00,0x00,0x06,
0x18,0x60,0x00,0x7C,0x8A,0x92,0xA2,0x7C,0x00,0x00,0x42,0xFE,0x02,0x42,0x86,0x8A,0x92,0x62,0x44,0x82,0x92,0x92,0x6C,0x18,0x28,0x48,0xFE,0x08,0xF2,0x92,0x92,0x92,0x8C,0x7C,0x92,0x92,
0x92,0x4C,0x80,0x80,0x80,0x9E,0xE0,0x6C,0x92,0x92,0x92,0x6C,0x60,0x90,0x90,0x92,0x7C,0x00,0x00,0x24,0x00,0x00,0x00,0x02,0x2C,0x00,0x00,0x00,0x10,0x28,0x44,0x82,0x00,0x28,0x28,0x28,
0x28,0x00,0x82,0x44,0x28,0x10,0x00,0x40,0x80,0x9A,0x60,0x7C,0x82,0xBA,0xBA,0x78,0x7E,0x90,0x90,0x90,0x7E,0xFE,0x92,0x92,0x92,0x6C,0x7C,0x82,0x82,0x82,0x44,0xFE,0x82,0x82,0x82,0x7C,
0xFE,0x92,0x92,0x82,0x82,0xFE,0x90,0x90,0x80,0x80,0x7C,0x82,0x8A,0x8A,0x4C,0xFE,0x10,0x10,0x10,0xFE,0x82,0x82,0xFE,0x82,0x82,0x04,0x82,0x82,0xFC,0x80,0xFE,0x10,0x28,0x44,0x82,0xFE,
0x02,0x02,0x02,0x02,0xFE,0x40,0x20,0x40,0xFE,0xFE,0x60,0x10,0x0C,0xFE,0x7C,0x82,0x82,0x82,0x7C,0xFE,0x90,0x90,0x90,0x60,0x7C,0x82,0x8A,0x84,0x7A,0xFE,0x90,0x98,0x94,0x62,0x64,0x92,
0x92,0x92,0x4C,0x80,0x80,0xFE,0x80,0x80,0xFC,0x02,0x02,0x02,0xFC,0xE0,0x18,0x06,0x18,0xE0,0xFC,0x02,0x1C,0x02,0xFC,0xC6,0x28,0x10,0x28,0xC6,0x80,0x40,0x3E,0x40,0x80,0x82,0x86,0x9A,
0xA2,0xC2,0x00,0x00,0x00,0xFF,0x81,0x80,0x60,0x18,0x06,0x01,0x81,0x81,0xFF,0x00,0x00,0x10,0x20,0x40,0x20,0x10,0x01,0x01,0x01,0x01,0x01,0x00,0x40,0x20,0x10,0x00,0x1C,0x22,0x22,0x1C,
0x02,0x7C,0x12,0x12,0x0C,0x00,0x1C,0x22,0x22,0x00,0x00,0x0C,0x12,0x12,0x7C,0x00,0x1C,0x2A,0x2A,0x2A,0x10,0x10,0x7E,0x90,0x40,0x00,0x00,0x20,0x52,0x55,0x2A,0x00,0x7E,0x10,0x10,0x0E,
0x00,0x00,0x5E,0x00,0x00,0x02,0x01,0x01,0x5E,0x00,0x00,0x7E,0x08,0x14,0x02,0x00,0x42,0x7E,0x02,0x00,0x0E,0x10,0x0E,0x10,0x0E,0x00,0x0E,0x10,0x0E,0x00,0x00,0x0C,0x12,0x12,0x0C,0x00,
0x1F,0x24,0x24,0x18,0x02,0x1F,0x24,0x24,0x18,0x00,0x3E,0x10,0x20,0x20,0x00,0x12,0x2A,0x2A,0x04,0x00,0x20,0x7C,0x22,0x00,0x00,0x1C,0x02,0x02,0x1C,0x00,0x1C,0x02,0x1C,0x00,0x1C,0x02,
0x04,0x02,0x1C,0x00,0x36,0x08,0x36,0x00,0x00,0x32,0x09,0x3E,0x00,0x00,0x26,0x2A,0x32,0x00
};

uint16_t* image_current = image_x;

uint8_t ballX = 4;
uint8_t ballY = 0;

//Hint: look in HaD_Badge.h for function and constant definitions

void eraseBall() {
    //uses global variables to erase current ball
    displayPixel(ballX, ballY, OFF);
    displayLatch();
}

void moveLeft() {
    if (ballX > 0) {
        //only move if we're not already at the edge
        eraseBall();
        --ballX;
        displayPixel(ballX, ballY, ON);
        displayLatch();
    }
}

void moveRight() {
    if (ballX < TOTPIXELX-1) {
        //only move if we're not already at the edge
        eraseBall();
        ++ballX;
        displayPixel(ballX, ballY, ON);
        displayLatch();
    }
}

void moveUp() {
    if (ballY > 0) {
        //only move if we're not already at the edge
        eraseBall();
        --ballY;
        displayPixel(ballX, ballY, ON);
        displayLatch();
    }
}

void moveDown() {
    //Limit ball travel to top 8 rows of the screen
    if (ballY < TOTPIXELY-1) {
        //only move if we're not already at the edge
        eraseBall();
        ++ballY;
        displayPixel(ballX, ballY, ON);
        displayLatch();
    }
}

void drawArrow(uint8_t rightOrLeft) {
    //0 == right
    //1 == left
    if (rightOrLeft) {
        Buffer[11] = 0b00100000;
        Buffer[12] = 0b01000000;
        Buffer[13] = 0b11111111;
        Buffer[14] = 0b01000000;
        Buffer[15] = 0b00100000;
    }
    else {
        Buffer[11] = 0b00000100;
        Buffer[12] = 0b00000010;
        Buffer[13] = 0b11111111;
        Buffer[14] = 0b00000010;
        Buffer[15] = 0b00000100;
    }
    displayLatch();
}

void drawNothing() {
    //if(index < 0) index = 0;
    //if(index > 15) index = 15;
    for(int i = 0; i <= 15; i++){
        Buffer[i] = 0x00;
    }
}

inline void scroll_down() {
    // Scroll display
    for(uint8_t i = TOTPIXELY - 1; i > 0; i--) {
        Buffer[i] = Buffer[i-1];
    }
}

inline void scroll_up() {
    // Scroll display
    for(uint8_t i = 0; i < TOTPIXELY - 1; i++) {
        Buffer[i] = Buffer[i+1];
    }
}

inline void rope() {
    if ((int8_t)AccXhigh > 0) { moveLeft(); } //Use high 8-bits of X value to decide what to do.
    else if ((int8_t)AccXhigh < 0) { moveRight(); } //Use high 8-bits of X value to decide what to do.
    //if ((int8_t)AccYhigh > 0) { moveUp(); } //Use high 8-bits of X value to decide what to do.
    //else if ((int8_t)AccYhigh < 0) { moveDown(); }
    scroll_down();
}

void drawImage(const uint16_t *image, int8_t index) {
    index &= 0x0f;
    for(int i = 0; i <= 15; i++){
        if(index > 0)
            Buffer[i] = (uint8_t)(image[i] >> index) & 0xff;
        else
            Buffer[i] = (uint8_t)(image[i] >> index) & 0xff;
    }
}

inline void image() {
    static int8_t f = 0;
    //f += (int8_t)AccXhigh;
    //if( f < 0) f = -1;
    //if( f >= 32) f = 32;

    if ((int8_t)AccXhigh > 0) { f--; if(f<0)f=0;} //Use high 8-bits of X value to decide what to do.
    else if ((int8_t)AccXhigh < 0) { f++; if(f>8)f=8;} //Use high 8-bits of X value to decide what to do.

    //f++;
    //if(f > 15) f = 0;

    if( f >= 0 && f <=15) drawImage(image_current, f);
    else drawNothing();
    //Buffer[0] = f;
}

void draw(uint8_t index) {
    if(index < ' ') return;
    if(index > 'z') return;
    if( index == ' ') {
        Buffer[0] = 0x00;
        Buffer[1] = 0x00;
        Buffer[2] = 0x00;
        Buffer[3] = 0x00;
        Buffer[4] = 0x00;
        Buffer[5] = 0x00;
        Buffer[6] = 0x00;
        Buffer[7] = 0x00;
        return;
    }
    
    index = index - '!';
/*
    Buffer[0] = font_5x8[(index * 5) + 4];
    Buffer[1] = font_5x8[(index * 5) + 3];
    Buffer[2] = font_5x8[(index * 5) + 2];
    Buffer[3] = font_5x8[(index * 5) + 1];
    Buffer[4] = font_5x8[(index * 5) + 0];
    Buffer[5] = 0x00;
 */
    Buffer[0] = font_8x5[(index * 8) + 0];
    Buffer[1] = font_8x5[(index * 8) + 1];
    Buffer[2] = font_8x5[(index * 8) + 2];
    Buffer[3] = font_8x5[(index * 8) + 3];
    Buffer[4] = font_8x5[(index * 8) + 4];
    Buffer[5] = font_8x5[(index * 8) + 5];
    Buffer[6] = font_8x5[(index * 8) + 6];
    Buffer[7] = font_8x5[(index * 8) + 7];
}

void draw_line(uint8_t index, uint32_t line) {
    if(index < ' ') return;
    if(index > 'z') return;
    if( index == ' ') {
        Buffer[15] = 0x00;
        return;
    }
    index = index - '!';
    Buffer[15] = font_8x5[(index * 8) + (line % 8)];
}

void animateBadge(void) {
    displayPixel(ballX, ballY, ON);
    displayLatch();
    uint32_t nextTime = getTime();
    uint16_t count = 0;
    int8_t task = 0;
    const uint8_t task_max = 4;
    uint16_t next = 50;
    char str[] = "Hackaday Superconference 2016";
    //char str[] = "abcdefghijklmnopqrstuvwxyz";
    
    while(1) {
        
        //This shows how to use non-blocking getTime() function
        if (getTime() > nextTime) {
            
            nextTime = getTime()+next;  //prepare next event for about 1000ms (1 second) from now
            
            //Buffer[8] ^= 0xFF;  //Toggle a whole row
            //Buffer[8] = count++;  //Toggle a whole row
            //Buffer[9] ^= 0xFF;  //Toggle a whole row

            //Use accelerometer to draw left or right arrow for X axis
            pollAccel();    //Tell kernel to read the accelerometer values

            if(task < 0) task = task_max;
            if(task > task_max) task = 0;
            switch(task){
                case 0:
                    next = 50;
                    rope();
                    break;
                case 1:
                    next = 50;
                    image();
                    break;
                case 2:
                    next = 100;
                    Buffer[15] = font_8x5[count];
                    count = (count + 1) % sizeof(font_8x5);
                    scroll_up();
                    break;
                case 3:
                    next = 1000;
                    draw(str[count]);
                    count = (count + 1) % (sizeof(str)-1);
                    //draw(count);
                    //count = (count + 1) % 90;
                    Buffer[15] = count;
                    break;
                case 4:
                    next = 100;
                    draw_line(str[count >> 3], count);
                    count = (count + 1) % ((sizeof(str) - 1) * 8);
                    scroll_up();
                    break;
            }            
            displayLatch();

            //Buffer[0] = AccXhigh;
            //Buffer[1] = AccYhigh;
            //Buffer[2] = AccZhigh;
        // AccXhigh and AccXlow (rotate left is lower, right higher)
        // AccYhigh and AccYlow (nose down is lower, up is higher)
        // AccZhigh and AccZlow (face down is lower, up is higher)
        }
#ifdef GONE
        //Use accelerometer to draw left or right arrow for X axis
        pollAccel();    //Tell kernel to read the accelerometer values
        if (AccXhigh < 0xF0) { drawArrow(1); } //Use high 8-bits of X value to decide what to do.
        else { drawArrow(0); }
#endif
        
        //This shows how to get user input
        switch (getControl()) {
            case (ESCAPE):
                displayClose();
                return;
            case (LEFT):
                //moveLeft();
                image_current = image_had;
                break;
            case (RIGHT):
                //moveRight();
                image_current = image_target;
                break;
            case (UP):
                //moveUp();
                drawNothing();
                count = 0;
                task++;
                break;
            case (DOWN):
                //moveDown();
                drawNothing();
                count = 0;
                task--;
                break;
        }
    }
    
}