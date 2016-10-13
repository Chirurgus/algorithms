#ifndef GUARD_STACK_H
#define GUARD_STACK_H

#include  <vector>
//well, this was easy
//hoping that i didn't make stupid mistakes

//I think that using a vector for a stack is fine
//since i will only ever be pushing or popping from 
//one size, so all the operations are guarateed to be O(1)
//without the mem overhead of lists. <= much smart words, вдiповiдаю

//30.06.16 update:
//Well do you know, I did make a mistake. Who would have thought
//that std::vector.pop_back() doesn't return anything, it just removes
//the last element. I had to use vector.back() before calling pop_back()

template<class T>
class Stack {
public:
	typedef typename std::vector<T>::size_type size_type;
	//this doesn't need any explicit constructors, right?, I sure hope so.
	T pop();
	void push(const T& d);

	bool isEmpty() const;
	size_type size() const;


private:
	std::vector<T> _stack;
};

template<class T>
T Stack<T>::pop() {
	T ret = this->_stack.back();
	this->_stack.pop_back();
	return ret;
}

template<class T>
void Stack<T>::push(const T & d) {
	this->_stack.push_back(d);
}

template<class T>
inline bool Stack<T>::isEmpty() const {
	return this->_stack.empty();
}

template<class T>
inline typename Stack<T>::size_type Stack<T>::size() const {
	return this->_stack.size();
}

#endif

