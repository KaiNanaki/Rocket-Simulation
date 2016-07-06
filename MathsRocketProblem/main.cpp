// Kris Stevenson
// Mathematics for Computer Games (Year 2) - Rocket Landing Simulation

// Our assignment was a rocket landing problem where we had to determine the altitude that the 
// rocket thruster needed to fire in order to touchdown at a speed that would not destroy the 
// rocket on impact.
// This program was part of the assignment and is used to show a successful landing at the 
// altitude determined in our equation from the Mathematics class. Extra functionality was added
// outside of our spec so that equation values could be changed.


// Please note: 
// I wrote the Rocket class and the main code file for this project.
// The 'Forminput' class was mainly written by the other member of my group on this project (DP). 
// I have moved some of this code to make it more modular, as initially the majority of it was
// placed in the update/render functions ('FrameFunc()' and 'RenderFunc()' respectively) and used 
// a lot of globals, which made the main functions look rather bloated, untidy, and hard to read. 
// The code was moved out of this file to Forminput and was edited by myself slighty to ensure 
// functionality of the program.

#include <hge.h>			//Include HG engine header
#include <hgegui.h>			//Include GUI header

#include "Forminput.h"		//Include Forminput header, used to check/change equation values
#include "Rocket.h"			//Include Rocket clas header, used to draw/update the rocket


HGE		*hge = NULL;		//Global pointer to the engine
hgeGUI	*gui;				//Global pointer to the GUI class
hgeFont	*fnt;				//Global pointer to the Font class

Rocket	theRocket;			//Create the rocket

bool 	isActive;			//Holds if the simulation is active
float	mouseX, mouseY;		//Holds coordinates for the mouse cursor position within the window


//Forminput globals
bool	errorMessage;		//Holds if an error needs to be displayed

double	formAltitude;		//Holds Altitude value in the form boxes	
double	formVelocity;		//Holds Velocity value in the form boxes
double	formExhaustSpeed;	//Holds Thrust speed value in the form boxes
double	formMass;			//Holds Mass value in the form boxes
double	formFuelConsRate;	//Holds alpha/Fuel Consumption Rate value in the form boxes
double	formGravity;		//Holds Gravity value in the form boxes


bool FrameFunc()
{//Update frame function
	hge->Input_GetMousePos(&mouseX, &mouseY);   //Get the current mouse position 

	gui->Update(0);	//Updates the GUI controls and processes user input.
	
	//If simulation has been actived, update the rocket.
	if(isActive)
	{
		//If the rocket has touched down, theRocket.update() will return false, setting isActive to false
		//and halting the simulation.
		isActive = theRocket.Update();
	}

	//Reset with current values
	if(hge->Input_GetKeyState(HGEK_R))
	{
		//Resets rocket to initial position with current input values when the 'R' key is pressed.
		//Current values are checked to see if they are valid
		if(FormReset())
		{
			//If values in form are valid, activate simulation and initialise rocket with form values
			//Set active to false to halt simulation, initialise rocket with current form values
			isActive = false;
			theRocket.Init(formAltitude, 
				formVelocity, 
				formExhaustSpeed, 
				formMass, 
				formFuelConsRate, 
				formGravity);

			//Set errorMessage to false if form input values are valid
			errorMessage = false;
		}
		//If values in form are not valid, sets errorMessage bool to true so that this may be displayed
		//in the render function.
		else errorMessage = true;
	}

	//Reset to Default Values
	if(hge->Input_GetKeyState(HGEK_D))
	{
		//Updates input values to their defaults, resets the rocket with these default values, 
		//deactivates the rocket simulation
		FormDefault();

		//initialise rocket
		theRocket.Init(formAltitude, 
				formVelocity, 
				formExhaustSpeed, 
				formMass, 
				formFuelConsRate, 
				formGravity);

		//stop simulation
		isActive = false;
	}

	//Activate simulation (if not already active)
	if (hge->Input_GetKeyState(HGEK_SPACE) && !isActive)
	{
		//If spacebar is pressed and the rocket is ready, start the simulation
		if(CheckForm())
		{
			//Check if the values in the form have been updated correctly,
			//If so set active to true, allowing the rocket simulation to update
			isActive = true;

			//initialise the rocket
			theRocket.Init(formAltitude, 
				formVelocity, 
				formExhaustSpeed, 
				formMass, 
				formFuelConsRate, 
				formGravity);

			//Set errorMessage to false if form input values are valid
			errorMessage = false;
		}
		//If values in form are not valid, sets errorMessage bool to true so that this may be displayed
		//in the render function.
		else errorMessage = true;
	}

	if (hge->Input_GetKeyState(HGEK_ESCAPE)) return true;	//Exit the program with 'Esc' key
	return false;


}

bool RenderFunc()
{//Main draw function

	hge->Gfx_BeginScene();			//Starts rendering graphics.
	hge->Gfx_Clear(0);				//Clears render target and z-buffer.
	
	theRocket.RenderBackground();	//Render background images
	theRocket.RenderCurrentValues();//Render current equation values on the sidebar
	
	theRocket.Render();				//render the rocket
	gui->Render();					//render label boxes and their values

	if(errorMessage)
	{//If an error is found, display error message until it is corrected
		fnt->Render(562,300, HGETEXT_CENTER, "Incorrect input");
	}

	//render the current mouse position (used for testing)
	//fnt->printf(800, 575, HGETEXT_RIGHT, "Mouse location: %.2f, %.2f", mouseX, mouseY);

	hge->Gfx_EndScene();				//Stops rendering graphics.

	return false;
}

void InitFunc()
{//Initialisation function
	isActive		= false;						//Set simulation to be inactive
	errorMessage	= false;						//Set error message for label boxes to false

	fnt = new hgeFont("font.fnt", false);			//Set up Font pointer
	
	gui = new hgeGUI();								//Set up GUI pointer

	CreateInputBoxes();								//Set up input boxes
	

	//Set form values to defaults
	formAltitude		= defaultAltitude;
	formVelocity		= defaultVelocity;
	formExhaustSpeed	= defaultExhaustSpeed;
	formMass			= defaultMass;
	formFuelConsRate	= defaultFuelConsumptionRate;
	formGravity			= defaultGravity;


	//Initialise the rocket equation values
	theRocket.Init(formAltitude, 
				formVelocity, 
				formExhaustSpeed, 
				formMass, 
				formFuelConsRate, 
				formGravity);

}

void CleanupFunc()
{//Clean Up Function

	fnt		= NULL;
	delete	fnt;
	gui		= NULL;
	delete	gui;

	//hge pointer is released at the end of the program.

}

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{//Windows Desktop Apllication Function
	hge = hgeCreate(HGE_VERSION);

	hge->System_SetState(HGE_FRAMEFUNC, FrameFunc);
	hge->System_SetState(HGE_RENDERFUNC, RenderFunc);
	hge->System_SetState(HGE_TITLE,
		"Kris Stevenson - Rocket Landing Simulation");

	hge->System_SetState(HGE_HIDEMOUSE, false);

	hge->System_SetState(HGE_FPS, 60);
	hge->System_SetState(HGE_WINDOWED, true);
	hge->System_SetState(HGE_SCREENWIDTH, 800);
	hge->System_SetState(HGE_SCREENHEIGHT, 600);
	hge->System_SetState(HGE_SCREENBPP, 32);

	if(hge->System_Initiate())
	{
		InitFunc();
		hge->System_Start();
	}
	else
	{	
		MessageBox(NULL, hge->System_GetErrorMessage(), "Error",
			MB_OK | MB_ICONERROR | MB_APPLMODAL);
	}
	CleanupFunc();
	hge->System_Shutdown();
	hge->Release();

	return 0;
}

//EOF main.cpp