#pragma once
template<typename Type> class BST
{
	struct Node
	{
		Node * m_LChild;
		Node * m_RChild;
		Type m_element; 
		Node()
		{
			m_LChild = nullptr;
			m_RChild = nullptr;
		}
	};

	Node * m_Root;

	void inorder(const Node* c) const;
	void preorder(const Node* c);
	void postorder(const Node* c);
	void insertHelper(Node* c, Node* curr);
	bool findHelper(const Type& v, Node* c) const;
	bool findAndRemoveHelper(const Type& v, Node* currNode, Node* prevNode);
	
public:
	BST() { m_Root = nullptr; }
	~BST() { clear(); }
	BST& operator=(const BST& that);
	BST(const BST& that);
	void insert(const Type& v);
	bool findAndRemove(const Type& v);
	bool find(const Type& v) const;
	void clear();
	void printInOrder() const;

};

/////////////////////////////////////////////////////////////////////////////
// Function : assignment operator
/////////////////////////////////////////////////////////////////////////////
template<typename Type>
BST<Type>& BST<Type>::operator=(const BST& that)
{
	if (&that != this)
	{
		clear();
		preorder(that.m_Root);
	}
	return *this;
}

/////////////////////////////////////////////////////////////////////////////
// Function: copy constructor
/////////////////////////////////////////////////////////////////////////////
template<typename Type>
BST<Type>::BST(const BST& that)
{
	m_Root = nullptr;
	operator=(that);
}

/////////////////////////////////////////////////////////////////////////////
// Function : insert
// Parameters :  v - the item to insert 
/////////////////////////////////////////////////////////////////////////////
template<typename Type>
void BST<Type>::insert(const Type& v)
{
	if (m_Root == nullptr) 
	{
		Node* c = new Node;
		c->m_element = v;
		m_Root = c;
	}
	else 
	{
		Node* c = new Node;
		c->m_element = v;
		Node* currNode = m_Root;
		insertHelper(c, currNode);
	}
}

template<typename Type>
void BST<Type>::insertHelper(Node* c, Node* curr)
{
	if (curr->m_element > c->m_element)
	{
		if (curr->m_LChild == nullptr)
			curr->m_LChild = c;
		else
			insertHelper(c, curr->m_LChild);
	}
	else if (curr->m_element <= c->m_element)
	{
		if (curr->m_RChild == nullptr)
			curr->m_RChild = c;
		else
			insertHelper(c, curr->m_RChild);
	}
}

/////////////////////////////////////////////////////////////////////////////
// Function : findAndRemove
// Parameters : v - the item to find (and remove if it is found)
// Return : bool - true if the item was removed, false otherwise
/////////////////////////////////////////////////////////////////////////////
template<typename Type>
bool BST<Type>::findAndRemove(const Type& v)
{
	Node* currNode = m_Root;
	Node* prevNode = nullptr;
	return findAndRemoveHelper(v, currNode, prevNode);
}

/////////////////////////////////////////////////////////////////////////////
// Function : Recursive Function - Find and Remove Helper Function
/////////////////////////////////////////////////////////////////////////////

template<typename Type>
bool BST<Type>::findAndRemoveHelper(const Type& v, Node* currNode, Node* prevNode)
{
	if (currNode != nullptr)
	{
		if (v < currNode->m_element)
			return findAndRemoveHelper(v, currNode->m_LChild, currNode);
		else if (v > currNode->m_element)
			return findAndRemoveHelper(v, currNode->m_RChild, currNode);
		else if (v == currNode->m_element)
		{
			if (currNode->m_LChild == nullptr && currNode->m_RChild == nullptr)			
			{									
				if (prevNode == nullptr)			
				{								
					delete m_Root;				
					m_Root = nullptr;
					return true;
				}								
				else if (prevNode->m_LChild == currNode)	
				{								
					prevNode->m_LChild = currNode->m_LChild;	
					delete currNode;				
					currNode = nullptr;				
					return true;				
				}								
				else if (prevNode->m_RChild == currNode)	
				{								
					prevNode->m_RChild = currNode->m_LChild;	
					delete currNode;				
					currNode = nullptr;				
					return true;					
				}										
			}
			else if (currNode->m_LChild != nullptr && currNode->m_RChild == nullptr)
			{									
				if (prevNode == nullptr)						
				{								
					m_Root = currNode->m_LChild;								 
					delete currNode;						
					return true;						
				}											
				else										
				{											
					if (prevNode->m_LChild == currNode)			
					{										
						prevNode->m_LChild = currNode->m_LChild;	
						delete currNode;					
						currNode = nullptr;					
						return true;						
					}										
					else if (prevNode->m_RChild == currNode)	
					{										
						prevNode->m_RChild = currNode->m_LChild;	
						delete currNode;								     
						currNode = nullptr;								     
						return true;
					}
				}
			}
			else if (currNode->m_LChild == nullptr && currNode->m_RChild != nullptr)
			{																
				if (prevNode == nullptr)									
				{															
					m_Root = currNode->m_RChild;								
					delete currNode;										
					currNode = nullptr;										
					return true;											
				}															
				else														
				{								
					if (prevNode->m_LChild == currNode)						
					{														
						prevNode->m_LChild = currNode->m_RChild;				
						delete currNode;									
						currNode = nullptr;									
						return true;										
					}														
					else if (prevNode->m_RChild == currNode)					
					{														
						prevNode->m_RChild = currNode->m_RChild;				
						delete currNode;
						currNode = nullptr;
						return true;
					}
				}
			}
			else if (currNode->m_LChild != nullptr && currNode->m_RChild != nullptr)		
			{																
				Node* min = currNode->m_RChild;								
				while (min->m_LChild)											
				{															
					prevNode = min;											
					min = min->m_LChild;										
				}															
				swap(min->m_element, currNode->m_element);					
				
				return findAndRemoveHelper(v, prevNode->m_LChild, prevNode);			
			}	
			return true;
		}																	
		else																
			return false;													
	}																		
	else																	
		return false;														
}																			


/////////////////////////////////////////////////////////////////////////////
// Function : find
// Parameters : v - the item to find
// Return : bool - true if the item was found, false otherwise
/////////////////////////////////////////////////////////////////////////////
template<typename Type>
bool BST<Type>::find(const Type& v) const
{
	Node* currNode = m_Root;
	return findHelper(v, currNode);	
}

/////////////////////////////////////////////////////////////////////////////
// Function : Recursive Function - Find Helper Fuction
/////////////////////////////////////////////////////////////////////////////
template<typename Type>
bool BST<Type>::findHelper(const Type& v, Node* c) const
{
	if (c != nullptr)
	{
		if (v < c->m_element)
			return findHelper(v, c->m_LChild);
		else if (v > c->m_element)
			return findHelper(v, c->m_RChild);
		else if (v == c->m_element)
			return true;
		else
			return false;
	}
	else
		return false;
}

/////////////////////////////////////////////////////////////////////////////
// Function : clear
// Notes : clears the BST, cleaning up any dynamic memory
/////////////////////////////////////////////////////////////////////////////
template<typename Type>
void BST<Type>::clear()
{
	postorder(m_Root);
	m_Root = nullptr;
}

/////////////////////////////////////////////////////////////////////////////
// Function : Recursive Function - post Order
/////////////////////////////////////////////////////////////////////////////
template<typename Type>
void BST<Type>::postorder(const Node* c)
{
	if (c != nullptr)
	{
		postorder(c->m_LChild);
		postorder(c->m_RChild);
		delete c;
	}
}

/////////////////////////////////////////////////////////////////////////////
// Function : printInOrder
// Notes : prints the contents of the BST to the screen, in ascending order
/////////////////////////////////////////////////////////////////////////////
template<typename Type>
void BST<Type>::printInOrder() const
{
	inorder(m_Root);
	cout << "\n";
}

/////////////////////////////////////////////////////////////////////////////
// Function : Recursive Function - In-Order
/////////////////////////////////////////////////////////////////////////////
template<typename Type>
void BST<Type>::inorder(const Node* c) const
{
	if (c != nullptr)
	{
		inorder(c->m_LChild);
		cout << c->m_element << " ";
		inorder(c->m_RChild);
	}
}

/////////////////////////////////////////////////////////////////////////////
// Function : Recursive Function - Pre-Order
/////////////////////////////////////////////////////////////////////////////
template<typename Type>
void BST<Type>::preorder(const Node* c)
{
	if (c!= nullptr)
	{
		insert(c->m_element);
		preorder(c->m_LChild);
		preorder(c->m_RChild);
	}	
}

