#include "gfx.hpp"

GraphicsSettings::GraphicsSettings() {
	this->title = "DEFAULT";
	this->verticalSync = false;
	this->resolution = sf::VideoMode::getDesktopMode();
	this->fullscreen = false;
	this->frameRateLimit = 120;
	this->contextSettings.antialiasingLevel = 0;
	this->videoModes = sf::VideoMode::getFullscreenModes();
	this->gridSize = 32.f;
}

//Functions
void GraphicsSettings::saveToFile(const std::string path) {
	std::ofstream ofs(path);

	if (ofs.is_open()) {
		ofs << this->title << '\n';
		ofs << this->resolution.width << " " << this->resolution.height << '\n';
		ofs << this->fullscreen << '\n';
		ofs << this->frameRateLimit << '\n';
		ofs << this->verticalSync << '\n';
		ofs << this->contextSettings.antialiasingLevel << '\n';
		ofs << this->gridSize;
	}
	ofs.close();
}

void GraphicsSettings::loadFromFile(const std::string path) {
	std::ifstream ifs(path);

	if (ifs.is_open()) {
		std::getline(ifs, this->title);
		ifs >> this->resolution.width >> this->resolution.height;
		ifs >> this->fullscreen;
		ifs >> this->frameRateLimit;
		ifs >> this->verticalSync;
		ifs >> this->contextSettings.antialiasingLevel;
		ifs >> this->gridSize;
	}
	ifs.close();
}