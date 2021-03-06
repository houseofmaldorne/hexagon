
#define DIR_SOME_PLACE "somewhere"

#define DIR_DOWN      "down"
#define DIR_UP        "up"
#define DIR_IN        "inside"
#define DIR_OUT       "outside"
#define DIR_NORTH     "north"
#define DIR_SOUTH     "south"
#define DIR_EAST      "east"
#define DIR_WEST      "west"
#define DIR_SOUTHWEST "southwest"
#define DIR_SOUTHEAST "southeast"
#define DIR_NORTHEAST "northeast"
#define DIR_NORTHWEST "northwest"
#define DIR_COMMON    "common"
#define DIR_STAIRS    "stairs"

#define DIR_DOWN_SHORTEN      "down"
#define DIR_UP_SHORTEN        "up"
#define DIR_IN_SHORTEN        "in"
#define DIR_OUT_SHORTEN       "out"
#define DIR_NORTH_SHORTEN     "n"
#define DIR_SOUTH_SHORTEN     "s"
#define DIR_EAST_SHORTEN      "e"
#define DIR_WEST_SHORTEN      "w"
#define DIR_SOUTHWEST_SHORTEN "sw"
#define DIR_SOUTHEAST_SHORTEN "se"
#define DIR_NORTHEAST_SHORTEN "ne"
#define DIR_NORTHWEST_SHORTEN "nw"
#define DIR_STAIRS_SHORTEN    "stairs"

/* Some common aliases */
#define EXPANSION ([ \
   DIR_DOWN_SHORTEN      : DIR_DOWN, \
   DIR_UP_SHORTEN        : DIR_UP, \
   DIR_IN_SHORTEN        : DIR_IN, \
   DIR_OUT_SHORTEN       : DIR_OUT, \
   DIR_NORTH_SHORTEN     : DIR_NORTH, \
   DIR_SOUTH_SHORTEN     : DIR_SOUTH, \
   DIR_EAST_SHORTEN      : DIR_EAST, \
   DIR_WEST_SHORTEN      : DIR_WEST, \
   DIR_SOUTHWEST_SHORTEN : DIR_SOUTHWEST, \
   DIR_SOUTHEAST_SHORTEN : DIR_SOUTHEAST, \
   DIR_NORTHEAST_SHORTEN : DIR_NORTHEAST, \
   DIR_NORTHWEST_SHORTEN : DIR_NORTHWEST, \
   DIR_STAIRS_SHORTEN    : DIR_STAIRS, \
])

#define OPPOSITES ([ \
   DIR_NORTH     : DIR_SOUTH, \
   DIR_SOUTH     : DIR_NORTH, \
   DIR_WEST      : DIR_EAST, \
   DIR_EAST      : DIR_WEST, \
   DIR_NORTHWEST : DIR_SOUTHEAST, \
   DIR_SOUTHEAST : DIR_NORTHWEST, \
   DIR_NORTHEAST : DIR_SOUTHWEST, \
   DIR_SOUTHWEST : DIR_NORTHEAST, \
   DIR_UP        : DIR_DOWN, \
   DIR_DOWN      : DIR_UP, \
   DIR_IN        : DIR_OUT, \
   DIR_OUT       : DIR_IN, \
   DIR_STAIRS    : DIR_STAIRS, \
])

#define EXIT_ARTICLES ([ \
   DIR_NORTH     : "the", \
   DIR_SOUTH     : "the", \
   DIR_WEST      : "the", \
   DIR_EAST      : "the", \
   DIR_NORTHWEST : "the", \
   DIR_SOUTHEAST : "the", \
   DIR_NORTHEAST : "the", \
   DIR_SOUTHWEST : "the", \
   DIR_STAIRS    : "the", \
])
