// NoteVisualization.cpp file
#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"
#include "cinder/Rand.h"
#include "cinder/Camera.h"
#include "Cinder-LeapMotion.h"
#include "cinder/Text.h"
#include "cinder/Utilities.h"
#include "cinder/ImageIo.h"
#include "cinder/Font.h"
#include "cinder/gl/Texture.h"

#include <Windows.h>
#include "portaudio.h"

#define REC_SECONDS (10)
#define SAMPLE_RATE   (44100)
#define FRAMES_PER_BUFFER  (4)

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


using namespace ci;
using namespace ci::app;
using namespace std;
using namespace LeapMotion;

static const bool PREMULT = false;

void beginRecording()
{
   record = true;
   playback = false;
   recordStartMs = GetTickCount();
   startingSampleIndex = sample_index;
}

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



class NoteVisualization : public AppNative {
  public:
     void setup();
     void update();
     void draw();
     void prepareSettings( Settings *settings );
     void mouseMove(MouseEvent event );
     int mMouseLoc;
       vector<int> v;
	   vector<int> turnOn;
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

      gl::Texture	mTexture, mSimpleTexture, aTexture, bTexture, cTexture, dTexture, eTexture, fTexture, gTexture, hTexture, iTexture, jTexture;

   private:
      // Leap
	   Leap::Frame				mFrame;
	   LeapMotion::DeviceRef	mDevice;
	   void 					onFrame( Leap::Frame frame );
      ci::CameraPersp			mCamera;

      int drawHands;

};

void NoteVisualization::prepareSettings( Settings *settings ) {
    settings->setWindowSize(800,600);
    //settings->setFullScreen();
    settings->setFrameRate(60.f);
}

// Called when Leap frame data is ready
void NoteVisualization::onFrame( Leap::Frame frame )
{
	mFrame = frame;

   // Get gestures
   const Leap::GestureList gestures = frame.gestures();
   for (int g = 0; g < gestures.count(); ++g) 
   {
      Leap::Gesture gesture = gestures[g];

      switch (gesture.type()) 
      {
         case Leap::Gesture::TYPE_CIRCLE:
         {
            drawHands = 1-drawHands;
         }
         break;
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

void NoteVisualization::setup()
{
   #if defined( CINDER_COCOA_TOUCH )
	std::string normalFont( "Arial" );
	std::string boldFont( "Arial-BoldMT" );
	std::string differentFont( "AmericanTypewriter" );
#else
	std::string normalFont( "Arial" );
	std::string boldFont( "Arial Bold" );
	std::string differentFont( "Papyrus" );
#endif
   
   drawHands = 0;

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

   // Set up camera
	mCamera = CameraPersp( getWindowWidth(), getWindowHeight(), 60.0f, 0.01f, 1000.0f );
	mCamera.lookAt( Vec3f( 0.0f, 125.0f, 500.0f ), Vec3f( 0.0f, 250.0f, 0.0f ) );

   // Start leap device
	mDevice 		= Device::create();
	mDevice->connectEventHandler( &NoteVisualization::onFrame, this );

   mDevice->getController()->enableGesture(Leap::Gesture::Type::TYPE_CIRCLE);
   mDevice->getController()->enableGesture(Leap::Gesture::Type::TYPE_SWIPE);

   TextLayout alayout;
	alayout.setFont( Font( differentFont, 72 ) );
	alayout.setColor( Color( 1, 1, 1 ) );
	alayout.addCenteredLine( "B" );

	Surface8u arendered = alayout.render( true, PREMULT );
	aTexture = gl::Texture( arendered );

	TextLayout blayout;
	blayout.setFont( Font( differentFont, 72 ) );
	blayout.setColor( Color( 1, 1, 1 ) );
	blayout.addCenteredLine( "C" );

	Surface8u brendered = blayout.render( true, PREMULT );
	bTexture = gl::Texture( brendered );

	TextLayout clayout;
	clayout.setFont( Font( differentFont, 72 ) );
	clayout.setColor( Color( 1, 1, 1 ) );
	clayout.addCenteredLine( "D" );

	Surface8u crendered = clayout.render( true, PREMULT );
	cTexture = gl::Texture( crendered );

	TextLayout dlayout;
	dlayout.setFont( Font( differentFont, 72 ) );
	dlayout.setColor( Color( 1, 1, 1 ) );
	dlayout.addCenteredLine( "E" );

	Surface8u drendered = dlayout.render( true, PREMULT );
	dTexture = gl::Texture( drendered );

	TextLayout elayout;
	elayout.setFont( Font( differentFont, 72 ) );
	elayout.setColor( Color( 1, 1, 1 ) );
	elayout.addCenteredLine( "F" );

	Surface8u erendered = elayout.render( true, PREMULT );
	eTexture = gl::Texture( erendered );


	TextLayout flayout;
	flayout.setFont( Font( differentFont, 72 ) );
	flayout.setColor( Color( 1, 1, 1 ) );
	flayout.addCenteredLine( "G" );

	Surface8u frendered = flayout.render( true, PREMULT );
	fTexture = gl::Texture( frendered );


	TextLayout glayout;
	glayout.setFont( Font( differentFont, 72 ) );
	glayout.setColor( Color( 1, 1, 1 ) );
	glayout.addCenteredLine( "A" );

	Surface8u grendered = glayout.render( true, PREMULT );
	gTexture = gl::Texture( grendered );

	TextLayout hlayout;
	hlayout.setFont( Font( differentFont, 72 ) );
	hlayout.setColor( Color( 1, 1, 1 ) );
	hlayout.addCenteredLine( "B" );

	Surface8u hrendered = hlayout.render( true, PREMULT );
	hTexture = gl::Texture( hrendered );

	TextLayout ilayout;
	ilayout.setFont( Font( differentFont, 72 ) );
	ilayout.setColor( Color( 1, 1, 1 ) );
	ilayout.addCenteredLine( "C" );

	Surface8u irendered = ilayout.render( true, PREMULT );
	iTexture = gl::Texture( irendered );

	TextLayout jlayout;
	jlayout.setFont( Font( differentFont, 72 ) );
	jlayout.setColor( Color( 1, 1, 1 ) );
	jlayout.addCenteredLine( "D" );

	Surface8u jrendered = jlayout.render( true, PREMULT );
	jTexture = gl::Texture( jrendered );


   // Audio stuff
   Sine sine;
   PaError err;

   sine.left_phase = sine.right_phase = 0;

   err = Pa_Initialize();
   if( err != paNoError ) goto error;

	playback = false;

    if (sine.open(Pa_GetDefaultOutputDevice()))
    {
        sine.start();
    }

error:
    Pa_Terminate();
    fprintf( stderr, "An error occured while using the portaudio stream\n" );
    fprintf( stderr, "Error number: %d\n", err );
    fprintf( stderr, "Error message: %s\n", Pa_GetErrorText( err ) );
}

void NoteVisualization::mouseMove( MouseEvent event ) 
{
    //mMouseLoc = event.getY();
	//console() << mMouseLoc << std::endl;
}

void NoteVisualization::update()
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
      //mMouseLoc = 630 - hand.palmPosition().y;
      shift = (hand.palmPosition().y)+35;
      mMouseLoc = 880-shift;

      console() << shift << std::endl;
   }
	
	#if defined( CINDER_COCOA_TOUCH )
	std::string normalFont( "Arial" );
	std::string boldFont( "Arial-BoldMT" );
	std::string differentFont( "AmericanTypewriter" );
#else
	std::string normalFont( "Arial" );
	std::string boldFont( "Arial Bold" );
	std::string differentFont( "Arial-BoldMT" );
#endif

   Color fontColor(1,1,1);

   if (record)
      fontColor = Color(1, 0, 0);

		if(shift > 279 && mMouseLoc < 289)
	{
		//console() << "Special thing happened!" << std::endl;
		aColor = ColorA(0.16,1,0.05);
		TextLayout alayout;
		alayout.setFont( Font( differentFont, 72 ) );
		alayout.setColor( fontColor );
		alayout.addCenteredLine( "B" );
		Surface8u arendered = alayout.render( true, PREMULT );
		aTexture = gl::Texture( arendered );
	}
	else
	{
		aColor=ColorA(0.98,0.97,1);
		TextLayout alayout;
		alayout.setFont( Font( differentFont, 72 ) );
		alayout.setColor( fontColor );
		alayout.addCenteredLine( "" );
		Surface8u arendered = alayout.render( true, PREMULT );
		aTexture = gl::Texture( arendered );
	}
		if(shift > 290 && shift < 299)
	{
		bColor = ColorA(1,0.32,.05);
		TextLayout blayout;
		blayout.setFont( Font( differentFont, 72 ) );
		blayout.setColor( fontColor );
		blayout.addCenteredLine( "C" );
		Surface8u brendered = blayout.render( true, PREMULT );
		bTexture = gl::Texture( brendered );

	}
	else
	{
		bColor=ColorA(0.90,0.89,1);
		TextLayout blayout;
		blayout.setFont( Font( differentFont, 72 ) );
		blayout.setColor( fontColor );
		blayout.addCenteredLine( "" );
		Surface8u brendered = blayout.render( true, PREMULT );
		bTexture = gl::Texture( brendered );
	}
	if(shift > 329 && shift < 341)
	{
		cColor = ColorA(0.21,0.07,1);
		TextLayout clayout;
		clayout.setFont( Font( differentFont, 72 ) );
		clayout.setColor( fontColor );
		clayout.addCenteredLine( "D" );
		Surface8u crendered = clayout.render( true, PREMULT );
		cTexture = gl::Texture( crendered );

	}
	else
	{
		cColor=ColorA(0.83,0.8,1);
		TextLayout clayout;
		clayout.setFont( Font( differentFont, 72 ) );
		clayout.setColor( fontColor );
		clayout.addCenteredLine( "" );
		Surface8u crendered = clayout.render( true, PREMULT );
		cTexture = gl::Texture( crendered );
	}
	if(shift > 364 && shift < 376)
	{
		dColor = ColorA(0.9,0.1,1);
		TextLayout dlayout;
		dlayout.setFont( Font( differentFont, 72 ) );
		dlayout.setColor( fontColor );
		dlayout.addCenteredLine( "E" );
		Surface8u drendered = dlayout.render( true, PREMULT );
		dTexture = gl::Texture( drendered );
	}
	else
	{
		dColor=ColorA(0.78,0.75,1);
		TextLayout dlayout;
		dlayout.setFont( Font( differentFont, 72 ) );
		dlayout.setColor( fontColor );
		dlayout.addCenteredLine( "" );
		Surface8u drendered = dlayout.render( true, PREMULT );
		dTexture = gl::Texture( drendered );

	}
	if(shift > 386 && shift < 398)
	{
		eColor = ColorA(1,0.83,0.05);
		TextLayout elayout;
		elayout.setFont( Font( differentFont, 72 ) );
		elayout.setColor( fontColor );
		elayout.addCenteredLine( "F" );
		Surface8u erendered = elayout.render( true, PREMULT );
		eTexture = gl::Texture( erendered );
	}
	else
	{
		eColor=ColorA(0.71,0.66,1);
		TextLayout elayout;
		elayout.setFont( Font( differentFont, 72 ) );
		elayout.setColor( fontColor );
		elayout.addCenteredLine( "" );
		Surface8u erendered = elayout.render( true, PREMULT );
		eTexture = gl::Texture( erendered );
	}
	if(shift > 436 && shift < 448)
	{
		fColor = ColorA(0.25,1,0.5);
		TextLayout flayout;
		flayout.setFont( Font( differentFont, 72 ) );
		flayout.setColor( fontColor );
		flayout.addCenteredLine( "G" );
		Surface8u frendered = flayout.render( true, PREMULT );
		fTexture = gl::Texture( frendered );
	}
	else
	{
		fColor=ColorA(0.65,0.61,1);
		TextLayout flayout;
		flayout.setFont( Font( differentFont, 72 ) );
		flayout.setColor( fontColor );
		flayout.addCenteredLine( "" );
		Surface8u frendered = flayout.render( true, PREMULT );
		fTexture = gl::Texture( frendered );
	}
	if(shift > 489 && shift < 501)
	{
		gColor = ColorA(1,0.09,0.29);
		TextLayout glayout;
		glayout.setFont( Font( differentFont, 72 ) );
		glayout.setColor( fontColor );
		glayout.addCenteredLine( "A" );
		Surface8u grendered = glayout.render( true, PREMULT );
		gTexture = gl::Texture( grendered );
	}
	else
	{
		gColor=ColorA(0.57,0.48,1);
		TextLayout glayout;
		glayout.setFont( Font( differentFont, 72 ) );
		glayout.setColor( fontColor );
		glayout.addCenteredLine( "" );
		Surface8u grendered = glayout.render( true, PREMULT );
		gTexture = gl::Texture( grendered );
	}
	if(shift > 556 && shift < 568)
	{
		hColor = ColorA(1,0.5,0);
		TextLayout hlayout;
		hlayout.setFont( Font( differentFont, 72 ) );
		hlayout.setColor( fontColor );
		hlayout.addCenteredLine( "B" );
		Surface8u hrendered = hlayout.render( true, PREMULT );
		hTexture = gl::Texture( hrendered );
	}
	else
	{
		hColor=ColorA(0.45,0.39,1);
		TextLayout hlayout;
		hlayout.setFont( Font( differentFont, 72 ) );
		hlayout.setColor( fontColor );
		hlayout.addCenteredLine( "" );
		Surface8u hrendered = hlayout.render( true, PREMULT );
		hTexture = gl::Texture( hrendered );
	}
	if(shift > 594 && shift < 606)
	{
		iColor = ColorA(0.65,0.09,1);
		TextLayout ilayout;
		ilayout.setFont( Font( differentFont, 72 ) );
		ilayout.setColor( fontColor );
		ilayout.addCenteredLine( "C" );
		Surface8u irendered = ilayout.render( true, PREMULT );
		iTexture = gl::Texture( irendered );
	}
	else
	{
		iColor=ColorA(0.36,0.31,1);
		TextLayout ilayout;
		ilayout.setFont( Font( differentFont, 72 ) );
		ilayout.setColor( fontColor );
		ilayout.addCenteredLine( "" );
		Surface8u irendered = ilayout.render( true, PREMULT );
		iTexture = gl::Texture( irendered );
	}
	if(shift > 679 && shift < 691)
	{
		jColor = ColorA(0,0,0);
		TextLayout jlayout;
		jlayout.setFont( Font( differentFont, 72 ) );
		jlayout.setColor( fontColor );
		jlayout.addCenteredLine( "D" );
		Surface8u jrendered = jlayout.render( true, PREMULT );
		jTexture = gl::Texture( jrendered );
	}
	else
	{
		jColor=ColorA(0.26,0.20,1);
		TextLayout jlayout;
		jlayout.setFont( Font( differentFont, 72 ) );
		jlayout.setColor( fontColor );
		jlayout.addCenteredLine( "" );
		Surface8u jrendered = jlayout.render( true, PREMULT );
		jTexture = gl::Texture( jrendered );
	}
}

void NoteVisualization::draw()
{
      // clear screen
      glClearColor( 0.1f, 0.1f, 0.1f, 1.0f );
	   glClear( GL_COLOR_BUFFER_BIT );
	   gl::setMatricesWindow( getWindowSize() );
	   gl::enableAlphaBlending( PREMULT );


		Rectf bar1 = Rectf( 0, 0, app::getWindowWidth(), app::getWindowHeight()/10 );
		gl::color(aColor);
        gl::drawSolidRect(bar9);
        gl::color(0,0,0);
        gl::drawStrokedRect(bar9);

		Rectf bar = Rectf( 0, app::getWindowHeight()/10, app::getWindowWidth(), app::getWindowHeight()*0.2 );
		gl::color(bColor);
        gl::drawSolidRect(bar8);
        gl::color(0,0,0);
        gl::drawStrokedRect(bar8);

		Rectf bar2 = Rectf( 0, app::getWindowHeight()*0.2, app::getWindowWidth(), app::getWindowHeight()*0.3 );
		gl::color(cColor);
        gl::drawSolidRect(bar7);
        gl::color(0,0,0);
        gl::drawStrokedRect(bar7);

		Rectf bar3 = Rectf( 0, app::getWindowHeight()*0.3, app::getWindowWidth(), app::getWindowHeight()*0.4 );
		gl::color(dColor);
        gl::drawSolidRect(bar6);
        gl::color(0,0,0);
        gl::drawStrokedRect(bar6);

		Rectf bar4 = Rectf( 0, app::getWindowHeight()*0.4, app::getWindowWidth(), app::getWindowHeight()*0.5 );
		gl::color(eColor);
        gl::drawSolidRect(bar5);
        gl::color(0,0,0);
        gl::drawStrokedRect(bar5);

		Rectf bar5 = Rectf( 0, app::getWindowHeight()*0.5, app::getWindowWidth(), app::getWindowHeight()*0.6 );
		gl::color(fColor);
        gl::drawSolidRect(bar4);
        gl::color(0,0,0);
        gl::drawStrokedRect(bar4);

		Rectf bar6 = Rectf( 0, app::getWindowHeight()*0.6, app::getWindowWidth(), app::getWindowHeight()*0.7 );
		gl::color(gColor);
        gl::drawSolidRect(bar3);
        gl::color(0,0,0);
        gl::drawStrokedRect(bar3);

		Rectf bar7 = Rectf( 0, app::getWindowHeight()*0.7, app::getWindowWidth(), app::getWindowHeight()*0.8 );
		gl::color(hColor);
        gl::drawSolidRect(bar2);
        gl::color(0,0,0);
        gl::drawStrokedRect(bar2);

		Rectf bar8 = Rectf( 0, app::getWindowHeight()*0.8, app::getWindowWidth(), app::getWindowHeight()*0.9 );
		gl::color(iColor);
        gl::drawSolidRect(bar);
        gl::color(0,0,0);
        gl::drawStrokedRect(bar);

		Rectf bar9 = Rectf( 0, app::getWindowHeight()*0.9, app::getWindowWidth(), app::getWindowHeight() );
		gl::color(jColor);
        gl::drawSolidRect(bar1);
        gl::color(0,0,0);
        gl::drawStrokedRect(bar1);

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

      gl::pushMatrices();


   if (drawHands == 0)
   {
      return;
   }

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

CINDER_APP_NATIVE( NoteVisualization, RendererGl )