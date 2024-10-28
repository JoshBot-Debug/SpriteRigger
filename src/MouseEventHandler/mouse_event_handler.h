#pragma once

#include <SDL3/SDL.h>
#include <functional>
#include <map>

enum MouseState
{
  MOUSE_STATE_NONE,
  MOUSE_STATE_LEFT_CLICK,
  MOUSE_STATE_RIGHT_CLICK,
  MOUSE_STATE_LONG_CLICK,
};

enum MouseEvent
{
  LEFT_CLICK,
  LEFT_CLICK_IN,
  LEFT_CLICK_OUT,
  LEFT_LONG_PRESS,
  RIGHT_LONG_PRESS,
  RIGHT_CLICK,
  RIGHT_CLICK_IN,
  RIGHT_CLICK_OUT,
  NONE
};

class MouseEventHandler
{
private:
  using Callback = std::function<void()>;

  MouseState state;
  int pressed_time;

  std::map<MouseEvent, std::vector<Callback>> listeners;

  SDL_FRect *boundingRect;
  SDL_FPoint *point;

  void callListeners(MouseEvent event);
  bool hasListeners(MouseEvent event);

public:
  MouseEventHandler();
  ~MouseEventHandler();
  
  MouseState getState();

  void onEvent(SDL_Event event);
  void onEventLoopStart();

  void setBoundingRect(SDL_FRect *rect);
  void addEventListener(MouseEvent event, Callback callback);
  void removeEventListener(MouseEvent event, Callback callback);
};