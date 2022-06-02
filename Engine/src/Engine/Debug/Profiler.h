#pragma once

#if ENABLE_PROFILE
#include <algorithm>
#include <chrono>
#include <fstream>
#include <iomanip>
#include <mutex>
#include <sstream>
#include <string>
#include <thread>

namespace eng
{
	using FloatingPointMicroseconds = std::chrono::duration<double, std::micro>;

	struct ProfileResult
	{
		std::string name;

		FloatingPointMicroseconds start;
		std::chrono::microseconds elapsedTime;
		std::thread::id threadID;
	};

	struct InstrumentationSession
	{
		InstrumentationSession(std::string_view name);

		std::string name;
	};

	class Profiler
	{
	public:
		Profiler() = default;
		Profiler(const Profiler&) = delete;
		Profiler(Profiler&&) = delete;
		static Profiler& Get();
		~Profiler();
	public:
		void BeginSession(std::string_view name);
		void EndSession();
		void WriteProfile(const ProfileResult& result);
	private:
		void WriteHeader();
		void WriteFooter();
		void InternalEndSession(); // You must already own lock on mutex before calling this.
	private:
		std::mutex m_Mutex;
		InstrumentationSession* m_CurrentSession = nullptr;
		std::ofstream m_OutputStream;
	};

	class ProfilerTimer
	{
	public:
		ProfilerTimer(const char* name);
		~ProfilerTimer();
	public:
		void Stop();
	private:
		const char* m_Name;
		std::chrono::time_point<std::chrono::steady_clock> m_StartTimepoint;
		bool m_Stopped;
	};
}
#endif

#if CONFIG_PROFILE
	#define PROFILE_BEGIN_RUNTIME(name) ::eng::Profiler::Get().BeginSession(name)
	#define PROFILE_END_RUNTIME() ::eng::Profiler::Get().EndSession()
	#define PROFILE_BEGIN(name)
	#define PROFILE_END()
	#define PROFILE_SCOPE(name) ::eng::ProfilerTimer timer##__LINE__(name)
	#define PROFILE_FUNCTION() PROFILE_SCOPE(__FUNCSIG__)
#elif ENABLE_PROFILE
	#define PROFILE_BEGIN_RUNTIME(name)
	#define PROFILE_END_RUNTIME()
	#define PROFILE_BEGIN(name) ::eng::Profiler::Get().BeginSession(name)
	#define PROFILE_END() ::eng::Profiler::Get().EndSession()
	#define PROFILE_SCOPE(name) ::eng::ProfilerTimer timer##__LINE__(name)
	#define PROFILE_FUNCTION() PROFILE_SCOPE(__FUNCSIG__)
#else
	#define PROFILE_BEGIN_RUNTIME(name)
	#define PROFILE_END_RUNTIME()
	#define PROFILE_BEGIN(name)
	#define PROFILE_END()
	#define PROFILE_SCOPE(name)
	#define PROFILE_FUNCTION()
#endif
