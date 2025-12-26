#ifndef SCRUM_MODELS_SCRUM_BOARD_HPP
#define SCRUM_MODELS_SCRUM_BOARD_HPP

#include "developer.hpp"
#include "task.hpp"
#include <optional>
#include <stdexcept>
#include <vector>

namespace scrum {

class ScrumBoard {
public:
  ScrumBoard() = default;

  // Управление разработчиками
  int addDeveloper(const std::string &name); // Добавить
  bool removeDeveloper(int id); // Удалить
  std::optional<Developer> getDeveloper(int id) const; // Найти
  const std::vector<Developer> &getDevelopers() const { return developers_; }

  // Управление задачами
  int addTask(const std::string &title, const std::string &description); // Добавить
  bool removeTask(int id); // Удалить
  std::optional<Task> getTask(int id) const; // Найти
  const std::vector<Task> &getTasks() const { return tasks_; }

  bool assignTask(int task_id, int developer_id); 
  bool unassignTask(int task_id);
  bool setTaskStatus(int task_id, TaskStatus status);


  std::vector<Task> getTasksByStatus(TaskStatus status) const; //Возвращает все задачи с указанным статусом
  std::vector<Task> getTasksByDeveloper(int developer_id) const; // Возвращает все задачи разработчика

  
  void setNextDeveloperId(int id) { next_developer_id_ = id; }
  void setNextTaskId(int id) { next_task_id_ = id; }
  int getNextDeveloperId() const { return next_developer_id_; }
  int getNextTaskId() const { return next_task_id_; }

  void addDeveloperDirect(const Developer &dev) { developers_.push_back(dev); } // Добавление уже созданного разработчика
  void addTaskDirect(const Task &task) { tasks_.push_back(task); } // Добавление уже созданной задачи 

  void clear();

private:
  std::vector<Developer> developers_;
  std::vector<Task> tasks_;
  int next_developer_id_ = 1;
  int next_task_id_ = 1;

  Task *findTask(int id);
  const Task *findTask(int id) const;
  Developer *findDeveloper(int id);
  const Developer *findDeveloper(int id) const;
};

} 

#endif 
