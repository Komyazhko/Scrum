#include "developer.hpp"

namespace scrum {

Developer::Developer(int id, std::string name)
    : id(id), name(std::move(name)) {}

} 