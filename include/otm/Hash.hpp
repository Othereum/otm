#pragma once
#include <iterator>

namespace otm
{
	constexpr auto kHashOffsetBasis = []{ if constexpr(sizeof size_t >= 8) { return 14695981039346656037ULL; } else { return 2166136261U; } }();
	constexpr auto kHashPrime = []{ if constexpr(sizeof size_t >= 8) { return 1099511628211ULL; } else { return 16777619U; } }();

	template <std::input_iterator It, std::invocable<typename std::iterator_traits<It>::value_type> Fn>
	constexpr size_t HashRange(size_t val, It first, It last, Fn modifier) noexcept
	{
	    for (; first != last; ++first)
		{
	        val ^= modifier(*first);
	        val *= kHashPrime;
	    }
	    return val;
	}

	template <std::input_iterator It>
	constexpr size_t HashRange(size_t val, It first, It last) noexcept
	{
	    return HashRange(val, first, last, []<class T>(T&& x){return std::forward<T>(x);});
	}

	template <std::input_iterator It, std::invocable<typename std::iterator_traits<It>::value_type> Fn>
	constexpr size_t HashRange(It first, It last, Fn modifier) noexcept
	{
		return HashRange(kHashOffsetBasis, first, last, modifier);
	}

	template <std::input_iterator It>
	constexpr size_t HashRange(It first, It last) noexcept
	{
		return HashRange(kHashOffsetBasis, first, last);
	}
}
