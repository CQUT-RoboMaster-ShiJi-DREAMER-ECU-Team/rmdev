/**
 * @file InitOnce.cppm
 * @module rmdev.lib.InitOnce
 * @author 杜以成
 * @date 2025-05-09
 * @brief 单次初始化类
 */

module;

#include <concepts>

export module rmdev.util.InitOnce;

export namespace rmdev {

/**
 * 单次初始化类
 * @tparam Type 存储的数据类型
 */
template<typename Type>
    requires(std::is_copy_constructible_v<Type> && std::is_default_constructible_v<Type>)
class InitOnce
{
public:
    InitOnce() = default;
    ~InitOnce() = default;

    InitOnce(const InitOnce&) = delete;
    InitOnce(InitOnce&&) = delete;
    InitOnce& operator=(const InitOnce&) = delete;
    InitOnce& operator=(InitOnce&&) = delete;

public:
    explicit InitOnce(const Type other_value) : v(other_value), is_init(true) {}

    /**
     * 写入值
     * @param other_value 待初始化的值
     */
    constexpr void init(Type other_value)
    {
        if (is_init) {
            return;
        }

        v = other_value;
        is_init = true;
    }

    constexpr InitOnce& operator=(Type other_value)
    {
        init(other_value);

        return *this;
    }

    /**
     * 获得值
     * @return 值
     */
    constexpr Type value() const
    {
        return v;
    }

    operator Type() const  // NOLINT(google-explicit-constructor): 允许隐式转换成为自身类型
    {
        return v;
    }

private:
    Type v;                ///< 值

    bool is_init = false;  ///< 是否已经初始化
};

}  // namespace rmdev
