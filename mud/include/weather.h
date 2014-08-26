#define WEATHER "/obj/handlers/weather.c"
#define HOME "/room/weather_room.c"
#define SAVE_FILE "/save/environ/weather"
#define LOG_FILE "weather_log"
#define TABLE "/table/weather_table.c"
#define WEATHER_SH "/obj/shadows/weather_sh.c"

// Cuantas zonas climaticas modificaremos cada
// vez que actualicemos el clima
#define MODIFY 3

// Incremento por defecto de los valores del clima
// si el incremente calculado es muy peque�o
#define DEF_INC 15

// Tres lunas para Cc, Folken 6/03
#define WHITE_MOON "Luna %^BOLD%^blanca%^RESET%^"
#define RED_MOON "Luna %^RED%^roja%^RESET%^"
#define BLACK_MOON "Luna %^BOLD%^%^BLACK%^negra%^RESET%^"

// Flags para el event_weather
#define FLAG_AMANECER 1
#define FLAG_ANOCHECER 2
#define FLAG_CLIMA 3
#define FLAG_CHECK 4

// Antiguo climate.h
// #define C_TEMP 0
// #define C_CLOUD 1
// #define C_RAIN 2

// #define C_OFFSET 0
// #define C_WEIGHT 1

#define SNOWBALL_OB "/obj/environ/snowball.c"
#define SNOWMAN_OB "/obj/environ/snowman.c"
