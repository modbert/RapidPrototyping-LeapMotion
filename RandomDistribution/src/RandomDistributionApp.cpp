#include "cinder/app/AppNative.h"
#include "cinder/gl/Texture.h"
#include "cinder/gl/gl.h"
#include "cinder/Text.h"
#include "cinder/Utilities.h"
#include "cinder/ImageIo.h"
#include "cinder/Font.h"
#include "Resources.h"

using namespace ci;
using namespace ci::app;
using namespace std;

#include <list>
using std::list;

static const bool PREMULT = false;


class RandomDistributionApp : public AppNative {
public:
	void setup();
	void draw();
	void prepareSettings( Settings *settings );
	void printFontNames();
	void update();
	void mouseMove(MouseEvent event );
	string aString;
	int mMouseLoc;

	gl::Texture	mTexture, mSimpleTexture, aTexture, bTexture, cTexture, dTexture, eTexture, fTexture, gTexture, hTexture, iTexture, jTexture;
		ci::ColorA       mColor;
	ci::ColorA       aColor;
	ci::ColorA       bColor;
	ci::ColorA       cColor;
	ci::ColorA       dColor;
	ci::ColorA       eColor;
	ci::ColorA       fColor;
	ci::ColorA       gColor;
	ci::ColorA       hColor;
	ci::ColorA       iColor;
	ci::ColorA       jColor;

};

void RandomDistributionApp::prepareSettings( Settings *settings ) {
    settings->setWindowSize(960,720);
    settings->setFrameRate(60.f);
}

void RandomDistributionApp::printFontNames()
{
	for( vector<string>::const_iterator fontName = Font::getNames().begin(); fontName != Font::getNames().end(); ++fontName )
		console() << *fontName << endl;
}

void RandomDistributionApp::setup()
{
    aColor=ColorA(0.98,0.97,1);
	bColor=ColorA(0.90,0.89,1);
	cColor=ColorA(0.83,0.8,1);
	dColor=ColorA(0.78,0.75,1);
	eColor=ColorA(0.71,0.66,1);
	fColor=ColorA(0.65,0.61,1);
	gColor=ColorA(0.57,0.48,1);
	hColor=ColorA(0.45,0.39,1);
	iColor=ColorA(0.36,0.31,1);
	jColor=ColorA(0.26,0.20,1);

	mMouseLoc = 0;

	printFontNames();

#if defined( CINDER_COCOA_TOUCH )
	std::string normalFont( "Arial" );
	std::string boldFont( "Arial-BoldMT" );
	std::string differentFont( "AmericanTypewriter" );
#else
	std::string normalFont( "Papyrus" );
	std::string boldFont( "Arial Bold" );
	std::string differentFont( "Arial-BoldMT" );
#endif


	// Japanese
	//unsigned char japanese[] = { 0xE6, 0x97, 0xA5, 0xE6, 0x9C, 0xAC, 0xE8, 0xAA, 0x9E, 0 };
	// this does a complicated layout
	//TextLayout layout;
	//layout.clear( ColorA( 0.2f, 0.2f, 0.2f, 0.2f ) );
	//layout.setFont( Font( normalFont, 24 ) );
	//layout.setColor( Color( 1, 1, 1 ) );
	//layout.addLine( std::string( "Unicode: " ) + (const char*)japanese );
	//layout.setColor( Color( 0.5f, 0.25f, 0.8f ) );
	//layout.setFont( Font( boldFont, 12 ) );
	//layout.addRightLine( "Now is the time" );
	//layout.setFont( Font( normalFont, 22 ) );
	//layout.setColor( Color( 0.75f, 0.25f, 0.6f ) );
	//layout.append( " for all good men" );
	//layout.addCenteredLine( "center justified" );
	//layout.addRightLine( "right justified" );
	//layout.setFont( Font( differentFont, 48 ) );
	//layout.setColor( Color( 1, 1, 1 ) );
	//layout.addCenteredLine( "The same font" );
	//layout.setFont( Font( normalFont, 22 ) );
	//layout.setColor( Color( 1.0f, 0.5f, 0.25f ) );
	//layout.addLine( " • Point 1 " );
	//layout.setLeadingOffset( -10 );
	//layout.addLine( " • Other point with -10 leading offset " );
	//layout.setLeadingOffset( 0 );
	//layout.setColor( ColorA( 0.25f, 0.5f, 1, 0.5f ) );
	//layout.addLine( " • Back to regular leading but translucent" );
	//Surface8u rendered = layout.render( true, PREMULT );
	//mTexture = gl::Texture( rendered );

	TextLayout alayout;
	alayout.setFont( Font( differentFont, 72 ) );
	alayout.setColor( Color( 1, 1, 1 ) );
	alayout.addCenteredLine( "F" );

	Surface8u arendered = alayout.render( true, PREMULT );
	aTexture = gl::Texture( arendered );

	TextLayout blayout;
	blayout.setFont( Font( differentFont, 72 ) );
	blayout.setColor( Color( 1, 1, 1 ) );
	blayout.addCenteredLine( "G" );

	Surface8u brendered = blayout.render( true, PREMULT );
	bTexture = gl::Texture( brendered );

	TextLayout clayout;
	clayout.setFont( Font( differentFont, 72 ) );
	clayout.setColor( Color( 1, 1, 1 ) );
	clayout.addCenteredLine( "A" );

	Surface8u crendered = clayout.render( true, PREMULT );
	cTexture = gl::Texture( crendered );

	TextLayout dlayout;
	dlayout.setFont( Font( differentFont, 72 ) );
	dlayout.setColor( Color( 1, 1, 1 ) );
	dlayout.addCenteredLine( "B" );

	Surface8u drendered = dlayout.render( true, PREMULT );
	dTexture = gl::Texture( drendered );

	TextLayout elayout;
	elayout.setFont( Font( differentFont, 72 ) );
	elayout.setColor( Color( 1, 1, 1 ) );
	elayout.addCenteredLine( "C" );

	Surface8u erendered = elayout.render( true, PREMULT );
	eTexture = gl::Texture( erendered );


	TextLayout flayout;
	flayout.setFont( Font( differentFont, 72 ) );
	flayout.setColor( Color( 1, 1, 1 ) );
	flayout.addCenteredLine( "D" );

	Surface8u frendered = flayout.render( true, PREMULT );
	fTexture = gl::Texture( frendered );


	TextLayout glayout;
	glayout.setFont( Font( differentFont, 72 ) );
	glayout.setColor( Color( 1, 1, 1 ) );
	glayout.addCenteredLine( "E" );

	Surface8u grendered = glayout.render( true, PREMULT );
	gTexture = gl::Texture( grendered );

	TextLayout hlayout;
	hlayout.setFont( Font( differentFont, 72 ) );
	hlayout.setColor( Color( 1, 1, 1 ) );
	hlayout.addCenteredLine( "F" );

	Surface8u hrendered = hlayout.render( true, PREMULT );
	hTexture = gl::Texture( hrendered );

	TextLayout ilayout;
	ilayout.setFont( Font( differentFont, 72 ) );
	ilayout.setColor( Color( 1, 1, 1 ) );
	ilayout.addCenteredLine( "G" );

	Surface8u irendered = ilayout.render( true, PREMULT );
	iTexture = gl::Texture( irendered );

	TextLayout jlayout;
	jlayout.setFont( Font( differentFont, 72 ) );
	jlayout.setColor( Color( 1, 1, 1 ) );
	jlayout.addCenteredLine( "A" );

	Surface8u jrendered = jlayout.render( true, PREMULT );
	jTexture = gl::Texture( jrendered );
}

void RandomDistributionApp::mouseMove( MouseEvent event ) 
{
    mMouseLoc = event.getY();
	//console() << mMouseLoc << std::endl;
}

void RandomDistributionApp::update()
{
   #if defined( CINDER_COCOA_TOUCH )
	std::string normalFont( "Arial" );
	std::string boldFont( "Arial-BoldMT" );
	std::string differentFont( "AmericanTypewriter" );
#else
	std::string normalFont( "Arial" );
	std::string boldFont( "Arial Bold" );
	std::string differentFont( "Arial-BoldMT" );
#endif
		if(mMouseLoc > 213 && mMouseLoc < 229)
	{
		//console() << "Special thing happened!" << std::endl;
		aColor = ColorA(0.16,1,0.05);
		TextLayout alayout;
		alayout.setFont( Font( differentFont, 72 ) );
		alayout.setColor( Color( 1, 1, 1 ) );
		alayout.addCenteredLine( "B" );
		Surface8u arendered = alayout.render( true, PREMULT );
		aTexture = gl::Texture( arendered );
	}
	else
	{
		aColor=ColorA(0.98,0.97,1);
		TextLayout alayout;
		alayout.setFont( Font( differentFont, 72 ) );
		alayout.setColor( Color( 1, 1, 1 ) );
		alayout.addCenteredLine( "" );
		Surface8u arendered = alayout.render( true, PREMULT );
		aTexture = gl::Texture( arendered );
	}
		if(mMouseLoc > 230 && mMouseLoc < 246)
	{
		bColor = ColorA(1,0.32,.05);
		TextLayout blayout;
		blayout.setFont( Font( differentFont, 72 ) );
		blayout.setColor( Color( 1, 1, 1 ) );
		blayout.addCenteredLine( "C" );
		Surface8u brendered = blayout.render( true, PREMULT );
		bTexture = gl::Texture( brendered );

	}
	else
	{
		bColor=ColorA(0.90,0.89,1);
		TextLayout blayout;
		blayout.setFont( Font( differentFont, 72 ) );
		blayout.setColor( Color( 1, 1, 1 ) );
		blayout.addCenteredLine( "" );
		Surface8u brendered = blayout.render( true, PREMULT );
		bTexture = gl::Texture( brendered );
	}
	if(mMouseLoc > 268 && mMouseLoc < 282)
	{
		cColor = ColorA(0.21,0.07,1);
		TextLayout clayout;
		clayout.setFont( Font( differentFont, 72 ) );
		clayout.setColor( Color( 1, 1, 1 ) );
		clayout.addCenteredLine( "D" );
		Surface8u crendered = clayout.render( true, PREMULT );
		cTexture = gl::Texture( crendered );

	}
	else
	{
		cColor=ColorA(0.83,0.8,1);
		TextLayout clayout;
		clayout.setFont( Font( differentFont, 72 ) );
		clayout.setColor( Color( 1, 1, 1 ) );
		clayout.addCenteredLine( "" );
		Surface8u crendered = clayout.render( true, PREMULT );
		cTexture = gl::Texture( crendered );
	}
	if(mMouseLoc > 307 && mMouseLoc < 323)
	{
		dColor = ColorA(0.9,0.1,1);
		TextLayout dlayout;
		dlayout.setFont( Font( differentFont, 72 ) );
		dlayout.setColor( Color( 1, 1, 1 ) );
		dlayout.addCenteredLine( "E" );
		Surface8u drendered = dlayout.render( true, PREMULT );
		dTexture = gl::Texture( drendered );
	}
	else
	{
		dColor=ColorA(0.78,0.75,1);
		TextLayout dlayout;
		dlayout.setFont( Font( differentFont, 72 ) );
		dlayout.setColor( Color( 1, 1, 1 ) );
		dlayout.addCenteredLine( "" );
		Surface8u drendered = dlayout.render( true, PREMULT );
		dTexture = gl::Texture( drendered );

	}
	if(mMouseLoc > 329 && mMouseLoc < 345)
	{
		eColor = ColorA(1,0.83,0.05);
		TextLayout elayout;
		elayout.setFont( Font( differentFont, 72 ) );
		elayout.setColor( Color( 1, 1, 1 ) );
		elayout.addCenteredLine( "F" );
		Surface8u erendered = elayout.render( true, PREMULT );
		eTexture = gl::Texture( erendered );
	}
	else
	{
		eColor=ColorA(0.71,0.66,1);
		TextLayout elayout;
		elayout.setFont( Font( differentFont, 72 ) );
		elayout.setColor( Color( 1, 1, 1 ) );
		elayout.addCenteredLine( "" );
		Surface8u erendered = elayout.render( true, PREMULT );
		eTexture = gl::Texture( erendered );
	}
	if(mMouseLoc > 377 && mMouseLoc < 393)
	{
		fColor = ColorA(0.25,1,0.5);
		TextLayout flayout;
		flayout.setFont( Font( differentFont, 72 ) );
		flayout.setColor( Color( 1, 1, 1 ) );
		flayout.addCenteredLine( "G" );
		Surface8u frendered = flayout.render( true, PREMULT );
		fTexture = gl::Texture( frendered );
	}
	else
	{
		fColor=ColorA(0.65,0.61,1);
		TextLayout flayout;
		flayout.setFont( Font( differentFont, 72 ) );
		flayout.setColor( Color( 1, 1, 1 ) );
		flayout.addCenteredLine( "" );
		Surface8u frendered = flayout.render( true, PREMULT );
		fTexture = gl::Texture( frendered );
	}
	if(mMouseLoc > 432 && mMouseLoc < 448)
	{
		gColor = ColorA(1,0.09,0.29);
		TextLayout glayout;
		glayout.setFont( Font( differentFont, 72 ) );
		glayout.setColor( Color( 1, 1, 1 ) );
		glayout.addCenteredLine( "A" );
		Surface8u grendered = glayout.render( true, PREMULT );
		gTexture = gl::Texture( grendered );
	}
	else
	{
		gColor=ColorA(0.57,0.48,1);
		TextLayout glayout;
		glayout.setFont( Font( differentFont, 72 ) );
		glayout.setColor( Color( 1, 1, 1 ) );
		glayout.addCenteredLine( "" );
		Surface8u grendered = glayout.render( true, PREMULT );
		gTexture = gl::Texture( grendered );
	}
	if(mMouseLoc > 492 && mMouseLoc < 508)
	{
		hColor = ColorA(1,0.5,0);
		TextLayout hlayout;
		hlayout.setFont( Font( differentFont, 72 ) );
		hlayout.setColor( Color( 1, 1, 1 ) );
		hlayout.addCenteredLine( "B" );
		Surface8u hrendered = hlayout.render( true, PREMULT );
		hTexture = gl::Texture( hrendered );
	}
	else
	{
		hColor=ColorA(0.45,0.39,1);
		TextLayout hlayout;
		hlayout.setFont( Font( differentFont, 72 ) );
		hlayout.setColor( Color( 1, 1, 1 ) );
		hlayout.addCenteredLine( "" );
		Surface8u hrendered = hlayout.render( true, PREMULT );
		hTexture = gl::Texture( hrendered );
	}
	if(mMouseLoc > 526 && mMouseLoc < 542)
	{
		iColor = ColorA(0.65,0.09,1);
		TextLayout ilayout;
		ilayout.setFont( Font( differentFont, 72 ) );
		ilayout.setColor( Color( 1, 1, 1 ) );
		ilayout.addCenteredLine( "C" );
		Surface8u irendered = ilayout.render( true, PREMULT );
		iTexture = gl::Texture( irendered );
	}
	else
	{
		iColor=ColorA(0.36,0.31,1);
		TextLayout ilayout;
		ilayout.setFont( Font( differentFont, 72 ) );
		ilayout.setColor( Color( 1, 1, 1 ) );
		ilayout.addCenteredLine( "" );
		Surface8u irendered = ilayout.render( true, PREMULT );
		iTexture = gl::Texture( irendered );
	}
	if(mMouseLoc > 601 && mMouseLoc < 617)
	{
		jColor = ColorA(0,0,0);
		TextLayout jlayout;
		jlayout.setFont( Font( differentFont, 72 ) );
		jlayout.setColor( Color( 1, 1, 1 ) );
		jlayout.addCenteredLine( "D" );
		Surface8u jrendered = jlayout.render( true, PREMULT );
		jTexture = gl::Texture( jrendered );
	}
	else
	{
		jColor=ColorA(0.26,0.20,1);
		TextLayout jlayout;
		jlayout.setFont( Font( differentFont, 72 ) );
		jlayout.setColor( Color( 1, 1, 1 ) );
		jlayout.addCenteredLine( "" );
		Surface8u jrendered = jlayout.render( true, PREMULT );
		jTexture = gl::Texture( jrendered );
	}
}

void RandomDistributionApp::draw()
{
  
		// this pair of lines is the standard way to clear the screen in OpenGL
	glClearColor( 0.1f, 0.1f, 0.1f, 1.0f );
	glClear( GL_COLOR_BUFFER_BIT );
	gl::setMatricesWindow( getWindowSize() );

	gl::enableAlphaBlending( PREMULT );


	Rectf bar1 = Rectf( 0, 0, app::getWindowWidth(), app::getWindowHeight()/10 );
		gl::color(aColor);
        gl::drawSolidRect(bar1);
        gl::color(0,0,0);
        gl::drawStrokedRect(bar1);

		Rectf bar = Rectf( 0, app::getWindowHeight()/10, app::getWindowWidth(), app::getWindowHeight()*0.2 );
		gl::color(bColor);
        gl::drawSolidRect(bar);
        gl::color(0,0,0);
        gl::drawStrokedRect(bar);

		Rectf bar2 = Rectf( 0, app::getWindowHeight()*0.2, app::getWindowWidth(), app::getWindowHeight()*0.3 );
		gl::color(cColor);
        gl::drawSolidRect(bar2);
        gl::color(0,0,0);
        gl::drawStrokedRect(bar2);

		Rectf bar3 = Rectf( 0, app::getWindowHeight()*0.3, app::getWindowWidth(), app::getWindowHeight()*0.4 );
		gl::color(dColor);
        gl::drawSolidRect(bar3);
        gl::color(0,0,0);
        gl::drawStrokedRect(bar3);

		Rectf bar4 = Rectf( 0, app::getWindowHeight()*0.4, app::getWindowWidth(), app::getWindowHeight()*0.5 );
		gl::color(eColor);
        gl::drawSolidRect(bar4);
        gl::color(0,0,0);
        gl::drawStrokedRect(bar4);

		Rectf bar5 = Rectf( 0, app::getWindowHeight()*0.5, app::getWindowWidth(), app::getWindowHeight()*0.6 );
		gl::color(fColor);
        gl::drawSolidRect(bar5);
        gl::color(0,0,0);
        gl::drawStrokedRect(bar5);

		Rectf bar6 = Rectf( 0, app::getWindowHeight()*0.6, app::getWindowWidth(), app::getWindowHeight()*0.7 );
		gl::color(gColor);
        gl::drawSolidRect(bar6);
        gl::color(0,0,0);
        gl::drawStrokedRect(bar6);

		Rectf bar7 = Rectf( 0, app::getWindowHeight()*0.7, app::getWindowWidth(), app::getWindowHeight()*0.8 );
		gl::color(hColor);
        gl::drawSolidRect(bar7);
        gl::color(0,0,0);
        gl::drawStrokedRect(bar7);

		Rectf bar8 = Rectf( 0, app::getWindowHeight()*0.8, app::getWindowWidth(), app::getWindowHeight()*0.9 );
		gl::color(iColor);
        gl::drawSolidRect(bar8);
        gl::color(0,0,0);
        gl::drawStrokedRect(bar8);

		Rectf bar9 = Rectf( 0, app::getWindowHeight()*0.9, app::getWindowWidth(), app::getWindowHeight() );
		gl::color(jColor);
        gl::drawSolidRect(bar9);
        gl::color(0,0,0);
        gl::drawStrokedRect(bar9);

	gl::color( Color::white() );
	gl::draw( aTexture, Vec2f( 0,app::getWindowHeight()*0.1 - 72) );

	gl::color( Color::white() );
	gl::draw( bTexture, Vec2f( 0, app::getWindowHeight()*0.2 - 72) );

	gl::color( Color::white() );
	gl::draw( cTexture, Vec2f( 0, app::getWindowHeight()*0.3 - 72) );

	gl::color( Color::white() );
	gl::draw( dTexture, Vec2f( 0, app::getWindowHeight()*0.4 - 72) );

	gl::color( Color::white() );
	gl::draw( eTexture, Vec2f( 0, app::getWindowHeight()*0.5 - 72) );

	gl::color( Color::white() );
	gl::draw( fTexture, Vec2f( 0, app::getWindowHeight()*0.6 - 72) );

	gl::color( Color::white() );
	gl::draw( gTexture, Vec2f( 0, app::getWindowHeight()*0.7 - 72) );

	gl::color( Color::white() );
	gl::draw( hTexture, Vec2f( 0, app::getWindowHeight()*0.8 - 72) );

	gl::color( Color::white() );
	gl::draw( iTexture, Vec2f( 0, app::getWindowHeight()* 0.9  - 72) );

	gl::color( Color::white() );
	gl::draw( jTexture, Vec2f( 0, app::getWindowHeight() - 72 ) );

}

CINDER_APP_NATIVE( RandomDistributionApp, RendererGl )