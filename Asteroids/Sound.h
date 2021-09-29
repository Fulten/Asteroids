#pragma once

#include "SoundOut.h"
#include "waveFileIn.h"

struct envelop
{
	double attack;
	double release;
	double start;
	double end;
	double active;
	bool playing;

	envelop()
	{
		attack = 0.1;
		release = 0.2;
		start = 0.0;
		end = 0.0;
		active = false;
		playing = false;
	}

	virtual double getAmplitude(double dTime) = 0;
};

struct Thrust : public envelop
{
	Thrust()
	{
		attack = 0.1;
		release = 0.2;
		start = 0.0;
		end = 0.0;
	}

	double getAmplitude(double dTime) override
	{
		double amplitude = 0.0;
		double releaseAmplitude = 0.0;

		if (active)
		{
			double dLifeTime = dTime - start;

			if (dLifeTime <= attack) // attack phase
				amplitude = (dLifeTime / attack) * 1.0;

			if (dLifeTime > (attack))
				amplitude = 1.0;
		}
		else
		{
			double dLifeTime = end - start;
			if (dLifeTime <= attack) // handle a stop in the middle of the attack
				releaseAmplitude = (dLifeTime / attack) * 1.0;

			if (dLifeTime > attack)
				releaseAmplitude = 1.0;

			amplitude = ((dTime - end) / release) * (0.0 - releaseAmplitude) + releaseAmplitude;
		}

		if (amplitude <= 0.0001)
			amplitude = 0.0;

		return amplitude;
	}
};

struct Expo : public envelop
{
	Expo()
	{
		attack = 0.01;
		release = 0.5;
		start = 0.0;
		end = 0.0;
	}

	double getAmplitude(double dTime) override
	{
		double amplitude = 0.0;
		double releaseAmplitude = 0.0;

		if (active)
		{
			double dLifeTime = dTime - start;

			if (dLifeTime <= attack) // attack phase
				amplitude = (dLifeTime / attack) * 1.0;

			if (dLifeTime > (attack))
			{
				amplitude = 1.0;
				active = false;
				end = dTime;
			}
		}
		else
		{
			double dLifeTime = end - start;
			if (dLifeTime <= attack) // handle a stop in the middle of the attack
				releaseAmplitude = (dLifeTime / attack) * 1.0;

			if (dLifeTime > attack)
				releaseAmplitude = 1.0;

			amplitude = ((dTime - end) / release) * (0.0 - releaseAmplitude) + releaseAmplitude;
		}

		if (amplitude <= 0.0001)
			amplitude = 0.0;

		return amplitude;
	}
};

struct Bul : public envelop
{
	Bul()
	{
		attack = 0.01;
		release = 0.2;
		start = 0.0;
		end = 0.0;
	}

	double getAmplitude(double dTime) override
	{
		double amplitude = 0.0;
		double releaseAmplitude = 0.0;

		if (active)
		{
			double dLifeTime = dTime - start;

			if (dLifeTime <= attack) // attack phase
				amplitude = (dLifeTime / attack) * 1.0;

			if (dLifeTime > (attack))
			{
				amplitude = 1.0;
				active = false;
				end = dTime;
			}
		}
		else
		{
			double dLifeTime = end - start;
			if (dLifeTime <= attack) // handle a stop in the middle of the attack
				releaseAmplitude = (dLifeTime / attack) * 1.0;

			if (dLifeTime > attack)
				releaseAmplitude = 1.0;

			amplitude = ((dTime - end) / release) * (0.0 - releaseAmplitude) + releaseAmplitude;
		}

		if (amplitude <= 0.0001)
			amplitude = 0.0;

		return amplitude;
	}
};

class Sound : public SoundOut
{
private:

	static waveFileIn* thrusterSound;
	static waveFileIn* explosionSound;
	static waveFileIn* bulletSound;
	static bool started; 

	static Thrust thrusterEnv;
	static Expo explosionEnv;
	static Bul bulletEnv;

	// converts hertz to angular velocity
	static double w(double dHertz)
	{
		return dHertz * 3.141569 * 2.0;
	}

	static double MakeWave(double dTime)
	{
		double freq = 0.0;
		if (started)
		{
			freq += 1.0 * thrusterSound->getRawSample() * thrusterEnv.getAmplitude(dTime);
			freq += 1.0 * bulletSound->getRawSample() * bulletEnv.getAmplitude(dTime);
			freq += 1.0 * explosionSound->getRawSample() * explosionEnv.getAmplitude(dTime);
		}
			
		return freq * 0.50;
	}

	
public:
	// constructor initilizes the waveOutDevice
	Sound() :
		SoundOut(0, 32000, 1, 8, 512)
	{
		setFunction(MakeWave);
		thrusterSound = new waveFileIn("Thruster.wav");
		bulletSound = new waveFileIn("Blaster.wav");
		explosionSound = new waveFileIn("Explosion.wav");

		started = true;
	}

	// destructor stops the main sound thread
	~Sound()
	{
		started = false;
		SoundOut::stop();
		//thrusterSound->~waveFileIn();
		//bulletSound->~waveFileIn();
		//thrusterSound->~waveFileIn();
	}

	void thruster(bool state) // used to control the thruster's noise
	{
		if (started)
			if (state)
			{ 
				if (!thrusterEnv.active)
				{
					thrusterEnv.start = getTime();
					thrusterEnv.active = true;
				}
			}
			else
			{
				if (thrusterEnv.active)
				{
					thrusterEnv.end = getTime();
					thrusterEnv.active = false;
				}
			}
	}

	void blaster() // triggers a blaster sound
	{
		if (started)
		{
			bulletEnv.start = getTime();
			bulletEnv.active = true;
			bulletSound->resetSample();
		}
	}

	void explosion() // triggers an explosion sound
	{
		if (started)
		{
			explosionEnv.start = getTime();
			explosionEnv.active = true;
			explosionSound->resetSample();
		}
	}
};
