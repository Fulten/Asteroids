#pragma once

#include <fstream>

class waveFileIn
{
private:
	// file being read
	
	short* samples;
	int nCurrentPos, nNumSamples;
	int nSampleRate, nNumChannel, nSampleSize;
	double maxSample;
	bool state;

	bool ReadFile(std::string fileName)
	{
		char cdata[4];
		int ndata = 0;
		int fileSize;
		int dataLength;
		std::string temp = "";
		nCurrentPos = 0;
		samples = nullptr;
		maxSample = (double)((short)pow(2, (sizeof(short) * 8) - 1) - 1);

		std::ifstream file;
		file.open(fileName, std::ios::binary);
		if (file.fail()) // break if unable to find file
			return false;

		file.read(cdata, 4); // read the riff header
		for (int i = 0; i < 4; i++)
		{ temp += cdata[i]; }

		if (temp != "RIFF") // break if file is incorrectly formated
			return false;

		file.read((char*)&fileSize, 4); // get the file size in bytes

		file.read(cdata, 4);
		temp = "";
		for (int i = 0; i < 4; i++)
		{temp += cdata[i];}

		if (temp != "WAVE")
			return false;

		file.seekg(22);
		file.read((char*)&nNumChannel, 2);
		file.read((char*)&nSampleRate, 4);
		file.seekg(34);
		file.read((char*)&nSampleSize, 2);
		file.seekg(40);
		file.read((char*)&dataLength, 4);
		
		nNumSamples = dataLength / 2;
		samples = new short[nNumSamples];
		file.seekg(44);

		// read the main bulk of data
		for (int i = 0; i < nNumSamples; i++)
		{
			file.read((char*)&ndata, 2);
			samples[i] = ndata;
		}

		return true;
	}

public:
	waveFileIn(std::string fileName)
	{
		state = ReadFile(fileName);
	}

	~waveFileIn()
	{
		delete[] samples;
	}

	short GetSample()
	{
		if (samples != nullptr)
		{
			short output = samples[nCurrentPos];

			nCurrentPos++;

			if (nCurrentPos >= nNumSamples - 100)
				nCurrentPos = 0;

			return output;
		}
		
		return 0.0;
	}

	double getRawSample()
	{
		return (double)GetSample() / maxSample;
	}

	void resetSample()
	{
		nCurrentPos = 0;
	}

	bool getStreamState()
	{
		return state;
	}
};