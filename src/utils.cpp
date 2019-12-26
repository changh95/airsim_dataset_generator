#include "utility.h"

utility::utility()
{
}

utility::~utility()
{
}

void utility::mkdir(std::string _path)
{
    std::string strDirPath = _path;
    std::string strTempDir;
    size_t indexOf = 0;

    while (true)
    {
        indexOf = strDirPath.find("/");
        strTempDir += strDirPath.substr(0, indexOf) + "/";
        int ret = _mkdir(strTempDir.c_str());// , 0777);
        strDirPath = strDirPath.substr((size_t)(indexOf + 1), strDirPath.length());
        if (indexOf < 0) break;
        if (strTempDir.compare(_path + "/") == 0) break;
    }
}

std::vector<std::string> utility::csvReadRow(std::istream &file, char delimiter) {
    std::stringstream ss;
    bool inquotes = false;
    std::vector<std::string> row;

    while (file.good()) {
        char c = file.get();

        if (!inquotes && c == '"') {
            inquotes = true;
        }
        else if (inquotes && c == '"') {
            if (file.peek() == '"') {
                ss << (char)file.get();
            }
            else {
                inquotes = false;
            }
        }
        else if (!inquotes && c == delimiter) {
            row.push_back(ss.str());
            ss.str("");
        }
        else if (!inquotes && (c == '\r' || c == '\n')) {
            if (file.peek() == '\n') {
                file.get();
            }

            row.push_back(ss.str());

            return row;
        }
        else {
            ss << c;
        }
    }
}