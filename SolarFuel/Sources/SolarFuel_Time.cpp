#include "..\Headers\SolarFuel.hpp"



static bool LocalInitialized = false;
static TIMECAPS LocalTimeCaps = { 0 };

const bool& SolarFuel::Time::Initialized = LocalInitialized;
const TIMECAPS& SolarFuel::Time::TimeCaps = LocalTimeCaps;



bool SolarFuel::Time::Init()
{
	if (LocalInitialized == true)
	{
		return true;
	}

	if (timeGetDevCaps(&LocalTimeCaps, sizeof(TIMECAPS)) != MMSYSERR_NOERROR)
	{
		return false;
	}

	if (timeBeginPeriod(LocalTimeCaps.wPeriodMin) != TIMERR_NOERROR)
	{
		return false;
	}

	LocalInitialized = true;

	return true;
}

void SolarFuel::Time::Stop()
{
	if (!LocalInitialized)
	{
		return;
	}

	timeEndPeriod(LocalTimeCaps.wPeriodMin);

	LocalInitialized = false;
	LocalTimeCaps = { 0 };
}



SolarFuel::Time::Timer::Timer() : Begin(std::chrono::system_clock::now()), End()
{
	End = Begin;
}

SolarFuel::Time::Timer::Timer(const Timer& _Other) : Begin(_Other.Begin), End(_Other.End)
{

}

SolarFuel::Time::Timer::Timer(Timer&& _Other) noexcept : Begin(std::move(_Other.Begin)), End(std::move(_Other.End))
{

}

SolarFuel::Time::Timer::~Timer()
{

}

void SolarFuel::Time::Timer::Start()
{
	Begin = std::chrono::system_clock::now();
	End = Begin;
}

void SolarFuel::Time::Timer::Stop()
{
	End = std::chrono::system_clock::now();
}

SolarFuel::Time::Timer::operator const float() const
{
	return std::chrono::duration<float>(End - Begin).count();
}

SolarFuel::Time::Timer& SolarFuel::Time::Timer::operator= (const Timer& _Other)
{
	Begin = _Other.Begin;
	End = _Other.End;

	return *this;
}

SolarFuel::Time::Timer& SolarFuel::Time::Timer::operator= (Timer&& _Other) noexcept
{
	Begin = std::move(_Other.Begin);
	End = std::move(_Other.End);

	return *this;
}
