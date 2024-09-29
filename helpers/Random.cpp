#pragma once
#include "Random.h"
#include <iostream>
#include <cstdlib>
#include <random>
using namespace std;

double Random::randRangeDouble(double min = 0, double max = 1.)
{
    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution distr(min, max);
    return distr(gen);
}

int Random::randRangeInt(int min = 0, int max = 1)
{
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution distr(min, max);
    return distr(gen);
}
int Random::randRangeNormal(int min = 0, int max = 2)
{
    random_device rd;
    mt19937 gen(rd());
    std::vector<int> weights = {1, 3, 1};

    std::discrete_distribution<> distr(weights.begin(), weights.end());

    return distr(gen);
}