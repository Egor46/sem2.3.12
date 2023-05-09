#include <iostream>
#include <fstream>
#include <assert.h>
#include <iomanip>

using namespace std;
//12


//
//AUXILIARY STUFF
//

char vars[3]{ 'x' ,'y','z' };

struct Coef {
	int value = 0;
	int st[3] = { 0,0,0 };
};

inline bool eqSt(int s1[3], int s2[3]) {
	return (s1[0] == s2[0] && s1[1] == s2[1] && s1[2] == s2[2]);
}

struct lElem {
	Coef c;
	lElem* next = nullptr;
};

/// <summary>
/// task A;
///  function that returns new linked list of Polynom from file
///	note: first element is zero-like for making next functions easier to make
/// </summary>
/// <param name="[in]">filestream with polynom in format <b>int {int int int}</b> of coefficent and powers</param>
/// <returns>pointer to first element in list</returns>
lElem* createList(ifstream& in) {
	lElem* list = new lElem;
	lElem* cur = new lElem;
	list->next = cur;
	while (!in.eof()) {
		in >> cur->c.value >> cur->c.st[0] >> cur->c.st[1] >> cur->c.st[2];
		if (in.eof()) break;
		cur->next = new lElem;
		cur = cur->next;
	}
	return list;
}

/// <summary>
/// task B;
/// write formatted polynom into file
/// </summary>
/// <param name="first">pointer to first elem in the list</param>
/// <param name="out">output file</param>
void outList(lElem* first, ofstream& out) { // ещё доделать
	lElem* a = first->next;

	if (a->c.value == 1);
	else if (a->c.value == -1) out << '-' << ' ';
	else (a->c.value > 0 ? (out << a->c.value << ' ') : (out << '-' << ' ' << abs(a->c.value) << ' '));

	for (int i = 0; i < 3; i++) {
		switch (a->c.st[i]) {
		case 0:
			break;
		case 1:
			out << vars[i] << ' ';
			break;
		default:
			out << vars[i] << '^' << a->c.st[i] << ' ';
		}
	}
	a = a->next;

	while (a != nullptr) {

		if (a->c.value == 1) out << '+' << ' ';
		else if (a->c.value == -1) out << '-' << ' ';
		else out << (a->c.value > 0 ? '+' : '-') << ' ' << abs(a->c.value) << ' ';

		for (int i = 0; i < 3; i++) {
			switch (a->c.st[i]) {
			case 0:
				break;
			case 1:
				out << vars[i] << ' ';
				break;
			default:
				out << vars[i] << '^' << a->c.st[i] << ' ';
			}
		}
		a = a->next;
	}
}

/// <summary>
/// task C, no need to describe
/// </summary>
/// <param name="a">pointer to first element</param>
void deleteList(lElem* a) {
	lElem* cur = a->next;
	while (cur != nullptr) {
		delete a;
		a = cur;
		cur = cur->next;
	}
}

/// <summary>
/// Auxiliary function for some other tasks (D, E)
/// </summary>
/// <param name="prev">pointer to element, that stands before element, that should be deleted</param>
void deleteElemAfter(lElem* prev) {
	lElem* t = nullptr;
	if (prev->next != nullptr) t = prev->next->next;
	else return;
	delete prev->next;
	prev->next = t;
}

/// <summary>
/// task D;
/// at first checks is there already element with those powers of variables, if true delete it from list
/// then it adds element with coefficent c at position pos
/// </summary>
/// <param name="a">pointer to head of the list</param>
/// <param name="c">coefficent that should be added</param>
/// <param name="pos">index where to add new element</param>
void addElem(lElem* a, Coef c, int pos) {
	lElem* b = a->next;
	while (b->next != nullptr) {
		if (eqSt(c.st, b->next->c.st)) {
			deleteElemAfter(b);
			break;
		}
		b = b->next;
	}
	b = a->next;
	for (int i = 0; i < pos && b->next != nullptr; i++) {
		b = b->next;
	}
	if (b->next != nullptr) {
		lElem* t = b->next;
		b->next = new lElem;
		b->next->next = t;
		b->next->c.value = c.value;
		for (int i = 0; i < 3; i++) {
			b->next->c.st[i] = c.st[i];
		}
	}
	else {
		b->next = new lElem{ {c.value, {c.st[0], c.st[1], c.st[2]}}, nullptr };
	}
}

/// <summary>
/// task E obviously
/// </summary>
/// <param name="st">вектор степеней</param>
void taskE(lElem* first, int st[3]) {
	lElem* a = first->next;
	while (a->next != nullptr) {
		if (eqSt(st, a->next->c.st) && a->next->c.value % 2 == 0) {
			deleteElemAfter(a);
		}
		else a = a->next;
	}
}

/// <summary>
/// task F
/// </summary>
/// <param name="a"></param>
/// <param name="st"></param>
/// <returns></returns>
bool isInList(lElem* a, int st[3]) {
	a = a->next;
	while (a != nullptr) {
		if (eqSt(a->c.st, st)) return true;
		a = a->next;
	}
	return false;
}

/// <summary>
/// task G, find polynom in some point
/// </summary>
double evaluate(lElem* a, int coord[3]) {
	a = a->next;
	double res = 0;
	while (a != nullptr) {
		res += a->c.value * pow(coord[0], a->c.st[0]) * pow(coord[1], a->c.st[1]) * pow(coord[2], a->c.st[2]);
		a = a->next;
	}
	return res;
}

/// <summary>
/// task H, finds min coef and returns its vector
/// </summary>
/// <returns>vector of elems</returns>
int* findStMin(lElem* list) {
	assert(list->next != nullptr);
	lElem* cpy = list->next;
	int m = cpy->c.value;
	while (cpy != nullptr) {
		if (cpy->c.value < m) m = cpy->c.value;
		cpy = cpy->next;
	}
	cpy = list->next;
	while (cpy->c.value != m) {
		cpy = cpy->next;
	}
	int* stm = new int[3] {cpy->c.st[0], cpy->c.st[1], cpy->c.st[2]};
	return stm;
}

int main() {
	ifstream in("polynom.txt");
	ofstream out("outPoly.txt");

	lElem* list = createList(in);
	lElem* b = list->next;

	int a[3]{ 1, 1, 1 };
	taskE(list, a);
	outList(list, out);

	deleteList(list);
	in.close();
	out.close();

	return 0;

}