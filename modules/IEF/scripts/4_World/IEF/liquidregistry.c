
///
class IE_LiquidDetailsBase
{
    // Note that item could be null in some cases
    string GetName(ItemBase item)
    {
        return "";
    }
    // Note that item could be null in some cases.
    int GetColor(ItemBase item)
    {
        return 0;
    }
}

class IE_SimpleLiquidDetails : IE_LiquidDetailsBase
{
    string name;
    int color;

    void IE_SimpleLiquidDetails(string _name, int _color)
    {
        name = _name;
        color = _color;
    }

    override string GetName(ItemBase item)
    {
        return name;
    }

    override int GetColor(ItemBase item)
    {
        return color;
    }
}


class IE_LiquidConfig
{
    int id;
    string name;
    int color;
}

class IE_LiquidRegistryConfig
{
    int version = 1;
    ref array<ref IE_LiquidConfig> liquids = new array<ref IE_LiquidConfig>;
}

class IE_LiquidRegistryVersion
{
    int version;
}

class IE_LiquidRegistry
{
    static private const string DIR_PATH = "$profile:IE";
    static private const string LIQUID_CONFIG_PATH = DIR_PATH + "\\Liquids.json";

    ref map<int, ref IE_LiquidDetailsBase> m_liquids;
	
	ref IE_LiquidRegistryConfig m_config;

    void IE_LiquidRegistry()
    {
        m_liquids = new ref map<int, ref IE_LiquidDetailsBase>;

        // Vanilla liquids
        RegisterLiquid(LIQUID_WATER, "#inv_inspect_water", Colors.COLOR_LIQUID);
        RegisterLiquid(LIQUID_RIVERWATER, "#inv_inspect_river_water", Colors.COLOR_LIQUID);
        RegisterLiquid(LIQUID_VODKA, "#inv_inspect_vodka", Colors.COLOR_LIQUID);
        RegisterLiquid(LIQUID_BEER, "#inv_inspect_beer", Colors.COLOR_LIQUID);
        RegisterLiquid(LIQUID_GASOLINE, "#inv_inspect_gasoline", Colors.COLOR_LIQUID);
        RegisterLiquid(LIQUID_DIESEL, "#inv_inspect_diesel", Colors.COLOR_LIQUID);
        RegisterLiquid(LIQUID_DISINFECTANT, "#inv_inspect_disinfectant", Colors.COLOR_LIQUID);
        RegisterLiquid(LIQUID_SALINE, "#inv_inspect_saline", Colors.COLOR_LIQUID);

        // Rag liquid framework introduced
        #ifdef rag_liquid_framework
        RegisterLiquid(LIQUID_MILK, "MILK", Colors.MILKLIQUID);
        RegisterLiquid(LIQUID_HONEY, "HONEY", Colors.HONEYLIQUID);
        RegisterLiquid(LIQUID_HONEYWINE, "HONEYWINE", Colors.HONEYWINELIQUID);
        RegisterLiquid(LIQUID_WINE, "WINE", Colors.WINELIQUID);
        RegisterLiquid(LIQUID_RAGAVGAS, "AVIATION FUEL", Colors.AVGASLIQUID);
        RegisterLiquid(LIQUID_WHISKEY, "WHISKEY", Colors.WHISKEYLIQUID);
        RegisterLiquid(LIQUID_ESPRESSO, "SPRESSO", Colors.ESPRESSOLIQUID);
        RegisterLiquid(LIQUID_COFFEE, "COFFEE", Colors.COFFEELIQUID);
        RegisterLiquid(LIQUID_TEA, "TEA", Colors.TEALIQUID);
        RegisterLiquid(LIQUID_ENGINEOIL, "ENGINEOIL", Colors.ENGINEOILLIQUID);
        RegisterLiquid(LIQUID_ENERGYDRINK, "ENERGYDRINK", Colors.ENERGYDRINKLIQUID);
        RegisterLiquid(LIQUID_VIRUS, "VIRUS", Colors.VIRUSLIQUID);
        RegisterLiquid(LIQUID_POISON, "POISON", Colors.POISONLIQUID);
        RegisterLiquid(LIQUID_MILKCOFFEE, "MILKCOFFEE", Colors.MILKCOFFEELIQUID);
        RegisterLiquid(LIQUID_ANTIVENOM, "ANTIVENOM", Colors.ANTIVENOMLIQUID);
        #endif

		if (GetGame().IsServer())
        	Load_Config();
    }

    void Load_Config()
    {
        if (!FileExist(DIR_PATH))
        {
            MakeDirectory(DIR_PATH);
        }

        if (FileExist(LIQUID_CONFIG_PATH))
        { // If config exist load File
            IE_LiquidRegistryVersion v;
            JsonFileLoader<IE_LiquidRegistryVersion>.JsonLoadFile(LIQUID_CONFIG_PATH, v);
            if (v.version == 1)
            {
                JsonFileLoader<IE_LiquidRegistryConfig>.JsonLoadFile(LIQUID_CONFIG_PATH, m_config);
                foreach(IE_LiquidConfig liquidConfig: m_config.liquids)
                {
                    RegisterLiquid(liquidConfig.id, liquidConfig.name, liquidConfig.color, true);
                }
            }
            else
            {
                IEF_LOG.Error("Non recognised liquid config version.");
            }
        }
        else
        {
			m_config = new IE_LiquidRegistryConfig;
            JsonFileLoader<IE_LiquidRegistryConfig>.JsonSaveFile(LIQUID_CONFIG_PATH, m_config);
        }
    }

    // Other custom liquids can be registered.
    void RegisterLiquid(int liquidType, string name, int color, bool override_existing = false)
    {
       RegisterLiquid(liquidType, new IE_SimpleLiquidDetails(name, color), override_existing);
    }

    // Other custom liquids can be registered.
    void RegisterLiquid(int liquidType, IE_LiquidDetailsBase liquidDetails, bool override_existing = false)
    {
        auto existing = m_liquids.Get(liquidType);
        if (existing)
        {
			if (!override_existing)
			{
            	IEF_LOG.Info("Trying to register " + liquidDetails.GetName(null) + " but " + existing.GetName(null) + " was already registered");
            	return;
			}
			m_liquids.Remove(liquidType);
        }

        m_liquids.Insert(liquidType, liquidDetails);
		IEF_LOG.Info("Registered Liquid: " + liquidType + " - " + liquidDetails.GetName(null));
    }

    IE_LiquidDetailsBase GetLiquid(int liquidType)
    {
        return m_liquids.Get(liquidType);
    }

    void SendLiquidConfig(PlayerIdentity identity)
    {
		GetRPCManager().SendRPC("IEF", "UpdateLiquidConfigRPC", new Param1<IE_LiquidRegistryConfig>(m_config), true, identity);
    }

    void UpdateLiquidConfigRPC(CallType type, ref ParamsReadContext ctx, ref PlayerIdentity sender, ref Object target)
    {
        Param1<IE_LiquidRegistryConfig> data;
        if (ctx.Read(data))
        {
            m_config = data.param1;
            foreach(IE_LiquidConfig liquidConfig: m_config.liquids)
            {
                RegisterLiquid(liquidConfig.id, liquidConfig.name, liquidConfig.color, true);
            }
        }
    }
}

static ref IE_LiquidRegistry g_LiquidRegistry;
static IE_LiquidRegistry IE_GetLiquidRegistry()
{
    if (!g_LiquidRegistry)
    {
        g_LiquidRegistry = new IE_LiquidRegistry();
    }

    return g_LiquidRegistry;
}