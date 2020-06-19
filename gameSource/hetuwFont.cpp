#include "hetuwFont.h"

#include "minorGems/graphics/RGBAImage.h"
#include "minorGems/game/Font.h"

#include <string.h>


typedef union rgbaColor {
        struct comp { 
                unsigned char r;
                unsigned char g;
                unsigned char b;
                unsigned char a;
            } comp;
        
        // access those bytes as an array
        unsigned char bytes[4];
        
        // reinterpret those bytes as an unsigned int
        unsigned int rgbaInt; 
    } rgbaColor;


// what alpha level counts as "ink" when measuring character width
// and doing kerning
// values at or below this level will not count as ink
// this improves kerning and font spacing, because dim "tips" of pointed
// glyphs don't cause the glyph to be logically wider than it looks visually 
const unsigned char inkA = 127;



HetuwFont::HetuwFont( const char *inFileName, int inCharSpacing, int inSpaceWidth,
            char inFixedWidth, double inScaleFactor, int inFixedCharWidth )
        : mScaleFactor( inScaleFactor ),
          mCharSpacing( inCharSpacing ), mSpaceWidth( inSpaceWidth ),
          mFixedWidth( inFixedWidth ), mEnableKerning( true ),
          mMinimumPositionPrecision( 0 ) {

    for( int i=0; i<256; i++ ) {
        mSpriteMap[i] = NULL;
        mKerningTable[i] = NULL;
        }
    
	hetuwMaxXActive = false;

    Image *spriteImage = readTGAFile( inFileName );
    
    if( spriteImage != NULL ) {
        
        int width = spriteImage->getWidth();
        
        int height = spriteImage->getHeight();
        
        int numPixels = width * height;
        
        rgbaColor *spriteRGBA = new rgbaColor[ numPixels ];
        
        
        unsigned char *spriteBytes = 
            RGBAImage::getRGBABytes( spriteImage );
        
        delete spriteImage;

        for( int i=0; i<numPixels; i++ ) {
            
            for( int b=0; b<4; b++ ) {
                
                spriteRGBA[i].bytes[b] = spriteBytes[ i*4 + b ];
                }
            }
        
        delete [] spriteBytes;
        
        

        // use red channel intensity as transparency
        // make entire image solid white and use transparency to 
        // mask it

        for( int i=0; i<numPixels; i++ ) {
            spriteRGBA[i].comp.a = spriteRGBA[i].comp.r;
            
            spriteRGBA[i].comp.r = 255;
            spriteRGBA[i].comp.g = 255;
            spriteRGBA[i].comp.b = 255;
            }
            
                        
                
        mSpriteWidth = width / 16;
        mSpriteHeight = height / 16;
        
        if( mSpriteHeight == mSpriteWidth ) {
            mAccentsPresent = false;
            }
        else {
            mAccentsPresent = true;
            }

        if( inFixedCharWidth == 0 ) {
            mCharBlockWidth = mSpriteWidth;
            }
        else {
            mCharBlockWidth = inFixedCharWidth;
            }


        int pixelsPerChar = mSpriteWidth * mSpriteHeight;
            
        // hold onto these for true kerning after
        // we've read this data for all characters
        rgbaColor *savedCharacterRGBA[256];
        

        for( int i=0; i<256; i++ ) {
            int yOffset = ( i / 16 ) * mSpriteHeight;
            int xOffset = ( i % 16 ) * mSpriteWidth;
            
            rgbaColor *charRGBA = new rgbaColor[ pixelsPerChar ];
            
            for( int y=0; y<mSpriteHeight; y++ ) {
                for( int x=0; x<mSpriteWidth; x++ ) {
                    
                    int imageIndex = (y + yOffset) * width
                        + x + xOffset;
                    int charIndex = y * mSpriteWidth + x;
                    
                    charRGBA[ charIndex ] = spriteRGBA[ imageIndex ];
                    }
                }
                
            // don't bother consuming texture ram for blank sprites
            char allTransparent = true;
            
            for( int p=0; p<pixelsPerChar && allTransparent; p++ ) {
                if( charRGBA[ p ].comp.a != 0 ) {
                    allTransparent = false;
                    }
                }
                
            if( !allTransparent ) {
                
                // convert into an image
                Image *charImage = new Image( mSpriteWidth, mSpriteHeight,
                                              4, false );
                
                for( int c=0; c<4; c++ ) {
                    double *chan = charImage->getChannel(c);
                    
                    for( int p=0; p<pixelsPerChar; p++ ) {
                        
                        chan[p] = charRGBA[p].bytes[c] / 255.0;
                        }
                    }
                

                mSpriteMap[i] = 
                    fillSprite( charImage );
                delete charImage;
                }
            else {
                mSpriteMap[i] = NULL;
                }
            

            if( mFixedWidth ) {
                mCharLeftEdgeOffset[i] = 0;
                mCharWidth[i] = mCharBlockWidth;
                }
            else if( allTransparent ) {
                mCharLeftEdgeOffset[i] = 0;
                mCharWidth[i] = mSpriteWidth;
                }
            else {
                // implement pseudo-kerning
                
                int farthestLeft = mSpriteWidth;
                int farthestRight = 0;
                
                char someInk = false;
                
                for( int y=0; y<mSpriteHeight; y++ ) {
                    for( int x=0; x<mSpriteWidth; x++ ) {
                        
                        unsigned char a = 
                            charRGBA[ y * mSpriteWidth + x ].comp.a;
                        
                        if( a > inkA ) {
                            someInk = true;
                            
                            if( x < farthestLeft ) {
                                farthestLeft = x;
                                }
                            if( x > farthestRight ) {
                                farthestRight = x;
                                }
                            }
                        }
                    }
                
                if( ! someInk  ) {
                    mCharLeftEdgeOffset[i] = 0;
                    mCharWidth[i] = mSpriteWidth;
                    }
                else {
                    mCharLeftEdgeOffset[i] = farthestLeft;
                    mCharWidth[i] = farthestRight - farthestLeft + 1;
                    }
                }
                

            if( !allTransparent && ! mFixedWidth ) {
                savedCharacterRGBA[i] = charRGBA;
                }
            else {
                savedCharacterRGBA[i] = NULL;
                delete [] charRGBA;
                }
            }
        

        // now that we've read in all characters, we can do real kerning
        if( !mFixedWidth )
        for( int i=0; i<256; i++ ) {
            if( savedCharacterRGBA[i] != NULL ) {
                
                mKerningTable[i] = new KerningTable;


                // for each character that could come after this character
                for( int j=0; j<256; j++ ) {

                    mKerningTable[i]->offset[j] = 0;

                    // not a blank character
                    if( savedCharacterRGBA[j] != NULL ) {
                        
                        short minDistance = 2 * mSpriteWidth;

                        // for each pixel row, find distance
                        // between the right extreme of the first character
                        // and the left extreme of the second
                        for( int y=0; y<mSpriteHeight; y++ ) {
                            
                            int rightExtreme = 0;
                            int leftExtreme = mSpriteWidth;
                            
                            for( int x=0; x<mSpriteWidth; x++ ) {
                                int p = y * mSpriteWidth + x;
                                
                                if( savedCharacterRGBA[i][p].comp.a > inkA ) {
                                    rightExtreme = x;
                                    }
                                if( x < leftExtreme &&
                                    savedCharacterRGBA[j][p].comp.a > inkA ) {
                                    
                                    leftExtreme = x;
                                    }
                                // also check pixel rows above and below
                                // for left character, to look for
                                // diagonal collisions (perfect nesting
                                // with no vertical gap)
                                if( y > 0 && x < leftExtreme ) {
                                    int pp = (y-1) * mSpriteWidth + x;
                                    if( savedCharacterRGBA[j][pp].comp.a 
                                        > inkA ) {
                                    
                                        leftExtreme = x;
                                        }
                                    }
                                if( y < mSpriteHeight - 1 
                                    && x < leftExtreme ) {
                                    
                                    int pp = (y+1) * mSpriteWidth + x;
                                    if( savedCharacterRGBA[j][pp].comp.a 
                                        > inkA ) {
                                    
                                        leftExtreme = x;
                                        }
                                    }
                                }
                            
                            int rowDistance =
                                ( mSpriteWidth - rightExtreme - 1 ) 
                                + leftExtreme;

                            if( rowDistance < minDistance ) {
                                minDistance = rowDistance;
                                }
                            }
                        
                        // have min distance across all rows for 
                        // this character pair

                        // of course, we've already done pseudo-kerning
                        // based on character width, so take that into account
                        // true kerning is a tweak to that
                        
                        // pseudo-kerning already accounts for
                        // gap to left of second character
                        minDistance -= mCharLeftEdgeOffset[j];
                        // pseudo-kerning already accounts for gap to right
                        // of first character
                        minDistance -= 
                            mSpriteWidth - 
                            ( mCharLeftEdgeOffset[i] + mCharWidth[i] );
                        
                        if( minDistance > 0 
                            // make sure we don't have a full overhang
                            // for characters that don't collide horizontally
                            // at all
                            && minDistance < mCharWidth[i] ) {
                            
                            mKerningTable[i]->offset[j] = - minDistance;
                            }
                        }
                    }
                
                }
            }
        

        for( int i=0; i<256; i++ ) {
            if( savedCharacterRGBA[i] != NULL ) {
                delete [] savedCharacterRGBA[i];
                }
            }
        

        delete [] spriteRGBA;
        }
    }



HetuwFont::~HetuwFont() {
    for( int i=0; i<256; i++ ) {
        if( mSpriteMap[i] != NULL ) {
            freeSprite( mSpriteMap[i] );
            }
        if( mKerningTable[i] != NULL ) {
            delete mKerningTable[i];
            }
        }
    }

void HetuwFont::hetuwSetScaleFactor( double newScaleFactor ) {
	mScaleFactor = newScaleFactor;
}

double HetuwFont::hetuwGetScaleFactor() {
	return mScaleFactor;
}

void HetuwFont::copySpacing( HetuwFont *inOtherHetuwFont ) {
    memcpy( mCharLeftEdgeOffset, inOtherHetuwFont->mCharLeftEdgeOffset,
            256 * sizeof( int ) );

    memcpy( mCharWidth, inOtherHetuwFont->mCharWidth,
            256 * sizeof( int ) );
    

    for( int i=0; i<256; i++ ) {
        if( mKerningTable[i] != NULL ) {
            delete mKerningTable[i];
            mKerningTable[i] = NULL;
            }

        if( inOtherHetuwFont->mKerningTable[i] != NULL ) {
            mKerningTable[i] = new KerningTable;
            memcpy( mKerningTable[i]->offset,
                    inOtherHetuwFont->mKerningTable[i]->offset,
                    256 * sizeof( short ) );
            }
        }

    mScaleFactor = inOtherHetuwFont->mScaleFactor;
        
    
    mCharSpacing = inOtherHetuwFont->mCharSpacing;
    mSpaceWidth = inOtherHetuwFont->mSpaceWidth;
        
    mFixedWidth = inOtherHetuwFont->mFixedWidth;
        
    mSpriteWidth = inOtherHetuwFont->mSpriteWidth;
    mSpriteHeight = inOtherHetuwFont->mSpriteHeight;
    
    mAccentsPresent = inOtherHetuwFont->mAccentsPresent;
        

    mCharBlockWidth = inOtherHetuwFont->mCharBlockWidth;
    }



// double pixel size
static double scaleFactor = 1.0 / 16;
//static double scaleFactor = 1.0 / 8;



double HetuwFont::getCharSpacing() {
    double scale = scaleFactor * mScaleFactor;
    
    return mCharSpacing * scale;
    }



double HetuwFont::getCharPos( SimpleVector<doublePair> *outPositions,
                         const char *inString, doublePair inPosition,
                         TextAlignment inAlign ) {

    double scale = scaleFactor * mScaleFactor;
    
    unsigned int numChars = strlen( inString );
    
    double x = inPosition.x;
    
    
    double y = inPosition.y;

	double lineHeight = getFontHeight(); // hetuw

    // compensate for extra headspace in accent-equipped HetuwFont files
    if( mAccentsPresent ) { 
        y += scale * mSpriteHeight / 4;
        }

    
    double stringWidth = 0;
    
    if( inAlign != alignLeft ) {
        stringWidth = measureString( inString );
        }
    
    switch( inAlign ) {
        case alignCenter:
            x -= stringWidth / 2;
            break;
        case alignRight:
            x -= stringWidth;
            break;
        default:
            // left?  do nothing
            break;            
        }
    
    // character sprites are drawn on their centers, so the alignment
    // adjustments above aren't quite right.
    x += scale * mSpriteWidth / 2;


    if( mMinimumPositionPrecision > 0 ) {
        x /= mMinimumPositionPrecision;
        
        x = lrint( floor( x ) );
        
        x *= mMinimumPositionPrecision;
        }

	hetuwNextCharPos = {x,y};
	hetuwWidth = 0;
	hetuwWidthLastLine = 0;
	hetuwHeight = mSpriteHeight/2*scale;
	bool skipChar = false;

	double startX = x;
    for( unsigned int i=0; i<numChars; i++ ) {
		int addToI = hetuwCheckForColorCode(inString, i, false);
		if (addToI != 0) { i += addToI; continue; }

        doublePair charPos = { x, y };
        
        doublePair drawPos;
        
        double charWidth = positionCharacter( (unsigned char)( inString[i] ), 
                                              charPos, &drawPos );

		if (hetuwIgnoreSpacesAtStartOfLine) {
			skipChar = false;
			if (charPos.x == startX) {
				if (inString[i] == ' ') {
					drawPos.x = inPosition.x;
					skipChar = true;
				}
			}
		}

        outPositions->push_back( drawPos );
		if (skipChar) continue;
        
		if ((inString[i] == '\n' || (hetuwMaxXActive && (x+charWidth) > hetuwMaxX)) && i+1 < numChars) {
			x = startX;
			if (x-inPosition.x > hetuwWidth) hetuwWidth = x-inPosition.x;
			y -= lineHeight;
			hetuwNextCharPos = {x, y};
			hetuwHeight += lineHeight;
			continue;
		}
        
		hetuwWidthLastLine = x-inPosition.x;
		if (hetuwWidthLastLine > hetuwWidth) hetuwWidth = hetuwWidthLastLine;
        x += charWidth + mCharSpacing * scale;
		hetuwNextCharPos.x = x;

        if( !mFixedWidth && mEnableKerning 
            && i < numChars - 1 
            && mKerningTable[(unsigned char)( inString[i] )] != NULL 
			&& inString[i+1] != hetuwFontColorCode ) {
            // there's another character after this
            // apply true kerning adjustment to the pair
            int offset = mKerningTable[ (unsigned char)( inString[i] ) ]->
                offset[ (unsigned char)( inString[i+1] ) ];
            x += offset * scale;
            }
        }
    // no spacing after last character
    x -= mCharSpacing * scale;
	hetuwNextCharPos.x = x - hetuwGetSpaceWidth();
	//hetuwWidth -= mCharSpacing * scale;

    return x;
    }

int HetuwFont::hetuwCheckForColorCode(const char *inString, int i, bool isDrawing) {
	if (inString[i] != (char)hetuwFontColorCode) return 0;
	//printf("Phex sizeOf(inString): %ld, i: %d\n", strlen(inString), i);
	if (strlen(inString) < (unsigned)(i+5)) return 0;
	//printf("Phex isDrawing: %d\n", (int)isDrawing);
	if (!isDrawing) return 4;
	setDrawColor((inString[i+1]-1)/126.0, (inString[i+2]-1)/126.0, (inString[i+3]-1)/126.0, (inString[i+4]-1)/126.0);
	//printf("Phex text drawColor %f %f %f %f\n", inString[i+1]/127.0, inString[i+2]/127.0, inString[i+3]/127.0, inString[i+4]/127.0);
	return 4;
}

char* HetuwFont::hetuwGetColorCode(const float rgba[]) {
	char* colorCode = new char[6];
	colorCode[0] = hetuwFontColorCode;
	for (int i=1; i<5; i++) colorCode[i] = (char)(1+rgba[i-1]*126);
	colorCode[5] = 0;
	return colorCode;
}


double HetuwFont::drawString( const char *inString, doublePair inPosition,
                         TextAlignment inAlign ) {
    SimpleVector<doublePair> pos( strlen( inString ) );

    double returnVal = getCharPos( &pos, inString, inPosition, inAlign );
	if (pos.size() <= 0) return returnVal;

    double scale = scaleFactor * mScaleFactor;
    
	int k = 0;
	double lastX = 99999999;
    for( int i=0; k<pos.size(); i++, k++ ) {
		int addToI = hetuwCheckForColorCode(inString, i, true);
		if (addToI != 0) { i += addToI; k--; continue; }
	
		if (hetuwIgnoreSpacesAtStartOfLine) {
			if (pos.getElementDirect(k).x <= lastX) { // new line
				if (inString[i] == ' ') continue;
			}
			lastX = pos.getElementDirect(k).x;
		}

        SpriteHandle spriteID = mSpriteMap[ (unsigned char)( inString[i] ) ];
    
        if( spriteID != NULL ) {
            drawSprite( spriteID, pos.getElementDirect(k), scale );
            }
    
        }
    
    return returnVal;
    }



double HetuwFont::hetuwGetSpaceWidth() {
	return mSpaceWidth * scaleFactor * mScaleFactor;
}

double HetuwFont::positionCharacter( unsigned char inC, doublePair inTargetPos,
                                doublePair *outActualPos ) {
    *outActualPos = inTargetPos;
    
    double scale = scaleFactor * mScaleFactor;

    if( inC == ' ' ) {
        return mSpaceWidth * scale;
        }

    if( !mFixedWidth ) {
        outActualPos->x -= mCharLeftEdgeOffset[ inC ] * scale;
        }
    
    if( mFixedWidth ) {
        return mCharBlockWidth * scale;
        }
    else {
        return mCharWidth[ inC ] * scale;
        }
    }

    


double HetuwFont::drawCharacter( unsigned char inC, doublePair inPosition ) {
    
    doublePair drawPos;
    double returnVal = positionCharacter( inC, inPosition, &drawPos );

    if( inC == ' ' ) {
        return returnVal;
        }

    SpriteHandle spriteID = mSpriteMap[ inC ];
    
    if( spriteID != NULL ) {
        double scale = scaleFactor * mScaleFactor;
        drawSprite( spriteID, drawPos, scale );
        }
    
    return returnVal;
    }



void HetuwFont::drawCharacterSprite( unsigned char inC, doublePair inPosition ) {
    SpriteHandle spriteID = mSpriteMap[ inC ];
    
    if( spriteID != NULL ) {
        double scale = scaleFactor * mScaleFactor;
        drawSprite( spriteID, inPosition, scale );
        }
    }



double HetuwFont::measureString( const char *inString, int inCharLimit ) {
    double scale = scaleFactor * mScaleFactor;

    int numChars = inCharLimit;

    if( numChars == -1 ) {
        // no limit, measure whole string
        numChars = strlen( inString );
        }
    
    double width = 0;
    
    for( int i=0; i<numChars; i++ ) {
        unsigned char c = inString[i];
        
        if( c == ' ' ) {
            width += mSpaceWidth * scale;
            }
        else if( mFixedWidth ) {
            width += mCharBlockWidth * scale;
            }
        else {
            width += mCharWidth[ c ] * scale;

            if( mEnableKerning
                && i < numChars - 1 
                && mKerningTable[(unsigned char)( inString[i] )] != NULL ) {
                // there's another character after this
                // apply true kerning adjustment to the pair
                int offset = mKerningTable[ (unsigned char)( inString[i] ) ]->
                    offset[ (unsigned char)( inString[i+1] ) ];
                width += offset * scale;
                }
            }
    
        width += mCharSpacing * scale;
        }

    if( numChars > 0 ) {    
        // no extra space at end
        // (added in last step of loop)
        width -= mCharSpacing * scale;
        }
    
    return width;
    }



double HetuwFont::getFontHeight() {
    double accentFactor = 1.0f;
    
    if( mAccentsPresent ) {
        accentFactor = 0.5f;
        }
    
    return scaleFactor * mScaleFactor * mSpriteHeight * accentFactor;
    }

double HetuwFont::hetuwGetHalfSpriteHeight() {
	return mSpriteHeight/4.0*scaleFactor*mScaleFactor;
}


void HetuwFont::enableKerning( char inKerningOn ) {
    mEnableKerning = inKerningOn;
    }



void HetuwFont::setMinimumPositionPrecision( double inMinimum ) {
    mMinimumPositionPrecision = inMinimum;
    }



