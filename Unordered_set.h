#ifndef UNORDERED_SET
#define UNORDERED_SET
#include <cmath>
#include <vector>
using std::vector;
using std::pow;

unsigned int maxValue = 99;

// unordered_set class template definition
template< typename T >
class unordered_set
{
public:
	using iterator = typename list< T >::iterator;

	unordered_set();

	~unordered_set();

	// Returns the number of elements in the unordered_set container.
	unsigned int size() const;

	// Searches the container for an element with k as value and
	// returns an iterator to it if found, otherwise it returns myList.end()
	iterator find(const T& k);

	// Inserts a new element in the unordered_set.
	// The element is inserted only if it is not equivalent to any other element
	// already in the container ( elements in an unordered_set have unique values ).
	// This effectively increases the container size by one.
	void insert(const T& val);

	// Removes from the unordered_set container a single element.
	// This effectively reduces the container size by one.
	void erase(const T& k);

	// Returns the number of buckets in the unordered_set container.
	unsigned int bucket_count() const;

	// Returns the number of elements in bucket n.
	unsigned int bucket_size(unsigned int n) const;

	// Returns the bucket number where the element with value k is located.
	unsigned int bucket(const T& k) const;

	// Returns true iff the current object is equal to data
	bool operator==(std::unordered_set< T >& data);

private:
	list< T > myList; // list of elements, must initialize before myVec

	// vector of list iterators for buckets:
	// each bucket is 2 iterators denoting the closed range of elements in the bucket,
	// or both iterators set to myList.end() if the bucket is empty.
	vector< iterator > myVec;

	unsigned int maxidx = 8; // current maximum key value, must be a power of 2

	// put a new element in the unordered_set when myVec is large enough
	void putIn(const T& val);

	// Computes hash value which will be used to compute bucket number
	unsigned int hashSeq(const unsigned int& key, unsigned int count) const;
}; // end class template unordered_set


template< typename T >
unordered_set< T >::unordered_set()
{
	myVec.reserve(16);
	myVec.assign(16, myList.end());
}

template< typename T >
unordered_set< T >::~unordered_set()
{
	myList.clear();
	myVec.clear();
}

template< typename T >
unsigned int unordered_set< T >::size() const
{
	return myList.size();
}

template< typename T >
typename unordered_set< T >::iterator unordered_set< T >::find(const T& k)
{
	iterator p1 = myList.begin();
	for (; p1 != myList.end(); ++p1)
		if (*p1 == k)
			return p1;
	return myList.end();
}

template< typename T >
void unordered_set< T >::insert(const T& val)
{
	if (find(val) != myList.end())
		return;

	if (size() == bucket_count())
	{
		int newMax = (maxidx < 512) ? maxidx * 8 : maxidx * 2;
		vector< iterator > newVec;
		newVec.reserve(newMax * 2);
		newVec.assign(newMax * 2, myList.end());

		list< T > newList;
		iterator p1 = myList.begin();
		for (; p1 != myList.end(); ++p1)
			newList.insert(newList.end(), *p1);

		myVec.clear();
		myVec = newVec;
		maxidx = newMax;
		myList.clear();

		for (p1 = newList.begin(); p1 != newList.end(); ++p1)
			putIn(*p1);
	}
	putIn(val);
}

template< typename T >
void unordered_set< T >::erase(const T& k)
{
	if (find(k) == myList.end())
		return;

	int b = bucket(k);
	iterator p1 = find(k);

	if (myVec[b * 2] == myVec[b * 2 + 1])
	{
		myVec[b * 2] = myVec[b * 2 + 1] = myList.end();
	}
	else
	{
		if (p1 == myVec[b * 2])
			myVec[b * 2] = ++p1;
		else if (p1 == myVec[b * 2 + 1])
			myVec[b * 2 + 1] = --p1;
	}
	myList.erase(find(k));
}

template< typename T >
unsigned int unordered_set< T >::bucket_count() const
{
	return maxidx;
}

// return size of bucket n
template< typename T >
unsigned int unordered_set< T >::bucket_size(unsigned int n) const
{
	if (myVec[n * 2] == myList.end())
		return 0;

	int count = 1;
	iterator p1 = myVec[n * 2];
	iterator p2 = myVec[n * 2 + 1];
	for (; p1 != p2; ++p1)
		count++;

	return count;
}

template< typename T >
unsigned int unordered_set< T >::bucket(const T& k) const
{
	unsigned int hashValue = hashSeq(k, sizeof(T));
	return hashValue % maxidx; // bucket number
}

template< typename T >
void unordered_set< T >::putIn(const T& val)
{
	int b = bucket(val);

	if (myVec[b * 2] == myList.end())
	{
		myList.insert(myList.end(), val);
		myVec[b * 2] = --myList.end();
		myVec[b * 2 + 1] = --myList.end();
	}
	else
	{
		myList.insert(myVec[b * 2], val);
		myVec[b * 2] = find(val);
	}
}

template< typename T >
unsigned int unordered_set< T >::hashSeq(const unsigned int& key, unsigned int count) const
{
	long long int h = 2166136261;
	int prime = 16777619;
	int buffer = key;

	vector<int> data;
	vector<int> arrhash;

	while (buffer > 0)
	{
		data.push_back(buffer % 2);
		buffer /= 2;
	}
	data.resize(32, 0);

	for (int i = 0; i < 4; i++)
	{
		arrhash.clear();
		while (h > 0)
		{
			arrhash.push_back(h % 2);
			h /= 2;
		}
		arrhash.resize(32, 0);

		for (int j = 0; j < 8; j++)
		{
			if (data[i * 8 + j] == 1 && arrhash[j] == 1)
				arrhash[j] = 0;
			else if (data[i * 8 + j] == 1 && arrhash[j] == 0)
				arrhash[j] = 1;
		}

		h = 0;
		for (int i = 0; i < 32; i++)
			if (arrhash[i] != 0)
				h += pow(2, i);
		h = (h * prime) % 4294967296;
	}

	return h;
}

template< typename T >
bool unordered_set< T >::operator==(std::unordered_set< T >& data)
{
	if (myList.size() != data.size())
		return false;

	unsigned int* firstVec = *(reinterpret_cast<unsigned int**>(&data) + 5);
	vector< list< T >::iterator >::iterator it = myVec.begin();
	for (unsigned int bucketNo = 0; it != myVec.end(); ++it, bucketNo++)
	{
		if (data.bucket_size(bucketNo) != bucket_size(bucketNo))
			return false;

		unsigned int* stlBucketFirst = *(reinterpret_cast<unsigned int**>(&firstVec[2 * bucketNo]));
		unsigned int* stlBucketLast = *(reinterpret_cast<unsigned int**>(&firstVec[2 * bucketNo + 1]));

		list< T >::iterator myBucketFirst = *it;
		++it;
		list< T >::iterator myBucketLast = *it;
		if (myBucketFirst != myList.end())
		{
			if (myBucketFirst == myBucketLast)
			{
				if (*myBucketFirst != *(stlBucketFirst + 2))
					return false;
			}
			else
			{
				unsigned int* stlPtr = stlBucketFirst;
				list< T >::iterator myIt = myBucketFirst;
				while (myIt != myBucketLast)
				{
					if (*myIt != *(stlPtr + 2))
						return false;
					stlPtr = *(reinterpret_cast<unsigned int**>(stlPtr));
					++myIt;
				}

				if (*myBucketLast != *(stlBucketLast + 2))
					return false;
			}
		}
	}

	for (unsigned int key = 1; key <= maxValue; key++)
		if (data.bucket(key) != bucket(key))
			return false;

	return true;
}

#endif
