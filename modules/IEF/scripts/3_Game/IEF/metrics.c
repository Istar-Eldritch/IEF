ref IE_ModMetrics IEF_Metrics = new IE_ModMetrics("IEF", "https://dayz-services.eldritchist.dev", "c05330f2-931b-4ff3-974f-eecd92afad0e");

class IE_MetricRequest<Class T> {
    string token;
    ref T metric;

    void IE_MetricRequest(string _token, T _metric)
    {
        token = _token;
        metric = _metric;
    }
}


class IE_ServerStarted
{
    string type = "ServerStarted";
    bool repacked;
	
	void IE_ServerStarted(bool _repacked)
	{
		repacked = _repacked;
	}
}

class IE_MetricsCallback: RestCallback
{
    IE_ModMetrics m_Metrics;
    string m_Metric;
    int m_Retry;

    void IE_MetricsCallback(IE_ModMetrics metrics, string body, int retry)
    {
        m_Metrics = metrics;
        m_Metric = body;
        m_Retry = retry;
    }


	override void OnError( int errorCode )
	{
        IEF_LOG.Error("Error reporting metrics: " + errorCode);
	};


	override void OnTimeout()
	{
        IEF_LOG.Warn("Timeout reporting metrics");
	};

	override void OnSuccess( string data, int dataSize )
	{
        IEF_LOG.Debug("Data sent to metric");
	};
}

class IE_ModMetrics
{
    string m_MetricsAPI;
    string m_token;
    string m_modName;

	void IE_ModMetrics(string modName, string metricsAPI, string token)
	{
        m_modName = modName;
		m_MetricsAPI = metricsAPI;
        m_token = token;
	}

    void SendMetric(string body, int retries)
    {
        IE_MetricsCallback cb = new IE_MetricsCallback(this, body, retries);
        RestContext ctx = GetRestApi().GetRestContext(m_MetricsAPI);
        ctx.SetHeader("application/json");
        ctx.POST(cb, "/metric", body);
    }

    void ReportServerStarted()
    {
        string modParam
        GetCLIParam("mod", modParam);
 
        bool repacked = !modParam.Contains(m_modName);
		IE_ServerStarted metric = new IE_ServerStarted(repacked);
        IE_MetricRequest<IE_ServerStarted> metricRequest = new IE_MetricRequest<IE_ServerStarted>(m_token, metric);

		JsonSerializer json = new JsonSerializer();
		string body;
		json.WriteToString(metricRequest, false, body);

		IEF_LOG.Debug("Metric: " + body);
		SendMetric(body, 3);
    }
}
