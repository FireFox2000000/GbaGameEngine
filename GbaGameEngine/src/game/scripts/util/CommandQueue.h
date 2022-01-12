#pragma once
#include <functional>
#include <vector>

class CommandQueue
{
public:
	using tCommand = std::function<void()>;
private:
	std::vector<tCommand> m_queue;

public:
	void Enque(tCommand command)
	{
		m_queue.emplace_back(command);
	}

	void ExecuteCommands()
	{
		for (tCommand& command : m_queue)
		{
			command();
		}

		m_queue.clear();
	}
};
