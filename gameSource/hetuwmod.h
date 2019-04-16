#ifndef HETUWMOD_H
#define HETUWMOD_H

#include "LivingLifePage.h"
#include <vector>
                            
class HetuwMod
{

	class RainbowColor {
	public:
		float color[3];
		RainbowColor();
		void step();
	private:
		const float stepSize = 0.03f;
		bool increase;
		int cycle; // from 0 - 2 / which color should change
	};

	struct PlayerInMap {
		int id;
		int x;
		int y;
		char* name;
		char* lastName;
		time_t lastTime;
		bool finalAgeSet;
		PlayerInMap() {
			name = NULL;
			lastName = NULL;
			x = 999999;
			y = 999999;
			id = -9999;
			finalAgeSet = false;
		}
		~PlayerInMap() {
			if (name) {
				delete[] name;
				name = NULL;
			}
			if (lastName) {
				delete[] lastName;
				lastName = NULL;
			}
		}
	};

public:

	static const int defaultViewWidth = 1280;
	static const int defaultViewHeight = 720;

	static int dangerousAnimalsLength;
	static int* dangerousAnimals;

	static int closedDoorIDsLength;
	static int* closedDoorIDs;

	static int viewWidth;
	static int viewHeight;

	static float zoomScale;
	static float guiScale;
	static float guiScaleRaw;
	static int panelOffsetX;
	static int panelOffsetY;
	
	static char charKey_Up;
	static char charKey_Down;
	static char charKey_Left;
	static char charKey_Right;
	static char charKey_TileStandingOn;

	static char charKey_Backpack;
	static char charKey_Eat;
	static char charKey_ShowHelp;
	static char charKey_ShowNames;
	static char charKey_ShowCords;

	static char charKey_ShowMap;
	static char charKey_MapZoomIn;
	static char charKey_MapZoomOut;

	static bool isCharKey( char c, char charKey );

	static time_t lastSpecialEmote;

	static bool stopAutoRoadRun;
	static time_t stopAutoRoadRunTime;
	static bool activateAutoRoadRun;

	static bool bDrawNames;

	static RainbowColor *colorRainbow;

	static LivingLifePage *livingLifePage;

	static int stepCount;
	static double ourAge;

	static SimpleVector<LiveObject> *gameObjects;
	static std::vector<PlayerInMap*> playersInMap;

	static void init();
	static void initOnBirth();

	static void setLivingLifePage(LivingLifePage *inLivingLifePage, SimpleVector<LiveObject> *inGameObjects);

	static void zoomIncrease();
	static void zoomDecrease();
	static void guiScaleIncrease();
	static void guiScaleDecrease();
	
	static void useTileRelativeToMe( int x, int y );
	static void dropTileRelativeToMe( int x, int y );
	static void remvTileRelativeToMe( int x, int y );
	
	static void actionAlphaRelativeToMe( int x, int y );
	static void actionBetaRelativeToMe( int x, int y );

	static void useBackpack( bool replace=false);
	static void useOnSelf();

	static void livingLifeStep();
	static void livingLifeDraw();
	static bool livingLifeKeyDown(unsigned char inASCII);
	static bool livingLifeKeyUp(unsigned char inASCII);
	static bool livingLifeSpecialKeyDown(unsigned char inKeyCode);

	static void setEmote(int id);

	static bool tileIsSafeToWalk(int x, int y); 
	static bool tileHasNoDangerousAnimals(int x, int y);
	static bool tileHasClosedDoor(int x, int y);

	static void getRelationNameColor( const char* name, float* color );
	static void drawPlayerNames( LiveObject* player );

	static void onPlayerUpdate( LiveObject* o, const char* deathReason = NULL );
	static void getLastName(char* lastName, const char* name);
	static void setLastNameColor( const char* lastName, float alpha );

private:

 	static LiveObject *ourLiveObject;

	static void zoomCalc();

	static bool bDrawHelp;
	static void drawHelp();

	static bool bDrawCords;
	static void drawCords();

	static bool bDrawMap;
	static void drawMap();

	static void drawAge();

	static float lastPosX;
	static float lastPosY;

	static bool upKeyDown;
	static bool downKeyDown;
	static bool leftKeyDown;
	static bool rightKeyDown;

	static bool mapZoomInKeyDown;
	static bool mapZoomOutKeyDown;
	
	static void initDangerousAnimals();
	static void initClosedDoorIDs();

	static void move(); // called only once on frame - dont use
	static bool cornerTileIsSafeToWalk( int sX, int sY, bool up, bool down, bool right, bool left);

	static bool waitForDoorToOpen;
	static int lastDoorToOpenX;
	static int lastDoorToOpenY;

	static doublePair debugRecPos;
	static doublePair debugRecPos2;

	static int currentEmote;
	static time_t lastEmoteTime;

	static float playerNameColor[3];
	static doublePair playerNamePos;

	static void drawPlayersInAreaInfo();
	static void updateMap();

	static float mapScale;
	static float mapOffsetX;
	static float mapOffsetY;

};


#endif
