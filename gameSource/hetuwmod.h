#ifndef HETUWMOD_H
#define HETUWMOD_H

#include "LivingLifePage.h"

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

public:
	static const int defaultViewWidth = 1280;
	static const int defaultViewHeight = 720;

	static int dangerousAnimalsLength;
	static int* dangerousAnimals;

	static int viewWidth;
	static int viewHeight;

	static float zoomScale;
	static int panelOffsetX;
	static int panelOffsetY;
	
	static char charKey_Up;
	static char charKey_Down;
	static char charKey_Left;
	static char charKey_Right;

	static RainbowColor *colorRainbow;

	static LivingLifePage *livingLifePage;

	static void init();
	static void initDangerousAnimals();

	static void setLivingLifePage(LivingLifePage *inLivingLifePage);

	static void zoomIncrease();
	static void zoomDecrease();
	
	static void useTileRelativeToMe( int x, int y );
	static void dropTileRelativeToMe( int x, int y );
	static void remvTileRelativeToMe( int x, int y );
	
	static void actionAlphaRelativeToMe( int x, int y );
	static void actionBetaRelativeToMe( int x, int y );

	static void useBackpack( bool replace=false);
	static void useOnSelf();

	static void livingLifeDraw();
	static bool livingLifeKeyDown(unsigned char inASCII);
	static bool livingLifeKeyUp(unsigned char inASCII);

	static void setEmote(int id);

	static bool tileIsSafeToWalk(int x, int y); 
	static bool tileHasNoDangerousAnimals(int x, int y);

private:
	static void zoomCalc();

	static bool bDrawHelp;
	static void drawHelp();

	static float lastPosX;
	static float lastPosY;

	static bool upKeyDown;
	static bool downKeyDown;
	static bool leftKeyDown;
	static bool rightKeyDown;

	static void move(); // called only once on frame - dont use
	static bool cornerTileIsSafeToWalk( int sX, int sY, bool up, bool down, bool right, bool left);

	static doublePair debugRecPos;
	static doublePair debugRecPos2;

	static int currentEmote;
	static time_t lastEmoteTime;
};


#endif
