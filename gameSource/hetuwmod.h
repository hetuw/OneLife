#ifndef HETUWMOD_H
#define HETUWMOD_H

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

	static void init();
	static void zoomIncrease();
	static void zoomDecrease();

private:
	static void zoomCalc();

};


#endif
