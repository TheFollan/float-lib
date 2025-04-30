#include "../include/float.h"

float sum_float (float num1, float num2){

    float_components num1_comp, num2_comp;
    decompose_float (num1, &num1_comp);
    decompose_float (num2, &num2_comp);

// processing +0 and -0
    if ((is_positive_zero(&num1_comp)) && (is_negative_zero(&num2_comp))) return num1; // +0

    if ((is_negative_zero(&num1_comp)) && (is_positive_zero(&num2_comp))) return num2; //+0
    
    if ((is_positive_zero(&num1_comp)) || (is_negative_zero(&num1_comp))) return num2; //unchanged
    
    if ((is_positive_zero(&num2_comp)) || (is_negative_zero(&num2_comp))) return num1; //unchanged
    
//processing +inf -inf 
    if ((is_positive_inf(num1, &num1_comp)) && (is_negative_inf(num2, &num2_comp))) return NAN;

    if ((is_positive_inf(num1, &num1_comp)) && (is_positive_inf(num2, &num2_comp))) return num1; //+inf 

    if ((is_negative_inf(num1, &num1_comp)) && (is_negative_inf(num2, &num2_comp))) return num1; //-inf

    if ((isfinite(num1)) && (!isfinite(num2))) return num1; //+-inf

    if ((!isfinite(num1)) && (isfinite(num2))) return num2; //+-inf

//processing NaN
    if ((isnan(num1)) || (isnan(num2))) return NAN;


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
    else{
        bool num1_is_larger = (num1_mant > num2_mant);
        sum_mant = num1_is_larger ? (num1_mant - num2_mant) : (num2_mant - num1_mant);
        result_sign = num1_is_larger ? num1_comp.sign : num2_comp.sign;

        if (num1_mant == num2_mant) return bits_to_float(0);
    }

    int32_t result_exp = num1_exp;

    normalization_result(&sum_mant, &result_exp);

    if (result_exp > 127){
        float_components inf = { .sign = result_sign, .exp = 0xFF, .mant = 0};
        return compose_float(&inf); 
    }
    if (result_exp < -126){
        int shift = -126 - result_exp;
        sum_mant >>= shift;
        result_exp = -126;
    }

    float_components result = {
        .sign = result_sign,
        .exp = (uint32_t)(result_exp + 127),
        .mant = (uint32_t)(sum_mant & 0x7FFFFF)
    };
    return compose_float(&result);

}


static uint32_t float_to_bits (float f){
    uint32_t bits; 
    memcpy(&bits, &f, sizeof(f));
    return bits;
}

static float bits_to_float (uint32_t bits){
    float f;
    memcpy(&f, &bits, sizeof(bits));
    return f;
}

static void decompose_float (float f, float_components* comp){
    uint32_t fbits = float_to_bits(f);
    comp -> sign = (fbits >> 31) & 0x1;
    comp -> exp = (fbits >> 23) & 0xFF; 
    comp -> mant = fbits & 0x7FFFFF;
}

static float compose_float (const float_components* comp){
    uint32_t bits = (comp -> sign << 31) | (comp -> exp << 23) | (comp -> mant);
    return bits_to_float(bits);
}

static bool is_positive_zero (float_components* comp){
    return ((comp -> sign == 0) && (comp -> exp == 0) && (comp -> mant == 0));
}

static bool is_negative_zero (float_components* comp){
    return ((comp -> sign == 1) && (comp -> exp == 0) && (comp -> mant == 0));
}

static bool is_positive_inf (float num, float_components* comp){
    return ((comp -> sign == 0) && (isinf(num)));
}

static bool is_negative_inf (float num, float_components* comp){
    return ((comp -> sign == 1) && (isinf(num)));
}

static bool is_subnormal (float_components* comp){
    return ((comp -> exp == 0) && (comp -> mant != 0));
}

static int32_t get_real_exponent(float_components* comp){
    return (comp -> exp ? comp -> exp - 127 : -126);
}

static uint64_t get_mantissa(float_components* comp){
    return (is_subnormal(comp) ? comp -> mant : comp -> mant | 0x800000);
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

static void normalization_result (uint64_t* mant, int32_t* exp){
    while (*mant > 0xFFFFFF){
        *mant >>= 1;
        (*exp)++;
    }

    while ((*mant != 0) && ((*mant & 0x800000) == 0)){
        *mant <<= 1;
        (*exp)--;
    }
}  

