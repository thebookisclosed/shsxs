enum POSITION
{
  POSITION_TOP_LEFT = 0,
  POSITION_TOP_RIGHT = 1,
  POSITION_MIDDLE_RIGHT = 2,
  POSITION_BOTTOM_RIGHT = 3,
  POSITION_BOTTOM_CENTER = 4,
  POSITION_BOTTOM_LEFT = 5,
  POSITION_MAX = 6,
};

enum CHARMS_SIZE {
	CHARMS_SIZE_DEFAULT = 0,
	CHARMS_SIZE_FIRST = 0,
	CHARMS_SIZE_NARROW = 1,
	CHARMS_SIZE_WIDE = 2,
	CHARMS_SIZE_LAST = 2
};

enum SCOPE_ITEM_TYPE
{
  SI_TYPE_INVALID = 0,
  SI_TYPE_INCLUDE = 1,
  SI_TYPE_EXCLUDE = 2,
};

enum SCOPE_ITEM_DEPTH
{
  SI_DEPTH_INVALID = 0,
  SI_DEPTH_SHALLOW = 1,
  SI_DEPTH_DEEP = 2,
};

enum SCOPE_ITEM_FLAGS
{
  SI_FLAG_DEFAULT = 0,
  SI_FLAG_FORCEEXHAUSTIVE = 1,
  SI_FLAG_KNOWNFOLDER = 2,
  SI_FLAG_FASTPROPERTIESONLY = 4,
  SI_FLAG_FASTITEMSONLY = 8,
  SI_FLAG_NOINFOBAR = 16,
  SI_FLAG_USECHILDSCOPES = 32,
  SI_FLAG_FASTPROVIDERSONLY = 64,
  SI_FLAG_OVERRIDE_FOLDER_BEHAVIOR = 128,
};

enum IMMAPPPROPERTYSTOREFLAGS {
	IAGPS_DEFAULT = 0
};

enum ZBID
{
  ZBID_DEFAULT = 0,
  ZBID_DESKTOP = 1,
  ZBID_UIACCESS = 2,
  ZBID_IMMERSIVE_IHM = 3,
  ZBID_IMMERSIVE_NOTIFICATION = 4,
  ZBID_IMMERSIVE_HORIZON = 5,
  ZBID_IMMERSIVE_DOWNSPACE = 6,
  ZBID_IMMERSIVE_MOGO = 7,
  ZBID_IMMERSIVE_ACTION_SPACE = 8,
  ZBID_IMMERSIVE_MOBODY = 9,
  ZBID_IMMERSIVE_DOCK = 10,
  ZBID_IMMERSIVE_BACKGROUND = 11,
  ZBID_IMMERSIVE_COMPANION = 12,
  ZBID_IMMERSIVE_STAGING = 13,
  ZBID_IMMERSIVE_ALTER_EGO = 14,
};