
// Users handler
// Keeps tracking of users connected and disconnected

#include <kernel.h>
#include <user/user.h>
#include <mud/config.h>

static mapping users;

void create() 
{
  ::create();
  users = ([ ]);
  
  if (!find_object(USER_OB)) 
    compile_object(USER_OB);

#ifdef __NETWORK_EXTENSIONS__
  open_port("telnet", TELNET_PORT);
#endif
}

// called only from this object, in 'new_connection'
static void add_new_user(object ob) 
{
  string id;

  // if (base_name(previous_object()) != USER_OB) 
  //   return;

  id = file_name(ob);

  users += ([ id : allocate(2) ]);

  // values[0] = the user object
  // values[1] = the user name
  users[id][0] = ob;
  users[id][1] = "";
}

void remove_user(object ob) 
{
  string id;

  // if (base_name(previous_object()) != USER_OB) 
  //   return;

  id = file_name(ob);

  users[id] = nil;
}

object * query_users()
{
  string * ids;
  int i;
  object * result;

  ids = m_indices(users);
  result = ({ });

  for (i = 0; i < sizeof(ids); i++)
    result += ({ users[ids[i]][0] });

  return result;
}

string * query_user_names()
{
  string * ids;
  int i;
  string * result;

  ids = m_indices(users);
  result = ({ });

  for (i = 0; i < sizeof(ids); i++)
    result += ({ users[ids[i]][1] });

  return result;
}

mapping query_user_data() 
{
  return users;
}

string * query_user_ids() 
{
  return m_indices(users);
}

object find_user(string id) 
{
  if (!users[id])
    return nil;
  
  return users[id];
}

int update_user(object ob)
{
  string id;

  // only the login object can do this (from /lib/core/login.c)
  if (base_name(previous_object()) != "/lib/core/login")
    return -1;

  id = file_name(ob);

  if (!users[id])
    return -1;

  // cannot change names of already logged-in players
  if (users[id][1] != "")
    return -1;

  users[id][1] = ob->query_name();

  return 1;
}

object new_connection()
{
  object new_user;

  new_user = clone_object(find_object(USER_OB));

  add_new_user(new_user);

  return new_user;
}

#ifdef __NETWORK_EXTENSIONS__
object connection(string ipnumber, int port)
{
  stderr(" - connection from: "+ipnumber+" with port "+port+"\n");
  return new_connection();
}

void open(int port)
{
  stderr("Now accepting connections on port "+port+"\n");
}

void close(int force)
{
  stderr("Not accepting any more connections (force "+force+")\n");
}
#endif