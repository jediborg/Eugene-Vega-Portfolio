#ifndef _VERT_LOADER_H_
#define _VERT_LOADER_H_

#include <string>

template < typename T >
class Animation;

template < typename T >
struct vec3;
typedef vec3<float> vec3f;

class VertLoader
{
public:

    static bool Load( const std::string& file_name, Animation< vec3f >& animation );
};

#endif // _VERT_LOADER_H_