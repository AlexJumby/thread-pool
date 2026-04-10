#include "thread-pool.hpp"
#include <random>

std::string get_thread_id() {
    auto myid = std::this_thread::get_id();
    std::stringstream ss;
    ss << myid;
    std::string mystr = ss.str();
    return mystr;
}

void add_task(int a, int b) {
    int c = a + b;
    std::ostringstream ss;
    ss << "[add_task]" << " " << a << " + " << b << " = " << c << "\n";
    //std::cout << ss.str();
}

void many_add_task(int count) {
    std::mt19937 rng(std::random_device{}());
    std::uniform_int_distribution<int> dist(1, 100);
    int sum = 0;
    for (int i = 0; i < count; ++i) {
        sum += dist(rng);
    }
    std::ostringstream ss;
    ss << "[many_add_task]" << " " << sum << "\n";
    std::cout << ss.str();
}

void slow_task(int task_id) {
    std::mt19937 rng(std::random_device{}());
    std::uniform_int_distribution<int> dist(500, 3000);
    int ms = dist(rng);
    std::ostringstream ss;
    ss << "[slow_task]" << " " << task_id << " " << "начало" << " " << "sleep" << " " << " " << ms << " " << "мс" << "\n";
    //std::cout << ss.str();
    ss.str("");
    std::this_thread::sleep_for(std::chrono::milliseconds(ms));
    ss << "[slow_task]" << " " << task_id << " " << "завершена" << "\n";
    //std::cout << ss.str();
}

void monitoring_loop(ThreadPool& pool, std::atomic<bool>& running) {
    while (running.load()) {
        auto states = pool.get_worker_states();
        size_t completed = pool.get_completed_count();
        
        std::ostringstream ss;
        ss << "\033[2J\033[H";
        ss << "\n--- Мониторинг ---\n";
        ss << "Выполнено задач: " << completed << "\n";

        for (size_t i = 0; i < states.size(); ++i) {
            ss << "Worker " << i << ": ";
            if (states[i].current_task.empty()) {
                ss << "idle";
            } else {
                ss << states[i].current_task;
            }
            ss << "\n"; 
        }

        std::cout << ss.str() << std::flush;
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
    }
}

int main() {

    ThreadPool pool(4); // Создаем пул с N потоками на вход

    std::cout << "Тред пул создан\n";

    std::atomic<bool> monitor_running{true};
    std::thread monitor_thread(monitoring_loop, std::ref(pool), std::ref(monitor_running));

    bool running = true;
    int slow_counter = 0;

    while (running) {
        char cmd;
        std::cin >> cmd;

        switch (cmd) {
            case '1':
                pool.enqueue("add_task", [](){ add_task(2, 4); });
                break;
            case '2':
                pool.enqueue("many_add", [](){ many_add_task(100000); });
                break;
            case '3':
                ++slow_counter;
                pool.enqueue("slow_task" + std::to_string(slow_counter),
                            [slow_counter](){ slow_task(slow_counter); });
                break;
            case 'q':
                running = false;
                break;
            case 's':
                pool.shutdown();
                running = false;
                break;
        }
    }

    std::cout << "Выполнено задач" << pool.get_completed_count() << "\n";
    monitor_running = false;
    monitor_thread.join();
    return 0;
}