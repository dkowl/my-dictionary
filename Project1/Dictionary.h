#pragma once
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
		virtual int Size() = 0;
		virtual std::vector<ReturnType> GetValues() = 0;
		virtual void Display() = 0;
	};
}