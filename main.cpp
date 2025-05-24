#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "json.hpp"

using json = nlohmann::json;

int get_variant(int count) {
    int variant;
    std::string s;
    std::getline(std::cin, s);

    while (sscanf(s.c_str(), "%d", &variant) != 1 || variant < 1 || variant > count) {
        std::cout << "Try again :<\n";
        std::getline(std::cin, s);
    }
    return variant;
}

struct Task {
    std::string content;
    bool done;

    Task(const std::string& content = "", const bool done = false)
        : content(content), done(done) {}
};

void to_json(json& j, const Task& t) {
    j = json{{"content", t.content}, {"done", t.done}};
}

void from_json(const json& j, Task& t) {
    j.at("content").get_to(t.content);
    j.at("done").get_to(t.done);
}

class TodoList {
private:
    std::vector<Task> tasks;

    void load_from_file() {
        tasks.clear();
        std::ifstream inFile("tasks.json");
        if (inFile.is_open()) {
            json j;
            try {
                inFile >> j;
                for (const auto& item : j) {
                    Task t;
                    t.content = item.at("content").get<std::string>();
                    t.done = item.at("done").get<bool>();
                    tasks.push_back(t);
                }
            } catch (...) {
                std::cerr << "Error reading JSON file\n";
            }
        }
        inFile.close();
    }

    void save_to_file() {
        std::ofstream outFile("tasks.json");
        if (outFile.is_open()) {
            json j = tasks;
            outFile << j.dump(4);
        }
    }

public:
    TodoList() {
        try {
            load_from_file();
        } catch (...) {
            std::cerr << "Warning: could not load task from json file :<\n";
        }
    }

    void addTask() {
        std::cout << "Enter new task:\n";
        std::string new_task;
        std::getline(std::cin, new_task);
        tasks.push_back({new_task, false});
        save_to_file();
    }

    void show_all_Tasks() {
        if (tasks.empty()) {
            std::cout << "No tasks yet.\n";
            return;
        }

        int i = 1;
        for (const auto& item : tasks) {
            std::cout << i << ". " << item.content << " --- "
                      << (item.done ? "[done]" : "[not done]") << '\n';
            i++;
        }
    }

    void mark_Task() {
        if (tasks.empty()) {
            std::cout << "No tasks to mark.\n";
            return;
        }

        std::cout << "Which task to mark done?\n";
        show_all_Tasks();
        int index = get_variant(tasks.size()) - 1;
        tasks[index].done = true;
        save_to_file();
    }

    void delete_a_task() {
        if (tasks.empty()) {
            std::cout << "No tasks to delete.\n";
            return;
        }

        std::cout << "Which task to delete?\n";
        show_all_Tasks();
        int index = get_variant(tasks.size()) - 1;
        tasks.erase(tasks.begin() + index);
        save_to_file();
    }

    void save_and_exit() {
        save_to_file();
        std::cout << "Tasks saved. Bye!\n";
    }
};

void print_menu() {
    std::cout << "\n======= TODO LIST =======\n";
    std::cout << "1. Show all tasks\n";
    std::cout << "2. Add a task\n";
    std::cout << "3. Mark task as done\n";
    std::cout << "4. Delete a task\n";
    std::cout << "5. Save and exit\n";
    std::cout << "==========================\n";
    std::cout << "Choose an action: ";
}

int main() {
    TodoList todo;
    int variant;

    do {
        print_menu();
        variant = get_variant(5);

        switch (variant) {
            case 1: todo.show_all_Tasks(); break;
            case 2: todo.addTask(); break;
            case 3: todo.mark_Task(); break;
            case 4: todo.delete_a_task(); break;
            case 5: todo.save_and_exit(); break;
        }

    } while (variant != 5);

    return 0;
}
