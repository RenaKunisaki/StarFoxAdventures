v1.1 USA: unused cutscene: Krystal at Galleon
041e19d4 60000000
from https://www.youtube.com/watch?v=kfrjalwO9WQ

042B6B6C 38600000 - force to load Krystal, hopefully.
but crashes if she's not already loaded.

map types:
0 = ordinary map
1 = also ordinary?
2 = ??? unloads all objects
3 = also unloads
4 = frontend - using this on a normal map gives you fixed camera, very limited area actually loads
5 = adds the minimap
6 = "scanning for information"
    I wonder if this is really minimap type
    but it doesn't change when I cycle that.
beyond this seems to be ordinary maps but the minimap cycles modes

what we've been calling local and global files are actually main map and submap.
eg as we wander around ThornTail Hollow, 0x2B is swaphol/MODELS.bin
when we enter the shop, 0x46 is shop/MODELS.bin
when we leave the shop that remains loaded until we enter the queen's chamber, then 0x46 is swapholbot/MODELS.bin
when we head to Ice Mountain, 0x2B is icemountain/MODELS.bin and 0x46 is empty.
magiccave is also considered part of swaphol

but sometimes it switches which is which
