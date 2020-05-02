#pragma once
#include <cassert>
#include <iostream>

template<class T>
class DynamicArray {
private:
	T* arr;
	size_t size;
	size_t capacity;

	void resize() {
		capacity *= 2;
		T* tmp = arr;

		arr = new T[capacity];

		for (size_t i = 0; i < size; i++) {
			arr[i] = tmp[i];
		}

		delete[] tmp;
	}

	void copy(const DynamicArray<T>& darr) {
		size = darr.size;
		capacity = darr.capacity;
		arr = new T[capacity];

		for (size_t i = 0; i < darr.size; i++) {
			arr[i] = darr.arr[i];
		}
	}

	void del()
	{
		delete[] arr;
	}
public:
	DynamicArray() {
		size = 0;
		capacity = 2;
		arr = new T[capacity];
	}

	DynamicArray(const DynamicArray<T>& darr) {
		copy(darr);
	}

	DynamicArray<T>& operator=(const DynamicArray<T>& darr) {
		if (this != &darr) {
			del();
			copy(darr);
		}

		return *this;
	}

	~DynamicArray() {
		del();
	}

	void push_back(const T& elem) {
		if (size == capacity)
			resize();

		arr[size++] = elem;
	}

	void push_front(const T& elem) {
		if (size == capacity) {
			resize();
		}

		for (size_t i = size; i > 0; i--) {
			arr[i] = arr[i - 1];
		}

		arr[0] = elem;
		size++;
	}

	void pop_back() {
		assert(size > 0);

		size--;
	}

	void pop_front() {
		assert(size > 0);

		for (size_t i = 0; i < size - 1; i++) {
			arr[i] = arr[i + 1];
		}

		size--;
	}

	size_t getSize()const {
		return size;
	}

	void print()const {
		for (size_t i = 0; i < size; i++) {
			std::cout << arr[i] << " ";
		}
		std::cout << std::endl;
	}

	void insertElem(size_t position, const T& elem) {
		if (position >= size) {
			std::cout << "Not valid positions" << std::endl;
			return;
		}
		if (size == capacity)
			resize();

		for (size_t i = size; i > position; i--) {
			arr[i] = arr[i - 1];
		}

		arr[position] = elem;

		size++;
	}

	T& operator[](size_t index) {
		assert(index < size);

		return arr[index];
	}

	const T& operator[](size_t index)const {
		assert(index < size);

		return arr[index];
	}

	bool removeElem(size_t index) {
		if (index >= size) {
			std::cout << "Invalid Index" << std::endl;
			return false;
		}

		for (size_t i = index; i < size - 1; i++) {
			arr[i] = arr[i + 1];
		}

		size--;
		return true;
	}

	bool empty()const {
		return size == 0;
	}

	bool find(const T& elem) {
		for (size_t i = 0; i < size; i++) {
			if (arr[i] == elem)
				return true;
		}

		return false;
	}

	void clear() {
		del();

		size = 0;
		capacity = 2;
		arr = new int[capacity];
	}
};