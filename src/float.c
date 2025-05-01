#include "../include/float.h"

float sum_float(float num1, float num2){

    float_components num1_comp, num2_comp;
    decompose_float(num1, &num1_comp);
    decompose_float(num2, &num2_comp);

    float special_cases_result;
    if (handle_special_cases(
        num1, num2,
        &num1_comp, &num2_comp,
        &special_cases_result)){

            return special_cases_result;
        }

    int32_t num1_exp = get_real_exponent(&num1_comp);
    int32_t num2_exp = get_real_exponent(&num2_comp);
    uint64_t num1_mant = get_mantissa(&num1_comp);
    uint64_t num2_mant = get_mantissa(&num2_comp);

    alignment_exp(&num1_exp, &num1_mant, &num2_exp, &num2_mant);
    
    uint64_t sum_mant;
    uint32_t result_sign;

    if (num1_comp.sign == num2_comp.sign){
        sum_mant = num1_mant + num2_mant;
        result_sign = num1_comp.sign;
    }
    else if (num1_mant == num2_mant){
        return bits_to_float(0);
    }
    else{
        bool num1_is_larger = (num1_mant > num2_mant);
        sum_mant = num1_is_larger ? (num1_mant - num2_mant) : (num2_mant - num1_mant);
        result_sign = num1_is_larger ? num1_comp.sign : num2_comp.sign;

    }

    return normalize_and_compose(result_sign, num1_exp, sum_mant);
}


static bool handle_special_cases(
    float num1, float num2,
    const float_components* num1_comp,
    const float_components* num2_comp,
    float* result_out)
{   
        // +0 + (-0) => +0
        if (((is_positive_zero(num1_comp)) && (is_negative_zero(num2_comp))) || 
            ((is_negative_zero(num1_comp)) && (is_positive_zero(num2_comp)))){
                *result_out = bits_to_float(0);
                return true;
            }

        // (+-0) + number => number
        if ((is_positive_zero(num1_comp)) || (is_negative_zero(num1_comp))){
            *result_out = num2;
            return true;
        }

        // number + (+-0) => number
        if ((is_positive_zero(num2_comp)) || (is_negative_zero(num2_comp))){
            *result_out = num1;
            return true;
        }

        // number + NaN || NaN + number => NaN
        if ((isnan(num1)) || (isnan(num2))){
            *result_out = NAN;
            return true;
        }

        // +inf + (-inf) => NaN
        if ((is_positive_inf(num1, num1_comp)) && (is_negative_inf(num2, num2_comp))){
            *result_out = NAN;
            return true;
        }

        // -inf + (+inf) => NaN
        if ((is_negative_inf(num1, num1_comp)) && (is_positive_inf(num2, num2_comp))){
            *result_out = NAN;
            return true;
        }

        // +inf + (+inf) => +inf
        if ((is_positive_inf(num1, num1_comp)) && (is_positive_inf(num2, num2_comp))){
            *result_out = num1;
            return true;
        }

        // -inf + (-inf) => -inf
        if ((is_negative_inf(num1, num1_comp)) && (is_negative_inf(num2, num2_comp))){
            *result_out = num1;
            return true;
        }

        // (+-inf) + number => inf
        if (isinf(num1)){
            *result_out = num1;
            return true;
        }
      
        // number + (+-inf) => inf
        if (isinf(num2)){
            *result_out = num2;
            return true;
        }

        // no special cases
        return false;
} 


static float normalize_and_compose(uint32_t sign, int32_t exp, uint64_t mant){

    normalization_result(&mant, &exp);

    // overflow exp => inf 
    if (exp > 127){
        float_components inf = { .sign = sign, .exp = 0xFF, .mant = 0};
        return compose_float(&inf); 
    }

    // denormalization 
    if (exp < -126){
        int shift = -126 - exp;
        mant >>= shift;
        exp = -126;
    }

    float_components result = {
        .sign = sign,
        .exp = (uint32_t)(exp + 127),
        .mant = (uint32_t)(mant & 0x7FFFFF)
    };

    return compose_float(&result);
}


static uint32_t float_to_bits(float f){
    uint32_t bits; 
    memcpy(&bits, &f, sizeof(f));
    return bits;
}

static float bits_to_float(uint32_t bits){
    float f;
    memcpy(&f, &bits, sizeof(bits));
    return f;
}

static void decompose_float(float f, float_components* comp){
    uint32_t fbits = float_to_bits(f);
    comp->sign = (fbits >> 31) & 0x1;
    comp->exp = (fbits >> 23) & 0xFF; 
    comp->mant = fbits & 0x7FFFFF;
}

static float compose_float(const float_components* comp){
    uint32_t bits = (comp->sign << 31) | (comp->exp << 23) | (comp->mant);
    return bits_to_float(bits);
}

static bool is_positive_zero(const float_components* comp){
    return ((comp->sign == 0) && (comp->exp == 0) && (comp->mant == 0));
}

static bool is_negative_zero(const float_components* comp){
    return ((comp->sign == 1) && (comp->exp == 0) && (comp->mant == 0));
}

static bool is_positive_inf(float num, const float_components* comp){
    return ((comp->sign == 0) && (isinf(num)));
}

static bool is_negative_inf(float num, const float_components* comp){
    return ((comp->sign == 1) && (isinf(num)));
}

static bool is_subnormal(const float_components* comp){
    return ((comp->exp == 0) && (comp->mant != 0));
}

static int32_t get_real_exponent(const float_components* comp){
    return (comp->exp ? comp->exp - 127 : -126);
}

static uint64_t get_mantissa(const float_components* comp){
    return (is_subnormal(comp) ? comp->mant : comp->mant | 0x800000);
}
 
static void alignment_exp(int32_t* a_exp, uint64_t* a_mant, int32_t* b_exp, uint64_t* b_mant){
    int32_t exp_diff = *a_exp - *b_exp;
    if (exp_diff > 0) {
        *b_mant >>= exp_diff;
        *b_exp = *a_exp;
    } else if (exp_diff < 0) {
        *a_mant >>= -exp_diff;
        *a_exp = *b_exp;
    }
}

static void normalization_result(uint64_t* mant, int32_t* exp){
    while (*mant > 0xFFFFFF){
        *mant >>= 1;
        (*exp)++;
    }

    while ((*mant != 0) && ((*mant & 0x800000) == 0)){
        *mant <<= 1;
        (*exp)--;
    }
}  

