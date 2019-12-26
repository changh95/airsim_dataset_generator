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

    int seqNum;
    char sequenceFolderName[256];
    char resFolderName[256];

    msr::airlib::RpcLibClientBase client;
    typedef msr::airlib::ImageCaptureBase::ImageRequest ImageRequest;
    typedef msr::airlib::ImageCaptureBase::ImageResponse ImageResponse;
    typedef msr::airlib::ImageCaptureBase::ImageType ImageType;
    typedef common_utils::FileSystem FileSystem;

public:
    trajectory_replay(std::string _path);
    ~trajectory_replay();
    ErrorCode run(void);
};

