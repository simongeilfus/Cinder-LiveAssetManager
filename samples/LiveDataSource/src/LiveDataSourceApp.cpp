#include "cinder/app/AppNative.h"
#include "cinder/gl/Texture.h"
#include "cinder/gl/GlslProg.h"

#include "LiveAssetManager.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class LiveDataSourceApp : public AppNative {
  public:
	void setup();
	void draw();
    
    gl::Texture 	mTexture;
    gl::GlslProg    mShader;
};

void LiveDataSourceApp::setup()
{    
    setWindowSize( 530, 530 );
    
    LiveAssetManager::load( "cinder_logo_alpha.png", [this](DataSourceRef dataSource){
        
        // Thanks to c++11 support on all platform we can use lambdas and
        // there's no need to write an extra function anywhere else.
        mTexture = gl::Texture( loadImage( dataSource ) );
    } );
    
    LiveAssetManager::load( "shader.glsl", [this](DataSourceRef dataSource){
        
        // Because you can provide your own reloading function, you can catch
        // exceptions and/or check if something went wrong while editing the file.
        try {
            mShader = gl::GlslProg( DataSourceRef(), dataSource );
        }
        catch( gl::GlslProgCompileExc exc ){ console() << exc.what() << endl; }
    });
    
    
    gl::enableAlphaBlending();
}

void LiveDataSourceApp::draw()
{    
    mShader.bind();
    gl::drawSolidRect( getWindowBounds() );
    mShader.unbind();
    
    gl::draw( mTexture, Vec2f( 9, 9 ) );
}

CINDER_APP_NATIVE( LiveDataSourceApp, RendererGl )
