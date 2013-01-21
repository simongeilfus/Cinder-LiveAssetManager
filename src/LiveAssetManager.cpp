//
//  LiveDataSource.h
//  LiveAssetAPI
//
//  Created by Simon Geilfus on 17/01/13.


#include "LiveAssetManager.h"

LiveAsset::LiveAsset( ci::fs::path relativePath, std::function<void(ci::DataSourceRef)> callback )
:   mRelativePath( relativePath ),
    mCallback( callback ),
    mLastTimeWritten( ci::fs::last_write_time( ci::app::getAssetPath( relativePath ) ) )
{
}

void LiveAsset::refresh()
{
    std::time_t time = ci::fs::last_write_time( ci::app::getAssetPath( mRelativePath ) );
    if( time > mLastTimeWritten ){
        mLastTimeWritten   = time;
        notify();
    }
}
void LiveAsset::notify()
{
    mCallback( ci::app::loadAsset( mRelativePath ) );
}



LiveAssetPair::LiveAssetPair( ci::fs::path firstRelativePath, ci::fs::path secondRelativePath, std::function<void(ci::DataSourceRef, ci::DataSourceRef)> callback )
:   mRelativePath( std::make_pair( firstRelativePath, secondRelativePath ) ),
    mLastTimeWritten( std::make_pair( ci::fs::last_write_time( ci::app::getAssetPath( firstRelativePath ) ), ci::fs::last_write_time( ci::app::getAssetPath( secondRelativePath ) ) ) ),
    mCallback( callback )
{
}
    
void LiveAssetPair::refresh()
{
    std::time_t firstTime = ci::fs::last_write_time( ci::app::getAssetPath( mRelativePath.first ) );
    std::time_t secondTime = ci::fs::last_write_time( ci::app::getAssetPath( mRelativePath.second ) );
    if( firstTime > mLastTimeWritten.first || secondTime > mLastTimeWritten.second ){
        mLastTimeWritten.first      = firstTime;
        mLastTimeWritten.second     = secondTime;
        notify();
    }
}
void LiveAssetPair::notify()
{
    mCallback( ci::app::loadAsset( mRelativePath.first ), ci::app::loadAsset( mRelativePath.second ) );
}


void LiveAssetManager::load( const ci::fs::path &relativePath, std::function<void(ci::DataSourceRef)> callback )
{
    if( ci::fs::exists( ci::app::getAssetPath( relativePath ) ) ){
        LiveAssetRef liveAsset = LiveAssetRef( new LiveAsset( relativePath, callback ) );
        instance()->mAssets.push_back( liveAsset );
        liveAsset->notify();
    }
    else
        throw ci::app::AssetLoadExc( relativePath );
};
void LiveAssetManager::load( const ci::fs::path &vertexRelPath, const ci::fs::path &fragmentRelPath, std::function<void(ci::DataSourceRef,ci::DataSourceRef)> callback )
{
    if( ci::fs::exists( ci::app::getAssetPath( vertexRelPath ) ) && ci::fs::exists( ci::app::getAssetPath( fragmentRelPath ) ) ){
        LiveAssetRef liveAsset = LiveAssetRef( new LiveAssetPair( vertexRelPath, fragmentRelPath, callback ) );
        instance()->mAssets.push_back( liveAsset );
        liveAsset->notify();
    }
    else
        throw ci::app::AssetLoadExc( vertexRelPath ); // not necessary correct!
    
}

LiveAssetManager* LiveAssetManager::instance()
{
    if( !mInstance ){
        mInstance = new LiveAssetManager();
        ci::app::App::get()->getSignalUpdate().connect( std::bind( &LiveAssetManager::update, mInstance ) );
    }
    return mInstance;
}

void LiveAssetManager::update()
{
    for( std::deque<LiveAssetRef>::iterator it = mAssets.begin(); it != mAssets.end(); ++it ){
        (*it)->refresh();
    }
}

LiveAssetManager* LiveAssetManager::mInstance;