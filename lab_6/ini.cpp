#include "ini.h"
#include <sstream>
#include <stdexcept>

namespace Ini {

Section& Document::AddSection(std::string name) {
    return sections[name];
}

const Section& Document::GetSection(const std::string& name) const {
    auto it = sections.find(name);
    if (it == sections.end()) {
        throw std::out_of_range("Секція не знайдена: " + name);
    }
    return it->second;
}

size_t Document::SectionCount() const {
    return sections.size();
}

Document Load(std::istream& input) {
    Document doc;
    std::string line;
    Section* current_section = nullptr;

    while (std::getline(input, line)) {
        // Ігнорування порожних рядків
        if (line.empty()) {
            continue;
        }

        if (line.front() == '[' && line.back() == ']') {
            // Заголовок секції
            std::string section_name = line.substr(1, line.size() - 2);
            current_section = &doc.AddSection(section_name);
        } else {
            // Пара ключ-значення
            if (!current_section) {
                throw std::logic_error("Пара ключ-значення без секції");
            }

            auto eq_pos = line.find('=');
            if (eq_pos == std::string::npos) {
                throw std::logic_error("Неправильна пара ключ-значення: " + line);
            }

            std::string key = line.substr(0, eq_pos);
            std::string value = line.substr(eq_pos + 1);
            current_section->emplace(std::move(key), std::move(value));
        }
    }

    return doc;
}

}