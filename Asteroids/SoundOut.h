#pragma once

// library for sound api
#pragma comment (lib, "Winmm.lib")

// standard packages
#include <iostream>
#include <cmath>
#include <fstream>
#include <vector>
#include <string>

// packages for multi threading
#include <thread>
#include <atomic>
#include <condition_variable>

// windows api
#include <Windows.h>
#include "waveFileOut.h"

class SoundOut
{
private:
	double(*userFunction)(double);
	// waveFileOut allows the proccess to create a .wav file as output
	waveFileOut<short>* outputFile;

	// data structs used for containing the audio device and sound buffer
	HWAVEOUT hwDevice;
	WAVEHDR* waveHeaders;
	short* nBlockMemory;

	// atomic variables needed to safely utilize the sound thread
	std::thread soundThread;
	std::atomic<double> dGlobalTime;
	std::atomic<bool> bReady;
	std::atomic<bool> rawData;
	std::condition_variable cvBlocksFree;
	std::mutex muxBlocksFree;

	// variables store data about the type of sound wave
	UINT nDeviceID, nChannels, nSampleRate;

	// variables store data about the sound buffer
	UINT nBlockCount, nBlockSamples, nBlockCurrent;
	std::atomic<UINT> nBlocksFree;

	bool create(UINT in_nDeviceID, DWORD in_nSampleRate = 44100, WORD in_nChannels = 1, UINT in_nBlockCount = 8, UINT in_nBlockSamples = 512)
	{
		// initilize variables
		nDeviceID = in_nDeviceID;
		nSampleRate = in_nSampleRate;
		nChannels = in_nChannels;
		nBlockCount = in_nBlockCount;
		nBlockSamples = in_nBlockSamples;
		nBlockCurrent = 0;
		nBlocksFree = nBlockCount;
		bReady = false;
		rawData = true;

		// set output file to null
		outputFile = nullptr;

		// set memory buffer and user function pointers to null
		nBlockMemory = nullptr;
		waveHeaders = nullptr;
		userFunction = nullptr;

		// validate that the given device id exists within the range of existing devices
		if (nDeviceID < 0 || nDeviceID > waveOutGetNumDevs() - 1)
		{
			throw "error audio Device out of bounds";
		}
		else
		{
			// create a waveFormat Stuct
			WAVEFORMATEX waveFormat;
			waveFormat.wFormatTag = WAVE_FORMAT_PCM;
			waveFormat.nChannels = nChannels;
			waveFormat.nSamplesPerSec = nSampleRate;
			waveFormat.wBitsPerSample = sizeof(short) * 8;
			waveFormat.nBlockAlign = (waveFormat.wBitsPerSample / 8) * waveFormat.nChannels;
			waveFormat.nAvgBytesPerSec = nSampleRate * waveFormat.nBlockAlign;
			waveFormat.cbSize = 0;

			// create a waveout device to comunicate with the sound card
			if (waveOutOpen(&hwDevice, nDeviceID, &waveFormat, (DWORD_PTR)waveOutProcW, (DWORD_PTR)this, CALLBACK_FUNCTION) != S_OK)
			{
				throw waveOutOpen(&hwDevice, nDeviceID, &waveFormat, (DWORD_PTR)waveOutProcW, (DWORD_PTR)this, CALLBACK_FUNCTION);
				return false;
			}
				

			// allocate memory for the sound buffer and clear it of prexsiting data
			nBlockMemory = new short[nBlockCount * nBlockSamples];
			if (nBlockMemory == nullptr)
			{
				throw "error: creating data buffer";
				return false;
			}
				
			ZeroMemory(nBlockMemory, sizeof(short) * nBlockCount * nBlockSamples);

			waveHeaders = new WAVEHDR[nBlockCount];
			if (waveHeaders == nullptr)
			{
				throw "error: creating data buffer";
				return false;
			}
				
			ZeroMemory(waveHeaders, sizeof(WAVEHDR) * nBlockCount);
			
			//link waveHeaders to nBlockMemory to simplify writing data to the buffer
			for (UINT i = 0; i < nBlockCount; i++)
			{
				waveHeaders[i].dwBufferLength = nBlockSamples * sizeof(short);
				waveHeaders[i].lpData = (LPSTR)(nBlockMemory + (i * nBlockSamples));
			}
			
			bReady = true; // set the main thread state to ready

			//start the main sound thread
			soundThread = std::thread(&SoundOut::MainThread, this);

			// start the sound thread
			std::unique_lock<std::mutex> lm(muxBlocksFree);
			cvBlocksFree.notify_one();

			return true;
		}
	}

	// wrapper for waveOutProc
	static void CALLBACK waveOutProcW(HWAVEOUT hwo, UINT uMsg, DWORD_PTR dwInstance, DWORD_PTR dwParam1, DWORD_PTR dwParam2)
	{
		((SoundOut*)dwInstance)->waveOutProc(hwo, uMsg, dwInstance, dwParam1, dwParam2);
	}

	// when the sound card finishes processing data it will send a notice by calling this function, needs to unpause the sound thread
	void waveOutProc(HWAVEOUT hwo, UINT uMsg, DWORD_PTR dwInstance, DWORD_PTR dwParam1, DWORD_PTR dwParam2)
	{
		if (uMsg != WOM_DONE) return; // if uMsg == WOM_DONE that means the sound card has finished using a data block

		nBlocksFree++;
		std::unique_lock<std::mutex> lm(muxBlocksFree);
		cvBlocksFree.notify_one();
	}

	// main sound thread handles translateing data from the user's function into the sound buffer then sending it out
	void MainThread()
	{
		//initilize global time
		dGlobalTime = 0.0;
		double dTimeStep = 1.0 / (double)nSampleRate; // initilize a time step based on the expected sample rate of the sythesiser

		// find the maximum value of a given type
		short nMaxSample = (short)pow(2, (sizeof(short) * 8) - 1) - 1;
		double dMaxSample = (double)nMaxSample;


		while (bReady)
		{
			// if no blocks are free pause the thread untill one is made avalible
			if (nBlocksFree == 0)
			{
				std::unique_lock<std::mutex> lm(muxBlocksFree);
				cvBlocksFree.wait(lm);
			}

			// a block is a bout to be written to
			nBlocksFree--;

			// unprepare used blocks
			if (waveHeaders[nBlockCurrent].dwFlags & WHDR_PREPARED)
				waveOutUnprepareHeader(hwDevice, &waveHeaders[nBlockCurrent], sizeof(WAVEHDR));


			short newSample = 0;

			// translate the current block location to samples 
			UINT currentBlock = nBlockCurrent * nBlockSamples;

			for (UINT i = 0; i < nBlockSamples; i++)
			{
				if(rawData) // if the user is inputing unformated data format it to be used with the sound card
					if (userFunction == nullptr) // if the user has not provided a function use a default process instead
						newSample = (short)(clip(userProcess(dGlobalTime), 1.0)* dMaxSample);
					else 
						newSample = (short)(clip(userFunction(dGlobalTime), 1.0)* dMaxSample);
				else// if data is marked as formated, use it as is
					newSample = (short)userFunction(dGlobalTime);

				nBlockMemory[currentBlock + i] = newSample; //write the newly created sample to the buffer
				dGlobalTime = dGlobalTime + dTimeStep; // increment the global time step forward one unit

				// if currently writing out to file add the new sample to the output
				if(outputFile != nullptr)
					outputFile->writeSampleToFile(newSample);

			}

			//prepare a block of data to be output to the audio buffer
			waveOutPrepareHeader(hwDevice, &waveHeaders[nBlockCurrent], sizeof(WAVEHDR));
			waveOutWrite(hwDevice, &waveHeaders[nBlockCurrent], sizeof(WAVEHDR));

			// increment to the next block of the buffer
			nBlockCurrent++;
			nBlockCurrent %= nBlockCount;
		}
	}

	double userProcess(double dTime)
	{
		return 0.0;
	}

public:

	// constructor needs to initilize the sound buffer, open up a device for audio streaming and start a thread for audio
	SoundOut(UINT in_nDeviceID, DWORD in_nSampleRate, WORD in_nChannels, UINT in_nBlockCount, UINT in_nBlockSamples)
	{
		create(in_nDeviceID, in_nSampleRate, in_nChannels, in_nBlockCount, in_nBlockSamples);
	}

	~SoundOut()
	{

	}

	static std::vector<std::wstring> getDevices()
	{
		// get the number of devices
		int deviceCount = waveOutGetNumDevs();
		std::vector<std::wstring> output;

		for (int i = 0; i < deviceCount; i++)
		{
			WAVEOUTCAPS temp;
			// retrive device information and store it into temp
			waveOutGetDevCaps(i, &temp, sizeof(temp));

			// push device identifier to output
			output.push_back(temp.szPname);
		}

		return output;
	}

	// sets the function to be used to find samples
	void setFunction(double(*func)(double) )
	{
		userFunction = func;
	}

	// stops the sound thread and waits for it to rejoin the main thread
	void stop()
	{
		bReady = false;
		soundThread.join();
		if (outputFile != nullptr)
			outputFile->~waveFileOut();

		waveOutClose(hwDevice);
	}
	
	//returns the current global time
	double getTime()
	{
		return dGlobalTime;
	}

	// takes a value and constrains it within a given range from 0
	double clip(double sample, double Max)
	{
		if (sample >= 0.0)
			return fmin(sample, Max);
		else
			return fmax(sample, -Max);
	}

	// creates a new waveFileOut object to write audio to
	void startFileOut(std::string fileName)
	{
		// only create a new object if no current outputfile is in use
		if (outputFile == nullptr)
			outputFile = new waveFileOut<short>(fileName, nSampleRate);
		else
			throw "attempting to open multiple output files";
	}

	// stops writing data to file out object
	void stopFileOut()
	{
		if (outputFile != nullptr)
			outputFile->~waveFileOut();
		else
			return;

		outputFile = nullptr;
	}

	void isFormated()
	{
		rawData = false;
	}

};
