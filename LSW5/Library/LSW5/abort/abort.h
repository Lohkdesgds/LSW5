#pragma once

// C++
#include <string>
#include <functional>

namespace LSW {
	namespace v5 {
		namespace Abort {

			enum class abort_level{GIVEUP,WARN,FATAL_ERROR};

			class Abort {
				std::string					s_from_where,
											s_details;
				abort_level					abort_lvl = abort_level::FATAL_ERROR;
				std::function<void(void)>	quick_exit = std::function<void(void)>();
			public:
				Abort(std::string, std::string, abort_level = abort_level::FATAL_ERROR, std::function<void(void)> = std::function<void(void)>());

				const std::string& getWhereFrom();
				const std::string& getDetails();
				const abort_level& getLevel();
				void runFunctionIfAny();
			};

		}
	}
}