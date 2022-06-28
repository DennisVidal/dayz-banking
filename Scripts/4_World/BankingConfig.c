class DC_BankingConfig
{
	const protected static string m_ProfilePath = "$profile:";
	const protected static string m_ConfigFolderName = "DC_Banking";
	const protected static string m_ConfigName = "BankingConfig.json";
	
	
	protected static ref DC_BankingConfigData m_BankingConfigData;
	
	void DC_BankingConfig()
	{
		LoadConfig();
	}
	
	DC_BankingConfigData GetConfigData()
	{
		if(!m_BankingConfigData)
		{
			LoadConfig();
		}
		return m_BankingConfigData;
	}
	
	protected void LoadConfig()
	{
		if (!FileExist(m_ProfilePath + m_ConfigFolderName + "/"))
        {
			MakeDirectory(m_ProfilePath + m_ConfigFolderName + "/");
		}
		
		m_BankingConfigData = new DC_BankingConfigData();
		
		if (FileExist(m_ProfilePath + m_ConfigFolderName + "/" + m_ConfigName))
        {
            JsonFileLoader<DC_BankingConfigData>.JsonLoadFile(m_ProfilePath + m_ConfigFolderName + "/" + m_ConfigName, m_BankingConfigData);
			m_BankingConfigData.SortCurrencyData();
			if(m_BankingConfigData.GetMessageDisplayTime() == 0.0)
			{
				m_BankingConfigData.SetMessageDisplayTime(5.0);
			}
        }
        else
        {
            CreateDefaultConfig();
        }
	}
	
	protected void CreateDefaultConfig()
	{
		m_BankingConfigData = new DC_BankingConfigData(10000);
		m_BankingConfigData.GetCurrency().Insert(new DC_BankingCurrency("MoneyRuble100", 100));
		m_BankingConfigData.GetCurrency().Insert(new DC_BankingCurrency("MoneyRuble50", 50));
		m_BankingConfigData.GetCurrency().Insert(new DC_BankingCurrency("MoneyRuble25", 25));
		m_BankingConfigData.GetCurrency().Insert(new DC_BankingCurrency("MoneyRuble10", 10));
		m_BankingConfigData.GetCurrency().Insert(new DC_BankingCurrency("MoneyRuble5", 5));
		m_BankingConfigData.GetCurrency().Insert(new DC_BankingCurrency("MoneyRuble1", 1));
		m_BankingConfigData.SortCurrencyData();
		
        JsonFileLoader<DC_BankingConfigData>.JsonSaveFile(m_ProfilePath + m_ConfigFolderName + "/" + m_ConfigName, m_BankingConfigData);
	}
}