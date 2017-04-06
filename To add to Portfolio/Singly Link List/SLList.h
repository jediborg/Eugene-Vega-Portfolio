#pragma once
template<typename Type> class SLLIter;

template<typename Type> class SLList
{
	// the iterator is the list’s friend
	friend class SLLIter<Type>;

	// add members/methods here…
	struct Node
	{
		Node * m_next;
		Type m_element;
	};

	Node *m_head;
	int m_currSize;

public:
	SLList();
	~SLList();
	SLList<Type>& operator=(const SLList<Type>& that);
	SLList(const SLList<Type>& that);
	void addHead(const Type& v);
	void clear();
	void insert(SLLIter<Type>& index, const Type& v);
	void remove(SLLIter<Type>& index);
	inline unsigned int size() const { return m_currSize; }

};

/////////////////////////////////////////////////////////////////////////////
// Function : Constructor
// Notes : constructs an empty list
/////////////////////////////////////////////////////////////////////////////
template<typename Type>
SLList<Type>::SLList()
{
	m_head = nullptr;
	m_currSize = 0;
}

/////////////////////////////////////////////////////////////////////////////
// Function : Destructor
// Notes : Destroys the list
/////////////////////////////////////////////////////////////////////////////
template<typename Type>
SLList<Type>::~SLList()
{
	while (m_head != nullptr)
	{
		Node* tmp = m_head;
		m_head = m_head->m_next;
		delete tmp;
	}
}

/////////////////////////////////////////////////////////////////////////////
// Function : Assignment Operator
/////////////////////////////////////////////////////////////////////////////
template<typename Type>
SLList<Type>& SLList<Type>::operator=(const SLList<Type>& that)
{
	//if (&that != nullptr)
	//{
		m_head = that.m_head;
		m_currSize = that.m_currSize;
	//}
	return *this;
}

/////////////////////////////////////////////////////////////////////////////
// Function : Copy Constructor
/////////////////////////////////////////////////////////////////////////////
template<typename Type>
SLList<Type>::SLList(const SLList<Type>& that)
{
	m_head = that.m_head;
	m_currSize = that.m_currSize;
}

/////////////////////////////////////////////////////////////////////////////
// Function : addHead
// Parameters :	v - the item to add to the list
/////////////////////////////////////////////////////////////////////////////
template<typename Type>
void SLList<Type>::addHead(const Type& v)
{
	Node* tmp = new Node;

	tmp->m_element = v;
	tmp->m_next = m_head;
	m_head = tmp;
	++m_currSize;
	


	//test
	//Node *tmp = new Node;
	//tmp->m_element = v;
	//tmp->m_next = nullptr;

	//if (m_head == nullptr)
	//{
	//	m_head = tmp;
	//}
	//else
	//{
	//	tmp->m_next = m_head;
	//	//tmp->m_next->m_prev = tmp;
	//	m_head = tmp;
	//}	

}

/////////////////////////////////////////////////////////////////////////////
// Function : clear
// Notes : clears the list, freeing any dynamic memory
/////////////////////////////////////////////////////////////////////////////
template<typename Type>
void SLList<Type>::clear()
{

	while (m_head != nullptr)
	{
		Node* tmp = m_head;
		m_head = m_head->m_next;
		delete tmp;
	}
	
	m_head = nullptr;
	m_currSize = 0;
}

/////////////////////////////////////////////////////////////////////////////
// Function : insert
// Parameters :	index - an iterator to the location to insert at
//				v - the item to insert
// Notes : do nothing on a bad iterator
/////////////////////////////////////////////////////////////////////////////
template<typename Type>
void SLList<Type>::insert(SLLIter<Type>& index, const Type& v)
{
	Node* tmp = new Node;
	tmp->m_element = v;

	Node* m_prev = index.c;

	if (m_prev == m_head)
		addHead(v);
	else
	{
		tmp->m_next = m_prev->m_next;
		m_prev->m_next = tmp;
	}
	++m_currSize;
}

/////////////////////////////////////////////////////////////////////////////
// Function : remove
// Parameters :	index - an iterator to the location to remove from
// Notes : do nothing on a bad iterator
/////////////////////////////////////////////////////////////////////////////
template<typename Type>
void SLList<Type>::remove(SLLIter<Type>& index)
{
	Node* tmp;
	Node* m_prev = index.c;
	
	if (m_prev == m_head)
	{
		m_prev = m_head->m_next;
		delete m_head;
		m_head = m_prev;
	}
	else
	{
		tmp = m_prev->m_next;
		m_prev->m_next = m_prev->m_next->m_next;

	}
	--m_currSize;
}


template<typename Type> class SLLIter
{
	friend class SLList<Type>;
	SLList<Type> m_list;
	typename SLList<Type>::Node * c;
	
public:
	SLLIter();
	~SLLIter();
	SLLIter(SLList<Type>& listToIterate);
	void begin();
	bool end() const;
	SLLIter<Type>& operator++();
	Type& current() const;
};

template<typename Type>
SLLIter<Type>::SLLIter()
{

}

template<typename Type>
SLLIter<Type>::~SLLIter()
{

}

/////////////////////////////////////////////////////////////////////////////
// Function : Constructor
// Parameters :	listToIterate - the list to iterate
/////////////////////////////////////////////////////////////////////////////
template<typename Type>
SLLIter<Type>::SLLIter(SLList<Type>& listToIterate)
{
	m_list = listToIterate;
}

/////////////////////////////////////////////////////////////////////////////
// Function : begin
// Notes : moves the iterator to the head of the list
/////////////////////////////////////////////////////////////////////////////
template<typename Type>
void SLLIter<Type>::begin()
{
	c = m_list.m_head;
}

/////////////////////////////////////////////////////////////////////////////
// Function : end
// Notes : returns true if we are at the end of the list, false otherwise
/////////////////////////////////////////////////////////////////////////////
template<typename Type>
bool SLLIter<Type>::end() const
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
SLLIter<Type>& SLLIter<Type>::operator++()
{
	if (c == nullptr)
	{
		return *this;
	}
	else
	{
		c = c->m_next;

		//return *this;
	}
}

/////////////////////////////////////////////////////////////////////////////
// Function : current
// Notes : returns the item at the current iterator location
/////////////////////////////////////////////////////////////////////////////
template<typename Type>
Type& SLLIter<Type>::current() const
{
	return c->m_element;
}




