//
// Created by Matthew on 2018-11-04.
//

#ifndef CAPTCHA_MODEL_H
#define CAPTCHA_MODEL_H

#include <vector>
#include <string>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/opencv.hpp>
#include "helpers.cpp"

using namespace std;



class Model {
    public:
        Model() {

        }

        void build_model(string training_set_dir) {
            this->process_images(training_set_dir);
        }

        void train() {

        }

    private:

        vector<cv::Mat> data = {};
        vector<string> labels = {};

        void process_images(const string &training_set_dir) {
            for(const auto &filename : list_directory(training_set_dir)) {
                cv::Mat image, grey, resized;

                image = cv::imread(filename);
                cv::cvtColor(image, grey, cv::COLOR_BGR2GRAY);

                cv::resize(grey, resized, resized.size(), 20, 20, cv::INTER_NEAREST);

                auto label = base_filename(base_path(filename));


                data.push_back(resized);
                labels.push_back(label);
            }
        }

        void get_model() {

        }
};


#endif //CAPTCHA_MODEL_H
