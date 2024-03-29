modded class MissionBase
{
    void MissionBase()
    {
        IEF_LOG = new IE_Log("IEF", LogLevel.INFO);
        LiquidRegistry registry = GetLiquidRegistry();
        GetRPCManager().AddRPC("IEF", "RequestLiquidConfigRPC", registry, SingleplayerExecutionType.Both);

        if (GetGame().IsClient())
        {
            registry.RequestLiquidConfig();
        }
    }
}