<%@ Language=VBScript %>
<!-- #include file="../../ArticleAccess.asp" -->
<!-- #include file="../../GetAccessCount.asp" -->
<%
 PrintArticleAccessCount 45
 AccessArticle  45
 %>
 <html>

<head>
<meta http-equiv="Content-Type" content="text/html; charset=windows-1252">
<meta name="GENERATOR" content="Microsoft FrontPage 4.0">
<meta name="ProgId" content="FrontPage.Editor.Document">
<title> Serial Communication in Windows </title>
</head>

<body>

<p align="center">&nbsp;</p>
<p align="center"><font face="Verdana" size="2"><b>[<u> </u></b></font><font size="2" face="Verdana"><b><u>Serial Communication in Windows</u></b></font><font face="Verdana" size="2"><b><u>
</u>]<br>
</b><a href="mailto:ashishdhar@hotmail.com"><i>ashish dhar</i></a></font></p>
<table borderColor="#ffffff" height="4318" cellSpacing="0" borderColorDark="#ffffff" width="99%" borderColorLight="#ffffff" border="1">
  <tbody>
    <tr>
      <td vAlign="top" width="11%" bgColor="#538ab3" height="4314">
        <div align="justify">
          <table borderColor="#538ab3" height="4654" cellSpacing="4" borderColorDark="#000000" width="100%" borderColorLight="#538ab3" border="4">
            <tbody>
              <tr>
                <td vAlign="top" borderColorLight="#538ab3" width="100%" bgColor="#538ab3" borderColorDark="#538ab3" height="4650"><br>
                  <br>
                  <br>
                  <br>
                  <br>
                  <br>
                  <br>
                  <br>
                  <br>
                  <br>
                  <br>
                  <br>
                  <br>
                  <br>
                  <br>
                  <br>
                </td>
              </tr>
            </tbody>
          </table>
        </div>
      </td>
      <td vAlign="top" borderColorLight="#e8e9e2" width="69%" bgColor="#ffffff" borderColorDark="#f0f0f0" height="4314">&nbsp;

<p><font size="2" face="Verdana"><a href="../Source.zip">Source code</a><b><br>
<br>
</b>This article is meant to give you a jump start on doing serial communication
in Windows (NT family). The article will provide a class called
CSerialCommHelper which you can use directly to do serial communication in your
application. The class that is provided here with this article does uses
overlapped IO. You donot need to know much about serial communication or
overlapped IO for this article. However, you need to know some about the
synchronization objects like Events and some Windows APIs like
WaitForSingleObject and WaitForMultipleObject etc. Also some basic understanding
of windows threads is required - like thread creation and <i>termination</i>.</font></p>
<p><font size="2" face="Verdana"><b>Introduction<br>
</b>In order for your computer to be able to do serial communication, computer
has to have a serial port. Most of the computers have at least one serial port
also known as COM port ( communication port ) and are generally called COM1 COM2
etc. Then there are the device drivers for the serial ports. If you think it
over, all you that you need to do in serial communication is either send data or
receive data. In other words, you are doing input/output (IO) to the serial
port. The same IO is done with disk based files. Hence there is no surprise that
the APIs for reading and writing to a file apply to serial ports as well. When
you send data to the serial port its in terms of bytes but when it leaves the
serial port it is in the form of bits. Similarly, when the data arrives at the
serial port, its in bit format and when you get data you get it in bytes.&nbsp;<br>
Without any further discussion lets get started.<br>
<br>
<b>Opening the COM port<br>
</b>The first and the foremost step in doing a serial communication is to open
the desired port. Lets say you have your device hooked to COM1 you can open the
COM port using following API:</font></p>
<table border="0" cellspacing="1" width="100%" bgcolor="#EEE8EE">
  <tr>
    <td width="100%">
      <pre><font size="2">HANDLE m_hCommPort = ::<font color="#0000FF">CreateFile</font>(	szPortName,
					GENERIC_READ|GENERIC_WRITE,//access ( read and write)
					0,	//(share) 0:cannot share the COM port						
					0,	//security  (None)				
					OPEN_EXISTING,// creation : open_existing
					FILE_FLAG_OVERLAPPED,// we want overlapped operation
					0// no templates file for COM port...
					);</font></pre>
    </td>
  </tr>
</table>
<p><font size="2" face="Verdana">The third fifth and seventh parameters have to be what they
are in the above example by law. We want to open the file ( the COM port ) in an
overlapped fashion - that's why the sixth param is FILE_FLAG_OVERLAPPED. We will
get into the details of overlapped IO a bit later. As you must have guessed from
the name , CreateFile() API can be used to create a file (disk based) and also
it can be used to open an existing file.&nbsp;<br>
To Windows a serial port or a disk based file both are IO devices . So, in order
to open an existing file ( serial port ) all we need to know the name of the
device ( COM1) and pass the creation flags as OPEN_EXISTING.<br>
If a COM port is opened successfully, the API returns handle to the com port
just like a handle to a file. However, if the system could not open the COM
port, it would return INVALID_HANDLE_VALUE . And you can get the reason by
calling GetLastError(). One of the common errors while opening a COM port is
that the COM port is already opened by some other application and in that case
you would get ERROR_ACCESS_DENIED (5). Similarly if you by mistake opened a COM
port that doesnot exist , you would get ERROR_FILE_NOT_FOUND&nbsp; as the last
error.<br>
<u>Note: Remember not to do make any function calls (like ASSERT) before calling
GetLastError() or you would get 0</u>.<u><br>
</u>Once you have opened the com port all you need to do now is to start using
it.<br>
<br>
<b>Reading and Writing<br>
</b>Now, once you have a com port open, you may want to send&nbsp; some data to
the connected device. For example, lets say you want to send &quot;Hello&quot;
to the device(e.g., another PC). When you want to send the data across the
serial port, you need to write to the serial port just like you would write to a
file. You would use following API:<br>
</font></p>
<table border="0" cellspacing="1" width="100%">
  <tr>
    <td width="100%" bgcolor="#C0C0C0"><font size="2" face="Verdana">iRet = WriteFile (m_hCommPort,data,dwSize,&amp;dwBytesWritten  ,&amp;ov);&nbsp;&nbsp;&nbsp;&nbsp;</font></td>
  </tr>
</table>
<p><font size="2" face="Verdana">where data contains &quot;Hello&quot; .&nbsp;<br>
Lets say in response to your &quot;Hello&quot; , the device sends you
&quot;Hi&quot; . So you need to read the data. Again ,you would&nbsp; use
following API:</font></p>
<table border="0" cellspacing="1" width="100%">
  <tr>
    <td width="100%" bgcolor="#C0C0C0"><font size="2" face="Verdana">abRet = ::ReadFile(m_hCommPort,szTmp
      ,sizeof(szTmp ),&amp;dwBytesRead,&amp;ovRead) ;</font></td>
  </tr>
</table>
<p><font size="2" face="Verdana"><i>For now do not try to understand everything.We will get to
all this later.<br>
</i>All this sounds very simple. Right? <br>
Now lets start digging into issues. </font></p>
<p><font size="2" face="Verdana"><b>Issues with serial communication</b><br>
Just now I said, in response to your &quot;Hello&quot;, the device may send you
&quot;Hi&quot; back and you would like to read that. But the problem here is
that you don't know when the device is going to respond? Or will it ever
respond? When should you start to read from the port. One option is that as soon
as you made call to WriteFile, you make call to ReadFile . If no data is there
you need to make read again later on. This leads to what is called polling. You
keep polling the port for data. This model does not really&nbsp; seem to be a
good one. It would be nice if somehow you were notified by the system when data
has arrived and only then would you make call to ReadFile. This is event driven
approach and fits well into Windows programming. And good news is that such a
model is possible . <br>
<br>
<br>
<br>
Another issue with the serial communication is that since it always occurs
between two devices, the two devices need to agree on how they talk to each
other. Each side needs to follow certain protocols to conduct business. Since
its the serial port that actually carries out the communication, we need to
configure the serial port. There is an API available for exact same purpose.
Following is the API:</font></p>
<table border="0" cellspacing="1" width="100%" bgcolor="#C0C0C0">
  <tr>
    <td width="100%"><font size="2" face="Verdana">SetCommState ( HANDLE hFile, LPDCB lpDCB)</font></td>
  </tr>
</table>
<p><font size="2" face="Verdana">The first parameter is the handle to COM port and the second
paramter is what is called device control block (DCB) . The DCB is a struct
defined in winbase.h and has 28 data members. For example, we need to specify
baud rate at which the COM port operates, you need to set the <b>BaudRate </b>member
of the struct . Baud rate is usual 9600 (bps) . But the two devices have to use
the same baud rate to conduct business. Similarly if you want to use parity you
need to set <b>Parity</b> member of the struct. Again the two devices have to
use same parity. Some of the data members are reserved and have to be 0. I have
found it easier to get the current DCB struct and then set those members which
we are interested in changing. Following code gets the current dcb and sets some
of the fields:<br>
<br>
</font></p>
<table border="0" cellspacing="1" width="100%" bgcolor="#EEE8EE">
  <tr>
    <td width="100%">
      <pre><font size="2" face="Verdana">DCB dcb = {0};
dcb.DCBlength = sizeof(DCB);

if (!::<font color="#0000FF">GetCommState </font>(m_hCommPort,&amp;dcb))
{
	TRACE ( &quot;CSerialCommHelper : Failed to Get Comm State Reason: %d&quot;,GetLastError());
	return E_FAIL;
}

dcb.BaudRate	= dwBaudRate;
dcb.ByteSize	= byByteSize;
dcb.Parity		= byParity;
if ( byStopBits == 1 )
	dcb.StopBits	= ONESTOPBIT;
else if (byStopBits == 2 ) 
	dcb.StopBits	= TWOSTOPBITS;
else 
	dcb.StopBits	= ONE5STOPBITS;


if (!::<font color="#0000FF">SetCommState</font> (m_hCommPort,&amp;dcb))
{
	ASSERT(0);
	TRACE ( &quot;CSerialCommHelper : Failed to Set Comm State Reason: %d&quot;,GetLastError());
	return E_FAIL;
}
TRACE ( &quot;CSerialCommHelper : Current Settings, (Baud Rate %d; Parity %d; Byte Size %d; Stop Bits %d&quot;, dcb.BaudRate, 
</font></pre>
    </td>
  </tr>
</table>
<p><font size="2" face="Verdana">Most of the time you won't need to change the
other fields of this structure. But if you need to change the structure you need
to be very careful about the fields as changing the fields will affect the
behavior of the serial communication and hence you should be very sure what you
want to change.&nbsp;<br>
<b><br>
Event Driven Approach<br>
</b>Coming back to our earlier problem with the reading of data. If we do not
want to&nbsp; keep polling the COM port for any data then we need to have some
kind of event mechanism available. Fortunately there is a way that you can ask
the system to notify you when certain events happen. The API to use is
</font></p>
<table border="0" cellspacing="1" width="100%" bgcolor="#C0C0C0">
  <tr>
    <td width="100%"><font size="2" face="Verdana">SetCommMask( HANDLE
      hHandle,DWORD dwEvtMask)</font></td>
  </tr>
</table>
<p><font size="2" face="Verdana">The first parameter is the handle to the open
COM port. The second parameter is used to specify a list of events which we are
interested in. <br>
The events that need to be specified in the mask depends upon the application
needs. For simplicity, lets say we are interested in getting&nbsp; notified
whenever a character arrives at the serial port, we would need to specify
EV_RXCHAR as the event mask. Similarly if we are interested to know when all the
data has been sent, we need to specify EV_TXEMPTY flag also. So out call would
look like this:
</font></p>
<table border="0" cellspacing="1" width="100%" bgcolor="#DADADA">
  <tr>
    <td width="100%"><font size="2" face="Verdana">SetCommMask(
      m_hCommPort,EV_TXTEMPTY|EV_RXCHAR);</font></td>
  </tr>
</table>
<p><font size="2" face="Verdana">&nbsp; The interesting thing here is that
although we told system about the events of our interest, we did not however
told system what to do when these events occur. Like how would system let us
know that a particular event occurred. An obvious thing seems to be a callback
mechanism. But there is not such mechanism available. Here is when things get a
little tricky. In order for system to let us know about the communication event
occurrence, we need to call <b>WaitCommEvent</b> This function waits for the
events specified in SetCommMask. But if your think a little more, it sounds like
we are turning a notification mechanism back to polling mechanism. Actually its
even worse that than . WaitCommEvent blocks till an event occurs. So whats the
use of WaitCommEvent ? Well , the answer lies in overlapped IO.<br>
If you look at the WaitCommEvent signature it looks like this:
</font></p>
<table border="0" cellspacing="1" width="100%" bgcolor="#DADADA">
  <tr>
    <td width="100%"><font face="Verdana" size="2">BOOL</font><font face="Verdana" size="2" color="#0000FF">
      WaitCommEvent</font><font face="Verdana" size="2">(HANDLE hCommPort,
      LPDWORD dwEvtMask,LPOVERLAPPED lpOverlapped);</font></td>
  </tr>
</table>
<p><font size="2" face="Verdana">&nbsp;&nbsp;&nbsp; The third parameter is the
key here.&nbsp;<br>
Think of overlapped IO as asynchronous IO. Whenever a function makes a call and
specifies the overlapped IO structure, it means that try to do the current
operation but if you are not able to complete it immediately let me know when
you are done with this IO. The way system lets you know about the completion is
by setting an kernel event object that is part of the lpOverlapped structure.
So, all you do is spawn a thread and make the thread wait for that event object
using one of the WaitForSingleObject() APIs.<br>
Lets look at the overlapped structure:<br>
</font></p>
<table border="0" cellspacing="1" width="100%" bgcolor="#DADADA">
  <tr>
    <td width="100%">
      <pre><font size="2" face="Verdana"><font color="#0000FF">typedef struct </font>_OVERLAPPED {
DWORD Internal;
DWORD InternalHigh;
DWORD Offset;
DWORD OffsetHigh;
<b>HANDLE hEvent</b>;
} OVERLAPPED, *LPOVERLAPPED;</font></pre>
    </td>
  </tr>
</table>
<p><font size="2" face="Verdana">The last parameter is the event handle that you
need to create . This event is generally a manual reset event. When you make a
call like WaitCommEvent () passing overlapped structure as the last parameter,
and the system could not complete call meaning it did not see any characters at
the port, it would return immediately but would return FALSE. If you now make a
call to GetLastError() you would get ERROR_IO_PENDING which means that the call
has been accepted but no characters have yet arrived at the COM port. Also it
means whenever the characters will arrive, the system will set the hEvent of the
overlapped structure that you passed in. So if your thread would wait for single
object on hEvent and you pass INFINITE, then whenever your Wait fn. returns
WAIT_OBJECT_0 it means some character has arrived&nbsp; or all the data in the
output buffer has been sent.<br>
In our current case since we are interested in more than one events we would
need to check what event did we get by making call to GetCommMask and checking
the dword against each event.Following&nbsp; pseudo code will explain it:</font></p>
<p><font size="2" face="Verdana">&nbsp;You can read the data from the com port
and reset the event and make the call to WaitCommEvent again and so on.
</font></p>
<table border="0" cellspacing="1" width="100%" bgcolor="#EEE8EE">
  <tr>
    <td width="100%">
      <pre><font face="Verdana" size="2" color="#417194">unsigned __stdcall CSerialCommHelper::ThreadFn(void*pvParam)
{
	OVERLAPPED ov;
	memset(&amp;ov,0,sizeof(ov));
	ov.hEvent = CreateEvent( 0,true,0,0);
	HANDLE arHandles[2];
	arHandles[0] = apThis-&gt;m_hThreadTerm;

	DWORD dwWait;
	SetEvent(apThis-&gt;m_hThreadStarted);
	while (  abContinue )
	{
		
</font><font face="Verdana" size="2">		BOOL abRet = ::WaitCommEvent(apThis-&gt;m_hCommPort,&amp;dwEventMask, &amp;ov) ;
</font><font face="Verdana" size="2" color="#417194">		if ( !abRet )
		{
			
			ASSERT( GetLastError () == ERROR_IO_PENDING);
		}

		
		arHandles[1] = ov.hEvent ;
		
		dwWait = WaitForMultipleObjects (2,arHandles,FALSE,INFINITE);
		switch ( dwWait )
		{
		case WAIT_OBJECT_0:
			{
				_endthreadex(1);
			}
			break;
		case WAIT_OBJECT_0 + 1:
			{
				</font><font face="Verdana" size="2">DWORD dwMask;
				if (GetCommMask(apThis-&gt;m_hCommPort,&amp;dwMask) )
				{
				   if ( dwMask &amp; EV_TXEMPTY )
				   TRACE(&quot;Data sent&quot;);
				   ResetEvent ( ov.hEvent );
				   continue;
				}<font color="#417194">
				</font>else  
				{
				   //read data here and reset ov.hEvent
				}
</font><font color="#417194"><font face="Verdana" size="2">			}
		}//switch
	}//while
return 0;
}</font></font></pre>
    </td>
  </tr>
</table>
<p><font size="2" face="Verdana">&nbsp;If you understood the above code , you
will understand the whole of this article and the source code provided.<br>
&nbsp;The above piece of code is simple using the overlapped IO method to do its
job.<br>
Once we have received the indication that the data has arrived we need to read
the data. Important thing to note here is that the when data arrives at the
serial port, it is copied over to system buffer.&nbsp; The data is removed from
the system buffer only when you have read the data using API such as ReadFile.&nbsp;
Like any buffer, system buffer has a limited size. So if you do not read the
data from the buffers quick enough the system buffers can be become full if more
data is arriving. What happens to further data depends upon the configuration
that you have set in the device configuration block (in call to SetCommState ).
Usually the applications do some kind of handshaking at the application level
but you can also make configurations such that the com port does not accept any
further data upon buffer-full events. But all that is beyond the scope of this
discussion. If possible its always better to have applications themselves
implementing some kind of handshaking&nbsp; - like do not send next block of
data until you get okay for the first block. Generally this kind of handshaking
is implemented using some sort of ACK / NAK&nbsp; and ENQ protocol.<br>
<br>
In order for us to read data we need to use ReadFile() API. ReadFile API has to
specify how much data to read. Lets say we are monitoring character arrivals and
10 characters arrive at the port. As soon as first character arrives at the port
the system will set the overlapped structure's event object and out
WaitSingleObject will return. Next we would need to read the data. So how much
data should we read? Should we read 1 byte or&nbsp; 10 bytes? That is a good
question.&nbsp;&nbsp; The way it works is as follows (Note:this
is not documented anywhere but this is what I have found by research on
Win2K,NT4.0)&nbsp; :<br>
When one (or more) characters arrive at the port, the event object associated with the
overlapped structure set once. Now lets say that you made a call to read and you
read 1 character. After reading 1 character , you would finally Reset the
overlapped structure's event object. Now you would go back to the WaitCommEvent
but it would return false since no &quot;new&quot; character has arrived. So you
will not be able to read any more characters.&nbsp; Now when another character
arrives, system will set the overlapped event and you would read one more
character but this time it will be the character that had arrived earlier and
you never read. This clearly is a&nbsp; problem.<br>
So what is the solution? The easiest solution is that as soon as you got the
event object indicating the arrival of a character, you should read all the
characters that are present in the port. (<i>If you are familiar with win API
MsgWaitForMultipleObjects you can draw a analogy here.</i>) </font></p>
<p><font size="2" face="Verdana">So again the question remains how many
characters to read. The answer is read all the characters in a loop using
ReadFile().
<br>
Here is the pseudo code</font></p>
<table border="0" cellspacing="1" width="100%" bgcolor="#DADADA">
  <tr>
    <td width="100%">
      <pre>threadFn...

	WaitCommEvent(m_hCommPort,&amp;dwEventMask, &amp;ov) ;
	if ( WaitForSingleObject(ov.hEvent,INFINITE) == WAIT_OBJECT_0)
	{	
		char szBuf[100];
		memset(szBuf,0,sizeof(szBuf));
		do
		{
			ReadFile( hPort,szBuf,sizeof(szBuf),&amp;dwBytesRead,&amp;ov);
		}while (dwBytesRead &gt; 0 );	
	}	</pre>
    </td>
  </tr>
</table>
<p><font size="2" face="Verdana">ReadFile API has following signature:<br>
</font></p>
<table border="0" cellspacing="1" width="100%" bgcolor="#DADADA">
  <tr>
    <td width="100%">
      <pre><font face="Verdana" size="2">BOOL ReadFile( HANDLE<i> <a class="synParam" onclick="showTip(this)" href>hFile</a></i>, // handle to file 
</font>		<font face="Verdana" size="2">LPVOID<i> <a class="synParam" onclick="showTip(this)" href>lpBuffer</a></i>, // data buffer 
		DWORD<i> <a class="synParam" onclick="showTip(this)" href>nNumberOfBytesToRead</a></i>, // number of bytes to read 
		LPDWORD<i> <a class="synParam" onclick="showTip(this)" href>lpNumberOfBytesRead</a></i>, // number of bytes read 
		LPOVERLAPPED<i> <a class="synParam" onclick="showTip(this)" href>lpOverlapped</a></i> // overlapped buffer );</font></pre>
    </td>
  </tr>
</table>
<p><font size="2" face="Verdana"><br>
<br>
The first parameter is as usual the com port, the last parameter is the
overlapped structure. Again we need to create a manual reset event and pass the
overlapped structure to the ReadFile function. Again if you issue a read for say
10 bytes and there is no data available , ReadFile will return FALSE and
GetLastError() will return ERROR_IO_PENDING and the system will set the
overlapped event when the overlapped operation(read ) completes.&nbsp;<br>
As you can see ReadFile returns dwBytesRead which as is clear returns the number
of bytes read. If there are no bytes remaining, the dwBytesRead will return 0.
Lets say there are 11 bytes that have arrived and you read 10&nbsp; characters
in the first go in while loop. In the first go 10 characters will be returned in
dwBytesRead. In the second go with while loop, the dwBytesRead will return 1.
Now in the third go the dwBytesRead will return 0 and you will break out of the
while loop. This allows you to read all the data. In this approach ,if you
noticed,we never really took advantage of the overlapped structure that we
passed to the ReadFile function but we still need to pass it because we opened
the COM port in Overlapped manner.<br>
<br>
<br>
And finally when you want to send data to other device, you need to call
WriteFile. WriteFile is not even worth discussing.<br>
<br>
<br>
There is one more thing that needs to be taken into account before we move on
and that is communication <b>timeouts</b>. Its important to set the timeout to
proper values for things to work. The API to do so is:&nbsp;
</font></p>
<p><font size="2" face="Verdana">SetCommTimeouts ( HANDLE hCommPort,
LPCOMMTIMEOUTS lpCommTimeOuts)<br>
<br>
<br>
&nbsp;COMTIMEOUTS is a structure with following members:<br>
</font></p>
<table border="0" cellspacing="1" width="70%" bgcolor="#DADADA">
  <tr>
    <td width="100%">
      <pre class="syntax"><font face="Verdana" size="2">typedef struct _COMMTIMEOUTS {  
  DWORD <a class="synParam" onclick="showTip(this)" href>ReadIntervalTimeout</a>; 
  DWORD <a class="synParam" onclick="showTip(this)" href>ReadTotalTimeoutMultiplier</a>; 
  DWORD <a class="synParam" onclick="showTip(this)" href>ReadTotalTimeoutConstant</a>; 
  DWORD <a class="synParam" onclick="showTip(this)" href>WriteTotalTimeoutMultiplier</a>; 
  DWORD <a class="synParam" onclick="showTip(this)" href>WriteTotalTimeoutConstant</a>; 
} COMMTIMEOUTS,*LPCOMMTIMEOUTS; </font></pre>
    </td>
  </tr>
</table>
<p><font size="2" face="Verdana">&nbsp;&nbsp;
<br>
For a description of all these fields consult MSDN documentation. But one thing
I want to point out is this:&nbsp;<br>
&quot;...A value of MAXDWORD, combined with zero values for both the <b>ReadTotalTimeoutConstant</b>
and <b>ReadTotalTimeoutMultiplier</b> members, specifies that the read operation
is to return immediately with the characters that have already been received,
even if no characters have been received...&quot;<br>
This is exactly what we want . We do NOT want the ReadFile to get stuck if there
is no data available as we will know with WaitCommEvent() API.<br>
and also &quot;...A value of zero for both the <b>WriteTotalTimeoutMultiplier</b>
and <b>WriteTotalTimeoutConstant</b> members indicates that total time-outs are
not used for write operations...&quot; is what we want. In short we need to do
this:</font></p>
<table border="0" cellspacing="1" width="70%" bgcolor="#DADADA">
  <tr>
    <td width="100%">
      <pre>		COMMTIMEOUTS timeouts;
		timeouts.ReadIntervalTimeout		= MAXDWORD; 
		timeouts.ReadTotalTimeoutMultiplier	= 0;
		timeouts.ReadTotalTimeoutConstant	= 0;
		timeouts.WriteTotalTimeoutMultiplier	= 0;
		timeouts.WriteTotalTimeoutConstant	= 0;
		
		if (!SetCommTimeouts(m_hCommPort, &amp;timeouts))
		{
			ASSERT(0);
			TRACE ( &quot;CSerialCommHelper :  Error setting time-outs. %d&quot;,GetLastError());
			return E_FAIL;
		}
</pre>
    </td>
  </tr>
</table>
<p><font size="2" face="Verdana"><br>
Now we have discussed almost everything that needs to be discussed for the sake
of this article.<br>
<b><br>
Putting it all together&nbsp;<br>
</b>All this I have put together in a form of two classes:&nbsp;<br>
The main class is <b>CSerialCommHelper</b> - the main class that does performs
all the communication .<br>
The helper class called <b>CSerialBuffer</b> that is an internal buffer used by
the CSerialCommHelper.<br>
<br>
Here is the main API of the <b>CSerialCommHelper:<br>
</b></font></p>
<table border="0" cellspacing="1" width="70%" bgcolor="#DADADA">
  <tr>
    <td width="100%">
      <pre><font face="Verdana" size="2">inline bool IsInputAvailable()
inline bool IsConnection() {return m_abIsConnected ;}
inline void SetDataReadEvent() { SetEvent ( m_hDataRx ); }
HRESULT Read_N (std::string&amp; data,long alCount,long alTimeOut);
HRESULT Read_Upto (std::string&amp; data,char chTerminator ,long* alCount,long alTimeOut);
HRESULT ReadAvailable(std::string&amp; data);
HRESULT Write (const char* data,DWORD dwSize);
HRESULT Init(std::string szPortName, DWORD dwBaudRate,BYTE byParity,BYTE byStopBits,BYTE byByteSize);
HRESULT Start();
HRESULT Stop();
HRESULT UnInit();</font></pre>
    </td>
  </tr>
</table>
<p><font size="2" face="Verdana">and the interface for CSerialBuffer is :<br>
</font></p>
<table border="0" cellspacing="1" width="70%" bgcolor="#DADADA">
  <tr>
    <td width="100%">
      <pre><font size="2" face="Verdana">inline void LockBuffer(); 
inline void UnLockBuffer();
void AddData( char ch ) ;
void AddData( std::string&amp; szData ) ;
void AddData( std::string&amp; szData,int iLen ) ;
void AddData( char *strData,int iLen ) ;
std::string GetData() ;
void Flush();
long Read_N( std::string &amp;szData,long alCount,HANDLE&amp; hEventToReset);
bool Read_Upto( std::string &amp;szData,char chTerm,long &amp;alBytesRead ,HANDLE&amp; hEventToReset);
bool Read_Available( std::string &amp;szData,HANDLE &amp; hEventToReset);
inline long GetSize() ;
inline bool IsEmpty() ;</font></pre>
    </td>
  </tr>
</table>
<p><font size="2" face="Verdana">&nbsp;</font></p>
<p><font size="2" face="Verdana">Here is the logic and working behind the
classes:</font></p>
<p><font size="2" face="Verdana">First of let me show you how to use the class.
In your application create an object of CSerialCommHelper like this:</font></p>
<p><font size="2" face="Verdana">CSerialCommHelper m_theCommPort;<br>
<br>
Call m_theCommPort.Init() passing in the necessary information. If you want you
can use default values.<br>
Next call m_theCommPort.Start()&nbsp;</font></p>
<p><font size="2" face="Verdana">If you want to get notification about when the
some data is available you can get the kernel event object to wait on by
calling&nbsp; m_theCommPort.GetWaitForEvent().</font></p>
<p><font size="2" face="Verdana">What CSerialCommHelper does is that on call to
Init(), it opens the specified COM port and also starts a thread. The thread
starts &quot;listening&quot; for any incoming data and once the data has been
received it reads&nbsp; all the data into a local buffer which is of type
CSerialBuffer . Once its done reading all the data it sets the event in case you
want to get the notification. Now you have three options </font></p>
<ul>
  <li><font size="2" face="Verdana">read all the data by calling ReadAvailable()
    which reads all the data . </font></li>
  <li><font size="2" face="Verdana">read up to some character by calling
    Read_Upto and passing character upto which you want to read.</font></li>
  <li><font size="2" face="Verdana">read N character calling Read_N passing the
    numbers to be read. </font></li>
</ul>
<p><font size="2" face="Verdana">There is one more thing that needs to be paid
attention. If you want to read 10 characters&nbsp; but there are only 5
characters in the local buffer, the read_N makes a blocking call and waits for
the timeout passed as the last parameter .&nbsp; Same is true for Read_Upto.</font></p>
<p><font size="2" face="Verdana">One more thing. If there are 10 characters in
the local buffer but you made a call to Read_N() for 5 characters you will be
returned first 5 characters. If you made a next call Read_N() for 5 characters
again, it would returned next 5 characters.<br>
<br>
Thats all there is to it. </font></p>
<p><font size="2" face="Verdana">If you think I have left something please feel
free to email me at <a href="mailto:ashishdhar@hotmail.com">ashishdhar@hotmail.com</a>
<br>
<br>
&nbsp;</font></p>

      </td>
    </tr>
  </tbody>
</table>
<hr width="70%" color="#000000">
<p align="center"><font face="Verdana" size="1">copyright © 2001-2002 <a href="http://www.c-sharpcenter.com">www.c-sharpcenter.com</a><br>
Material present in the articles is a copyright of the author or authors of the
articles of the respective articles. No material<br>
shall be copied without consent of the author.Also the material presented here
should be used at your own risk. No warranties or guarantees<br>
of any kind or nature are given.The web site is not responsible for any damages
whatsoever incurred by the use of the material presented in this web site.<br>
</font></p>

<hr width="60%" color="#000080">
<p align="center"><font face="Verdana" size="1">The contents of this article are
copyright of the author .<br>
Everything else is copyright of <a href="http://www.c-sharpcenter.com">www.codeconduct.com</a>
2002</font></p>

</body>

</html>
