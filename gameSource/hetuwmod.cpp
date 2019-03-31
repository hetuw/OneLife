#include "hetuwmod.h"

#include "LivingLifePage.h"
#include "objectBank.h"

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

void HetuwMod::livingLifeDraw() {

 	LiveObject *ourLiveObject = livingLifePage->getOurLiveObject();

	// cords 
	setDrawColor( 0, 0, 0, 1 );
	doublePair jDrawPos = livingLifePage->hetuwGetLastScreenViewCenter();
	jDrawPos.x -= HetuwMod::viewWidth/2 - 40;
	jDrawPos.y += HetuwMod::viewHeight/2 - 40;
	char sBuf[64];
	sprintf(sBuf, "%d %d", (int)ourLiveObject->currentPos.x , (int)ourLiveObject->currentPos.y);
	int jWidthLimit = 250;
	double jFade = 1.0;
	livingLifePage->drawChalkBackgroundString( jDrawPos, sBuf, jFade, jWidthLimit );

	// age
	sprintf(sBuf, "%d", (int)(ourLiveObject->age));
	jDrawPos = livingLifePage->hetuwGetLastScreenViewCenter();
	jDrawPos.x += 330;
	jDrawPos.y -= HetuwMod::viewHeight/2 - 25;
	livingLifePage->hetuwDrawWithHandwritingFont( sBuf, jDrawPos );
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
