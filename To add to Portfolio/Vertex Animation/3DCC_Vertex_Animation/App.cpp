#include "App.h"

#include "Math/matrix4.h"
#include "Math/vec3.h"
#include "Loader/VertLoader.h"

void App::Initialize( )
{
    matrix4f& camera_matrix = m_CameraFrame.GetLocalMat( );

    camera_matrix.make_identity( );
    camera_matrix.axis_pos.y = 4.0f;
    camera_matrix.axis_pos.z = 8.0f;
    camera_matrix.axis_pos.x = 0.0f;

    camera_matrix.axis_z = vec3f( 0.0f, 0.0f, 0.0f ) - camera_matrix.axis_pos;
    camera_matrix.axis_z.normalize( );

    cross_product( camera_matrix.axis_x, vec3f( 0.0f, 1.0f, 0.0f ), camera_matrix.axis_z );
    camera_matrix.axis_x.normalize( );

    cross_product( camera_matrix.axis_y, camera_matrix.axis_z, camera_matrix.axis_x );
    camera_matrix.axis_y.normalize( );

    m_CameraFrame.Update( );

    m_Camera.SetFrame( &m_CameraFrame );

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
    glEnable(GL_CULL_FACE);
 	glEnableClientState(GL_VERTEX_ARRAY);

    // Load the animation
    VertLoader::Load( "..\\Assets\\monster.msh", m_Animation );

    // Attach the animation to the interpolator
	m_Interpolator.SetAnimation(&m_Animation);
	m_Reverse.SetAnimation(&m_Animation);
}

void App::Shutdown( )
{
}

void App::Update( )
{    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glPushMatrix();

        m_Camera.Update( 0.01f );
        m_Camera.ApplyCameraTransform( );

        glPushMatrix( );
        glTranslatef( -2.5f, 0.0f, 0.0f );

		// Add elapsed time to our interpolator
		m_Interpolator.AddTime(0.005f);
		// Interpolate our animation
		m_Interpolator.Process();

        if ( m_Interpolator.GetPose( ).size( ) > 0 )
        {
		    // Draw the interpolator's current state.
		    glVertexPointer(3, GL_FLOAT, 0, &m_Interpolator.GetPose( )[0]);
		    glColor4f(0.0f, 1.0f, 0.0f, 1.0f);
		    glDrawArrays(GL_TRIANGLES, 0, m_Interpolator.GetPose( ).size());
		    glDisable(GL_DEPTH_TEST);
			    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
				    glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
				    glDrawArrays(GL_TRIANGLES, 0, m_Interpolator.GetPose( ).size());
			    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);	
		    glEnable(GL_DEPTH_TEST);
        }

        glPopMatrix( );

		glPushMatrix();
		glTranslatef(2.5f, 0.0f, 0.0f);

		// Add elapsed time to our interpolator
		m_Reverse.AddTime(-0.005f);
		// Interpolate our animation
		m_Reverse.Process();

        if ( m_Reverse.GetPose( ).size( ) > 0 )
        {
		    // Draw the interpolator's current state.
		    glVertexPointer(3, GL_FLOAT, 0, &m_Reverse.GetPose( )[0]);
		    glColor4f(1.0f, 0.0f, 0.0f, 1.0f);
		    glDrawArrays(GL_TRIANGLES, 0, m_Reverse.GetPose( ).size());
		    glDisable(GL_DEPTH_TEST);
			    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
				    glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
				    glDrawArrays(GL_TRIANGLES, 0, m_Reverse.GetPose( ).size());
			    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		    glEnable(GL_DEPTH_TEST);
        }

		glPopMatrix();

		// Draws the ground.
		float width = 20.0f;
		float y = -5.0f;
		float step = 1.f;
		glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
		for(float x = -width; x <= width; x += step)
		{
			glBegin(GL_TRIANGLE_STRIP);
			for(float z = -width; z <= width; z += step)
			{
				glNormal3f(0.0f, 1.0f, 0.0);
				glVertex3f(x + step, y, z);
				glVertex3f(x, y, z);            
			}
			glEnd();
		}

    glPopMatrix();
}