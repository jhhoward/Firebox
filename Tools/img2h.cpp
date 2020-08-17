#include <iostream>
#include <fstream>
#include <vector>
#include "lodepng.cpp"

using namespace std;

unsigned char palette[] = 
{
	0,	0,	0,
	0xff,	0xff,	0xff
};

unsigned char matchColour(unsigned char r, unsigned char g, unsigned char b)
{
	unsigned char closest = 0;
	int closestDistance = -1;
	
	for(int n = 0; n < 2; n++)
	{
		int distance = (r - palette[n * 3]) * (r - palette[n * 3])
					+ (g - palette[n * 3 + 1]) * (g - palette[n * 3 + 1])
					+ (b - palette[n * 3 + 2]) * (b - palette[n * 3 + 2]);
		if(closestDistance == -1 || distance < closestDistance)
		{
			closestDistance = distance;
			closest = (unsigned char) (n);
		}
	}
	
	return closest;
}

void writeImageData(ofstream& output, const char* filename, const char* varName)
{
	unsigned int width, height;
	vector<unsigned char> image;
	unsigned error = lodepng::decode(image, width, height, filename);
	
	if(!error)
	{
		output << "// " << filename << " " << width << "x" << height << endl;
		output << "const unsigned char " << varName << "[] PROGMEM = {" << endl;
		
		output << width << ", " << height << ",		// Dimensions" << endl;
		
		for(int y = 0; y < height; y++)
		{
			for(int x = 0; x < width; x++)
			{
				if(image[(y * width + x) * 4 + 3] == 0)
				{
					output << 255 << ",";
				}
				else
				{
					int colour = matchColour(
						image[(y * width + x) * 4],
						image[(y * width + x) * 4 + 1],
						image[(y * width + x) * 4 + 2]);
					output << colour << ",";
				}
			}
		}
		
		output << endl;
		output << "};" << endl;
		
	}	
}

int main()
{
	ofstream output;
	output.open("../Firebox/images.h");
	
	writeImageData(output, "../Images/glasses.png", "glassesImageData");
	writeImageData(output, "../Images/log.png", "logImageData");
	writeImageData(output, "../Images/arduboy.png", "arduboyImageData");
	writeImageData(output, "../Images/face.png", "faceImageData");
	writeImageData(output, "../Images/battery.png", "batteryImageData");
	writeImageData(output, "../Images/mouse.png", "mouseImageData");
	writeImageData(output, "../Images/paper.png", "paperImageData");
	writeImageData(output, "../Images/bomb.png", "bombImageData");
	writeImageData(output, "../Images/fireball.png", "fireballImageData");
	
	output.close();
	
	return 0;
}

