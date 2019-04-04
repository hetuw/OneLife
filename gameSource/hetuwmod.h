#ifndef HETUWMOD_H
#define HETUWMOD_H

#include "LivingLifePage.h"

class HetuwMod
{

	class RainbowColor {
	public:
		float color[3];
		RainbowColor();
		void step();
	private:
		const float stepSize = 0.03f;
		bool increase;
		int cycle; // from 0 - 2 / which color should change
	};

public:
	static const int defaultViewWidth = 1280;
	static const int defaultViewHeight = 720;

	static int viewWidth;
	static int viewHeight;

	static float zoomScale;
	static int panelOffsetX;
	static int panelOffsetY;

	static RainbowColor *colorRainbow;

	static LivingLifePage *livingLifePage;

	static void init();

	static void setLivingLifePage(LivingLifePage *inLivingLifePage);

	static void zoomIncrease();
	static void zoomDecrease();

	static void livingLifeDraw();
	static bool livingLifeKeyDown(unsigned char inASCII);

private:
	static bool bDrawHelp;

	static void zoomCalc();

	static void drawHelp();

};


#endif
