#pragma once

#include "Engine/Rendering/RenderCommand.h"
#include <condition_variable>
#include <mutex>
#include <deque>

namespace eng
{
	class RenderCommandQueue
	{
	public:
		void IssueCommand(RenderCommand command, bool highPriority = false);
		RenderCommand WaitForCommand();
		void ClearPendingCommands();
	private:
		std::deque<RenderCommand> m_Queue;
		std::condition_variable m_Condition;
		std::mutex m_Mutex;
	};
}
