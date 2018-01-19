#include <iostream>
#include <random>
using namespace std;

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

using namespace MyUtils;

class HashSet {

	static const int ARR_SIZE = 997;
	static const int EMPTY = 0;
	static const int DELETED = -1;
	static const int POW2[];

public :
	enum ColResMethod {
		LINEAR,
		DOUBLE
	};

private:
	int arr[ARR_SIZE];
	ColResMethod colResMethod;

public:
	HashSet(ColResMethod colResMethod = LINEAR) :
		colResMethod(colResMethod)
	{
		Init();
	}

	void Insert(int key) {
		int h = Hash(key);
		while (!Empty(arr[h])) {
			MoveIndex(h, key);
		}
		arr[h] = key;
	}

	void Delete(int key) {
		int h = Hash(key);
		while (!Empty(arr[h])) {
			if (arr[h] == key) {
				arr[h] = DELETED;
				break;
			}
			MoveIndex(h, key);
		}
	}

	bool Contains(int key) {
		int h = Hash(key);
		while (!Empty(arr[h])) {
			if (arr[h] == key) {
				return true;
			}
			MoveIndex(h, key);
		}
		return false;
	}

	void Display(int begin = 0, int end = ARR_SIZE - 1) {
		for (int i = begin; i <= end; i++) {
			cout << arr[i] << " ";
		}
	}

private:

	void Init() {
		for (auto&& key : arr) {
			key = EMPTY;
		}
	}

	int Hash(int key) {
		return ((key % 1000) + POW2[key % 10] + 1) % 997;
	}

	int Hash2(int key) {
		return (3 * key) % 19 + 1;
	}

	void MoveIndex(int& h, int& key) {
		int delta;
		if (colResMethod == LINEAR) delta = 1;
		else delta = Hash2(key);
		h = (h + delta) % ARR_SIZE;
	}

	bool Empty(int value) {
		return value == EMPTY || value == DELETED;
	}

	
};

const int HashSet::POW2[] = { 1, 2, 4, 8, 16, 32, 64, 128, 256, 512 };

void InsertRandom(HashSet &hashSet, int X, default_random_engine &randomEngine) {
	static const int MIN_RANGE = 20000, MAX_RANGE = 40000;
	static const uniform_int_distribution<int> dist(MIN_RANGE, MAX_RANGE);
	while (X > 0) {
		int r = dist(randomEngine);
		if (!hashSet.Contains(r)) {
			hashSet.Insert(r);
			X--;
		}
	}
}

void TestHashSet(HashSet::ColResMethod colResMethod, int X, int k1, int k2, int k3, int k4) {
	default_random_engine randomEngine;
	Clock::Start();
	HashSet hashSet(colResMethod);
	hashSet.Delete(k1);
	hashSet.Insert(k1);
	hashSet.Display(0, 100);
	InsertRandom(hashSet, X, randomEngine);
	hashSet.Display(0, 100);
	hashSet.Insert(k2);
	hashSet.Insert(k3);
	hashSet.Insert(k4);
	hashSet.Display(0, 100);
	hashSet.Display(500, 600);
	hashSet.Delete(k3);
	hashSet.Delete(k4);
	hashSet.Display(0, 100);
	hashSet.Display(500, 600);
	Clock::End();
}

int main() {

	int X, k1, k2, k3, k4;
	cin >> X >> k1 >> k2 >> k3 >> k4;
	TestHashSet(HashSet::LINEAR, X, k1, k2, k3, k4);
	TestHashSet(HashSet::DOUBLE, X, k1, k2, k3, k4);

	system("PAUSE");
	return 0;
}