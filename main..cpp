#include <atomic>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "json.hpp"

using json = nlohmann::json;

struct Task {
    std::string content;
    bool done;

    Task(const std::string& content = nullptr, const bool done = false):
        content(content), done(done) {}
};

void to_json(json& j, const Task& t) {
    j = json{{"content", t.content}, {"done", t.done}};
}

void from_json(const json& j, Task& t) {
    j.at("content").get_to(t.content);
    j.at("done").get_to(t.done);
}

class TodoList {
    friend void to_json();
    friend void from_json();
  public:
    void addTask(const std::string& newTask) {
        std::vector<Task> tasks;
        std::ifstream inFile("tasks.json");
        if (inFile.is_open()) {
            json j;
            try {
                inFile >> j;
                for (const auto& item : j) {
                    Task t;
                    t.content = item.at({"content"}).get<std::string>();
                    t.done = item.at({"done"}).get<bool>();
                    tasks.push_back(t);
                }
            } catch (...) {
                std::cerr << "Error cin json-file\n";
            }
        }
        inFile.close();

        tasks.push_back(Task(newTask, false));

        std::ofstream outFile("tasks.json");
        if (outFile.is_open()) {
            json j = tasks;
            outFile << j.dump(4);
        }
    }

    void showTasks() {

    }
};

void showMenu() {
    std::cout << "\n======= TODO LIST =======\n";
    std::cout << "1. Show all tasks\n";
    std::cout << "2. Add a task\n";
    std::cout << "3. Mark task as done\n";
    std::cout << "4. Delete a task\n";
    std::cout << "5. Save and exit\n";
    std::cout << "==========================\n";
    std::cout << "Chose an action: ";
}

void handler() {
    int x;
    std::cin >> x;
    switch(x) {
        case 2:
            std::cout << "Enter the task:\n";
            std::string task;
            std::getline(std::cin, task);
            TodoList::addTask(task);
    }
}

int main() {
    setlocale(LC_ALL, "ru");
    showMenu();
    handler();
    return 0;
}
