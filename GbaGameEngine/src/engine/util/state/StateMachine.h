#pragma once
#include "engine/base/Macros.h"

template<typename... Params>
class StateMachine
{
public:
	class IState
	{
		friend class StateMachine<Params...>;

	protected:
		virtual void Enter(Params... params) {}
		virtual void Update(Params... params) {}
		virtual void Exit(Params... params) {}

		virtual ~IState() = default;
	};

	IState* m_current = nullptr;

public:
	template<typename STATE, typename... ConstructorArgs>
	void ChangeState(Params... params, ConstructorArgs... args)
	{
		STATIC_ASSERT(IS_BASE_OF(IState, STATE), "SceneManager::Change must be provided a type that derives from Scene.h");

		if (m_current)
			m_current->Exit(params...);

		delete m_current;

		m_current = new STATE(args...);

		m_current->Enter(params...);
	}

	void Update(Params... params)
	{
		if (m_current)
			m_current->Update(params...);
	}
};
