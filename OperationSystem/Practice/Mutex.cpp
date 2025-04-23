//Задание 2. Семафор

#include <thread> 
#include <iostream> 
#include <chrono> 
#include <mutex> 
#include <random> 

std::random_device dev;
std::mt19937 rng(dev());
std::uniform_int_distribution<std::mt19937::result_type> dist(100, 300);
std::mutex mut;

int counter1 = 0;
int counter2 = 0;
std::atomic_int counter3 = 0;

void case1()
{
    while (counter1 < 10)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(dist(rng)));
        ++counter1;
    }
}
void case2()
{
    while (true)
    {
        mut.lock();
        if (counter2 < 10)
        {
            ++counter2;
        }
        else
        {
            mut.unlock();
            break;
        }
        mut.unlock();
        std::this_thread::sleep_for(std::chrono::milliseconds(dist(rng)));
    }
}
void case3()
{
    while (counter3.load() < 10)
    {
        ++counter3;
        std::this_thread::sleep_for(std::chrono::milliseconds(dist(rng)));
    }
}

void test1()
{
    std::thread th1(&case1), th2(&case1), th3(&case1), th4(&case1);
    auto begin = std::chrono::system_clock::now();
    th1.join(); th2.join(); th3.join(); th4.join();
    auto end = std::chrono::system_clock::now();
    std::cout << "Тест 1. Результат = " << counter1 << "; Время - " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() << " мс; \n";
}
void test2()
{
    std::thread th1(&case2), th2(&case2), th3(&case2), th4(&case2);
    auto begin = std::chrono::system_clock::now();
    th1.join(); th2.join(); th3.join(); th4.join();
    auto end = std::chrono::system_clock::now();
    std::cout << "Тест 2. Результат = " << counter2 << "; Время - " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() << " мс; \n";
}
void test3()
{
    std::thread th1(&case3), th2(&case3), th3(&case3), th4(&case3);
    auto begin = std::chrono::system_clock::now();
    th1.join(); th2.join(); th3.join(); th4.join();
    auto end = std::chrono::system_clock::now();
    std::cout << "Тест 2. Результат = " << counter3 << "; Время - " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() << " мс; \n";
}

int main()
{
    setlocale(0, "rus");
    test1();
    test2();
    test3();
    return 0;
}