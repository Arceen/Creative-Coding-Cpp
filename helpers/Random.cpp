#pragma once
#include "Random.h"
#include <iostream>
#include <cstdlib>
#include <random>
using namespace std;

double Random::randRange(double min = 0, double max = 1.)
{
    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution distr(min, max);
    return distr(gen);
}