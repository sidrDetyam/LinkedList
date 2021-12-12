//
// Created by argem on 10.12.2021.
//

#ifndef LINKEDLIST_LINKEDLIST_H
#define LINKEDLIST_LINKEDLIST_H


#define DEBUG_LL

#include <algorithm>
#include <memory>
#include <iterator>


template<typename T, typename Allocator = std::allocator<T>>
class LinkedList;

namespace detail{


    template<typename T>
    struct ListNode{
        T* object_;
        ListNode* next_;
        ListNode* prev_;
    };


    template<typename T, typename Allocator>
    class ConstListIterator: public std::iterator<std::bidirectional_iterator_tag,
            const T,std::ptrdiff_t, const T*, const T&> {

    private:
        ListNode<T>* ptr_;
    public:

        friend class LinkedList<T, Allocator>;

        explicit ConstListIterator(ListNode<T>* ptr): ptr_(ptr){};
        explicit ConstListIterator(const ListNode<T>* ptr):
        ptr_(const_cast<ListNode<T>*>(ptr)){};

        ConstListIterator& operator ++(){
            ptr_ = ptr_->next_;
            return *this;
        }

        ConstListIterator operator ++(int){
            ConstListIterator ret(ptr_);
            ptr_ = ptr_->next_;
            return ret;
        }

        ConstListIterator& operator --(){
            ptr_ = ptr_->prev_;
            return *this;
        }

        ConstListIterator operator --(int){
            ConstListIterator ret(ptr_);
            ptr_ = ptr_->prev_;
            return ret;
        }

        bool operator ==(const ConstListIterator& other) const{
            return ptr_ == other.ptr_;
        }
        bool operator !=(const ConstListIterator& other) const{
            return ptr_ != other.ptr_;
        }

        const T& operator *() const{
            return *ptr_->object_;
        }
    };


    template<typename T, typename Allocator>
    class ListIterator: public std::iterator<std::bidirectional_iterator_tag,
            T,std::ptrdiff_t,T*,T&> {

    private:
        ListNode<T>* ptr_;
    public:

        friend class LinkedList<T, Allocator>;

        explicit ListIterator(ListNode<T>* ptr): ptr_(ptr){};

        ListIterator& operator ++(){
            ptr_ = ptr_->next_;
            return *this;
        }

        ListIterator operator ++(int){
            ListIterator ret(ptr_);
            ptr_ = ptr_->next_;
            return ret;
        }

        ListIterator& operator --(){
            ptr_ = ptr_->prev_;
            return *this;
        }

        ListIterator operator --(int){
            ListIterator ret(ptr_);
            ptr_ = ptr_->prev_;
            return ret;
        }

        bool operator ==(const ListIterator& other) const{
            return ptr_ == other.ptr_;
        }
        bool operator !=(const ListIterator& other) const{
            return ptr_ != other.ptr_;
        }

        T& operator *(){
            return *ptr_->object_;
        }

        operator ConstListIterator<T, Allocator>() const{
            return ConstListIterator<T, Allocator>(ptr_);
        }
    };
}



template<typename T, typename Allocator>
class LinkedList {

public:

    using value_type = T;
    using const_value_type = const T;
    using size_type = std::size_t;
    using allocator_type = Allocator;
    using difference_type = std::ptrdiff_t;
    using reference = value_type&;
    using const_reference = const value_type&;
    using pointer = typename std::allocator_traits<Allocator>::pointer;
    using const_pointer = typename std::allocator_traits<Allocator>::const_pointer;

public:

    using iterator = detail::ListIterator<value_type, Allocator>;
    using const_iterator = detail::ConstListIterator<value_type, Allocator>;
    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

public:

    LinkedList();
    LinkedList(const LinkedList& other);
    LinkedList(LinkedList&& other) noexcept;
    LinkedList(size_type count, const T& value);
    explicit LinkedList(size_type count);
    template<typename InputIt>
    LinkedList(InputIt first, InputIt last);
    LinkedList(std::initializer_list<T> init);
    ~LinkedList();

public:

    LinkedList& operator=(const LinkedList& other);
    LinkedList& operator=(LinkedList&& other) noexcept;
    LinkedList& operator=(std::initializer_list<T> ilist);

    void assign(size_type count, const T& value );
    template<typename InputIt>
    void assign(InputIt first, InputIt last);
    void assign(std::initializer_list<T> ilist);

public:

    reference front();
    const_reference front() const;
    reference back();
    const_reference back() const;

public:

    iterator begin() noexcept;
    const_iterator begin() const noexcept;
    const_iterator cbegin() const noexcept;
    iterator end() noexcept;
    const_iterator end() const noexcept;
    const_iterator cend() const noexcept;

    reverse_iterator rbegin() noexcept;
    const_reverse_iterator rbegin() const noexcept;
    const_reverse_iterator crbegin() const noexcept;
    reverse_iterator rend() noexcept;
    const_reverse_iterator rend() const noexcept;
    const_reverse_iterator crend() const noexcept;

public:

    [[nodiscard]] size_type size() const noexcept;
    [[nodiscard]] bool empty() const;

private:

    template<typename U>
    iterator insert_(const_iterator pos, U&& value);

public:

    iterator insert(const_iterator pos, const T& value);
    iterator insert(const_iterator pos, T&& value);
    iterator insert(const_iterator pos, size_type count, const T& value);
    template<typename InputIt>
    iterator insert(const_iterator pos, InputIt first, InputIt last);
    iterator insert(const_iterator pos, std::initializer_list<T> ilist);

    void push_front(const T& value);
    void push_front(T&& value);
    void push_back(const T& value);
    void push_back(T&& value);

public:

    iterator erase(const_iterator pos);
    iterator erase(const_iterator first, const_iterator last);

    void pop_back();
    void pop_front();

private:

    template<typename Func, typename ...Args>
    size_type remove_some_elements_(Func, Args&&...);

public:

    size_type unique();
    template<typename BinaryPredicate>
    size_type unique(BinaryPredicate p);
    size_type remove(const T& value);
    template<typename UnaryPredicate>
    size_type remove_if(UnaryPredicate p);

public:

    void clear() noexcept;
    void resize(size_type count, const value_type& value);
    void resize(size_type count);
    void swap(LinkedList& other) noexcept;
    void sort();
    template<typename Compare>
    void sort(Compare comp);

    void reverse() noexcept;

private:

    detail::ListNode<value_type> base_;
    size_type size_{};

};


/* insert by universal reference */
template<typename T, typename Allocator>
template<typename U>
typename LinkedList<T, Allocator>::iterator
LinkedList<T, Allocator>::insert_(LinkedList::const_iterator pos, U &&value) {

    static_assert(std::is_same_v<std::decay_t<T>, std::decay_t<U>>);

    Allocator alloc;
    typename Allocator::template rebind<detail::ListNode<value_type>>::other other_alloc;

    detail::ListNode<value_type>* new_node = std::allocator_traits<decltype(other_alloc)>::allocate(other_alloc, 1ull);
    new_node->object_ = std::allocator_traits<Allocator>::allocate(alloc, 1ull);
    std::allocator_traits<Allocator>::construct(alloc, new_node->object_, std::forward<U>(value));

    auto prev = pos.ptr_->prev_;
    prev->next_ = new_node;
    new_node->prev_ = prev;
    pos.ptr_->prev_ = new_node;
    new_node->next_ = pos.ptr_;
    ++size_;

    return iterator(new_node);
}


/* insert methods */
template<typename T, typename Allocator>
typename LinkedList<T, Allocator>::iterator
LinkedList<T, Allocator>::insert(LinkedList::const_iterator pos, const T &value) {

    return insert_(pos, value);
}


template<typename T, typename Allocator>
typename LinkedList<T, Allocator>::iterator
LinkedList<T, Allocator>::insert(LinkedList::const_iterator pos, T &&value) {

    return insert_(pos, std::move(value));
}


template<typename T, typename Allocator>
typename LinkedList<T, Allocator>::iterator
LinkedList<T, Allocator>::insert(LinkedList::const_iterator pos, LinkedList::size_type count, const T &value) {

    if(count==0){
        return pos;
    }

    auto ret = insert(pos, value);
    for(size_type i=1; i<count; ++i){
        insert(pos, value);
    }

    return ret;
}


template<typename T, typename Allocator>
template<typename InputIt>
typename LinkedList<T, Allocator>::iterator
LinkedList<T, Allocator>::insert(LinkedList::const_iterator pos, InputIt first, InputIt last) {

    if(first==last){
        return iterator(pos.ptr_);
    }

    auto ret = insert(pos, std::forward<decltype(*first)>(*first));

    for(auto it = ++first; it!=last; ++it){
        insert(pos, std::forward<decltype(*it)>(*it));
    }

    return ret;
}


template<typename T, typename Allocator>
typename LinkedList<T, Allocator>::iterator
LinkedList<T, Allocator>::insert(LinkedList::const_iterator pos, std::initializer_list<T> ilist) {

    return insert(pos, ilist.begin(), ilist.end());
}


template<typename T, typename Allocator>
void LinkedList<T, Allocator>::push_front(const T &value) {
    insert(cbegin(), value);
}


template<typename T, typename Allocator>
void LinkedList<T, Allocator>::push_front(T &&value) {
    insert(cbegin(), std::move(value));
}


template<typename T, typename Allocator>
void LinkedList<T, Allocator>::push_back(const T &value) {
    insert(cend(), value);
}


template<typename T, typename Allocator>
void LinkedList<T, Allocator>::push_back(T &&value) {
    insert(cend(), std::move(value));
}


/* erase methods */
template<typename T, typename Allocator>
typename LinkedList<T, Allocator>::iterator
LinkedList<T, Allocator>::erase(LinkedList::const_iterator pos) {

    Allocator alloc;
    typename Allocator::template rebind<detail::ListNode<value_type>>::other other_alloc;

    std::allocator_traits<Allocator>::destroy(alloc, pos.ptr_->object_);
    std::allocator_traits<Allocator>::deallocate(alloc, pos.ptr_->object_, 1ull);

    pos.ptr_->prev_->next_ = pos.ptr_->next_;
    pos.ptr_->next_->prev_ = pos.ptr_->prev_;
    auto ret_ptr = pos.ptr_->next_;
    std::allocator_traits<decltype(other_alloc)>::deallocate(other_alloc, pos.ptr_, 1ull);
    --size_;

    return iterator(ret_ptr);
}


template<typename T, typename Allocator>
typename LinkedList<T, Allocator>::iterator
LinkedList<T, Allocator>::erase(LinkedList::const_iterator first, LinkedList::const_iterator last) {

    for(auto it = first; it != last;){
        auto del_it = it++;
        erase(del_it);
    }

    return iterator(last.ptr_);
}


template<typename T, typename Allocator>
void LinkedList<T, Allocator>::pop_back() {

    erase(--cend());
}


template<typename T, typename Allocator>
void LinkedList<T, Allocator>::pop_front() {

    erase(cbegin());
}


/* Constructors and assignment operators */
template<typename T, typename Allocator>
LinkedList<T, Allocator>::LinkedList():size_(0ull){
    base_.next_ = &base_;
    base_.prev_ = &base_;
    base_.object_ = nullptr;
}

template<typename T, typename Allocator>
LinkedList<T, Allocator>::~LinkedList() {

    clear();
}

template<typename T, typename Allocator>
LinkedList<T, Allocator>::LinkedList(const LinkedList &other): LinkedList(){

    insert(cend(), other.cbegin(), other.cend());
}

template<typename T, typename Allocator>
LinkedList<T, Allocator>::LinkedList(LinkedList &&other) noexcept: LinkedList() {

    std::swap(*this, other);
}

template<typename T, typename Allocator>
LinkedList<T, Allocator>::LinkedList(LinkedList::size_type count, const T &value): LinkedList() {

    insert(cend(), count, value);
}

template<typename T, typename Allocator>
LinkedList<T, Allocator>::LinkedList(LinkedList::size_type count): LinkedList(count, value_type()) {}

template<typename T, typename Allocator>
template<typename InputIt>
LinkedList<T, Allocator>::LinkedList(InputIt first, InputIt last): LinkedList() {

    insert(cend(), first, last);
}

template<typename T, typename Allocator>
LinkedList<T, Allocator>::LinkedList(std::initializer_list<T> init): LinkedList() {

    insert(cend(), init);
}


template<typename T, typename Allocator>
LinkedList<T, Allocator>& LinkedList<T, Allocator>::operator=(const LinkedList &other) {

    if(this == &other){
        return *this;
    }

    erase(cbegin(), cend());
    insert(cend(), other.cbegin(), other.cend());

    return *this;
}


template<typename T, typename Allocator>
LinkedList<T, Allocator> &LinkedList<T, Allocator>::operator=(LinkedList &&other)  noexcept {

    erase(cbegin(), cend());
    std::swap(*this, other);

    return *this;
}


template<typename T, typename Allocator>
LinkedList<T, Allocator> &LinkedList<T, Allocator>::operator=(std::initializer_list<T> ilist) {

    erase(cbegin(), cend());
    insert(cend(), ilist.begin(), ilist.end());

    return *this;
}


/* assign methods */
template<typename T, typename Allocator>
void LinkedList<T, Allocator>::assign(LinkedList::size_type count, const T &value) {

    auto it = begin();
    for(size_type i=0; i < std::min(count, size_); ++i){
        *it = value;
        ++it;
    }

    if(count < size_){
        erase(it, end());
    }else{
        insert(cend(), count - size_, value);
    }
}


template<typename T, typename Allocator>
template<typename InputIt>
void LinkedList<T, Allocator>::assign(InputIt first, InputIt last) {

    clear();
    insert(cend(), first, last);
}


template<typename T, typename Allocator>
void LinkedList<T, Allocator>::assign(std::initializer_list<T> ilist) {

    auto it = begin();
    for(size_type i=0; i < std::min(ilist.size(), size_); ++i){
        *it = ilist[i];
        ++it;
    }

    if(ilist.size() < size_){
        erase(it, end());
    }else{
        insert(cend(), ilist.begin()+size_, ilist.end());
    }
}


/* front-back methods */
template<typename T, typename Allocator>
typename LinkedList<T, Allocator>::reference LinkedList<T, Allocator>::front() {

    return *begin();
}

template<typename T, typename Allocator>
typename LinkedList<T, Allocator>::const_reference LinkedList<T, Allocator>::front() const {

    return *cbegin();
}

template<typename T, typename Allocator>
typename LinkedList<T, Allocator>::reference LinkedList<T, Allocator>::back() {

    return *base_.prev_->object_;
}

template<typename T, typename Allocator>
typename LinkedList<T, Allocator>::const_reference LinkedList<T, Allocator>::back() const {

    return *base_.prev_->object_;
}

template<typename T, typename Allocator>
typename LinkedList<T, Allocator>::size_type LinkedList<T, Allocator>::size() const noexcept {

    return size_;
}

template<typename T, typename Allocator>
bool LinkedList<T, Allocator>::empty() const {

    return size_ == 0ull;
}

template<typename T, typename Allocator>
void LinkedList<T, Allocator>::clear() noexcept {

    erase(cbegin(), cend());
}


template<typename T, typename Allocator>
void LinkedList<T, Allocator>::resize(LinkedList::size_type count, const value_type &value) {

    assign(count, value);
}


template<typename T, typename Allocator>
void LinkedList<T, Allocator>::resize(LinkedList::size_type count) {

    if(count > size_){
        assign(count, value_type());
    }

    if(count < size_){
        auto it = cbegin();
        std::advance(it, count);
        erase(it, cend());
    }
}


template<typename T, typename Allocator>
void LinkedList<T, Allocator>::swap(LinkedList &other) noexcept {

    std::swap(*this, other);
}


template<typename T, typename Allocator>
void LinkedList<T, Allocator>::reverse() noexcept {

    for(auto it = begin(); it!=end(); --it){

        std::swap(it.ptr_->prev_, it.ptr_->next_);
    }

    std::swap(base_.prev_, base_.next_);
}


/* remove methods */
template<typename T, typename Allocator>
template<typename Func, typename... Args>
typename LinkedList<T, Allocator>::size_type
LinkedList<T, Allocator>::remove_some_elements_(Func f, Args&&... arg) {

    auto it = f(begin(), end(), std::forward<Args>(arg)...);
    size_type ret = std::distance(it, end());
    erase(it, end());
    return ret;
}


template<typename T, typename Allocator>
typename LinkedList<T, Allocator>::size_type
LinkedList<T, Allocator>::remove(const T &value) {

    return remove_some_elements_(std::remove<iterator, const_value_type>, value);
}


template<typename T, typename Allocator>
template<typename UnaryPredicate>
typename LinkedList<T, Allocator>::size_type
LinkedList<T, Allocator>::remove_if(UnaryPredicate p) {

    return remove_some_elements_(std::remove_if<iterator, UnaryPredicate>, p);
}


template<typename T, typename Allocator>
typename LinkedList<T, Allocator>::size_type
LinkedList<T, Allocator>::unique() {

    return remove_some_elements_(std::unique<iterator>);
}


template<typename T, typename Allocator>
template<typename BinaryPredicate>
typename LinkedList<T, Allocator>::size_type
LinkedList<T, Allocator>::unique(BinaryPredicate p) {

    return remove_some_elements_(std::unique<iterator, BinaryPredicate>, p);
}


/* iterators */

template<typename T, typename Allocator>
typename LinkedList<T, Allocator>::
        iterator LinkedList<T, Allocator>::begin() noexcept {

    return iterator(base_.next_);
}


template<typename T, typename Allocator>
typename LinkedList<T, Allocator>::
        const_iterator LinkedList<T, Allocator>::begin() const noexcept {

    return const_iterator(base_.next_);
}


template<typename T, typename Allocator>
typename LinkedList<T, Allocator>::
        const_iterator LinkedList<T, Allocator>::cbegin() const noexcept {

    return const_iterator(base_.next_);
}


template<typename T, typename Allocator>
typename LinkedList<T, Allocator>::
        iterator LinkedList<T, Allocator>::end() noexcept {

    return iterator(&base_);
}


template<typename T, typename Allocator>
typename LinkedList<T, Allocator>::
        const_iterator LinkedList<T, Allocator>::end() const noexcept {

    return const_iterator(&base_);
}


template<typename T, typename Allocator>
typename LinkedList<T, Allocator>::
        const_iterator LinkedList<T, Allocator>::cend() const noexcept {

    return const_iterator(&base_);
}


template<typename T, typename Allocator>
typename LinkedList<T, Allocator>::
        reverse_iterator LinkedList<T, Allocator>::rbegin() noexcept {

    return reverse_iterator(iterator(&base_));
}


template<typename T, typename Allocator>
typename LinkedList<T, Allocator>::
        const_reverse_iterator LinkedList<T, Allocator>::rbegin() const noexcept {

    return const_reverse_iterator(const_iterator(&base_));
}


template<typename T, typename Allocator>
typename LinkedList<T, Allocator>::
        const_reverse_iterator LinkedList<T, Allocator>::crbegin() const noexcept {

    return const_reverse_iterator(const_iterator(&base_));
}


template<typename T, typename Allocator>
typename LinkedList<T, Allocator>::
        reverse_iterator LinkedList<T, Allocator>::rend() noexcept {

    return reverse_iterator(iterator(base_.next_));
}


template<typename T, typename Allocator>
typename LinkedList<T, Allocator>::
        const_reverse_iterator LinkedList<T, Allocator>::rend() const noexcept {

    return const_reverse_iterator(const_iterator(base_.next_));
}


template<typename T, typename Allocator>
typename LinkedList<T, Allocator>::
        const_reverse_iterator LinkedList<T, Allocator>::crend() const noexcept {

    return const_reverse_iterator(const_iterator(base_.next_));
}


#endif //LINKEDLIST_LINKEDLIST_H
