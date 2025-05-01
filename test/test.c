#include <stdio.h>
#include <math.h>
#include <assert.h>
#include "float.h"

float sum_float (float, float); 

int floats_equal(float a, float b) {
    if (isnan(a) && isnan(b)) return 1;
    if (isinf(a) || isinf(b)) return a == b;
    return fabsf(a - b) < 1e-6f;
}

void test_zero_cases(){
    if (!floats_equal(sum_float(0.0f, -0.0f), 0.0f)) {
        printf("Test failed: sum_float(0.0f, -0.0f) != 0.0f\n");
    }
    if (!floats_equal(sum_float(-0.0f, -0.0f), -0.0f)) {
        printf("Test failed: sum_float(-0.0f, -0.0f) != -0.0f\n");
    }
    if (!floats_equal(sum_float(+0.0f, 5.0f), 5.0f)) {
        printf("Test failed: sum_float(+0.0f, 5.0f) != 5.0f\n");
    }
    if (!floats_equal(sum_float(-0.0f, -3.5f), -3.5f)) {
        printf("Test failed: sum_float(-0.0f, -3.5f) != -3.5f\n");
    }
}

void test_inf_cases() {
    if (!isnan(sum_float(INFINITY, -INFINITY))) {
        printf("Test failed: sum_float(INFINITY, -INFINITY) != NAN\n");
    }
    if (!isnan(sum_float(-INFINITY, INFINITY))) {
        printf("Test failed: sum_float(-INFINITY, INFINITY) != NAN\n");
    }
    if (!(sum_float(INFINITY, INFINITY) == INFINITY)) {
        printf("Test failed: sum_float(INFINITY, INFINITY) != INFINITY\n");
    }
    if (!(sum_float(-INFINITY, -INFINITY) == -INFINITY)) {
        printf("Test failed: sum_float(-INFINITY, -INFINITY) != -INFINITY\n");
    }
    if (!(sum_float(INFINITY, 1.0f) == INFINITY)) {
        printf("Test failed: sum_float(INFINITY, 1.0f) != INFINITY\n");
    }
    if (!(sum_float(-100.0f, -INFINITY) == -INFINITY)) {
        printf("Test failed: sum_float(-100.0f, -INFINITY) != -INFINITY\n");
    }
}

void test_nan_cases() {
    float nan = NAN;

    if (!isnan(sum_float(nan, 1.0f))) {
        printf("Test failed: sum_float(NAN, 1.0f) != NAN\n");
    }
    if (!isnan(sum_float(5.0f, nan))) {
        printf("Test failed: sum_float(5.0f, NAN) != NAN\n");
    }
    if (!isnan(sum_float(nan, nan))) {
        printf("Test failed: sum_float(NAN, NAN) != NAN\n");
    }
}

void test_normal_cases() {
    if (!floats_equal(sum_float(1.5f, 2.5f), 4.0f)) {
        printf("Test failed: sum_float(1.5f, 2.5f) != 4.0f\n");
    }
    if (!floats_equal(sum_float(-2.0f, 1.0f), -1.0f)) {
        printf("Test failed: sum_float(-2.0f, 1.0f) != -1.0f\n");
    }
    if (!floats_equal(sum_float(123456.0f, 0.0f), 123456.0f)) {
        printf("Test failed: sum_float(123456.0f, 0.0f) != 123456.0f\n");
    }
    if (!floats_equal(sum_float(-1.0f, -1.0f), -2.0f)) {
        printf("Test failed: sum_float(-1.0f, -1.0f) != -2.0f\n");
    }
}

void test_subnormal_case() {
    float sub = 1e-45f; 

    if (!floats_equal(sum_float(sub, sub), 2e-45f)) {
        printf("Test failed: sum_float(1e-45f, 1e-45f) != 2e-45f\n");
    }
}

void test_overflow() {
    float maxn = 3.4028235e+38f;
    float big = 1e38f;

    if (!(isinf(sum_float(maxn, maxn)) && sum_float(maxn, maxn) > 0)) {
        printf("Test failed: sum_float(maxn, maxn) not INF\n");
    }
    if (!(isinf(sum_float(maxn, big)) && sum_float(maxn, big) > 0)) {
        printf("Test failed: sum_float(maxn, big) not INF\n");
    }
    if (!(isinf(sum_float(-maxn, -big)) && sum_float(-maxn, -big) < 0)) {
        printf("Test failed: sum_float(-maxn, -big) not INF\n");
    }
}

void test_underflow() {
    float tiny = 1.4e-45f; 
    float smaller = 1.17549435e-38f; 

    if (!floats_equal(sum_float(tiny, smaller), tiny)) {
        printf("Test failed: sum_float(tiny, smaller) != tiny\n");
    }

    float ultra_small = 1.4e-45f;  
    float neg_ultra_small = -1.4e-45f; 
    if (!floats_equal(sum_float(ultra_small, -ultra_small), 0.0f)) {
        printf("Test failed: sum_float(ultra_small, -ultra_small) != 0.0f\n");
    }
}

int main (){
    test_zero_cases();
    test_inf_cases();
    test_nan_cases();
    test_normal_cases();
    test_subnormal_case();
    test_underflow();
    test_overflow();

    printf("All tests passed \n");
    return 0;
}
