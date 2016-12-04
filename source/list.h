#ifndef GUARD_LIST_H
#define GUARD_LIST_H

//Note to self: FUCK ITERATORS

// Unsorted doubly linked list
template<class T>
class List {
private:
	struct list_s;
public:
	typedef unsigned long long size_type;

	//class forward_iterator;

	List();
	List(const List<T>& list);
	List<T>& operator=(const List<T>& list);
	~List();

	void push_front(const T& key);
	void push_back(const T& key);
	T pop_front();
	T pop_back();

	void remove(const T& key);
	bool search(const T& key);

	//forward_iterator begin();
	//forward_iterator end();

	size_type size() const;
	bool isEmpty() const;

	T& operator[](const size_type& n);
	const T& operator[](const size_type& n) const;
private:
	list_s* _head;
	list_s* _tail;

	size_type _size;
};

/*template<class T>
class List<T>::forward_iterator {
	private:
		friend bool operator!=(const List<T>::forward_iterator & l,
							   const List<T>::forward_iterator & r);
		friend bool operator==(const List<T>::forward_iterator & l,
							   const List<T>::forward_iterator & r);
		list_s* _ptr;
	public:
		forward_iterator();
		forward_iterator(list_s* ptr);
		T& operator++();
		T& operator*();
};*/

template<class T>
struct List<T>::list_s {
	list_s* next;
	list_s* prev;
	T data;
};

template<class T>
List<T>::List(): _head(0), _tail(0), _size(0) { }

template<class T>
List<T>::List(const List & list): List() {
	for (size_type i = 0; i < list.size(); ++i) {
		this->push_back(list[i]);
	} 
}

template<class T>
List<T>& List<T>::operator=(const List & list)
{
	List<T>::list_s* rs_it = list->_head,
					 ls_it = this->_head;
	while (rh_it != 0 && ls_it != 0) {//_tail->next == 0 is true
		ls_it->data = rs_it->data;

		rs_it = rs_it->next, ls_it = ls_it->ls_it->next;
	}
	if (ls_it != 0) {
		this->_tail = ls_it->prev;
		this->_tail->next = 0;
	}
	while (ls_it != 0) {
		List<T>::list_s* to_delete = ls_it;
		ls_it = ls_it->next;

		delete to_delete;
	}
	while (rs_it != 0) {
		this->push_back(rs_it->data);
		rs_it = rs_it->next;
	}
}

template<class T>
List<T>::~List() {
	List<T>::list_s* ptr = _head,
	            	 *next_ptr;
	while (ptr) {
		next_ptr = ptr->next;
		delete ptr;
		ptr = next_ptr;
	}
}

template<class T>
void List<T>::push_front(const T & key) {
	list_s* new_node = new list_s;

	new_node->data = key;

	if (_tail == 0) {
		new_node->next = new_node->prev = 0;
		_tail = new_node;
	}
	else {
		new_node->next = this->_head;
		this->_head->prev = new_node;
		new_node->prev = 0;
	}

	_head = new_node;
	++this->_size;
}

template<class T>
void List<T>::push_back(const T & key) {
	list_s* new_node = new list_s;

	new_node->data = key;
	
	if (_head == 0) {
		new_node->next = new_node->prev = 0;
		_head = new_node;
	}
	else {
		new_node->next = 0;
		new_node->prev = _tail;
		_tail->next = new_node;
	}
	
	_tail = new_node;

	++this->_size;
}

template<class T>
T List<T>::pop_front() {
	//this shouldn't be called on an  empty list.
	list_s* old_node = _head;
	
	if (old_node == this->_tail) {
		this->_tail = 0;
		this->_head = 0;
	}
	else {
		_head = _head->next;
		_head->prev = 0;
	}

	T ret_data = old_node->data;
	delete old_node;

	--this->_size;
	
	return ret_data;
}

template<class T>
T List<T>::pop_back() {
	list_s* old_node = _tail;

	if (old_node == this->_head) {
		this->_head = 0;
		this->_tail = 0;
	}
	else {
		_tail = _tail->prev;
		_tail->next = 0;
	}

	T ret_data = old_node->data;
	delete old_node;

	--this->_size;

	return ret_data;
}

template<class T>
void List<T>::remove(const T & key) {
	//too proud of this to remove it
	for (list_s** it = &_head; (*it) != 0; *it = (*it)->next) {
		if ((*it)->data == key) {
			*it = (*it)->next;
			delete *it;
			break;
		}
	}
	--this->_size;
}

template<class T>
bool List<T>::search(const T & key) {
	for (list_s* it = _head; it != 0; it = it->next) {
		if (it->data == key)
			return true;
	}
	return false;
}

template<class T>
inline typename List<T>::size_type List<T>::size() const {
	return this->_size;
}

template<class T>
inline bool List<T>::isEmpty() const {
	return this->_size;
}

template<class T>
T & List<T>::operator[](const size_type & n) {
	list_s* it;

	if (n > this->size() / 2) {
		it = this->_tail;
		size_type i = this->size() - 1;
		while (i > n) {
			it = it->prev;
			--i;
		}
	}
	else {
		it = this->_head;
		size_type i = 0;
		while (i < n) {
			it = it->next;
			++i;
		}
	}
	return it->data;
}

template<class T>
const T & List<T>::operator[](const size_type & n) const {
	list_s* it = _head;
	
	if (n > this->size() / 2) {
		size_type i = this->size() - 1;
		while (i > n) {
			it = it->prev;
			--i;
		}
	}
	else {
		size_type i = 0;
		while (i < n) {
			it = it->next;
			++i;
		}
	}
	return it->data;
}

/*template<class T>
typename List<T>::forward_iterator List<T>::begin() {
	return forward_iterator(this->_head);
}

template<class T>
typename List<T>::forward_iterator List<T>::end() {
	return forward_iterator(this->_tail->next);// returns 0
}

template<class T>
List<T>::forward_iterator::forward_iterator(): _prt(0) {}

template<class T>
List<T>::forward_iterator::forward_iterator(list_s * ptr) : _ptr(ptr) {}

template<class T>
T& List<T>::forward_iterator::operator++() {
	this->_prt = (this->_ptr)->next;
	return this;
}

template<class T>
T& List<T>::forward_iterator::operator*() {
	return this->_prt->data;
}

// External functions
template<class T>
bool operator==(const List<T>::forward_iterator & l,
				const List<T>::forward_iterator & r) {
	return l->_ptr == r->_prt;
}

template<class T>
bool operator!=(const List<T>::forward_iterator & l,
				const List<T>::forward_iterator & r) {
	return !(r == l);
}
*/
#endif