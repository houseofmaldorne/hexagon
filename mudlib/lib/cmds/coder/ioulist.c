
#include <mud/cmd.h>

inherit CMD_BASE;

static int cmd(string name, object me, string verb)
{
  // Quick hack of a frooble for auto_load IOUs...
  // Radix May 1, 1996
  int i;
  object pl, *all;
  mixed tmp;

  if (!strlen(name))
    name = me->query_name();
  else
    name = me->expand_nickname(name);

  pl = find_living(name);
 
  if (!pl) 
  {
    notify_fail("Could not find "+name+".\n");
    return 0;
  }

  all = deep_inventory(pl);
  
  tell_object(me, "Looking...\n");
  
  for (i = 0; i < sizeof(all); i++)
  {
    if (all[i] && all[i]->query_iou_object())
    {
      tmp = all[i]->query_auto_load_attributes();
      
      if (!undefinedp(tmp["auto string"]))
        tell_object(me, sprintf("%s - %y\n", tmp["auto string"][0], tmp["auto string"][1]));
    }
  }

  tell_object(me, "Done.\n");
  return 1;
}
