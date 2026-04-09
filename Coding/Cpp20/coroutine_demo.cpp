#include <iostream>
#include <coroutine>
#include <future>
using namespace std;

/*
C++20引入了协程（coroutines），提供了一种更简洁和高效的方式来编写异步代码。
协程允许我们在函数中使用co_await、co_yield和co_return等关键字来实现异步操作，而不需要使用回调函数或复杂的状态机。
协程的核心概念包括：
协程句柄（coroutine handle）：用于管理协程的状态和控制流。
协程承诺（promise）：用于定义协程的返回类型和行为。
协程挂起点（suspension point）：协程在执行过程中可以挂起，并在需要时恢复执行。
协程的使用可以大大简化异步编程，使代码更易读和维护。
协程的特点：
1. 简洁性：协程允许我们以同步的方式编写异步代码
2. 可读性：协程的代码结构更清晰，易于理解和维护
3. 性能：协程的实现通常比传统的回调函数更高效，因为它们避免了不必要的上下文切换和内存分配。
*/


// 简单的协程返回类型
// Task是一个简单的协程返回类型，包含一个promise_type结构体和一个coroutine_handle成员变量。
// promise_type定义了协程的行为，包括初始挂起、最终挂起、返回值和异常处理等。
// coroutine_handle用于管理协程的状态和控制流。
struct Task {
    struct promise_type {
        // get_return_object()函数返回一个Task对象,
        // 它使用coroutine_handle::from_promise(*this)创建一个协程句柄，
        // 并将当前的promise对象作为参数传递给它。
        Task get_return_object() {
            // 使用coroutine_handle::from_promise(*this)创建一个协程句柄，并将当前的promise对象作为参数传递给它。
            return Task{handle_type::from_promise(*this)};
        }
        // initial_suspend()函数和final_suspend()函数分别定义了协程的初始挂起和最终挂起行为。
        // 在这个示例中，initial_suspend()返回suspend_never{}，表示协程在创建时不会挂起，
        // 而final_suspend()返回suspend_never{}，表示协程在结束时也不会挂起。
        suspend_never initial_suspend() {
            return {};
        }
        suspend_never final_suspend() noexcept {
            return {};
        }
        // return_void()函数和unhandled_exception()函数分别定义了协程的返回值和异常处理行为。
        void return_void() {}
        void unhandled_exception() {}
    };
    
    // handle_type是一个类型别名，表示协程句柄的类型。
    // 它使用coroutine_handle<promise_type>来定义，表示这个协程句柄与promise_type相关联。
    using handle_type = coroutine_handle<promise_type>;
    handle_type handle; // 协程句柄成员变量，用于管理协程的状态和控制流
    
    // Task的构造函数和析构函数分别用于创建和销毁协程对象。
    Task(handle_type h) : handle(h) {}
    ~Task() {
        if (handle) handle.destroy();
    }
    
    // resume()函数用于恢复协程的执行。当调用resume()时，协程将继续执行，直到遇到下一个挂起点或结束。
    void resume() {
        handle.resume();
    }
};

// 协程函数
// coroutine_example()函数是一个简单的协程示例，使用了co_await关键字来实现挂起和恢复的行为。
Task coroutine_example() {
    cout << "Coroutine started" << endl;
    // co_await suspend_always{}; 表示协程在这个点上会挂起，并且每次调用resume()时都会恢复执行。
    co_await suspend_always{};
    cout << "Coroutine resumed" << endl;
    // 再次使用co_await suspend_always{}，表示协程在这个点上再次挂起，并且每次调用resume()时都会恢复执行。
    co_await suspend_always{};
    cout << "Coroutine finished" << endl;
}

// 生成器协程示例（使用 co_yield）
// Generator是一个生成器协程的示例，使用了co_yield关键字来实现生成器的行为。
struct Generator {
    struct promise_type {
        int current_value;
        
        // get_return_object()函数返回一个Generator对象，它使用coroutine_handle::from_promise(*this)创建一个协程句柄，并将当前的promise对象作为参数传递给它。
        Generator get_return_object() {
            return Generator{handle_type::from_promise(*this)};
        }
        // initial_suspend()函数和final_suspend()函数分别定义了协程的初始挂起和最终挂起行为。
        suspend_always initial_suspend() { return {}; }
        // final_suspend()函数返回suspend_always{}，表示协程在结束时会挂起，等待外部调用resume()来销毁协程对象。
        suspend_always final_suspend() noexcept { return {}; }
        // yield_value(int value)函数定义了当协程使用co_yield时的行为。它将当前的值存储在current_value成员变量中，并返回suspend_always{}，表示协程在这个点上会挂起，等待外部调用resume()来继续执行。
        suspend_always yield_value(int value) {
            current_value = value;  // 将当前的值存储在current_value成员变量中
            return {};  // 
        }
        void return_void() {}   // return_void()函数定义了当协程使用co_return时的行为。在这个示例中，它没有返回值，因此直接返回void。
        void unhandled_exception() {}   // unhandled_exception()函数定义了当协程抛出异常时的行为。在这个示例中，它没有处理异常，因此直接返回void。
    };
    
    using handle_type = coroutine_handle<promise_type>;
    handle_type handle;
    
    // Generator的构造函数和析构函数分别用于创建和销毁生成器对象。在析构函数中，如果协程句柄存在，则调用handle.destroy()来销毁协程对象，释放相关资源。
    Generator(handle_type h) : handle(h) {}
    ~Generator() { if (handle) handle.destroy(); }
    
    // next()函数用于获取生成器的下一个值。当调用next()时，协程将继续执行，直到遇到下一个co_yield点或结束。如果协程没有结束，则返回true；否则返回false。
    bool next() {
        if (!handle.done()) {
            handle.resume();
            return true;
        }
        return false;
    }
    
    // current_value()函数用于获取生成器当前的值。它通过访问协程句柄的promise对象来获取当前的值，并返回给调用者。
    int current_value() { return handle.promise().current_value; }
};

// 使用 co_yield 的协程
// fibonacci_generator()函数是一个使用co_yield关键字的生成器协程示例，它生成斐波那契数列。
Generator fibonacci_generator() {
    int a = 0, b = 1;
    while (true) {
        co_yield a; // 使用co_yield关键字将当前的值a返回给调用者，并挂起协程，等待外部调用resume()来继续执行。
        int temp = a;   // 在每次生成一个新的斐波那契数之前，先将当前的值a存储在temp变量中，然后更新a和b的值，以生成下一个斐波那契数。
        a = b;
        b = temp + b;
    }
}

int main() {
    // 第一个示例：基本协程
    auto task = coroutine_example();
    cout << "After creating coroutine" << endl;
    task.resume();  // 第一次调用resume()，协程从初始挂起点恢复执行，输出"Coroutine resumed"
    cout << "After first resume" << endl;
    task.resume();  // 第二次调用resume()，协程从第二个挂起点恢复执行，输出"Coroutine finished"
    cout << "After second resume" << endl;
    
    cout << "---------" << endl;
    
    // 第二个示例：生成器协程
    auto gen = fibonacci_generator();   // 创建一个fibonacci_generator协程对象gen，它将生成斐波那契数列。
    cout << "Fibonacci sequence: ";
    for (int i = 0; i < 10 && gen.next(); ++i) {    // 使用一个循环来获取生成器的前10个值。在每次迭代中，调用gen.next()来继续执行协程并获取下一个值。如果协程没有结束，则输出当前的值。
        // gen.current_value()函数用于获取生成器当前的值。它通过访问协程句柄的promise对象来获取当前的值，并返回给调用者。
        cout << gen.current_value() << " ";
    }
    cout << endl;
    
    return 0;
}