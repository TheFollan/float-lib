#pragma once 

#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <math.h>

typedef struct {
    uint32_t sign : 1;
    uint32_t exponent : 8;
    uint32_t mantissa : 23;
}floatComponents;

uint32_t floatToBits (float);
float bitsToFloat (uint32_t); 
void decomposeFloat (float, floatComponents*);
float composeFloat (const floatComponents*);
float sumFloat (float, float); 
bool isPositivZero (floatComponents*);
bool isNegativeZero (floatComponents*);
bool isPositiveInf (float, floatComponents*);
bool isNegativeInf (float, floatComponents*);


