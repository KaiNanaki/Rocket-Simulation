//Rocket Class code, Written by Kris Stevenson
#include "Rocket.h"					//Include Rocket header

const int	theGround		= 580;	//Location to place the ground (y-axis)
const int	rocketHeight	= 70;	//Height of rocket/image
const int	rocketWidth		= 40;	//Width of rocket/image

extern bool	isActive;				//Is the rocket simulation active or not
extern hgeFont *fnt;				//External pointer to the global font class



//************************************
// Method:    Init
// FullName:  Rocket::Init
// Access:    public 
// Returns:   void
//************************************
void Rocket::Init(double altitude, double velocity, double thrust, double mass, double alpha, double gravity)
{//Set rocket equation values with the values passed to the function
	mAltitude			= altitude;   
	mVelocity			= velocity;  
	mInitialVelocity	= velocity;
	mThrust				= thrust;     
	mMass				= mass;  
	mCurrentMass		= mass;
	mGravity			= gravity;
	mAlpha				= alpha;
	mFuelRemaining		= (mass/mass)*100;		//shown as %
	mTime				= 0;					//initialize time to zero

	//mScaleFactor used to condense animation/landing in to the window size from altitude set
	mScaleFactor			= mAltitude/510;	


	//load textures for the rocket and the background
	HTEXTURE tex1 = hge->Texture_Load("rocket2.png");
	mSprite		= new hgeSprite(tex1, 0, 0,(float)rocketWidth,(float)rocketHeight);
	HTEXTURE tex2 = hge->Texture_Load("thruster.png");
	mThruster	= new hgeSprite(tex2, 0, 0, 40, 30);
	HTEXTURE tex3 = hge->Texture_Load("ground.png");
	mGround		= new hgeSprite(tex3, 0, 0, 800, 20);
	HTEXTURE tex4 = hge->Texture_Load("sky.png");	
	mSky		= new hgeSprite(tex4, 0, 0, 800, 600);
	HTEXTURE tex5 = hge->Texture_Load("sidebar.png");
	mSidebar	= new hgeSprite(tex5, 0.0f, 0.0f, 330.0f, 600.0f);


	//Load animation texture for flames
	HTEXTURE tex6 = hge->Texture_Load("fire1Comp.png");
	mFire		= new hgeAnimation(tex6, 13, 13, 64, 64, 64, 64);
	mFire->Play();								//Set animation to play as soon as it is rendered


	//Load sound effects
	sndCrash = hge->Effect_Load("Crash.wav");
	sndYay = hge->Effect_Load("Yay.mp3");
	sndThruster = hge->Effect_Load("Thruster2.wav");


	if (hge->Channel_IsPlaying(soundChannel))
	{//If thruster sound is playing (because the rocket was reset), set volume to 0 and stop it
		hge->Channel_SetVolume(soundChannel, 0);
		hge->Channel_Stop(soundChannel);
	}
}



//************************************
// Method:    Update
// FullName:  Rocket::Update
// Access:    public 
// Returns:   bool
//************************************
bool Rocket::Update()
{
	//Add frame time to mTime (for use in equation)
	mTime += hge->Timer_GetDelta();

	if (mRocketPoint < theGround)
	{//BURN - Updates rocket position/animation with the velocity equation if the rocket is
	 //above the ground
	
		if ((mAlpha*mTime)<(mMass*0.7))
		{	//If there is fuel remaining...
			//Calculate velocity with our completed equation: 
			//V = -gt -U +C*log(M/(M-(alpha*time)
			//Where:
			//mVelocity;		= V
			//mInitialVelocity	= U
			//mThrust;			= C
			//mMass;			= M
			//mGravity;			= g
			//mAlpha			= alpha (fuel consumption rate)
			//mTime;			= t

			mVelocity = -1*((-mGravity * mTime) - mInitialVelocity + mThrust * 
				log (mMass/(mMass-(mAlpha * mTime))));
		
			//Update mass/fuel values so they can be displayed on screen
			mCurrentMass	= mMass -(mAlpha * mTime);
			mFuelRemaining	= mMass - (((mAlpha * mTime)/70)*100);

			if (!hge->Channel_IsPlaying(soundChannel))
			{//If in burn phase, play Thruster sound effect
			 //The sound channel is used so that the sound effect does not start to play each frame
				soundChannel = hge->Effect_PlayEx(sndThruster, 100, 0, 1, false);
				hge->Channel_SetVolume(soundChannel, 100);
			}
			
		}
		else if ((mAlpha*mTime)>=(mMass*0.7))
		{//If there is no fuel remaining...
		 //Stop thrusting and calculate velocity with: v = u + at
			mVelocity -= ((-mGravity * hge->Timer_GetDelta()));
			
			mCurrentMass = mMass*0.3;	//Fuel depleted, 30% of total initial mass remains		
			mThrust = 0.0;				//Fuel depleted, no thrust, set thrust to 0

			if (hge->Channel_IsPlaying(soundChannel))
			{//As thrust is 0, stop thruster sound
				hge->Channel_SetVolume(soundChannel, 0);
				hge->Channel_Stop(soundChannel);
			}
		}

		//Update the altitude with with rocket velocity each frame 
		mAltitude -= mVelocity*hge->Timer_GetDelta();

		//Set the rocket point on screen relative to the ground 
		//(altitude scaled to condense animation/landing in to the window size,
		//altitude value remains unchanged)
		mRocketPoint = theGround - (mAltitude/mScaleFactor);

	}
	else if (mRocketPoint >= theGround)
	{//LANDED - Stops rocket simulation when rocket touches down, Equation values
	 //will freeze at the point of touchdown

		//Rocket has landed, altitude equals zero
		mAltitude = 0.00;

		if (hge->Channel_IsPlaying(soundChannel))
		{//Stop thruster sound upon touchdown
			hge->Channel_SetVolume(soundChannel, 0);
			hge->Channel_Pause(soundChannel);
		}

		if (mVelocity >= 10)
		{//If velocity is greater than/equal to than 10m/s - hard landing
		 //Play crash sound effect
			hge->Effect_Play(sndCrash);//, 100, 0, 1, false);
		}
		else
		{//Else, velocity must be less than 10m/s - soft landing
		 //Play successful landing sound effect
			hge->Effect_PlayEx(sndYay, 100, 0, 1, false);
		}

		//Return false to stop the simulation
		return false;
	}
	//continue simulation
	return true;
	
}



//************************************
// Method:    Render
// FullName:  Rocket::Render
// Access:    public 
// Returns:   void
//************************************
void Rocket::Render()
{
	//Set rocket at scaled point above the ground
	mRocketPoint = theGround - (mAltitude/mScaleFactor);

	//Draw rocket on screen at its current point (ground - altitude/scale)
	mSprite->RenderEx(562-(rocketWidth/2),(float(mRocketPoint-rocketHeight)),0,1,1);


	if (mThrust > 0.0 && isActive)
	{//If simulation is active and the rocket is thrusting, draw thrust image at base of rocket
		mThruster->Render(562-(rocketWidth/2), mRocketPoint);
	}
	else if (mThrust == 0.0 && isActive)
	{//If rocket is not thrusting but simulation is still active, display "Burnout!" message
		fnt->Render(562, 300, HGETEXT_CENTER, "Burnout!");
	}

	if (mAltitude == 0)
	{//If rocket has landed
		fnt->Render(562, 330, HGETEXT_CENTER, "Rocket has touched down");
		if (mVelocity >= 10)
		{//If velocity is greater than/equal to 10m/s - hard landing
		 //Draw Hard landing message and fire animation.
			fnt->Render(562, 360, HGETEXT_CENTER, "Hard landing!");
			mFire->RenderStretch(562-(rocketWidth), mRocketPoint-rocketHeight, 562+rocketWidth, mRocketPoint+5);
		}
		else if (mVelocity < 10)
		{//If less than 10m/s - soft landing
		 //Draw soft landing message
			fnt->Render(562, 360, HGETEXT_CENTER, "Soft landing");
		}
	}

	if (mFire->IsPlaying())
	{//Update fire animation if it is playing/being rendered.
		mFire->Update(hge->Timer_GetDelta());
	}

}



//************************************
// Method:    RenderBackground
// FullName:  Rocket::RenderBackground
// Access:    public 
// Returns:   void
//************************************
void Rocket::RenderBackground()
{
	//Draw the sky
	mSky->Render(0,0);	

	//Draw the ground
	mGround->Render(0,(float)theGround);

	//Draw the sidebar
	mSidebar->Render(0,0);

}



//************************************
// Method:    Render
// FullName:  Rocket::Render
// Access:    public 
// Returns:   void
//************************************
void Rocket::RenderCurrentValues()
{
	int placement	= 25;	//Used for placing labels onscreen
	int spacing		= 30;	//Used for spacing labels onscreen

	int placement2	= 375;	//Used for placing real time values onscreen
	int spacing2	= 20;	//Used for spacing real time values onscreen


	//Display Labels on the side bar, uses placement and spacing values
	fnt->printf(5, 0, HGETEXT_LEFT, "Input values:");						//Heading
	fnt->printf(5, placement, HGETEXT_LEFT,  "Altitude (H)" );				//Altitude
	fnt->printf(5, placement+(spacing*1), HGETEXT_LEFT,  "Velocity (v)" );	//Velocity
	fnt->printf(5, placement+(spacing*2), HGETEXT_LEFT,  "Thrust (C)" );	//Thrust
	fnt->printf(5, placement+(spacing*3), HGETEXT_LEFT,  "Mass (M)" );		//Mass
	fnt->printf(5, placement+(spacing*4), HGETEXT_LEFT, "Alpha (a)" );		//Alpha Value
	fnt->printf(5, placement+(spacing*5), HGETEXT_LEFT, "Gravity (g*)" );	//Gravity

	//Display key legend on the side bar
	fnt->printf(5, placement+(spacing*7), HGETEXT_LEFT, "Keys:");
	fnt->printf(5, placement+(spacing*8), HGETEXT_LEFT, "SPACE - Launch!");
	fnt->printf(5, placement+(spacing*9), HGETEXT_LEFT, "R - Reset");
	fnt->printf(5, placement+(spacing*10), HGETEXT_LEFT, "D - Reset to Default");


	//Display equation values on the side bar in real time, uses placement2 and spacing values
	fnt->printf(5, placement2, HGETEXT_LEFT, "Current values");						//Heading

	fnt->printf(5, placement2+(spacing2*1), HGETEXT_LEFT, "Time:");					//Time
	fnt->printf(120, placement2+(spacing2*1), HGETEXT_LEFT, "%.2f s", mTime);
	
	fnt->printf(5, placement2+(spacing2*2), HGETEXT_LEFT, "Altitude:");				//Altitude
	fnt->printf(120, placement2+(spacing2*2), HGETEXT_LEFT, "%.2f M", mAltitude);
	
	fnt->printf(5, placement2+(spacing2*3), HGETEXT_LEFT, "Velocity:");				//Velocity
	fnt->printf(120, placement2+(spacing2*3), HGETEXT_LEFT, "%.2f M/s", mVelocity);

	fnt->printf(5, placement2+(spacing2*4), HGETEXT_LEFT, "Thrust:");				//Thrust
	fnt->printf(120, placement2+(spacing2*4), HGETEXT_LEFT, "%.2f NM", mThrust);
	
	fnt->printf(5, placement2+(spacing2*5), HGETEXT_LEFT, "Mass:");					//Mass
	fnt->printf(120, placement2+(spacing2*5), HGETEXT_LEFT, "%.2f N", mCurrentMass);
	
	fnt->printf(5, placement2+(spacing2*6), HGETEXT_LEFT, "Alpha:");				//Alpha value
	fnt->printf(120, placement2+(spacing2*6), HGETEXT_LEFT, "%.2f", mAlpha );
	
	fnt->printf(5, placement2+(spacing2*7), HGETEXT_LEFT, "Gravity:");				//Gravity
	fnt->printf(120, placement2+(spacing2*7), HGETEXT_LEFT, "%.2f M/s", mGravity);

	fnt->printf(5, placement2+(spacing2*9), HGETEXT_LEFT,							//Fuel level 
		"Fuel remaining: %.2f %%", mFuelRemaining);

	fnt->printf(5, placement2+(spacing2*10), HGETEXT_LEFT,							//Scaling info
		"1 pixel equals: %.2f Meters", mScaleFactor);

}


Rocket::~Rocket()
{//clean up textures and sounds

	hge->Effect_Free(sndThruster);
	hge->Effect_Free(sndYay);
	hge->Effect_Free(sndCrash);

	delete mFire;
	delete mSidebar;
	delete mSky;
	delete mGround;
	delete mThruster;
	delete mSprite;
	
}

//EOF Rocket.cpp
