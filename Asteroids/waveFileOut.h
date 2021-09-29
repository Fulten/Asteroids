#pragma once

// class can write synthesised sound waves to a file in the riff .wav format
template <class T>
class waveFileOut
{
private:
	std::ofstream file;
	double maxSample;
	bool fileOpen;
	int chunkDataPos;

	// makes clips sound to fall within a given range
	double clip(double sample, double Max)
	{
		if (sample >= 0.0)
			return fmin(sample, Max);
		else
			return fmax(sample, -Max);
	}

	void writeAsBinary(WORD input, short size)
	{
		file.write((char*)&input, size);
	}

public:
	waveFileOut(std::string fileName, UINT sampleRate = 44100, int channels = 1)
	{
		T nMaxSample = (T)pow(2, (sizeof(T) * 8) - 1) - 1;
		maxSample = (double)nMaxSample;
		fileOpen = true;

		// create file using supplied file name
		file.open(fileName, std::ios::binary);

		file << "RIFF----WAVEfmt "; // denotes file as riff .wav and creates placeholder for file size, 4 bytes
		writeAsBinary(16, 4); // extension data in this case none
		writeAsBinary(1, 2); // file format pcm
		writeAsBinary(channels, 2); // number of channels, for now just one
		writeAsBinary(sampleRate, 4); // sample rate, usally 44100
		writeAsBinary(sampleRate * sizeof(T) * channels, 4); // average bits persecond, usally 88200 or 176400
		writeAsBinary(sizeof(T) * channels, 2); // bytes per step
		writeAsBinary(sizeof(T) * 8, 2); // bits per sample, expected to be a multiple of 8
		file << "data----"; // preface before the main brunt of the file, temparary values are assigned to chucnk size
		chunkDataPos = file.tellp(); 
	}

	~waveFileOut()
	{
		int fileSize = file.tellp(); // note the file's end size
		file.seekp(40);
		writeAsBinary(fileSize - chunkDataPos + 8, 4); // correct sub chunk size
		file.seekp(4); // seek the file size parameter
		writeAsBinary(fileSize - 8, 4);
		file.close(); // close file
	}

	// accepts a single sample formated as a double expected to be within the bounds of 1.0 to -1.0 and converts it to binary data
	void writeRawSampleToFile(double sample)
	{
		if (fileOpen)
			writeAsBinary((int)(clip(sample, 1.0) * maxSample), sizeof(T));
	}

	// accepts a single sample already formated as type T and writes it as binary data
	void writeSampleToFile(T sample)
	{
		if (fileOpen)
			writeAsBinary(sample, sizeof(T));
	}
};