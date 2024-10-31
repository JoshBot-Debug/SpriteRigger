#pragma once

#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>

#include <unordered_map>

struct Text
{
  /**
   * The name identifier for the text to be used
   */
  const char *textName;

  /**
   * The UTF-8 text to be rendered, may be NULL.
   */
  const char *content;

  /**
   * The length of the text, in bytes, or 0 for null terminated text.
   */
  size_t length = 0;

  /**
   * The position of the text on the x axis.
   */
  float x;

  /**
   * The position of the text on the y axis.
   */
  float y;
};

/**
 * TextManager class is responsible for managing fonts and text rendering.
 *
 * This class handles the creation and storage of fonts and text instances,
 * allowing for easy rendering of text using SDL_ttf. It maintains a mapping
 * of font names to font resources and text names to text instances, facilitating
 * dynamic text rendering in the application.
 */
class TextManager
{
public:
  /**
   * Pointer to the text engine used for rendering text.
   */
  TTF_TextEngine *textEngine;

  /**
   * Map to store font resources associated with their names.
   * Each key is a pointer to the font name (const char*), and the value
   * is a pointer to the corresponding TTF_Font resource.
   */
  std::unordered_map<const char *, TTF_Font *> fonts;

  /**
   * Map to store text instances associated with their names.
   * Each key is a pointer to the text name (const char*), and the value
   * is a pointer to the corresponding TTF_Text resource.
   */
  std::unordered_map<const char *, TTF_Text *> texts;

public:
  TextManager(SDL_Renderer *renderer);
  ~TextManager();

  /**
   * Free all resources used by the application.
   *
   * This method should be called to release any dynamically allocated
   * resources, ensuring proper cleanup before the application exits.
   */
  void free();

  /**
   * Create a font and store its information.
   *
   * @param name The name identifier for the font (used as a key in the font map).
   * @param file The file path of the font file to be loaded.
   * @param ptsize The size of the font in points.
   */
  TTF_Font *createFont(const char *name, const char *file, float ptsize);

  /**
   * Create text using the specified font name.
   *
   * @param name The name identifier for the text to be created.
   * @param fontName The name of the font to use for the text.
   *
   * If the specified font name exists in the font map. If the font does not exist,
   * an error message will be displayed.
   */
  TTF_Text *createText(const char *name, const char *fontName);

  /**
   * Used to set the color of the text, it's better to call this once after the text is created
   * rather than calling it every frame.
   *
   * @param textName The name identifier for the text to be rendered.
   *
   * @param r the red value used to draw on the rendering target.
   * @param g the green value used to draw on the rendering target.
   * @param b the blue value used to draw on the rendering target.
   * @param a the alpha value used to draw on the rendering target; usually SDL_ALPHA_OPAQUE (255). Use SDL_SetRenderDrawBlendMode to specify how the alpha channel is used.
   */
  void setTextColor(const char *textName, Uint8 r, Uint8 g, Uint8 b, Uint8 a);

  /**
   * Renders the specified text on the screen.
   *
   * @param text A reference to the Text to render
   */
  void render(Text *text);
};