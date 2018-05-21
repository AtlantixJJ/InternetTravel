#ifndef EXTERN_H_
#define EXTERN_H_

#include <cstdio>
#include <assert.h>
#include <cstring>
#include <Python.h>
#include "InternetTravel.h"

PyObject* search_node(const Node *src, const Node *dst);

extern "C" {

// test functions
int set_argument(char *s);
void test_python();

PyObject* getList(){
    PyObject *PList = PyList_New(0);

    std::vector <int> intVector;
    std::vector<int>::const_iterator it;

    for(int i = 0 ; i < 10 ; i++){
        intVector.push_back(i);
    }

    for(it = intVector.begin(); it != intVector.end() ; it++ ){
        PyList_Append(PList, Py_BuildValue("i", *it));
    }

    return PList;
}

InternetTravel* engine  = NULL;

int init(const char *dir) {

    if(engine != NULL) {
        printf("[!] Already initialized!\n");
        return -1;
    }

    engine = new InternetTravel();
    printf("Dir: %s\n", dir);
    engine->startup(dir);

    return 0;
}

PyObject* get_node_in_map(double x, double y) {
    const Node *cur = engine->getMap()->getNode(x, y);
    PyObject *res = PyList_New(0);
    PyList_Append(res, Py_BuildValue("i", cur->id));
    PyList_Append(res, Py_BuildValue("d", cur->x));
    PyList_Append(res, Py_BuildValue("d", cur->y));

    return res;
}

PyObject* search_xy(double st_x, double st_y, double ed_x, double ed_y) {
    printf("Pos: (%.2lf, %.2lf) (%.2lf, %.2lf)\n", st_x, st_y, ed_x, ed_y);
    const Node* src = engine->getMap()->getNode(st_x, st_y);
    const Node* dst = engine->getMap()->getNode(ed_x, ed_y);
    return search_node(src, dst);
}

PyObject* search_id(int srcID, int dstID) {
    const Node* src = engine->getMap()->getNode(srcID);
    const Node* dst = engine->getMap()->getNode(dstID);
    return search_node(src, dst);
}

int destroy() {
    if(engine == NULL) {
        printf("[!] Not initialized!\n");
        return -1;
    }

    delete engine;
    engine = NULL;

    return 0;
}

}


PyObject* search_node(const Node *src, const Node *dst) {
    PyObject *ret = PyList_New(0);

    SolutionList res = engine->query(src, dst);
    printf("[*] Total solution number: %d\n", res.size());
    for (auto sol : res) {
        PyObject *single_sol    = PyList_New(0);
        PyObject *node_x        = PyList_New(0);
        PyObject *node_y        = PyList_New(0);
        PyObject *node_id       = PyList_New(0);
        PyObject *pass_id       = PyList_New(0);
        PyObject *pass_x        = PyList_New(0);
        PyObject *pass_y        = PyList_New(0);

        // add car id
        PyList_Append(single_sol, Py_BuildValue("i", sol.car->getId()));

        // add car x&y
        PyList_Append(single_sol, Py_BuildValue("d", sol.car->getPos()->x));
        PyList_Append(single_sol, Py_BuildValue("d", sol.car->getPos()->y));

        //printf("[*] Total passenger number: %d\n", sol.car->getPassenger()->size());
        for(auto iter = sol.car->getPassenger()->begin(); iter != sol.car->getPassenger()->end(); iter++) {
            PyList_Append(pass_id, Py_BuildValue("i", (*iter)->id));
            PyList_Append(pass_x, Py_BuildValue("d", (*iter)->x));
            PyList_Append(pass_y, Py_BuildValue("d", (*iter)->y));
        }

        for(auto iter = sol.path.begin(); iter != sol.path.end(); iter++) {
            PyList_Append(node_id, Py_BuildValue("i", (*iter)->id));
            PyList_Append(node_x, Py_BuildValue("d",  (*iter)->x));
            PyList_Append(node_y, Py_BuildValue("d",  (*iter)->y));
        }

        PyList_Append(single_sol, Py_BuildValue("d", sol.dis        ));
        PyList_Append(single_sol, Py_BuildValue("d", sol.detour_dis1));
        PyList_Append(single_sol, Py_BuildValue("d", sol.detour_dis2));
        PyList_Append(single_sol, Py_BuildValue("d", sol.value      ));

        PyList_Append(single_sol, pass_id   );
        PyList_Append(single_sol, pass_x    );
        PyList_Append(single_sol, pass_y    );
        PyList_Append(single_sol, node_id   );
        PyList_Append(single_sol, node_x    );
        PyList_Append(single_sol, node_y    );

        PyList_Append(ret, single_sol);

        Py_DECREF(single_sol);
        Py_DECREF(node_x    );
        Py_DECREF(node_y    );
        Py_DECREF(node_id   );
        Py_DECREF(pass_id   );
        Py_DECREF(pass_x    );
        Py_DECREF(pass_y    );
    }
    return ret;
}

#endif
