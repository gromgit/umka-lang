#ifndef UMKA_IDENT_H_INCLUDED
#define UMKA_IDENT_H_INCLUDED

#include "umka_common.h"
#include "umka_vm.h"


typedef enum
{
    // Built-in functions are treated specially, all other functions are either constants or variables of "fn" type
    IDENT_CONST,
    IDENT_VAR,
    IDENT_TYPE,
    IDENT_BUILTIN_FN
} IdentKind;


typedef struct tagIdent
{
    IdentKind kind;
    IdentName name;
    int hash;
    Type *type;
    int module, block;                  // Global identifiers are in block 0
    bool exported, inHeap;
    int prototypeOffset;                // For function prototypes
    union
    {
        BuiltinFunc builtin;            // For built-in functions
        void *ptr;                      // For global variables
        int offset;                     // For functions (code offset) or local variables (stack offset)
        Const constant;                 // For constants
    };
    struct tagIdent *next;
} Ident;


typedef struct
{
    Ident *first, *last;
    ErrorFunc error;
} Idents;


void identInit(Idents *idents, ErrorFunc error);
void identFree(Idents *idents, int startBlock /* < 0 to free in all blocks*/);

Ident *identFind          (Idents *idents, Modules *modules, Blocks *blocks, int module, char *name);
Ident *identAssertFind    (Idents *idents, Modules *modules, Blocks *blocks, int module, char *name);

Ident *identAddConst      (Idents *idents, Modules *modules, Blocks *blocks, char *name, Type *type, bool exported, Const constant);
Ident *identAddGlobalVar  (Idents *idents, Modules *modules, Blocks *blocks, char *name, Type *type, bool exported, void *ptr);
Ident *identAddLocalVar   (Idents *idents, Modules *modules, Blocks *blocks, char *name, Type *type, bool exported, int offset);
Ident *identAddType       (Idents *idents, Modules *modules, Blocks *blocks, char *name, Type *type, bool exported);
Ident *identAddBuiltinFunc(Idents *idents, Modules *modules, Blocks *blocks, char *name, Type *type, BuiltinFunc builtin);

int    identAllocStack    (Idents *idents, Blocks *blocks, int size);
Ident *identAllocVar      (Idents *idents, Types *types, Modules *modules, Blocks *blocks, char *name, Type *type, bool exported);
Ident *identAllocParam    (Idents *idents, Types *types, Modules *modules, Blocks *blocks, Signature *sig, int index);

#endif // UMKA_IDENT_H_INCLUDED
