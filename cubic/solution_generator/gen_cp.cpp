#include <filesystem>
#include <fstream>
#include <iostream>

#include "../../solution_cpp/e3.hpp"
#include "../../solution_cpp/utility.hpp"

int main(int argc, char** argv)
{
    namespace fs = std::filesystem;
    using wasd314::e3::solvers;
    fs::path here = fs::absolute(".").parent_path();
    fs::path problem_root = here.parent_path();
    fs::path template_root = here / "template_cp";

    const std::string auto_prefix = "__auto_cp__";

    auto remove_prev = [&] {
        std::vector<fs::path> to_remove;
        for (const auto& e : fs::directory_iterator(problem_root)) {
            std::string name = e.path().filename().string();
            if (e.is_directory() && name.starts_with(auto_prefix)) {
                to_remove.push_back(name);
            }
        }
        for (const auto& name : to_remove) {
            fs::remove_all(problem_root / name);
            std::cout << "removed: " << name << std::endl;
        }
    };
    auto gen = [&](int i) {
        auto solver = solvers[i];
        std::string name = solver.get().name;
        fs::path target_root = problem_root / (auto_prefix + name);
        fs::create_directory(target_root);
        fs::copy_file(template_root / "SOLUTION", target_root / "SOLUTION", fs::copy_options::update_existing);

        std::ifstream from(template_root / "main.cpp");
        std::ofstream to(target_root / "main.cpp");
        std::string line;
        while (std::getline(from, line)) {
            auto pos = line.find("dummy");
            if (pos != std::string::npos) {
                line = line.replace(pos, 5, std::format("solvers[{}]", i));
            }
            to << line << '\n';
        }
        from.close();
        to.close();
        std::cout << "generated: " << name << std::endl;
    };

    std::vector<int> todo;
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        auto pos = arg.find("-");
        if (arg == "-") {
            for (int e = 0; e < (int)solvers.size(); ++e) {
                todo.push_back(e);
            }
        } else if (pos != std::string::npos) {
            int l = std::stoi(arg.substr(0, pos));
            int r = std::stoi(arg.substr(pos + 1));
            l = std::max(l, 0);
            r = std::min(r + 1, (int)solvers.size());
            for (int e = l; e < r; ++e) {
                todo.push_back(e);
            }
        } else {
            int e = std::stoi(arg);
            if (0 <= e && e < (int)solvers.size()) {
                todo.push_back(e);
            }
        }
    }
    std::ranges::sort(todo);
    auto dup = std::ranges::unique(todo);
    todo.erase(dup.begin(), dup.end());

    remove_prev();
    for (int i : todo) {
        gen(i);
    }
}
