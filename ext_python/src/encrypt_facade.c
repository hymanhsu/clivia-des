#include <Python.h>

#include "cdes/encrypt.h"


static PyObject* pcdes_encrypt(PyObject *self, PyObject *args) {

    //unsigned char des_key [8] = {'a','b','c','d','e','f','g','h'};
    const char *plainString;
    if (!PyArg_ParseTuple(args, "s", &plainString)) {
        return NULL;
    }
    //printf("plainString.addr = %p\n",plainString);
    char * cypher_text = NULL;
    //des_encrypt_with_key(des_key,(unsigned char *)plainString,strlen(plainString),&cypher_text);
    des_encrypt((unsigned char *)plainString,strlen(plainString),&cypher_text);
    //printf("cypher_text = [%s]\n",cypher_text);

    //NOTE:千万别用Py_ssize_t，Python里这个数据类型的高位存储有值，当size_t用的时候，会得到一个非常大的无符号数

    PyObject* result = NULL;
    if(cypher_text){
        result = Py_BuildValue("s", cypher_text);
        free(cypher_text);
    }

    return result;
}


static PyObject* pcdes_decrypt(PyObject *self, PyObject *args) {
    const char* cypherString;
    if (!PyArg_ParseTuple(args, "s", &cypherString)) {
        return NULL;
    }
    //printf("cypherString = [%s]\n",cypherString);
    char * plain_text = NULL;
    des_decrypt(cypherString,&plain_text);
    //printf("plain_text = [%s]\n",plain_text);

    PyObject* result = NULL;
    if(plain_text){
        result = Py_BuildValue("s", plain_text);
        free(plain_text);
    }

    return result;
}


// Method definition object for this extension, these argumens mean:
// ml_name: The name of the method
// ml_meth: Function pointer to the method implementation
// ml_flags: Flags indicating special features of this method, such as
//          accepting arguments, accepting keyword arguments, being a
//          class method, or being a static method of a class.
// ml_doc:  Contents of this method's docstring
static PyMethodDef pcdes_methods[] = { 
    {   
        "encrypt", pcdes_encrypt, METH_VARARGS,
        "encrypt method"
    },  
    {   
        "decrypt", pcdes_decrypt, METH_VARARGS,
        "decrypt method"
    },  
    {NULL, NULL, 0, NULL}
};


// Module definition
// The arguments of this structure tell Python what to call your extension,
// what it's methods are and where to look for it's method definitions
static struct PyModuleDef pcdes_definition = { 
    PyModuleDef_HEAD_INIT,
    "pcdes",
    "pcdes module",
    -1, 
    pcdes_methods
};


// Module initialization
// Python calls this function when importing your extension. It is important
// that this function is named PyInit_[[your_module_name]] exactly, and matches
// the name keyword argument in setup.py's setup() call.
PyMODINIT_FUNC PyInit_pcdes(void) {
    Py_Initialize();
    return PyModule_Create(&pcdes_definition);
}

