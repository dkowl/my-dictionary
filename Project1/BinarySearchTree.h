#pragma once
#include "Dictionary.h"
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
				if (prevParent->left == prev)
					prevParent->left = nullptr;
				else
					prevParent->right = nullptr;

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