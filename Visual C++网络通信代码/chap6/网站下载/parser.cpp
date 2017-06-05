
/*
	Parser.cpp : CHtmlParser implemention.

	Implements the base HTML parser class.

	Author: Steven E. Sipe
*/

#include "stdafx.h"

#include <stdlib.h>

#include "parser.h"


// Constructor
CHtmlParser::CHtmlParser()
{
}

// Destructor
CHtmlParser::~CHtmlParser()
{
}

// Finds the specified token(s) in the supplied input buffer.  Sets nIndex to the 
// location of the token
BOOL CHtmlParser::FindToken(const char *pTokens, const char *pBuffer, int& nIndex, 
                                             int nMaxLen)
{
	int nTokenCount = strlen(pTokens);
	BOOL bQuote = FALSE;

	// While we haven't run out of buffer space...
	while(nIndex < nMaxLen)
	{
		// Does this match any of the tokens?
		for(int i = 0; i < nTokenCount; i++)
		{
			// Yes, return now
			if(pBuffer[nIndex] == pTokens[i])
				return(TRUE);
		}

		// Try the next character
		nIndex++;
	}

	return(FALSE);
}

// Returns a string delimited by the specifed terminating characters.  Sets nIndex
// to point to the end of the string.
CString CHtmlParser::GetString(const char *pTerminals, const char *pBuffer, 
                                       int &nIndex, int nMaxLen)
{
	int nOldIndex = nIndex;
	CString strText;

	// Skip leading spaces
	while(nIndex < nMaxLen && pBuffer[nIndex] == ' ')
		nIndex++;

	// Look for the terminators
	if(FindToken(pTerminals,pBuffer,nIndex,nMaxLen))
	{
		// Found one, save the location and the new string
		nIndex++;
		strText = pBuffer+nOldIndex;
		strText = strText.Left(nIndex-nOldIndex-1);

	}

	return(strText);
}

// Gets a parameter name
CString CHtmlParser::SaveParm(const CString& strParm, BOOL bLowercase)
{
	int nLen = strParm.GetLength();
	CString strNewParm = strParm;

	// Parameter are delimited by a space or a quote
	for(int i = 0; i < nLen; i++)
	{
		if(strNewParm[i] == '"')
			strNewParm.SetAt(i,' ');
	}

	// Remove leading or trailing spaces
	strNewParm.TrimLeft();
	strNewParm.TrimRight();

	// Make the parameter's name lower case --- NOT the value
	if(bLowercase)
		strNewParm.MakeLower();

	// Return the new name
	return(strNewParm);
}

// Gets options supplied with a parameter
void CHtmlParser::GetOptions(const char *pBuffer, int &nIndex, int nMaxLen)
{
	CString strParm;
	int nStart;

	// Iterate through the buffer until we're finished
	while(nIndex < nMaxLen)
	{
		nStart = nIndex;

		// Find the first assignment statement
		strParm = GetString("=",pBuffer,nIndex,nMaxLen);

		// Do we have a parameter name?
		if(!strParm.IsEmpty())
		{
			COptions opt;
			int nStart = nIndex;

			opt.SetParm(SaveParm(strParm,TRUE));

			// Skip leading spaces
			while(nIndex < nMaxLen && pBuffer[nIndex] == ' ')
				nIndex++;

			// Look for the end of the parameter or for the end
			// of the HTML tag
//			if(FindToken("\" >\r\n",pBuffer,nIndex,nMaxLen))
			if(FindToken("\" >",pBuffer,nIndex,nMaxLen))
			{
				// Handle quoted arguments
				if(pBuffer[nIndex] == '"')
					FindToken("\"",pBuffer,++nIndex,nMaxLen);

				// Save the option's location
				strParm = pBuffer+nStart;
				strParm = strParm.Left(nIndex-nStart);
				opt.SetValue(SaveParm(strParm,FALSE));
				opt.SetIndex(nStart);
			}

			// Add the option to the collection
			m_arrOptions.Add(opt);

		}
		else break;
	}
}

// Parses an HTML tag into parameters and values.
BOOL CHtmlParser::GetTag(const char *pBuffer, int& nIndex, int nMaxLen)
{
	m_strTag.Empty();
	m_arrOptions.RemoveAll();

	// Find the start of the tag
	if(FindToken("<",pBuffer,nIndex,nMaxLen))
	{
		int nStart = nIndex+1;
		BOOL bFound = FALSE;

		// Point to the next character
		nIndex++;

		// Not a valid tag, or a comment...continue looking
		if(pBuffer[nIndex] == ' ' || pBuffer[nIndex] == '>' || 
                     pBuffer[nIndex] == '!')
			return(FALSE);

		// Find the end of the tag
		if(FindToken(">",pBuffer,nIndex,nMaxLen))
		{
	         // Get the tag
			m_strTag = GetString(" >\r\n",pBuffer,nStart,nMaxLen);
			m_strTag.MakeLower();

			// Do we care about this tag???
			if(WantTag(m_strTag))
			{
				// Move back to the terminator so we can look for options
				nStart--;

				// Get the tags options
				GetOptions(pBuffer,nStart,nIndex+1);
			}

				return(TRUE);
		}
	}

	return(FALSE);
}

// Default implementation -- gets every tag.  OVERRIDE THIS
BOOL CHtmlParser::WantTag(const CString& strTag)
{
   return(TRUE);
}

// Default implementation -- does nothing with tags.  OVERRIDE THIS
BOOL CHtmlParser::ProcessTag(const CString& strTag, int nIndex, 
											const HTML_ARGS& arrOptions)
{
   return(TRUE);
}

// Parses the specified buffer of data for HTML tags
void CHtmlParser::ParseText(char *pBuffer, int nMaxLen)
{
	int nIndex = 0;

	// Initialize some work flags
	m_strTag.Empty();
	m_arrOptions.RemoveAll();
	m_pRawBuffer = pBuffer;

	// Iterate through the entire buffer
	while(nIndex < nMaxLen)
	{
		// Look for tags until no more are found
		if(GetTag(pBuffer,nIndex,nMaxLen))
		{
			// Should we continue processing?
			if(!ProcessTag(m_strTag,nIndex,m_arrOptions))
				break;
		}
	}
}
