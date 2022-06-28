# DayZ-Banking

![An image of the ATMs in the Banking mod](https://github.com/DennisVidal/DayZ-Banking/blob/main/DayZ_Mod_Banking.png)

## What does this mod do?
This mod adds a simple banking system to DayZ. More specifically it adds ATMs that can be placed around the map. Through these ATMs players can deposit and withdraw the configured currency. That way they don't have to carry it on them at all times or stash it in their base. The mod can be found [here](https://steamcommunity.com/workshop/filedetails/?id=1836257061 "Steam workshop page of Banking").

This mod requires [CF](https://steamcommunity.com/workshop/filedetails/?id=1559212036 "Steam workshop page of CF") to be loaded as well. The currency items themselves aren't part of this mod. Instead, the currency of the [Trader mod by Dr_J0nes](https://steamcommunity.com/sharedfiles/filedetails/?id=1590841260) is used by default as it is widely used. Should you use a different mod you can adjust the currency easily in the config.

---
## How do I set up this mod?
### Clients
1. Download the mod from the [workshop page](https://steamcommunity.com/workshop/filedetails/?id=1836257061 "Steam workshop page of Banking")
2. Add the mod, [CF](https://steamcommunity.com/workshop/filedetails/?id=1559212036 "Steam workshop page of CF") and any other mods you need to your `-mod` launch parameter
3. Start your game and join your server

Alternatively, it's probably easier to just use either DayZ's launcher or the [DayZSA launcher](https://dayzsalauncher.com/ "Website of the DayZSA launcher") to join a server.

### Server
1. Download the mod from the [workshop page](https://steamcommunity.com/workshop/filedetails/?id=1836257061 "Steam workshop page of Banking")
2. Add the mod, [CF](https://steamcommunity.com/workshop/filedetails/?id=1559212036 "Steam workshop page of CF") and any other mods you need to the `-mod` launch parameter of your server
3. Start your server to generate the config file in your server's profile folder
4. Adjust the values in the config to your liking
5. Start your server again

---
## How do I spawn the ATMs?
There are many ways to spawn the ATMs in. Any way that lets you spawn persistent items or rather buildings should work. The easiest way in my opinion though is to add a function to the init file of your server's mission and spawn the ATMs using that function. If you choose to use this way you can follow the following steps:
1. Open the `init.c` file in your server's mission folder
2. Add the following above the `main` function: 
```cs
void SpawnObject(string type, vector position, vector orientation)
{
    Object obj = GetGame().CreateObject(type, position);
    if(!obj)
      return;
    obj.SetPosition(position);
    obj.SetOrientation(orientation);
    obj.SetOrientation(obj.GetOrientation());
    obj.Update();
    obj.SetAffectPathgraph(true, false);
    if(obj.CanAffectPathgraph()) 
      GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(GetGame().UpdatePathgraphRegionByObject, 100, false, obj);
}
```
3. Spawn the ATMs using the above function in `main` like this:
```cs
void main()
{
  SpawnObject("DC_BankingATM", "3706.7 402.012 5984.86", "90 0 0"); //Green Mountain ATM 1
  SpawnObject("DC_BankingATM", "3704.7 402.332 6003.1", "275 0 0"); //Green Mountain ATM 2

  SpawnObject("DC_BankingATM", "8360.15 292.054 5994.15", "325 0 0"); //Kumyrna ATM 1
  SpawnObject("DC_BankingATM", "8350.7 292.011 5978.3", "235 0 0"); //Kumyrna ATM 2
   
   //...
```
The `SpawnObject` function takes three parameters: a `class name`, a `position` and an `orientation`.

The `class name` of the ATM can be one of the following depending on the variant that you want to spawn: 
<table>
  <tbody>
    <td><b>"DC_BankingLocker"</b></td>
    <td><b>"DC_BankingATM"</b></td>
    <td><b>"DC_BankingATMRed"</b></td>
    <td><b>"DC_BankingATMBlue"</b></td>
    <td><b>"DC_BankingATMDarkGreen"</b></td>
    <td><b>"DC_BankingATMDarkBlue"</b></td>
    <td><b>"DC_BankingATMOrange"</b></td>
    <td><b>"DC_BankingATMYellow"</b></td>
    <td><b>"DC_BankingATMPurple"</b></td>
    <td><b>"DC_BankingATMWhite"</b></td>
    <td><b>"DC_BankingATMGray"</b></td>
  </tbody>
</table>

The `position` parameter is just a vector denoting the world-space position of the ATM. Note that vectors in Enforce are represented by a string in which each component is separated by a space.

The last parameter `orientation` is the rotation of the object. It is another vector in which each component is the angle in degrees around the corresponding axis. 

The easiest approach to find both the position and orientation for an ATM is probably to use some admin tool to get those values from a fitting location.

A more in-depth description of how to spawn objects in general using this method can be found [here](https://github.com/Arkensor/DayZCommunityOfflineMode/wiki/Add-custom-objects-to-your-server-or-mission "Adding custom objects section of the Community Offline Mode").

---
## How do I configure this mod?
### Config
After the config file is generated, you can navigate to the profile folder of your server to find the config. If you haven't set a profile folder for your server yet, you might want to consider doing so with the `-profiles` launch parameter, as it makes it easier to find the config and any log files in general. 
In the profile folder you should find a folder labeled `DC_Banking`. This folder contains both the config and the player data for this mod. The config itself is a normal Json file and can therefore be edited rather easily. The following table explains its contents:

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
      <td>"m_DefaultMaxCurrency":&nbsp;10000</td>
      <td>
        The actual limit might vary individually based on the set bonus. 
        <br>
        <br>
        The upper limit is Int.MAX (2,147,483,647), although it should be chosen a lot lower.
      </td>
    </tr>
    <tr>
      <td><b>m_StartCurrency</b></td>
      <td>The amount of curreny players start with in their account.</td>
      <td>"m_StartCurrency":&nbsp;500</td>
      <td>This only applies to player files created after this value has been changed.</td>
    </tr>
    <tr>
      <td><b>m_PlaceExcessiveCurrencyOnGround</b></td>
      <td>Decides whether currency that doesn't fit into the inventory of the player should be placed on the ground beneath them or not.</td>
      <td>"m_PlaceExcessiveCurrencyOnGround":&nbsp;0</td>
      <td>
        Setting it to 0 will place excessive currency in the banking account of the player, which might lead to them storing more than the upper limit in their account.
        <br>
        <br>
        Setting it to 1 will place any excessive curreny on the ground, which can potentially lead to a large number of currency stacks being spawned depending on how the currency is set up.
      </td>
    </tr>
    <tr>
      <td><b>m_CreateLogs</b></td>
      <td>Decides whether logs should be created in the script*.log file.</td>
      <td>"m_CreateLogs":&nbsp;1</td>
      <td></td>
    </tr>
    <tr>
      <td><b>m_MessageDisplayTime</b></td>
      <td>The time in seconds that messages will be displayed for.</td>
      <td>"m_MessageDisplayTime":&nbsp;5.0</td>
      <td></td>
    </tr>
    <tr>
      <td><b>m_TransactionDelayTime</b></td>
      <td>The time in seconds that players will need to wait between transactions.</td>
      <td>"m_TransactionDelayTime":&nbsp;1.5</td>
      <td>Lower times allow players to spam transactions in quick succession, which could cause issues when set to a very low value.</td>
    </tr>
    <tr>
      <td><b>m_BankingCurrencyArray</b></td>
      <td>An array that contains the data of the accepted currency.</td>
      <td>
      "m_BankingCurrencyArray":&nbsp;[<br>
        &emsp;{<br>
        &emsp;&emsp;"m_ClassName":&nbsp;"MoneyRuble100",<br>
        &emsp;&emsp;"m_Value":&nbsp;100<br>
        &emsp;},<br>
        &emsp;{<br>
        &emsp;&emsp;"m_ClassName":&nbsp;"MoneyRuble50",<br>
        &emsp;&emsp;"m_Value":&nbsp;50<br>
        &emsp;}<br>
      ]
      </td>
      <td>Each entry consists of a block of <b><i>m_ClassName</i></b> and <b><i>m_Value</i></b>. Blocks are separated by commas.</td>
    </tr>
    <tr>
      <td><b>m_ClassName</b></td>
      <td>The class name of the currency item.</td>
      <td>"m_ClassName":&nbsp;"MoneyRuble25"</td>
      <td></td>
    </tr>
    <tr>
      <td><b>m_Value</b></td>
      <td>The value of a currency item.</td>
      <td>"m_Value":&nbsp;25</td>
      <td></td>
    </tr>
  </tbody>
</table>

### Player Data
Similarly to the config the player data is also stored in the `DC_Banking` folder, more specifically in a subfolder called `PlayerDatabase`. Each file in this folder is named after the steamID64 of the corresponding player.

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
      <td>The steamID64 of the player.</td>
      <td>"m_PlainID":&nbsp;"76561198048233208"</td>
      <td>This doesn't need to be edited manually as it gets set with the creation of the file.</td>
    </tr>
    <tr>
      <td><b>m_Username</b></td>
      <td>The name used when the player last interacted with an ATM.</td>
      <td>"m_Username":&nbsp;"Survivor"</td>
      <td>This doesn't need to be edited manually as it gets updated every time the player interacts with an ATM.</td>
    </tr> 
    <tr>
      <td><b>m_OwnedCurrency</b></td>
      <td>The amount of currency the player currently has in their account.</td>
      <td>"m_OwnedCurrency":&nbsp;5622</td>
      <td></td>
    </tr>
    <tr>
      <td><b>m_MaxOwnedCurrencyBonus</b></td>
      <td>An individual bonus on top of <b><i>m_DefaultMaxCurrency</i></b> for the maximum currency limit.</td>
      <td>"m_MaxOwnedCurrencyBonus":&nbsp;4000</td>
      <td>This is intended to be used as a reward in combination with events or anything of that sort.</td>
    </tr>
  </tbody>
</table>


## I'm having some sort of issue with this mod, what can I do?

The cause of most issues is some simple syntax error in the config file. Therefore the first thing you should try is running the config through some Json validator like [this](https://jsonformatter.curiousconcept.com/) one. 

If there are no errors in your config check that you didn't set a float value instead of an integer one for certain variables in the config, e.g. setting the value of some currency to 1.0 instead of just 1.

Similarly setting the currency limit to anything close to the maximum representable integer value (2,147,483,647) will likely lead to problems as well.

In case none of the above is the case, feel free to add me on either Discord(Deadcraft#8378) or [Steam](https://steamcommunity.com/id/Deadcraft "Steam profile of Dennis Vidal") and I'll try to assist you with the problem.
