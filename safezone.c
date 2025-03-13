class SafeZone_PlugIn
{
	protected bool Activate_SafeZone_PlugIn = true; // safezone on (true) or off (false)
	protected float SAFEZONE_time_repeat_checking = 10; //In seconds
	
	// "6307.81 2000 9511.72"
	ref static TStringArray SAFEZONE_LOCATIONS = {"6974.876465 388.432281 11345.492188"};//Map coords (positions of the safe zone)
	protected static float  SAFEZONE_RADIUS   = 800; //In meter
	protected static string SAFEZONE_ENTRY_MESSAGE     = "You are in the safezone you canot be harmed";
	protected static string SAFEZONE_EXIT_MESSAGE      = "You left the safezone you can now be harmed";
		
	
	void OnInit()
	{
		if(Activate_SafeZone_PlugIn)
		{
			GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater(RunCheckStart, (SAFEZONE_time_repeat_checking * 1000), true); 
		}
	}
	
	static void RunCheckStart()
	{
		private array<Man> players = new array<Man>;
		GetGame().GetPlayers( players );
		if (SAFEZONE_LOCATIONS.Count() > 0)
		{
			if (players.Count() > 0)
			{
				foreach(string SAFEZONE_LOCATION: SAFEZONE_LOCATIONS)
				{
					if( players.Count() > 0 )
					{
						foreach(Man player: players)
						{
							if(player)
							{
								private PlayerBase player_casted = PlayerBase.Cast(player);
								CheckingPosition(player_casted,SAFEZONE_LOCATION.ToVector());
							}
						}
				   }
			   }	
		   }
		}
	}

	static void CheckingPosition(PlayerBase player,vector SAFEZONE_LOCATION)
	{
		private float SAFEZONE_distance;
		private string SAFEZONE_ZoneCheck, SAFEZONE_UID_PLAYER, SAFEZONE_NAME_PLAYER;
		SAFEZONE_NAME_PLAYER = player.GetIdentity().GetName(); 
		SAFEZONE_UID_PLAYER = player.GetIdentity().GetPlainId(); //Steam 64	

		private vector SAFEZONE_pos_player = player.GetPosition();
		private vector SAFEZONE_LOCATION_FIXED = CorrectToGroundPosY(SAFEZONE_LOCATION);
		private string name_mesage_profile = "GodModeEnabledFor: " + SAFEZONE_UID_PLAYER + " Location: " + SAFEZONE_LOCATION.ToString();
		SAFEZONE_distance = vector.Distance(SAFEZONE_pos_player,SAFEZONE_LOCATION_FIXED);
		if (SAFEZONE_distance <= SAFEZONE_RADIUS) //Player Inside Zone
		{
			SAFEZONE_ZoneCheck = "";
			GetGame().GetProfileString(name_mesage_profile,SAFEZONE_ZoneCheck);
			if (SAFEZONE_ZoneCheck == "true") //Already in zone
			{
				GetGame().SetProfileString(name_mesage_profile,"true");
				player.SetAllowDamage(false); //GodMode On
				return;
			}
			else
			{
				GetGame().SetProfileString(name_mesage_profile,"true");
				Print("[SafeZone] " + SAFEZONE_NAME_PLAYER + " (" + SAFEZONE_UID_PLAYER + ") Enter in safeZone in position: " + SAFEZONE_LOCATION_FIXED.ToString());
				player.SetAllowDamage(false); //GodMode On
				SendPersonalMessage(SAFEZONE_ENTRY_MESSAGE, player);
			}
		} else 
		{
			 //Player Outside of Zone
			if (SAFEZONE_distance > SAFEZONE_RADIUS) //Player Outside of Zone
			{
				SAFEZONE_ZoneCheck = "";
				GetGame().GetProfileString(name_mesage_profile,SAFEZONE_ZoneCheck);
				if (SAFEZONE_ZoneCheck == "false")
				{
					GetGame().SetProfileString(name_mesage_profile,"false");
					return;
				}
				else
				{
					if (SAFEZONE_ZoneCheck != "")
					{
						GetGame().SetProfileString(name_mesage_profile,"false");
						Print("[SafeZone] " + SAFEZONE_NAME_PLAYER + " (" + SAFEZONE_UID_PLAYER + ") Left safeZone in position: " + SAFEZONE_LOCATION_FIXED.ToString());
						player.SetAllowDamage(true); //GodMode Off
						SendPersonalMessage(SAFEZONE_EXIT_MESSAGE, player);
					} else
					{
						GetGame().SetProfileString(name_mesage_profile,"false");
						return;
					}
				}				
			}
		}
	}

	static vector CorrectToGroundPosY(vector pos)
	{
		private float pos_x = pos[0];
		private float pos_z = pos[2];
		private float pos_y = GetGame().SurfaceY(pos_x, pos_z);
		private vector tmp_pos = Vector(pos_x, pos_y, pos_z);
		return tmp_pos;
	}
	static void SendPersonalMessage(string message, PlayerBase casted_player) 
	{
		Man player; 
		Class.CastTo(player, casted_player);
		if(( player ) && (message != ""))
		{
			Param1<string> m_GlobalMessage = new Param1<string>(message); 
			GetGame().RPCSingleParam(player, ERPCs.RPC_USER_ACTION_MESSAGE, m_GlobalMessage, true, player.GetIdentity()); 
		}
	}
}
ref SafeZone_PlugIn SafeZone = new SafeZone_PlugIn();
modded class CustomMission
{	
	override void OnInit () 
    {
		super.OnInit();
		SafeZone.OnInit();		
    }
}