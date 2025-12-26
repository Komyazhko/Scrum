#ifndef SCRUM_MODELS_DEVELOPER_HPP
#define SCRUM_MODELS_DEVELOPER_HPP

#include <string>

namespace scrum {

struct Developer {
  int id = 0;
  std::string name;

  Developer() = default;
  Developer(int id, std::string name);
};

}

#endif 