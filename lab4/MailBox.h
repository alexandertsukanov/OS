#pragma once
#include "stdafx.h"
#ifndef __MAILBOX_H
#define __MAILBOX_H
// Проверка среды компиляции
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
	class ERRORS {};	// Класс для обработки ошибок

	HANDLE h;
	TCHAR fName[MAX_PATH];
	DWORD MaxSize, CommonSize, MessageCounts;
	int  charSize;
	static int count;
	static TCHAR mailBoxDir[MAX_PATH];


public:
	MAILBOX(LPCTSTR fName, DWORD MaxSize);
	MAILBOX& operator=(MAILBOX &m);

	MAILBOX& operator+=(LPCTSTR  Msg);// Добавить сообщение
	MAILBOX& operator-=(LPCTSTR  Msg); // Удалить заданное сообщение
	MAILBOX& operator-=(int  Number); // Удалить сообщение по номеру

	DWORD operator()() {// Определить количество сообщений
		return MessageCounts;
	}
	// Определить количество сообщений
	DWORD ReadCounts() {
		return MessageCounts;
	}
	// Прочитать сообщение с заданным номером
	bool RdMsg(TCHAR *res, int i, DWORD &len, bool deleteMessage = false);
	MAILBOX MAILBOX::DeleteMessage(int number);
	int MAILBOX::cleanMailBoxes();
	int MAILBOX::GetNumberOfMail();
	int MAILBOX::GetMd5MailBox();
	static int MAILBOX::GetNumberOfMailBoxes();
};
#endif