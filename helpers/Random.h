#pragma once
class Random
{
public:
    Random() = delete;
    static double randRangeDouble(double min, double max);
    static int randRangeInt(int min, int max);
    static int randRangeNormal(int min, int max);
    static int randGaussian(int min, int max);
};