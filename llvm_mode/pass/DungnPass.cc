#include "llvm/IR/LegacyPassManager.h"
#include "llvm/Pass.h"
#include "llvm/Transforms/IPO/PassManagerBuilder.h"
#include "llvm/IR/Module.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/InstVisitor.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/InstrTypes.h" // callbase
#include "llvm/IR/DataLayout.h"
#include "llvm/IR/Type.h"
#include "llvm/IR/Constants.h"
#include "llvm/Support/FileSystem.h"
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <algorithm>
#include <cassert>
#include <cstdint>
#include <memory>
#include <utility>
#include <vector>
#include <fstream>

using namespace llvm;

Function *printFunc;
Function *exitFunc;
Function *int2kCheck;
Function *int2kTranslateName; // Function name to ID
Function *int2kHandleArg; // handling exe arguments
Function *int2kInitFlag; // init flags and stuffs
Function *int2kDeleteFlag; // delete flags.
Function *int2kAddEdge; // help metadata
Function *int2kHandleFuncName; //
Function *int2kHandleMetaData; // this one literally handle the unrelated functions stuffs
Function *markFunctionID; // this one calls all the NameToID and TranslateAddr
Function *metaData; // this one will call the AddEdge

//Aibar
Function *AibarFunc;

std::vector<Function*> func;
Constant* allFuncName;
int nFunction; // number of functions;
std::string output;
Value *flag; // GlobalVariable type
Type *flagType;
std::map<std::string, bool> focusedFunction;
std::map<Function*, Constant*> funcNameStr;

Value *getAndInsertFuncPtr(IRBuilder<> &builder, LLVMContext &C, Function* F);
Constant *getFuncNameStr(IRBuilder<> &builder, Function* F);
Function *makeMarkFunctionID(Module &M);
Function *makeMetaData(Module &M);
Function *getPrint(Module &M);
Function *getQuit(Module &M);
Function *getInt2kCheck(Module &M); // exiter
Function *getInt2kInitFlag(Module &M);
Function *getInt2kDeleteFlag(Module &M);
Function *getInt2kAddEdge(Module &M);
Function *getInt2kHandleArg(Module &M);
Function *getInt2kHandleFuncName(Module &M);
Function *getInt2kTranslateName(Module &M);
Function *getInt2kHandleMetaData(Module &M);

Function *getAibarFunc(Module &M);

// migrating from Module.cpp
void dfs(int x);
void freemem();
void injectSVFInfo(Module &M);
void readSVF();
void readICFG();
int runSVF();
void dumpToFile(Module &M);
bool SVF_Flag = false;
std::vector<std::string> needInstrumentedFunc;
std::vector<Function *> *unrelatedFunc;
std::map<std::string, int> nodeID;
std::string *nodeName;
std::vector<int> *adj;
int _nNode, _nEdge;
int nINode, nIEdge; // ICFG
std::map<int, Function*> funcAddr;
bool *visited;
struct iNode {
  std::string name, info, func;
  int ID;
  iNode(std::string x, std::string y, std::string z){name = x; info = y; func = z;}
};
std::map<std::string, iNode*> iNodeMap;
std::map<std::string, int> iNodeToInt;
std::vector<iNode*> iNodeList;
std::vector<int> *iAdj;
std::vector<int> *iReachable;
struct iEdge {
  iNode *src;
  iNode *dst;
};
//---

std::map<std::string, bool> needInstrument;

struct Int2kVisitor : public InstVisitor<Int2kVisitor>
{
  void visitFunction(Function &F) 
  {
    if(F.begin()==F.end() || !needInstrument[F.getName().str()])
      return;
      
    LLVMContext &C = F.getParent()->getContext();
    BasicBlock *b = &*F.begin();
    IRBuilder<> builder(b, b->begin());
    Value *funcNameStr = getFuncNameStr(builder, &F);
    //errs() << F.getName() << "\n";
    SmallVector<Value*, 1> params;
    params.push_back(funcNameStr);
    builder.CreateCall(int2kCheck, params);
    builder.CreateCall(AibarFunc, params);
  }
};

void getThings(Module &M)
{
 		printFunc = getPrint(M);
    exitFunc = getQuit(M);
    int2kCheck = getInt2kCheck(M);
    AibarFunc = getAibarFunc(M);  


		int2kTranslateName = getInt2kTranslateName(M);
    int2kInitFlag = getInt2kInitFlag(M);
    int2kDeleteFlag = getInt2kDeleteFlag(M);
    int2kAddEdge = getInt2kAddEdge(M);
    int2kHandleArg = getInt2kHandleArg(M);
    int2kHandleFuncName = getInt2kHandleFuncName(M);
    int2kHandleMetaData = getInt2kHandleMetaData(M);
    //makeMetaData(M);
    makeMarkFunctionID(M); 
		
}

void insertThings(Module &M)
{
    nFunction = 0;
    //errs() << "Function list: \n";
    for(auto f = M.begin(), fe = M.end(); f!=fe; ++f)
    {
      Function *F = &*f;
      nFunction++;
      func.push_back(&*f);
    }
    Int2kVisitor int2kVisitor;
    int2kVisitor.visit(M);
    for(auto f = M.begin(), fe = M.end(); f!=fe; ++f)
    {
      Function *F = &*f;
      if(F->getName().compare("main") == 0)
      {
        LLVMContext &C = M.getContext(); 
        std::vector<Value*> arg;
        for(auto i = F->arg_begin(), iE = F->arg_end(); i != iE; ++i)
            arg.push_back(i);
        BasicBlock *bStart = &*(F->begin());
        BasicBlock *bEnd = &(F->back());
        IRBuilder<> builder(bStart, bStart->begin());
        // insert init flags, metadata
        SmallVector<Value*, 1> params;
        params.push_back(ConstantInt::get(Type::getInt32Ty(C), nFunction));
        builder.CreateCall(int2kInitFlag, params); 
        
        // insert metaData function
        //SmallVector<Value*, 0> params1;
        // insert init translation from funcPtr||funcName to ID
        //builder.CreateCall(markFunctionID, params1);
        // insert argument handler
        
        SmallVector<Value*, 2> params2;
        params2.push_back(arg[0]);
        params2.push_back(arg[1]);
        builder.CreateCall(int2kHandleArg, params2);
        BasicBlock::iterator it = bEnd->end();
        it--;
        IRBuilder<> builderEnd(bEnd, it);
        // insert function that delete allocated arrays to free memeory
        params.clear();
        builderEnd.CreateCall(int2kDeleteFlag, params);
        
        break;
      }
    }
}

class DungN : public ModulePass {
public:
  static char ID;
  DungN():ModulePass(ID) {} 
  bool runOnModule(Module &M) override;
};

bool DungN::runOnModule(Module &M)
{
  errs() << "\ngluten tag, ?????? >:)\n";
  dumpToFile(M); // int2kBC.bc;
  if(!runSVF())
    return true;
  errs() << "\ndone running SVF\n";
	readSVF();
  errs() << "\ndone reading SVF\n";
	injectSVFInfo(M); 
  errs() << "\ndone injecting infos\n";
//  system("wpa --ander --gen-icfg --dump-icfg int2kBC.bc > /dev/null");
//  system("python /readDot.py icfg_final.dot > int2kTmp.txt");
  //readICFG();
	getThings(M);
	insertThings(M);
  errs() << "finished DungN pass\n";
  //system("rm callgraph_final.dot callgraph_initial.dot 1> /dev/null");
  //system("rm int2kGraph.txt int2kInfo.txt int2kBC.txt int2kBC.bc 1> /dev/null");
  errs() << "\n";
  freemem();
  return false;
}

void dfs(int x)
{
  visited[x] = true;
  for(int i = 0; i < (int)adj[x].size(); ++i)
  {
    if(!visited[adj[x][i]])
      dfs(adj[x][i]);
  }
}

void iDfs(int x)
{
  visited[x] = true;
  for(int i = 0; i < (int)iAdj[x].size(); ++i)
  {
    if(!visited[iAdj[x][i]])
      iDfs(iAdj[x][i]);
  }
}

void freemem()
{
  delete [] nodeName;
  delete [] adj;
  delete [] visited;
}

void injectSVFInfo(Module &M)
{
  if(!SVF_Flag)
  {
    freemem();
    return;
  }
  for(auto it = M.begin(), eit = M.end();
    it != eit; it ++)
    {
      Function *func = &*it;
      int id = nodeID[func->getName().str()];
      funcAddr[id] = func;
    }
  for(auto it = M.begin(), eit = M.end();
    it != eit; it ++)
    {
      Function *func = &*it;
      int id = nodeID[func->getName().str()];
      std::memset(visited, false, sizeof(visited));
      dfs(id);
      for(int j = 0; j < _nNode; ++j)
      {
        if(!visited[j])
          unrelatedFunc[id].push_back(funcAddr[j]);
      }
    }
}

void readICFG()
{
  if(!SVF_Flag)
  {
     errs() << "SVF_Flag is false, please check if SVF is installed to your PATH\n";
    errs() << "to check, command 'wpa --version' should output version of some llvm\n";
    return;
  }
  std::fstream file;
  file.open("int2kTmp.txt", std::ios::in);
  file >> nINode >> nIEdge;
  for(int i = 0; i < nINode; ++i)
  {
    file.ignore(256, '\n');
    std::string x, y, z; 
    std::getline(file, x); 
    std::getline(file, y); 
    std::getline(file, z); 
    errs() << x << "\n" << y << "\n" << z << "\n ------------\n";
    iNode *node = new iNode(x, y, z);
    iNodeMap[x] = node;
    iNodeList.push_back(node);
  }
  std::vector<iEdge*> tmp;
  iAdj = new std::vector<int>[iNodeList.size()];
  iReachable = new std::vector<int>[iNodeList.size()];
  visited = new bool[iNodeList.size()];
  for(int i = 0; i < iNodeList.size(); ++i) 
  {
    iNodeToInt[iNodeList[i]->name] = i;
    iNodeList[i]->ID = i;
  }
  for(int i = 0; i < nIEdge; ++i)
  {
    std::string x, y;
    file >> x >> y;
    iNode *nodeSrc = iNodeMap[x];
    iNode *nodeDst = iNodeMap[y];
    iEdge *e = new iEdge();
    e->src = iNodeMap[x];
    e->dst = iNodeMap[y];
    int idX = iNodeToInt[x];
    int idY = iNodeToInt[y];
    iAdj[idY].push_back(idX);
  }
  for(int i = 0; i < iNodeList.size(); ++i) 
  {
    std::memset(visited, false, sizeof(visited));
    iDfs(i);
     errs() << iNodeList[i]->name << " " << strlen((iNodeList[i]->name).c_str()) << " \n ";
    for(int j = 0; j < iNodeList.size(); ++j)
      if(j != i && visited[j])  {
        errs() << "\t" << iNodeList[j]->name << "\n";
        iReachable[i].push_back(j);
      }
  }
  file.close();
}

void readSVF()
{
  if(!SVF_Flag)
  {
    errs() << "SVF_Flag is false, please check if SVF is installed to your PATH\n";
    errs() << "to check, command 'wpa --version' should output version of some llvm\n";
    return ;
  }
  std::fstream file;
  file.open("int2kInfo.txt", std::ios::in);
  file >> _nNode >> _nEdge;
  int n = _nNode, m = _nEdge;
  file.close();
  adj = new std::vector<int>[n];
  nodeName = new std::string[n];
  visited = new bool[n];
  file.open("int2kGraph.txt", std::ios::in);
  std::string x, y;
  int nodeMarker = 0;
  for(int i = 0; i < n; ++i)
  {
    file >> x;
    if(nodeID.find(x) == nodeID.end()) {
      nodeID[x] = nodeMarker ++;
    }
    int ID = nodeID[x];
    nodeName[ID] = x;
    needInstrument[x] = true;
  }
  unrelatedFunc = new std::vector<Function*>[nodeMarker];
  for(int i = 0; i < m; ++i)
  {
    file >> x >> y;
    int ID1 = nodeID[x], ID2 = nodeID[y];
    adj[ID2].push_back(ID1);
  }
  file.close();
}


int runSVF()
{
  int res = system("wpa -ander -dump-callgraph int2kBC.bc > /dev/null");
  if(res != 0)
    errs () << "run SVF command failed, should install SVF first. \n";
  else
    SVF_Flag = true;
  return SVF_Flag;
}


void dumpToFile(Module &M)
{
  std::error_code EC;
  raw_ostream *out = new raw_fd_ostream("int2kBC.txt", EC, llvm::sys::fs::FileAccess::FA_Write);
  M.print(*out, nullptr, false, false);
  system("llvm-as int2kBC.txt -o int2kBC.bc");
  //errs() << "int2kBC.bc created\n";
}

Function *getInt2kHandleMetaData(Module &M) 
{
  LLVMContext &C = M.getContext();
  SmallVector<Type*, 4> params;
  params.push_back(Type::getInt32Ty(C));
  params.push_back(Type::getInt32PtrTy(C));
  params.push_back(Type::getInt8PtrTy(C)->getPointerTo());
  params.push_back(Type::getInt8PtrTy(C)->getPointerTo());
  FunctionType *FT = FunctionType::get(Type::getVoidTy(C), params, false);
  M.getOrInsertFunction("_int2k_handle_meta_data", FT);
  return M.getFunction("_int2k_handle_meta_data");
}

/*
* insert call to this function at begining of main
* this function translates ALL functions pointer at run-time into unique ID
*/

Function* makeMarkFunctionID(Module &M)
{
  LLVMContext &C = M.getContext();
  SmallVector<Type*, 0> params;
  FunctionType *FT = FunctionType::get(Type::getVoidTy(C), params, false);
  M.getOrInsertFunction("markFunctionID", FT);
  markFunctionID = M.getFunction("markFunctionID");
  BasicBlock* block = BasicBlock::Create(C, "entry", markFunctionID);
  IRBuilder<> builder(block);
  int cnt = 0;
  std::vector<Constant*> namesVec;
  for(auto f = M.begin(), fe = M.end(); f!=fe; ++f)
  {
    Function *F = &*f;
    Constant* name = getFuncNameStr(builder, F); //builder.CreateGlobalStringPtr(F->getName());
    namesVec.push_back(name);
    cnt ++;
  }
  ArrayRef<Constant*> namesArr(namesVec);
  Type *i8pp = Type::getInt8PtrTy(C)->getPointerTo();
  Constant *c = ConstantArray::get(ArrayType::get(PointerType::Type::getInt8PtrTy(C), cnt),namesArr);
  Type* tc = c->getType(); //tc->print(errs()); errs() << "\n"; // [19 x i8*]
  GlobalVariable *G = new GlobalVariable(
    M, tc, true, GlobalVariable::LinkageTypes::ExternalLinkage, nullptr, "int2kBestArray");
  G->setInitializer(c); // type [n * i8*]*
  Constant *index0 = ConstantInt::get(Type::getInt64Ty(C), 0); // index 0
  Value *tmp0 = builder.CreateAlloca(Type::getInt64Ty(C), nullptr, "tmp0");
  builder.CreateStore(index0, tmp0);
  Value *tmp1 = builder.CreateLoad(Type::getInt64Ty(C), tmp0, "tmp1");
  Value *tmp2 = builder.CreateConstGEP2_32(tc, G, 0, 0, "tmp2"); // type i8**
  Value *tmp3 = builder.CreateAlloca(i8pp, nullptr, "tmp3");
  builder.CreateStore(tmp2, tmp3);
  Value *tmp4_ = builder.CreateLoad(i8pp, tmp3, "tmp4_");
  //tmp4->getType()->print(errs()); errs() << "\n";
  SmallVector<Value*, 2> args;
  args.push_back(ConstantInt::get(Type::getInt32Ty(C), cnt));
  args.push_back(tmp4_);
  builder.CreateCall(int2kHandleFuncName, args);
	
  /* * * * * * * * * * * * * * * * * * * * * * *
   * Now we call the int2kHandleMetaData here. *
   * * * * * * * * * * * * * * * * * * * * * * */
  // 1st argument is cnt  
  // 3rd argument is tmp4 (derived from tmp3, which is made from tmp2) -- the array of names
	
  std::vector<Constant*> bIndex;
  std::vector<Constant*> namesVec1; // name of the unrelated functions (flatten)
  bIndex.push_back(ConstantInt::get(Type::getInt32Ty(C), 0));
  int sum = 0;
  cnt = 1;
  for(auto f1 = M.begin(), fle = M.end(); f1!=fle; ++f1)
  {
    if(nodeID.find(f1->getName().str()) == nodeID.end())
      continue;
    int id = nodeID[f1->getName().str()];
    sum += (int)unrelatedFunc[id].size();
    bIndex.push_back(ConstantInt::get(Type::getInt32Ty(C), sum));
    for(int i = 0; i < (int)unrelatedFunc[id].size(); ++i)
    {
      Function *F2 = unrelatedFunc[id][i];
      Constant* name = getFuncNameStr(builder, F2); //builder.CreateGlobalStringPtr(F2->getName());
      namesVec1.push_back(name);
    }
  }
  Constant *bIndexArr = ConstantArray::get(ArrayType::get(Type::getInt32Ty(C), cnt), bIndex);
  Type *bIndexArrTy = bIndexArr->getType();
  GlobalVariable *gBIndexArr = new GlobalVariable(
    M, bIndexArrTy, true, GlobalVariable::LinkageTypes::ExternalLinkage, nullptr, "int2kBlockIndexFunction");
  gBIndexArr->setInitializer(bIndexArr);
  Value *tmp5 = builder.CreateConstGEP2_32(bIndexArrTy, gBIndexArr, 0, 0, "tmp5");
  Value *tmp6 = builder.CreateAlloca(Type::getInt32PtrTy(C), nullptr, "tmp6");
  builder.CreateStore(tmp5, tmp6);
  Value *tmp7_ = builder.CreateLoad(Type::getInt32PtrTy(C), tmp6, "tmp7_"); // this is 2nd argument. -- 
                                                                          // the array of block index
  Constant *namesVec1Arr = ConstantArray::get(ArrayType::get(PointerType::getInt8PtrTy(C), sum), namesVec1);
  Type *namesVec1ArrTy = namesVec1Arr->getType();
  GlobalVariable *gNamesVec1Arr = new GlobalVariable(
    M, namesVec1ArrTy, true, GlobalVariable::LinkageTypes::ExternalLinkage, nullptr, "int2kFlattenedFunctionNames");
  gNamesVec1Arr->setInitializer(namesVec1Arr);
  Value *tmp8 = builder.CreateConstGEP2_32(namesVec1ArrTy, gNamesVec1Arr, 0, 0, "tmp8");
  Value *tmp9 = builder.CreateAlloca(i8pp, nullptr, "tmp9");
  builder.CreateStore(tmp8, tmp9);
  Value *tmp10_ = builder.CreateLoad(i8pp, tmp9, "tmp10_"); // this is 4th argument. -- the flattened function array.
  SmallVector<Value*, 4> args1;
  args1.push_back(ConstantInt::get(Type::getInt32Ty(C), cnt));
  args1.push_back(tmp7_); args1.push_back(tmp4_); args1.push_back(tmp10_);
  builder.CreateCall(int2kHandleMetaData, args1);
  
	builder.CreateRetVoid();
	return M.getFunction("markFunctionID");
}

// insert IR instructions translates Function ptr to int32ptr at runtime.
Value* getAndInsertFuncPtr(IRBuilder<> &builder, LLVMContext &C, Function* F)
{
  Value *ptr = builder.CreateAlloca(Type::getInt32PtrTy(C), nullptr);
  Value *cast = builder.CreateBitCast(F, Type::getInt32PtrTy(C));
  builder.CreateStore(cast, ptr);
  return cast;
}
Constant *getFuncNameStr(IRBuilder<> &builder, Function* F)
{
  Constant *tmp;
  if(funcNameStr.find(F) == funcNameStr.end())
  {
    tmp = builder.CreateGlobalStringPtr(F->getName());
    funcNameStr[F] = tmp;
  } else 
    tmp = funcNameStr[F];
  return tmp;
}

Function *getPrint(Module &M)
{
  Function *tmp = M.getFunction("printf"); // external
  if(tmp == NULL)
  {
    LLVMContext &C = M.getContext();
    std::vector<Type*> params;
    params.push_back(Type::getInt8PtrTy(C));
    FunctionType *printType = 
      FunctionType::get(Type::getInt32Ty(C), params, true);
    Function *x = Function::Create(printType, 
      Function::ExternalLinkage, "printf", &M);
    tmp = x;
  }
  return tmp;
}
Function *getQuit(Module &M)
{
  Function *tmp = M.getFunction("exit"); // external
  if(tmp == NULL)
  {
    LLVMContext &C = M.getContext();
    std::vector<Type*> params;
    params.push_back(Type::getInt32Ty(C));
    FunctionType *exitType = 
      FunctionType::get(Type::getVoidTy(C), params, false);
    Function *x = Function::Create(exitType,
      Function::ExternalLinkage, "exit", &M);
    tmp = x;
  }
  return tmp;
}
Function *getInt2kCheck(Module &M) // exiter
{
  LLVMContext &C = M.getContext();
  std::vector<Type *> params;
  params.push_back(Type::getInt8PtrTy(C));// run-time func ptr
  FunctionType *FT = FunctionType::get(Type::getVoidTy(C), params, false);
  M.getOrInsertFunction("_int2k_check", FT);
  return M.getFunction("_int2k_check");
}
Function *getInt2kInitFlag(Module &M) 
{
  LLVMContext &C = M.getContext();
  SmallVector<Type *, 1> params;
  params.push_back(Type::getInt32Ty(C));
  FunctionType *FT = FunctionType::get(Type::getVoidTy(C), params, false);
  M.getOrInsertFunction("_int2k_init_flag", FT);
  return M.getFunction("_int2k_init_flag");
}
Function *getInt2kDeleteFlag(Module &M)
{
  LLVMContext &C = M.getContext();
  SmallVector<Type *, 0> params;
  FunctionType *FT = FunctionType::get(Type::getVoidTy(C), params, false);
  M.getOrInsertFunction("_int2k_delete_flag", FT);
  return M.getFunction("_int2k_delete_flag");
}
Function *getInt2kAddEdge(Module &M)
{
  LLVMContext &C = M.getContext();
  SmallVector<Type*, 2> params;
  for(int i = 0; i < 2; ++i)
    params.push_back(Type::getInt8PtrTy(C));
  FunctionType *FT = FunctionType::get(Type::getVoidTy(C), params, false);
  M.getOrInsertFunction("_int2k_add_edge", FT);
  return M.getFunction("_int2k_add_edge");
}
Function *getInt2kHandleArg(Module &M)
{
  LLVMContext &C = M.getContext();
  SmallVector<Type*, 2> params;
  params.push_back(Type::getInt32Ty(C)); // i32
  params.push_back(PointerType::getUnqual(Type::getInt8PtrTy(C))); //i8**
  FunctionType *FT = FunctionType::get(Type::getInt32Ty(C), params, false);
  M.getOrInsertFunction("_int2k_handle_arg", FT);
  return M.getFunction("_int2k_handle_arg");
}
Function *getAibarFunc(Module &M)
{
  LLVMContext &C = M.getContext();
  SmallVector<Type*, 1> params;
  params.push_back(Type::getInt8PtrTy(C)); //i8**
  FunctionType *FT = FunctionType::get(Type::getVoidTy(C), params, false);
  M.getOrInsertFunction("_aibarFunc", FT);
  return M.getFunction("_aibarFunc");
}
Function *getInt2kHandleFuncName(Module &M)
{
  LLVMContext &C = M.getContext();
  SmallVector<Type*, 2> params;
  params.push_back(Type::getInt32Ty(C)); // i32
  params.push_back(PointerType::getUnqual(Type::getInt8PtrTy(C))); //i8**
  FunctionType *FT = FunctionType::get(Type::getVoidTy(C), params, false);
  //FT->print(errs()); errs() << "\n";
  M.getOrInsertFunction("_int2k_handle_function_name", FT);
  return M.getFunction("_int2k_handle_function_name");
}
Function *getInt2kTranslateName(Module &M)
{
  LLVMContext &C = M.getContext();
  SmallVector<Type*, 2> params;
  params.push_back(Type::getInt8PtrTy(C));
  params.push_back(Type::getInt32Ty(C)); 
  FunctionType *FT = FunctionType::get(Type::getVoidTy(C), params, false);
  M.getOrInsertFunction("_int2k_translateName", FT);
  return M.getFunction("_int2k_translateName");
}

char DungN::ID = 0;

static RegisterPass<DungN> X("aibar_pass", "Dungn Pass");

static void registerAflDungnPass(const PassManagerBuilder &, 
                                legacy::PassManagerBase &PM) {
  PM.add(new DungN());
}

static RegisterStandardPasses
  RegisterAflDungnPass(PassManagerBuilder::EP_OptimizerLast,
                      registerAflDungnPass);

/*
static RegisterStandardPasses
     RegisterAflDungnPass0(PassManagerBuilder::EP_EnabledOnOptLevel0,
                           registerAflDungnPass);
*/
