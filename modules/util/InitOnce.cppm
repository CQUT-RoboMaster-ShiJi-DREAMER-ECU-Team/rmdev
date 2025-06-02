/**
 * @file InitOnce.cppm
 * @module rmdev.lib.InitOnce
 * @author 杜以成
 * @date 2025-05-09
 * @brief 单次初始化类
 */

module;

#include "etl/type_traits.h"
#include "etl/utility.h"

import rmdev.error_handler;

export module rmdev.util.InitOnce;

export namespace rmdev {

/**
 * 单次初始化类
 * @tparam Type 存储的数据类型
 */
template<typename Type>
    requires(etl::is_copy_constructible_v<Type> && etl::is_default_constructible_v<Type> &&
             etl::is_move_constructible_v<Type>)
class InitOnce
{
public:
    InitOnce() : v(), is_init(false) {}
    ~InitOnce() = delete;

    InitOnce(const InitOnce& other) : v(other.v), is_init(true) {}

    InitOnce(InitOnce&& other) noexcept : v(etl::move(other.v)), is_init(true) {}

    /**
     * 初始化值
     * @param other_value 待初始化的值
     * @return 错误码。若已被初始化后，返回 ErrorCode::AlreadyExists。
     */
    constexpr ErrorCode init(Type&& other_value)
    {
        if (is_init) {
            return ErrorCode::AlreadyExists;
        }

        v = etl::forward<Type>(other_value);
        is_init = true;

        return ErrorCode::Success;
    }

    constexpr InitOnce& operator=(const InitOnce& other)
    {
        init(other.v);

        return *this;
    }

    constexpr InitOnce& operator=(InitOnce&& other) noexcept
    {
        init(etl::move(other.v));

        return *this;
    }

    explicit InitOnce(Type&& other_value) : v(etl::forward<Type>(other_value)), is_init(true) {}

    constexpr InitOnce& operator=(Type&& other_value)
    {
        init(etl::forward<Type>(other_value));

        return *this;
    }

    /**
     * 获得值的常量引用
     * @return 值的常量引用
     */
    constexpr const Type& operator()() const
    {
        return v;
    }

    operator Type() const  // NOLINT(google-explicit-constructor): 允许隐式转换成为自身类型
    {
        return v;
    }

private:
    Type v;        ///< 值

    bool is_init;  ///< 是否已经初始化
};

}  // namespace rmdev
