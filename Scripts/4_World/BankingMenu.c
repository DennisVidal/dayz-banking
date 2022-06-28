class DC_BankingMenu : UIScriptedMenu
{
	
	protected static ref DC_BankingMenu Instance;
	
	protected ButtonWidget m_ButtonDeposit;
	protected ButtonWidget m_ButtonWithdraw;
	protected ButtonWidget m_ButtonClose;
	protected TextWidget m_TextMaxOwnedAmount;
	protected TextWidget m_TextOwnedAmount;
	protected TextWidget m_TextAmountInput;
	protected EditBoxWidget m_EditBoxAmount;
	protected TextWidget m_TextOnPlayerAmount;
	protected MultilineTextWidget m_MultilineTextMessage;
	protected Widget m_PanelMessage;
	protected Widget m_PanelAmountInput;
	
	protected float m_MessageTimer;
	protected float m_MessageTime;
	
	protected float m_TransactionDelayTimer;
	protected float m_TransactionDelayTime;
	
	protected bool m_IsWaitingOnServerResponse;
	
	void DC_BankingMenu()
	{
		if(GetGame().IsClient())
		{
			m_MessageTimer = -1;
			m_MessageTime = -1;
			m_TransactionDelayTimer = 0;
			m_TransactionDelayTime = 5;
			m_IsWaitingOnServerResponse = false;
			GetRPCManager().AddRPC("DC_Banking", "RequestResponse", this, SingleplayerExecutionType.Client);
			GetRPCManager().AddRPC("DC_Banking", "ConfigDataRequestResponse", this, SingleplayerExecutionType.Client);
			
			RequestConfigData();
		}
	}
	
	void ~DC_BankingMenu()
	{
        GetGame().GetUIManager().ShowCursor(false);
        GetGame().GetInput().ResetGameFocus();
        GetGame().GetMission().PlayerControlEnable(true);
	}
	
	static DC_BankingMenu GetInstance()
	{
		if (!Instance)
        {
            Instance = new DC_BankingMenu();
			Instance.Init();
        }
		return Instance;
	}
	
	static void ClearInstance()
	{
		Instance = null;
	}
	
	void RequestResponse(CallType type, ref ParamsReadContext ctx, ref PlayerIdentity sender, ref Object target)
	{
		Param4<int, int, int, int> params;
        if(!ctx.Read(params)) return;
        if(type == CallType.Client)
		{
			m_TextMaxOwnedAmount.SetText(params.param1.ToString());
			m_TextOwnedAmount.SetText(params.param2.ToString());
			m_TextOnPlayerAmount.SetText(params.param3.ToString());
			
			ShowMessage(params.param4);
			m_IsWaitingOnServerResponse = false;
		}
	}
	
	void RequestConfigData()
	{
		GetRPCManager().SendRPC("DC_Banking", "ConfigDataRequest", null, true);
	}
	
	void ConfigDataRequestResponse(CallType type, ref ParamsReadContext ctx, ref PlayerIdentity sender, ref Object target)
	{
		Param2<float, float> params;
        if(!ctx.Read(params)) return;
        if(type == CallType.Client)
		{
			m_MessageTime = params.param1;
			m_TransactionDelayTime = params.param2;
		}
	}
	
	override Widget Init()
    {
		
		layoutRoot = GetGame().GetWorkspace().CreateWidgets("DC_Banking/Scripts/4_World/layouts/DC_BankingMenu.layout");
        m_ButtonDeposit = ButtonWidget.Cast(layoutRoot.FindAnyWidget("ButtonDeposit"));
		m_ButtonWithdraw = ButtonWidget.Cast(layoutRoot.FindAnyWidget("ButtonWithdraw"));
		m_ButtonClose = ButtonWidget.Cast(layoutRoot.FindAnyWidget("ButtonClose"));
		m_TextMaxOwnedAmount = TextWidget.Cast(layoutRoot.FindAnyWidget("TextMaxOwnedAmount"));
		m_TextOwnedAmount = TextWidget.Cast(layoutRoot.FindAnyWidget("TextOwnedAmount"));
		m_TextOnPlayerAmount = TextWidget.Cast(layoutRoot.FindAnyWidget("TextOnPlayerAmount"));
		m_PanelAmountInput = Widget.Cast(layoutRoot.FindAnyWidget("PanelAmountInput"));
		m_TextAmountInput = TextWidget.Cast(layoutRoot.FindAnyWidget("TextAmountInput"));
		m_EditBoxAmount = EditBoxWidget.Cast(layoutRoot.FindAnyWidget("EditBoxAmountInput"));
		m_MultilineTextMessage = MultilineTextWidget.Cast(layoutRoot.FindAnyWidget("MultilineTextMessage"));
		m_PanelMessage = Widget.Cast(layoutRoot.FindAnyWidget("PanelMessage"));
		
		m_TextOwnedAmount.SetText("0");
		m_TextMaxOwnedAmount.SetText("0");
		m_TextOnPlayerAmount.SetText("0");
		m_TextAmountInput.SetText("0");
		m_EditBoxAmount.SetText("0");
		layoutRoot.Show(false);
		
        return layoutRoot;
    }
	
	override void OnShow()
	{
		super.OnShow();
		
        GetGame().GetMission().PlayerControlDisable(INPUT_EXCLUDE_INVENTORY);
        GetGame().GetUIManager().ShowCursor(true);
        GetGame().GetInput().ChangeGameFocus(1);
		
		m_TextAmountInput.SetText("0");
		m_EditBoxAmount.SetText("0");
		m_PanelMessage.Show(false);
		
		RequestPlayerData();
	}
	
	override void OnHide()
	{
		super.OnHide();

        GetGame().GetUIManager().ShowCursor(false);
        GetGame().GetInput().ResetGameFocus();
        GetGame().GetMission().PlayerControlEnable(true);
		Close();
	}
	
	override void Update(float timeslice)
	{
		super.Update(timeslice);
		
		if(m_MultilineTextMessage.IsVisible())
		{
			if(m_MessageTimer > 0)
			{
				m_MessageTimer -= timeslice;
			}
			else
			{
				m_MultilineTextMessage.SetText("");
				m_PanelMessage.Show(false);
			}
		}
		
		if(m_TransactionDelayTimer > 0)
		{
			m_TransactionDelayTimer -= timeslice;
		}
	}
	
	override bool OnClick( Widget w, int x, int y, int button )
	{
		super.OnClick(w, x, y, button);
	
		switch(w)
		{
			case m_ButtonDeposit:
				RequestDeposit(m_EditBoxAmount.GetText().ToInt());
				return true;
				break;
			
			case m_ButtonWithdraw:
				RequestWithdraw(m_EditBoxAmount.GetText().ToInt());
				return true;
				break;
			
			case m_ButtonClose:
				GetGame().GetUIManager().HideScriptedMenu(this);
				return true;
				break;
		}
		return false;
	}
	
	override bool OnDoubleClick( Widget w, int x, int y, int button )
	{
		super.OnDoubleClick(w, x, y, button);
	
		switch(w)
		{
			case m_PanelAmountInput:
				m_TextAmountInput.SetText("");
				m_EditBoxAmount.SetText("");
				return true;
				break;
			case m_ButtonDeposit:
				if(m_EditBoxAmount.GetText() == "0")
				{
					RequestDeposit(-1);
				}
				return true;
				break;
			
			case m_ButtonWithdraw:
				if(m_EditBoxAmount.GetText() == "0")
				{
					RequestWithdraw(-1);
				}
				return true;
				break;
		}
		return false;
	}
	
	override bool OnMouseEnter( Widget w, int x, int y )
	{
		super.OnMouseEnter(w, x, y);
		
		switch(w)
		{
			case m_PanelAmountInput:
				if(m_EditBoxAmount.GetText() == "0")
				{
					m_TextAmountInput.SetText("");
					m_EditBoxAmount.SetText("");
				}
	        	SetFocus(m_EditBoxAmount);
				return true;
				break;
		}
		return false;
	}
	
	override bool OnMouseLeave( Widget w, Widget enterW, int x, int y )
	{
		super.OnMouseLeave(w, enterW, x, y);
		
		switch(w)
		{
			case m_PanelAmountInput:
	        	SetFocus(layoutRoot);
				if(m_EditBoxAmount.GetText() == "")
				{
					m_TextAmountInput.SetText("0");
					m_EditBoxAmount.SetText("0");
				}
				return true;
				break;
		}
		return false;
	}
	
	override bool OnChange(Widget w, int x, int y, bool finished)
	{
		super.OnChange(w,x,y,finished);
		
		switch(w)
		{
			case m_EditBoxAmount:
				string onlyNumbers = ConstrainToPositiveNumber(m_EditBoxAmount.GetText());
				m_TextAmountInput.SetText(onlyNumbers);
        		m_EditBoxAmount.SetText(onlyNumbers);
            	return true;
				break;
        }
        return false;
	}
	
	
	protected void RequestPlayerData()
    {
		GetRPCManager().SendRPC("DC_Banking", "PlayerDataRequest", null, true);
		m_IsWaitingOnServerResponse = true;
    }
	
	protected void RequestDeposit(int amount)
    {
		if(amount != 0)
		{
			if(m_TransactionDelayTimer <= 0)
			{
				if(!m_IsWaitingOnServerResponse)
				{
					GetRPCManager().SendRPC("DC_Banking", "DepositRequest", new Param1<int>(amount), true);
					m_TransactionDelayTimer = m_TransactionDelayTime;
					m_IsWaitingOnServerResponse = true;
				}
				else
				{
					ShowMessage(-2);
				}
			}
			else
			{
				ShowMessage(-1);
			}
		}		
    }
	
	protected void RequestWithdraw(int amount)
    {
		if(amount != 0)
		{
			if(m_TransactionDelayTimer <= 0)
			{
				if(!m_IsWaitingOnServerResponse)
				{
					GetRPCManager().SendRPC("DC_Banking", "WithdrawRequest", new Param1<int>(amount), true);
					m_TransactionDelayTimer = m_TransactionDelayTime;
					m_IsWaitingOnServerResponse = true;
				}
				else
				{
					ShowMessage(-2);
				}
			}
			else
			{
				ShowMessage(-1);
			}
		}
    }
	
	protected void SetMessage(string message)
	{
		m_MultilineTextMessage.SetText(message);
		m_PanelMessage.Show(true);
		m_MessageTimer = m_MessageTime;
	}
	
	protected void ShowMessage(int messageCode)
	{
		switch(messageCode)
			{
				case -2:
					SetMessage("You are currently waiting on a server response of a previous transaction.");
				break;
				
				case -1:
					SetMessage("You have to wait " + m_TransactionDelayTime + "s between transaction requests. Please wait another " + m_TransactionDelayTimer + "s.");
				break;
				
				case 1:
					SetMessage("You do not have enough space to carry everything and some currency has been placed on the ground.");
				break;
				
				case 2:
					SetMessage("You do not have enough space to carry everything and some currency has not been added to your inventory.");
				break;
				
				case 3:
					SetMessage("You do not have enough space to carry all of the exchanged currency and some has been placed on the ground.");
				break;
				
				case 4:
					SetMessage("You do not have enough space to carry all of the exchanged currency and some has been added to your account.");
				break;
			}
	}
	
	protected string ConstrainToPositiveNumber(string input)
    {
		string onlyNumbers = "";
		for(int i = 0; i<input.Length(); i++)
		{
			if(ContainsNumber(input.Get(i)))
			{
				onlyNumbers += input.Get(i);
			}
		}
		return onlyNumbers;
    }
	
	protected bool ContainsNumber(string input)
    {
		if(input.Contains("0"))
		{
			return true;
		}
		if(input.Contains("1"))
		{
			return true;
		}
		if(input.Contains("2"))
		{
			return true;
		}
		if(input.Contains("3"))
		{
			return true;
		}
		if(input.Contains("4"))
		{
			return true;
		}
		if(input.Contains("5"))
		{
			return true;
		}
		if(input.Contains("6"))
		{
			return true;
		}
		if(input.Contains("7"))
		{
			return true;
		}
		if(input.Contains("8"))
		{
			return true;
		}
		if(input.Contains("9"))
		{
			return true;
		}
		return false;
    }
}