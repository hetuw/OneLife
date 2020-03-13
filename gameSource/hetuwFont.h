#ifndef HETUWFONT_INCLUDED
#define HETUWFONT_INCLUDED


#include "minorGems/game/gameGraphics.h"
#include "minorGems/util/SimpleVector.h"
#include "minorGems/game/Font.h"

#define hetuwFontColorCode 7


class HetuwFont {
        
    public:
        
        // file contains TGA with 16x16 ascii table
        // if inFixedCharWidth > 0, it overrides sprite width for width
        // of fixed-spacing characters.
        HetuwFont( const char *inFileName, int inCharSpacing, int inSpaceWidth,
              char inFixedWidth,
              double inScaleFactor = 1.0,
              int inFixedCharWidth = 0 );
        
        ~HetuwFont();
        
		void hetuwSetScaleFactor( double newScaleFactor );
		double hetuwGetScaleFactor();
		double hetuwMaxX;
		bool hetuwMaxXActive;
		int hetuwCheckForColorCode(const char *inString, int i, bool isDrawing);
		char* hetuwGetColorCode(const float rgba[]);
		double hetuwWidth;
		double hetuwHeight;
		double hetuwWidthLastLine;
		doublePair hetuwNextCharPos;
		double hetuwGetHalfSpriteHeight();
		double hetuwGetSpaceWidth();
		bool hetuwIgnoreSpacesAtStartOfLine = true;

        // copies character spacing and kerning from another HetuwFont
        // (useful for erased HetuwFonts that can't be scanned properly for
        //  spacing but must line up with non-erased version perfectly)
        void copySpacing( HetuwFont *inOtherHetuwFont );
        
        
        // draws a string on the current screen using set draw color
        // returns x coordinate of string end
        double drawString( const char *inString, doublePair inPosition,
                           TextAlignment inAlign = alignCenter );


        double measureString( const char *inString, int inCharLimit = -1 );
        
        // gets per-character position of string without drawing it
        double getCharPos( SimpleVector<doublePair> *outPositions,
            const char *inString, doublePair inPosition,
            TextAlignment inAlign = alignCenter );
        

        // height of basic, non-accented characters
        double getFontHeight();
        

        // defaults to being on
        void enableKerning( char inKerningOn );


        // sets minimum floating point precision for positioning
        // the start of a string (to avoid round-off errors on graphics
        // cards that don't handle sub-pixel polygon positioning consistently)
        // In most cases, this should be the world dimensions of one
        // screen pixel.
        // Example:  if set to 0.5, the positions of 10.0, 10.5, 11.0
        // are allowed, but 10.35 will be rounded down to 10.0 before the
        // string is drawn.
        // If set to 2, only even positions are allowed, and odd positions
        // are rounded down.
        // Defaults to 0, which indicates no precision limit.
        void setMinimumPositionPrecision( double inMinimum );
        

        double getCharSpacing();
        
        // returns x coordinate to right of drawn character
        double drawCharacter( unsigned char inC, doublePair inPosition );

        // draws sprite centered on inPosition with no spacing adjustments
        void drawCharacterSprite( unsigned char inC, doublePair inPosition );

    private:        
        
        // returns x coordinate to right of drawn character
        double positionCharacter( unsigned char inC, doublePair inTargetPos,
                                  doublePair *outActualPos );

        
        double mScaleFactor;
        
        
        int mCharSpacing;
        int mSpaceWidth;
        
        char mFixedWidth;
        
        int mSpriteWidth;
        int mSpriteHeight;
        
        char mAccentsPresent;
        

        int mCharBlockWidth;
        

        // maps ascii chars to sprite IDs
        SpriteHandle mSpriteMap[ 256 ];
        
        // for kerning (ignored if fixed width flag on)
        int mCharLeftEdgeOffset[ 256 ];
        int mCharWidth[ 256 ];
        
        
        // sparse
        // contains NULL for every blank character
        KerningTable *mKerningTable[256];
        

        char mEnableKerning;

        double mMinimumPositionPrecision;
    };


#endif
