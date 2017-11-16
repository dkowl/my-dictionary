#include <iostream>
#include <vector>
#include <random>
#include "Clock.h"

using namespace std;
using namespace MyUtils;

static const int DATA_SIZE = 10;

class Node {
public:
	enum TraversalType {
		IN_ORDER,
		PRE_ORDER,
		POST_ORDER
	};

	int key_;
	Node *left_, *right_;
	char data_[DATA_SIZE];

	Node() :
		Node(0)
	{

	}

	Node(int key) :
		key_(key),
		left_(nullptr),
		right_(nullptr)
	{
		int i = 0;
		if (key < 0) {
			data_[0] = '-';
			key *= -1;
			i++;
		}
		int powerOf10 = 1000000000;
		for (; i < DATA_SIZE && powerOf10 > 0; i++, powerOf10 /= 10) {
			data_[i] = key / powerOf10;
			key -= powerOf10 * data_[i];
			data_[i] += '0';
		}
	}

	Node* search(int key) {
		Node* node = this;
		while (true) {
			if (node == nullptr)
				return nullptr;
			if (node->key_ == key)
				return node;

			if (node->key_ < key)
				node = node->right_;
			else
				node = node->left_;
		}
	}

	bool insert(Node *node) {
		Node* currentNode = this;

		while (true) {
			if (node->key_ == currentNode->key_)
				return false;
			else if (node->key_ < currentNode->key_) {
				if (currentNode->left_ == nullptr) {
					currentNode->left_ = node;
					return true;
				}
				else
					currentNode = currentNode->left_;
			}
			else {
				if (currentNode->right_ == nullptr) {
					currentNode->right_ = node;
					return true;
				}
				else
					currentNode = currentNode->right_;
			}
		}
	}

	bool insert(int key) {
		Node* newNode = new Node(key);
		return insert(newNode);
	}

	void insertRandom(int amount, default_random_engine &randomEngine) {
		const int MIN_RANGE = -100000000, MAX_RANGE = 100000000;
		const uniform_int_distribution<int> dist(MIN_RANGE, MAX_RANGE);

		while (amount > 0) {
			if (insert(dist(randomEngine)))
				amount--;
		}
	}

	void traverse(TraversalType type, int &count, bool printCount = true) {

		count++;

		if (type == PRE_ORDER) printData();
		if (left_ != nullptr) left_->traverse(type, count, false);
		if (type == IN_ORDER) printData();
		if (right_ != nullptr) right_->traverse(type, count, false);
		if (type == POST_ORDER)	printData();

		if (printCount) {
			cout << "Node count: " << count << endl;
		}
	}

	void printData() {
		for (int i = 0; i < DATA_SIZE; i++) {
			cout << data_[i];
		}
		cout << endl;
	}
};

void SearchRandom(Node &dict, int amount, default_random_engine &randomEngine) {
	const int MIN_RANGE = -100000000, MAX_RANGE = 100000000;
	const uniform_int_distribution<int> dist(MIN_RANGE, MAX_RANGE);

	while (amount > 0) {
		Node* dupa = dict.search(dist(randomEngine));
		amount--;
	}
}

int main() {

	default_random_engine randomEngine;

	Node bst(10);

	int testNo = 1000000;
	cout << "Inserting " << testNo << " elements\n";
	Clock::Start();
	bst.insertRandom(1000000, randomEngine);
	Clock::End();
	cout << "Searching " << testNo << " elements\n";
	Clock::Start();
	SearchRandom(bst, testNo, randomEngine);
	Clock::End();

	system("PAUSE");
	return 0;
}