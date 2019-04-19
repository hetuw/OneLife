#include "hetuwmod.h"

#include <iostream>
#include <vector>

#include "LivingLifePage.h"
#include "objectBank.h"
#include "emotion.h"
#include "minorGems/util/SimpleVector.h"
#include "minorGems/game/drawUtils.h"
#include "groundSprites.h"

int HetuwMod::viewWidth;
int HetuwMod::viewHeight;

float HetuwMod::zoomScale;
float HetuwMod::guiScaleRaw;
float HetuwMod::guiScale;
int HetuwMod::panelOffsetX;
int HetuwMod::panelOffsetY;

HetuwMod::RainbowColor *HetuwMod::colorRainbow;

LivingLifePage *HetuwMod::livingLifePage;
LiveObject *HetuwMod::ourLiveObject;

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
char HetuwMod::charKey_Baby;
char HetuwMod::charKey_ShowHelp;
char HetuwMod::charKey_ShowNames;
char HetuwMod::charKey_ShowCords;
char HetuwMod::charKey_ShowPlayersInRange;
char HetuwMod::charKey_ShowDeathMessages;

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

int HetuwMod::iDrawNames;
float HetuwMod::playerNameColor[3];
doublePair HetuwMod::playerNamePos;

bool HetuwMod::bDrawCords;

int HetuwMod::stepCount;
double HetuwMod::ourAge;

SimpleVector<LiveObject> *HetuwMod::gameObjects;

std::vector<HetuwMod::PlayerInMap*> HetuwMod::playersInMap;
bool HetuwMod::bDrawMap;
float HetuwMod::mapScale;
float HetuwMod::mapOffsetX;
float HetuwMod::mapOffsetY;

char HetuwMod::charKey_ShowMap;
char HetuwMod::charKey_MapZoomIn;
char HetuwMod::charKey_MapZoomOut;
bool HetuwMod::mapZoomInKeyDown;
bool HetuwMod::mapZoomOutKeyDown;

int HetuwMod::playersInRangeNum;
int HetuwMod::youngWomenInRange;
bool HetuwMod::bDrawPlayersInRangePanel;

bool HetuwMod::bDrawDeathMessages;
std::vector<HetuwMod::DeathMsg*> HetuwMod::deathMessages;

void HetuwMod::init() {
	zoomScale = 1.5f;
	guiScaleRaw = 0.8f;
	guiScale = guiScaleRaw * zoomScale;
	zoomCalc();
	
	colorRainbow = new RainbowColor();

	bDrawHelp = false;
	bDrawPlayersInRangePanel = true;
	bDrawDeathMessages = true;

	charKey_Up = 'w';
	charKey_Down = 's';
	charKey_Left = 'a';
	charKey_Right = 'd';
	charKey_TileStandingOn = ' ';

	charKey_Backpack = 'q';
	charKey_Eat = 'e';
	charKey_Baby = 'c';
	charKey_ShowHelp = 'h';
	charKey_ShowNames = 'n';
	charKey_ShowCords = 'z';
	charKey_ShowPlayersInRange = 'p';
	charKey_ShowDeathMessages = 't';

	charKey_ShowMap = 'm';
	charKey_MapZoomIn = 'u';
	charKey_MapZoomOut = 'o';

	debugRecPos = { 0.0, 0.0 };
	debugRecPos2 = { 0.0, 0.0 };

	iDrawNames = 1;
	bDrawCords = true;

	bDrawMap = false;
	mapScale = 20000;
	mapOffsetX = 0;
	mapOffsetY = 0;

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
	closedDoorIDsLength = 8;
	closedDoorIDs = new int[closedDoorIDsLength];

	closedDoorIDs[0] = 116; // 116.txt:Pine Door# installed vert
	closedDoorIDs[1] = 2759; // 2759.txt:Springy Wooden Door# installed vert
	closedDoorIDs[2] = 876; // 876.txt:Wooden Door# Installed
	closedDoorIDs[3] = 1930; // 1930.txt:Twenty Minute Wooden Door# Installed
	closedDoorIDs[4] = 2757; // 2757.txt:Springy Wooden Door# Installed
	closedDoorIDs[5] = 877; // 877.txt:Wooden Door# installed vert
	closedDoorIDs[6] = 115; // 115.txt:Pine Door# Installed
	closedDoorIDs[7] = 1851; // 1851.txt:Fence Gate
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
	
	playersInMap.clear();
	playersInMap.shrink_to_fit();

	mapZoomInKeyDown = false;
	mapZoomInKeyDown = false;

	playersInRangeNum = 0;
	youngWomenInRange = 0;

	deathMessages.clear();
	deathMessages.shrink_to_fit();
}

void HetuwMod::setLivingLifePage(LivingLifePage *inLivingLifePage, SimpleVector<LiveObject>* inGameObjects) {
	livingLifePage = inLivingLifePage;
	gameObjects = inGameObjects;
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
	guiScale = guiScaleRaw * zoomScale;
}

void HetuwMod::zoomIncrease() {
	zoomScale *= 1.25f;
	if (zoomScale > 10.0f) zoomScale = 10.0f;
	zoomCalc();
}

void HetuwMod::zoomDecrease() {
	zoomScale *= 0.75f;
	if (zoomScale < 1.0f) zoomScale = 1.0f;
	zoomCalc();
}

void HetuwMod::guiScaleIncrease() {
	guiScaleRaw *= 0.9f;
	if (guiScaleRaw < 0.1) guiScaleRaw = 0.1;
	guiScale = guiScaleRaw * zoomScale;
}

void HetuwMod::guiScaleDecrease() {
	guiScaleRaw *= 1.1f;
	if (guiScaleRaw > 1.5) guiScaleRaw = 1.5;
	guiScale = guiScaleRaw * zoomScale;
}

void HetuwMod::livingLifeStep() {

	stepCount++;
	if (stepCount > 10000) stepCount = 0;

 	ourLiveObject = livingLifePage->getOurLiveObject();
	if (!ourLiveObject) return;

	if (stepCount % 10 == 0) 
		ourAge = livingLifePage->hetuwGetAge( ourLiveObject );

	move();

	colorRainbow->step();

	if (stepCount % 50 == 0) {
		updateMap();
	}
	if (mapZoomInKeyDown) mapScale *= 0.96;
	if (mapZoomOutKeyDown) mapScale *= 1.04;

	if (stepCount % 46 == 0) {
		updatePlayersInRangePanel();
	}

	if (activateAutoRoadRun) {
		if (time(NULL) > stopAutoRoadRunTime+2) {
			stopAutoRoadRun = false;
			stopAutoRoadRunTime = 0;
			activateAutoRoadRun = false;
		}
	}

	if (currentEmote >= 0 && lastEmoteTime+8 < time(NULL)) {
		lastEmoteTime = time(NULL);
		char message[64];
		sprintf( message, "EMOT 0 0 %i#", currentEmote);
        livingLifePage->sendToServerSocket( message );
	}
}

void HetuwMod::livingLifeDraw() {

 	ourLiveObject = livingLifePage->getOurLiveObject();
	if (!ourLiveObject) return;

	drawAge();

	if (bDrawCords) drawCords();

	if (bDrawPlayersInRangePanel) drawPlayersInRangePanel();

	if (bDrawDeathMessages) drawDeathMessages();

	if (bDrawMap) drawMap();

	if (bDrawHelp) drawHelp();

	//setDrawColor( 1.0, 0, 0, 1.0 );
	//drawRect( debugRecPos, 10, 10 );
	//setDrawColor( 0.0, 1.0, 0, 1.0 );
	//drawRect( debugRecPos2, 10, 10 );

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
		color[0] = 0.3f; color[1] = 1.0f; color[2] = 1.0f; 
	} else if ( strstr( name, "SON" ) || strstr( name, "DAUGHTER" )) {
		if( strstr( name, "GRAND" )) {
			color[0] = 0.4f; color[1] = 1.0f; color[2] = 0.0f; 
		} else { // DIRECT SON / DAUGTHER
			color[0] = 0.6f; color[1] = 1.0f; color[2] = 0.2f; 
		}
	} else if ( strstr( name, "UNCLE" ) || strstr( name, "AUNT" )) {
		if ( strstr( name, "GREAT" )) {
			color[0] = 0.2f; color[1] = 0.7f; color[2] = 0.5f; 
		} else { // DIRECT UNCLE / AUNT
			color[0] = 0.2f; color[1] = 0.7f; color[2] = 0.5f; 
		}
	} else if ( strstr( name, "NIECE" ) || strstr( name, "NEPHEW" )) {
		if ( strstr( name, "GREAT" )) {
			color[0] = 0.1f; color[1] = 0.7f; color[2] = 0.8f; 
		} else { // DIRECT NIECE / NEPHEW
			color[0] = 0.1f; color[1] = 0.7f; color[2] = 1.0f; 
		}
	} else if ( strstr( name, "COUSIN" )) {
		if ( strstr( name, "FIRST" )) {
			color[0] = 0.3f; color[1] = 0.6f; color[2] = 1.0f; 
		} else if ( strstr( name, "SECOND" )) {
			color[0] = 0.4f; color[1] = 0.5f; color[2] = 1.0f; 
		} else {
			color[0] = 0.6f; color[1] = 0.4f; color[2] = 1.0f; 
		}
	} else {
		color[0] = 0.6f; color[1] = 0.4f; color[2] = 1.0f; 
	}
}

void HetuwMod::drawPlayerNames( LiveObject* player ) {
	if ( !player->name ) return;
	if( player->hide || player->outOfRange ) return;
	if( !player->allSpritesLoaded ) return;

	playerNamePos.x = player->currentPos.x * CELL_D;
	playerNamePos.y = player->currentPos.y * CELL_D;
	playerNamePos.y += 44;

	getRelationNameColor( player->relationName, playerNameColor );

	setDrawColor( 0.0, 0.0, 0.0, 0.8 );
	if ( iDrawNames == 2) {
		float textWidth = livingLifePage->hetuwMeasureStringHandwritingFont( player->name );
		drawRect( playerNamePos, textWidth/2 + 6, 16 );
		setDrawColor( playerNameColor[0], playerNameColor[1], playerNameColor[2], 1 );
		livingLifePage->hetuwDrawWithHandwritingFont( player->name, playerNamePos, alignCenter );
	} else if ( iDrawNames == 1) {
		char playerName[48];
		removeLastName( playerName, player->name );
		float textWidth = livingLifePage->hetuwMeasureStringHandwritingFont( playerName );
		drawRect( playerNamePos, textWidth/2 + 6, 16 );
		setDrawColor( playerNameColor[0], playerNameColor[1], playerNameColor[2], 1 );
		livingLifePage->hetuwDrawWithHandwritingFont( playerName, playerNamePos, alignCenter );
	}
	//playerNamePos.y += 40;
	//if ( player->relationName ) livingLifePage->hetuwDrawWithHandwritingFont( player->relationName, playerNamePos );
}

void HetuwMod::useTileRelativeToMe( int x, int y ) {
	x += ourLiveObject->xd;
	y += ourLiveObject->yd;
	x = livingLifePage->sendX(x);
	y = livingLifePage->sendY(y);
	char msg[32];
	sprintf( msg, "USE %d %d#", x, y);
	livingLifePage->hetuwSetNextActionMessage( msg, x, y );
}

void HetuwMod::dropTileRelativeToMe( int x, int y ) {
	x += ourLiveObject->xd;
	y += ourLiveObject->yd;
	x = livingLifePage->sendX(x);
	y = livingLifePage->sendY(y);
	char msg[32];
	sprintf( msg, "DROP %d %d -1#", x, y);
	livingLifePage->hetuwSetNextActionMessage( msg, x, y );
}

void HetuwMod::remvTileRelativeToMe( int x, int y ) {
	x += ourLiveObject->xd;
	y += ourLiveObject->yd;
	x = livingLifePage->sendX(x);
	y = livingLifePage->sendY(y);
	char msg[32];
	sprintf( msg, "REMV %d %d -1#", x, y);
	livingLifePage->hetuwSetNextActionMessage( msg, x, y );
}

void HetuwMod::actionAlphaRelativeToMe( int x, int y ) {
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
	
	bool remove = false;
	if ( ourLiveObject->holdingID <= 0 && objId == 253) // full berry clay bowl
		remove = true;

	if ( ourLiveObject->holdingID < 0 ) { // holding babay
		remove = false;
		use = false;
	}

	x = livingLifePage->sendX(x);
	y = livingLifePage->sendY(y);
	char msg[32];
	if (remove) sprintf( msg, "REMV %d %d -1#", x, y);
	else if (use) sprintf( msg, "USE %d %d#", x, y);
	else sprintf( msg, "DROP %d %d -1#", x, y);
	livingLifePage->hetuwSetNextActionMessage( msg, x, y );
}

void HetuwMod::actionBetaRelativeToMe( int x, int y ) {
	x += ourLiveObject->xd;
	y += ourLiveObject->yd;

	bool remove = false;
	if (ourLiveObject->holdingID <= 0) {
		remove = true;
	}
	bool use = false;
	int objId = livingLifePage->hetuwGetObjId( x, y );
	if (objId > 0) {
		ObjectRecord* obj = getObject(objId);
		if (obj->numSlots == 0 && obj->blocksWalking) {
			TransRecord *r = getTrans( ourLiveObject->holdingID, objId );
			if ( r != NULL && r->newTarget != 0 ) {
				use = true;
			}
		}
	}

	if (objId == 253) // full berry clay bowl
		use = true;

	if ( ourLiveObject->holdingID < 0 ) { // holding babay
		remove = false;
		use = false;
	}

	x = livingLifePage->sendX(x);
	y = livingLifePage->sendY(y);
	char msg[32];
	if (use) sprintf( msg, "USE %d %d#", x, y);
	else if (remove) sprintf( msg, "REMV %d %d -1#", x, y);
	else sprintf( msg, "DROP %d %d -1#", x, y);
	livingLifePage->hetuwSetNextActionMessage( msg, x, y );
	if (!remove) livingLifePage->hetuwSetNextActionDropping( true );
}

void HetuwMod::useBackpack(bool replace) {
	int clothingSlot = 5; // backpack clothing slot

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

void HetuwMod::pickUpBaby( int x, int y ) {
	char msg[32];
	sprintf( msg, "BABY %d %d#", x, y );
	livingLifePage->hetuwSetNextActionMessage( msg, x, y );
}

bool HetuwMod::playerIsInCloseRange( LiveObject* o ) {
	if ( o->outOfRange ) return false;

	if ( o->xd != ourLiveObject->xd && o->yd != ourLiveObject->yd ) return false; 
	int posDiff = 0;
	if ( o->xd == ourLiveObject->xd) posDiff = o->yd - ourLiveObject->yd;
	else if ( o->yd == ourLiveObject->yd) posDiff = o->xd - ourLiveObject->xd;
	if (posDiff > 1 || posDiff < -1) return false;
	return true;
}

void HetuwMod::pickUpBabyInRange() {
	if ( livingLifePage->hetuwGetAge( ourLiveObject ) < 13 ) return;

	if ( ourLiveObject->holdingID != 0 ) {
		dropTileRelativeToMe( 0, 0 );
		return;
	}

	// find new baby to pick up - prefer babies further away
	int babyFound = false;
	int babyX = 0;
	int babyY = 0;
	for(int i=0; i<gameObjects->size(); i++) {
		LiveObject *o = gameObjects->getElement( i );
			
		if ( livingLifePage->hetuwGetAge( o ) > 5 ) continue;

		if ( o->xd != ourLiveObject->xd && o->yd != ourLiveObject->yd ) continue; 
		if ( !babyFound ) {
			if ( o->xd == ourLiveObject->xd && o->yd == ourLiveObject->yd ) {
				babyFound = true;
				babyX = o->xd;
				babyY = o->yd;
				continue;
			}
		}
		int posDiff = 0;
		if ( o->xd == ourLiveObject->xd) posDiff = o->yd - ourLiveObject->yd;
		else if ( o->yd == ourLiveObject->yd) posDiff = o->xd - ourLiveObject->xd;
		if (posDiff > 1 || posDiff < -1) continue;

		pickUpBaby( o->xd, o->yd );
		return;
	}
	if ( !babyFound ) return;
	pickUpBaby( babyX, babyY );
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
	if (!commandKey && !shiftKey) {
		int jic = (int)inASCII - 48;
		if (jic >= 0 && jic <= 9) {
			if (jic > 6) jic += 2;
			currentEmote = -1;
			char message[64];
			sprintf( message, "EMOT 0 0 %i#", jic);
	        livingLifePage->sendToServerSocket( message );
			return true;
		}
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

	if (!commandKey && isCharKey(inASCII, charKey_ShowHelp)) {
		bDrawHelp = !bDrawHelp;
		return true;
	}
	if (!commandKey && isCharKey(inASCII, charKey_ShowNames)) {
		iDrawNames++;
		if (iDrawNames >= 3) iDrawNames = 0;
		return true;
	}
	if (!commandKey && isCharKey(inASCII, charKey_ShowCords)) {
		bDrawCords = !bDrawCords;
		return true;
	}
	if (!commandKey && isCharKey(inASCII, charKey_ShowPlayersInRange)) {
		bDrawPlayersInRangePanel = !bDrawPlayersInRangePanel;
		return true;
	}
	if (!commandKey && isCharKey(inASCII, charKey_ShowDeathMessages)) {
		bDrawDeathMessages = !bDrawDeathMessages;
		return true;
	}

	if (commandKey) {
		if (isCharKey(inASCII, charKey_TileStandingOn)) {
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
		if (inASCII+64 == toupper(charKey_Up)) {
			actionBetaRelativeToMe( 0, 1 );
			return true;
		}
		if (inASCII+64 == toupper(charKey_Left)) {
			actionBetaRelativeToMe( -1, 0 );
			return true;
		}
		if (inASCII+64 == toupper(charKey_Down)) {
			actionBetaRelativeToMe( 0, -1 );
			return true;
		}
		if (inASCII+64 == toupper(charKey_Right)) {
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
	if (isCharKey(inASCII, charKey_Baby)) {
		pickUpBabyInRange();
		return true;
	}

	if (!commandKey && isCharKey(inASCII, charKey_ShowMap)) {
		bDrawMap = !bDrawMap;
		return true;
	}
	if (!commandKey && isCharKey(inASCII, charKey_MapZoomIn)) {
		mapZoomInKeyDown = true;
		return true;
	}
	if (!commandKey && isCharKey(inASCII, charKey_MapZoomOut)) {
		mapZoomOutKeyDown = true;
		return true;
	}

	if (!commandKey && !shiftKey && inASCII == 27) { // ESCAPE KEY
		upKeyDown = false;
		leftKeyDown = false;
		downKeyDown = false;
		rightKeyDown = false;
		bDrawHelp = false;
		bDrawMap = false;
		lastPosX = 9999;
		lastPosY = 9999;
		stopAutoRoadRunTime = time(NULL);
		activateAutoRoadRun = true;
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

	if (inASCII == charKey_MapZoomIn || inASCII == toupper(charKey_MapZoomIn)) {
		mapZoomInKeyDown = false;
		r = true;
	}
	if (inASCII == charKey_MapZoomOut || inASCII == toupper(charKey_MapZoomOut)) {
		mapZoomOutKeyDown = false;
		r = true;
	}

	if (!upKeyDown && !leftKeyDown && !downKeyDown && !rightKeyDown) {
		lastPosX = 9999;
		lastPosY = 9999;
		stopAutoRoadRunTime = time(NULL);
		activateAutoRoadRun = true;
	}

	return r;
}

bool HetuwMod::livingLifeSpecialKeyDown(unsigned char inKeyCode) {
	bool commandKey = isCommandKeyDown();
	bool shiftKey = isShiftKeyDown();
	bool r = false;

	if (!commandKey && !shiftKey) {
		if ( inKeyCode == MG_KEY_F1 ) {
			currentEmote = -1;
			char message[] = "EMOT 0 0 12#"; // HMPH
	        livingLifePage->sendToServerSocket( message );
			r = true;
		}
		if ( inKeyCode == MG_KEY_F2 ) {
			currentEmote = -1;
			char message[] = "EMOT 0 0 13#"; // LOVE
	        livingLifePage->sendToServerSocket( message );
			r = true;
		}
		if ( inKeyCode == MG_KEY_F3 ) {
			currentEmote = -1;
			char message[] = "EMOT 0 0 14#"; // OREALLY
	        livingLifePage->sendToServerSocket( message );
			r = true;
		}
		if ( inKeyCode == MG_KEY_F4 ) {
			currentEmote = -1;
			char message[] = "EMOT 0 0 15#"; // SHOCK
	        livingLifePage->sendToServerSocket( message );
			r = true;
		}
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

void HetuwMod::removeLastName(char *newName, const char* name) {
	int k = 0;
	bool skip = false;
	for (int i=0; name[i] != 0; i++) {
		if (name[i] == ' ') {
			skip = !skip;
		}
		if (!skip) {
			newName[k] = name[i];
			k++;
		}
	}
	if (k < 0) newName[0] = 0;
	else newName[k] = 0;
}

void HetuwMod::getLastName(char* lastName, const char* name) {
	int k = -1;
	for (int i=0; name[i] != 0; i++) {
		if (name[i] == ' ') {
			if (k >= 0) break;
			else {
				k = 0;
				continue;
			}
		}
		if (k >= 0) {
			lastName[k] = name[i];
			k++;
		}
	}
	if (k < 0) lastName[0] = 0;
	else lastName[k] = 0;
}

void HetuwMod::setLastNameColor( const char* lastName, float alpha ) {
	if (!lastName) {
		setDrawColor( 1, 1, 1, alpha );
		return;
	}
	int num = 0;
	for (int i=0; lastName[i] != 0; i++) {
		num += (int)lastName[i];
	}
	setDrawColor(
		0.0 + (num%100)/100.0f,  
		0.0 + (num%182)/150.0f,  
		0.0 + (num%77)/77.0f,  
		alpha );
}

void HetuwMod::updateMap() {
	//printf("hetuw printing %d gameObjects -------------------------------------- \n", gameObjects->size());
	time_t timeNow = time(NULL);
	for(int i=0; i<gameObjects->size(); i++) {
		LiveObject *o = gameObjects->getElement( i );
		int p = -1;
		for(unsigned k=0; k<playersInMap.size(); k++) {
			if (playersInMap[k]->id == o->id) {
				p = (int)k;
				break;
			}
		}
		if (p < 0) {
			p = playersInMap.size();
			PlayerInMap *pInMap = new PlayerInMap();
			pInMap->id = o->id;
			pInMap->lastTime = timeNow;
			playersInMap.push_back(pInMap);	
		}
		if (!playersInMap[p]->name && o->name) {
			playersInMap[p]->name = new char[64];
			strcpy(playersInMap[p]->name, o->name);
			playersInMap[p]->lastName = new char[32];
			getLastName( playersInMap[p]->lastName, playersInMap[p]->name );
			//printf("hetuw name: %s lastName: %s\n", playersInMap[p]->name, playersInMap[p]->lastName);
			if (playersInMap[p]->lastName[0] == 0) {
				delete[] playersInMap[p]->lastName;
				playersInMap[p]->lastName = NULL;
			}
			playersInMap[p]->lastTime = timeNow;
		}
		if (o->xd != 1977) {
			playersInMap[p]->x = o->xd;
			playersInMap[p]->lastTime = timeNow;
		}
		if (o->yd != 1977) {
			playersInMap[p]->y = o->yd;
			playersInMap[p]->lastTime = timeNow;
		}
		playersInMap[p]->finalAgeSet = o->finalAgeSet;
	}

	//for(unsigned k=0; k<playersInMap.size(); k++) {
		//printf("hetuw %d.pim x:%d y:%d %s\n", (int)k, playersInMap[k]->x, playersInMap[k]->y, playersInMap[k]->lastName);
	//}
}

#define hetuwPlayersInRangeDistance 50
void HetuwMod::updatePlayersInRangePanel() {
	playersInRangeNum = 0;
	youngWomenInRange = 0;
	for(int i=0; i<gameObjects->size(); i++) {
		LiveObject *o = gameObjects->getElement( i );

		if ( o->outOfRange ) continue;
		int distX = o->xd - ourLiveObject->xd;
		if ( distX > hetuwPlayersInRangeDistance || distX < -hetuwPlayersInRangeDistance)
			continue;
		int distY = o->yd - ourLiveObject->yd;
		if ( distY > hetuwPlayersInRangeDistance || distY < -hetuwPlayersInRangeDistance)
			continue;

		playersInRangeNum++;

		if ( !getObject( o->displayID )->male )
			if ( livingLifePage->hetuwGetAge( o ) < 40 )
				youngWomenInRange++;
	}
}

#define hetuwDeathMessageRange 200
void HetuwMod::onPlayerUpdate( LiveObject* inO, const char* line ) {
	if ( inO == NULL ) return;
	if ( ourLiveObject == NULL ) return;

	bool isDeathMsg = ( strstr( line, "X X" ) != NULL );
	if ( !isDeathMsg ) return;

	LiveObject *o = NULL;
	for(int i=0; i<gameObjects->size(); i++) {
		LiveObject *kO = gameObjects->getElement(i);
		if (inO->id == kO->id) {
			o = kO;
		}
	}
	if ( o == NULL ) return;
	if ( !o->name ) return;
	
	int diffX = o->xd - ourLiveObject->xd;
	int diffY = o->yd - ourLiveObject->yd;
	if ( diffX > hetuwDeathMessageRange || diffX < -hetuwDeathMessageRange) return;
	if ( diffY > hetuwDeathMessageRange || diffY < -hetuwDeathMessageRange) return;

	DeathMsg* deathMsg = new DeathMsg();

	deathMsg->timeReci = time(NULL);

	deathMsg->name = new char[64];
	sprintf( deathMsg->name, "%s", o->name); 

	deathMsg->age = (int)livingLifePage->hetuwGetAge( o );
	if ( getObject( o->displayID ) )
		deathMsg->male = getObject( o->displayID )->male;
	deathMsg->killed = strstr( line, "kille" ) != NULL;

	getRelationNameColor( o->relationName, deathMsg->nameColor );

	deathMessages.push_back(deathMsg);
}

void HetuwMod::drawDeathMessages() {
	if ( deathMessages.size() <= 0 ) return;

	DeathMsg* dm = deathMessages[0];

	doublePair drawPos = livingLifePage->hetuwGetLastScreenViewCenter();
	drawPos.y += viewHeight/2;
	drawPos.y -= 20*guiScale;
	
	char gender[8];
	sprintf( gender, "%c ", dm->male ? 'M' : 'F');
	char age[8];
	sprintf( age, "%d ", dm->age);

	double nameWidth = livingLifePage->hetuwMeasureScaledHandwritingFont( dm->name, guiScale );
	double ripWidth = livingLifePage->hetuwMeasureScaledHandwritingFont( "RIP ", guiScale );
	double genderWidth = livingLifePage->hetuwMeasureScaledHandwritingFont( gender, guiScale );
	double ageWidth = livingLifePage->hetuwMeasureScaledHandwritingFont( age, guiScale );
	double textWidth = nameWidth + ripWidth + genderWidth + ageWidth;

	doublePair recDrawPos = drawPos;

	setDrawColor( 0, 0, 0, 0.8 );
	drawRect( recDrawPos, (textWidth)/2 + 10*guiScale, 20*guiScale );

	drawPos.x -= textWidth/2;

	if ( dm->killed ) setDrawColor( 1, 0.2, 0, 1 );
	else setDrawColor( 1, 1, 1, 1 );
	livingLifePage->hetuwDrawScaledHandwritingFont( "RIP " , drawPos, guiScale );
	drawPos.x += ripWidth;

	if ( dm->male ) setDrawColor( 0.2, 0.6, 1.0, 1 );
	else setDrawColor( 1, 0.4, 0.8, 1 );
	livingLifePage->hetuwDrawScaledHandwritingFont( gender , drawPos, guiScale );
	drawPos.x += genderWidth;

	setDrawColor( 1, 1, 1, 1 );
	livingLifePage->hetuwDrawScaledHandwritingFont( age , drawPos, guiScale );
	drawPos.x += ageWidth;

	setDrawColor( dm->nameColor[0], dm->nameColor[1], dm->nameColor[2], 1 );
	livingLifePage->hetuwDrawScaledHandwritingFont( dm->name , drawPos, guiScale );

	if ( dm->timeReci+15 < time(NULL) ) {
		delete deathMessages[0];
		deathMessages.erase( deathMessages.begin() );
		if ( deathMessages.size() > 0 )
			deathMessages[0]->timeReci = time(NULL);
	}
}

void HetuwMod::drawMap() {
	doublePair drawPos;
	doublePair screenCenter = livingLifePage->hetuwGetLastScreenViewCenter();
	int mouseX, mouseY;
	livingLifePage->hetuwGetMouseXY( mouseX, mouseY );

	setDrawColor( 0, 0, 0, 0.2 );
	drawRect( screenCenter, viewWidth/2, viewHeight/2 );
	setDrawColor( 1, 1, 1, 1 );

	char names[32][32];
	int namesCount = 0;
	double minX = screenCenter.x - viewWidth/2;
	double minY = screenCenter.y - viewHeight/2;
	double maxX = screenCenter.x + viewWidth/2;
	double maxY = screenCenter.y + viewHeight/2;
	char drawMouseOver[128];
	bool bDrawMouseOver = false;
	int recWidthHalf = 10*zoomScale;
	int recHeightHalf = 10*zoomScale;
	for(unsigned k=0; k<playersInMap.size(); k++) {
		if (playersInMap[k]->x == 999999) continue;
		if (!playersInMap[k]->name) continue;
		drawPos.x = (playersInMap[k]->x - ourLiveObject->xd) / mapScale;
		drawPos.y = (playersInMap[k]->y - ourLiveObject->yd) / mapScale;
		drawPos.x += mapOffsetX;
		drawPos.y += mapOffsetY;
		drawPos.x *= viewHeight;
		drawPos.y *= viewHeight;
		drawPos.x += screenCenter.x;
		drawPos.y += screenCenter.y;
		if (drawPos.x < minX || drawPos.x > maxX || drawPos.y < minY || drawPos.y > maxY)
			continue;
		if (drawPos.x > mouseX-recWidthHalf && drawPos.x < mouseX+recWidthHalf &&
			drawPos.y > mouseY-recHeightHalf && drawPos.y < mouseY+recHeightHalf) {
			bDrawMouseOver = true;
			if (playersInMap[k]->lastName) {
				sprintf(drawMouseOver, "%s X:%d Y:%d", playersInMap[k]->lastName, playersInMap[k]->x, playersInMap[k]->y);
			} else {
				sprintf(drawMouseOver, "X:%d Y:%d", playersInMap[k]->x, playersInMap[k]->y);
			}
		}
		
		if (playersInMap[k]->lastName) {
			bool nameFound = false;
			for (int i=0; i<namesCount; i++) {
				if (strcmp(names[i], playersInMap[k]->lastName) == 0) {
					nameFound = true;
					break;
				}
			}
			if (!nameFound) {
				strcpy( names[namesCount], playersInMap[k]->lastName );
				namesCount++;
			}
		}
		
		float alpha = 1.0f;
		if (playersInMap[k]->finalAgeSet) alpha = 0.4f;

		if (ourLiveObject->id == playersInMap[k]->id) 
			setDrawColor( colorRainbow->color[0], 1.0f, colorRainbow->color[2], 1 );
		else setLastNameColor( playersInMap[k]->lastName, alpha );

		drawRect( drawPos, recWidthHalf, recHeightHalf );
	}

	setDrawColor( 0, 0, 0, 0.8 );

	char strZoomKeys[64];
	sprintf( strZoomKeys, "USE %c/%c TO ZOOM IN/OUT", toupper(charKey_MapZoomIn), toupper(charKey_MapZoomOut)); 
	float strZoomKeysWidth = livingLifePage->hetuwMeasureScaledHandwritingFont( strZoomKeys, guiScale ); 
	doublePair drawKeysRecPos;
	drawKeysRecPos.x = screenCenter.x - viewWidth/2;
	drawKeysRecPos.y = screenCenter.y - viewHeight/2;
	drawKeysRecPos.x += strZoomKeysWidth/2 + 10*guiScale;
	drawKeysRecPos.y += 80;
	drawRect( drawKeysRecPos, strZoomKeysWidth/2+10*guiScale, 15*guiScale);  

	doublePair drawNameRecPos;
	drawNameRecPos.x = screenCenter.x - viewWidth/2 + 50*guiScale;
	drawNameRecPos.y = drawKeysRecPos.y + 15*guiScale;
	float drawNameRecWidth = 100*guiScale;
	float drawNameRecHeight = namesCount*15*guiScale + 10*guiScale;
	drawNameRecPos.y += drawNameRecHeight;
	drawRect( drawNameRecPos, drawNameRecWidth, drawNameRecHeight );

	doublePair drawNamesPos;
	drawNamesPos.x = screenCenter.x - viewWidth/2;
	drawNamesPos.y = drawKeysRecPos.y + 40*guiScale;
	drawNamesPos.x += 20*guiScale;
	for (int i=0; i<namesCount; i++) {
		setLastNameColor( names[i] , 1.0f );
		livingLifePage->hetuwDrawScaledHandwritingFont( names[i], drawNamesPos, guiScale );
		drawNamesPos.y += 30*guiScale;
	}

	setDrawColor( 1, 1, 1, 1 );
	livingLifePage->hetuwDrawScaledHandwritingFont( strZoomKeys, drawKeysRecPos, guiScale, alignCenter );

	if (bDrawMouseOver) {
		doublePair drawMouseOverPos;
		float textWidth = livingLifePage->hetuwMeasureScaledHandwritingFont( drawMouseOver, guiScale ); 
		drawMouseOverPos.x = mouseX - textWidth/2;
		drawMouseOverPos.y = mouseY + 20*guiScale;
		setDrawColor( 0, 0, 0, 0.5 );
		doublePair bckgrRecPos;
		bckgrRecPos.x = mouseX;
		bckgrRecPos.y = mouseY + 20*guiScale;
		drawRect( bckgrRecPos, textWidth/2 + 10*guiScale, 15*guiScale );
		setDrawColor( 1, 1, 1, 1 );
		livingLifePage->hetuwDrawScaledHandwritingFont( drawMouseOver, drawMouseOverPos, guiScale );
	}
}

void HetuwMod::drawPlayersInRangePanel() {
	setDrawColor( 0, 0, 0, 0.8 );
	doublePair bckgrRecPos = livingLifePage->hetuwGetLastScreenViewCenter();
	int bckgrRecWidthHalf = 140*guiScale;
	int bckgrRecHeightHalf = 35*guiScale;
	bckgrRecPos.x += viewWidth/2;
	bckgrRecPos.y += viewHeight/2;
	doublePair textPos = bckgrRecPos;
	bckgrRecPos.x -= bckgrRecWidthHalf;
	bckgrRecPos.y -= bckgrRecHeightHalf;
	drawRect( bckgrRecPos, bckgrRecWidthHalf, bckgrRecHeightHalf );

	setDrawColor( 1, 1, 1, 1 );
	char text[32];
	textPos.y -= 20*guiScale;
	textPos.x -= 20*guiScale;
	if (playersInRangeNum < 10) sprintf(text, "PLAYERS IN RANGE:   %d", playersInRangeNum);
	else if (playersInRangeNum < 100) sprintf(text, "PLAYERS IN RANGE:  %d", playersInRangeNum);
	else sprintf(text, "PLAYERS IN RANGE: %d", playersInRangeNum);
	livingLifePage->hetuwDrawScaledHandwritingFont( text, textPos, guiScale, alignRight );
	textPos.y -= 25*guiScale;

	if (youngWomenInRange <= 0) setDrawColor( 1, 0.2, 0, 1 );
	else if (youngWomenInRange == 1) setDrawColor( 1, 0.7, 0, 1);
	else if (youngWomenInRange == 2) setDrawColor( 1, 0.9, 0, 1);
	else if (youngWomenInRange == 3) setDrawColor( 0.8, 1, 0, 1);
	else if (youngWomenInRange == 4) setDrawColor( 0.5, 1, 0, 1);
	else if (youngWomenInRange == 5) setDrawColor( 0.3, 1, 0, 1);
	else setDrawColor( 0.0, 1, 0, 1);
	if (youngWomenInRange < 10) sprintf(text, "YOUNG WOMEN:   %d", youngWomenInRange);
	else if (youngWomenInRange < 100) sprintf(text, "YOUNG WOMEN:  %d", youngWomenInRange);
	else sprintf(text, "YOUNG WOMEN: %d", youngWomenInRange);
	livingLifePage->hetuwDrawScaledHandwritingFont( text, textPos, guiScale, alignRight );
}

void HetuwMod::drawAge() {
	setDrawColor( 0, 0, 0, 1 );
	doublePair drawPos;
	char sBuf[32];
	int age = (int)(ourAge*10);
	int ageDecimal = age - int(age*0.1)*10;
	age = (int)((age-ageDecimal)*0.1);
	sprintf(sBuf, "%d.%d", age, ageDecimal);
	drawPos = livingLifePage->hetuwGetLastScreenViewCenter();
	drawPos.x += 310;
	drawPos.y -= viewHeight/2 - 25;
	livingLifePage->hetuwDrawWithHandwritingFont( sBuf, drawPos );
}

void HetuwMod::drawCords() {
	int x = round(ourLiveObject->currentPos.x);
	int y = round(ourLiveObject->currentPos.y);

	char sBufA[16];
	sprintf(sBufA, "%d", x );
	float textWidthA = livingLifePage->hetuwMeasureScaledHandwritingFont( sBufA, guiScale );
	char sBufB[16];
	sprintf(sBufB, "%d", y );
	float textWidthB = livingLifePage->hetuwMeasureScaledHandwritingFont( sBufB, guiScale );

	doublePair drawPosA = livingLifePage->hetuwGetLastScreenViewCenter();
	doublePair drawPosB;
	drawPosA.x -= HetuwMod::viewWidth/2 - (40*guiScale);
	drawPosA.y += HetuwMod::viewHeight/2 - (40*guiScale);
	drawPosB.x = drawPosA.x + (20*guiScale) + textWidthA/2 + textWidthB/2;
	drawPosB.y = drawPosA.y;

	setDrawColor( 0, 0, 0, 1 );
	drawRect( drawPosA, textWidthA/2 + 6*guiScale, 16*guiScale );
	drawRect( drawPosB, textWidthB/2 + 6*guiScale, 16*guiScale );

	if (x < 0) setDrawColor( 1, 0.8, 0, 1 );
	else setDrawColor( 0.2, 1, 0.2, 1 );
	livingLifePage->hetuwDrawScaledHandwritingFont( sBufA, drawPosA, guiScale, alignCenter );
	if (y < 0) setDrawColor( 1, 0.8, 0, 1 );
	else setDrawColor( 0.2, 1, 0.2, 1 );
	livingLifePage->hetuwDrawScaledHandwritingFont( sBufB, drawPosB, guiScale, alignCenter );
}

void HetuwMod::drawHelp() {
	float guiScale = (guiScaleRaw+0.1) * zoomScale;
	char str[64];
	setDrawColor( 0, 0, 0, 0.8 );
	drawRect( livingLifePage->hetuwGetLastScreenViewCenter(), viewWidth/2, viewHeight/2 );

	setDrawColor( colorRainbow->color[0], 1.0f, colorRainbow->color[2], 1 );

	double lineHeight = 30*guiScale;

	// emotion words
	doublePair drawPos = livingLifePage->hetuwGetLastScreenViewCenter();
	drawPos.x -= viewWidth/2 - 20*guiScale;
	drawPos.y += viewHeight/2 - 80*guiScale;
	SimpleVector<Emotion> emotions = hetuwGetEmotions();
    for( int i=0; i<emotions.size(); i++ ) {
		if (i == 7 || i == 8) continue;
		int id = i;
		if (i > 6) id -= 2;

		if (id < 10) sprintf(str, " %i: %s", id, emotions.getElement(i)->triggerWord);
		else sprintf(str, "F%i: %s", id-9, emotions.getElement(i)->triggerWord);

		livingLifePage->hetuwDrawScaledHandwritingFont( str, drawPos, guiScale );
		drawPos.y -= lineHeight;
	}
	livingLifePage->hetuwDrawScaledHandwritingFont( "PRESS NUMBER KEY FOR SHORT EMOTE", drawPos, guiScale );
	drawPos.y -= lineHeight;
	livingLifePage->hetuwDrawScaledHandwritingFont( "WRITE EMOTE FOR PERMANENT EMOTE", drawPos, guiScale );
	drawPos.y -= lineHeight;

	drawPos = livingLifePage->hetuwGetLastScreenViewCenter();
	drawPos.x -= viewWidth/2 - 250*guiScale;
	drawPos.y += viewHeight/2 - 80*guiScale;

	sprintf(str, "%s - BABY SUICIDE", translate( "dieCommand" ));
	livingLifePage->hetuwDrawScaledHandwritingFont( str, drawPos, guiScale );
	drawPos.y -= lineHeight;
	sprintf(str, "%s - TOGGLE SHOW FPS", translate( "fpsCommand" ));
	livingLifePage->hetuwDrawScaledHandwritingFont( str, drawPos, guiScale );
	drawPos.y -= lineHeight;
	sprintf(str, "%s - TOGGLE SHOW NETWORK", translate( "netCommand" ));
	livingLifePage->hetuwDrawScaledHandwritingFont( str, drawPos, guiScale );
	drawPos.y -= lineHeight;
	sprintf(str, "%s - SHOW PING", translate( "pingCommand" ));
	livingLifePage->hetuwDrawScaledHandwritingFont( str, drawPos, guiScale );
	drawPos.y -= lineHeight;

	sprintf(str, "%c TOGGLE SHOW HELP", toupper(charKey_ShowHelp));
	livingLifePage->hetuwDrawScaledHandwritingFont( str, drawPos, guiScale );
	drawPos.y -= lineHeight;
	livingLifePage->hetuwDrawScaledHandwritingFont( "= MAKE SCREENSHOT", drawPos, guiScale );
	drawPos.y -= lineHeight;
	livingLifePage->hetuwDrawScaledHandwritingFont( "F TOGGLE FIX CAMERA", drawPos, guiScale );
	drawPos.y -= lineHeight;
	sprintf(str, "%c TOGGLE SHOW NAMES", toupper(charKey_ShowNames));
	livingLifePage->hetuwDrawScaledHandwritingFont( str, drawPos, guiScale );
	drawPos.y -= lineHeight;
	sprintf(str, "%c TOGGLE SHOW CORDS", toupper(charKey_ShowCords));
	livingLifePage->hetuwDrawScaledHandwritingFont( str, drawPos, guiScale );
	drawPos.y -= lineHeight;
	sprintf(str, "%c TOGGLE SHOW MAP", toupper(charKey_ShowMap));
	livingLifePage->hetuwDrawScaledHandwritingFont( str, drawPos, guiScale );
	drawPos.y -= lineHeight;
	sprintf(str, "%c TOGGLE SHOW PLAYERS IN RANGE", toupper(charKey_ShowPlayersInRange));
	livingLifePage->hetuwDrawScaledHandwritingFont( str, drawPos, guiScale );
	drawPos.y -= lineHeight;
	sprintf(str, "%c TOGGLE SHOW DEATH MESSAGES", toupper(charKey_ShowDeathMessages));
	livingLifePage->hetuwDrawScaledHandwritingFont( str, drawPos, guiScale );
	drawPos.y -= lineHeight;

	drawPos = livingLifePage->hetuwGetLastScreenViewCenter();
	drawPos.x -= viewWidth/2 - 640*guiScale;
	drawPos.y += viewHeight/2 - 80*guiScale;

	sprintf(str, "%c - USE BACKPACK", toupper(charKey_Backpack));
	livingLifePage->hetuwDrawScaledHandwritingFont( str, drawPos, guiScale );
	drawPos.y -= lineHeight;
	sprintf(str, "SHIFT+%c - USE BACKPACK", toupper(charKey_Backpack));
	livingLifePage->hetuwDrawScaledHandwritingFont( str, drawPos, guiScale );
	drawPos.y -= lineHeight;
	sprintf(str, "%c - EAT / PUT CLOTHES ON", toupper(charKey_Eat));
	livingLifePage->hetuwDrawScaledHandwritingFont( str, drawPos, guiScale );
	drawPos.y -= lineHeight;
	sprintf(str, "%c - PICK UP / DROP BABY", toupper(charKey_Baby));
	livingLifePage->hetuwDrawScaledHandwritingFont( str, drawPos, guiScale );
	drawPos.y -= lineHeight;
	sprintf(str, "%c%c%c%c - MOVE", toupper(charKey_Up), toupper(charKey_Left), toupper(charKey_Down), toupper(charKey_Right));
	livingLifePage->hetuwDrawScaledHandwritingFont( str, drawPos, guiScale );
	drawPos.y -= lineHeight;
	sprintf(str, "SHIFT+%c%c%c%c - USE/PICK UP ITEM", toupper(charKey_Up), toupper(charKey_Left), toupper(charKey_Down), toupper(charKey_Right));
	livingLifePage->hetuwDrawScaledHandwritingFont( str, drawPos, guiScale );
	drawPos.y -= lineHeight;
	sprintf(str, "CTRL+%c%c%c%c - DROP / PICK ITEM FROM CONTAINER", toupper(charKey_Up), toupper(charKey_Left), toupper(charKey_Down), toupper(charKey_Right));
	livingLifePage->hetuwDrawScaledHandwritingFont( str, drawPos, guiScale );
	drawPos.y -= lineHeight;
	if (charKey_TileStandingOn == ' ') sprintf(str, "SPACE - USE/PICK UP ITEM ON THE TILE YOU ARE STANDING ON");
	else sprintf(str, "%c - USE/PICK UP ITEM ON THE TILE YOU ARE STANDING ON", charKey_TileStandingOn);
	livingLifePage->hetuwDrawScaledHandwritingFont( str, drawPos, guiScale );
	drawPos.y -= lineHeight;
	if (charKey_TileStandingOn == ' ') sprintf(str, "CTRL+SPACE - DROP / PICK ITEM FROM CONTAINER");
	else sprintf(str, "CTRL+%c - DROP / PICK ITEM FROM CONTAINER", charKey_TileStandingOn);
	livingLifePage->hetuwDrawScaledHandwritingFont( str, drawPos, guiScale );
	drawPos.y -= lineHeight;
	livingLifePage->hetuwDrawScaledHandwritingFont( "LEFTARROWKEY ZOOM IN", drawPos, guiScale );
	drawPos.y -= lineHeight;
	livingLifePage->hetuwDrawScaledHandwritingFont( "RIGHTARROWKEY ZOOM OUT", drawPos, guiScale );
	drawPos.y -= lineHeight;
	livingLifePage->hetuwDrawScaledHandwritingFont( "CTRL+ARROWKEYS SCALE GUI", drawPos, guiScale );
	drawPos.y -= lineHeight;
}
