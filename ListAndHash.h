#pragma once
#include <exception>
#include <iostream>
enum TYPE_HASH_EXCEPTION {
	HASH_mem = 1,
	HASH_invalidInput,
	HASH_failure
};
class hashExceptions {
	TYPE_HASH_EXCEPTION type;
public:
	hashExceptions(TYPE_HASH_EXCEPTION type)
		:
		type(type)
	{}
	TYPE_HASH_EXCEPTION getType() {
		return type;
	}
};

template <class T, class S>
class List {
private:
	class Node {
	public:
		T* key;
		S* data;
		Node* next;
	public:
		Node(T* key_in, S* data_in) {
			key = nullptr;
			try {
				key = new T(*key_in);
				data = new S(*data_in);
			}
			catch (std::bad_alloc a) {
				if (key != nullptr)
					delete key;
				throw hashExceptions(HASH_mem);
			}
		}
		~Node() {
			delete key;
			delete data;
		}
		void setNext(Node* next_node) {
			next = next_node;
		}
		bool operator== (const Node& node2) const{
			return *key == *(node2.key);
		}
		bool isEqual(T* key_check) {
			return *key == *key_check;
		}
	}; //end Node. 
private:
	//List field : 
	int size;
	Node* source;

public:
	List() 
	:
	size(0),
	source(nullptr)
	{}
	

	//returns nullptr if not found.
	//returns the pointer to the original data inside the list.
	S* getData(T* key_search) {
		if (size == 0)
			return nullptr;
		Node* node_temp = source;
		while (node_temp != nullptr) {
			if (node_temp->isEqual(key_search))
				return node_temp->data;
			node_temp = node_temp->next;
		}
		//got here if not found.
		return nullptr;
	}

	void insertFirst( T* key_in, S* data_in) {
		Node* new_node = nullptr;
		try {
			new_node = new Node(key_in, data_in);
		}
		catch (std::bad_alloc a) {
			throw hashExceptions(HASH_mem);
		}
		new_node->setNext(source);
		source = new_node;
		size++;
	}

	void popFirst() {
		if (size == 0)
			return;
		Node* node_temp = source->next;
		delete source;
		source = node_temp;
		size--;
	}

    void pop(T* key_search) {
        if (size == 0)
            throw hashExceptions(HASH_failure);
        Node* last_node = source;
        if (last_node->isEqual(key_search)){
            popFirst();
            return;
        }
        if (size == 1)
            throw hashExceptions(HASH_failure);
        Node* node_temp = source->next;
        while (node_temp != nullptr) {
            if (node_temp->isEqual(key_search)){
                last_node->next = node_temp->next;
                Node* node_to_delete = node_temp;
                node_temp = node_temp->next;
                last_node = last_node->next;
                delete node_to_delete;
                return;
            }
            node_temp = node_temp->next;
        }
        //got here if not found.
        throw hashExceptions(HASH_failure);
    }

	T* getFirstKey() {
		return source->key;
	}

	S* getFirstData() {
		return source->data;
	}
	~List() {
		destroyAllNodes();
	}

	void destroyAllNodes() {
		while (size != 0) {
			popFirst();
		}
	}

	bool isEmpty() {
		return !size;
	}

	int getSize() {
		return size;
	}
	void print() {
		Node* curr_node = source;
		while (curr_node != nullptr) {
			//std::cout << *(curr_node->data) << " ";
			curr_node->data->print();
			curr_node = curr_node->next;
		}
	}
};	//end List

class HashFunc {
private:
	int n;
public:
	HashFunc(int nn) : n(nn) {}
	int operator() (int value) {
		return value % n;
	}
};

template < class T, class S, class FUN> 
class Hash {
private:
	FUN* hash_function_obj; //initiallized at the start.
	int len_curr;
	int size;
	List<T, S>** array_lists;

public:
	Hash(int size_hash, FUN hash_func) {
		hash_function_obj = new FUN (hash_func);
		size = 0;
		len_curr = size_hash;
		try {
			array_lists = new List<T, S>*[len_curr];
		}
		catch (std::bad_alloc a) {
			throw hashExceptions(HASH_mem);
		}
		for (int i = 0; i < len_curr; ++i) {
			try {
				array_lists[i] = new List<T, S>();
			}
			catch (std::bad_alloc a) {
				for (int j = 0; j < i; ++j) {
					delete array_lists[j];
				}
				delete[] array_lists;
				throw hashExceptions(HASH_mem);
			}
		}
	}

	~Hash() {
		for (int j = 0; j < len_curr; ++j) {
			delete array_lists[j];
		}
		delete[] array_lists;
		delete hash_function_obj;
	}

	void insert(T* key, S* data) {
		int key_hashed = (*hash_function_obj)(*key);
		if(array_lists[key_hashed]->getData(key) != nullptr)
			throw hashExceptions(HASH_failure);	
		try {
			array_lists[key_hashed]->insertFirst(key, data);
		}
		catch (std::bad_alloc a) {
			throw hashExceptions(HASH_mem);
		}
		size++;
		if (size >= (2 * len_curr)) {
			expand();
		}
	}

	void expand() {
		int new_length = 2 * len_curr;
		delete hash_function_obj;
		hash_function_obj = new FUN(new_length);
		List<T, S>** new_array = nullptr;
		try {
			new_array = new List<T, S>*[new_length];
		}
		catch (std::bad_alloc a) {
			throw hashExceptions(HASH_mem);
		}
		for (int i = 0; i < new_length; ++i) {
			try {
				new_array[i] = new List<T, S>();
			}
			catch (std::bad_alloc a) {
				for (int j = 0; j < i; ++j) {
					delete new_array[j];
				}
				delete[] new_array;
				throw hashExceptions(HASH_mem);
			}
		}
		size = 0;
		for (int i = 0; i < len_curr; ++i) {
			while (!(array_lists[i]->isEmpty())) {
				T* first_key = array_lists[i]->getFirstKey();
				S* first_data = array_lists[i]->getFirstData();
				int key_hashed = (*hash_function_obj)(*first_key);
				try {
					new_array[key_hashed]->insertFirst(first_key, first_data);
					array_lists[i]->popFirst();
					size++;
				}
				catch (std::bad_alloc a) {
					throw hashExceptions(HASH_mem);
				}
			}	
		}
		for (int j = 0; j < len_curr; ++j) {
			delete array_lists[j];
		}
		delete[] array_lists;
		len_curr = new_length;
		array_lists = new_array;
	}

    void pop(T* key) {
        int key_hashed = (*hash_function_obj)(*key);
        if(array_lists[key_hashed]->getData(key) == nullptr)
            throw hashExceptions(HASH_failure);
        array_lists[key_hashed]->pop(key);
        size--;
        if ( size <= len_curr/2) {
            if(len_curr > 2)
                contract();
        }
    }

    void contract() {
        int new_length = len_curr/2;
        delete hash_function_obj;
        hash_function_obj = new FUN(new_length);
        List<T, S>** new_array = nullptr;
        try {
            new_array = new List<T, S>*[new_length];
        }
        catch (std::bad_alloc a) {
            throw hashExceptions(HASH_mem);
        }
        for (int i = 0; i < new_length; ++i) {
            try {
                new_array[i] = new List<T, S>();
            }
            catch (std::bad_alloc a) {
                for (int j = 0; j < i; ++j) {
                    delete new_array[j];
                }
                delete[] new_array;
                throw hashExceptions(HASH_mem);
            }
        }
        size = 0;
        for (int i = 0; i < len_curr; ++i) {
            while (!(array_lists[i]->isEmpty())) {
                T* first_key = array_lists[i]->getFirstKey();
                S* first_data = array_lists[i]->getFirstData();
                int key_hashed = (*hash_function_obj)(*first_key);
                try {
                    new_array[key_hashed]->insertFirst(first_key, first_data);
                    array_lists[i]->popFirst();
                    size++;
                }
                catch (std::bad_alloc a) {
                    throw hashExceptions(HASH_mem);
                }
            }
        }
        for (int j = 0; j < len_curr; ++j) {
            delete array_lists[j];
        }
        delete[] array_lists;
        len_curr = new_length;
        array_lists = new_array;
    }

	//clan copy constructor need to pass the tree to the new clan and insert nullptr into the old clan.
	//also should pass the pointer to minheap inside.
	S* find(T* key) {
		int key_hashed = (*hash_function_obj)(*key);
		List<T, S>* curr_list = array_lists[key_hashed];
		return curr_list->getData(key); 
	}

	void print() {
		
		std::cout << "hash print : " << std::endl;
		for (int i = 0; i < len_curr; ++i) {
			std::cout << i << " : ";
			array_lists[i]->print();
			std::cout << std::endl;
		}
		std::cout << "hash print end" << std::endl;
	}
};




