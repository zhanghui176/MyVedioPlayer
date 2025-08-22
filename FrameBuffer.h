#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include <mutex>
#include <condition_variable>
#include <queue>
#include <atomic>
#include <chrono>
#include <optional>

template <typename FrameType>
class FrameBuffer {
public:
    FrameBuffer(size_t maxFrames = 100, bool dropOld = true)
        : maxFrames(maxFrames), isActive(true), dropOld(dropOld) {}

    ~FrameBuffer() {
        clear();
    }

    // 添加帧到缓冲区
    void enqueue(FrameType frame) {
        std::unique_lock<std::mutex> lock(mutex);

        if (dropOld) {
            // 如果满了就丢弃最旧的帧
            if (buffer.size() >= maxFrames) {
                buffer.pop();
            }
        } else {
            // 阻塞直到有空位
            cvFull.wait(lock, [this] {
                return buffer.size() < maxFrames || !isActive;
            });
        }

        buffer.push(std::move(frame));
        cvEmpty.notify_one();
    }

    // 从缓冲区取出帧（支持超时）
    std::optional<FrameType> dequeue(int timeoutMs = 10000) {
        std::unique_lock<std::mutex> lock(mutex);

        if (!cvEmpty.wait_for(lock, std::chrono::milliseconds(timeoutMs), [this] {
                return !buffer.empty() || !isActive;
            })) {
            return std::nullopt; // 超时
        }

        if (!isActive && buffer.empty()) {
            return std::nullopt;
        }

        auto frame = std::move(buffer.front());
        buffer.pop();
        cvFull.notify_one();

        return frame;
    }

    void clear() {
        std::lock_guard<std::mutex> lock(mutex);
        while (!buffer.empty()) {
            buffer.pop();
        }
        cvFull.notify_all();
        cvEmpty.notify_all();
    }

    void setActive(bool active) {
        std::lock_guard<std::mutex> lock(mutex);
        isActive = active;
        if (!active) {
            cvFull.notify_all();
            cvEmpty.notify_all();
        }
    }

    size_t size() const {
        std::lock_guard<std::mutex> lock(mutex);
        return buffer.size();
    }

private:
    std::queue<FrameType> buffer;
    mutable std::mutex mutex;
    std::condition_variable cvFull;
    std::condition_variable cvEmpty;
    size_t maxFrames;
    std::atomic<bool> isActive;
    bool dropOld; // 是否丢弃旧帧
};


#endif // FRAMEBUFFER_H
