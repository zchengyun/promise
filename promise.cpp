#include <iostream>
#include <thread>   
#include <string>  
#include <future>  
#include <chrono>   
using namespace std::chrono;
//一个std::promise实例只能与一个std::future关联共享状态
void read(std::future<std::string>* future) {
    // future会一直阻塞，直到有值到来
    std::cout << future->get() << std::endl;
}

void read1(std::future<std::string>* future) {
    try {
        future->get();
    }
    catch (std::future_error& e) {
        std::cerr << e.code() << "\n" << e.what() << std::endl;
    }
}

int main() {
    // promise 相当于生产者
    std::promise<std::string> promise;
    // future 相当于消费者
    std::future<std::string> future = promise.get_future();
    // 另一线程中通过future来读取promise的值
    std::thread thread(read, &future);
    // 让read等一会儿3s
    std::this_thread::sleep_for(seconds(1));
    promise.set_value("hello future");

    std::thread thread1;
    {
        std::promise<std::string> promise1;
        std::future<std::string> future1 = promise1.get_future();
        thread1=std::thread(read1, &future1);
    } 
    // 等待线程执行完成
    thread.join();
    thread1.join();

    return 0;
}