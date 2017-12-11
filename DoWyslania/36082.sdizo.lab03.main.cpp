//SDIZO I1 XXXY LAB03
//Dominik Kowalczyk
//kd36082@zut.edu.pl

//Program sklada sie z wielu plikow ktore "includuje recznie" ponizej

//#include "Clock.h"

#include <chrono>
#include <string>
#include <iostream>

using namespace std;
using namespace std::chrono;

namespace MyUtils {

	class Clock {

	public:
		static void Start(string s = "");
		static void End();

	private:
		static steady_clock::time_point t1, t2;
		static duration<double> timeSpan;
		static bool clockStarted;
	};
}

//#include "Dictionary.h"

#include <vector>

namespace MyDictionary {

	template <typename TKey, typename TValue>
	class Dictionary {

	public:
		typedef TValue const * ReturnType;

		virtual void Insert(TKey key, TValue value) = 0;
		virtual void Delete(TKey key) = 0;
		virtual bool Contains(TKey key) = 0;
		virtual ReturnType Search(TKey key) = 0;
		virtual std::vector<ReturnType> GetValues() = 0;
	};
}

//#include "BinarySearchTree.h"

#include <stack>

namespace MyDictionary {

	template<typename TKey, typename TValue>
	class BinarySearchTree : public Dictionary<TKey, TValue> {

	public:

		enum TraversalType {
			InOrder,
			PreOrder,
			PostOrder
		}; 
		
		static void AddValue(ReturnType value) {
			values.push_back(value);
		}

		BinarySearchTree() {
			root = nullptr;
			size = 0;
		}

		~BinarySearchTree() {
			delete root;
		}

		bool Contains(TKey key) {
			return GetNode(key) != nullptr;
		}

		ReturnType Search(TKey key) {
			Node *node = GetNode(key);
			if (node == nullptr)
				return nullptr;
			else
				return &(node->value);
		}	

		void Insert(TKey key, TValue value) {
			size++;
			if (root == nullptr) {
				root = new Node(key, value);
				return;
			}
			Node* currentNode = root;
			while (true) {
				if (key == currentNode->key)
					currentNode->value = value;
				else if (key < currentNode->key) {
					if (currentNode->left == nullptr) {
						currentNode->left = new Node(key, value);
						return;
					}
					currentNode = currentNode->left;
				}
				else {
					if (currentNode->right == nullptr) {
						currentNode->right = new Node(key, value);
						return;
					}
					currentNode = currentNode->right;
				}
			}
		}

		void Delete(TKey key) {
			Delete(GetNode(key));
		}

		std::vector<ReturnType> GetValues() {
			values.clear();
			Traverse(InOrder, AddValue);
			std::vector<ReturnType> result(std::move(values));
			return result;
		}

		void Traverse(TraversalType type, void(*callback)(ReturnType)) {
			traversalType = type;
			traversalCallback = callback;
			Traverse(root);
		}

		int Size() {
			return size;
		}


	private:

		class Node {
		public:
			TKey key;
			TValue value;
			Node *left;
			Node *right;

			Node(TKey key, TValue value) :
				key(key),
				value(value),
				left(nullptr),
				right(nullptr)
			{

			}

			~Node() {
				delete left;
				delete right;
			}

			int ChildCount() {
				int result = 0;
				if (left != nullptr) result++;
				if (right != nullptr) result++;
				return result;
			}

			Node* GetOneChild() {
				if (left != nullptr) return left;
				else return right;
			}
		};

		static std::vector<ReturnType> values;
		Node *root;
		int size;
		TraversalType traversalType;
		void(*traversalCallback)(ReturnType);

		void Delete(Node* node) {
			if (node == nullptr)
				return;
			if (node->ChildCount() == 0) {
				if (node == root) {
					delete node;
					size--;
					root = nullptr;
					return;
				}
				Node *parent = GetParent(node);
				if (parent->right == node) parent->right = nullptr;
				else parent->left = nullptr;
				delete node;
				size--;
			}
			else if (node->ChildCount() == 1) {
				Node *child = node->GetOneChild();
				if (node == root) {
					root = child;
				}
				else {
					Node *parent = GetParent(node);
					if (parent->right == node) parent->right = child;
					else parent->left = child;
				}
				node->left = nullptr;
				node->right = nullptr;
				delete node;
				size--;
			}
			else {
				Node *prev = GetPredecessor(node);
				TKey newKey = prev->key;
				TValue newValue = prev->value;
				Delete(prev);
				node->key = newKey;
				node->value = newValue;
			}
		}

		Node* GetNode(TKey key) {
			Node *currentNode = root;
			while (true) {
				if (currentNode == nullptr)
					return nullptr;

				if (key == currentNode->key)
					return currentNode;
				else if (key < currentNode->key)
					currentNode = currentNode->left;
				else
					currentNode = currentNode->right;
			}
		}

		Node* GetParent(Node* node) {
			Node *parent = nullptr;
			Node *currentNode = root;
			while (true) {
				if (currentNode == nullptr)
					return nullptr;

				if (node == currentNode)
					return parent;
				else if (node->key < currentNode->key) {
					parent = currentNode;
					currentNode = currentNode->left;
				}
				else {
					parent = currentNode;
					currentNode = currentNode->right;
				}
			}
		}

		Node* GetMin(Node *node) {
			while (node->left != nullptr)
				node = node->left;
			return node;
		}

		Node* GetMax(Node *node) {
			while (node->right != nullptr)
				node = node->right;
			return node;
		}

		Node* GetPredecessor(Node* node) {
			if (node->left != nullptr)
				return GetMax(node->left);
			else
				return nullptr;
		}

		Node* GetSuccessor(Node* node) {
			if (node->right != nullptr)
				return GetMin(node->right);
			else
				return nullptr;
		}

		void Traverse(Node *node) {
			if (traversalType == PreOrder) traversalCallback(&node->value);
			if (node->left != nullptr) Traverse(node->left);
			if (traversalType == InOrder) traversalCallback(&node->value);
			if (node->right != nullptr) Traverse(node->right);
			if (traversalType == PostOrder) traversalCallback(&node->value);
		}

	};

	template<typename TKey, typename TValue>
	std::vector<typename BinarySearchTree<TKey, TValue>::ReturnType> BinarySearchTree<TKey, TValue>::values;
}

//#include "Clock.h"

namespace MyUtils {

	void Clock::Start(string s) {
		if (!clockStarted) {
			if (s.size()) cout << s << endl;
			t1 = steady_clock::now();
			clockStarted = true;
		}
	}

	void Clock::End() {
		t2 = steady_clock::now();
		timeSpan = duration_cast<duration<double>>(t2 - t1);
		cout << timeSpan.count() << " sekund" << endl;
		clockStarted = false;
	}

	steady_clock::time_point Clock::t1, Clock::t2;
	duration<double> Clock::timeSpan;
	bool Clock::clockStarted;
}

//main.cpp

#include <iostream>
#include <vector>
#include <random>

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
	cout << tree.Size() <<endl;
}

int main() {

	default_random_engine randomEngine;

	int x, k1, k2, k3, k4;
	cin >> x >> k1 >> k2 >> k3 >> k4;

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

	system("PAUSE");
	return 0;
}

