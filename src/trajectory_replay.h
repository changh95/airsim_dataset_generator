#pragma once
#include "common/common_utils/StrictMode.hpp"
STRICT_MODE_OFF
#ifndef RPCLIB_MSGPACK
#define RPCLIB_MSGPACK clmdep_msgpack
#endif // !RPCLIB_MSGPACK
#include "rpc/rpc_error.h"
STRICT_MODE_ON

#include "utility.h"

class trajectory_replay
{
private: 
    std::string path;
    bool ingFlag;
    utility* utils;
    int sleeptime;

    int seqNum;
    char sequenceFolderName[256];
    char resFolderName[256];

    msr::airlib::RpcLibClientBase client;
    typedef msr::airlib::ImageCaptureBase::ImageRequest ImageRequest;
    typedef msr::airlib::ImageCaptureBase::ImageResponse ImageResponse;
    typedef msr::airlib::ImageCaptureBase::ImageType ImageType;
    typedef common_utils::FileSystem FileSystem;

    std::vector<std::string> row;
    msr::airlib::Pose cvPose;
    std::vector<msr::airlib::Pose> lastFiveFrames;

public:
    trajectory_replay(std::string _path, int _sleeptime = 75);
    ~trajectory_replay();
    ErrorCode run(void);
};

