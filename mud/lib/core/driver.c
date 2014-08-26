

#include <kernel.h>
#include <user.h>

#include "/lib/core/efuns.c"

#include <status.h>
#include <trace.h>

// # define TLSVAR2    call_trace()[1][TRACE_FIRSTARG][1]
// # define TLSVAR3    call_trace()[1][TRACE_FIRSTARG][2]

// object rsrcd;        /* resource manager object */
// object accessd;      /* access manager object */
// object userd;        /* user manager object */
// object initd;         // init manager object 
// object objectd;      /* object manager object */
// // object errord;       /* error manager object */
// int tls_size;        /* thread local storage size */

// object TMP;


// ************************************************************
// start neverbot

static object user_h;
static object error_h;
static object object_h;


// Function prototypes
static nomask void log_driver(string str);


static nomask void initialize()
{
  string date;

  date = ::ctime(time())[4 .. 18];

  log_driver("[" + date + "] ** " + status()[ST_VERSION] + "\n");
  log_driver("Initializing...\n");

  load_object(AUTO);

  call_other(error_h  = load_object(ERROR_HANDLER), "???"); // obviously, must be the first
  call_other(user_h   = load_object(USER_HANDLER), "???");
  call_other(object_h = load_object(OBJECT_HANDLER), "???");

  date = ::ctime(time())[4 .. 18];

  log_driver("[" + date + "] ** " + "Initialization complete.\n\n");
}

// Called from stderr efun (so it shouldn't be static)
nomask void _stderr(string str)
{
  if (!str)
    return;

  send_message(str);
}

static nomask void log_driver(string str)
{
  _stderr(str);
}

// The driver object cannot clone objects
nomask object clone_object(mixed what, varargs string uid) 
{
  log_driver("Attempt to clone object from driver, avoided\n");
  return nil;
}

// nomask object compile_object(string path) 
// {
//   log_driver("Attempt to compile object from driver, avoided\n");
//   return nil;
// }

static object telnet_connect(int port)
{
  object user;

  log_driver(" ~ telnet_connect: " + port + "\n");

  user = user_h->new_connection();

  return user;
}

static string object_type(string file, string type)
{
  string str;

  str = resolve_path(file);
  
  log_driver(" ~ object_type: " + file + " = " + str + "\n");

  return str;

  // return normalize_path(type, file + "/..", creator(file));

  // TODO check this is working
  // return resolve_path(file);
}

static int compile_rlimits(string objname)
{
  // unlimited resource usage for kernel objects 
  // return sscanf(objname, "/lib/core/%*s");
  return TRUE;
}

static object inherit_program(string from, string path, int priv)
{
  int i;
  object ob;
  string err;

  if ((i = strlen(path)) >= 2 && path[i - 2 ..] == ".c") 
    path = path[0 .. i - 3];

  log_driver(" + inherit_program: " + path + " from " + from + "\n");

  ob = ::find_object(path);
  
  if (!ob) 
  {
    log_driver(" - compile_object: " + path + " (from driver)\n");
    err = catch(ob = ::compile_object(path));
  }

  if (err)
    log_driver(" + inherit_program error: " + err + "\n");
    
  return ob;
}

static mixed include_file(string file, string path)
{ 
  if (path[0] != '/')
    // path = file + "/../" + path;
    path = resolve_path(path);

  log_driver(" + include_file: " + path + " inside " + file + "\n");

  return path;
}


// Error handling
// void set_error_manager(object ob) { error_h = ob; }

static void runtime_error(string error, int caught, int ticks) 
{
  string ret;

  ret = error_h->runtime_error(error, caught, ticks);

  if (!ret)
    return;

  log_driver(ret);
}

static void compile_error(string file, int line, string err)
{
  string ret;
  ret = error_h->compile_error(file, line, err);

  if (!ret)
    return;

  log_driver(ret);
}

static string atomic_error(string error, int atom, mixed **trace)
{
  string ret;
  ret = error_h->atomic_error(error, atom, trace);

  if (!ret)
    return "";

  log_driver(ret);

  return ret;
}

// Object handler

//   The given object has just been cloned.  Called just before the object
//   is initialized with create(1).
static void clone(string owner, object obj) 
{ 
  log_driver(" + clone: <" + object_name(obj) + "> " + owner + "\n");
  object_h->clone(owner, obj); 
}

//   The given object is about to be destructed.
static void destruct(string owner, object obj) 
{ 
  log_driver(" + destruct: <" + object_name(obj) + "> " + owner + "\n");
  object_h->destruct(owner, obj); 
}

//   The given inheritable object is about to be destructed.
static void destruct_lib(string owner, string path) 
{ 
  log_driver(" + destruct_lib: " + path + " " + owner + "\n");
  object_h->destruct_lib(owner, path); 
}






//   The given object is about to be compiled.
static void compiling(string path) 
{ 
  log_driver(" + compiling: " + path + "\n");
}

//   The given object has just been compiled.  If the source array is not
//   empty, it was compiled from those strings.  Called just before the
//   object is initialized with create(0).
static void compile(string owner, object obj, string *source, string inherited...) 
{ 
  log_driver(" + compile: ");
  print_object(obj);
}

//   The given inheritable object has just been compiled.  If the source
//   array is not empty, it was compiled from those strings.
static void compile_lib(string owner, string path, string *source, string inherited...) 
{ 
  log_driver(" + compile_lib: " + path + "\n");
}

//   An attempt to compile the given object has failed.
static void compile_failed(string owner, string path) 
{ 
  log_driver(" + compile_failed: " + path + "\n");
}

//   The last reference to the given program has been removed.
static void remove_program(string path, int timestamp, int index)
{ 
  log_driver(" + remove_program: " + path + "\n");
}

//   An object which has been marked by call_touch() is about to have the
//   given function called in it.  A non-zero return value indicates that the
//   object's "untouched" status should be preserved through the following
//   call.
static int touch(object obj, string function) 
{ 
  log_driver(" + touch: " + function);
  print_object(obj);

  return FALSE;
}

//   Return a non-zero value if `path' is not a legal first argument
//   for call_other().
static int forbid_call(string path) 
{ 
  log_driver(" + forbid_call: " + path + "\n");

  return FALSE;
}

//   Return a non-zero value if inheritance of `path' by `from' is not
//   allowed.  The flag `priv' indicates that inheritance is private.
static int forbid_inherit(string from, string path, int priv) 
{ 
  log_driver(" + forbid_inherit: " + path + " from " + from + "\n");

  return FALSE;
}






// end neverbot
// ************************************************************








/*
 * NAME:    creator()
 * DESCRIPTION:    get creator of file
 */
// string creator(string file)
// {
//     return (sscanf(file, "/lib/core/%*s") != 0) ? "system" :
//         (sscanf(file, USR_DIR + "/%s/", file) != 0) ? file : nil;
// }

/*
 * NAME:    normalize_path()
 * DESCRIPTION:    reduce a path to its minimal absolute form
 */
// string normalize_path(string file, string dir, varargs string creator)
// {
//     string *path;
//     int i, j, sz;

//     if (strlen(file) == 0) {
//     file = dir;
//     }
//     switch (file[0]) {
//     case '~':
//     /* ~path */
//     if (creator && (strlen(file) == 1 || file[1] == '/')) {
//         file = USR_DIR + "/" + creator + file[1 ..];
//     } else {
//         file = USR_DIR + "/" + file[1 ..];
//     }
//     /* fall through */
//     case '/':
//     /* absolute path */
//     if (sscanf(file, "%*s//") == 0 && sscanf(file, "%*s/.") == 0) {
//         return file;    /* no changes */
//     }
//     path = explode(file, "/");
//     break;

//     default:
//      relative path 
//     if (sscanf(file, "%*s//") == 0 && sscanf(file, "%*s/.") == 0 &&
//         sscanf(dir, "%*s/..") == 0) {
//         /*
//          * simple relative path
//          */
//         return dir + "/" + file;
//     }
//     /* fall through */
//     case '.':
//     /*
//      * complex relative path
//      */
//     path = explode(dir + "/" + file, "/");
//     break;
//     }

//     for (i = 0, j = -1, sz = sizeof(path); i < sz; i++) {
//     switch (path[i]) {
//     case "..":
//         if (j >= 0) {
//         --j;
//         }
//         /* fall through */
//     case "":
//     case ".":
//         continue;
//     }
//     path[++j] = path[i];
//     }

//     return "/" + implode(path[.. j], "/");
// }

/*
 * NAME:    dir_size()
 * DESCRIPTION:    get the size of all files in a directory
 */
// private int dir_size(string file)
// {
//     mixed **info;
//     int *sizes, size, i, sz;

//     info = get_dir(file + "/*");
//     sizes = info[1];
//     size = 1;        /* 1K for directory itself */
//     i = sizeof(sizes);
//     while (--i >= 0) {
//     sz = sizes[i];
//     size += (sz > 0) ?
//          (sz + 1023) >> 10 :
//          (sz == 0) ? 1 : dir_size(file + "/" + info[0][i]);
//     }

//     return size;
// }

/*
 * NAME:    file_size()
 * DESCRIPTION:    get the size of a file in K, or 0 if the file doesn't exist
 */
// int file_size(string file, varargs int dir)
// {
//     if (KERNEL() || SYSTEM()) {
//     mixed **info;
//     string *files, name;
//     int i, sz;

//     if (file == "/") {
//         file = ".";
//     }
//     info = get_dir(file);
//     files = explode(file, "/");
//     name = files[sizeof(files) - 1];
//     files = info[0];
//     i = 0;
//     sz = sizeof(files);

//     if (sz <= 1) {
//         if (sz == 0 || files[0] != name) {
//         return 0;    /* file does not exist */
//         }
//     } else {
//         /* name is a pattern: find in file list */
//         while (name != files[i]) {
//         if (++i == sz) {
//             return 0;    /* file does not exist */
//         }
//         }
//     }

//     i = info[1][i];
//     return (i > 0) ?
//         (i + 1023) >> 10 :
//         (i == 0) ? 1 : (i == -2 && dir) ? dir_size(file) : 0;
//     }
// }

/*
 * NAME:    set_object_manager()
 * DESCRIPTION:    set the object manager
 */
// void set_object_manager(object obj)
// {
//     if (SYSTEM()) {
//     objectd = obj;
//     }
// }

/*
 * NAME:    set_error_manager()
 * DESCRIPTION:    set the error manager
 */
// void set_error_manager(object obj)
// {
//     if (SYSTEM()) {
//     errord = obj;
//     }
// }

/*
 * NAME:    compiling()
 * DESCRIPTION:    object being compiled
 */
// void compiling(string path)
// {
//   if (previous_program() == AUTO) {
//   if (path != AUTO && path != DRIVER && !find_object(AUTO)) {
//     string err;

//     if (objectd) {
//       objectd->compiling(AUTO);
//     }
//     TLSVAR3 = ({ AUTO });
//     err = catch(compile_object(AUTO));
//     if (err) {
//       if (objectd) {
//           objectd->compile_failed("system", AUTO);
//       }
//       error(err);
//     }
//     rsrcd->rsrc_incr("system", "objects", nil, 1, TRUE);
//     if (objectd) {
//       objectd->compile_lib("system", AUTO, ({ }));
//     }
//   }
//   if (objectd) {
//     objectd->compiling(path);
//   }
//   TLSVAR3 = ({ path });
//   }
// }

/*
 * NAME:    compile()
 * DESCRIPTION:    object compiled
 */
// void compile(object obj, string owner, string source...)
// {
//     // if (previous_program() == AUTO) {
//     if (objectd) {
//         objectd->compile(owner, obj, source, TLSVAR3[1 ..]...);
//     }
//     // }
// }

/*
 * NAME:    compile_lib()
 * DESCRIPTION:    inherited object compiled
 */
// void compile_lib(string path, string owner, string source...)
// {
//     // if (previous_program() == AUTO) {
//     if (objectd) {
//         objectd->compile_lib(owner, path, source, TLSVAR3[1 ..]...);
//     }
//     // }
// }

/*
 * NAME:    compile_failed()
 * DESCRIPTION:    object couldn't be compiled
 */
// void compile_failed(string path, string owner)
// {
//     // if (previous_program() == AUTO) {
//     if (objectd) {
//         objectd->compile_failed(owner, path);
//     }
//     // }
// }

/*
 * NAME:    clone()
 * DESCRIPTION:    object cloned
 */
// void clone(object obj, string owner)
// {
//     if (objectd && previous_program() == AUTO) {
//     objectd->clone(owner, obj);
//     }
// }

/*
 * NAME:    destruct()
 * DESCRIPTION:    object about to be destructed
 */
// void destruct(object obj, string owner)
// {
//     if (objectd && previous_program() == AUTO) {
//     objectd->destruct(owner, obj);
//     }
// }

/*
 * NAME:    destruct_lib()
 * DESCRIPTION:    inherited object about to be destructed
 */
// void destruct_lib(string path, string owner)
// {
//     if (objectd && previous_program() == AUTO) {
//     objectd->destruct_lib(owner, path);
//     }
// }

/*
 * NAME:    query_owner()
 * DESCRIPTION:    return owner of driver object
 */
// string query_owner()
// {
//     return "system";
// }

/*
 * NAME:    set_tls_size()
 * DESCRIPTION:    set the thread local storage size
 */
// void set_tls_size(int size)
// {
//     if (previous_program() == API_TLS) {
//     tls_size = size + 3;
//     }
// }

/*
 * NAME:    query_tls_size()
 * DESCRIPTION:    return the thread local storage size
 */
// int query_tls_size()
// {
//     return tls_size;
// }

/*
 * NAME:    get_tlvar()
 * DESCRIPTION:    return value of thread local variable
 */
// mixed get_tlvar(int index)
// {
//     if (previous_program() == API_TLS) {
//     return call_trace()[1][TRACE_FIRSTARG][index + 3];
//     }
// }

/*
 * NAME:    set_tlvar()
 * DESCRIPTION:    set value of thread local variable
 */
// void set_tlvar(int index, mixed value)
// {
//     if (previous_program() == API_TLS) {
//     call_trace()[1][TRACE_FIRSTARG][index + 3] = value;
//     }
// }


/*
 * NAME:    _message()
 * DESCRIPTION:    show _message
 */
// void _message(string str)
// {
//     if (KERNEL() || SYSTEM()) {
//     send_message(ctime(time())[4 .. 18] + " ** " + str);
//     }
// }

/*
 * NAME:    _load()
 * DESCRIPTION:    find or compile object
 */
// private object _load(string path)
// {
//     object obj;

//     obj = find_object(path);
//     if (obj) {
//     return obj;
//     }
//     TLSVAR3 = ({ path });
//     return compile_object(path);
// }

/*
 * NAME:    _initialize()
 * DESCRIPTION:    initialize system, with proper TLS on the stack
 */
// private void _initialize(mixed *tls)
// {
//     object rsrcobj;
//     string * users;
//     int i;

//     driver_initialize();





//     /* _load initial objects */
//     _load(AUTO);
//     call_other(_load(OBJREGD), "???");
//     call_other(rsrcd = _load(RSRCD), "???");

//     /* initialize some resources */
//     rsrcd->set_rsrc("stack",            50, 0, 0);
//     rsrcd->set_rsrc("ticks",        250000, 0, 0);
//     rsrcd->set_rsrc("create stack",      5, 0, 0);
//     rsrcd->set_rsrc("create ticks",  10000, 0, 0);

//     rsrcobj = _load(RSRCOBJ);

//     /* create initial resource owners */
//     rsrcd->add_owner("system");
//     rsrcd->rsrc_incr("system", "filequota", nil,
//              dir_size("/lib/core") + file_size(USR_DIR + "/system",
//              TRUE));
//     rsrcd->add_owner(nil);    /* Ecru */
//     rsrcd->rsrc_incr(nil, "filequota", nil,
//              file_size("/doc", TRUE) + file_size("/include", TRUE));

//     /* _load remainder of manager objects */
//     call_other(rsrcobj, "???");
//     call_other(accessd = _load(ACCESSD), "???");
//     call_other(userd = _load(USERD), "???");
//     call_other(_load(DEFAULT_WIZTOOL), "???");

//     /* initialize other users as resource owners */
//     users = (accessd->query_users() - ({ "system" })) | ({ "admin" });
//     for (i = sizeof(users); --i >= 0; ) {
//     rsrcd->add_owner(users[i]);
//     rsrcd->rsrc_incr(users[i], "filequota", nil,
//              file_size(USR_DIR + "/" + users[i], TRUE));
//     }

//     /* correct object count */
//     rsrcd->rsrc_incr("system", "objects", nil,
//              status()[ST_NOBJECTS] -
//                  rsrcd->rsrc_get("system", "objects")[RSRC_USAGE],
//              1);

//     /* system-specific initialization */
//     if (file_size(USR_DIR + "/system/initd.c") != 0) {
//     catch {
//         initd = rsrcd->initd();
//     } : {
//         _message("Initialization failed.\n");
//         shutdown();
//         return;
//     }
//     }

//     _message("Initialization complete.\n\n");
// }

/*
 * NAME:    initialize()
 * DESCRIPTION:    first function called at system startup
 */
// static void initialize()
// {
//   _initialize(allocate(tls_size = 3));
// }

/*
 * NAME:    prepare_reboot()
 * DESCRIPTION:    prepare for a state dump
 */
// void prepare_reboot()
// {
//   if (KERNEL()) {
//     if (initd) {
//         initd->prepare_reboot();
//     }
//     rsrcd->prepare_reboot();
//     userd->prepare_reboot();
//   }
// }

/*
 * NAME:    _restored()
 * DESCRIPTION:    re-initialize the system, with proper TLS on the stack
 */
// private void _restored(mixed *tls, int hotboot)
// {
//   _message(status()[ST_VERSION] + "\n");

//   if (hotboot) {
//     catch {
//       if (initd) {
//         initd->hotboot();
//       }
//     }
//   } else {
//     rsrcd->reboot();
//     userd->reboot();
//     if (initd) {
//       catch {
//        initd->reboot();
//       }
//     }
//   }

//   _message("State restored.\n\n");
// }

/*
 * NAME:    restored()
 * DESCRIPTION:    re-initialize system after a restore
 */
// static void restored(varargs int hotboot)
// {
//   _restored(allocate(tls_size), hotboot);
// }

/*
 * NAME:    path_read()
 * DESCRIPTION:    handle an editor read path
 */
// static string path_read(string path)
// {
//   string oname, creator;

//   catch {
//    path = previous_object()->path_read(path);
//   }
//   if (path) {
//     creator = creator(oname = object_name(previous_object()));
//     path = normalize_path(path, oname, creator);
//     return ((creator == "system" ||
//          accessd->access(oname, path, READ_ACCESS)) ? path : nil);
//   }
//   return nil;
// }

/*
 * NAME:    path_write()
 * DESCRIPTION:    handle an editor write path
 */
// static string path_write(string path)
// {
//   string oname, creator;
//   int *rsrc, size;

//   catch {
//    path = previous_object()->path_write(path);
//   }
//   if (path) {
//     creator = creator(oname = object_name(previous_object()));
//     path = normalize_path(path, oname, creator);
//     rsrc = rsrcd->rsrc_get(creator, "filequota");
//     if (sscanf(path, "/lib/core/%*s") == 0 &&
//         sscanf(path, "/include/core/%*s") == 0 &&
//         (creator == "system" ||
//          (accessd->access(oname, path, WRITE_ACCESS) &&
//           (rsrc[RSRC_USAGE] < rsrc[RSRC_MAX] || rsrc[RSRC_MAX] < 0)))) {
//       TLSVAR2 = ({ path, file_size(path) });
//       return path;
//     }
//   }
//   return nil;
// }

/*
 * NAME:    call_object()
 * DESCRIPTION:    get an object for call_other's first (string) argument
 */
// static object call_object(string path)
// {
//   int lib;

//   // lib = (sscanf(path, "%*s" + INHERITABLE_SUBDIR) != 0);
//   lib = FALSE;

//   if (path[0] != '/') {
//     string oname;

//     oname = object_name(previous_object());
//     path = normalize_path(path, oname + "/..", creator(oname));
//   }
//   if (lib ||
//   (objectd && objectd->forbid_call(path))) {
//     error("Illegal use of call_other");
//   }
//   return find_object(path);
// }

/*
 * NAME:    object_type()
 * DESCRIPTION:    return normalized object type
 */
// static string object_type(string file, string type)
// {
//   return normalize_path(type, file + "/..", creator(file));
// }

/*
 * NAME:    _touch()
 * DESCRIPTION:    touch an object that has been flagged with call_touch()
 */
// private int _touch(mixed *tls, object obj, string function)
// {
//   return objectd->touch(obj, function);
// }

/*
 * NAME:    touch()
 * DESCRIPTION:    wrapper for _touch()
 */
// static int touch(object obj, string function)
// {
//   mixed *tls;
//   string prog;

//   if (objectd) {
//     if (!previous_object()) {
//       tls = allocate(tls_size);
//     } else if (KERNEL()) {
//       prog = function_object(function, obj);
//       if (prog && sscanf(prog, "/lib/core/%*s") != 0 &&
//         status()[ST_STACKDEPTH] < 0) {
//         /*
//          * protected kernel-to-kernel calls leave the object
//          * "untouched"
//          */
//         return TRUE;
//       }
//     }

//     return _touch(tls, obj, function);
//   }
//   return FALSE;
// }

/*
 * NAME:    inherit_program()
 * DESCRIPTION:    inherit a program, compiling it if needed
 */
// static object inherit_program(string from, string path, int priv)
// {
//   string creator;
//   object obj;
//   int lib;

//   // lib = (sscanf(path, "%*s" + INHERITABLE_SUBDIR) == 0);
//   lib = FALSE;

//   path = normalize_path(path, from + "/..", creator = creator(from));
//   if (lib ||
//     (sscanf(path, "/lib/core/%*s") != 0 && creator != "system") ||
//     !accessd->access(from, path, READ_ACCESS) ||
//     (objectd && objectd->forbid_inherit(from, path, priv))) {
//       return nil;
//   }

//   obj = find_object(path);
//   if (!obj) {
//     int *rsrc;
//     string err;

//     creator = creator(path);
//     rsrc = rsrcd->rsrc_get(creator, "objects");
//     if (rsrc[RSRC_USAGE] >= rsrc[RSRC_MAX] && rsrc[RSRC_MAX] >= 0) {
//         error("Too many objects");
//     }

//     if (objectd) {
//       objectd->compiling(path);
//     }
//     TLSVAR3 = ({ path });
//     err = catch(obj = compile_object(path));
//     if (err) {
//       if (objectd) {
//         objectd->compile_failed(creator, path);
//       }
//       error(err);
//     }
//     rsrcd->rsrc_incr(creator, "objects", nil, 1, TRUE);
//     if (objectd) {
//       objectd->compile_lib(creator, path, ({ }), TLSVAR3[1 ..]...);

//       objectd->compiling(from);
//     }
//     TLSVAR3 = ({ from });
//   } else {
//     TLSVAR3 += ({ path });
//   }
//   return obj;
// }


// static string include_file(string file, string path) 
// {
//   if (path[0] != '/') {
//     return file + "/../" + path;
//   }
//   return path;
// }
/*
 * NAME:    include_file()
 * DESCRIPTION:    translate and return an include path, or the contents of the
 *        file as an array of strings
 */
// static mixed include_file(string from, string path)
// {
//   if (strlen(path) != 0 && path[0] != '~' && sscanf(path, "%*s/../") == 0 &&
//   (sscanf(path, "/include/%*s") != 0 || sscanf(path, "%*s/") == 0)) {
//     /*
//      * safe include: skip access check
//      */
//     if (path[0] != '/') {
//         path = normalize_path(path, from + "/..", creator(from));
//     }
//   } else {
//     path = normalize_path(path, from + "/..", creator(from));
//     if (!accessd->access(from, path, READ_ACCESS)) {
//         return nil;
//     }
//   }
//   if (objectd) {
//     mixed result;

//     result = objectd->include_file(TLSVAR3[0], from, path);
//     if (sscanf(from, "/lib/core/%*s") == 0) {
//         return result;
//     }
//   }
//   return path;
// }

/*
 * NAME:    remove_program()
 * DESCRIPTION:    the last reference to a program is removed
 */
// static void remove_program(string path, int timestamp, int index)
// {
//   string creator;

//   creator = creator(path);
//   if (path != RSRCOBJ) {
//     rsrcd->rsrc_incr(creator, "objects", nil, -1);
//   }
//   if (objectd) {
//     objectd->remove_program(creator, path, timestamp, index);
//   }
// }

/*
 * NAME:    recompile()
 * DESCRIPTION:    recompile an inherited object
 */
// static void recompile(object obj)
// {
//   if (objectd) {
//     string name;

//     name = object_name(obj);
//     objectd->destruct_lib(creator(name), name);
//   }
//   destruct_object(obj);
// }

/*
 * NAME:    telnet_connect()
 * DESCRIPTION:    return a telnet connection user object
 */
// static object telnet_connect(int port)
// {
//   return userd->telnet_connection(allocate(tls_size), port);
// }

/*
 * NAME:    binary_connect()
 * DESCRIPTION:    return a binary connection user object
 */
// static object binary_connect(int port)
// {
//   return userd->binary_connection(allocate(tls_size), port);
// }

/*
 * NAME:    _interrupt()
 * DESCRIPTION:    handle interrupt signal, with proper TLS on the stack
 */
// private void _interrupt(mixed *tls)
// {
//   _message("Interrupt.\n");

// # ifdef SYS_PERSISTENT
//   prepare_reboot();
//   dump_state(1);
// # endif
//   shutdown();
// }

/*
 * NAME:    interrupt()
 * DESCRIPTION:    called when a kill signal is sent to the server
 */
// static void interrupt()
// {
//   _interrupt(allocate(tls_size));
// }

/*
 * NAME:    _runtime_error()
 * DESCRIPTION:    handle runtime error, with proper TLS on the stack
 */
// private void _runtime_error(mixed tls, string str, int caught, int ticks,
//                 mixed **trace)
// {
//   string line, function, progname, objname;
//   int i, sz, len;
//   object user;

//   i = sz = sizeof(trace) - 1;

//   if (ticks >= 0) {
//     mixed *limits;

//     limits = tls[0];
//     while (--i >= caught) {
//       if (trace[i][TRACE_FUNCTION] == "_F_call_limited" &&
//       trace[i][TRACE_PROGNAME] == AUTO) {
//         ticks = rsrcd->update_ticks(limits, ticks);
//         if (ticks < 0) {
//             break;
//         }
//         limits = limits[LIM_NEXT];
//       }
//     }
//     tls[0] = limits;
//   }

//   if (errord) {
//     errord->runtime_error(str, caught, trace);
//   } else {
//     if (caught != 0) {
//       str += " [caught]";
//     }
//     str += "\n";

//     for (i = 0; i < sz; i++) {
//       progname = trace[i][TRACE_PROGNAME];
//       len = trace[i][TRACE_LINE];
//       if (len == 0) {
//        line = "    ";
//       } else {
//         line = "    " + len;
//         line = line[strlen(line) - 4 ..];
//       }

//       function = trace[i][TRACE_FUNCTION];
//       len = strlen(function);
//       if (progname == AUTO && i != sz - 1 && len > 3) {
//         switch (function[.. 2]) {
//           case "bad":
//           case "_F_":
//           case "_Q_":
//               continue;
//         }
//       }
//       if (len < 17) {
//         function += "                 "[len ..];
//       }

//       objname = trace[i][TRACE_OBJNAME];
//       if (progname != objname) {
//         len = strlen(progname);
//         if (len < strlen(objname) && progname == objname[.. len - 1] &&
//             objname[len] == '#') {
//           objname = objname[len ..];
//         }
//         str += line + " " + function + " " + progname + " (" + objname +
//                ")\n";
//       } else {
//         str += line + " " + function + " " + progname + "\n";
//       }
//     }
//     _message(str);
//     if (caught == 0) {
//       user = this_user();
//       while (user && function_object("query_user", user) == LIB_CONN) {
//         user = user->query_user();
//       }
//       if (user) {
//         user->message(str);
//       }
//     }
//   }
// }

/*
 * NAME:    runtime_error()
 * DESCRIPTION:    log a runtime error
 */
// static void runtime_error(string str, int caught, int ticks)
// {
//   mixed **trace, tls;

//   trace = call_trace();

//   if (sizeof(trace) == 1) 
//   {
//     /* top-level error */
//     tls = allocate(tls_size);
//   } 
//   else 
//   {
//     tls = trace[1][TRACE_FIRSTARG];
//     trace[1][TRACE_FIRSTARG] = nil;

//     if (caught == 2 && trace[1][TRACE_PROGNAME] == DRIVER) 
//     {
//       /* top-level catch in driver object: ignore */
//       caught = 0;
//     } 
//     else if (caught != 0 && ticks < 0 &&
//            sscanf(trace[caught - 1][TRACE_PROGNAME],
//               "/lib/core/%*s") != 0) 
//     {
//       tls[1] = str;
//       return;
//     }
//   }

//   _runtime_error(tls, str, caught, ticks, trace);
// }

/*
 * NAME:    atomic_error()
 * DESCRIPTION:    log a runtime error in atomic code
 */
// static void atomic_error(string str, int atom, int ticks)
// {
//   mixed **trace;
//   string line, function, progname, objname;
//   int i, sz, len;
//   object obj;

//   trace = call_trace();
//   sz = sizeof(trace) - 1;

//   if (errord) {
//     errord->atomic_error(str, atom, trace);
//   } else {
//     str += " [atomic]\n";

//     for (i = atom; i < sz; i++) {
//       progname = trace[i][TRACE_PROGNAME];
//       len = trace[i][TRACE_LINE];
//       if (len == 0) {
//         line = "    ";
//       } else {
//         line = "    " + len;
//         line = line[strlen(line) - 4 ..];
//       }

//       function = trace[i][TRACE_FUNCTION];
//       len = strlen(function);
//       if (progname == AUTO && i != sz - 1 && len > 3) {
//         switch (function[.. 2]) {
//           case "bad":
//           case "_F_":
//           case "_Q_":
//             continue;
//         }
//       }
//       if (len < 17) {
//         function += "                 "[len ..];
//       }

//       objname = trace[i][TRACE_OBJNAME];
//       if (progname != objname) {
//         len = strlen(progname);
//         if (len < strlen(objname) && progname == objname[.. len - 1] &&
//             objname[len] == '#') {
//             objname = objname[len ..];
//         }
//         str += line + " " + function + " " + progname + " (" + objname +
//                ")\n";
//       } else {
//         str += line + " " + function + " " + progname + "\n";
//       }
//     }

//     _message(str);
//   }
// }

/*
 * NAME:    compile_error()
 * DESCRIPTION:    deal with a compilation error
 */
// static void compile_error(string file, int line, string err)
// {
//   object obj;

//   if (errord) {
//    errord->compile_error(file, line, err);
//   } else {
//     send_message(file += ", " + line + ": " + err + "\n");
//     if (this_user() && (obj=this_user()->query_user())) {
//       obj->message(file);
//     }
//   }
// }

/*
 * NAME:    compile_rlimits()
 * DESCRIPTION:    compile-time check on rlimits
 */
// static int compile_rlimits(string objname)
// {
//    // unlimited resource usage for kernel objects 
//   return sscanf(objname, "/lib/core/%*s");
// }

/*
 * NAME:    runtime_rlimits()
 * DESCRIPTION:    runtime check on rlimits
 */
// static int runtime_rlimits(object obj, int maxdepth, int maxticks)
// {
//   int depth, ticks;

//   if (maxdepth != 0) {
//     if (maxdepth < 0) {
//         return FALSE;
//     }
//     depth = status()[ST_STACKDEPTH];
//     if (depth >= 0 && maxdepth > depth + 1) {
//         return SYSTEM();
//     }
//   }
//   if (maxticks != 0) {
//     if (maxticks < 0) {
//         return SYSTEM();
//     }
//     ticks = status()[ST_TICKS];
//     if (ticks >= 0 && maxticks > ticks) {
//         return SYSTEM();
//     }
//   }

//   return TRUE;
// }