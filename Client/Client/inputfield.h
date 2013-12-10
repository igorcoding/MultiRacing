#ifndef INPUTFIELD_H
#define INPUTFIELD_H

#include <hge.h>
#include <hgegui.h>
#include <hgeguictrls.h>
#include "resourcemanager.h"

namespace NeonHockey
    {
    class InputField : public hgeGUIObject
    {
    public:
        InputField(ResourceManager* _rm, int _id, float xPos, float yPos, char* prevalue);
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
        ResourceManager* _rm;

        int caretposition;
        char* caret;
        bool caretVisible; //for blinking effect
        bool focused;
        float caretTimer; //for blinking effect
        const static int maxchar = 16;
        float xPos;
        float yPos;
        char *content;
    };
}

#endif // INPUTFIELD_H
