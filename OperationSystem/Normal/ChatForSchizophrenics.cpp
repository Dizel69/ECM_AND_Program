//Задание 3. Чат для шизофреников.

#include <iostream>
#include <thread>
#include <fstream>
#include <string>
#include <chrono>
#include <Windows.h>

HANDLE Sem1 = CreateSemaphore(NULL, 1, 1, L"Semaphore1");
HANDLE Sem2 = CreateSemaphore(NULL, 1, 1, L"Semaphore2");

char* arg;

int write() 
{
    std::ofstream outFile("buba.txt", std::ios::app);
    std::string message;

    while (true) 
    {

        WaitForSingleObject(Sem1, INFINITE);
        std::this_thread::sleep_for(std::chrono::seconds(1));
        std::cout << "Написано сообщение: ";
        std::getline(std::cin, message);

        outFile << "Пользователь [" << arg[0] << "] Написал: " << message << std::endl;
        outFile.flush();

        ReleaseSemaphore(Sem1, 1, NULL);
    }

    outFile.close();

    return 1;
}

int read() 
{
    while (true) 
    {
        std::ifstream inFile("buba.txt", std::ios::ate);

        WaitForSingleObject(Sem2, INFINITE);

        inFile.seekg(0, std::ios::end);
        long long currentSize = inFile.tellg();

        std::string lastMessage;

        while (true) 
        {
            inFile.seekg(0, std::ios::end);
            long long newSize = inFile.tellg();

            std::this_thread::sleep_for(std::chrono::milliseconds(300));

            if (newSize > currentSize) 
            {
                inFile.seekg(0, std::ios::beg);
                std::string line;
                while (std::getline(inFile, line)) 
                {
                    lastMessage = line;
                }
                break;
            }
        }

        std::cout << lastMessage << std::endl;

        std::this_thread::sleep_for(std::chrono::seconds(2));
        ReleaseSemaphore(Sem2, 1, NULL);
        inFile.close();
    }

    return 1;
}

int main(int argc, char* argv[])
{
    setlocale(0, "rus");
    arg = argv[2];

    if (arg[0] == '1') //Легендарный метод Джаника с блокировкой чтения, а не записи.
    {

        std::thread writeThread(write);
        std::this_thread::sleep_for(std::chrono::seconds(5));
        std::thread readThread(read);

        writeThread.join();
        readThread.join();
    }
    else if (arg[0] == '2') 
    {

        std::thread readThread(read);
        std::thread writeThread(write);

        readThread.join();
        writeThread.join();
    }
}