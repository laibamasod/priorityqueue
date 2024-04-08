#include <iostream>
#include<fstream>
#include <string>
using namespace std;
struct character
{
	char k;
	int f;
	string b;
	character* left, * right;
	bool operator>(const character& c) {
		return f > c.f;
	}
	bool operator<(const character& c) {
		return f < c.f;
	}
	bool operator==(const character& c) {
		return f == c.f;
	}
	bool operator>=(const character& c) {
		return f >= c.f;
	}
	bool operator<=(const character& c) {
		return f <= c.f;
	}
};
template<typename T>
void ReheapUp(T data[], int first, int last) {
	int parent = (last - 1) / 2;
	if (last > parent) {
		if (data[last] <= data[parent]) {
			swap(data[last], data[parent]);
			ReheapUp(data, first, parent);
		}
	}
}
template<typename T>
void ReheapDown(T data[], int first, int last) {
	int left, right, max;
	left = (first * 2) + 1;
	right = (first * 2) + 2;
	max = left;
	if (left <= last) {
		if (right >= last && data[right] < data[left]) {
			max = right;
		}
		if (data[first] >= data[max]) {
			swap(data[first], data[max]);
			ReheapDown(data, max, last);
		}
	}
}
template<typename T>
class priority_queue {
	T* data;
	int last_index, SIZE;
public:
	priority_queue(int s = 100) {
		SIZE = s;
		last_index = -1;
		data = new T[SIZE];
	}
	~priority_queue() {
		delete[]data;
	}
	void push(const T& val) {
		if (full()) {
			throw("Queue Overflow!");
		}
		data[++last_index] = val;
		ReheapUp(data, 0, last_index);
	}
	bool empty() const {
		return last_index == -1;
	}
	bool full() const {
		return last_index == SIZE - 1;
	}
	int size() const {
		return last_index + 1;
	}
	void make_empty() {
		last_index = -1;
	}
	T top() const {
		if (empty()) {
			throw("Queue Underflow!");
		}
		return data[0];
	}
	void pop() {
		if (empty()) {
			throw("Queue Underflow!");
		}
		data[0] = data[last_index--];
		ReheapDown(data, 0, last_index);
	}
};

void encode(character* root, string str, string codes[])
{
	if (root == NULL)
		return;

	if (!root->left && !root->right) {
		codes[root->k] = str;
	}

	encode(root->left, str + "0", codes);
	encode(root->right, str + "1", codes);
}

int main()
{
	char freq[128];
	for (int i = 0; i < 128; i++) {
		freq[i] = 0;
	}
	string text = "";

	ifstream theFile("data.txt");
	if (theFile.is_open()) {
		char c;
		while (theFile.get(c)) {
			text += c;
			int ind = c;
			freq[ind]++;
		}
	}
	else {
		cout << "Couldn't open file!" << endl;
	}

	priority_queue<character*> q;

	for (int i = 0; i < 128; i++) {
		if (freq[i] != 0) {
			character* c = new character();
			c->k = i;
			c->f = freq[i];
			c->left = NULL;
			c->right = NULL;
			q.push(c);
		}
	}
	while (q.size() > 1) {
		character* temp1 = q.top();
		q.pop();
		character* temp2 = q.top();
		q.pop();
		character* c = new character();
		c->k = '\0';
		c->f = temp1->f + temp2->f;
		c->left = temp1;
		c->right = temp2;
		q.push(c);
	}

	character* root = q.top();

	string codes[128];
	encode(root, "", codes);

	cout << "File Read: " << text << endl;

	string str = "";
	for (int i = 0; i < text.size(); i++) {
		str += codes[text[i]];
	}

	cout << "String Encoded: " << str << endl;
	cout << "String Decoded: ";
	int i = 0;
	while (i < str.size()) {
		if (str[i] == '1')
			root = root->right;
		else
			root = root->left;

		if (root->k != '\0') {
			cout << root->k;
			root = q.top();
		}
		++i;
	}
	return 0;
}