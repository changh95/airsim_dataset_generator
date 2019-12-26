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

class create_dataset
{
private:
    msr::airlib::RpcLibClientBase client;
    typedef msr::airlib::ImageCaptureBase::ImageRequest ImageRequest;
    typedef msr::airlib::ImageCaptureBase::ImageResponse ImageResponse;
    typedef msr::airlib::ImageCaptureBase::ImageType ImageType;
    typedef common_utils::FileSystem FileSystem;

    utility* utils;
    std::vector<std::string> listCamName;
    std::vector<ImageType> listCamType;

    std::string path;
    int seqNum;
    char sequenceFolderName[256];
    char resFolderName[256];
    char imageFileName[256];

    msr::airlib::Pose cvPose;
    std::vector<msr::airlib::Pose> lastFiveFrames;
    bool ingFlag;
    int frameNum;

    std::ofstream readme;
    std::string readmePath;

    std::string resFolderPath;

    std::ofstream *dataFile;
    std::string dataFilePath;

    std::string imageFilePath;

    std::vector<std::string> row;

public:
    create_dataset(std::string _path, std::vector<std::string> _listCamName, std::vector<ImageType> _listCamType);
    ~create_dataset();

    ErrorCode run();
};

