#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"
#include "cinder/ImageIo.h"
#include "cinder/gl/Texture.h"
#include "Resources.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class ThereminViewApp : public AppNative {
  public:
	void setup();
	void mouseDown( MouseEvent event );	
	void update();
	void draw();
	gl::Texture myImage;
};

void ThereminViewApp::setup()
{
	myImage = gl::Texture( loadImage( loadResource( RES_SPLASHSCREEN_ID ) ) );
}

void ThereminViewApp::mouseDown( MouseEvent event )
{
}

void ThereminViewApp::update()
{
}

void ThereminViewApp::draw()
{
	// clear out the window with black
	gl::clear( Color( 1, 0, 0 ), true );
	gl::draw( myImage, getWindowBounds() );
}

CINDER_APP_NATIVE( ThereminViewApp, RendererGl )
