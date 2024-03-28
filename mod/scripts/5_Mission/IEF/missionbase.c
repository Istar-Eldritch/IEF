modded class MissionBase
{
    void MissionBase()
    {
        LiquidRegistry registry = GetLiquidRegistry();
        GetRPCManager().AddRPC("IEF", "RequestLiquidConfigRPC", registry, SingleplayerExecutionType.Both);

        if (GetGame().IsClient())
        {
            registry.RequestLiquidConfig();
        }
    }
}