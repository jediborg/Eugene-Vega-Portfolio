#ifndef _EDFRAME_H_
#define _EDFRAME_H_

#include "Utility/EDNode.h"

#include "Math/matrix4.h"

// EDFrame
//
// This class allows for the creation of matrix/transform hierarchies.
// Hierarchies consist of multiple EDFrames, each with it's own object/local
// space matrix and it's own global/world space matrix.
//
// The object/local space matrix (m_LocalMat) stores transformations applied only to this frame.
// The position and orientation of the local space matrix is relative to the postion and orientation of the parent.
//
// The global/world space matrix (m_WorldMat) stores the concatenation (multiplication) of the local space
// matrix with the parent frame's world space matrix. The position and orientation of the world space matrix
// is in absolute world-space. It is where the object would/could be rendered.
//
// NOTE:
// The implementation of this class is not an exercise in efficiency. It is designed to achieve the desired
// result in a clear and concise manner and to demonstrate the principle of a matrix/transform hierarchy.
class EDFrame : public EDNode
{
	// The local space matrix of the frame.
	matrix4f m_LocalMat;

	// The world space matrix of the frame.
	//
	// If the frame has a parent,
	//
	//	m_WorldMat = m_LocalMat * m_pParent->m_WorldMat
	//
	// Else,
	//
	//	m_WorldMat = m_LocalMat;
	matrix4f m_WorldMat;

	// Flags used to identify if the frame needs to be updated
	unsigned int m_uiFlags;

public:
	// Enumeration of valid flags in m_uiFlags
	enum { DIRTY = 1 };

	// Constructor
	EDFrame(void) : m_uiFlags(0)
	{
	}

	// Destructor
	virtual ~EDFrame(void){}

	// Update
	// 
	// "Dirties" a frame and it's children.
	// The worldspace matrix (m_WorldMat) of "dirty" frames is
	// updated and "cleaned" when GetWorldMat() is called. 
	void Update(void)
	{
		if( (m_uiFlags & DIRTY) == 0 )
		{
			m_uiFlags |= DIRTY;

			for( unsigned int i = 0; i < GetChildren().size(); ++i )
				((EDFrame *)GetChildren()[i])->Update();
		}
	}

	// GetLocalMat
	//
	// Get the local space matrix of the frame.
	//
	// Return:
	//	matrix4f& - The local space matrix of the frame
	matrix4f& GetLocalMat(void){ return m_LocalMat; }

	// GetWorldMat
	//
	// Get the world space matrix of the frame.
	// If the frame is "dirty", the world space matrix is first
	// recalculated and the frame is cleaned.
	//
	// Return:
	//	const matrix4f& -	The world space matrix of the frame. This is returned
	//						as constant, as the world space matrix should not be
	//						edited directly. Any changes made directly to the world
	//						space matrix will be lost when the matrix is next updated
	//						because m_WorldMat = m_LocalMat, or m_WorldMat = m_LocalMat * pParent->m_WorldMat.
	matrix4f& GetWorldMat(void)
	{
		if( (m_uiFlags & DIRTY) != 0 )
		{
			if( GetParent() != NULL )
				m_WorldMat = m_LocalMat * ((EDFrame *)GetParent())->GetWorldMat();
			else
				m_WorldMat = m_LocalMat;

			m_uiFlags &= ~DIRTY;
		}

		return m_WorldMat;
	}
};

#endif