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

static uint32_t float_to_bits (float);
static float bits_to_float (uint32_t); 
static void decompose_float (float, float_components*);
static float compose_float (const float_components*);
static bool is_positive_zero (const float_components*);
static bool is_negative_zero (const float_components*);
static bool is_positive_inf (float, const float_components*);
static bool is_negative_inf (float, const float_components*);
static bool is_subnormal (const float_components*);
static int32_t get_real_exponent (const float_components*);
static uint64_t get_mantissa(const float_components*);
static void alignment_exp (int32_t*, uint64_t*, int32_t*, uint64_t*);
static void normalization_result(uint64_t*, int32_t*);
static bool handle_special_cases(float, float, const float_components*, const float_components*, float*);
static float normalize_and_compose(uint32_t, int32_t, uint64_t);