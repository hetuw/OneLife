#include "hetuwmod.h"

#include "LivingLifePage.h"
#include "objectBank.h"
#include "emotion.h"
#include "minorGems/util/SimpleVector.h"
#include "minorGems/game/drawUtils.h"

int HetuwMod::viewWidth;
int HetuwMod::viewHeight;

float HetuwMod::zoomScale;
int HetuwMod::panelOffsetX;
int HetuwMod::panelOffsetY;

HetuwMod::RainbowColor *HetuwMod::colorRainbow;

LivingLifePage *HetuwMod::livingLifePage;

bool HetuwMod::bDrawHelp;

void HetuwMod::init() {
	zoomScale = 1.5f;
	zoomCalc();
	
	colorRainbow = new RainbowColor();

	bDrawHelp = false;
}

void HetuwMod::setLivingLifePage(LivingLifePage *inLivingLifePage) {
	livingLifePage = inLivingLifePage;
}

HetuwMod::RainbowColor::RainbowColor() {
	color[0] = 1.0f;
	color[1] = 0.0f;
	color[2] = 0.0f;
	increase = true;
	cycle = 1;
}

void HetuwMod::RainbowColor::step() {
	bool nextCycle = false;
	if (increase) {
		color[cycle] += stepSize;
		if (color[cycle] >= 1.0f) {
			color[cycle] = 1.0f;
			nextCycle = true;
		}
	} else {
		color[cycle] -= stepSize;
		if (color[cycle] <= 0.0f) {
			color[cycle] = 0.0f;
			nextCycle = true;
		}
	}
	if (nextCycle) {
		increase = !increase;
		cycle--;
		if (cycle < 0) cycle = 2;
	}
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

	colorRainbow->step();

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

	if (bDrawHelp) drawHelp();
}

void HetuwMod::useTileRelativeToMe( int x, int y ) {
 	LiveObject *ourLiveObject = livingLifePage->getOurLiveObject();
	char msg[32];
	x += ourLiveObject->currentPos.x;
	y += ourLiveObject->currentPos.y;
	x = livingLifePage->sendX(x);
	y = livingLifePage->sendY(y);
	sprintf( msg, "USE %d %d", x, y);
	livingLifePage->hetuwSetNextActionMessage( msg );
}

void HetuwMod::dropTileRelativeToMe( int x, int y ) {
 	LiveObject *ourLiveObject = livingLifePage->getOurLiveObject();
	char msg[32];
	x += ourLiveObject->currentPos.x;
	y += ourLiveObject->currentPos.y;
	x = livingLifePage->sendX(x);
	y = livingLifePage->sendY(y);
	sprintf( msg, "USE %d %d", x, y);
	sprintf( msg, "DROP %d %d -1#", x, y);
	livingLifePage->hetuwSetNextActionMessage( msg );
}

void HetuwMod::remvTileRelativeToMe( int x, int y ) {
	char msg[32];
 	LiveObject *ourLiveObject = livingLifePage->getOurLiveObject();
	x += ourLiveObject->currentPos.x;
	y += ourLiveObject->currentPos.y;
	x = livingLifePage->sendX(x);
	y = livingLifePage->sendY(y);
	sprintf( msg, "USE %d %d", x, y);
	sprintf( msg, "REMV %d %d -1#", x, y);
	livingLifePage->hetuwSetNextActionMessage( msg );
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

	if (inASCII == 'h') {
		bDrawHelp = !bDrawHelp;
		return true;
	}

	if (inASCII == 'u') {
		useTileRelativeToMe(1, 0);
		return true;
	}
	if (inASCII == 'd') {
		dropTileRelativeToMe(1, 0);
		return true;
	}
	if (inASCII == 'r') {
		remvTileRelativeToMe(1, 0);
		return true;
	}
	if (inASCII == ' ') {
		//return true;
	}

	return false;
}

void HetuwMod::drawHelp() {
	char str[64];
	setDrawColor( 0, 0, 0, 0.8 );
	drawRect( livingLifePage->hetuwGetLastScreenViewCenter(), viewWidth/2, viewHeight/2 );

	setDrawColor( colorRainbow->color[0], colorRainbow->color[1], colorRainbow->color[2], 1 );

	double lineHeight = 30;

	// emotion words
	doublePair drawPos = livingLifePage->hetuwGetLastScreenViewCenter();
	drawPos.x -= viewWidth/2 - 20;
	drawPos.y += viewHeight/2 - 80;
	SimpleVector<Emotion> emotions = hetuwGetEmotions();
    for( int i=0; i<emotions.size(); i++ ) {
		if (i == 7 || i == 8) continue;
		int id = i;
		if (i > 6) id -= 2;

		if (id < 10) sprintf(str, " %i: %s", id, emotions.getElement(i)->triggerWord);
		else sprintf(str, "%i: %s", id, emotions.getElement(i)->triggerWord);

		livingLifePage->hetuwDrawWithHandwritingFont( str, drawPos );
		drawPos.y -= lineHeight;
	}

	drawPos = livingLifePage->hetuwGetLastScreenViewCenter();
	drawPos.x -= viewWidth/2 - 250;
	drawPos.y += viewHeight/2 - 80;

	livingLifePage->hetuwDrawWithHandwritingFont( "H TOGGLE SHOW HELP", drawPos );
	drawPos.y -= lineHeight;
	livingLifePage->hetuwDrawWithHandwritingFont( "= MAKE SCREENSHOT", drawPos );
	drawPos.y -= lineHeight;
	livingLifePage->hetuwDrawWithHandwritingFont( "+ ZOOM IN", drawPos );
	drawPos.y -= lineHeight;
	livingLifePage->hetuwDrawWithHandwritingFont( "- ZOOM OUT", drawPos );
	drawPos.y -= lineHeight;

	drawPos = livingLifePage->hetuwGetLastScreenViewCenter();
	drawPos.x -= viewWidth/2 - 570;
	drawPos.y += viewHeight/2 - 80;

	sprintf(str, "%s - BABY SUICIDE", translate( "dieCommand" ));
	livingLifePage->hetuwDrawWithHandwritingFont( str, drawPos );
	drawPos.y -= lineHeight;
	sprintf(str, "%s - TOGGLE SHOW FPS", translate( "fpsCommand" ));
	livingLifePage->hetuwDrawWithHandwritingFont( str, drawPos );
	drawPos.y -= lineHeight;
	sprintf(str, "%s - TOGGLE SHOW NETWORK", translate( "netCommand" ));
	livingLifePage->hetuwDrawWithHandwritingFont( str, drawPos );
	drawPos.y -= lineHeight;
	sprintf(str, "%s - SHOW PING", translate( "pingCommand" ));
	livingLifePage->hetuwDrawWithHandwritingFont( str, drawPos );
	drawPos.y -= lineHeight;
}
