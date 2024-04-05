#ifdef DIAG_DEVELOPER
ref IE_Log IEF_LOG = new IE_Log("IEF", IE_LogLevel.TRACE);
#endif
#ifndef DIAG_DEVELOPER
ref IE_Log IEF_LOG = new IE_Log("IEF", IE_LogLevel.INFO);
#endif

enum IE_LogLevel
{
    TRACE,
    DEBUG,
    INFO,
    WARN,
    ERROR
}

class IE_LogOutput
{
    void write(string msg);
}

class IE_StdOutput : IE_LogOutput
{
    override void write(string msg)
    {
        Print("" + msg);
    }
}

class IE_Log
{
    IE_LogLevel m_level;
    string m_name;
    ref IE_LogOutput m_output;

    void IE_Log(string name, IE_LogLevel level = IE_LogLevel.INFO, IE_LogOutput output = null)
    {
        m_name = name;
        m_level = level;
        m_output = output;
        if (!m_output)
        {
            m_output = new IE_StdOutput;
        }
    }

    void SetLogLevel(IE_LogLevel level)
    {
        m_level = level;
    }

    void Trace(string msg)
    {
        if (m_level <= IE_LogLevel.TRACE)
        {
            m_output.write("[" + m_name + "] [TRACE] " + msg);
        }
    }

    void Debug(string msg)
    {
        if (m_level <= IE_LogLevel.DEBUG)
        {
            m_output.write("[" + m_name + "] [DEBUG] " + msg);
        }
    }

    void Info(string msg)
    {
        if (m_level <= IE_LogLevel.INFO)
        {
            m_output.write("[" + m_name + "] [INFO] " + msg);
        }
    }

    void Warn(string msg)
    {
        if (m_level <= IE_LogLevel.WARN)
        {
            m_output.write("[" + m_name + "] [WARN] " + msg);
        }
    }

    void Error(string msg)
    {
        if (m_level <= IE_LogLevel.ERROR)
        {
            m_output.write("[" + m_name + "] [ERROR] " + msg);
        }
    }
}