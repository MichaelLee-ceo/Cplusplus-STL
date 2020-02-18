#ifndef STRING_H
#define STRING_H

#include <string> // STL string class definition

// string class definition
class string
{
public:
   typedef char *iterator;

   string(); // Constructs an empty string, with a length of zero characters.

   string( const string &str ); // Constructs a copy of str.

   // Constructs a string object, initializing its value by
   // coping the first n characters from the array of characters pointed by s.
   string( const char *s, unsigned int n );

   ~string(); // Destroys the string object.

   iterator begin(); // Returns an pointer pointing to the first character of the string.

   iterator end();   // Returns an pointer pointing to the past-the-end character of the string.

   unsigned int size() const; // Returns the number of characters in the string.

   // Returns the size of the storage space currently allocated for the string,
   // expressed in terms of bytes.
   unsigned int capacity() const;

   // Resizes the string to a length of n characters.
   // If n is smaller than the current string length,
   // the current value is shortened to its first n character,
   // removing the characters beyond the nth.
   // If n is greater than the current string length,
   // the current content is extended by inserting at the end
   // as many characters as needed to reach a size of n.
   // The new elements are initialized as copies of null characters.
   void resize( unsigned int n );

   // Assigns str to the string, replacing its current contents.
   string& assign( const string &str );

   // Inserts character c into the string right before the character indicated by p
   iterator insert( iterator p, char c );
   
   // Erases the character pointed by p.
   iterator erase( iterator p );

   // Returns true if and only if str is equal to the current string object.
   bool equal( std::string &str );

private:
   union Bxty
   {
      char buf[ 16 ]; // storage reserved for string provided that myRes == 15
      char *ptr;      // a pointer to a storage reserved for string provided that myRes > 15
   } bx;

   unsigned int mySize; // current length of string
   unsigned int myRes;  // current length of storage reserved for string
}; // end class string

#endif