

// Non-dgd efuns
// neverbot, 03/2014

#include "sprintf/sprintf.c"


// To simulate the member_array(character, string)

int at(int character, string str)
{
  int i;
  
  if (!character || !str) 
   return -1;
  
  for (i = 0; i < strlen(str); i++)
    if (character == str[i])
      return i; 

  return -1;  
}

string replace_string(string str, string foo, string bar)
{
  string a, b, result;
  int i;

  i = sscanf(str, "%s"+foo+"%s", a, b);
  result = a + bar + b;

  while (i > 0)
  {
    i = sscanf(result, "%s"+foo+"%s", a, b);
    result = a + bar + b;
  }

  return result;
}

string replace(string str, mixed bing, varargs string rep) 
{
  int i;

  if (pointerp(bing)) 
  {
    for (i = 0; i < sizeof(bing); i+=2)
      str = replace_string(str, bing[i], bing[i+1]);
    return str;
  }
  return replace_string(str, bing, rep);
}

string extract(string str, int start, varargs int end) 
{
  return str[start..(end?end:10000)];
} 

string trim(string str) 
{
  if(strlen(str) == 0)
    return str;

  if((str[strlen(str)-1] != 32 && str[strlen(str)-1] != 9) &&
    (str[0] != 32 && str[0] != 9))
    return str;

  while(str[strlen(str)-1] == 32 || str[strlen(str)-1] == 9)
  {
    if(strlen(str) == 1)
      return str;
    str = str[0..strlen(str)-2];
  }

  while(str[0] == 32 || str[0] == 9)
  {
    if(strlen(str) == 1)
      return str;
    str = str[1..strlen(str)-1];
  }  

  return str;
}

//      /secure/simul_efun/wrap.c
//      from the RotD Mudlib
//      wraps text automatically
//      created by Pallando@Nightmare

string wrap(string str, varargs int width) 
{
    return (width ? sprintf("%-=" + width + "s", str + "\n") : 
      sprintf("%-=75s", str + "\n"));
}

//  /adm/simul_efun/arrange_string.c
//  from the Nightmare mudlib
//  makes a string a certain length
//  created by Descartes of Borg 23 december 1992

string arrange_string(string str, int x) 
{
    int i, y;

    if (!str || str == "") 
      return "";
    if (!x) 
      return "";
    if (intp(str)) 
      str = str+"";

    y = strlen(str);

    if (y>x) 
      return str[0..(x-1)];

    for (i=y; i<x; i++) 
      str += " ";

    return str;
}

// What the hell would need this???
int vowel(int i) 
{
  return (i == 'a' || i == 'e' || i == 'i' || i == 'o' || i == 'u');
}


// spanish characters, neverbot 4/03

string lower_case(string str)
{
  int first_char;

  first_char = str[0];
  
  switch(first_char)
  {
    case '�': str[0] = '�'; break;
    case '�': str[0] = '�'; break;
    case '�': str[0] = '�'; break;
    case '�': str[0] = '�'; break;
    case '�': str[0] = '�'; break;
    case '�': str[0] = '�'; break;
    case '�': str[0] = '�'; break;
    default:
      str[0] = first_char + 32;
  }

  return str;
}

string capitalize(string str)
{
  int first_char;

  first_char = str[0];
  
  switch(first_char)
  {
    case '�': str[0] = '�'; break;
    case '�': str[0] = '�'; break;
    case '�': str[0] = '�'; break;
    case '�': str[0] = '�'; break;
    case '�': str[0] = '�'; break;
    case '�': str[0] = '�'; break;
    case '�': str[0] = '�'; break;
    default:
      str[0] = first_char - 32;
  }

  return str;
}

string upper_case(string str)
{
  return capitalize(str);
}

// Taniwha 1995, since the efun one doesn't
string pluralize(string str)
{
  if (!str || str == "") 
    return "objects";
  else return (str + "s");
}
// Antes o despues haria falta...
// Cambia unicamente los acentos, o tambien los acentos y la �

string replace_spanish_characters(string str, varargs int flag_enye)
{
	int i;
	string ret;
	ret = "";
	
	for (i = 0; i < strlen(str); i++)
	{
		switch(str[i..i])
		{
			case "�": ret += "a"; break;
			case "�": ret += "e"; break;
			case "�": ret += "i"; break;
			case "�": ret += "o"; break;
			case "�": ret += "u"; break;
			case "�": ret += "A"; break;
			case "�": ret += "E"; break;
			case "�": ret += "I"; break;
			case "�": ret += "O"; break;
			case "�": ret += "U"; break;
			case "�": ret += (flag_enye?"ny":"�"); break;
			case "�": ret += (flag_enye?"NY":"N"); break;
			default: ret += str[i..i];
		}
	}
	return ret;
}

// format_page()
// Tweaked by Radix for who command : MArch 4, 1997
string format_page(string *items, int columns) {
    int width, i, j, x;
    string ret;

    if(!columns) columns = 2;
    ret = "";
    if(!this_user())
        width = 75/columns;
    else
        width = this_user()->query_cols() / columns;
    for(i=0, x = sizeof(items); i<x; i+=columns) {
        for(j=0; j<columns; j++) {
            if(i+j >= x) break;
            // ret += arrange_string(items[i+j], width);
            // Cambio para mejorar la presentacion, Folken 02/05
            ret += arrange_string("  " +capitalize(items[i+j]), width);
        }
        ret += "\n";
    }
    return ret;
}

/* pad takes a mixed arg, converts to string, and pads it to the desired
   length. Left-justify is the default: if the justify arg is 1, it will
   be right-justified. */
static nomask string pad (string str, int size, varargs int justify) 
{
  int i, len;

  len = strlen(str);
  if (len >= size) 
    return str[0..size-1];
  
  len = size-len;
  
  for (i = 0; i < len; i++) 
  {
    if (justify) 
      str = " " + str;
    else 
      str += " ";
  }
  return str;
}
