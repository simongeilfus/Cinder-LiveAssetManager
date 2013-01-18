#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"
#include "cinder/gl/GlslProg.h"

#include "LiveAssetManager.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class GlslProgApp : public AppNative {
  public:
	void setup();
	void draw();
    
    gl::GlslProg mShader;
};

void GlslProgApp::setup()
{
    LiveAssetManager::load( "shader.vert", "shader.frag", [this](DataSourceRef vert,DataSourceRef frag){
        try{
            mShader = gl::GlslProg( vert, frag );
        } catch( gl::GlslProgCompileExc exc ) { cout << exc.what() << endl; }
    } );
}


void GlslProgApp::draw()
{
	// clear out the window with black
	gl::clear( Color( 0, 0, 0 ) );
    
    gl::enableWireframe();
    
    mShader.bind();
    mShader.uniform( "time", (float) getElapsedSeconds() );
    gl::drawSphere( Vec3f( getWindowCenter().x, getWindowCenter().y, 0.0f ), 150.0f );
    mShader.unbind();
}

CINDER_APP_NATIVE( GlslProgApp, RendererGl )
