#include <iostream>
#include <vector>
#include <random>
#include <fstream>
#include "Clock.h"
#include "AvlTree.h"
#include "LinkedList.h"

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

typedef AvlTree<Key, Value> Dict;

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

void PrintValues(AvlTree<Key, Value> &tree, AvlTree<Key, Value>::TraversalType type) {
	tree.TraverseAndShowHeight(type, PrintValue);
	cout << tree.Size() << endl;
}

struct Nothing {

};

void TestDictionary(Dictionary<int, Nothing> &dict, int n, bool isRandom, default_random_engine &randomEngine, int seed) {
	const int MIN_RANGE = 0, MAX_RANGE = 100000;
	const uniform_int_distribution<int> dist(MIN_RANGE, MAX_RANGE);
	randomEngine.seed(seed);

	cout << "==========================================\n";
	cout << "Testing... || n = " << n << " || ";
	if (isRandom) cout << "random values ";
	else cout << "ordered values ";
	cout << "\n\n";

	//Inserting
	Clock::Start();
	for (int i = 0; i < n; i++) {
		int key;
		if (i % 2 && !isRandom)
			key = i / 2;
		else
			key = dist(randomEngine);
		dict.Insert(key, Nothing());
	}
	cout << "Elements inserted: " << dict.Size() << endl;
	Clock::End();
	cout << endl;

	//Searching
	int count = 0;
	Clock::Start();
	for (int i = 0; i < n; i++) {
		int key;
		if (i % 2 && !isRandom)
			key = i / 2;
		else
			key = dist(randomEngine);
		if(dict.Contains(key)) count++;
	}
	cout << "Elements found: " << count << endl;
	Clock::End();
	cout << endl;

	//Deleting
	int sizeBefore = dict.Size();
	Clock::Start();
	for (int i = 0; i < n; i++) {
		int key;
		if (i % 2 && !isRandom)
			key = i / 2;
		else
			key = dist(randomEngine);
		dict.Delete(key);
	}
	cout << "Elements deleted: " << sizeBefore - dict.Size() << endl;
	Clock::End();
	cout << endl << endl << endl;
}

int main() {

	default_random_engine randomEngine;

	int testCount = 5;
	int n[] = { 10000, 15000, 20000, 25000, 30000 };
	int seed[] = { 12710, 108542, 387666, 764929, 873881 };

	int i = 0;
	int isRandom = true;
	while (i < testCount) {
		cout << "BinarySearchTree " << endl;
		TestDictionary(BinarySearchTree<int, Nothing>(), n[i], isRandom, randomEngine, seed[i]);
		cout << "AvlTree " << endl;
		TestDictionary(AvlTree<int, Nothing>(), n[i], isRandom, randomEngine, seed[i]);
		cout << "LinkedList " << endl;
		TestDictionary(LinkedList<int, Nothing>(), n[i], isRandom, randomEngine, seed[i]);

		if (!isRandom) i++;
		isRandom = !isRandom;
	}

	/*fstream file("inlab04.txt");
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

	Dict dict = Dict();
	dict.Delete(k1);
	dict.Insert(k1, Value(k1));
	InsertRandom(dict, x, randomEngine);
	PrintValues(dict, Dict::TraversalType::InOrder);
	PrintValues(dict, Dict::TraversalType::PreOrder);
	dict.Insert(k2, Value(k2));
	PrintValues(dict, Dict::TraversalType::InOrder);
	dict.Insert(k3, Value(k3));
	dict.Insert(k4, Value(k4));
	dict.Delete(k1);
	PrintValues(dict, Dict::TraversalType::PreOrder);
	dict.Search(k1);
	dict.Delete(k2);
	PrintValues(dict, Dict::TraversalType::InOrder);
	dict.Delete(k3);
	dict.Delete(k4);

	Clock::End();*/

	system("PAUSE");
	return 0;
}