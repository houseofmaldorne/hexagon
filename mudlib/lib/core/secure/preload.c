
/*
 * This is all the autoloading stuff.
 * please don't remove again - lynscar.
 */
void add_preload(string file) 
{
  if (previous_object() == this_object() ||
      is_administrator(geteuid(initial_object()))) 
  {
    if (!sizeof(preload)) 
    {
      preload = ({ file });
    } 
    else if (member_array(file, preload) == -1) 
    {
      preload += ({ file });
    }

    save_object(SECURE_SAVE_PATH);
  }
}

void remove_preload(string file) {
  int i;

  if (previous_object() == this_object() ||
      is_administrator(geteuid(initial_object()))) 
  {
    if (sizeof(preload)) 
    {
      i = member_array(file, preload);

      if (i >= 0) 
      {
        preload = preload[0 .. i - 1] + preload[i + 1 .. 1000];
        save_object(SECURE_SAVE_PATH);
      }
    }
  }
}

void add_call_out_preload(string file) 
{
  if (previous_object() == this_object() ||
      is_administrator(geteuid(initial_object()))) 
  {
    if (!sizeof(call_out_preload)) 
    {
      call_out_preload = ({ file });
    } 
    else if (member_array(file, call_out_preload) == -1) 
    {
      call_out_preload += ({ file });
    }

    save_object(SECURE_SAVE_PATH);
  }
}

void remove_call_out_preload(string file) 
{
  int i;

  if (previous_object() == this_object() ||
      is_administrator(geteuid(initial_object()))) 
  {
    if (sizeof(call_out_preload)) 
    {
      i = member_array(file, call_out_preload);
      if (i >= 0) 
      {
        call_out_preload = delete(call_out_preload, i, 1);
        save_object(SECURE_SAVE_PATH);
      }
    }
  }
}

string *epilog() 
{
  int i;

  // if (!sizeof(preload)) 
  //   load_secure_object();

  for (i = 0; i < sizeof(call_out_preload); i++)
    call_out("preload", 2, call_out_preload[i]);

  // Wonderflug 96, Making secure
  return ( preload ? preload + ({ }) : ({ }) );
} 

void preload(string file) 
{
  string e;

  stderr(" ~ preloading: "+file+".\n");

  if ((e = catch(load_object(file)->dummy()))) 
    write("      "+e+"\n");

}

void load_secure_object() 
{
  if (!done) 
  {
    done = 1;
    seteuid(ROOT);
    restore_object(SECURE_SAVE_PATH);
    epilog();
  }
}