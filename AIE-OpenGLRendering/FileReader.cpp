#include "FileReader.h"
#include <fstream>
#include <sstream>
#include <iostream>

std::string FileReader::LoadFileAsString(std::string filename)
{
    std::stringstream workingFile;
    std::ifstream file(filename);

    if (file.is_open())
    {
        while (!file.eof())
        {
            std::string thisLine;
            std::getline(file, thisLine);
            workingFile << thisLine << std::endl;
        }
        file.close();
        return workingFile.str();
    }
    else
    {
        std::cout << "Error loading file: " << filename << std::endl;
        return "";
    }
}

int FileReader::GetAllFilesWithExtension(std::string dir, std::string extension, std::vector<fs::path>* paths)
{
    if (fs::exists(dir) && fs::is_directory(dir))
    {
        for (fs::directory_entry const& entry : fs::recursive_directory_iterator(dir))
        {
            if (!fs::is_regular_file(entry)) continue;
            fs::path path = entry.path();

            if (path.extension() == extension)
            {
                paths->push_back(path);
                continue;
            }
        }
        return (int)paths->size();
    }
    else
    {
        std::cout << "Invalid directory." << "\n";
        return -1;
    }
}