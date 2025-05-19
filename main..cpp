#include <atomic>
#include <immintrin.h>
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

    Task(const std::string& content = "", const bool done = false):
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
  private:
    std::vector<Task> tasks;

    void load_from_file() {
        std::ifstream inFile("tasks.json");
        if (inFile.is_open()) {
            json j;
            try {
                inFile >> j;
                for (const auto& item : j) {
                    Task t;
                    t.content = item.at({"content"}).get<std::string>();
                    t.done = item.at({"done"}).get<bool>();
                    TodoList::tasks.push_back(t);
                }
            } catch (...) {
                std::cerr << "Error cin json-file\n";
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

    void addTask() {
        std::string new_task;
        std::getline(std::cin, new_task);

        load_from_file();
        tasks.push_back({new_task, false});
        save_to_file();
    }

    void show_all_Tasks() {
        load_from_file();
        int i = 1;
        for(auto &Item : tasks) {
            std::cout << i << ". " <<Item.content << "-------" << Item.done << std::endl;
            i++;
        }
    }

    void mark_Task() {
        std::cout << "Which Task will you choose?\n";
        show_all_Tasks();
        int index = get_variant(tasks.size());
        tasks[index].done = true;
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
    std::cout << "Chose an action: ";
}


int main() {
    setlocale(LC_ALL, "ru");

    int variant;

    do {
        print_menu();

        variant = get_variant(5);

        switch (variant) {
            case 1:
        }

    } while(variant != 5);

    return 0;
}
