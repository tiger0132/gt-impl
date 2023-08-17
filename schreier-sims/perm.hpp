#pragma once

#include "common.hpp"

class perm {
	std::vector<int> v;
	inline perm(const std::vector<int> &v, bool check) : v(v) {
		if (!check) return;
		std::vector<u8> t(v.size());
		for (const int &i : v)
			if (0 <= i && i < v.size() && !t[i])
				t[i] = 1;
			else
				throw new std::runtime_error("Input is not a permutation.");
	}

public:
	inline perm() = default;
	inline perm(int n) : v(n) { std::iota(v.begin(), v.end(), 0); }
	inline perm(const std::vector<int> &v) : perm(v, true) {}
	inline perm(const perm &p) : v(p.v) {}

	inline int operator[](int i) const { return v[i]; }
	inline int size() const { return v.size(); }

	void print() const {
		for (int i : v) printf("%d ", i);
		puts("");
	}
	bool isId() const {
		for (int i = 0; i < size(); i++)
			if (v[i] != i) return false;
		return true;
	}
	inline perm operator*(const perm &rhs) const {
		if (size() != rhs.size())
			throw new std::runtime_error("Operands have unequal lengths.");
		std::vector<int> ret(size());
		for (int i = 0; i < size(); i++) ret[i] = rhs[v[i]];
		return ret;
	}
	inline perm &operator*=(const perm &rhs) {
		if (size() != rhs.size())
			throw new std::runtime_error("Operands have unequal lengths.");
		std::vector<int> ret(size());
		for (int i = 0; i < size(); i++) ret[i] = rhs[v[i]];
		return v = ret, *this;
	}
	inline perm operator~() const {
		std::vector<int> ret(size());
		for (int i = 0; i < size(); i++) ret[v[i]] = i;
		return ret;
	}
};
