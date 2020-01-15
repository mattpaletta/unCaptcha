#include <boost/assign/list_of.hpp>
#include <boost/filesystem.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <vector>
#include <string>
#include <iostream>
#include <algorithm>
#include <utility>

#include "Model.h"
#include "helpers.h"

const std::string ROOT_FOLDER = "";
const std::string CAPTCHA_IMAGE_FOLDER = ROOT_FOLDER + "generated_captcha_images";
const std::string OUTPUT_FOLDER = ROOT_FOLDER + "extracted_letter_images";

std::map<std::string, int> counts = {
        {"A", 1},
        {"B", 1},
        {"C", 1},
        {"D", 1},
        {"E", 1},
        {"F", 1},
        {"G", 1},
        {"H", 1},
        {"I", 1},
        {"J", 1},
        {"K", 1},
        {"L", 1},
        {"M", 1},
        {"N", 1},
        {"O", 1},
        {"P", 1},
        {"Q", 1},
        {"R", 1},
        {"S", 1},
        {"T", 1},
        {"U", 1},
        {"V", 1},
        {"W", 1},
        {"X", 1},
        {"Y", 1},
        {"Z", 1},
        {"0", 1},
        {"1", 1},
        {"2", 1},
        {"3", 1},
        {"4", 1},
        {"5", 1},
        {"6", 1},
        {"7", 1},
        {"8", 1},
        {"9", 1}};

void read_image(const std::string &filename) {
    std::cout << "Processing file: " + filename << std::endl;

    cv::Mat image, grey, grey2, thresholded;

    image = cv::imread(filename);

    if (!image.data) {
        printf("No image data \n");
//        return -1;
    } else {
        // Turn it greyscale
        cv::cvtColor(image, grey, cv::COLOR_BGR2GRAY);

        cv::copyMakeBorder(grey, grey2, 8, 8, 8, 8, cv::BORDER_REPLICATE);
        std::vector<std::vector<cv::Point> > contours;
        std::vector<cv::Vec4i> hierarchy;

        cv::threshold(grey2, thresholded, 0, 255, cv::THRESH_BINARY_INV | cv::THRESH_OTSU);
        cv::findContours(thresholded, contours, hierarchy, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

        std::vector<cv::Rect> letter_image_regions = {};

        for (const auto &contour : contours) {
            auto rect = cv::boundingRect(contour);

            int x = rect.x;
            int y = rect.y;
            int width = rect.width;
            int height = rect.height;

            if ((float(x) / float(y)) > 1.25) {
                auto half_width = int(float(width) / 2.0);
                letter_image_regions.emplace_back(x, y, half_width, height);
                letter_image_regions.emplace_back(x + half_width, y, half_width, height);
            } else {
                letter_image_regions.emplace_back(x, y, width, height);
            }
        }

        if (contours.size() != 4) {
            std::cout << "Found no bounds." << std::endl;
            return;
        }

        std::sort(letter_image_regions.begin(), letter_image_regions.end(), [](const cv::Rect& lhs, const cv::Rect& rhs) {
            return lhs.x < rhs.x;
        });

        auto base_name = Utils::base_filename(Utils::base_path(filename));
        std::vector<char> captcha_text(base_name.begin(), base_name.end());
        auto combined = Utils::zip(captcha_text, letter_image_regions);

        for (auto curr : combined) {
            auto letter_text = curr.first;
            auto bounding_box = curr.second;

            int x = bounding_box.x;
            int y = bounding_box.y;
            int width = bounding_box.width;
            int height = bounding_box.height;

            cv::Mat Y;
            grey2(cv::Rect(x, y, width, height)).copyTo(Y);

            auto save_path = Utils::make_path({OUTPUT_FOLDER, std::string(1, letter_text)});

            boost::filesystem::path odir(OUTPUT_FOLDER);
            if (boost::filesystem::create_directory(odir)) {
                std::cerr << "Directory Created: "<< OUTPUT_FOLDER << std::endl;
            }

            boost::filesystem::path dir(save_path);
            if (boost::filesystem::create_directory(dir)) {
                std::cerr << "Directory Created: "<< save_path << std::endl;
            }

            auto count = counts.find(std::string(1, letter_text))->second;
            char output_file_name[9];
            sprintf(output_file_name, "%06d.png", count);

            auto output_file_path = Utils::make_path({save_path, output_file_name});

            cv::imwrite(output_file_path, Y);

            counts.at(counts.find(std::string(1, letter_text))->first)++;
        }
    }
}

int main() {
//    auto workingFiles = Utils::list_directory(CAPTCHA_IMAGE_FOLDER);
//    std::for_each(workingFiles.begin(), workingFiles.end(), read_image);

    Model mdl;
    mdl.train(10, CAPTCHA_IMAGE_FOLDER);
    return 0;
}