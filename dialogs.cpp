#include "dialogs.hpp"
#include <ftxui/component/component.hpp>

namespace scrum {

using namespace ftxui;

Component Dialogs::TextInput(const std::string &label, std::string *content) {
  auto input = Input(content, label);
  return input;
}

Component Dialogs::StatusDropdown(int *selected_index) {
  auto options = GetStatusOptions();
  return Dropdown(options, selected_index);
}

Component Dialogs::DeveloperDropdown(const std::vector<Developer> &developers,
                                     int *selected_index) {
  std::vector<std::string> names;
  names.push_back("(Не назначен)");
  for (const auto &dev : developers) {
    names.push_back(dev.name);
  }
  return Dropdown(names, selected_index);
}

std::vector<std::string> Dialogs::GetStatusOptions() {
  return {"Backlog", "Assigned", "InProgress", "Blocked", "Done"};
}

TaskStatus Dialogs::IndexToStatus(int index) {
  switch (index) {
  case 0:
    return TaskStatus::Backlog;
  case 1:
    return TaskStatus::Assigned;
  case 2:
    return TaskStatus::InProgress;
  case 3:
    return TaskStatus::Blocked;
  case 4:
    return TaskStatus::Done;
  default:
    return TaskStatus::Backlog;
  }
}

int Dialogs::StatusToIndex(TaskStatus status) {
  switch (status) {
  case TaskStatus::Backlog:
    return 0;
  case TaskStatus::Assigned:
    return 1;
  case TaskStatus::InProgress:
    return 2;
  case TaskStatus::Blocked:
    return 3;
  case TaskStatus::Done:
    return 4;
  default:
    return 0;
  }
}

} 
