#include "Keyboard.h"

void Keyboard::onEvent(SDL_Event *event)
{
  SDL_Scancode &scancode = event->key.scancode;

  isLGuiDown = scancode == (int)Key::LGUI;
  isLAltDown = scancode == (int)Key::LALT;
  isLCtrlDown = scancode == (int)Key::LCTRL;
  isLShiftDown = scancode == (int)Key::LSHIFT;

  isRGuiDown = scancode == (int)Key::RGUI;
  isRAltDown = scancode == (int)Key::RALT;
  isRCtrlDown = scancode == (int)Key::RCTRL;
  isRShiftDown = scancode == (int)Key::RSHIFT;

  if (event->type == SDL_EVENT_KEY_UP) {
    pressed = Key::NONE;
    return;
  }

  switch (scancode)
  {
  case (int)Key::A:
    pressed = Key::A;
    break;
  case (int)Key::B:
    pressed = Key::B;
    break;
  case (int)Key::C:
    pressed = Key::C;
    break;
  case (int)Key::D:
    pressed = Key::D;
    break;
  case (int)Key::E:
    pressed = Key::E;
    break;
  case (int)Key::F:
    pressed = Key::F;
    break;
  case (int)Key::G:
    pressed = Key::G;
    break;
  case (int)Key::H:
    pressed = Key::H;
    break;
  case (int)Key::I:
    pressed = Key::I;
    break;
  case (int)Key::J:
    pressed = Key::J;
    break;
  case (int)Key::K:
    pressed = Key::K;
    break;
  case (int)Key::L:
    pressed = Key::L;
    break;
  case (int)Key::M:
    pressed = Key::M;
    break;
  case (int)Key::N:
    pressed = Key::N;
    break;
  case (int)Key::O:
    pressed = Key::O;
    break;
  case (int)Key::P:
    pressed = Key::P;
    break;
  case (int)Key::Q:
    pressed = Key::Q;
    break;
  case (int)Key::R:
    pressed = Key::R;
    break;
  case (int)Key::S:
    pressed = Key::S;
    break;
  case (int)Key::T:
    pressed = Key::T;
    break;
  case (int)Key::U:
    pressed = Key::U;
    break;
  case (int)Key::V:
    pressed = Key::V;
    break;
  case (int)Key::W:
    pressed = Key::W;
    break;
  case (int)Key::X:
    pressed = Key::X;
    break;
  case (int)Key::Y:
    pressed = Key::Y;
    break;
  case (int)Key::Z:
    pressed = Key::Z;
    break;

  // Numbers
  case (int)Key::ONE:
    pressed = Key::ONE;
    break;
  case (int)Key::TWO:
    pressed = Key::TWO;
    break;
  case (int)Key::THREE:
    pressed = Key::THREE;
    break;
  case (int)Key::FOUR:
    pressed = Key::FOUR;
    break;
  case (int)Key::FIVE:
    pressed = Key::FIVE;
    break;
  case (int)Key::SIX:
    pressed = Key::SIX;
    break;
  case (int)Key::SEVEN:
    pressed = Key::SEVEN;
    break;
  case (int)Key::EIGHT:
    pressed = Key::EIGHT;
    break;
  case (int)Key::NINE:
    pressed = Key::NINE;
    break;
  case (int)Key::ZERO:
    pressed = Key::ZERO;
    break;

  // Function Keys
  case (int)Key::F1:
    pressed = Key::F1;
    break;
  case (int)Key::F2:
    pressed = Key::F2;
    break;
  case (int)Key::F3:
    pressed = Key::F3;
    break;
  case (int)Key::F4:
    pressed = Key::F4;
    break;
  case (int)Key::F5:
    pressed = Key::F5;
    break;
  case (int)Key::F6:
    pressed = Key::F6;
    break;
  case (int)Key::F7:
    pressed = Key::F7;
    break;
  case (int)Key::F8:
    pressed = Key::F8;
    break;
  case (int)Key::F9:
    pressed = Key::F9;
    break;
  case (int)Key::F10:
    pressed = Key::F10;
    break;
  case (int)Key::F11:
    pressed = Key::F11;
    break;
  case (int)Key::F12:
    pressed = Key::F12;
    break;

  // Arrow Keys
  case (int)Key::LEFT:
    pressed = Key::LEFT;
    break;
  case (int)Key::RIGHT:
    pressed = Key::RIGHT;
    break;
  case (int)Key::UP:
    pressed = Key::UP;
    break;
  case (int)Key::DOWN:
    pressed = Key::DOWN;
    break;

  // Modifier Keys
  case (int)Key::LSHIFT:
    pressed = Key::LSHIFT;
    break;
  case (int)Key::RSHIFT:
    pressed = Key::RSHIFT;
    break;
  case (int)Key::LCTRL:
    pressed = Key::LCTRL;
    break;
  case (int)Key::RCTRL:
    pressed = Key::RCTRL;
    break;
  case (int)Key::LALT:
    pressed = Key::LALT;
    break;
  case (int)Key::RALT:
    pressed = Key::RALT;
    break;
  case (int)Key::LGUI:
    pressed = Key::LGUI;
    break;
  case (int)Key::RGUI:
    pressed = Key::RGUI;
    break;

  // Enter and Backspace
  case (int)Key::ENTER:
    pressed = Key::ENTER;
    break;
  case (int)Key::ESCAPE:
    pressed = Key::ESCAPE;
    break;
  case (int)Key::BACKSPACE:
    pressed = Key::BACKSPACE;
    break;

  // Space and Tab
  case (int)Key::SPACE:
    pressed = Key::SPACE;
    break;
  case (int)Key::TAB:
    pressed = Key::TAB;
    break;

  // Punctuation Keys
  case (int)Key::COMMA:
    pressed = Key::COMMA;
    break;
  case (int)Key::PERIOD:
    pressed = Key::PERIOD;
    break;
  case (int)Key::SLASH:
    pressed = Key::SLASH;
    break;
  case (int)Key::SEMICOLON:
    pressed = Key::SEMICOLON;
    break;
  case (int)Key::APOSTROPHE:
    pressed = Key::APOSTROPHE;
    break;

  // Brackets and Others
  case (int)Key::LEFTBRACKET:
    pressed = Key::LEFTBRACKET;
    break;
  case (int)Key::RIGHTBRACKET:
    pressed = Key::RIGHTBRACKET;
    break;
  case (int)Key::BACKSLASH:
    pressed = Key::BACKSLASH;
    break;

  // Other Special Keys
  case (int)Key::CAPSLOCK:
    pressed = Key::CAPSLOCK;
    break;
  case (int)Key::PRINTSCREEN:
    pressed = Key::PRINTSCREEN;
    break;
  case (int)Key::SCROLLLOCK:
    pressed = Key::SCROLLLOCK;
    break;
  case (int)Key::PAUSE:
    pressed = Key::PAUSE;
    break;
  case (int)Key::INSERT:
    pressed = Key::INSERT;
    break;
  case (int)Key::HOME:
    pressed = Key::HOME;
    break;
  case (int)Key::PAGEUP:
    pressed = Key::PAGEUP;
    break;
  case (int)Key::PAGEDOWN:
    pressed = Key::PAGEDOWN;
    break;
  case (int)Key::DELETE:
    pressed = Key::DELETE;
    break;
  case (int)Key::END:
    pressed = Key::END;
    break;

  default:
    pressed = Key::NONE;
    break;
  }
}

bool Keyboard::isPressed(Key key) const
{
  return pressed == key;
}

bool Keyboard::isGuiPressed(LRKey lr) const
{
  return lr == LRKey::LEFT ? isLGuiDown : isRGuiDown;
}

bool Keyboard::isAltPressed(LRKey lr) const
{
  return lr == LRKey::LEFT ? isLAltDown : isRAltDown;
}

bool Keyboard::isCtrlPressed(LRKey lr) const
{
  return lr == LRKey::LEFT ? isLCtrlDown : isRCtrlDown;
}

bool Keyboard::isShiftPressed(LRKey lr) const
{
  return lr == LRKey::LEFT ? isLShiftDown : isRShiftDown;
}

Key Keyboard::getPressed() const
{
  return pressed;
}
