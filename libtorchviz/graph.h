#pragma once
#include <string>
#include <set>
#include <map>
#include <memory>

namespace libtorchviz {

	class Graph : public std::enable_shared_from_this<Graph> {
	public:
		Graph( const std::string name_, const std::set<std::shared_ptr<Graph>> child_ ) :
			name( name_ ),
			child( child_ ),
			id( create_id() ) {}

		const std::string output( const bool show_attr, std::map<size_t, std::shared_ptr<Graph>>& created = std::map<size_t, std::shared_ptr<Graph>>() );

		const std::string name;
		const size_t id;

	protected:
		const std::set<std::shared_ptr<Graph>> child;

		virtual const std::string header( const bool show_atte ) const = 0;
		virtual const std::string futter( const bool show_atte ) const = 0;

	private:
		static const size_t create_id() {
			static size_t used = 1;
			return used++;
		}
	};

	class Node : public Graph {
	public:
		Node( const std::string name, const std::set<std::shared_ptr<Graph>> child, const std::map<std::string, std::string> param_ = {} ) :
			Graph( name, child ),
			param( param_ ) {}

	protected:
		virtual const std::string header( const bool show_atte ) const override;
		virtual const std::string futter( const bool show_atte ) const override;

	private:
		const std::map<std::string, std::string> param;
	};

	class Variable : public Graph {
	public:
		Variable( const std::string name ) :
			Graph( name, {} ) {}

	protected:
		virtual const std::string header( const bool show_atte ) const override;
		virtual const std::string futter( const bool show_atte ) const override;
	};

	class Root : public Graph {
	public:
		Root( const std::string name, const std::set<std::shared_ptr<Graph>> child ) :
			Graph( name, child ) {}

	protected:
		virtual const std::string header( const bool show_atte ) const override;
		virtual const std::string futter( const bool show_atte ) const override;
	};
}