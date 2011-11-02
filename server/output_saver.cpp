#include "output_saver.hpp"
#include "logging.hpp"

#include <boost/lexical_cast.hpp>

#include <iostream>
#include <fstream>

std::string OutputSaver::fileExtension = ".txt";
std::string OutputSaver::storageDir = "storage/";

//OutputSaver::OutputSaver()
//    : guid(1), storagePath("/") { }

OutputSaver::OutputSaver(std::string rootPath)
    : guid(0), websiteRoot(rootPath + "/") { }

std::string OutputSaver::storeOutput(std::string content) {

    std::string newGuid = getNewGuid();
    std::string filePathToSave(websiteRoot + storageDir + newGuid + fileExtension);
    std::string filePathToReturn(storageDir + newGuid + fileExtension);

    INFO("Output saved to file: " << filePathToSave);

    std::ofstream outfile(filePathToSave.c_str());
    outfile << content;
    outfile.close();

    return filePathToReturn;
}

std::string OutputSaver::getNewGuid() {
    guid++;
    return boost::lexical_cast<std::string>(guid);
}
