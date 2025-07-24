/**
 * @file ArrayTraitsAggregate.cppm
 * @module rmdev.util.ArrayTraitsAggregate
 * @author DuYicheng
 * @date 2025-07-24
 * @brief 数组特征的聚合类型
 */

module;

#include <array>

export module rmdev.util.ArrayTraitsAggregate;

export namespace rmdev {

/**
 * 有数组特征的聚合类型
 * @tparam Type 数据类型
 * @tparam size 数组长度
 */
template<typename Type, std::size_t size>
class ArrayTraitsAggregate
{
public:
    constexpr Type& operator[](const std::size_t n) noexcept
    {
        return *(reinterpret_cast<Type*>(this) + n);
    }

    constexpr const Type& operator[](const std::size_t n) const noexcept
    {
        return const_cast<ArrayTraitsAggregate*>(this)->operator[](n);
    }

    constexpr Type* at(const std::size_t n)
    {
        if (n >= size) {
            return nullptr;
        }

        return &this->operator[](n);
    }

    constexpr const Type* at(const std::size_t n) const
    {
        return const_cast<ArrayTraitsAggregate*>(this)->at(n);
    }
};

}  // namespace rmdev
