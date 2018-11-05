//
// Created by Matthew on 2018-11-04.
//

#include <vector>
#include <string>
#include <iostream>
#include <algorithm>
#include <utility>
#include <boost/filesystem.hpp>
#include <sys/stat.h>
#include <opencv2/opencv.hpp>
#include "helpers.h"

using namespace boost::filesystem;


bool Utils::file_exists(const std::string& name) {
    struct stat buffer{};
    return (stat (name.c_str(), &buffer) == 0);
}

//template <class T, class K>
std::vector<std::pair<char, cv::Rect>> Utils::zip(std::vector<char> i1, std::vector<cv::Rect> i2) {
    std::vector<std::pair<char, cv::Rect>> new_vector = {};
    std::transform(i1.begin(),
                   i1.end(),
                   i2.begin(),
                   std::back_inserter(new_vector),
                   [](const auto& aa, const auto& bb)  {
                       return std::pair<char, cv::Rect>{aa, bb};
                   });
    return new_vector;
}

std::string Utils::base_filename(std::string filename) {
    std::string start = std::move(filename);
    std::string delimiter = ".";

    size_t pos = 0;
    std::string token;
    pos = start.find(delimiter);
    token = start.substr(0, pos);

    return token;
}

std::string Utils::base_path(std::string filepath) {
    std::string start = std::move(filepath);
    std::string delimiter = "/";

    size_t pos = 0;
    std::string token;
    while ((pos = start.find(delimiter)) != std::string::npos) {
        token = start.substr(0, pos);
//        std::cout << token << std::endl;
        start.erase(0, pos + delimiter.length());
    }

    return start;
}

std::string Utils::get_last(const std::string &str) {
    if (str.length() <= 1) {
        return str;
    }
    return str.substr(str.length() - 2, str.length() - 1);
}

std::string Utils::make_path(std::vector<std::string> path_pieces) {
    std::string output;

    for (const auto &v : path_pieces) {
        if (get_last(v) == "/") {
            output += v;
        } else {
            output += (v + "/");
        }
    }

    if (get_last(output) != "/") {
        output.pop_back();
    }

    return output;
}

std::vector<std::string> Utils::list_directory(std::string folder_to_read) {
    path p(folder_to_read);

    std::vector<std::string> outputFiles = {};

    for (auto i = directory_iterator(p); i != directory_iterator(); i++) {
        if (!is_directory(i->path())) { //we eliminate directories
            std::string file_name = i->path().filename().string();
            outputFiles.push_back(make_path({folder_to_read, file_name}));
        } else {
            continue;
        }
    }

    std::cout << "Found files: " << std::endl;
    for (const auto &v : outputFiles) {
        std::cout << v << std::endl;
    }

    return outputFiles;
}