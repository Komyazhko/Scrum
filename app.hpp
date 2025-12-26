#ifndef SCRUM_UI_APP_HPP
#define SCRUM_UI_APP_HPP

#include "board_view.hpp"
#include "models/scrum_board.hpp"
#include <ftxui/component/component.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <functional>
#include <string>

namespace scrum {

class App {
public:
  App();

  void Run();

private:
  ScrumBoard board_;
  ftxui::ScreenInteractive screen_;

  
  enum class View {
    MainMenu,
    Board,
    AddDeveloper,
    ManageDevelopers,
    AddTask,
    AssignTask,
    EditTask,
    SaveBoard,
    LoadBoard
  };
  View current_view_ = View::MainMenu;

  
  std::string input_name_;
  std::string input_title_;
  std::string input_description_;
  std::string input_filename_;
  int selected_task_id_ = 0;
  int selected_developer_index_ = 0;
  int selected_status_index_ = 0;
  int tab_index_ = 0;
  std::string status_message_;

  // Компоненты
  ftxui::Component CreateMainMenuComponent();
  ftxui::Component CreateBoardViewComponent();
  ftxui::Component CreateAddDeveloperComponent();
  ftxui::Component CreateManageDevelopersComponent();
  ftxui::Component CreateAddTaskComponent();
  ftxui::Component CreateEditTaskComponent();
  ftxui::Component CreateSaveLoadComponent(bool is_save);

  // Действия
  void ShowBoard();
  void ShowAddDeveloper();
  void ShowManageDevelopers();
  void ShowAddTask();
  void ShowEditTask();
  void ShowSaveBoard();
  void ShowLoadBoard();

  void AddDeveloper();
  void AddTask();
  void UpdateTask();
  void SaveBoard();
  void LoadBoard();

  void SetStatusMessage(const std::string &msg);
  void ClearInputs();
  void GoToMainMenu();
};

} 

#endif 
