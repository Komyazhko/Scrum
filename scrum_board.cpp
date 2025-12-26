#include "scrum_board.hpp"
#include <algorithm>

namespace scrum {

// Управление разработчиками

int ScrumBoard::addDeveloper(const std::string &name) {
  int id = next_developer_id_++;
  developers_.emplace_back(id, name);
  return id;
}

bool ScrumBoard::removeDeveloper(int id) {
  //Удаление задач от разработчика
  for (auto &task : tasks_) {
    if (task.assignee_id.has_value() && task.assignee_id.value() == id) {
      task.unassign();
    }
  }

  auto it = std::find_if(developers_.begin(), developers_.end(),
                         [id](const Developer &d) { return d.id == id; }); // равен ли искомому id

  if (it != developers_.end()) {
    developers_.erase(it);
    return true;
  }
  return false;
}

std::optional<Developer> ScrumBoard::getDeveloper(int id) const {
  const Developer *dev = findDeveloper(id);
  if (dev)
    return *dev;
  return std::nullopt;
}

// Управление задачами

int ScrumBoard::addTask(const std::string &title,
                        const std::string &description) {
  int id = next_task_id_++;
  tasks_.emplace_back(id, title, description);
  return id;
}

bool ScrumBoard::removeTask(int id) {
  auto it = std::find_if(tasks_.begin(), tasks_.end(),
                         [id](const Task &t) { return t.id == id; }); 

  if (it != tasks_.end()) {
    tasks_.erase(it);
    return true;
  }
  return false;
}

std::optional<Task> ScrumBoard::getTask(int id) const {
  const Task *task = findTask(id);
  if (task)
    return *task;
  return std::nullopt;
}

// Операции с задачами

bool ScrumBoard::assignTask(int task_id, int developer_id) {
  Task *task = findTask(task_id);
  const Developer *dev = findDeveloper(developer_id);

  if (task && dev) {
    task->assign(developer_id);
    return true;
  }
  return false;
}

bool ScrumBoard::unassignTask(int task_id) {
  Task *task = findTask(task_id);
  if (task) {
    task->unassign();
    return true;
  }
  return false;
}

bool ScrumBoard::setTaskStatus(int task_id, TaskStatus status) {
  Task *task = findTask(task_id);
  if (task) {
    task->setStatus(status);
    return true;
  }
  return false;
}

std::vector<Task> ScrumBoard::getTasksByStatus(TaskStatus status) const {
  std::vector<Task> result;
  for (const auto &task : tasks_) {
    if (task.status == status) {
      result.push_back(task);
    }
  }
  return result;
}

std::vector<Task> ScrumBoard::getTasksByDeveloper(int developer_id) const {
  std::vector<Task> result;
  for (const auto &task : tasks_) {
    if (task.assignee_id.has_value() &&
        task.assignee_id.value() == developer_id) {
      result.push_back(task);
    }
  }
  return result;
}

void ScrumBoard::clear() {
  developers_.clear();
  tasks_.clear();
  next_developer_id_ = 1;
  next_task_id_ = 1;
}

// Private

Task *ScrumBoard::findTask(int id) {
  auto it = std::find_if(tasks_.begin(), tasks_.end(),
                         [id](const Task &t) { return t.id == id; });
  return it != tasks_.end() ? &(*it) : nullptr;
}

const Task *ScrumBoard::findTask(int id) const {
  auto it = std::find_if(tasks_.begin(), tasks_.end(),
                         [id](const Task &t) { return t.id == id; });
  return it != tasks_.end() ? &(*it) : nullptr;
}

Developer *ScrumBoard::findDeveloper(int id) {
  auto it = std::find_if(developers_.begin(), developers_.end(),
                         [id](const Developer &d) { return d.id == id; });
  return it != developers_.end() ? &(*it) : nullptr;
}

const Developer *ScrumBoard::findDeveloper(int id) const {
  auto it = std::find_if(developers_.begin(), developers_.end(),
                         [id](const Developer &d) { return d.id == id; });
  return it != developers_.end() ? &(*it) : nullptr;
}

} 
