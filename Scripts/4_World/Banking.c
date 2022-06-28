#define DC_BANKING

class DC_Banking
{
	protected static ref DC_Banking Instance;
	
	protected ref DC_BankingConfig m_Config;
	
	void DC_Banking()
	{
		if(GetGame().IsServer())
		{
			GetRPCManager().AddRPC("DC_Banking", "DepositRequest", this, SingleplayerExecutionType.Server);
			GetRPCManager().AddRPC("DC_Banking", "WithdrawRequest", this, SingleplayerExecutionType.Server);
			GetRPCManager().AddRPC("DC_Banking", "PlayerDataRequest", this, SingleplayerExecutionType.Server);
			GetRPCManager().AddRPC("DC_Banking", "ConfigDataRequest", this, SingleplayerExecutionType.Server);
			
			m_Config = new DC_BankingConfig();
		}
	}
	
	static DC_Banking GetInstance()
	{
		if (!Instance)
        {
            Instance = new DC_Banking();
        }
		return Instance;
	}
	
	static void ClearInstance()
	{
		Instance = null;
	}
	
	DC_BankingConfig GetConfig()
	{
		if(!m_Config)
		{
			m_Config = new DC_BankingConfig();
		}
		return m_Config;
	}
	
	//------------------------------Requests------------------------------
	
	void ConfigDataRequest(CallType type, ref ParamsReadContext ctx, ref PlayerIdentity sender, ref Object target)
	{
		if(type == CallType.Server)
        {
			GetRPCManager().SendRPC("DC_Banking", "ConfigDataRequestResponse", new Param2<float, float>(m_Config.GetConfigData().GetMessageDisplayTime(), m_Config.GetConfigData().GetTransactionDelayTime()), true, sender);
	
		}
	}
	
	//Player requested banking account data
	void PlayerDataRequest(CallType type, ref ParamsReadContext ctx, ref PlayerIdentity sender, ref Object target)
    {
		if(type == CallType.Server)
        {
			DC_BankingData playerData = DC_BankingData.LoadPlayerData(sender.GetPlainId(), sender.GetName());
			if(playerData)
			{
			    GetRPCManager().SendRPC("DC_Banking", "RequestResponse", new Param4<int, int, int, int>(playerData.GetMaxOwnedCurrency(), playerData.GetOwnedCurrency(), GetCurrencyAmountOnPlayer(GetPlayerByPlainID(sender.GetPlainId())), 0), true, sender);
				if(m_Config.GetConfigData().GetCreateLogs()) {Print("[DC_Banking]		Player with PlainID: " + sender.GetPlainId() + " and Name: " + sender.GetName() + ": Request for banking account data -> data was sent");}
			}
			else
			{
				if(m_Config.GetConfigData().GetCreateLogs()) {Print("[DC_Banking]		Player with PlainID: " + sender.GetPlainId() + " and Name: " + sender.GetName() + ": Request for banking account data -> banking account data could not be loaded and wasn't sent");}
			}
        }
    }
	
	//Player requested to withdraw currency
	void WithdrawRequest(CallType type, ref ParamsReadContext ctx, ref PlayerIdentity sender, ref Object target)
    {
		Param1<int> param;
        if(!ctx.Read(param)) return;
        if(type == CallType.Server)
        {
			if(m_Config.GetConfigData().GetCreateLogs()) {Print("[DC_Banking]		Player with PlainID: " + sender.GetPlainId() + " and Name: " + sender.GetName() + ": Request to withraw " + param.param1);}
			PlayerBase player = GetPlayerByPlainID(sender.GetPlainId());
			if(player)
			{
				DC_BankingData playerData = DC_BankingData.LoadPlayerData(sender.GetPlainId(), sender.GetName());
				if(playerData)
				{
					if(m_Config.GetConfigData().GetCreateLogs()) {Print("[DC_Banking]		Player with PlainID: " + sender.GetPlainId() + " and Name: " + sender.GetName() + ": Max owned currency before withdrawal = " + playerData.GetMaxOwnedCurrency() + ", owned currency before withdrawal = " + playerData.GetOwnedCurrency());}
				
					int amountToWithdraw = LimitAmountToWithdraw(player, param.param1, playerData);
					int notAddableAmount = AddCurrencyToPlayer(player, amountToWithdraw);
					int errorCode = HandleNotAddableAmount(player, notAddableAmount, playerData, true);
					RemoveCurrencyFromBankingAccount(amountToWithdraw , playerData);
					//SavePlayer(player);
					
					if(m_Config.GetConfigData().GetCreateLogs()) {Print("[DC_Banking]		Player with PlainID: " + sender.GetPlainId() + " and Name: " + sender.GetName() + ": Max owned currency after withdrawal = " + playerData.GetMaxOwnedCurrency() + ", owned currency after withdrawal = " + playerData.GetOwnedCurrency());}
					GetRPCManager().SendRPC("DC_Banking", "RequestResponse", new Param4<int, int, int, int>(playerData.GetMaxOwnedCurrency(), playerData.GetOwnedCurrency(), GetCurrencyAmountOnPlayer(player), errorCode), true, sender);
				}
				else
				{
					if(m_Config.GetConfigData().GetCreateLogs()) {Print("[DC_Banking]		Player with PlainID: " + sender.GetPlainId() + " and Name: " + sender.GetName() + ": PlayerData could not be loaded and a new one could not be created");}
				}
			}
			else
			{
				if(m_Config.GetConfigData().GetCreateLogs()) {Print("[DC_Banking]		Player with PlainID: " + sender.GetPlainId() + " and Name: " + sender.GetName() + ": Player could not be found");}
			}
		}
    }
	
	//Player requested to deposit currency
	void DepositRequest(CallType type, ref ParamsReadContext ctx, ref PlayerIdentity sender, ref Object target)
    {
		Param1<int> param;
        if(!ctx.Read(param)) return;
        if(type == CallType.Server)
        {
			if(m_Config.GetConfigData().GetCreateLogs()) {Print("[DC_Banking]		Player with PlainID: " + sender.GetPlainId() + " and Name: " + sender.GetName() + ": Request to deposit " + param.param1);}
			PlayerBase player = GetPlayerByPlainID(sender.GetPlainId());
			if(player)
			{
				DC_BankingData playerData = DC_BankingData.LoadPlayerData(sender.GetPlainId(), sender.GetName());
				if(playerData)
				{
					if(m_Config.GetConfigData().GetCreateLogs()) {Print("[DC_Banking]		Player with PlainID: " + sender.GetPlainId() + " and Name: " + sender.GetName() + ": Max owned currency before deposit = " + playerData.GetMaxOwnedCurrency() + ", owned currency before deposit = " + playerData.GetOwnedCurrency());}
				
					int amountToDeposit = LimitAmountToDeposit(player, param.param1, playerData);
					int notAddableAmount = RemoveCurrencyFromPlayer(player, amountToDeposit);
					int errorCode = HandleNotAddableAmount(player, notAddableAmount, playerData, false);
					AddCurrencyToBankingAccount(amountToDeposit , playerData);	
					//SavePlayer(player);
					
					if(m_Config.GetConfigData().GetCreateLogs()) {Print("[DC_Banking]		Player with PlainID: " + sender.GetPlainId() + " and Name: " + sender.GetName() + ": Max owned currency after deposit = " + playerData.GetMaxOwnedCurrency() + ", owned currency after deposit = " + playerData.GetOwnedCurrency());}
            		GetRPCManager().SendRPC("DC_Banking", "RequestResponse", new Param4<int, int, int, int>(playerData.GetMaxOwnedCurrency(), playerData.GetOwnedCurrency(), GetCurrencyAmountOnPlayer(player), errorCode), true, sender);
				}
				else
				{
					if(m_Config.GetConfigData().GetCreateLogs()) {Print("[DC_Banking]		Player with PlainID: " + sender.GetPlainId() + " and Name: " + sender.GetName() + ": PlayerData could not be loaded and a new one could not be created");}
				}
			}
			else
			{
				if(m_Config.GetConfigData().GetCreateLogs()) {Print("[DC_Banking]		Player with PlainID: " + sender.GetPlainId() + " and Name: " + sender.GetName() + ": Player could not be found");}
			}
		}
	}
	
	//--------------------------------------------------------------------
	
	//Limits the amount that can be withdrawn and returns the limited amount
	protected int LimitAmountToWithdraw(PlayerBase player, int amountToWithdraw, DC_BankingData playerData)
	{
		int LimitedAmountToWithdraw = amountToWithdraw;
		int	withdrawableAmount = playerData.GetOwnedCurrency();
		if(LimitedAmountToWithdraw < 0 || LimitedAmountToWithdraw > withdrawableAmount)
		{
			LimitedAmountToWithdraw = withdrawableAmount;
			if(m_Config.GetConfigData().GetCreateLogs()) {Print("[DC_Banking]		Player with PlainID: " + playerData.GetID() + " and Name: " + playerData.GetUsername() + ": Amount to withdraw was limited to the owned amount " + LimitedAmountToWithdraw);}
		}
		return LimitedAmountToWithdraw;
	}
	
	//Limits the amount that can be deposited and returns the limited amount
	protected int LimitAmountToDeposit(PlayerBase player, int amountToDeposit, DC_BankingData playerData)
	{
		int LimitedAmountToDeposit = amountToDeposit;
		
		int	depositableAmount = playerData.GetDepositableAmount();
		if(LimitedAmountToDeposit < 0 || LimitedAmountToDeposit > depositableAmount)
		{
			LimitedAmountToDeposit = depositableAmount;
			if(m_Config.GetConfigData().GetCreateLogs()) {Print("[DC_Banking]		Player with PlainID: " + playerData.GetID() + " and Name: " + playerData.GetUsername() + ": Amount to deposit was limited to the depositable amount " + LimitedAmountToDeposit);}
		}
					
		int amountOnPlayer = GetCurrencyAmountOnPlayer(player);
		if(amountOnPlayer < LimitedAmountToDeposit)
		{
			LimitedAmountToDeposit = amountOnPlayer;
			if(m_Config.GetConfigData().GetCreateLogs()) {Print("[DC_Banking]		Player with PlainID: " + playerData.GetID() + " and Name: " + playerData.GetUsername() + ": Amount to deposit was limited to the amount on the player " + LimitedAmountToDeposit);}
		}
		
		return LimitedAmountToDeposit;
	}
	
	//Handles not addable currency after a withdrawal or deposit and returns an error code
	protected int HandleNotAddableAmount(PlayerBase player, int notAddableAmount, DC_BankingData playerData, bool notAddableInWithdrawalRequest)
	{
		if(notAddableAmount > 0)
		{
			if(m_Config.GetConfigData().GetPlaceExcessiveCurrencyOnGround())
			{
				CreateCurrencyOnGround(player, notAddableAmount);
				if(notAddableInWithdrawalRequest)
				{
					if(m_Config.GetConfigData().GetCreateLogs()) {Print("[DC_Banking]		Player with PlainID: " + playerData.GetID() + " and Name: " + playerData.GetUsername() + ": " + notAddableAmount + " could not be added to the player and have been spawned on the ground");}
					return 1;
				}
				else
				{
					if(m_Config.GetConfigData().GetCreateLogs()) {Print("[DC_Banking]		Player with PlainID: " + playerData.GetID() + " and Name: " + playerData.GetUsername() + ": " + notAddableAmount + " could not be replaced and added back to the player and have been spawned on the ground");}
					return 3;
				}
			}
			else
			{
				playerData.SetOwnedCurrency(playerData.GetOwnedCurrency() + notAddableAmount);
				if(notAddableInWithdrawalRequest)
				{
					if(m_Config.GetConfigData().GetCreateLogs()) {Print("[DC_Banking]		Player with PlainID: " + playerData.GetID() + " and Name: " + playerData.GetUsername() + ": " + notAddableAmount + " could not be added to the player and have been added to the account of the player");}
					return 2;
				}
				else
				{
					if(m_Config.GetConfigData().GetCreateLogs()) {Print("[DC_Banking]		Player with PlainID: " + playerData.GetID() + " and Name: " + playerData.GetUsername() + ": " + notAddableAmount + " could not be replaced and added back to the player and have been added to the account of the player");}
					return 4;
				}
			}
		}
		return 0;
	}
	
	//Removes currency from a banking account and optionally saves it afterwards
	protected void RemoveCurrencyFromBankingAccount(int amountToRemove, DC_BankingData playerData, bool saveBankingDataAfterwards = true)
	{
		playerData.SetOwnedCurrency(playerData.GetOwnedCurrency() - amountToRemove);
		if(m_Config.GetConfigData().GetCreateLogs()) {Print("[DC_Banking]		Player with PlainID: " + playerData.GetID() + " and Name: " + playerData.GetUsername() + ": " + amountToRemove + " has been removed from the banking account");}
		if(saveBankingDataAfterwards)
		{
			DC_BankingData.SavePlayerData(playerData);
		}
	}
	
	//Adds currency to a banking account and optionally saves it afterwards
	protected void AddCurrencyToBankingAccount(int amountToAdd, DC_BankingData playerData, bool saveBankingDataAfterwards = true)
	{
		playerData.SetOwnedCurrency(playerData.GetOwnedCurrency() + amountToAdd);
		if(m_Config.GetConfigData().GetCreateLogs()) {Print("[DC_Banking]		Player with PlainID: " + playerData.GetID() + " and Name: " + playerData.GetUsername() + ": " + amountToAdd + " has been added to the banking account");}
		if(saveBankingDataAfterwards)
		{
			DC_BankingData.SavePlayerData(playerData);
		}
	}
	//Saves player
	protected bool SavePlayer(PlayerBase player)
	{
		return player.Save();
	}
	
	//Removes currency from the player and returns the amount that could not be replaced and added back to the player
	protected int RemoveCurrencyFromPlayer(PlayerBase player, int amountToRemove)
	{
		if(amountToRemove <= 0)
		{
			return 0;
		}
		
		int amountStillNeeded = amountToRemove;
		
		array<ref DC_BankingCurrency> currency = m_Config.GetConfigData().GetCurrency();
		
		array<EntityAI> inventory = new array<EntityAI>;
		player.GetInventory().EnumerateInventory(InventoryTraversalType.PREORDER, inventory);
		
		ItemBase item;
		
		int currencyValue;
		int quantityNeeded;
		for(int i = currency.Count() - 1; i >= 0; i--)
		{
			for(int j = 0; j < inventory.Count(); j++)
			{
				if(currency.Get(i).GetClassName() == inventory.Get(j).GetType())
				{
					Class.CastTo(item, inventory.Get(j));
					if(item)
					{
						currencyValue = currency.Get(i).GetValue();
						quantityNeeded = Math.Floor(amountStillNeeded / currencyValue); //floor is probably unnecessary since both are int
						if(GetItemQuantityMax(item) == 0)
						{
							GetGame().ObjectDelete(item);
							if(quantityNeeded >= 1)
							{
								amountStillNeeded -= currencyValue;
							}
							else
							{
								return AddCurrencyToPlayer(player, currencyValue - amountStillNeeded);							
							}
						}
						else
						{
							if(quantityNeeded >= GetItemQuantity(item))
							{
								amountStillNeeded -= GetItemQuantity(item) * currencyValue;
								GetGame().ObjectDelete(item);
							}
							else
							{
								SetItemQuantity(item, GetItemQuantity(item) - quantityNeeded);
								amountStillNeeded -= quantityNeeded * currencyValue;
								
								if(amountStillNeeded < currencyValue)
								{
									if(GetItemQuantity(item) == 1)
									{
										GetGame().ObjectDelete(item);
									}
									else
									{
										SetItemQuantity(item, GetItemQuantity(item) - 1);
									}
									return AddCurrencyToPlayer(player, currencyValue - amountStillNeeded);
								}
							}
						}
					}
				}
			}
		}
		return 0;
	}
	
	//Adds currency to the player and returns the amount that could not be added to the player
	protected int AddCurrencyToPlayer(PlayerBase player, int amountToAdd)
	{
		if(amountToAdd <= 0)
		{
			return 0;
		}
		int amountStillNeeded = amountToAdd;
		
		int maxQuantity;
		int currencyValue;
		int quantityNeeded;
		int quantityLeftToAdd;
		
		array<ref DC_BankingCurrency> currency = m_Config.GetConfigData().GetCurrency();
		
		for(int i = 0; i < currency.Count(); i++)
		{
			currencyValue = currency.Get(i).GetValue();
			quantityNeeded = Math.Floor(amountStillNeeded / currencyValue);
			if(quantityNeeded > 0)
			{
				quantityLeftToAdd = AddCurrencyToInventory(player, i, quantityNeeded);
				amountStillNeeded -= (quantityNeeded - quantityLeftToAdd) * currencyValue;
				
				if(amountStillNeeded == 0)
				{
					return 0;
				}
			}
		}
		return amountStillNeeded;
	}
	
	//Adds currency to the inventory and hands of the player and returns the amount that could not be added
	protected int AddCurrencyToInventory(PlayerBase player, int indexOfCurrency, int quantityToAdd)
	{
		if(quantityToAdd <= 0)
		{
			return 0;
		}
		
		int quantityLeftToAdd = quantityToAdd;
		
		DC_BankingCurrency currency = m_Config.GetConfigData().GetCurrency().Get(indexOfCurrency);
		
		array<EntityAI> inventory = new array<EntityAI>;
		player.GetInventory().EnumerateInventory(InventoryTraversalType.PREORDER, inventory);
		ItemBase currencyItem;
		
		int addableQuantity;
		
		for(int i = 0; i < inventory.Count(); i++) //Add currency to already in the inventory existing currency
		{
			if(inventory.Get(i).GetType() == currency.GetClassName())
			{
				Class.CastTo(currencyItem, inventory.Get(i));
				if(currencyItem)
				{
					addableQuantity = GetItemQuantityMax(currencyItem) - GetItemQuantity(currencyItem);
					if(addableQuantity > 0)
					{
						if(addableQuantity >= quantityLeftToAdd)
						{
							SetItemQuantity(currencyItem, GetItemQuantity(currencyItem) + quantityLeftToAdd);
							quantityLeftToAdd = 0;
						}
						else
						{
							SetItemQuantity(currencyItem, GetItemQuantityMax(currencyItem));
							quantityLeftToAdd -= addableQuantity;
						}
					}
					
					if(quantityLeftToAdd == 0)
					{
						return 0;
					}
				}
			}
		}
		
		EntityAI createdCurrencyEntity;
		int currencyItemMaxQuantity;
		InventoryLocation invLocation = new InventoryLocation();
		while(player.GetInventory().FindFirstFreeLocationForNewEntity(currency.GetClassName(), FindInventoryLocationType.CARGO, invLocation)) //Create new currency in the inventory
		{
			createdCurrencyEntity = player.GetHumanInventory().CreateInInventory(currency.GetClassName());
			Class.CastTo(currencyItem, createdCurrencyEntity);
			if(currencyItem)
			{
				currencyItemMaxQuantity = GetItemQuantityMax(currencyItem);
				if(currencyItemMaxQuantity == 0)
				{
					SetItemQuantity(currencyItem, 0);
					quantityLeftToAdd -= 1;
				}
				else
				{
					if(quantityLeftToAdd <= currencyItemMaxQuantity)
					{
						SetItemQuantity(currencyItem, quantityLeftToAdd);
						quantityLeftToAdd = 0;
					}
					else
					{
						SetItemQuantity(currencyItem, currencyItemMaxQuantity);
						quantityLeftToAdd -= currencyItemMaxQuantity;
					}
				}
				
				if(quantityLeftToAdd == 0)
				{
					return 0;
				}
			}
			else
			{
				break;
			}
		}
		
		if(!player.GetHumanInventory().GetEntityInHands()) //Create new currency in the hands of the player
		{
			createdCurrencyEntity = player.GetHumanInventory().CreateInHands(currency.GetClassName());
			Class.CastTo(currencyItem, createdCurrencyEntity);
			if(currencyItem)
			{
				currencyItemMaxQuantity = GetItemQuantityMax(currencyItem);
				if(currencyItemMaxQuantity == 0)
				{
					SetItemQuantity(currencyItem, 0);
					quantityLeftToAdd -= 1;
				}
				else
				{
					if(quantityLeftToAdd <= currencyItemMaxQuantity)
					{
						SetItemQuantity(currencyItem, quantityLeftToAdd);
						quantityLeftToAdd = 0;
					}
					else
					{
						SetItemQuantity(currencyItem, currencyItemMaxQuantity);
						quantityLeftToAdd -= currencyItemMaxQuantity;
					}
				}
				
				if(quantityLeftToAdd == 0)
				{
					return 0;
				}
			}
		}
		return quantityLeftToAdd;
	}
	
	//Spawns currency on the ground beneath the player
	protected void CreateCurrencyOnGround(PlayerBase player, int amount)
	{	
		if(amount <= 0)
		{
			return;
		}
		
		int amountStillNeeded = amount;
		int quantityNeeded;
		int currencyItemMaxQuantity;
		DC_BankingCurrency currency;
		EntityAI currencyEntity;
		ItemBase currencyItem;
		while(amountStillNeeded > 0)
		{
			currency = BestCurrencyStackForAmount(amountStillNeeded);
			currencyEntity = player.SpawnEntityOnGroundPos(currency.GetClassName(), player.GetPosition());
			Class.CastTo(currencyItem, currencyEntity);
			if(currencyItem)
			{
				currencyItemMaxQuantity = GetItemQuantityMax(currencyItem);
				quantityNeeded = Math.Floor(amountStillNeeded / currency.GetValue());
				if(currencyItemMaxQuantity == 0)
				{
					SetItemQuantity(currencyItem, 0);
					amountStillNeeded -= currency.GetValue();
				}
				else
				{
					if(quantityNeeded < currencyItemMaxQuantity)
					{
						SetItemQuantity(currencyItem, quantityNeeded);
						amountStillNeeded -= quantityNeeded * currency.GetValue();
					}
					else
					{
						SetItemQuantity(currencyItem, currencyItemMaxQuantity);
						amountStillNeeded -= currencyItemMaxQuantity * currency.GetValue();
					}
				}
			}
			else
			{
				break;
			}
		}
	}
	
	//Returns the currency that would have the lowest amount left to add
	protected DC_BankingCurrency BestCurrencyStackForAmount(int amount)
	{
		int bestIndex = -1;
		int bestValueLeft = int.MAX;
		int value;
		int valueLeft;
		int quantityOverMax;
		
		array<ref DC_BankingCurrency> currency = m_Config.GetConfigData().GetCurrency();
		
		for(int i = 0; i < currency.Count(); i++)
		{
			value = currency.Get(i).GetValue();
			valueLeft = amount % value;
			quantityOverMax = Math.Floor(amount / value) - GetMaxQuantityFromItemConfig(currency.Get(i).GetClassName());
			if(quantityOverMax > 0)
			{
				valueLeft += (quantityOverMax * value);
			}
			
			if(bestValueLeft > valueLeft)
			{
				bestValueLeft = valueLeft;
				bestIndex = i;
			}
		}
		
		if(bestIndex == -1)
		{
			return null;
		}
		return currency.Get(bestIndex);
	}
	
	//Returns the player that has the matching plain ID
	protected static PlayerBase GetPlayerByPlainID(string plainID)
	{
		array<Man> players = new array<Man>;
		GetGame().GetPlayers(players);
		for(int i = 0; i < players.Count(); i++)
		{
			if(players.Get(i).GetIdentity().GetPlainId() == plainID)
			{
				return PlayerBase.Cast(players.Get(i));
			}
		}
		return null;
	}
	
	//Returns the amount of currency the player currently carries
	protected int GetCurrencyAmountOnPlayer(PlayerBase player)
	{
		int currencyAmountOnPlayer = 0;
		
		array<EntityAI> inventory = new array<EntityAI>;
		player.GetInventory().EnumerateInventory(InventoryTraversalType.PREORDER, inventory);
		
		array<ref DC_BankingCurrency> currency = m_Config.GetConfigData().GetCurrency();
		
		ItemBase item;
		for (int i = 0; i < inventory.Count(); i++)
		{
			Class.CastTo(item, inventory.Get(i));
			if (item)
			{
				for (int j = 0; j < currency.Count(); j++)
				{
					if(item.GetType() == currency.Get(j).GetClassName())
					{
						if(GetItemQuantityMax(item) == 0)
						{
							currencyAmountOnPlayer += currency.Get(j).GetValue();
						}
						else
						{
							currencyAmountOnPlayer += GetItemQuantity(item) * currency.Get(j).GetValue();
						}
					}
				}
			}
		}
		return currencyAmountOnPlayer;
	}
		
	//Returns the max quantity of an item without needing an instance
	protected static int GetMaxQuantityFromItemConfig(string itemClassname, bool actualMaxQuantity = false)
	{
		int max = 0;
		if(GetGame().ConfigIsExisting(CFG_VEHICLESPATH + " " + itemClassname + " varQuantityMax"))
		{
			max = GetGame().ConfigGetInt(CFG_VEHICLESPATH + " " + itemClassname + " varQuantityMax");
		}
		else if(GetGame().ConfigIsExisting(CFG_MAGAZINESPATH + " " + itemClassname + " count"))
		{
			max = GetGame().ConfigGetInt(CFG_MAGAZINESPATH + " " + itemClassname + " count");
		}
		
		if(max == 0 && !actualMaxQuantity)
		{
			return 1;
		}
		return max;
	}
	
	protected void SetItemQuantity(ItemBase item, int quantity)
	{
		if(!item || quantity < 0)
		{
			return;
		}
		
		if(item.IsMagazine())
		{
			Magazine magazineItem = Magazine.Cast(item);
			if(magazineItem)
			{
				magazineItem.ServerSetAmmoCount(quantity);
			}
		}
		else
		{
			item.SetQuantity(quantity);
		}
	}
	
	protected int GetItemQuantity(ItemBase item)
	{
		if(!item)
		{
			return 0;
		}
		
		if(item.IsMagazine())
		{
			Magazine magazineItem = Magazine.Cast(item);
			if(magazineItem)
			{
				return magazineItem.GetAmmoCount();
			}
		}
		return item.GetQuantity();
	}
	
	protected int GetItemQuantityMax(ItemBase item)
	{
		if(!item)
		{
			return 0;
		}
		
		if(item.IsMagazine())
		{
			Magazine magazineItem = Magazine.Cast(item);
			if(magazineItem)
			{
				return magazineItem.GetAmmoMax();
			}
		}
		return item.GetQuantityMax();
	}
}	