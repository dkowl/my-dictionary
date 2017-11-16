#include <iostream>
#include <vector>
#include <random>
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
		cout << endl;
	}
};

void InsertRandom(Dictionary<Key, Value> &dict, int amount, default_random_engine &randomEngine) {
	const int MIN_RANGE = -100000000, MAX_RANGE = 100000000;
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
	const int MIN_RANGE = -100000000, MAX_RANGE = 100000000;
	const uniform_int_distribution<int> dist(MIN_RANGE, MAX_RANGE);

	while (amount > 0) {
		int r = dist(randomEngine);
		dict.Contains(r);
		amount--;
	}
}

void PrintValues(Dictionary<Key, Value> &dict) {
	vector<Dictionary<Key, Value>::ReturnType> values = dict.GetValues();
	for (auto&& value : values) {
		value->Print();
	}
}

int main() {

	default_random_engine randomEngine;

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
	}

	system("PAUSE");
	return 0;
}