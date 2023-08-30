### How To:
* Download [latest release](https://github.com/genchsusu/Dota2Patcher/releases/latest);

## [gameinfo.gi CRC file check](https://www.reddit.com/r/DotA2/comments/s46mj8/dota_2_update_for_11422_11522_utc) bypass
Allows you to use vpk mods

### How To:
* Drop desired vpk to your mods folder inside `\dota 2 beta\game\` (ex: `\dota 2 beta\game\dota_tempcontent`);
* Open `\dota 2 beta\game\dota\gameinfo.gi` with text editor;
* Change line 39 to `Game dota_tempcontent`
* And line 40 to `Mod dota_tempcontent`

gameinfo.gi should look like this:
```
		SearchPaths
		{
			// These are optional language paths. They must be mounted first, which is why there are first in the list.
			// *LANGUAGE* will be replaced with the actual language name. If not running a specific language, these paths will not be mounted
			Game_Language		dota_*LANGUAGE*
			
			Game				auto_accept
			Mod					auto_accept
...
```
