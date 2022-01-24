//
//  main.cpp
//  BST
//
//  Created by George Koubbe on 3/27/18.
//  Copyright © 2018 George Koubbe. All rights reserved.
//

#include <iostream>
using std::cin;
using std::invalid_argument;
using std::cout;
using std::endl;

#include <deque>
using std::deque;

#include <algorithm>    // std::max
using std::max;

#include <string>
using std::string;

#include <fstream>
using std::ifstream;


// Class representing the Binary Search Tree
template <class Key, class Value>
class BST
{
private:
    
    /* A magic number that I will never use just to represent the check against null
     * in Java Integer class. For int keys, use int magic number. For other types
     * of keys, change the type of magic number. */
    const Key nil = -333;
    
    // private helper function to compare two elements.
    int compareTwo(Key a, Key b) {
        if (a < b) return -1;
        else if (a == b) return 0;
        else return 1;
    }
    
    // BST helper node data type
    class Node
    {
    public:
        
        Key key;                            // key
        Value val;                        // associated data
        Node *left, *right, *parent;        // links to left and right subtrees
        int size;                           // subtree count
        
        // constructor
        Node(Key key, Value val, int size) {
            this->key = key;
            this->val = val;
            this->size = size;
            left = right = parent = NULL;
        }
    };
    
    int size(Node *x) {
        if (x == NULL) return 0;
        else return x->size;
    }
    
    Value get(Node *x, Key key) {
        if (key == nil) throw invalid_argument("calls get() with a null key");
        if (x == NULL) return NULL;
        int cmp = compareTwo(key,x->key);
        if      (cmp < 0) return get(x->left, key);
        else if (cmp > 0) return get(x->right, key);
        else              return x->val;
    }
    
    Node *put(Node *x, Key key, Value val) {
        if (x == NULL){
            Node *h = new Node(key, val, 1);
            h->parent = x;
            return h;
        }
        int cmp = compareTwo(key,x->key);
        if      (cmp < 0) x->left  = put(x->left,  key, val);
        else if (cmp > 0) x->right = put(x->right, key, val);
        else              x->val   = val;
        x->size = 1 + size(x->left) + size(x->right);
        return x;
    }
    
    Node *deleteMin(Node *x) {
        if (x->left == NULL) return x->right;
        x->left = deleteMin(x->left);
        x->size = size(x->left) + size(x->right) + 1;
        return x;
    }
    
    Node *deleteMax(Node *x) {
        if (x->right == NULL) return x->left;
        x->right = deleteMax(x->right);
        x->size = size(x->left) + size(x->right) + 1;
        return x;
    }
    
    Node *deleteKey(Node *x, Key key) {
        if (x == NULL) return NULL;
        
        int cmp = compareTwo(key,x->key);
        if      (cmp < 0) x->left  = deleteKey(x->left,  key);
        else if (cmp > 0) x->right = deleteKey(x->right, key);
        else {
            if (x->right == NULL) return x->left;
            if (x->left  == NULL) return x->right;
            Node *t = x;
            x = min(t->right);
            x->right = deleteMin(t->right);
            x->left = t->left;
        }
        x->size = size(x->left) + size(x->right) + 1;
        return x;
    }
    
    Node *min(Node *x) {
        if (x->left == NULL) return x;
        else                return min(x->left);
    }
    
    Node *max(Node *x) {
        if (x->right == NULL) return x;
        else                 return max(x->right);
    }
    
    Node *floor(Node *x, Key key) {
        if (x == NULL) return NULL;
        int cmp = compareTwo(key,x->key);
        if (cmp == 0) return x;
        if (cmp <  0) return floor(x->left, key);
        Node *t = floor(x->right, key);
        if (t != NULL) return t;
        else return x;
    }
    
    Key floor2(Node *x, Key key, Key best) {
        if (x == NULL) return best;
        int cmp = compareTwo(key,x->key);
        if      (cmp  < 0) return floor2(x->left, key, best);
        else if (cmp  > 0) return floor2(x->right, key, x->key);
        else               return x->key;
    }
    
    Node *ceiling(Node *x, Key key) {
        if (x == NULL) return NULL;
        int cmp = compareTwo(key,x->key);
        if (cmp == 0) return x;
        if (cmp < 0) {
            Node *t = ceiling(x->left, key);
            if (t != NULL) return t;
            else return x;
        }
        return ceiling(x->right, key);
    }
    
    Node *select(Node *x, int k) {
        if (x == NULL) return NULL;
        int t = size(x->left);
        if      (t > k) return select(x->left,  k);
        else if (t < k) return select(x->right, k-t-1);
        else            return x;
    }
    
    int rank(Key key, Node *x) {
        if (x == NULL) return 0;
        int cmp = compareTwo(key,x->key);
        if      (cmp < 0) return rank(key, x->left);
        else if (cmp > 0) return 1 + size(x->left) + rank(key, x->right);
        else              return size(x->left);
    }
    
    void keys(Node *x, deque<Key> &Queue, Key lo, Key hi) {
        if (x == NULL) return;
        int cmplo = compareTwo(lo,x->key);
        int cmphi = compareTwo(hi,x->key);
        if (cmplo < 0) keys(x->left, Queue, lo, hi);
        if (cmplo <= 0 && cmphi >= 0) Queue.push_front(x->key);
        if (cmphi > 0) keys(x->right, Queue, lo, hi);
    }
    
    int height(Node *x) {
        if (x == NULL) return -1;
        return 1 + max(height(x->left), height(x->right));
    }

    /*************************************************************************
     *  Check integrity of BST data structure.
     ***************************************************************************/
    bool check() {
        if (!isBST())            cout << "Not in symmetric order" << endl;
        if (!isSizeConsistent()) cout << "Subtree counts not consistent" << endl;
        if (!isRankConsistent()) cout << "Ranks not consistent" << endl;
        return isBST() && isSizeConsistent() && isRankConsistent();
    }
    
    // does this binary tree satisfy symmetric order?
    // Note: this test also ensures that data structure is a binary tree since order is strict
    bool isBST() {
        return isBST(root, nil, nil);
    }
    
    // is the tree rooted at x a BST with all keys strictly between min and max
    // (if min or max is null, treat as empty constraint)
    // Credit: Bob Dondero's elegant solution
    bool isBST(Node *x, Key min, Key max) {
        if (x == NULL) return true;
        if (min != nil && compareTwo(x->key,min) <= 0) return false;
        if (max != nil && compareTwo(x->key,max) >= 0) return false;
        return isBST(x->left, min, x->key) && isBST(x->right, x->key, max);
    }
    
    // are the size fields correct?
    bool isSizeConsistent() { return isSizeConsistent(root); }
    bool isSizeConsistent(Node *x) {
        if (x == NULL) return true;
        if (x->size != size(x->left) + size(x->right) + 1) return false;
        return isSizeConsistent(x->left) && isSizeConsistent(x->right);
    }
    
    // check that ranks are consistent
    bool isRankConsistent() {
        for (int i = 0; i < size(); i++)
            if (i != rank(select(i))) return false;
        for (Key key : keys())
            if (compareTwo(key,select(rank(key))) != 0) return false;
        return true;
    }
    
    Node *mystery(Node *x, Key key, Node *best) {
        if (x == NULL) return best;
        int cmp = compareTwo(key,x->key);
        if (cmp < 0) return mystery(x->left,key,x);
        else if (cmp > 0) return mystery(x->right, key, best);
        else return x;
    }
    
public:
    
    Node *root;     // root of the BST
    
    bool isEmpty() {
        return size() == 0;
    }
    
    int size() {
        return size(root);
    }
    
    bool contains(Key key) {
        if (key == nil) throw invalid_argument("Argument to contains() cannot be null");
        return get(key) != nil;
    }
    
    Value get(Key key) {
        return get(root, key);
    }
    
    void put(Key key, Value val) {
        if (key == nil) throw invalid_argument("calls put() with a null key");
        if (val == nil) {
            deleteKey(key);
            return;
        }
        root = put(root, key, val);
    }
    
    void deleteMin() {
        if (isEmpty()) return;
        root = deleteMin(root);
    }
    
    void deleteMax() {
        if (isEmpty()) return;
        root = deleteMax(root);
    }
    
    void deleteKey(Key key) {
        if (key == nil) throw invalid_argument("calls delete() with a null key");
            root = deleteKey(root, key);
    }
    
    Key min() {
        return min(root)->key;
    }
    
    Key max() {
        return max(root)->key;
    }
    
    Key floor(Key key) {
        if (key == nil) throw invalid_argument("argument to floor() is null");
        if (isEmpty()) return nil;
        Node *x = floor(root, key);
        if (x == NULL) return nil;
        else return x->key;
    }
    
    Key floor2(Key key) {
        return floor2(root, key, nil);
    }
    
    Key ceiling(Key key) {
        if (key == nil) throw invalid_argument("argument to ceiling() is null");
        if (isEmpty()) return nil;
        Node *x = ceiling(root, key);
        if (x == NULL) return nil;
        else return x->key;
    }
    
    Key select(int k) {
        if (k < 0 || k >= size()) {
            throw invalid_argument("argument to select() is invalid: ");
        }
        Node *x = select(root, k);
        return x->key;
    }
    
    int rank(Key key) {
        if (key == nil) throw invalid_argument("argument to rank() is null");
        return rank(key, root);
    }
    
    deque<Key> keys() {
        if (isEmpty()) {
            deque<Key> Queue;
            return Queue;
        }
        return keys(min(), max());
    }
    
    deque<Key> keys(Key low, Key high) {
        if (low == nil)  {
            throw invalid_argument("First argument to keys() cannot be null");
        }
        if (high == nil) {
            throw invalid_argument("Second argument to keys() cannot be null");
        }
        
        deque<Key> Queue;
        keys(root, Queue, low, high);
        return Queue;
    }
    
    int size(Key lo, Key hi) {
        if (lo == nil) throw invalid_argument("first argument to size() is null");
        if (hi == nil) throw invalid_argument("second argument to size() is null");
        
        if (compareTwo(lo,hi) > 0) return 0;
        if (contains(hi)) return rank(hi) - rank(lo) + 1;
        else              return rank(hi) - rank(lo);
    }
    
    int height() {
        return height(root);
    }
    
    // print a copy of the keys without deleting the original queue
    void printKeys()
    {
        deque<Key> copy = keys();
        while (!copy.empty())
        {
            cout << copy.back() << " ";
            copy.pop_back();
        }
        cout << endl;
    }
    
    Key mystery(Key key) {
        Node *best = mystery(root, key, NULL);
        if (best == NULL) return NULL;
        return best->key;
    }
    
};

int main()
{
    ifstream file;
    string input;
    int n = 0;
    int i = 0;
    
    BST<double,double> st;
    file.open("select-data.txt");
    if (!file.is_open())
        exit(EXIT_FAILURE);

    while (!file.eof())// i used while end of file instead of array or vector also the program is taking alot of time
        //to input the data from file to the tree 1 to 1000000 value so expect long time to run
    {
        
        file >> input;
        st.put(stoi(input),i);
        cout << n << "-";
        cout << input << endl;
        ++n;
        i++;
    }
    
    cout << endl;
    
    st.printKeys();
    cout << endl;
    
    cout << "The value of select(7) is " << st.select(7) << endl;   // arg: rank 7
    cout << "The value of rank(7) is " << st.rank(7) << endl;   // arg: key 7
    
    cout << "floor of Key: 7.2 is " << st.floor(7.2) << endl;
    cout << "ceiling of Key: 7.7 is " << st.ceiling(7.7) << endl;
    cout << "floor2 of Key: 6.4 is " << st.floor2(6.4) << endl;
    cout << "mystery of Key: 8.7 is  " << st.mystery(8.7) << endl;
    
    cout << endl;
    return 0;
}
