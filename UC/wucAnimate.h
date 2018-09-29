#pragma once
#include "wucdef.h"
#include "wucControl.h"
#include "AC/SyncObject.h"
namespace wuc {
// ����״̬
typedef enum BEPlayState {
	BEPlayStateNone			= 0,	// 
	BEPlayStatePlaying		,		// ������
	BEPlayStateFast			,		// ���
	BEPlayStateSlow			,		// ��֡����
	BEPlayStatePause		,		// ��ͣ
	BEPlayStateStop					// ֹͣ
}BEPlayState;
////////////////////////////////////////////////////////////////////////////////////////////////////////
//	IAnimate��		2014-08-04
//	�����ӿ�
class IAnimate
{
public:
	virtual ~IAnimate(){}
	virtual bool playStart() = 0;
	virtual bool playFast() = 0;
	virtual bool playSlow() = 0;
	virtual bool playPause() = 0;
	virtual bool playStop() = 0;
	virtual BEPlayState getPlayState() const = 0;
	virtual bool isPlaying() const = 0;
};
////////////////////////////////////////////////////////////////////////////////////////////////////////
//	animateFrameUC��		2014-08-04
//	�򵥵�ͼƬ֡�������ڲ�����ʱ��������֡
//	����ͣʱ������ʾ��ǰ֡
////////////////////////////////////////////////////////////////////////////////////////////////////////

class animateFrameUC : public controlUC, public IAnimate
{
public:
	animateFrameUC(void);
	virtual ~animateFrameUC(void);

	virtual PCWSTR getClass() const;
	virtual LPVOID getInterface(PCWSTR pstrName);

	virtual void Event(BSEventUC& event);

	virtual SIZE estimateSize(SIZE szAvailable);
	virtual void setAttribute(PCWSTR pstrName, PCWSTR pstrValue);
	virtual void paintBkImage(HDC hdc);
	virtual void paintStatusImage(HDC hdc);
public:
	virtual bool playStart();
	virtual bool playFast();
	virtual bool playSlow();
	virtual bool playPause();
	virtual bool playStop();
	virtual BEPlayState getPlayState() const;
	virtual bool isPlaying() const;
public:
	gtc::image* getFrameImage() const;
	void setFrameImage(PCWSTR pimgKeyName);
	int getCurrentFrame() const;
	void setCurrentFrame(int frameIndex);
	UINT getFrameElapse() const;
	void setFrameElapse(UINT elapse);

public:
	//////////////////////////////////////////////////////////////////////////////////////////
	//	2015-10-27 �����m_vecFrames�ı�����	
#if defined(GTC_BOOST_THREAD)
protected:
	mutable boost::shared_mutex m_rwMutex;
	mutable bool				m_bReadLocked;
	mutable bool				m_bWriteLocked;
public:
#define READ_LOCK	gtc::readLock	rdLock(m_rwMutex, m_bReadLocked, __FILEW__, __FUNCTIONW__);
#define WRITE_LOCK	gtc::writeLock	wtLock(m_rwMutex, m_bWriteLocked, __FILEW__, __FUNCTIONW__);
#else
#define READ_LOCK
#define WRITE_LOCK
#endif
protected:
	enum { 
		DEFAULT_FRAME_TIMERID = 20
	};

	//std::wstring	m_wsFrameImage;		// ֡ͼ�񣬼���֡ͼƬ����һ��
	int				m_iFrame;			// ��ǰ֡����
	BEPlayState		m_bePlayState;		// 
	UINT			m_uiFrameElapse;	// ֡���ż������λ������
	std::vector<RECT>	m_vecFrames;	// ����֡λ�ü���
};

}