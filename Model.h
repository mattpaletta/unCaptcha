//
// Created by Matthew on 2018-11-04.
//
#ifndef CAPTCHA_MODEL_H
#define CAPTCHA_MODEL_H

#include <vector>
#include <string>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/opencv.hpp>
#include "helpers.h"
#include "CNN/cnn.h"

using namespace std;

struct case_t
{
    tensor_t<float> data;
    tensor_t<float> out;
};

class Model {
public:
    Model() {

    }

    void train(unsigned int ep, const string &training_dir) {
        auto cases = process_images(training_dir);

        vector<layer_t*> layers = get_model({20, 20, 3});

        // Ep = 10'000

        float amse = 0;
        int ic = 0;

        for (unsigned long e = 0; e < ep; e++) {
            cout << "iterating episodes" <<endl;
            for (auto & t : cases) {

//                printf("iterating cases: %d %d\n", ic, e);
                float xerr = train_net(layers, t.data, t.out );
                amse += xerr;

                if ( ic % 1000 == 0 ) {
//                    cout << "printing!" << endl;
                    cout << "case " << ic << " err=" << amse << endl;
                }
                ic++;
            }
        }
    }

private:
    vector<case_t> process_images(const string &training_set_dir) {
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
    }

    vector<layer_t*> get_model(point_t input_size) {
        vector<layer_t*> layers;
        auto * layer1 = new conv_layer_t(1, 5, 8, input_size);
        auto * layer2 = new relu_layer_t(layer1->out.size);
        auto * layer3 = new pool_layer_t( 2, 2, layer2->out.size);

        auto * layer4 = new conv_layer_t(5, 3, 10, layer3->out.size);
        auto * layer5 = new pool_layer_t(2, 2, layer4->out.size);

        auto * layer6 = new fc_layer_t(layer5->out.size, 500);
        auto * layer7 = new relu_layer_t(layer6->out.size);

        auto * layer8 = new fc_layer_t(layer7->out.size, 32);
        auto * layer9 = new relu_layer_t(layer8->out.size);

        layers.push_back( (layer_t*)layer1 );
        layers.push_back( (layer_t*)layer2 );
        layers.push_back( (layer_t*)layer3 );
        layers.push_back( (layer_t*)layer4 );
        layers.push_back( (layer_t*)layer5 );
        layers.push_back( (layer_t*)layer6 );
        layers.push_back( (layer_t*)layer7 );
        layers.push_back( (layer_t*)layer8 );
        layers.push_back( (layer_t*)layer9 );

        return layers;
    }

    float train_net(vector<layer_t *> &layers, tensor_t<float> &data, tensor_t<float> &expected) {
        for (int i = 0; i < layers.size(); i++) {
            if (i == 0)
                activate(layers[i], data);
            else
                activate(layers[i], layers[i - 1]->out);
        }

        tensor_t<float> grads = layers.back()->out - expected;

        for (long i = layers.size() - 1; i >= 0; i--) {
            if (i == layers.size() - 1) {
                calc_grads(layers[i], grads);
            } else {
                calc_grads(layers[i], layers[i + 1]->grads_in);
            }
        }

        for (auto &layer : layers) {
            fix_weights(layer);
        }

        float err = 0;
        for (int i = 0; i < grads.size.x * grads.size.y * grads.size.z; i++) {
            float f = expected.data[i];
            if (f > 0.5) {
                err += abs(grads.data[i]);
            }
        }
        return err * 100;
    }


    void forward_pass(vector<layer_t *> &layers, tensor_t<float> &data) {
        for (int i = 0; i < layers.size(); i++) {
            if (i == 0) {
                activate(layers[i], data);
            } else {
                activate(layers[i], layers[i - 1]->out);
            }
        }
    }
};


#endif //CAPTCHA_MODEL_H
