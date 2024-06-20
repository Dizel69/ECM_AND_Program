#include <iostream>
#include <iomanip>

struct el_ts
{
	int label;
	int ind_tl;
	int next;
};
class Hash
{
	int n_max, n_os, NTS;
	el_ts* t;

	int hash_func(int key)
	{
		return key % n_os + 1;
	}

	int fill_gap(int gap) {
		if (gap >= NTS || gap <= n_os) return 1;

		NTS--;

		if (gap == NTS) return 1;

		int i = hash_func(t[NTS].label);
		int prev;
		while (t[i].next != -1) {
			prev = i;
			i = t[i].next;
		}

		t[prev].next = gap;
		t[gap].label = t[NTS].label;
		t[gap].ind_tl = t[NTS].ind_tl;
		t[gap].next = t[NTS].next;

		return 0;
	}
public:
	Hash(int n2 = 100, int n1 = 400)
	{
		n_max = n1; n_os = n2; NTS = n_os + 1;
		t = new el_ts[n_max];
		for (int i = 0; i < n_max; i++)
			t[i].label = t[i].ind_tl = t[i].next = -1;
	}
	Hash(const Hash& a)
	{
		n_max = a.n_max;  n_os = a.n_os;  NTS = a.NTS;
		t = new el_ts[n_max];
		for (int i = 0; i < n_max; i++)
			t[i] = a.t[i];
	}

	~Hash() { delete[]t; }


	int Find(int key)
	{
		int i;
		i = hash_func(key);
		if (t[i].label == key) return t[i].ind_tl;
		while (t[i].next != -1)
		{
			i = t[i].next;
			if (t[i].label == key) return t[i].ind_tl;
		}
		return -1;
	}
	int Add(int label1, int ind1)
	{
		int i = hash_func(label1);

		if (t[i].label == -1) {
			t[i].label = label1;
			t[i].ind_tl = ind1;
			return 0;
		}

		if (t[i].label == label1) {
			if (t[i].ind_tl == -1)
				t[i].ind_tl = ind1;
			return 0;
		}

		while (t[i].next != -1) {
			i = t[i].next;
			if (t[i].label == label1) {
				if (t[i].ind_tl == -1)
					t[i].ind_tl = ind1;
				return 0;
			}
		}

		if (NTS >= n_max)
			return 1;

		t[NTS].label = label1;
		t[NTS].ind_tl = ind1;
		t[i].next = NTS;
		NTS++;

		return 0;
	}
	
	int Del(int key) {
		int i = hash_func(key);

		if (t[i].label == key) {
			if (t[i].next == -1) {
				t[i].label = -1;
				t[i].ind_tl = -1;
				return 0;
			}

			t[i].label = t[t[i].next].label;
			t[i].ind_tl = t[t[i].next].ind_tl;

			int gap = t[i].next;
			t[i].next = t[t[i].next].next;

			fill_gap(gap);
			return 0;
		}
		int prev = -1;
		while (t[i].next != -1) {
			prev = i;
			i = t[i].next;

			if (t[i].label == key) {
				t[prev].next = t[i].next;
				fill_gap(i);
				return 0;
			}
		}
		
		return 1;
	}
	Hash& operator=(const Hash& a)
	{
		if (this == &a) return *this;
		n_max = a.n_max;  n_os = a.n_os;  NTS = a.NTS;
		delete[]t;
		t = new el_ts[n_max];
		for (int i = 0; i < n_max; i++)
			t[i] = a.t[i];
		return *this;
	}
	void Print_t(std::ostream& os = std::cout)
	{
		os << "Index\tLabel\tInd_tl\tNext\n";
		os << std::left;
		for (int i = 1; i < NTS; ++i)
			if (t[i].label != -1) {
				os.width(5);
				os << i << '\t';
				os.width(5);
				os << t[i].label << '\t';
				os.width(6);
				os << t[i].ind_tl << '\t';
				os.width(4);
				os << t[i].next << '\t';
				os << '\n';
			}
		os << std::right;
	}
};