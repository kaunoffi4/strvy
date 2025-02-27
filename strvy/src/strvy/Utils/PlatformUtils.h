#pragma once

#include <string>

namespace strvy {

	class FileDialogs
	{
	public:
		// These return empty strings if cancelled
		static std::string openFile(const char* filter);
		static std::string saveFile(const char* filter);
	};

}