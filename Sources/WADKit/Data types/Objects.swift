//
//  Objects.swift
//  WADKit
//
//  Created by Evgenij Lutz on 21.02.25.
//

public enum TR4ObjectType: UInt32, Sendable {
    case LARA = 0
    case PISTOLS_ANIM
    case UZI_ANIM
    case SHOTGUN_ANIM
    case CROSSBOW_ANIM
    case GRENADE_GUN_ANIM
    case SIXSHOOTER_ANIM
    case FLARE_ANIM
    
    case LARA_SKIN
    case LARA_SKIN_JOINTS
    case LARA_SCREAM
    
    case LARA_CROSSBOW_LASER
    
    case LARA_REVOLVER_LASER
    
    case LARA_HOLSTERS
    case LARA_HOLSTERS_PISTOLS
    case LARA_HOLSTERS_UZIS
    case LARA_HOLSTERS_SIXSHOOTER
    
    case LARA_SPEECH_HEAD1
    case LARA_SPEECH_HEAD2
    case LARA_SPEECH_HEAD3
    case LARA_SPEECH_HEAD4
    
    case ACTOR1_SPEECH_HEAD1
    case ACTOR1_SPEECH_HEAD2
    
    case ACTOR2_SPEECH_HEAD1
    case ACTOR2_SPEECH_HEAD2
    
    case LARA_WATER_MESH
    case LARA_PETROL_MESH
    case LARA_DIRT_MESH
    
    case CROWBAR_ANIM
    case TORCH_ANIM
    
    case HAIR
    
    case MOTORBIKE
    case JEEP
    case VEHICLE_EXTRA
    

    case ENEMY_JEEP
    

    case SKELETON
    case SKELETON_MIP
    
    case GUIDE
    case GUIDE_MIP
    
    case VON_CROY
    case VON_CROY_MIP
    
    case BADDY_1
    case BADDY_1_MIP
    
    case BADDY_2
    case BADDY_2_MIP
    
    case SETHA
    case SETHA_MIP
    

    case MUMMY
    case MUMMY_MIP
    
    case SPHINX
    case SPHINX_MIP
    
    case CROCODILE
    case CROCODILE_MIP
    
    case HORSEMAN
    case HORSEMAN_MIP
    
    case SCORPION
    case SCORPION_MIP
    
    case JEAN_YVES
    case JEAN_YVES_MIP
    
    case TROOPS
    case TROOPS_MIP
    
    case KNIGHTS_TEMPLAR
    case KNIGHTS_TEMPLAR_MIP
    
    case MUTANT
    case MUTANT_MIP
    
    case HORSE
    case HORSE_MIP
    
    case BABOON_NORMAL
    case BABOON_NORMAL_MIP
    
    case BABOON_INV
    case BABOON_INV_MIP
    
    case BABOON_SILENT
    case BABOON_SILENT_MIP
    
    case WILD_BOAR
    case WILD_BOAR_MIP
    
    case HARPY
    case HARPY_MIP
    
    case DEMIGOD1
    case DEMIGOD1_MIP
    
    case DEMIGOD2
    case DEMIGOD2_MIP
    
    case DEMIGOD3
    case DEMIGOD3_MIP
    
    case LITTLE_BEETLE
    
    case BIG_BEETLE
    case BIG_BEETLE_MIP
    
    case WRAITH1
    case WRAITH2
    case WRAITH3
    case WRAITH4
    
    case BAT
    
    case DOG
    case DOG_MIP
    
    case HAMMERHEAD
    case HAMMERHEAD_MIP
    
    case SAS
    case SAS_MIP
    
    case SAS_DYING
    case SAS_DYING_MIP
    
    case SAS_CAPTAIN
    case SAS_CAPTAIN_MIP
    
    case SAS_DRAG_BLOKE
    
    case AHMET
    case AHMET_MIP
    
    case LARA_DOUBLE
    case LARA_DOUBLE_MIP
    
    case SMALL_SCORPION
    
    case FISH
    

    case GAME_PIECE1
    case GAME_PIECE2
    case GAME_PIECE3
    case ENEMY_PIECE
    case WHEEL_OF_FORTUNE
    
    case SCALES
    

    case DARTS
    case DART_EMITTER
    case HOMING_DART_EMITTER
    
    case FALLING_CEILING
    case FALLING_BLOCK
    case FALLING_BLOCK2
    
    case SMASHABLE_BIKE_WALL
    
    case SMASHABLE_BIKE_FLOOR
    
    case TRAPDOOR1
    case TRAPDOOR2
    case TRAPDOOR3
    
    case FLOOR_TRAPDOOR1
    case FLOOR_TRAPDOOR2
    
    case CEILING_TRAPDOOR1
    case CEILING_TRAPDOOR2
    
    case SCALING_TRAPDOOR
    
    case ROLLINGBALL
    
    case SPIKEY_FLOOR
    case TEETH_SPIKES
    case JOBY_SPIKES
    case SLICER_DICER
    
    case CHAIN
    case PLOUGH
    case STARGATE
    case HAMMER
    case BURNING_FLOOR
    case COG
    case SPIKEBALL
    
    case FLAME
    case FLAME_EMITTER
    case FLAME_EMITTER2
    case FLAME_EMITTER3
    
    case ROPE
    case FIREROPE
    case POLEROPE
    
    case ONEBLOCK_PLATFORM
    case TWOBLOCK_PLATFORM
    
    case RAISING_BLOCK1
    case RAISING_BLOCK2
    case EXPANDING_PLATFORM
    
    case SQUISHY_BLOCK1
    case SQUISHY_BLOCK2
    
    case PUSHABLE_OBJECT1
    case PUSHABLE_OBJECT2
    case PUSHABLE_OBJECT3
    case PUSHABLE_OBJECT4
    case PUSHABLE_OBJECT5
    
    case TRIPWIRE
    case SENTRY_GUN
    case MINE
    
    case MAPPER
    case OBELISK
    
    case FLOOR_4BLADE
    case ROOF_4BLADE
    case BIRD_BLADE
    case CATWALK_BLADE
    case MOVING_BLADE
    case PLINTH_BLADE
    case SETH_BLADE
    
    case LIGHTNING_CONDUCTOR
    
    case ELEMENT_PUZZLE
    

    case PUZZLE_ITEM1
    case PUZZLE_ITEM2
    case PUZZLE_ITEM3
    case PUZZLE_ITEM4
    
    case PUZZLE_ITEM5
    case PUZZLE_ITEM6
    case PUZZLE_ITEM7
    case PUZZLE_ITEM8
    
    case PUZZLE_ITEM9
    case PUZZLE_ITEM10
    case PUZZLE_ITEM11
    case PUZZLE_ITEM12
    

    case PUZZLE_ITEM1_COMBO1
    case PUZZLE_ITEM1_COMBO2
    
    case PUZZLE_ITEM2_COMBO1
    case PUZZLE_ITEM2_COMBO2
    
    case PUZZLE_ITEM3_COMBO1
    case PUZZLE_ITEM3_COMBO2
    
    case PUZZLE_ITEM4_COMBO1
    case PUZZLE_ITEM4_COMBO2
    
    case PUZZLE_ITEM5_COMBO1
    case PUZZLE_ITEM5_COMBO2
    
    case PUZZLE_ITEM6_COMBO1
    case PUZZLE_ITEM6_COMBO2
    
    case PUZZLE_ITEM7_COMBO1
    case PUZZLE_ITEM7_COMBO2
    
    case PUZZLE_ITEM8_COMBO1
    case PUZZLE_ITEM8_COMBO2
    

    case KEY_ITEM1
    case KEY_ITEM2
    case KEY_ITEM3
    case KEY_ITEM4
    
    case KEY_ITEM5
    case KEY_ITEM6
    case KEY_ITEM7
    case KEY_ITEM8
    
    case KEY_ITEM9
    case KEY_ITEM10
    case KEY_ITEM11
    case KEY_ITEM12
    

    case KEY_ITEM1_COMBO1
    case KEY_ITEM1_COMBO2
    
    case KEY_ITEM2_COMBO1
    case KEY_ITEM2_COMBO2
    
    case KEY_ITEM3_COMBO1
    case KEY_ITEM3_COMBO2
    
    case KEY_ITEM4_COMBO1
    case KEY_ITEM4_COMBO2
    
    case KEY_ITEM5_COMBO1
    case KEY_ITEM5_COMBO2
    
    case KEY_ITEM6_COMBO1
    case KEY_ITEM6_COMBO2
    
    case KEY_ITEM7_COMBO1
    case KEY_ITEM7_COMBO2
    
    case KEY_ITEM8_COMBO1
    case KEY_ITEM8_COMBO2
    

    case PICKUP_ITEM1
    case PICKUP_ITEM2
    case PICKUP_ITEM3
    case PICKUP_ITEM4
    

    case PICKUP_ITEM1_COMBO1
    case PICKUP_ITEM1_COMBO2
    
    case PICKUP_ITEM2_COMBO1
    case PICKUP_ITEM2_COMBO2
    
    case PICKUP_ITEM3_COMBO1
    case PICKUP_ITEM3_COMBO2
    
    case PICKUP_ITEM4_COMBO1
    case PICKUP_ITEM4_COMBO2
    

    case EXAMINE1
    case EXAMINE2
    case EXAMINE3
    

    case CROWBAR_ITEM
    
    case BURNING_TORCH_ITEM
    
    case CLOCKWORK_BEETLE
    case CLOCKWORK_BEETLE_COMBO1
    case CLOCKWORK_BEETLE_COMBO2
    
    case MINE_DETECTOR
    

    case QUEST_ITEM1
    case QUEST_ITEM2
    case QUEST_ITEM3
    case QUEST_ITEM4
    case QUEST_ITEM5
    case QUEST_ITEM6
    
    case MAP
    case SECRET_MAP
    
    case PUZZLE_HOLE1
    case PUZZLE_HOLE2
    case PUZZLE_HOLE3
    case PUZZLE_HOLE4
    
    case PUZZLE_HOLE5
    case PUZZLE_HOLE6
    case PUZZLE_HOLE7
    case PUZZLE_HOLE8
    
    case PUZZLE_HOLE9
    case PUZZLE_HOLE10
    case PUZZLE_HOLE11
    case PUZZLE_HOLE12
    
    case PUZZLE_DONE1
    case PUZZLE_DONE2
    case PUZZLE_DONE3
    case PUZZLE_DONE4
    
    case PUZZLE_DONE5
    case PUZZLE_DONE6
    case PUZZLE_DONE7
    case PUZZLE_DONE8
    
    case PUZZLE_DONE9
    case PUZZLE_DONE10
    case PUZZLE_DONE11
    case PUZZLE_DONE12
    
    case KEY_HOLE1
    case KEY_HOLE2
    case KEY_HOLE3
    case KEY_HOLE4
    
    case KEY_HOLE5
    case KEY_HOLE6
    case KEY_HOLE7
    case KEY_HOLE8
    
    case KEY_HOLE9
    case KEY_HOLE10
    case KEY_HOLE11
    case KEY_HOLE12
    

    case WATERSKIN1_EMPTY
    case WATERSKIN1_1
    case WATERSKIN1_2
    case WATERSKIN1_3
    
    case WATERSKIN2_EMPTY
    case WATERSKIN2_1
    case WATERSKIN2_2
    case WATERSKIN2_3
    case WATERSKIN2_4
    case WATERSKIN2_5
    

    case SWITCH_TYPE1
    case SWITCH_TYPE2
    case SWITCH_TYPE3
    case SWITCH_TYPE4
    case SWITCH_TYPE5
    case SWITCH_TYPE6
    case SWITCH_TYPE7
    case SWITCH_TYPE8
    
    case UNDERWATER_SWITCH1
    case UNDERWATER_SWITCH2
    
    case TURN_SWITCH
    
    case COG_SWITCH
    
    case LEVER_SWITCH
    
    case JUMP_SWITCH
    
    case CROWBAR_SWITCH
    
    case PULLEY
    
    case DOOR_TYPE1
    case DOOR_TYPE2
    case DOOR_TYPE3
    case DOOR_TYPE4
    case DOOR_TYPE5
    case DOOR_TYPE6
    case DOOR_TYPE7
    case DOOR_TYPE8
    
    case PUSHPULL_DOOR1
    case PUSHPULL_DOOR2
    
    case KICK_DOOR1
    case KICK_DOOR2
    
    case UNDERWATER_DOOR
    
    case DOUBLE_DOORS
    
    case BRIDGE_FLAT
    case BRIDGE_TILT1
    case BRIDGE_TILT2
    

    case SARCOPHAGUS
    

    case SEQUENCE_DOOR1
    
    case SEQUENCE_SWITCH1
    case SEQUENCE_SWITCH2
    case SEQUENCE_SWITCH3
    

    case SARCOPHAGUS_CUT
    
    case HORUS_STATUE
    
    case GOD_HEAD
    
    case SETH_DOOR
    
    case STATUE_PLINTH
    

    case PISTOLS_ITEM
    
    case PISTOLS_AMMO_ITEM
    
    case UZI_ITEM
    
    case UZI_AMMO_ITEM
    
    case SHOTGUN_ITEM
    
    case SHOTGUN_AMMO1_ITEM
    
    case SHOTGUN_AMMO2_ITEM
    
    case CROSSBOW_ITEM
    
    case CROSSBOW_AMMO1_ITEM
    
    case CROSSBOW_AMMO2_ITEM
    
    case CROSSBOW_AMMO3_ITEM
    
    case CROSSBOW_BOLT
    
    case GRENADE_GUN_ITEM
    
    case GRENADE_GUN_AMMO1_ITEM
    
    case GRENADE_GUN_AMMO2_ITEM
    
    case GRENADE_GUN_AMMO3_ITEM
    
    case GRENADE
    
    case SIXSHOOTER_ITEM
    
    case SIXSHOOTER_AMMO_ITEM
    
    case BIGMEDI_ITEM
    case SMALLMEDI_ITEM
    
    case LASERSIGHT_ITEM
    case BINOCULARS_ITEM
    
    case FLARE_ITEM
    case FLARE_INV_ITEM
    
    case DIARY_ITEM
    case COMPASS_ITEM
    
    case MEMCARD_LOAD_INV_ITEM
    
    case MEMCARD_SAVE_INV_ITEM
    
    case PC_LOAD_INV_ITEM
    
    case PC_SAVE_INV_ITEM
    

    case SMOKE_EMITTER_WHITE
    case SMOKE_EMITTER_BLACK
    case STEAM_EMITTER
    
    case EARTHQUAKE
    
    case BUBBLES
    
    case WATERFALLMIST
    
    case GUNSHELL
    case SHOTGUNSHELL
    
    case GUN_FLASH
    
    case BUTTERFLY
    
    case SPRINKLER
    

    case RED_LIGHT
    case GREEN_LIGHT
    case BLUE_LIGHT
    case AMBER_LIGHT
    case WHITE_LIGHT
    case BLINKING_LIGHT
    
    case LENS_FLARE
    

    case AI_GUARD
    case AI_AMBUSH
    case AI_PATROL1
    case AI_MODIFY
    case AI_FOLLOW
    case AI_PATROL2
    case AI_X1
    case AI_X2
    case LARA_START_POS
    

    case KILL_ALL_TRIGGERS
    
    case TRIGGER_TRIGGERER
    
    case SMASH_OBJECT1
    case SMASH_OBJECT2
    case SMASH_OBJECT3
    case SMASH_OBJECT4
    case SMASH_OBJECT5
    case SMASH_OBJECT6
    case SMASH_OBJECT7
    case SMASH_OBJECT8
    
    case MESHSWAP1
    case MESHSWAP2
    case MESHSWAP3
    
    case DEATH_SLIDE
    
    case BODY_PART
    case CAMERA_TARGET
    
    case WATERFALL1
    case WATERFALL2
    case WATERFALL3
    
    case PLANET_EFFECT
    
    case ANIMATING1
    case ANIMATING1_MIP
    case ANIMATING2
    case ANIMATING2_MIP
    case ANIMATING3
    case ANIMATING3_MIP
    case ANIMATING4
    case ANIMATING4_MIP
    
    case ANIMATING5
    case ANIMATING5_MIP
    case ANIMATING6
    case ANIMATING6_MIP
    case ANIMATING7
    case ANIMATING7_MIP
    case ANIMATING8
    case ANIMATING8_MIP
    
    case ANIMATING9
    case ANIMATING9_MIP
    case ANIMATING10
    case ANIMATING10_MIP
    case ANIMATING11
    case ANIMATING11_MIP
    case ANIMATING12
    case ANIMATING12_MIP
    
    case ANIMATING13
    case ANIMATING13_MIP
    case ANIMATING14
    case ANIMATING14_MIP
    case ANIMATING15
    case ANIMATING15_MIP
    case ANIMATING16
    case ANIMATING16_MIP
    
    case HORIZON
    case SKY_GRAPHICS
    case BINOCULAR_GRAPHICS
    case TARGET_GRAPHICS
    

    case DEFAULT_SPRITES
    
    case MISC_SPRITES
    

    //case NUMBER_OBJECTS
    
    
    // MARK: WAD v130 extension
    
    case MOTOR_BOAT,
    MOTOR_BOAT_LARA,
    RUBBER_BOAT,
    RUBBER_BOAT_LARA,
    MOTORBIKE_LARA,
    FONT_GRAPHICS,
    PARALLEL_BARS,
    PANEL_BORDER,
    PANEL_MIDDLE,
    PANEL_CORNER,
    PANEL_DIAGONAL,
    PANEL_STRIP,
    PANEL_HALF_BORDER1,
    PANEL_HALF_BORDER2,
    PANEL_MIDDLE_CORNER,
    TIGHT_ROPE,
    LASER_HEAD,
    LASER_HEAD_BASE,
    LASER_HEAD_TENTACLE,
    HYDRA,
    HYDRA_MISSILE,
    ENEMY_SUB_MARINE,
    ENEMY_SUB_MARINE_MIP,
    SUB_MARINE_MISSILE,
    FROG_MAN,
    FROG_MAN_HARPOON,
    FISH_EMITTER,
    KAYAK,
    KAYAK_LARA,
    CUSTOM_SPRITES,
    BRIDGE_TILT3,
    BRIDGE_TILT4,
    BRIDGE_CUSTOM,
    ROBOT_CLEANER,
    ROBOT_STAR_WARS,
    MECH_WARRIOR,
    MECH_WARRIOR_LARA,
    UW_PROPULSOR,
    UW_PROPULSOR_LARA,
    MINE_CART,
    MINE_CART_LARA,
    NEW_SLOT5,
    NEW_SLOT6,
    NEW_SLOT7,
    NEW_SLOT8,
    NEW_SLOT9,
    NEW_SLOT10,
    NEW_SLOT11,
    NEW_SLOT12,
    NEW_SLOT13,
    NEW_SLOT14,
    NEW_SLOT15,
    NEW_SLOT16,
    NEW_SLOT17,
    NEW_SLOT18
    
    case NUMBER_OBJECTS
}


public enum StaticObjectType: UInt32, Sendable {
    case PLANT0 = 0
    case PLANT1
    case PLANT2
    case PLANT3
    case PLANT4
    case PLANT5
    case PLANT6
    case PLANT7
    case PLANT8
    case PLANT9
    
    case FURNITURE0
    case FURNITURE1
    case FURNITURE2
    case FURNITURE3
    case FURNITURE4
    
    case FURNITURE5
    case FURNITURE6
    case FURNITURE7
    case FURNITURE8
    case FURNITURE9
    
    case ROCK0
    case ROCK1
    case ROCK2
    case ROCK3
    case ROCK4
    case ROCK5
    case ROCK6
    case ROCK7
    case ROCK8
    case ROCK9
    
    case ARCHITECTURE0
    case ARCHITECTURE1
    case ARCHITECTURE2
    case ARCHITECTURE3
    case ARCHITECTURE4
    
    case ARCHITECTURE5
    case ARCHITECTURE6
    case ARCHITECTURE7
    case ARCHITECTURE8
    case ARCHITECTURE9
    
    case DEBRIS0
    case DEBRIS1
    case DEBRIS2
    case DEBRIS3
    case DEBRIS4
    case DEBRIS5
    case DEBRIS6
    case DEBRIS7
    case DEBRIS8
    case DEBRIS9
    
    case SHATTER0
    case SHATTER1
    case SHATTER2
    case SHATTER3
    case SHATTER4
    case SHATTER5
    case SHATTER6
    case SHATTER7
    case SHATTER8
    case SHATTER9
    
    case NUMBER_STATIC_OBJECTS
}


public enum FloorType: UInt32, Sendable {
    case FLOOR_TYPE = 0
    case DOOR_TYPE
    case TILT_TYPE
    case ROOF_TYPE
    case TRIGGER_TYPE
    case LAVA_TYPE
    case CLIMB_TYPE
    case SPLIT1
    case SPLIT2
    case SPLIT3
    case SPLIT4
    
    case NOCOLF1T
    case NOCOLF1B
    case NOCOLF2T
    case NOCOLF2B
    case NOCOLC1T
    case NOCOLC1B
    case NOCOLC2T
    case NOCOLC2B
    
    case MONKEY_TYPE
    case TRIGTRIGGER_TYPE
    case MINER_TYPE
}


public enum TriggerType: UInt32, Sendable {
    case TRIGGER = 0
    case PAD
    case SWITCH
    case KEY
    case PICKUP
    case HEAVY
    case ANTIPAD
    case COMBAT
    case DUMMY
    case ANTITRIGGER
    case HEAVYSWITCH
    case HEAVYANTITRIGGER
    case MONKEY
}

public enum TriggerObjectType: UInt32, Sendable {
    case TO_OBJECT
    case TO_CAMERA
    case TO_SINK
    case TO_FLIPMAP
    case TO_FLIPON
    case TO_FLIPOFF
    case TO_TARGET
    case TO_FINISH
    case TO_CD
    case TO_FLIPEFFECT
    case TO_SECRET
    case TO_BODYBAG
    case TO_FLYBY
    case TO_CUTSCENE
}
