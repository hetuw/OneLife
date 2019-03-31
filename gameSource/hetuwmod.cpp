#include "hetuwmod.h"

float HetuwMod::zoomScale;
int HetuwMod::panelOffsetX;
int HetuwMod::panelOffsetY;

void HetuwMod::init() {
	zoomScale = 1.5f;
	zoomCalc();
}

void HetuwMod::zoomCalc() {
	panelOffsetX = (int)(defaultScreenWidth*zoomScale - defaultScreenWidth)/2;
	panelOffsetY = (int)(defaultScreenHeight*zoomScale - defaultScreenHeight)/2;
}

void HetuwMod::zoomIncrease() {
	zoomScale += 0.25f;
	if (zoomScale > 10.0f) zoomScale = 10.0f;
	zoomCalc();
}

void HetuwMod::zoomDecrease() {
	zoomScale -= 0.25f;
	if (zoomScale < 0.5f) zoomScale = 0.5f;
	zoomCalc();
}
