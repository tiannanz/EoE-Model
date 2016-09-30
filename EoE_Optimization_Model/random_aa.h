#pragma once
#include "stdafx.h"

double uniform_rng();
double uniform_rng(double lower, double upper);
double normal_rng(double mean, double standard_deviation); 
int uniform_int_rng(int lower, int upper);
bool check_small_double_equality(double a, double b); 