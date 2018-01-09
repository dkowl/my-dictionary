#pragma once

#include "Dictionary.h"

namespace MyDictionary {

	template<typename TKey, typename TValue>
	class LinkedList : public Dictionary<TKey, TValue> {

	public:

		LinkedList() {
			head = nullptr;
			size = 0;
		}

		virtual void Insert(TKey key, TValue value) {
			Node *newNode;
			if (head == nullptr) {
				newNode = new Node(key, value);
				head = newNode;
				head->prev = head;
				head->next = head;
				size++;
			}
			else {				
				newNode = new Node(key, value);
				Node *successor = Successor(key);
				if (successor->key != key) {
					InsertBefore(Successor(key), newNode);
					size++;
				}
			}
		}

		virtual bool Contains(TKey key) {
			return(Search(key) != nullptr);
		}

		virtual ReturnType Search(TKey key) {
			Node *successor = Successor(key);
			if (successor->key == key) 
				return (ReturnType)&successor->value;
			else 
				return nullptr;
		}

		virtual void Delete(TKey key) {
			Node *node = GetNode(key);
			if (node != nullptr) {
				Delete(node);
			}
		}

		virtual int Size() {
			return size;
		}

		virtual std::vector<ReturnType> GetValues() {
			std::vector<ReturnType> result;
			Node *currentNode = head;
			if (currentNode != nullptr) {
				do {
					result.push_back((ReturnType)currentNode);
					currentNode = currentNode->next;
				} while (currentNode != head);
			}
			return result;
		}

		virtual void Display() {

		}

	private:
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

		Node *head;
		int size;

		void InsertBefore(Node *node, Node *newNode) {
			Node *prevNode = node->prev;
			node->prev = newNode;
			prevNode->next = newNode;
			newNode->next = node;
			newNode->prev = prevNode;
			if (newNode->key < head->key) {
				head = newNode;
			}
		}

		void Delete(Node *node) {
			Node *prev = node->prev;
			Node *next = node->next;
			prev->next = next;
			next->prev = prev;
			if (head == node) {
				head = next;
			}
			delete node;
			size--;
		}

		Node* Successor(TKey key) {
			Node *currentNode = head;
			if (currentNode != nullptr) {
				do {
					if (currentNode->key >= key) return currentNode;
					currentNode = currentNode->next;
				} while (currentNode != head);
			}
			return currentNode;
		}

		Node* GetNode(TKey key) {
			Node* successor = Successor(key);
			if (successor->key == key) {
				return successor;
			}
			else {
				return nullptr;
			}
		}

		
	};
}
