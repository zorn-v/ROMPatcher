# Symbian ROMPatcher

## Patches

Patches should be in E:\Patches directory on mobile and have rmp extention.
Symbols count for addresses and patch data must divided by 2. 12345 - WRONG, 012345 - RIGHT.

## Example patch

```
00DDA6745FFDE000
First line is ROM timestamp (8 bytes at offset 0x80 of ROM)\nThis line (first 256 symbols, I think it enough) will be shown while you choose "Patch info" submenu.\nThis patch just for test. Replace Z:\resource\versions\sw.txt

Lines that dont start with one of patch method (like this one :)) just ignored.
abs:F93C70A6:34002E0030003600330032002E0030002E00330038:35002E0038003800380038002E0037002E00360035

;This line also ignored. Next line show relative patch method (doing the same as line above)
rel:resource\versions\sw.txt:06:34002E0030003600330032002E0030002E00330038:35002E0038003800380038002E0037002E00360035
```
