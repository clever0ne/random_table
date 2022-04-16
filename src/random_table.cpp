#include "random_table.h"

#include <iostream>
#include <sstream>
#include <iomanip>

RandomTable::RandomTable() :
    m_device(),
    m_generator()
{
    const auto seed = m_device() ^
        (static_cast<RandomType>(std::chrono::duration_cast<std::chrono::seconds>
        (std::chrono::system_clock::now().time_since_epoch()).count()) +
        (static_cast<RandomType>(std::chrono::duration_cast<std::chrono::microseconds>
        (std::chrono::high_resolution_clock::now().time_since_epoch()).count())));
    m_generator = RandomGenerator(seed);

    auto numbers = std::vector<uint8_t>(table_size);
    std::iota(std::begin(numbers), std::end(numbers), 0);

    for (size_t i = 0; i < table_size; i++)
    {
        auto distribution = RandomDistribution(0, numbers.size() - 1);
        const auto index = distribution(m_generator);
        const auto number = numbers.at(index);

        numbers.erase(numbers.begin() + index);
        numbers.shrink_to_fit();

        for (size_t j = 0; j < table_size; j++)
        {
            size_t row = j;
            size_t col = table_size - 1 - (j / block_size + j * block_size + i) % table_size;

            m_values[row][col] = number;
        }
    }
}

void RandomTable::transpose() noexcept
{
    for (size_t row = 0; row < table_size; row++)
    {
        for (size_t col = 0; col < table_size; col++)
        {
            std::swap(m_values[row][col], m_values[col][row]);
        }
    }
}

void RandomTable::swapRows(const size_t first, const size_t second) noexcept
{
    if (first >= table_size || second >= table_size)
    {
        return;
    }

    for (size_t col = 0; col < table_size; col++)
    {
        std::swap(m_values[first][col], m_values[second][col]);
    }
}

void RandomTable::swapBlockRows(const size_t first, const size_t second) noexcept
{
    if (first >= block_size || second >= block_size)
    {
        return;
    }

    for (size_t offset = 0; offset < block_size; offset++)
    {
        swapRows(first * block_size + offset, second * block_size + offset);
    }
}

void RandomTable::swapCols(const size_t first, const size_t second) noexcept
{
    if (first >= table_size || second >= table_size)
    {
        return;
    }

    for (size_t row = 0; row < table_size; row++)
    {
        std::swap(m_values[row][first], m_values[row][second]);
    }
}

void RandomTable::swapBlockCols(const size_t first, const size_t second) noexcept
{
    if (first >= block_size || second >= block_size)
    {
        return;
    }

    for (size_t offset = 0; offset < block_size; offset++)
    {
        swapCols(first * block_size + offset, second * block_size + offset);
    }
}

void RandomTable::mix() noexcept
{
    static auto operations = RandomDistribution(0, static_cast<size_t>(Operation::COUNT) - 1);
    static constexpr size_t min_iterations_number = 1000;
    static constexpr size_t max_iterations_number = 10000;

    size_t iterations = RandomDistribution(min_iterations_number,
                                           max_iterations_number)(m_generator);
    for (size_t iteration = 0; iteration < iterations; iteration++)
    {
        auto operation = static_cast<Operation>(operations(m_generator));
        switch (operation)
        {
            case Operation::TRANSPOSE:
            {
                transpose();
                break;
            }
            case Operation::SWAP_ROWS:
            {
                auto distribution = RandomDistribution(0, block_size - 1);
                auto block  = distribution(m_generator);
                auto first  = distribution(m_generator);
                auto second = distribution(m_generator);
                swapRows(block * block_size + first, block * block_size + second);
                break;
            }
            case Operation::SWAP_COLS:
            {
                auto distribution = RandomDistribution(0, block_size - 1);
                auto block  = distribution(m_generator);
                auto first  = distribution(m_generator);
                auto second = distribution(m_generator);
                swapCols(block * block_size + first, block * block_size + second);
                break;
            }
            case Operation::SWAP_BLOCK_ROWS:
            {
                auto distribution = RandomDistribution(0, block_size - 1);
                auto first  = distribution(m_generator);
                auto second = distribution(m_generator);
                swapBlockRows(first, second);
                break;
            }
            case Operation::SWAP_BLOCK_COLS:
            {
                auto distribution = RandomDistribution(0, block_size - 1);
                auto first  = distribution(m_generator);
                auto second = distribution(m_generator);
                swapBlockCols(first, second);
                break;
            }
            default:
            {
                break;
            }
        }
    }
}

void RandomTable::print() const noexcept
{
    for (size_t row = 0; row < table_size; row++)
    {
        std::stringstream str;
        str << std::hex << std::uppercase;
        for (size_t col = 0; col < table_size; col++)
        {
            str << "0x" << std::setw(2) << std::setfill('0') << static_cast<uint32_t>(m_values[row][col]) << " ";
        }
        std::cout << str.str() << std::endl;
    }
}
