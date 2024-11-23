#ifndef PRIORITY_COLLECTION_H
#define PRIORITY_COLLECTION_H

#include <unordered_map>
#include <map>
#include <set>
#include <utility>
#include <vector>

template <typename T>
class PriorityCollection {
public:
    using Id = int;  // Тип ідентифікатора

    // Додати елемент з нульовим пріоритетом
    Id Add(T object) {
        int new_id = current_id++;
        elements[new_id] = { std::move(object), 0 };
        priorities[0].insert(new_id);
        return new_id;
    }

    // Додати всі елементи з діапазону [range_begin, range_end)
    template <typename ObjInputIt, typename IdOutputIt>
    void Add(ObjInputIt range_begin, ObjInputIt range_end, IdOutputIt ids_begin) {
        for (auto it = range_begin; it != range_end; ++it) {
            *ids_begin++ = Add(std::move(*it));
        }
    }

    // Перевірка, чи є ідентифікатор дійсним
    bool IsValid(Id id) const {
        return elements.count(id) > 0;
    }

    // Отримати об'єкт за ідентифікатором
    const T& Get(Id id) const {
        return elements.at(id).first;
    }

    // Збільшити пріоритет об'єкта на 1
    void Promote(Id id) {
        auto& element = elements.at(id);  // Явне звернення до пари
        T& obj = element.first;
        int& priority = element.second;

        priorities[priority].erase(id);
        if (priorities[priority].empty()) {
            priorities.erase(priority);
        }
        ++priority;
        priorities[priority].insert(id);
    }

    // Отримати об'єкт з максимальним пріоритетом і його пріоритет
    std::pair<const T&, int> GetMax() const {
        auto it = std::prev(priorities.end());
        Id id = *it->second.rbegin();
        return { elements.at(id).first, it->first };
    }

    // Видобути об'єкт з максимальним пріоритетом
    std::pair<T, int> PopMax() {
        auto it = std::prev(priorities.end());
        Id id = *it->second.rbegin();
        int priority = it->first;

        T object = std::move(elements.at(id).first);
        elements.erase(id);
        priorities[priority].erase(id);

        if (priorities[priority].empty()) {
            priorities.erase(priority);
        }

        return { std::move(object), priority };
    }

private:
    std::unordered_map<Id, std::pair<T, int>> elements;  // Зберігає елементи та їх пріоритети
    std::map<int, std::set<Id>> priorities;  // Зберігає ідентифікатори елементів відповідно до пріоритету
    Id current_id = 0;  // Поточний унікальний ідентифікатор
};

#endif // PRIORITY_COLLECTION_H
