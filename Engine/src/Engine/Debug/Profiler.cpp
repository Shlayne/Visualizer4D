#include "Engine/pch.h"

#if ENABLE_PROFILE
#include "Engine/Debug/Profiler.h"
#include "Engine/Core/Core.h"
#include "Engine/Util/String.h"
#include <filesystem>

namespace eng
{
	InstrumentationSession::InstrumentationSession(std::string_view name)
		: name(name) {}

	void Profiler::BeginSession(std::string_view name)
	{
		std::lock_guard lock(m_Mutex);

		if (m_CurrentSession != nullptr)
			InternalEndSession();

		// Get filepath. TODO: see if this works.
		std::string filepathPrefix = "Profiles/";
		filepathPrefix += name;
		std::string filepath = filepathPrefix + ".json";
		uint32 index = 0;
		std::error_code error;
		while (!std::filesystem::exists(filepath, error))
		{
			filepath = filepathPrefix;
			std::string indexString;
			UNUSED(util::string::IToS(index, indexString));
			filepath += indexString;
			filepath += ".json";
		}

		// Create any directories that need to be created before opening the file
		size_t lastSlashIndex = filepath.find_last_of("/\\");
		if (lastSlashIndex != std::string::npos)
		{
			std::filesystem::path directoryPath = std::string_view(filepath).substr(0, lastSlashIndex);
			if (!std::filesystem::exists(directoryPath))
				std::filesystem::create_directories(directoryPath);
		}

		m_OutputStream.open(filepath);
		if (m_OutputStream.is_open())
		{
			m_CurrentSession = new InstrumentationSession(name);
			WriteHeader();
		}
	}

	void Profiler::EndSession()
	{
		std::lock_guard lock(m_Mutex);
		InternalEndSession();
	}

	void Profiler::WriteProfile(const ProfileResult& result)
	{
		std::lock_guard lock(m_Mutex);
		if (m_CurrentSession != nullptr)
		{
			std::stringstream json;

			json << std::setprecision(3) << std::fixed;
			json << ",{";
			json << "\"cat\":\"function\",";
			json << "\"dur\":" << result.elapsedTime.count() << ',';
			json << "\"name\":\"" << result.name << "\",";
			json << "\"ph\":\"X\",";
			json << "\"pid\":0,";
			json << "\"tid\":" << result.threadID << ',';
			json << "\"ts\":" << result.start.count();
			json << '}';

			m_OutputStream << json.str();
			m_OutputStream.flush();
		}
	}

	Profiler& Profiler::Get()
	{
		static Profiler s_Instance;
		return s_Instance;
	}

	Profiler::~Profiler()
	{
		EndSession();
	}

	void Profiler::WriteHeader()
	{
		m_OutputStream << "{\"otherData\":{},\"traceEvents\":[{}";
		m_OutputStream.flush();
	}

	void Profiler::WriteFooter()
	{
		m_OutputStream << "]}";
		m_OutputStream.flush();
	}

	void Profiler::InternalEndSession()
	{
		if (m_CurrentSession != nullptr)
		{
			WriteFooter();
			m_OutputStream.close();
			delete m_CurrentSession;
			m_CurrentSession = nullptr;
		}
	}

	ProfilerTimer::ProfilerTimer(const char* name)
		: m_Name(name), m_Stopped(false), m_StartTimepoint(std::chrono::steady_clock::now()) {}

	ProfilerTimer::~ProfilerTimer()
	{
		if (!m_Stopped)
			Stop();
	}

	void ProfilerTimer::Stop()
	{
		auto endTimepoint = std::chrono::steady_clock::now();
		FloatingPointMicroseconds highResStart = m_StartTimepoint.time_since_epoch();
		auto elapsedTime =
			std::chrono::time_point_cast<std::chrono::microseconds>(endTimepoint).time_since_epoch() -
			std::chrono::time_point_cast<std::chrono::microseconds>(m_StartTimepoint).time_since_epoch();

		Profiler::Get().WriteProfile({ m_Name, highResStart, elapsedTime, std::this_thread::get_id() });

		m_Stopped = true;
	}
}

#endif
