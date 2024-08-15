#ifndef VECTOR_H
# define VECTOR_H
# include <iostream>
# include <functional>
# include <initializer_list>

template<typename T>
class Vector{
public:
	class ConstIterator;
	class Iterator;
	using value_type = Key;
	using size_type = std::size_t;
	using difference_type = std::ptrdiff_t;
	using reference = value_type&;
	using const_reference = const value_type&;
	using pointer = value_type*;
	using const_pointer = const value_type*;
	using iterator = Vector::Iterator;
	using const_iterator = Vector::ConstIterator;

private:
	size_type sz;
	size_type max_sz;
	value_type* values;

	void allocate(size_type n){
		if(n > sz)
		{
			value_type *temp = new value_type[n];
			for(size_type i = 0; i < sz; ++i){
				temp[i] = values[i];
			}
			delete[] values;
			values = temp;
			max_sz = n;	
		}
	}
public:
	Vector() : sz{0}, max_sz{0}, values{nullptr}{}
	Vector(const Vector& other) : sz{other.sz}, max_sz{other.max_sz}, values{new value_type[max_sz]}{
		for(size_type i{0}; i < sz; ++i){
			values[i] = other.values[i];
		}
	}
	Vector(std::initializer_list<value_type> lst) : sz{lst.size()}, max_sz{lst.size()}, values{new value_type[max_sz]}{
		size_type i = 0;
		for(const auto& x : lst){
			values[i] = x;
			i++;
		}
	}
	Vector(size_type n) : sz{n}, max_sz{n}, values{nullptr}{};
	~Vector(){
		delete[] values;
	}

	Vector& operator=(Vector other){
		std::swap(sz, other.sz);
		std::swap(max_sz, other.max_sz);
		std::swap(values, other.values);
		return *this;
	}
	size_t size() const{
		return sz;
	}
	bool empty() const{
		return sz == 0;
	}
	void clear(){
		sz = 0;
	}
	void reserve(size_type n){
		if(n > max_sz){
			allocate(n);
			max_sz = n;
		}
	}
	void shrink_to_fit(){
		if(sz < max_sz){
			allocate(sz);
			max_sz = sz;
		}
	}
	void push_back(const value_type& x){
		if(sz == max_sz)
		{
			allocate(max_sz * 2 + 1);
		}
		values[sz++] = x;
	}
	void pop_back(){
		if(sz <= 0){
			throw std::runtime_error("stop pls");
		}
		--sz;
	}
	T& operator[](size_t index){
		if(index >= sz){
			throw std::runtime_error("Out of range!");
		}
		return values[index];
	}
	const T& operator[](size_t index) const{
		if(index >= sz){
			throw std::runtime_error("Out of range!");
		}
		return values[index];
	}
	size_t capacity() const{
		return max_sz;
	}
	friend std::ostream& operator<<(std::ostream& os, const Vector& vector){
		os << "[";
		for(size_type i = 0; i < vector.size(); ++i){
			os << vector[i];
			if(i < vector.size() - 1){
				os << ", ";
			}
		}
		os << "]";
		return os;
	}
	
	class Iterator {
	public:
		using value_type = Vector::value_type;
		using reference = Vector::reference;
		using pointer = Vector::pointer;
		using difference_type = Vector::difference_type;
		using iterator_category = std::forward_iterator_tag;
	private:
		pointer ptr;
	public:
		Iterator() : ptr{nullptr}{}
		Iterator(pointer pnt) : ptr{pnt}{}

		reference operator*() const{
			return *ptr;
		}
		pointer operator->() const{
			return ptr;
		}
		bool operator==(const const_iterator& other) const {
			return static_cast<const_iterator>(ptr) == other;
		}
		bool operator!=(const const_iterator& other) const {
			return !(*this == other);
		}
		iterator& operator++(){
			++ptr;
			return *this;
		}
		iterator operator++(int){
			iterator temp = *this;
			++ptr;
			return temp;
		}
		operator const_iterator() const{
			return ConstIterator(ptr);
		}
	};
	class ConstIterator {
	public:
		using value_type = Vector::value_type;
		using reference = Vector::const_reference;
		using pointer = Vector::const_pointer;
		using difference_type = Vector::difference_type;
		using iterator_category = std::forward_iterator_tag;
	private:
		pointer c_ptr;
	public:
		ConstIterator() : c_ptr{nullptr}{}
		ConstIterator(pointer pnt) : c_ptr{pnt}{}

		reference operator*() const{
			return *c_ptr;
		}
		pointer operator->() const{
			return c_ptr;
		}
		bool operator==(const const_iterator& other) const {
			return c_ptr == other.c_ptr;
		}
		bool operator!=(const const_iterator& other) const{
			return !(*this == other);
		}
		const_iterator& operator++(){
			++c_ptr;
			return *this;
		}
		const_iterator operator++(int){
			ConstIterator temp = *this;
			++c_ptr;
			return temp;
		}

		friend Vector::difference_type operator-(const Vector::ConstIterator& lop, const Vector::ConstIterator& rop){
			return lop.c_ptr - rop.c_ptr;
		}
	};
	iterator begin(){
		return values;
	}
	iterator end(){
		return values+sz;
	}
	const_iterator begin() const{
		return values;
	}
	const_iterator end() const{
		return values+sz;
	}

	iterator insert(const_iterator pos, const_reference val) {
		auto diff = pos - begin();
		if(diff < 0 || static_cast<size_type>(diff) > sz)
			throw std::runtime_error("Iterator out of bounds");
		size_type current{static_cast<size_type>(diff)};
		if (sz >= max_sz)
			reserve(max_sz * 2 + 1);
		for (auto i{sz}; i-- > current;)
			values[i + 1] = values[i];
		values[current] = val;
		++sz;
		return iterator{values + current};
	}

	iterator erase(const_iterator pos) {
		auto diff = pos - begin();
		if (diff < 0 || static_cast<size_type>(diff) >= sz)
			throw std::runtime_error("Iterator out of bounds");
		size_type current{static_cast<size_type>(diff)};
		for (auto i{current}; i < sz - 1; ++i)
			values[i] = values[i+1];
		--sz;
		return iterator{values + current};
	}
};
#endif
