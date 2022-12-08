#include <chrono>
#include <functional>
#include <iomanip>
#include <iostream>
#include <map>
 
const int n_operations = 1000000;

// delta -- разница между ближайшей левой позиции к hint
// и позицией вставки.

// Обычная вставка по индексу
std::size_t map_insert() {
    std::map<int, char> map;
    for (int i = 0; i < n_operations; ++i) {
        map.insert({i, 'a'});
    }
    return map.size();
}

// Вставка с использованием hint. delta=1
std::size_t map_insert_hint() {
    std::map<int, char> map;
    auto it = map.begin();
    for (int i = 0; i < n_operations; ++i) {
        map.insert(it, {i, 'a'});
        it = map.end();
    }
    return map.size();
}

// Вставка по итератору в обратном порядке.
// Работает быстрее, так как hint делает вставку
// в позицию, максимально близкую к той, которая
// предшествует hint: delta=0
std::size_t map_insert_hint_reverse() {
    std::map<int, char> map;
    auto it = map.begin();
    for (int i = n_operations; i > 0; --i) {
        map.insert(it, {i, 'a'});
        it = map.begin();
    }
    return map.size();
}

// Вставка с присваиванием позиции вставки. delta=2
// При использовании -O3 медленнее, чем просто по индексу
std::size_t map_insert_hint_closest() {
    std::map<int, char> map;
    auto it = map.begin();
    for (int i = 0; i < n_operations; ++i) {
        it = map.insert(it, {i, 'a'});
    }
    return map.size();
}


// Обратная вставка с присваиванием позиции вставки. delta=0
std::size_t map_insert_hint_closest_reverse() {
    std::map<int, char> map;
    auto it = map.begin();
    for (int i = n_operations; i > 0; --i) {
        it = map.insert(it, {i, 'a'});
    }
    return map.size();
}

// Вставка в один и тот же элемент по индексу
std::size_t map_insert_same() {
    std::map<int, char> map;
    for (int i = n_operations; i > 0; --i) {
        map.insert({100, 'a'});
    }
    return map.size();
}

// Вставка в один и тот же элемент с hint. delta=0
std::size_t map_insert_same_hint() {
    std::map<int, char> map;
    map.insert({100, 'a'});

    auto it = map.find(101);
    for (int i = 0; i > n_operations; ++i) {
        it = map.insert(it, {100, 'a'});
    }
    return map.size();
}

// Вставка в один и тот же элемент с hint. delta=1
std::size_t map_insert_same_hint_100() {
    std::map<int, char> map;
    map.insert({100, 'a'});
    auto it = map.find(100);
    
    for (int i = 0; i > n_operations; ++i) {
        it = map.insert(it, {100, 'a'});
    }
    return map.size();
}

void timeit(std::function<std::size_t()> map_test, std::string what = "") {
    auto start = std::chrono::system_clock::now();
    std::size_t mapsize = map_test();
    auto stop = std::chrono::system_clock::now();
    std::chrono::duration<double, std::milli> time = stop - start;
    if (what.size() > 0 && mapsize > 0) {
        std::cout << std::setw(5) << time.count() << "  ms for " << what << '\n';
    }
}
 
int main() {
    std::cout << std::fixed << std::setprecision(2);
    timeit(map_insert); // stack warmup
    timeit(map_insert, "plain insert");
    timeit(map_insert_hint, "insert with hint");
    timeit(map_insert_hint_reverse, "reversed insert with hint");
    timeit(map_insert_hint_closest, "insert using returned iterator");
    timeit(map_insert_hint_closest_reverse, "reversed insert using returned iterator");
    timeit(map_insert_same, "insert same");
    timeit(map_insert_same_hint, "insert with same hint and delta = 0");
    timeit(map_insert_same_hint_100, "insert with same hint and delta = 1");
}