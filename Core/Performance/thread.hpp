/************************************/
/*           thread.hpp             */
/*                                  */
/*       RatLab Game Engine         */
/*          2026-Present            */
/*         On MIT License           */
/************************************/

#pragma once

#include "../Essentials/essentials.hpp"
#include <thread>

#if WINDOWS_ENABLED
#include <windows.h>
#elif defined(LINUX_ENABLED)
#include <pthread.h>
#include <sched.h>
#endif

struct Thread {
    private:
    std::thread thread_handle;

    public:
    // Constructors - Deconstructors

    Thread() {}

    template<typename Callable, typename... Args>
    Thread(Callable&& p_fn, Args&&... p_args)
        : thread_handle(std::forward<Callable>(p_fn), std::forward<Args>(p_args)...) {}

    ~Thread() {
        if (thread_handle.joinable()) {
            thread_handle.join();
        }
    }

    // Non-copyable

    Thread(const Thread&) = delete;
    Thread& operator=(const Thread&) = delete;

    // Movable

    Thread(Thread&& p_other) noexcept : thread_handle(std::move(p_other.thread_handle)) {}

    Thread& operator=(Thread&& p_other) noexcept {
        if (this != &p_other) {
            if (thread_handle.joinable()) {
                thread_handle.join();
            }
            thread_handle = std::move(p_other.thread_handle);
        }
        return *this;
    }

    // Core Affinity

    void set_affinity(u32 p_core) {
        if (!thread_handle.joinable()) return;
        #if WINDOWS_ENABLED
        SetThreadAffinityMask(thread_handle.native_handle(), 1ULL << p_core.get_value());
        #elif defined(LINUX_ENABLED)
        cpu_set_t set;
        CPU_ZERO(&set);
        CPU_SET(p_core.get_value(), &set);
        pthread_setaffinity_np(thread_handle.native_handle(), sizeof(cpu_set_t), &set);
        #endif
    }

    void set_affinity_mask(u64 p_mask) {
        if (!thread_handle.joinable()) return;
        #if WINDOWS_ENABLED
        SetThreadAffinityMask(thread_handle.native_handle(), p_mask.get_value());
        #elif defined(LINUX_ENABLED)
        cpu_set_t set;
        CPU_ZERO(&set);
        for (u32 i = 0; i < 64; i += 1) {
            if ((p_mask.get_value() >> i.get_value()) & 1) {
                CPU_SET(i.get_value(), &set);
            }
        }
        pthread_setaffinity_np(thread_handle.native_handle(), sizeof(cpu_set_t), &set);
        #endif
    }

    // Static

    static u32 get_core_count() {
        return u32(std::thread::hardware_concurrency());
    }

    // Lifecycle

    template<typename Callable, typename... Args>
    void start(Callable&& p_fn, Args&&... p_args) {
        if (thread_handle.joinable()) {
            thread_handle.join();
        }
        thread_handle = std::thread(std::forward<Callable>(p_fn), std::forward<Args>(p_args)...);
    }

    void join() {
        if (thread_handle.joinable()) {
            thread_handle.join();
        }
    }

    void detach() {
        if (thread_handle.joinable()) {
            thread_handle.detach();
        }
    }

    // Accessors

    bool is_running() const { return thread_handle.joinable(); }
    bool is_joinable() const { return thread_handle.joinable(); }
    std::thread& get_handle() { return thread_handle; }
};
