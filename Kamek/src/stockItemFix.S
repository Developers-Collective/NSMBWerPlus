#include <asm_setup.S>

# Add this to the bl hook!
.global StockItemHack
.global StockItemHackR3
.global ItemNamesHack
.global ItemNamesHackR26

# Add these to the symbol map!
.extern SI_Kinoko, SI_Fireflower, SI_Propeller, SI_Iceflower, SI_Penguin, SI_Star

# From loading.S
.extern aI_kinoko_3, aI_fireflower_2, aI_iceflower_0, aI_star_0, aI_propeller_0, aI_penguin, aI_hammer_0

.text
StockItemHack:
  lis r28, StockItemList@h
  ori r28, r28, StockItemList@l
  blr

.text
StockItemHackR3:
  lis r3, StockItemListR3@h
  ori r3, r3, StockItemListR3@l
  blr

.text
ItemNamesHack:
  lis r31, ItemNamesList@h
  ori r31, r31, ItemNamesList@l
  blr

.text
ItemNamesHackR26:
  lis r26, ItemNamesList@h
  ori r26, r26, ItemNamesList@l
  blr

.data
.align 4
StockItemList:
	.long SI_Kinoko
	.long SI_Fireflower
	.long SI_Propeller
	.long SI_Iceflower
	.long SI_Penguin
	.long SI_Star
	.long SI_hammer

StockItemListR3:
	.long SI_Kinoko
	.long SI_Fireflower
	.long SI_Propeller
	.long SI_Iceflower
	.long SI_Penguin
	.long SI_Kinoko
	.long SI_Star
	.long SI_hammer

ItemNamesList:
	.long aI_kinoko_3							# "I_kinoko"
	.long aI_fireflower_2						# "I_fireflower"
	.long aI_iceflower_0						# "I_iceflower"
	.long aI_star_0								# "I_star"
	.long aI_propeller_0						# "I_propeller"
	.long aI_penguin							# "I_penguin"
	.long aI_hammer_0							# "I_hammer"

SI_hammer:
	.string "SI_Hammer"

