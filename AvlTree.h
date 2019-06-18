#pragma once
#ifndef AVLTREE_H
#define AVLTREE_H
#include "Stack.h"
#include <exception>
#include <iostream>

enum TYPE_AVL_EXCEPTION {
    AVL_mem = 1,
    AVL_invalidInput,
    AVL_failure
};
class avlExceptions {
    TYPE_AVL_EXCEPTION type;
public:
    avlExceptions(TYPE_AVL_EXCEPTION type)
            :
            type(type)
    {}
    TYPE_AVL_EXCEPTION getType() {
        return type;
    }
};


template<class S, class T>
class AvlTree {
public:
    class Node {
    public:
        S* key;
        T* data;
        Node* left;
        Node* right;
        int height;
        int type;
    public:
        /*
            functions:
        */
        Node(S* key_in, T* data_in, int type)
                :
                key(NULL),
                data(NULL),
                left(nullptr),
                right(nullptr),
                height(0),
                type(type)
        {
            try {
                if (type == 1) {
                    key = new S(*key_in);
                    data = new T(*data_in);
                }
                else if (type == 2){
                    key = new S(*key_in);
                    data = data_in;
                }
                else{
                    key = key_in;
                    data = data_in;
                }

            }
            catch (std::bad_alloc a){
                if (key != NULL)
                    delete key;
                throw avlExceptions(AVL_mem);
            }
        }

        ~Node() {
            if (type == 1) {
                delete key;
                delete data;
            }
            else {
                delete key;
                if(data != NULL) {
                    delete data;
                    data = NULL;
                }
                else
                    data = NULL;
            }
        }

        Node(Node& node) {
            try {
                key = NULL;
                data = NULL;

                if (node.type == 1) {
                    key = new S(*node.key);
                    data = new T(*node.data);
                }
                else if (node.type == 2) {
                    key = new S(*node.key);
                    data = node.data;
                }
                else{
                    key = node.key;
                    data = node.data;
                }
                left = NULL;
                right = NULL;
                height = 0;

                type = node.type;
            }
            catch (std::bad_alloc a) {
                if (key != NULL)
                    delete key;
                throw avlExceptions(AVL_mem);
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
                throw avlExceptions(AVL_mem);
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
                throw avlExceptions(AVL_mem);
            }
        }

        bool operator<(const Node& node2) {
            //if(type == 1) {
            //    if (*key > *node2.key)
            //        return false;
            //    if (*key < *node2.key)
            //        return true;
            //    return *data < *node2.data;
            //}
            return *key < *node2.key;
        }

        bool operator==(const Node& node2) {
            if(type == 1)
                return *key == *node2.key && *data == *node2.data;
            return *key == *node2.key;
            //return !(*key < *node2.key || *node2.key < *key);
        }

        T* getData() {
            return data;
        }

       ////new - 11/5/19*****************************************************************
        S* getKey() {
            return key;
        }
        //new - 11/5/19*****************************************************************
        void changeData(T* new_data)
        {
            if (type == 1) {
                delete data;
                data = new T(*new_data);
            }
            else {
                data = new_data;
            }
        }
        //new - 14/5/19*****************************************************************
        int getType() {
            return type;
        }

        //returns balance factor of a node.
        int getBFF() {
            if (left == NULL && right == NULL) {
                return 0;
            }
            if (left == NULL)
                return (-1)*((*right).height + 1);
            if (right == NULL)
                return ((*left).height + 1);
            return ((*left).height + 1) - ((*right).height + 1);
        }
        void updateHeight() {
            Node* curr_r = right;
            Node* curr_l = left;
            if (curr_l == NULL && curr_r == NULL) {
                height = 0;
            }
            else if (curr_l == NULL) {
                height = (*curr_r).height + 1;
            }
            else if (curr_r == NULL) {
                height = (*curr_l).height + 1;
            }
            else {
                height = max((*curr_l).height, (*curr_r).height) + 1;
            }
        }
        int max(int num1, int num2) {
            return num1 > num2 ? num1 : num2;
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

    AvlTree<S, T>(int type)
            :
            size(0),
            source(NULL),
            type(type)
    {}

    AvlTree<S, T>(Node** array_of_nodes, int size, int type=1)
            :
            size(0),
            type(type) {
        if (array_of_nodes == NULL) {
            source = NULL;
            this->size = 0;
            return;
        }
        source = rec_connect_nodes(array_of_nodes, 0, size - 1);
        this->size = size;

    }

    ~AvlTree<S, T>() {
        //go in post order and distroy every node.
        //postOrder(source, operator delete[]);
        if(source)
            destroyAllNodes(source);
    }

    bool operator==(const AvlTree<S, T>& tree2) {
        return false;
    }
    bool operator<(const AvlTree<S, T>& tree2) {
        return false;
    }

    void destroyAllNodes(Node* current) {
        if (current == NULL)
            return;
        destroyAllNodes((*current).left);
        destroyAllNodes((*current).right);
        delete current;
    }

    /*
        throws exceptions:
            * memory allocation fail (object from standard library).
            * allready in.
            *
    */
    void insert(S* key, T* data) {
        //firstly, have to find the location for insert.
        //in case the key is found in the tree throw exception.
        //create a new node with the given information.
        //connect the node to it's father.
        //update the height of each node on the path from the source.
        ///////optional : create a function which update the path back to the source (height update).
        Node* new_node = nullptr;
        try {
            new_node = new Node(key, data, type);
        }
        catch (std::bad_alloc a) {
            throw avlExceptions(AVL_mem);
        }

        if (source == NULL && size == 0) {
            source = new_node;
            size++;
            return;
        }

        Stack<Node*>* stack = NULL;
        try {
            stack = new Stack<Node*>(100); //local stack shouldn't be deleted.
        }
        catch (stackExceptions &a) {
            if (a.getType() == STACK_mem) {
                delete new_node;
                throw avlExceptions(AVL_mem);
            }
        }
        Node* temp_node = source;
        bool finish_search = false;
        while (!finish_search) {
            if (*new_node == *temp_node) {
                delete stack;
                delete new_node;
                throw avlExceptions(AVL_failure);
            }
            //go left son.

            if (*new_node < *temp_node) {
                if ((*temp_node).left == NULL) {
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
                            throw avlExceptions(AVL_mem);
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
                            throw avlExceptions(AVL_mem);
                        }
                    }
                    temp_node = (*temp_node).left;
                }
            }
            //go right son.
            if (*temp_node < *new_node) {
                if ((*temp_node).right == NULL) {
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
                            throw avlExceptions(AVL_mem);
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
                            throw avlExceptions(AVL_mem);
                        }
                    }
                    temp_node = (*temp_node).right;
                }
            }
        }
        //if got here we inserted successfully a new node to the tree and should update heights.
        stack -> pop();
        updateHeights(*stack);
        delete stack;
        //alse activate the rolls whenever a node with a bigger then |1| height is found.
    }


    void updateHeights(Stack<Node*>& stack) {
        while (!stack.isEmpty()) {
            Node* curr_node = stack.top();
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
                stack.pop();
            }

        }
    }


    /*
        function returns the minimal key in the tree.
    */
    //S& min() {
    //	return S();
    //}
    //const S& min() const {
    //	return S();
    //}

    /*
        function going throw the tree and activates the function on each
        intersection in the the traveling order (in order).
        when inserting a FUN object , we should allocate the object with new operator outside.
        and make sure we delete it afterwards.
    //*/
    template<class FUN>
    void preOrder(FUN& g_function) {
        preOrderCover<FUN>(source, g_function);
    }

    template<class FUN>
    void preOrderCover(Node* starting_node, FUN& g_function) {
        if (starting_node == NULL)
            return;
        g_function.doSomething(*((*starting_node).data));
        preOrderCover((*starting_node).left, g_function);
        preOrderCover((*starting_node).right, g_function);
    }

    template<class FUN>
    void inOrder(FUN& g_function) {
        inOrderCover<FUN>(source, g_function);
    }

    template<class FUN>
    void inOrderCover(Node* starting_node, FUN& g_function) {
        if (starting_node == NULL)
            return;
        inOrderCover((*starting_node).left, g_function);
        g_function.doSomething(*((*starting_node).key), *((*starting_node).data));
        inOrderCover((*starting_node).right, g_function);
    }

    template<class FUN>
    void postOrder(FUN& g_function) {
        postOrderCover<FUN>(source, g_function);
    }

    template<class FUN>
    void postOrderCover(Node* starting_node, FUN& g_function) {
        if (starting_node == NULL)
            return;
        postOrderCover((*starting_node).left, g_function);
        postOrderCover((*starting_node).right, g_function);
        g_function.doSomething(*((*starting_node).data));
    }

    void pop(S* key) {
        //find the node. (might throw exception if didnt found it)
        // temp implemention of connecting the father with one of the sons and the second son connected to the first son.
        //later on will change it to avl implemention.
        if (source == NULL && size == 0) {
            //throw exception.
        }

        Stack<Node*>* stack = NULL;
        try {
            stack = new Stack<Node*>(100); //local stack shouldn't be deleted.
        }
        catch (stackExceptions &a) {
            if(a.getType() == STACK_mem)
                throw avlExceptions(AVL_mem);
        }

        Node* temp_node = findNode(key, stack);
        if (!temp_node)
        {
            //throw exception
        }

        if (isLeaf(temp_node)) {
            if (*temp_node == *source) {
                source = NULL;
            }
            else {
                Node* father_node = (*stack).top();
                if (((*father_node).left) == temp_node)
                    (*father_node).left = NULL;
                else
                    (*father_node).right = NULL;
            }
            size--;//great success.
            delete temp_node;//great success.
        }
        else if (isHalfLeaf(temp_node)) {

            Node* son_of_target_node = (*temp_node).left == NULL ? (*temp_node).right : (*temp_node).left;
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
            catch (avlExceptions &a) {
                delete stack;
                delete temp_node;
                if(a.getType() == AVL_mem)
                    throw avlExceptions(AVL_mem);
            }
            Node* new_traget_node = stack->top();

            //swap
            S* key_temp = (*new_traget_node).key;
            T* data_temp = (*new_traget_node).data;
            (*new_traget_node).key = (*temp_node).key;
            (*new_traget_node).data = (*temp_node).data;
            (*temp_node).key = key_temp;
            (*temp_node).data = data_temp;
            //switch key and data occoured.

            stack->pop();
            Node* fathers_little_boy = stack->top();

            if ( (*fathers_little_boy).right == new_traget_node)
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

    void pop(S* key, T* data) {
        //find the node. (might throw exception if didnt found it)
        // temp implemention of connecting the father with one of the sons and the second son connected to the first son.
        //later on will change it to avl implemention.
        if (source == NULL && size == 0) {
            //throw exception.
        }

        Stack<Node*>* stack = NULL;
        try {
            stack = new Stack<Node*>(100); //local stack shouldn't be deleted.
        }
        catch (stackExceptions &a) {
            if(a.getType() == STACK_mem)
                throw avlExceptions(AVL_mem);
        }

        Node* temp_node = findNode(key, data, stack);
        if (!temp_node)
        {
            //throw exception
        }

        if (isLeaf(temp_node)) {
            if (*temp_node == *source) {
                source = NULL;
            }
            else {
                Node* father_node = (*stack).top();
                if (((*father_node).left) == temp_node)
                    (*father_node).left = NULL;
                else
                    (*father_node).right = NULL;
            }
            size--;//great success.
            delete temp_node;//great success.
        }
        else if (isHalfLeaf(temp_node)) {

            Node* son_of_target_node = (*temp_node).left == NULL ? (*temp_node).right : (*temp_node).left;
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
            catch (avlExceptions &a) {
                delete stack;
                delete temp_node;
                if(a.getType() == AVL_mem)
                    throw avlExceptions(AVL_mem);
            }
            Node* new_traget_node = stack->top();

            //swap
            S* key_temp = (*new_traget_node).key;
            T* data_temp = (*new_traget_node).data;
            (*new_traget_node).key = (*temp_node).key;
            (*new_traget_node).data = (*temp_node).data;
            (*temp_node).key = key_temp;
            (*temp_node).data = data_temp;
            //switch key and data occoured.

            stack->pop();
            Node* fathers_little_boy = stack->top();

            if ( (*fathers_little_boy).right == new_traget_node)
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
    //find the first node which have no left son, might be a leaf .
    void findNodeToReplace(Node* node_to_destroy, Stack<Node*>* stack) {
        stack->push(node_to_destroy);
        Node* temp_node = (*node_to_destroy).right; //right son.
        try {
            stack->push(temp_node);
        }
        catch (stackExceptions &a) {
            if (a.getType() == STACK_mem)
                throw avlExceptions(AVL_mem);
        }
        while ((*temp_node).left) {
            temp_node = (*temp_node).left;
            try {
                stack->push(temp_node);
            }
            catch (stackExceptions &a) {
                if (a.getType() == STACK_mem)
                    throw avlExceptions(AVL_mem);
            }
        }
    }

    bool isLeaf(Node* node) {
        return ((*node).left == NULL && (*node).right == NULL);
    }
    //activated after isLeaf is failed.
    //return true if at least one of the sons is NULL.
    //when activated after isLeaf we are sure that it is halfLeaf.
    bool isHalfLeaf(Node* node) {
        return ((*node).left == NULL || (*node).right == NULL);
    }

    int getSize() {
        return size;
    }

    Node* findNode(S* key, Stack<Node*>* stack)
    {
        if (source == NULL && size == 0) {
            throw avlExceptions(AVL_failure);
        }
        //Node* node_to_find = NULL;
        //try {
        //    node_to_find = new Node(key, (*source).getData(), type);
        //}
        //catch (std::bad_alloc a) {
        //    throw avlExceptions(AVL_mem);
        //}

        Node* temp_node = source;
        bool finish_search = false;
        while (!finish_search) {
            if ((temp_node == NULL) || (*((*temp_node).key) == *key) ){
                finish_search = true;
                continue;
            }
            //go left son.
            if (*key < (*((*temp_node).key)) ){
                try {
                    stack->push(temp_node);
                }
                catch (stackExceptions &a) {
                    if (a.getType() == STACK_mem) {
                        throw avlExceptions(AVL_mem);
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
                        throw avlExceptions(AVL_mem);
                    }
                }
                temp_node = (*temp_node).right;
                continue;
            }
        }//end of while.
        //delete node_to_find; //***********************************************************************************
        if (temp_node == NULL) {
            throw avlExceptions(AVL_failure);
        }
        return temp_node;
    }

    Node* findNode(S* key, T* data, Stack<Node*>* stack)
    {
        if (source == NULL && size == 0) {
            throw avlExceptions(AVL_failure);
            return NULL;
        }
        Node* node_to_find = NULL;
        try {
            node_to_find = new Node(key, data, type);
        }
        catch (std::bad_alloc a) {
            throw avlExceptions(AVL_mem);
        }

        Node* temp_node = source;
        bool finish_search = false;
        while (!finish_search) {
            if ((temp_node == NULL) || (*temp_node == *node_to_find)) {
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
                        throw avlExceptions(AVL_mem);
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
                        throw avlExceptions(AVL_mem);
                    }
                }
                temp_node = (*temp_node).right;
                continue;
            }
        }//end of while.
        delete node_to_find;
        if (temp_node == NULL) {
            throw avlExceptions(AVL_failure);
        }
        return temp_node;
    }

    T* find(S* key) {
        Stack<Node*>* stack = NULL;
        try {
            stack = new Stack<Node*>(100); //local stack shouldn't be deleted.
        }
        catch (stackExceptions &a) {
            if (a.getType() == STACK_mem) {
                throw avlExceptions(AVL_mem);
            }
        }
        Node* temp_node = NULL;
        try {
            temp_node = findNode(key, stack);
        }
        catch (avlExceptions &a) {
            if (a.getType() == AVL_failure) {
                delete stack;
                throw avlExceptions(AVL_failure);
            }
        }


        //if (temp_node != NULL) {
        //	try {
        //		data = new T(*((*temp_node).data));
        //	}
        //	catch (std::bad_alloc a) {
        //		delete stack;
        //		throw avlExceptions(AVL_mem);
        //	}
        //}
        T* data = NULL;
        if (temp_node != NULL)
            data = ((*temp_node).data);
        delete stack;
        return data;
    }

    //in case unbalanced node is source we should assign papa with source too.
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
        un_bal_l ->updateHeight();
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
        un_bal_r->updateHeight();
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
        if (starting_node == NULL)
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
            return NULL;
        }
        int m = (l + r) / 2;
        //std::cout << "hi1" << std::endl;

        Node* curr_in_array = array_of_nodes[m];
        Node* curr_source = NULL;
        try {
            //curr_source = curr_in_array;
            //std::cout << "type Array 2 = " << array_of_nodes[m]->getType() << std::endl;
            //std::cout << "type Node = " << curr_in_array->getType() << std::endl;

            curr_source = new Node(*curr_in_array); //old version
            //std::cout << "JAMUS" << std::endl;

        }
        catch (std::bad_alloc a) {
            throw avlExceptions(AVL_mem);
        }
        try {
            //std::cout << "hi3" << std::endl;

            (*curr_source).left = rec_connect_nodes(array_of_nodes, l, m - 1);
            (*curr_source).right = rec_connect_nodes(array_of_nodes, m + 1, r);
            curr_source->updateHeight();
        }
        catch (avlExceptions &a) {
            delete curr_source;
            if(a.getType() == AVL_mem)
                throw avlExceptions(AVL_mem);
        }
        return curr_source;
    }

    template<class FUN>
    void inOrderNodes(FUN& g_function) {
        inOrderNodesCover<FUN>(source, g_function);
    }

    template<class FUN>
    void inOrderNodesCover(Node* starting_node, FUN& g_function) {
        if (starting_node == NULL)
            return;
        inOrderNodesCover((*starting_node).left, g_function);
        g_function.doSomething(starting_node);
        inOrderNodesCover((*starting_node).right, g_function);
    }


};
#endif