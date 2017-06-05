// Sound.cpp: implementation of the CSound class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Example2_ChatRoom.h"
#include "Sound.h"
#include "Example2_ChatRoomDlg.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSound::CSound()
{

}

CSound::~CSound()
{
	waveInClose(m_hWaveIn); //关闭录音设备
	waveOutClose(m_hWaveOut); //关闭录音设备
}
void CSound::Init(CExample2_ChatRoomDlg *dlg)
{
	int result;
	m_dlg=dlg;
	if(waveInGetNumDevs()==0)
	{
		AfxMessageBox("There is no sound input device");
	}
	if(waveOutGetNumDevs()==0)
	{
		AfxMessageBox("There is no sound output device");
	}
	m_soundFormat.wFormatTag=WAVE_FORMAT_PCM;
	m_soundFormat.nChannels=1;
	m_soundFormat.nSamplesPerSec=8000;
	m_soundFormat.nAvgBytesPerSec=16000;
	m_soundFormat.nBlockAlign=2;
	m_soundFormat.cbSize=0;
	m_soundFormat.wBitsPerSample=16;     //指定录音格式

	int  res=waveInOpen(&m_hWaveIn,WAVE_MAPPER, &m_soundFormat, (DWORD)m_dlg->m_hWnd,0L,CALLBACK_WINDOW); //打开录音设备
	if(res!= MMSYSERR_NOERROR)
	{
		AfxMessageBox("Error in waveInOpen function");
		return ;
	}
	res=waveOutOpen(&m_hWaveOut,WAVE_MAPPER, &m_soundFormat, (DWORD)m_dlg->m_hWnd,0L,CALLBACK_WINDOW); //打开录音设备
	if(res!= MMSYSERR_NOERROR)
	{
		AfxMessageBox("Error in waveOutOpen function");
		return ;
	}
	//in
	m_pWaveHdrIn[0].lpData=m_cBufferIn;
	m_pWaveHdrIn[0].dwBufferLength=MAX_BUFFER_SIZE;
	m_pWaveHdrIn[0].dwBytesRecorded=0;
	m_pWaveHdrIn[0].dwFlags=0;

	result=waveInPrepareHeader(m_hWaveIn,&m_pWaveHdrIn[0],sizeof(WAVEHDR)); //准备内存块录音
    if  (result!= MMSYSERR_NOERROR) 
    {
        AfxMessageBox(_T("Cannot InPrepareHeader !"));
        return;
    };

	result=waveInAddBuffer(m_hWaveIn,&m_pWaveHdrIn[0],sizeof(WAVEHDR)); //增加内存块
    if  (result!= MMSYSERR_NOERROR) 
    {
        AfxMessageBox(_T("Cannot InAddBuffer !"));
        return;
    };

	//out
	m_pWaveHdrOut[0].lpData=m_cBufferOut;
	m_pWaveHdrOut[0].dwBufferLength=MAX_BUFFER_SIZE;
	m_pWaveHdrOut[0].dwBytesRecorded=0;
	m_pWaveHdrOut[0].dwFlags=0;

	waveOutPrepareHeader(m_hWaveOut,&m_pWaveHdrOut[0],sizeof(WAVEHDR)); //准备内存块录音
	waveOutWrite(m_hWaveOut,&m_pWaveHdrOut[0],sizeof(WAVEHDR));
}

void CSound::Record()
{
	waveInStart(m_hWaveIn);//开始录音
}

void CSound::Play()
{
	memcpy(m_cBufferOut,m_cBufferIn,MAX_BUFFER_SIZE);
}

void CSound::StopRecord()
{
	waveInStop(m_hWaveIn); //停止录音
	waveInReset(m_hWaveIn); //清空内存块
}

void CSound::FreeRecordBuffer()
{
	int result=waveInUnprepareHeader(m_hWaveIn,&m_pWaveHdrIn[0],sizeof(WAVEHDR));
    if  (result!= MMSYSERR_NOERROR) 
    {
        AfxMessageBox(_T("Cannot UnInPrepareHeader !"));
        return;
    };	//in

	m_pWaveHdrIn[0].lpData=m_cBufferIn;
	m_pWaveHdrIn[0].dwBufferLength=MAX_BUFFER_SIZE;
	m_pWaveHdrIn[0].dwBytesRecorded=0;
	m_pWaveHdrIn[0].dwFlags=0;

	result=waveInPrepareHeader(m_hWaveIn,&m_pWaveHdrIn[0],sizeof(WAVEHDR)); //准备内存块录音
    if  (result!= MMSYSERR_NOERROR) 
    {
        AfxMessageBox(_T("Cannot InPrepareHeader !"));
        return;
    };	//in
	result=waveInAddBuffer(m_hWaveIn,&m_pWaveHdrIn[0],sizeof(WAVEHDR)); //增加内存块
    if  (result!= MMSYSERR_NOERROR) 
    {
        AfxMessageBox(_T("Cannot InAddBuffer !"));
        return;
    };	//in
}
void CSound::FreePlayBuffer()
{

	static bool once=true;
	int result;
	
	result=waveOutUnprepareHeader(m_hWaveOut,&m_pWaveHdrOut[0],sizeof(WAVEHDR));
 /*   
	if  (result!= MMSYSERR_NOERROR) 
    {
		if(once)
		{
			char  errinfor[100];
			waveOutGetErrorText(result,errinfor,100);
			AfxMessageBox(_T("Cannot UnOutPrepareHeader !"));
			AfxMessageBox(errinfor);
			once=false;
		}
        return;
    };	//out		//这个地方太怪了，老是返回错误，所以我干脆把它给注销掉，居然能
	//听出声音，^_^
*/	
	//	waveOutReset(m_hWaveOut); //清空内存块

	m_pWaveHdrOut[0].lpData=m_cBufferOut;
	m_pWaveHdrOut[0].dwBufferLength=MAX_BUFFER_SIZE;
	m_pWaveHdrOut[0].dwBytesRecorded=0;
	m_pWaveHdrOut[0].dwFlags=0;

	result=waveOutPrepareHeader(m_hWaveOut,&m_pWaveHdrOut[0],sizeof(WAVEHDR)); //准备内存块录音
    if  (result!= MMSYSERR_NOERROR) 
    {
        AfxMessageBox(_T("Cannot OutPrepareHeader !"));
        return;
    };	//out
	result=waveOutWrite(m_hWaveOut,&m_pWaveHdrOut[0],sizeof(WAVEHDR)); //增加内存块
    if  (result!= MMSYSERR_NOERROR) 
    {
        AfxMessageBox(_T("Cannot OutWrite !"));
        return;
    };	//out
}
void CSound::InBufferZero()
{
	int i;
	for(i=0;i<MAX_BUFFER_SIZE;i++)
	{
		m_cBufferIn[i]=0;
	}
}

void CSound::OutBufferZero()
{
	int i;
	for(i=0;i<MAX_BUFFER_SIZE;i++)
	{
		m_cBufferOut[i]=0;
	}
}