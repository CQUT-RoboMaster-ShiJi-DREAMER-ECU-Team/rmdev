/**
 * @file Subscriber_test.cpp
 * @brief 订阅者测试
 */

#include "emdevif_test_framework.h"
#include "emdevif/core/line_separator.h"

#include "heap_usage_checker.hpp"

#include <initializer_list>

import rmdev.message_manager.subscriber;
import emdevif.sys.sysQueue;
import emdevif.sys.sysMessageSlot;
import emdevif.core.data_container.message_slot;
import emdevif.core.data_container.message_queue;
import emdevif.core.error_handler;

TEST_SUIT(SubscriberTest)
{
    RECORD_MEMORY_INFO();

    TEST_CASE_BEGIN(SysQueue)
    {
        emdevif::SysQueue<int, 5> q1 = decltype(q1)::create({.name = "test q1"});
        rmdev::Subscriber<int, decltype(q1)> sub1{q1};

        using emdevif::ErrorCode;

        ASSERT_TRUE(sub1.getHandle() == q1.getHandle(),
                    "sub1.getHandle() is %p" EMDEVIF_LINE_SEPARATOR "q1.getHandle() is %p",
                    sub1.getHandle(),
                    q1.getHandle());

        ASSERT_TRUE(&sub1.getQueueInstance() == &q1,
                    "&sub1.getQueueInstance() is %p" EMDEVIF_LINE_SEPARATOR "&q1 is %p",
                    &sub1.getQueueInstance(),
                    &q1);

        UINT_ASSERT_EQ(sub1.storeCount(), 0, "");
        UINT_ASSERT_EQ(sub1.remainCount(), 5, "");

        for (const int i : {1, 4, 3, 7}) {
            auto ec = q1.push(false, i);

            ASSERT_TRUE(ec == ErrorCode::Success, "i = %d; ec = %d", i, static_cast<int>(ec));
        }

        UINT_ASSERT_EQ(sub1.storeCount(), 4, "");
        UINT_ASSERT_EQ(sub1.remainCount(), 1, "");

        int value = -1;

        auto ec = sub1.pop(false, value);
        ASSERT_TRUE(ec == ErrorCode::Success, "");
        UINT_ASSERT_EQ(sub1.storeCount(), 3, "");
        UINT_ASSERT_EQ(sub1.remainCount(), 2, "");
        UINT_EXPECT_EQ(value, 1);

        ec = sub1.pop(false, value);
        ASSERT_TRUE(ec == ErrorCode::Success, "");
        UINT_ASSERT_EQ(sub1.storeCount(), 2, "");
        UINT_ASSERT_EQ(sub1.remainCount(), 3, "");
        UINT_EXPECT_EQ(value, 4);

        ec = sub1.pop(false, value);
        ASSERT_TRUE(ec == ErrorCode::Success, "");
        UINT_ASSERT_EQ(sub1.storeCount(), 1, "");
        UINT_ASSERT_EQ(sub1.remainCount(), 4, "");
        UINT_EXPECT_EQ(value, 3);

        ec = sub1.pop(false, value);
        ASSERT_TRUE(ec == ErrorCode::Success, "");
        UINT_ASSERT_EQ(sub1.storeCount(), 0, "");
        UINT_ASSERT_EQ(sub1.remainCount(), 5, "");
        UINT_EXPECT_EQ(value, 7);
    }
    TEST_CASE_END();

    TEST_CASE_BEGIN(SysSlot)
    {
        emdevif::SysMessageSlot<int> q2 = decltype(q2)::create({.name = "test q2"});
        rmdev::Subscriber<int, decltype(q2)> sub2{q2};

        using emdevif::ErrorCode;

        auto& q1 = q2;
        auto& sub1 = sub2;

        ASSERT_TRUE(sub1.getHandle() == q1.getHandle(),
                    "sub1.getHandle() is %p" EMDEVIF_LINE_SEPARATOR "q1.getHandle() is %p",
                    sub1.getHandle(),
                    q1.getHandle());

        ASSERT_TRUE(&sub1.getQueueInstance() == &q1,
                    "&sub1.getQueueInstance() is %p" EMDEVIF_LINE_SEPARATOR "&q1 is %p",
                    &sub1.getQueueInstance(),
                    &q1);

        UINT_ASSERT_EQ(sub1.storeCount(), 0, "");
        UINT_ASSERT_EQ(sub1.remainCount(), 1, "");

        for (const int i : {1, 4, 3, 252345, 457, -2536, 7}) {
            auto ec = q1.forcePush(false, i);

            ASSERT_TRUE(ec == ErrorCode::Success, "i = %d; ec = %d", i, static_cast<int>(ec));
        }

        UINT_ASSERT_EQ(sub1.storeCount(), 1, "");
        UINT_ASSERT_EQ(sub1.remainCount(), 0, "");

        int value = -1;

        auto ec = sub1.peek(false, value);
        ASSERT_TRUE(ec == ErrorCode::Success, "");
        UINT_ASSERT_EQ(sub1.storeCount(), 1, "");
        UINT_ASSERT_EQ(sub1.remainCount(), 0, "");
        UINT_EXPECT_EQ(value, 7);

        value = -2;

        ec = sub1.peek(false, value);
        ASSERT_TRUE(ec == ErrorCode::Success, "");
        UINT_ASSERT_EQ(sub1.storeCount(), 1, "");
        UINT_ASSERT_EQ(sub1.remainCount(), 0, "");
        UINT_EXPECT_EQ(value, 7);
    }
    TEST_CASE_END();

    CHECK_MEMORY_LEAK();
}

void subscriberTest()
{
    RUN_SUIT(SubscriberTest);
}
