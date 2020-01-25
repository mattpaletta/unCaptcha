//
// Created by Matthew on 2018-11-04.
//
#pragma once

#include <vector>
#include <string>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/opencv.hpp>
#include "helpers.h"
#include "deepcpp.hpp"

class Model {
public:
    Model() {

    }

    auto get_model(const std::size_t input_size = 20) {
		return deepcpp::ModelBuilder().
			Conv2D(20, {5, 5}, "same", "relu", {input_size, input_size, 1})->
			MaxPooling2D({2, 2}, {2, 2})->
			Conv2D(50, {5, 5}, "same", "relu")->
			MaxPooling2D({2, 2}, {2, 2})->
			Flatten()->
			Dense(500, "relu")->
			Dense(32, "softmax")->
			Build();
    }

	auto process_images(std::string data) {
		deepcpp::train::Batch<std::string> d;
		return d;
	}

	void train(const std::size_t ep, const std::string &training_dir) {
		std::string base_folder = "/Users/bennyfriedman/Code/TF2example/TF2example/data/cats_and_dogs_small/train";
		deepcpp::train::Batch<std::string> data = this->process_images(base_folder);

		constexpr auto test_size = 0.25;
		auto train_validate = deepcpp::train::train_test_split(data, test_size);
		auto train_data = train_validate.first;
		auto validation_data = train_validate.second;

		auto model = this->get_model();

		model->compile("categorical_crossentropy", "adam", {"accuracy"});
		constexpr auto epochs = 32;
		constexpr auto batch_size = 10;
		constexpr auto verbose = true;
		model->fit(train_data, validation_data, epochs, batch_size, verbose);

		constexpr auto MODEL_FILENAME = "my_model";
		model->save(MODEL_FILENAME);
	}

private:
	/*
	std::vector<> process_images(const std::string &training_set_dir) {
        vector<case_t> cases = {};

        for (const auto &filename : Utils::list_directory(training_set_dir)) {
            cv::Mat image, grey, resized;

            image = cv::imread(filename);
            cv::cvtColor(image, grey, cv::COLOR_BGR2GRAY);

            cv::resize(grey, resized, resized.size(), 20, 20, cv::INTER_NEAREST);

            auto label = Utils::base_filename(Utils::base_path(filename));

            auto new_matrix = tensor_t<float>(20, 20, 3);
            auto new_label = tensor_t<float>(26 + 10, 1, 1); // Store one hot number/letters (in caps).

            // Convert the matrix to a tensor.
//            for (int r = 0; r < &resized.size.p; r++) {
//                for (int c = 0; c < &resized.size.p; c++) {
//
//                }
//            }

            case_t c {
                new_matrix,
                new_label
            };

            cases.push_back(c);
        }

        return cases;
    }*/
};
