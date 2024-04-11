#include "PadInput.h"

int KeyInput::NowKey[KEY_MAX];
int KeyInput::OldKey[KEY_MAX];
char PadInput::NowKey[BUTTONS];
char PadInput::OldKey[BUTTONS];
XINPUT_STATE PadInput::Input;
Stick PadInput::Rstick;
Stick PadInput::Lstick;
CURSOR KeyInput::Cursor; 
int KeyInput::NowMouse[MOUSE_MAX];
int KeyInput::OldMouse[MOUSE_MAX];