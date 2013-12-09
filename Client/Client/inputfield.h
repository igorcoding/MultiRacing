#ifndef INPUTFIELD_H
#define INPUTFIELD_H

#include <hge.h>
#include <hgegui.h>
#include <hgeguictrls.h>

class InputField : public hgeGUIObject{

public:
    InputField(hgeFont* _fnt, int _id, float xPos, float yPos,
        float length, char* prevalue);
    ~InputField();
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
};
#endif // INPUTFIELD_H
