#pragma once
#include <string>

class string;
class Vector3;

class SimpleLog
{
public:
	SimpleLog() = delete;
	SimpleLog(const SimpleLog&) = delete;
	SimpleLog& operator=(const SimpleLog&) = delete;

	static void logFloat(std::string name, float value);
	static void logVector(std::string name, const Vector3& vector);
};