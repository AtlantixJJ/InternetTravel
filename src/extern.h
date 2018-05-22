#ifndef EXTERN_H_
#define EXTERN_H_

#include <cstdio>
#include <assert.h>
#include <cstring>
#include <Python.h>
#include "InternetTravel.h"

PyObject* search_node(const Node *src, const Node *dst);

extern "C" {

// export to python

// test functions
int set_argument(char *s);
void test_python();
PyObject* getList();

// Extern interface work by maintaining the IT object
InternetTravel* engine  = NULL;

/// Call this before using
int init(const char *dir);

/// Find the nearest point in map given x, y
PyObject* get_node_in_map(double x, double y);

/// Find the path given start and end position
PyObject* search_xy(double st_x, double st_y, double ed_x, double ed_y);

/// Find the path given start point ID and end point ID
PyObject* search_id(int srcID, int dstID);

/// Call this after using
int destroy();

}

/// Backend function for
PyObject* search_node(const Node *src, const Node *dst);
#endif
