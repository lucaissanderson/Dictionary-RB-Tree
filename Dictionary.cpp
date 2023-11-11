/****************************************
 *  Dictionary.cpp
 *  lzsander
 *  Dictionary function implementation
 * **************************************/

#include "Dictionary.h"

#define BLACK 0
#define RED 1

// Helper Functions (Optional) ---------------------------------------------

// inOrderString()
// Appends a string representation of the tree rooted at R to string s. The
// string appended consists of: "key : value \n" for each key-value pair in
// tree R, arranged in order by keys.
void Dictionary::inOrderString(std::string& s, Node* R) const{
    // recursive call with multiple base cases
    if(R!=nil){
        inOrderString(s, R->left);
        s += R->key + " : " + std::to_string(R->val) + "\n";
        inOrderString(s, R->right);
    }
}

// preOrderString()
// appends to s everything in R's subtree in preorder
void Dictionary::preOrderString(std::string& s, Node* R) const{
    // recursive call with multiple base cases
    if(R!=nil){
        s += R->key + (R->color ? " (RED)":"") + "\n";
        preOrderString(s, R->left);
        preOrderString(s, R->right);
    }
}

// preOrderCopy()
void Dictionary::preOrderCopy(Node* R, Node* N){
    if(R!=N){
        setValue(R->key, R->val);
        preOrderCopy(R->left, N);
        preOrderCopy(R->right, N);
    }
}

// postOrderDelete()
// recursively delete R's subtree in postorder
void Dictionary::postOrderDelete(Node* R){
    if(R!=nil){
        postOrderDelete(R->left);
        postOrderDelete(R->right);
        delete R;
    }
}

// search()
// returns node matching key k
Dictionary::Node* Dictionary::search(Dictionary::Node* R, keyType k) const{
    // iterative 
    while(R!=nil && k!=R->key){
        if(k < R->key) R = R->left;
        else R = R->right;
    }
    return R;
}

// findMin()
// returns left-most node
Dictionary::Node* Dictionary::findMin(Dictionary::Node* R){
    while(R->left!=nil){
        R = R->left;
    }
    return R;
};

// findMax()
// returns right-most node
Dictionary::Node* Dictionary::findMax(Dictionary::Node* R) const{
    while(R->right!=nil){
        R = R->right;
    }
    return R;
}

// findNext()
// returns next node from inorder walk
Dictionary::Node* Dictionary::findNext(Dictionary::Node* R){
    Node* y;
    if(R->right != nil){
        return findMin(R->right);
    }
    y = R->parent;
    while(y!=nil && R==y->right){
        R = y;
        y = y->parent;
    }
    return y;
}

// findPrev()
// returns previous node from inorder walk
Dictionary::Node* Dictionary::findPrev(Dictionary::Node* R){
    Node* y;
    if(R->left != nil){
        return findMax(R->left);
    }
    y = R->parent;
    while(y!=nil && R==y->left){
        R = y;
        y = y->parent;
    }
    return y;
}


// RBT Helper Functions ---------------------------------

// LeftRotate()
void Dictionary::LeftRotate(Node* x){
    Node* y = x->right;
    x->right = y->left;
    if(y->left != nil){
        y->left->parent = x;
    }
    y->parent = x->parent;
    if(x->parent == nil){
        root = y;
    } else if(x == x->parent->left){
        x->parent->left = y;
    } else {
        x->parent->right = y;
    }
    y->left = x;
    x->parent = y;
}

// RightRotate()
void Dictionary::RightRotate(Node* y){
    Node* x = y->left;
    y->left = x->right;
    if(x->right != nil){
        x->right->parent = y;
    }
    x->parent = y->parent;
    if(y->parent == nil){
        root = x;
    } else if(y == y->parent->right){
        y->parent->right = x;
    } else {
        y->parent->left = x;
    }
    x->right = y;
    y->parent = x;
}

// RB_InsertFixUp()
void Dictionary::RB_InsertFixUp(Node* z){
    Node* y;
    while(z->parent->color == RED){
        if(z->parent == z->parent->parent->left){
            y = z->parent->parent->right;
            if(y->color == RED){
                z->parent->color = BLACK;
                y->color = BLACK;
                z->parent->parent->color = RED;
                z = z->parent->parent;
            } else {
                if(z == z->parent->right){
                    z = z->parent;
                    LeftRotate(z);
                }
                z->parent->color = BLACK;
                z->parent->parent->color = RED;
                RightRotate(z->parent->parent);
            }
        } else {
            y = z->parent->parent->left;
            if(y->color == RED){
                z->parent->color = BLACK;
                y->color = BLACK;
                z->parent->parent->color = RED;
                z = z->parent->parent;
            } else {
                if(z == z->parent->left){
                    z = z->parent;
                    RightRotate(z);
                }
                z->parent->color = BLACK;
                z->parent->parent->color = RED;
                LeftRotate(z->parent->parent);
            }
        }
    }
    root->color = BLACK;
}

// RB_Transplant()
void Dictionary::RB_Transplant(Node* u, Node* v){
    if(u->parent == nil){
        root = v;
    } else if(u == u->parent->left){
        u->parent->left = v;
    } else {
        u->parent->right = v;
    }
    v->parent = u->parent;
}

// RB_Delete()
void Dictionary::RB_Delete(Node* z){
    Node* y = z;
    Node* x;
    int y_original_color = y->color;
    if(z->left == nil){
        x = z->right;
        RB_Transplant(z, z->right);
    } else if(z->right == nil){
        x = z->left;
        RB_Transplant(z, z->left);
    } else {
        y = findMin(z->right);
        y_original_color = y->color;
        x = y->right;
        if(y->parent == z){
            x->parent = y;
        } else {
            RB_Transplant(y, y->right);
            y->right = z->right;
            y->right->parent = y;
        }
        RB_Transplant(z,y);
        y->left = z->left;
        y->left->parent = y;
        y->color = z->color;
    }

    delete z;

    if(y_original_color == BLACK){
        RB_DeleteFixUp(x);
    }
}

// RB_DeleteFixUp()
void Dictionary::RB_DeleteFixUp(Node* x){
    Node* w;
    while(x != root && x->color == BLACK){
        if(x == x->parent->left){
            w = x->parent->right;
            if(w->color == RED){
                // case 1
                w->color = BLACK;
                x->parent->color = RED;
                LeftRotate(x->parent);
                w = x->parent->right;
            }
            if(w->left->color == BLACK and w->right->color == BLACK){
                // case 2
                w->color = RED;
                x = x->parent;
            } else {
                // case 3
                if(w->right->color == BLACK){
                    w->left->color = BLACK;
                    w->color = RED;
                    RightRotate(w);
                    w = x->parent->right;
                }
                // case 4
                w->color = x->parent->color;
                x->parent->color = BLACK;
                w->right->color = BLACK;
                LeftRotate(x->parent);
                x = root;
            }
        } else {
            w = x->parent->left;
            if(w->color == RED){
                // case 5
                w->color = BLACK;
                x->parent->color = RED;
                RightRotate(x->parent);
                w = x->parent->left;
            }
            if(w->right->color == BLACK && w->left->color == BLACK){
                // case 6
                w->color = RED;
                x = x->parent;
            } else {
                // case 7
                if(w->left->color == BLACK){
                    w->right->color = BLACK;
                    w->color = RED;
                    LeftRotate(w);
                    w = x->parent->left;
                }
                // case 8
                w->color = x->parent->color;
                x->parent->color = BLACK;
                w->left->color = BLACK;
                RightRotate(x->parent);
                x = root;
            }
        }
    }
    x->color = BLACK;
}

// Constructor/Destructors ------------------------------

// node constructor
// define node constructor
Dictionary::Node::Node(keyType k, valType v){
    val = v;
    key = k;
}

// create dictionary of empty state
// define Dictionary constructor
Dictionary::Dictionary(){
    num_pairs = 0;
    nil = new Node("0",0);
    nil->color = BLACK;
    root = nil;
    current = nil;
}

// create copy
// copy D onto this
Dictionary::Dictionary(const Dictionary& D){
    // parameters
    nil = new Node("0",0);
    nil->color = BLACK;
    root = nil;
    current = nil;
    num_pairs = 0;
    // copy dynamic members
    preOrderCopy(D.root,D.nil);
}

// destroy dictionary
// destry dynamic members and lets C++ take care of rest
Dictionary::~Dictionary(){
    if(num_pairs) postOrderDelete(root);
    delete nil;
}

// Access functions --------------------------------------------------------

// size()
// Returns the size of this Dictionary.
int Dictionary::size() const{
    return num_pairs;
}

// contains()
// returns true if this contains node with k, false otherwise
bool Dictionary::contains(keyType k) const{
    return search(root,k) != nil ? true : false;
}

// getValue()
// return reference to value at key k
// Pre: contains(k)
valType& Dictionary::getValue(keyType k) const{

    if(!contains(k)) throw std::logic_error("Dictionary: getValue(): does not contain k");

    Node* N = root;

    while(N != nil){
        if(N->key == k) break;
        else if(N->key < k) N = N->right;
        else N = N->left;
    }

    return N->val;

};

// hasCurrent()
// returns true if cursor defined, false otherwise
bool Dictionary::hasCurrent() const{

    return current == nil ? false : true;

};

// currentKey()
// returns the key of the current node
// Pre: hasCurrent() 
keyType Dictionary::currentKey() const{

    if(!hasCurrent()) throw std::logic_error("Dictionary: currentKey(): current not set");

    return current->key;

};

// currentVal()
// returns value reference of current node
// Pre: hasCurrent()
valType& Dictionary::currentVal() const{

    if(!hasCurrent()) throw std::logic_error("Dictionary: currentVal(): current not set");

    return current->val;

}



// Manipulation procedures -------------------------------------------------

// setValue()
// If a pair with key==k exists, overwrites the corresponding value with v, 
// otherwise inserts the new pair (k, v).
void Dictionary::setValue(keyType k, valType v){

    Node* z;
    Node* y = nil;
    Node* x = root;

    while(x != nil){
        if(x->key == k){
            x->val = v;
            return;
        }
        y = x;
        if(k < x->key){
            x = x->left;
        } else x = x->right;
    }

    z = new Node(k,v);
    z->parent = y;

    if(y == nil){
        root = z;
    } else if(z->key < y->key){
        y->left = z;
    } else {
        y->right = z;
    }

    z->left = nil;
    z->right = nil;
    z->color = RED;

    RB_InsertFixUp(z);

    num_pairs++;

}

// remove()
// deletes node with key k
// Pre: contains(k)
void Dictionary::remove(keyType k){
    if(current->key==k) current=nil;
    RB_Delete(search(root,k));
    num_pairs--;
}

// clear()
// set this to zero state
void Dictionary::clear(){
    if(num_pairs) postOrderDelete(root);
    num_pairs = 0;
    root = nil;
    current = nil;
}

// begin()
// set current node to smallest value
void Dictionary::begin(){
    if(!num_pairs) throw std::logic_error("Dictionary: begin(): empty dictionary");
    current = findMin(root);
}

// end()
// set current node to largest value
void Dictionary::end(){
    if(!num_pairs) throw std::logic_error("Dictionary: end(): empty dictionary");
    current = findMax(root);
}

// next()
// set current to next node in inorder walk
// Pre: hasCurrent()
void Dictionary::next(){
    if(!hasCurrent()) throw std::logic_error("Dictionary: next(): current not set");
    current = findNext(current);
}

// prev()
// set current to prev node in inorder walk
// Pre: hasCurrent()
void Dictionary::prev(){
    if(!hasCurrent()) throw std::logic_error("Dictionary: prev(): current not set");
    current = findPrev(current);
}

// Other functions --------------------------------------------------------

// to_string()
// returns string of dictionary, inorder walk
std::string Dictionary::to_string() const{

    std::string s = "";
    inOrderString(s, root);
    return s;

};

// pre_string()
// returns string of dictionary, preorder walk
std::string Dictionary::pre_string() const{

    std::string s = "";
    preOrderString(s, root);
    return s;

}

// equals()
// returns true if equal, false otherwise
bool Dictionary::equals(const Dictionary& D) const{

    return this->to_string()==D.to_string();

};

// overloaded -------------------------------------------------

// stream
// returns file stream of to_string()
std::ostream& operator<<( std::ostream& stream, Dictionary& D ){
    return stream << D.Dictionary::to_string();
}

// operator==()
// overload for equals()
bool operator==(const Dictionary& A, const Dictionary& B){
    return A.equals(B);
}

// operator=()
// overwrite this with D
Dictionary& Dictionary::operator=( const Dictionary& D ){
    this->clear();
    // parameters
    nil = new Node("0",0);
    nil->color = BLACK;
    root = nil;
    current = nil;
    // copy dynamic members
    Node* N = D.root;
    preOrderCopy(N, nil);
    return *this;
};
