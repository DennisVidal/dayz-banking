class DC_BankingData
{
	const protected static string m_ProfilePath = "$profile:";
	const protected static string m_DatabaseFolderName = "DC_Banking/PlayerDatabase";
	
	protected string m_PlainID;
	protected string m_Username;
	protected int m_OwnedCurrency;
	protected int m_MaxOwnedCurrencyBonus;
	
	void DC_BankingData(string id = "",string playerUsername = "", int ownedCurrency = 0, int maxOwnedCurrencyBonus = 0)
	{
		m_PlainID = id;
		m_Username = playerUsername;
		m_OwnedCurrency = ownedCurrency;
		m_MaxOwnedCurrencyBonus = maxOwnedCurrencyBonus;
	}
	
	static DC_BankingData LoadPlayerData(string id, string username = "")
	{
		if(id == "") 
		{
			return null;
		}
		
		DC_BankingData playerData = new DC_BankingData();
		
		if (!FileExist(m_ProfilePath + m_DatabaseFolderName + "/"))
        {
			MakeDirectory(m_ProfilePath + m_DatabaseFolderName + "/");
		}
		
		if (FileExist(m_ProfilePath + m_DatabaseFolderName + "/" + id + ".json"))
        {
            JsonFileLoader<DC_BankingData>.JsonLoadFile(m_ProfilePath + m_DatabaseFolderName + "/" + id + ".json", playerData);
			if(username != "")
			{
				playerData.SetUsername(username);
			}
        }
        else
        {
			playerData = CreateDefaultPlayerData(id, username);
        }
		
		return playerData;
	}
	
	protected static DC_BankingData CreateDefaultPlayerData(string id, string username = "")
	{
		DC_BankingData playerData = new DC_BankingData(id, username, DC_Banking.GetInstance().GetConfig().GetConfigData().GetStartCurrency(), 0);
		
		SavePlayerData(playerData);
		
		return playerData;
	}
	
	static void SavePlayerData(DC_BankingData playerData, string username = "")
	{
		if(!playerData)
		{
			return;
		}
		
		if(playerData.GetID() == "")
		{
			return;
		}
		
		if(username != "")
		{
			playerData.SetUsername(username);
		}
		
		JsonFileLoader<DC_BankingData>.JsonSaveFile(m_ProfilePath + m_DatabaseFolderName + "/" + playerData.GetID() + ".json", playerData);
	}
	
	
	string GetID()
	{
		return m_PlainID;
	}
	
	string GetUsername()
	{
		return m_Username;
	}
	
	int GetOwnedCurrency()
	{
		return m_OwnedCurrency;
	}
	
	int GetMaxOwnedCurrencyBonus()
	{
		return m_MaxOwnedCurrencyBonus;
	}
	
	int GetMaxOwnedCurrency()
	{
		return DC_Banking.GetInstance().GetConfig().GetConfigData().GetDefaultMaxCurrency() + m_MaxOwnedCurrencyBonus;
	}
	
	int GetDepositableAmount()
	{
		int depositableAmount = GetMaxOwnedCurrency() - m_OwnedCurrency;
		if(depositableAmount < 0)
		{
			depositableAmount = 0;
		}
		return depositableAmount;
	}
	
	
	void SetID(string id)
	{
		m_PlainID = id;
	}
	
	void SetUsername(string playerUsername)
	{
		m_Username = playerUsername;
	}
	
	void SetOwnedCurrency(int ownedCurrency)
	{
		m_OwnedCurrency = ownedCurrency;
	}
	
	void SetMaxOwnedCurrencyBonus(int maxOwnedCurrencyBonus)
	{
		m_MaxOwnedCurrencyBonus = maxOwnedCurrencyBonus;
	}
}