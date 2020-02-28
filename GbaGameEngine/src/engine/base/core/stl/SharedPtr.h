#pragma once

namespace _SharedPtr
{
	template<class T>
	struct SharedPtrObj
	{
	private:
		typename std::aligned_storage<sizeof(T), alignof(T)>::type m_obj[1];	// Prevents constructor from running when creating the null version of this object
		int m_refCount = 0;

	public:
		SharedPtrObj() : m_refCount(-1) {}

		template <typename... ConstructorArgs>
		SharedPtrObj(int initRefCount, ConstructorArgs... args) : m_refCount(initRefCount) {
			new(&m_obj[0]) T(args...);
		}

		inline void IncRef() { ++m_refCount; }
		inline void DecRef() { --m_refCount; }
		inline int GetRefCount() const { return m_refCount; }

		bool IsNull() const { return GetRefCount() < 0; }

		inline T& Get() { return *reinterpret_cast<T*>(&m_obj[0]); }
		inline const T& Get() const { return *reinterpret_cast<const T*>(&m_obj[0]); }

		const static SharedPtrObj Null;
	};

	template<class T>
	struct DefaultDelete
	{
		void operator()(SharedPtrObj<T>* sharedPtrObj) const {
			sharedPtrObj->Get().~T();
			delete sharedPtrObj;
		}
	};

	template<class T, class Deleter = DefaultDelete<T>>
	class SharedPtr_Base
	{
	protected:
		SharedPtrObj<T>* m_obj = nullptr;

		SharedPtr_Base() {}
		SharedPtr_Base(SharedPtrObj<T>* obj) : m_obj(obj) {}

		void Dispose() {
			Deleter()(m_obj);
		}

	public:
		SharedPtr_Base(const SharedPtr_Base<T, Deleter>& that)
		{
			m_obj = that.m_obj;
			if (m_obj)		// Guard against null
				m_obj->IncRef();
		}

		~SharedPtr_Base() {
			if (m_obj)		// Guard against null
			{
				m_obj->DecRef();
				if (m_obj->GetRefCount() <= 0)
				{
					Dispose();
				}
			}
		}

		inline bool operator == (const SharedPtr_Base<T, Deleter>& other) const
		{
			return m_obj == other.m_obj;
		}

		inline bool operator != (const SharedPtr_Base<T, Deleter>& other) const
		{
			return !(*this == other);
		}

		inline bool operator == (const void* other) const
		{
			const void* ptr = m_obj->IsNull() ? nullptr : &m_obj->Get();
			return ptr == other;
		}

		inline bool operator != (const void* other) const
		{
			return !(*this == other);
		}

		inline const T& operator * () const
		{
			return m_obj->Get();
		}

		inline T& operator * ()
		{
			return m_obj->Get();
		}

		inline const T* operator->() const {
			return &m_obj->Get();
		}

		inline T* operator->() {
			return &m_obj->Get();
		}

		explicit inline operator bool() const {
			return m_obj;
		}
	};
}

template<typename T>
class SharedPtr : public _SharedPtr::SharedPtr_Base<T>
{
public:
	using tSharedPtrObj = _SharedPtr::SharedPtrObj<T>;

private:
	SharedPtr(tSharedPtrObj* obj) : _SharedPtr::SharedPtr_Base<T>(obj) {}

public:

	template <typename... ConstructorArgs>
	static SharedPtr MakeShared(ConstructorArgs... args)
	{
		tSharedPtrObj* data = new tSharedPtrObj(0, args...);
		data->IncRef();
		return SharedPtr(data);
	}
};

template <typename T> const _SharedPtr::SharedPtrObj<T> _SharedPtr::SharedPtrObj<T>::Null = _SharedPtr::SharedPtrObj<T>();
