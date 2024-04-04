modded class MissionBase
{
    void MissionBase()
    {
        IE_LiquidRegistry registry = IE_GetLiquidRegistry();
        GetRPCManager().AddRPC("IEF", "UpdateLiquidConfigRPC", registry, SingleplayerExecutionType.Both);
        IEF_LOG.Info("Initialized");
		if (GetGame().IsServer())
		{
			IEF_Metrics.ReportServerStarted();
		}
    }
}

modded class MissionServer
{
    override void InvokeOnConnect(PlayerBase player, PlayerIdentity identity)
    {
        super.InvokeOnConnect(player, identity);
        IE_LiquidRegistry registry = IE_GetLiquidRegistry();
        registry.SendLiquidConfig(identity);
    }
}