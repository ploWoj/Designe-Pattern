// Paterns.cpp : Ten plik zawiera funkcję „main”. W nim rozpoczyna się i kończy wykonywanie programu.
//

#include <iostream>
#include <cstdio>
#include <string>
#include <vector>
#include <fstream>
#include <boost/lexical_cast.hpp>
#include <tuple>

using namespace std;
using namespace boost;

// OPEN CLOSE PRINCIPLE

//enum class Color { red, green, blue};
//enum class Size { small, medium, large};
//
//struct Product {
//	string name_;
//	Size size_;
//	Color color_;
//	Product(const string& name, const Size& size, const Color& color) : name_(name), size_(size), color_(color) {}
//};
//// Podejście nie zgodne z Open Close Principle
//struct ProductFilter {
//	vector<Product*> by_color(vector<Product*> items, Color color) {
//		vector<Product*> result;
//		for (const auto& item : items) {
//			if (item->color_ == color) {
//				result.push_back(item);
//			}
//		}
//		return result;
//	}
//
//	vector<Product*> by_size(vector<Product*> items, Size size) {
//		vector<Product*> result;
//		for (const auto& item : items) {
//			if (item->size_ == size) {
//				result.push_back(item);
//			}
//		}
//		return result;
//	}
//
//	vector<Product*> by_size_and_color(vector<Product*> items, Size size, Color color) {
//		vector<Product*> result;
//		for (const auto& item : items) {
//			if (item->size_ == size && item->color_ == color) {
//				result.push_back(item);
//			}
//		}
//		return result;
//	}
//
//};
//
//template <typename T> struct Specfication {
//	virtual bool isSatisfied(T* item) = 0;
//};
//
//template <typename T> struct Filter {
//	virtual vector<T*> filter(vector<T*> items, 
//							  Specfication<T>& spec) = 0;
//};
//
//struct BetterFilter : Filter<Product> {
//	vector<Product*> filter(vector<Product*> items,
//		Specfication<Product>& spec) override
//	{
//		vector<Product*> result;
//		for (const auto& item : items) {
//			if (spec.isSatisfied(item)) {
//				result.push_back(item);
//			}
//		}
//		return result;
//	}
//};
//
//struct ColorSpecification : Specfication<Product>
//{
//	Color color_;
//	ColorSpecification(Color color) : color_(color){ }
//
//	bool isSatisfied(Product* item) override {
//		return item->color_ == color_;
//	}
//
//};
//
//struct SizeSpecification : Specfication<Product>
//{
//	Size size_;
//	SizeSpecification(Size color) : size_(color) { }
//
//	bool isSatisfied(Product* item) override {
//		return item->size_ == size_;
//	}
//};
//
//template <typename T> struct AndSpecification : Specfication<T> {
//	Specfication<T>& first_;
//	Specfication<T>& second_;
//
//	AndSpecification(Specfication<T>& first,
//					 Specfication<T>& second) : first_(first), second_(second) {};
//	bool isSatisfied(T* item) override {
//		return first_.isSatisfied(item) && second_.isSatisfied(item);
//	}
//};
//

// END OF open/close PRINCIPLE

// DEPENDENCY INVERSION RULE

enum class Relationship {
	parent, 
	child,
	sibling
};

struct Person {
	string name_;
};

struct RelationshipBrowser
{
	virtual vector<Person> find_all_childrem_of(const string& name) = 0;
};

struct Relationships : RelationshipBrowser {
	vector<tuple<Person, Relationship, Person>> relations;
	void add_parent_and_child(const Person& parent, const Person& child) {
		relations.push_back({ parent, Relationship::parent, child });
		relations.push_back({ child, Relationship::child, parent });
	}

	vector<Person> find_all_childrem_of(const string& name) {
		vector<Person> result;
		for (auto&& [first, rel, second] : relations) {
			if (first.name_ == name && rel == Relationship::parent) {
				result.push_back(second);
			}
		}
		return result;
 	}
};

struct Research {
	Research(RelationshipBrowser& browser,const string& name) {
		for (auto& child : browser.find_all_childrem_of(name)) {
			cout << "John has child called " << child.name_ << '\n';
		}
	}
};

int main() {

	Person parent{ "John" };
	Person child1{ "Chris" }, child2{ "Matt" };

	Relationships family;
	family.add_parent_and_child(parent, child1);
	family.add_parent_and_child(parent, child2);

	Research _(family, parent.name_);

}



