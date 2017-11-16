#include "Clock.h"

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
