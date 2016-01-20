#pragma once
#include "sfwdraw.h"


class Plotter
{
private:
	float x1, y1;

protected:
	float x, y;
	
	virtual void step(float t) = 0;

	Plotter() : color(MAGENTA), x1(0), y1(0), rangeStepSize(0.01f),
	x(0),y(0), rangeStart(0), rangeEnd(1)
	{ }

public:
	float rangeStart, rangeEnd, rangeStepSize;
	unsigned int color;
	void draw()
	{
		step(rangeStart);
		x1 = x;
		y1 = y;
		for (float i = rangeStart; i <= rangeEnd; i += rangeStepSize)
		{
			step(i+ rangeStepSize);
			sfw::drawLine(x, y, x1, y1, color);
			x1 = x;
			y1 = y;
		}
	}
};