#include "trajectory_replay.h"



trajectory_replay::trajectory_replay(std::string _path)
{
    path = _path;
    ingFlag = true;
    utils = new utility();
}

trajectory_replay::~trajectory_replay()
{
    // WHAT TO PUT?
}

ErrorCode trajectory_replay::run(void) 
{
    try
    {
        std::cout << "Enter the trajectory sequence number" << std::endl;
        std::cin >> seqNum;
        sprintf(sequenceFolderName, "/seq%04d", seqNum);
        std::string trajFilePath = path + sequenceFolderName + "/Trajectory.csv";

        std::cout << "\n---------------------------------------------------------------\n";
        std::cout << "\nLoading the trajectory file from " << trajFilePath << std::endl;

        std::ifstream trajFile(trajFilePath);
        if (trajFile.fail())
        {
            std::cout << "\nFailed to load the trajectory file\n";
            std::cout << "\nShutting down...";

            return E_TRAJECTORY_FILE_NOT_FOUND;
        }

        std::cout << "\nTrajectory successfully loaded\n";
        std::cout << "\nPress Enter to connect to Unreal Client\n";
        std::cin.get();

        client.confirmConnection();
        std::vector<ImageRequest> request = { ImageRequest("0", ImageType::Scene), ImageRequest("1", ImageType::Scene), ImageRequest("2", ImageType::Scene)};
        const std::vector<ImageResponse>& response = client.simGetImages(request);

        std::cout << "\n---------------------------------------------------------------\n";
        
        std::cout << "\nThe camera resolution is " << response.at(0).width << " x " << response.at(0).height << std::endl;
        std::cout << "\nThe camera resolution is " << response.at(1).width << " x " << response.at(1).height << std::endl;
        std::cout << "\nThe camera resolution is " << response.at(2).width << " x " << response.at(2).height << std::endl;
        
        //개발필요

    }
    catch (...)
    {

    }
}
