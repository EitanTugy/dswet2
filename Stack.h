#pragma once
#include <exception>
#include <new> 
#include <cstddef>

enum TYPE_STACK_EXCEPTION {
	STACK_mem = 1,
	STACK_nospace,
	STACK_failure
};
class stackExceptions {
	TYPE_STACK_EXCEPTION type;
public:
	stackExceptions(TYPE_STACK_EXCEPTION type)
		:
		type(type)
	{}
	TYPE_STACK_EXCEPTION getType() {
		return type;
	}
};


template<class T>
class Stack {
private:
	int next_index;
	T** arr;
	int max_size;
public:
	Stack(int max_size_init = 100)
		:
		next_index(0),
		arr(NULL),
		max_size(max_size_init)
	{
		try {
			arr = new T*[max_size];
			for (int i = 0; i < max_size; ++i) {
				arr[i] = NULL;
			}
		}
		catch (std::bad_alloc a) {
			throw stackExceptions(STACK_mem);
		}
	}
	~Stack() {
		if (arr != NULL) {
			while (!isEmpty()) {
				pop();
			}
		delete[] arr;
		}
	}
	void push(const T& elem) {
		try {
			if (next_index == max_size) {
				throw stackExceptions(STACK_nospace);
			}
			arr[next_index++] = new T(elem);
		}
		catch (std::bad_alloc a) {
			throw stackExceptions(STACK_mem);
		}
	}

	T& top() {
		if (next_index <= 0) {
			throw stackExceptions(STACK_failure);
		}
		return *(arr[next_index - 1]);
	}

	//const T& top() {
	//	if (next_index <= 0) {
	//		//throw exception, stack is empty.
	//	}
	//	return arr[next_index - 1];
	//}

	void pop() {
		if (next_index <= 0) {
			throw stackExceptions(STACK_failure);
		}
		delete(arr[--next_index]);
		arr[next_index] = NULL;
	}

	bool isEmpty() {
		return !(next_index);
	}
};