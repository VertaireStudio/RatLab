/************************************/
/*        thread_test.cpp           */
/*                                  */
/*       RatLab Game Engine         */
/*          2026-Present            */
/*         On MIT License           */
/************************************/

#include "tester.hpp"
#include "../Core/Performance/thread.hpp"

#include <atomic>
#include <cstdio>

// get_core_count - must return a positive value.

void test_core_count(Tester& r_tester) {
    u32 count = Thread::get_core_count();
    r_tester.test_greater_or_equal(count, u32(1));
}

// lifecycle - start, join, is_joinable state transitions.

void test_lifecycle(Tester& r_tester) {
    std::atomic<bool> executed(false);

    Thread t;
    r_tester.test_false(t.is_joinable());

    t.start([&executed]() { executed.store(true); });
    r_tester.test_true(t.is_joinable());

    t.join();
    r_tester.test_false(t.is_joinable());
    r_tester.test_true(executed.load());
}

// start_and_join_multiple - reuse the same Thread for multiple tasks.

void test_start_and_join_multiple(Tester& r_tester) {
    std::atomic<int> counter(0);

    Thread t;
    t.start([&counter]() { counter.fetch_add(1); });
    t.join();

    t.start([&counter]() { counter.fetch_add(1); });
    t.join();

    r_tester.test_equal(i32(counter.load()), i32(2));
}

// affinity - set_affinity and set_affinity_mask execute without crashing.

void test_affinity(Tester& r_tester) {
    Thread t;
    t.start([]() {
        u32 cores = Thread::get_core_count();
        Thread self;
        if (cores.get_value() > 0) {
            self.set_affinity(u32(0));
        }
        if (cores.get_value() > 1) {
            self.set_affinity_mask(u64(0x3));
        }
    });
    t.join();
    r_tester.test_true(true);
}

// move - Thread can be moved and the source becomes empty.

void test_move(Tester& r_tester) {
    std::atomic<bool> executed(false);

    Thread t;
    t.start([&executed]() { executed.store(true); });

    Thread t2(static_cast<Thread&&>(t));
    r_tester.test_false(t.is_joinable());

    t2.join();
    r_tester.test_true(executed.load());
}

int main() {
    Tester tester;

    tester.begin_group("core_count");     test_core_count(tester);              tester.end_group();
    tester.begin_group("lifecycle");      test_lifecycle(tester);               tester.end_group();
    tester.begin_group("multiple");       test_start_and_join_multiple(tester); tester.end_group();
    tester.begin_group("affinity");       test_affinity(tester);                tester.end_group();
    tester.begin_group("move");           test_move(tester);                    tester.end_group();

    tester.print_results();
    return tester.all_passed() ? 0 : 1;
}
