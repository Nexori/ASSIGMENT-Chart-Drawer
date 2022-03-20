#pragma once

const int WINX = 1600;
const int WINY = 1000;


class Function
{
public:
	double x, y, Xdisp, Ydisp,Ydisp2;
	double FXdisp(double);
	double FYdisp(double);
	Function();
	~Function();
};

