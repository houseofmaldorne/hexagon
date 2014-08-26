

void say(string str, varargs mixed avoid) 
{
  if (!pointerp(avoid))
    avoid = ({ this_player(), previous_object() }) + ({ avoid });
  else
    avoid += ({ this_player(), previous_object() });
  if (!environment(previous_object()))
    if (this_player() && environment(this_player()))
      event(environment(this_player()), "say", str, avoid);
    else
      event(previous_object(), "say", str, avoid);
  else
    event(environment(previous_object()), "say", str, avoid);
} 

void tell_room(object ob, string str, varargs mixed avoid) 
{
  if (!ob || !objectp(ob))
    return ;
  event(ob, "say", str, avoid);
} 

void tell_object(object ob, string str) 
{
  if (objectp(ob))
    ob->event_write(previous_object(), str);
} 

void shout(string str, varargs object avoid) 
{
  object *ob;
  int i;

  ob = users();

  for (i = 0; i < sizeof(ob); i++)
    if (ob[i] != avoid && !ob[i]->query_earmuffs())

  if(ob[i] != this_player()) 
    ob[i]->event_say(previous_object(), str, ({ }));
}

static nomask void write(string str) 
{
  object user;

  if (str == nil)
    return;

  // TODO write this is temporal
  // find_object(DRIVER)->_stderr("[from "+previous_program()+"] " + str);
  // find_object(DRIVER)->_stderr(str);  
  // return;

  user = this_user();

  if (!user)
    return;

  user->do_efun_write(str);
}

static nomask void stderr(string str)
{
  if (!str)
    return;

  find_object(DRIVER)->_stderr(str);

  return;  
}


void tell_player(object player, string text)
{
    tell_object(player, "\n\t%^BOLD%^" + text + "%^RESET%^\n");
}

