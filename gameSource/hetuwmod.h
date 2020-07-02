#ifndef HETUWMOD_H
#define HETUWMOD_H

#define hetuwWaitingText "press %c in game for help"
#define hetuwSettingsFileName "hetuw.cfg"
#define hetuwLogFileName "hetuwlog.txt"
#define hetuwLogSeperator " | " // needs to be 3 char long
#define hetuwFakeCoord 1977
#define hetuwBitcoinWallet "bc1q66jzg06xxd8uup0svwhhwum23d3mqlrnsccf2j" // donate something :)
#define hetuwGetNewestVersionFromGithub "get newest version from github https://github.com/hetuw/OneLife/releases"
#define hetuwPhotoSize 400

#define hetuwLinkMainServer "bigserver2.onehouronelife.com"
#define hetuwLinkArcReport "http://onehouronelife.com/arcServer/arcReport.php"

#include <vector>
#include <string>

#include "LivingLifePage.h"
#include "hetuwFont.h"

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
		char gender;
		time_t lastTime;
		int age;
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
		std::string raceName = "UNKNOWN";
		int generation = 0;
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

	struct HttpRequest {
		string link;
		void (*callBackFunc)(const char*, string); // first argument contains data from website, second argument contains error
		string type; // for example GET
		int webRequest;
		time_t startTime; // in seconds
		int intervalSeconds; // if this is >= 0 than dont report error but instead try again in intervalSeconds
		bool waitForRetry;
		HttpRequest(string inLink, void (*inCallBackFunc)(const char*, string), string inType="GET", int inIntervalSeconds=60) {
			link = inLink;
			callBackFunc = inCallBackFunc;
			type = inType;
			intervalSeconds = inIntervalSeconds;
			start();
		}
		void start() {
			waitForRetry = false;
			webRequest = startWebRequest(type.c_str(), link.c_str(), NULL);
			startTime = time(NULL);
		}
		bool step() { // returns false if request is finished
			if (waitForRetry) {
				if (time(NULL)-startTime >= intervalSeconds) start();
				return true;
			}
			if (webRequest == -1) { // error
				if (intervalSeconds < 0) {
					callBackFunc("", "webRequest == -1");
					return false;
				} else {
					printf("hetuw Warning: HttpRequest failed %s\n", link.c_str());
					printf("hetuw webRequest == -1\n");
					printf("hetuw resend request in %i seconds\n", intervalSeconds-(int)(time(NULL)-startTime));
					waitForRetry = true;
					return true;
				}
			}
			int result = stepWebRequest(webRequest);
			if (result == 0) return true; // still waiting for request to complete
			if (result == -1) { // error
				clearWebRequest(webRequest);
				if (intervalSeconds < 0) {
					callBackFunc("", "result == -1");
					return false;
				} else {
					printf("hetuw Warning: HttpRequest failed %s\n", link.c_str());
					printf("hetuw result == -1\n");
					printf("hetuw resend request in %i seconds\n", intervalSeconds-(int)(time(NULL)-startTime));
					waitForRetry = true;
					return true;
				}
			}
			const char *resultText = getWebResult(webRequest);
			clearWebRequest(webRequest);
			webRequest = -1;
			callBackFunc(resultText, "");
			return false;
		}
	};

public:

	struct KeyboardKey {
		unsigned char c;
		double timePressedSince = -1;
	};

	// WARNING: unknown bugs will make this not work correctly
	// sometimes key down events will be receveived but not key up events,
	// causing it to spam a key, this will be different on different computer / OSs
	// idk the cause of this, might also be related to the underlying engine / sdl lib
	struct KeyHandler {
		KeyboardKey lastKeyPressed;
		void (*onKeyEvent)(unsigned char);
		// everything in seconds
		double waitTimeUntilRepeatSignal = 0.4;
		double repeatFrequency = 0.03;
		double timeSinceLastSignal = -1;

		KeyHandler(void (*inOnKeyEvent)(unsigned char)) {
			onKeyEvent = inOnKeyEvent;
		}
		void sendKeyEvent() {
			lastKeyPressed.c = isShiftKeyDown() ? toupper(lastKeyPressed.c) : tolower(lastKeyPressed.c);
			onKeyEvent(lastKeyPressed.c);
		}
		void onKeyDown(unsigned char c) {
			lastKeyPressed.c = c;
			lastKeyPressed.timePressedSince = curStepTime;
			sendKeyEvent();
		}
		void onKeyUp(unsigned char c) {
			lastKeyPressed.timePressedSince = -1;
			timeSinceLastSignal = -1;
		}
		void step() {
			if (lastKeyPressed.timePressedSince < 0) return;
			if (curStepTime - lastKeyPressed.timePressedSince < waitTimeUntilRepeatSignal) return;
			if (timeSinceLastSignal < 0) {
				sendKeyEvent();
				timeSinceLastSignal = curStepTime;
				return;
			}
			if (curStepTime - timeSinceLastSignal < repeatFrequency) return;
			sendKeyEvent();
			timeSinceLastSignal = curStepTime;
		}
	};

	struct IntervalTimed {
		double lastTime;
		double interval;
		IntervalTimed(double inInterval) {
			interval = inInterval;
			lastTime = 0;
		}
		bool step() {
			if (HetuwMod::curStepTime-lastTime < interval) return false;
			lastTime = HetuwMod::curStepTime;
			return true;
		}
	};

	enum homePosType {
		hpt_custom, hpt_birth, hpt_home, hpt_bell, hpt_apoc, hpt_tarr, hpt_map, hpt_baby, hpt_babyboy, hpt_babygirl, hpt_expert, hpt_phex,
	};

	typedef struct {
		int x;
		int y;
		char c = 0;
		string text = "";
		homePosType type;
		string drawStr;
		doublePair drawStartPos;
		doublePair drawEndPos;
		int personID = -1;
		bool hasCustomColor = false; // set it to true if you want rgb to be used
		float rgba[4];
	} HomePos;

	static constexpr int languageArraySize1 = 460;
	static constexpr int languageArraySize2 = 9;
	//static constexpr char languageArray[460][9] = { "A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L", "M", "N", "O", "P", "Q", "R", "S", "T", "U", "V", "W", "X", "Y", "Z", "PEACE", "FIRE", "ABOUT", "GIRL", "ALL", "BEAR", "ALSO", "BERRY", "AND", "RADIO", "AS", "GRIEFER", "AT", "HELP", "BE", "FOOD", "BECAUSE", "FARM", "BUT", "WHEAT", "BY", "MILKWEED", "CAN", "STRING", "COME", "THREAD", "COULD", "NEEDLE", "DAY", "STONE", "DO", "GATE", "EVEN", "KEROSENE", "FIND", "PLS", "FIRST", "WASTES", "FOR", "THX", "FROM", "TY", "GET", "TEACH", "GIVE", "NEED", "GO", "ROAD", "HAVE", "CARROTS", "HE", "BOWL", "HER", "WATER", "HERE", "KNIFE", "HIM", "SHEEP", "HIS", "STOP", "HOW", "BOW", "TOWN", "IF", "NORTH", "IN", "FAMILY", "INTO", "WEST", "IT", "EAST", "ITS", "SOUTH", "JUST", "HELLO", "KNOW", "RABBITS", "LIKE", "CLOTHES", "LOOK", "START", "MAKE", "HEY", "MAN", "PLANT", "MANY", "SOIL", "ME", "COMPOST", "MORE", "HIDE", "MY", "CORN", "NEW", "STEW", "NO", "PIGS", "NOT", "BUCKET", "NOW", "ENGINE", "OF", "HORSE", "ON", "CART", "ONE", "BASKET", "ONLY", "OR", "CLAY", "OTHER", "WALL", "OUR", "ADOBE", "OUT", "TOOLS", "PEOPLE", "HOE", "SAY", "SHOVEL", "SEE", "AXE", "SHE", "PICK", "SO", "BRANCH", "SOME", "CHISEL", "TAKE", "MALLET", "TELL", "CURSE", "THAN", "TWINS", "THAT", "FROE", "THE", "FENCE", "THEIR", "RUBBER", "THEM", "HEAL", "THEN", "OIL", "THERE", "DIESEL", "THESE", "BACKPACK", "THEY", "PUMP", "THING", "SKEWER", "THINK", "OLD", "THIS", "FLINT", "THOSE", "SHARP", "TIME", "ROPE", "TO", "LASSO", "TWO", "FORGE", "UP", "STEEL", "USE", "SNARE", "VERY", "KINDLING", "WANT", "PLATES", "WAY", "PIE", "WE", "BAKE", "WELL", "JUNGLE", "WHAT", "DESERT", "WHEN", "FERTILE", "WHICH", "CISTERN", "WHO", "MOTHER", "WILL", "SISTER", "WITH", "BROTHER", "WOULD", "GOOD", "YEAR", "EVIL", "YOU", "DAUGTHER", "YOUR", "SON", "BIG", "FAR", "CLOSE", "FEED", "YES", "KILLED", "GRIEFS", "MURDER", "FOLLOW", "BABIES", "PADS", "ARROW", "GOOSE", "COAL", "PAPER", "PENCIL", "SULFUR", "BELLOWS", "MINE", "VEIN", "GOLD", "EVE", "NG", "ST", "OU", "EA", "SS", "BL", "NT", "SH", "TR", "CH", "IO", "PR", "TH", "CK", "OO", "EE", "IE", "AI", "ND", "GR", "PL", "CR", "BR", "LL", "CL", "FL", "SP", "NC", "PH", "AU", "RD", "DR", "AY", "RT", "FR", "UI", "GL", "SM", "UE", "OA", "SC", "OI", "RS", "IA", "RN", "STR", "SL", "UA", "CT", "GHT", "NK", "LD", "NS", "RK", "MP", "WH", "TCH", "EI", "SK", "RM", "DG", "SW", "EY", "LT", "WN", "SN", "SCR", "TL", "DL", "FF", "GH", "FT", "OY", "WR", "RG", "GN", "EU", "NCH", "LK", "THR", "IOU", "PT", "OE", "PS", "KN", "TT", "SQ", "AE", "RV", "RTH", "CS", "MB", "RCH", "LV", "RL", "TW", "TS", "YA", "RH", "SPR", "RC", "WL", "LM", "WS", "RB", "LS", "CHR", "SCH", "RR", "LF", "EYE", "SHR", "RP", "MS", "EAU", "PP", "NGS", "EO", "NN", "SPL", "ZL", "NTH", "YO", "YE", "SPH", "LP", "KL", "DS", "RST", "MPT", "UO", "LTH", "GG", "WK", "MN", "ZZ", "WD", "NX", "NDS", "TZ", "CKS", "RF", "EOU", "PHR", "RDS", "KR", "YEA", "NCT", "BT", "UY", "UEA", "RTS", "LG", "DD", "CHL", "NTS", "MM", "DW", "DH", "AYE", "RSH", "RKS", "NGTH", "KH", "GS", "YU", "UEE", "NKS", "LLS", "LCH", "IEU", "AA", "WZ", "HN", "BS", "UAI", "STH", "RLD", "PHL", "MPS", "IU", "XT", "RTZ", "NTZ", "GHTS", "CZ", "YI", "UOI", "WNS", "TTS", "RNT", "MPH", "LMS", "LB", "DTH", "WTH", "VR", "THW", "RRH", "RQ", "RNS", "RMS", "RGH", "HM", "GM", "FFS", "BB", "UOY", "THM", "SV", "SCL", "SCHL", "RPS", "PN"};

	static constexpr int OBJID_SharpStone = 34;
	static constexpr int OBJID_Fire = 82;
	static constexpr int OBJID_HotCoals = 85;
	static constexpr int OBJID_ClayBowl = 235;
	static constexpr int OBJID_HotAdobeOven = 250;

	static constexpr int OBJID_BellTowerSound = 839;
	static constexpr int OBJID_EndTowerSound = 2481;

	static constexpr int OBJID_TarrMonument = 3112;

	static const int defaultViewWidth = 1280;
	static const int defaultViewHeight = 720;

	static constexpr float zoomValueKey = 0.25f;
	static constexpr float zoomValueScroll = 0.15f;

	static constexpr int spriteBodyWhiteID = 53;

	static int maxObjects;

	static int dangerousAnimalsLength;
	static int* dangerousAnimals;

	static int closedDoorIDsLength;
	static int* closedDoorIDs;

	static int viewWidth;
	static int viewHeight;
	static double viewWidthToHeightFactor;
	static double viewHeightToWidthFactor;

	static doublePair fromViewToMapCoordsVec;

	static float zoomScale;
	static float guiScale;
	static float guiScaleRaw;
	static int panelOffsetX;
	static int panelOffsetY;
	static int tutMessageOffsetX;
	static int tutMessageOffsetX2;
	
	static int cfgVersionNumber;
	static int cfgVersionRead;

	static unsigned char charKey_Up;
	static unsigned char charKey_Down;
	static unsigned char charKey_Left;
	static unsigned char charKey_Right;
	static unsigned char charKey_TileStandingOn;

	static unsigned char charKey_Backpack;
	static unsigned char charKey_TakeOffBackpack;
	static unsigned char charKey_Pocket;
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
	static unsigned char charKey_ShowGrid;
	static unsigned char charKey_MakePhoto;
	static unsigned char charKey_Phex;

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
 	static LiveObject *ourLiveObject;

	static int stepCount;
	static double ourAge;
	
	static bool phexIsEnabled;
	static std::string phexIp;
	static int phexPort;
	static bool debugPhex;

	static bool sendKeyEvents;

	static HetuwFont *customFont;

	static std::string helpTextSearch[];
	static std::string helpTextCustomCoord[];

	static std::string hexRaceColor_brown;
	static std::string hexRaceColor_ginger;
	static std::string hexRaceColor_white;
	static std::string hexRaceColor_black;
	static float raceColor_brown[];
	static float raceColor_ginger[];
	static float raceColor_white[];
	static float raceColor_black[];

	static SimpleVector<LiveObject> *gameObjects;
	static std::vector<PlayerInMap*> playersInMap;
	static SimpleVector<int> *mMapContainedStacks;
	static SimpleVector<SimpleVector<int>> *mMapSubContainedStacks;
	static int *mMapD;

	static void hSetDrawColor(float rgba[]);

	static bool invalidVersionDetected;
	static string strInvalidVersion;
	static void drawWaitingText(doublePair pos);
	static void onInvalidVersionDetected(int version, int requiredVersion);

	static int *mCurMouseOverID;
	static int selectedPlayerID; 
	static void onPlayerHoverOver(int id);
	static double timeLastPlayerHover;

	static void pickUpBaby( int x, int y );
	static bool playerIsInCloseRange( LiveObject* o );
	static void pickUpBabyInRange();

	static void takeOffBackpack();

	static void splitLogLine(string* lineElements, string line);
	static int getLastIdFromLogs();
	static int lastLoggedId;
	static bool bWriteLogs;
	static void createNewLogFile();
	static void writeLineToLogs(string name, string str);

	static void init();
	static void initHelpText();
	static void initOnBirth();
	static void initOnServerJoin();

	static void setLivingLifePage(LivingLifePage *inLivingLifePage, SimpleVector<LiveObject> *inGameObjects,
							SimpleVector<int> *inmMapContainedStacks, SimpleVector<SimpleVector<int>> *inmMapSubContainedStacks,
							int &inmMapD, int &inmCurMouseOverID);

	static string getTimeStamp();
	static string getTimeStamp(time_t t);

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

	static void setOurSendPosXY(int &x, int &y);
	static void useBackpack(bool replace=false);
	static void useApronPocket();
	static void usePantsPocket();
	static void usePocket(int clothingID);
	static void useOnSelf();

	static void stepLoopTroughObjectsInRange();

	static double curStepTime;
	static time_t curStepSecondsSince1970;
	static void gameStep();

	static void livingLifeStep();
	static void livingLifeDraw();
	static bool livingLifeKeyDown(unsigned char inASCII);
	static bool livingLifeKeyUp(unsigned char inASCII);
	static bool livingLifeSpecialKeyDown(unsigned char inKeyCode);
	static bool livingLifePageMouseDown( float mX, float mY );
	static void moveToAndClickTile(int tileX, int tileY, bool alpha);

	static char* stringToChar(string str); // dont forget to delete[] cstr
	static void setEmote(int id);
	static void sendEmote(string emoteName);
	static void sendEmote(int emoteId);

	static void drawCoordsHelpA();
	static void drawCoordsHelpB();
	static void drawCoordsHelpC();
	static void drawSearchHelpText();
	static void drawCustomCoordHelpText();

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
	static bool itsTimeToDrawPhexName();
	static void drawPlayerNames( LiveObject* player );
	static void drawHighlightedPlayer();

	static bool charArrEqualsCharArr(const char *a, const char *b);
	static void drawTextWithBckgr( doublePair pos, const char* text );
	static void drawTextWithBckgr( doublePair pos, const char* text, float rgba[] );
	static doublePair drawCustomTextWithBckgr(doublePair pos, const char* text);

	static doublePair getFromMapToViewCoordsVec();
	static doublePair getFromViewToMapCoordsVec();
	static void pointFromPercentToMapCoords(float &x, float &y);
	static void pointFromPercentToMapCoords(double &x, double &y);
	static void pointFromMapToPercentCoords(float &x, float &y);
	static void pointFromMapToPercentCoords(double &x, double &y);
	static void xFromPercentToMapCoords(double &x);
	static void yFromPercentToMapCoords(double &y);

	static int getRecWidth(int rec[]);
	static double getRecWidth(double rec[]);
	static int getRecHeight(int rec[]);
	static double getRecHeight(double rec[]);
	static void setRecPosition(int rec[], int startX, int endX);
	static void setRecFromCenterWidthHeight(int rec[], int centerX, int centerY, int width, int height);
	static void addToRec(int rec[], int x, int y);
	static void addToRec(double rec[], double x, double y);
	static bool pointIsInsideRec(double rec[], double x, double y);
	static void set4BorderRecs(double rec[4], double outRecs[4][4], double borderWidth, double borderHeight);
	static void logRec(string desc, double rec[]);
	static void recToPixelCoords(int *rec);
	static void recFromPercentToMapCoords(double rec[]);
	static void recFromMapToPercentCoords(double rec[]);
	static void drawPointFromPercent(float x, float y);
	static void hDrawRecFromPercent(double rec[]);
	static void hDrawRecsFromPercent(double rec[][4], int recCount);
	static void hDrawRect( doublePair startPos, doublePair endPos );
	static void hDrawRect(double rec[]);
	static void hDrawRectWidthHeight(int left, int bottom, int width, int height);
	static void hDrawRect(int startX, int startY, int endX, int endY);
	static void hDrawRect(int rec[]);
	static void drawTileRect( int x, int y );

	static void onOurDeath();
	static void onPlayerUpdate( LiveObject* o, const char* line );
	static void onNameUpdate(LiveObject* o);
	static void removeLastName(char *newName, const char* name );
	static void getLastName( char* lastName, const char* name );
	static void setLastNameColor( const char* lastName, float alpha );
	static void getLastNameColor(const char* lastName, float rgba[]);

	static int playersInRangeNum;

	static std::vector<HomePos*> homePosStack;
	static void addHomeLocation(HomePos *p);
	static void addHomeLocation( int x, int y, homePosType type, char c = 0, int personID = -1 );
	static void setHomeLocationText(int x, int y, homePosType type, char *text);
	static void setMapText(char *message, int mapX, int mapY);
	static void logHomeLocation(HomePos* hp);
	static void addPersonHomeLocation( int x, int y, int personId );

	static GridPos cordOffset;

	static void getSettingsFileLine( char* name, char* value, string line );
	static bool setSetting( const char* name, const char* value );
	static bool setCharKey( unsigned char &key, const char *value );

	static std::vector<std::string> splitStrXTimes(const std::string &str, char splitChar, int count);

	static bool strContainsDangerousAnimal(const char* str);
	static bool *isDangerousAnimal;

	static void setTakingPhoto( bool inTakingPhoto );
	static bool takingPhoto;
	static bool takingSpecialPhoto;
	static int recTakePhoto[4];
	static bool bDrawPhotoRec;
	static void drawPhotoRec(int rec[]);
	static void updatePhotoRecPosition(int rec[]);
	static int* getPhotoRecForImage();

	static void saveImage(Image *image);
	static void saveImage(Image *image, string name);

	static float drawColorAlpha;
	static float xRayOpacity;
	static bool bxRay;
	static bool bHidePlayers;
	static char ourGender;

	static bool bFoundFamilyName;
	static std::vector<FamilyInRange*> familiesInRange;
	static string ourFamilyName;

	static bool charArrContainsCharArr(const char* arr1, const char* arr2);
	static void strToUpper(const char* src, char* dest, int maxSize);
	static void objDescrToUpper(const char* arr, char* output, int maxSize);
	static void getObjSearchDescr(const char* arr, char* output, int maxSize);
	static void objGetDescrWithoutHashtag(const char* arr, char* output, int maxSize);
	
	static bool searchIncludeHashText;
	static bool *objIsBeingSearched;
	static void setSearchArray();

	static bool cameraIsFixed;
	static void SetFixCamera(bool b);

	static void Say(const char *text);
	static void causeDisconnect();

	static float colorRainbowFast[3];
	static void stepColorRainbowFast();

	static int getObjYumID(ObjectRecord *obj);
	static void initBecomesFood();
	static int becomesFood( int objectID, int depth );
	static int *becomesFoodID;
	static SimpleVector<int> yummyFoodChain;
	static bool isYummy(int objID);
	static void foodIsMeh(ObjectRecord *obj);
	static void onJustAteFood(ObjectRecord *food);
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
	static bool bHoldDownTo_ShowGrid;

	static bool b_drawYumColor;
	static bool b_drawYumPulsate;
	static bool b_drawSearchText;
	static bool b_drawSearchTileRec;
	static bool b_drawSearchPulsate;

	static char usingCustomServer;
	static char *serverIP;
	static int serverPort;
	static void onGotServerAddress(char usingCustomServer, char *serverIP, int serverPort);

	static bool bAutoDataUpdate;

	static void makeHttpRequest(string link, void (*callBackFunc)(const char*, string), string type="GET", int inIntervalSeconds=60);

	static bool connectedToMainServer;
	static time_t arcRunningSince;

	static doublePair mouseRelativeToView;
	static void onMouseEvent(float mX, float mY);
	static void getMouseXY(int &x, int &y);

	static bool isMovingInVog;
	static IntervalTimed intervalVogMove;

	static bool bDrawBiomeInfo;

	static bool isWearingABackpack(LiveObject *obj);
	static bool isWearingPantsWithPocket(LiveObject *obj);
	static bool isWearingShirtWithPocket(LiveObject *obj);
	static bool weAreWearingABackpack();
	static bool weAreWearingPantsWithPocket();
	static bool weAreWearingShirtWithPocket();
	static void getSkinColor(float *rgba, ObjectRecord *obj);
	static std::string getRaceName(ObjectRecord *obj);

private:

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

	static void updatePlayerToMap(LiveObject *o, bool deathMsg = false);
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
	static void setDrawColorToCoordType(homePosType type);
	static bool bNextCharForHome;
	static void createCordsDrawStr();
	static float longestCordsTextWidth;

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
	static float sayDelay;

	static bool bMoveClick;
	static bool bMoveClickAlpha;
	static int bMoveClickX;
	static int bMoveClickY;

	static bool bDrawGrid;
	static void drawGrid();

	static bool addBabyCoordsToList;

	static std::vector<HttpRequest*> httpRequests;
	static void stepHttpRequests();
	static void processArcReport(const char* data, string error);
	static string getArcTimeStr();

	static void moveInVogMode();

	static void drawBiomeIDs();

	static void initCustomFont();
};


#endif
