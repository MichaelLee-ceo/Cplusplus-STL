#ifndef DEQUE_H
#define DEQUE_H

template< class BidIt >
class ReverseIterator // wrap iterator to run it backwards
{
public:
	using value_type      = typename BidIt::value_type;
	using difference_type = typename BidIt::difference_type;
	using pointer         = typename BidIt::pointer;
	using reference       = typename BidIt::reference;

	ReverseIterator() // construct with value-initialized wrapped iterator
		: current()
	{
	}

	// construct wrapped iterator from right
	explicit ReverseIterator(BidIt right)
		: current(right)
	{
	}

	template< class other >
	ReverseIterator(const ReverseIterator< other >& right)
		: current(right.current) // initialize with compatible base
	{
	}

	// assign from compatible base
	template< class other >
	ReverseIterator& operator=(const ReverseIterator< other >& right)
	{
		current = right.current;
		return *this;
	}

	BidIt base() const // return wrapped iterator
	{
		return current;
	}

	reference operator*() const // return designated value
	{
		BidIt tmp = current;
		return *--tmp;
	}

	pointer operator->() const // return pointer to class object
	{
		BidIt tmp = current;
		--tmp;
		return tmp.operator->();
	}

	ReverseIterator& operator++() // preincrement
	{
		--current;
		return *this;
	}

	ReverseIterator operator++(int) // postincrement
	{
		ReverseIterator tmp = *this;
		--current;
		return tmp;
	}

	ReverseIterator& operator--()	// predecrement
	{
		++current;
		return *this;
	}

	ReverseIterator operator--(int) // postdecrement
	{
		ReverseIterator tmp = *this;
		++current;
		return tmp;
	}

	// N.B. functions valid for random-access iterators only beyond this point

	// increment by integer
	ReverseIterator& operator+=(const difference_type off)
	{
		current -= off;
		return *this;
	}

	// return this + integer
	ReverseIterator operator+(const difference_type off) const
	{
		return ReverseIterator(current - off);
	}

	// decrement by integer
	ReverseIterator& operator-=(const difference_type off)
	{
		current += off;
		return *this;
	}

	// return this - integer
	ReverseIterator operator-(const difference_type off) const
	{
		return ReverseIterator(current + off);
	}

	// subscript
	reference operator[](const difference_type off) const
	{
		return current[static_cast<difference_type>(-off - 1)];
	}

protected:
	BidIt current; // the wrapped iterator
};

template< class BidIt1, class BidIt2 >
bool operator==(const ReverseIterator< BidIt1 >& left,
	const ReverseIterator< BidIt2 >& right)
{
	return left.base() == right.base();
}

template< class BidIt1, class BidIt2 >
bool operator!=(const ReverseIterator< BidIt1 >& left,
	const ReverseIterator< BidIt2 >& right)
{
	return !(left == right);
}

template< class BidIt1, class BidIt2 >
bool operator<(const ReverseIterator< BidIt1 >& left,
	const ReverseIterator< BidIt2 >& right)
{
	return right.base() < left.base();
}

template< class BidIt1, class BidIt2 >
bool operator>(const ReverseIterator< BidIt1 >& left,
	const ReverseIterator< BidIt2 >& right)
{
	return right < left;
}

template< class BidIt1, class BidIt2 >
bool operator<=(const ReverseIterator< BidIt1 >& left,
	const ReverseIterator< BidIt2 >& right)
{
	return !(right < left);
}

template< class BidIt1, class BidIt2 >
bool operator>=(const ReverseIterator< BidIt1 >& left,
	const ReverseIterator< BidIt2 >& right)
{
	return !(left < right);
}


// CLASS TEMPLATE DequeConstIterator
template< class MyDeque >
class DequeConstIterator // iterator for nonmutable deque
{
private:
	using size_type = typename MyDeque::size_type;

public:
	using value_type      = typename MyDeque::value_type;
	using difference_type = typename MyDeque::difference_type;
	using pointer         = typename MyDeque::const_pointer;
	using reference       = const value_type &;

	// construct with null pointer
	DequeConstIterator()
		: myOff(0),
		myCont(nullptr)
	{
	}

	// construct with offset off in *pDeque
	DequeConstIterator(size_type off, const MyDeque* pDeque)
		: myOff(off),
		myCont(pDeque)
	{
	}

	DequeConstIterator(const DequeConstIterator& right) // copy constructor
		: myOff(right.myOff),
		myCont(right.myCont)
	{
	}

	~DequeConstIterator() // destructor
	{
	}

	// const return avoids: ( a1 = a2 ) = a3
	const DequeConstIterator& operator=(const DequeConstIterator& right)
	{
		if (right != *this) // avoid self-assignment
		{
			myCont = right.myCont;
			myOff = right.myOff;
		}
		return *this; // enables x = y = z, for example
	}

	reference operator*() const
	{
		//      size_type block = myOff % ( 4 * myCont->mapSize ) / 4;
		size_type block = myCont->getBlock(myOff);
		size_type off = myOff % 4;
		return myCont->map[block][off];
	}

	pointer operator->() const
	{
		return &(this->operator*());
	}

	DequeConstIterator& operator++() // preincrement
	{
		++myOff;
		return *this;
	}

	DequeConstIterator operator++(int) // postincrement
	{
		DequeConstIterator tmp = *this;
		++* this;
		return tmp;
	}

	DequeConstIterator& operator--() // predecrement
	{
		--myOff;
		return *this;
	}

	DequeConstIterator operator--(int) // postdecrement
	{
		DequeConstIterator tmp = *this;
		--* this;
		return tmp;
	}

	// increment by integer
	DequeConstIterator& operator+=(const difference_type off)
	{
		myOff += off;
		return *this;
	}

	// return this + integer
	DequeConstIterator operator+(const difference_type off) const
	{
		DequeConstIterator tmp = *this;
		return tmp += off;
	}

	// decrement by integer
	DequeConstIterator& operator-=(const difference_type off)
	{
		return *this += -off;
	}

	// return this - integer
	DequeConstIterator operator-(const difference_type off) const
	{
		DequeConstIterator tmp = *this;
		return tmp -= off;
	}

	// return difference of iterators
	difference_type operator-(const DequeConstIterator& right) const
	{
		return (static_cast<difference_type>(this->myOff - right.myOff));
	}

	reference operator[](const difference_type off) const // subscript
	{
		return *(*this + off);
	}

	// test for iterator equality
	bool operator==(const DequeConstIterator& right) const
	{
		return this->myOff == right.myOff;
	}

	// test for iterator inequality
	bool operator!=(const DequeConstIterator& right) const
	{
		return !(*this == right);
	}

	// test if this < right
	bool operator<(const DequeConstIterator& right) const
	{
		return this->myOff < right.myOff;
	}

	// test if this > right
	bool operator>(const DequeConstIterator& right) const
	{
		return right < *this;
	}

	// test if this <= right
	bool operator<=(const DequeConstIterator& right) const
	{
		return !(right < *this);
	}

	// test if this >= right
	bool operator>=(const DequeConstIterator& right) const
	{
		return !(*this < right);
	}

	const MyDeque* myCont; // keep a pointer to deque
	size_type myOff;       // offset of element in deque
};


// CLASS TEMPLATE DequeIterator
template< class MyDeque >
class DequeIterator : public DequeConstIterator< MyDeque >
{ // iterator for mutable deque
private:
	using size_type = typename MyDeque::size_type;
	using MyBase = DequeConstIterator< MyDeque >;

public:
	using value_type      = typename MyDeque::value_type;
	using difference_type = typename MyDeque::difference_type;
	using pointer         = typename MyDeque::pointer;
	using reference       = value_type &;

	DequeIterator() // construct with null deque pointer
	{
	}

	DequeIterator(size_type off, const MyDeque* pDeque)
		: MyBase(off, pDeque) // construct with offset off in *pDeque
	{
	}

	reference operator*() const // return designated object
	{
		return const_cast<reference>(MyBase::operator*());
	}

	pointer operator->() const // return pointer to class object
	{
		return &(this->operator*());
	}

	DequeIterator& operator++() // preincrement
	{
		MyBase::operator++();
		return *this;
	}

	DequeIterator operator++(int) // postincrement
	{
		DequeIterator tmp = *this;
		MyBase::operator++();
		return tmp;
	}

	DequeIterator& operator--() // predecrement
	{
		MyBase::operator--();
		return *this;
	}

	DequeIterator operator--(int) // postdecrement
	{
		DequeIterator tmp = *this;
		MyBase::operator--();
		return tmp;
	}

	// increment by integer
	DequeIterator& operator+=(const difference_type off)
	{
		MyBase::operator+=(off);
		return *this;
	}

	// return this + integer
	DequeIterator operator+(const difference_type off) const
	{
		DequeIterator tmp = *this;
		return tmp += off;
	}

	// decrement by integer
	DequeIterator& operator-=(const difference_type off)
	{
		MyBase::operator-=(off);
		return *this;
	}

	using MyBase::operator-; // return difference of iterators

	// return this - integer
	DequeIterator operator-(const difference_type off) const
	{
		DequeIterator tmp = *this;
		return tmp -= off;
	}

	reference operator[](const difference_type off) const // subscript
	{
		return const_cast<reference>(MyBase::operator[](off));
	}
};


template < class Ty >
struct DequeSimpleTypes // wraps types needed by iterators
{
	using value_type      = Ty;
	using size_type       = size_t;
	using difference_type = ptrdiff_t;
	using pointer         = value_type *;
	using const_pointer   = const value_type*;
	using MapPtr          = Ty * *;
};

// CLASS TEMPLATE DequeVal
template < class ValTypes >
class DequeVal // base class for deque to hold data
{
public:
	using value_type      = typename ValTypes::value_type;
	using size_type       = typename ValTypes::size_type;
	using difference_type = typename ValTypes::difference_type;
	using pointer         = typename ValTypes::pointer;
	using const_pointer   = typename ValTypes::const_pointer;
	using reference       = value_type &;
	using const_reference = const value_type &;
	using MapPtr          = typename ValTypes::MapPtr;

	DequeVal() // initialize values
		: map(),
		mapSize(0),
		myOff(0),
		mySize(0)
	{
	}

	// determine block from offset
	size_type getBlock(size_type off) const
	{
		return off % (4 * mapSize) / 4;
		//      return ( off / 4 ) & ( mapSize - 1 );
	}

	MapPtr map;        // pointer to array of pointers to blocks
	size_type mapSize; // size of map array, zero or 2^N
	size_type myOff;   // offset of initial element
	size_type mySize;  // current length of sequence
};


// CLASS TEMPLATE deque
template< class Ty >
class deque // circular queue of pointers to blocks
{
private:
	using MapPtr = Ty * *;
	using ScaryVal = DequeVal< DequeSimpleTypes< Ty > >;

public:
	using value_type      = Ty;
	using size_type       = size_t;
	using difference_type = ptrdiff_t;
	using pointer         = value_type *;
	using const_pointer   = const value_type*;
	using reference       = value_type &;
	using const_reference = const value_type &;

	using iterator               = DequeIterator< ScaryVal >;
	using const_iterator         = DequeConstIterator< ScaryVal >;

	using reverse_iterator       = ReverseIterator< iterator >;
	using const_reverse_iterator = ReverseIterator< const_iterator >;

	// construct empty deque
	deque()
		: myData()
	{
	}

	// construct from count * val
	deque(size_type count, const Ty& val)
		: myData()
	{
		if (count == 0)
		{
			myData.map = nullptr;
			myData.mapSize = 0;
		}
		else
		{
			myData.mapSize = 8;
			while (count > 4 * (myData.mapSize - 1))
				myData.mapSize *= 2;

			myData.map = new Ty * [myData.mapSize]();
			for (size_type i = 0; i <= (count - 1) / 4; i++)
				myData.map[i] = new Ty[4];

			for (size_type i = 0; i < count; i++)
				myData.map[i / 4][i % 4] = val;
		}
		myData.mySize = count;
	}

	// construct by copying right
	deque(const deque& right)
		: myData()
	{
		myData.mapSize = right.myData.mapSize;
		myData.map = new Ty * [myData.mapSize]();
		myData.myOff = right.myData.myOff;
		myData.mySize = right.myData.mySize;

		for (int i = 0; i < myData.mapSize; i++)
			if (right.myData.map[i] != nullptr)
				myData.map[i] = new Ty[4]();

		iterator p1 = this->begin();
		const_iterator p2 = right.begin();

		for (; p2 != right.end(); ++p1, ++p2)
			* p1 = *p2;
	}

	// destroy the deque
	~deque()
	{
		clear();
	}

	// Assigns new contents to the container, replacing its current contents,
	// and modifying its size accordingly.
	deque& operator=(const deque& right)
	{
		if (&right != this) // avoid self-assignment
		{
			clear();

			myData.mapSize = right.myData.mapSize;
			myData.map = new Ty * [myData.mapSize]();
			myData.myOff = right.myData.myOff;
			myData.mySize = right.myData.mySize;

			for (int i = 0; i < myData.mapSize; i++)
				if (right.myData.map[i] != nullptr)
					myData.map[i] = new Ty[4]();

			iterator p1 = this->begin();
			const_iterator p2 = right.begin();

			for (; p2 != right.end(); ++p1, ++p2)
				* p1 = *p2;
		} // end outer if

		return *this; // enables right = y = z, for example
	} // end function operator=

	// return iterator for beginning of mutable sequence
	iterator begin()
	{
		return iterator(myData.myOff, &myData);
	}

	// return iterator for beginning of nonmutable sequence
	const_iterator begin() const
	{
		return const_iterator(myData.myOff, &myData);
	}

	// return iterator for end of mutable sequence
	iterator end()
	{
		return iterator(myData.myOff + myData.mySize, &myData);
	}

	// return iterator for end of nonmutable sequence
	const_iterator end() const
	{
		return const_iterator(myData.myOff + myData.mySize, &myData);
	}

	// return iterator for beginning of reversed mutable sequence
	reverse_iterator rbegin()
	{
		return reverse_iterator(end());
	}

	// return iterator for beginning of reversed nonmutable sequence
	const_reverse_iterator rbegin() const
	{
		return const_reverse_iterator(end());
	}

	// return iterator for end of reversed mutable sequence
	reverse_iterator rend()
	{
		return reverse_iterator(begin());
	}

	// return iterator for end of reversed nonmutable sequence
	const_reverse_iterator rend() const
	{
		return const_reverse_iterator(begin());
	}

	// return iterator for beginning of nonmutable sequence
	const_iterator cbegin() const
	{
		return begin();
	}

	// return iterator for end of nonmutable sequence
	const_iterator cend() const
	{
		return end();
	}

	// return iterator for beginning of reversed nonmutable sequence
	const_reverse_iterator crbegin() const
	{
		return rbegin();
	}

	// return iterator for end of reversed nonmutable sequence
	const_reverse_iterator crend() const
	{
		return rend();
	}

	// return length of sequence
	size_type size() const
	{
		return myData.mySize;
	}

	// test if sequence is empty
	bool empty() const
	{
		return myData.mySize == 0;
	}

	const_reference operator[](size_type pos) const
	{
		return *(begin() + static_cast<difference_type>(pos));
	}

	reference operator[](size_type pos)
	{
		return *(begin() + static_cast<difference_type>(pos));
	}

	reference front()
	{
		return *begin();
	}

	const_reference front() const
	{
		return *begin();
	}

	reference back()
	{
		return *(end() - 1);
	}

	const_reference back() const
	{
		return *(end() - 1);
	}

	// insert element at beginning
	void push_front(const Ty& val)
	{
		if (myData.mySize == 0)
		{
			if (myData.mapSize == 0)
			{
				myData.mapSize = 8;
				myData.map = new Ty * [myData.mapSize]();
			}
			myData.myOff = 4 * myData.mapSize - 1;
		}
		else
		{
			if (myData.myOff == 4 * myData.mapSize)
				myData.myOff = 0;

			int newFront = (myData.myOff - 1) % (4 * myData.mapSize);
			if (newFront % 4 == 3 && myData.mySize >= 4 * (myData.mapSize - 1))
			{
				doubleMapSize();
				newFront = (myData.myOff - 1) % (4 * myData.mapSize);
			}
			myData.myOff = newFront;
		}

		int block = getBlock(myData.myOff);
		if (myData.map[block] == nullptr)
			myData.map[block] = new Ty[4]();
		myData.map[block][myData.myOff % 4] = val;
		myData.mySize++;
	}

	// erase element at beginning
	void pop_front()
	{
		if (--myData.mySize == 0)
			myData.myOff = 0;
		else
			++myData.myOff;
	}

	// insert element at end
	void push_back(const Ty& val)
	{
		if (myData.mapSize == 0)
		{
			myData.mapSize = 8;
			myData.map = new Ty * [myData.mapSize]();
		}

		int newBack = myData.myOff + myData.mySize;
		if (newBack % 4 == 0 && myData.mySize >= 4 * (myData.mapSize - 1))
			doubleMapSize();

		int block = getBlock(newBack);
		if (myData.map[block] == nullptr)
			myData.map[block] = new Ty[4]();
		myData.map[block][newBack % 4] = val;
		myData.mySize++;
	}

	// erase element at end
	void pop_back()
	{
		if (--myData.mySize == 0)
			myData.myOff = 0;
	}

	// erase all
	void clear()
	{
		if (myData.mapSize > 0)
		{
			for (size_type i = 0; i < myData.mapSize; i++)
				if (myData.map[i] != nullptr)
					delete[] myData.map[i];
			delete[] myData.map;

			myData.mapSize = 0;
			myData.mySize = 0;
			myData.myOff = 0;
			myData.map = MapPtr();
		}
	}

private:

	// determine block from offset
	size_type getBlock(size_type off) const
	{
		return myData.getBlock(off);
		//      return ( ( myData.off / 4 ) & ( myData.mapSize - 1 ) );
	}

	void doubleMapSize()
	{
		int newSize = myData.mapSize * 2;
		Ty** buffer = new Ty * [newSize]();

		for (int i = myData.myOff / 4, t = 0; t < myData.mapSize; i++, t++)
			if (myData.map[i % myData.mapSize] != nullptr)
				buffer[i] = myData.map[i % myData.mapSize];

		delete[] myData.map;
		myData.map = buffer;
		myData.mapSize = newSize;
	}

	ScaryVal myData;
};

// test for deque equality
template< class Ty >
bool operator==(const deque< Ty >& left, const deque< Ty >& right)
{
	return left.size() == right.size()
		&& std::equal(left.begin(), left.end(), right.begin());
}

// test for deque inequality
template< class Ty >
bool operator!=(const deque< Ty >& left, const deque< Ty >& right)
{
	return !(left == right);
}

// test if left < right for deques
template< class Ty >
bool operator<(const deque< Ty >& left, const deque< Ty >& right)
{
	return std::lexicographical_compare(left.begin(), left.end(),
		right.begin(), right.end());
}

// test if left <= right for deques
template< class Ty >
bool operator<=(const deque< Ty >& left, const deque< Ty >& right)
{
	return !(right < left);
}

// test if left > right for deques
template< class Ty >
bool operator>(const deque< Ty >& left, const deque< Ty >& right)
{
	return right < left;
}

// test if left >= right for deques
template< class Ty >
bool operator>=(const deque< Ty >& left, const deque< Ty >& right)
{
	return !(left < right);
}

#endif