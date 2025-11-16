#pragma once

namespace rr {
/**
 * @brief Random Number Generator -> RNG -> Roylaty Never Gives up
 */
class Rng {
public:
	static Rng& singleton() {
		static Rng rng;
		return rng;
	}

	/**
	 * @brief [start, end)
	 */
	int pick(int start, int end) const {
		return rand() % (end - start) + start;
	}

	int pick(int end) const {
		return pick(0, end);
	}
private:
	Rng() {
		std::srand(std::time({})); // use current time as seed for random generator
	}
};
}
