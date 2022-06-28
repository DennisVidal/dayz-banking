class DC_BankingATM : BuildingSuper
{	
	override void SetActions()
	{
		super.SetActions();
		
		AddAction(DC_ActionOpenBankingMenu);
	}
}