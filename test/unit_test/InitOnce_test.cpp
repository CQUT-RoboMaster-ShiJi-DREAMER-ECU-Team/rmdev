/**
 * @file InitOnce_test.cpp
 * @author 杜以成
 * @date 2025-06-14
 * @brief 单次初始化类测试
 */

#include <algorithm>

#include "rmdev_test_framework.h"

import rmdev.util.InitOnce;
import rmdev.util.math;

static void basicTest()
{
    RMDEV_TEST_ITEM("InitOnce Basic Test");

    using rmdev::InitOnce;

    InitOnce<int> a;
    RMDEV_TEST_ASSERT(a == 0);
    RMDEV_TEST_ASSERT(a() == 0);

    a = 114514;
    RMDEV_TEST_ASSERT(a == 114514);

    a = 1;
    RMDEV_TEST_ASSERT(a() == 114514 && a != 1);

    auto error_code = a.init(2472);
    RMDEV_TEST_ASSERT(error_code == rmdev::ErrorCode::AlreadyExists);
    RMDEV_TEST_ASSERT(a == 114514 && a() != 2472);

    InitOnce<float> f;
    RMDEV_TEST_ASSERT(rmdev::weakEqu(f(), 0.0f));

    error_code = f.init(2.3f);
    RMDEV_TEST_ASSERT(error_code == rmdev::ErrorCode::Success && rmdev::weakEqu(f(), 2.3f));

    f = 114514.1919810f;
    RMDEV_TEST_ASSERT(rmdev::weakEqu(f(), 2.3f));

    error_code = f.init(4235.2);
    RMDEV_TEST_ASSERT(rmdev::weakEqu(f(), 2.3f) && error_code == rmdev::ErrorCode::AlreadyExists);

    InitOnce<short> b(8);
    RMDEV_TEST_ASSERT(b == 8);

    b = 1;
    RMDEV_TEST_ASSERT(b == 8 && b() == 8);

    const auto c = static_cast<InitOnce<signed char>>(2);
    RMDEV_TEST_ASSERT(c == 2 && c() == 2);

    constexpr InitOnce d{2.3f};
    RMDEV_TEST_ASSERT(rmdev::weakEqu<float>(d, 2.3f));
}

constinit rmdev::InitOnce global1{5};
constinit rmdev::InitOnce<int> global2{};
constinit const rmdev::InitOnce global3{5.4};
constexpr rmdev::InitOnce global4{5};
constinit const rmdev::InitOnce<int> global5;

static void globalTest()
{
    RMDEV_TEST_ITEM("InitOnce Global Test");

    global1 = 1;
    RMDEV_TEST_ASSERT(global1() == 5);

    auto err = global1.init(23415);
    RMDEV_TEST_ASSERT(global1 == 5 && err == rmdev::ErrorCode::AlreadyExists);

    RMDEV_TEST_ASSERT(global2 == 0);
    global2 = 2;
    RMDEV_TEST_ASSERT(global2 == 2);

    err = global2.init(23415);
    RMDEV_TEST_ASSERT(global2 == 2 && err == rmdev::ErrorCode::AlreadyExists);

    RMDEV_TEST_ASSERT(rmdev::weakEqu(global3(), 5.4));

    RMDEV_TEST_ASSERT(global4 == 5);

    RMDEV_TEST_ASSERT(global5 == 0);
}

static void moveTest()
{
    RMDEV_TEST_ITEM("Class Move Test");

    class Test final
    {
    public:
        Test()
        {
            p_data = new int[5];
            if (p_data == nullptr) {
                RMDEV_TEST_ASSERT(false);
            }
        }

        Test(const std::initializer_list<int> data)
        {
            p_data = new int[5];
            if (p_data == nullptr) {
                RMDEV_TEST_ASSERT(false);
            }
            auto size = data.size();
            if (size > 5) {
                size = 5;
            }
            std::copy_n(data.begin(), size, p_data);
        }

        Test(const Test& other)
        {
            p_data = new int[5];
            if (p_data == nullptr) {
                RMDEV_TEST_ASSERT(false);
            }
            std::copy_n(other.p_data, 5, p_data);
        }

        Test& operator=(const Test& other)
        {
            if (this != &other) {
                if (p_data == nullptr) {
                    p_data = new int[5];
                    if (p_data == nullptr) {
                        RMDEV_TEST_ASSERT(false);
                    }
                }
                std::copy_n(other.p_data, 5, p_data);
            }
            return *this;
        }

        Test(Test&& other) noexcept
        {
            p_data = other.p_data;
            other.p_data = nullptr;
        }

        Test& operator=(Test&& other) noexcept
        {
            if (this == &other) {
                return *this;
            }

            if (p_data != nullptr) {
                delete p_data;
            }

            p_data = other.p_data;
            other.p_data = nullptr;

            return *this;
        }

        ~Test()
        {
            delete p_data;
            p_data = nullptr;
        }

        bool operator==(const Test& other) const
        {
            return std::equal(p_data, p_data + 5, other.p_data, other.p_data + 5);
        }

        [[nodiscard]] int* data()
        {
            return p_data;
        }

        [[nodiscard]] const int* data() const
        {
            return p_data;
        }

    private:
        int* p_data = nullptr;
    };

    {
        const Test a{1, 2, 3, 4, 5};
        Test b{a};

        RMDEV_TEST_ASSERT(a == b);

        Test c(std::move(b));
        RMDEV_TEST_ASSERT(c == a && b.data() == nullptr);

        b = a;
        RMDEV_TEST_ASSERT(b == a);

        Test d{};
        d = a;
        RMDEV_TEST_ASSERT(d == a);

        d = std::move(b);
        RMDEV_TEST_ASSERT(d == a && b.data() == nullptr);
    }

    RMDEV_TEST_ITEM("InitOnce Move Test");

    {
        using rmdev::InitOnce;

        InitOnce<Test> b;
        const Test a{1, 2, 3, 4, 5};
        const Test dummy{114514, 1919, 810, 24, 251};

        Test dummy_2(dummy);

        b = a;
        RMDEV_TEST_ASSERT(b() == a);
        b = dummy;
        RMDEV_TEST_ASSERT(b() == a);
        b = std::move(dummy_2);
        RMDEV_TEST_ASSERT(b() == a && dummy_2.data() != nullptr);

        InitOnce<Test> c, d;
        c = a;
        RMDEV_TEST_ASSERT(c() == a);
        c = dummy;
        RMDEV_TEST_ASSERT(c() == a);

        d = std::move(c);
        RMDEV_TEST_ASSERT(d() == a && c().data() == nullptr);
        d = dummy;
        RMDEV_TEST_ASSERT(d() == a && c().data() == nullptr);

        InitOnce<Test> e(b);
        RMDEV_TEST_ASSERT(e() == b() && e() == a);
    }
}

void initOnceTest()
{
    basicTest();
    globalTest();
    moveTest();
}
