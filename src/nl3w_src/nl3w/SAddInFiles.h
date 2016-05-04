#pragma once

#define MAX_ADDIN 16

class SAddInFiles
{
public:
	SAddInFiles(void);
	~SAddInFiles(void);

	HINSTANCE m_hAddin[MAX_ADDIN];
	int m_nAddinCount;
	void update(void);
private:
	void ReleaseAddin(void);


};

extern SAddInFiles g_addinFiles;
