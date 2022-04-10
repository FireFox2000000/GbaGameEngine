#pragma once
#include <functional>
#include <vector>

template<typename... Params>
class CommandQueue
{
public:
	using tCommand = std::function<void(Params ...)>;
private:
	std::vector<tCommand> m_queue;

public:
	CommandQueue() = default;

	CommandQueue(const std::initializer_list<tCommand>& l)
	{
		m_queue.reserve(l.size());
		for (auto& val : l)
		{
			Enque(val);
		}
	}

	void Enque(const tCommand& command)
	{
		m_queue.emplace_back(command);
	}

	void ExecuteCommands(Params... params)
	{
		for (tCommand& command : m_queue)
		{
			command(params ...);
		}

		m_queue.clear();
	}

	void ExecuteNext(Params... params)
	{
		if (!IsEmpty())
		{
			m_queue[0](params ...);
			m_queue.erase(m_queue.begin());
		}
	}

	bool IsEmpty()
	{
		return m_queue.size() == 0;
	}
};
