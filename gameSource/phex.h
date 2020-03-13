#ifndef PHEX_H
#define PHEX_H

#define PHEX_VERSION 1
#define PHEX_CHAR_END 4
#define PHEX_MAX_INPUT_STR_LENGTH 127

#include <string>
#include <vector>

#include "hetuwmod.h"
#include "hetuwFont.h"
#include "hetuwTCPConnection.h"

class Phex {

public:

	struct Text {
		doublePair drawStartPos = {0,0};
		std::string str = "";
		double scale = 1.0;
		double width = 0;
		double height = 0;
		bool bDrawCursor = false;
		double cursorRec[4];
		double lastChangedTime = 0;

		void deleteLastChar() {
			if (str.length() <= 0) return;
			str = str.substr(0, str.length()-1);
		}
		void getWidthHeight();
		void setTextPosToCenter(double rec[4]);
		void setLastChangedTime() { lastChangedTime = HetuwMod::curStepTime; }
		void log(std::string desc) { printf("Phex %s: %s\n", desc.c_str(), str.c_str()); }
		void draw();
	};

	struct Button {
		Text text;
		double rec[4];
		float colorBckgr[4] = { 0.6, 0.6, 0.6, 0.6 };
		float colorBckgrHighlight[4] = { 0.8, 0.8, 0.8, 0.6 };
		void (*onClicked)();
		bool mouseDown = false;
		bool highlightIsActive = false;
		bool visible = true;
		bool widthIsSet = false;
		bool heightIsSet = false;
		static constexpr float textPadding = 0.1;
		float textPaddingX;
		float textPaddingY;
		bool bDrawBorder = false;
		double borderRecs[4][4];
		double borderSize;
		bool borderSizeIsSet = false;

		void init(std::string str, void (*inOnClicked)()) {
			text.str = str;
			onClicked = inOnClicked;
		}
		void setPosition(double x, double y) {
			float width = rec[2]-rec[0];
			float height = rec[3]-rec[1];
			rec[0] = x; rec[1] = y;
			rec[2] = rec[0]+width; rec[3] = rec[1]+height;
		}
		void setY(double y) {
			float height = rec[3]-rec[1];
			rec[1] = y;
			rec[3] = rec[1]+height;
		}
		void setWidth(double width) {
			rec[2] = rec[0] + width;
			widthIsSet = true;
		}
		void setHeight(double height) {
			rec[3] = rec[1] + height;
			heightIsSet = true;
		}
		void setBorderSize(double inBorderSize) {
			borderSize = inBorderSize;
			borderSizeIsSet = true;
		}
		bool onMouseEvent(float x, float y) {
			if (!visible) return false;
			highlightIsActive = false;
			if (!HetuwMod::pointIsInsideRec(rec, x, y)) return false;
			highlightIsActive = true;
			return true;
		}
		bool onMouseDown(float x, float y) {
			if (!visible) return false;
			if (!HetuwMod::pointIsInsideRec(rec, x, y)) return false;
			mouseDown = true;
			return true;
		}
		bool onMouseUp(float x, float y) {
			if (!mouseDown) return false;
			mouseDown = false;
			if (!visible) return false;
			if (!HetuwMod::pointIsInsideRec(rec, x, y)) return false;
			highlightIsActive = false;
			onClicked();
			return true;
		}
		void setDrawData();
		void draw();
	private:
		void calcTextDimensions();
		void calcRecDimensions();
		void calcDimensions();
		void setTextPosition();
		void setBorderRecs();
	};

	struct ChatElement {
		static constexpr int maxHashDisplayLength = 8;
		std::string text;
		std::string hash;
		std::string name;
		time_t unixTimeStamp;
		double textHeight;
		double textHeightScaled;
		std::string textToDraw;
	};

	struct ChatWindow {
		static constexpr int messageDisplayDurationInSec = 10;
		double rec[4];
		doublePair textDrawPos;
		std::vector<ChatElement> elements;
		int msgDisplayDur = -1;
		double topMinimum;

		void init(double inRec[4]) { for(int i=0; i<4; i++) rec[i] = inRec[i]; }
		void addElement(ChatElement element);
		void onZoom();
		void draw(bool bDraw=true);
		double getTopMinimum();
		void clear() { elements.clear(); }
	};

	struct PhexTitleText {
		Text phex;
		Text status;
		double rec[4];
		float color[4];
		double recBckgr[4];
		
		PhexTitleText() {
			phex.str = "P H E X";
			phex.scale = 2;
			status.scale = 0.9;
		}
		void init(double inRec[4]) {
			setArray(rec, inRec, 4);
			phex.getWidthHeight();
			phex.setTextPosToCenter(rec);
			setToOffline();
			phex.drawStartPos.y += phex.height/2.0;
			status.drawStartPos.y = phex.drawStartPos.y - phex.height;
		}
		void setStatusPostion() {
			status.getWidthHeight();
			status.setTextPosToCenter(rec);
			status.drawStartPos.y -= phex.height*0.1;
		}
		void setToOffline() {
			status.str = "offline";
			setArray(color, (const float[]){ 1.0f, 0.1f, 0.1f, 1.0f }, 4);
			setStatusPostion();
		}
		void setToOnline() {
			status.str = "online";
			setArray(color, (const float[]){ 0.1f, 1.0f, 0.2f, 1.0f }, 4);
			setStatusPostion();
		}
		void setToConnecting() {
			status.str = "connecting";
			setArray(color, (const float[]){ 1.0f, 0.8f, 0.0f, 1.0f }, 4);
			setStatusPostion();
		}
		void draw() {
			setDrawColor(0.0, 0.0, 0.0, 0.3);
			HetuwMod::hDrawRecFromPercent(recBckgr);
			HetuwMod::hSetDrawColor(color);
			status.draw();
			phex.draw();
		}
	};

	static TCPConnection tcp;
	static bool bSendFirstMsg;
	
	static bool hasFocus;
	static bool isMinimized;
	static bool bDrawRecInput;

	static double textScale;

	static double textInRecPaddingX;
	static double textInRecPaddingY;

	static double recBckgr[4];
	static double recBckgrBig[4];
	static float colorRecBckgr[4];
	static float colorRecBckgrHighlight[4];

	static double recInput[4];
	static float colorRecInput[4];
	static void sendInputStr();
	static bool addToInputStr(unsigned char c);
	static Text inputText;

	static float colorWhite[4];
	static float colorNamesInChat[4];
	static std::string colorCodeWhite;
	static std::string colorCodeNamesInChat;

	static double butHeight;
	static double butBorderSize;
	static std::vector<Button*> buttons;
	static Button butPhex;
	static Button butTurnOff;
	static Button butMinimize;
	static Button butMaximize;

	static PhexTitleText titleText;
	static double recLineBelowTitle[4];
	static Text textHotKeyInfo;
	static double recLineBelowHotKey[4];

	static HetuwMod::IntervalTimed intervalStep;
	static HetuwMod::IntervalTimed intervalRead;

	static HetuwFont *mainFont;
	static double mainFontScaleFactor;

	static ChatWindow mainChatWindow;

	static HetuwMod::KeyHandler keyHandler;

	static void init();
	static void initFont();
	static void setMainFontScale();
	static void onGuiScaleChange();
	static void setButtonStyle(Button *but);

	static void setArray(float arrDst[], const float arrSrc[], int size);
	static void setArray(double arrDst[], const double arrSrc[], int size);
	static void multipleArray(double arr[], double factor, int size);

	static bool strEquals(std::string strA, std::string strB);
	static doublePair getStringWidthHeight(doublePair startPos, std::string str);
	static double getLineHeight(HetuwFont *font);
	static void drawString(std::string str, doublePair startPos);

	static time_t strToTimeT(std::string str);

	static void onZoom();

	static void sendFirstMessage();

	static void recvPong();
	static void recvSay(std::string msg);

	static void setInputRecDrawData();
	static void drawInputRec();

	static void draw();
	static void drawNormal();
	static void drawMinimized();

	static void minimize();
	static void maximize();

	static std::string getSecretHash();

	static void onReceivedMessage(std::string msg);
	static void onConnectionStatusChanged(TCPConnection::statusType status);

	static void onClickPhex();
	static void onClickTurnOff();
	static void onServerJoin();
	static void onPhexKeyPressed();
	static bool onUpdateFocus(bool focus);

	static void onMouseEvent(float x, float y);
	static bool onMouseDown(float x, float y);
	static bool onMouseUp(float x, float y);
	static void onKey(unsigned char inASCII);
	static bool onKeyDown(unsigned char inASCII);
	static bool onKeyUp(unsigned char inASCII);

private:

	static void fontSetMaxX();
	static void initButtons();

};

#endif
