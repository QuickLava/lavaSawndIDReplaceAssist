# lavaSawndIDReplaceAssist
A tool for generating patches used by other programs to change SFX IDs within EX Character moveset files.

The math for calculating InfoIndex values for EX Character's was based directly on work by:
- Soopercool101, as well as Kryal, BlackJax96, and libertyernie (BrawlLib, BrawlBox, BrawlCrate)

## Menu Navigation and Input
At any point where you're prompted to select something, simply press the number key associated with your desired option.

At any point where you're prompted to input a numerical value (eg. Soundbank ID), type the number either in decimal with no prefix or in hexadecimal prefixed with "0x", then press the "Enter" key.

## Actions
Snake->EX: Generates a patch for converting Snake SFX IDs into those appropriate for an EX Clone with the specified Soundbank ID. Typically used in conjunction with [lavaSawndPortingUtility](https://github.com/QuickLava/lavaSawndPortingUtility) in order to quickly set up unique soundbanks for EX Clones.

EX->EX: Generates a patch for converting SFX IDs for an EX Clone with one Soundbank ID into those appropriate for an EX Clone with a different Soundbank ID. Useful if you need to change a clone's Soundbank ID for some reason.

EX->Snake: Generate a patch for converting the SFX IDs for an EX Clone with the specified Soundbank ID into those appropriate for Snake. Useful, I suppose, if you wanted to port an EX Clone to be over Snake directly.

## Patch Modes
"sound.txt" (for P+EX Porting Tools): Generates the specified patch in the format used by Codes' Porting Tools, which you can find linked near the top of the official [BrawlEX Guide for P+Ex](https://docs.google.com/document/d/1ZoL_qDcwUpUXg82cKaUp-6D_AcfpFctoW6GXFY74_0k/edit#).
