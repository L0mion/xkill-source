#pragma once

template <class T>
class SimpleQueue
{
private:
	T* buffer;
	int front;
	int back;
	int numElements;
	int capacity;

	void grow();
public:
	SimpleQueue(int capacity = 1);
	~SimpleQueue();

	T at(int index);
	int count();
	void push(T value);
	T pop();
	bool isEmpty();
};

template <class T>
T SimpleQueue<T>::at( int index )
{
	// Offset index relative to memory layout
	index += front;
	if(index > capacity-1)
		index -= capacity;

	// Return index
	return buffer[index];
}

template <class T>
bool SimpleQueue<T>::isEmpty()
{
	return numElements <= 0;
}

template <class T>
T SimpleQueue<T>::pop()
{
	T value = buffer[front];
	front++;

	// Wrap index to beginning if needed
	if(front >= capacity)
		front = 0;

	numElements--;

	return value;
}

template <class T>
void SimpleQueue<T>::push( T value )
{
	// Expand capacity if needed
	if(numElements + 1 > capacity)
		grow();

	back++;

	// Wrap index to beginning if needed
	if(back >= capacity)
		back = 0;

	buffer[back] = value;
	numElements++;
}

template <class T>
int SimpleQueue<T>::count()
{
	return numElements;
}

template <class T>
SimpleQueue<T>::~SimpleQueue()
{
	delete [] buffer;
}

template <class T>
SimpleQueue<T>::SimpleQueue( int capacity /*= 1*/ )
{
	this->capacity = capacity;
	buffer = new T[capacity];
	front = 0;
	back = -1;
	numElements = 0;
}

template <class T>
void SimpleQueue<T>::grow()
{
	int tmp_capacity = capacity*2;
	T* tmp_buffer = new T[tmp_capacity];
	int tmp_numElements = numElements;
	for(int i=0; i<tmp_numElements; i++)
	{
		tmp_buffer[i] = pop();
	}
	capacity = tmp_capacity;
	numElements = tmp_numElements;
	front = 0;
	back = numElements - 1;

	delete [] buffer;
	buffer = tmp_buffer;
}