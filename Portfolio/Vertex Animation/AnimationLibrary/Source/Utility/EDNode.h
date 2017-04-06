#ifndef _EDNODE_H_
#define _EDNODE_H_

#include <vector>

class EDNode
{
	// Parent for tree.
	EDNode *m_pParent;
	
	// Children for tree.
	std::vector< EDNode * > m_vrgChildren;

public:

	// Constructor
	EDNode(void) : m_pParent(0) {}
	// Destructor
	virtual ~EDNode(void){}

	// Return the parent of this node
	EDNode *GetParent(void){ return m_pParent; }
	
	// Return the children of this node
	std::vector< EDNode *> &GetChildren(void){ return m_vrgChildren; }

	// Add a child to the hierarchy of this node
	EDNode *AddChild( EDNode *pNode )
	{
		if( pNode->GetParent() != 0 )
			return 0;

		pNode->m_pParent = this;
		m_vrgChildren.push_back( pNode );
		return pNode;
	}

	// Remove a child from the hierarchy of this node
	EDNode *RemoveChild( EDNode *pNode )
	{
		if( pNode->GetParent() != this )
			return 0;

		for( unsigned int i = 0; i < m_vrgChildren.size(); ++i )
		{
			if( m_vrgChildren[i] == pNode )
			{
				m_vrgChildren.erase( m_vrgChildren.begin() + i );
				pNode->m_pParent = 0;
				return pNode;
			}
		}

		return 0;
	}
};

#endif
