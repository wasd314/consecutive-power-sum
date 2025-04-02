#include <algorithm>
#include <filesystem>
#include <fstream>
#include <iostream>

#include "../../solution_cpp/e2.hpp"

int main()
{
    namespace fs = std::filesystem;
    using namespace wasd314::e2;
    using namespace std::literals::string_literals;

    fs::path here = fs::absolute(".").parent_path();
    fs::path problem_root = here.parent_path();
    fs::path template_root_ac = here / "template_cp_ac";
    fs::path template_root_tle = here / "template_cp_tle";

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

    auto gen = [&](fs::path template_root, auto& solvers, int i) {
        auto solver = solvers[i];
        std::string name = solver.name;
        fs::path target_root = problem_root / (auto_prefix + name);
        fs::create_directory(target_root);
        fs::copy_file(template_root / "SOLUTION", target_root / "SOLUTION", fs::copy_options::update_existing);

        std::ifstream from(template_root / "main.cpp");
        std::ofstream to(target_root / "main.cpp");
        std::string line;
        while (std::getline(from, line)) {
            auto pos = line.find("[0]");
            if (pos != std::string::npos) {
                line = line.replace(pos, 3, std::format("[{}]", i));
            }
            to << line << '\n';
        }
        from.close();
        to.close();
        std::cout << "generated: " << name << std::endl;
    };

    remove_prev();
    std::string line;
    {
        std::cout << "ac[" << solvers_ac.size() << "]: ";
        std::getline(std::cin, line);
        auto todo = wasd314::parse_range(solvers_ac.size(), line);
        for (int i : todo) {
            gen(template_root_ac, solvers_ac, i);
        }
    }
    {
        std::cout << "tle[" << solvers_tle.size() << "]: ";
        std::getline(std::cin, line);
        auto todo = wasd314::parse_range(solvers_tle.size(), line);
        for (int i : todo) {
            gen(template_root_tle, solvers_tle, i);
        }
    }
}
