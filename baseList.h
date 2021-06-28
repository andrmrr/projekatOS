/*
 * baseList.h
 *
 *  Created on: Jun 23, 2021
 *      Author: OS1
 */

#ifndef BASELIST_H_
#define BASELIST_H_

#include <IOSTREAM.H>

template <class T>
class BaseList {

	struct Elem {
		T data;
		Elem* next;
		Elem(const T& p) {
			data = p;
			next = 0;
		}
	};

	Elem* first, * last, * current, * beforeCurrent;
	void copy(const BaseList& l)  {
		first = last = current = beforeCurrent = 0;

		for (Elem* cur = l.first; cur; cur = cur->next) {
			Elem* newElem = new Elem(cur->data);
			last = (!first ? first : last->next) = newElem;

			if (cur == l.current) current = newElem;
			if (cur == l.beforeCurrent) beforeCurrent = newElem;
		}
	}

	void move(BaseList& l) {
		first = l.first;
		last = l.last;
		l.first = l.last = 0;
	}

	void deleteAll() {
			Elem* elem = first;
		while (first) {
			elem = first->next;
			delete first;
			first = elem;
		}
		last = current = beforeCurrent = 0;
	}

public:
	BaseList() {
		first = last = beforeCurrent = current = 0;
	}

	BaseList(const BaseList& l) {
		copy(l);
	}

	~BaseList() {
		deleteAll();
	}

	BaseList& operator=(const BaseList& l) {
		if (this != &l) {
			deleteAll();
			copy(l);
		}
		return *this;
	}


	BaseList& add(const T& t) {
		last = (!first ? first : last->next) = new Elem(t);
		return *this;
	}

	BaseList& goToFirst() {
		current = first;
		beforeCurrent = 0;
		return *this;
	}

	BaseList& goToNext() {
		beforeCurrent = current;
		if(current) current = current->next;
		return *this;
	}

	int isCurrent() const {
		return current != 0;
	}

	T& getCurrent(){
		if (!current) cout << "Ne postoji element ";
		return current->data;
	}

	const T& getCurrent() const {
		if (!current) cout << "Ne postoji element ";
		return current->data;
	}

	BaseList& removeCurrent() {
		if (!current) cout << "Ne postoji element ";
		Elem* old = current;
		current = current->next;
		(!beforeCurrent ? first : beforeCurrent->next) = current;
		if (current == 0) last = beforeCurrent;
		delete old;
		return *this;
	}

	friend ostream& operator<<(ostream& os, const BaseList& l1) {
		for (Elem *current = l1.first; current; current = current->next) {
			os << current->data << endl;
		}
		return os;
	}
};





#endif /* BASELIST_H_ */
