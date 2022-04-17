#include <torch/torch.h>
#include <iostream>
#include <libtorchviz.h>

int main() {
	auto t1 = torch::tensor( { 3 }, torch::TensorOptions().requires_grad( true ).dtype( torch::ScalarType::Float ) );
	auto t2 = t1.pow( 2 ).mul( 2 ).add( 3 ).sin().sigmoid();
	auto dot = libtorchviz::make_dot( t2, { { "t1", t1 } } );
	auto image = libtorchviz::make_image( dot );
}