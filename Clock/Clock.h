#pragma once

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



