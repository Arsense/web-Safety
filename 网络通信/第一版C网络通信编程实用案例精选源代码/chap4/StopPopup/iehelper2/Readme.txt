IEHelper - How to implement a Browser Helper Object
---------------------------------------------------

1. Introduction

In order to attach to a running instance of Internet Explorer 4.0 (IE4), An application called a "Browser Helper Object" can be used.  This application is a DLL that will attach itself to every instance of IE that is executed on the system.  IEHelper demonstrates how to create a Browser Helper Object.

When an instance of IE is started, it looks in the registry for CLSIDs stored under the following key:
      HKLM\SOFTWARE\Microsoft\Windows\CurrentVersion\Explorer\Browser Helper Objects

If this key exists and there are CLSIDs listed under this key, IE will use CoCreateInstance() to try to create an instance of each object listed as a subkey under this key.  Note that the CLSID of the object you create must be listed as a subkey and not a named value.


2. Restrictions of Browser Helper Objects

   1) The application that you create must be an in-proc server (i.e., DLL)
   2) This DLL must implement IObjectWithSite.
   3) The IObjectWithSite::SetSite() method must be implemented.  It is 
      through this method that your application receives a pointer to IE's 
      IUnknown. (IE actually passes a pointer to IWebBrowser2 but the 
      implementation of SetSite() receives a pointer to IUnknown.) You can 
      use this IUnknown pointer to automate IE or to sink events from IE.


3. Using IEHelper

To compile and run IEHelper, follow these steps:

   1) Compile IEHelper using Visual C++ 5.0 or above.  Compiling IEHelper will register the DLL in the registry.  The registry script contained in IEHlprObj.rgs will create the necessary registry entries to register IEHelper as an IE4 Browser Helper Object.

   2) Each time a new instance of IE4 is started, IEHelper will be loaded.  IE will create an instance of each Browser Helper Object listed in the registry every time a new instance of IE is executed.  This means that if you have Active Desktop installed, the Browser Helper Objects will be loaded every time you open a folder or change to a new folder as well as when the browser itself is started.

   3) View IE4 events in IEHelper's events window.

   4) To unregister IEHelper so that it is no longer registered as a Browser Helper Object, run regsvr32 with the following options from the command line: regsvr32 /u IEHelper.dll