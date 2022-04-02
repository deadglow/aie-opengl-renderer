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

int FileReader::LoadFileAsStringVector(std::vector<std::string>* lines, const std::string filename)
{
    std::ifstream file(filename);

    if (file.is_open())
    {
        while (!file.eof())
        {
            std::string thisLine;
            std::getline(file, thisLine);
            lines->push_back(thisLine);
        }
        file.close();
        return (int)lines->size();
    }
    else
    {
        std::cout << "Error loading file: " << filename << std::endl;
        return -1;
    }
}

int FileReader::GetAllFilesWithExtensions(std::string dir, std::vector<std::string>* extensions, std::vector<fs::path>* paths)
{
    if (fs::exists(dir) && fs::is_directory(dir))
    {
        for (fs::directory_entry const& entry : fs::recursive_directory_iterator(dir))
        {
            if (!fs::is_regular_file(entry)) continue;
            fs::path path = entry.path();

            for (int i = 0; i < extensions->size(); ++i)
            {
                if (path.extension() == (*extensions)[i])
                {
                    paths->push_back(path);
                    break;
                }
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