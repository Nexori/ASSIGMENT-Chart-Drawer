#include <iostream>
#include <math.h>
#include "Function.h"
Function::Function()
{
}
Function::~Function()
{
}
double Function::FXdisp(double x) {
	Xdisp = ((WINX / 2) + x) - (WINX - WINY) / 2;
	return Xdisp;
}
double Function::FYdisp(double y) {
	double Ydisp = ((WINY / 2) - y);
	return Ydisp;
}