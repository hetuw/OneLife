#ifndef HETUWMOD_H
#define HETUWMOD_H

class HetuwMod
{

public:
	static const int defaultScreenWidth = 1280;
	static const int defaultScreenHeight = 720;

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
