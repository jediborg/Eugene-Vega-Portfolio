#ifndef _EDCAMERA_H_
#define _EDCAMERA_H_

#include "Utility/EDFrame.h"

#include "Math/vec4.h"

class EDCamera
{
	unsigned int m_uiViewDimWidth;
	unsigned int m_uiViewDimHeight;
	float m_fNear;
	float m_fFar;
	float m_fFov;

	matrix4f m_ProjectionMatrix;

	EDFrame *m_pCameraFrame;

	// Fog parameters
	unsigned int m_uiFogMode;
	float m_fFogDensity;
	float m_fFogStart;
	float m_fFogEnd;
	unsigned int m_uiFogIndex;
	vec4f m_vFogColor;
	vec4f m_vClearColor;
	
	bool m_bShowCursor;
public:
	// Constructor
	EDCamera(void) : m_vFogColor( 0.0f, 0.0f, 0.0f, 0.0f ), m_vClearColor( 0.0f, 0.0f, 0.0f, 1.0f )
	{
		m_pCameraFrame = 0;

		m_uiFogMode = 0;
		m_fFogDensity = 1.0f;
		m_fFogStart = 0.0f;
		m_fFogEnd = 1.0f;
		m_uiFogIndex = 0;

		m_fNear = 0.0f;
		m_fFar = 0.0f;
		m_fFov = 0.0f;
		m_bShowCursor = true;
	}

	// Destructor
	virtual ~EDCamera(void){}

	EDFrame *GetFrame(void){ return m_pCameraFrame; }
	void SetFrame( EDFrame *pFrame )
	{ 
		m_pCameraFrame = pFrame;
	}

	void SetProjectionPerspective(float fovy, unsigned int viewDimWidth, unsigned int viewDimHeight, float zNear, float zFar);

	void SetProjectionOrthographic(float left, float right, float bottom, float top, float zNear = -1.0f , float zFar = 1.0f );

	const matrix4f &GetProjectionMatrix(void) const { return m_ProjectionMatrix; }

	matrix4f &GetModelViewMatrix( matrix4f &matrix );

	unsigned int GetFogMode(void){ return m_uiFogMode; }
	void SetFogMode( unsigned int uiFogMode ){ m_uiFogMode = uiFogMode; }
	
	float GetFogDensity(void){ return m_fFogDensity; }
	void SetFogDensity( float fFogDensity ){ m_fFogDensity = fFogDensity; }

	float GetFogStart(void){ return m_fFogStart; }
	void SetFogStart( float fFogStart ){ m_fFogStart = fFogStart; }

	float GetFogEnd(void){ return m_fFogEnd; }
	void SetFogEnd( float fFogEnd ){ m_fFogEnd = fFogEnd; }

	unsigned int GetFogIndex(void){ return m_uiFogIndex; }
	void SetFogIndex( unsigned int uiFogIndex ){ m_uiFogIndex = uiFogIndex; }

	const vec4f &GetFogColor(void) const { return m_vFogColor; }
	void SetFogColor( const vec4f &vFogColor ){ m_vFogColor = vFogColor; }

	const vec4f &GetClearColor(void) const { return m_vClearColor; }
	void SetClearColor( const vec4f &vClearColor ){ m_vClearColor = vClearColor; }

	unsigned int GetViewWidth(void){ return m_uiViewDimWidth; }
	void SetViewWidth( unsigned int uiWidth ){ m_uiViewDimWidth = uiWidth; }

	unsigned int GetViewHeight(void){ return m_uiViewDimHeight; }
	void SetViewHeight( unsigned int uiHeight ){ m_uiViewDimHeight = uiHeight; }

	float GetFOV(void){ return m_fFov; }
	void SetFOV( float fFov ){ m_fFov = fFov; }

	float GetNear(void){ return m_fNear; }
	void SetNear( float fNear ){ m_fNear = fNear; }

	float GetFar(void){ return m_fFar; }
	void SetFar( float fFar ){ m_fFar = fFar; }

	void Update(float fTime);
	void ApplyCameraTransform(void);
};

#endif
