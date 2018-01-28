//===- Application.cpp ----------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/Core/Application.h>
#include <onnc/Option/CommandLine.h>
#include <onnc/Support/ManagedStatic.h>

using namespace onnc;

//===----------------------------------------------------------------------===//
// CoreApplication
//===----------------------------------------------------------------------===//
CoreApplication::CoreApplication(int pArgc, char* pArgv[])
{
  cl::ParseCommandLine(pArgc, pArgv);
}

CoreApplication::~CoreApplication()
{
  shutdown();
}
