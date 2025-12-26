#ifndef SCRUM_UI_BOARD_VIEW_HPP
#define SCRUM_UI_BOARD_VIEW_HPP

#include "models/scrum_board.hpp"
#include <ftxui/component/component.hpp>
#include <ftxui/dom/elements.hpp>

namespace scrum {

class BoardView {
public:
  explicit BoardView(ScrumBoard &board);

  // Визуализация доски с 5 колоннами
  ftxui::Element Render();

private:
  ScrumBoard &board_;
  
  ftxui::Element RenderColumn(const std::string &title, TaskStatus status);

  ftxui::Element RenderTaskCard(const Task &task);

  // Получить имя разработчика по ID
  std::string GetDeveloperName(int id) const;
};

} 

#endif 
