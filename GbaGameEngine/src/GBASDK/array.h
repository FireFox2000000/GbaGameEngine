#pragma once

#include <string.h> // memcpy
#include <initializer_list>

namespace GBA
{
	using u32 = unsigned long;

	// A wrapper around regular C++ arrays with a bunch of extra helper features. 
	// Do not add any more member variables to this! Need to make sure this can reinterpret_cast to regular arrays. 

	template<class T, u32 SIZE>
	class array
	{
		T m_buffer[SIZE];

		inline T& Get(u32 index) { return m_buffer[index]; }
		inline const T& Get(u32 index) const { return m_buffer[index]; }
		inline volatile T& Get(u32 index) volatile { return m_buffer[index]; }

	public:
		typedef T* iterator;
		typedef const T* const_iterator;
		typedef volatile T* volatile_iterator;

		array()
		{
		}

		array(const T& initialValue)
		{
			SetAllTo(initialValue);
		}

		array(const Array<T, SIZE>& that) { *this = that; }

		array(const std::initializer_list<T>& l)
		{
			u32 i = 0;
			for (auto& val : l)
			{
				m_buffer[i] = T(val);
				if (++i >= size())
				{
					break;
				}
			}
		}

		~array() {}

		const T* data() const
		{
			return &m_buffer;
		}

		static inline constexpr u32 size() { return SIZE; }

		iterator begin() { return &Get(0); }
		const_iterator begin() const { return &Get(0); }
		volatile_iterator begin() volatile { return &Get(0); }

		iterator end() { return &Get(size()); }
		const_iterator end() const { return &Get(size()); }
		volatile_iterator end() volatile { return &Get(size()); }

		inline T& operator[](u32 index) { return Get(index); }
		inline const T& operator[](u32 index) const { return Get(index); }
		inline volatile T& operator[](u32 index) volatile { return Get(index); }

		inline T& at(u32 index) { return Get(index); }
		inline const T& at(u32 index) const { return Get(index); }
		inline volatile T& at(u32 index) volatile { return Get(index); }

		inline int IndexOf(T* item)
		{
			return int(item - begin());
		}

		inline int IndexOf(T* item) volatile
		{
			return int(item - begin());
		}

		int IndexOf(const T& item) const
		{
			for (u32 i = 0; i < size(); ++i)
			{
				if (item == Get(i))
				{
					return (int)i;
				}
			}

			return -1;
		}

		inline bool Contains(const T& item) const
		{
			return IndexOf(item) >= 0;
		}

		void SetAllTo(const T& val)
		{
			for (u32 i = 0; i < size(); ++i)
			{
				m_buffer[i] = T(val);	// Todo, maybe use this instead?- new (&(m_buffer[i])) T();
			}
		}
	};

	template<class T>
	class array<T, 0>
	{
	public:
		typedef T* iterator;
		typedef const T* const_iterator;
		typedef volatile T* volatile_iterator;

		static inline u32 size() { return 0; }

		iterator begin() { return nullptr; }
		const_iterator begin() const { return nullptr; }
		volatile_iterator begin() volatile { return nullptr; }

		iterator end() { return nullptr; }
		const_iterator end() const { return nullptr; }
		volatile_iterator end() volatile { return nullptr; }
	};

}
