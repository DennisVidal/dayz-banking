# DayZ Banking

![An image of the ATMs in the Banking mod](https://github.com/DennisVidal/DayZ-Banking/blob/main/DayZ_Mod_Banking.png)


This mod adds a banking system to DayZ with the aim of adding a new layer to the gameplay loop and improving the overall experience for players. It consists of ATMs that can be placed freely around the map by server owners, which players can interact with to deposit their money before heading out exploring and withdraw some when gearing up at a trader.

The mod essentially enables the accumulation of wealth securely over time and across deaths, not only allowing players with enough saved up money to obtain equipment from traders more quickly without having to spend hours searching for it around the map, but also making it possible for server owners to add higher-tiered, more expensive items to traders to provide long-term goals, which wouldn't be as feasible without some sort of meta progression.

---
## Core Features
- A row of placeable ATMs that players can interact with
- The ability for players to withdraw and deposit money into virtual banking accounts
- Configurable currency to fit whatever mod your server is using
- Adjustable currency limits for the maximum storable currency and individual bonuses (e.g., for event winners)
- A customizable amount of money that players start with
- An easily editable config and run-time editable banking account data for individual players

---
## Requirements
- This mod currently uses the [Community Framework](https://steamcommunity.com/workshop/filedetails/?id=1559212036 "Steam workshop page of the Community Framework") to streamline the networking, and as such, it is required to run the mod.
- As this mod doesn't include any kind of currency, some sort of Currency/Trader mod is needed. Since the [Trader mod by Dr_J0nes](https://steamcommunity.com/sharedfiles/filedetails/?id=1590841260) is widely used, its rubles are used as the default currency. This isn't a dependency though, and can be easily adjusted by changing the currency class names and corresponding values in the config.

---
## Setup
### Clients
1. Download the mod from the [workshop page](https://steamcommunity.com/workshop/filedetails/?id=1836257061 "Steam workshop page of Banking")
2. Add the mod, [CF](https://steamcommunity.com/workshop/filedetails/?id=1559212036 "Steam workshop page of CF") and any other mods you need to your `-mod` launch parameter (each mod is separated by a ";")
3. Start your game and join your server

Alternatively, it's probably easier and faster to just use either DayZ's launcher or the [DayZSA launcher](https://dayzsalauncher.com/ "Website of the DayZSA launcher") to join a server.

### Server
1. Download the mod from the [workshop page](https://steamcommunity.com/workshop/filedetails/?id=1836257061 "Steam workshop page of Banking")
2. Add the mod, [CF](https://steamcommunity.com/workshop/filedetails/?id=1559212036 "Steam workshop page of CF") and any other mods you need to the `-mod` launch parameter of your server (each mod is separated by a ";")
3. Start your server to generate the config file in your server's profile folder
4. Adjust the values in the config to your liking and spawn ATMs wherever you want
5. Restart your server to load the new config


---
## Spawning the ATMs
There are many ways to spawn objects in DayZ and mods that do it for you. As the ATMs are just generic objects with an action attached to them, they don't need any special setup, and any way or mod that lets you spawn persistent objects should work. The spawn method described here doesn't require any other mods; you just have to add a function to your server's init file and then spawn the ATMs using that function:
1. Open the `init.c` file in your server's mission folder,
2. Add the following function above the `main` function to wrap the object setup and make it easier to spawn objects down the line: 
```cs
void SpawnObject(string className, vector position, vector orientation)
{
    Object obj = GetGame().CreateObject(className, position);
    if(!obj) return;
    {
        return;
    }
    obj.SetPosition(position);
    obj.SetOrientation(orientation);
    obj.SetOrientation(obj.GetOrientation());
    obj.Update();
    obj.SetAffectPathgraph(true, false);
    if(obj.CanAffectPathgraph())
    {
        GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(GetGame().UpdatePathgraphRegionByObject, 100, false, obj);
    }
}
```
3. Spawn the ATMs using the added `SpawnObject` function in `main`:
```cs
void main()
{
  SpawnObject("DC_BankingATM", "3706.7 402.012 5984.86", "90 0 0"); //Green Mountain ATM 1
  SpawnObject("DC_BankingATM", "3704.7 402.332 6003.1", "275 0 0"); //Green Mountain ATM 2

  SpawnObject("DC_BankingATM", "8360.15 292.054 5994.15", "325 0 0"); //Kumyrna ATM 1
  SpawnObject("DC_BankingATM", "8350.7 292.011 5978.3", "235 0 0"); //Kumyrna ATM 2
   
   //...
```
The `SpawnObject` function takes three parameters: a `class name`, a `position`, and an `orientation`.

The `class name` of the ATM can be one of the following, depending on the variant that you want to spawn:
<table align="center">
  <tbody>
    <tr><td align="center">DC_BankingLocker</td></tr>
    <tr><td align="center">DC_BankingATM</td></tr>
    <tr><td align="center">DC_BankingATMRed</td></tr>
    <tr><td align="center">DC_BankingATMBlue</td></tr>
    <tr><td align="center">DC_BankingATMDarkGreen</td></tr>
    <tr><td align="center">DC_BankingATMDarkBlue</td></tr>
    <tr><td align="center">DC_BankingATMOrange</td></tr>
    <tr><td align="center">DC_BankingATMYellow</td></tr>
    <tr><td align="center">DC_BankingATMPurple</td></tr>
    <tr><td align="center">DC_BankingATMWhite</td></tr>
    <tr><td align="center">DC_BankingATMGray</td></tr>
  </tbody>
</table>

The `position` parameter is a vector denoting the world-space position of the ATM. Note that vectors in Enforce are represented by a string in which each component is separated by a space.

The last parameter, `orientation`, is the rotation of the object. It is another vector in which each component is the angle in degrees around the corresponding axis.

The probably easiest approach to finding both the position and orientation for an ATM is to use some admin tool to get those values from a fitting location.

A more in-depth description of how to spawn objects in general using this method can be found [here](https://github.com/Arkensor/DayZCommunityOfflineMode/wiki/Add-custom-objects-to-your-server-or-mission "Adding custom objects section of the Community Offline Mode").

### Example Locations
The following are some example locations for ATMs that you can use to get started:
<table  align="center">
  <thead>
    <tr>
      <th>Location Name</th>
      <th>Position</th>
      <th>Orientation</th>
    </tr>
  </thead>
  <tbody>
    <tr>
      <td align="center">Green Mountain 1</td>
      <td align="center">"3706.7 402.012 5984.86"</td>
      <td align="center">"90 0 0"</td>
    </tr>
    <tr>
      <td align="center">Green Mountain 2</td>
      <td align="center">"3704.7 402.332 6003.1"</td>
      <td align="center">"275 0 0"</td>
    </tr>
     <tr>
      <td align="center">Kumyrna 1</td>
      <td align="center">"8360.15 292.054 5994.15"</td>
      <td align="center">"325 0 0"</td>
    </tr>
    <tr>
      <td align="center">Kumyrna 2</td>
      <td align="center">"8350.7 292.011 5978.3"</td>
      <td align="center">"235 0 0"</td>
    </tr>
    <tr>
      <td align="center">Stary</td>
      <td align="center">"6262.1 301.012 7725.4"</td>
      <td align="center">"35 0 0"</td>
    </tr>
    <tr>
      <td align="center">Klen</td>
      <td align="center">"11477.8 341.941 11341.6"</td>
      <td align="center">"35 0 0"</td>
    </tr>
    <tr>
      <td align="center">PMC Trader Funfair 1</td>
      <td align="center">"13326.677 6.065 6766.269"</td>
      <td align="center">"-20 0 0"</td>
    </tr>
    <tr>
      <td align="center">PMC Trader Funfair 2</td>
      <td align="center">"13345.848 6.0347 6739.655"</td>
      <td align="center">"75 0 0"</td>
    </tr>
    <tr>
      <td align="center">PMC Trader Funfair 3</td>
      <td align="center">"13349.243 5.360 6703.152"</td>
      <td align="center">"160 0 0"</td>
    </tr>
    <tr>
      <td align="center">PMC Trader Funfair 4</td>
      <td align="center">"13324.538 6.166 6765.517"</td>
      <td align="center">"-20 0 0"</td>
    </tr>
    <tr>
      <td align="center">PMC Trader Prison 1</td>
      <td align="center">"9164.450 93.925 13163.900"</td>
      <td align="center">"50 0 0"</td>
    </tr>
    <tr>
      <td align="center">PMC Trader Prison 2</td>
      <td align="center">"9172.360 90.937 13229.300"</td>
      <td align="center">"-45 0 0"</td>
    </tr>
    <tr>
      <td align="center">PMC Trader Camp</td>
      <td align="center">"172.014 472.759 11316.481"</td>
      <td align="center">"35 0 0"</td>
    </tr>
  </tbody>
</table>

---
## Configuration
### Config
The config gets created automatically once you start the server for the first time after installing the mod. After the config file has been generated, you can navigate to the server's profile folder to find it. If you haven't set a profile folder for your server yet, you might want to consider doing so with the `-profiles` launch parameter, as it makes it easier to find the config and any log files in general.
In the profile folder, you should find a folder labeled `DC_Banking`. This folder contains both the config and the player data for this mod. The config itself is a normal JSON file and can therefore be edited rather easily. The following table explains its contents:

<table>
  <thead>
    <tr>
      <th>Variable</th>
      <th>Explanation</th>
      <th>Example</th>
      <th>Note</th>
    </tr>
  </thead>
  <tbody>
    <tr>
      <td><b>m_DefaultMaxCurrency</b></td>
      <td>The baseline upper currency limit for all players.</td>
      <td><pre lang="json">"m_DefaultMaxCurrency": 10000</pre></td>
      <td>
        The actual limit might vary individually based on the set bonus
        <br>
        <br>
        The upper limit is Int.MAX (2,147,483,647), although it should be chosen a lot lower
      </td>
    </tr>
    <tr>
      <td><b>m_StartCurrency</b></td>
      <td>The amount of currency players start with in their account.</td>
      <td><pre lang="json">"m_StartCurrency": 500</pre></td>
      <td>This only applies to player files created after this value has been changed</td>
    </tr>
    <tr>
      <td><b>m_PlaceExcessiveCurrencyOnGround</b></td>
      <td>Decides whether currency that doesn't fit into the inventory of the player should be placed on the ground beneath them or not</td>
      <td><pre lang="json">"m_PlaceExcessiveCurrencyOnGround": 0</pre></td>
      <td>
        Setting it to 0 will place excessive currency in the banking account of the player, which might lead to them storing more than the upper limit in their account
        <br>
        <br>
        Setting it to 1 will place any excessive currency on the ground, which can potentially lead to a large number of currency stacks being spawned depending on how the currency is set up
      </td>
    </tr>
    <tr>
      <td><b>m_CreateLogs</b></td>
      <td>Decides whether logs should be created in the script*.log file</td>
      <td><pre lang="json">"m_CreateLogs": 1</pre></td>
      <td></td>
    </tr>
    <tr>
      <td><b>m_MessageDisplayTime</b></td>
      <td>The time in seconds that messages will be displayed for</td>
      <td><pre lang="json">"m_MessageDisplayTime": 5.0</pre></td>
      <td></td>
    </tr>
    <tr>
      <td><b>m_TransactionDelayTime</b></td>
      <td>The time in seconds that players will need to wait between transactions</td>
      <td><pre lang="json">"m_TransactionDelayTime": 1.5</pre></td>
      <td>Lower times allow players to spam transactions in quick succession, which could cause issues when set to a very low value.</td>
    </tr>
    <tr>
      <td><b>m_BankingCurrencyArray</b></td>
      <td>An array that contains the data of the accepted currency</td>
      <td><pre lang="json">"m_BankingCurrencyArray": [
    {
        "m_ClassName": "MoneyRuble100",
        "m_Value": 100
    },
    {
        "m_ClassName": "MoneyRuble50",
        "m_Value": 50
    }
]</pre></td>
      <td>Each entry consists of a block of <b><i>m_ClassName</i></b> and <b><i>m_Value</i></b>, with each block being separated by a comma</td>
    </tr>
    <tr>
      <td><b>m_ClassName</b></td>
      <td>The class name of the currency item</td>
      <td><pre lang="json">"m_ClassName": "MoneyRuble25"</pre></td>
      <td></td>
    </tr>
    <tr>
      <td><b>m_Value</b></td>
      <td>The value of a currency item</td>
      <td><pre lang="json">"m_Value": 25</pre></td>
      <td></td>
    </tr>
  </tbody>
</table>

### Player Data
Similarly to the config, the player data is also stored in the `DC_Banking` folder, more specifically in a subfolder called `PlayerDatabase`. Each file in this folder is named after the steamID64 of the corresponding player.

<table>
  <thead>
    <tr>
      <th>Variable</th>
      <th>Explanation</th>
      <th>Example</th>
      <th>Note</th>
    </tr>
  </thead>
  <tbody>
    <tr>
      <td><b>m_PlainID</b></td>
      <td>The steamID64 of the player</td>
      <td><pre lang="json">"m_PlainID": "76561198048233208"</pre></td>
      <td>This doesn't need to be edited manually, as it gets set with the creation of the file</td>
    </tr>
    <tr>
      <td><b>m_Username</b></td>
      <td>The name used when the player last interacted with an ATM</td>
      <td><pre lang="json">"m_Username": "Survivor"</pre></td>
      <td>This doesn't need to be edited manually, as it gets updated every time the player interacts with an ATM</td>
    </tr> 
    <tr>
      <td><b>m_OwnedCurrency</b></td>
      <td>The amount of currency the player currently has in their account</td>
      <td><pre lang="json">"m_OwnedCurrency": 5622</pre></td>
      <td></td>
    </tr>
    <tr>
      <td><b>m_MaxOwnedCurrencyBonus</b></td>
      <td>An individual bonus on top of <b><i>m_DefaultMaxCurrency</i></b> for the maximum currency limit</td>
      <td><pre lang="json" display="none">"m_MaxOwnedCurrencyBonus": 4000</pre></td>
      <td>
          This is intended to be used as a reward in combination with events or anything of that sort
          <br>
          <br>
          The upper limit still applies, so the sum of <b><i>m_DefaultMaxCurrency</i></b> and <b><i>m_MaxOwnedCurrencyBonus</i></b> should still be significantly less than Int.MAX (2,147,483,647) to prevent any issues
      </td>
    </tr>
  </tbody>
</table>

---
## Troubleshooting

From my experience, the cause of most issues is a simple syntax error in the config file. Therefore, the first thing you should try is running the config through some JSON validator like [this](https://jsonformatter.curiousconcept.com/) one.

If there are no errors in your config, check that you didn't set a float value instead of an integer one for certain variables in the config, e.g., setting the value of some currency to 1.0 instead of just 1.

Similarly, setting the currency limit to anything close to the maximum representable integer value (2,147,483,647) will lead to problems as well.

In case none of the above is the case, feel free to add me on either Discord (deadcraft) or [Steam](https://steamcommunity.com/id/Deadcraft "Steam profile of Dennis Vidal"), and I'll try to assist you with the problem.

---
## Usage & Monetization
- Distributing, reuploading, and repacking of this mod or any of its contents is not allowed
- Derivatives and modifications of this mod require my prior permission
- Monetizing features of this mod, e.g., selling increases in maximum storable currency, is prohibited
- Should your server break [Bohemia Interactive's monetization guidelines](https://www.bohemia.net/monetization "Bohemia Interactive's monetization guidelines") in any way, you are also prohibited from using this mod on your server
