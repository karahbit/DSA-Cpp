//
//  main.cpp
//  2-3 Tree Without Balance
//
//  Created by George Koubbe on 3/27/18.
//  Copyright © 2018 George Koubbe. All rights reserved.
//

#include <iostream>

using std::invalid_argument;
using std::cout;
using std::endl;

#include <deque>
using std::deque;

#include <fstream>
using std::ofstream;


// Class representing the 2-3 Tree Without Balance requirement
template <class Key, class Value>
class TwoThreeTreeNonBalanced
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
    
    static const bool RED = true;
    static const bool BLACK = false;
    
    // BST helper node data type
    class Node
    {
    public:
        
        Key key;                            // key
        Value value;                        // associated data
        Node *left, *right, *parent;        // links to left and right subtrees
        bool color;                         // color of parent link
        int size;                           // subtree count
        
        int depth;  // used only to compute the internal path length
        
        // constructor
        Node(Key key, Value value, int size, bool color) {
            this->key = key;
            this->value = value;
            this->size = size;
            this->color = color;
            left = right = parent = NULL;
        }
    };
    
    // number of node in subtree rooted at node; 0 if node is null
    int size(Node *node) {
        if (node == NULL) return 0;
        return node->size;
    }
    
    // is node red; false if node is null ?
    static bool isRed(Node *node) {
        if (node == NULL) return false;
        return node->color == RED;
    }
    
    // make a right-leaning link lean to the left
    Node *rotateLeft(Node *node) {
        if (node == NULL || node->right == NULL) {
            return node;
        }
        
        else
        {
            Node *newRoot = node->right;
            if(newRoot->left!=NULL)
            {
                node->right=newRoot->left;
                newRoot->left->parent=node;
            }
            else
                node->right=NULL;
            
            newRoot->parent=node->parent;
            
            if(node->parent!=NULL)
            {
                if(node==node->parent->left)
                    node->parent->left=newRoot;
                else
                    node->parent->right=newRoot;
            }
            newRoot->left=node;
            node->parent=newRoot;
        
            newRoot->color = node->color;
            node->color = RED;
        
            newRoot->size = node->size;
            node->size = size(node->left) + 1 + size(node->right);
        
            return newRoot;
        }
    }
    
    // make a left-leaning link lean to the right
    Node *rotateRight(Node *node) {
        if (node == NULL || node->left == NULL) {
            return node;
        }
        
        else
        {
            Node *newRoot = node->left;
            if(newRoot->right!=NULL)
            {
                node->left=newRoot->right;
                newRoot->right->parent=node;
            }
            else
                node->left=NULL;
            
            newRoot->parent=node->parent;
      
            if(node->parent!=NULL)
            {
                if(node==node->parent->left)
                    node->parent->left=newRoot;
                else
                    node->parent->right=newRoot;
            }
            newRoot->right=node;
            node->parent=newRoot;
        
            newRoot->color = node->color;
            node->color = RED;
        
            newRoot->size = node->size;
            node->size = size(node->left) + 1 + size(node->right);
        
            return newRoot;
        }
    }
    
    // flip the colors of a node and its two children
    void flipColors(Node *node) {
        if (node == NULL || node->left == NULL || node->right == NULL) {
            return;
        }
        
        //The root must have opposite color of its two children
        if ((isRed(node) && !isRed(node->left) && !isRed(node->right))
            || (!isRed(node) && isRed(node->left) && isRed(node->right))) {
            node->color = !node->color;
            node->left->color = !node->left->color;
            node->right->color = !node->right->color;
        }
    }
    
    // insert the key-value pair in the subtree rooted at node
    Node *put(Node *node, Key key, Value value, Node *parent) {
        if (node == NULL) {
            bool isTwoThreeNode = true;
            
            //If the parent is red, it is a 3-node
            //Otherwise, check the node sibling's color
            if (parent != NULL && !isRed(parent)) {
                bool isLeftChild = compareTwo(key,parent->key) < 0;
                
                if (isLeftChild) {
                    if (!isRed(parent->right)) {
                        isTwoThreeNode = false;
                    }
                } else {
                    if (!isRed(parent->left)) {
                        isTwoThreeNode = false;
                    }
                }
            }
            
            Node *h = new Node(key, value, 1, !isTwoThreeNode);
            h->parent = node;
            return h;
        }
        
        int compare = compareTwo(key,node->key);
        
        if (compare < 0) {
            node->left = put(node->left, key, value, node);
        } else if (compare > 0) {
            node->right = put(node->right, key, value, node);
        } else {
            node->value = value;
        }
        
        node->size = size(node->left) + 1 + size(node->right);
        return node;
    }
    
    // value associated with the given key in subtree rooted at node; NULL if no such key
    Value get(Node *node, Key key) {
        if (node == NULL) {
            return NULL;
        }
        
        int compare = compareTwo(key,node->key);
        if (compare < 0) {
            return get(node->left, key);
        } else if (compare > 0) {
            return get(node->right, key);
        } else {
            return node->value;
        }
    }
    
    // the smallest key in subtree rooted at node; null if no such key
    Node *min(Node *node) {
        if (node->left == NULL) {
            return node;
        }
        
        return min(node->left);
    }
    
    // the largest key in the subtree rooted at node; null if no such key
    Node *max(Node *node) {
        if (node->right == NULL) {
            return node;
        }
        
        return max(node->right);
    }
    
    // the largest key in the subtree rooted at node less than or equal to the given key
    Node *floor(Node *node, Key key) {
        if (node == NULL) {
            return NULL;
        }
        
        int compare = compareTwo(key,node->key);
        
        if (compare == 0) {
            return node;
        } else if (compare < 0) {
            return floor(node->left, key);
        } else {
            Node *rightNode = floor(node->right, key);
            if (rightNode != NULL) {
                return rightNode;
            } else {
                return node;
            }
        }
    }
    
    // the smallest key in the subtree rooted at node greater than or equal to the given key
    Node *ceiling(Node *node, Key key) {
        if (node == NULL) {
            return NULL;
        }
        
        int compare = compareTwo(key,node->key);
        
        if (compare == 0) {
            return node;
        } else if (compare > 0) {
            return ceiling(node->right, key);
        } else {
            Node *leftNode = ceiling(node->left, key);
            if (leftNode != NULL) {
                return leftNode;
            } else {
                return node;
            }
        }
    }
    
    // the key of rank k in the subtree rooted at node
    Node *select(Node *node, int index) {
        int leftSubtreeSize = size(node->left);
        
        if (leftSubtreeSize == index) {
            return node;
        } else if (leftSubtreeSize > index) {
            return select(node->left, index);
        } else {
            return select(node->right, index - leftSubtreeSize - 1);
        }
    }
    
    // number of keys less than key in the subtree rooted at node
    int rank(Node *node, Key key) {
        if (node == NULL) {
            return 0;
        }
        
        //Returns the number of keys less than node.key in the subtree rooted at node
        int compare = compareTwo(key,node->key);
        if (compare < 0) {
            return rank(node->left, key);
        } else if (compare > 0) {
            return size(node->left) + 1 + rank(node->right, key);
        } else {
            return size(node->left);
        }
    }
    
    // delete the key-value pair with the minimum key rooted at node
    Node *deleteMin(Node *node) {
        if (node->left == NULL) {
            return node->right;
        }
        
        if (!isRed(node->left) && !isRed(node->left->left)) {
            node = moveRedLeft(node);
        }
        
        node->left = deleteMin(node->left);
        return balance(node);
    }
    
    // delete the key-value pair with the maximum key rooted at node
    Node *deleteMax(Node *node) {
        if (isRed(node->left)) {
            node = rotateRight(node);
        }
        
        if (node->right == NULL) {
            return node->left;
        }
        
        if (!isRed(node->right) && !isRed(node->right->left)) {
            node = moveRedRight(node);
        }
        
        node->right = deleteMax(node->right);
        return balance(node);
    }
    
    // delete the key-value pair with the given key rooted at node
    Node *deleteKey(Node *node, Key key) {
        if (node == NULL) {
            return NULL;
        }
        
        if (compareTwo(key,node->key) < 0) {
            if (!isRed(node->left) && node->left != NULL && !isRed(node->left->left)) {
                node = moveRedLeft(node);
            }
            
            node->left = deleteKey(node->left, key);
        } else {
            if (isRed(node->left)) {
                node = rotateRight(node);
            }
            
            if (compareTwo(key,node->key) == 0 && node->right == NULL) {
                return node->left;
            }
            
            if (!isRed(node->right) && node->right != NULL && !isRed(node->right->left)) {
                node = moveRedRight(node);
            }
            
            if (compareTwo(key,node->key) == 0) {
                Node *aux = min(node->right);
                node->key = aux->key;
                node->value = aux->value;
                node->right = deleteMin(node->right);
            } else {
                node->right = deleteKey(node->right, key);
            }
        }
        
        return balance(node);
    }
    
    Node *moveRedLeft(Node *node) {
        // Assuming that node is red and both node.left and node.left.left are black,
        // make node.left or one of its children red
        flipColors(node);
        
        if (node->right != NULL && isRed(node->right->left)) {
            node->right = rotateRight(node->right);
            node = rotateLeft(node);
            flipColors(node);
        }
        
        return node;
    }
    
    Node *moveRedRight(Node *node) {
        // Assuming that node is red and both node.right and node.right.left are black,
        // make node.right or one of its children red
        flipColors(node);
        
        if (node->left != NULL && isRed(node->left->left)) {
            node = rotateRight(node);
            flipColors(node);
        }
        
        return node;
    }
    
    // restore red-black tree invariant
    Node *balance(Node *node) {
        if (node == NULL) {
            return NULL;
        }
        
        if (isRed(node->right) && isRed(node->right->right)) {
            node = rotateLeft(node);
        }
        
        if (isRed(node->left) && isRed(node->left->left)) {
            node = rotateRight(node);
        }
        
        if (isRed(node->left) && isRed(node->right)) {
            flipColors(node);
        }
        
        node->size = size(node->left) + 1 + size(node->right);
        
        return node;
    }
    
    // add the keys between lo and hi in the subtree rooted at node to the queue
    void keys(Node *node, deque<Key> &Queue, Key low, Key high) {
        if (node == NULL) {
            return;
        }
        
        int compareLow = compareTwo(low,node->key);
        int compareHigh = compareTwo(high,node->key);
        
        if (compareLow < 0) {
            keys(node->left, Queue, low, high);
        }
        
        if (compareLow <= 0 && compareHigh >= 0) {
            Queue.push_front(node->key);
        }
        
        if (compareHigh > 0) {
            keys(node->right, Queue, low, high);
        }
    }
    
    int internalPathLength() {
        if (root == NULL) {
            return 0;
        }
        
        int internalPathLength = 0;
        
        deque<Node*> Queue;
        root->depth = 0;
        Queue.push_front(root);
        
        while (!Queue.empty()) {
            Node *current;
            current = Queue.back();
            Queue.pop_back();
            internalPathLength += current->depth;
            
            if (current->left != NULL) {
                current->left->depth = current->depth + 1;
                Queue.push_front(current->left);
            }
            if (current->right != NULL) {
                current->right->depth = current->depth + 1;
                Queue.push_front(current->right);
            }
        }
        
        return internalPathLength;
    }
    
    bool isTwoThreeTree(Node *node) {
        if (node == NULL) {
            return true;
        }
        
        if (isRed(node->left) && (isRed(node->left->left) || isRed(node->left->right))) {
            return false;
        }
        
        if (isRed(node->right) && (isRed(node->right->left) || isRed(node->right->right))) {
            return false;
        }

        return isTwoThreeTree(node->left) && isTwoThreeTree(node->right);
    }
    
public:
    
    Node *root;     // root of the BST
    
    // constructor
    TwoThreeTreeNonBalanced()
    {
        root = NULL;
    }
    
    // returns the number of key-value pairs in this symbol table.
    int size() {
        return size(root);
    }
    
    // is this symbol table empty?
    bool isEmpty() {
        return size(root) == 0;
    }
    
    /* Inserts the specified key-value pair into the symbol table, overwriting the old
     * value with the new value if the symbol table already contains the specified key.
     * Deletes the specified key (and its associated value) from this symbol table
     * if the specified value is NULL. */
    void put(Key key, Value value) {
        if (key == nil) return;
        
        if (value == nil) {
            deleteKey(key);
            return;
        }
        
        root = put(root, key, value, root);
        root->color = BLACK;
    }
    
    // returns the value associated with the given key.
    Value get(Key key) {
        if (key == nil) return NULL;
        
        return get(root, key);
    }
    
    // does this symbol table contain the given key?
    bool contains(Key key) {
        if (key == nil) {
            throw invalid_argument("Argument to contains() cannot be null");
        }
        return get(key) != nil;
    }
    
    // returns the smallest key in the symbol table.
    Key min() {
        if (root == NULL) {
            cout << "Error! Empty binary search tree" << endl;
        }
        
        return min(root)->key;
    }
    
    // returns the largest key in the symbol table.
    Key max() {
        if (root == NULL) {
            cout << "Error! Empty binary search tree" << endl;
        }
        
        return max(root)->key;
    }
    
    // returns the highest key in the symbol table smaller than or equal to key.
    Key floor(Key key) {
        Node *node = floor(root, key);
        if (node == NULL) {
            return NULL;
        }
        
        return node->key;
    }
    
    // returns the smallest key in the symbol table greater than or equal to key.
    Key ceiling(Key key) {
        Node *node = ceiling(root, key);
        if (node == NULL) {
            return NULL;
        }
        
        return node->key;
    }
    
    /* Return the key in the symbol table whose rank is {@code k}.
     * This is the (k+1)st smallest key in the symbol table. */
    Key select(int index) {
        if (index >= size()) {
            throw invalid_argument("Index is higher than tree size");
        }
        
        return select(root, index)->key;
    }
    
    // return the number of keys in the symbol table strictly less than key.
    int rank(Key key) {
        return rank(root, key);
    }
    
    // removes the smallest key and associated value from the symbol table.
    void deleteMin() {
        if (isEmpty()) {
            return;
        }
        
        if (!isRed(root->left) && !isRed(root->right)) {
            root->color = RED;
        }
        
        root = deleteMin(root);
        
        if (!isEmpty()) {
            root->color = BLACK;
        }
    }
    
    // removes the largest key and associated value from the symbol table.
    void deleteMax() {
        if (isEmpty()) {
            return;
        }
        
        if (!isRed(root->left) && !isRed(root->right)) {
            root->color = RED;
        }
        
        root = deleteMax(root);
        
        if (!isEmpty()) {
            root->color = BLACK;
        }
    }
    
    /* Removes the specified key and its associated value from this symbol table
     * (if the key is in this symbol table).*/
    void deleteKey(Key key) {
        if (isEmpty()) {
            return;
        }
        
        if (!contains(key)) {
            return;
        }
        
        if (!isRed(root->left) && !isRed(root->right)) {
            root->color = RED;
        }
        
        root = deleteKey(root, key);
        
        if (!isEmpty()) {
            root->color = BLACK;
        }
    }
    
    /* returns all keys in the symbol table.
     * To iterate over all of the keys in the symbol table named st,
     * use the foreach notation: for (Key key : st.keys()).*/
    deque<Key> keys() {
        return keys(min(), max());
    }
    
    // returns all keys in the symbol table in the given range.
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
    
    // returns the number of keys in the symbol table in the given range.
    int size(Key low, Key high) {
        if (low == NULL)  {
            throw invalid_argument("First argument to size() cannot be null");
        }
        if (high == NULL) {
            throw invalid_argument("Second argument to size() cannot be null");
        }
        
        if (compareTwo(low,high) > 0) {
            return 0;
        }
        
        if (contains(high)) {
            return rank(high) - rank(low) + 1;
        } else {
            return rank(high) - rank(low);
        }
    }
    
    int averagePathLength() {
        if (size() == 0) {
            return 0;
        }
        
        return (internalPathLength() / size()) + 1;
    }
    
    bool isTwoThreeTree() {
        return isTwoThreeTree(root);
    }
    
    // print a copy of the keys without deleting the original queue
    void printKeys()
    {
        deque<int> copy = keys();
        while (!copy.empty())
        {
            cout << copy.back() << " ";
            copy.pop_back();
        }
        cout << endl;
    }
    
    // print a copy of the keys between a given range, without deleting the original queue
    void printKeys(Key low, Key high)
    {
        deque<int> copy = keys(low, high);
        while (!copy.empty())
        {
            cout << copy.back() << " ";
            copy.pop_back();
        }
        cout << endl;
    }
    
    static int countRed(Node *node) {
        int nbRed = 0;
        if (node == NULL) {
            return 0;
        }
        nbRed += countRed(node->left);
        nbRed += countRed(node->right);
        
        if(isRed(node)){
            nbRed++;
        }
        return nbRed;
    }
    
};

int main() {
    
    // create the tree
    TwoThreeTreeNonBalanced<int,int> twoThreeTreeNonBalanced;
    
    /* test API
     * Expected tree
     *            (B)5
     *       (R)1      (B)9
     *    (B)0  (B)2     (R)99
     *  (R)-1
     * (B)-2
     */
    
    twoThreeTreeNonBalanced.put(5, 5);
    twoThreeTreeNonBalanced.put(1, 1);
    twoThreeTreeNonBalanced.put(9, 9);
    twoThreeTreeNonBalanced.put(2, 2);
    twoThreeTreeNonBalanced.put(0, 0);
    twoThreeTreeNonBalanced.put(99, 99);
    twoThreeTreeNonBalanced.put(-1, -1);
    twoThreeTreeNonBalanced.put(-2, -2);
    
    cout << ("Keys() test") << endl;
    twoThreeTreeNonBalanced.printKeys();
    cout << ("Expected: -2 -1 0 1 2 5 9 99\n") << endl;
    
    //Test min()
    cout << "Min key: " << twoThreeTreeNonBalanced.min() << " Expected: -2" << endl;
    
    //Test max()
    cout << "Max key: " << twoThreeTreeNonBalanced.max() << " Expected: 99" << endl;
    
    //Test floor()
    cout << "Floor of 5: " << twoThreeTreeNonBalanced.floor(5) << " Expected: 5" << endl;
    cout << "Floor of 15: " << twoThreeTreeNonBalanced.floor(15) << " Expected: 9" << endl;
    
    //Test ceiling()
    cout << "Ceiling of 5: " << twoThreeTreeNonBalanced.ceiling(5) << " Expected: 5" << endl;
    cout << "Ceiling of 15: " << twoThreeTreeNonBalanced.ceiling(15) << " Expected: 99" << endl;
    
    //Test select()
    cout << "Select key of rank 4: " << twoThreeTreeNonBalanced.select(4) << " Expected: 2" << endl;
    
    //Test rank()
    cout << "Rank of key 9: " << twoThreeTreeNonBalanced.rank(9) << " Expected: 6" << endl;
    cout << "Rank of key 10: " << twoThreeTreeNonBalanced.rank(10) << " Expected: 7" << endl;
    
    //Test delete()
    cout << "\nDelete key 2" << endl;
    twoThreeTreeNonBalanced.deleteKey(2);
    twoThreeTreeNonBalanced.printKeys();
    cout << "Is 2-3 three: " << twoThreeTreeNonBalanced.isTwoThreeTree() << " Expected: true" << endl;
    
    //Test deleteMin()
    cout << "\nDelete min (key -2)" << endl;
    twoThreeTreeNonBalanced.deleteMin();
    twoThreeTreeNonBalanced.printKeys();
    cout << "Is 2-3 three: " << twoThreeTreeNonBalanced.isTwoThreeTree() << " Expected: true" << endl;
    
    //Test deleteMax()
    cout << "\nDelete max (key 99)" << endl;
    twoThreeTreeNonBalanced.deleteMax();
    twoThreeTreeNonBalanced.printKeys();
    cout << "Is 2-3 three: " << twoThreeTreeNonBalanced.isTwoThreeTree() << " Expected: true" << endl;
    
    //Test keys() with range
    cout << "\nKeys in range [2, 10]" << endl;
    twoThreeTreeNonBalanced.printKeys(2,10);
    
    //Delete all
    cout << "\nDelete all" << endl;
    while (twoThreeTreeNonBalanced.size() > 0) {
        twoThreeTreeNonBalanced.printKeys();
        cout << "Is 2-3 three: " << twoThreeTreeNonBalanced.isTwoThreeTree() << " Expected: true" << endl;
        twoThreeTreeNonBalanced.deleteKey(twoThreeTreeNonBalanced.select(twoThreeTreeNonBalanced.size() - 1));
        cout << endl;
    }
    
    
    // Q2. Experiments
    
    // i) random insertions
    int redBlackTreeSize = 100;
    
    for(int experiment = 0; experiment < 6; experiment++) {
        TwoThreeTreeNonBalanced<int, int> twoThreeTreeNonBalanced;
        
        for(int i = 0; i < redBlackTreeSize; i++) {
            int randomKey = rand();
            twoThreeTreeNonBalanced.put(randomKey, randomKey);
        }
        
        cout << "Average path length for " << redBlackTreeSize << " random insertions: "
                       << twoThreeTreeNonBalanced.averagePathLength() << endl;
        redBlackTreeSize *= 10;
    }
    
    cout << endl;
    
    // ii) sorted insertions
    redBlackTreeSize = 100;
    
    for(int experiment = 0; experiment < 4; experiment++) {
        TwoThreeTreeNonBalanced<int, int> twoThreeTreeNonBalanced;
        
        for(int i = 0; i < redBlackTreeSize; i++) {
            int sortedKey = i++;
            twoThreeTreeNonBalanced.put(sortedKey, sortedKey);
        }
        
        cout << "Average path length for " << redBlackTreeSize << " sorted insertions: "
        << twoThreeTreeNonBalanced.averagePathLength() << endl;
        redBlackTreeSize *= 10;
    }
    
    cout << endl;
    
    // Q3. Percentage of Red Nodes
    redBlackTreeSize = 10000;
    
    for(int experiment = 0; experiment < 3; experiment++) {
        TwoThreeTreeNonBalanced<int, int> twoThreeTreeNonBalanced;
        
        for(int i = 0; i < redBlackTreeSize; i++) {
            int randomKey = rand();
            twoThreeTreeNonBalanced.put(randomKey, randomKey);
        }
        
        double percentage = (static_cast<double>(twoThreeTreeNonBalanced.countRed(twoThreeTreeNonBalanced.root)) /
                             twoThreeTreeNonBalanced.size())*100;
        cout << "Percentage of Red Nodes for " << redBlackTreeSize << " random insertions: "
        << percentage << " %" << endl;
        redBlackTreeSize *= 10;
    }
    
    cout << endl;
    
    // Q4. Avg and std deviation of average path length
    ofstream file;
    file.open ("Q4.txt");
    
    redBlackTreeSize = 1;
    
    for(int experiment = 0; experiment < 10000; experiment++) {
        TwoThreeTreeNonBalanced<int, int> twoThreeTreeNonBalanced;
     
        for(int i = 0; i < redBlackTreeSize; i++) {
            int randomKey = rand();
            twoThreeTreeNonBalanced.put(randomKey, randomKey);
        }
     
        file << twoThreeTreeNonBalanced.averagePathLength() << endl;
        redBlackTreeSize++;
     }
    
    file.close();
    
    cout << endl;
    return 0;
}
