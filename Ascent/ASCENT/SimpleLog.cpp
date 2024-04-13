#include "SimpleLog.h"

#include <iostream>
#include "Vector3.h"

using std::cout;
using std::endl;

void SimpleLog::logFloat(std::string name, float value)
{
	cout << name << ": " << value << endl;
}

void SimpleLog::logVector(std::string name, const Vector3& vector)
{
	cout << name << ": " << vector.x << " " << vector.y << " " << vector.z << endl;
}
