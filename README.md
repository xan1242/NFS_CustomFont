# Need for Speed - Custom Font Loader & Scaler

A plugin for Need for Speed games by Black Box which allows custom font loading and scaling!

## Game Support

All Need for Speed games by Black Box in the same engine (except World & Hot Pursuit 2)

The only caveat is that you are running the games with a NoCD patch.

Below is a table of supported executable hashes & versions. Others may or may not work (case in point: Undercover has multiple versions that work).

| Game              | Version     | NoCD              | MD5                              |
| ----------------- | ----------- | ----------------- | -------------------------------- |
| NFS Underground   | 1.4.0       | Drunk!            | 22C731FE0C0C41FE14CE80B728B6CBE0 |
| NFS Underground 2 | 1.2         | HOODLUM           | 6445ADBF4E8D64BE51E96A69BBA168A4 |
| NFS Most Wanted   | 1.3         | RELOADED (Proper) | C0516B485065FABDD69579816B5DF763 |
| NFS Carbon        | 1.4         | ViTALiTY          | 53708939D0DEE3A3C58D046CB73EB80D |
| NFS Pro Street    | 1.1         | Battery           | 0B2F2FF8E8C334AD33F13AB789FCFDE2 |
| NFS Undercover    | 1.0 (Steam) | PROPHET           | 6919A24CBDF13D345128AFCEE812C779 |

- NOTE: If you are using XNFSModFiles, it may conflict with this mod. This is a fault of XNFSModFiles and will be remedied at a later time.
- NFS Undercover is a bit of an exception - these fonts are only used in the HUD and not anywhere else

## Features

- Autoloading of files from a folder

- Configurable font scales (per each font)

## Installation & Usage

1. Extract the archive to the root of the game directory.

If you're planning on using HD fonts from the HD font pack:

- Extract the HD font pack to the root of the game directory and overwrite any files.

If you're planning on using/making your own fonts:

1. Put your custom fonts in the "CustomFonts" directory with the filename matching the name of the font

2. If necessary, add a line in NFS_CustomFont.txt with the desired font scale. The line should look something like this:

```
FONT_NFS_BODY = 0.208334
```

Replace "FONT_NFS_BODY" with your own name and the scale number with the one you need.

# Tools

You may find the tools used to build custom fonts in the "Tools" folder. Open the README there for more information.

# HD Font Packs

In the "Releases" tab, you may find the "HDFonts" archive for the game of your choice. Simply extract the file to the root of the game directory to install!
