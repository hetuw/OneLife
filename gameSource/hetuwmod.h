#ifndef HETUWMOD_H
#define HETUWMOD_H

#define hetuwWaitingText "press %c in game for help"
#define hetuwSettingsFileName "hetuw.cfg"

#include "LivingLifePage.h"
#include <vector>
#include <string>

using namespace std;
                            
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

	struct DeathMsg {
		time_t timeReci;
		char* name;
		float nameColor[3];
		int age;
		bool male;
		int deathReason;
		char *description; // can be NULL
		DeathMsg() {
			name = NULL;
			deathReason = 0;
			description = NULL;
		}
		~DeathMsg() {
			if (name) {
				delete[] name;
				name = NULL;
			}
			if (description) {
				delete[] description;
				description = NULL;
			}
		}
	};

	#define hetuwDefaultOurFamilyName "MYFAMILY"
	#define hetuwMaxFamilyNameLen 13
	struct FamilyInRange {
		char* name;
		int count;
		int youngWomenCount;
		FamilyInRange() {
			name = new char[hetuwMaxFamilyNameLen];
			count = 0;
			youngWomenCount = 0;
		}
		~FamilyInRange() {
			if (name) { delete name; name = NULL; }
		}
		void reset() {
			count = 0;
			youngWomenCount = 0;
		}
		void setName(const char* inName) {
			int i=0;
			for ( ; i < (hetuwMaxFamilyNameLen-1); i++) {
				name[i] = inName[i];
				if (inName[i] == 0) break;
			}
			name[i] = 0;
		}
		bool nameEqualsName(const char* inName) {
			for (int i=0; i < (hetuwMaxFamilyNameLen-1); i++) {
				if (inName[i] == 0) break;
				if (name[i] != inName[i]) return false;
			}
			return true;
		}
	};

public:

	typedef struct {
		int x;
		int y;
		bool ancient;
		char c;
		doublePair drawStartPos;
		doublePair drawEndPos;
	} HomePos;

	static int maxObjects;

	static const int defaultViewWidth = 1280;
	static const int defaultViewHeight = 720;

	static constexpr float zoomValueKey = 0.25f;
	static constexpr float zoomValueScroll = 0.15f;

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
	static int tutMessageOffsetX;
	static int tutMessageOffsetX2;
	
	static unsigned char charKey_Up;
	static unsigned char charKey_Down;
	static unsigned char charKey_Left;
	static unsigned char charKey_Right;
	static unsigned char charKey_TileStandingOn;

	static unsigned char charKey_Backpack;
	static unsigned char charKey_Eat;
	static unsigned char charKey_Baby;

	static unsigned char charKey_ShowHelp;
	static unsigned char charKey_ShowNames;
	static unsigned char charKey_ShowCords;
	static unsigned char charKey_ShowPlayersInRange;
	static unsigned char charKey_ShowDeathMessages;
	static unsigned char charKey_ShowHomeCords;
	static unsigned char charKey_ShowHostileTiles;
	static unsigned char charKey_xRay;
	static unsigned char charKey_Search;

	static unsigned char charKey_ShowMap;
	static unsigned char charKey_MapZoomIn;
	static unsigned char charKey_MapZoomOut;

	static unsigned char charKey_CreateHome;
	static unsigned char charKey_FixCamera;

	static bool isCharKey( unsigned char c, unsigned char charKey );

	static time_t lastSpecialEmote;

	static bool stopAutoRoadRun;
	static time_t stopAutoRoadRunTime;
	static bool activateAutoRoadRun;

	static int iDrawNames;

	static RainbowColor *colorRainbow;

	static LivingLifePage *livingLifePage;

	static int stepCount;
	static double ourAge;

	static SimpleVector<LiveObject> *gameObjects;
	static std::vector<PlayerInMap*> playersInMap;
	static SimpleVector<int> *mMapContainedStacks;
	static SimpleVector<SimpleVector<int>> *mMapSubContainedStacks;
	static int *mMapD;

	static void pickUpBaby( int x, int y );
	static bool playerIsInCloseRange( LiveObject* o );
	static void pickUpBabyInRange();

	static void init();
	static void initOnBirth();
	static void initOnServerJoin();

	static void setLivingLifePage(LivingLifePage *inLivingLifePage, SimpleVector<LiveObject> *inGameObjects,
							SimpleVector<int> *inmMapContainedStacks, SimpleVector<SimpleVector<int>> *inmMapSubContainedStacks,
							int &inmMapD);

	static void zoomIncrease(float value);
	static void zoomDecrease(float value);
	static void setZoom(float newZoom);
	static void guiScaleIncrease();
	static void guiScaleDecrease();
	
	static void useTileRelativeToMe( int x, int y );
	static void dropTileRelativeToMe( int x, int y );
	static void remvTileRelativeToMe( int x, int y );
	
	static void actionAlphaRelativeToMe( int x, int y );
	static void actionBetaRelativeToMe( int x, int y );

	static void useBackpack( bool replace=false);
	static void useOnSelf();

	static void gameStep();

	static void livingLifeStep();
	static void livingLifeDraw();
	static bool livingLifeKeyDown(unsigned char inASCII);
	static bool livingLifeKeyUp(unsigned char inASCII);
	static bool livingLifeSpecialKeyDown(unsigned char inKeyCode);
	static bool livingLifePageMouseDown( float mX, float mY );

	static void setEmote(int id);

	static bool tileIsSafeToWalk(int x, int y); 
	static bool tileHasNoDangerousAnimals(int x, int y);
	static bool tileHasClosedDoor(int x, int y);

	static void setOurFamilyName(const char* lastName);
	static void getOurFamilyName();
	static bool isRelated( LiveObject* player );
	static void getRelationNameColor( const char* name, float* color );
	static void drawPlayerNames( LiveObject* player );

	static void drawTextWithBckgr( doublePair pos, const char* text );

	static void hDrawRect( doublePair startPos, doublePair endPos );
	static void drawTileRect( int x, int y );

	static void onPlayerUpdate( LiveObject* o, const char* line );
	static void removeLastName(char *newName, const char* name );
	static void getLastName( char* lastName, const char* name );
	static void setLastNameColor( const char* lastName, float alpha );

	static int playersInRangeNum;

	static std::vector<HomePos*> homePosStack;
	static void addHomeLocation( int x, int y, bool ancient = false, char c = 0 );

	static GridPos cordOffset;

	static void getSettingsFileLine( char* name, char* value, string line );
	static bool setSetting( const char* name, const char* value );
	static bool setCharKey( unsigned char &key, const char *value );

	static bool strContainsDangerousAnimal(const char* str);
	static bool isDangerousAnimal( int objId );

	static void setTakingPhoto( bool inTakingPhoto );
	static bool takingPhoto;

	static bool bxRay;

	static bool bFoundFamilyName;
	static std::vector<FamilyInRange*> familiesInRange;
	static string ourFamilyName;

	static bool charArrContainsCharArr(const char* arr1, const char* arr2);
	static void objDescrToUpper(const char* arr, char* output, int maxSize);
	
	static bool *objIsBeingSearched;
	static void SetSearchArray();

	static bool cameraIsFixed;
	static void SetFixCamera(bool b);

private:

 	static LiveObject *ourLiveObject;

	static void zoomCalc();

	static bool bDrawHelp;
	static void drawHelp();
	static void setHelpColorNormal();
	static void setHelpColorSpecial();

	static bool bDrawCords;
	static void drawCords();

	static bool bDrawHostileTiles;
	static void drawHostileTiles();

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
	static void initSettings();

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

	static void updateMap();

	static int iDrawPlayersInRangePanel;
	static void updatePlayersInRangePanel();
	static void drawPlayersInRangePanel();

	static void drawSearchList();

	static float mapScale;
	static float mapOffsetX;
	static float mapOffsetY;

	static bool bDrawDeathMessages;
	static void drawDeathMessages();
	static std::vector<DeathMsg*> deathMessages;

	static bool objIdReverseAction( int objId );

	static bool bDrawHomeCords;
	static void drawHomeCords();
	static bool bNextCharForHome;

	static void writeCharKeyToStream( ofstream &ofs, const char* keyName, char key );

	static void drawInputString();
	static bool addToTempInputString( unsigned char c, bool onlyNumbers, int minStrLen );
	static bool bDrawInputString;
	static string tempInputString;

	static int getCustomCords;
	static char tempCordChar;
	static int tempCordX;
	static int tempCordY;

	static void drawSearchTiles();
	static void drawSearchTilesLoop(bool drawText);
	static int getSearchInput;
	static std::vector<char*> searchWordList;
	static bool bDrawSearchList;
	static int drawSearchListTopY;
	static std::vector<doublePair*> searchWordStartPos;
	static std::vector<doublePair*> searchWordEndPos;
	static std::vector<bool> searchWordListDelete;

};


#endif
