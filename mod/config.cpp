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
    	dependencies[] = { "Core", "Game", "World", "Mission" };
		class defs
		{
			class engineScriptModule
			{
				value = "";
				files[] = {"IE/Framework/scripts/1_Core"};
			};
			class gameLibScriptModule
			{
				value = "";
				files[] = {"IE/Framework/scripts/2_GameLib"};
			};
			class gameScriptModule
			{
				value = "";
				files[] = {"IE/Framework/scripts/3_Game"};
			};
			class worldScriptModule
			{
				value = "";
				files[] = {"IE/Framework/scripts/4_World"};
			};
			class missionScriptModule
			{
				value = "";
				files[] = {"IE/Framework/scripts/5_Mission"};
			};
		};
	};
};