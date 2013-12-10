#include "inputfield.h"
#include <hgesprite.h>
#include <hgefont.h>


namespace NeonHockey
    {
    //TODO: rewrite this completely
    InputField::InputField(NeonHockey::ResourceManager* rm, int _id, float _xPos, float _yPos, const char *prevalue)
        : hgeGUIObject()
    {
        _rm = rm;

        id = _id;
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
        rect.Set(xPos, yPos, xPos+256, yPos+32);
    }

    InputField::~InputField(){

    }

    void InputField::Render(){
        //render background
        hgeSprite *background = _rm->getSprite(GfxType::INPUTFIELD);
        background->Render(xPos - 10, yPos - 4);

        hgeFont *font = _rm->getFont(FontType::SCORE);

        font->Render(xPos, yPos, HGETEXT_LEFT, content);

        //Renders the caret
        if (focused && caretVisible){
            float width = font->GetStringWidth(content);
            font->Render(xPos+width, yPos, HGETEXT_LEFT, caret);
        }
    }

    //Gets Focus with LeftMouseKlick
    bool InputField::MouseLButton(bool bDown){
        if (bDown){
            return false;
        }
        gui->SetFocus(id);
        return false;
    }

    //Saving focus state in focused
    void InputField::Focus(bool bFocused){
        if (!bFocused){
            focused = false;
        }
        else{
            focused = true;
        }

    }

    void InputField::Update(float fDeltaTime){
        caretTimer += fDeltaTime;
        /*Makes the caret blinking effect
          You might want to change the value below for slower/faster blinking*/
        if (caretTimer >= 0.1f){
            caretVisible = !caretVisible;
            caretTimer = 0.0f;
        }
    }

    bool InputField::KeyClick(int key, int chr){
        //Loses Focus with Enter
        if (key == HGEK_ENTER){
            //TODO: gui object seems to be not initialized
            //gui->SetFocus(-1);
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
        else if (chr != 0 && caretposition < maxchar-1){
            tmp[caretposition] = chr;
            tmp[caretposition+1] = '\0'; //add overwritten string terminator
            //Assures that the String will not take to much place
            //if (font->GetStringWidth(tmp) <= length){
                strcpy(content, tmp); //Store back
                caretposition+=1;
            //}
        }
        return false;
    }

    //Setting the Text
    void InputField::setText(char *newtext){
        strncpy(content ,newtext ,maxchar);
        content = newtext;
    }

    char *InputField::getText(){
        return content;
    }
}
