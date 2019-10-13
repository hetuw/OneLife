#ifndef HETUWMOD_H
#define HETUWMOD_H

#define hetuwSayDelay 2.1

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

	static constexpr int languageArraySize1 = 460;
	static constexpr int languageArraySize2 = 9;
	static constexpr char languageArray[460][9] = { "A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L", "M", "N", "O", "P", "Q", "R", "S", "T", "U", "V", "W", "X", "Y", "Z", "PEACE", "FIRE", "ABOUT", "GIRL", "ALL", "BEAR", "ALSO", "BERRY", "AND", "RADIO", "AS", "GRIEFER", "AT", "HELP", "BE", "FOOD", "BECAUSE", "FARM", "BUT", "WHEAT", "BY", "MILKWEED", "CAN", "STRING", "COME", "THREAD", "COULD", "NEEDLE", "DAY", "STONE", "DO", "GATE", "EVEN", "KEROSENE", "FIND", "PLS", "FIRST", "WASTES", "FOR", "THX", "FROM", "TY", "GET", "TEACH", "GIVE", "NEED", "GO", "ROAD", "HAVE", "CARROTS", "HE", "BOWL", "HER", "WATER", "HERE", "KNIFE", "HIM", "SHEEP", "HIS", "STOP", "HOW", "BOW", "TOWN", "IF", "NORTH", "IN", "FAMILY", "INTO", "WEST", "IT", "EAST", "ITS", "SOUTH", "JUST", "HELLO", "KNOW", "RABBITS", "LIKE", "CLOTHES", "LOOK", "START", "MAKE", "HEY", "MAN", "PLANT", "MANY", "SOIL", "ME", "COMPOST", "MORE", "HIDE", "MY", "CORN", "NEW", "STEW", "NO", "PIGS", "NOT", "BUCKET", "NOW", "ENGINE", "OF", "HORSE", "ON", "CART", "ONE", "BASKET", "ONLY", "OR", "CLAY", "OTHER", "WALL", "OUR", "ADOBE", "OUT", "TOOLS", "PEOPLE", "HOE", "SAY", "SHOVEL", "SEE", "AXE", "SHE", "PICK", "SO", "BRANCH", "SOME", "CHISEL", "TAKE", "MALLET", "TELL", "CURSE", "THAN", "TWINS", "THAT", "FROE", "THE", "FENCE", "THEIR", "RUBBER", "THEM", "HEAL", "THEN", "OIL", "THERE", "DIESEL", "THESE", "BACKPACK", "THEY", "PUMP", "THING", "SKEWER", "THINK", "OLD", "THIS", "FLINT", "THOSE", "SHARP", "TIME", "ROPE", "TO", "LASSO", "TWO", "FORGE", "UP", "STEEL", "USE", "SNARE", "VERY", "KINDLING", "WANT", "PLATES", "WAY", "PIE", "WE", "BAKE", "WELL", "JUNGLE", "WHAT", "DESERT", "WHEN", "FERTILE", "WHICH", "CISTERN", "WHO", "MOTHER", "WILL", "SISTER", "WITH", "BROTHER", "WOULD", "GOOD", "YEAR", "EVIL", "YOU", "DAUGTHER", "YOUR", "SON", "BIG", "FAR", "CLOSE", "FEED", "YES", "KILLED", "GRIEFS", "MURDER", "FOLLOW", "BABIES", "PADS", "ARROW", "GOOSE", "COAL", "PAPER", "PENCIL", "SULFUR", "BELLOWS", "MINE", "VEIN", "GOLD", "EVE", "NG", "ST", "OU", "EA", "SS", "BL", "NT", "SH", "TR", "CH", "IO", "PR", "TH", "CK", "OO", "EE", "IE", "AI", "ND", "GR", "PL", "CR", "BR", "LL", "CL", "FL", "SP", "NC", "PH", "AU", "RD", "DR", "AY", "RT", "FR", "UI", "GL", "SM", "UE", "OA", "SC", "OI", "RS", "IA", "RN", "STR", "SL", "UA", "CT", "GHT", "NK", "LD", "NS", "RK", "MP", "WH", "TCH", "EI", "SK", "RM", "DG", "SW", "EY", "LT", "WN", "SN", "SCR", "TL", "DL", "FF", "GH", "FT", "OY", "WR", "RG", "GN", "EU", "NCH", "LK", "THR", "IOU", "PT", "OE", "PS", "KN", "TT", "SQ", "AE", "RV", "RTH", "CS", "MB", "RCH", "LV", "RL", "TW", "TS", "YA", "RH", "SPR", "RC", "WL", "LM", "WS", "RB", "LS", "CHR", "SCH", "RR", "LF", "EYE", "SHR", "RP", "MS", "EAU", "PP", "NGS", "EO", "NN", "SPL", "ZL", "NTH", "YO", "YE", "SPH", "LP", "KL", "DS", "RST", "MPT", "UO", "LTH", "GG", "WK", "MN", "ZZ", "WD", "NX", "NDS", "TZ", "CKS", "RF", "EOU", "PHR", "RDS", "KR", "YEA", "NCT", "BT", "UY", "UEA", "RTS", "LG", "DD", "CHL", "NTS", "MM", "DW", "DH", "AYE", "RSH", "RKS", "NGTH", "KH", "GS", "YU", "UEE", "NKS", "LLS", "LCH", "IEU", "AA", "WZ", "HN", "BS", "UAI", "STH", "RLD", "PHL", "MPS", "IU", "XT", "RTZ", "NTZ", "GHTS", "CZ", "YI", "UOI", "WNS", "TTS", "RNT", "MPH", "LMS", "LB", "DTH", "WTH", "VR", "THW", "RRH", "RQ", "RNS", "RMS", "RGH", "HM", "GM", "FFS", "BB", "UOY", "THM", "SV", "SCL", "SCHL", "RPS", "PN"};

	static constexpr int OBJID_Fire = 82;
	static constexpr int OBJID_HotCoals = 85;
	static constexpr int OBJID_ClayBowl = 235;
	static constexpr int OBJID_HotAdobeOven = 250;

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
	static unsigned char charKey_TeachLanguage;
	static unsigned char charKey_FindYum;
	static unsigned char charKey_HidePlayers;

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
	static bool bDrawSelectedPlayerInfo;

	static RainbowColor *colorRainbow;

	static LivingLifePage *livingLifePage;

	static int stepCount;
	static double ourAge;

	static SimpleVector<LiveObject> *gameObjects;
	static std::vector<PlayerInMap*> playersInMap;
	static SimpleVector<int> *mMapContainedStacks;
	static SimpleVector<SimpleVector<int>> *mMapSubContainedStacks;
	static int *mMapD;

	static int *mCurMouseOverID;
	static int selectedPlayerID; 
	static void OnPlayerHoverOver(int id);
	static double timeLastPlayerHover;

	static void pickUpBaby( int x, int y );
	static bool playerIsInCloseRange( LiveObject* o );
	static void pickUpBabyInRange();

	static void init();
	static void initOnBirth();
	static void initOnServerJoin();

	static void setLivingLifePage(LivingLifePage *inLivingLifePage, SimpleVector<LiveObject> *inGameObjects,
							SimpleVector<int> *inmMapContainedStacks, SimpleVector<SimpleVector<int>> *inmMapSubContainedStacks,
							int &inmMapD, int &inmCurMouseOverID);

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

	static void setMoveDirection(int &x, int &y, int direction);
	static int getMoveDirection();
	static int getNextMoveDir(int direction, int add);
	static bool tileIsSafeToWalk(int x, int y); 
	static bool tileHasNoDangerousAnimals(int x, int y);
	static bool tileHasClosedDoor(int x, int y);
	static bool dirIsSafeToWalk(int x, int y, int dir);
	static bool findNextMove(int &x, int &y, int dir);

	static void setOurFamilyName(const char* lastName);
	static void getOurFamilyName();
	static bool isRelated( LiveObject* player );
	static void getRelationNameColor( const char* name, float* color );
	static void drawPlayerNames( LiveObject* player );
	static void drawHighlightedPlayer();

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
	static bool *isDangerousAnimal;

	static void setTakingPhoto( bool inTakingPhoto );
	static bool takingPhoto;

	static bool bxRay;
	static bool bHidePlayers;
	static char ourGender;

	static bool bFoundFamilyName;
	static std::vector<FamilyInRange*> familiesInRange;
	static string ourFamilyName;

	static bool charArrContainsCharArr(const char* arr1, const char* arr2);
	static void objDescrToUpper(const char* arr, char* output, int maxSize);
	
	static bool *objIsBeingSearched;
	static void setSearchArray();

	static bool cameraIsFixed;
	static void SetFixCamera(bool b);

	static void Say(const char *text);
	static void causeDisconnect();

	static float colorRainbowFast[3];
	static void stepColorRainbowFast();

	static void initBecomesFood();
	static int becomesFood( int objectID, int depth );
	static int *becomesFoodID;
	static SimpleVector<int> yummyFoodChain;
	static bool isYummy(int objID);
	static void foodIsMeh(int objID);
	static bool bDrawYum;
	static void setYumObjectsColor();
	static void resetObjectsColor();

	static double *objectDrawScale;
	static void resetObjectDrawScale();
	static void objectDrawScaleStep();

	static float *objectDefaultColors;

	static bool bHoldDownTo_FixCamera;
	static bool bHoldDownTo_XRay;
	static bool bHoldDownTo_FindYum;

	static bool b_drawYumColor;
	static bool b_drawYumPulsate;
	static bool b_drawSearchText;
	static bool b_drawSearchTileRec;
	static bool b_drawSearchPulsate;

	static bool bAutoDataUpdate;

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
	static bool setMoveDirIfSafe(int &x, int &y, int dir);
	static int magnetMoveDir;
	static int magnetWrongMoveDir;
	static int magnetMoveCount;

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

	static bool bTeachLanguage;
	static int teachLanguageCount;
	static double timeLastLanguage;
	static void teachLanguage();

	static void SayStep();
	static vector<char*> sayBuffer;
	static double timeLastSay;
	static bool clearSayBuffer;
};


#endif
