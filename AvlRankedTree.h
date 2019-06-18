#pragma once
#include "Stack.h"
#include <exception>

enum TYPE_AVLR_EXCEPTION {
	AVLR_mem = 1,
	AVLR_invalidInput,
	AVLR_failure
};
class avlrExceptions {
	TYPE_AVLR_EXCEPTION type;
public:
	avlrExceptions(TYPE_AVLR_EXCEPTION type)
		:
		type(type)
	{}
	TYPE_AVLR_EXCEPTION getType() {
		return type;
	}
};


template<class S, class T>
class AvlRankedTree {
public:
	class Node {
	public:
		S* key;
		T* data;
		Node* left;
		Node* right;
		int height;
		int type;
		int size;
		int sum;
		int value;
	public:
		/*
		functions:
		*/
		Node(S* key_in = nullptr, T* data_in = nullptr, int value = 0, int type = 1)
			:
			key(nullptr),
			data(nullptr),
			left(nullptr),
			right(nullptr),
			height(0),
			type(type),
			size(1),
			sum(value),
			value(value)
		{
		    if(!key_in || !data_in){
		        return;
		    }
			try {
				if (type == 1) {
					key = new S(*key_in);
					data = new T(*data_in);
					//sum = new T(*data_in);
				}
				else {
					key = new S(*key_in);
					data = data_in;
					//sum = data_in;
				}
			}
			catch (std::bad_alloc a) {
				if (key != nullptr)
					delete key;
				throw avlrExceptions(AVLR_mem);
			}
		}

		void addValues(S* key_in, T* data_in, int value){
            try {
                if (type == 1) {
                    key = new S(*key_in);
                    data = new T(*data_in);
                    sum = value;
                    value = value;
                }
                else {
                    key = new S(*key_in);
                    data = data_in;
                }
            }
            catch (std::bad_alloc a) {
                if (key != nullptr)
                    delete key;
                throw avlrExceptions(AVLR_mem);
            }
		}

		~Node() {
			if (type == 1) {
				delete key;
				delete data;
				//delete sum;
			}
			else {
				delete key;
				data = nullptr;
			//	sum = nullptr;
			}
		}

		Node(Node& node) {
			try {
				key = nullptr;
				data = nullptr;
				if (node.type == 1) {
					key = new S(*node.key);
					data = new T(*node.data);
					//sum = new T(*node.sum);
				}
				else {
					key = new S(*node.key);
					data = node.data;
					//sum = node.sum;
				}
				left = nullptr;
				right = nullptr;
				height = 0;
				type = node.type;
				size = 0;
				sum = 0;
                value = 0;
			}
			catch (std::bad_alloc a) {
				if (key != nullptr)
					delete key;
				throw avlrExceptions(AVLR_mem);
			}
		}

		void insertSonL(Node* new_node) {
			left = new_node;
		}

		void insertSonL(S* key_in, T* data_in) {
			try {
				Node* temp_node = new Node(key_in, data_in);
				insertSonL(temp_node);
			}
			catch (std::bad_alloc a) {
				throw avlrExceptions(AVLR_mem);
			}
		}

		void insertSonR(Node* new_node) {
			right = new_node;
		}

		void insertSonR(S* key_in, T* data_in) {
			try {
				Node* temp_node = new Node(key_in, data_in);
				insertSonR(temp_node);
			}
			catch (std::bad_alloc a) {
				throw avlrExceptions(AVLR_mem);
			}
		}
		bool operator<(const Node& node2) {
			return *key < *node2.key;
		}

		bool operator==(const Node& node2) {
			return !(*key < *node2.key || *node2.key < *key);
		}
		T* getData() {
			return data;
		}
        S* getKey() {
            return key;
        }

		//returns balance factor of a node.
		int getBFF() {
			if (left == nullptr && right == nullptr) {
				return 0;
			}
			if (left == nullptr)
				return (-1)*((*right).height + 1);
			if (right == nullptr)
				return ((*left).height + 1);
			return ((*left).height + 1) - ((*right).height + 1);
		}
		void updateHeight() {
			Node* curr_r = right;
			Node* curr_l = left;
			if (curr_l == nullptr && curr_r == nullptr) {
				height = 0;
			}
			else if (curr_l == nullptr) {
				height = (*curr_r).height + 1;
			}
			else if (curr_r == nullptr) {
				height = (*curr_l).height + 1;
			}
			else {
				height = max((*curr_l).height, (*curr_r).height) + 1;
			}
		}
		int max(int num1, int num2) {
			return num1 > num2 ? num1 : num2;
		}
		void updateSizeAndSum() {
			Node* curr_r = right;
			Node* curr_l = left;
			if (curr_l == nullptr && curr_r == nullptr) {
				size = 1;
				sum = value;
			}
			else if (curr_l == nullptr) {
				size = (*curr_r).size + 1;
				sum = ((*curr_r).sum) + value;
			}
			else if (curr_r == nullptr) {
				size = (*curr_l).size + 1;
				sum = ((*curr_l).sum) + value;
			}
			else {
				size = (*curr_l).size + (*curr_r).size + 1;
				sum = ((*curr_l).sum) + ((*curr_r).sum) + value;
			}
		}
	};
private:
	int size;
	Node* source;
	int type;
	// type = 1 means the tree holds objects which have operator<.
	// type = 2 means the tree holds pointer to objects which have operator<.

public:
	/*
	functions of the tree:
	*/

	AvlRankedTree<S, T>(int type)
		:
		size(0),
		source(nullptr),
		type(type)
	{}

    AvlRankedTree<S, T>(Node** array_of_nodes, int size, int type)
    	:
    	size(0),
    	type(type) {
    	if (array_of_nodes == nullptr) {
    		source = nullptr;
    		this->size = 0;
    		return;
    	}
    	source = rec_connect_nodes(array_of_nodes, 0, size - 1);
    	updateSums();
    	this->size = size;
    }

	~AvlRankedTree<S, T>() {
		//go in post order and distroy every node.
		//postOrder(source, operator delete[]);
		destroyAllNodes(source);
	}
	void destroyAllNodes(Node* current) {
		if (current == nullptr)
			return;
		destroyAllNodes((*current).left);
		destroyAllNodes((*current).right);
		delete current;
	}

    ///*
    //throws exceptions:
    //* memory allocation fail (object from standard library).
    //* allready in.
    //*
    //*/
    void insert(S* key, T* data, int value) {
        //firstly, have to find the location for insert.
        //in case the key is found in the tree throw exception.
        //create a new node with the given information.
        //connect the node to it's father.
        //update the height of each node on the path from the source.
        ///////optional : create a function which update the path back to the source (height update).
        Node* new_node = nullptr;
        try {
            new_node = new Node(key, data, value, type);
        }
        catch (std::bad_alloc a) {
            throw avlrExceptions(AVLR_mem);
        }
        if (source == nullptr && size == 0) {
            source = new_node;
            size++;
            return;
        }
        Stack<Node*>* stack = nullptr;
        try {
            stack = new Stack<Node*>(100); //local stack shouldn't be deleted.
        }
        catch (stackExceptions &a) {
            if (a.getType() == STACK_mem) {
                delete new_node;
                throw avlrExceptions(AVLR_mem);
            }
        }
        Node* temp_node = source;
        bool finish_search = false;
        while (!finish_search) {
            if (*temp_node == *new_node) {
                delete stack;
                delete new_node;
                throw avlrExceptions(AVLR_failure);
            }
            //go left son.
            if (*new_node < *temp_node) {
                if ((*temp_node).left == nullptr) {
                    (*temp_node).insertSonL(new_node);
                    ++size;
                    try {
                        stack->push(temp_node);
                        stack->push(new_node);
                    }
                    catch (stackExceptions &a) {
                        if (a.getType() == STACK_mem) {
                            delete stack;
                            delete new_node;
                            throw avlrExceptions(AVLR_mem);
                        }
                    }
                    finish_search = true;
                    continue;
                }
                else {
                    try {
                        stack->push(temp_node);
                    }
                    catch (stackExceptions &a) {
                        if (a.getType() == STACK_mem) {
                            delete stack;
                            delete new_node;
                            throw avlrExceptions(AVLR_mem);
                        }
                    }
                    temp_node = (*temp_node).left;
                }
            }

            //go right son.
            if (*temp_node < *new_node) {
                if ((*temp_node).right == nullptr) {
                    (*temp_node).insertSonR(new_node);
                    ++size;
                    try {
                        stack->push(temp_node);
                        stack->push(new_node);
                    }
                    catch (stackExceptions &a) {
                        if (a.getType() == STACK_mem) {
                            delete stack;
                            delete new_node;
                            throw avlrExceptions(AVLR_mem);
                        }
                    }

                    finish_search = true;
                    continue;
                }
                else {
                    try {
                        stack->push(temp_node);
                    }
                    catch (stackExceptions &a) {
                        if (a.getType() == STACK_mem) {
                            delete stack;
                            delete new_node;
                            throw avlrExceptions(AVLR_mem);
                        }
                    }
                    temp_node = (*temp_node).right;
                }
            }
        }
        //if got here we inserted successfully a new node to the tree and should update heights.
        stack->top()->updateSizeAndSum();
        stack->pop();
        updateHeights(*stack);
        delete stack;
        //alse activate the rolls whenever a node with a bigger then |1| height is found.
    }

    void updateSums() {
        updateSumsRec(source);
    }

    void /*int*/ updateSumsRec(Node* source) {
            //if (!source)
            //    return 0;
            //if(!(source->left)&&!(source->right)) {
            //    source->sum = source->value;
            //    return source->value;
            //}
            //source->sum = updateSumsRec(source->left) + updateSumsRec(source->right) + source->value;
            //return source->sum;

            if(!source)
                return;
            updateSumsRec(source->left);
            updateSumsRec(source->right);
            (*source).updateSizeAndSum();
    }

    void updateHeights(Stack<Node*>& stack) {
        while (!stack.isEmpty()) {
            Node* curr_node = stack.top();
            curr_node -> updateSizeAndSum();
            Node* left_son = (*curr_node).left;
            Node* right_son = (*curr_node).right;
            int balance_factor = curr_node->getBFF();
            if (balance_factor == 2) {
                stack.pop();
                Node* papa = source;
                if (!stack.isEmpty())
                    papa = stack.top();
                if (left_son->getBFF() == -1)
                    dodoLR(curr_node, papa);
                else if (left_son->getBFF() >= 0)
                    dodoLL(curr_node, papa);
            }
            else if (balance_factor == -2) {
                stack.pop();
                Node* papa = source;
                if (!stack.isEmpty())
                    papa = stack.top();
                if (right_son->getBFF() <= 0)
                    dodoRR(curr_node, papa);
                else if (right_son->getBFF() == 1)
                    dodoRL(curr_node, papa);
            }
            else {
                curr_node->updateHeight();
                curr_node->updateSizeAndSum();
                stack.pop();
            }

        }
    }


    ///*
    //function returns the minimal key in the tree.
    //*/
    ////S& min() {
    ////	return S();
    ////}
    ////const S& min() const {
    ////	return S();
    ////}
    //
    ///*
    //function going throw the tree and activates the function on each
    //intersection in the the traveling order (in order).
    //when inserting a FUN object , we should allocate the object with new operator outside.
    //and make sure we delete it afterwards.
    ////*/
    //template<class FUN>
    //void preOrder(FUN& g_function) {
    //	preOrderCover<FUN>(source, g_function);
    //}
    //
    //template<class FUN>
    //void preOrderCover(Node* starting_node, FUN& g_function) {
    //	if (starting_node == nullptr)
    //		return;
    //	g_function.doSomething(*((*starting_node).data));
    //	preOrderCover((*starting_node).left, g_function);
    //	preOrderCover((*starting_node).right, g_function);
    //}
    //
    template<class FUN>
    void inOrder(FUN& g_function) {
        inOrderCover<FUN>(source, g_function);
    }
    //
    template<class FUN>
    void inOrderCover(Node* starting_node, FUN& g_function) {
        if (starting_node == nullptr)
            return;
        inOrderCover((*starting_node).left, g_function);
        g_function.doSomething(*((*starting_node).key), *((*starting_node).data));
        inOrderCover((*starting_node).right, g_function);
    }
    //
    //template<class FUN>
    //void postOrder(FUN& g_function) {
    //	postOrderCover<FUN>(source, g_function);
    //}
    //
    //template<class FUN>
    //void postOrderCover(Node* starting_node, FUN& g_function) {
    //	if (starting_node == nullptr)
    //		return;
    //	postOrderCover((*starting_node).left, g_function);
    //	postOrderCover((*starting_node).right, g_function);
    //	g_function.doSomething(*((*starting_node).data));
    //}
    //
    void pop(S* key) {
        //find the node. (might throw exception if didnt found it)
        // temp implemention of connecting the father with one of the sons and the second son connected to the first son.
        //later on will change it to avl implemention.
        if (source == nullptr && size == 0) {
            //throw exception.
        }

        Stack<Node*>* stack = nullptr;
        try {
            stack = new Stack<Node*>(100); //local stack shouldn't be deleted.
        }
        catch (stackExceptions &a) {
            if (a.getType() == STACK_mem)
                throw avlrExceptions(AVLR_mem);
        }

        Node* temp_node = findNode(key, stack);
        if (!temp_node)
        {
            //throw exception
        }

        if (isLeaf(temp_node)) {
            if (*temp_node == *source) {
                source = nullptr;
            }
            else {
                Node* father_node = (*stack).top();
                if (((*father_node).left) == temp_node)
                    (*father_node).left = nullptr;
                else
                    (*father_node).right = nullptr;
            }
            size--;//great success.
            delete temp_node;//great success.
        }
        else if (isHalfLeaf(temp_node)) {

            Node* son_of_target_node = (*temp_node).left == nullptr ? (*temp_node).right : (*temp_node).left;
            if (*temp_node == *source) {
                source = son_of_target_node;
            }
            else {
                Node* father_node = (*stack).top();
                if (((*father_node).left) == temp_node)
                    (*father_node).left = son_of_target_node;
                else
                    (*father_node).right = son_of_target_node;
            }
            size--;
            delete temp_node;//great success.
        }
        else {//option 3 (have two sons).
            try {
                findNodeToReplace(temp_node, stack);
            }
            catch (avlrExceptions &a) {
                delete stack;
                delete temp_node;
                if (a.getType() == AVLR_mem)
                    throw avlrExceptions(AVLR_mem);
            }
            Node* new_traget_node = stack->top();

            //swap
            S* key_temp = (*new_traget_node).key;
            T* data_temp = (*new_traget_node).data;
        //	T* sum_temp = (*new_traget_node).sum;
            (*new_traget_node).key = (*temp_node).key;
            (*new_traget_node).data = (*temp_node).data;
        //	(*new_traget_node).sum = (*temp_node).sum;
            (*temp_node).key = key_temp;
            (*temp_node).data = data_temp;
        //	(*temp_node).sum = sum_temp;
            //switch key and data occoured.

            stack->pop();
            Node* fathers_little_boy = stack->top();

            if ((*fathers_little_boy).right == new_traget_node)
                (*fathers_little_boy).right = (*new_traget_node).right;
            else
                (*fathers_little_boy).left = (*new_traget_node).right;


            size--;
            delete new_traget_node; //great success.
        }
        ////now we got a stack to the father of the deleted node.
        ////we can activate function for updating heights and rolls.
        //////need to do stuff.
        //
        ////massage for eitan with memory issues.
        updateHeights(*stack);
        delete stack;
    }

    ////find the first node which have no left son, might be a leaf .
    void findNodeToReplace(Node* node_to_destroy, Stack<Node*>* stack) {
        stack->push(node_to_destroy);
        Node* temp_node = (*node_to_destroy).right; //right son.
        try {
            stack->push(temp_node);
        }
        catch (stackExceptions &a) {
            if (a.getType() == STACK_mem)
                throw avlrExceptions(AVLR_mem);
        }
        while ((*temp_node).left) {
            temp_node = (*temp_node).left;
            try {
                stack->push(temp_node);
            }
            catch (stackExceptions &a) {
                if (a.getType() == STACK_mem)
                    throw avlrExceptions(AVLR_mem);
            }
        }
    }

    bool isLeaf(Node* node) {
        return ((*node).left == nullptr && (*node).right == nullptr);
    }
    ////activated after isLeaf is failed.
    ////return true if at least one of the sons is nullptr.
    ////when activated after isLeaf we are sure that it is halfLeaf.
    bool isHalfLeaf(Node* node) {
        return ((*node).left == nullptr || (*node).right == nullptr);
    }

    int getSize() {
        return size;
    }
    //
    Node* findNode(S* key, Stack<Node*>* stack)
    {
        if (source == nullptr && size == 0) {
            throw avlrExceptions(AVLR_failure);
            return nullptr;
        }
        Node* node_to_find = nullptr;
        try {
            node_to_find = new Node(key, (*source).getData(), -1, type);
        }
        catch (std::bad_alloc a) {
            throw avlrExceptions(AVLR_mem);
        }

        Node* temp_node = source;
        bool finish_search = false;
        while (!finish_search) {
            if ((temp_node == nullptr) || (*temp_node == *node_to_find)) {
                finish_search = true;
                continue;
            }
            //go left son.
            if (*node_to_find < *temp_node) {
                try {
                    stack->push(temp_node);
                }
                catch (stackExceptions &a) {
                    if (a.getType() == STACK_mem) {
                        throw avlrExceptions(AVLR_mem);
                    }
                }
                temp_node = (*temp_node).left;
                continue;
            }
            //go right son.
            else {
                try {
                    stack->push(temp_node);
                }
                catch (stackExceptions &a) {
                    if (a.getType() == STACK_mem) {
                        throw avlrExceptions(AVLR_mem);
                    }
                }
                temp_node = (*temp_node).right;
                continue;
            }
        }//end of while.
        delete node_to_find;
        if (temp_node == nullptr) {
            throw avlrExceptions(AVLR_failure);
        }
        return temp_node;
    }

    T* find(S* key) {
        Stack<Node*>* stack = nullptr;
        try {
            stack = new Stack<Node*>(100); //local stack shouldn't be deleted.
        }
        catch (stackExceptions &a) {
            if (a.getType() == STACK_mem) {
                throw avlrExceptions(AVLR_mem);
            }
        }
        Node* temp_node = nullptr;
        try {
            temp_node = findNode(key, stack);
        }
        catch (avlrExceptions &a) {
            if (a.getType() == AVLR_failure) {
                delete stack;
                throw avlrExceptions(AVLR_failure);
            }
        }


        //if (temp_node != nullptr) {
        //	try {
        //		data = new T(*((*temp_node).data));
        //	}
        //	catch (std::bad_alloc a) {
        //		delete stack;
        //		throw avlrExceptions(AVLR_mem);
        //	}
        //}
        T* data = nullptr;
        if (temp_node != nullptr)
            data = ((*temp_node).data);
        delete stack;
        return data;
    }

    ////in case unbalanced node is source we should assign papa with source too.
    void dodoLL(Node* unbalanced_node, Node* papa) {
        //remember :
        Node* un_bal_l = (*unbalanced_node).left;
        Node* un_bal_l_r = (*un_bal_l).right;
        //reconnect:
        (*unbalanced_node).left = un_bal_l_r;
        (*un_bal_l).right = unbalanced_node;
        //deal with papa he might be source.
        if (*papa == *unbalanced_node)
            source = un_bal_l;
        else if ((*papa).right == unbalanced_node)
            (*papa).right = un_bal_l;
        else
            (*papa).left = un_bal_l;
        unbalanced_node->updateHeight();
        unbalanced_node->updateSizeAndSum();
        un_bal_l->updateHeight();
        un_bal_l->updateSizeAndSum();
    }

    void dodoRR(Node* unbalanced_node, Node* papa) {
        //remember :
        Node* un_bal_r = (*unbalanced_node).right;
        Node* un_bal_r_l = (*un_bal_r).left;
        //reconnect:
        (*unbalanced_node).right = un_bal_r_l;
        (*un_bal_r).left = unbalanced_node;
        //deal with papa he might be source.
        if (*papa == *unbalanced_node)
            source = un_bal_r;
        else if ((*papa).right == unbalanced_node)
            (*papa).right = un_bal_r;
        else
            (*papa).left = un_bal_r;
        unbalanced_node->updateHeight();
        unbalanced_node->updateSizeAndSum();
        un_bal_r->updateHeight();
        un_bal_r->updateSizeAndSum();
    }

    void dodoLR(Node* unbalanced_node, Node* papa) {
        Node* org_un_l = (*unbalanced_node).left;
        dodoLL(unbalanced_node, papa);
        dodoLL(unbalanced_node, org_un_l);
        if (org_un_l == source)
            papa = source;
        dodoRR(org_un_l, papa);
    }

    void dodoRL(Node* unbalanced_node, Node* papa) {
        Node* org_un_r = (*unbalanced_node).right;
        //need to check which functions to activate .
        dodoRR(unbalanced_node, papa);
        dodoRR(unbalanced_node, org_un_r);
        if (org_un_r == source)
            papa = source;
        dodoLL(org_un_r, papa);
    }

    bool isAvl() {
        return  preOrderAvlCheck(source);
    }


    bool preOrderAvlCheck(Node* starting_node) {
        if (starting_node == nullptr)
            return true;
        int bf = starting_node->getBFF();
        if (bf >= 2 || bf <= -2) {
            return false;
        }
        return  preOrderAvlCheck((*starting_node).left)
            && preOrderAvlCheck((*starting_node).right);
    }

    Node* rec_connect_nodes(Node** array_of_nodes, int l, int r) {
        if (l > r) {
            return nullptr;
        }
        int m = (l + r) / 2;
        Node* curr_in_array = array_of_nodes[m];
        Node* curr_source = nullptr;
        try {
            curr_source = new Node(*curr_in_array);
        }
        catch (std::bad_alloc a) {
            throw avlrExceptions(AVLR_mem);
        }
        try {
            (*curr_source).left = rec_connect_nodes(array_of_nodes, l, m - 1);
            (*curr_source).right = rec_connect_nodes(array_of_nodes, m + 1, r);
            curr_source->updateHeight();
            curr_source->size = r - l;
        }
        catch (avlrExceptions &a) {
            delete curr_source;
            if (a.getType() == AVLR_mem)
                throw avlrExceptions(AVLR_mem);
        }
        return curr_source;
    }

    template<class FUN>
    void inOrderNodes(FUN& g_function) {
        inOrderNodesCover<FUN>(source, g_function);
    }

    template<class FUN>
    void inOrderNodesCover(Node* starting_node, FUN& g_function) {
        if (starting_node == nullptr)
            return;
        inOrderNodesCover((*starting_node).left, g_function);
        g_function.doSomething(starting_node);
        inOrderNodesCover((*starting_node).right, g_function);
    }

    int getSumScoresOfCourse(int numGroups) {
        if (numGroups >= size)
            return source->sum;
        int sum = 0;
        getSumScoresOfCourseRecursive(source, numGroups, &sum);
        return sum;
    }

    void getSumScoresOfCourseRecursive(Node* source, int numGroups, int* sum){
        if (source->size <= numGroups){
            *sum += source->sum;
            return;
        }
        Node* right = source->right;
        Node* left = source->left;
        if (right->size > numGroups){
            getSumScoresOfCourseRecursive(right, numGroups, sum);
            return;
        }
        if (right->size == numGroups){
            *sum += right->sum;
            return;
        }
        if (right->size == numGroups-1){
            *sum += source->sum;
            *sum -= left->sum;
            return;
        }
        if (right->size < numGroups){
            *sum += source->sum;
            *sum -= left->sum;
            getSumScoresOfCourseRecursive(source->left, numGroups - source->size + left->size, sum);
            return;
        }
    }
	
};