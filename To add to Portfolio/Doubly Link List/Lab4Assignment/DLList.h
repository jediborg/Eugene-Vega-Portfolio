#pragma once
template<typename Type> class DLLIter;

template<typename Type> class DLList
{
	// the iterator is the list’s friend
	friend class DLLIter<Type>;

	// add members/methods here…
	struct Node
	{
		Node * m_next, *m_prev;
		Type m_element;
	};

	Node *m_head;
	Node *m_tail;
	

public:
	DLList();
	~DLList();
	DLList<Type>& operator=(const DLList<Type>& that);
	DLList(const DLList<Type>& that);
	void addHead(const Type& v);
	void addTail(const Type& v);
	void clear();
	void insert(DLLIter<Type>& index, const Type& v);
	void remove(DLLIter<Type>& index);
};

/////////////////////////////////////////////////////////////////////////////
// Function : Constructor
// Notes : constructs an empty list
/////////////////////////////////////////////////////////////////////////////
template<typename Type>
DLList<Type>::DLList()
{
	m_head = nullptr;
	m_tail = nullptr;
}

/////////////////////////////////////////////////////////////////////////////
// Function : Destructor
// Notes : Destroys a list
/////////////////////////////////////////////////////////////////////////////
template<typename Type>
DLList<Type>::~DLList()
{
	clear();
}

/////////////////////////////////////////////////////////////////////////////
// Function : Assignment Operator
/////////////////////////////////////////////////////////////////////////////
template<typename Type>
DLList<Type>& DLList<Type>::operator=(const DLList<Type>& that)
{

	if (&that != this)
	{
		clear();
		Node* tmp = that.m_tail;
		while (tmp != nullptr)
		{
				addHead(tmp->m_element);
				tmp = tmp->m_prev;
		}
	}
	return *this;
}

/////////////////////////////////////////////////////////////////////////////
// Function : Copy Constructor
/////////////////////////////////////////////////////////////////////////////
template<typename Type>
DLList<Type>::DLList(const DLList<Type>& that)
{


	m_head = nullptr;
	m_tail = nullptr;

	operator=(that);

}

/////////////////////////////////////////////////////////////////////////////
// Function : addHead
// Parameters : v - the item to add to the head of the list
/////////////////////////////////////////////////////////////////////////////
template<typename Type>
void DLList<Type>::addHead(const Type& v)
{
	Node* tmp = new Node;

	tmp->m_element = v;
	tmp->m_next = m_head;

	if (m_head != nullptr)
		m_head->m_prev = tmp;

	tmp->m_prev = nullptr;

	if (m_head == nullptr)
		m_tail = tmp;

	m_head = tmp;
}

/////////////////////////////////////////////////////////////////////////////
// Function : addTail
// Parameters : v - the item to add to the tail of the list
/////////////////////////////////////////////////////////////////////////////
template<typename Type>
void DLList<Type>::addTail(const Type& v)
{
	Node* tmp = new Node;

	tmp->m_element = v;
	tmp->m_next = nullptr;

	if (m_tail != nullptr)
		m_tail->m_next = tmp;

	tmp->m_prev = m_tail;

	if (m_tail == nullptr)
		m_head = tmp;

	m_tail = tmp;
}

/////////////////////////////////////////////////////////////////////////////
// Function : clear
// Notes : clears the list, freeing any dynamic memory
/////////////////////////////////////////////////////////////////////////////
template<typename Type>
void DLList<Type>::clear()
{

	while (m_head != nullptr)
	{
		Node* tmp = m_head;
		m_head = m_head->m_next;
		delete tmp;
	}

	m_head = nullptr;
	m_tail = nullptr;
}


/////////////////////////////////////////////////////////////////////////////
// Function : insert
// Parameters :	index - an iterator to the location to insert at
//				v - the item to insert
// Notes : do nothing on a bad iterator
/////////////////////////////////////////////////////////////////////////////
template<typename Type>
void DLList<Type>::insert(DLLIter<Type>& index, const Type& v)
{
	if (index.c != nullptr)
	{
		if (index.c == m_head)
		{
			addHead(v);
		}
		else if (index.c != nullptr)
		{
			Node* tmp = new Node;
			tmp->m_element = v;
			tmp->m_next = index.c;
			tmp->m_prev = index.c->m_prev;
			index.c->m_prev->m_next = tmp;
			index.c->m_prev = tmp;
			index.c = tmp;
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// Function : remove
// Parameters :	index - an iterator to the location to remove from
// Notes : do nothing on a bad iterator
/////////////////////////////////////////////////////////////////////////////
template<typename Type>
void DLList<Type>::remove(DLLIter<Type>& index)
{
	Node* tmp;
	if (m_head != m_tail)
	{
		if (m_head == index.c)
		{
			tmp = m_head->m_next;
			tmp->m_prev = nullptr;
			delete m_head;
			m_head = tmp;
			index.c = tmp;
		}
		else if (m_tail == index.c)
		{
			tmp = m_tail->m_prev;
			tmp->m_prev = nullptr;
			delete m_tail;
			m_tail = tmp;
			index.c = tmp;
		}
		else if (index.c != nullptr)
		{
			tmp = index.c->m_next;
			tmp->m_prev = index.c->m_prev;
			tmp->m_prev->m_next = tmp;
			delete index.c;
			index.c = tmp;
		}
	}
	else
	{
		delete m_head;
		m_head = nullptr;
		m_tail = nullptr;
		index.c = m_head;
	}
}

template<typename Type> class DLLIter
{
	friend class DLList<Type>;
	DLList<Type>* m_list;
	typename DLList<Type>::Node * c;

public:
	DLLIter(DLList<Type>& listToIterate);
	void beginHead();
	void beginTail();
	bool end() const;
	DLLIter<Type>& operator++();
	DLLIter<Type>& operator--();
	Type& current() const;
};

/////////////////////////////////////////////////////////////////////////////
// Function : Constructor
// Parameters :	listToIterate - the list to iterate
/////////////////////////////////////////////////////////////////////////////
template<typename Type>
DLLIter<Type>::DLLIter(DLList<Type>& listToIterate)
{
	m_list = &listToIterate;

}
/////////////////////////////////////////////////////////////////////////////
// Function : beginHead
// Notes : moves the iterator to the head of the list
/////////////////////////////////////////////////////////////////////////////
template<typename Type>
void DLLIter<Type>::beginHead()
{
	c = m_list->m_head;

}
/////////////////////////////////////////////////////////////////////////////
// Function : beginTail
// Notes : moves the iterator to the tail of the list
/////////////////////////////////////////////////////////////////////////////
template<typename Type>
void DLLIter<Type>::beginTail()
{
	c = m_list->m_tail;
}
/////////////////////////////////////////////////////////////////////////////
// Function : end
// Notes : returns true if we are at the end of the list, false otherwise
/////////////////////////////////////////////////////////////////////////////
template<typename Type>
bool DLLIter<Type>::end() const
{
	if (c == nullptr)
		return true;
	else
		return false;
}
/////////////////////////////////////////////////////////////////////////////
// Function : operator++
// Notes : move the iterator forward one node
/////////////////////////////////////////////////////////////////////////////
template<typename Type>
DLLIter<Type>& DLLIter<Type>::operator++()
{
	if (c != nullptr)
		c = c->m_next;
	return *this;
}
/////////////////////////////////////////////////////////////////////////////
// Function : operator--
// Notes : move the iterator backward one node
/////////////////////////////////////////////////////////////////////////////
template<typename Type>
DLLIter<Type>& DLLIter<Type>::operator--()
{
	if (c != nullptr)
		c = c->m_prev;
	return *this;
}

/////////////////////////////////////////////////////////////////////////////
// Function : current
// Notes : returns the item at the current iterator location
////////////////////////////////////////////////////////////////////////////
template<typename Type>
Type& DLLIter<Type>::current() const
{
	return c->m_element;
}