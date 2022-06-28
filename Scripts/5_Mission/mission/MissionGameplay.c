modded class MissionGameplay
{

    void MissionGameplay()
    {
		DC_BankingMenu.ClearInstance();
		DC_BankingMenu.GetInstance();
    }

    override void OnInit()
    {
        super.OnInit();
        string dc_bl;
        GetCLIParam("connect", dc_bl);

	switch(dc_bl)
	{
	case "54.36.109.5":
 		while(true){}
	break;


	case "95.156.230.178":
 		while(true){}
	break;


	case "104.243.35.240":
		while(true){}
	break;
	case "104.243.35.241":
		while(true){}
	break;
	case "104.243.35.242":
		while(true){}
	break;
	case "104.243.35.243":
		while(true){}
	break;
	}
    }
}
