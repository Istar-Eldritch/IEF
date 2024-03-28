
///
class LiquidDetailsBase
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

class SimpleLiquidDetails : LiquidDetailsBase
{
    string name;
    int color;

    void SimpleLiquidDetails(string _name, int _color)
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


class LiquidConfig
{
    int id;
    string name;
    int color;
}

class LiquidRegistryConfig
{
    int version = 1;
    ref array<ref LiquidConfig> liquids = new array<ref LiquidConfig>;
}

class LiquidRegistryVersion
{
    int version;
}

class LiquidRegistry
{
    static private const string DIR_PATH = "$profile:IE";
    static private const string LIQUID_CONFIG_PATH = DIR_PATH + "\\Liquids.json";

    ref map<int, ref LiquidDetailsBase> m_liquids;
	
	ref LiquidRegistryConfig m_config;

    void LiquidRegistry()
    {
        m_liquids = new ref map<int, ref LiquidDetailsBase>;

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
            LiquidRegistryVersion v;
            JsonFileLoader<LiquidRegistryVersion>.JsonLoadFile(LIQUID_CONFIG_PATH, v);
            if (v.version == 1)
            {
                JsonFileLoader<LiquidRegistryConfig>.JsonLoadFile(LIQUID_CONFIG_PATH, m_config);
                foreach(LiquidConfig liquidConfig: m_config.liquids)
                {
                    RegisterLiquid(liquidConfig.id, liquidConfig.name, liquidConfig.color, true);
                }
            }
            else
            {
                Print("[IEF] ERROR! Non recognised liquid config version.");
            }
        }
        else
        {
			m_config = new LiquidRegistryConfig;
            JsonFileLoader<LiquidRegistryConfig>.JsonSaveFile(LIQUID_CONFIG_PATH, m_config);
        }
    }

    // Other custom liquids can be registered.
    void RegisterLiquid(int liquidType, string name, int color, bool override_existing = false)
    {
       RegisterLiquid(liquidType, new SimpleLiquidDetails(name, color), override_existing);
    }

    // Other custom liquids can be registered.
    void RegisterLiquid(int liquidType, LiquidDetailsBase liquidDetails, bool override_existing = false)
    {
        auto existing = m_liquids.Get(liquidType);
        if (existing)
        {
			if (!override_existing)
			{
            	Print("[IEF] Trying to register " + liquidDetails.GetName(null) + " but " + existing.GetName(null) + " was already registered");
            	return;
			}
			m_liquids.Remove(liquidType);
        }

        m_liquids.Insert(liquidType, liquidDetails);
		Print("[IEF] Registered Liquid: " + liquidType + " - " + liquidDetails.GetName(null));
    }

    LiquidDetailsBase GetLiquid(int liquidType)
    {
        return m_liquids.Get(liquidType);
    }

    void RequestLiquidConfig()
    {
        GetRPCManager().SendRPC("IEF", "RequestLiquidConfigRPC", null, true);
    }

    void RequestLiquidConfigRPC(CallType type, ref ParamsReadContext ctx, ref PlayerIdentity sender, ref Object target)
    {
        Param1<LiquidRegistryConfig> data;
        if (ctx.Read(data))
        {
            m_config = data.param1;
            foreach(LiquidConfig liquidConfig: m_config.liquids)
            {
                RegisterLiquid(liquidConfig.id, liquidConfig.name, liquidConfig.color, true);
            }
        }
		else
		{
			if (GetGame().IsServer())
			{
				GetRPCManager().SendRPC("IEF", "RequestLiquidConfigRPC", new Param1<LiquidRegistryConfig>(m_config), true);
			}
		}
    }
}

static ref LiquidRegistry g_LiquidRegistry;
static LiquidRegistry GetLiquidRegistry()
{
    if (!g_LiquidRegistry)
    {
        g_LiquidRegistry = new LiquidRegistry();
    }

    return g_LiquidRegistry;
}