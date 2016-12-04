#ifndef GUARD_DEQUE_H
#define GUARD_DEQUE_H

//I shall use my list because I can
#include "list.h"

template<class T>
class Deque {
public:
	using size_type = List<T>::size_type;
	

	void pushFront(const T& d);
	T popFront();
	T popBack();
	
	bool isEmpty() const;
	size_type size() const;
private:
	List<T> _list;
};

template<class T>
void Deque<T>::pushFront(const T & d) {
	_list.push_front(T);
}

template<class T>
T Deque<T>::popFront() {
	return _list.pop_front();
}

template<class T>
T Deque<T>::popBack() {
	return _list.pop_back();
}

template<class T>
bool Deque<T>::isEmpty() const
{
	return this->_list.isEmpty();
}

template<class T>
Deque<T>::size_type Deque<T>::size() const {
	return this->_list.size();
}

#endif