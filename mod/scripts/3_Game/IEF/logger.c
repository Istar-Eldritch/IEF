ref IE_Log IEF_LOG;

enum LogLevel
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

class IE_StdOutput: IE_LogOutput
{
    override void write(string msg) {
        Print("" + msg);
    }
}

class IE_Log
{
    LogLevel m_level;
    string m_name;
    ref IE_LogOutput m_output;

    void IE_Log(string name, LogLevel level, IE_LogOutput output = null)
    {
        m_name = name;
        m_level = level;
        m_output = output;
        if (!m_output)
        {
            m_output = new IE_StdOutput;
        }
    }

    void Trace(string msg)
    {
        if (m_level <= LogLevel.TRACE)
        {
            m_output.write("[" + m_name + "] [TRACE] " + msg);
        }
    }

    void Debug(string msg)
    {
        if (m_level <= LogLevel.DEBUG)
        {
            m_output.write("[" + m_name + "] [DEBUG] " + msg);
        }
    }

    void Info(string msg)
    {
        if (m_level <= LogLevel.INFO)
        {
            m_output.write("[" + m_name + "] [INFO] " + msg);
        }
    }

    void Warn(string msg)
    {
        if (m_level <= LogLevel.WARN)
        {
            m_output.write("[" + m_name + "] [WARN] " + msg);
        }
    }

    void Error(string msg)
    {
        if (m_level <= LogLevel.ERROR)
        {
            m_output.write("[" + m_name + "] [ERROR] " + msg);
        }
    }
}