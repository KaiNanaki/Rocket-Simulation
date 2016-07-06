#include "Forminput.h"

extern hgeGUI	*gui;			//Global GUI object
extern hgeFont	*fnt;			//holds font object



double to_double ( const char *p )
{
	std::stringstream ss ( p );
	double result = 0;

	ss>> result;

	return result;
}

bool stringToDouble(char* tempString, double& toConvert )
{
	bool floatFlag = true;
	bool decimalFound = false;
	int i = 0;

	while(tempString[i] != '\0' && floatFlag == true)
	{
		if(tempString[i] == '0' ||
			tempString[i] == '1' ||
			tempString[i] == '2' ||
			tempString[i] == '3' ||
			tempString[i] == '4' ||
			tempString[i] == '5' ||
			tempString[i] == '6' ||
			tempString[i] == '7' ||
			tempString[i] == '8' ||
			tempString[i] == '9' ||
			tempString[i] == '.')
		{

		}
		else
		{
			floatFlag = false;
			return false;
		}
		if(tempString[i] == '.' && decimalFound == true)
		{
			floatFlag = false;
		}
		else if(tempString[i] == '.') 
			decimalFound = true;

		i++;


	}

	if(floatFlag = true)
	{
		toConvert = to_double(tempString);
	}

	return floatFlag;
}

char* convertDoubleToCString(double d)
{
	//CString DoubleString;
	char* NumChar = new char[32];
	int Decimal = 0;
	int Sign = 0;

	//Convert the double to a string and store it in NumChar
	
	_fcvt_s(NumChar, 10, d, 2, &Decimal, &Sign);

	//Add a null terminator on the end
	strcat(NumChar, "\0");

	//Create a new character array for the Decimal part
	char* DecimalChar = new char[11];

	//char DecimalChar[11];

	//Fill the characters with spaces
	for(int i = 0; i < 11; i++)
	{
		DecimalChar[i] = ' ';
	}
	
	DecimalChar[0] = '\0';

	int charCounter = 0;

	while(NumChar[charCounter+Decimal]!='\0')
	{
		DecimalChar[charCounter] = NumChar[charCounter+Decimal];
		charCounter++;
	}
	DecimalChar[charCounter] = '\0';

	NumChar[Decimal] = '.';
	NumChar[Decimal+1] = '\0';

	strcat(NumChar,DecimalChar);
	strcat(NumChar, "\0");


	delete[] DecimalChar;

	return NumChar;
}



//Small Input textfield 
Forminput::Forminput(hgeFont* _fnt, int _id, float _xPos, float _yPos, 
						  float _length, char *prevalue)
{ 
	 font = _fnt; 
	 id = _id; 
	 length = _length; 
	 xPos = _xPos; 
	 yPos = _yPos; 
	 bStatic = false; 
	 bVisible = true; 
	 bEnabled = true; 
	 caretVisible = true; 
	 focused = false; 
	 caretTimer = 0.0f; 
	 caret = "_"; 
	 content = new char[maxchar+1]; //maxchar is defined in the header 
	 strncpy(content, prevalue, maxchar); 
	 int prevalue_length = (int)strlen(prevalue);

	 if (prevalue_length < maxchar){ 
		 caretposition = prevalue_length; 
	 } 
	 else{ 
		 caretposition = maxchar-1; 
	 } 
	 content[caretposition] = '\0'; 
	 content[maxchar+1] = '\0'; //Last char will always be a string terminator 
	 rect.Set(xPos, yPos, xPos+length, yPos+(font->GetHeight())); 

	 HTEXTURE boxText = hge->Texture_Load("box.png") ;
	 mSprite = new hgeSprite(boxText, 0, 0,(float)200,(float)font->GetHeight());



}; 
Forminput::~Forminput()
{ 
	delete font; 
} 

void Forminput::Render()
{ 
	//hge->Texture_Create(Texture)
	//	DWORD TexData = hge->Texture_Lock(Texture) ;
	//(Fiddle about here)
	//	hge->Texture_Unlock(Texture) ;

	//hgeSprite tehSprite = new hgeSprite(Texture, 0, 0, rect.w, rect.h) ;

	mSprite->RenderEx(rect.x1-5,rect.y1,0,1,1);

	

	font->Render(xPos, yPos, HGETEXT_LEFT, content); 
	//Renders the caret 
	if (focused && caretVisible){ 
		float width = font->GetStringWidth(content); 
		font->Render(xPos+width, yPos, HGETEXT_LEFT, caret); 
	}
} 

//Gets Focus with LeftMouseKlick 
bool Forminput::MouseLButton(bool bDown)
{ 
	if (bDown){ 
		return false; 
	} 
	gui->SetFocus(id); 
	return false; 
}; 

//Saving focus state in focused 
void Forminput::Focus(bool bFocused)
{ 
	if (!bFocused){ 
		focused = false; 
	} 
	else{ 
		focused = true; 
	} 

}; 

void Forminput::Update(float fDeltaTime)
{ 
	caretTimer += fDeltaTime; 
	/*Makes the caret blinking effect 
	change the value below for slower/faster blinking*/ 
	if (caretTimer >= 0.1f){ 
		caretVisible = !caretVisible; 
		caretTimer = 0.0f; 
	} 
} 

bool Forminput::KeyClick(int key, int chr)
{ 
	//Loses Focus with Enter 
	if (key == HGEK_ENTER){ 
		gui->SetFocus(-1); 
		return false; 
	} 
	char tmp[maxchar+1]; 
	strncpy(tmp, content, maxchar+1); 
	if (key == HGEK_BACKSPACE){ //Delete one character per backspace 
		if (caretposition != 0){ 
			caretposition -=1; 
			tmp[caretposition] = '\0'; //Put string terminator one character back 
		} 
		strcpy(content, tmp); 
		return false; 
	} 
	/*When chr==0 no ascii-key (e.g. shift) is pressed and chr would 
	be saved as string terminator. The other thing to look after, is 
	whether we have max. characters*/ 
	if (chr != 0 && caretposition < maxchar-1){ 
		tmp[caretposition] = chr; 
		tmp[caretposition+1] = '\0'; //add overwritten string terminator 
		//Assures that the String will not take to much place 
		if (font->GetStringWidth(tmp) <= length){ 
			strcpy(content, tmp); //Store back 
			caretposition+=1; 
		} 
	} 
	return false; 
}; 

//Setting the Text 
void Forminput::setText(char *newtext)
{ 
	strncpy(content ,newtext ,maxchar); 
	content = newtext; 
}; 

char *Forminput::getText()
{ 
	return content; 
}
void CreateInputBoxes()
{
	gui->AddCtrl(new Forminput(fnt, 1, 125.0f, 25.0f,  200.0f,  convertDoubleToCString(defaultAltitude)));
	gui->AddCtrl(new Forminput(fnt, 2, 125.0f, 55.0f,  200.0f,  convertDoubleToCString(defaultVelocity)));
	gui->AddCtrl(new Forminput(fnt, 3, 125.0f, 85.0f,  200.0f,  convertDoubleToCString(defaultExhaustSpeed)));
	gui->AddCtrl(new Forminput(fnt, 4, 125.0f, 115.0f, 200.0f,  convertDoubleToCString(defaultMass)));
	gui->AddCtrl(new Forminput(fnt, 5, 125.0f, 145.0f, 200.0f,  convertDoubleToCString(defaultFuelConsumptionRate)));
	gui->AddCtrl(new Forminput(fnt, 6, 125.0f, 175.0f, 200.0f,  convertDoubleToCString(defaultGravity)));

}
bool FormReset()
{
	    bool everythingIsOK = true;

		Forminput* tempForminput;

		tempForminput = static_cast<Forminput*>(gui->GetCtrl(1));
		char* tempString = tempForminput->getText();

		if(!stringToDouble(tempString, formAltitude))
			everythingIsOK = false;

		tempForminput = static_cast<Forminput*>(gui->GetCtrl(2));
		tempString = tempForminput->getText();

		if(!stringToDouble(tempString, formVelocity))
			everythingIsOK = false;

		tempForminput = static_cast<Forminput*>(gui->GetCtrl(3));
		tempString = tempForminput->getText();

		if(!stringToDouble(tempString, formExhaustSpeed))
			everythingIsOK = false;

		tempForminput = static_cast<Forminput*>(gui->GetCtrl(4));
		tempString = tempForminput->getText();

		if(!stringToDouble(tempString, formMass))
			everythingIsOK = false;

		tempForminput = static_cast<Forminput*>(gui->GetCtrl(5));
		tempString = tempForminput->getText();

		if(!stringToDouble(tempString, formFuelConsRate))
			everythingIsOK = false;

		tempForminput = static_cast<Forminput*>(gui->GetCtrl(6));
		tempString = tempForminput->getText();

		if(!stringToDouble(tempString, formGravity))
			everythingIsOK = false;

		if(everythingIsOK)
		{
			return true;
		}
		else
		{
			return false;
		}

}

void FormDefault()
{
		formAltitude			= defaultAltitude;
		formVelocity			= defaultVelocity;
		formExhaustSpeed		= defaultExhaustSpeed;
		formMass				= defaultMass;
		formFuelConsRate	= defaultFuelConsumptionRate;
		formGravity				= defaultGravity;

		static_cast<Forminput*>(gui->GetCtrl(1))->setText(convertDoubleToCString(formAltitude));
		static_cast<Forminput*>(gui->GetCtrl(2))->setText(convertDoubleToCString(formVelocity));
		static_cast<Forminput*>(gui->GetCtrl(3))->setText(convertDoubleToCString(formExhaustSpeed));
		static_cast<Forminput*>(gui->GetCtrl(4))->setText(convertDoubleToCString(formMass));
		static_cast<Forminput*>(gui->GetCtrl(5))->setText(convertDoubleToCString(formFuelConsRate));
		static_cast<Forminput*>(gui->GetCtrl(6))->setText(convertDoubleToCString(formGravity));
}

bool CheckForm()
{
	    bool everythingIsOK = true;

		Forminput* tempForminput;

		tempForminput = static_cast<Forminput*>(gui->GetCtrl(1));
		char* tempString = tempForminput->getText();

		if(!stringToDouble(tempString, formAltitude))
			everythingIsOK = false;

		tempForminput = static_cast<Forminput*>(gui->GetCtrl(2));
		tempString = tempForminput->getText();

		if(!stringToDouble(tempString, formVelocity))
			everythingIsOK = false;

		tempForminput = static_cast<Forminput*>(gui->GetCtrl(3));
		tempString = tempForminput->getText();

		if(!stringToDouble(tempString, formExhaustSpeed))
			everythingIsOK = false;

		tempForminput = static_cast<Forminput*>(gui->GetCtrl(4));
		tempString = tempForminput->getText();

		if(!stringToDouble(tempString, formMass))
			everythingIsOK = false;

		tempForminput = static_cast<Forminput*>(gui->GetCtrl(5));
		tempString = tempForminput->getText();

		if(!stringToDouble(tempString, formFuelConsRate))
			everythingIsOK = false;

		tempForminput = static_cast<Forminput*>(gui->GetCtrl(6));
		tempString = tempForminput->getText();

		if(!stringToDouble(tempString, formGravity))
			everythingIsOK = false;


		if(everythingIsOK)
		{
			return true;
		}
		else
		{
			return false;
		}

}
