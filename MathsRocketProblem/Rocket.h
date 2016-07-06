//Rocket Class header, Written by Kris Stevenson
#ifndef ROCKET
#define ROCKET

#include <hge.h>			//Include HG engine header
#include <hgesprite.h>		//Include texture header
#include <hgeanim.h>		//include texture animation header
#include <hgefont.h>		//Include text header
#include <math.h>			//Include mathlib

extern HGE *hge;			//External pointer to the HGE interface.

//Default equation values. Used to show successful landing of the rocket
const double defaultAltitude			= 6344;
const double defaultVelocity			= 350;
const double defaultExhaustSpeed		= 1200;
const double defaultMass				= 100;
const double defaultGravity				= 1.3;
const double defaultFuelConsumptionRate	= 0.8235208; //alpha


class Rocket
{
private:

	//-------------------
	//Rendering variables
	//-------------------
	hgeSprite		*mSprite;	//Holds the image of the rocket
	hgeSprite		*mThruster;	//holds the image of the thruster
	hgeSprite		*mGround;	//Holds the image of the floor
	hgeSprite		*mSky;		//Holds the image of the sky
	hgeSprite		*mSidebar;	//Holds the image of the sidebar
	hgeAnimation	*mFire;		//Holds the fire animation

	double mRocketPoint;	//Holds the rocket's current location (top left)
	double mCurrentMass;	//Holds the rocket's current mass (including fuel)
	double mFuelRemaining;	//Holds the rocket's remaining fuel (percentage)
	double mScaleFactor;	//Holds value to apply scaling to the rockets altitude/location
	
	//------------------
	//Equation variables - Equation used: V = -gt -U +C*log(M/(M-(alpha*t)
	//------------------
	double mAltitude;			// H
	double mVelocity;			// V
	double mInitialVelocity;	// U
	double mThrust;				// C
	double mMass;				// M
	double mGravity;			// g
	double mAlpha;				// alpha
	float  mTime;				// t
	
	//------------------
	//Sound variables
	//------------------
	HEFFECT		sndCrash;		//Sound effect for rocket crash (Velocity >= 10M/s)
	HEFFECT		sndYay;			//Sound effect for successful landing (Velocity < 10M/s)
	HEFFECT		sndThruster;	//Sound effect for rocket thrust
	HCHANNEL	soundChannel;	//Sound channel, used to alter sounds/use various effects
	


public:
	void Init(double altitude, double velocity, double thrust, double mass, double alpha, double gravity);
	bool Update();
	void Render();
	void RenderBackground();
	void RenderCurrentValues();	
	~Rocket();

};

#endif
//EOF Rocket.h