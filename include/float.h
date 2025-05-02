#pragma once 

#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <math.h>

typedef struct {
    uint32_t sign : 1;
    uint32_t exp : 8;
    uint32_t mant : 23;
}float_components;

float sum_float (float, float); 

