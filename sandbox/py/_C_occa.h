#define NPY_NO_DEPRECATED_API NPY_1_7_API_VERSION

#include "Python.h"
#include "numpy/arrayobject.h"

#include "occa_c.h"

//---[ Globals & Flags ]----------------
static PyObject* py_occaSetVerboseCompilation(PyObject *self, PyObject *args);
//======================================

//----[ Background Device ]-------------
//  |---[ Device ]----------------------
static PyObject* py_occaSetDevice(PyObject *self, PyObject *args);
static PyObject* py_occaSetDeviceFromInfo(PyObject *self, PyObject *args);

static PyObject* py_occaGetCurrentDevice(PyObject *self, PyObject *args);

static PyObject* py_occaSetCompiler(PyObject *self, PyObject *args);
static PyObject* py_occaSetCompilerEnvScript(PyObject *self, PyObject *args);
static PyObject* py_occaSetCompilerFlags(PyObject *self, PyObject *args);

static PyObject* py_occaGetCompiler(PyObject *self, PyObject *args);
static PyObject* py_occaGetCompilerEnvScript(PyObject *self, PyObject *args);
static PyObject* py_occaGetCompilerFlags(PyObject *self, PyObject *args);

static PyObject* py_occaFlush(PyObject *self, PyObject *args);
static PyObject* py_occaFinish(PyObject *self, PyObject *args);

// static PyObject* py_occaWaitFor(PyObject *self, PyObject *args);

static PyObject* py_occaCreateStream(PyObject *self, PyObject *args);
static PyObject* py_occaGetStream(PyObject *self, PyObject *args);
static PyObject* py_occaSetStream(PyObject *self, PyObject *args);
static PyObject* py_occaWrapStream(PyObject *self, PyObject *args);

// static PyObject* py_occaTagStream(PyObject *self, PyObject *args);
//  |===================================

//  |---[ Kernel ]----------------------
static PyObject* py_occaBuildKernel(PyObject *self, PyObject *args); // x
static PyObject* py_occaBuildKernelFromSource(PyObject *self, PyObject *args);
static PyObject* py_occaBuildKernelFromString(PyObject *self, PyObject *args);
static PyObject* py_occaBuildKernelFromBinary(PyObject *self, PyObject *args);
static PyObject* py_occaBuildKernelFromLoopy(PyObject *self, PyObject *args);
static PyObject* py_occaBuildKernelFromFloopy(PyObject *self, PyObject *args);
//  |===================================

//  |---[ Memory ]----------------------
static PyObject* py_occaWrapMemory(PyObject *self, PyObject *args);
static PyObject* py_occaWrapManagedMemory(PyObject *self, PyObject *args);

static PyObject* py_occaMalloc(PyObject *self, PyObject *args);
static PyObject* py_occaManagedAlloc(PyObject *self, PyObject *args); // x

static PyObject* py_occaUvaAlloc(PyObject *self, PyObject *args);
static PyObject* py_occaManagedUvaAlloc(PyObject *self, PyObject *args);

static PyObject* py_occaMappedAlloc(PyObject *self, PyObject *args);
static PyObject* py_occaManagedMappedAlloc(PyObject *self, PyObject *args);
//  |===================================
//======================================

//---[ Device ]-------------------------
static PyObject* py_occaPrintAvailableDevices(PyObject *self, PyObject *args);

static PyObject* py_occaCreateDeviceInfo(PyObject *self, PyObject *args);
static PyObject* py_occaDeviceInfoAppend(PyObject *self, PyObject *args);
static PyObject* py_occaDeviceInfoAppendType(PyObject *self, PyObject *args);
static PyObject* py_occaDeviceInfoFree(PyObject *self, PyObject *args);

static PyObject* py_occaCreateDevice(PyObject *self, PyObject *args);
static PyObject* py_occaCreateDeviceFromInfo(PyObject *self, PyObject *args);

static PyObject* py_occaDeviceMode(PyObject *self, PyObject *args);
static PyObject* py_occaDeviceSetCompiler(PyObject *self, PyObject *args);
static PyObject* py_occaDeviceSetCompilerFlags(PyObject *self, PyObject *args);

static PyObject* py_occaDeviceBytesAllocated(PyObject *self, PyObject *args);

static PyObject* py_occaDeviceBuildKernel(PyObject *self, PyObject *args);
static PyObject* py_occaDeviceBuildKernelFromSource(PyObject *self, PyObject *args);
static PyObject* py_occaDeviceBuildKernelFromString(PyObject *self, PyObject *args);
static PyObject* py_occaDeviceBuildKernelFromBinary(PyObject *self, PyObject *args);
static PyObject* py_occaDeviceBuildKernelFromLoopy(PyObject *self, PyObject *args);
static PyObject* py_occaDeviceBuildKernelFromFloopy(PyObject *self, PyObject *args);

static PyObject* py_occaDeviceMalloc(PyObject *self, PyObject *args);
static PyObject* py_occaDeviceManagedAlloc(PyObject *self, PyObject *args);

static PyObject* py_occaDeviceUvaAlloc(PyObject *self, PyObject *args);
static PyObject* py_occaDeviceManagedUvaAlloc(PyObject *self, PyObject *args);

static PyObject* py_occaDeviceMappedAlloc(PyObject *self, PyObject *args);
static PyObject* py_occaDeviceManagedMappedAlloc(PyObject *self, PyObject *args);

static PyObject* py_occaDeviceFlush(PyObject *self, PyObject *args);
static PyObject* py_occaDeviceFinish(PyObject *self, PyObject *args);

static PyObject* py_occaDeviceCreateStream(PyObject *self, PyObject *args);
static PyObject* py_occaDeviceGetStream(PyObject *self, PyObject *args);
static PyObject* py_occaDeviceSetStream(PyObject *self, PyObject *args);

// static PyObject* py_occaDeviceTagStream(PyObject *self, PyObject *args);
// static PyObject* py_occaDeviceWaitForTag(PyObject *self, PyObject *args);
// static PyObject* py_occaDeviceTimeBetweenTags(PyObject *self, PyObject *args);

static PyObject* py_occaDeviceStreamFree(PyObject *self, PyObject *args);
static PyObject* py_occaDeviceFree(PyObject *self, PyObject *args); // x
//======================================

//---[ Kernel ]-------------------------
static PyObject* py_occaKernelMode(PyObject *self, PyObject *args);
static PyObject* py_occaKernelName(PyObject *self, PyObject *args);

static PyObject* py_occaKernelGetDevice(PyObject *self, PyObject *args);

static PyObject* py_occaCreateArgumentList(PyObject *self, PyObject *args);
static PyObject* py_occaArgumentListClear(PyObject *self, PyObject *args);
static PyObject* py_occaArgumentListFree(PyObject *self, PyObject *args);
static PyObject* py_occaArgumentListAddArg(PyObject *self, PyObject *args);

static PyObject* py_occaKernelRun_(PyObject *self, PyObject *args);

static PyObject* py_occaCreateKernelInfo(PyObject *self, PyObject *args);
static PyObject* py_occaKernelInfoAddDefine(PyObject *self, PyObject *args);
static PyObject* py_occaKernelInfoAddInclude(PyObject *self, PyObject *args);

static PyObject* py_occaKernelFree(PyObject *self, PyObject *args); // x
//======================================

//---[ Memory ]-------------------------
static PyObject* py_occaMemoryMode(PyObject *self, PyObject *args);

static PyObject* py_occaMemoryGetMemoryHandle(PyObject *self, PyObject *args);
static PyObject* py_occaMemoryGetMappedPointer(PyObject *self, PyObject *args);
static PyObject* py_occaMemoryGetTextureHandle(PyObject *self, PyObject *args);

static PyObject* py_occaMemcpy(PyObject *self, PyObject *args);

static PyObject* py_occaCopyMemToMem(PyObject *self, PyObject *args);
static PyObject* py_occaCopyPtrToMem(PyObject *self, PyObject *args);
static PyObject* py_occaCopyMemToPtr(PyObject *self, PyObject *args);

static PyObject* py_occaAsyncCopyMemToMem(PyObject *self, PyObject *args);
static PyObject* py_occaAsyncCopyPtrToMem(PyObject *self, PyObject *args);
static PyObject* py_occaAsyncCopyMemToPtr(PyObject *self, PyObject *args);

static PyObject* py_occaMemoryFree(PyObject *self, PyObject *args); // x
//======================================

// Init stuff

static PyMethodDef _C_occaMethods[] = {
  //---[ Globals & Flags ]--------------
  {"setVerboseCompilation", py_occaSetVerboseCompilation, METH_VARARGS},
  //====================================

  //----[ Background Device ]-----------
  //  |---[ Device ]--------------------
  {"setDevice"        , py_occaSetDevice        , METH_VARARGS},
  {"setDeviceFromInfo", py_occaSetDeviceFromInfo, METH_VARARGS},

  {"getCurrentDevice", py_occaGetCurrentDevice, METH_VARARGS},

  {"setCompiler"         , py_occaSetCompiler         , METH_VARARGS},
  {"setCompilerEnvScript", py_occaSetCompilerEnvScript, METH_VARARGS},
  {"setCompilerFlags"    , py_occaSetCompilerFlags    , METH_VARARGS},

  {"getCompiler"         , py_occaGetCompiler         , METH_VARARGS},
  {"getCompilerEnvScript", py_occaGetCompilerEnvScript, METH_VARARGS},
  {"getCompilerFlags"    , py_occaGetCompilerFlags    , METH_VARARGS},

  {"flush" , py_occaFlush , METH_VARARGS},
  {"finish", py_occaFinish, METH_VARARGS},

  // {"waitFor", py_occaWaitFor, METH_VARARGS},

  {"createStream", py_occaCreateStream, METH_VARARGS},
  {"getStream"   , py_occaGetStream   , METH_VARARGS},
  {"setStream"   , py_occaSetStream   , METH_VARARGS},
  {"wrapStream"  , py_occaWrapStream  , METH_VARARGS},

  // {"tagStream", py_occaTagStream, METH_VARARGS},
  //  |=================================

  //  |---[ Kernel ]--------------------
  {"buildKernel"          , py_occaBuildKernel          , METH_VARARGS},
  {"buildKernelFromSource", py_occaBuildKernelFromSource, METH_VARARGS},
  {"buildKernelFromString", py_occaBuildKernelFromString, METH_VARARGS},
  {"buildKernelFromBinary", py_occaBuildKernelFromBinary, METH_VARARGS},
  {"buildKernelFromLoopy" , py_occaBuildKernelFromLoopy , METH_VARARGS},
  {"buildKernelFromFloopy", py_occaBuildKernelFromFloopy, METH_VARARGS},
  //  |=================================

  //  |---[ Memory ]--------------------
  {"wrapMemory"       , py_occaWrapMemory       , METH_VARARGS},
  {"wrapManagedMemory", py_occaWrapManagedMemory, METH_VARARGS},

  {"malloc"      , py_occaMalloc      , METH_VARARGS},
  {"managedAlloc", py_occaManagedAlloc, METH_VARARGS},

  {"uvaAlloc"       , py_occaUvaAlloc       , METH_VARARGS},
  {"managedUvaAlloc", py_occaManagedUvaAlloc, METH_VARARGS},

  {"mappedAlloc"       , py_occaMappedAlloc       , METH_VARARGS},
  {"managedMappedAlloc", py_occaManagedMappedAlloc, METH_VARARGS},
  //  |=================================
  //====================================

  //---[ Device ]-----------------------
  {"printAvailableDevices", py_occaPrintAvailableDevices, METH_VARARGS},

  {"createDeviceInfo"    , py_occaCreateDeviceInfo    , METH_VARARGS},
  {"deviceInfoAppend"    , py_occaDeviceInfoAppend    , METH_VARARGS},
  {"deviceInfoAppendType", py_occaDeviceInfoAppendType, METH_VARARGS},
  {"deviceInfoFree"      , py_occaDeviceInfoFree      , METH_VARARGS},

  {"createDevice"        , py_occaCreateDevice        , METH_VARARGS},
  {"createDeviceFromInfo", py_occaCreateDeviceFromInfo, METH_VARARGS},

  {"deviceMode"            , py_occaDeviceMode            , METH_VARARGS},
  {"deviceSetCompiler"     , py_occaDeviceSetCompiler     , METH_VARARGS},
  {"deviceSetCompilerFlags", py_occaDeviceSetCompilerFlags, METH_VARARGS},

  {"deviceBytesAllocated", py_occaDeviceBytesAllocated, METH_VARARGS},

  {"deviceBuildKernel"          , py_occaDeviceBuildKernel          , METH_VARARGS},
  {"deviceBuildKernelFromSource", py_occaDeviceBuildKernelFromSource, METH_VARARGS},
  {"deviceBuildKernelFromString", py_occaDeviceBuildKernelFromString, METH_VARARGS},
  {"deviceBuildKernelFromBinary", py_occaDeviceBuildKernelFromBinary, METH_VARARGS},
  {"deviceBuildKernelFromLoopy" , py_occaDeviceBuildKernelFromLoopy , METH_VARARGS},
  {"deviceBuildKernelFromFloopy", py_occaDeviceBuildKernelFromFloopy, METH_VARARGS},

  {"deviceMalloc"      , py_occaDeviceMalloc      , METH_VARARGS},
  {"deviceManagedAlloc", py_occaDeviceManagedAlloc, METH_VARARGS},

  {"deviceUvaAlloc"       , py_occaDeviceUvaAlloc       , METH_VARARGS},
  {"deviceManagedUvaAlloc", py_occaDeviceManagedUvaAlloc, METH_VARARGS},

  {"deviceMappedAlloc"       , py_occaDeviceMappedAlloc       , METH_VARARGS},
  {"deviceManagedMappedAlloc", py_occaDeviceManagedMappedAlloc, METH_VARARGS},

  {"deviceFlush" , py_occaDeviceFlush , METH_VARARGS},
  {"deviceFinish", py_occaDeviceFinish, METH_VARARGS},

  {"deviceCreateStream", py_occaDeviceCreateStream, METH_VARARGS},
  {"deviceGetStream"   , py_occaDeviceGetStream   , METH_VARARGS},
  {"deviceSetStream"   , py_occaDeviceSetStream   , METH_VARARGS},

  // {"deviceTagStream", py_occaDeviceTagStream, METH_VARARGS},
  // {"deviceWaitForTag", py_occaDeviceWaitForTag, METH_VARARGS},
  // {"deviceTimeBetweenTags", py_occaDeviceTimeBetweenTags, METH_VARARGS},

  {"deviceStreamFree", py_occaDeviceStreamFree, METH_VARARGS},
  {"deviceFree"      , py_occaDeviceFree      , METH_VARARGS},
  //====================================

  //---[ Kernel ]-----------------------
  {"kernelMode", py_occaKernelMode, METH_VARARGS},
  {"kernelName", py_occaKernelName, METH_VARARGS},

  {"kernelGetDevice", py_occaKernelGetDevice, METH_VARARGS},

  {"createArgumentList", py_occaCreateArgumentList, METH_VARARGS},
  {"argumentListClear" , py_occaArgumentListClear , METH_VARARGS},
  {"argumentListFree"  , py_occaArgumentListFree  , METH_VARARGS},
  {"argumentListAddArg", py_occaArgumentListAddArg, METH_VARARGS},

  {"kernelRun_", py_occaKernelRun_, METH_VARARGS},

  {"createKernelInfo"    , py_occaCreateKernelInfo    , METH_VARARGS},
  {"kernelInfoAddDefine" , py_occaKernelInfoAddDefine , METH_VARARGS},
  {"kernelInfoAddInclude", py_occaKernelInfoAddInclude, METH_VARARGS},

  {"kernelFree", py_occaKernelFree, METH_VARARGS},
  //====================================

  //---[ Memory ]-----------------------
  {"memoryMode", py_occaMemoryMode, METH_VARARGS},

  {"memoryGetMemoryHandle" , py_occaMemoryGetMemoryHandle , METH_VARARGS},
  {"memoryGetMappedPointer", py_occaMemoryGetMappedPointer, METH_VARARGS},
  {"memoryGetTextureHandle", py_occaMemoryGetTextureHandle, METH_VARARGS},

  {"memcpy", py_occaMemcpy, METH_VARARGS},

  {"copyMemToMem", py_occaCopyMemToMem, METH_VARARGS},
  {"copyPtrToMem", py_occaCopyPtrToMem, METH_VARARGS},
  {"copyMemToPtr", py_occaCopyMemToPtr, METH_VARARGS},

  {"asyncCopyMemToMem", py_occaAsyncCopyMemToMem, METH_VARARGS},
  {"asyncCopyPtrToMem", py_occaAsyncCopyPtrToMem, METH_VARARGS},
  {"asyncCopyMemToPtr", py_occaAsyncCopyMemToPtr, METH_VARARGS},

  {"memoryFree", py_occaMemoryFree, METH_VARARGS}
  //====================================
};

void init_C_occa(){
  (void) Py_InitModule("_C_occa", _C_occaMethods);
  import_array();
}