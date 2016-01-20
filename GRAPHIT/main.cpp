
#include "sfwdraw.h"
#include "Plotter.h"
#include <cmath>

class Lerp : public Plotter
{
protected:
	void step(float t)
	{
		x = (xe - xs)*t + xs;
		y = (ye - ys)*t + ys;
	}
public:
	float xs, ys;
	float xe, ye;

	Lerp(float x, float y, float x1, float y1) : Plotter(),
		xs(x), ys(y), xe(x1), ye(y1)
	{ }
};

float lerp(float a, float b, float t)
{
	return (b - a)*t + a;
}
class QuadraticBezier : public Plotter
{
protected:
	void step(float t)
	{
		x = lerp(lerp(xs, xm, t), lerp(xe, xm, t), t);
		y = lerp(lerp(ys, ym, t), lerp(ye, ym, t), t);
	}
public:
	float xs, ys;
	float xe, ye;
	float xm, ym;

	QuadraticBezier(float x, float y, float x1, float y1, float x2, float y2) : Plotter(),
		xs(x), ys(y), xe(x1), ye(y1), xm(x2), ym(y2)
	{ }
};

class Hermite : public Plotter
{
protected:
	void step(float t)
	{
		float tsq = t * t;
		float tcub = tsq * t;

		float h00 = 2 * tcub - 3 * tsq + 1;
		float h01 = -2 * tcub + 3 * tsq;
		float h10 = tcub - 2 * tsq + t;
		float h11 = tcub - tsq;

		x = h00 * xP1 + h10 * xT1 + h01 * xP2 + h11 * xT2;		y = h00 * yP1 + h10 * yT1 + h01 * yT2 + h11 * yT2;
	}
public:
	float xP1, yP1;
	float xP2, yP2;
	float xT1, yT1;
	float xT2, yT2;

	Hermite(float x, float y, float x1, float y1, float x2, float y2, float x3, float y3) : Plotter(),
		xP1(x), yP1(y), xP2(x1), yP2(y1), xT1(x2), yT1(y2), yT2(x3), xT2(y3)
	{ }
};

class Quad : public Plotter {
protected:
	void step(float t) {
		x = t;
		y = t * t;
	}
public:
	Quad() {
		rangeStart = -1;
	}


};

void main()
{
	sfw::initContext(400, 400, "NSFW Draw");

	float range = 1;
	float step = .1f;
	float wing = step / 5;

	float r = range,
		t = range,
		l = -r,
		b = -t,
		f = 1,
		n = -1;

	float ortho[16] = { 2 / (r - l),       0,        0, -(r + l) / (r - l),
							 0, 2 / (t - b),        0, -(t + b) / (t - b),
							 0,       0, -2 / (f - n), -(f + n) / (f - n),
							 0,       0,        0,            1 };

	sfw::setProjectionMatrix(ortho);



	
	/////////////////////
	Lerp temp(.2,0,.4,1);
	Quad quad;
	QuadraticBezier bezier(0, 0, .1, .2, .3, -.4);
	Hermite her(-.1, -.1, .4, -.1, -.1, .1, -.3, .1);



	while (sfw::stepContext())
	{	

		//////////////////////////////////////////////////////
		//	Draw Grid
		#pragma region Grid
		for (float x = l; x <= r; x += step)
		{
			sfw::drawLine(x, b, x, t, 0x88888844);
			sfw::drawLine(x,-wing,x,wing,0x00000088);
		}

		for (float y = b; y <= t; y += step)
		{
			sfw::drawLine(l, y, r, y, 0x88888844);
			sfw::drawLine(-wing, y, wing, y, 0x00000088);
		}
		sfw::drawLine(0, t, 0, b, 0x000000cc);
		sfw::drawLine(l, 0, r, 0, 0x000000cc);
		#pragma endregion
		/////////////////////////////////////////////////////

		//temp.draw();
		//quad.draw();
		//bezier.draw();
		her.draw();
	}

	sfw::termContext();
}