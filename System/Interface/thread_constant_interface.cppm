/**
 * @file thread_constant_interface.cppm
 * @module rmdev.thread.constant:interface
 * @author 杜以成
 * @date 2025-06-18
 * @brief 线程相关常数接口
 */

module;

export module rmdev.thread.constant:interface;

export namespace rmdev::thread {

consteval auto MAX_DELAY() noexcept;

}  // namespace rmdev::thread
