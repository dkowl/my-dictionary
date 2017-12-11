#pragma once

#include "BinarySearchTree.h"
#include <algorithm>

namespace MyDictionary {

	template<typename TKey, typename TValue>
	class AvlTree : public BinarySearchTree<TKey, TValue> {

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
		};

		virtual Node* Allocate(TKey key, TValue value) {
			size++;
			return new AvlNode(key, value);
		}

		void Retrace(Node *node){
			//building parent stack
			std::stack<AvlNode*> parentStack;
			AvlNode* currentNode = (AvlNode*)root;
			while (true) {
				if (currentNode == node)
					break;

				parentStack.push(currentNode);
				if (node->key < currentNode->key) {
					currentNode = (AvlNode*)currentNode->left;
				}
				else {
					currentNode = (AvlNode*)currentNode->right;
				}
			}

			while (!parentStack.size() > 1) {
				AvlNode *parent = parentStack.top();
				parentStack.pop();
				AvlNode *grandparent = parentStack.top();

				if (std::abs(parent->BalanceFactor()) > 1) {
					bool firstLeft, secondLeft;
					firstLeft = parent->left == currentNode;
					secondLeft = node->key < currentNode->key;
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
			if (grandparent->left == parent) {
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
			grandparent->UpdateHeight();
		}
	};
}
