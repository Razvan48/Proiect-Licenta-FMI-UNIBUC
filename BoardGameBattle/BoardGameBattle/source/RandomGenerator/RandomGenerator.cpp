#include "RandomGenerator.h"

#include <random>

double RandomGenerator::randomUniformDouble(double minimum, double maximum)
{
    static std::random_device randomDevice;
    static std::mt19937 generator(randomDevice());
    std::uniform_real_distribution<double> distribution(minimum, maximum);
    return distribution(generator);
}

int RandomGenerator::randomUniformInt(int minimum, int maximum)
{
    static std::random_device randomDevice;
    static std::mt19937 generator(randomDevice());
    std::uniform_int_distribution<int> distribution(minimum, maximum);
    return distribution(generator);
}

unsigned long long RandomGenerator::randomUniformUnsignedLongLong(unsigned long long minimum, unsigned long long maximum)
{
	static std::random_device randomDevice;
	static std::mt19937 generator(randomDevice());
	std::uniform_int_distribution<unsigned long long> distribution(minimum, maximum);
	return distribution(generator);
}