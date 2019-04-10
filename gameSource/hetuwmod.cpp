#include "hetuwmod.h"

#include <iostream>

#include "LivingLifePage.h"
#include "objectBank.h"
#include "emotion.h"
#include "minorGems/util/SimpleVector.h"
#include "minorGems/game/drawUtils.h"
#include "groundSprites.h"

int HetuwMod::viewWidth;
int HetuwMod::viewHeight;

float HetuwMod::zoomScale;
int HetuwMod::panelOffsetX;
int HetuwMod::panelOffsetY;

HetuwMod::RainbowColor *HetuwMod::colorRainbow;

LivingLifePage *HetuwMod::livingLifePage;

bool HetuwMod::bDrawHelp;

float HetuwMod::lastPosX;
float HetuwMod::lastPosY;

char HetuwMod::charKey_Up;
char HetuwMod::charKey_Down;
char HetuwMod::charKey_Left;
char HetuwMod::charKey_Right;
char HetuwMod::charKey_TileStandingOn;

char HetuwMod::charKey_Backpack;
char HetuwMod::charKey_Eat;
char HetuwMod::charKey_ShowHelp;
char HetuwMod::charKey_ShowNames;
char HetuwMod::charKey_ShowCords;

bool HetuwMod::upKeyDown;
bool HetuwMod::downKeyDown;
bool HetuwMod::leftKeyDown;
bool HetuwMod::rightKeyDown;

doublePair HetuwMod::debugRecPos;
doublePair HetuwMod::debugRecPos2;

int HetuwMod::currentEmote;
time_t HetuwMod::lastEmoteTime;
time_t HetuwMod::lastSpecialEmote;

int* HetuwMod::dangerousAnimals;
int HetuwMod::dangerousAnimalsLength;

int* HetuwMod::closedDoorIDs;
int HetuwMod::closedDoorIDsLength;

bool HetuwMod::waitForDoorToOpen;
int HetuwMod::lastDoorToOpenX;
int HetuwMod::lastDoorToOpenY;

bool HetuwMod::stopAutoRoadRun;
time_t HetuwMod::stopAutoRoadRunTime;
bool HetuwMod::activateAutoRoadRun;

bool HetuwMod::bDrawNames;
float HetuwMod::playerNameColor[3];
doublePair HetuwMod::playerNamePos;

bool HetuwMod::bDrawCords;

void HetuwMod::init() {
	zoomScale = 1.5f;
	zoomCalc();
	
	colorRainbow = new RainbowColor();

	bDrawHelp = false;

	charKey_Up = 'w';
	charKey_Down = 's';
	charKey_Left = 'a';
	charKey_Right = 'd';
	charKey_TileStandingOn = ' ';

	charKey_Backpack = 'q';
	charKey_Eat = 'e';
	charKey_ShowHelp = 'h';
	charKey_ShowNames = 'n';
	charKey_ShowCords = 'c';

	debugRecPos = { 0.0, 0.0 };
	debugRecPos2 = { 0.0, 0.0 };

	bDrawNames = true;
	bDrawCords = true;

	initDangerousAnimals();	
	initClosedDoorIDs();

	initOnBirth();
}

void HetuwMod::initDangerousAnimals() {
	if (dangerousAnimals != NULL) {
		delete[] dangerousAnimals;
		dangerousAnimals = NULL;
	}
	dangerousAnimalsLength = 35;
	dangerousAnimals = new int[dangerousAnimalsLength];
	dangerousAnimals[0] = 2156; // Mosquito swarm

	dangerousAnimals[1] = 764; // Rattle Snake
	dangerousAnimals[2] = 1385; // Attacking Rattle Snake

	dangerousAnimals[3] = 1323; // Wild Boar
	dangerousAnimals[4] = 1328; // Wild Boar with Piglet 
	dangerousAnimals[5] = 1333; // Attacking Wild Boar
	dangerousAnimals[6] = 1334; // Attacking Wild Boar with Piglet
	dangerousAnimals[7] = 1339; // Domestic Boar
	dangerousAnimals[8] = 1341; // Domestic Boar with Piglet
	dangerousAnimals[9] = 1347; // Attacking Boar# domestic
	dangerousAnimals[10] = 1348; // Attacking Boar with Piglet# domestic

	dangerousAnimals[11] = 418; // Wolf
	dangerousAnimals[12] = 1630; // Semi-tame Wolf
	dangerousAnimals[13] = 420; // Shot Wolf
	dangerousAnimals[14] = 428; // Attacking Shot Wolf
	dangerousAnimals[15] = 429; // Dying Shot Wolf
	dangerousAnimals[16] = 1761; // Dying Semi-tame Wolf
	dangerousAnimals[17] = 1640; // Semi-tame Wolf# just fed
	dangerousAnimals[18] = 1642; // Semi-tame Wolf# pregnant
	dangerousAnimals[19] = 1636; // Semi-tame Wolf with Puppy#1
	dangerousAnimals[20] = 1635; // Semi-tame Wolf with Puppies#2
	dangerousAnimals[21] = 1631; // Semi-tame Wolf with Puppies#3
	dangerousAnimals[22] = 1748; // Old Semi-tame Wolf
	dangerousAnimals[23] = 1641; // @ Deadly Wolf
	
	dangerousAnimals[24] = 628; // Grizzly Bear
	dangerousAnimals[25] = 655; // Shot Grizzly Bear#2 attacking
	dangerousAnimals[26] = 653; // Hungry Grizzly Bear#attacking
	dangerousAnimals[27] = 644; // Dying Shot Grizzly Bear#3
	dangerousAnimals[28] = 631; // Hungry Grizzly Bear
	dangerousAnimals[29] = 646; // @ Unshot Grizzly Bear
	dangerousAnimals[30] = 635; // Shot Grizzly Bear#2
	dangerousAnimals[31] = 645; // Fed Grizzly Bear
	dangerousAnimals[32] = 632; // Shot Grizzly Bear#1
	dangerousAnimals[33] = 637; // Shot Grizzly Bear#3
	dangerousAnimals[34] = 654; // Shot Grizzly Bear#1 attacking
}

void HetuwMod::initClosedDoorIDs() {
	if (closedDoorIDs != NULL) {
		delete[] closedDoorIDs;
		closedDoorIDs = NULL;
	}
	closedDoorIDsLength = 7;
	closedDoorIDs = new int[closedDoorIDsLength];

	closedDoorIDs[0] = 116; // 116.txt:Pine Door# installed vert
	closedDoorIDs[1] = 2759; // 2759.txt:Springy Wooden Door# installed vert
	closedDoorIDs[2] = 876; // 876.txt:Wooden Door# Installed
	closedDoorIDs[3] = 1930; // 1930.txt:Twenty Minute Wooden Door# Installed
	closedDoorIDs[4] = 2757; // 2757.txt:Springy Wooden Door# Installed
	closedDoorIDs[5] = 877; // 877.txt:Wooden Door# installed vert
	closedDoorIDs[6] = 115; // 115.txt:Pine Door# Installed
}

void HetuwMod::initOnBirth() {
	lastPosX = 9999;
	lastPosY = 9999;

	upKeyDown = false;
	downKeyDown = false;
	leftKeyDown = false;
	rightKeyDown = false;

	currentEmote = -1;
	lastSpecialEmote = 0;

	stopAutoRoadRun = false;
	activateAutoRoadRun = false;
	stopAutoRoadRunTime = 0;

	waitForDoorToOpen = false;
	lastDoorToOpenX = 9999;
	lastDoorToOpenY = 9999;
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

void HetuwMod::livingLifeStep() {

	move();

	colorRainbow->step();

	if (activateAutoRoadRun) {
		if (time(NULL) > stopAutoRoadRunTime+2) {
			stopAutoRoadRun = false;
			stopAutoRoadRunTime = 0;
			activateAutoRoadRun = false;
		}
	}
}

void HetuwMod::livingLifeDraw() {

 	LiveObject *ourLiveObject = livingLifePage->getOurLiveObject();

	if (bDrawCords) drawCords();

	// age
	doublePair jDrawPos;
	char sBuf[32];
	sprintf(sBuf, "%d", (int)(ourLiveObject->age));
	jDrawPos = livingLifePage->hetuwGetLastScreenViewCenter();
	jDrawPos.x += 330;
	jDrawPos.y -= HetuwMod::viewHeight/2 - 25;
	livingLifePage->hetuwDrawWithHandwritingFont( sBuf, jDrawPos );

	if (bDrawHelp) drawHelp();

	//setDrawColor( 1.0, 0, 0, 1.0 );
	//drawRect( debugRecPos, 10, 10 );
	//setDrawColor( 0.0, 1.0, 0, 1.0 );
	//drawRect( debugRecPos2, 10, 10 );

	if (currentEmote >= 0 && lastEmoteTime+8 < time(NULL)) {
		lastEmoteTime = time(NULL);
		char message[64];
		sprintf( message, "EMOT 0 0 %i#", currentEmote);
        livingLifePage->sendToServerSocket( message );
	}
}

void HetuwMod::getRelationNameColor( const char* name, float* color ) {
	if ( !name ) {
		color[0] = 1.0f; color[1] = 1.0f; color[2] = 1.0f; 
	} else if ( strstr( name, "MOTHER" )) {
		if ( strstr( name, "GRANDMOTHER" )) {
			color[0] = 0.0f; color[1] = 0.7f; color[2] = 0.0f; 
		} else { // MOTHER
			color[0] = 0.0f; color[1] = 1.0f; color[2] = 0.0f; 
		}
	} else if ( strstr( name, "BROTHER" ) || strstr( name, "SISTER" )) {
		color[0] = 0.0f; color[1] = 1.0f; color[2] = 0.7f; 
	} else if ( strstr( name, "SON" ) || strstr( name, "DAUGHTER" )) {
		if( strstr( name, "GRAND" )) {
			color[0] = 0.4f; color[1] = 1.0f; color[2] = 0.0f; 
		} else { // DIRECT SON / DAUGTHER
			color[0] = 0.6f; color[1] = 1.0f; color[2] = 0.2f; 
		}
	} else if ( strstr( name, "UNCLE" ) || strstr( name, "AUNT" )) {
		if ( strstr( name, "GREAT" )) {
			color[0] = 0.1f; color[1] = 0.7f; color[2] = 0.5f; 
		} else { // DIRECT UNCLE / AUNT
			color[0] = 0.1f; color[1] = 0.7f; color[2] = 0.5f; 
		}
	} else if ( strstr( name, "COUSIN" ) && strstr( name, "FIRST" )) {
		color[0] = 0.3f; color[1] = 0.6f; color[2] = 1.0f; 
	} else {
		color[0] = 1.0f; color[1] = 0.2f; color[2] = 1.0f; 
	}
}

void HetuwMod::drawPlayerNames( LiveObject* player ) {
	if ( !player->name ) return;
	playerNamePos.x = player->currentPos.x * CELL_D;
	playerNamePos.y = player->currentPos.y * CELL_D;
	playerNamePos.y += 44;

	getRelationNameColor( player->relationName, playerNameColor );

	setDrawColor( 0.0, 0.0, 0.0, 0.8 );
	float textWidth = livingLifePage->hetuwMeasureStringHandwritingFont( player->name );
	drawRect( playerNamePos, textWidth/2 + 6, 16 );
	setDrawColor( playerNameColor[0], playerNameColor[1], playerNameColor[2], 1 );
	if ( player->name ) livingLifePage->hetuwDrawWithHandwritingFont( player->name, playerNamePos, alignCenter );
	//playerNamePos.y += 40;
	//if ( player->relationName ) livingLifePage->hetuwDrawWithHandwritingFont( player->relationName, playerNamePos );
}

void HetuwMod::useTileRelativeToMe( int x, int y ) {
 	LiveObject *ourLiveObject = livingLifePage->getOurLiveObject();
	x += ourLiveObject->xd;
	y += ourLiveObject->yd;
	x = livingLifePage->sendX(x);
	y = livingLifePage->sendY(y);
	char msg[32];
	sprintf( msg, "USE %d %d#", x, y);
	livingLifePage->hetuwSetNextActionMessage( msg, x, y );
}

void HetuwMod::dropTileRelativeToMe( int x, int y ) {
 	LiveObject *ourLiveObject = livingLifePage->getOurLiveObject();
	x += ourLiveObject->xd;
	y += ourLiveObject->yd;
	x = livingLifePage->sendX(x);
	y = livingLifePage->sendY(y);
	char msg[32];
	sprintf( msg, "DROP %d %d -1#", x, y);
	livingLifePage->hetuwSetNextActionMessage( msg, x, y );
}

void HetuwMod::remvTileRelativeToMe( int x, int y ) {
 	LiveObject *ourLiveObject = livingLifePage->getOurLiveObject();
	x += ourLiveObject->xd;
	y += ourLiveObject->yd;
	x = livingLifePage->sendX(x);
	y = livingLifePage->sendY(y);
	char msg[32];
	sprintf( msg, "REMV %d %d -1#", x, y);
	livingLifePage->hetuwSetNextActionMessage( msg, x, y );
}

void HetuwMod::actionAlphaRelativeToMe( int x, int y ) {
 	LiveObject *ourLiveObject = livingLifePage->getOurLiveObject();
	x += ourLiveObject->xd;
	y += ourLiveObject->yd;

	int objId = livingLifePage->hetuwGetObjId( x, y);
	bool use = false;

	if (objId > 0) use = true;
	else use = false;

	if( ourLiveObject->holdingID > 0 ) {
		ObjectRecord *held = getObject( ourLiveObject->holdingID );

		if( held->foodValue == 0 ) {
			TransRecord *r = getTrans( ourLiveObject->holdingID, -1 );
			if( r != NULL && r->newTarget != 0 ) { // a use-on-ground transition exists!
                use = true;	// override the drop action
			}
		}
	}

	x = livingLifePage->sendX(x);
	y = livingLifePage->sendY(y);
	char msg[32];
	if (use) sprintf( msg, "USE %d %d#", x, y);
	else sprintf( msg, "DROP %d %d -1#", x, y);
	livingLifePage->hetuwSetNextActionMessage( msg, x, y );
}

void HetuwMod::actionBetaRelativeToMe( int x, int y ) {
 	LiveObject *ourLiveObject = livingLifePage->getOurLiveObject();
	x += ourLiveObject->xd;
	y += ourLiveObject->yd;

	bool remove = false;
	if (ourLiveObject->holdingID <= 0) {
		remove = true;
	}

	x = livingLifePage->sendX(x);
	y = livingLifePage->sendY(y);
	char msg[32];
	if (remove) sprintf( msg, "REMV %d %d -1#", x, y);
	else sprintf( msg, "DROP %d %d -1#", x, y);
	livingLifePage->hetuwSetNextActionMessage( msg, x, y );
	if (!remove) livingLifePage->hetuwSetNextActionDropping( true );
}

void HetuwMod::useBackpack(bool replace) {
	int clothingSlot = 5; // backpack clothing slot

 	LiveObject *ourLiveObject = livingLifePage->getOurLiveObject();
	int x = round( ourLiveObject->xd );
	int y = round( ourLiveObject->yd );
	x = livingLifePage->sendX(x);
	y = livingLifePage->sendY(y);

	if( ourLiveObject->holdingID > 0 ) {
		char msg[32];
		if (replace) {
			sprintf( msg, "DROP %d %d %d#", x, y, clothingSlot );
		} else {
			sprintf( msg, "SELF %d %d %d#", x, y, clothingSlot );
		}
		livingLifePage->hetuwSetNextActionMessage( msg, x, y );
		livingLifePage->hetuwSetNextActionDropping(true);
	} else {
		char msg[32];
		sprintf( msg, "SREMV %d %d %d %d#", x, y, clothingSlot, -1 );
		livingLifePage->hetuwSetNextActionMessage( msg, x, y );
	}
}

void HetuwMod::useOnSelf() {
 	LiveObject *ourLiveObject = livingLifePage->getOurLiveObject();
	int x = round( ourLiveObject->xd );
	int y = round( ourLiveObject->yd );
	x = livingLifePage->sendX(x);
	y = livingLifePage->sendY(y);

	if( ourLiveObject->holdingID <= 0 ) return;

	char msg[32];
	sprintf( msg, "SELF %d %d %d#", x, y, -1 );
	livingLifePage->hetuwSetNextActionMessage( msg, x, y );

	if( getObject( ourLiveObject->holdingID )->foodValue > 0)
		livingLifePage->hetuwSetNextActionEating(true);
}

void HetuwMod::setEmote(int id) {
	lastEmoteTime = time(NULL);
	currentEmote = id;
}

bool HetuwMod::isCharKey(char c, char key) {
	return (c == key || c == toupper(key));
}

// when return true -> end/return in keyDown function in LivingLife
bool HetuwMod::livingLifeKeyDown(unsigned char inASCII) {

	if (livingLifePage->hetuwSayFieldIsFocused()) {
		return false;
	}
	// player is not trying to say something

	bool commandKey = isCommandKeyDown();
	bool shiftKey = isShiftKeyDown();

	// emotes
	int jic = (int)inASCII - 48;
	if (jic >= 0 && jic <= 9) {
		if (jic > 6) jic += 2;
		currentEmote = -1;
		char message[64];
		sprintf( message, "EMOT 0 0 %i#", jic);
        livingLifePage->sendToServerSocket( message );
		return true;
	}

//	if (inASCII == 'u') {
//		useTileRelativeToMe(1, 0);
//		return true;
//	}
//	if (inASCII == 't') {
//		dropTileRelativeToMe(1, 0);
//		return true;
//	}
//	if (inASCII == 'r') {
//		remvTileRelativeToMe(1, 0);
//		return true;
//	}

	if (isCharKey(inASCII, charKey_ShowHelp)) {
		bDrawHelp = !bDrawHelp;
		return true;
	}

	if (isCharKey(inASCII, charKey_ShowNames)) {
		bDrawNames = !bDrawNames;
		return true;
	}
	
	if (isCharKey(inASCII, charKey_ShowCords)) {
		bDrawCords = !bDrawCords;
		return true;
	}

	if (commandKey) {
		if (isCharKey(inASCII, charKey_TileStandingOn) || inASCII == charKey_TileStandingOn-64) {
			actionBetaRelativeToMe( 0, 0 );
			return true;
		}
	} else {
		if (isCharKey(inASCII, charKey_TileStandingOn)) {
			actionAlphaRelativeToMe( 0, 0 );
			return true;
		}
	}

	if (!shiftKey && !commandKey) {
		if (inASCII == charKey_Up || inASCII == toupper(charKey_Up)) {
			upKeyDown = true;
			stopAutoRoadRun = true;
			return true;
		}
		if (inASCII == charKey_Left || inASCII == toupper(charKey_Left)) {
			leftKeyDown = true;
			stopAutoRoadRun = true;
			return true;
		}
		if (inASCII == charKey_Down || inASCII == toupper(charKey_Down)) {
			downKeyDown = true;
			stopAutoRoadRun = true;
			return true;
		}
		if (inASCII == charKey_Right || inASCII == toupper(charKey_Right)) {
			rightKeyDown = true;
			stopAutoRoadRun = true;
			return true;
		}
	} else if (commandKey) {
		if (inASCII == charKey_Up || inASCII == toupper(charKey_Up) || inASCII+64 == toupper(charKey_Up)) {
			actionBetaRelativeToMe( 0, 1 );
			return true;
		}
		if (inASCII == charKey_Left || inASCII == toupper(charKey_Left) || inASCII+64 == toupper(charKey_Left)) {
			actionBetaRelativeToMe( -1, 0 );
			return true;
		}
		if (inASCII == charKey_Down || inASCII == toupper(charKey_Down) || inASCII+64 == toupper(charKey_Down)) {
			actionBetaRelativeToMe( 0, -1 );
			return true;
		}
		if (inASCII == charKey_Right || inASCII == toupper(charKey_Right) || inASCII+64 == toupper(charKey_Right)) {
			actionBetaRelativeToMe( 1, 0 );
			return true;
		}
	} else if (shiftKey) {
		if (inASCII == charKey_Up || inASCII == toupper(charKey_Up)) {
			actionAlphaRelativeToMe( 0, 1 );
			return true;
		}
		if (inASCII == charKey_Left || inASCII == toupper(charKey_Left)) {
			actionAlphaRelativeToMe( -1, 0 );
			return true;
		}
		if (inASCII == charKey_Down || inASCII == toupper(charKey_Down)) {
			actionAlphaRelativeToMe( 0, -1 );
			return true;
		}
		if (inASCII == charKey_Right || inASCII == toupper(charKey_Right)) {
			actionAlphaRelativeToMe( 1, 0 );
			return true;
		}
	}

	if (!shiftKey && isCharKey(inASCII, charKey_Backpack)) {
		useBackpack();
		return true;
	}
	if ((shiftKey || commandKey) && isCharKey(inASCII, charKey_Backpack)) {
		useBackpack(true);
		return true;
	}
	if (isCharKey(inASCII, charKey_Eat)) {
		useOnSelf();
		return true;
	}

	return false;
}

bool HetuwMod::livingLifeKeyUp(unsigned char inASCII) {

	bool r = false;

	bool commandKey = isCommandKeyDown();

	if (inASCII == charKey_Up || inASCII == toupper(charKey_Up)) {
		upKeyDown = false;
		r = true;
	}
	if (inASCII == charKey_Left || inASCII == toupper(charKey_Left)) {
		leftKeyDown = false;
		r = true;
	}
	if (inASCII == charKey_Down || inASCII == toupper(charKey_Down)) {
		downKeyDown = false;
		r = true;
	}
	if (inASCII == charKey_Right || inASCII == toupper(charKey_Right)) {
		rightKeyDown = false;
		r = true;
	}
	if (commandKey) {
		if (inASCII+64 == toupper(charKey_Up)) {
			upKeyDown = false;
			r = true;
		}
		if (inASCII+64 == toupper(charKey_Left)) {
			leftKeyDown = false;
			r = true;
		}
		if (inASCII+64 == toupper(charKey_Down)) {
			downKeyDown = false;
			r = true;
		}
		if (inASCII+64 == toupper(charKey_Right)) {
			rightKeyDown = false;
			r = true;
		}
	}

	if (!upKeyDown && !leftKeyDown && !downKeyDown && !rightKeyDown) {
		lastPosX = 9999;
		lastPosY = 9999;
		stopAutoRoadRunTime = time(NULL);
		activateAutoRoadRun = true;
	}

	return r;
}

bool HetuwMod::tileIsSafeToWalk(int x, int y) {
	int objId = livingLifePage->hetuwGetObjId( x, y);
	if (objId > 0) {
		for (int i = 0; i < dangerousAnimalsLength; i++) {
			if (objId == dangerousAnimals[i]) return false;
		}
		ObjectRecord* obj = getObject(objId);
		if (obj->blocksWalking) {
 			LiveObject *ourLiveObject = livingLifePage->getOurLiveObject();
			if (ourLiveObject->xd == x || ourLiveObject->yd == y)
				if (tileHasClosedDoor( x, y )) return true;
			return false;
		}
	}
	return true;
}

bool HetuwMod::tileHasNoDangerousAnimals(int x, int y) {
	int objId = livingLifePage->hetuwGetObjId( x, y);
	if (objId > 0) {
		for (int i = 0; i < dangerousAnimalsLength; i++) {
			if (objId == dangerousAnimals[i]) return false;
		}
	}
	return true;
}

bool HetuwMod::cornerTileIsSafeToWalk( int sX, int sY, bool up, bool down, bool right, bool left) {
	bool tileNextIsSafe = true;
	if (up && ( right || left )) {
		tileNextIsSafe = tileHasNoDangerousAnimals( sX, sY+1 );
		if (tileNextIsSafe) {
			if (right) {
				tileNextIsSafe = tileHasNoDangerousAnimals( sX+1, sY );
			} else {
				tileNextIsSafe = tileHasNoDangerousAnimals( sX-1, sY );
			}
		}	
	}
	else if (down && ( right || left )) {
		tileNextIsSafe = tileHasNoDangerousAnimals( sX, sY-1 );
		if (tileNextIsSafe) {
			if (right) {
				tileNextIsSafe = tileHasNoDangerousAnimals( sX+1, sY );
			} else {
				tileNextIsSafe = tileHasNoDangerousAnimals( sX-1, sY );
			}
		}
	}
	return tileNextIsSafe;
}

bool HetuwMod::tileHasClosedDoor(int x, int y) {
	int objId = livingLifePage->hetuwGetObjId( x, y);
	if (objId > 0) {
		for (int i = 0; i < closedDoorIDsLength; i++) {
			if (objId == closedDoorIDs[i]) return true;
		}
	}
	return false;
}

void HetuwMod::move() {
	
	if (!upKeyDown && !leftKeyDown && !downKeyDown && !rightKeyDown)
		return;

	if (!livingLifePage) return;
 	LiveObject *ourLiveObject = livingLifePage->getOurLiveObject();
	if (!ourLiveObject) return;
	float x = round(ourLiveObject->currentPos.x);
	float y = round(ourLiveObject->currentPos.y);

	if (x == lastPosX && y == lastPosY && ourLiveObject->inMotion)
		return;

	float sX = x;
	float sY = y;

	//debugRecPos2.x = x*CELL_D;
	//debugRecPos2.y = y*CELL_D;

	if (upKeyDown)
		y += 1.0f;
	else if (downKeyDown)
		y -= 1.0f;

	if (rightKeyDown)
		x += 1.0f;
	else if (leftKeyDown)
		x -= 1.0f;

	bool tileIsSafe = false;
	bool tileNextIsSafe = true;

	tileNextIsSafe = cornerTileIsSafeToWalk( sX, sY, upKeyDown, downKeyDown, rightKeyDown, leftKeyDown );

	if (tileNextIsSafe)
		tileIsSafe = tileIsSafeToWalk( round(x), round(y) );

	if (!tileIsSafe && upKeyDown && !downKeyDown) {
		x = sX;
		y = sY + 1;
		tileIsSafe = tileIsSafeToWalk( round(x), round(y) );
		if (!tileIsSafe && !leftKeyDown) {
			tileNextIsSafe = cornerTileIsSafeToWalk( sX, sY, true, false, true, false );
			if (tileNextIsSafe) {
				x = sX + 1;
				tileIsSafe = tileIsSafeToWalk( round(x), round(y) );
			}
		}
		if (!tileIsSafe && !rightKeyDown) {
			tileNextIsSafe = cornerTileIsSafeToWalk( sX, sY, true, false, false, true );
			if (tileNextIsSafe) {
				x = sX - 1;
				tileIsSafe = tileIsSafeToWalk( round(x), round(y) );
			}
		}
	}
	if (!tileIsSafe && downKeyDown && !upKeyDown) {
		x = sX;
		y = sY - 1;
		tileIsSafe = tileIsSafeToWalk( round(x), round(y) );
		if (!tileIsSafe && !leftKeyDown) {
			tileNextIsSafe = cornerTileIsSafeToWalk( sX, sY, false, true, true, false );
			if (tileNextIsSafe) {
				x = sX + 1;
				tileIsSafe = tileIsSafeToWalk( round(x), round(y) );
			}
		}
		if (!tileIsSafe && !rightKeyDown) {
			tileNextIsSafe = cornerTileIsSafeToWalk( sX, sY, false, true, false, true );
			if (tileNextIsSafe) {
				x = sX - 1;
				tileIsSafe = tileIsSafeToWalk( round(x), round(y) );
			}
		}
	}
	if (!tileIsSafe && rightKeyDown && !leftKeyDown) {
		x = sX + 1;
		y = sY;
		tileIsSafe = tileIsSafeToWalk( round(x), round(y) );
		if (!tileIsSafe && !downKeyDown) {
			tileNextIsSafe = cornerTileIsSafeToWalk( sX, sY, true, false, true, false );
			if (tileNextIsSafe) {
				y = sY + 1;
				tileIsSafe = tileIsSafeToWalk( round(x), round(y) );
			}
		}
		if (!tileIsSafe && !upKeyDown) {
			tileNextIsSafe = cornerTileIsSafeToWalk( sX, sY, false, true, true, false );
			if (tileNextIsSafe) {
				y = sY - 1;
				tileIsSafe = tileIsSafeToWalk( round(x), round(y) );
			}
		}
	}
	if (!tileIsSafe && leftKeyDown && !rightKeyDown) {
		x = sX - 1;
		y = sY;
		tileIsSafe = tileIsSafeToWalk( round(x), round(y) );
		if (!tileIsSafe && !downKeyDown) {
			tileNextIsSafe = cornerTileIsSafeToWalk( sX, sY, true, false, false, true );
			if (tileNextIsSafe) {
				y = sY + 1;
				tileIsSafe = tileIsSafeToWalk( round(x), round(y) );
			}
		}
		if (!tileIsSafe && !upKeyDown) {
			tileNextIsSafe = cornerTileIsSafeToWalk( sX, sY, false, true, false, true );
			if (tileNextIsSafe) {
				y = sY - 1;
				tileIsSafe = tileIsSafeToWalk( round(x), round(y) );
			}
		}
	}

	if (!tileIsSafe || !tileNextIsSafe) return;

	lastPosX = sX;
	lastPosY = sY;

	if (waitForDoorToOpen && (lastDoorToOpenX != x || lastDoorToOpenY != y)) {
		waitForDoorToOpen = false;
	} else if (waitForDoorToOpen) {
		if (tileHasClosedDoor( lastDoorToOpenX, lastDoorToOpenY ))
			return;
		waitForDoorToOpen = false;
	} else if (tileHasClosedDoor( x, y )) {
		char msg[32];
		sprintf( msg, "USE %d %d#", livingLifePage->sendX(x), livingLifePage->sendY(y));
		livingLifePage->hetuwSetNextActionMessage( msg, x, y );
		waitForDoorToOpen = true;
		lastDoorToOpenX = (int)x;
		lastDoorToOpenY = (int)y;
		return;
	}

	x *= CELL_D;
	y *= CELL_D;

	livingLifePage->hetuwClickMove(x, y);

	//debugRecPos.x = x;
	//debugRecPos.y = y;
}

void HetuwMod::drawCords() {
 	LiveObject *ourLiveObject = livingLifePage->getOurLiveObject();

	char sBufA[16];
	sprintf(sBufA, "%d", (int)ourLiveObject->currentPos.x );
	float textWidthA = livingLifePage->hetuwMeasureStringHandwritingFont( sBufA );
	char sBufB[16];
	sprintf(sBufB, "%d", (int)ourLiveObject->currentPos.y );
	float textWidthB = livingLifePage->hetuwMeasureStringHandwritingFont( sBufB );

	doublePair drawPosA = livingLifePage->hetuwGetLastScreenViewCenter();
	doublePair drawPosB;
	drawPosA.x -= HetuwMod::viewWidth/2 - 40;
	drawPosA.y += HetuwMod::viewHeight/2 - 40;
	drawPosB.x = drawPosA.x + 20 + textWidthA/2 + textWidthB/2;
	drawPosB.y = drawPosA.y;

	setDrawColor( 0, 0, 0, 1 );
	drawRect( drawPosA, textWidthA/2 + 6, 16 );
	drawRect( drawPosB, textWidthB/2 + 6, 16 );

	if (ourLiveObject->currentPos.x < 0) setDrawColor( 1, 0.8, 0, 1 );
	else setDrawColor( 0, 1, 0.8, 1 );
	livingLifePage->hetuwDrawWithHandwritingFont( sBufA, drawPosA, alignCenter );
	if (ourLiveObject->currentPos.y < 0) setDrawColor( 1, 0.8, 0, 1 );
	else setDrawColor( 0, 1, 0.8, 1 );
	livingLifePage->hetuwDrawWithHandwritingFont( sBufB, drawPosB, alignCenter );
}

void HetuwMod::drawHelp() {
	char str[64];
	setDrawColor( 0, 0, 0, 0.8 );
	drawRect( livingLifePage->hetuwGetLastScreenViewCenter(), viewWidth/2, viewHeight/2 );

	setDrawColor( colorRainbow->color[0], 1.0f, colorRainbow->color[2], 1 );

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
	livingLifePage->hetuwDrawWithHandwritingFont( "PRESS NUMBER KEY FOR SHORT EMOTE", drawPos );
	drawPos.y -= lineHeight;
	livingLifePage->hetuwDrawWithHandwritingFont( "WRITE EMOTE FOR PERMANENT EMOTE", drawPos );
	drawPos.y -= lineHeight;

	drawPos = livingLifePage->hetuwGetLastScreenViewCenter();
	drawPos.x -= viewWidth/2 - 250;
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

	sprintf(str, "%c TOGGLE SHOW HELP", toupper(charKey_ShowHelp));
	livingLifePage->hetuwDrawWithHandwritingFont( str, drawPos );
	drawPos.y -= lineHeight;
	livingLifePage->hetuwDrawWithHandwritingFont( "= MAKE SCREENSHOT", drawPos );
	drawPos.y -= lineHeight;
	livingLifePage->hetuwDrawWithHandwritingFont( "+ ZOOM IN", drawPos );
	drawPos.y -= lineHeight;
	livingLifePage->hetuwDrawWithHandwritingFont( "- ZOOM OUT", drawPos );
	drawPos.y -= lineHeight;
	livingLifePage->hetuwDrawWithHandwritingFont( "F TOGGLE FIX CAMERA", drawPos );
	drawPos.y -= lineHeight;
	sprintf(str, "%c TOGGLE SHOW NAMES", toupper(charKey_ShowNames));
	livingLifePage->hetuwDrawWithHandwritingFont( str, drawPos );
	drawPos.y -= lineHeight;
	sprintf(str, "%c TOGGLE SHOW CORDS", toupper(charKey_ShowCords));
	livingLifePage->hetuwDrawWithHandwritingFont( str, drawPos );
	drawPos.y -= lineHeight;

	drawPos = livingLifePage->hetuwGetLastScreenViewCenter();
	drawPos.x -= viewWidth/2 - 630;
	drawPos.y += viewHeight/2 - 80;

	sprintf(str, "%c - USE BACKPACK", toupper(charKey_Backpack));
	livingLifePage->hetuwDrawWithHandwritingFont( str, drawPos );
	drawPos.y -= lineHeight;
	sprintf(str, "SHIFT+%c - USE BACKPACK", toupper(charKey_Backpack));
	livingLifePage->hetuwDrawWithHandwritingFont( str, drawPos );
	drawPos.y -= lineHeight;
	sprintf(str, "%c - EAT / PUT CLOTHES ON", toupper(charKey_Eat));
	livingLifePage->hetuwDrawWithHandwritingFont( str, drawPos );
	drawPos.y -= lineHeight;
	sprintf(str, "%c%c%c%c - MOVE", toupper(charKey_Up), toupper(charKey_Left), toupper(charKey_Down), toupper(charKey_Right));
	livingLifePage->hetuwDrawWithHandwritingFont( str, drawPos );
	drawPos.y -= lineHeight;
	sprintf(str, "SHIFT+%c%c%c%c - USE/PICK UP ITEM", toupper(charKey_Up), toupper(charKey_Left), toupper(charKey_Down), toupper(charKey_Right));
	livingLifePage->hetuwDrawWithHandwritingFont( str, drawPos );
	drawPos.y -= lineHeight;
	sprintf(str, "CTRL+%c%c%c%c - DROP / PICK ITEM FROM CONTAINER", toupper(charKey_Up), toupper(charKey_Left), toupper(charKey_Down), toupper(charKey_Right));
	livingLifePage->hetuwDrawWithHandwritingFont( str, drawPos );
	drawPos.y -= lineHeight;
	if (charKey_TileStandingOn == ' ') sprintf(str, "SPACE - USE/PICK UP ITEM ON THE TILE YOU ARE STANDING ON");
	else sprintf(str, "%c - USE/PICK UP ITEM ON THE TILE YOU ARE STANDING ON", charKey_TileStandingOn);
	livingLifePage->hetuwDrawWithHandwritingFont( str, drawPos );
	drawPos.y -= lineHeight;
	if (charKey_TileStandingOn == ' ') sprintf(str, "CTRL+SPACE - DROP / PICK ITEM FROM CONTAINER");
	else sprintf(str, "CTRL+%c - DROP / PICK ITEM FROM CONTAINER", charKey_TileStandingOn);
	livingLifePage->hetuwDrawWithHandwritingFont( str, drawPos );
	drawPos.y -= lineHeight;
}
