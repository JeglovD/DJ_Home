#pragma once

#include <cstddef>	// size_t
#include <climits>	// CHAR_BIT
#include <string>	// basic_ostream

namespace std
{
	template <size_t N> class DJ_bitset
	{
	public:
		DJ_bitset()
		{
			reset();
		}
		DJ_bitset(size_t value)
		{
			reset();
			size_t count = sizeof(value) * CHAR_BIT;
			if (count > N)
				count = N;
			for (size_t i = 0; i < count; ++i)
				set(i, ((value >> i) & 1));
		}

		// Логическое И
		DJ_bitset<N>& operator&=(const DJ_bitset<N>& rhs)
		{
			for (size_t i = 0; i < mByteCount; ++i)
				mData[i] &= rhs.mData[i];
			return *this;
		}

		// Логическое ИЛИ
		DJ_bitset<N>& operator|=(const DJ_bitset<N>& rhs)
		{
			for (size_t i = 0; i < mByteCount; ++i)
				mData[i] |= rhs.mData[i];
			return *this;
		}

		// Логическое Исключающее ИЛИ
		DJ_bitset<N>& operator^=(const DJ_bitset<N>& rhs)
		{
			for (size_t i = 0; i < mByteCount; ++i)
				mData[i] ^= rhs.mData[i];
			return *this;
		}

		// Сдвигаем биты влево
		DJ_bitset<N>& operator<<=(size_t pos)
		{
			for (size_t i = N - 1; i >= pos; --i)
				set(i, test(i - pos));
			for (size_t i = 0; i < pos; ++i)
				reset(i);
			return *this;
		}

		// Сдвигаем биты вправо
		DJ_bitset<N>& operator>>=(size_t pos)
		{
			for (size_t i = 0; i < (N - pos); ++i)
				set(i, test(i + pos));
			for (size_t i = pos; i > 0; --i)
				reset(N - i);
			return *this;
		}

		// Возвращает копию, у которой все биты будут инвертированы (логическое "НЕ")
		DJ_bitset<N> operator~() const
		{
			return DJ_bitset(*this).flip();
		}

		// Инвертирование всех битов
		DJ_bitset<N>& flip()
		{
			for (size_t i = 0; i < mByteCount; ++i)
				mData[i] = ~mData[i];
			return *this;
		}

		// Инвертирование конкретного бита, отсчет битов идет от 0
		DJ_bitset<N>& flip(size_t bitPosition)
		{
			char temp = mData[ByteNumber(bitPosition)] & (1 << BitNumber(bitPosition));
			if (temp == 0)	// Bit was 0
				mData[ByteNumber(bitPosition)] |= (1 << BitNumber(bitPosition));
			else			// Bit was 1
				mData[ByteNumber(bitPosition)] &= ~(1 << BitNumber(bitPosition));
			return *this;
		}

		// Возвращает целочисленное представление данных в типе unsigned int
		unsigned int to_uint() const
		{
			if (N > sizeof(unsigned int) * CHAR_BIT)
				__throw_overflow_error();
			unsigned int result(0);
			for (size_t i = N; i > 0; --i)
			{
				if (test(i))
					result += 1;
				result <<= 1;
			}
			if (test(0))
				result += 1;
			return result;
		}

		template <class charT, class traits, class Allocator>
		basic_string<charT, traits, Allocator> to_string() const
		{
			basic_string<charT, traits, Allocator> result;
			result.reserve(N);
			for (size_t i = N; i > 0; --i)
				result.append(test(i - 1) ? "1" : "0");
			return result;
		}

		// Число бит, установленных в true 
		size_t count() const
		{
			size_t result(0);
			for (size_t i = 0; i < N; ++i)
				if (test(i))
					++result;
			return result;
		}

		// Возвращает число битов, которые bitset может хранить
		size_t size() const
		{
			return N;
		}

		// Присвоение
		DJ_bitset<N>& operator=(const DJ_bitset<N>& rhs)
		{
			if (&rhs == this)
				return *this;
			for (size_t i = 0; i < mByteCount; ++i)
				mData[i] = rhs.mData[i];
			return *this;
		}

		// Возвращает true, если все соответствующие биты в *this и rhs равны.
		bool operator==(const DJ_bitset<N>& rhs) const
		{
			for (size_t i = 0; i< N; ++i)
				if (test(i) != rhs.test(i))
					return false;
			return true;
		}

		// Возвращает true, если любой бит в *this не равен соответствующему в rhs
		bool operator!=(const DJ_bitset<N>& rhs) const
		{
			for (size_t i = 0; i< N; ++i)
				if (test(i) != rhs.test(i))
					return true;
			return false;
		}

		// Проверяет, чтобы хотя бы один бит был установлен в true
		bool any() const
		{
			for (size_t i = 0; i< N; ++i)
				if (test(i))
					return true;
			return false;
		}

		// Проверяет, что ни один бит не установлен в true
		bool none() const
		{
			return !any();
		}

		// Возвращает копию, со сдвигом влево
		DJ_bitset<N> operator<<(size_t pos) const
		{
			return DJ_bitset(*this) <<= pos;
		}

		// Возвращает копию, со сдвигом вправо
		DJ_bitset<N> operator >> (size_t pos) const
		{
			return DJ_bitset(*this) >>= pos;
		}

		// Устанавливает все биты в false
		const DJ_bitset<N>& reset() const
		{
			for (size_t i = 0; i < mByteCount; ++i)
				mData[i] = 0;
			return *this;
		}

		// Устанавливает бит в false, отсчет номера бита от 0
		DJ_bitset<N>& reset(size_t bitPosition)
		{
			mData[ByteNumber(bitPosition)] &= ~(1 << BitNumber(bitPosition));
			return *this;
		}

		// Устанавливает все биты в true
		DJ_bitset<N>& set()
		{
			for (size_t i = 0; i < N; ++i)
				mData[ByteNumber(i)] |= (1 << BitNumber(i));
			return *this;
		}

		DJ_bitset<N>& set(size_t bitPosition, int value = true)
		{
			if (value)
				mData[ByteNumber(bitPosition)] |= (1 << BitNumber(bitPosition));
			else
				mData[ByteNumber(bitPosition)] &= ~(1 << BitNumber(bitPosition));
			return *this;
		}

		// Доступ к конкретному биту, нумерация начинается с 0
		bool test(size_t bitPosition) const
		{
			return (mData[ByteNumber(bitPosition)] & (1 << BitNumber(bitPosition))) != 0;
		}

	private:
		// Номер байта хранения, отсчет байтов начинается с 0
		size_t ByteNumber(const size_t& bitPosition) const
		{
			return bitPosition / CHAR_BIT;
		}

		// Номер бита в байте хранения, отсчет битов начинается с 0
		// Например, для 8 разрядной системы для 9 бита вернет 1 (00000010 00000000), второй по счету бит во втором байте хранения
		size_t BitNumber(const size_t& bitPosition) const
		{
			return bitPosition % CHAR_BIT;
		}

		// Количество byte для хранения данных, отсчет начинается с 1
		static const size_t mByteCount = (N + CHAR_BIT - 1) / CHAR_BIT;
		mutable char mData[mByteCount];
	};

	template <class charT, class traits, size_t N>
	basic_ostream<charT, traits>& operator<<(basic_ostream<charT, traits>& os, const DJ_bitset<N>& x)
	{
		for (size_t i = N; i > 0; --i)
			os << x.test(i - 1) ? "1" : "0";
		return os;
	}

} // namespace std