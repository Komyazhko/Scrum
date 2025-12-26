#include "board_view.hpp"
#include <algorithm>

namespace scrum {

using namespace ftxui;

BoardView::BoardView(ScrumBoard &board) : board_(board) {}

Element BoardView::Render() {
  return hbox({
             RenderColumn("BACKLOG", TaskStatus::Backlog) | flex,
             separator(),
             RenderColumn("ASSIGNED", TaskStatus::Assigned) | flex,
             separator(),
             RenderColumn("IN PROGRESS", TaskStatus::InProgress) | flex,
             separator(),
             RenderColumn("BLOCKED", TaskStatus::Blocked) | flex,
             separator(),
             RenderColumn("DONE", TaskStatus::Done) | flex,
         }) |
         border;
}

Element BoardView::RenderColumn(const std::string &title, TaskStatus status) {
  auto tasks = board_.getTasksByStatus(status);

  Elements task_elements;
  for (const auto &task : tasks) {
    task_elements.push_back(RenderTaskCard(task));
    task_elements.push_back(separator());
  }

  if (task_elements.empty()) {
    task_elements.push_back(text("(пусто)") | dim | center);
  } else {
    task_elements.pop_back();
  }

  Color header_color;
  switch (status) {
  case TaskStatus::Backlog:
    header_color = Color::Blue;
    break;
  case TaskStatus::Assigned:
    header_color = Color::Cyan;
    break;
  case TaskStatus::InProgress:
    header_color = Color::Yellow;
    break;
  case TaskStatus::Blocked:
    header_color = Color::Red;
    break;
  case TaskStatus::Done:
    header_color = Color::Green;
    break;
  }

  return vbox({
      text(title) | bold | center | color(header_color),
      separator(),
      vbox(std::move(task_elements)) | flex,
  });
}

Element BoardView::RenderTaskCard(const Task &task) {
  std::string assignee_str;
  if (task.assignee_id.has_value()) {
    assignee_str = GetDeveloperName(task.assignee_id.value());
  }

  Elements card_content;
  card_content.push_back(
      text("[" + std::to_string(task.id) + "] " + task.title) | bold);

  if (!task.description.empty()) {
    std::string desc = task.description;
    if (desc.length() > 30) {
      desc = desc.substr(0, 27) + "...";
    }
    card_content.push_back(text(desc) | dim);
  }

  if (!assignee_str.empty()) {
    card_content.push_back(text("→ " + assignee_str) | color(Color::Cyan));
  }

  return vbox(std::move(card_content)) | border;
}

std::string BoardView::GetDeveloperName(int id) const {
  auto dev = board_.getDeveloper(id);
  if (dev.has_value()) {
    return dev->name;
  }
  return "Unknown";
}

} 
