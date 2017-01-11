#pragma once
#include "stdafx.h"
#ifndef __MAILBOX_H
#define __MAILBOX_H
// �������� ����� ����������
#ifdef _MSC_VER
#define STRICT
#define _WIN32_WINNT 0x0500
#endif

#ifdef _MSC_VER
#include <stdlib.h>
#else
#include <dir.h>
#endif

class MAILBOX {
	class ERRORS {};	// ����� ��� ��������� ������

	HANDLE h;
	TCHAR fName[MAX_PATH];
	DWORD MaxSize, CommonSize, MessageCounts;
	int  charSize;
	static int count;
	static TCHAR mailBoxDir[MAX_PATH];


public:
	MAILBOX(LPCTSTR fName, DWORD MaxSize);
	MAILBOX& operator=(MAILBOX &m);

	MAILBOX& operator+=(LPCTSTR  Msg);// �������� ���������
	MAILBOX& operator-=(LPCTSTR  Msg); // ������� �������� ���������
	MAILBOX& operator-=(int  Number); // ������� ��������� �� ������

	DWORD operator()() {// ���������� ���������� ���������
		return MessageCounts;
	}
	// ���������� ���������� ���������
	DWORD ReadCounts() {
		return MessageCounts;
	}
	// ��������� ��������� � �������� �������
	bool RdMsg(TCHAR *res, int i, DWORD &len, bool deleteMessage = false);
	MAILBOX MAILBOX::DeleteMessage(int number);
	int MAILBOX::cleanMailBoxes();
	int MAILBOX::GetNumberOfMail();
	int MAILBOX::GetMd5MailBox();
	static int MAILBOX::GetNumberOfMailBoxes();
};
#endif