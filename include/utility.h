#pragma once
#include <iostream>
#include <chrono>
#include <fstream>
#include <direct.h>
#include <vector>
#include "api/RpcLibClientBase.hpp"
#include "common/common_utils/FileSystem.hpp"

enum ErrorCode {
    E_SUCCESS = 0,
    E_TRAJECTORY_FILE_NOT_FOUND,
    E_API_EXCEPTION
};

class utility
{
private:

public:
    utility();
    ~utility();
    void mkdir(std::string _path);
    std::vector<std::string> csvReadRow(std::istream &file, char delimiter);
};
