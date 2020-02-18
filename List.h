#ifndef LIST_H
#define LIST_H

#include <list>

// ListNode class template definition
template< typename T >
struct ListNode
{
   ListNode *next;
   T myVal;
   ListNode *prev;
}; // end class template ListNode


// list class template definition
template< typename T >
class list
{
public:
   using iterator = ListNode< T > *;
   using const_iterator = ListNode< T > * const;

   list(); // Constructs an empty list container, with no elements.
   list( unsigned int n ); // Constructs a list container with n elements.

   // Destroys all list elements,
   // and deallocates all the storage allocated by the list container.
   ~list();

   iterator begin(); // Returns a pointer pointing to the first element in the list container.

   iterator end(); // Returns an pointer referring to the past-the-end element in the list container.

   bool empty() const; // Returns a bool value indicating whether the linked list is empty.

   // Returns the number of elements in the list container.
   unsigned int size() const;

   // The list container is extended by inserting a new element
   // before the element at the specified position.
   // This effectively increases the list size by one.
   iterator insert( const_iterator position, const T &val );

   // Removes from the list container the element at the specified position.
   // This effectively reduces the list size by one.
   iterator erase( const_iterator position );

   // Resizes the list container so that it contains n elements.
   // If n is smaller than the current list container mySize,
   // the content is reduced to its first n elements, removing those beyond.
   // If n is greater than the current list container mySize,
   // the content is expanded by inserting at the end
   // as many elements as needed to reach a mySize of n.
   void resize( unsigned int n );

   // Removes all elements from the list container (which are destroyed).
   void clear();

   // determine if two lists are equal
   bool equal( std::list< T > &stdList );

private:
   unsigned int mySize; // the number of elements in the list container

   // pointing to the past-the-end element in the list container
   ListNode< T > *myHead;
}; // end class template list


// Constructs an empty list container, with no elements.
template< typename T >
list< T >::list()
   : mySize( 0 )
{
   myHead = new ListNode< T >;
   myHead->myVal = T();
   myHead->prev = myHead->next = myHead;
}

// Constructs a list container with n elements.
template< typename T >
list< T >::list( unsigned int n )
   : mySize( n )
{
   myHead = new ListNode< T >;
   myHead->myVal = T();
   myHead->prev = myHead->next = myHead;

   ListNode< T > *newNode = nullptr;
   if( n > 0 )
   {
      for( unsigned int i = 0; i < n; i++ )
      {
         newNode = new ListNode< T >;
         newNode->myVal = T();
         newNode->prev = myHead->prev;
         myHead->prev->next = newNode;
         myHead->prev = newNode;
      }
      newNode->next = myHead;
   }
}

// Destroys all list elements,
// and deallocates all the storage allocated by the list container.
template< typename T >
list< T >::~list()
{
   clear();
   delete myHead;
}

// Returns a pointer pointing to the first element in the list container.
template< typename T >
typename list< T >::iterator list< T >::begin()
{
   return myHead->next;
}

// Returns an pointer referring to the past-the-end element in the list container.
template< typename T >
typename list< T >::iterator list< T >::end()
{
   return myHead;
}

// Returns a bool value indicating whether the linked list is empty.
template< typename T >
bool list< T >::empty() const
{
   return ( mySize == 0 );
}

template< typename T >
unsigned int list< T >::size() const
{
   return mySize;
}

// The list container is extended by inserting a new element
// before the element at the specified position.
// This effectively increases the list size by one.
template< typename T >
typename list< T >::iterator list< T >::insert( const_iterator position, const T &val )
{
	ListNode< T > *newNode = new ListNode< T >;
	newNode->myVal = val;

	newNode->prev = position->prev;
	position->prev->next = newNode;
	position->prev = newNode;
	newNode->next = position;

	mySize++;

	return position;
}

// Removes from the list container the element at the specified position.
// This effectively reduces the list size by one.
template< typename T >
typename list< T >::iterator list< T >::erase( const_iterator position )
{
	iterator temp = position;
	position->next->prev = position->prev;
	position->prev->next = position->next;

	delete temp;

	mySize--;
	return position;
}

// Resizes the list container so that it contains n elements.
template< typename T >
void list< T >::resize( unsigned int n )
{
	while (n != mySize)
	{
		if (n > mySize)
			insert(end(), T());
		else
			erase(myHead->prev);
	}
}

// Removes all elements from the list container (which are destroyed)
template< typename T >
void list< T >::clear()
{
   if( mySize > 0 ) // the list is not empty
   {
      while( myHead->next != myHead )
      {
         myHead->next = myHead->next->next;
         delete myHead->next->prev;
      }

      myHead->prev = myHead;
      mySize = 0;
   }
}

// determine if two lists are equal
template< typename T >
bool list< T >::equal( std::list< T > &stdList )
{
   if( mySize != stdList.size() ) // different number of elements
      return false;

   list< T >::iterator ptr = myHead->next;
   typename std::list< T >::iterator it = stdList.begin();
   for( ; ptr != myHead && it != stdList.end(); ptr = ptr->next, ++it )
      if( ptr->myVal != *it )
         return false;

   return true;
}

#endif