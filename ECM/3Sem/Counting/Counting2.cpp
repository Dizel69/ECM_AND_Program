#include <iostream>
#include <fstream>
#include <vector>

template <class info_type>
class cycled_list {
private:
	struct node {
		info_type info;
		node* next;
	};

	node* head;
	size_t m_size;
public:
	cycled_list() { head = nullptr; m_size = 0; }
	cycled_list(cycled_list&& other) {
		head = other.head;
		m_size = other.m_size;

		other.head = nullptr;
		other.m_size = 0;
	}
	void clear() {
		if (!head) return;

		node* t_ptr = head;
		do {
			node* tt_ptr = t_ptr;
			t_ptr = t_ptr->next;
			delete tt_ptr;
		} while (t_ptr != head);

		head == nullptr;
		m_size = 0;
	}
	~cycled_list() { clear(); }

	size_t size() const { return m_size; }

	void append(info_type& el) {
		node* new_node = new node;
		new_node->info = el;

		if (head == nullptr) {
			new_node->next = new_node;
			head = new_node;
			m_size++;
			return;
		}

		node* t_ptr = head;
		while (t_ptr->next != head) {
			t_ptr = t_ptr->next;
		}
		t_ptr->next = new_node;
		new_node->next = head;

		m_size++;
	}

	class iterator {
	private:
		node* node_ptr;

		node* operator ->() { return node_ptr; }
	public:
		iterator() { node_ptr == nullptr; }
		iterator(const iterator& _it) { node_ptr = _it.node_ptr; }
		~iterator() = default;

		iterator& operator =(const iterator& _it) { node_ptr = _it.node_ptr; }

		iterator& operator ++() { node_ptr = node_ptr->next; return *this; }
		iterator operator ++(int) { iterator old = *this; node_ptr = node_ptr->next; return old; }

		bool operator ==(const iterator& _it) { return node_ptr == _it.node_ptr; }
		bool operator !=(const iterator& _it) { return node_ptr != _it.node_ptr; }

		info_type& operator *() { return node_ptr->info; }

		friend class cycled_list;
	};

	iterator begin() const { iterator _it; _it.node_ptr = head; return _it; }

	void erase(iterator& _it) {
		if (size() == 1) {
			delete head;
			_it.node_ptr = head = nullptr;
			m_size = 0;
			return;
		}
		node* t_ptr = _it->next;

		_it->info = t_ptr->info;
		_it->next = t_ptr->next;

		if (t_ptr == head) head = head->next;
		delete t_ptr;
		m_size--;
	}

	template <class T>
	friend std::ostream& operator <<(std::ostream&, const cycled_list<T>&);
};

template <class info_type>
std::ostream& operator <<(std::ostream& stream, const cycled_list<info_type>& list) {
	if (list.size() == 0) return stream;

	auto _it = list.begin();

	do {
		stream << *_it;
		_it++;

		if (_it == list.begin()) break;

		stream << ' ';
	} while (true);

	return stream;
}

void load(cycled_list<std::string>& str_list, const char* filename) {
	std::ifstream in(filename);

	if (!in) return;

	str_list.clear();
	std::string input_buf;
	while (in) {
		char x;
		x = in.get();

		if (x == '\n') { str_list.append(input_buf); input_buf.clear(); }
		else { input_buf += x; }
	}

	in.close();
}

cycled_list<std::string> counting(cycled_list<std::string>& people, uint32_t stride) {
	if (people.size() == 0) return cycled_list<std::string>();

	cycled_list<std::string> kicked_out;

	auto _it = people.begin();
	while (people.size() > 1) {
		for (int i = 0; i < stride - 1; ++i) {
			++_it;
		}
		kicked_out.append(*_it);
		people.erase(_it);
	}

	return kicked_out;
}

int main(int argc, char* argv[]) {
	int stride;
	if (argc < 2) stride = 3;
	else stride = atoi(argv[1]);

	cycled_list<std::string> people;
	load(people, "people.txt");

	std::cout << "Начальный набор людей: "; std::cout << people << '\n';
	std::cout << "-------------------------------------\n";

	cycled_list<std::string> kicked_out = counting(people, stride);
	std::cout << "Порядок выбывания: "; std::cout << kicked_out << '\n';
	std::cout << "-------------------------------------\n";

	std::cout << "Победитель: "; std::cout << people << '\n';

	return 0;
}