#include "common.hpp"
#include "perm.hpp"

const int radix = 1e7;
struct B {
	std::vector<int> v;
	B() : v(1) {}
	inline B &operator*=(int x) {
		if (v.back() > radix / x) v.emplace_back(0);
		for (int i = 0; i < v.size(); i++) v[i] *= x;
		for (int i = 0; i < v.size() - 1; i++) {
			v[i + 1] += v[i] / radix;
			v[i] %= radix;
		}
		return *this;
	}
	inline void print() const {
		for (int i = v.size() - 1, f = 1; i >= 0; i--, f = 0) printf(f ? "%d" : "%07d", v[i]);
		puts("");
	}
};

class schreier_sims {
	int base;
	std::map<int, perm> orbit;
	std::vector<perm> trans, gen;

	template <typename... _Args>
	inline void emplace_perm(_Args &&...args) {
		trans.emplace_back(args...);
		orbit.emplace(trans.back()[base], trans.back());
	}
	void add_orbit(const perm &g) {
		emplace_perm(g);
		for (const perm &h : gen) {
			if (!orbit.count(h[g[base]]))
				add_orbit(g * h);
			else
				subgroup->extend(g * h * ~orbit.at(h[g[base]]));
		}
	}

public:
	schreier_sims *subgroup;

	schreier_sims() : base(0), subgroup(nullptr) {}
	~schreier_sims() { delete subgroup; }

	bool test(const perm &g) const {
		if (g.isId()) return true;
		int b = g[base];
		if (!orbit.count(b)) return false;
		return subgroup->test(g * ~orbit.at(b));
	}
	void extend(const perm &g) {
		if (test(g)) return;

		gen.emplace_back(g);
		if (!subgroup) {
			for (int i = 0; i < g.size(); i++)
				if (g[i] != i) {
					base = i;
					break;
				}

			perm h(g.size());
			do {
				emplace_perm(h);
				h *= g;
			} while (h[base] != base);

			(subgroup = new schreier_sims())->extend(h);
		} else {
			int orig_sz = trans.size();
			for (int i = 0; i < orig_sz; i++) {
				perm &p = trans[i];
				if (!orbit.count(g[p[base]]))
					add_orbit(p * g);
				else
					subgroup->extend(p * g * ~orbit.at(g[p[base]]));
			}
		}
	}
	inline int size() { return trans.size(); }
	void print(std::string _indent = "") {
		const char *indent = _indent.c_str();
		printf("%sbase = %d\n", indent, base);
		printf("%sgen:\n", indent);
		for (int i = 0; i < gen.size(); i++) {
			printf("%s[%d]:\n", indent, i);
			printf("%sgen: ", indent), gen[i].print();
			printf("%simg: %d\n", indent, gen[i][base]);
			printf("%strans: ", indent), orbit.at(gen[i][base]).print();
			puts("");
		}
		printf("%sorbit:\n", indent);
		int i = 0;
		for (auto [img, p] : orbit) {
			printf("%s[%d]:\n", indent, i++);
			printf("%simg: %d\n", indent, img);
			printf("%strans: ", indent), p.print();
			puts("");
		}
		printf("%ssubgroup:\n", indent);
		if (subgroup)
			subgroup->print(_indent + "  ");
		else
			printf("%snull\n", indent);
	}
};

schreier_sims s;
int n, m;
int main() {
	scanf("%d%d", &n, &m);
	std::vector<int> v(n);
	for (int i = 1; i <= m; i++) {
		for (int j = 0; j < n; j++) scanf("%d", &v[j]), v[j]--;
		s.extend(v);
		// s.print();
	}
	B b;
	b.v[0] = 1;
	for (schreier_sims *p = &s; p->subgroup; p = p->subgroup) b *= p->size();
	b.print();
}
