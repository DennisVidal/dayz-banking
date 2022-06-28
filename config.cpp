class CfgPatches
{
	class DC_Banking
	{
		units[] = {};
		weapons[] = {};
		requiredVersion = 0.1;
		requiredAddons[] =
		{
			"DZ_Data",
			"DZ_Scripts",
			"JM_CF_Scripts"
		};
	};
};

class CfgMods
{
	class DC_Banking
	{
		dir = "DC_Banking";
		name = "Banking";
		credits = "";
		author = "Deadcraft";
		authorID = "711";
		version = "1.1";
		type = "mod";
		dependencies[] =
		{
			"World",
			"Mission"
		}; 
		class defs
		{
			class worldScriptModule
			{
				value = "";
				files[] =
				{
					"DC_Banking/Scripts/4_World"
				};
			};
			class missionScriptModule
			{
				value = "";
				files[] =
				{
					"DC_Banking/Scripts/5_Mission"
				};
			};
		};
	};
};

class CfgModels
{
	class ATM
	{
		sections[] =
		{
			"everything"
		};
	};
};
class CfgVehicles
{

	class HouseNoDestruct;
	class DC_BankingATM : HouseNoDestruct
	{
		scope = 1;
		model = "DC_Banking\Data\ATM.p3d";
		hiddenSelections[] =
		{
			"everything"
		};
		hiddenSelectionsTextures[] =
		{
			"DC_Banking\Data\ATMBlack_co.paa"
		};
	};

	class DC_BankingATMGreen : DC_BankingATM
	{
		scope = 1;
		hiddenSelectionsTextures[] =
		{
			"DC_Banking\Data\ATMGreen_co.paa"
		};
	};
	
	class DC_BankingATMBlue : DC_BankingATM
	{
		scope = 1;
		hiddenSelectionsTextures[] =
		{
			"DC_Banking\Data\ATMBlue_co.paa"
		};
	};

	class DC_BankingATMRed : DC_BankingATM
	{
		scope = 1;
		hiddenSelectionsTextures[] =
		{
			"DC_Banking\Data\ATMRed_co.paa"
		};
	};
	
	class DC_BankingATMOrange : DC_BankingATM
	{
		scope = 1;
		hiddenSelectionsTextures[] =
		{
			"DC_Banking\Data\ATMOrange_co.paa"
		};
	};
	
	class DC_BankingATMYellow : DC_BankingATM
	{
		scope = 1;
		hiddenSelectionsTextures[] =
		{
			"DC_Banking\Data\ATMYellow_co.paa"
		};
	};
	
	class DC_BankingATMPurple : DC_BankingATM
	{
		scope = 1;
		hiddenSelectionsTextures[] =
		{
			"DC_Banking\Data\ATMPurple_co.paa"
		};
	};
	
	class DC_BankingATMDarkGreen : DC_BankingATM
	{
		scope = 1;
		hiddenSelectionsTextures[] =
		{
			"DC_Banking\Data\ATMDarkGreen_co.paa"
		};
	};

	class DC_BankingATMDarkBlue : DC_BankingATM
	{
		scope = 1;
		hiddenSelectionsTextures[] =
		{
			"DC_Banking\Data\ATMDarkBlue_co.paa"
		};
	};

	class DC_BankingATMWhite : DC_BankingATM
	{
		scope = 1;
		hiddenSelectionsTextures[] =
		{
			"DC_Banking\Data\ATMWhite_co.paa"
		};
	};

	class DC_BankingATMGray : DC_BankingATM
	{
		scope = 1;
		hiddenSelectionsTextures[] =
		{
			"DC_Banking\Data\ATMGray_co.paa"
		};
	};

	class DC_BankingLocker : DC_BankingATM
	{
		scope = 1;
		model = "DZ\structures\furniture\cases\locker\locker_closed_v3.p3d";
	};
};