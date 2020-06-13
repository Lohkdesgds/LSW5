#pragma once

// C
#include <Windows.h>
#include <ShlObj.h>
// C++
#include <string>
#include <vector>
#include <sstream>
#include <chrono>
// Others
#include "..\abort\abort.h"

#define CHAR_INIT_LIMIT 512
#define CHAR_INIT(a) a, strnlen_s(a, CHAR_INIT_LIMIT)

#define MILI_NOW std::chrono::milliseconds(std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count())
#define MILI_ZERO std::chrono::milliseconds::zero()

namespace LSW {
	namespace v5 {
		namespace Tools {

			inline const char* paths_known[] = { "%appdata%", "%win_fonts%", "%win_photos_path%" }; //CSIDL_APPDATA, CSIDL_FONTS, CSIDL_MYPICTURES
			inline const char* known_size_ends[] = {"k", "M", "G", "T", "P", "E", "Z", "Y"};

			void createPath(std::string);
			LONGLONG getFileSize(const std::string&);
			bool _getFolderCSIDL(std::string&, const int&);
			void _clearPath(std::string&, const bool = false);

			// clear path and interpret %path%
			void interpretPath(std::string&);

			// do all the necessary stuff (to open a file already) and interpret path
			void handlePath(std::string&); // old: interpretClearAndCreatePath

			// FORMAT = "STARTSTUFF######ENDSTUFF" (# = number space)
			std::vector<std::string> genStrFormat(const std::string, const size_t, const size_t = 0);

			uint64_t getThreadID();

			// value, how many bytes to test?
			std::vector<bool> translBinary(const int, const size_t);

			// BASE10! auto conv bytes to string like 1000 -> 1 kB, with size itself and how many .numbers kB, bool is if you want "1.0 k" instead of "1.0k"
			std::string byteAutoString(double, const size_t = 1, const bool = false);

			// the string, the separator (like :, =, ...), comment characters and characters in the end aka endline // setting: value#comment
			std::vector<std::pair<std::string, std::string>> breakLines(const std::string, const std::string = ":= ", const std::string = "#", const std::string = "\r\n");

			template<size_t N> inline const size_t strlen_s(char(&str)[N]) { return strnlen_s(str, N); }

			template<typename T> const T* assert_cast(const T* w, const Abort::abort_level ab = Abort::abort_level::GIVEUP) { if (!w) { throw Abort::Abort(__FUNCSIG__, "assert_cast got exception: null", ab); } return w; }

			template<typename T, typename... Args> T* new_guaranteed(Args... args) { T* ptr = nullptr; while (!(ptr = new T(args...))); return ptr; }
		}
	}
}
