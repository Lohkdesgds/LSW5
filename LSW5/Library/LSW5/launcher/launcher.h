#pragma once

// C
#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
// C++
#include <functional>
#include <thread>
#include <string>
#include <sstream>
#include <algorithm>

namespace LSW {
	namespace v5 {

		class Launcher {
			HANDLE g_hChildStd_OUT_Rd = nullptr;
			HANDLE g_hChildStd_OUT_Wr = nullptr;
			SECURITY_ATTRIBUTES saAttr{};

			PROCESS_INFORMATION piProcInfo{}; // handle = piProcInfo.hProcess
			STARTUPINFO siStartInfo{};

			char inter_buf[8];

			std::function<void(const std::string)> prunt = std::function<void(const std::string)>();
			std::thread* autosav = nullptr;
			bool keep = false;
			bool still_running = false;

			void keep_reading();
		public:
			~Launcher();

			void hookPrint(const std::function<void(const std::string)>);
			bool launch(std::string);
			bool stillRunning();
			void kill();
		};

	}
}