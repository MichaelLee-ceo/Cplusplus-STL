#include "String.h" // string class definition
#include <iostream>
using std::cout;
using std::endl;

// Constructs an empty string, with a length of zero characters.
string::string()
   : mySize( 0 ),
     myRes( 15 )
{
   bx.buf[ 0 ] = '\0';
}

// Constructs a copy of str.
string::string( const string &str )
   : mySize( str.mySize ),
     myRes( str.myRes )
{
	if (myRes == 15)
	{
		strcpy_s(bx.buf, str.bx.buf);
	}
	else
	{
		bx.ptr = new char[myRes + 1];
		for (int i = 0; i <= mySize; i++)
			bx.ptr[i] = str.bx.ptr[i];
	}
}

// Constructs a string object, initializing its value by
// coping the first n characters from the array of characters pointed by s.
string::string( const char *s, unsigned int n )
   : mySize(n),
     myRes()
{
	myRes = 15 + (n / 16) * 16;

	if (myRes == 15)
	{
		strcpy_s(bx.buf, s);
	}
	else
	{
		bx.ptr = new char[myRes + 1];
		for (int i = 0; i <= n; i++)
			bx.ptr[i] = s[i];
	}
}

// Destroys the string object.
string::~string()
{
   if( myRes > 15 )
      delete[] bx.ptr;
}

// Returns an pointer pointing to the first character of the string.
string::iterator string::begin()
{
   if( myRes == 15 )
      return bx.buf;
   else
      return bx.ptr;
}

// Returns an pointer pointing to the past-the-end character of the string.
string::iterator string::end()
{
   if( myRes == 15 )
      return bx.buf + mySize;
   else
      return bx.ptr + mySize;
}

// Returns the number of characters in the string.
unsigned int string::size() const
{
   return mySize;
}

// Returns the size of the storage space currently allocated for the string,
// expressed in terms of bytes.
unsigned int string::capacity() const
{
   return myRes;
}

// Resizes the string to a length of n characters.
// If n is smaller than the current string length,
// the current value is shortened to its first n character,
// removing the characters beyond the nth.
// If n is greater than the current string length,
// the current content is extended by inserting at the end
// as many characters as needed to reach a size of n.
// The new elements are initialized as copies of null characters.
void string::resize( unsigned int n )
{
	int newMyRes;
	if (n > myRes)
	{
		if ((n / 16) * 16 + 15 > myRes * 1.5)
			newMyRes = (n / 16) * 16 + 15;
		else
			newMyRes = myRes * 1.5;
	}
	else
		newMyRes = myRes;

	/*if (myRes == 15)
	{
		bx.buf[n] = '\0';
	}
	else
	{
		char *buffer = new char[myRes + 1];
		for (int i = 0; i < mySize; i++)
			buffer[i] = bx.ptr[i];

		for (int j = mySize; j < n; j++)
			buffer[j] = '\0';

		bx.ptr = buffer;
	}*/
	if (newMyRes == 15)
	{
		if (mySize > n)
			for (int i = n; i <= mySize; i++)
				bx.buf[i] = '\0';
		else
			for (int i = mySize; i <= n; i++)
				bx.buf[i] = '\0';
	}
	else
	{
		char *buffer = new char[newMyRes + 1];
		
		if (myRes > 15)
			for (int i = 0; i <= mySize; i++)
				buffer[i] = bx.ptr[i];
		else
			for (int i = 0; i <= mySize; i++)
				buffer[i] = bx.buf[i];

		if (mySize > n)
			for (int i = n; i <= mySize; i++)
				buffer[i] = '\0';
		else
			for (int i = mySize; i <= n; i++)
				buffer[i] = '\0';

		if (mySize > 15)
			delete[] bx.ptr;

		bx.ptr = buffer;
	}
	mySize = n;
	myRes = newMyRes;
	
}

// Assigns str to the string, replacing its current contents.
string& string::assign( const string &str )
{
	if (str.myRes > myRes)
	{
		if (myRes > 15)
			delete[] bx.ptr;

		myRes = (myRes * 1.5 > str.myRes) ? myRes * 1.5 : str.myRes;

		bx.ptr = new char[myRes + 1];
		for (int i = 0; i <= str.mySize; i++)
			bx.ptr[i] = str.bx.ptr[i];
	}
	else
	{
		if (myRes == 15)
		{
			strcpy_s(bx.buf, str.bx.buf);
		}
		else
		{
			if (str.myRes == 15)
				for (int i = 0; i <= str.mySize; i++)
					bx.ptr[i] = str.bx.buf[i];
			else
				for (int i = 0; i <= str.mySize; i++)
					bx.ptr[i] = str.bx.ptr[i];
		}
	}
	
	mySize = str.mySize;
   return *this; // enables x = y = z, for example
}

// Inserts character c into the string right before the character indicated by p
string::iterator string::insert( iterator p, char c )
{
	int newMyRes;
	if (mySize == myRes)
		if (myRes < 47)
			newMyRes = myRes + 16;
		else
			newMyRes = myRes * 1.5;
	else
		newMyRes = myRes;

	if (newMyRes > myRes)
	{
		char *buffer = new char[newMyRes + 1];

		iterator p1 = buffer;
		iterator p2 = begin();

		for (; p2 < p; p1++, p2++)
			*p1 = *p2;

		*p1 = c;

		for (p1++; p2 != end(); p1++, p2++)
			*p1 = *p2;

		delete[] bx.ptr;
		bx.ptr = buffer;
	}
	else
	{
		iterator p1 = end();
		for (; p1 != p; p1--)
			*p1 = *(p1 - 1);
		*p1 = c;
	}
	mySize++;
	myRes = newMyRes;
	return p;

}

// Erases the character pointed by p.
string::iterator string::erase( iterator p )
{
	while (p < end() - 1)
	{
		*p = *(p + 1);
		p++;
	}

	*p = '\0';
	mySize--;

	return p;
}

// Returns true if and only if str is equal to the current string object.
bool string::equal( std::string &str )
{
   if( myRes != str.capacity() )
      return false;

   if( mySize != str.size() )
      return false;

   iterator tempPtr = ( myRes == 15 ? bx.buf : bx.ptr );
   for( unsigned int i = 0; i < mySize; i++ )
      if( tempPtr[ i ] != str[ i ] )
         return false;

   return true;
}