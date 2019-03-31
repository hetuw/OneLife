#ifndef HETUWMOD_H
#define HETUWMOD_H

#include "LivingLifePage.h"

class HetuwMod
{

public:
	static const int defaultViewWidth = 1280;
	static const int defaultViewHeight = 720;

	static int viewWidth;
	static int viewHeight;

	static float zoomScale;
	static int panelOffsetX;
	static int panelOffsetY;

	static LivingLifePage *livingLifePage;

	static void init();

	static void setLivingLifePage(LivingLifePage *inLivingLifePage);

	static void zoomIncrease();
	static void zoomDecrease();

	static void livingLifeDraw();
	static bool livingLifeKeyDown(unsigned char inASCII);

private:
	static void zoomCalc();

};


#endif
