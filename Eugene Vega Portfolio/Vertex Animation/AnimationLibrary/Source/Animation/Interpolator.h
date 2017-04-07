#ifndef _INTERPOLATOR_H_
#define _INTERPOLATOR_H_
#include <Windows.h>
#include <vector>
#include <math.h>
#include <algorithm>

#include "Animation/Animation.h"
#include "Math/matrix4.h"

#undef min
#undef max

template < typename T >
class Interpolator
{
public:

    Interpolator( );
    virtual ~Interpolator( );

    void SetAnimation( const Animation< T >* animation );
    const Animation< T >* GetAnimation( ) const;

    void AddTime( float time );
    void SetTime( float time );
    float GetTime( ) const;

    virtual void Process( );

    const std::vector< T >& GetPose( ) const;

protected:

    void ProcessKeyframes( const std::vector< typename Animation< T >::KeyFrame >& key_frames );

    const Animation< T >* animation;
    float current_time;

    std::vector< T > pose;
};

template < typename T >
Interpolator< T >::Interpolator( )
{
    animation = 0;
    current_time = 0.0f;
}

template < typename T >
Interpolator< T >::~Interpolator( )
{
}

template < typename T >
void Interpolator< T >::SetAnimation( const Animation< T >* animation )
{
    // TODO
	
	//set the class member animation to the incoming animation, and set the current time to zero.
	this->animation = animation;
	current_time = 0.0f;
}

template < typename T >
const Animation< T >* Interpolator< T >::GetAnimation( ) const
{
    return animation;
}

template < typename T >
void Interpolator< T >::AddTime( float time )
{
    SetTime( current_time + time );
}

template < typename T >
void Interpolator< T >::SetTime( float time )
{
    // TODO
	if (animation == nullptr)
	{
		current_time = 0.0f;
	}
	else
	{
		current_time = time;
		if (current_time > animation->GetDuration())
		{
			current_time -= animation->GetDuration();
		}

		if (current_time < 0)
		{
			current_time += animation->GetDuration();
		}
	}
}

template < typename T >
float Interpolator< T >::GetTime( ) const
{
    return current_time;
}

template < typename T >
void Interpolator< T >::Process( )
{
    if ( animation == 0 )
    {
        pose.clear( );
        return;
    }

	ProcessKeyframes( animation->GetKeyFrames( ));
}

template < typename T >
const std::vector<T>& Interpolator< T >::GetPose( ) const
{
    return pose;
}

template < typename T >
void Interpolator< T >::ProcessKeyframes( const std::vector< typename Animation< T >::KeyFrame >& key_frames )
{
	int kfprev = 0;
	int kfnext = 1;

	while (true)
	{
		if (current_time < key_frames[kfnext].time && current_time > key_frames[kfprev].time)
			break;

		if (current_time < key_frames[kfprev].time)
		{
			kfprev = key_frames.size() - 1;
			kfnext = 0;
			break;
		}

		kfprev++;
		kfnext++;
	}

	float kfdur = 0.0f;
	float kfratio = 0.0f;

	if (kfprev < kfnext)
	{
		kfdur = key_frames[kfnext].time - key_frames[kfprev].time;
		kfratio = (current_time - key_frames[kfprev].time) / kfdur;
	}
	else
	{
		kfratio = current_time / key_frames[kfnext].time;
	}
	
	pose.resize(key_frames[0].nodes.size());

	for (unsigned int i = 0; i < pose.size(); i++)
	{
		pose[i] = key_frames[kfprev].nodes[i] + (key_frames[kfnext].nodes[i] - key_frames[kfprev].nodes[i]) * kfratio;
	}



}

#endif // _INTERPOLATOR_H_