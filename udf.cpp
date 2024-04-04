#include <Python.h>
#include <mysql.h>
#include <mariadb_ctype.h>
#include <cstring>
#include <iostream>
#include <dlfcn.h>
#include "pybd.h"

extern "C"
{
  my_bool pyBd_init(UDF_INIT *initid, UDF_ARGS *args, char *message);
  void pyBd_deinit(UDF_INIT *initid);
  char *pyBd(UDF_INIT *initid, UDF_ARGS *args, char *result,
             unsigned long *length, char *is_null, char *error);
}

my_bool pyBd_init(UDF_INIT *initid, UDF_ARGS *args, char *message)
{
  if (args->arg_count != 2)
  {
    strcpy(message, "py_eval must have one argument");
    return true;
  }
  args->arg_type[0] = STRING_RESULT;
  args->arg_type[1] = STRING_RESULT;
  initid->maybe_null = true;

  int pint = Py_IsInitialized();
  if (pint == 0)
  {
    Py_Initialize();  
  }

  return false;
}

char *pyBd(UDF_INIT *initid, UDF_ARGS *args, char *result,
           unsigned long *length, char *is_null, char *error)
{
  char* dynamic = NULL;

  PyThreadState *i = NULL;
  PyStatus ninter = Py_NewInterpreterFromConfig(&i, &config); 
  PyThreadState_Swap(i);

  PyGILState_STATE gstate = PyGILState_Ensure();

  PyObject *dict = PyDict_New();
  char *codigo = args->args[0];
  char *parametro = args->args[1];

  PyObject *param = PyUnicode_FromString(parametro);
  PyDict_SetItemString(dict, "parametro", param);

  PyObject *result_obj = PyRun_StringFlags(codigo, Py_file_input, dict, dict, NULL);

  PyObject *texto = PyDict_GetItemString(dict, "texto");
  const char *texto_str = PyUnicode_AsUTF8(texto);

  if (dynamic != NULL)
    {
      free(dynamic);
    }

  dynamic = (char *)malloc(strlen(texto_str) + 1);
  strcpy(dynamic, texto_str);
  *length = strlen(texto_str);
  
  PyGILState_Release(gstate);

  return dynamic;
}

void pyBd_deinit(UDF_INIT *initid)
{
  //PyThreadState_Swap(i);
}
