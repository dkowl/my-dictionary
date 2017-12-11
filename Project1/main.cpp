#include <iostream>
#include <vector>
#include <random>
#include <fstream>
#include "Clock.h"
#include "BinarySearchTree.h"

using namespace std;
using namespace MyDictionary;
using namespace MyUtils;


typedef int Key;
struct Value {
	static const int DATA_SIZE = 10;
	char data[DATA_SIZE];

	Value(int x) {
		int i = 0;
		int powerOf10 = 1000000000;
		if (x < 0) {
			data[0] = '-';
			x *= -1;
			i++;
			powerOf10 /= 10;
		}
		for (; i < DATA_SIZE && powerOf10 > 0; i++, powerOf10 /= 10) {
			data[i] = x / powerOf10;
			x -= powerOf10 * data[i];
			data[i] += '0';
		}
	}

	void Print() const {
		for (auto&& c : data) {
			cout << c;
		}
		cout << " ";
	}
};

typedef BinarySearchTree<Key, Value> Tree;

void InsertRandom(Dictionary<Key, Value> &dict, int amount, default_random_engine &randomEngine) {
	const int MIN_RANGE = -10000, MAX_RANGE = 10000;
	const uniform_int_distribution<int> dist(MIN_RANGE, MAX_RANGE);

	while (amount > 0) {
		int r = dist(randomEngine);
		if (dict.Search(r) == nullptr) {
			dict.Insert(r, Value(r));
			amount--;
		}
	}
}

void SearchRandom(Dictionary<Key, Value> &dict, int amount, default_random_engine &randomEngine) {
	const int MIN_RANGE = -10000, MAX_RANGE = 10000;
	const uniform_int_distribution<int> dist(MIN_RANGE, MAX_RANGE);

	while (amount > 0) {
		int r = dist(randomEngine);
		dict.Contains(r);
		amount--;
	}
}

void PrintValue(Value const * value) {
	value->Print();
}

void PrintValues(Tree &tree, Tree::TraversalType type) {
	tree.Traverse(type, PrintValue);
	cout << tree.Size() << endl;
}

int main() {

	default_random_engine randomEngine;

	fstream file("inlab03.txt");
	int x, k1, k2, k3, k4;
	if (file.good()) {
		file >> x >> k1 >> k2 >> k3 >> k4;
		file.close();
	}
	else {
		cout << "File not found" << endl;
		system("PAUSE");
		return 1;
	}

	Clock::Start();

	Tree tree = Tree();
	tree.Delete(k1);
	tree.Insert(k1, Value(k1));
	InsertRandom(tree, x, randomEngine);
	PrintValues(tree, Tree::TraversalType::InOrder);
	PrintValues(tree, Tree::TraversalType::PreOrder);
	tree.Insert(k2, Value(k2));
	PrintValues(tree, Tree::TraversalType::InOrder);
	tree.Insert(k3, Value(k3));
	tree.Insert(k4, Value(k4));
	tree.Delete(k1);
	PrintValues(tree, Tree::TraversalType::PreOrder);
	tree.Search(k1);
	tree.Delete(k2);
	PrintValues(tree, Tree::TraversalType::InOrder);
	tree.Delete(k3);
	tree.Delete(k4);

	Clock::End();

	/* speed test

	BinarySearchTree<Key, Value> dict = BinarySearchTree<Key, Value>();

	int testNo = 10;
	cout << "Inserting " << testNo << " elements\n";
	Clock::Start();
	InsertRandom(dict, testNo, randomEngine);
	Clock::End();
	cout << "Searching " << testNo << " elements\n";
	Clock::Start();
	SearchRandom(dict, testNo, randomEngine);
	Clock::End();

	int deleteTestCount = 5;
	for (int i = 0; i < deleteTestCount; i++) {
		PrintValues(dict);
		Key toDelete;
		cout << "Key to delete: ";
		cin >> toDelete;
		dict.Delete(toDelete);
	}*/

	system("PAUSE");
	return 0;
}