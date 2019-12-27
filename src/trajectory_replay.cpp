#include "trajectory_replay.h"



trajectory_replay::trajectory_replay(std::string _path, int _sleeptime)
{
    path = _path;
    ingFlag = true;
    utils = new utility();
    sleeptime = _sleeptime;
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

        std::cout << "\nPress Enter to play trajectory.\n";
        std::cin.get();

        float tx, ty, tz, qw, qx, qy, qz;

        while (trajFile.good())
        {
            printf_s("\nTrajectory playing...\n");

            row = utils->csvReadRow(trajFile, ',');

            if (row.empty())
            {
                ingFlag = false;
                break;
            }

            tx = std::stof(row[0]);
            ty = std::stof(row[1]);
            tz = std::stof(row[2]);
            qw = std::stof(row[3]);
            qx = std::stof(row[4]);
            qy = std::stof(row[5]);
            qz = std::stof(row[6]);

            Eigen::Vector3f position(tx, ty, tz);
            Eigen::Quaternion<float, Eigen::DontAlign> quaternion(qz, qw, qx, qy);
            cvPose = msr::airlib::Pose(position, quaternion);
            client.simSetVehiclePose(cvPose, true);

            Sleep(sleeptime);
        }
        std::cout << "\n---------------------------------------------------------------\n";
        std::cout << "\nTrajectory replay finished.\n" << std::endl;
        return E_SUCCESS;
    }
    catch (...)
    {

    }
}
