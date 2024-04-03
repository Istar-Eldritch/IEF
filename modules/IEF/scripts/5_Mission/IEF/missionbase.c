modded class MissionBase
{
    void MissionBase()
    {
        IEF_LOG = new IE_Log("IEF", IE_LogLevel.INFO);
        IE_LiquidRegistry registry = IE_GetLiquidRegistry();
        GetRPCManager().AddRPC("IEF", "UpdateLiquidConfigRPC", registry, SingleplayerExecutionType.Both);
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