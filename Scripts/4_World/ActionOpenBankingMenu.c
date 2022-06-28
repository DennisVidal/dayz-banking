class DC_ActionOpenBankingMenu: ActionInteractBase
{
	void DC_ActionOpenBankingMenu()
	{
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONMOD_INTERACTONCE;
		m_StanceMask = DayZPlayerConstants.STANCEMASK_ALL;
		m_HUDCursorIcon = CursorIcons.None;
	}

	override string GetText()
	{
		return "Open banking menu";
	}

	override bool ActionCondition(PlayerBase player, ActionTarget target, ItemBase item)
	{
		return true;
	}
	
	override void OnExecuteClient(ActionData action_data)
	{
		super.OnExecuteClient(action_data);
		
		if(!DC_BankingMenu.GetInstance().IsVisible())
		{
			UIManager uiManager = GetGame().GetUIManager();
			uiManager.CloseAll();
			uiManager.ShowScriptedMenu(DC_BankingMenu.GetInstance(), null);
		}
	}
	
	override void OnExecuteServer(ActionData action_data)
	{
		super.OnExecuteServer(action_data);
		
		if(DC_Banking.GetInstance().GetConfig().GetConfigData().GetCreateLogs()) {Print("[DC_Banking]		Player with PlainID: " + action_data.m_Player.GetIdentity().GetPlainId() + " and Name: " + action_data.m_Player.GetIdentity().GetName() + " opened the banking menu");}
	}
}