#include "stdafx.h"
#include "stdlib.h"

#include "HTMLParser.h"

CHtmlParser::CHtmlParser()
{
}

CHtmlParser::~CHtmlParser()
{
}

// 在buffer里找出预定义的标号，设置标号位置的索引
BOOL CHtmlParser::FindToken(const char *pTokens, const char *pBuffer, int& nIndex, int nMaxLen)
{
	int nTokenCount = strlen(pTokens);
	BOOL bQuote = FALSE;

	while(nIndex < nMaxLen)
	{
		for(int i = 0; i < nTokenCount; i++)
		{
			if(pBuffer[nIndex] == pTokens[i])
				return(TRUE);
		}
		nIndex++;
	}
	return(FALSE);
}

// 返回一个特定的字符结尾的字符串，设置索引的位置在字符串的尾部
CString CHtmlParser::GetString(const char *pTerminals, const char *pBuffer,int &nIndex, int nMaxLen)
{
	int nOldIndex = nIndex;
	CString strText;

	while(nIndex < nMaxLen && pBuffer[nIndex] == ' ')
		nIndex++;

	if(FindToken(pTerminals,pBuffer,nIndex,nMaxLen))
	{
		nIndex++;
		strText = pBuffer+nOldIndex;
		strText = strText.Left(nIndex-nOldIndex-1);
	}
	return(strText);
}

//得到参数名称
CString CHtmlParser::SaveParm(const CString& strParm, BOOL bLowercase)
{
	int nLen = strParm.GetLength();
	CString strNewParm = strParm;

	// 参数含有引号的地方用空格代替
	for(int i = 0; i < nLen; i++)
	{
		if(strNewParm[i] == '"')
			strNewParm.SetAt(i,' ');
	}

	// 删除前导和后驱的空格
	strNewParm.TrimLeft();
	strNewParm.TrimRight();

	if(bLowercase)
		strNewParm.MakeLower();

	return(strNewParm);
}

// 得到一个参数的配置信息
void CHtmlParser::GetOptions(const char *pBuffer, int &nIndex, int nMaxLen)
{
	CString strParm;
	int nStart;

	while(nIndex < nMaxLen)
	{
		nStart = nIndex;

		// 找出等号左边的字符串，以index为界定线
		strParm = GetString("=",pBuffer,nIndex,nMaxLen);

		if(!strParm.IsEmpty())
		{
			COptions opt;
			int nStart = nIndex;

			opt.SetParm(SaveParm(strParm,TRUE));

			while(nIndex < nMaxLen && pBuffer[nIndex] == ' ')
				nIndex++;

			//寻找html的tag的结束位置
			if(FindToken("\" >",pBuffer,nIndex,nMaxLen))
			{
				// 处理引号参数
				if(pBuffer[nIndex] == '"')
					FindToken("\"",pBuffer,++nIndex,nMaxLen);

				// 保存options的位置
				strParm = pBuffer+nStart;
				strParm = strParm.Left(nIndex-nStart);
				opt.SetValue(SaveParm(strParm,FALSE));
				opt.SetIndex(nStart);
			}

			// 往option集合中添加option
			m_arrOptions.Add(opt);

		}
		else 
			break;
	}
}

// 分析html的tag
BOOL CHtmlParser::GetTag(const char *pBuffer, int& nIndex, int nMaxLen)
{
	m_strTag.Empty();
	m_arrOptions.RemoveAll();

	// 找出tag的起始位置即"<"的位置
	if(FindToken("<",pBuffer,nIndex,nMaxLen))
	{
		int nStart = nIndex+1;
		BOOL bFound = FALSE;

		nIndex++;

		// 不是有效的tag，可能是注释也可能是html文件错误
		if(pBuffer[nIndex] == ' ' || pBuffer[nIndex] == '>' || pBuffer[nIndex] == '!')
			return FALSE;

		// 找出tag的终止位置，即">"的位置
		if(FindToken(">",pBuffer,nIndex,nMaxLen))
		{
			m_strTag = GetString(" >\r\n",pBuffer,nStart,nMaxLen);
			m_strTag.MakeLower();

			// 判断tag是否是预定义的tag
			if(WantTag(m_strTag))
			{
				nStart--;
				GetOptions(pBuffer,nStart,nIndex+1);
			}
			return TRUE;
		}
	}

	return FALSE;
}

// 缺省返回true
BOOL CHtmlParser::WantTag(const CString& strTag)
{
   return(TRUE);
}

// 缺省返回true
BOOL CHtmlParser::ProcessTag(const CString& strTag, int nIndex, 
											const HTML_ARGS& arrOptions)
{
   return(TRUE);
}

// 综合上面的函数分析html文件的tag
void CHtmlParser::ParseText(char *pBuffer, int nMaxLen)
{
	int nIndex = 0;

	m_strTag.Empty();
	m_arrOptions.RemoveAll();
	m_pRawBuffer = pBuffer;

	while(nIndex < nMaxLen)
	{
		if(GetTag(pBuffer,nIndex,nMaxLen))
		{
			if(!ProcessTag(m_strTag,nIndex,m_arrOptions))
				break;
		}
	}
}
