// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.
#include "create_trajectory.h"

create_trajectory::create_trajectory(std::string _path)
{
    path = _path;
    loopFlag = true;
    utils = new utility();
}

create_trajectory::~create_trajectory(void)
{
    //WHAT TO PUT?
}

ErrorCode create_trajectory::run() 
{
    try 
    {
        std::cout << "\nEnter the trajectory sequence number" << std::endl;
        std::cin >> seqNum;
        sprintf(folderName, "/seq%04d", seqNum);
        folderPath = path + folderName;

        std::cout << "\n---------------------------------------------------------------\n";
        std::cout << "\nThe sequence will be recorded at " << folderPath << std::endl;
        utils->mkdir(folderPath);
        
        readmePath = folderPath + "/Readme.md";
        trajFilePath = folderPath + "/Trajectory.csv";

        readme.open(readmePath, std::ios::out | std::ios::app | std::ios::binary);
        readme << "Trajectory file structure: \n";
        readme << "Tx(m), Ty(m), Tz(m), Qw, Qx, Qy, Qz, \n";
        readme.close();

        trajFile.open(trajFilePath, std::ios::out | std::ios::app | std::ios::binary);

        std::cout << "\n---------------------------------------------------------------\n";
        std::cout << "\nThe trajectory file will be saved at " << trajFilePath << ". \n";
        std::cout << "\nThe trajectory file name is .../Trajectory.csv \n";
        std::cout << "\nPlay the AirSim client, and press Enter to begin trajectory recording\n" << std::endl;
        std::cout << "\n---------------------------------------------------------------\n";
        std::cin.get();

        client.confirmConnection();
        std::vector<ImageRequest> request = { ImageRequest("0", ImageType::Scene) };

        int frameNum = 0;

        while (loopFlag)
        {
            printf("Generating Trajectory... Frame Number %04d\n", frameNum);

            const std::vector<ImageResponse>& response = client.simGetImages(request);
            cvPose = msr::airlib::Pose(response[0].camera_position, response[0].camera_orientation);
            lastFiveFrames.emplace_back(cvPose);

            trajFile << response.at(0).camera_position[0] << ",";
            trajFile << response.at(0).camera_position[1] << ",";
            trajFile << response.at(0).camera_position[2] << ",";
            trajFile << response.at(0).camera_orientation.coeffs()[0] << ",";
            trajFile << response.at(0).camera_orientation.coeffs()[1] << ",";
            trajFile << response.at(0).camera_orientation.coeffs()[2] << ",";
            trajFile << response.at(0).camera_orientation.coeffs()[3] << std::endl;

            if (lastFiveFrames.size() >= 6)
            {
                lastFiveFrames.erase(lastFiveFrames.begin());

                if (lastFiveFrames[0] == lastFiveFrames[1] && lastFiveFrames[1] == lastFiveFrames[2] & lastFiveFrames[2] == lastFiveFrames[3] && lastFiveFrames[3] == lastFiveFrames[4])
                {
                    loopFlag = false;
                }
            }
            frameNum++;
        }

        trajFile.close();
        std::cout << "\nTrajectory Generation Complete! \n" << std::endl;
        return ErrorCode::E_SUCCESS;
    }
    catch (rpc::rpc_error&  e) 
    {
        std::string msg = e.get_error().as<std::string>();
        std::cout << "Exception raised by the API, something went wrong." << std::endl << msg << std::endl; 
        return E_API_EXCEPTION;
    }
}