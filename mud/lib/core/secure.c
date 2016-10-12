
#include <mud/secure.h>
#include <mud/patch_version.h>
#include <mud/config.h>
#include <mud/version.h>
#include <files/log.h>

inherit permission "/lib/core/secure/permission.c";
inherit bastards   "/lib/core/secure/bastards.c";

string * preload;
string * call_out_preload;

static int done;
static mapping snoop_list;

void create() 
{
	permission::create();
	bastards::create();

	preload = ({ });
	call_out_preload = ({ });
	
	done = FALSE;
	snoop_list = ([ ]);

	call_out("load_secure_object", 0);
} 

// every valid_xxx function
#include "/lib/core/secure/checks.c"


#include "/lib/core/secure/crash.c"
#include "/lib/core/secure/coders.c"
#include "/lib/core/secure/domains.c"

#include "/lib/core/secure/ed_stuff.c"
#include "/lib/core/secure/logging.c"
#include "/lib/core/secure/retire.c"

#include "/lib/core/secure/preload.c"



int query_admin(string str) 
{
	return ((positions[str] == POS_ADMIN) || (is_administrator(str)));
} 

int query_only_lord(string str) 
{
	return positions[str] == POS_ADMIN;
} 

string *query_admins() 
{
	return filter_array(keys(positions), "query_only_lord", this_object());
} 

int query_player_high_lord(string str) 
{
	return is_administrator(str) && "/lib/core/login"->test_user(str);
} 

string *is_administrators() 
{
	return filter_array(keys(positions), "query_player_high_lord", this_object())+
		keys(ADMINISTRATORS);
} 

int check_permission(string euid, string *path, int mask);

string get_root_uid() { return ROOT; }
string get_bb_uid() { return BACKBONE; }

string *define_include_dirs() 
{
	return ({ "/include/%s" });
} 

int valid_trace() { return 1; }

void shut(int min) 
{
	"/obj/shut"->shut(min);
} 

void remove_checked_master(string name) 
{
	map_delete(checked_master, name);
} 

// Wonderflug 96, Secure this baby.
mapping query_checked_master() { return checked_master + ([ ]); }

// Allow masters to be unchecked
void uncheck_master(string str) 
{
	if(!this_player() || !is_administrator(geteuid(this_player(1)))) return;
	map_delete(checked_master, str);
}


