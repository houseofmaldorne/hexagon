
#include <common/properties.h>

// all the posible events will be called from
// the event efun:
//   call_other( event_obs, "event_"+ event_name, who, arg... ) ;

// show general info to the object
// used from tell_object and tell_room

void event_write(object caller, string msg)
{
  if (!strlen(msg))
    return;

  msg = fix_string(msg);

  // only will do if interactive(this_object())  
  this_object()->catch_tell(msg);
}

void event_say(object caller, string msg, mixed avoid)
{
  if (!strlen(msg))
    return;

  if (avoid)
  {
    if (pointerp(avoid)) 
    {
      if (member_array(this_object(), avoid) != -1)
        return;
    } 
    else if (objectp(avoid) && (avoid == this_object()))
      return;
  }

  msg = fix_string("\n" + msg);

  this_object()->catch_tell(msg);
}


// TODO ob no es caller!?!
void event_inform(object ob, string msg, string type)
{
  string * on;

  if (!strlen(msg))
    return;

  on = (string *)this_object()->query_property(INFORM_PROP);
  
  if (!on) 
    on = ({ });
  
  // TODO inform properties
  // if (this_object()->query_property(NO_INFORM) || 
  //    (ob->query_invis() && !this_object()->query_coder()) || 
  //    ((int)ob->query_invis() == 2 && !this_object()->query_admin()) || 
  //    !sizeof(on))
  // {
  //     return;
  // }
  
  // if (member_array(type, on) == -1) 
  //   return;
  
  msg = fix_string("\n[" + msg + "]\n\n");

  this_object()->catch_tell(msg);
}

void event_enter(object ob, string mess, varargs object from, object *ignore) 
{
  stderr(" * event_enter " + object_name(this_object()) + "\n");
}

void event_exit(object ob, string mess, varargs object dest, object *ignore) 
{
  stderr(" * event_exit " + object_name(this_object()) + "\n");
}

void event_soul(object ob, string str, mixed avoid)
{
}

void event_person_say(object ob, string start, string mess, string lang, int speaker)
{
}

void event_person_tell(object ob, string start, string mess, string lang)
{
}

void event_whisper(object ob, string start, string mess, 
                   object *obs, string lang)
{
}

void event_person_shout(object ob, string start, string mess, string lang)
{
}

void event_creator_tell(object ob, string start, string mess)
{
}

nomask void event_god_inform(object ob, string start, string mess)
{
}

void event_inter_creator_tell(object ob, string mname, string pname,
                              string mess, object ig, int emote) 
{
}

void event_player_echo_to(object ob, string mess) 
{
}

void event_player_emote_all(object ob, string mess)
{
}

void event_player_echo(object ob, string mess)
{
}
