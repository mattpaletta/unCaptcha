//
// Created by Matthew on 2018-11-04.
//

#include <vector>
#include <string>
#include <iostream>
#include <algorithm>
#include <utility>
#include <boost/filesystem.hpp>
#include <opencv2/opencv.hpp>

#ifndef CAPTCHA_HELPERS_H
#define CAPTCHA_HELPERS_H

class Utils{
public:
    static inline bool file_exists(const std::string& name);

//    template<typename T, typename K>
    static std::vector<std::pair<char, cv::Rect>> zip(std::vector<char> i1, std::vector<cv::Rect> i2);

    static std::string base_filename(std::string filename);

    static std::string base_path(std::string filepath);

    static std::string get_last(const std::string &str);

    static std::string make_path(std::vector<std::string> path_pieces);

    static std::vector<std::string> list_directory(std::string folder_to_read);
};

#endif //CAPTCHA_HELPERS_H