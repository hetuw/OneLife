#include "hetuwmod.h"

#include "LivingLifePage.h"

int HetuwMod::viewWidth;
int HetuwMod::viewHeight;

float HetuwMod::zoomScale;
int HetuwMod::panelOffsetX;
int HetuwMod::panelOffsetY;

LivingLifePage *HetuwMod::livingLifePage;

void HetuwMod::init() {
	zoomScale = 1.5f;
	zoomCalc();
}

void HetuwMod::setLivingLifePage(LivingLifePage *inLivingLifePage) {
	livingLifePage = inLivingLifePage;
}

void HetuwMod::zoomCalc() {
	viewWidth = defaultViewWidth*zoomScale;
	viewHeight = defaultViewHeight*zoomScale;
	panelOffsetX = (int)(viewWidth - defaultViewWidth)/2;
	panelOffsetY = (int)(viewHeight - defaultViewHeight)/2;
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

// when return true -> end/return in keyDown function in LivingLife
bool HetuwMod::livingLifeKeyDown(unsigned char inASCII) {

	if (livingLifePage->hetuwSayFieldIsFocused()) {
		return false;
	}
	// player is not trying to say something

	// emotes
	int jic = (int)inASCII - 48;
	if (jic >= 0 && jic <= 9) {
		if (jic > 6) jic += 2;
		char message[64];
		sprintf( message, "EMOT 0 0 %i#", jic);
        livingLifePage->sendToServerSocket( message );
		return true;
	}

	return false;
}
