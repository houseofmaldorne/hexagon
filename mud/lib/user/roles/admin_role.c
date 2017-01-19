
// this is the admin/lord role object

inherit base "/lib/user/roles/manager_role.c";

void create()
{
  base::create();
}

static void start_role(object player)
{
  base::start_role(player);
}

static void role_commands()
{
  // player and coder and manager commands
  base::role_commands();
} 

mixed stats() 
{
  return ({ }) + base::stats();
}