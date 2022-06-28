class DC_BankingCurrency
{
	protected string m_ClassName;
	protected float m_Value;

	void DC_BankingCurrency(string className = "", float value = 1.0)
	{
		m_ClassName = className;
		m_Value = value;
	}
	
	void SetClassName(string name)
	{
		m_ClassName = name;
	}
	
	string GetClassName()
	{
		return m_ClassName;
	}
	
	void SetValue(float value = 1.0)
	{
		m_Value = value;
	}
	
	float GetValue()
	{
		return m_Value;
	}
}