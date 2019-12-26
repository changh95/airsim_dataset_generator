#include "create_dataset.h"

create_dataset::create_dataset(std::string _path, std::vector<std::string> _listCamName, std::vector<ImageType> _listCamType)
{
    path = _path;
    ingFlag = true;
    utils = new utility();
    listCamName = _listCamName;
    listCamType = _listCamType;
}

create_dataset::~create_dataset()
{
    //WHAT TO PUT?
}

ErrorCode create_dataset::run() 
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
        std::vector<ImageRequest> request{};

        for (int i = 0; i < listCamName.size(); i++)
        {
            request.push_back(ImageRequest(listCamName.at(i), listCamType.at(i)));
        }

        const std::vector<ImageResponse>& response = client.simGetImages(request);

        std::cout << "\n---------------------------------------------------------------\n";

        dataFile = new std::ofstream[listCamName.size()];
        for (int i = 0; i < listCamName.size(); i++)
        {
            std::cout << "\nThe camera resolution is \n" << response.at(i).width << " x " << response.at(i).height << std::endl;
            //std::cout << "\NThe image type is \n"; // 개발플리즈
            sprintf(resFolderName, "/%04d_%04d", response.at(i).width, response.at(i).height);
            resFolderPath = path + sequenceFolderName + resFolderName;
            utils->mkdir(resFolderPath);

            readmePath = resFolderPath + "/Readme.md";
            readme.open(readmePath, std::ios::out | std::ios::app | std::ios::binary);
            readme << "Dataset file structure: \n" << std::endl;
            readme << "Image file name, System time, Tx(m), Ty(m), Tz(m), Qw, Qx, Qy, Qz, \n" << std::endl;
            readme.close();

            dataFilePath = resFolderPath + "/Data.csv";
            dataFile[i].open(dataFilePath, std::ios::out | std::ios::app | std::ios::binary);
        }

        float tx, ty, tz, qw, qx, qy, qz;
        frameNum = 0;

        std::string datasetFolderPath = path + sequenceFolderName;
        std::cout << "\nThe dataset directory will be saved at " << datasetFolderPath << std::endl;
        std::cout << "\nPress Enter to begin dataset recording\n";
        std::cin.get();

        std::cout << "\n---------------------------------------------------------------\n";

        while (trajFile.good())
        {
            std::cout << "Generating dataset... Frame Number: " << frameNum << std::endl;

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

            const std::vector<ImageResponse>& response = client.simGetImages(request);
            sprintf(imageFileName, "%06d.png", frameNum);

            for (int i = 0; i < listCamName.size(); i++) 
            {
                sprintf(resFolderName, "/%04d_%04d", response.at(i).width, response.at(i).height);
                imageFilePath = path + sequenceFolderName + resFolderName + "/" + imageFileName;
                std::string imageFileName_ = imageFileName;
                std::ofstream imageFile(imageFilePath, std::ios::binary);
                imageFile.write(reinterpret_cast<const char*>(response[i].image_data_uint8.data()), response[i].image_data_uint8.size());
                imageFile.close();

                dataFile[i] << imageFileName_ << ",";
                dataFile[i] << response[0].time_stamp << ",";
                dataFile[i] << response[0].camera_position[0] << ",";
                dataFile[i] << response[0].camera_position[1] << ",";
                dataFile[i] << response[0].camera_position[2] << ",";
                dataFile[i] << response[0].camera_orientation.coeffs()[0] << ",";
                dataFile[i] << response[0].camera_orientation.coeffs()[1] << ",";
                dataFile[i] << response[0].camera_orientation.coeffs()[2] << ",";;
                dataFile[i] << response[0].camera_orientation.coeffs()[3] << std::endl;
            }

            frameNum++;
        }
        for (int i = 0; i < listCamName.size(); i++)
        {
            dataFile[i].close();
        }

        std::cout << "\n---------------------------------------------------------------\n";
        std::cout << "\nDataset saved!\n" << std::endl;
        return E_SUCCESS;
    }
    catch (rpc::rpc_error&  e) 
    {
        std::string msg = e.get_error().as<std::string>();
        std::cout << "\n---------------------------------------------------------------\n";
        std::cout << "\nException raised by the API, something went wrong.\n" << std::endl << msg << std::endl;
        return E_API_EXCEPTION;
    }

}