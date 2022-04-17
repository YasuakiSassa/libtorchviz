#include "graph.h"

#define s( v ) ( std::string( "\"") + v  + std::string( "\"") )

static const std::string create_setting( std::string title, std::map<std::string, std::string> map ) {
	if( map.empty() ) return title + ";\n";

	std::string ret = title + " [\n";
	for( auto it = map.begin(); it != map.end(); it++ ) ret += it->first + " = " + it->second + ";\n";
	return ret + "];\n";
}

const std::string libtorchviz::Graph::output( const bool show_attr, std::map<size_t, std::shared_ptr<Graph>>& created ) {
	if( created.find( id ) != created.end() ) return "";
	created.insert( { id, shared_from_this() } );

	auto ret = header( show_attr );
	for( auto x : child ) ret += x->output( show_attr, created);
	ret += futter( show_attr );
	return ret;
}

const std::string libtorchviz::Node::header( const bool show_atte ) const {
	std::map<std::string, std::string> node_attr = {
		{ "label", s( name ) },
		{ "fillcolor", "lightgray" }
	};

	return ::create_setting( std::to_string( id ), node_attr );
}

const std::string libtorchviz::Node::futter( const bool show_atte ) const {
	std::string ret;
	for( auto x : child ) ret += std::to_string( x->id ) + " -> " + std::to_string( id ) + ";\n";
	return ret;
}

const std::string libtorchviz::Variable::header( const bool show_atte ) const {
	std::map<std::string, std::string> node_attr = {
		{ "label", s( name ) },
		{ "fillcolor", "lightblue" }
	};

	return ::create_setting( std::to_string( id ), node_attr );
}

const std::string libtorchviz::Variable::futter( const bool show_atte ) const {
	return std::string();
}

const std::string libtorchviz::Root::header( const bool show_atte ) const {
	std::map<std::string, std::string> graph_setting = {
		{ "charaset", s( "UTF-8" ) },
		{ "bgcolor", "white" }
	};

	std::map<std::string, std::string> node_setting = {
		{ "style", "filled" },
		{ "shape", "box" },
		{ "align", "left" },
		{ "fontsize", "10" },
		{ "ranksep", "0.1" },
		{ "height", "0.2" },
		{ "fontname", s( "monospace" ) }
	};

	std::map<std::string, std::string> edge_setting = {
		{ "style", "solid" }
	};

	std::map<std::string, std::string> node_attr = {
		{ "label", s( name ) },
		{ "fillcolor", "lightgreen" }
	};

	return "digraph librotchviz {\n" +
		::create_setting( "graph", graph_setting ) +
		::create_setting( "node", node_setting ) +
		::create_setting( "edge", edge_setting ) +
		::create_setting( std::to_string( id ), node_attr );
}

const std::string libtorchviz::Root::futter( const bool show_atte ) const {
	std::string ret;
	for( auto x : child ) ret += std::to_string( x->id ) + " -> " + std::to_string( id ) + ";\n";
	return ret + "}";
}
