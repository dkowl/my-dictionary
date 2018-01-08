#pragma once

#include "BinarySearchTree.h"
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
			if (grandparent != nullptr) grandparent->UpdateHeight();
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
