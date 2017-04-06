#include <windows.h>
#include <gl/gl.h>

#include "Utility/EDCamera.h"

void EDCamera::SetProjectionPerspective(float fovy, unsigned int viewDimWidth, unsigned int viewDimHeight, float zNear, float zFar)
{
	float fAspect = float(viewDimWidth)/float(viewDimHeight);
	float f = 1.0f / tan(fovy/2.0f);

	m_uiViewDimWidth = viewDimWidth;
	m_uiViewDimHeight = viewDimHeight;

	m_fNear = zNear;
	m_fFar = zFar;
	m_fFov = fovy;

	m_ProjectionMatrix.axis_x.x = f / fAspect;
	m_ProjectionMatrix.axis_x.y = 0.0f;
	m_ProjectionMatrix.axis_x.z = 0.0f;
	m_ProjectionMatrix.pad_xw = 0.0f;

	m_ProjectionMatrix.axis_y.x = 0.0f;
	m_ProjectionMatrix.axis_y.y = f;
	m_ProjectionMatrix.axis_y.z = 0.0f;	
	m_ProjectionMatrix.pad_yw = 0.0f;

	m_ProjectionMatrix.axis_z.x = 0.0f;
	m_ProjectionMatrix.axis_z.y = 0.0f;
	m_ProjectionMatrix.axis_z.z = (zFar+zNear)/(zNear-zFar);
	m_ProjectionMatrix.pad_zw = -1.0f;

	m_ProjectionMatrix.axis_w.x = 0.0f;
	m_ProjectionMatrix.axis_w.y = 0.0f;
	m_ProjectionMatrix.axis_w.z = (2.0f*zFar*zNear)/(zNear-zFar);
	m_ProjectionMatrix.pad_ww = 0.0f;
}

void EDCamera::SetProjectionOrthographic(float left, float right, float bottom, float top, float zNear, float zFar)
{
	m_ProjectionMatrix.axis_x.x = 2.0f / (right - left);
	m_ProjectionMatrix.axis_x.y = 0.0f;
	m_ProjectionMatrix.axis_x.z = 0.0f;
	m_ProjectionMatrix.pad_xw = 0.0f;

	m_ProjectionMatrix.axis_y.x = 0.0f;
	m_ProjectionMatrix.axis_y.y = 2.0f / (top - bottom);
	m_ProjectionMatrix.axis_y.z = 0.0f;
	m_ProjectionMatrix.pad_yw = 0.0f;

	m_ProjectionMatrix.axis_z.x = 0.0f;
	m_ProjectionMatrix.axis_z.y = 0.0f;
	m_ProjectionMatrix.axis_z.z = -2.0f / (zFar - zNear);
	m_ProjectionMatrix.pad_zw = 0.0f;

	m_ProjectionMatrix.axis_w.x = -( (right+left)/(right-left) );
	m_ProjectionMatrix.axis_w.y = -( (top+bottom)/(top-bottom) );
	m_ProjectionMatrix.axis_w.z = -( (zFar+zNear)/(zFar-zNear) );
	m_ProjectionMatrix.pad_ww = 1.0f;
}

void EDCamera::Update(float fTime)
{
	SHORT KeyState = GetAsyncKeyState( VK_RBUTTON );

	if( (KeyState & 0x0001) != 0 )
	{
		SetCursorPos( (GetViewWidth() / 2), (GetViewHeight() / 2) );
		m_bShowCursor = false;
		ShowCursor(false);
	}
	else if( KeyState != 0 )
	{
		if( this->GetFrame() != 0 )
		{
			POINT mousePos;
			GetCursorPos( &mousePos );
			SetCursorPos( (GetViewWidth() / 2), (GetViewHeight() / 2) );

			int x = GetViewWidth() / 2;
			int y = GetViewHeight() / 2;

			float mouseDiff[2] = { float(x - mousePos.x), float(mousePos.y - y) };

			GetFrame()->GetLocalMat().rotate_x_pre( mouseDiff[1] * fTime );
			GetFrame()->GetLocalMat().rotate_y_pre( mouseDiff[0] * fTime );

			cross_product( GetFrame()->GetLocalMat().axis_x, vec3f( 0.0f, 1.0f, 0.0f ), GetFrame()->GetLocalMat().axis_z );
			GetFrame()->GetLocalMat().axis_x.normalize();
			cross_product( GetFrame()->GetLocalMat().axis_y, GetFrame()->GetLocalMat().axis_z, GetFrame()->GetLocalMat().axis_x );
			GetFrame()->GetLocalMat().axis_y.normalize();
			
			if( GetAsyncKeyState( VK_UP ) )
				GetFrame()->GetLocalMat().axis_pos += GetFrame()->GetLocalMat().axis_z * fTime;
			if( GetAsyncKeyState( VK_DOWN ) )
				GetFrame()->GetLocalMat().axis_pos -= GetFrame()->GetLocalMat().axis_z * fTime;
			if( GetAsyncKeyState( VK_LEFT ) )
				GetFrame()->GetLocalMat().axis_pos += GetFrame()->GetLocalMat().axis_x * fTime;
			if( GetAsyncKeyState( VK_RIGHT ) )
				GetFrame()->GetLocalMat().axis_pos -= GetFrame()->GetLocalMat().axis_x * fTime;

			GetFrame()->Update();
		}
	}
	else if( m_bShowCursor == false )
	{
		m_bShowCursor = true;
		ShowCursor(true);
	}
}

void EDCamera::ApplyCameraTransform(void)
{
	if( this->GetFrame() != 0 )
	{
		matrix4f tempMat( this->GetFrame()->GetWorldMat() );
		tempMat.axis_z.negate();
		tempMat.axis_x.negate();

		matrix4f invCamMat;

		invCamMat = tempMat;
		invCamMat.transpose();
		invCamMat.pad_xw = 0.0f;
		invCamMat.pad_yw = 0.0f;
		invCamMat.pad_zw = 0.0f;
		invCamMat.pad_ww = 1.0f;

		invCamMat.axis_pos.x = -dot_product( tempMat.axis_pos, tempMat.axis_x );
		invCamMat.axis_pos.y = -dot_product( tempMat.axis_pos, tempMat.axis_y );
		invCamMat.axis_pos.z = -dot_product( tempMat.axis_pos, tempMat.axis_z );

		glMultMatrixf( invCamMat.ma );
	}
}

matrix4f &EDCamera::GetModelViewMatrix(matrix4f &matrix)
{
	if( this->GetFrame() != 0 )
	{
		matrix4f tempMat;

		tempMat = this->GetFrame()->GetWorldMat();
		tempMat.axis_z.negate();
		tempMat.axis_x.negate();

		matrix = tempMat;
		matrix.transpose();
		matrix.pad_xw = 0.0f;
		matrix.pad_yw = 0.0f;
		matrix.pad_zw = 0.0f;
		matrix.pad_ww = 1.0f;

		matrix.axis_pos.x = -dot_product( tempMat.axis_pos, tempMat.axis_x );
		matrix.axis_pos.y = -dot_product( tempMat.axis_pos, tempMat.axis_y );
		matrix.axis_pos.z = -dot_product( tempMat.axis_pos, tempMat.axis_z );
	}

	return matrix;
}