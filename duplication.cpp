#include <iostream>
#include <memory>
#include <vector>
#include <unordered_map>
#include <string>

// Преобразует shared_ptr в unique_ptr, создавая независимые копии строк
std::vector<std::unique_ptr<std::string>> Duplicate(const std::vector<std::shared_ptr<std::string>>& in) {
    std::vector<std::unique_ptr<std::string>> out;

    for (const std::shared_ptr<std::string>& element : in) {
        std::unique_ptr<std::string> unique_string(new std::string(*element));
        out.push_back(std::make_unique<std::string>(*element));
    }

    return out;
}

// Преобразует unique_ptr в shared_ptr
// *in[i] == *out[i]
// Каждое уникальное значение создано только в одном экземпляре и все ссылки ведут на него
std::vector<std::shared_ptr<std::string>> DeDuplicate(const std::vector<std::unique_ptr<std::string>>& in) {
    std::unordered_map<std::string, std::shared_ptr<std::string>> string_map;
    std::vector<std::shared_ptr<std::string>> out;

    for (const std::unique_ptr<std::string>& element : in) {
        if (string_map.find(*element) == string_map.end()) {
            std::shared_ptr<std::string> shared_string(new std::string(*element));
            string_map[*element] = shared_string;
        }
        out.push_back(string_map[*element]);
    }

    return out;
}
