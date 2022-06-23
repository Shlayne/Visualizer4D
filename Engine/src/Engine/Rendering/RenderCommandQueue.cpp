#include "Engine/pch.h"
#include "Engine/Rendering/RenderCommandQueue.h"
#include "Engine/Core/Core.h"

namespace eng
{
	void RenderCommandQueue::IssueCommand(RenderCommand command, bool highPriority)
	{
		//CORE_ASSERT(command.GetType() < RenderCommand_Count, "Attempted to issue invalid Render Command!");

		//std::lock_guard<std::mutex> lock(m_Mutex);
		//if (highPriority) // If the command is high priority, call it next.
		//	m_Queue.push_front(command);
		//else // Otherwise, it will be called after other all commands in the queue.
		//	m_Queue.push_back(command);
		//// Notify the render thread that a command has been issued.
		//m_Condition.notify_one();
	}

	RenderCommand RenderCommandQueue::WaitForCommand()
	{
		// Wait for a command to be issued.
		std::unique_lock<std::mutex> lock(m_Mutex);
		while (m_Queue.empty())
			m_Condition.wait(lock);

		// Dequeue and return the command.
		RenderCommand command = m_Queue.front();
		m_Queue.pop_front();
		return command;
	}

	void RenderCommandQueue::ClearPendingCommands()
	{
		std::lock_guard<std::mutex> lock(m_Mutex);
		m_Queue.clear();
	}
}
