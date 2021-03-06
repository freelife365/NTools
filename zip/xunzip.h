// XUnzip.h  Version 1.3
//
// Authors:      Mark Adler et al. (see below)
//
// Modified by:  Lucian Wischik
//               lu@wischik.com
//
// Version 1.0   - Turned C files into just a single CPP file
//               - Made them compile cleanly as C++ files
//               - Gave them simpler APIs
//               - Added the ability to zip/unzip directly in memory without 
//                 any intermediate files
// 
// Modified by:  Hans Dietrich
//               hdietrich@gmail.com
//
///////////////////////////////////////////////////////////////////////////////
//
// Lucian Wischik's comments:
// --------------------------
// THIS FILE is almost entirely based upon code by info-zip.
// It has been modified by Lucian Wischik.
// The original code may be found at http://www.info-zip.org
// The original copyright text follows.
//
///////////////////////////////////////////////////////////////////////////////
//
// Original authors' comments:
// ---------------------------
// This is version 2002-Feb-16 of the Info-ZIP copyright and license. The 
// definitive version of this document should be available at 
// ftp://ftp.info-zip.org/pub/infozip/license.html indefinitely.
// 
// Copyright (c) 1990-2002 Info-ZIP.  All rights reserved.
//
// For the purposes of this copyright and license, "Info-ZIP" is defined as
// the following set of individuals:
//
//   Mark Adler, John Bush, Karl Davis, Harald Denker, Jean-Michel Dubois,
//   Jean-loup Gailly, Hunter Goatley, Ian Gorman, Chris Herborth, Dirk Haase,
//   Greg Hartwig, Robert Heath, Jonathan Hudson, Paul Kienitz, 
//   David Kirschbaum, Johnny Lee, Onno van der Linden, Igor Mandrichenko, 
//   Steve P. Miller, Sergio Monesi, Keith Owens, George Petrov, Greg Roelofs, 
//   Kai Uwe Rommel, Steve Salisbury, Dave Smith, Christian Spieler, 
//   Antoine Verheijen, Paul von Behren, Rich Wales, Mike White
//
// This software is provided "as is", without warranty of any kind, express
// or implied.  In no event shall Info-ZIP or its contributors be held liable
// for any direct, indirect, incidental, special or consequential damages
// arising out of the use of or inability to use this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it
// freely, subject to the following restrictions:
//
//    1. Redistributions of source code must retain the above copyright notice,
//       definition, disclaimer, and this list of conditions.
//
//    2. Redistributions in binary form (compiled executables) must reproduce 
//       the above copyright notice, definition, disclaimer, and this list of 
//       conditions in documentation and/or other materials provided with the 
//       distribution. The sole exception to this condition is redistribution 
//       of a standard UnZipSFX binary as part of a self-extracting archive; 
//       that is permitted without inclusion of this license, as long as the 
//       normal UnZipSFX banner has not been removed from the binary or disabled.
//
//    3. Altered versions--including, but not limited to, ports to new 
//       operating systems, existing ports with new graphical interfaces, and 
//       dynamic, shared, or static library versions--must be plainly marked 
//       as such and must not be misrepresented as being the original source.  
//       Such altered versions also must not be misrepresented as being 
//       Info-ZIP releases--including, but not limited to, labeling of the 
//       altered versions with the names "Info-ZIP" (or any variation thereof, 
//       including, but not limited to, different capitalizations), 
//       "Pocket UnZip", "WiZ" or "MacZip" without the explicit permission of 
//       Info-ZIP.  Such altered versions are further prohibited from 
//       misrepresentative use of the Zip-Bugs or Info-ZIP e-mail addresses or 
//       of the Info-ZIP URL(s).
//
//    4. Info-ZIP retains the right to use the names "Info-ZIP", "Zip", "UnZip",
//       "UnZipSFX", "WiZ", "Pocket UnZip", "Pocket Zip", and "MacZip" for its 
//       own source and binary releases.
//
///////////////////////////////////////////////////////////////////////////////

#ifndef XUNZIP_H
#define XUNZIP_H


#ifndef XZIP_H
DECLARE_HANDLE(HZIP);	// An HZIP identifies a zip file that has been opened
#endif

//////////////////////////////////////////////////////////////////////////////////////////
// 2016-3-8		kp007	
#include "AC/sysupport.h"

namespace gtc {


//////////////////////////////////////////////////////////////////////////////////////////
// 2016-3-9		kp007	xunzip库异常
static std::wstring _errorInfoXUnzip(int errCode);

#define XUNZIP_EXCEPT(code) GTC_EXCEPT_EX(gtc::baseException::BSExceptionCodeXUnzipFailed, code, _errorInfoXUnzip(code))

////////////////////////////////////////////////////////////////////////////////////////////////////////
//	unzip	类 2016-3-8	kp007
//	解压zip文件
//	
////////////////////////////////////////////////////////////////////////////////////////////////////////
class unzip 
{
public:
	//// zip来源标记
	//enum _BESourceFlag {
	//	handle		= 1,	// 已打开的文件句柄
	//	filename	,		// zip来源于文件
	//	memory				// 内存
	//};
public:
	unzip();
	virtual ~unzip();

	bool open(HANDLE hFile);
	bool open(PCWSTR pfileName);
	bool open(SBDataType &vecData);
	void close();
	/****************************************************************************************
		<< --- uncompress	2016-3-9	kp007 --- >>         
		说明：查找指定名称的记录是否存在，可能的话并解压
		参数：
			pName			= 要查找的文件名
			refData			= 存放解压的数据
			caseInsensitive	= true：忽略大小写
		返回值：文件是否存在
	*****************************************************************************************/
	bool uncompress(PCWSTR pName, SBDataType &refData, bool caseInsensitive = false);
	//////////////////////////////////////////////////////////////////////////////////////////
	// 2016-3-9		kp007	解压所有文件到指定目录
	bool uncompress(PCWSTR pSavePath);
	
private:
	HZIP	_hzip;
};

}	// gtc
#endif //XUNZIP_H
