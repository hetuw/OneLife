#include "hetuwmod.h"

#include <iostream>
#include <vector>
#include <fstream>
#include <string>

#include "LivingLifePage.h"
#include "objectBank.h"
#include "emotion.h"
#include "minorGems/util/SimpleVector.h"
#include "minorGems/game/drawUtils.h"
#include "minorGems/graphics/openGL/ScreenGL.h"
#include "groundSprites.h"

using namespace std;

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

unsigned char HetuwMod::charKey_Up;
unsigned char HetuwMod::charKey_Down;
unsigned char HetuwMod::charKey_Left;
unsigned char HetuwMod::charKey_Right;
unsigned char HetuwMod::charKey_TileStandingOn;

unsigned char HetuwMod::charKey_Backpack;
unsigned char HetuwMod::charKey_Eat;
unsigned char HetuwMod::charKey_Baby;
unsigned char HetuwMod::charKey_ShowHelp;
unsigned char HetuwMod::charKey_ShowNames;
unsigned char HetuwMod::charKey_ShowCords;
unsigned char HetuwMod::charKey_ShowPlayersInRange;
unsigned char HetuwMod::charKey_ShowDeathMessages;
unsigned char HetuwMod::charKey_ShowHomeCords;
unsigned char HetuwMod::charKey_ShowHostileTiles;

unsigned char HetuwMod::charKey_CreateHome;
unsigned char HetuwMod::charKey_FixCamera;

unsigned char HetuwMod::charKey_ShowMap;
unsigned char HetuwMod::charKey_MapZoomIn;
unsigned char HetuwMod::charKey_MapZoomOut;

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

bool HetuwMod::bDrawHostileTiles;

int HetuwMod::stepCount;
double HetuwMod::ourAge;

SimpleVector<LiveObject> *HetuwMod::gameObjects;

std::vector<HetuwMod::PlayerInMap*> HetuwMod::playersInMap;
bool HetuwMod::bDrawMap;
float HetuwMod::mapScale;
float HetuwMod::mapOffsetX;
float HetuwMod::mapOffsetY;

bool HetuwMod::mapZoomInKeyDown;
bool HetuwMod::mapZoomOutKeyDown;

int HetuwMod::playersInRangeNum;
int HetuwMod::youngWomenInRange;
bool HetuwMod::bDrawPlayersInRangePanel;

bool HetuwMod::bDrawDeathMessages;
std::vector<HetuwMod::DeathMsg*> HetuwMod::deathMessages;

bool HetuwMod::bDrawHomeCords;
std::vector<HetuwMod::HomePos*> HetuwMod::homePosStack;
bool HetuwMod::bNextCharForHome;

GridPos HetuwMod::cordOffset;

bool HetuwMod::bDrawInputString;
string HetuwMod::tempInputString;

int HetuwMod::getCustomCords;
char HetuwMod::tempCordChar;
int HetuwMod::tempCordX;
int HetuwMod::tempCordY;

void HetuwMod::init() {
	zoomScale = 1.5f;
	guiScaleRaw = 0.8f;
	guiScale = guiScaleRaw * zoomScale;
	zoomCalc();
	
	colorRainbow = new RainbowColor();

	bDrawHelp = false;
	bDrawPlayersInRangePanel = false;
	bDrawDeathMessages = true;
	bDrawHomeCords = false;
	iDrawNames = 1;
	bDrawCords = true;
	bDrawHostileTiles = false;

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
	charKey_ShowHomeCords = 'g';
	charKey_ShowHostileTiles = 'u';

	charKey_ShowMap = 'm';
	charKey_MapZoomIn = 'u';
	charKey_MapZoomOut = 'o';

	charKey_CreateHome = 'r';
	charKey_FixCamera = 'f';

	debugRecPos = { 0.0, 0.0 };
	debugRecPos2 = { 0.0, 0.0 };

	bDrawMap = false;
	mapScale = 20000;
	mapOffsetX = 0;
	mapOffsetY = 0;

	initDangerousAnimals();	
	initClosedDoorIDs();

	initSettings();

	initOnBirth();
}

void HetuwMod::initDangerousAnimals() {
	if (dangerousAnimals != NULL) {
		delete[] dangerousAnimals;
		dangerousAnimals = NULL;
	}
	dangerousAnimalsLength = 38;
	dangerousAnimals = new int[dangerousAnimalsLength];

	int a = -1;

	a++; dangerousAnimals[a] = 2156; // Mosquito swarm

	a++; dangerousAnimals[a] = 764; // Rattle Snake
	a++; dangerousAnimals[a] = 1385; // Attacking Rattle Snake

	a++; dangerousAnimals[a] = 1323; // Wild Boar
	a++; dangerousAnimals[a] = 1328; // Wild Boar with Piglet 
	a++; dangerousAnimals[a] = 1333; // Attacking Wild Boar
	a++; dangerousAnimals[a] = 1334; // Attacking Wild Boar with Piglet
	a++; dangerousAnimals[a] = 1339; // Domestic Boar
	a++; dangerousAnimals[a] = 1341; // Domestic Boar with Piglet
	a++; dangerousAnimals[a] = 1347; // Attacking Boar# domestic
	a++; dangerousAnimals[a] = 1348; // Attacking Boar with Piglet# domestic

	a++; dangerousAnimals[a] = 418; // Wolf
	a++; dangerousAnimals[a] = 1630; // Semi-tame Wolf
	a++; dangerousAnimals[a] = 420; // Shot Wolf
	a++; dangerousAnimals[a] = 428; // Attacking Shot Wolf
	a++; dangerousAnimals[a] = 429; // Dying Shot Wolf
	a++; dangerousAnimals[a] = 1761; // Dying Semi-tame Wolf
	a++; dangerousAnimals[a] = 1640; // Semi-tame Wolf# just fed
	a++; dangerousAnimals[a] = 1642; // Semi-tame Wolf# pregnant
	a++; dangerousAnimals[a] = 1636; // Semi-tame Wolf with Puppy#1
	a++; dangerousAnimals[a] = 1635; // Semi-tame Wolf with Puppies#2
	a++; dangerousAnimals[a] = 1631; // Semi-tame Wolf with Puppies#3
	a++; dangerousAnimals[a] = 1748; // Old Semi-tame Wolf
	a++; dangerousAnimals[a] = 1641; // @ Deadly Wolf
	
	a++; dangerousAnimals[a] = 628; // Grizzly Bear
	a++; dangerousAnimals[a] = 655; // Shot Grizzly Bear#2 attacking
	a++; dangerousAnimals[a] = 653; // Hungry Grizzly Bear#attacking
	a++; dangerousAnimals[a] = 644; // Dying Shot Grizzly Bear#3
	a++; dangerousAnimals[a] = 631; // Hungry Grizzly Bear
	a++; dangerousAnimals[a] = 646; // @ Unshot Grizzly Bear
	a++; dangerousAnimals[a] = 635; // Shot Grizzly Bear#2
	a++; dangerousAnimals[a] = 645; // Fed Grizzly Bear
	a++; dangerousAnimals[a] = 632; // Shot Grizzly Bear#1
	a++; dangerousAnimals[a] = 637; // Shot Grizzly Bear#3
	a++; dangerousAnimals[a] = 654; // Shot Grizzly Bear#1 attacking

	a++; dangerousAnimals[a] = 1789; // Abused Pit Bull
	a++; dangerousAnimals[a] = 1747; // Mean Pit Bull
	a++; dangerousAnimals[a] = 1712; // Attacking Pit Bull

	a++;
	if (a != dangerousAnimalsLength) {
		printf("hetuw ERROR: a != dangerousAnimalsLength\n");
		printf("hetuw ERROR: %i != %i\n", a, dangerousAnimalsLength);
	}
}

void HetuwMod::initClosedDoorIDs() {
	if (closedDoorIDs != NULL) {
		delete[] closedDoorIDs;
		closedDoorIDs = NULL;
	}
	closedDoorIDsLength = 10;
	closedDoorIDs = new int[closedDoorIDsLength];

	closedDoorIDs[0] = 116; // 116.txt:Pine Door# installed vert
	closedDoorIDs[1] = 2759; // 2759.txt:Springy Wooden Door# installed vert
	closedDoorIDs[2] = 876; // 876.txt:Wooden Door# Installed
	closedDoorIDs[3] = 1930; // 1930.txt:Twenty Minute Wooden Door# Installed
	closedDoorIDs[4] = 2757; // 2757.txt:Springy Wooden Door# Installed
	closedDoorIDs[5] = 877; // 877.txt:Wooden Door# installed vert
	closedDoorIDs[6] = 115; // 115.txt:Pine Door# Installed
	closedDoorIDs[7] = 1851; // 1851.txt:Fence Gate
	closedDoorIDs[8] = 2984; // 2984.txt:Shaky Property Gate# +owned
	closedDoorIDs[9] = 2962; // 2962.txt:Property Gate# +owned
}

// outputs name[64] and value[64] - input line
void HetuwMod::getSettingsFileLine( char* name, char* value, string line ) {
	int n = 0;
	int v = 0;
	bool readName = true;
	for (unsigned i=0; i<line.length(); i++) {
		if (line[i] == ' ') continue;

		if (readName) {
			if (line[i] == '=') {
				readName = false;
				continue;
			}
			if (n >= 63) break;
			name[n] = line[i];
			n++;
		} else {
			if (v >= 63) break;
			value[v] = line[i];
			v++;
		}
	}
	name[n] = 0;
	value[v] = 0;
}

bool HetuwMod::setCharKey( char unsigned &key, const char *value ) {
	if (value[0] != '<') {
		key = value[0];
		return true;
	}
	if (strstr(value, "<space>")) {
		key = ' ';
		return true;
	}
	return false;
}

bool HetuwMod::setSetting( const char* name, const char* value ) {
	if (strlen(value) < 1) return false;
	if (strstr(name, "key_up")) {
		return setCharKey( charKey_Up, value );
	}
	if (strstr(name, "key_down")) {
		return setCharKey( charKey_Down, value );
	}
	if (strstr(name, "key_left")) {
		return setCharKey( charKey_Left, value );
	}
	if (strstr(name, "key_right")) {
		return setCharKey( charKey_Right, value );
	}
	if (strstr(name, "key_center")) {
		return setCharKey( charKey_TileStandingOn, value );
	}
	if (strstr(name, "key_backpack")) {
		return setCharKey( charKey_Backpack, value );
	}
	if (strstr(name, "key_eat")) {
		return setCharKey( charKey_Eat, value );
	}
	if (strstr(name, "key_baby")) {
		return setCharKey( charKey_Baby, value );
	}
	if (strstr(name, "key_show_help")) {
		return setCharKey( charKey_ShowHelp, value );
	}
	if (strstr(name, "key_show_names")) {
		return setCharKey( charKey_ShowNames, value );
	}
	if (strstr(name, "key_show_cords")) {
		return setCharKey( charKey_ShowCords, value );
	}
	if (strstr(name, "key_show_playersinrange")) {
		return setCharKey( charKey_ShowPlayersInRange, value );
	}
	if (strstr(name, "key_show_deathmessages")) {
		return setCharKey( charKey_ShowDeathMessages, value );
	}
	if (strstr(name, "key_show_homecords")) {
		return setCharKey( charKey_ShowHomeCords, value );
	}
	if (strstr(name, "key_show_hostiletiles")) {
		return setCharKey( charKey_ShowHostileTiles, value );
	}
	if (strstr(name, "key_remembercords")) {
		return setCharKey( charKey_CreateHome, value );
	}
	if (strstr(name, "key_fixcamera")) {
		return setCharKey( charKey_FixCamera, value );
	}

	if (strstr(name, "init_show_names")) {
		iDrawNames = (int)(value[0]-'0');
		return true;
	}
	if (strstr(name, "init_show_cords")) {
		bDrawCords = bool(value[0]-'0');
		return true;
	}
	if (strstr(name, "init_show_playersinrange")) {
		bDrawPlayersInRangePanel = bool(value[0]-48);
		return true;
	}
	if (strstr(name, "init_show_deathmessages")) {
		bDrawDeathMessages = bool(value[0]-48);
		return true;
	}
	if (strstr(name, "init_show_homecords")) {
		bDrawHomeCords = bool(value[0]-48);
		return true;
	}
	if (strstr(name, "init_show_hostiletiles")) {
		bDrawHostileTiles = bool(value[0]-48);
		return true;
	}

	return false;
}

void HetuwMod::writeCharKeyToStream( ofstream &ofs, const char* keyName, char key ) {
	ofs << keyName << " = ";
	if (key == ' ')
		ofs << "<space>";
	else
		ofs << key;
	ofs << endl;
}

void HetuwMod::initSettings() {
	ifstream ifs( hetuwSettingsFileName );

	if (!ifs.good()) { // file doesnt exist
		//printf("hetuw creating %s\n", hetuwSettingsFileName);
		ofstream ofs( hetuwSettingsFileName, ofstream::out );

		writeCharKeyToStream( ofs, "key_up", charKey_Up );
		writeCharKeyToStream( ofs, "key_down", charKey_Down );
		writeCharKeyToStream( ofs, "key_left", charKey_Left );
		writeCharKeyToStream( ofs, "key_right", charKey_Right );
		writeCharKeyToStream( ofs, "key_center", charKey_TileStandingOn );
		ofs << endl;
		writeCharKeyToStream( ofs, "key_backpack", charKey_Backpack );
		writeCharKeyToStream( ofs, "key_eat", charKey_Eat );
		writeCharKeyToStream( ofs, "key_baby", charKey_Baby );
		ofs << endl;
		writeCharKeyToStream( ofs, "key_show_help", charKey_ShowHelp );
		writeCharKeyToStream( ofs, "key_show_names", charKey_ShowNames );
		writeCharKeyToStream( ofs, "key_show_cords", charKey_ShowCords );
		writeCharKeyToStream( ofs, "key_show_playersinrange", charKey_ShowPlayersInRange );
		writeCharKeyToStream( ofs, "key_show_deathmessages", charKey_ShowDeathMessages );
		writeCharKeyToStream( ofs, "key_show_homecords", charKey_ShowHomeCords );
		writeCharKeyToStream( ofs, "key_show_hostiletiles", charKey_ShowHostileTiles );
		ofs << endl;
		writeCharKeyToStream( ofs, "key_remembercords", charKey_CreateHome );
		writeCharKeyToStream( ofs, "key_fixcamera", charKey_FixCamera );
		ofs << endl;
		ofs << "init_show_names = " << (char)(iDrawNames+48) << endl;
		ofs << "init_show_cords = " << (char)(bDrawCords+48) << endl;
		ofs << "init_show_playersinrange = " << (char)(bDrawPlayersInRangePanel+48) << endl;
		ofs << "init_show_deathmessages = " << (char)(bDrawDeathMessages+48) << endl;
		ofs << "init_show_homecords = " << (char)(bDrawHomeCords+48) << endl;
		ofs << "init_show_hostiletiles = " << (char)(bDrawHostileTiles+48) << endl;

		ofs.close();
		return;
	}

	string line;
	while (getline(ifs, line)) {
		//printf("hetuw read line: %s\n", line.c_str());
		if (line.length() < 2) continue;
		char name[64];
		char value[64];
		getSettingsFileLine( name, value, line );
		if (strlen(name) < 1) continue;
		//printf("hetuw name: %s, value: %s\n", name, value);
		if (!setSetting( name, value ))
			printf("hetuw WARNING invalid %s line: %s\n", hetuwSettingsFileName, line.c_str());
	}
}

void HetuwMod::initOnBirth() {

	initOnServerJoin();

	currentEmote = -1;
	lastSpecialEmote = 0;

	playersInMap.clear();
	playersInMap.shrink_to_fit();

	playersInRangeNum = 0;
	youngWomenInRange = 0;

	deathMessages.clear();
	deathMessages.shrink_to_fit();

	homePosStack.clear();
	homePosStack.shrink_to_fit();

	cordOffset = { 0, 0 };
	addHomeLocation( 0, 0, false, 12 ); // add birth location
}

void HetuwMod::initOnServerJoin() {
	lastPosX = 9999;
	lastPosY = 9999;

	upKeyDown = false;
	downKeyDown = false;
	leftKeyDown = false;
	rightKeyDown = false;

	mapZoomInKeyDown = false;
	mapZoomInKeyDown = false;

	stopAutoRoadRun = false;
	activateAutoRoadRun = false;
	stopAutoRoadRunTime = 0;

	waitForDoorToOpen = false;
	lastDoorToOpenX = 9999;
	lastDoorToOpenY = 9999;
	
	bNextCharForHome = false;

	bDrawInputString = false;
	getCustomCords = 0;
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
	hetuwSetViewSize();
}

void HetuwMod::zoomIncrease(float value) {
	zoomScale *= 1.0f + value;
	if (zoomScale > 10.0f) zoomScale = 10.0f;
	zoomCalc();
}

void HetuwMod::zoomDecrease(float value) {
	zoomScale *= 1.0f - value;
	if (zoomScale < 1.0f) zoomScale = 1.0f;
	zoomCalc();
}

void HetuwMod::setZoom(float newZoom) {
	zoomScale = newZoom;
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

void HetuwMod::gameStep() {
	HetuwMouseActionBuffer* mouseBuffer = hetuwGetMouseActionBuffer();
	for (int i = 0; i < mouseBuffer->bufferPos; i++) {
		switch (mouseBuffer->buffer[i]) {
			case MouseButton::WHEELUP:
				zoomDecrease(HetuwMod::zoomValueScroll);
				break;
			case MouseButton::WHEELDOWN:
				zoomIncrease(HetuwMod::zoomValueScroll);
				break;
		}
	}
	mouseBuffer->Reset();
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
	if (bDrawMap && mapZoomInKeyDown) {
		mapScale *= 0.96;
		if (mapScale < 8.7333) mapScale = 8.73333;
	}
	else if (bDrawMap && mapZoomOutKeyDown) {
		mapScale *= 1.04;
		if (mapScale > 80177784) mapScale = 80177784;
	}

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

void HetuwMod::addHomeLocation( int x, int y, bool ancient, char c ) {
	int id = -1;
	if (c != 0) {
		bool cordsAlreadyExist = false;
		for (unsigned i=0; i<homePosStack.size(); i++) {
			if (c == homePosStack[i]->c) {
				id = i;
			}
			if (homePosStack[i]->x == x && homePosStack[i]->y == y) {
				cordsAlreadyExist = true;
			}
		}
		if (cordsAlreadyExist) return;
		if (id >= 0) {
			homePosStack[id]->x = x;
			homePosStack[id]->y = y;
			homePosStack[id]->ancient = ancient;
			return;
		}
	}
 
	for (unsigned i=0; i<homePosStack.size(); i++) {
		if (homePosStack[i]->x == x && homePosStack[i]->y == y) {
			id = i;
			break;
		}
	}
	if (id >= 0) return; // home already exists
	HomePos *p = new HomePos();
	p->x = x;
	p->y = y;
	p->ancient = ancient;
	p->c = c;
	homePosStack.push_back(p);
}

void HetuwMod::livingLifeDraw() {

 	ourLiveObject = livingLifePage->getOurLiveObject();
	if (!ourLiveObject) return;

	drawAge();

	if (bDrawCords) drawCords();

	if (bDrawPlayersInRangePanel) drawPlayersInRangePanel();

	if (bDrawDeathMessages) drawDeathMessages();

	if (bDrawHomeCords) drawHomeCords();

	if (bDrawHostileTiles) drawHostileTiles();

	if (bDrawMap) drawMap();

	if (bDrawInputString) drawInputString();

	if (bDrawHelp) drawHelp();

	//setDrawColor( 1.0, 0, 0, 1.0 );
	//drawRect( debugRecPos, 10, 10 );
	//setDrawColor( 0.0, 1.0, 0, 1.0 );
	//drawRect( debugRecPos2, 10, 10 );

}

void HetuwMod::drawTextWithBckgr( doublePair pos, const char* text ) {
	float textWidth = livingLifePage->hetuwMeasureScaledHandwritingFont( text, guiScale );
	setDrawColor( 0, 0, 0, 0.8 );
	drawRect( pos, (textWidth/2) + 6*guiScale, 14*guiScale );
	setDrawColor( 1, 1, 1, 1 );
	livingLifePage->hetuwDrawScaledHandwritingFont( text, pos, guiScale, alignCenter );
}

void HetuwMod::hDrawRect( doublePair startPos, doublePair endPos ) {
	double width = endPos.x - startPos.x;
	double height = endPos.y - startPos.y;
	width /= 2;
	height /= 2;
	startPos.x += width;
	startPos.y += height;
	drawRect( startPos, width, height );
}

void HetuwMod::drawTileRect( int x, int y ) {
	doublePair startPos = { (double)x, (double)y };
	startPos.x *= CELL_D;
	startPos.y *= CELL_D;
	drawRect( startPos, CELL_D/2, CELL_D/2 );
}

void HetuwMod::drawHostileTiles() {
	float alpha = 0.2;
	float interv = stepCount % 40 / (float)40;
	if (interv > 0.5) interv = 1 - interv;
	alpha += interv;
	setDrawColor( 1, 0, 0, alpha );
	//drawTileRect( ourLiveObject->xd, ourLiveObject->yd );

	int radius = 32;
	int startX = ourLiveObject->xd - radius;
	int endX = ourLiveObject->xd + radius;
	int startY = ourLiveObject->yd - radius;
	int endY = ourLiveObject->yd + radius;
	for (int x = startX; x < endX; x++) {
		for (int y = startY; y < endY; y++) {
			int objId = livingLifePage->hetuwGetObjId( x, y );
			if (objId > 0) {
				bool drawTile = false;
				drawTile = isDangerousAnimal( objId );
				if (drawTile) drawTileRect( x, y );
			}
		}
	}
}

void HetuwMod::drawInputString() {
	doublePair drawPosA = livingLifePage->hetuwGetLastScreenViewCenter();
	const char *sBufA = tempInputString.c_str();
	float textWidth = livingLifePage->hetuwMeasureScaledHandwritingFont( sBufA, guiScale );
	setDrawColor( 0, 0, 0, 0.8 );
	drawRect( drawPosA, (textWidth/2) + 6*guiScale, 14*guiScale );
	setDrawColor( 1, 1, 1, 1 );
	livingLifePage->hetuwDrawScaledHandwritingFont( sBufA, drawPosA, guiScale, alignCenter );
}

void HetuwMod::drawHomeCords() {
	if (homePosStack.size() <= 0) return;

	int mouseX, mouseY;
	livingLifePage->hetuwGetMouseXY( mouseX, mouseY );

	doublePair drawPosA = livingLifePage->hetuwGetLastScreenViewCenter();
	drawPosA.x -= HetuwMod::viewWidth/2 - (20*guiScale);
	drawPosA.y += HetuwMod::viewHeight/2 - (40*guiScale);
	drawPosA.y -= (40*guiScale);

	float biggestTextWidth = 0;
	char sBufA[64];
	int bellCount = 0;
	int markerCount = 0;
	for (unsigned i=0; i<homePosStack.size(); i++) {
		if (homePosStack[i]->c != 0) {
			if (homePosStack[i]->c == 12)
				sprintf( sBufA, "BIRTH %d %d", homePosStack[i]->x+cordOffset.x, homePosStack[i]->y+cordOffset.y );
			else 
				sprintf( sBufA, "%c %d %d", homePosStack[i]->c, homePosStack[i]->x+cordOffset.x, homePosStack[i]->y+cordOffset.y );
		} else if (homePosStack[i]->ancient) {
			sprintf( sBufA, "BELL %c %d %d", (char)(bellCount+65), homePosStack[i]->x+cordOffset.x, homePosStack[i]->y+cordOffset.y );
			bellCount++;
		} else {
			sprintf( sBufA, "HOME %c %d %d", (char)(markerCount+65), homePosStack[i]->x+cordOffset.x, homePosStack[i]->y+cordOffset.y );
			markerCount++;
		}
		float textWidth = livingLifePage->hetuwMeasureScaledHandwritingFont( sBufA, guiScale );
		if (textWidth > biggestTextWidth) biggestTextWidth = textWidth;
	}

	float recWidth = biggestTextWidth/2;
	float recHeight = homePosStack.size()*24*guiScale/2-12*guiScale;
	doublePair drawPosB = drawPosA;
	drawPosB.x += recWidth;
	drawPosB.y -= recHeight;
	setDrawColor( 0, 0, 0, 0.8 );
	drawRect( drawPosB, recWidth + 6*guiScale, recHeight + 14*guiScale );

	bellCount = 0;
	markerCount = 0;
	for (unsigned i=0; i<homePosStack.size(); i++) {
		if (homePosStack[i]->c != 0) {
			if (homePosStack[i]->c == 12) {
				sprintf( sBufA, "BIRTH %d %d", homePosStack[i]->x+cordOffset.x, homePosStack[i]->y+cordOffset.y );
				setDrawColor( 0.7, 0.6, 1.0, 1.0 );
			} else {
				sprintf( sBufA, "%c %d %d", homePosStack[i]->c, homePosStack[i]->x+cordOffset.x, homePosStack[i]->y+cordOffset.y );
				setDrawColor( 1.0, 1.0, 1.0, 1.0 );
			}
		} else if (homePosStack[i]->ancient) {
			sprintf( sBufA, "BELL %c %d %d", (char)(bellCount+65), homePosStack[i]->x+cordOffset.x, homePosStack[i]->y+cordOffset.y );
			bellCount++;
			setDrawColor( 1.0, 1.0, 0.2, 1.0 );
		} else {
			sprintf( sBufA, "HOME %c %d %d", (char)(markerCount+65), homePosStack[i]->x+cordOffset.x, homePosStack[i]->y+cordOffset.y );
			markerCount++;
			setDrawColor( 0.2, 0.8, 1.0, 1.0 );
		}
		livingLifePage->hetuwDrawScaledHandwritingFont( sBufA, drawPosA, guiScale );
		homePosStack[i]->drawStartPos.x = drawPosB.x-recWidth-6*guiScale;
		homePosStack[i]->drawEndPos.x = drawPosB.x+recWidth+6*guiScale;
		homePosStack[i]->drawEndPos.y = drawPosA.y+14*guiScale;
		homePosStack[i]->drawStartPos.y = drawPosA.y-14*guiScale;
		drawPosA.y -= 24*guiScale;
		if (mouseX >= homePosStack[i]->drawStartPos.x && mouseX <= homePosStack[i]->drawEndPos.x) {
			if (mouseY >= homePosStack[i]->drawStartPos.y && mouseY <= homePosStack[i]->drawEndPos.y) {
				setDrawColor( 1, 1, 1, 0.4 );
				hDrawRect( homePosStack[i]->drawStartPos, homePosStack[i]->drawEndPos );
			}
		}

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

bool HetuwMod::objIdReverseAction( int objId ) {
	if (objId <= 0) return false;

	bool r = false;
	if ( ourLiveObject->holdingID <= 0 ) {
		switch (objId) {
			case 253: // full berry clay bowl
			case 225: // wheat bundle
				return true;
				break;
		}
		if ( getObject(objId) ) {
			char* descr	= getObject(objId)->description;
			if ( strstr(descr, "Bowl of") != NULL ) {
				return true;
			}
		}
	}
	return r;
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
	if (objIdReverseAction(objId)) remove = true;
	
	if ( ourLiveObject->holdingID < 0 ) { // holding babay
		remove = false;
		use = false;
	}
	//printf("hetuw alphaActionObjId: %d\n", objId);

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

	if ( objIdReverseAction( objId ) ) use = true;

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

bool HetuwMod::isCharKey(unsigned char c, unsigned char key) {
	return (c == key || c == toupper(key));
}

bool HetuwMod::addToTempInputString( unsigned char c, bool onlyNumbers, int minStrLen ) {
	if (c == 8) { // EREASE
		if (tempInputString.length() <= (unsigned)minStrLen) return true;
		tempInputString = tempInputString.substr(0, tempInputString.length()-1);
		return true;
	}
	if (onlyNumbers) {
		if ((c < '0' || c > '9') && c != '-')
			return false;
	}
	tempInputString += c;
	return true;
}

// when return true -> end/return in keyDown function in LivingLife
bool HetuwMod::livingLifeKeyDown(unsigned char inASCII) {

	if (livingLifePage->hetuwSayFieldIsFocused()) {
		return false;
	}
	// player is not trying to say something

	//printf("hetuw key pressed %c, value: %i\n", inASCII, (int)inASCII);

	bool commandKey = isCommandKeyDown();
	bool shiftKey = isShiftKeyDown();

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
		getCustomCords = 0;
		bDrawInputString = false;
	}

	if (bNextCharForHome) {
		bNextCharForHome = false;
		char c = toupper(inASCII);
		if (c >= 65 && c <= 90) {
			addHomeLocation( ourLiveObject->xd, ourLiveObject->yd, false, c );
			bDrawHomeCords = true;
			return true;
		}
	}

	if (getCustomCords > 0) {
		if (getCustomCords == 1) {
			tempCordChar = toupper(inASCII);
			tempInputString = "X: ";
			getCustomCords++;
			bDrawInputString = true;
			return true;
		} else {
			if (inASCII == 13) { // ENTER
				if (getCustomCords == 3) {
					string cordStr = tempInputString.substr(3, tempInputString.length());
					try {
						tempCordY = stoi( cordStr );
					} catch(std::exception const & e) {
						getCustomCords = 0;
						bDrawInputString = false;
						return true;
					}
					addHomeLocation( tempCordX-cordOffset.x, tempCordY-cordOffset.y, false, tempCordChar );
					getCustomCords = 0;
					bDrawInputString = false;
					bDrawHomeCords = true;
					return true;
				}
				string cordStr = tempInputString.substr(3, tempInputString.length());
				try {
					tempCordX = stoi( cordStr );
				} catch(std::exception const & e) {
					getCustomCords = 0;
					bDrawInputString = false;
					return true;
				}
				tempInputString = "Y: ";
				getCustomCords++;
				return true;
			}
			addToTempInputString( inASCII, true, 3);
			return true;
		}
	}

	// for debugging
	if (true && inASCII == 'i') {
		int mouseX, mouseY;
		livingLifePage->hetuwGetMouseXY( mouseX, mouseY );
		int x = round( mouseX / (float)CELL_D );
		int y = round( mouseY / (float)CELL_D );
		int objId = livingLifePage->hetuwGetObjId( x, y );
		printf("hetuw cell: %i, %i objID: %i\n", x, y, objId);
		if (objId > 0) {
			ObjectRecord *o = getObject( objId );
			if (o && o->description) {
				printf("hetuw description: %s\n", o->description);
			}
		}
	}

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
	if (!commandKey && !shiftKey && isCharKey(inASCII, charKey_ShowCords)) {
		bDrawCords = !bDrawCords;
		return true;
	}
	if (!commandKey && shiftKey && isCharKey(inASCII, charKey_ShowCords)) {
		cordOffset.x = -ourLiveObject->xd;
		cordOffset.y = -ourLiveObject->yd;
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
	if (!commandKey && isCharKey(inASCII, charKey_ShowHomeCords)) {
		bDrawHomeCords = !bDrawHomeCords;
		return true;
	}
	if (!commandKey && !shiftKey && isCharKey(inASCII, charKey_CreateHome)) {
		bNextCharForHome = true;
		return true;
	}
	if (!commandKey && shiftKey && isCharKey(inASCII, charKey_CreateHome)) {
		getCustomCords = 1;
		return true;
	}
	if (!commandKey && isCharKey(inASCII, charKey_FixCamera)) {
		livingLifePage->hetuwToggleFixCamera();
		return true;
	}
	if (!bDrawMap && !commandKey && isCharKey(inASCII, charKey_ShowHostileTiles)) {
		bDrawHostileTiles = !bDrawHostileTiles;
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

	if (!commandKey && shiftKey && isCharKey(inASCII, charKey_ShowMap)) {
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

	//printf("hetuw unknown key %c, value: %i\n", inASCII, (int)inASCII);

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

	if (!isCommandKeyDown() && !isShiftKeyDown()) {
		if( inKeyCode == MG_KEY_LEFT ) { 
			zoomDecrease(HetuwMod::zoomValueKey);
		} else if( inKeyCode == MG_KEY_RIGHT ) { 
			zoomIncrease(HetuwMod::zoomValueKey);
		}
	}
	if (isCommandKeyDown()) {
		if( inKeyCode == MG_KEY_LEFT ) {
			guiScaleDecrease();
		} else if( inKeyCode == MG_KEY_RIGHT ) {
			guiScaleIncrease();
		}
	}

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

bool HetuwMod::livingLifePageMouseDown( float mX, float mY ) {

	if (bDrawHomeCords) {
		for (unsigned i=0; i<homePosStack.size(); i++) {
			if (mX >= homePosStack[i]->drawStartPos.x && mX <= homePosStack[i]->drawEndPos.x) {
				if (mY >= homePosStack[i]->drawStartPos.y && mY <= homePosStack[i]->drawEndPos.y) {
					cordOffset.x = -homePosStack[i]->x;
					cordOffset.y = -homePosStack[i]->y;
					return true;
				}
			}
		}
	}

	return false;
}

bool HetuwMod::isDangerousAnimal( int objId ) {
	if (objId <= 0) return false;
	for (int i = 0; i < dangerousAnimalsLength; i++) {
		if (objId == dangerousAnimals[i]) return true;
	}

	ObjectRecord *o = getObject( objId );
	if (!o) return false;

	if (strstr( o->description, "Wild Boar") != NULL)
		return true;
	if (strstr( o->description, "Domestic Boar") != NULL)
		return true;

	if (strstr( o->description, "Semi-tame Wolf") != NULL)
		return true;

	return false;
}

bool HetuwMod::tileIsSafeToWalk(int x, int y) {
	int objId = livingLifePage->hetuwGetObjId( x, y);
	if (objId > 0) {
		if (!tileHasNoDangerousAnimals(x, y)) return false;

		ObjectRecord* obj = getObject(objId);
		if (obj && obj->blocksWalking) {
			if (ourLiveObject->xd == x || ourLiveObject->yd == y)
				if (tileHasClosedDoor( x, y )) return true;
			return false;
		}
	}
	return true;
}

bool HetuwMod::tileHasNoDangerousAnimals(int x, int y) {
	int objId = livingLifePage->hetuwGetObjId( x, y);
	if (objId <= 0) return true;
	if (ourLiveObject->holdingID > 0 && getObject(ourLiveObject->holdingID)->rideable) {
		if (ourLiveObject->holdingID == 2395 || // Crude Car with Empty Tank
			ourLiveObject->holdingID == 2400 || // Crude Car with Empty Tank#driven
			ourLiveObject->holdingID == 2396 || // Running Crude Car
			ourLiveObject->holdingID == 2394) { // Unpowered Crude Car
				return true; // no dangerous animals for cars
		}
		// check dangerous animals for horses
		if (objId == 764) return false; // Rattle Snake	
		if (objId == 1385) return false; // Attacking Rattle Snake
	} else { // moving by walking / not riding
		if (isDangerousAnimal(objId)) return false;
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
// 1341060 2464 0 0 0 0 798 0 0 0 -1 0.24 0 0 X X 50.67 60.00 2.81 2885;202;0;0;200;198,560,3101 0 0 -1 0 reason_killed_152
void HetuwMod::onPlayerUpdate( LiveObject* inO, const char* line ) {
	if ( inO == NULL ) return;
	if ( ourLiveObject == NULL ) return;

	bool isDeathMsg = ( strstr( line, "X X" ) != NULL );
	if ( !isDeathMsg ) return;

	//printf("hetuw %s\n", line);

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
	deathMsg->description = new char[128];

	string strLine(line);
	string reasonKilled = "reason_killed_"; // reason_killed_
	size_t reasonKilledIndex = strLine.find(reasonKilled);
	if (reasonKilledIndex != string::npos) {
		deathMsg->deathReason = 2; // killer
		string sstr = strLine.substr(reasonKilledIndex + reasonKilled.length());
		string strKillerId;
		for (unsigned i = 0; i < sstr.length(); i++) {
			if (sstr[i] < '0' || sstr[i] > '9') break;
			strKillerId += sstr[i]; 
		}
		int killerObjId = stoi(strKillerId); // object id - like knife or grizzly bear
		if (isDangerousAnimal(killerObjId)) {
			deathMsg->deathReason = 1; // animal
		}
		ObjectRecord *ko = getObject( killerObjId );
		if ( ko && ko->description ) {
			char capitalDesc[128];
			int k = 0;
			for ( ; ko->description[k] != 0 && k < 128; k++) {
				if (ko->description[k] == '#') break;
				capitalDesc[k] = toupper(ko->description[k]);
			}
			capitalDesc[k] = 0;
			sprintf( deathMsg->description, "KILLED BY %s", capitalDesc );
		} else {
			sprintf( deathMsg->description, "KILLED BY ID %i", killerObjId );
		}
	} else if ( strLine.find("reason_hunger") != string::npos ) {
		sprintf( deathMsg->description, "KILLED BY STARVATION" );
	} else if ( strLine.find("reason_SID") != string::npos ) {
		sprintf( deathMsg->description, "SUDDEN INFANT DEATH" );
	} else if ( strLine.find("reason_age") != string::npos ) {
		sprintf( deathMsg->description, "KILLED BY OLD AGE" );
	} else {
		delete[] deathMsg->description;
		deathMsg->description = NULL;
	}

	deathMsg->timeReci = time(NULL);

	deathMsg->name = new char[64];
	sprintf( deathMsg->name, "%s", o->name); 

	deathMsg->age = (int)livingLifePage->hetuwGetAge( o );
	if ( getObject( o->displayID ) )
		deathMsg->male = getObject( o->displayID )->male;

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

	if ( dm->deathReason == 1 ) setDrawColor( 1, 0.8, 0, 1 ); // animal
	else if ( dm->deathReason == 2 ) setDrawColor( 1, 0.2, 0, 1 ); // killer
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

	int mouseX, mouseY;
	livingLifePage->hetuwGetMouseXY( mouseX, mouseY );

	float recStartX = recDrawPos.x - textWidth/2 - 10*guiScale;
	float recEndX = recDrawPos.x + textWidth/2 + 10*guiScale;
	float recStartY = recDrawPos.y - 20*guiScale;
	float recEndY = recDrawPos.y + 20*guiScale;

	if (mouseX >= recStartX && mouseX <= recEndX) {
		if (mouseY >= recStartY && mouseY <= recEndY) {
			doublePair descDrawPos = { (double)mouseX, (double)mouseY };
			if ( dm->description )
				drawTextWithBckgr( descDrawPos, dm->description );
			else
				drawTextWithBckgr( descDrawPos, "UNKNOWN DEATH" );
		}
	}

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
	sprintf( strZoomKeys, "USE %c/%c TO ZOOM IN/OUT - CLOSE IT WITH SHIFT+%c", toupper(charKey_MapZoomIn), toupper(charKey_MapZoomOut), toupper(charKey_ShowMap)); 
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
	int x = round(ourLiveObject->currentPos.x+cordOffset.x);
	int y = round(ourLiveObject->currentPos.y+cordOffset.y);

	char sBufA[16];
	sprintf(sBufA, "%d", x );
	float textWidthA = livingLifePage->hetuwMeasureScaledHandwritingFont( sBufA, guiScale );
	char sBufB[16];
	sprintf(sBufB, "%d", y );
	float textWidthB = livingLifePage->hetuwMeasureScaledHandwritingFont( sBufB, guiScale );

	doublePair drawPosA = livingLifePage->hetuwGetLastScreenViewCenter();
	doublePair drawPosB;
	drawPosA.x -= HetuwMod::viewWidth/2 - (20*guiScale);
	drawPosA.y += HetuwMod::viewHeight/2 - (40*guiScale);
	drawPosB.x = drawPosA.x + (20*guiScale) + textWidthA;
	drawPosB.y = drawPosA.y;

	doublePair drawPosC = drawPosA;
	drawPosC.x += textWidthA/2;
	doublePair drawPosD = drawPosB;
	drawPosD.x += textWidthB/2;
	setDrawColor( 0, 0, 0, 1 );
	drawRect( drawPosC, textWidthA/2 + 6*guiScale, 16*guiScale );
	drawRect( drawPosD, textWidthB/2 + 6*guiScale, 16*guiScale );

	if (x < 0) setDrawColor( 1, 0.8, 0, 1 );
	else setDrawColor( 0.2, 1, 0.2, 1 );
	livingLifePage->hetuwDrawScaledHandwritingFont( sBufA, drawPosA, guiScale );
	if (y < 0) setDrawColor( 1, 0.8, 0, 1 );
	else setDrawColor( 0.2, 1, 0.2, 1 );
	livingLifePage->hetuwDrawScaledHandwritingFont( sBufB, drawPosB, guiScale );
}

void HetuwMod::drawHelp() {
	float guiScale = (guiScaleRaw+0.1) * zoomScale;
	char str[128];
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

	drawPos.y -= lineHeight;
	sprintf(str, "YOU CAN CHANGE KEYS AND SETTINGS BY MODIFYING THE HETUW.CFG FILE");
	livingLifePage->hetuwDrawScaledHandwritingFont( str, drawPos, guiScale );
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
	sprintf(str, "%c TOGGLE FIX CAMERA", toupper(charKey_FixCamera));
	livingLifePage->hetuwDrawScaledHandwritingFont( str, drawPos, guiScale );
	drawPos.y -= lineHeight;
	sprintf(str, "%c TOGGLE SHOW NAMES", toupper(charKey_ShowNames));
	livingLifePage->hetuwDrawScaledHandwritingFont( str, drawPos, guiScale );
	drawPos.y -= lineHeight;
	sprintf(str, "%c TOGGLE SHOW CORDS", toupper(charKey_ShowCords));
	livingLifePage->hetuwDrawScaledHandwritingFont( str, drawPos, guiScale );
	drawPos.y -= lineHeight;
	sprintf(str, "%c TOGGLE SHOW PLAYERS IN RANGE", toupper(charKey_ShowPlayersInRange));
	livingLifePage->hetuwDrawScaledHandwritingFont( str, drawPos, guiScale );
	drawPos.y -= lineHeight;
	sprintf(str, "%c TOGGLE SHOW DEATH MESSAGES", toupper(charKey_ShowDeathMessages));
	livingLifePage->hetuwDrawScaledHandwritingFont( str, drawPos, guiScale );
	drawPos.y -= lineHeight;
	sprintf(str, "%c TOGGLE SHOW HOME CORDS", toupper(charKey_ShowHomeCords));
	livingLifePage->hetuwDrawScaledHandwritingFont( str, drawPos, guiScale );
	drawPos.y -= lineHeight;
	sprintf(str, "%c TOGGLE SHOW HOSTILE TILES", toupper(charKey_ShowHostileTiles));
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
	else sprintf(str, "%c - USE/PICK UP ITEM ON THE TILE YOU ARE STANDING ON", toupper(charKey_TileStandingOn));
	livingLifePage->hetuwDrawScaledHandwritingFont( str, drawPos, guiScale );
	drawPos.y -= lineHeight;
	if (charKey_TileStandingOn == ' ') sprintf(str, "CTRL+SPACE - DROP / PICK ITEM FROM CONTAINER");
	else sprintf(str, "CTRL+%c - DROP / PICK ITEM FROM CONTAINER", toupper(charKey_TileStandingOn));
	livingLifePage->hetuwDrawScaledHandwritingFont( str, drawPos, guiScale );
	drawPos.y -= lineHeight;
	livingLifePage->hetuwDrawScaledHandwritingFont( "LEFTARROWKEY ZOOM IN", drawPos, guiScale );
	drawPos.y -= lineHeight;
	livingLifePage->hetuwDrawScaledHandwritingFont( "RIGHTARROWKEY ZOOM OUT", drawPos, guiScale );
	drawPos.y -= lineHeight;
	livingLifePage->hetuwDrawScaledHandwritingFont( "CTRL+ARROWKEYS SCALE GUI", drawPos, guiScale );
	drawPos.y -= lineHeight;
	sprintf(str, "%c THEN KEY - REMEMBER CORDS", toupper(charKey_CreateHome));
	livingLifePage->hetuwDrawScaledHandwritingFont( str, drawPos, guiScale );
	drawPos.y -= lineHeight;
	sprintf(str, "SHIFT+%c THEN KEY - REMEMBER CUSTOM CORDS", toupper(charKey_CreateHome));
	livingLifePage->hetuwDrawScaledHandwritingFont( str, drawPos, guiScale );
	drawPos.y -= lineHeight;
	sprintf(str, "SHIFT+%c - RESET CORDS TO WHERE YOU ARE STANDING", toupper(charKey_ShowCords));
	livingLifePage->hetuwDrawScaledHandwritingFont( str, drawPos, guiScale );
	drawPos.y -= lineHeight;
}
