#ifndef SERIALIZED_H
#define SERIALIZED_H

typedef struct SerializedDerived SerializedDerived;
struct SerializedDerived
{
    int len;
    int *data;
};

#endif