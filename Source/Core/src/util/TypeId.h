#pragma once

#include "../Common.h"

namespace Detail
{
    // https://stackoverflow.com/questions/40993441/constexpr-tricks

	template <typename T> 
    class Flag
    {
        struct Dummy
        {
            constexpr Dummy() { }
            friend constexpr void adl_flag(Dummy);
        };

        template <bool>
        struct Writer
        {
            friend constexpr void adl_flag(Dummy) {}
        };

        template <class Dummy, int = (adl_flag(Dummy{}), 0) >
        static constexpr bool Check(int)
        {
            return true;
        }

        template <class Dummy>
        static constexpr bool Check(short)
        {
            return false;
        }

    public:

        template <class Dummy = Dummy, bool Value = Check<Dummy>(0)>
        static constexpr bool ReadSet()
        {
            Writer<Value && 0> tmp{};
            (void)tmp;
            return Value;
        }

        template <class Dummy = Dummy, bool Value = Check<Dummy>(0)>
        static constexpr int Read()
        {
            return Value;
        }
    };

    template <typename T, int I>
    struct Tag
    {

        constexpr int Value() const noexcept
        {
            return I;
        }
    };

    template<typename T, int N, bool B>
    struct Checker
    {
        static constexpr int CurrentVal() noexcept
        {
            return N;
        }
    };

    template<typename T, int N>
    struct CheckerWrapper
    {
        template<bool B = Flag<Tag<T, N>>{}.Read(), int M = Checker<T, N, B>{}.CurrentVal() >
        static constexpr int CurrentVal()
        {
            return M;
        }
    };

    template<typename T, int N>
    struct Checker<T, N, true>
    {
        template<int M = CheckerWrapper<T, N + 1>{}.CurrentVal() >
        static constexpr int CurrentVal() noexcept
        {
            return M;
        }
    };

    template<typename T, int N, bool B = Flag<Tag<T, N>>{}.ReadSet() >
    struct Next
    {
        static constexpr int Value() noexcept
        {
            return N;
        }
    };
}

namespace Quartz
{
	template<typename Type>
	class TypeId
	{
	private:
		constexpr static UInt64 Hash(const char* signature)
		{
			const char* val = signature;
			UInt64 value = 14695981039346656037ull;

			while (*val != 0)
			{
				value = (value ^ static_cast<UInt64>(*(val++))) * 1099511628211ull;
			}

			return value;
		}

		template<USize StrSize>
		constexpr static UInt64 TypeHash(const char (&signature)[StrSize])
		{
			return Hash(signature);
		}

	public:
		constexpr static UInt64 Value()
		{
			constexpr UInt64 value = static_cast<UInt64>(TypeHash(__FUNCSIG__));
			return value;
		}
	};

    template<typename Type>
    class TypeName
    {
    public:
        constexpr static const char* Value()
        {
            // TODO: Doesn't actually work
            #define TypeStr(type) #type
            return TypeStr(Type);
        }
    };

    template <class CounterTag = void>
    class TypeCounter
    {
    public:
        template <int N = Detail::CheckerWrapper<CounterTag, 0>{}.CurrentVal() >
        static constexpr int Next()
        {
            return Detail::Next<CounterTag, N>{}.Value();
        }
    };

	template<typename CategoryType, typename Type>
	class TypeIndex
	{
    private:
        constexpr static UInt64 mIndex = TypeCounter<CategoryType>::Next();

    public:
		constexpr static UInt64 Value()
		{
			return mIndex;
		}
	};
}