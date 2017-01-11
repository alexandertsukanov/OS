
#include "stdafx.h"
#include "MailBox.h"
#pragma hdrstop
#ifndef _MSC_VER
#include <condefs.h>
USEUNIT("MailBox.cpp");
#pragma argsused
#endif
#ifdef UNICODE
#define _tcscout  std::wcout
#else
#define _tcscout  std::cout
#endif

int MAILBOX::count = 0;
TCHAR MAILBOX::mailBoxDir[] = __TEXT("C:\\MailBox");

int main()
{
	try {
		MAILBOX mb1(__TEXT("First_Mail_Box"), 75);
		mb1 += __TEXT("The message 1.");
		mb1 += __TEXT("The message 2.");
		mb1 += __TEXT("The message 3.");
		mb1 += __TEXT("The message 4.");
		mb1 += __TEXT("The message 5.");
		// Test size - this message will cause an overflow
		mb1 += __TEXT("The message 6.");

		// Get number of messages
		_tcscout << _T("Mail box has ")<< mb1.GetNumberOfMail();
		_tcscout << _T(" messages.") << std::endl;


		TCHAR *buff;
		DWORD len = 0;

		// Read and delete the second message 
		mb1.RdMsg(0, 2, len);
		buff = new TCHAR[len + 1];
		mb1.RdMsg(buff, 2, len, true);
		buff[len] = 0;
		_tcscout << _T("The second message is:") << std::endl;
		_tcscout << buff << std::endl;
		delete[]buff;

		// Read the second message 
		mb1.RdMsg(0, 2, len);
		buff = new TCHAR[len + 1];
		mb1.RdMsg(buff, 2, len);
		buff[len] = 0;
		_tcscout << _T("After deleted the second message is:") << std::endl;
		_tcscout << buff << std::endl;
		delete[]buff;

		// Delete the message by its number
		mb1 -= 2;
		// Read the second message 
		mb1.RdMsg(0, 2, len);
		buff = new TCHAR[len + 1];
		mb1.RdMsg(buff, 2, len);
		buff[len] = 0;
		_tcscout << _T("After deleted the second message is:") << std::endl;
		_tcscout << buff << std::endl;
		delete[]buff;

		// Delete the message on its content
		mb1 -= __TEXT("The message 4.");
		// Read the second message 
		mb1.RdMsg(0, 2, len);
		buff = new TCHAR[len + 1];
		mb1.RdMsg(buff, 2, len);
		buff[len] = 0;
		_tcscout << _T("After deleted the second message is:") << std::endl;
		_tcscout << buff << std::endl;
		delete[]buff;

		// Get MD5 hash
		mb1.GetMd5MailBox();

		// Delete all messages
		mb1.cleanMailBoxes();
		_tcscout << _T("After deleted all messages mail box has ") << mb1.GetNumberOfMail();
		_tcscout << _T(" messages.") << std::endl;

		// Create some mail boxes
		MAILBOX mb2(__TEXT("Second_Mail_Box2"), 75);
		MAILBOX mb3(__TEXT("Third_Mail_Box3"), 75);

		// Get number of mail boxes
		_tcscout << _T("We have ") << MAILBOX::GetNumberOfMailBoxes();
		_tcscout << _T(" mail boxes") << std::endl;

		return 0;
	}
	catch (...)
	{
		_tprintf_s(_T("%s"), _T("Error\n"));
	}
	
	return 0;
}

