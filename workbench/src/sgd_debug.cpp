//=======================================================================
// Copyright (c) 2014-2016 Baptiste Wicht
// Distributed under the terms of the MIT License.
// (See accompanying file LICENSE or copy at
//  http://opensource.org/licenses/MIT)
//=======================================================================

#include <deque>

#include "dll/neural/dense_layer.hpp"
#include "dll/test.hpp"
#include "dll/dbn.hpp"
#include "dll/trainer/stochastic_gradient_descent.hpp"

#include "mnist/mnist_reader.hpp"
#include "mnist/mnist_utils.hpp"

int main(int /*argc*/, char* /*argv*/ []) {
    auto dataset = mnist::read_dataset_direct<std::vector, etl::dyn_vector<float>>();
    dataset.training_images.resize(10000);
    dataset.training_labels.resize(10000);

    auto n = dataset.training_images.size();
    std::cout << n << " samples to test" << std::endl;

    mnist::binarize_dataset(dataset);

    using dbn_t = dll::dbn_desc<
        dll::dbn_layers<
            dll::dense_desc<28 * 28, 250>::layer_t,
            dll::dense_desc<250, 10, dll::activation<dll::function::SOFTMAX>>::layer_t>,
        dll::momentum, dll::batch_size<50>, dll::trainer<dll::sgd_trainer>>::dbn_t;

    auto net = std::make_unique<dbn_t>();

    net->learning_rate = 0.1;
    net->momentum = 0.9;
    net->initial_momentum = 0.9;

    net->display();

    auto ft_error = net->fine_tune(dataset.training_images, dataset.training_labels, 10);
    std::cout << "ft_error:" << ft_error << std::endl;

    return 0;
}