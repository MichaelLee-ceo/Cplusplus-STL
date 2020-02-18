#ifndef LIST_H
#define LIST_H

#include <forward_list> // include definition of class template forward_list

// FlistNode class template definition
template< typename T >
struct FlistNode
{
	FlistNode *next;
	T myVal;
}; // end class template FlistNode


// ListIterator class template definition
template< typename T >
class ListIterator
{
	template< typename T > friend class forward_list;
public:
	ListIterator(FlistNode< T > *p = nullptr) // default constructor
		: ptr(p)
	{
	}

	ListIterator(const ListIterator &iteratorToCopy) // copy constructor
		: ptr(iteratorToCopy.ptr)
	{
	}

	~ListIterator() // destructor
	{
	}

	// const return avoids: ( a1 = a2 ) = a3
	const ListIterator& operator=(const ListIterator &right)
	{
		if (&right != this) // avoid self-assignment
			ptr = right.ptr;
		return *this; // enables x = y = z, for example
	}

	bool operator==(const ListIterator &right) const // equal to
	{
		return ptr == right.ptr;
	}

	bool operator!=(const ListIterator &right) const // not equal to
	{
		return ptr != right.ptr;
	}

	T& operator*() const // dereferencing operator
	{
		return ptr->myVal;
	}

	ListIterator& operator++() // prefix increment operator
	{
		ptr = ptr->next;
		return *this;
	}

private:
	FlistNode< T > *ptr; // keep a pointer to forward_list
}; // end class template ListIterator


// forward_list class template definition
template< typename T >
class forward_list
{
	template< typename T >
	friend bool operator==(std::forward_list< T > &stdList, forward_list< T > &myList);

	template< typename T >
	friend bool operator==(const forward_list< T > &lhs, const forward_list< T > &rhs);

	template< typename T >
	friend bool operator!=(const forward_list< T > &lhs, const forward_list< T > &rhs);

public:
	using iterator = ListIterator< T >;

	// Constructs a forward_list container with n elements.
	forward_list(unsigned int n = 0);

	// Constructs a forward_list container with a copy of each of the elements in x,
	// in the same order.
	forward_list(const forward_list &x);

	// Destroys all forward_list elements,
	// and deallocates all the storage allocated by the forward_list container.
	~forward_list();

	// Assigns new contents to the container, replacing its current contents,
	const forward_list& operator=(const forward_list &x);

	// Returns an iterator pointing to the first element in the forward_list container.
	iterator begin() const;

	// Returns an iterator referring to the past-the-end element in the forward_list container.
	iterator end() const;

	// Returns a bool value indicating whether the forward_list container is empty
	bool empty() const;

	T& front(); // Returns a reference to the first element in the forward_list container

	// Inserts a new element at the beginning of the forward_list,
	// right before its current first element.
	// The content of val is copied (or moved) to the inserted element.
	void push_front(const T val);

	// Removes the first element in the forward_list container
	void pop_front();

	// Resizes the forward_list container to contain n elements.
	// If n is smaller than the current number of elements in the forward_list container,
	// the content is trimmed to contain only its first n elements, removing those beyond.
	// If n is greater than the current number of elements in the forward_list container,
	// the content is expanded by inserting at the end
	// as many elements as needed to reach a size of n elements.
	void resize(unsigned int n);

	// Removes all elements from the forward_list container (which are destroyed).
	void clear();

	// Reverses the order of the elements in the forward_list container.
	void reverse();

private:
	// pointing to the past-the-end element in the forward_list container
	FlistNode< T > *myHead;
}; // end class template forward_list


// Constructs a container with n elements.
template< typename T >
forward_list< T >::forward_list(unsigned int n)
	: myHead(nullptr)
{
	if (n > 0)
	{
		FlistNode< T > *newNode;
		for (unsigned int i = 0; i < n; i++)
		{
			newNode = new FlistNode< T >;
			newNode->myVal = T();
			newNode->next = myHead;
			myHead = newNode;
		}
	}
}

// Constructs a forward_list container with a copy of each of the elements in x,
// in the same order.
template< typename T >
forward_list< T >::forward_list(const forward_list< T > &x)
	: myHead(nullptr)
{
	iterator right = x.begin();
	for (; right != x.end(); ++right)
		push_front(*right);

	reverse();
}

// Destroys all forward_list container elements,
// and deallocates all the storage allocated by the forward_list container.
template< typename T >
forward_list< T >::~forward_list()
{
	clear();
	delete myHead;
}

// Assigns new contents to the container, replacing its current contents,
template< typename T >
const forward_list< T >& forward_list< T >::operator=(const forward_list< T > &x)
{
	if (&x != this) // avoid self-assignment
	{
		clear();

		iterator right = x.begin();
		for (; right != x.end(); ++right)
			push_front(*right);

		reverse();
	}

	return *this; // enables x = y = z, for example
} // end function operator=

// Returns an iterator pointing to the first element in the forward_list container.
template< typename T >
typename forward_list< T >::iterator forward_list< T >::begin() const
{
	return iterator(myHead);
}

// Returns an iterator referring to the past-the-end element in the forward_list container.
template< typename T >
typename forward_list< T >::iterator forward_list< T >::end() const
{
	return iterator(nullptr);
}

// Returns a bool value indicating whether the forward_list container is empty
template< typename T >
bool forward_list< T >::empty() const
{
	return (myHead == nullptr);
}

// Returns a reference to the first element in the forward_list container
template< typename T >
T& forward_list< T >::front()
{
	return myHead->myVal;
}

// Inserts a new element at the beginning of the forward_list,
// right before its current first element.
// The content of val is copied (or moved) to the inserted element.
template< typename T >
void forward_list< T >::push_front(const T val)
{
	FlistNode< T > *newNode = new FlistNode< T >;
	newNode->myVal = val;

	newNode->next = myHead;
	myHead = newNode;
}

// Removes the first element in the forward_list container
template< typename T >
void forward_list< T >::pop_front()
{
	FlistNode< T > *temp = myHead;
	myHead = myHead->next;
	delete temp;
}

// Resizes the forward_list container to contain n elements.
template< typename T >
void forward_list< T >::resize(unsigned int n)
{
	reverse();

	int size = 0;
	iterator p1 = begin();
	for (; p1 != end(); ++p1)
		size++;

	if (size > n)
		for (int i = 0; i < size - n; i++)
			pop_front();
	else if (n > size)
		for (int i = 0; i < n - size; i++)
			push_front(T());

	reverse();
}

// Removes all elements from the forward_list container (which are destroyed)
template< typename T >
void forward_list< T >::clear()
{
	FlistNode< T > *temp;
	while (myHead != nullptr) // the forward_list is not empty
	{
		temp = myHead;
		myHead = myHead->next;
		delete temp;

	}
}

// Reverses the order of the elements in the forward_list container.
template< typename T >
void forward_list< T >::reverse()
{
	if (myHead != nullptr)
	{
		FlistNode< T > *current = myHead;
		FlistNode< T > *b = myHead->next;
		FlistNode< T > *a = b;

		current->next = nullptr;

		while (b != nullptr)
		{
			b = b->next;
			a->next = current;
			current = a;
			a = b;
		}

		myHead = current;
	}
}

// determine if two lists are equal and return true, otherwise return false
template< typename T >
bool operator==(const forward_list< T > &lhs, const forward_list< T > &rhs)
{
	FlistNode< T > *p1 = lhs.myHead;
	FlistNode< T > *p2 = rhs.myHead;
	for (; p1 != nullptr && p2 != nullptr; p1 = p1->next, p2 = p2->next)
		if (p1->myVal != p2->myVal)
			return false;

	if (p1 == nullptr && p2 == nullptr)
		return true;

	return false;
}

// inequality operator; returns opposite of == operator
template< typename T >
bool operator!=(const forward_list< T > &lhs, const forward_list< T > &rhs)
{
	return !(lhs == rhs);
}

// determine if two lists are equal
template< typename T >
bool operator==(std::forward_list< T > &stdList, forward_list< T > &myList)
{
	FlistNode< T > *ptr = myList.myHead;
	typename std::forward_list< T >::iterator it = stdList.begin();
	for (; ptr != nullptr && it != stdList.end(); ptr = ptr->next, ++it)
		if (ptr->myVal != *it)
			return false;

	if (ptr == nullptr && it == stdList.end())
		return true;

	return false;
}

#endif