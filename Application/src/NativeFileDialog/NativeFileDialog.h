#pragma once

#include <string>
#include <stdio.h>
#include <exception>

#include <SDL3/SDL.h>

#include "nfd.h"

namespace NativeFileDialog
{
  using Filters = nfdu8filteritem_t;
  using FilterSize = nfdfiltersize_t;

  /**
   * Sets the native window handle for the given SDL_Window, allowing the use of
   * platform-specific windowing systems in file dialogs.
   *
   * @param window A pointer to the SDL_Window for which to retrieve the native handle.
   * @param nativeWindow A pointer to a nfdwindowhandle_t structure where the native
   *                     window type and handle will be stored. This structure should
   *                     be pre-allocated by the caller.
   *
   * @return Returns true if the native window handle was successfully retrieved and set;
   *         otherwise, it returns false.
   *
   * This function checks the platform and retrieves the appropriate window handle:
   * - On Windows, it uses the HWND type.
   * - On macOS, it retrieves the NSWindow type.
   * - On Linux, it differentiates between X11 and Wayland, fetching the appropriate handle
   *   based on the current video driver.
   *
   * Note: Ensure that the SDL_Window has been properly initialized and is valid before
   * calling this function.
   */
  inline bool setNativeWindow(SDL_Window *window,
                              nfdwindowhandle_t *nativeWindow)
  {

#if defined(SDL_PLATFORM_WIN32)
    HWND hwnd = (HWND)SDL_GetPointerProperty(SDL_GetWindowProperties(window), SDL_PROP_WINDOW_WIN32_HWND_POINTER, NULL);
    if (hwnd)
    {
      nativeWindow->type = NFD_WINDOW_HANDLE_TYPE_WINDOWS;
      nativeWindow->handle = (void *)hwnd;
      return true;
    }
#elif defined(SDL_PLATFORM_MACOS)
    NSWindow *nswindow = (__bridge NSWindow *)SDL_GetPointerProperty(SDL_GetWindowProperties(window), SDL_PROP_WINDOW_COCOA_WINDOW_POINTER, NULL);
    if (nswindow)
    {
      nativeWindow->type = NFD_WINDOW_HANDLE_TYPE_COCOA;
      nativeWindow->handle = (void *)info.info.cocoa.window;
      return true
    }
#elif defined(SDL_PLATFORM_LINUX)
    if (SDL_strcmp(SDL_GetCurrentVideoDriver(), "x11") == 0)
    {
      auto xwindow = SDL_GetNumberProperty(SDL_GetWindowProperties(window), SDL_PROP_WINDOW_X11_WINDOW_NUMBER, 0);
      if (xwindow)
      {
        nativeWindow->type = NFD_WINDOW_HANDLE_TYPE_X11;
        nativeWindow->handle = (void *)xwindow;
        return true;
      }
    }
    else if (SDL_strcmp(SDL_GetCurrentVideoDriver(), "wayland") == 0)
    {
      // Native window handle for Wayland is not supported by nfd yet.
      // struct wl_window *wayland = (struct wl_window *)SDL_GetPointerProperty(SDL_GetWindowProperties(window), SDL_PROP_WINDOW_WAYLAND_EGL_WINDOW_POINTER, NULL);
      // if (wayland)
      // {
      //   nativeWindow->type = NFD_WINDOW_HANDLE_TYPE_UNSET;
      //   nativeWindow->handle = (void *)wayland;
      //   return true;
      // }
    }

#endif
    (void)nativeWindow;
    SDL_SetError("Unsupported native window type.");
    return false;
  }

  /**
   * Opens a file selection dialog and returns the selected file's path as a UTF-8 encoded string.
   *
   * @param window A pointer to the SDL_Window that serves as the parent for the dialog. This helps
   *               to position the dialog relative to the specified window.
   * @param filters An optional pointer to a list of file filters to limit the types of files
   *                that can be selected. If nullptr, no filters will be applied.
   * @param filterSize The number of filters in the filters list. If filters is nullptr,
   *                   this parameter is ignored and can be set to 0.
   *
   * @return the file path
   */
  inline std::string SelectFile(SDL_Window *window, Filters *filters = nullptr, FilterSize filterSize = 0)
  {
    nfdu8char_t *outPath = nullptr;
    std::string filePath;

    try
    {
      nfdopendialogu8args_t oargs = {0};
      oargs.filterList = filters;
      oargs.filterCount = filterSize;
      setNativeWindow(window, &oargs.parentWindow);
      nfdresult_t result = NFD_OpenDialogU8_With(&outPath, &oargs);

      if (result == NFD_OKAY)
      {
        filePath = std::string(outPath);
        NFD_FreePathU8(outPath);
      }

      if (result == NFD_ERROR)
        printf("NativeDialog error: %s", NFD_GetError());
    }
    catch (const std::exception &e)
    {
      printf("NativeDialog error: %s", e.what());
    }

    return filePath;
  }

  /**
   * Opens a file selection dialog and returns the selected file's path as a UTF-8 encoded string.
   *
   * @param window A pointer to the SDL_Window that serves as the parent for the dialog. This helps
   *               to position the dialog relative to the specified window.
   * @param filters An optional pointer to a list of file filters to limit the types of files
   *                that can be selected. If nullptr, no filters will be applied.
   * @param filterSize The number of filters in the filters list. If filters is nullptr,
   *                   this parameter is ignored and can be set to 0.
   *
   * @return A pointer to a UTF8Char string representing the selected file's path, or nullptr
   *         if the user cancels the operation or if an error occurs.
   *
   * IMPORTANT: You must free the NativeFileDialog::UTF8Char *path using NativeFileDialog::free().
   *            Failing to do so will result in a memory leak.
   */
  inline std::vector<std::string> SelectFiles(SDL_Window *window, Filters *filters = nullptr, FilterSize filterSize = 0)
  {
    const nfdpathset_t *outPaths = nullptr;
    std::vector<std::string> filesPaths;

    try
    {
      nfdopendialogu8args_t oargs = {0};
      oargs.filterList = filters;
      oargs.filterCount = filterSize;
      setNativeWindow(window, &oargs.parentWindow);
      nfdresult_t result = NFD_OpenDialogMultipleU8_With(&outPaths, &oargs);
      if (result == NFD_ERROR)
        printf("NativeDialog error: %s", NFD_GetError());

      if (result == NFD_OKAY)
      {
        nfdpathsetsize_t numPaths;
        NFD_PathSet_GetCount(outPaths, &numPaths);

        nfdpathsetsize_t i;
        for (i = 0; i < numPaths; ++i)
        {
          nfdchar_t *pathChar;
          NFD_PathSet_GetPath(outPaths, i, &pathChar);
          filesPaths.push_back(std::string(pathChar));
          NFD_PathSet_FreePath(pathChar);
        }

        NFD_PathSet_Free(outPaths);
      }
    }
    catch (const std::exception &e)
    {
      printf("NativeDialog error: %s", e.what());
    }

    return filesPaths;
  }

  /**
   * Opens a save file dialog and returns the selected file's path as a UTF-8 encoded string.
   *
   * @param window A pointer to the SDL_Window that serves as the parent for the dialog. This helps
   *               to position the dialog relative to the specified window.
   * @param filters An optional pointer to a list of file filters to limit the types of files
   *                that can be selected. If nullptr, no filters will be applied.
   * @param filterSize The number of filters in the filters list. If filters is nullptr,
   *                   this parameter is ignored and can be set to 0.
   *
   * @return A pointer to a UTF8Char string representing the selected file's path, or nullptr
   *         if the user cancels the operation or if an error occurs.
   *
   * IMPORTANT: You must free the NativeFileDialog::UTF8Char *path using NativeFileDialog::free().
   *            Failing to do so will result in a memory leak.
   */
  inline std::string SaveFile(SDL_Window *window, Filters *filters = nullptr, FilterSize filterSize = 0)
  {
    nfdu8char_t *outPath = nullptr;
    std::string filePath;

    try
    {
      nfdsavedialogu8args_t sargs = {0};
      sargs.filterList = filters;
      sargs.filterCount = filterSize;
      setNativeWindow(window, &sargs.parentWindow);
      nfdresult_t result = NFD_SaveDialogU8_With(&outPath, &sargs);

      if (result == NFD_ERROR)
        printf("NativeDialog error: %s", NFD_GetError());

      if (result == NFD_OKAY)
      {
        filePath = std::string(outPath);
        NFD_FreePathU8(outPath);
      }
    }
    catch (const std::exception &e)
    {
      printf("NativeDialog error: %s", e.what());
    }

    return filePath;
  }

  /**
   * Opens a folder selection dialog and returns the selected file's path as a UTF-8 encoded string.
   *
   * @param window A pointer to the SDL_Window that serves as the parent for the dialog. This helps
   *               to position the dialog relative to the specified window.
   *
   * @return the folder path.
   */
  inline std::string SelectFolder(SDL_Window *window)
  {
    nfdu8char_t *outPath = nullptr;
    std::string folderPath;
    try
    {
      nfdpickfolderu8args_t fargs = {0};
      setNativeWindow(window, &fargs.parentWindow);
      nfdresult_t result = NFD_PickFolderU8_With(&outPath, &fargs);
      if (result == NFD_ERROR)
        printf("NativeDialog error: %s", NFD_GetError());

      if (result == NFD_OKAY)
      {
        folderPath = std::string(outPath);
        NFD_FreePathU8(outPath);
      }
    }
    catch (const std::exception &e)
    {
      printf("NativeDialog error: %s", e.what());
    }

    return folderPath;
  }
}
