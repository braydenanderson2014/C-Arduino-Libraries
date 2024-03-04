#ifndef INITIALIZER_LIST_H
#define INITIALIZER_LIST_H

template<class E>
class initializer_list
{
public:
    typedef E value_type;
    typedef const E& reference;
    typedef const E& const_reference;
    typedef size_t size_type;
    typedef const E* iterator;
    typedef const E* const_iterator;

private:
    iterator _begin;
    size_type _size;

public:
    constexpr initializer_list() noexcept
        : _begin(nullptr), _size(0) {}

    constexpr size_t size() const noexcept { return _size; }
    constexpr const E* begin() const noexcept { return _begin; }
    constexpr const E* end() const noexcept { return _begin + _size; }
};
#endif // INITIALIZER_LIST_H
