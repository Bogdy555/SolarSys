#ifndef SolarFuel_Time_hpp

#define SolarFuel_Time_hpp



#include "SolarFuel.hpp"



namespace SolarFuel
{

	namespace Time
	{

		extern const bool& Initialized;
		extern const TIMECAPS& TimeCaps;

		bool Init();
		void Stop();

		class Timer
		{

		public:

			Timer();
			Timer(const Timer& _Other);
			Timer(Timer&& _Other) noexcept;
			~Timer();

			void Start();
			void Stop();

			operator const float() const;

			Timer& operator= (const Timer& _Other);
			Timer& operator= (Timer&& _Other) noexcept;

		private:

			std::chrono::system_clock::time_point Begin;
			std::chrono::system_clock::time_point End;

		};

	}

}



#endif
