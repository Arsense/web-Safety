// Sound.h: interface for the CSound class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SOUND_H__69DAFAB9_C3EE_4821_8E74_9D069B9DECB9__INCLUDED_)
#define AFX_SOUND_H__69DAFAB9_C3EE_4821_8E74_9D069B9DECB9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include <mmsystem.h>

#define MAX_BUFFER_SIZE 2048

class CExample2_ChatRoomDlg;

class CSound  
{
public:
	void Init(CExample2_ChatRoomDlg * dlg);
	void Record();
	void Play();
	void StopRecord();
	void StopPlay();
	void FreeRecordBuffer();
	void FreePlayBuffer();
	void InBufferZero();
	void OutBufferZero();
	CSound();
	virtual ~CSound();
public:
	CExample2_ChatRoomDlg * m_dlg;
	WAVEFORMATEX            m_soundFormat;
	HWAVEIN					m_hWaveIn;
	HWAVEOUT				m_hWaveOut;
	WAVEHDR					m_pWaveHdrIn[3];
	WAVEHDR					m_pWaveHdrOut[3];
	CHAR					m_cBufferIn[MAX_BUFFER_SIZE];
	CHAR					m_cBufferOut[MAX_BUFFER_SIZE];
	
};

#endif // !defined(AFX_SOUND_H__69DAFAB9_C3EE_4821_8E74_9D069B9DECB9__INCLUDED_)
