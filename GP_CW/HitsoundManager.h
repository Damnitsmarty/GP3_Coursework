#pragma once
#include <vector>
#include <string>
#include "Mp3.h"
using namespace std;

class HitsoundManager {
public:
	HitsoundManager() ;
	static HitsoundManager* getInstance();
	void HitsoundManager::playSound();
private:
	static HitsoundManager* pInstance;
	vector<Mp3*> emitters;
	int currentSound = 0;
};