#include "task.hpp"

namespace scrum {

Task::Task(int id, std::string title, std::string description)
    : id(id), title(std::move(title)), description(std::move(description)) {}

void Task::assign(int developer_id) {
  assignee_id = developer_id;
  if (status == TaskStatus::Backlog) {
    status = TaskStatus::Assigned;
  }
}

void Task::unassign() {
  assignee_id = std::nullopt;
  if (status == TaskStatus::Assigned) {
    status = TaskStatus::Backlog;
  }
}

void Task::setStatus(TaskStatus new_status) { status = new_status; }

} 