# NFS Font Toolkit

This is a library of tools which generates compatible FEngFont chunks for NFS games.

## Usage

1. Create your font with FontWiz and save it as an FFN file, preferably somewhere next to the NFSRealFont binary.

2. Invoke the FontToNFS script via a command line.

```batch
FontToNFS FFN_name FEngFont_name [TPKmode: 0/1]
```

- FFN_name: your FFN filename

- FEngFont_name: your resulting FEngFont name - this MUST match the one you're replacing, including the casing! (e.g. FONT_MW_BODY)

- TPKmode: 0 is for MW and earlier, 1 is for Carbon and later

Example:

```batch
FontToNFS Arial12b.ffn FONT_MW_BODY 0
```

This will result in a font bundle compatible with NFS Most Wanted and earlier.

For Carbon and newer, use the TPKmode 1 instead.

3. Use the resulting files to load them into the game. Ideally, load the .bun file, which is a concatinated file of the TPK and the FEngFont together.

## Tool list

- NFSRealFont - a custom utility designed to separate the necessary kerning and texture data from the FFN font, as well as generate a FEngFont

- strhash - string hashing algorithm from the NFS games

- [XNFSTPKTool](https://github.com/xan1242/xnfstpktool) - tool used to manipulate TexturePacks

- FontToNFS - a custom script which ties all the tools together and generates a TPK ini for each font.

- KrnAdjust - Kern batch adjuster - allows you to easily change the offsets of all glyphs in a .krn file exported from FontWiz - use this if fonts are offset

# Full package

This repository will only contain non-copyrighted code due to legal issues.

You may download the full package with all the tools [here](https://mega.nz/file/501QUbSJ#-9EcJsgU3N_FIke6RWlXzs1dnurCpZ_nntN1sMXE0QM).
