class DC_BankingConfigData
{
	protected int m_DefaultMaxCurrency;
	protected int m_StartCurrency;
	protected bool m_PlaceExcessiveCurrencyOnGround;
	protected bool m_CreateLogs;
	protected float m_MessageDisplayTime;
	protected float m_TransactionDelayTime;
	protected ref array<ref DC_BankingCurrency> m_BankingCurrencyArray;
	
	void DC_BankingConfigData(int defaultMaxCurrency = 0, int startCurrency = 0, bool placeExcessiveCurrencyOnGround = true, bool createLogs = true, float messageDisplayTime = 5.0, float transactionDelayTime = 1.0)
	{
		m_DefaultMaxCurrency = defaultMaxCurrency;
		m_StartCurrency = startCurrency;
		m_PlaceExcessiveCurrencyOnGround = placeExcessiveCurrencyOnGround;
		m_CreateLogs = createLogs;
		m_MessageDisplayTime = messageDisplayTime;
		m_TransactionDelayTime = transactionDelayTime;
		m_BankingCurrencyArray = new array<ref DC_BankingCurrency>;
		SortCurrencyData();
	}
	
	void SetDefaultMaxCurrency(int defaultMaxCurrency)
	{
		m_DefaultMaxCurrency = defaultMaxCurrency;
	}
		
	int GetDefaultMaxCurrency()
	{
		return m_DefaultMaxCurrency;
	}
	
	void SetStartCurrency(int startCurrency)
	{
		m_StartCurrency = startCurrency;
	}
		
	int GetStartCurrency()
	{
		return m_StartCurrency;
	}
	
	array<ref DC_BankingCurrency> GetCurrency()
	{
		return m_BankingCurrencyArray;
	}
	
	bool GetPlaceExcessiveCurrencyOnGround()
	{
		return m_PlaceExcessiveCurrencyOnGround;
	}
	
	void SetPlaceExcessiveCurrencyOnGround(bool placeOnGround)
	{
		m_PlaceExcessiveCurrencyOnGround = placeOnGround;
	}
	
	bool GetCreateLogs()
	{
		return m_CreateLogs;
	}
	
	void SetCreateLogs(bool createLogs)
	{
		m_CreateLogs = createLogs;
	}
	
	float GetMessageDisplayTime()
	{
		return m_MessageDisplayTime;
	}
	
	void SetMessageDisplayTime(float messageDisplayTime)
	{
		m_MessageDisplayTime = messageDisplayTime;
	}
	
	float GetTransactionDelayTime()
	{
		return m_TransactionDelayTime;
	}
	
	void SetTransactionDelayTime(float transactionDelayTime)
	{
		m_TransactionDelayTime = transactionDelayTime;
	}
	
	
	int GetCurrencyValueByClassName(string className)
	{
		for(int i = 0; i < m_BankingCurrencyArray.Count(); i++)
		{
			if(m_BankingCurrencyArray.Get(i).GetClassName() == className)
			{
				return m_BankingCurrencyArray.Get(i).GetValue();
			}
		}
		return -1;
	}
		
	void SortCurrencyData()
	{
		int highestValueIndex;
		for(int i = 0; i < m_BankingCurrencyArray.Count(); i++)
		{
			highestValueIndex = i;
			for(int j = i+1; j < m_BankingCurrencyArray.Count(); j++)
			{
				if(m_BankingCurrencyArray.Get(highestValueIndex).GetValue() < m_BankingCurrencyArray.Get(j).GetValue())
				{
					highestValueIndex = j;
				}
			}
			
			if(highestValueIndex > i)
			{
				m_BankingCurrencyArray.SwapItems(i, highestValueIndex);
			}
		}
	}
}