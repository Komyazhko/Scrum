#ifndef SCRUM_UI_DIALOGS_HPP
#define SCRUM_UI_DIALOGS_HPP

#include "models/scrum_board.hpp"
#include <ftxui/component/component.hpp>
#include <functional>
#include <string>
#include <vector>

namespace scrum {

// Вспомогательные функции
class Dialogs {
public:
  // Создает компонент ввода текста 
  static ftxui::Component TextInput(const std::string &label,
                                    std::string *content);

  // Создает выпадающий компонент для выбора статуса
  static ftxui::Component StatusDropdown(int *selected_index);

  // Создает выпадающий список для выбора разработчика
  static ftxui::Component
  DeveloperDropdown(const std::vector<Developer> &developers,
                    int *selected_index);

  // Возвращает параметры состояния
  static std::vector<std::string> GetStatusOptions();

  static TaskStatus IndexToStatus(int index);
  static int StatusToIndex(TaskStatus status);
};

} 

#endif 
