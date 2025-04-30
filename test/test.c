#include <stdio.h>
#include <math.h>
#include <assert.h>

float sum_float(float a, float b);

int floats_equal(float a, float b) {
    if (isnan(a) && isnan(b)) return 1;
    if (isinf(a) || isinf(b)) return a == b;
    return fabsf(a - b) < 1e-6f;
}
void test_zero_cases(){
    assert(floats_equal(sum_float(0.0f, -0.0f), 0.0f));
    assert(floats_equal(sum_float(-0.0f, -0.0f), -0.0f));
    assert(floats_equal(sum_float(+0.0f, 5.0f), 5.0f));
    assert(floats_equal(sum_float(-0.0f, -3.5f), -3.5f));
}

void test_inf_cases() {
    assert(isnan(sum_float(INFINITY, -INFINITY)));
    assert(isnan(sum_float(-INFINITY, INFINITY)));
    assert(sum_float(INFINITY, INFINITY) == INFINITY);
    assert(sum_float(-INFINITY, -INFINITY) == -INFINITY);
    assert(sum_float(INFINITY, 1.0f) == INFINITY);
    assert(sum_float(-100.0f, -INFINITY) == -INFINITY);
}

void test_nan_cases() {

    float nan = NAN;
    
    assert(isnan(sum_float(nan, 1.0f)));
    assert(isnan(sum_float(5.0f, nan)));
    assert(isnan(sum_float(nan, nan)));
}

void test_normal_cases() {
    assert(floats_equal(sum_float(1.5f, 2.5f), 4.0f));
    assert(floats_equal(sum_float(-2.0f, 1.0f), -1.0f));
    assert(floats_equal(sum_float(123456.0f, 0.0f), 123456.0f));
    assert(floats_equal(sum_float(-1.0f, -1.0f), -2.0f));
}

void test_subnormal_case() {

    float sub = 1e-45f; 

    assert(floats_equal(sum_float(sub, sub), 2e-45f));
}

void test_overflow() {

    float maxn = 3.4028235e+38f;
    float big = 1e38f;

    assert(isinf(sum_float(maxn, maxn)) && sum_float(maxn, maxn) > 0);
    assert(isinf(sum_float(maxn, big)) && sum_float(maxn, big) > 0);
    assert(isinf(sum_float(-maxn, -big)) && sum_float(-maxn, -big) < 0);
}

void test_underflow(){

    float tiny = 1e-45f;       
    float smaller = 5e-46f;    

    assert(floats_equal(sum_float(tiny, tiny), 2e-45f));
    assert(floats_equal(sum_float(tiny, smaller), tiny));
    assert(floats_equal(sum_float(-tiny, -tiny), -2e-45f));

    float ultra_small = 1e-46f;
    float neg_ultra_small = -1e-46f;

    assert(floats_equal(sum_float(ultra_small, -ultra_small), 0.0f));
}


