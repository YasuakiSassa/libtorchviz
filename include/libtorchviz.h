#pragma once
#include <torch/torch.h>
#include <filesystem>

namespace libtorchviz {
	const std::filesystem::path make_dot( torch::Tensor target, torch::OrderedDict<std::string, torch::Tensor> params = torch::OrderedDict<std::string, torch::Tensor>(), std::filesystem::path path = {} );

	const std::filesystem::path make_image( const std::filesystem::path dot_path, const std::string format = "svg", std::filesystem::path image_path = {}, const bool delete_dot = true );
}