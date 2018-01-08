#pragma once

#include "Dictionary.h"

namespace MyDictionary {

	template<typename TKey, typename TValue>
	class LinkedList : Dictionary<TKey, TValue> {

	public:

		LinkedList() {
			head = nullptr;
		}

		virtual void Insert(TKey key, TValue value) {
			Node *newNode = new Node(key, value);
			if (head == nullptr) {
				head = newNode;
				head.prev = head;
				head.next = head;
			}
			else {
				Node *currentNode = head;
				while (currentNode->key < key && currentNode->next != head) {
					currentNode = currentNode->next;
				}
				InsertBefore(currentNode, newNode);
			}
		}

		virtual ReturnType Search(TKey key) {
			Node *currentNode = head;
			do {
				if (currentNode->key == key) return currentNode;
				currentNode = currentNode->next;
			} while (currentNode != head || currentNode->key <= key);
			return nullptr;
		}

		virtual void Delete(TKey key) {

		}

	private:
		Node *head;

		void InsertBefore(Node *node, Node *newNode) {
			Node *nextNode = node->next;
			node->next = newNode;
			nextNode->prev = newNode;
			newNode->prev = node;
			newNode->next = nextNode;
		}

		class Node {

		public:
			TKey key;
			TValue value;
			Node *prev;
			Node *next;

			Node(TKey key, TValue value) :
				key(key),
				value(value),
				prev(nullptr),
				next(nullptr)
			{
			}
		};
	};
}
