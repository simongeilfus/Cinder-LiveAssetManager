//
//  LiveDataSource.h
//  LiveAssetAPI
//
//  Created by Simon Geilfus on 17/01/13.

#pragma once


#include "cinder/Filesystem.h"
#include "cinder/DataSource.h"

typedef std::shared_ptr<class LiveAsset> LiveAssetRef;

class LiveAsset {
public:
    LiveAsset(){}
    LiveAsset( ci::fs::path relativePath, std::function<void(ci::DataSourceRef)> callback );
    
    virtual void refresh();
    virtual void notify();
    
protected:
    ci::fs::path                            mRelativePath;
    std::time_t                             mLastTimeWritten;
    std::function<void(ci::DataSourceRef)>  mCallback;
};

class LiveAssetPair : public LiveAsset {
public:
    LiveAssetPair( ci::fs::path firstRelativePath, ci::fs::path secondRelativePath, std::function<void(ci::DataSourceRef, ci::DataSourceRef)> callback );
    
    virtual void refresh();
    virtual void notify();
    
protected:
    std::pair<ci::fs::path,ci::fs::path>                        mRelativePath;
    std::pair<std::time_t,std::time_t>                          mLastTimeWritten;
    std::function<void(ci::DataSourceRef, ci::DataSourceRef)>   mCallback;
};

class LiveAssetManager {
public:
    
    static void load( const ci::fs::path &relativePath, std::function<void(ci::DataSourceRef)> callback );
    static void load( const ci::fs::path &vertexRelPath, const ci::fs::path &fragmentRelPath, std::function<void(ci::DataSourceRef,ci::DataSourceRef)> callback );
    
protected:
    LiveAssetManager(){}
    
    static LiveAssetManager* instance();
    
    void update();
    
    std::deque< LiveAssetRef > mAssets;
    static LiveAssetManager* mInstance;
};