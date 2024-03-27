// Paterns.cpp : Ten plik zawiera funkcję „main”. W nim rozpoczyna się i kończy wykonywanie programu.
//

#include <iostream>
#include <cstdio>
#include <string>
#include <vector>
#include <fstream>
#include <boost/lexical_cast.hpp>

using namespace std;
using namespace boost;

enum class Color { red, green, blue};
enum class Size { small, medium, large};

struct Product {
	string name_;
	Size size_;
	Color color_;
	Product(const string& name, const Size& size, const Color& color) : name_(name), size_(size), color_(color) {}
};
// Podejście nie zgodne z Open Close Principle
struct ProductFilter {
	vector<Product*> by_color(vector<Product*> items, Color color) {
		vector<Product*> result;
		for (const auto& item : items) {
			if (item->color_ == color) {
				result.push_back(item);
			}
		}
		return result;
	}

	vector<Product*> by_size(vector<Product*> items, Size size) {
		vector<Product*> result;
		for (const auto& item : items) {
			if (item->size_ == size) {
				result.push_back(item);
			}
		}
		return result;
	}

	vector<Product*> by_size_and_color(vector<Product*> items, Size size, Color color) {
		vector<Product*> result;
		for (const auto& item : items) {
			if (item->size_ == size && item->color_ == color) {
				result.push_back(item);
			}
		}
		return result;
	}

};

template <typename T> struct Specfication {
	virtual bool isSatisfied(T* item) = 0;
};

template <typename T> struct Filter {
	virtual vector<T*> filter(vector<T*> items, 
							  Specfication<T>& spec) = 0;
};

struct BetterFilter : Filter<Product> {
	vector<Product*> filter(vector<Product*> items,
		Specfication<Product>& spec) override
	{
		vector<Product*> result;
		for (const auto& item : items) {
			if (spec.isSatisfied(item)) {
				result.push_back(item);
			}
		}
		return result;
	}
};

struct ColorSpecification : Specfication<Product>
{
	Color color_;
	ColorSpecification(Color color) : color_(color){ }

	bool isSatisfied(Product* item) override {
		return item->color_ == color_;
	}

};

struct SizeSpecification : Specfication<Product>
{
	Size size_;
	SizeSpecification(Size color) : size_(color) { }

	bool isSatisfied(Product* item) override {
		return item->size_ == size_;
	}
};

template <typename T> struct AndSpecification : Specfication<T> {
	Specfication<T>& first_;
	Specfication<T>& second_;

	AndSpecification(Specfication<T>& first,
					 Specfication<T>& second) : first_(first), second_(second) {};
	bool isSatisfied(T* item) override {
		return first_.isSatisfied(item) && second_.isSatisfied(item);
	}
};


int main() {
	Product apple("Apple", Size::small, Color::red);
	Product tree("Tree", Size::large, Color::green);
	Product house("House", Size::medium, Color::blue);
	Product car("Car", Size::medium, Color::green);

	vector<Product*> items{ &apple, &tree, &house, &car };

	ProductFilter pf;
	auto greenThings  = pf.by_color(items, Color::green);

	BetterFilter bf;
	ColorSpecification green(Color::green);

	/*for (auto& item : bf.filter(items, green)) {
		cout << item->name_ << " is green\n";
	}*/

	SizeSpecification medium(Size::medium);
	AndSpecification<Product> greenMedium(green, medium);

	for (auto& item : bf.filter(items, greenMedium)) {
		cout << item->name_ << " is green and medium\n";
	}

}



