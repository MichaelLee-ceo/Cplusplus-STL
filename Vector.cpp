#include "Vector.h" // include definition of class vector 
#include <iostream>
using std::cout;
using std::endl;

// Constructs a zero vector with size n.
vector::vector( unsigned int n )
{
	if (n > 0)
	{
		myFirst = new int[n]();
		myLast = myEnd = myFirst + n;
	}
	else
		myFirst = myLast = myEnd = NULL;
} // end default constructor

// Destroys the vector.
vector::~vector()
{
   if( myFirst != nullptr )
      delete[] myFirst;
} // end destructor

unsigned int vector::size()
{
   return ( myLast - myFirst );
}

unsigned int vector::capacity()
{
   return ( myEnd - myFirst );
}

void vector::push_back( const int val )
{
	if (size() < capacity())
	{
		myFirst[size()] = val;
		myLast++;
	}
	else
	{
		int newCap;
		if (capacity() < 2)
			newCap = capacity() + 1;
		else
			newCap = capacity() * 1.5;

		int newSize = size() + 1;

		int *buffer = new int[newCap]();
		for (int i = 0; i < size(); i++)
			buffer[i] = myFirst[i];
		buffer[size()] = val;

		delete[] myFirst;

		myFirst = buffer;
		myLast = myFirst + newSize;
		myEnd = myFirst + newCap;
	}
	
}

// Removes the last element in the vector,
// effectively reducing the container size by one.
void vector::pop_back()
{
	if (size() > 0)
	{
		myLast--;
		*myLast = 0;
	}
}

void vector::resize( unsigned int n )
{
	int newCap;
	if (n > capacity())
		newCap = capacity() * 1.5;
	else
		newCap = capacity();
	
	if (n > newCap)
		newCap = n;

	int *buffer = new int[newCap]();
	for (int i = 0; i < size(); i++)
		buffer[i] = myFirst[i];

	delete[] myFirst;

	myFirst = buffer;
	myLast = myFirst + n;
	myEnd = myFirst + newCap;
}

// Determines if two vectors are equal.
bool vector::equal( std::vector< int > &v )
{
   if( capacity() != v.capacity() )
      return false;

   if( size() != v.size() )
      return false;

   for( unsigned int i = 0; i < size(); i++ )
      if( myFirst[ i ] != v[ i ] )
         return false;

   return true;
}