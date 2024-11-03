#include "imgui.h"
#include "application.h"

class MainMenu
{
private:
  Application *application = nullptr;

public:
  MainMenu() {}
  ~MainMenu() {}

  void setApplication(Application *application);

  void onDraw();
  void File_Menu();

  void File_New();
  void File_Open();
  void File_OpenRecent();
  void File_Save();
  void File_SaveAs();

  void File_Options();
  void File_Quit();


  void Edit_Menu();

  void Edit_Undo();
  void Edit_Redo();
  void Edit_Cut();
  void Edit_Copy();
  void Edit_Paste();
};