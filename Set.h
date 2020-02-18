#ifndef SET_H
#define SET_H

enum Color { Red, Black };

// TreeNode class template definition
template< typename Kty >
class TreeNode
{
	template< typename K > friend class set;
private:
	TreeNode* left;   // left subtree, or smallest element if head
	TreeNode* parent; // parent, or root of tree if head
	TreeNode* right;  // right subtree, or largest element if head
	Color color;      // red or black, black if head
	bool isNil;       // true only if head (also nil) node
	Kty myval;
}; // end class template TreeNode

// set class template definition
template< typename Kty >
class set
{
public:
	set(); // Constructs an empty set object, with head node.

	~set(); // Destroys the set object.

	// Extends the set container by inserting a new element,
	// effectively increasing the container size by one.
	void insert(const Kty& val);

	// Removes from the set container a single element whose value is val
	// This effectively reduces the container size by one, which are destroyed.
	// Returns the number of elements erased.
	unsigned int erase(const Kty& val);

private:
	TreeNode< Kty >* myHead; // pointer to head node
	unsigned int mySize;     // number of elements ( not include head node )

	// Removes all elements from the set object (which are destroyed)
	void clear(TreeNode< Kty >* node);

	// rebalance for insertion
	void reBalance(TreeNode< Kty >* node);

	// rotate right at g, where p = g->left and node = p->left
	void LLRotation(TreeNode< Kty >* p);

	// LR rotation; p = g->left and node = p->right
	void LRRotation(TreeNode< Kty >* node);

	// RL rotation; p = g->right and node = p->left
	void RLRotation(TreeNode< Kty >* node);

	// rotate left at g, where p = g->right and node = p->right
	void RRRotation(TreeNode< Kty >* p);

	// erase node provided that the degree of node is at most one
	void eraseDegreeOne(TreeNode< Kty >* node);

	// rebalance for deletion
	void fixUp(TreeNode< Kty >* N, TreeNode< Kty >* P);

	void twoTraversals(); // preorder traversal and inorder traversal

	void preorder(TreeNode< Kty >* node); // preorder traversal

	void inorder(TreeNode< Kty >* node); // inorder traversal
}; // end class template set

// Constructs an empty set object, with head node.
template< typename Kty >
set< Kty >::set()
	: myHead(new TreeNode< Kty >),
	mySize(0)
{
	myHead->left = myHead;
	myHead->parent = myHead;
	myHead->right = myHead;
	myHead->color = Black;
	myHead->isNil = true;
}

// Destroys the set object.
template< typename Kty >
set< Kty >::~set()
{
	clear(myHead->parent);
	delete myHead;
}

// Extends the container by inserting a new element,
// effectively increasing the container size by one.
template< typename Kty >
void set< Kty >::insert(const Kty& val)
{
	TreeNode< Kty >* p = myHead->parent;
	TreeNode< Kty >* pp = myHead;

	while (p != myHead)
	{
		pp = p;
		if (val < p->myval)
			p = p->left;
		else if (val > p->myval)
			p = p->right;
		else
			return;
	}

	p = new TreeNode< Kty >;
	p->color = Red;
	p->isNil = false;
	p->left = myHead;
	p->right = myHead;
	p->myval = val;
	p->parent = pp;

	if (myHead->parent == myHead)
	{
		p->color = Black;
		myHead->parent = p;
		myHead->left = p;
		myHead->right = p;
	}
	else
	{
		if (val < pp->myval)
			pp->left = p;
		else
			pp->right = p;

		reBalance(p);
	}
	mySize++;
}

// Removes from the set container a single element whose value is val
// This effectively reduces the container size by one, which are destroyed.
// Returns the number of elements erased.
template< typename Kty >
unsigned int set< Kty >::erase(const Kty& val)
{
	TreeNode< Kty >* p = myHead->parent;
	
	while (p != myHead && p->myval != val)
	{
		if (val < p->myval)
			p = p->left;
		else
			p = p->right;
	}

	if (p != myHead)
	{
		if (p->left == myHead || p->right == myHead)
			eraseDegreeOne(p);
		else
		{
			TreeNode< Kty >* rightSubMin = p->right;
			while (rightSubMin->left != myHead)
				rightSubMin = rightSubMin->left;

			p->myval = rightSubMin->myval;
			eraseDegreeOne(rightSubMin);
		}
		return val;
	}
	return 0;
}

// Removes all elements from the set object (which are destroyed)
template< typename Kty >
void set< Kty >::clear(TreeNode< Kty >* node)
{
	if (!node->isNil) // node is not an external node
	{
		clear(node->left);
		clear(node->right);
		delete node;
	}
}

// rebalance for insertion
template< typename Kty >
void set< Kty >::reBalance(TreeNode< Kty >* node)
{  // node->parent cannot be the root
	while (node->color == Red && node->parent->color == Red)
	{
		TreeNode< Kty >* p = node->parent;
		TreeNode< Kty >* g = p->parent;
		if ((p == g->left && (g->right)->color == Red) ||
			(p == g->right && (g->left)->color == Red))
		{
			g->color = Red;
			p->color = Black;
			if (p == g->left)
				g->right->color = Black;
			else if (p == g->right)
				g->left->color = Black;

			node = g;
		}
		else
		{
			if (p == g->left && node == p->left)
			{
				g->color = Red;
				p->color = Black;
				LLRotation(p);
				node = p;
			}
			else if (p == g->left && node == p->right)
			{
				node->color = Black;
				g->color = Red;
				LRRotation(node);
			}
			else if (p == g->right && node == p->left)
			{
				node->color = Black;
				g->color = Red;
				RLRotation(node);
			}
			else if (p == g->right && node == p->right)
			{
				g->color = Red;
				p->color = Black;
				RRRotation(p);
				node = p;
			}
		}
	}
	myHead->parent->color = Black;
}

// rotate right at g, where p = g->left and node = p->left
template< typename Kty >
void set< Kty >::LLRotation(TreeNode< Kty >* p)
{
	TreeNode< Kty >* g = p->parent;

	g->left = p->right;
	if (p->right != myHead)
		p->right->parent = g;

	p->right = g;

	if (g->parent != myHead)
		if (g == g->parent->right)
			g->parent->right = p;
		else
			g->parent->left = p;
	else
		myHead->parent = p;

	p->parent = g->parent;
	g->parent = p;
}

// LR rotation; p = g->left and node = p->right
template< typename Kty >
void set< Kty >::LRRotation(TreeNode< Kty >* node)
{
	TreeNode< Kty >* p = node->parent;
	TreeNode< Kty >* g = node->parent->parent;

	p->right = node->left;
	if (node->left != myHead)
		node->left->parent = p;

	g->left = node->right;
	if (node->right != myHead)
		node->right->parent = g;

	if (g->parent != myHead)
		if (g == g->parent->left)
			g->parent->left = node;
		else
			g->parent->right = node;
	else
		myHead->parent = node;

	node->left = p;
	node->right = g;
	node->parent = g->parent;
	p->parent = node;
	g->parent = node;
}

// RL rotation; p = g->right and node = p->left
template< typename Kty >
void set< Kty >::RLRotation(TreeNode< Kty >* node)
{
	TreeNode< Kty >* p = node->parent;
	TreeNode< Kty >* g = node->parent->parent;

	p->left = node->right;
	if (node->right != myHead)
		node->right->parent = p;

	g->right = node->left;
	if (node->left != myHead)
		node->left->parent = g;

	if (g->parent != myHead)
		if (g == g->parent->left)
			g->parent->left = node;
		else
			g->parent->right = node;
	else
		myHead->parent = node;

	node->left = g;
	node->right = p;
	node->parent = g->parent;
	p->parent = node;
	g->parent = node;
}

// rotate left at g, where p = g->right and node = p->right
template< typename Kty >
void set< Kty >::RRRotation(TreeNode< Kty >* p)
{
	TreeNode< Kty >* g = p->parent;

	g->right = p->left;
	if (p->left != myHead)
		p->left->parent = g;

	if (g->parent != myHead)
		if (g == g->parent->left)
			g->parent->left = p;
		else
			g->parent->right = p;
	else
		myHead->parent = p;

	p->left = g;
	p->parent = g->parent;
	g->parent = p;
}

// erase node provided that the degree of node is at most one
template< typename Kty >
void set< Kty >::eraseDegreeOne(TreeNode< Kty >* node)
{
	TreeNode< Kty >* child;
	if (node->left != myHead)
		child = node->left;
	else if (node->right != myHead)
		child = node->right;
	else
		child = myHead;

	if (node == myHead->parent) // node is the root
	{
		myHead->parent = child;
	}
	else
	{
		if (node == node->parent->left)
			node->parent->left = child;
		else
			node->parent->right = child;
	}

	if (child != myHead)
		child->parent = node->parent;

	if (node->color == Black)
		fixUp(child, node->parent);

	delete node;
	mySize--;
}

// rebalance for deletion
template< typename Kty >
void set< Kty >::fixUp(TreeNode< Kty >* N, TreeNode< Kty >* P)
{
	TreeNode< Kty >* S = nullptr;
	Color buffer;

	while (N != myHead->parent && N->color == Black)
	{
		if (N == P->left)
		{
			S = P->right;

			if (S->color == Red)
			{
				buffer = P->color;
				P->color = Red;
				S->color = buffer;

				RRRotation(S);
			}
			else
			{
				if (S->right->color == Red)
				{
					buffer = P->color;
					P->color = Black;
					S->color = buffer;
					S->right->color = Black;

					RRRotation(S);
					break;
				}
				else
				{
					if (S->left->color == Red)
					{
						S->color = Red;
						S->left->color = Black;

						LLRotation(S->left);
					}
					else
					{
						if (P->color == Red)
						{
							P->color = Black;
							S->color = Red;
							break;
						}
						else
						{
							S->color = Red;

							if (P == myHead->parent)
								break;
							else
							{
								N = P;
								P = P->parent;
							}
						}
					}
				}
			}
		}
		else
		{
			S = P->left;

			if (S->color == Red)
			{
				buffer = P->color;
				P->color = Red;
				S->color = buffer;

				LLRotation(S);
			}
			else
			{
				if (S->left->color == Red)
				{
					buffer = P->color;
					P->color = Black;
					S->color = buffer;
					S->left->color = Black;

					LLRotation(S);
					break;
				}
				else
				{
					if (S->right->color == Red)
					{
						S->color = Red;
						S->right->color = Black;
						RRRotation(S->right);
					}
					else
					{
						if (P->color == Red)
						{
							P->color = Black;
							S->color = Red;
							break;
						}
						else
						{
							S->color = Red;

							if (P == myHead->parent)
								break;
							else
							{
								N = P;
								P = P->parent;
							}
						}
					}
				}
			}
		}
	}
	N->color = Black;
}

// preorder traversal and inorder traversal
template< typename Kty >
void set< Kty >::twoTraversals()
{
	cout << "Preorder sequence:\n";
	preorder(myHead->parent);

	cout << "\nInorder sequence:\n";
	inorder(myHead->parent);
	cout << endl;
}

// preorder traversal
template< typename Kty >
void set< Kty >::preorder(TreeNode< Kty >* node)
{
	if (node != myHead)
	{
		cout << setw(5) << node->myval << (node->color == Red ? "R" : "B");
		preorder(node->left);
		preorder(node->right);
	}
}

// inorder traversal
template< typename Kty >
void set< Kty >::inorder(TreeNode< Kty >* node)
{
	if (node != myHead)
	{
		inorder(node->left);
		cout << setw(5) << node->myval << (node->color == Red ? "R" : "B");
		inorder(node->right);
	}
}

#endif