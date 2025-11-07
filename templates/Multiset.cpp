#include "headers/Multiset.hpp"
#include <queue>
#include <stack>
#include <limits>
#include <iomanip>
#include <cassert>

template <typename Data>
std::ostream&
operator<<(std::ostream& out, const MultiSet<Data>& rhv) 
{
    rhv.outputTree(rhv.root_, out);
    return out;
}

template <typename Data>
MultiSet<Data>::MultiSet()
    : root_(NULL)
{}

template <typename Data>
MultiSet<Data>::MultiSet(const MultiSet& rhv)
    : root_(NULL)
{
    insert(rhv.begin(), rhv.end());
}

template <typename Data>
template <typename InputIt>
MultiSet<Data>::MultiSet(InputIt first, InputIt last)
    : root_(NULL)
{
    insert(first, last);
}

template <typename Data>
MultiSet<Data>::~MultiSet()
{
    clear();
}

template <typename Data>
const MultiSet<Data>&
MultiSet<Data>::operator=(const MultiSet& rhv)
{
    insert(rhv.begin(), rhv.end());
    return *this;
}

template <typename Data>
void 
MultiSet<Data>::swap(MultiSet& rhv)
{
    std::swap(root_, rhv.root_);
}

template <typename Data>
typename MultiSet<Data>::size_type 
MultiSet<Data>::size() const
{
    int counter = 0;
    const_iterator it = begin();
    while (it != end()) {
        ++it;
        ++counter;
    }
    return counter;
}

template <typename Data>
typename MultiSet<Data>::size_type 
MultiSet<Data>::max_size() const
{
    return std::numeric_limits<size_t>::max() / sizeof(Node*); 
}

template <typename Data>
bool 
MultiSet<Data>::empty() const
{
    return NULL == root_;
}

template <typename Data>
void 
MultiSet<Data>::clear()
{
    clearHelper(root_);
}

template <typename Data>
void
MultiSet<Data>::clearHelper(Node*& root) 
{
    if (NULL == root) { return; }
    clearHelper(root->left_);
    clearHelper(root->right_);
    delete root;
    root = NULL;
}

template <typename Data>
typename MultiSet<Data>::iterator 
MultiSet<Data>::begin()
{
    return iterator(getLeftMost(root_));
}

template <typename Data>
typename MultiSet<Data>::iterator 
MultiSet<Data>::end()
{
    return iterator(NULL);
}

template <typename Data>
typename MultiSet<Data>::const_iterator 
MultiSet<Data>::begin() const
{
    return iterator(getLeftMost(root_));
}

template <typename Data>
typename MultiSet<Data>::const_iterator 
MultiSet<Data>::end() const
{
    return const_iterator(NULL);
}

template <typename Data>
typename MultiSet<Data>::reverse_iterator 
MultiSet<Data>::rbegin()
{
    return reverse_iterator(getRightMost(root_));
}

template <typename Data>
typename MultiSet<Data>::reverse_iterator 
MultiSet<Data>::rend()
{
    return reverse_iterator(NULL);
}

template <typename Data>
typename MultiSet<Data>::const_reverse_iterator 
MultiSet<Data>::rbegin() const
{
    return const_reverse_iterator(getRightMost(root_));
}

template <typename Data>
typename MultiSet<Data>::const_reverse_iterator 
MultiSet<Data>::rend() const
{
    return const_reverse_iterator(NULL);
}

template <typename Data>
typename MultiSet<Data>::iterator
MultiSet<Data>::insert(const value_type& x)
{
    return insertHelper(iterator(root_), x);
}

template <typename Data>
typename MultiSet<Data>::iterator
MultiSet<Data>::insert(iterator it, const value_type& x)
{
    return insertHelper(it, x);    
}

template <typename Data>
typename MultiSet<Data>::iterator
MultiSet<Data>::insertHelper(iterator it, const value_type& x)
{
    if (empty()) { root_ = new Node(x); return begin(); }
    goUp(it, x);
    goDownAndInsert(it, x);
    balance(it); 
    return it;
}

template <typename Data>
void 
MultiSet<Data>::goUp(iterator& it, const value_type& x)
{
    if (!it.parent() || x == *it) { return; }
    const const_iterator temp = (x < *it) 
                              ? it.firstLeftParent()
                              : it.firstRightParent();
    if (isRoot(temp)) { return; }
    const bool isRightPlace = (x < *it) 
                            ? (*temp < x) 
                            : (*temp > x);
    if (isRightPlace) { return; }
    it.ptr_ = temp.ptr_;
    goUp(it, x);
}

template <typename Data>
void
MultiSet<Data>::goDownAndInsert(iterator& it, const value_type& x) 
{
    if (!it) { return; }
    if (it && x <= *it) {
        if (!it.left()) { it.createLeft(x); return; }
        return goDownAndInsert(it.goLeft(), x);
    }
    if (it && x >= *it) {
        if (!it.right()) { it.createRight(x); return; }
        return goDownAndInsert(it.goRight(), x);
    }
}

template <typename Data>
void 
MultiSet<Data>::balance(iterator& it)
{
    if (!it) { return; }
    const int factor = it.balance();
    if (factor > 1) {
        iterator itLeft = it.left();
        if (itLeft.balance() < 0) { rotateRight(itLeft); }
        rotateLeft(it);
        return;
    } 
    if (factor < -1) {
        iterator itRight = it.right();
        if (itRight.balance() > 0) { rotateLeft(itRight); }
        rotateRight(it);
        return;
    }
    balance(it.goParent()); 
} 

template <typename Data>
void 
MultiSet<Data>::rotateRight(iterator& it)
{
    iterator itParent = it.parent(), itRight = it.right();
    const bool isRightParent = it.isRightParent();
    it.setRight(itRight.left());
    if (itRight.left()) { itRight.left().setParent(it); }
    itRight.setLeft(it);
    it.setParent(itRight);
    itRight.setParent(itParent);
    if (itParent) {
        isRightParent ? itParent.setLeft(itRight)
                      : itParent.setRight(itRight);
    } else { root_ = itRight.getPtr(); }
    it = itRight;
}

template <typename Data>
void 
MultiSet<Data>::rotateLeft(iterator& it)
{
    iterator itParent = it.parent(), itLeft = it.left();
    const bool isLeftParent = it.isLeftParent();
    it.setLeft(itLeft.right());
    if (itLeft.right()) { itLeft.right().setParent(it); }
    itLeft.setRight(it);
    it.setParent(itLeft);
    itLeft.setParent(itParent);
    if (itParent) {
        isLeftParent ? itParent.setRight(itLeft)
                     : itParent.setLeft(itLeft);
    } else { root_ = itLeft.getPtr(); }
    it = itLeft;
}

template <typename Data>
template <typename InputIt>
void 
MultiSet<Data>::insert(InputIt first, InputIt last)
{
    while (first != last) { insert(*first++); }
}

template <typename Data>
void 
MultiSet<Data>::erase(iterator pos)
{
    assert(pos != end());
    Node* posNode = pos.getPtr();
    Node* replaceNode = getRightMost(posNode->left_);
    iterator posParent = pos.parent(); 
    if (NULL == replaceNode) {
        if (posParent) {
            pos.isRightParent() ? posNode->parent_->left_   = posNode->right_
                                : posNode->parent_->right_  = posNode->right_;
            if (posNode->right_) { posNode->right_->parent_ = posNode->parent_; } 
            delete posNode; 
            balance(posParent);
            return; 
        }
        Node* temp = root_; 
        root_ = root_->right_; 
        if (root_) { root_->parent_ = NULL; } 
        delete temp; 
        balance(posParent);
        return; 
    }
    if (posParent) {
        pos.isRightParent() ? posNode->parent_->left_  = replaceNode
                            : posNode->parent_->right_ = replaceNode;
    }
    iterator(replaceNode).isRightParent() ? replaceNode->parent_->left_  = replaceNode->left_
                                          : replaceNode->parent_->right_ = replaceNode->left_;
    if (replaceNode->left_) { replaceNode->left_->parent_ = replaceNode->parent_; }
    replaceNode->parent_ = posNode->parent_;
    
    replaceNode->right_ = posNode->right_;
    if (posNode->right_) { posNode->right_->parent_ = replaceNode; }
    
    replaceNode->left_ = posNode->left_;
    if (posNode->left_)  { posNode->left_->parent_  = replaceNode; }
    
    if (posNode == root_) { root_ = replaceNode; }
    delete posNode;
    balance(posParent);
    
}

template <typename Data>
typename MultiSet<Data>::size_type 
MultiSet<Data>::erase(const key_type& k)
{
    return eraseRangeHelper(lower_bound(k), upper_bound(k));
}

template <typename Data>
void 
MultiSet<Data>::erase(iterator first, iterator last)
{
    eraseRangeHelper(first, last);
    return void();
}

template <typename Data>
typename MultiSet<Data>::size_type 
MultiSet<Data>::eraseRangeHelper(iterator first, iterator last)
{
    int counter = 0;
    for (iterator it = first; it != last;) {
        ++it;
        erase(first);
        first = it;
        ++counter;
    }
    return counter;
}

template <typename Data>
typename MultiSet<Data>::iterator 
MultiSet<Data>::find(const key_type& key) const
{
    iterator it = lower_bound(key);
    return (it == end() || *it != key) ? iterator(NULL) : it;
}

template <typename Data>
typename MultiSet<Data>::size_type 
MultiSet<Data>::count(const key_type& key) const
{
    iterator it = lower_bound(key);
    int counter = 0;
    while (it != end() && key == *it) { ++it; ++counter; }
    return counter;
}

template <typename Data>
typename MultiSet<Data>::iterator 
MultiSet<Data>::lower_bound(const key_type& key) const
{
    return boundHelper(iterator(root_), key);
}

template <typename Data>
typename MultiSet<Data>::iterator 
MultiSet<Data>::upper_bound(const key_type& key) const
{
    iterator it = lower_bound(key);
    while (it != end() && key == *it) { ++it; }
    return it;
}

template <typename Data>
std::pair<typename MultiSet<Data>::iterator, typename MultiSet<Data>::iterator> 
MultiSet<Data>::equal_range(const key_type& k) const
{
    return std::make_pair(lower_bound(k), upper_bound(k));
}

template <typename Data>
typename MultiSet<Data>::iterator
MultiSet<Data>::boundHelper(iterator root, const key_type& key) const
{
    if (!root) { return root; }
    if (key <= *root) { 
        if (root.left()) { return boundHelper(root.left(), key); }
        return root;
    } 
    if (key >= *root) { 
        if (root.right()) { return boundHelper(root.right(), key); }
        return ++root;
    }
    assert(false);
    return root;
}

template <typename Data>
bool 
MultiSet<Data>::isRoot(const const_iterator& temp) const
{
    return temp == const_iterator(root_);
}
    
template <typename Data>
bool 
MultiSet<Data>::operator==(const MultiSet& rhv) const
{
    const_iterator first1 = begin();
    const_iterator first2 = rhv.begin();
    while (first1 && first2) {
        if (*first1 != *first2) { return false; }
        ++first1;
        ++first2;
    }
    return first1 == end() && first2 == rhv.end();
}

template <typename Data>
bool 
MultiSet<Data>::operator!=(const MultiSet& rhv) const
{
    return !(*this == rhv);
}

template <typename Data>
bool 
MultiSet<Data>::operator<(const MultiSet& rhv) const
{
    const_iterator first1 = begin();
    const_iterator first2 = rhv.begin();
    while (first1 && first2) {
        if (*first1 >= *first2) { return false; }
        ++first1;
        ++first2;
    }
    return first1 == end() && first2 != rhv.end();
}

template <typename Data>
bool 
MultiSet<Data>::operator<=(const MultiSet& rhv) const
{
    return !(rhv < *this);
}

template <typename Data>
bool
MultiSet<Data>::operator>(const MultiSet& rhv) const
{
    return rhv < *this;
}

template <typename Data>
bool 
MultiSet<Data>::operator>=(const MultiSet& rhv) const
{
    return !(*this < rhv);
}

template <typename Data>
void 
MultiSet<Data>::preOrderIter(std::ostream& out) const
{
    std::stack<Node*> st;
    Node* temp = root_;
    st.push(root_);
    while (!st.empty()) {
        out << temp->data_ << ' ';
        if (temp->right_) { st.push(temp->right_); }
        if (temp->left_)  { 
            temp = temp->left_; 
            continue; 
        }
        temp = st.top(); 
        st.pop(); 
    }
}

template <typename Data>
void 
MultiSet<Data>::preOrderRec(std::ostream& out) const
{
    preOrderHelper(root_, out);
}

template <typename Data>
void 
MultiSet<Data>::preOrderHelper(Node* root, std::ostream& out) const
{
    if (NULL == root) { return; }
    out << root->data_ << ' ';
    preOrderHelper(root->left_, out);
    preOrderHelper(root->right_, out);
}

template <typename Data>
void 
MultiSet<Data>::inOrderIter(std::ostream& out) const
{
    for (const_iterator it = begin(); it != end(); ++it) {
        out << *it << ' ';
    }
}

template <typename Data>
void 
MultiSet<Data>::inOrderRec(std::ostream& out) const
{
    inOrderHelper(root_, out);
}

template <typename Data>
void 
MultiSet<Data>::inOrderHelper(Node* root, std::ostream& out) const
{
    if (NULL == root) { return; }
    inOrderHelper(root->left_, out);
    out << root->data_ << ' ';
    inOrderHelper(root->right_, out);
}

template <typename Data>
void 
MultiSet<Data>::postOrderIter(std::ostream& out) const
{
    std::stack<Node*> stk1, stk2;
    stk1.push(root_);
    Node* current;

    while (!stk1.empty()) {
        current = stk1.top();
        stk1.pop();
        stk2.push(current);
        if (current->left_)  { stk1.push(current->left_);  }
        if (current->right_) { stk1.push(current->right_); }
    }

    while (!stk2.empty()) {
        current = stk2.top();
        stk2.pop();
        out << current->data_ << ' ';
    }
}

template <typename Data>
void 
MultiSet<Data>::postOrderRec(std::ostream& out) const
{
    postOrderHelper(root_, out);
}

template <typename Data>
void 
MultiSet<Data>::postOrderHelper(Node* root, std::ostream& out) const
{
    if (NULL == root) { return; }
    postOrderHelper(root->left_, out);
    postOrderHelper(root->right_, out);
    out << root->data_ << ' ';
}

template <typename Data>
void 
MultiSet<Data>::levelOrderIter(std::ostream& out) const
{
    std::queue<Node*> que;
    que.push(root_);
    while (!que.empty()) {
        Node* temp = que.front();
        out << temp->data_ << ' ';
        if (temp->left_)  { que.push(temp->left_);  }
        if (temp->right_) { que.push(temp->right_); }
        que.pop();
    }
}

template <typename Data>
void 
MultiSet<Data>::outputTree(Node* ptr, std::ostream& out, const int totalSpaces) const
{
    if (NULL == ptr) { return; }
    outputTree(ptr->right_, out, totalSpaces + 5);
    out << std::setw(totalSpaces) << ptr->data_ << std::endl;
    outputTree(ptr->left_, out, totalSpaces + 5);
}

template <typename Data>
void
MultiSet<Data>::print(std::ostream& out) const
{
    inOrderIter(out);
    out << std::endl;
}

template <typename Data>
typename MultiSet<Data>::Node* 
MultiSet<Data>::getRightMost(Node* rhv)
{
    if (NULL == rhv) { return rhv; }
    while (rhv->right_ != NULL) { rhv = rhv->right_; }
    return rhv;
}


template <typename Data>
typename MultiSet<Data>::Node* 
MultiSet<Data>::getLeftMost(Node* rhv)
{
    if (NULL == rhv) { return rhv; }
    while (rhv->left_ != NULL) { rhv = rhv->left_; }
    return rhv;
}

/// const_iterator

template <typename Data>
MultiSet<Data>::const_iterator::const_iterator()
    : ptr_(NULL)
{}

template <typename Data>
MultiSet<Data>::const_iterator::const_iterator(Node* ptr)
    : ptr_(ptr)
{}

template <typename Data>
MultiSet<Data>::const_iterator::const_iterator(const const_iterator& rhv)
    : ptr_(rhv.ptr_)
{}

template <typename Data>
MultiSet<Data>::const_iterator::~const_iterator()
{
    destroy();
}

template <typename Data>
void 
MultiSet<Data>::const_iterator::destroy()
{
    ptr_ = NULL;
}

template <typename Data>
const typename MultiSet<Data>::const_iterator& 
MultiSet<Data>::const_iterator::operator=(const const_iterator& rhv)
{
    ptr_ = rhv.ptr_;
    return *this;
}

template <typename Data>
typename MultiSet<Data>::const_reference
MultiSet<Data>::const_iterator::operator*() const
{
    return ptr_->data_;
}

template <typename Data>
const typename MultiSet<Data>::value_type*
MultiSet<Data>::const_iterator::operator->() const
{
    return &ptr_->data_;
}

template <typename Data>
typename MultiSet<Data>::const_iterator 
MultiSet<Data>::const_iterator::operator++()
{
    if (NULL == ptr_->right_) { 
        while (isLeftParent()) {
            goParent();
        }
        goParent();
        return *this;
    }
    ptr_ = getLeftMost(ptr_->right_);
    return *this;
}

template <typename Data>
typename MultiSet<Data>::const_iterator 
MultiSet<Data>::const_iterator::operator++(int)
{
    Node* temp = ptr_;
    if (NULL == ptr_->right_) { 
        while (isLeftParent()) {
            goParent();
        }
        goParent();
        return const_iterator(temp);
    }
    ptr_ = getLeftMost(ptr_->right_);
    return const_iterator(temp);
}

template <typename Data>
typename MultiSet<Data>::const_iterator 
MultiSet<Data>::const_iterator::operator--()
{
    if (NULL == ptr_->left_) { 
        while (isRightParent()) {
            goParent();
        }
        goParent();
        return *this;
    }
    ptr_ = getRightMost(ptr_->left_);
    return *this;
}

template <typename Data>
typename MultiSet<Data>::const_iterator 
MultiSet<Data>::const_iterator::operator--(int)
{
    Node* temp = ptr_;
    if (NULL == ptr_->left_) { 
        while (isRightParent()) {
            goParent();
        }
        goParent();
        return const_iterator(temp);
    }
    ptr_ = getRightMost(ptr_->left_);
    return const_iterator(temp);
}

template <typename Data>
bool
MultiSet<Data>::const_iterator::isLeftParent() const
{
    return ptr_->parent_ != NULL && ptr_->parent_->right_ == ptr_;
}

template <typename Data>
bool
MultiSet<Data>::const_iterator::isRightParent() const
{
    return ptr_->parent_ != NULL && ptr_->parent_->left_ == ptr_;
}

template <typename Data>
bool 
MultiSet<Data>::const_iterator::operator==(const const_iterator& rhv) const
{
    return ptr_ == rhv.ptr_;
}

template <typename Data>
bool 
MultiSet<Data>::const_iterator::operator!=(const const_iterator& rhv) const
{
    return !(*this == rhv);
}

template <typename Data>
bool 
MultiSet<Data>::const_iterator::operator!() const
{
    return NULL == ptr_;
}

template <typename Data>
typename MultiSet<Data>::Node* 
MultiSet<Data>::const_iterator::getPtr() const
{
    return ptr_;
}

template <typename Data>
void 
MultiSet<Data>::const_iterator::setPtr(Node* temp)
{
    ptr_ = temp;
}

template <typename Data>
typename MultiSet<Data>::const_iterator 
MultiSet<Data>::const_iterator::parent() const
{
    return const_iterator(ptr_->parent_);
}

template <typename Data>
typename MultiSet<Data>::const_iterator 
MultiSet<Data>::const_iterator::left() const
{
    return const_iterator(ptr_->left_);
}

template <typename Data>
typename MultiSet<Data>::const_iterator 
MultiSet<Data>::const_iterator::right() const
{
    return const_iterator(ptr_->right_);
}

template <typename Data>
const typename MultiSet<Data>::const_iterator& 
MultiSet<Data>::const_iterator::goParent()
{
    ptr_ = ptr_->parent_;
    return *this;
}

template <typename Data>
const typename MultiSet<Data>::const_iterator& 
MultiSet<Data>::const_iterator::goLeft()
{
    ptr_ = ptr_->left_;
    return *this;
}

template <typename Data>
const typename MultiSet<Data>::const_iterator& 
MultiSet<Data>::const_iterator::goRight()
{
    ptr_ = ptr_->right_;
    return *this;
}

template <typename Data>
typename MultiSet<Data>::const_iterator 
MultiSet<Data>::const_iterator::firstLeftParent() const
{
    if (!this->parent()) { return const_iterator(NULL); }
    const_iterator p = this->parent();
    if (p.right() == *this) { return p; }
    return p.firstLeftParent(); 
}

template <typename Data>
typename MultiSet<Data>::const_iterator 
MultiSet<Data>::const_iterator::firstRightParent() const
{
    if (!this->parent()) { return const_iterator(NULL); }
    const_iterator p = this->parent();
    if (p.left() == *this) { return p; }
    return p.firstRightParent(); 
}

template <typename Data>
void 
MultiSet<Data>::const_iterator::createLeft(const value_type& x)
{
    ptr_->left_ = new Node(x, ptr_);
}

template <typename Data>
void 
MultiSet<Data>::const_iterator::createRight(const value_type& x)
{
    ptr_->right_ = new Node(x, ptr_);
}

template <typename Data>
void
MultiSet<Data>::const_iterator::setParent(const_iterator it)
{
    ptr_->parent_ = it.getPtr();
}

template <typename Data>
void
MultiSet<Data>::const_iterator::setLeft(const_iterator it)
{
    ptr_->left_ = it.getPtr();
}

template <typename Data>
void
MultiSet<Data>::const_iterator::setRight(const_iterator it)
{
    ptr_->right_ = it.getPtr();
}

template <typename Data>
int 
MultiSet<Data>::const_iterator::balance() const
{
    return left().depth() - right().depth();
}

template <typename Data>
int
MultiSet<Data>::const_iterator::depth() const
{
    if (NULL == ptr_) { return 0; }
    const int leftDepth = left().depth();
    const int rightDepth = right().depth();
    return std::max(leftDepth, rightDepth) + 1;
}

template <typename Data>
MultiSet<Data>::const_iterator::operator bool() const
{
    return NULL != ptr_;
}

/// iterator

template <typename Data>
MultiSet<Data>::iterator::iterator()
    : const_iterator()
{}

template <typename Data>
MultiSet<Data>::iterator::iterator(Node* ptr)
    : const_iterator(ptr)
{}

template <typename Data>
MultiSet<Data>::iterator::iterator(const iterator& rhv)
    : const_iterator(rhv)
{}

template <typename Data>
MultiSet<Data>::iterator::~iterator()
{
    this->destroy();
}

template <typename Data>
const typename MultiSet<Data>::iterator& 
MultiSet<Data>::iterator::operator=(const iterator& rhv)
{
    this->setPtr(rhv.getPtr());
    return *this;
}

template <typename Data>
typename MultiSet<Data>::reference 
MultiSet<Data>::iterator::operator*()
{
    return this->getPtr()->data_;
}

template <typename Data>
typename MultiSet<Data>::value_type*
MultiSet<Data>::iterator::operator->()
{
    return &this->getPtr()->data_;
}

template <typename Data>
typename MultiSet<Data>::iterator 
MultiSet<Data>::iterator::operator++()
{
    if (NULL == this->getPtr()->right_) { 
        while (this->isLeftParent()) {
            this->goParent();
        }
        this->goParent();
        return *this;
    }
    this->setPtr(getLeftMost(this->getPtr()->right_));
    return *this;
}

template <typename Data>
typename MultiSet<Data>::iterator 
MultiSet<Data>::iterator::operator++(int)
{
    Node* temp = this->getPtr();
    if (NULL == temp->right_) { 
        while (this->isLeftParent()) {
            this->goParent();
        }
        this->goParent();
        return iterator(temp);
    }
    this->setPtr(getLeftMost(this->getPtr()->right_));
    return iterator(temp);
}

template <typename Data>
typename MultiSet<Data>::iterator 
MultiSet<Data>::iterator::operator--()
{
    if (NULL == this->getPtr()->left_) { 
        while (this->isRightParent()) {
            this->goParent();
        }
        this->goParent();
        return *this;
    }
    this->setPtr(getRightMost(this->getPtr()->left_));
    return *this;
}

template <typename Data>
typename MultiSet<Data>::iterator 
MultiSet<Data>::iterator::operator--(int)
{
    Node* temp = this->getPtr();
    if (NULL == this->getPtr()->left_) { 
        while (this->isRightParent()) {
            this->goParent();
        }
        this->goParent();
        return iterator(temp);
    }
    this->setPtr(getRightMost(this->getPtr()->left_));
    return iterator(temp);
}

template <typename Data>
typename MultiSet<Data>::iterator 
MultiSet<Data>::iterator::parent()
{
    Node* temp = this->getPtr();
    return NULL == temp ? iterator(temp) : iterator(temp->parent_);
}

template <typename Data>
typename MultiSet<Data>::iterator 
MultiSet<Data>::iterator::left()
{
    Node* temp = this->getPtr();
    return NULL == temp ? iterator(temp) : iterator(temp->left_);
}

template <typename Data>
typename MultiSet<Data>::iterator 
MultiSet<Data>::iterator::right()
{
    Node* temp = this->getPtr();
    return NULL == temp ? iterator(temp) : iterator(temp->right_);
}

template <typename Data>
typename MultiSet<Data>::iterator& 
MultiSet<Data>::iterator::goParent()
{
    const_iterator::goParent();
    return *this;
}

template <typename Data>
typename MultiSet<Data>::iterator& 
MultiSet<Data>::iterator::goLeft()
{
    const_iterator::goLeft();
    return *this;
}

template <typename Data>
typename MultiSet<Data>::iterator&
MultiSet<Data>::iterator::goRight()
{
    const_iterator::goRight();
    return *this;
}

/// const_reverse_iterator

template <typename Data>
MultiSet<Data>::const_reverse_iterator::const_reverse_iterator()
    : ptr_(NULL)
{}

template <typename Data>
MultiSet<Data>::const_reverse_iterator::const_reverse_iterator(Node* ptr)
    : ptr_(ptr)
{}

template <typename Data>
MultiSet<Data>::const_reverse_iterator::const_reverse_iterator(const const_reverse_iterator& rhv)
    : ptr_(rhv.ptr_)
{}

template <typename Data>
MultiSet<Data>::const_reverse_iterator::~const_reverse_iterator()
{
    destroy();
}

template <typename Data>
void 
MultiSet<Data>::const_reverse_iterator::destroy()
{
    ptr_ = NULL;
}

template <typename Data>
const typename MultiSet<Data>::const_reverse_iterator& 
MultiSet<Data>::const_reverse_iterator::operator=(const const_reverse_iterator& rhv)
{
    ptr_ = rhv.ptr_;
    return *this;
}

template <typename Data>
typename MultiSet<Data>::const_reference
MultiSet<Data>::const_reverse_iterator::operator*() const
{
    return ptr_->data_;
}

template <typename Data>
const typename MultiSet<Data>::value_type*
MultiSet<Data>::const_reverse_iterator::operator->() const
{
    return &ptr_->data_;
}

template <typename Data>
typename MultiSet<Data>::const_reverse_iterator 
MultiSet<Data>::const_reverse_iterator::operator++()
{
    if (NULL == ptr_->left_) { 
        while (isRightParent()) {
            goParent();
        }
        goParent();
        return *this;
    }
    ptr_ = getRightMost(ptr_->left_);
    return *this;
}

template <typename Data>
typename MultiSet<Data>::const_reverse_iterator 
MultiSet<Data>::const_reverse_iterator::operator++(int)
{
    Node* temp = ptr_;
    if (NULL == ptr_->right_) { 
        while (isLeftParent()) {
            goParent();
        }
        goParent();
        return const_reverse_iterator(temp);
    }
    ptr_ = getLeftMost(ptr_->right_);
    return const_reverse_iterator(temp);
}

template <typename Data>
typename MultiSet<Data>::const_reverse_iterator 
MultiSet<Data>::const_reverse_iterator::operator--(int)
{
    Node* temp = ptr_;
    if (NULL == ptr_->right_) { 
        while (isLeftParent()) {
            goParent();
        }
        goParent();
        return const_reverse_iterator(temp);
    }
    ptr_ = getLeftMost(ptr_->right_);
    return const_reverse_iterator(temp);
}

template <typename Data>
bool
MultiSet<Data>::const_reverse_iterator::isLeftParent() const
{
    return ptr_->parent_ != NULL && ptr_->parent_->right_ == ptr_;
}

template <typename Data>
bool
MultiSet<Data>::const_reverse_iterator::isRightParent() const
{
    return ptr_->parent_ != NULL && ptr_->parent_->left_ == ptr_;
}

template <typename Data>
bool 
MultiSet<Data>::const_reverse_iterator::operator==(const const_reverse_iterator& rhv) const
{
    return ptr_ == rhv.ptr_;
}

template <typename Data>
bool 
MultiSet<Data>::const_reverse_iterator::operator!=(const const_reverse_iterator& rhv) const
{
    return !(*this == rhv);
}

template <typename Data>
bool 
MultiSet<Data>::const_reverse_iterator::operator!() const
{
    return NULL == ptr_;
}

template <typename Data>
typename MultiSet<Data>::Node* 
MultiSet<Data>::const_reverse_iterator::getPtr() const
{
    return ptr_;
}

template <typename Data>
void 
MultiSet<Data>::const_reverse_iterator::setPtr(Node* temp)
{
    ptr_ = temp;
}

template <typename Data>
const typename MultiSet<Data>::const_reverse_iterator&
MultiSet<Data>::const_reverse_iterator::goParent()
{
    ptr_ = ptr_->parent_;
    return *this;
}

/// reverse_iterator

template <typename Data>
MultiSet<Data>::reverse_iterator::reverse_iterator()
    : const_reverse_iterator()
{}

template <typename Data>
MultiSet<Data>::reverse_iterator::reverse_iterator(Node* ptr)
    : const_reverse_iterator(ptr)
{}

template <typename Data>
MultiSet<Data>::reverse_iterator::reverse_iterator(const reverse_iterator& rhv)
    : const_reverse_iterator(rhv)
{}

template <typename Data>
MultiSet<Data>::reverse_iterator::~reverse_iterator()
{
    this->destroy();
}

template <typename Data>
const typename MultiSet<Data>::reverse_iterator& 
MultiSet<Data>::reverse_iterator::operator=(const reverse_iterator& rhv)
{
    this->setPtr(rhv.getPtr());
    return *this;
}

template <typename Data>
typename MultiSet<Data>::reference 
MultiSet<Data>::reverse_iterator::operator*()
{
    return this->getPtr()->data_;
}

template <typename Data>
typename MultiSet<Data>::value_type*
MultiSet<Data>::reverse_iterator::operator->()
{
    return &this->getPtr()->data_;
}

template <typename Data>
typename MultiSet<Data>::reverse_iterator 
MultiSet<Data>::reverse_iterator::operator++()
{
    if (NULL == this->getPtr()->left_) { 
        while (this->isRightParent()) {
            this->goParent();
        }
        this->goParent();
        return *this;
    }
    this->setPtr(getRightMost(this->getPtr()->left_));
    return *this;
}

template <typename Data>
typename MultiSet<Data>::reverse_iterator 
MultiSet<Data>::reverse_iterator::operator++(int)
{
    Node* temp = this->getPtr();
    if (NULL == this->getPtr()->left_) { 
        while (this->isRightParent()) {
            this->goParent();
        }
        this->goParent();
        return reverse_iterator(temp);
    }
    this->setPtr(getRightMost(this->getPtr()->left_));
    return reverse_iterator(temp);
}

template <typename Data>
typename MultiSet<Data>::reverse_iterator 
MultiSet<Data>::reverse_iterator::operator--()
{
    if (NULL == this->getPtr()->right_) { 
        while (this->isLeftParent()) {
            this->goParent();
        }
        this->goParent();
        return *this;
    }
    this->setPtr(getLeftMost(this->getPtr()->right_));
    return *this;
}

template <typename Data>
typename MultiSet<Data>::reverse_iterator 
MultiSet<Data>::reverse_iterator::operator--(int)
{
    Node* temp = this->getPtr();
    if (NULL == temp->right_) { 
        while (this->isLeftParent()) {
            this->goParent();
        }
        this->goParent();
        return reverse_iterator(temp);
    }
    this->setPtr(getLeftMost(this->getPtr()->right_));
    return reverse_iterator(temp);
}

template <typename Data>
typename MultiSet<Data>::reverse_iterator& 
MultiSet<Data>::reverse_iterator::goParent()
{
    const_reverse_iterator::goParent();
    return *this;
}

