#include "VertLoader.h"

#include <fstream>

#include "Animation/Animation.h"
#include "Math/vec3.h"
using namespace std;

bool VertLoader::Load( const std::string& file_name, Animation< vec3f >& animation )
{
    // TODO
	
	fstream myFile;
	myFile.open(file_name, ios_base::binary | ios_base::in);
	
	int kfNumber = 0;
	std::vector<Animation<vec3f>::KeyFrame>& KfAnim = animation.GetKeyFrames();

	if (myFile.is_open())
	{
		myFile.read((char*)&kfNumber, sizeof(kfNumber));
		KfAnim.resize(kfNumber);
		for (int i = 0; i < kfNumber; i++)
		{
			float time = 0.0f;
			myFile.read((char*)&time, sizeof(time));
			KfAnim[i].time = time;

			unsigned int nCount = 0;
			myFile.read((char*)&nCount, sizeof(time));
			KfAnim[i].nodes.resize(nCount);

			for (int j = 0; j < nCount; j++)
			{
				vec3f cNode;
				myFile.read((char*)&cNode, sizeof(cNode));
				animation.GetKeyFrames()[i].nodes.push_back(cNode);
			}
		}
		myFile.close();
	}
	
    return true;
}