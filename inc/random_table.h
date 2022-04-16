#pragma once

#ifndef RANDOM_TABLE_H
#define RANDOM_TABLE_H

#include <vector>
#include <random>
#include <chrono>
#include <numeric>

#include <cstdint>

class RandomTable
{
public:
    using RandomDevice = std::random_device;
    using RandomGenerator = std::mt19937;
    using RandomType = std::mt19937::result_type;
    using RandomDistribution = std::uniform_int_distribution<std::mt19937::result_type>;

    enum class Operation { TRANSPOSE, SWAP_ROWS, SWAP_COLS, SWAP_BLOCK_ROWS, SWAP_BLOCK_COLS, COUNT };

    static constexpr size_t table_size = 16;
    static constexpr size_t block_size = 4;

    RandomTable();
    virtual ~RandomTable() = default;

    void transpose() noexcept;
    void swapRows(const size_t first, const size_t second) noexcept;
    void swapBlockRows(const size_t first, const size_t second) noexcept;
    void swapCols(const size_t first, const size_t second) noexcept;
    void swapBlockCols(const size_t first, const size_t second) noexcept;
    void mix() noexcept;
    void print() const noexcept;

    uint8_t *data() noexcept { return &m_values[0][0]; }
    size_t size() const noexcept { return table_size * table_size; }

    uint8_t *begin() noexcept { return &m_values[0][0]; }
    const uint8_t *begin() const noexcept { return &m_values[0][0]; }
    const uint8_t *cbegin() const noexcept { return &m_values[0][0]; }

    uint8_t *end() noexcept { return &m_values[0][0] + size(); }
    const uint8_t *end() const noexcept { return &m_values[0][0] + size(); }
    const uint8_t *cend() const noexcept { return &m_values[0][0] + size(); }

private:
    uint8_t m_values[table_size][table_size] = { { 0x00 } };
    RandomDevice m_device;
    RandomGenerator m_generator;
};

#endif /* RANDOM_TABLE_H */
