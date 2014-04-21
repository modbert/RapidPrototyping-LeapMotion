// RandomDistributionApp.cpp file
#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"
#include "cinder/Rand.h"
#include "cinder/Camera.h"
#include "Cinder-LeapMotion.h"


using namespace ci;
using namespace ci::app;
using namespace std;
using namespace LeapMotion;


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

   private:
      // Leap
	   Leap::Frame				mFrame;
	   LeapMotion::DeviceRef	mDevice;
	   void 					onFrame( Leap::Frame frame );
      ci::CameraPersp			mCamera;

};

void RandomDistributionApp::prepareSettings( Settings *settings ) {
    settings->setWindowSize(640,480);
    settings->setFrameRate(60.f);
}

// Called when Leap frame data is ready
void RandomDistributionApp::onFrame( Leap::Frame frame )
{
	mFrame = frame;
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
	
	mMouseLoc = 0;

   // Set up camera
	mCamera = CameraPersp( getWindowWidth(), getWindowHeight(), 60.0f, 0.01f, 1000.0f );
	mCamera.lookAt( Vec3f( 0.0f, 125.0f, 500.0f ), Vec3f( 0.0f, 250.0f, 0.0f ) );

   // Start leap device
	mDevice 		= Device::create();
	mDevice->connectEventHandler( &RandomDistributionApp::onFrame, this );
}

void RandomDistributionApp::mouseMove( MouseEvent event ) 
{
    //mMouseLoc = event.getY();
	//console() << mMouseLoc << std::endl;
}

void RandomDistributionApp::update()
{
    //int index = randInt(v.size());
    //v[index]+=1;

   // Interact with first hand
	const Leap::HandList& hands = mFrame.hands();
	if ( hands.isEmpty() ) {
		// nothing
	} else {
		const Leap::Hand& hand = *hands.begin();

		// Update hand position
      mMouseLoc = hand.palmPosition().y;
   }
	
	if(mMouseLoc > 0 && mMouseLoc < 50)
	{
		//console() << "Special thing happened!" << std::endl;
		aColor = ColorA(0.16,1,0.05);
	}
	else
	{
		aColor=ColorA(0.98,0.97,1);
	}
	if(mMouseLoc > 50 && mMouseLoc < 100)
	{
		bColor = ColorA(1,0.32,.05);
	}
	else
	{
		bColor=ColorA(0.90,0.89,1);
	}
	if(mMouseLoc > 100 && mMouseLoc < 150)
	{
		cColor = ColorA(0.21,0.07,1);
	}
	else
	{
		cColor=ColorA(0.83,0.8,1);
	}
	if(mMouseLoc > 150 && mMouseLoc < 200)
	{
		dColor = ColorA(0.9,0.1,1);
	}
	else
	{
		dColor=ColorA(0.78,0.75,1);
	}
	if(mMouseLoc > 200 && mMouseLoc < 250)
	{
		eColor = ColorA(1,0.83,0.05);
	}
	else
	{
		eColor=ColorA(0.71,0.66,1);
	}
	if(mMouseLoc > 250 && mMouseLoc < 300)
	{
		fColor = ColorA(0.25,1,0.5);
	}
	else
	{
		fColor=ColorA(0.65,0.61,1);
	}
	if(mMouseLoc > 300 && mMouseLoc < 350)
	{
		gColor = ColorA(1,0.09,0.29);
	}
	else
	{
		gColor=ColorA(0.57,0.48,1);
	}
}

void RandomDistributionApp::draw()
{
      gl::setMatricesWindow( getWindowSize() );

		Rectf bar1 = Rectf( 0, 0, app::getWindowWidth(), app::getWindowHeight()/7 );
		gl::color(aColor);
        gl::drawSolidRect(bar1);
        gl::color(0,0,0);
        gl::drawStrokedRect(bar1);

		Rectf bar = Rectf( 0, app::getWindowHeight()/7, app::getWindowWidth(), app::getWindowHeight()*2/7 );
		gl::color(bColor);
        gl::drawSolidRect(bar);
        gl::color(0,0,0);
        gl::drawStrokedRect(bar);

		Rectf bar2 = Rectf( 0, app::getWindowHeight()*2/7, app::getWindowWidth(), app::getWindowHeight()*3/7 );
		gl::color(cColor);
        gl::drawSolidRect(bar2);
        gl::color(0,0,0);
        gl::drawStrokedRect(bar2);

		Rectf bar3 = Rectf( 0, app::getWindowHeight()*3/7, app::getWindowWidth(), app::getWindowHeight()*4/7 );
		gl::color(dColor);
        gl::drawSolidRect(bar3);
        gl::color(0,0,0);
        gl::drawStrokedRect(bar3);

		Rectf bar4 = Rectf( 0, app::getWindowHeight()*4/7, app::getWindowWidth(), app::getWindowHeight()*5/7 );
		gl::color(eColor);
        gl::drawSolidRect(bar4);
        gl::color(0,0,0);
        gl::drawStrokedRect(bar4);

		Rectf bar5 = Rectf( 0, app::getWindowHeight()*5/7, app::getWindowWidth(), app::getWindowHeight()*6/7 );
		gl::color(fColor);
        gl::drawSolidRect(bar5);
        gl::color(0,0,0);
        gl::drawStrokedRect(bar5);

		Rectf bar6 = Rectf( 0, app::getWindowHeight()*6/7, app::getWindowWidth(), app::getWindowHeight() );
		gl::color(gColor);
        gl::drawSolidRect(bar6);
        gl::color(0,0,0);
        gl::drawStrokedRect(bar6);

      gl::pushMatrices();

      // Clear window
	gl::setViewport( getWindowBounds() );
	//gl::clear( Colorf::black() );
	gl::setMatrices( mCamera );

   //gl::enableDepthRead();
	//gl::enableDepthWrite();
	
	// Iterate through hands
	const Leap::HandList& hands = mFrame.hands();
	for ( Leap::HandList::const_iterator handIter = hands.begin(); handIter != hands.end(); ++handIter ) {
		const Leap::Hand& hand = *handIter;

		

		// Pointables
		const Leap::PointableList& pointables = hand.pointables();
		for ( Leap::PointableList::const_iterator pointIter = pointables.begin(); pointIter != pointables.end(); ++pointIter ) {
			const Leap::Pointable& pointable = *pointIter;

			Vec3f dir		= LeapMotion::toVec3f( pointable.direction() );
			float length	= pointable.length();
			Vec3f tipPos	= LeapMotion::toVec3f( pointable.tipPosition() );
			Vec3f basePos	= tipPos + dir * length;
			

			gl::drawColorCube( tipPos, Vec3f( 20, 20, 20 ) );
			gl::color( ColorAf::gray( 0.8f ) );
			gl::drawLine( basePos, tipPos );

         // Draw palm
		gl::color(1, .2, .4, 1);
		gl::drawSphere(LeapMotion::toVec3f(hand.palmPosition()),20,12);
		}
	}

   //gl::disableDepthRead();
	//gl::disableDepthWrite();

   gl::popMatrices();

}

CINDER_APP_NATIVE( RandomDistributionApp, RendererGl )