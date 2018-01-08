//SDIZO IS1 214B LAB04
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
		virtual void Display() = 0;
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

		virtual void Insert(TKey key, TValue value) {
			if (root == nullptr) {
				root = Allocate(key, value);
				return;
			}
			Node* currentNode = root;
			while (true) {
				if (key == currentNode->key)
					currentNode->value = value;
				else if (key < currentNode->key) {
					if (currentNode->left == nullptr) {
						currentNode->left = Allocate(key, value);
						Retrace(currentNode);
						return;
					}
					currentNode = currentNode->left;
				}
				else {
					if (currentNode->right == nullptr) {
						currentNode->right = Allocate(key, value);
						Retrace(currentNode);
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

		virtual void Display() {
			queue<Node*> nodeQueue;
			queue<int> levelQueue;
			int previousLevel = 0;
			nodeQueue.push(root);
			levelQueue.push(0);
			while (!nodeQueue.empty()) {
				Node* currentNode = nodeQueue.front();
				int currentLevel = levelQueue.front();
				nodeQueue.pop();
				levelQueue.pop();
				if (currentLevel > previousLevel) {
					cout << endl;
				}
				if (currentNode == nullptr) {
					cout << "null ";
				}
				else {
					cout << currentNode->key << " ";
					nodeQueue.push(currentNode->left);
					levelQueue.push(currentLevel + 1);
					nodeQueue.push(currentNode->right);
					levelQueue.push(currentLevel + 1);
				}
				previousLevel = currentLevel;
			}
			cout << endl << endl;
		}


	protected:

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
					Retrace(parent);
				}
				node->left = nullptr;
				node->right = nullptr;
			}
			else {
				Node *prev = GetPredecessor(node);

				//removing prev from prevParent
				Node *prevParent = GetParent(prev);
				Node *prevChild = prev->GetOneChild();
				if (prevParent->left == prev)
					prevParent->left = prevChild;
				else
					prevParent->right = prevChild;

				//replacing node with prev
				if (node == root) {
					root = prev;
				}
				else {
					Node *parent = GetParent(node);
					if (parent->left == node)
						parent->left = prev;
					else
						parent->right = prev;
				}

				//switching children
				prev->left = node->left;
				prev->right = node->right;
				node->left = nullptr;
				node->right = nullptr;

				Retrace(prevParent);
			}
			delete node;
			size--;
		}

		virtual Node* Allocate(TKey key, TValue value) {
			size++;
			return new Node(key, value);
		}

		virtual void Retrace(Node *node) {

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

//#include "AvlTree.h"
#include <algorithm>
#include <queue>

namespace MyDictionary {

	template<typename TKey, typename TValue>
	class AvlTree : public BinarySearchTree<TKey, TValue> {

	public:

		void TraverseAndShowHeight(TraversalType type, void(*callback)(ReturnType)) {
			traversalType = type;
			traversalCallback = callback;
			TraverseAndShowHeight((AvlNode*)root);
		}

		virtual void Display() {
			queue<AvlNode*> nodeQueue;
			queue<int> levelQueue;
			int previousLevel = 0;
			nodeQueue.push((AvlNode*)root);
			levelQueue.push(0);
			while (!nodeQueue.empty()) {
				AvlNode* currentNode = nodeQueue.front();
				int currentLevel = levelQueue.front();
				nodeQueue.pop();
				levelQueue.pop();
				if (currentLevel > previousLevel) {
					cout << endl;
				}
				if (currentNode == nullptr) {
					cout << "null ";
				}
				else {
					cout << currentNode->key << "(" << currentNode->height << ")[" << currentNode->BalanceFactor() << "] ";
					nodeQueue.push((AvlNode*)currentNode->left);
					levelQueue.push(currentLevel + 1);
					nodeQueue.push((AvlNode*)currentNode->right);
					levelQueue.push(currentLevel + 1);
				}
				previousLevel = currentLevel;
			}
			cout << endl << endl;
		}

	private:

		class AvlNode : public Node {
		public:
			int height;

			AvlNode(TKey key, TValue value) :
				Node(key, value),
				height(1)
			{

			}

			void UpdateHeight() {
				height = Height();
			}

			int BalanceFactor() {
				int
					lf = 0,
					rf = 0;

				if (left != nullptr)
					lf = ((AvlNode*)left)->height;
				if (right != nullptr)
					rf = ((AvlNode*)right)->height;

				return lf - rf;
			}

			int Height() {
				int
					lh = 0,
					rh = 0;

				if (left != nullptr)
					lh = ((AvlNode*)left)->height;
				if (right != nullptr)
					rh = ((AvlNode*)right)->height;

				return std::max(lh, rh) + 1;
			}

			void DisplayHeight() {
				cout << " (" << height << ") ";
			}
		};

		virtual Node* Allocate(TKey key, TValue value) {
			size++;
			return new AvlNode(key, value);
		}

		void Retrace(Node *node) {
			//building parent stack
			std::stack<AvlNode*> parentStack;
			AvlNode* currentNode = (AvlNode*)root;
			while (true) {
				parentStack.push(currentNode);

				if (currentNode == node)
					break;

				if (node->key < currentNode->key) {
					if (currentNode->left != nullptr)
						currentNode = (AvlNode*)currentNode->left;
					else
						break;
				}
				else {
					if (currentNode->right != nullptr)
						currentNode = (AvlNode*)currentNode->right;
					else
						break;
				}
			}

			currentNode->UpdateHeight();
			while (!parentStack.empty()) {
				AvlNode *parent = parentStack.top();
				parent->UpdateHeight();
				parentStack.pop();
				AvlNode *grandparent = nullptr;
				if (!parentStack.empty()) {
					grandparent = parentStack.top();
					grandparent->UpdateHeight();
				}

				if (std::abs(parent->BalanceFactor()) > 1) {
					bool firstLeft, secondLeft;
					firstLeft = parent->left == currentNode;
					secondLeft = currentNode->BalanceFactor() > 0;
					if (firstLeft == secondLeft) {
						Rotate(currentNode, parent, grandparent);
					}
					else if (firstLeft && !secondLeft) {
						Rotate((AvlNode*)currentNode->right, currentNode, parent);
						Rotate((AvlNode*)parent->left, parent, grandparent);
					}
					else if (!firstLeft && secondLeft) {
						Rotate((AvlNode*)currentNode->left, currentNode, parent);
						Rotate((AvlNode*)parent->right, parent, grandparent);
					}
				}

				currentNode = parent;
			}
		}

		void Rotate(AvlNode *node, AvlNode *parent, AvlNode *grandparent) {
			/*cout << "Before: " << node->BalanceFactor() << " " << parent->BalanceFactor() << " ";
			if (grandparent != nullptr) {
				cout << grandparent->BalanceFactor();
			}
			else {
				cout << " - ";
			}
			cout << endl;*/
			//Display();
			if (grandparent == nullptr) {
				root = node;
			}
			else if (grandparent->left == parent) {
				grandparent->left = node;
			}
			else {
				grandparent->right = node;
			}

			if (parent->left == node) {
				parent->left = node->right;
				node->right = parent;
			}
			else {
				parent->right = node->left;
				node->left = parent;
			}
			parent->UpdateHeight();
			node->UpdateHeight();
			if(grandparent != nullptr) grandparent->UpdateHeight();
			/*cout << "After: " << node->BalanceFactor() << " " << parent->BalanceFactor() << " ";
			if (grandparent != nullptr) {
				cout << grandparent->BalanceFactor();
			}
			else {
				cout << " - ";
			}
			cout << endl;*/
		}

		void TraverseAndShowHeight(AvlNode *node) {
			if (traversalType == PreOrder) {
				traversalCallback(&node->value);
				node->DisplayHeight();
			}
			if (node->left != nullptr) TraverseAndShowHeight((AvlNode*)(node->left));
			if (traversalType == InOrder) {
				traversalCallback(&node->value);
				node->DisplayHeight();
			}
			if (node->right != nullptr) TraverseAndShowHeight((AvlNode*)(node->right));
			if (traversalType == PostOrder) {
				traversalCallback(&node->value);
				node->DisplayHeight();
			}
		}
	};
}

//#include "Clock.cpp"
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
#include <fstream>

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

typedef AvlTree<Key, Value> Tree;

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
	tree.TraverseAndShowHeight(type, PrintValue);
	cout << tree.Size() << endl;
}

int main() {

	default_random_engine randomEngine;

	fstream file("inlab04.txt");
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

	system("PAUSE");
	return 0;
}