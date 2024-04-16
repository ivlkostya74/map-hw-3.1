// ConsoleApplication2.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//
#include <iostream>
#include <vector>
#include <future>
#include <utility>


int findMin(const std::vector<int>& vec, size_t start, size_t end) {
    int min = vec[start];
    for (size_t i = start + 1; i < end; ++i) {
        if (vec[i] < min) {
            min = vec[i];
        }
    }
    return min;
}


void findMinAsync(const std::vector<int>& vec, size_t start, size_t end, std::promise<int>&& prom) {
    int min = findMin(vec, start, end);
    prom.set_value(min); 
}

// Функция сортировки выбором
void selectionSort(std::vector<int>& vec) {
    size_t n = vec.size();
    for (size_t i = 0; i < n; ++i) {
        
        std::promise<int> prom;
        std::future<int> fut = prom.get_future();

        
        std::thread thread(findMinAsync, std::ref(vec), i, n, std::move(prom));

        int min_value = fut.get(); 
        thread.join(); 

       
        size_t min_index = i;
        for (size_t j = i; j < n; ++j) {
            if (vec[j] == min_value) {
                min_index = j;
                break;
            }
        }
       
        std::swap(vec[i], vec[min_index]);
    }
}

int main() {
    std::vector<int> vec = { 64, 25, 12, 22, 11 };
    std::cout << "Original array: ";
    for (int num : vec) {
        std::cout << num << " ";
    }
    std::cout << std::endl;

    selectionSort(vec);

    std::cout << "Sorted array: ";
    for (int num : vec) {
        std::cout << num << " ";
    }
    std::cout << std::endl;

    return 0;
}
