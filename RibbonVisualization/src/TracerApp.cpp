 /*
* 
* Copyright (c) 2013, Ban the Rewind
* All rights reserved.
* 
* Redistribution and use in source and binary forms, with or 
* without modification, are permitted provided that the following 
* conditions are met:
* 
* Redistributions of source code must retain the above copyright 
* notice, this list of conditions and the following disclaimer.
* Redistributions in binary form must reproduce the above copyright 
* notice, this list of conditions and the following disclaimer in 
* the documentation and/or other materials provided with the 
* distribution.
* 
* Neither the name of the Ban the Rewind nor the names of its 
* contributors may be used to endorse or promote products 
* derived from this software without specific prior written 
* permission.
* 
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS 
* "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT 
* LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS 
* FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE 
* COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, 
* INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
* BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; 
* LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER 
* CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, 
* STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
* ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF 
* ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
* 
*/

#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"
#include "cinder/Camera.h"
#include "cinder/gl/Fbo.h"
#include "cinder/gl/GlslProg.h"
#include "cinder/params/Params.h"
#include "Cinder-LeapMotion.h"
#include "Ribbon.h"

#include "cinder/ImageIO.h"
#include "cinder/gl/Texture.h"
#include "cinder/Perlin.h"
#include "cinder/Channel.h"
#include "cinder/Vector.h"
#include "cinder/Utilities.h"
#include "ParticleController.h"
#include "Resources.h"

#include "portaudio.h"

#define RESOLUTION 10
#define NUM_PARTICLES_TO_SPAWN 25

using namespace ci;
using namespace ci::app;

#define REC_SECONDS (10)
#define SAMPLE_RATE   (44100)
#define FRAMES_PER_BUFFER  (64)

#ifndef M_PI
#define M_PI  (3.14159265)
#endif

#define TABLE_SIZE   (50000)

int shift = 1;
bool playback = false;
bool record = false;
int sample_index = 0;
int recorded_samples = 0;
int startingSampleIndex = 0;
unsigned int recordStartMs = 0;
unsigned int playbackStartMs = 0;

class Sine
{
public:
   int left_phase;
   int right_phase;
    Sine() : stream(0), left_phase(0), right_phase(0)
    {
       left_phase = right_phase = 0;
        /* initialise sinusoidal wavetable */
        for( int i=0; i<TABLE_SIZE; i++ )
        {
            sine[i] = (float) sin( ((double)i/(double)TABLE_SIZE) * M_PI * 2. );
        }

        sprintf( message, "No Message" );
    }

    void endRecording()
   {
      recorded_samples = sample_index-startingSampleIndex;
      record = false;
      playback = true;
      playbackStartMs = GetTickCount();
   }

    bool open(PaDeviceIndex index)
    {
        PaStreamParameters outputParameters;

        outputParameters.device = index;
        if (outputParameters.device == paNoDevice) {
            return false;
        }

        outputParameters.channelCount = 2;       /* stereo output */
        outputParameters.sampleFormat = paFloat32; /* 32 bit floating point output */
        outputParameters.suggestedLatency = Pa_GetDeviceInfo( outputParameters.device )->defaultLowOutputLatency;
        outputParameters.hostApiSpecificStreamInfo = NULL;

        PaError err = Pa_OpenStream(
            &stream,
            NULL, /* no input */
            &outputParameters,
            SAMPLE_RATE,
            FRAMES_PER_BUFFER,
            paClipOff,      /* we won't output out of range samples so don't bother clipping them */
            &Sine::paCallback,
            this            /* Using 'this' for userData so we can cast to Sine* in paCallback method */
            );

        if (err != paNoError)
        {
            /* Failed to open stream to device !!! */
            return false;
        }

        err = Pa_SetStreamFinishedCallback( stream, &Sine::paStreamFinished );

        if (err != paNoError)
        {
            Pa_CloseStream( stream );
            stream = 0;

            return false;
        }

        return true;
    }

    bool close()
    {
      if (stream == 0)
         return false;

      PaError err = Pa_CloseStream( stream );
      stream = 0;

      return (err == paNoError);
    }


    bool start()
    {
        if (stream == 0)
            return false;

		sample_index = 0;

		shift_data = (int*)malloc(REC_SECONDS*SAMPLE_RATE*sizeof(int));

        PaError err = Pa_StartStream( stream );

        return (err == paNoError);
    }

    bool stop()
    {
        if (stream == 0)
            return false;

        PaError err = Pa_StopStream( stream );

        return (err == paNoError);
    }


private:
	FILE* record_file;
	int * shift_data;

    /* The instance callback, where we have access to every method/variable in object of class Sine */
    int paCallbackMethod(const void *inputBuffer, void *outputBuffer,
        unsigned long framesPerBuffer,
        const PaStreamCallbackTimeInfo* timeInfo,
        PaStreamCallbackFlags statusFlags)
    {
        float *out = (float*)outputBuffer;
        unsigned long i;
		  int* shift_data_ptr;

        (void) timeInfo; /* Prevent unused variable warnings. */
        (void) statusFlags;
        (void) inputBuffer;

        if ((GetTickCount() > recordStartMs + (REC_SECONDS * 1000)) && record)
        {
           endRecording();
        }



		if (record)
		{
			shift_data_ptr = &shift_data[(sample_index-startingSampleIndex)/framesPerBuffer];
			*shift_data_ptr = shift; 
		}
		else if (playback)
		{
         shift_data_ptr = &shift_data[((sample_index-startingSampleIndex)-recorded_samples)/framesPerBuffer];
		}
        for( i=0; i<framesPerBuffer; i++ )
        {
            *out++ = sine[left_phase];  /* left */
			//*record_data_ptr++ = sine[left_phase];
            
			//*record_data_ptr++ = sine[right_phase];
            left_phase += shift;
            if( left_phase >= TABLE_SIZE ) left_phase -= TABLE_SIZE;

            if ((GetTickCount() > playbackStartMs +(REC_SECONDS-2)*1000) && playback)
            {
               playback = false;
            }

			if (playback)
			{
				right_phase += *shift_data_ptr; /* higher pitch so we can distinguish left and right. */
				if( right_phase >= TABLE_SIZE ) right_phase -= TABLE_SIZE;
				*out++ = sine[right_phase];  /* right */
			}
			else
			{
            right_phase += shift;
            if (right_phase >= TABLE_SIZE) right_phase -= TABLE_SIZE;
				*out++ = sine[right_phase];
			}
        }

		sample_index += framesPerBuffer;
        return paContinue;

    }

    /* This routine will be called by the PortAudio engine when audio is needed.
    ** It may called at interrupt level on some machines so don't do anything
    ** that could mess up the system like calling malloc() or free().
    */
    static int paCallback( const void *inputBuffer, void *outputBuffer,
        unsigned long framesPerBuffer,
        const PaStreamCallbackTimeInfo* timeInfo,
        PaStreamCallbackFlags statusFlags,
        void *userData )
    {
        /* Here we cast userData to Sine* type so we can call the instance method paCallbackMethod, we can do that since 
           we called Pa_OpenStream with 'this' for userData */
        return ((Sine*)userData)->paCallbackMethod(inputBuffer, outputBuffer,
            framesPerBuffer,
            timeInfo,
            statusFlags);
    }


    void paStreamFinishedMethod()
    {
        printf( "Stream Completed: %s\n", message );
    }

    /*
     * This routine is called by portaudio when playback is done.
     */
    static void paStreamFinished(void* userData)
    {
        return ((Sine*)userData)->paStreamFinishedMethod();
    }

    PaStream *stream;
    float sine[TABLE_SIZE];
    char message[20];
};

class TracerApp : public ci::app::AppNative
{
public:
	void					draw();
	void					prepareSettings( ci::app::AppNative::Settings* settings );
	void					setup();
	void					update();
private:
	RibbonMap				mRibbons;

	// Leap
	Leap::Frame				mFrame;
	LeapMotion::DeviceRef	mDevice;
	void 					onFrame( Leap::Frame frame );

	// Trails
	ci::gl::Fbo				mFbo[ 3 ];
	ci::gl::GlslProg		mShader[ 2 ];

	// Camera
	ci::CameraPersp			mCamera;

	// Params
	float					mFrameRate;
	ci::params::InterfaceGl	mParams;

	// Save screen shot
	void					screenShot();

	//Theremin texture
	gl::Texture myImage;
};

#include "cinder/ImageIo.h"
#include "cinder/Rand.h"
#include "cinder/Utilities.h"
#include "Resources.h"

// Imports
using namespace ci;
using namespace ci::app;
using namespace LeapMotion;
using namespace std;

// Render
void TracerApp::draw()
{
	//Draw Theremin Image
	// clear out the window with black
	gl::clear( Color( 0, 0, 0 ), true );
	gl::draw( myImage, getWindowBounds() );
   gl::popMatrices();

	// Add to accumulation buffer
	mFbo[ 0 ].bindFramebuffer();
	gl::setViewport( mFbo[ 0 ].getBounds() );
	gl::setMatricesWindow( mFbo[ 0 ].getSize() );
	gl::enableAlphaBlending();

	// Dim last frame
	gl::color( ColorAf( Colorf::black(), 0.03f ) );
	gl::drawSolidRect( Rectf( mFbo[ 0 ].getBounds() ) );

	// Draw finger tips into the accumulation buffer
	gl::setMatrices( mCamera );
	gl::enableAdditiveBlending();
	for ( RibbonMap::const_iterator iter = mRibbons.begin(); iter != mRibbons.end(); ++iter ) {
		iter->second.draw();
	}
	mFbo[ 0 ].unbindFramebuffer();

	// Blur the accumulation buffer
	gl::enable( GL_TEXTURE_2D );
	gl::enableAlphaBlending();
	gl::color( ColorAf::white() );
	Vec2f pixel	= Vec2f::one() / Vec2f( mFbo[ 0 ].getSize() );
	pixel		*= 3.0f;
	for ( size_t i = 0; i < 2; ++i ) {
		mFbo[ i + 1 ].bindFramebuffer();
		gl::clear();
		
		mShader[ i ].bind();
		mShader[ i ].uniform( "size",	pixel );
		mShader[ i ].uniform( "tex",	0 );
				
		gl::Texture& texture = mFbo[ i ].getTexture();
		texture.bind();
		gl::drawSolidRect( Rectf( mFbo[ i ].getBounds() ) );
		texture.unbind();
		
		mShader[ i ].unbind();
		mFbo[ i + 1 ].unbindFramebuffer();
	}

	// Draw blurred image
	gl::setMatricesWindow( getWindowSize(), false );
	gl::color( ColorAf::white() );
	mFbo[ 0 ].bindTexture();
	gl::drawSolidRect( Rectf( getWindowBounds() ) );
	mFbo[ 0 ].unbindTexture();
	
	gl::color( ColorAf( Colorf::white(), 0.8f ) );
	mFbo[ 2 ].bindTexture();
	gl::drawSolidRect( Rectf( getWindowBounds() ) );
	mFbo[ 2 ].unbindTexture();
	gl::disableAlphaBlending();
	gl::disable( GL_TEXTURE_2D );

	// Draw the interface
	//mParams.draw();
}

// Called when Leap frame data is ready
void TracerApp::onFrame( Leap::Frame frame )
{
	mFrame = frame;

   // Get gestures
   const Leap::GestureList gestures = frame.gestures();
   for (int g = 0; g < gestures.count(); ++g) 
   {
      Leap::Gesture gesture = gestures[g];

      switch (gesture.type()) 
      {
         case Leap::Gesture::TYPE_SWIPE:
         {
           if (!record && !playback)
           {
              //beginRecording();
           }
         }
         break;
         default:
            break;
       }
   }
}

// Prepare window
void TracerApp::prepareSettings( Settings* settings )
{
	settings->setFrameRate( 60.0f );
	settings->setResizable( false );
	settings->setWindowSize( 1024, 768 );
   //settings->setFullScreen();
}

// Take screen shot
void TracerApp::screenShot()
{
#if defined( CINDER_MSW )
	fs::path path = getAppPath();
#else
	fs::path path = getAppPath().parent_path();
#endif
	writeImage( path / fs::path( "frame" + toString( getElapsedFrames() ) + ".png" ), copyWindowSurface() );
}

// Set up
void TracerApp::setup()
{
	//Setup Texture for drawing Theremin Image
	myImage = gl::Texture( loadImage( loadResource( RES_SPLASHSCREEN_ID ) ) );

	// Set up camera
	mCamera		= CameraPersp( getWindowWidth(), getWindowHeight(), 60.0f, 0.01f, 1000.0f );
	mCamera.lookAt( Vec3f( 0.0f, 93.75f, 250.0f ), Vec3f( 0.0f, 250.0f, 0.0f ) );
	
	// Start device
	mDevice = Device::create();
	mDevice->connectEventHandler( &TracerApp::onFrame, this );

   mDevice->getController()->enableGesture(Leap::Gesture::Type::TYPE_SWIPE);

	// Load shaders
	try {
		mShader[ 0 ]	= gl::GlslProg( loadResource( RES_GLSL_PASS_THROUGH_VERT ), loadResource( RES_GLSL_BLUR_X_FRAG ) );
	} catch ( gl::GlslProgCompileExc ex ) {
		console() << "Unable to compile blur X shader: \n" << string( ex.what() ) << "\n";
		quit();
	}
	try {
		mShader[ 1 ]	= gl::GlslProg( loadResource( RES_GLSL_PASS_THROUGH_VERT ), loadResource( RES_GLSL_BLUR_Y_FRAG ) );
	} catch ( gl::GlslProgCompileExc ex ) {
		console() << "Unable to compile blur Y shader: \n" << string( ex.what() ) << "\n";
		quit();
	}

	// Params
	mFrameRate	= 0.0f;
	mParams = params::InterfaceGl( "Params", Vec2i( 200, 105 ) );
	mParams.addParam( "Frame rate",		&mFrameRate,							"", true );
	mParams.addButton( "Screen shot",	bind( &TracerApp::screenShot, this ),	"key=space" );
	mParams.addButton( "Quit",			bind( &TracerApp::quit, this ),			"key=q" );

	// Enable polygon smoothing
	gl::enable( GL_POLYGON_SMOOTH );
	glHint( GL_POLYGON_SMOOTH_HINT, GL_NICEST );
	
	// Set up FBOs
	gl::Fbo::Format format;
#if defined( CINDER_MSW )
	format.setColorInternalFormat( GL_RGBA32F );
#else
	format.setColorInternalFormat( GL_RGBA32F_ARB );
#endif
	format.setMinFilter( GL_LINEAR );
	format.setMagFilter( GL_LINEAR );
	format.setWrap( GL_CLAMP, GL_CLAMP );
	for ( size_t i = 0; i < 3; ++i ) {
		mFbo[ i ]	= gl::Fbo( getWindowWidth(), getWindowHeight(), format );
		mFbo[ i ].bindFramebuffer();
		gl::setViewport( mFbo[ i ].getBounds() );
		gl::clear();
		mFbo[ i ].unbindFramebuffer();
	}
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );

   // Audio stuff
   Sine sine;
   PaError err;

   sine.left_phase = sine.right_phase = 0;

   err = Pa_Initialize();
   //if( err != paNoError ) goto error;

	playback = false;

    if (sine.open(Pa_GetDefaultOutputDevice()))
    {
        console() << "sine started" << std::endl;
        sine.start();
    }
    /*

error:
    Pa_Terminate();
    fprintf( stderr, "An error occured while using the portaudio stream\n" );
    fprintf( stderr, "Error number: %d\n", err );
    fprintf( stderr, "Error message: %s\n", Pa_GetErrorText( err ) );*/
}

// Runs update logic
void TracerApp::update()
{
	// Update frame rate
	mFrameRate = getAverageFps();

	// Process hand data
	const Leap::HandList& hands = mFrame.hands();
	for ( Leap::HandList::const_iterator handIter = hands.begin(); handIter != hands.end(); ++handIter ) {
		const Leap::Hand& hand = *handIter;

      shift = (hand.palmPosition().y)+35;
		
		const Leap::PointableList& pointables = hand.pointables();
		for ( Leap::PointableList::const_iterator pointIter = pointables.begin(); pointIter != pointables.end(); ++pointIter ) {
			const Leap::Pointable& pointable = *pointIter;

			int32_t id = pointable.id();
			if ( mRibbons.find( id ) == mRibbons.end() ) {
				Vec3f v = randVec3f() * 0.01f;
				v.x = math<float>::abs( v.x );
				v.y = math<float>::abs( v.y );
				v.z = math<float>::abs( v.z );
				Colorf color( ColorModel::CM_RGB, v );
				Ribbon ribbon( id, color );
				mRibbons[ id ] = ribbon;
			}
			float width = math<float>::abs( pointable.tipVelocity().y ) * 0.0025f;
			width		= math<float>::max( width, 5.0f );
			mRibbons[ id ].addPoint( LeapMotion::toVec3f( pointable.tipPosition() ), width );
		}
	}

	// Update ribbons
	for ( RibbonMap::iterator iter = mRibbons.begin(); iter != mRibbons.end(); ++iter ) {
		iter->second.update();
	}
}

// Run application
CINDER_APP_NATIVE( TracerApp, RendererGl )

