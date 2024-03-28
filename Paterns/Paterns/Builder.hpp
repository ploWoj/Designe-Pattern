#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <memory>

using namespace std;

struct HtmlBuilder;

struct HtmlElement
{
	string name_;
	string text_;
	vector<HtmlElement> elements;
	const size_t indent_size = 2;

	HtmlElement() {}
	HtmlElement(const string& name, const string& text) 
		: name_(name), text_(text) {}
	
	string str(int indent = 0) const {
		ostringstream oss;
		string i(indent_size * indent, ' ');
		oss << i << "<" << name_ << ">" << '\n';
		if (text_.size() > 0) {
			oss << string(indent_size * (indent + 1), ' ') << text_ << '\n';
		}
		for (const auto& e : elements) {
			oss << e.str(indent + 1);
		}

		oss << i << "</" << name_ << ">" << '\n';
		return oss.str();
	}


	static unique_ptr<HtmlBuilder> build(string root_name) {
		return make_unique<HtmlBuilder>(root_name);
	}
};

struct HtmlBuilder
{
	HtmlBuilder(string root_name) {
		root.name_ = root_name;
	}

	HtmlBuilder& add_child(string child_name, string child_text) {
		HtmlElement e{ child_name, child_text };
		root.elements.emplace_back(e);
		return *this;
	}

	// pointer based
	HtmlBuilder* add_child_2(string child_name, string child_text) {
		HtmlElement e{ child_name, child_text };
		root.elements.emplace_back(e);
		return this;
	}

	string str() const { return root.str(); }

	operator HtmlElement() const { return root; }
	HtmlElement root;
};