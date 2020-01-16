#pragma once
#include "engine/base/Macros.h"
#include "engine/base/core/stl/SharedPtr.h"

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

	SharedPtr<IState> m_current = nullptr;

public:

	void ChangeState(SharedPtr<IState> nextState, Params... params)
	{
		if (m_current)
			m_current->Exit(params...);

		m_current = nextState;

		m_current->Enter(params...);
	}

	void Update(Params... params)
	{
		if (m_current)
			m_current->Update(params...);
	}
};
