void main()
{
	//INIT ECONOMY--------------------------------------
	Hive ce = CreateHive();
	if ( ce )
		ce.InitOffline();


	//CEApi TestHive = GetCEApi();
    //GetCEApi().ExportProxyProto();
    //GetCEApi().ExportProxyData( "10240 0 10240", 20480 );
    //GetCEApi().ExportProxyData(vector.Zero, 100000); //Loot
	//GetCEApi().ExportClusterData() ;
   // DeleteAllItemsInMap();

	// /*
    //   LXD Sahrani Weather System
    //    WARNING!!!!: DO NOT ALTER following values as they are interconnected with other Sahrani-specific systems! It will F it all up. Its a delicate ecosystem I tell thee.
    //	  Also cheers to Dab and Sumrak <3
    // */
   	Weather weather = GetGame().GetWeather();
    weather.MissionWeather( true );
    weather.GetOvercast().SetLimits( 0.0, 0.35 );
    weather.GetRain().SetLimits( 0.0, 0.0 );
    weather.GetFog().SetLimits( 0.0, 0.2 );
    weather.GetOvercast().SetForecastChangeLimits( 0.2, 0.35 );
    weather.GetOvercast().SetForecastTimeLimits( 1600, 2100 );
    weather.GetOvercast().Set( Math.RandomFloatInclusive( 0.0, 0.2 ), 0, 0 );   // ignored if storage is present
    weather.GetRain().Set( 0, 0, 0 );                                           // ignored if storage is present
    weather.GetFog().Set( 0, 0, 0 );                                            // ignored if storage is present
    weather.SetWindMaximumSpeed( 15 );
    weather.SetWindFunctionParams( 0.1, 0.3, 42 );
    weather.SetStorm( 0, 1, 1 );
    weather.SetRainThresholds( 0.0, 1.0, 0 );

	//DATE RESET AFTER ECONOMY INIT-------------------------
	int year, month, day, hour, minute;
	int reset_month = 8, reset_day = 1;
	GetGame().GetWorld().GetDate(year, month, day, hour, minute);

	if ((month == reset_month) && (day < reset_day))
	{
		GetGame().GetWorld().SetDate(year, reset_month, reset_day, hour, minute);
	}
	else
	{
		if ((month == reset_month + 1) && (day > reset_day))
		{
			GetGame().GetWorld().SetDate(year, reset_month, reset_day, hour, minute);
		}
		else
		{
			if ((month < reset_month) || (month > reset_month + 1))
			{
				GetGame().GetWorld().SetDate(year, reset_month, reset_day, hour, minute);
			}
		}
	}
}

class CustomMission: MissionServer
{
	void SetRandomHealth(EntityAI itemEnt)
	{
		if ( itemEnt )
		{
			float rndHlt = Math.RandomFloat( 0.45, 0.65 );
			itemEnt.SetHealth01( "", "", rndHlt );
		}
	}

	override void OnInit()
	{
		super.OnInit();

		// Sahrani Events
		if ( m_EventManager )
		{
			// min time between events, max time between events, max number of events at the same time - 10 mins and 20 mins atm
		 	m_EventManager.Run( 600, 1200, 1 );
			// registering events and their probability
			m_EventManager.RegisterEvent( LXDTropicalStorm, 0.15 );
			m_EventManager.RegisterEvent( LXDRainShower, 0.08 );
			m_EventManager.RegisterEvent( LXDCloudySkies, 0.18 );
			m_EventManager.RegisterEvent( LXDClearSkies, 0.68 );
		}
	}

	override PlayerBase CreateCharacter(PlayerIdentity identity, vector pos, ParamsReadContext ctx, string characterName)
	{
		Entity playerEnt;
		playerEnt = GetGame().CreatePlayer( identity, characterName, pos, 0, "NONE" );
		Class.CastTo( m_player, playerEnt );

		GetGame().SelectPlayer( identity, m_player );

		return m_player;
	}

	override void StartingEquipSetup(PlayerBase player, bool clothesChosen)
	{
		EntityAI itemClothing;
		EntityAI itemEnt;
		ItemBase itemBs;
		float rand;

		itemClothing = player.FindAttachmentBySlotName( "Body" );
		if ( itemClothing )
		{
			SetRandomHealth( itemClothing );
			
			itemEnt = itemClothing.GetInventory().CreateInInventory( "BandageDressing" );
			if ( Class.CastTo( itemBs, itemEnt ) )
				itemBs.SetQuantity( 4 );

			string chemlightArray[] = { "Chemlight_White", "Chemlight_Yellow", "Chemlight_Green", "Chemlight_Red" };
			int rndIndex = Math.RandomInt( 0, 4 );
			itemEnt = itemClothing.GetInventory().CreateInInventory( chemlightArray[rndIndex] );
			SetRandomHealth( itemEnt );

			rand = Math.RandomFloatInclusive( 0.0, 1.0 );
			if ( rand < 0.35 )
				itemEnt = player.GetInventory().CreateInInventory( "Apple" );
			else if ( rand > 0.65 )
				itemEnt = player.GetInventory().CreateInInventory( "Pear" );
			else
				itemEnt = player.GetInventory().CreateInInventory( "Plum" );

			SetRandomHealth( itemEnt );
		}
		
		itemClothing = player.FindAttachmentBySlotName( "Legs" );
		if ( itemClothing )
			SetRandomHealth( itemClothing );
		
		itemClothing = player.FindAttachmentBySlotName( "Feet" );
	}
};

Mission CreateCustomMission(string path)
{
	return new CustomMission();
}