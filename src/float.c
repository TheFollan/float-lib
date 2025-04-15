#include "../include/float.h"

float sumFloat (float num1, float num2){

    floatComponents num1_comp, num2_comp;
    decomposeFloat (num1, &num1_comp);
    decomposeFloat (num2, &num2_comp);

//processing +0 and -0
//-------------------------------------------------------------------------
    if ((isPositiveZero(&num1_comp)) && (isNegativeZero(&num2_comp))){
        return num1; //+0
    }

    else if ((isPositiveZero(&num2_comp)) && (isNegativeZero(&num1_comp))){
        return num2; //+0
    }

    else if ((isPositiveZero(&num1_comp)) || (isNegativeZero(&num1_comp))){
        return num2; //unchanged
    }

    else if ((isPositiveZero(&num2_comp)) || (isNegativeZero(&num2_comp))){
        return num1; //unchanged
    }
//-------------------------------------------------------------------------
    
//processing +inf -inf 
//-----------------------------------------------------------------------------------
    if ((isPositiveInf(num1, &num1_comp)) && (isNegativeInf(num2, &num2_comp))){
        return NAN;
    }

    else if ((isPositiveInf(num1, &num1_comp)) && (isPositiveInf(num2, &num2_comp))){
        return num1; //+inf 
    }

    else if ((isNegativeInf(num1, &num1_comp)) && (isNegativeInf(num2, &num2_comp))){
        return num1; //-inf
    }

    else if ((isfinite(num1)) && (!isfinite(num2))){
        return num1; //+-inf
    }

    else if ((!isfinite(num1)) && (isfinite(num2))){
        return num2; //+-inf
    }
//-----------------------------------------------------------------------------------

//processing NaN
//-----------------------------------------------------------------------------------
    if ((isnan(num1)) || (isnan(num2))){
        return NAN;
    }
//-----------------------------------------------------------------------------------

//processing subnormal numbers
//-----------------------------------------------------------------------------------

}


uint32_t floatToBits (float f){
    uint32_t bits; 
    memcpy(&bits, &f, sizeof(f));
    return bits;
}

float bitsToFloat (uint32_t bits){
    float f;
    memcpy(&f, &bits, sizeof(bits));
    return f;
}

void decomposeFloat (float f, floatComponents* comp){
    uint32_t fbits = floatToBits(f);
    comp -> sign = (fbits >> 31) & 0x1;
    comp -> exponent = (fbits >> 23) & 0xFF; 
    comp -> mantissa = fbits & 0x7FFFFF;
}

float composeFloat (const floatComponents* comp){
    uint32_t bits = (comp -> sign >> 31) | (comp -> exponent >> 23) | (comp -> mantissa);
    return bitsToFloat(bits);
}

bool isPositiveZero (floatComponents* comp){
    return ((comp -> sign == 0) && (comp -> exponent == 0) && (comp -> mantissa == 0));
}

bool isNegativeZero (floatComponents* comp){
    return ((comp -> sign == 1) && (comp -> exponent == 0) && (comp -> mantissa == 0));
}

bool isPositiveInf (float num, floatComponents* comp){
    return ((comp -> sign == 0) && (isinf(num)));
}
bool isNegativeInf (float num, floatComponents* comp){
    return ((comp -> sign == 1) && (isinf(num)));

}
 



