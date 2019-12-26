#include "create_trajectory.h"
#include "create_dataset.h"
#include "trajectory_replay.h"

typedef msr::airlib::ImageCaptureBase::ImageType ImageType;

int main()
{
    //Edit this variable for your needs;
    std::string path = "E:/Projects/AR Engine/Data";
    std::vector<std::string> listCamName = { "0", "1", "2" };
    std::vector<ImageType> listCamType = { ImageType::Scene, ImageType::Scene, ImageType::Scene }; // Scene, DepthPlanner, DepthPerspective, DepthVis, DisparityNormalized, Segmentation, SurfaceNormals, Infrared

    if (listCamName.empty() || listCamName.size() != listCamType.size())
    {
        std::cout << "Please fill in listCamName and listCamType correctly\n";
        return -2;;
    }

    std::cout << "Make sure settings.json has \"SimMode\"=\"ComputerVision\" at root. Press Enter to continue." << std::endl; std::cin.get();
    std::cout << "\nThis program offers the following modes:\n";
    std::cout << "Type \"1\" for trajectory generation\n";
    std::cout << "Type \"2\" for trajectory replay\n";
    std::cout << "Type \"3\" for dataset generation\n";
    //std::cout << "Type \"4\" for evaluate SLAM algorithm \n";
    std::string answer;
    
    while (answer != "1" || answer != "2" || answer != "3")
    {
        answer.clear();
        std::getline(std::cin, answer);

        if (answer == "1" || answer == "2" || answer == "3") 
        {
            break;
        }
    }

    if (answer == "1")
    {
        std::cout << "\n---------------------------------------------------------------\n";
        std::cout << "\nYou selected trajectory generation option. \n";
        std::cout << "\nYou need to set an action sequence prior to running this program. Make sure you have turned on the \'autoplay\' option.\n";
        create_trajectory trajectoryGeneration(path);
        ErrorCode result = trajectoryGeneration.run();
        return result;
    } 
    if (answer == "2") 
    {
        std::cout << "\n---------------------------------------------------------------\n";
        std::cout << "\nYou selected trajectory replay option. \n";
        std::cout << "\nYou need the trajectory file saved inthe path prior to running this program.\n";
        std::cout << "\nMake sure you have turned off the \'autoplay\' option. \n";

        trajectory_replay trajectoryReplay(path);
        ErrorCode result = trajectoryReplay.run();
    }
    if (answer == "3")
    {
        std::cout << "\n---------------------------------------------------------------\n";
        std::cout << "\nYou selected dataset generation option. \n";
        std::cout << "\nYou need the trajectory file saved inthe path prior to running this program.\n";
        std::cout << "\nMake sure you have turned off the \'autoplay\' option. \n";
        create_dataset datasetGeneration(path, listCamName, listCamType);
        ErrorCode result = datasetGeneration.run();
        return result;
    }

    return -1;
}
