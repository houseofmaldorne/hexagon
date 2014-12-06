
// Non-dgd efuns
// neverbot, 03/2014

static nomask object * previous_objects(varargs int step)
{
  object result;
  int arg;

  if (!step)
    arg = 0;
  else
    arg = step;

  result = previous_object(arg);

  if (result == nil)
    return ({ });

  return ({ result }) + previous_objects(arg + 1);

}

static nomask object previous_object(varargs int number)
{
  if (!number || (number != -1))
    return ::previous_object(number);
  else
    return previous_objects(1)[0];
}

static nomask string previous_function() 
{
  mixed * trace;

  trace = call_trace();

  // last element = this function
  // second to last element = the function that requested previous_function()
  // third to last element is the function before that: that's the one we want.    
  if (sizeof(trace) < 3) 
    return nil;
  
  // second element is the function name. 
  return trace[sizeof(trace) - 3][2];
}

/*
 * Give a path to a simul_efun file. Observe that it is a string returned,
 * not an object. But the object has to be loaded here. Return 0 if this
 * feature isn't wanted.
 */
// string get_simul_efun() 
// {
//   string fname;
//   fname = "/secure/simul_efun";
  
//   if (catch(call_other(fname, "??"))) 
//   {
//     write("Failed to load " + fname + "\n");
//     shutdown();
//     return 0;
//   }
//   return fname;
// }

