#include "app.hpp"
#include "persistence/json_serializer.hpp"
#include <ftxui/component/component.hpp>
#include <ftxui/dom/elements.hpp>

namespace scrum {

using namespace ftxui;

App::App() : screen_(ScreenInteractive::Fullscreen()) {}

void App::Run() {
  // Выбор меню
  int menu_selected = 0;
  std::vector<std::string> menu_entries = {
      "Просмотр доски",  "Добавить разработчика", "Управление разработчиками",
      "Добавить задачу", "Назначить задачу",      "Редактировать задачу",
      "Сохранить доску", "Загрузить доску",       "Выход"};

  // Создание компонентов
  auto menu = Menu(&menu_entries, &menu_selected);
  auto input_name = Input(&input_name_, "имя разработчика");
  auto input_title = Input(&input_title_, "название задачи");
  auto input_desc = Input(&input_description_, "описание");
  auto input_file_save = Input(&input_filename_, "board.json");
  auto input_file_load = Input(&input_filename_, "board.json");

  auto input_name_with_enter = CatchEvent(input_name, [&](Event event) {
    if (event == Event::Return) {
      if (!input_name_.empty()) {
        int id = board_.addDeveloper(input_name_);
        status_message_ =
            "Разработчик добавлен (ID: " + std::to_string(id) + ")";
        input_name_.clear();
      } else {
        status_message_ = "Ошибка: введите имя";
      }
      return true;
    }
    return false;
  });

  auto input_task_container = Container::Vertical({input_title, input_desc});
  auto input_task_with_enter =
      CatchEvent(input_task_container, [&](Event event) {
        if (event == Event::Return && current_view_ == View::AddTask) {
          if (!input_title_.empty()) {
            int id = board_.addTask(input_title_, input_description_);
            status_message_ =
                "Задача добавлена (ID: " + std::to_string(id) + ")";
            input_title_.clear();
            input_description_.clear();
          } else {
            status_message_ = "Ошибка: введите название задачи";
          }
          return true;
        }
        return false;
      });

  auto input_file_save_with_enter =
      CatchEvent(input_file_save, [&](Event event) {
        if (event == Event::Return && current_view_ == View::SaveBoard) {
          std::string filename =
              input_filename_.empty() ? "board.json" : input_filename_;
          try {
            JsonSerializer::save(board_, filename);
            status_message_ = "Сохранено в " + filename;
          } catch (const std::exception &e) {
            status_message_ = "Ошибка: " + std::string(e.what());
          }
          return true;
        }
        return false;
      });

  auto input_file_load_with_enter =
      CatchEvent(input_file_load, [&](Event event) {
        if (event == Event::Return && current_view_ == View::LoadBoard) {
          std::string filename =
              input_filename_.empty() ? "board.json" : input_filename_;
          try {
            board_ = JsonSerializer::load(filename);
            status_message_ = "Загружено из " + filename;
          } catch (const std::exception &e) {
            status_message_ = "Ошибка: " + std::string(e.what());
          }
          return true;
        }
        return false;
      });

  auto main_container = Container::Tab(
      {
          menu, // 0 - Главное меню
          Renderer(
              [] { return text(""); }), // 1 - Просмотр доски
          input_name_with_enter,        // 2 - Добавить разработчика
          Renderer([] {
            return text("");
          }), // 3 - Управление разработчиками
          input_task_with_enter, // 4 - Добавить задачу
          Renderer([] {
            return text("");
          }), // 5 - Назначить задачу
          Renderer([] {
            return text("");
          }), // 6 - Редактировать задачу
          input_file_save_with_enter, // 7 - Сохранить доску
          input_file_load_with_enter, // 8 - Загрузить доску
      },
      &tab_index_);

  auto component = Renderer(main_container, [&]() {
    Element content;

    switch (current_view_) {
    case View::MainMenu: {
      tab_index_ = 0;
      content = vbox({
          text("") | size(HEIGHT, EQUAL, 1),
          text("ГЛАВНОЕ МЕНЮ") | bold | center,
          text("") | size(HEIGHT, EQUAL, 1),
          separator(),
          menu->Render() | border | size(WIDTH, EQUAL, 40) | center,
          text("") | size(HEIGHT, EQUAL, 1),
          text("↑↓ - выбор, Enter - подтвердить, Q - выход") | dim | center,
      });
      break;
    }
    case View::Board: {
      tab_index_ = 1;
      BoardView board_view(board_);
      content = vbox({
          text("SCRUM ДОСКА") | bold | center,
          separator(),
          board_view.Render() | flex,
          separator(),
          text("ESC - назад") | dim | center,
      });
      break;
    }
    case View::AddDeveloper: {
      tab_index_ = 2;
      content = vbox({
                    text("ДОБАВИТЬ РАЗРАБОТЧИКА") | bold | center,
                    separator(),
                    text("") | size(HEIGHT, EQUAL, 1),
                    hbox({text("Имя: "), input_name->Render() | flex}) | border,
                    text("") | size(HEIGHT, EQUAL, 1),
                    text("Введите имя и нажмите Enter") | dim | center,
                    text("ESC - назад") | dim | center,
                }) |
                size(WIDTH, EQUAL, 50) | center;
      break;
    }
    case View::ManageDevelopers: {
      tab_index_ = 3;
      Elements dev_list;
      dev_list.push_back(text("РАЗРАБОТЧИКИ") | bold | center);
      dev_list.push_back(separator());

      if (board_.getDevelopers().empty()) {
        dev_list.push_back(text("Список пуст") | dim | center);
      } else {
        for (const auto &dev : board_.getDevelopers()) {
          auto tasks = board_.getTasksByDeveloper(dev.id);
          dev_list.push_back(hbox({
              text("[" + std::to_string(dev.id) + "] ") | bold,
              text(dev.name),
              text(" (" + std::to_string(tasks.size()) + " задач)") | dim,
          }));
        }
      }

      dev_list.push_back(separator());
      dev_list.push_back(text("Введите ID для удаления разработчика") | dim |
                         center);
      dev_list.push_back(text("ESC - назад") | dim | center);

      content =
          vbox(std::move(dev_list)) | border | size(WIDTH, EQUAL, 50) | center;
      break;
    }
    case View::AddTask: {
      tab_index_ = 4;
      content =
          vbox({
              text("ДОБАВИТЬ ЗАДАЧУ") | bold | center,
              separator(),
              text("") | size(HEIGHT, EQUAL, 1),
              hbox({text("Название: "), input_title->Render() | flex}) | border,
              hbox({text("Описание: "), input_desc->Render() | flex}) | border,
              text("") | size(HEIGHT, EQUAL, 1),
              text("Tab - следующее поле, Enter - добавить") | dim | center,
              text("ESC - назад") | dim | center,
          }) |
          size(WIDTH, EQUAL, 60) | center;
      break;
    }
    case View::AssignTask: {
      tab_index_ = 5;
      Elements list;

      if (selected_task_id_ == 0) {
        list.push_back(text("ВЫБЕРИТЕ ЗАДАЧУ") | bold | center);
        list.push_back(separator());

        if (board_.getTasks().empty()) {
          list.push_back(text("Нет задач") | dim | center);
        } else {
          for (const auto &task : board_.getTasks()) {
            std::string assignee = "не назначена";
            if (task.assignee_id.has_value()) {
              auto dev = board_.getDeveloper(task.assignee_id.value());
              if (dev)
                assignee = dev->name;
            }
            list.push_back(
                hbox({text("[" + std::to_string(task.id) + "] ") | bold,
                      text(task.title) | flex, text(" → " + assignee) | dim}));
          }
        }
        list.push_back(separator());
        list.push_back(text("Введите ID задачи") | dim | center);
      } else {
        auto task = board_.getTask(selected_task_id_);
        list.push_back(text("НАЗНАЧИТЬ: " + (task ? task->title : "?")) | bold |
                       center);
        list.push_back(separator());
        list.push_back(text("[0] Снять назначение") | color(Color::Yellow));
        list.push_back(separator());

        for (const auto &dev : board_.getDevelopers()) {
          list.push_back(hbox({text("[" + std::to_string(dev.id) + "] ") | bold,
                               text(dev.name)}));
        }
        list.push_back(separator());
        list.push_back(text("Введите ID разработчика") | dim | center);
      }
      list.push_back(text("ESC - назад") | dim | center);

      content =
          vbox(std::move(list)) | border | size(WIDTH, EQUAL, 60) | center;
      break;
    }
    case View::EditTask: {
      tab_index_ = 6;
      Elements task_list;
      task_list.push_back(text("УПРАВЛЕНИЕ СТАТУСАМИ") | bold | center);
      task_list.push_back(separator());

      if (board_.getTasks().empty()) {
        task_list.push_back(text("Нет задач") | dim | center);
      } else {
        for (const auto &task : board_.getTasks()) {
          std::string status_str = taskStatusToString(task.status);
          Color status_color;
          switch (task.status) {
          case TaskStatus::Backlog:
            status_color = Color::Blue;
            break;
          case TaskStatus::Assigned:
            status_color = Color::Cyan;
            break;
          case TaskStatus::InProgress:
            status_color = Color::Yellow;
            break;
          case TaskStatus::Blocked:
            status_color = Color::Red;
            break;
          case TaskStatus::Done:
            status_color = Color::Green;
            break;
          }

          std::string marker = (task.id == selected_task_id_) ? "▶ " : "  ";

          task_list.push_back(hbox({
              text(marker) | bold | color(Color::GreenLight),
              text("[" + std::to_string(task.id) + "] ") | bold,
              text(task.title),
              text(" [" + status_str + "]") | color(status_color),
          }));
        }
      }

      task_list.push_back(separator());
      if (selected_task_id_ == 0) {
        task_list.push_back(text("Введите ID задачи для выбора") | dim |
                            center);
      } else {
        task_list.push_back(text("1-5: сменить статус, D: удалить") | dim |
                            center);
      }
      task_list.push_back(text("ESC - назад") | dim | center);

      content =
          vbox(std::move(task_list)) | border | size(WIDTH, EQUAL, 70) | center;
      break;
    }
    case View::SaveBoard: {
      tab_index_ = 7;
      content =
          vbox({
              text("СОХРАНИТЬ ДОСКУ") | bold | center,
              separator(),
              text("") | size(HEIGHT, EQUAL, 1),
              hbox({text("Файл: "), input_file_save->Render() | flex}) | border,
              text("") | size(HEIGHT, EQUAL, 1),
              text("Enter - сохранить, ESC - назад") | dim | center,
          }) |
          size(WIDTH, EQUAL, 50) | center;
      break;
    }
    case View::LoadBoard: {
      tab_index_ = 8;
      content =
          vbox({
              text("ЗАГРУЗИТЬ ДОСКУ") | bold | center,
              separator(),
              text("") | size(HEIGHT, EQUAL, 1),
              hbox({text("Файл: "), input_file_load->Render() | flex}) | border,
              text("") | size(HEIGHT, EQUAL, 1),
              text("Enter - загрузить, ESC - назад") | dim | center,
          }) |
          size(WIDTH, EQUAL, 50) | center;
      break;
    }
    }

    // Заголовок
    auto header =
        hbox({
            text("SCRUM BOARD") | bold | color(Color::Cyan),
            filler(),
            text("Разработчиков: " +
                 std::to_string(board_.getDevelopers().size())) |
                dim,
            text(" | ") | dim,
            text("Задач: " + std::to_string(board_.getTasks().size())) | dim,
        }) |
        border;

    // Строка состояния
    Color status_color = status_message_.find("Ошибка") != std::string::npos
                             ? Color::Red
                             : Color::Green;
    auto status_bar = text(status_message_) | color(status_color);

    return vbox({
        header,
        content | flex | center,
        separator(),
        status_bar,
    });
  });

  component = CatchEvent(component, [&](Event event) {
    // Блокировка мышки
    if (event.is_mouse()) {
      return true;
    }

    // ESC - выход
    if (event == Event::Escape) {
      if (current_view_ != View::MainMenu) {
        if (current_view_ == View::AssignTask && selected_task_id_ != 0) {
          selected_task_id_ = 0;
          status_message_ = "";
          return true;
        }
        if (current_view_ == View::EditTask && selected_task_id_ != 0) {
          selected_task_id_ = 0;
          status_message_ = "";
          return true;
        }
        current_view_ = View::MainMenu;
        selected_task_id_ = 0;
        status_message_ = "";
        return true;
      }
    }

    // Управление главным меню
    if (current_view_ == View::MainMenu) {
      if (event == Event::Character('q') || event == Event::Character('Q')) {
        screen_.Exit();
        return true;
      }
      if (event == Event::Return) {
        switch (menu_selected) {
        case 0:
          current_view_ = View::Board;
          break;
        case 1:
          input_name_.clear();
          current_view_ = View::AddDeveloper;
          break;
        case 2:
          current_view_ = View::ManageDevelopers;
          break;
        case 3:
          input_title_.clear();
          input_description_.clear();
          current_view_ = View::AddTask;
          break;
        case 4:
          selected_task_id_ = 0;
          current_view_ = View::AssignTask;
          break;
        case 5:
          selected_task_id_ = 0;
          current_view_ = View::EditTask;
          break;
        case 6:
          input_filename_ = "board.json";
          current_view_ = View::SaveBoard;
          break;
        case 7:
          input_filename_ = "board.json";
          current_view_ = View::LoadBoard;
          break;
        case 8:
          screen_.Exit();
          break;
        }
        return true;
      }
    }

    // Управление разработчиками - удаление по ID
    if (current_view_ == View::ManageDevelopers) {
      if (event.is_character() && !event.character().empty()) {
        char c = event.character()[0];
        if (c >= '1' && c <= '9') {
          int id = c - '0';
          if (board_.removeDeveloper(id)) {
            status_message_ =
                "Разработчик ID " + std::to_string(id) + " удалён";
          } else {
            status_message_ =
                "Ошибка: разработчик ID " + std::to_string(id) + " не найден";
          }
          return true;
        }
      }
    }

    // Добавить задачу — выбор разработчика для задачи
    if (current_view_ == View::AssignTask) {
      if (event.is_character() && !event.character().empty()) {
        char c = event.character()[0];
        if (c >= '0' && c <= '9') {
          int id = c - '0';

          if (selected_task_id_ == 0) {
            // Выбор задачи
            if (id > 0 && board_.getTask(id).has_value()) {
              selected_task_id_ = id;
              status_message_ = "Выбрана задача " + std::to_string(id);
            } else if (id > 0) {
              status_message_ = "Задача " + std::to_string(id) + " не найдена";
            }
          } else {
            // выбор разработчика
            if (id == 0) {
              board_.unassignTask(selected_task_id_);
              status_message_ = "Задача снята с исполнителя";
              selected_task_id_ = 0;
            } else if (board_.getDeveloper(id).has_value()) {
              board_.assignTask(selected_task_id_, id);
              auto dev = board_.getDeveloper(id);
              status_message_ = "Задача назначена на " + dev->name;
              selected_task_id_ = 0;
            } else {
              status_message_ =
                  "Разработчик " + std::to_string(id) + " не найден";
            }
          }
          return true;
        }
      }
    }

    // Редактировать задачу - выбрать задачу, изменить статус, удалить0
    if (current_view_ == View::EditTask) {
      if (event.is_character() && !event.character().empty()) {
        char c = event.character()[0];

        if (selected_task_id_ == 0) {
          // Выбрать ID задачи 
          if (c >= '1' && c <= '9') {
            int id = c - '0';
            if (board_.getTask(id).has_value()) {
              selected_task_id_ = id;
              status_message_ = "Выбрана задача " + std::to_string(id);
            } else {
              status_message_ = "Задача " + std::to_string(id) + " не найдена";
            }
            return true;
          }
        } else {
          // Выбрать статус (1-5) или удалить (D)
          if (c >= '1' && c <= '5') {
            TaskStatus new_status;
            switch (c) {
            case '1':
              new_status = TaskStatus::Backlog;
              break;
            case '2':
              new_status = TaskStatus::Assigned;
              break;
            case '3':
              new_status = TaskStatus::InProgress;
              break;
            case '4':
              new_status = TaskStatus::Blocked;
              break;
            case '5':
              new_status = TaskStatus::Done;
              break;
            default:
              new_status = TaskStatus::Backlog;
            }
            board_.setTaskStatus(selected_task_id_, new_status);
            status_message_ = "Статус: " + taskStatusToString(new_status);
            selected_task_id_ = 0;
            return true;
          }
          if (c == 'd' || c == 'D') {
            board_.removeTask(selected_task_id_);
            status_message_ = "Задача удалена";
            selected_task_id_ = 0;
            return true;
          }
        }
      }
    }

    return false;
  });

  screen_.Loop(component);
}

Component App::CreateMainMenuComponent() {
  return Renderer([] { return text(""); });
}
Component App::CreateBoardViewComponent() {
  return Renderer([] { return text(""); });
}
Component App::CreateAddDeveloperComponent() {
  return Renderer([] { return text(""); });
}
Component App::CreateManageDevelopersComponent() {
  return Renderer([] { return text(""); });
}
Component App::CreateAddTaskComponent() {
  return Renderer([] { return text(""); });
}
Component App::CreateEditTaskComponent() {
  return Renderer([] { return text(""); });
}
Component App::CreateSaveLoadComponent(bool) {
  return Renderer([] { return text(""); });
}

void App::ShowBoard() {}
void App::ShowAddDeveloper() {}
void App::ShowManageDevelopers() {}
void App::ShowAddTask() {}
void App::ShowEditTask() {}
void App::ShowSaveBoard() {}
void App::ShowLoadBoard() {}
void App::GoToMainMenu() {}
void App::AddDeveloper() {}
void App::AddTask() {}
void App::UpdateTask() {}
void App::SaveBoard() {}
void App::LoadBoard() {}
void App::SetStatusMessage(const std::string &) {}
void App::ClearInputs() {}

} 
