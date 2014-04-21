// RandomDistributionApp.cpp file
#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"
#include "cinder/Rand.h"

using namespace ci;
using namespace ci::app;
using namespace std;


class RandomDistributionApp : public AppNative {
  public:
  void setup();
  void update();
  void draw();
  void prepareSettings( Settings *settings );
  void mouseMove(MouseEvent event );
  int mMouseLoc;
    vector<int> v;
	vector<int> turnOn;
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

void RandomDistributionApp::setup()
{
    for(int i=0; i<20; i++) {
        v.push_back(0);
    }

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
}

void RandomDistributionApp::mouseMove( MouseEvent event ) 
{
    mMouseLoc = event.getY();
	//console() << mMouseLoc << std::endl;
}

void RandomDistributionApp::update()
{
    //int index = randInt(v.size());
    //v[index]+=1;
	
	if(mMouseLoc > 213 && mMouseLoc < 229)
	{
		//console() << "Special thing happened!" << std::endl;
		aColor = ColorA(0.16,1,0.05);
	}
	else
	{
		aColor=ColorA(0.98,0.97,1);
	}
	if(mMouseLoc > 230 && mMouseLoc < 246)
	{
		bColor = ColorA(1,0.32,.05);
	}
	else
	{
		bColor=ColorA(0.90,0.89,1);
	}
	if(mMouseLoc > 268 && mMouseLoc < 282)
	{
		cColor = ColorA(0.21,0.07,1);
	}
	else
	{
		cColor=ColorA(0.83,0.8,1);
	}
	if(mMouseLoc > 307 && mMouseLoc < 323)
	{
		dColor = ColorA(0.9,0.1,1);
	}
	else
	{
		dColor=ColorA(0.78,0.75,1);
	}
	if(mMouseLoc > 329 && mMouseLoc < 345)
	{
		eColor = ColorA(1,0.83,0.05);
	}
	else
	{
		eColor=ColorA(0.71,0.66,1);
	}
	if(mMouseLoc > 377 && mMouseLoc < 393)
	{
		fColor = ColorA(0.25,1,0.5);
	}
	else
	{
		fColor=ColorA(0.65,0.61,1);
	}
	if(mMouseLoc > 432 && mMouseLoc < 448)
	{
		gColor = ColorA(1,0.09,0.29);
	}
	else
	{
		gColor=ColorA(0.57,0.48,1);
	}
	if(mMouseLoc > 492 && mMouseLoc < 508)
	{
		hColor = ColorA(1,0.5,0);
	}
	else
	{
		hColor=ColorA(0.45,0.39,1);
	}
	if(mMouseLoc > 526 && mMouseLoc < 542)
	{
		iColor = ColorA(0.65,0.09,1);
	}
	else
	{
		iColor=ColorA(0.36,0.31,1);
	}
	if(mMouseLoc > 601 && mMouseLoc < 617)
	{
		jColor = ColorA(0,0,0);
	}
	else
	{
		jColor=ColorA(0.26,0.20,1);
	}
}

void RandomDistributionApp::draw()
{
  
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


}

CINDER_APP_NATIVE( RandomDistributionApp, RendererGl )