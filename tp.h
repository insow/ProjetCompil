#include <stdlib.h>

#define TRUE 1
#define FALSE 0

typedef unsigned char bool;

#define NEW(howmany, type) (type *) calloc((unsigned) howmany, sizeof(type))
#define NIL(type) (type *) 0

/* Etiquettes additionnelles pour les arbres de syntaxe abstraite.
 * Certains tokens servent directement d'etiquette. Attention ici a ne pas
 * donner des valeurs identiques a celles des tokens.
 */
enum {LIST, PLUS, MINUS, MULT, QUO, NE, EQ, LT, LE, GT, GE, CONCAT, ID, IDC, ITE, NEWC, ENVOI, CAST, SELEC, CONST, STRG, THI, SUP, RES, RET, AFFECT, ISBLOC, OVRD, EXT, BODY, DECLS, LISTEXP};

enum {ARG, GLOBAL, LOCAL};

/* Codes d'erreurs */
#define NO_ERROR	0
#define USAGE_ERROR	1
#define LEXICAL_ERROR	2
#define SYNTAX_ERROR    3
#define CONTEXT_ERROR	40	/* default value for this stage */
#define DECL_ERROR	41	/* more precise information */
#define TYPE_ERROR	42
#define CIRCUIT_ERROR   43
#define OVERRIDE_ERROR  44
#define SURCHARGE_ERROR 45
#define EVAL_ERROR	50
#define UNEXPECTED	10O

struct _Tree;
typedef struct _Tree Tree, *TreeP;

struct _Class;
typedef struct _Class Class, *ClassP;

struct _Param;
typedef struct _Param Param, *ParamP;

struct _Method;
typedef struct _Method Method, *MethodP;

struct _DeclParam;
typedef struct _DeclParam DeclParam, *DeclParamP;

struct _env;
typedef struct _Env Env, *EnvP;

/* la structure d'un arbre (noeud ou feuille) */
struct _Tree {
  short op;         /* etiquette de l'operateur courant */
  short nbChildren; /* nombre de sous-arbres */
  ClassP idc;
  int isCallMethod;
  EnvP env;
  EnvP envS;
  union {	
    char *str;      /* valeur de la feuille si op = Id ou STR */
    int val;        /* valeur de la feuille si op = Cste */
    DeclParamP declParams;
    struct _Tree **children; /* tableau des sous-arbres */
  } u;
};

struct _Method {
  char* name;
  DeclParamP params;
  TreeP body;
  ClassP returnType;
  int override;
  EnvP env;
  EnvP envS;
  struct _Method *next;
};

struct _Class {
  char* name;
  TreeP constructorBody;
  DeclParamP constructorParams;
  MethodP methods;
  EnvP env;
  EnvP envS;
  int envSet;
  DeclParamP members;
  struct _Class *super;
  TreeP superTree;
  struct _Class *next;
};

struct _DeclParam {
  char* name;
  ClassP type;
  char* typeName;
  TreeP expression;
  int decl;
  EnvP env;
  EnvP envS;
  struct _DeclParam *next;    
};

struct _Env {
  int isMethod;
  char* name;
  ClassP type;
  struct _Env *next;
};

typedef union
{
  char *S;
  char C;
  int I;
  TreeP pT;
  MethodP pM;
  ClassP pC;
  DeclParamP pDP;
} YYSTYPE;


#define YYSTYPE YYSTYPE

ClassP listClass;

TreeP makeLeafStr(short op, char *str);       
TreeP makeLeafCallMethod(char *str);
TreeP makeLeafInt(short op, int val);	      
TreeP makeTree(short op, int nbChildren, ...);
TreeP makeLeafLVar(short op, DeclParamP lvar);
void fillClass(ClassP class, DeclParamP params, TreeP extends, TreeP constructor, DeclParamP members, MethodP methods);
ClassP makeClass(char *name);
MethodP makeMethod(int override, char* name, DeclParamP params, TreeP body);
DeclParamP makeDecl(char* name, char* class, TreeP expression);
DeclParamP makeParam(char* name, char* class);

ClassP getClass(ClassP classEnv, char* name);
MethodP getMethod(ClassP cl,char* name);

void resolveTree(TreeP tree);
void resolveDeclParam(DeclParamP _declParam);
void resolveMethod(MethodP _method);
void resolveTreeMain();

void evalMain(TreeP tree);

/*Print*/
void pprintTree2(TreeP tree, char *op);
void pprintTree1(TreeP tree, char *op);
void pprintIf(TreeP tree);
void pprintBloc(TreeP tree);
void pprintAff(TreeP tree); 
void pprintMes(TreeP tree);
void pprintNew(TreeP tree);
void pprintDec(TreeP tree);
void pprintListExp(TreeP tree);
void pprintList(TreeP tree);
void pprintBody(TreeP tree);
void pprintCast(TreeP tree);
void pprint(TreeP tree);
void pprintChamps (DeclParamP cc);
void pprintParam (DeclParamP pp);
void pprintExtends (TreeP e);
void pprintMethods (MethodP mm);
void pprintAllClasse (ClassP c);
void pprintMain(TreeP tree);

/*Verif*/
bool circuitClass(ClassP lc);
bool sameParams (DeclParamP p1, DeclParamP p2);
bool overrideMethod(MethodP m, MethodP r);
bool overrideSuper(ClassP lc, MethodP m);
bool isOverride(MethodP m);
bool overrideSuper(ClassP lc, MethodP m);
bool surcharge(ClassP lc, MethodP method);
void override(ClassP lc);
bool scopeType(ClassP c, TreeP main);
void envClass(ClassP c);
void envClass2(ClassP c);
void envClassProp(ClassP c);
void envMethods(MethodP m, EnvP envH);
void envDeclParam(DeclParamP d, EnvP envH);
EnvP inEnv(EnvP env, char* name);
EnvP inEnvMethod(EnvP env, char* name);
void envTree(TreeP t, EnvP envH);
void envTree2(TreeP t);
EnvP envSClass(ClassP c);
EnvP envSDeclParam(DeclParamP d);
EnvP envSMethod(MethodP m);
EnvP envSTree(TreeP t);
EnvP concatEnv(EnvP e1, EnvP e2);
EnvP copyEnv(EnvP e);
EnvP envSExpr(TreeP t, EnvP envH);
