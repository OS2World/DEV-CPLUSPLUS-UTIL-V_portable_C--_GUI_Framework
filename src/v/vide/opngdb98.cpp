//====================>>> vdbCmdWindow::OpenGDB <<<====================
  bool vdbCmdWindow::OpenGDB(const char* exe)
  {
    SECURITY_ATTRIBUTES saAttr; 
    BOOL fSuccess; 

    /* Set the bInheritHandle flag so pipe handles are inherited. */ 

    saAttr.nLength = sizeof(SECURITY_ATTRIBUTES);
    saAttr.bInheritHandle = TRUE;
    saAttr.lpSecurityDescriptor = NULL;
 
    /* 
     * The steps for redirecting child's STDOUT: 
     *     1.  Save current STDOUT, to be restored later. 
     *     2.  Create anonymous pipe to be STDOUT for child. 
     *     3.  Set STDOUT of parent to be write handle of pipe, so 
     *         it is inherited by child.
     */ 
 
    /* Save the handle to the current STDOUT. */ 

    hSaveStdout = GetStdHandle(STD_OUTPUT_HANDLE);
 
    /* Create a pipe for the child's STDOUT. */ 
 
    if (! CreatePipe(&hChildStdoutRd, &hChildStdoutWr, &saAttr, 0))
        return 0;	// ErrorExit("Stdout pipe creation failed\n");
 
    /* Set a write handle to the pipe to be STDOUT. */ 

    if (! SetStdHandle(STD_OUTPUT_HANDLE, hChildStdoutWr))
        return 0;	// ErrorExit("Redirecting STDOUT failed"); 
 
    /* 
     * The steps for redirecting child's STDIN: 
     *     1.  Save current STDIN, to be restored later. 
     *     2.  Create anonymous pipe to be STDIN for child. 
     *     3.  Set STDIN of parent to be read handle of pipe, so 
     *         it is inherited by child. 
     *     4.  Create a noninheritable duplicate of write handle, 
     *         and close the inheritable write handle. 
     */ 
 
    /* Save the handle to the current STDIN. */ 

    hSaveStdin = GetStdHandle(STD_INPUT_HANDLE);
 
    /* Create a pipe for the child's STDIN. */ 
 
    if (! CreatePipe(&hChildStdinRd, &hChildStdinWr, &saAttr, 0)) 
        return 0;	// ErrorExit("Stdin pipe creation failed\n");

    /* Set a read handle to the pipe to be STDIN. */ 
 
    if (! SetStdHandle(STD_INPUT_HANDLE, hChildStdinRd)) 
        return 0;	// ErrorExit("Redirecting Stdin failed"); 
 
    /* Duplicate the write handle to the pipe so it is not inherited. */
 
    fSuccess = DuplicateHandle(GetCurrentProcess(), hChildStdinWr, 
        GetCurrentProcess(), &hChildStdinWrDup, 0, 
        FALSE,       /* not inherited */ 
        DUPLICATE_SAME_ACCESS); 

    if (! fSuccess) 
        return 0;	// ErrorExit("DuplicateHandle failed"); 
 
    CloseHandle(hChildStdinWr); 
 
    /* Now create the child process. */


    static PROCESS_INFORMATION piProcInfo;
    STARTUPINFO siStartInfo; 

    /* Set up members of STARTUPINFO structure. */ 

    siStartInfo.cb = sizeof(STARTUPINFO); 
    siStartInfo.lpReserved = NULL; 
    siStartInfo.lpReserved2 = NULL; 
    siStartInfo.cbReserved2 = 0; 
    siStartInfo.lpDesktop = NULL;
    siStartInfo.wShowWindow = SW_MINIMIZE;
    siStartInfo.dwFlags = STARTF_USESHOWWINDOW;


    char cmd[maxFileNameSize+20];

    strcpy(cmd,"gdb -f");
    if (exe && *exe)
      {
	strcat(cmd," ");
	strcat(cmd,exe);
      }
    
    /* Create the child process. */

    if (! CreateProcess(NULL,
		cmd,
		NULL,          /* process security attributes        */ 
		NULL,          /* primary thread security attributes */ 
		TRUE,          /* handles are inherited              */
		0,             /* creation flags                     */
		NULL,          /* use parent's environment           */
		NULL,          /* use parent's current directory     */ 

		&siStartInfo,  /* STARTUPINFO pointer                */ 
		&piProcInfo))  /* receives PROCESS_INFORMATION       */
      {
	static DWORD errorCode = ::GetLastError();
	if (errorCode != 0)
	  {
	    vdbCanvas->addLine("CreateProcess Fails....");
	    vdbCanvas->bufferBottom();
	  }

	return 0;	// create fails
      }


    dbProcessId = piProcInfo.dwProcessId;	// save these for termination
    dbProcessHandle = piProcInfo.hProcess;

    /* After process creation, restore the saved STDIN and STDOUT. */

    if (! SetStdHandle(STD_INPUT_HANDLE, hSaveStdin))
        return 0;	// ErrorExit("Re-redirecting Stdin failed\n");

    if (! SetStdHandle(STD_OUTPUT_HANDLE, hSaveStdout))
        return 0;	// ErrorExit("Re-redirecting Stdout failed\n");

    return 1;

  }

//====================>>> vdbCmdWindow::dbSendCmd <<<====================
