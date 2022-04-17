#include <libtorchviz.h>
#include <cstdlib>
#include <ctime>
#include <set>
#include <fstream>
#include <torch/csrc/autograd/functions/accumulate_grad.h>
#include "graph.h"

using libtorchviz::Graph;
using libtorchviz::Node;
using libtorchviz::Root;
using libtorchviz::Variable;


#ifdef _WIN32
#include <windows.h>
#include <shellapi.h>
#define ShellExecute_( filename ) ShellExecuteA( nullptr, "open", filename, nullptr, nullptr, SW_SHOWNORMAL )
#else
#define ShellExecute_( filename )
#endif

static std::map<std::string, std::string> to_map( torch::OrderedDict<std::string, torch::Tensor> params ) {
	std::map<std::string, std::string> ret;
	for( auto x : params ) ret.insert( { x.key(), "" } );
	return ret;
}

static std::string to_string( torch::IntArrayRef arr ) {
	if( arr.size() == 0 ) return "";

	std::string ret = "( " + std::to_string( *arr.begin() );
	for( auto it = arr.begin() + 1; it < arr.end(); it++ ) ret += ", " + std::to_string( *it );
	return ret + " )";
}

static std::shared_ptr<Graph> make_graph( std::shared_ptr<torch::autograd::Node> node, torch::OrderedDict<std::string, torch::Tensor> params, std::map<std::shared_ptr<torch::autograd::Node>, std::shared_ptr<Graph>> created = {} ) {
	TORCH_CHECK( node, "Error argument node" );

	auto find = created.find( node );
	if( find != created.end() ) return find->second;

	std::set<std::shared_ptr<Graph>> child;
	std::string name;
	if( node->name() == "struct torch::autograd::AccumulateGrad" ) {
		name = "AccumulateGrad";
		auto acc = std::reinterpret_pointer_cast<torch::autograd::AccumulateGrad>( node );
		auto find = std::find_if( params.begin(), params.end(), [acc]( auto x ) { return x->unsafeGetTensorImpl() == acc->variable.unsafeGetTensorImpl(); } );
		if( find != params.end() ) child.insert( std::make_shared<Variable>( find->key() + "\n" + to_string( find->value().sizes() ) ) );
	} else {
		name = node->name();
		for( auto edge : node->next_edges() ) child.insert( ::make_graph( edge.function, params, created ) );
	}

	auto graph = std::make_shared<Node>( name , child, to_map( params ) );
	created.insert( { node, graph } );

	return graph;
}

const std::filesystem::path libtorchviz::make_dot( torch::Tensor target, torch::OrderedDict<std::string, torch::Tensor> params, std::filesystem::path path ) {
	if( path.empty() ) path = std::filesystem::path( std::getenv( "temp" ) ).append( std::to_string( std::time( nullptr ) ) ).replace_extension( "dot" );

	auto graph = std::make_shared<Root>( to_string( target.sizes() ), std::set<std::shared_ptr<Graph>>( { ::make_graph( target.grad_fn(), params ) } ) );
	std::ofstream output( path );
	output << graph->output( false );
	output.close();

	return path;
}

const std::filesystem::path libtorchviz::make_image( const std::filesystem::path dot_path, const std::string format, std::filesystem::path image_path, const bool delete_dot ) {
	if( image_path.empty() ) {
		image_path = dot_path;
		image_path.replace_extension( format );
	}

	auto command = "dot -Kdot -T" + format + " -o" + image_path.string() + " " + dot_path.string();
	if( std::system( command.c_str() ) == 0 ) {
		if( delete_dot ) std::filesystem::remove( dot_path );
		ShellExecute_( image_path.string().c_str() );
	}

	return image_path;
}

