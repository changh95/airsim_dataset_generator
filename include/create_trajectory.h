// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.
#pragma once

#include "common/common_utils/StrictMode.hpp"
STRICT_MODE_OFF
#ifndef RPCLIB_MSGPACK
#define RPCLIB_MSGPACK clmdep_msgpack
#endif // !RPCLIB_MSGPACK
#include "rpc/rpc_error.h"
STRICT_MODE_ON

#include "utility.h"

class create_trajectory
{
private:
    utility* utils;

    msr::airlib::RpcLibClientBase client;
    typedef msr::airlib::ImageCaptureBase::ImageRequest ImageRequest;
    typedef msr::airlib::ImageCaptureBase::ImageResponse ImageResponse;
    typedef msr::airlib::ImageCaptureBase::ImageType ImageType;
    typedef common_utils::FileSystem FileSystem;

    std::string path;
    msr::airlib::Pose cvPose;
    std::vector<msr::airlib::Pose> lastFiveFrames;
    bool loopFlag;
    int seqNum;

    char folderName[256];
    std::string folderPath;
    std::ofstream readme;
    std::string readmePath;
    std::ofstream trajFile;
    std::string trajFilePath;
public:
    create_trajectory(std::string _path);
    ~create_trajectory();

    ErrorCode run();
};

