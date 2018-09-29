#ifndef __ITOBJADMIN_H__
#define __ITOBJADMIN_H__
#pragma once

#include "AC/items/items.h"
#include "AC/items/pages.h"

namespace gtc{
////////////////////////////////////////////////////////////////////////////////////////////////////////
//	<< --- itobjAdmin	2013-12-13 --- >>
//	��ͼ��Ԫ������
////////////////////////////////////////////////////////////////////////////////////////////////////////
class itobjAdmin{
public:
	itobjAdmin();
	itobjAdmin(const itobjAdmin &admin);
	itobjAdmin& operator=(const itobjAdmin &admin);
	virtual ~itobjAdmin();
public:
	// ��������
	/*****************************************************************************************
		<< --- itobjAdmin::add	2013-12-13 --- >>
		˵�������һ����ͼ����
		������
		pobj	= ����ӵĻ�ͼ����
		����ֵ��
	*****************************************************************************************/
	void add(itobj* pobj) {
		if(pobj && std::find(m_vecItems.begin(), m_vecItems.end(), pobj) == m_vecItems.end())
			m_vecItems.push_back(pobj);
	}

	/*****************************************************************************************
		<< --- itobjAdmin::remove	2013-12-13 --- >>
		˵����ɾ��һ����ͼ���󣬲��ͷ��ڴ�
		������
		pobj	= ��ɾ���Ļ�ͼ����
		����ֵ��
	*****************************************************************************************/
	void remove(itobj* pobj) {
		itobj::PtrArrayType::iterator iter, end = m_vecItems.end();
		iter = std::find(m_vecItems.begin(), end, pobj);
		if(iter != end)
		{
			(*iter)->remove();
			m_vecItems.erase(iter);
		}
	}
	/*****************************************************************************************
		<< --- itobjAdmin::removeAll	2013-12-13 --- >>
		˵�����ͷ����л�ͼ��Ԫ
		������
		����ֵ��
	*****************************************************************************************/
	void removeAll() {
		for(itobj::PtrArrayType::size_type idx = 0; idx < m_vecItems.size(); ++idx)
		{
			m_vecItems[idx]->remove();
		}
		m_vecItems.clear();
	}
	/*****************************************************************************************
		<< --- itobjAdmin::copy	2013-12-13 --- >>
		˵��������ͼ��Ԫ׷�ӵ����м�����
		������
		vecItem	= ��׷�ӵĻ�ͼ��Ԫ
		����ֵ��
	*****************************************************************************************/
	void copy(const itobj::PtrArrayType &vecItem) {
		for(itobj::PtrArrayType::size_type idx = 0; idx < vecItem.size(); ++idx)
		{
			m_vecItems.push_back(vecItem[idx]->clone());
		}
	}

public:
#if defined(_WIN32)
	/*****************************************************************************************
		<< --- itobjAdmin::draw	2013-12-13 --- >>
		˵�������ƻ�ͼģ��
		������
		pdc		= ��ͼ����
		pView	= ��ͼ����������ͼ
		����ֵ��
	*****************************************************************************************/
	void draw(CDC *pdc, CView* pView);
	/*****************************************************************************************
		<< --- itobjAdmin::drawBackground	2013-12-02 --- >>
		˵�������ƹ�������ֽ��
		������
		pdc			= ��ͼ����
		szWorker	= ����̨�ߴ�
		����ֵ��
	*****************************************************************************************/
	void drawBackground(CDC *pdc, const CSize &szWorker);
	/*****************************************************************************************
		<< --- itobjAdmin::prepareDC	2013-12-13 --- >>
		˵������λ��ͼ����ϵ����ͼԭ��
		������
		pdc			= ��ͼ����
		szWorker	= ����̨�ߴ�
		����ֵ����λ����
	*****************************************************************************************/
	void prepareDC(CDC* pdc, const CSize &szWorker);
	/*****************************************************************************************
		<< --- itobjAdmin::objectAt	2013-12-02 --- >>
		˵�������ص�point���ĵ�һ����������������ӵ��Ⱥ�˳��
		������
		point	= �ο���
		����ֵ���оͷ��ض�����ָ�룬���򷵻ؿ�ָ��
	*****************************************************************************************/
	itobj* objectAt(const CPoint &point)
	{
		CRect rect(point, CSize(1, 1));
		for(itobj::PtrArrayType::size_type idx = 0; idx < m_vecItems.size(); ++idx)
		{
			if(m_vecItems[idx]->intersects(gtc::rect(rect.left, rect.top, rect.right, rect.bottom)))
			{
				return m_vecItems[idx];
			}
		}
		return nullptr;
	}
#endif
public:
	// ����
	itobjPage*				getPager() {				return m_pobjPage;					}
	void					resetPager(itobjPage *ppage) {
		if(m_pobjPage)
			m_pobjPage->remove();
		m_pobjPage = ppage;
	}

	itobj::PtrArrayType&	getItems() {				return m_vecItems;					}
	
#if defined(BOOST_SERIALIZE)
private:
	friend class boost::serialization::access;  
	template<class Archive>
	void serialize(Archive & ar, const unsigned int version)
	{
		ar & m_clrTracker;  
		ar & m_beWorkaxis;
		ar & m_pobjPage;
		ar & m_vecItems;
	}
#endif

protected:
	//itobj					*mpobj;
	uint32_t				m_clrTracker;	// ����ģ�����ɫ
	BEWorkAxis				m_beWorkaxis;	// ����WORK_AXIS_NORMAL��WORK_AXIS_DOC��WORK_AXIS_CAD�Ķ���õ�
	itobjPage				*m_pobjPage;	// ҳ����Ϣ
	itobj::PtrArrayType		m_vecItems;		// ���ƶ��󼯺�
};

}
#endif