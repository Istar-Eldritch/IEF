class CfgPatches
{
	class IE_Framework
	{
		units[] = {};
		weapons[] = {};
		requiredVersion = 0.1;
		requiredAddons[] =
			{
				"DZ_Data",
				"DZ_Scripts"
			};
	};
};

class CfgMods
{
	class IE_Framework
	{
		type = "mod";
		author = "Istar Eldritch";
    		dependencies[] = { "Game", "World", "Mission" };
		class defs
		{
			class gameScriptModule
			{
				value = "";
				files[] = {"IEF/scripts/3_Game"};
			};
			class worldScriptModule
			{
				value = "";
				files[] = {"IEF/scripts/4_World"};
			};
			class missionScriptModule
			{
				value = "";
				files[] = {"IEF/scripts/5_Mission"};
			};
		};
	};
};