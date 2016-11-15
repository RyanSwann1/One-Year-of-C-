#pragma once
#include <string>
#include <random>

namespace Utilities
{
	inline int randomNumberGenerator(const int min, const int max)
	{

		static std::random_device rd;
		static std::mt19937 gen(rd());
		std::uniform_int_distribution<> dis(min, max);

		return dis(gen);
	}

	inline std::string& getWorkingDirectory() 
	{
		return std::string("Resources");
	}
}