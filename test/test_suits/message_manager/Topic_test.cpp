/**
 * @file Topic_test.cpp
 * @brief 话题 - 测试
 */

#include "emdevif_test_framework.h"
#include "heap_usage_checker.hpp"

// 整个活~
// https://cppreference.cn/w/cpp/language/operator_alternative
%:include <ciso646>
#include <memory>
#include <algorithm>
#include <utility>
#include <initializer_list>

import rmdev.message_manager.subscriber;
import rmdev.message_manager.topic;
import emdevif.core.data_container.message_queue;
import emdevif.core.error_handler;
import emdevif.sys.sysQueue;
import emdevif.sys.sysMessageSlot;
import emdevif.sys.heap;

TEST_SUIT(TopicTest)
{
    RECORD_MEMORY_INFO();

    TEST_CASE_BEGIN(SysQueue)
    <%
        rmdev::Topic<emdevif::SysQueue<int, 5>, emdevif::heap::Allocator> topic<%"test topic1"%>;
        static_assert(decltype(topic)::is_queue_not_slot);
        static_assert(std::is_same_v<decltype(topic)::ValueType, int>);
        static_assert(decltype(topic)::queue_item_size == 5);

        EXPECT_STREQ(topic.getName().data(), "test topic1");
        UINT_EXPECT_EQ(topic.getQueueListInstance().capacity(), 3U);

        using emdevif::ErrorCode;

        ASSERT_TRUE(topic.getQueueListInstance().empty(), "");
        UINT_ASSERT_EQ(topic.getQueueListInstance().size(), 0U, "");

        auto sub1 = topic.addSubscriber();

        UINT_ASSERT_EQ(topic.getQueueListInstance().size(), 1, "");

        UINT_ASSERT_EQ(sub1.storeCount(), 0, "");
        UINT_ASSERT_EQ(sub1.remainCount(), 5, "");

        auto <:pub_ec, err_it:> = topic.push(false, 1);
        EXPECT_TRUE(pub_ec == ErrorCode::Success)->MESSAGE("pub_ec is %d, ...", static_cast<int>(pub_ec));
        ASSERT_TRUE(err_it == topic.getQueueListInstance().end(),
                    "err_it - topic.getQueueListInstance().end() = %td",
                    std::distance(err_it, topic.getQueueListInstance().end()));

        UINT_ASSERT_EQ(sub1.storeCount(), 1, "");
        UINT_ASSERT_EQ(sub1.remainCount(), 5 - 1, "");

        int value = -1;
        ASSERT_TRUE(sub1.pop(false, value) == ErrorCode::Success, "");
        INT_EXPECT_EQ(value, 1);

        /////////////////////////////////////////

        UINT_ASSERT_EQ(sub1.storeCount(), 0, "");
        UINT_ASSERT_EQ(sub1.remainCount(), 5, "");

        UINT_ASSERT_EQ(topic.getQueueListInstance().size(), 1, "");
        UINT_ASSERT_EQ(topic.getQueueListInstance()<:0:>.storeCount(), 0, "");
        UINT_ASSERT_EQ(topic.getQueueListInstance().operator<::>(0).remainCount(), 5, "");

        auto sub2 = topic.addSubscriber<float>();
        UINT_ASSERT_EQ(sub2.storeCount(), 0, "");
        UINT_ASSERT_EQ(sub2.remainCount(), 5, "");

        UINT_ASSERT_EQ(topic.getQueueListInstance().size(), 2, "");
        UINT_ASSERT_EQ(topic.getQueueListInstance()<:0:>.storeCount(), 0, "");
        UINT_ASSERT_EQ(topic.getQueueListInstance()<:0:>.remainCount(), 5, "");
        UINT_ASSERT_EQ(topic.getQueueListInstance()<:1:>.storeCount(), 0, "");
        UINT_ASSERT_EQ(topic.getQueueListInstance()<:1:>.remainCount(), 5, "");

        for (const int i : <%1, 4, 3, 7%>) <%
            auto <:ec, it:> = topic.push(false, i);

            EXPECT_TRUE(ec == ErrorCode::Success)->MESSAGE("i = %d; ec = %d, ...", i, static_cast<int>(ec));
            ASSERT_TRUE(it == topic.getQueueListInstance().end(),
                        "it - topic.getQueueListInstance().end() = %td",
                        std::distance(it, topic.getQueueListInstance().end()));
        %>

        value = -2;

        // 1
        ASSERT_TRUE(sub1.pop(false, value) == ErrorCode::Success, "");
        INT_EXPECT_EQ(value, 1);

        float fvalue = -1.0f;
        ASSERT_TRUE(sub2.pop(false, fvalue) == ErrorCode::Success, "");
        FP_EXPECT_EQ(fvalue, 1.0f);

        // 4
        ASSERT_TRUE(sub1.pop(false, value) == ErrorCode::Success, "");
        INT_EXPECT_EQ(value, 4);

        ASSERT_TRUE(sub2.pop(false, fvalue) == ErrorCode::Success, "");
        FP_EXPECT_EQ(fvalue, 4.0f);

        // 3
        ASSERT_TRUE(sub1.pop(false, value) == ErrorCode::Success, "");
        INT_EXPECT_EQ(value, 3);

        ASSERT_TRUE(sub2.pop(false, fvalue) == ErrorCode::Success, "");
        FP_EXPECT_EQ(fvalue, 3.0f);

        // 7
        ASSERT_TRUE(sub1.pop(false, value) == ErrorCode::Success, "");
        INT_EXPECT_EQ(value, 7);

        ASSERT_TRUE(sub2.pop(false, fvalue) == ErrorCode::Success, "");
        FP_EXPECT_EQ(fvalue, 7.0f);
    %>
    TEST_CASE_END();

    TEST_CASE_BEGIN(SysSlot)
    {
        rmdev::Topic<emdevif::SysMessageSlot<int>, emdevif::heap::Allocator> topic{"test topic2", 2};
        static_assert(not decltype(topic)::is_queue_not_slot);
        static_assert(std::is_same_v<decltype(topic)::ValueType, int>);
        static_assert(decltype(topic)::queue_item_size == 1);

        EXPECT_STREQ(topic.getName().data(), "test topic2");
        UINT_EXPECT_EQ(topic.getQueueListInstance().capacity(), 2U);

        using emdevif::ErrorCode;

        ASSERT_TRUE(topic.getQueueListInstance().empty(), "");
        UINT_ASSERT_EQ(topic.getQueueListInstance().size(), 0U, "");

        auto sub1 = topic.addSubscriber();

        UINT_ASSERT_EQ(topic.getQueueListInstance().size(), 1, "");

        UINT_ASSERT_EQ(sub1.storeCount(), 0, "");
        UINT_ASSERT_EQ(sub1.remainCount(), 1, "");

        auto [pub_ec, err_it] = topic.forcePush(false, 1);
        EXPECT_TRUE(pub_ec == ErrorCode::Success)->MESSAGE("pub_ec is %d, ...", static_cast<int>(pub_ec));
        ASSERT_TRUE(err_it == topic.getQueueListInstance().end(),
                    "err_it - topic.getQueueListInstance().end() = %td",
                    std::distance(err_it, topic.getQueueListInstance().end()));

        UINT_ASSERT_EQ(sub1.storeCount(), 1, "");
        UINT_ASSERT_EQ(sub1.remainCount(), 0, "");

        int value = -1;
        ASSERT_TRUE(sub1.peek(false, value) == ErrorCode::Success, "");
        INT_EXPECT_EQ(value, 1);

        /////////////////////////////////////////

        UINT_ASSERT_EQ(sub1.storeCount(), 1, "");
        UINT_ASSERT_EQ(sub1.remainCount(), 0, "");

        UINT_ASSERT_EQ(topic.getQueueListInstance().size(), 1, "");
        UINT_ASSERT_EQ(topic.getQueueListInstance()[0].storeCount(), 1, "");
        UINT_ASSERT_EQ(topic.getQueueListInstance().operator[](0).remainCount(), 0, "");

        auto sub2 = topic.addSubscriber<float>();
        UINT_ASSERT_EQ(sub2.storeCount(), 0, "");
        UINT_ASSERT_EQ(sub2.remainCount(), 1, "");

        UINT_ASSERT_EQ(topic.getQueueListInstance().size(), 2, "");
        UINT_ASSERT_EQ(topic.getQueueListInstance()[0].storeCount(), 1, "");
        UINT_ASSERT_EQ(topic.getQueueListInstance()[0].remainCount(), 0, "");
        UINT_ASSERT_EQ(topic.getQueueListInstance()[1].storeCount(), 0, "");
        UINT_ASSERT_EQ(topic.getQueueListInstance()[1].remainCount(), 1, "");

        for (const int i : {1, 4, 3, 3, 3, -1525345, 25253, 7}) {
            auto [ec, it] = topic.forcePush(false, i);

            EXPECT_TRUE(ec == ErrorCode::Success)->MESSAGE("i = %d; ec = %d, ...", i, static_cast<int>(ec));
            ASSERT_TRUE(it == topic.getQueueListInstance().end(),
                        "it - topic.getQueueListInstance().end() = %td",
                        std::distance(it, topic.getQueueListInstance().end()));
        }

        value = -2;
        float fvalue = -2.0f;

        // 只有 7
        ASSERT_TRUE(sub1.peek(false, value) == ErrorCode::Success, "");
        INT_EXPECT_EQ(value, 7);

        ASSERT_TRUE(sub2.peek(false, fvalue) == ErrorCode::Success, "");
        FP_EXPECT_EQ(fvalue, 7.0f);
    }
    TEST_CASE_END();

    CHECK_MEMORY_LEAK();
}

void topicTest()
{
    RUN_SUIT(TopicTest);
}
