#ifndef ITERATOR_H
#define ITERATOR_H
// iterator.h
template <typename T>
class Iterator {
    public:
    virtual T& operator*() = 0;
    virtual Iterator& operator++() = 0;
    virtual bool operator!=(const Iterator& other) const = 0;
};

#endif // ITERATOR_H
