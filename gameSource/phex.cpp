#include "phex.h"

#include <string>
#include <vector>
#include <unordered_map>

#include "minorGems/crypto/hashes/sha1.h"

#include "hetuwmod.h"
#include "LivingLifePage.h"
#include "accountHmac.h"
#include "hetuwFont.h"

TCPConnection Phex::tcp;
bool Phex::bSendFirstMsg = false;

std::unordered_map<std::string, Phex::ServerCommand> Phex::serverCommands;

bool Phex::hasFocus = false;
bool Phex::isMinimized = false;
bool Phex::bDrawRecInput = false;

double Phex::textScale = 0.6;

double Phex::textInRecPaddingX;
double Phex::textInRecPaddingY;

double Phex::recBckgr[4];
double Phex::recBckgrBig[4];
float Phex::colorRecBckgr[4];
float Phex::colorRecBckgrHighlight[4];

double Phex::recInput[4];
float Phex::colorRecInput[4];
Phex::Text Phex::inputText;

float Phex::colorWhite[4];
float Phex::colorNamesInChat[4];
string Phex::colorCodeWhite;
string Phex::colorCodeNamesInChat;

double Phex::butHeight = 0.04;
double Phex::butBorderSize = 0.004;
std::vector<Phex::Button*> Phex::buttons;
Phex::Button Phex::butPhex;
Phex::Button Phex::butTurnOff;
Phex::Button Phex::butMinimize;
Phex::Button Phex::butMaximize;

Phex::PhexTitleText Phex::titleText;
double Phex::recLineBelowTitle[4];
Phex::Text Phex::textHotKeyInfo;
double Phex::recLineBelowHotKey[4];

HetuwFont *Phex::mainFont = NULL;
double Phex::mainFontScaleFactor = 16;

Phex::ChatWindow Phex::mainChatWindow;

HetuwMod::KeyHandler Phex::keyHandler(&onKey);

extern doublePair lastScreenViewCenter;
extern char *userEmail;
extern int versionNumber;

void Phex::init() {
	if (!HetuwMod::phexIsEnabled) return;

	initServerCommands();

	tcp.init(HetuwMod::phexIp, HetuwMod::phexPort, &onReceivedMessage, &onConnectionStatusChanged);
	tcp.logTag = "Phex";
	tcp.charEnd = PHEX_CHAR_END;
	tcp.connect();

	textInRecPaddingX = 0.01;
	textInRecPaddingY = textInRecPaddingX * HetuwMod::viewWidthToHeightFactor;

	setArray(recBckgrBig, (const double[]){ 0.7f, 0.0f, 1.0f, 1.0f }, 4);
	setArray(recBckgr, recBckgrBig, 4);

	setArray(colorRecBckgr, (const float[]){ 0.0f, 0.0f, 0.0f, 0.7f }, 4);

	setArray(colorWhite, (const float[]){ 1.0f, 1.0f, 1.0f, 1.0f }, 4);
	setArray(colorRecInput, (const float[]){ 0.0f, 0.0f, 0.0f, 0.6 }, 4);
	setArray(colorNamesInChat, (const float[]){ 0.2f, 0.7f, 1.0f, 1.0f }, 4);

	mainChatWindow.init(recBckgr);
	mainChatWindow.rec[0] += textInRecPaddingX;
	mainChatWindow.rec[1] += textInRecPaddingY;
	mainChatWindow.rec[2] -= textInRecPaddingX;
	mainChatWindow.rec[3] -= textInRecPaddingY;
	mainChatWindow.onZoom();

	inputText.bDrawCursor = false;

	initFont();

	colorCodeWhite = mainFont->hetuwGetColorCode(colorWhite);
	colorCodeNamesInChat = mainFont->hetuwGetColorCode(colorNamesInChat);

	initButtons();

	double titleTextRec[4];
	titleTextRec[0] = recBckgr[0]; titleTextRec[1] = recBckgr[3]-butHeight-0.13;
	titleTextRec[2] = recBckgr[2]; titleTextRec[3] = recBckgr[3]-butHeight;
	titleText.init(titleTextRec);
	setArray(titleText.recBckgr, titleTextRec, 4);
	titleText.recBckgr[1] += 0.04;

	recLineBelowTitle[0] = recBckgr[0];
	recLineBelowTitle[1] = titleText.recBckgr[1]-butBorderSize;
	recLineBelowTitle[2] = recBckgr[2];
	recLineBelowTitle[3] = recLineBelowTitle[1]+butBorderSize;

	double recHotKeyInfo[4];
	recHotKeyInfo[0] = recBckgr[0];
	recHotKeyInfo[1] = recLineBelowTitle[1]-0.03;
	recHotKeyInfo[2] = recBckgr[2];
	recHotKeyInfo[3] = recLineBelowTitle[1];
	textHotKeyInfo.str = "press ";
	textHotKeyInfo.str += (char)HetuwMod::charKey_Phex;
	textHotKeyInfo.str += " to toggle";
	textHotKeyInfo.getWidthHeight();
	textHotKeyInfo.setTextPosToCenter(recHotKeyInfo);
	
	recLineBelowHotKey[0] = recHotKeyInfo[0];
	recLineBelowHotKey[1] = recHotKeyInfo[1]-butBorderSize;
	recLineBelowHotKey[2] = recHotKeyInfo[2];
	recLineBelowHotKey[3] = recHotKeyInfo[1];

	mainChatWindow.rec[3] = recLineBelowHotKey[3] - textInRecPaddingY;

	minimize();
}

void Phex::initFont() {
	int fontCharSpacing = 3; // vanilla main font is 6
	int fontSpaceWidth = 8; // vanilla main font is 16
	char fontFixedWidth = false;
	mainFont = new HetuwFont("font_32_64.tga", fontCharSpacing, fontSpaceWidth, fontFixedWidth, mainFontScaleFactor);
	mainFont->setMinimumPositionPrecision( 1 );
	mainFont->hetuwMaxXActive = true;
	setMainFontScale();
	fontSetMaxX();
}

void Phex::fontSetMaxX() {
	mainFont->hetuwMaxX = HetuwMod::viewWidth/2.0;
	mainFont->hetuwMaxX += lastScreenViewCenter.x;
	mainFont->hetuwMaxX -= textInRecPaddingX*HetuwMod::viewWidth;
	mainFont->hetuwMaxX = round(mainFont->hetuwMaxX); 
}

void Phex::setMainFontScale() {
	if (!mainFont) return;
	//mainFont->hetuwSetScaleFactor(mainFontScaleFactor * HetuwMod::guiScale * textScale);
	mainFont->hetuwSetScaleFactor(mainFontScaleFactor * textScale * HetuwMod::zoomScale);
}

void Phex::onGuiScaleChange() {
	//setMainFontScale();
}

void Phex::setButtonStyle(Button *but) {
	but->bDrawBorder = true;
	setArray(but->colorBckgr, (const float[]){ 0.0f, 0.0f, 0.0f, 0.0f }, 4);
	but->setBorderSize(butBorderSize);
}

void Phex::initButtons() {
	double recBckgrWidth = HetuwMod::getRecWidth(recBckgr);
	double butWidth = recBckgrWidth/2.0 + butBorderSize/2.0;

	butPhex.init("PH", &onClickPhex);
	setButtonStyle(&butPhex);
	double butPhexWidth = 0.03;
	double butPhexHeight = butPhexWidth * HetuwMod::viewWidthToHeightFactor;
	double butPhexPaddingX = 0.01;
	double butPhexPaddingY = butPhexPaddingX * HetuwMod::viewWidthToHeightFactor;
	butPhex.setPosition(1.0-butPhexWidth-butPhexPaddingX, butPhexPaddingY);
	butPhex.setWidth(butPhexWidth);
	butPhex.setHeight(butPhexHeight);
	setArray(butPhex.colorBckgr, colorRecBckgr, 4);
	butPhex.text.scale = 1.2;
	butPhex.setDrawData();
	butPhex.visible = false;
	buttons.push_back(&butPhex);

	butTurnOff.init("Turn Off", &onClickTurnOff);
	setButtonStyle(&butTurnOff);
	butTurnOff.setPosition(recBckgr[0], recBckgr[3]-butHeight);
	butTurnOff.setWidth(butWidth);
	butTurnOff.setHeight(butHeight);
	butTurnOff.setDrawData();
	buttons.push_back(&butTurnOff);

	butMinimize.init("Minimize", &minimize);
	setButtonStyle(&butMinimize);
	butMinimize.setPosition(recBckgr[0]+butWidth-butBorderSize/2.0, recBckgr[3]-butHeight);
	butMinimize.setWidth(butWidth-butBorderSize/2.0);
	butMinimize.setHeight(butHeight);
	butMinimize.setDrawData();
	buttons.push_back(&butMinimize);

	butMaximize.init("Maximize", &maximize);
	setButtonStyle(&butMaximize);
	butMaximize.setPosition(1.0-recBckgrWidth, 0);
	butMaximize.setWidth(recBckgrWidth);
	butMaximize.setHeight(butHeight);
	setArray(butMaximize.colorBckgr, colorRecBckgr, 4);
	butMaximize.setDrawData();
	butMaximize.visible = false;
	buttons.push_back(&butMaximize);
}

void Phex::initServerCommands() {
	serverCommands["VERSION"].func = &serverCmdVERSION;
	serverCommands["VERSION"].minWords = 2;
	serverCommands["HASH"].func = serverCmdHASH;
	serverCommands["HASH"].minWords = 2;
	serverCommands["USERNAME"].func = serverCmdUSERNAME;
	serverCommands["USERNAME"].minWords = 2;
	serverCommands["USERNAME_ERR"].func = serverCmdUSERNAME_ERR;
	serverCommands["USERNAME_ERR"].minWords = 2;
	serverCommands["SAY"].func = &serverCmdSAY;
	serverCommands["SAY"].minWords = 5;
	serverCommands["HASH_USERNAME"].func = serverCmdHASH_USERNAME;
	serverCommands["HASH_USERNAME"].minWords = 2;
	serverCommands["ONLINE"].func = serverCmdONLINE;
	serverCommands["ONLINE"].minWords = 2;
	serverCommands["OFFLINE"].func = serverCmdOFFLINE;
	serverCommands["OFFLINE"].minWords = 2;
	serverCommands["JOINED_CHANNEL"].func = serverCmdJOINED_CHANNEL;
	serverCommands["JOINED_CHANNEL"].minWords = 3;
	serverCommands["LEFT_CHANNEL"].func = serverCmdLEFT_CHANNEL;
	serverCommands["LEFT_CHANNEL"].minWords = 3;
}

void Phex::serverCmdVERSION(std::vector<std::string> input) {

}

void Phex::serverCmdHASH(std::vector<std::string> input) {

}

void Phex::serverCmdUSERNAME(std::vector<std::string> input) {

}

void Phex::serverCmdUSERNAME_ERR(std::vector<std::string> input) {

}

void Phex::serverCmdSAY(std::vector<std::string> input) {
	ChatElement chatElement;
	chatElement.hash = input[2];
	chatElement.unixTimeStamp = strToTimeT(input[3]);

	chatElement.text = "";
	for (unsigned i=4; i<input.size(); i++) {
		chatElement.text += input[i];
		if (i+1 != input.size()) chatElement.text += ' ';
	}

	chatElement.name = chatElement.hash;
	if (chatElement.name.length() > ChatElement::maxHashDisplayLength)
		chatElement.name = chatElement.name.substr(0, ChatElement::maxHashDisplayLength);

	mainChatWindow.addElement(chatElement);
}

void Phex::serverCmdHASH_USERNAME(std::vector<std::string> input) {

}

void Phex::serverCmdONLINE(std::vector<std::string> input) {

}

void Phex::serverCmdOFFLINE(std::vector<std::string> input) {

}

void Phex::serverCmdJOINED_CHANNEL(std::vector<std::string> input) {

}

void Phex::serverCmdLEFT_CHANNEL(std::vector<std::string> input) {

}

void Phex::setArray(float arrDst[], const float arrSrc[], int size) {
	for (int i=0; i<size; i++) arrDst[i] = arrSrc[i];
}

void Phex::setArray(double arrDst[], const double arrSrc[], int size) {
	for (int i=0; i<size; i++) arrDst[i] = arrSrc[i];
}

void Phex::multipleArray(double arr[], double factor, int size) {
	for (int i=0; i<size; i++) arr[i] *= factor;
}

bool Phex::strEquals(std::string strA, std::string strB) {
	return strA.compare(strB) == 0;
}

doublePair Phex::getStringWidthHeight(doublePair startPos, string str) {
	HetuwMod::pointFromPercentToMapCoords(startPos.x, startPos.y);
	SimpleVector<doublePair> outPos;
	mainFont->getCharPos(&outPos, str.c_str(), startPos, alignLeft);
	doublePair widthHeight = { mainFont->hetuwWidth, mainFont->hetuwHeight };
	widthHeight.x /= HetuwMod::viewWidth; widthHeight.y /= HetuwMod::viewHeight;
	return widthHeight;
}

double Phex::getLineHeight(HetuwFont *font) {
	double lineHeight = font->getFontHeight();
	lineHeight /= HetuwMod::viewHeight;
	return lineHeight;
}

void Phex::drawString(string str, doublePair startPos) {
	HetuwMod::pointFromPercentToMapCoords(startPos.x, startPos.y);
	startPos.y -= mainFont->hetuwGetHalfSpriteHeight();
	mainFont->drawString(str.c_str(), startPos, alignLeft);
}

void Phex::Text::getWidthHeight() {
	double scaleTemp = mainFont->hetuwGetScaleFactor();
	mainFont->hetuwSetScaleFactor(scaleTemp * scale);

	doublePair pos = drawStartPos;
	HetuwMod::pointFromPercentToMapCoords(pos.x, pos.y);
	SimpleVector<doublePair> outPos;
	mainFont->getCharPos(&outPos, str.c_str(), pos, alignLeft);
	width = mainFont->hetuwWidth / HetuwMod::viewWidth;
	height = mainFont->hetuwHeight / HetuwMod::viewHeight;

	doublePair cursorPos = mainFont->hetuwNextCharPos;
	cursorPos.y -= mainFont->getFontHeight() * 0.95;
	double halfSpaceWidth = mainFont->hetuwGetSpaceWidth() / 2;
	cursorRec[0] = cursorPos.x - halfSpaceWidth;
	cursorRec[2] = cursorPos.x + halfSpaceWidth;
	double halfSpriteHeight = mainFont->hetuwGetHalfSpriteHeight() * 0.8;
	cursorRec[1] = cursorPos.y - halfSpriteHeight;
	cursorRec[3] = cursorPos.y + halfSpriteHeight;
	double cursorPaddingY = halfSpriteHeight*0.15;
	cursorRec[1] += cursorPaddingY;
	cursorRec[3] += cursorPaddingY;
	HetuwMod::recFromMapToPercentCoords(cursorRec);
	HetuwMod::addToRec(cursorRec, -drawStartPos.x, -drawStartPos.y);

	mainFont->hetuwSetScaleFactor(scaleTemp);
}

void Phex::Text::setTextPosToCenter(double rec[4]) {
	double recWidthHalf = (rec[2]-rec[0])/2.0;
	double recHeightHalf = (rec[3]-rec[1])/2.0;
	drawStartPos.x = rec[0] + recWidthHalf - width/2.0;
	drawStartPos.y = rec[1] + recHeightHalf + height/2.0;

	double scaleTemp = mainFont->hetuwGetScaleFactor();
	mainFont->hetuwSetScaleFactor(scaleTemp * scale);

	// substract the special height of chracters like 'g' that go below the normal text position
	drawStartPos.y -= mainFont->hetuwGetHalfSpriteHeight()/8/HetuwMod::viewHeight;

	mainFont->hetuwSetScaleFactor(scaleTemp);
}

void Phex::Text::draw() {
	double scaleTemp = mainFont->hetuwGetScaleFactor();
	mainFont->hetuwSetScaleFactor(scaleTemp * scale);
	drawString(str, drawStartPos);
	mainFont->hetuwSetScaleFactor(scaleTemp);

	if (!bDrawCursor) return;
	if (lastChangedTime+1 < HetuwMod::curStepTime) {
		if ((unsigned long)(HetuwMod::curStepTime*2.0) % 2 == 0) return;
	}
	double drawRec[4];
	setArray(drawRec, cursorRec, 4);
	HetuwMod::addToRec(drawRec, drawStartPos.x, drawStartPos.y);
	HetuwMod::hDrawRecFromPercent(drawRec);
}

time_t Phex::strToTimeT(std::string str) {
	return (time_t) strtol(str.c_str(), NULL, 10); // on some architectures it may be 32 bit integer and the cast will cause an overflow
}

void Phex::onZoom() {
	setMainFontScale();
	mainChatWindow.onZoom();
}

void Phex::ChatWindow::addElement(ChatElement element) {
	element.textToDraw = colorCodeNamesInChat+element.name+": "+colorCodeWhite+element.text;
	doublePair pos = {rec[0], rec[1]};
	doublePair widthHeight = getStringWidthHeight(pos, element.textToDraw);
	element.textHeight = widthHeight.y;
	elements.push_back(element);
}

void Phex::ChatWindow::onZoom() {
	if (true) return;
	textDrawPos.x = rec[0]*HetuwMod::viewWidth - (HetuwMod::viewWidth/2);
	textDrawPos.y = rec[1]*HetuwMod::viewHeight - (HetuwMod::viewHeight/2);
	for(int i=(int)elements.size()-1; i>=0; i--) {
		elements[i].textHeightScaled = elements[i].textHeight * HetuwMod::zoomScale;
		textDrawPos.y += elements[i].textHeightScaled;
	}
}

void Phex::ChatWindow::draw(bool bDraw) {
	float x = rec[0];
	float y = rec[1];
	topMinimum = 0;
	if (bDraw) setDrawColor(1.0f, 1.0f, 1.0f, 1.0f);
	for(int i=(int)elements.size()-1; i>=0; i--) {
		if ((int)elements.size()-i > drawMaxElements) break;
		if (msgDisplayDur > 0)
			if (elements[i].unixTimeStamp+msgDisplayDur < HetuwMod::curStepSecondsSince1970) break;
		y += elements[i].textHeight;
		if (y > rec[3]) break;
		topMinimum = y;
		if (bDraw) drawString(elements[i].textToDraw.c_str(), {x, y});
	}
}

double Phex::ChatWindow::getTopMinimum() {
	draw(false);
	return topMinimum;
}

void Phex::Button::draw() {
	if (!visible) return;
	HetuwMod::hSetDrawColor(colorBckgr);
	HetuwMod::hDrawRecFromPercent(rec);
	if (highlightIsActive) {
		HetuwMod::hSetDrawColor(colorBckgrHighlight);
		HetuwMod::hDrawRecFromPercent(rec);
	}
	setDrawColor(1.0, 1.0, 1.0, 1.0);
	if (bDrawBorder) {
		HetuwMod::hDrawRecsFromPercent(borderRecs, 4);
	}
	text.draw();
}

void Phex::Button::calcTextDimensions() {
	text.getWidthHeight();
	textPaddingY = text.height * textPadding;
	textPaddingX = textPaddingY * HetuwMod::viewHeightToWidthFactor * 1.05;
}

void Phex::Button::calcRecDimensions() {
	if (!widthIsSet)  rec[2] = rec[0] + text.width +  (textPaddingX*2);
	if (!heightIsSet) rec[3] = rec[1] + text.height + (textPaddingY*2);
}

void Phex::Button::calcDimensions() {
	calcTextDimensions();
	calcRecDimensions();
}

void Phex::Button::setTextPosition() {
	float recWidthHalf = (rec[2]-rec[0])/2.0;
	float recHeightHalf = (rec[3]-rec[1])/2.0;
	text.drawStartPos.x = rec[0] + recWidthHalf - text.width/2.0;
	text.drawStartPos.y = rec[1] + recHeightHalf + text.height/2.0;
}

void Phex::Button::setBorderRecs() {
	double borderHeight = borderSize;
	double borderWidth = borderSize * HetuwMod::viewHeightToWidthFactor;
	if (!borderSizeIsSet) {
		borderHeight = HetuwMod::getRecHeight(rec) * 0.08;
		borderWidth = borderHeight * HetuwMod::viewHeightToWidthFactor;
	}
	HetuwMod::set4BorderRecs(rec, borderRecs, borderWidth, borderHeight);
}

void Phex::Button::setDrawData() {
	calcDimensions();
	text.setTextPosToCenter(rec);
	if (bDrawBorder) setBorderRecs();
}

void Phex::setInputRecDrawData() {
	recInput[0] = recBckgr[0]+(textInRecPaddingX/2);
	recInput[2] = recBckgr[2]-(textInRecPaddingX/2);
	recInput[1] = recBckgr[1]+textInRecPaddingY;
	inputText.drawStartPos.x = recInput[0]+(textInRecPaddingX/2);
	inputText.drawStartPos.y = recInput[1];

	inputText.getWidthHeight();
	double height = inputText.height;

	recInput[3] = recInput[1]+height;
	inputText.drawStartPos.y = recInput[3]+(textInRecPaddingY*0.07);
	recInput[3] += textInRecPaddingY/2.0;
}

void Phex::drawInputRec() {
	HetuwMod::hSetDrawColor(colorRecInput);
	HetuwMod::hDrawRecFromPercent(recInput);
	setDrawColor(1.0, 1.0, 1.0, 1.0);
	inputText.draw();
}

void Phex::sendInputStr() {
	if (inputText.str.length() < 1) return;
	tcp.send("SAY global "+inputText.str);
}

bool Phex::addToInputStr(unsigned char c) {
	if (c == 13) { // enter
		if (tcp.status != TCPConnection::ONLINE) return true;
		sendInputStr();
		inputText.str = "";
		return true;
	}
	inputText.setLastChangedTime();
	if (c == 8) { // backspace
		inputText.deleteLastChar();
		return true;
	}
	if (c < 32) return false;
	if (inputText.str.length() > PHEX_MAX_INPUT_STR_LENGTH) return false;
	inputText.str += c;
	return true;
}

void Phex::sendFirstMessage() {
	string clientName = "hetuw";
	string phexVersionNumber = to_string(PHEX_VERSION);
	string secretHash = getSecretHash();
	string jasonsOneLifeVersion = to_string(versionNumber);
	string msg = "FIRST "+clientName+" "+phexVersionNumber+" "+secretHash+" "+jasonsOneLifeVersion;
	tcp.send(msg);

	tcp.send("JOIN global");
	mainChatWindow.clear();
	tcp.send("GETLAST global 30");
}

void Phex::draw() {
	if (!HetuwMod::phexIsEnabled) return;
	fontSetMaxX();
	tcp.step();
	keyHandler.step();

	if (isMinimized) drawMinimized();
	else drawNormal();
}

void Phex::drawNormal() {
	setInputRecDrawData();

	HetuwMod::hSetDrawColor(colorRecBckgr);
	HetuwMod::hDrawRecFromPercent(recBckgr);

	setDrawColor(1.0, 1.0, 1.0, 1.0);
	mainChatWindow.rec[1] = recInput[3] + textInRecPaddingY;
	mainChatWindow.draw();

	butTurnOff.draw();
	butMinimize.draw();

	//setDrawColor(0.6, 0.0, 1.0, 0.5);
	//HetuwMod::hDrawRecFromPercent(titleText.rec);
	titleText.draw();

	setDrawColor(1.0, 1.0, 1.0, 1.0);
	HetuwMod::hDrawRecFromPercent(recLineBelowTitle);
	textHotKeyInfo.draw();
	HetuwMod::hDrawRecFromPercent(recLineBelowHotKey);

	drawInputRec();
}

void Phex::drawMinimized() {
	if (bDrawRecInput) {
		setInputRecDrawData();
		mainChatWindow.rec[1] = recInput[3] + textInRecPaddingY;
		mainChatWindow.msgDisplayDur = -1;
		mainChatWindow.drawMaxElements = 4;
	} else {
		mainChatWindow.rec[1] = textInRecPaddingY;
		mainChatWindow.msgDisplayDur = ChatWindow::messageDisplayDurationInSec;
		mainChatWindow.drawMaxElements = 4;
	}

	double chatTop = mainChatWindow.getTopMinimum();
	if (chatTop <= 0 && !bDrawRecInput) {
		butPhex.visible = true;
		butPhex.draw();
		return;
	}
	butPhex.visible = false;

	if (chatTop <= 0) {
		recBckgr[3] = mainChatWindow.rec[1];
	} else {
		recBckgr[3] = chatTop + textInRecPaddingY;
	}

	butMaximize.visible = false;
	if (hasFocus) {
		butMaximize.visible = true;
		butMaximize.setY(recBckgr[3]);
		butMaximize.setDrawData();
		recBckgr[3] = butMaximize.rec[3];
	}

	HetuwMod::hSetDrawColor(colorRecBckgr);
	HetuwMod::hDrawRecFromPercent(recBckgr);

	if (hasFocus) butMaximize.draw();

	mainChatWindow.draw();

	if (bDrawRecInput) drawInputRec();
}

void Phex::minimize() {
	if (isMinimized) return;
	isMinimized = true;

	butTurnOff.visible = false;
	butMinimize.visible = false;
	onUpdateFocus(false);
}

void Phex::maximize() {
	if (!isMinimized) return;
	isMinimized = false;

	mainChatWindow.msgDisplayDur = -1;
	mainChatWindow.drawMaxElements = 99999;
	setArray(recBckgr, recBckgrBig, 4);
	butTurnOff.visible = true;
	butMinimize.visible = true;
	inputText.bDrawCursor = true;
	onUpdateFocus(true);
}

string Phex::getSecretHash() {
	char *accKey = getPureAccountKey();
	int keyLength = strlen(accKey)+5;
	char key[keyLength];
	sprintf(key, "%sphex", accKey);
	char *hash = hmac_sha1("phex", key);
	string strHash(hash);
	delete[] hash;
	delete[] accKey;
	return strHash;
}

void Phex::onReceivedMessage(std::string msg) {
	if (msg.length() <= 0) {
		printf("Phex Error: received empty message from server\n");
		return;
	}
	std::vector<std::string> splittedMsg = HetuwMod::splitStrXTimes(msg, ' ', 99);
	std::string command = splittedMsg[0];
	if (command.length() <= 0) {
		printf("Phex Error: received empty command from server\n");
		return;
	}
	if (serverCommands.find(command) == serverCommands.end()) {
		printf("Phex Error: unknown command '%s'\n", command.c_str());
		printf("Phex Error: message: '%s'\n", msg.c_str());
		return;
	}
	if (serverCommands[command].minWords > (int)splittedMsg.size()) {
		printf("Phex Error: server message to short, expected atleast %d words, but got %d\n", serverCommands[command].minWords, (int)splittedMsg.size());
		printf("Phex Error: message: '%s'\n", msg.c_str());
		return;
	}
	serverCommands[command].func(splittedMsg);
}

void Phex::onConnectionStatusChanged(TCPConnection::statusType status) {
	switch (status) {
		case TCPConnection::OFFLINE:
			titleText.setToOffline();
			break;
		case TCPConnection::CONNECTING:
			titleText.setToConnecting();
			bSendFirstMsg = true;
			break;
		case TCPConnection::ONLINE:
			titleText.setToOnline();
			if (bSendFirstMsg) {
				sendFirstMessage();
				bSendFirstMsg = false;
			}
			break;
	}
}

void Phex::onClickPhex() {
	onUpdateFocus(true);
}

void Phex::onClickTurnOff() {
	if (strEquals(butTurnOff.text.str, "Turn Off")) {
		tcp.disconnect();
		butTurnOff.text.str = "Turn On";
	} else {
		tcp.connect();
		butTurnOff.text.str = "Turn Off";
	}
	butTurnOff.setDrawData();
}

void Phex::onServerJoin() {
	if (!HetuwMod::phexIsEnabled) return;
}

void Phex::onPhexKeyPressed() {
	if (!hasFocus) {
		onUpdateFocus(true);
		return;
	}
	if (!isMinimized) {
		minimize();
		return;
	}
	onUpdateFocus(false);
}

void Phex::onPhexKeyPressed2() {
	if (isMinimized) {
		maximize();
		onUpdateFocus(true);
		return;
	}
	minimize();
	onUpdateFocus(false);
}

bool Phex::onUpdateFocus(bool focus) {
	if (hasFocus == focus) return false;
	hasFocus = focus;
	bDrawRecInput = hasFocus;
	inputText.bDrawCursor = hasFocus;
	return true;
}

void Phex::onMouseEvent(float x, float y) {
	if (!HetuwMod::phexIsEnabled) return;
	HetuwMod::pointFromMapToPercentCoords(x, y);
	for(unsigned k=0; k<buttons.size(); k++) {
		buttons[k]->onMouseEvent(x, y);
	}
}

bool Phex::onMouseDown(float x, float y) {
	if (!HetuwMod::phexIsEnabled) return false;
	HetuwMod::pointFromMapToPercentCoords(x, y);
	if (!HetuwMod::pointIsInsideRec(recBckgr, x, y)) {
		onUpdateFocus(false);
		return false;
	}
	for(unsigned k=0; k<buttons.size(); k++) {
		if (buttons[k]->onMouseDown(x, y)) return true;
	}
	if (!butPhex.visible) {
		onUpdateFocus(true);
		return true;
	} else return false;
}

bool Phex::onMouseUp(float x, float y) {
	if (!HetuwMod::phexIsEnabled) return false;
	HetuwMod::pointFromMapToPercentCoords(x, y);
	for(unsigned k=0; k<buttons.size(); k++) {
		if (buttons[k]->onMouseUp(x, y)) return true;
	}
	return false;
}

void Phex::onKey(unsigned char inASCII) {
	if (!hasFocus) return;
	addToInputStr(inASCII);
}

bool Phex::onKeyDown(unsigned char inASCII) {
	if (!HetuwMod::phexIsEnabled) return false;
	if (HetuwMod::isCharKey(inASCII, HetuwMod::charKey_Phex)) {
		if (isCommandKeyDown()) onPhexKeyPressed2();
		else onPhexKeyPressed();
		return true;
	}
	keyHandler.onKeyDown(inASCII);
	if (!hasFocus) return false;
	return true;
}

bool Phex::onKeyUp(unsigned char inASCII) {
	if (!HetuwMod::phexIsEnabled) return false;
	keyHandler.onKeyUp(inASCII);
	if (!hasFocus) return false;
	return true;
}
