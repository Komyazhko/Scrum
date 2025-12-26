#ifndef SCRUM_MODELS_TASK_HPP
#define SCRUM_MODELS_TASK_HPP

#include <string>
#include <optional>

namespace scrum {

enum class TaskStatus {
    Backlog, // задача не назначена
    Assigned,// назначена
    InProgress,// в работе 
    Blocked,// заблокированна 
    Done // готово
};
inline std::string taskStatusToString(TaskStatus status) {
    switch (status) {
        case TaskStatus::Backlog: return "Backlog";
        case TaskStatus::Assigned: return "Assigned";
        case TaskStatus::InProgress: return "InProgress";
        case TaskStatus::Blocked: return "Blocked";
        case TaskStatus::Done: return "Done";
    }
    return "Unknown";
}

inline TaskStatus stringToTaskStatus(const std::string& str) {
    if (str == "Backlog") return TaskStatus::Backlog;
    if (str == "Assigned") return TaskStatus::Assigned;
    if (str == "InProgress") return TaskStatus::InProgress;
    if (str == "Blocked") return TaskStatus::Blocked;
    if (str == "Done") return TaskStatus::Done;
    return TaskStatus::Backlog;
}

struct Task {
    int id = 0;
    std::string title;
    std::string description;
    TaskStatus status = TaskStatus::Backlog;
    std::optional<int> assignee_id;

    Task() = default;
    Task(int id, std::string title, std::string description);
    
    bool isAssigned() const { return assignee_id.has_value(); }
    void assign(int developer_id);
    void unassign();
    void setStatus(TaskStatus new_status);
};

} 

#endif 
