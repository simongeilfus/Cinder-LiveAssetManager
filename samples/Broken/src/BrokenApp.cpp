#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"
#include "cinder/gl/GlslProg.h"
#include "LiveAssetManager.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class Test {
public:
    Test(){}
    Test( int dummy )
    {
        LiveAssetManager::load( "shader.vert", "shader.frag", [this](DataSourceRef vert,DataSourceRef frag){
            console() << "Callback fired!" << endl;
            try{
                mShader = gl::GlslProg( vert, frag );
            } catch( gl::GlslProgCompileExc exc ) { cout << exc.what() << endl; }
        } );
    }
    void init()
    {
        LiveAssetManager::load( "shader.vert", "shader.frag", [this](DataSourceRef vert,DataSourceRef frag){
            try{
                mShader = gl::GlslProg( vert, frag );
            } catch( gl::GlslProgCompileExc exc ) { cout << exc.what() << endl; }
        } );
    }
    
    gl::GlslProg mShader;
};

class BrokenApp : public AppNative {
  public:
	void setup();
	void draw();
    
    Test    mTest1;
};

void BrokenApp::setup()
{
    mTest1 = Test( 0 );     // for some reason the callback is called but the variable not updated...
    //mTest1.init();        // will work
}
void BrokenApp::draw()
{
	gl::clear( Color( 0, 0, 0 ) );
    
    gl::enableWireframe();
    
    mTest1.mShader.bind();
    mTest1.mShader.uniform( "time", (float) getElapsedSeconds() );
    gl::drawSphere( Vec3f( getWindowCenter().x, getWindowCenter().y, 0.0f ), 150.0f );
    mTest1.mShader.unbind();
}

CINDER_APP_NATIVE( BrokenApp, RendererGl )
