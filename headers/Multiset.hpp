#ifndef __MULTI_SET_T_HPP__
#define __MULTI_SET_T_HPP__

#include <iostream>
#include <utility>

template <typename Data>
class MultiSet 
{
    template <typename T> friend std::ostream& operator<<(std::ostream& out, const MultiSet<T>& rhv);
private:
    struct Node {
        Node(const Data& data,
                   Node* parent = NULL,
                   Node* left = NULL,
                   Node* right = NULL)
            : data_(data)
            , parent_(parent), left_(left), right_(right)
        {}
        Data data_;
        Node* parent_;
        Node* left_;
        Node* right_;
    };
public:
    typedef Data value_type;
    typedef Data key_type;
    typedef value_type& reference;
    typedef const value_type& const_reference;
    typedef value_type* pointer;
    typedef std::ptrdiff_t difference_type;
    typedef std::size_t size_type;

private:
    static Node* getRightMost(Node* rhv);
    static Node* getLeftMost(Node* rhv);

public:
    MultiSet();
    MultiSet(const MultiSet& rhv); 
    template <typename InputIterator>
    MultiSet(InputIterator first, InputIterator last); 
    ~MultiSet();
    const MultiSet& operator=(const MultiSet& rhv);
    void swap(MultiSet& rhv);
    size_type size() const;
    size_type max_size() const;
    void clear();
     
    bool empty() const; 
    bool operator==(const MultiSet& rhv) const;
    bool operator!=(const MultiSet& rhv) const;
    bool operator< (const MultiSet& rhv) const;
    bool operator<=(const MultiSet& rhv) const;
    bool operator> (const MultiSet& rhv) const;
    bool operator>=(const MultiSet& rhv) const;

    class const_iterator {
        friend class MultiSet<Data>; 
    public:
        const_iterator();
        const_iterator(const const_iterator& rhv);
        ~const_iterator();
        const const_iterator& operator=(const const_iterator& rhv); 
        const value_type& operator*() const;
        const value_type* operator->() const;
        const_iterator operator++();
        const_iterator operator++(int);
        const_iterator operator--();
        const_iterator operator--(int);
        bool operator==(const const_iterator& rhv) const;
        bool operator!=(const const_iterator& rhv) const;
        bool operator!() const;
    protected:
        Node* getPtr() const;
        void setPtr(Node* temp);
        void destroy();
        const_iterator parent() const;
        const_iterator left() const;
        const_iterator right() const;
        const const_iterator& goParent();
        const const_iterator& goLeft();
        const const_iterator& goRight();
        void setParent(const_iterator it);
        void setLeft(const_iterator it);
        void setRight(const_iterator it);
        void createLeft(const value_type& x);
        void createRight(const value_type& x);
        const_iterator firstLeftParent() const;
        const_iterator firstRightParent() const;
        bool isLeftParent() const;
        bool isRightParent() const;
        int balance() const;
        int depth() const;
        operator bool() const;
    private:
        explicit const_iterator(Node* ptr);
    private:
        Node* ptr_;
    };

    class iterator : public const_iterator {
        friend class MultiSet<Data>; 
    public:
        iterator();
        iterator(const iterator& rhv);
        ~iterator();
        const iterator& operator=(const iterator& rhv); 
        value_type& operator*();
        value_type* operator->();
        iterator operator++();
        iterator operator++(int);
        iterator operator--();
        iterator operator--(int);
    private:
        explicit iterator(Node* ptr);
        iterator parent();
        iterator left();
        iterator right();
        iterator& goParent();
        iterator& goLeft();
        iterator& goRight();
    };

    class const_reverse_iterator {
        friend class MultiSet<Data>; 
    public:
        const_reverse_iterator();
        const_reverse_iterator(const const_reverse_iterator& rhv);
        ~const_reverse_iterator();
        const const_reverse_iterator& operator=(const const_reverse_iterator& rhv); 
        const value_type& operator*() const;
        const value_type* operator->() const;
        const_reverse_iterator operator++();
        const_reverse_iterator operator++(int);
        const_reverse_iterator operator--();
        const_reverse_iterator operator--(int);
        bool operator==(const const_reverse_iterator& rhv) const;
        bool operator!=(const const_reverse_iterator& rhv) const;
        bool operator!() const;
    protected:
        Node* getPtr() const;
        void setPtr(Node* temp);
        void destroy();
        const const_reverse_iterator& goParent();
        bool isLeftParent() const;
        bool isRightParent() const;
    private:
        explicit const_reverse_iterator(Node* ptr);
    private:
        Node* ptr_;
    };
    
    class reverse_iterator : public const_reverse_iterator {
        friend class MultiSet<Data>;
    public:
        reverse_iterator();
        reverse_iterator(const reverse_iterator& rhv);
        ~reverse_iterator();
        const reverse_iterator& operator=(const reverse_iterator& rhv); 
        value_type& operator*();
        value_type* operator->();
        reverse_iterator operator++();
        reverse_iterator operator++(int);
        reverse_iterator operator--();
        reverse_iterator operator--(int);
    private:
        explicit reverse_iterator(Node* ptr);
        reverse_iterator& goParent();
    };

    iterator begin();
    iterator end();
    const_iterator begin() const;
    const_iterator end() const;
    reverse_iterator rbegin();
    reverse_iterator rend();
    const_reverse_iterator rbegin() const;
    const_reverse_iterator rend() const;

    iterator insert(const value_type& x);
    iterator insert(iterator pos, const value_type& x);
    template <typename InputIt>
    void insert(InputIt first, InputIt last);

    void erase(iterator pos);
    size_type erase(const key_type& k);
    void erase(iterator first, iterator last);

    iterator find(const key_type& k) const;
    size_type count(const key_type& k) const;
    iterator lower_bound(const key_type& k) const;
    iterator upper_bound(const key_type& k) const;
    std::pair<iterator, iterator> equal_range(const key_type& k) const;
    void print(std::ostream& out = std::cout) const;
    void preOrderIter(std::ostream& out = std::cout) const;
    void preOrderRec(std::ostream& out = std::cout) const;
    void inOrderIter(std::ostream& out = std::cout) const;
    void inOrderRec(std::ostream& out = std::cout) const;
    void postOrderIter(std::ostream& out = std::cout) const;
    void postOrderRec(std::ostream& out = std::cout) const;
    void levelOrderIter(std::ostream& out = std::cout) const;
private:
    void preOrderHelper(Node* root, std::ostream& out = std::cout) const;
    void inOrderHelper(Node* root, std::ostream& out = std::cout) const;
    void postOrderHelper(Node* root, std::ostream& out = std::cout) const;
    void outputTree(Node* ptr, std::ostream& out, const int totalSpaces = 0) const;
    iterator findHelper(iterator root, const key_type& key) const;
    iterator boundHelper(iterator root, const key_type& key) const;
    void goUp(iterator& it, const value_type& x);
    void goDownAndInsert(iterator& it, const value_type& x);
    void rotateRight(iterator& it);
    void rotateLeft(iterator& it);
    void balance(iterator& it);
    bool isRoot(const const_iterator& temp) const;
    void clearHelper(Node*& root); 
    iterator insertHelper(iterator it, const value_type& x);
    size_type eraseRangeHelper(iterator first, iterator last);
private:
    Node* root_;

};

#include "templates/Multiset.cpp"
#endif /// __MULTI_SET_T_HPP__


