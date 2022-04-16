#pragma once

struct CRandomEncounters {
	struct Settings {
		static constexpr float event_system_interval = 1000.f; // millis
		static const bool is_enabled = true;
	} settings;
};

struct CInGameConfigWrapper {
    const char* GetVarValue(const char *, const char *) { return "2.0"; }
};

#include <iostream>
#define LogChannel(ch,s) ( std::cout << "[" << ch << "] " << s << std::endl )

struct Game {
    CInGameConfigWrapper GetInGameConfigWrapper() { return CInGameConfigWrapper(); }
} theGame;

#include <thread>
#define Sleep(d) (std::this_thread::sleep_for( std::chrono::duration<float,std::milli>(d) ))