/**
 * @file Subscriber_test.cpp
 * @brief 订阅者测试
 */

#include "emdevif_test_framework.h"

#include "emdevif/line_separator.h"

#include <initializer_list>

import rmdev.messageManager.subscriber;
import emdevif.sys.sysQueue;
import emdevif.sys.sysMessageSlot;

TEST_SUIT(SubscriberTest)
{
    auto testSubscriber = []<class QueueImpl, typename SubType>
        requires(emdevif::ValidMessageQueue<QueueImpl> || emdevif::ValidMessageSlot<QueueImpl>)
    (emdevif_test_TestCaseContex& contex, QueueImpl& q, SubType& subscriber) -> void {
        EMDEVIF_TEST_RUN_TEST_CASE_WITHIN_CONTEX_BEGIN(&contex)
        {
            using emdevif::ErrorCode;

            ASSERT_TRUE(subscriber.getHandle() == q.getHandle(),
                        "subscriber.getHandle() is %p" EMDEVIF_LINE_SEPARATOR "q.getHandle() is %p",
                        subscriber.getHandle(),
                        q.getHandle());

            ASSERT_TRUE(&subscriber.getQueueInstance() == &q,
                        "&subscriber.getQueueInstance() is %p" EMDEVIF_LINE_SEPARATOR "&q is %p",
                        &subscriber.getQueueInstance(),
                        &q);

            UINT_ASSERT_EQ(subscriber.storeCount(), 0, "");
            UINT_ASSERT_EQ(subscriber.remainCount(), 5, "");

            for (const auto i : {1, 4, 3, 7}) {
                const auto ec = q.push(false, i);
                ASSERT_TRUE(ec == ErrorCode::Success, "i = %d; ec = %d", i, static_cast<int>(ec));
            }

            UINT_ASSERT_EQ(subscriber.storeCount(), 4, "");
            UINT_ASSERT_EQ(subscriber.remainCount(), 1, "");

            int value = -1;

            auto ec = subscriber.pop(false, value);
            ASSERT_TRUE(ec == ErrorCode::Success, "");
            UINT_ASSERT_EQ(subscriber.storeCount(), 3, "");
            UINT_ASSERT_EQ(subscriber.remainCount(), 2, "");
            UINT_EXPECT_EQ(value, 1);

            ec = subscriber.pop(false, value);
            ASSERT_TRUE(ec == ErrorCode::Success, "");
            UINT_ASSERT_EQ(subscriber.storeCount(), 2, "");
            UINT_ASSERT_EQ(subscriber.remainCount(), 3, "");
            UINT_EXPECT_EQ(value, 4);

            ec = subscriber.pop(false, value);
            ASSERT_TRUE(ec == ErrorCode::Success, "");
            UINT_ASSERT_EQ(subscriber.storeCount(), 1, "");
            UINT_ASSERT_EQ(subscriber.remainCount(), 4, "");
            UINT_EXPECT_EQ(value, 3);

            ec = subscriber.pop(false, value);
            ASSERT_TRUE(ec == ErrorCode::Success, "");
            UINT_ASSERT_EQ(subscriber.storeCount(), 0, "");
            UINT_ASSERT_EQ(subscriber.remainCount(), 5, "");
            UINT_EXPECT_EQ(value, 7);
        }
        EMDEVIF_TEST_RUN_TEST_CASE_WITHIN_CONTEX_END();
    };

    TEST_CASE_BEGIN(SysQueue)
    {
        emdevif::SysQueue<int, 5> q1 = decltype(q1)::create({.name = "test q1"});
        rmdev::Subscriber<int, decltype(q1)> sub1{q1};

        emdevif_test_TestCaseContex contex = EMDEVIF_TEST_GET_THIS_TEST_CASE_CONTEXT();
        testSubscriber(contex, q1, sub1);
    }
    TEST_CASE_END();

    TEST_CASE_BEGIN(SysSlot)
    {
        emdevif::SysMessageSlot<int> q2 = decltype(q2)::create({.name = "test q2"});
        rmdev::Subscriber<int, decltype(q2)> sub2{q2};

        emdevif_test_TestCaseContex contex = EMDEVIF_TEST_GET_THIS_TEST_CASE_CONTEXT();
        testSubscriber(contex, q2, sub2);
    }
    TEST_CASE_END();
}

void subscriberTest()
{
    RUN_SUIT(SubscriberTest);
}
