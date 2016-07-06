#ifndef FORMINPUT
#define FORMINPUT
//Modification of a class taken from http://relishgames.com/forum/viewtopic.php?t=598

//Modified/written by: DP

//Features added:
// # Floating point number interpretation
// # Removed the ability to input text, and only allow numbers and decimal points

#include <hge.h>
#include <hgegui.h>
#include <hgefont.h>
#include <hgesprite.h>
#include <iostream>
#include <sstream>
#include <string>
#include <stdexcept>
#include <stdlib.h>

#include "rocket.h"
//#include "convert.h"


extern double formAltitude;
extern double formVelocity;
extern double formExhaustSpeed;
extern double formMass;
extern double formFuelConsRate;
extern double formGravity;

double to_double ( const char *p );
bool stringToDouble(char* tempString, double& toConvert );
inline char* convertDoubleToCString(double d);


void CreateInputBoxes();
bool FormReset();
void FormDefault();
bool CheckForm();


//Small Input textfield 
class Forminput : public hgeGUIObject{ 

public: 
	Forminput(hgeFont* _fnt, int _id, float xPos, float yPos, 
		float length, char* prevalue); 
	~Forminput(); 
	virtual void Render(); 
	//void Render(float x, float y); 
	virtual bool MouseLButton(bool bDown); 
	virtual void Focus(bool bFocused); 
	virtual bool KeyClick(int key, int chr); 
	virtual void Update(float fDeltaTime); 
	void setText(char *newText); 
	char *getText(); 


protected: 
	int caretposition; 
	char* caret; 
	bool caretVisible; //for blinking effect 
	bool focused; 
	float caretTimer; //for blinking effect 
	const static int maxchar = 30; 
	float xPos; 
	float yPos; 
	hgeFont *font; 
	float length; 
	char *content; 
	hgeSprite* mSprite;
}; 


#endif