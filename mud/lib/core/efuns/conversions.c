
/* A non-recursive copy function for arrays, mappings, and other
   variables represented by pointers. If the variable being copied
   itself contains pointers, those pointers are _not_ copied so the
   data they point to is not secured. (So far I have no need of such
   a structure so this is adequate. And much faster.)
*/
nomask mixed copy (mixed a) 
{
  mixed b;

  if (typeof(a)==T_ARRAY || typeof(a)==T_MAPPING) 
      b = a[..];
  else 
      b = a;

  return b;
}

// Extracted from the old /global/basic/print_object.c, neverbot
string print_object(mixed ob, varargs int offset, string pre) 
{
  int i;

  if (pre)
    write("                   "[0..offset - strlen(pre)] + pre);
  else
    write("                   "[0..offset]);

  if (intp(ob)) 
  {
    write(ob + "\n");
    return "int";
  }

  if (stringp(ob)) 
  {
    write("\"" + ob + "\"\n");
    return "string";
  }

  if (objectp(ob)) 
  {
    write("<" + file_name(ob) + ">");
    
    // if (ob->short())
    //   write("(short) " + ob->short() + "\n");
    // else if (ob->query_name())
    //   write("(name)  " + ob->query_name() + "\n");
    // else
    //   write("(argghhhh)\n");
    
    return "object";
  }

  if (pointerp(ob)) 
  {
    if (!sizeof(ob))
      write("({ })\n");
    else 
    {
      write("({  -- size " + sizeof(ob) + "\n");
      
      for (i = 0; i < sizeof(ob); i++)
        print_object(ob[i], offset + 5, ""+i+". ");
      
      write("                       "[0..offset] + "})\n");
    }
    return "pointer";
  }

  if (undefinedp(ob))
  {
    return "nil";
  }

  return "oh dear";
}

private string number_as_string_aux(int n);
string number_as_string(int n);

string query_num(int n, int limit) 
{
  if (limit && n>limit) 
    return "miles de"; /* this is a little pointless ... */
  return number_as_string(n);
}

string number_as_string(int n)
{
  string ret;
  int i;

  if (n < 0) return "miles de"; /* fo wraps... */
  if (n > 99999) return "miles de"; /* sorry 'bout the hard limit */
  ret = nil;
  if ((i = n/1000)) {
    n = n%1000;
    if (i==1) ret = "mil";
    else
      ret = number_as_string_aux(i) + " mil";
    if (!n) return ret;
  }
  if ((i = n/100)) {
    n = n%100;
      if (!ret) ret="";
      else ret += " ";
      switch (i)
      {
        case 1: if (!n) ret += "cien"; else ret += "ciento"; break;
        case 2: ret += "doscientos"; break;
        case 3: ret += "trescientos"; break;
        case 4: ret += "cuatrocientos"; break;
        case 5: ret += "quinientos"; break;
        case 6: ret += "seiscientos"; break;
        case 7: ret += "setecientos"; break;
        case 8: ret += "ochocientos"; break;
        case 9: ret += "novecientos"; break;
      }  
      if (!n) return ret;
  }
  if (ret)
    return ret + " " + number_as_string_aux(n);
  return number_as_string_aux(n);
} /* query_num() */

private string number_as_string_aux(int n) 
{
  string ret;

  if (!n) return "cero";

  if (n > 99) return "muchos";
  
  if (n<30 && n>9) return ({ "diez", "once", "doce", "trece",
                             "catorce", "quince", "dieciséis", "diecisiete",
                             "dieciocho", "diecinueve", "veinte",
                             "veintiún", "veintidós", "veintitrés",
                             "veinticuatro", "veinticinco", "veintiséis",
                             "veintisiete", "veintiocho", "veintinueve" })[n-10];
  ret = ({ "", "", "", "treinta", "cuarenta", "cincuenta", "sesenta",
           "setenta", "ochenta", "noventa"})[n/10];
  if ((n = n%10)&&(ret!="")) ret += " y ";
  return ret + ({ "", "un", "dos", "tres", "cuatro", "cinco", "seis",
                  "siete", "ocho", "nueve"})[n];
} /* number_as_string() */


string query_multiple_short(mixed *obs, varargs int flag) 
{
  int i, j, k;
  string *str;
  mixed *bity;

  bity = ({ });
  for (i = 0; i < sizeof(obs); i++) 
  {
    if (!stringp(obs[i])) 
    {
      if (!pointerp(obs[i])) 
      // like a string.  First bit is the string.
      // second the object 
      {
        if(objectp(obs[i]))
        {
          if (flag)
            str = (string *)obs[i]->pretty_short();
          else
            str = (string *)obs[i]->short();
        }
      }
      else 
      {
        str = obs[i][0];
        obs[i] = obs[i][1];
      }
    } 
    else
      str = obs[i];
      
    if (!str) // ignore invis objects
      continue;
      
    if (stringp(str))
      str = ({ str });
      
    for (j=0;j<sizeof(str);j++)
      if ((k=member_array(str[j], bity)) == -1)
        bity += ({ str[j], ({ obs[i] }) });
      else
        bity[k+1] = ({ obs[i] })+bity[k+1];
  }
  str = ({ });
  for (i = 0; i < sizeof(bity); i+=2) 
  {
    j = sizeof(bity[i+1]);
    if (stringp(bity[i+1][0]))
      if (j==1)
        str += ({ bity[i] });
      else
      str += ({ (query_num(j,20)+
        " "+pluralize(bity[i])) });
    else if (j == 1)
    /* Propiedad determinate eliminada
    if (bity[i+1][0]->query_property("determinate"))
    str += ({ bity[i+1][0]->query_property("determinate")+bity[i] });
    else
    */
      // str += ({ add_a(bity[i]) });
      str += ({ bity[i] });
    else
    {
      if (flag)
        str += ({ (query_num(j,20)+
          " "+bity[i+1][0]->pretty_plural(bity[i])) });
      else
        str += ({ (query_num(j,20)+
          " "+bity[i+1][0]->query_plural(bity[i])) });
    }
  }
  
  if (!sizeof(str))
    return "";
    
  if (sizeof(str) == 1)
    return str[0];
    
  if (lower_case(str[sizeof(str)-1][0..0]) == "i")
    return implode(str[0..sizeof(str)-2],", ")+" e "+str[sizeof(str)-1];
  else
    return implode(str[0..sizeof(str)-2],", ")+" y "+str[sizeof(str)-1];

}
