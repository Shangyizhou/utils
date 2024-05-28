#include "compare.h"
#include <stdexcept>
using std::runtime_error;

double compare::max(const double* data, int size)
{
    if (size <= 0) 
        throw runtime_error("The size <= 0");
    double result = data[0];
    for (int i = 1; i < size; i++) {
        if (result < data[i])
            result = data[i];
    }
    return result;
}


double compare::min(const double* data,int size)
{
	double result = data[0];
	for (int i = 1; i < size; i++)
		if (result > data[i])
			result = data[i];
		return result;
}
