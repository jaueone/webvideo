// WaveSound.cpp: implementation of the CWaveSound class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "WaveSound.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif




//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CWaveSound::CWaveSound():m_mutex(FALSE)
{
	m_hWaveOut = NULL;
	m_hEventSound = NULL;
	m_bOpenSound = FALSE;
	m_pWaveHead = NULL;
	m_nBlockSize = 0;
	m_nBlockNum = 0;
	m_nWavePlayCount = 0;
	m_nCurUser = 0;
	m_nVolume = 0xffff;
	m_nPort = -1;
	m_pRefBuf = NULL;

}

CWaveSound::~CWaveSound()
{
	if (m_bOpenSound)
	{
		CloseSound();
	}

}

BOOL CWaveSound::OpenSound(WAVEFORMATEX& wfex, DWORD nBlockNum, DWORD nBlockSize, DWORD nFlag, LONG nPort)
{
	if (m_bOpenSound)
	{
		return FALSE;
	}
	m_nPort = nPort;
	m_bOpenSound = TRUE;
	MMRESULT hMMRet;
	DWORD dwCallBack;
	if (nFlag == CALLBACK_EVENT)
	{
		m_hEventSound = CreateEvent(NULL, FALSE, FALSE, NULL);
		dwCallBack = (DWORD)m_hEventSound;;
	}
	else if (nFlag == CALLBACK_FUNCTION)
	{
		dwCallBack = (DWORD)waveOutProc;
	}
	else
	{
		return FALSE;
	}
	m_nWaveWriteCount = 0;
	m_nWavePlayCount = 0;
	m_nCurUser = 0;
	m_nBlockSize = nBlockSize;
	m_nBlockNum = nBlockNum;
	m_pWaveHead = new WAVEHDR[nBlockNum];
	ZeroMemory(m_pWaveHead, sizeof(WAVEHDR) * m_nBlockNum);
	m_pRefBuf = new char [m_nBlockSize];
	for (DWORD nCount=0; nCount<m_nBlockNum; nCount++)
	{
		m_pWaveHead[nCount].lpData = new char[m_nBlockSize];
		m_pWaveHead[nCount].dwBufferLength = m_nBlockSize;
		m_pWaveHead[nCount].dwFlags = 0;
		m_pWaveHead[nCount].dwUser = 0;
	}

	
    if (hMMRet = waveOutOpen(&m_hWaveOut,
                   WAVE_MAPPER,
                   (LPWAVEFORMATEX) &wfex,
                   dwCallBack,
                   (DWORD)this,
                   WAVE_ALLOWSYNC |nFlag) != MMSYSERR_NOERROR )
	{
		TRACE("Open audio fail\n");
		CloseSound();
		return FALSE;
	}
	
	waveOutPause(m_hWaveOut);
	
	return TRUE;
}

void CALLBACK CWaveSound::waveOutProc(HWAVEOUT hwo, UINT uMsg, DWORD dwInstance, DWORD dwParam1, DWORD dwParam2)
{
	CWaveSound*pOwner = (CWaveSound*)dwInstance;
	LPWAVEHDR lpWaveHdr = (LPWAVEHDR) dwParam1;
	switch (uMsg)
	{
		case WOM_DONE:
			
			pOwner->m_nWavePlayCount++;
			pOwner->m_nCurUser = lpWaveHdr->dwUser;
			//TRACE("Audio play:%d\n",pOwner->m_nWavePlayCount);
			break;
	}

}

BOOL CWaveSound::CloseSound()
{
	MMRESULT hMMRet;
	if (!m_bOpenSound)
	{
		return TRUE;
	}
	m_bOpenSound = FALSE;
	//�޸��ڴ��豸ʧ��ʱ��ҲҪ�ͷ���Դ��
	if (m_hWaveOut)
	{	
		hMMRet = waveOutReset(m_hWaveOut);
		hMMRet = waveOutClose(m_hWaveOut);
	}

	if (!m_pWaveHead)
	{
		return TRUE;
	}
	for (DWORD nCount=0; nCount<m_nBlockNum; nCount++)
	{
		if (m_hWaveOut)
		{
			hMMRet = waveOutUnprepareHeader(m_hWaveOut, &m_pWaveHead[nCount], sizeof(WAVEHDR));
		}
		if (m_pWaveHead[nCount].lpData)
		{
			delete[] m_pWaveHead[nCount].lpData;
			m_pWaveHead[nCount].lpData = NULL;
		}
	}
	delete[] m_pWaveHead;
	if (m_pRefBuf)
	{
		delete[] m_pRefBuf;
		m_pRefBuf = NULL;
	}
	m_hWaveOut = NULL;
	m_pWaveHead = NULL;
	
	m_nWaveWriteCount = 0;
	m_nWavePlayCount = 0;
	return TRUE;
}

BOOL CWaveSound::PauseSound()
{
	MMRESULT hMMRet;
	if ((hMMRet = waveOutPause(m_hWaveOut)) != MMSYSERR_NOERROR)
	{
		return FALSE;
	}
	return TRUE;
}

BOOL CWaveSound::PlaySound()
{
	MMRESULT hMMRet;
	if ((hMMRet = waveOutRestart(m_hWaveOut)) != MMSYSERR_NOERROR)
	{
		return FALSE;
	}
	return TRUE;
}

BOOL CWaveSound::InputData(PBYTE pBuf,DWORD nUser)
{
	if (!m_bOpenSound)
	{
		return FALSE;
	}
	
	DWORD nSequenceCount = m_nWaveWriteCount % m_nBlockNum;

	if (m_pWaveHead[nSequenceCount].dwFlags & WHDR_INQUEUE)
	{
		return FALSE;
	}
	MMRESULT hMMRet;
	hMMRet = waveOutUnprepareHeader(m_hWaveOut, &m_pWaveHead[nSequenceCount], sizeof(WAVEHDR));

	
	m_pWaveHead[nSequenceCount].dwBufferLength = m_nBlockSize;
	m_pWaveHead[nSequenceCount].dwUser = nUser;
	m_pWaveHead[nSequenceCount].dwBytesRecorded = 0;
	m_pWaveHead[nSequenceCount].dwFlags = 0;
	m_pWaveHead[nSequenceCount].dwLoops = 0;

	short * pBufOut = (short*)pBuf;
	BOOL bFilter = FALSE;	

	if (m_pWaveHead && m_pWaveHead[nSequenceCount].lpData)
	{
		CopyMemory(m_pWaveHead[nSequenceCount].lpData, pBufOut, m_nBlockSize);
	}

	if (bFilter)
	{
		delete[] pBufOut;
		bFilter = FALSE;
	}
	hMMRet = waveOutPrepareHeader(m_hWaveOut, &m_pWaveHead[nSequenceCount], sizeof(WAVEHDR));
	hMMRet = waveOutWrite(m_hWaveOut, &m_pWaveHead[nSequenceCount], sizeof(WAVEHDR));
	m_nWaveWriteCount++;
	return TRUE;
}

int CWaveSound::GetPlaySeqNum()
{
	return m_nWaveWriteCount-m_nWavePlayCount;
}

DWORD CWaveSound::GetAudioTimeStamp()
{
	return m_nCurUser;
}

BOOL CWaveSound::SetVolume(DWORD nVolume)
{
	m_nVolume = nVolume;
	if (!m_bOpenSound)
	{
		return FALSE;
	}
	MMRESULT hMMRet = waveOutSetVolume(m_hWaveOut, m_nVolume);	
	if (hMMRet != MMSYSERR_NOERROR)
	{
		return FALSE;
	}
	return TRUE;	
}

BOOL CWaveSound::ResetSound()
{
	if (!m_bOpenSound)
	{
		return FALSE;
	}
	MMRESULT hMMRet = waveOutReset(m_hWaveOut);	
	if (hMMRet != MMSYSERR_NOERROR)
	{
		return FALSE;
	}
	m_nWavePlayCount = m_nWaveWriteCount;
	return TRUE;
}
char* GetWaveError(MMRESULT hMRel)
{
  switch (hMRel)
  {
    case MMSYSERR_NOERROR :
      return "MMSYSERR_NOERROR \n";
      
    case MMSYSERR_INVALHANDLE:
      return  "MMSYSERR_INVALHANDLE\n";
    
    case MMSYSERR_NODRIVER:
      return  "MMSYSERR_NODRIVER\n";
   
    case MMSYSERR_NOMEM:
      return  "MMSYSERR_NOMEM\n";
    
    case MMSYSERR_NOTSUPPORTED:
      return  "MSYSERR_NOTSUPPORTED\n";
    case WAVERR_UNPREPARED:
      return  "WAVERR_UNPREPARED\n";

    case MMSYSERR_ALLOCATED:
      return  "MMSYSERR_ALLOCATED\n";
    case MMSYSERR_BADDEVICEID:
      return  "MMSYSERR_BADDEVICEID\n";
    case WAVERR_SYNC:
      return  "WAVERR_SYNC\n";
    case WAVERR_BADFORMAT:
      return  "WAVERR_BADFORMAT\n";
    
    default:
      return  ("wave Default\n");
  }
 
}
void CWaveSound::InputRefData(PBYTE pBuf)
{
	if (!m_pRefBuf)
	{
		return ;
	}
	CSingleLock sLock(&m_mutex);
	sLock.Lock();
	CopyMemory(m_pRefBuf, pBuf, m_nBlockSize);
	sLock.Unlock();

}
