#include "nfd.h"

enum class NativeFileSystem
{
  FOLDER_PICKER,
  FILE_PICKER,
};

/**
 * @brief Opens a native file system dialog to select files or folders.
 *
 * This function provides a simple interface for opening a file picker or
 * folder picker dialog using the Native File Dialog library. Depending on
 * the specified type, it will either allow the user to select a folder
 * or a file with the given filter.
 *
 * @param type The type of dialog to open. It can be either:
 *             - NativeFileSystem::FOLDER_PICKER to open a folder selection dialog.
 *             - NativeFileSystem::FILE_PICKER to open a file selection dialog.
 *
 * @param filterList A string specifying the types of files to filter in the file picker.
 *                   The format is a semicolon-separated list of extensions, e.g., "png,jpg;txt".
 *                   This parameter is ignored if the folder picker is selected.
 *
 * @return nfdchar_t* A pointer to a dynamically allocated string containing the selected path.
 *                    The caller is responsible for freeing this memory using `free()`.
 *                    Returns nullptr if the dialog is canceled or an error occurs.
 */
nfdchar_t *NativeFileSystemDialog(NativeFileSystem type, const nfdchar_t *filterList)
{
  nfdchar_t *path = nullptr; // Pointer to store the selected path
  nfdresult_t result;        // Result of the dialog operation

  // Open a folder picker dialog
  if (type == NativeFileSystem::FOLDER_PICKER)
    result = NFD_PickFolder(NULL, &path);

  // Open a file picker dialog with the specified filters
  if (type == NativeFileSystem::FILE_PICKER)
    result = NFD_OpenDialog(filterList, nullptr, &path);

  // Check if the operation was successful
  if (result == nfdresult_t::NFD_OKAY)
    return path; // Return the selected path if successful

  return nullptr; // Return nullptr if canceled or an error occurred
}